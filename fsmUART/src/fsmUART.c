#include "fsmUART.h"

//*************************************************************
// Funciones internas

void fsmUART_s_Idle (fsmUART* fsmStruct);
void fsmUART_s_Tx (fsmUART* fsmStruct);
void fsmUART_s_Rx (fsmUART* fsmStruct);
unsigned int fsmUARTGetTxWaitingBytes (fsmUART* fsmStruct);
unsigned int fsmUARTGetRxFreeSpace (fsmUART* fsmStruct);
//*************************************************************



void fsmUARTInit (fsmUART* fsmStruct, LPC_UART_TypeDef* UARTn, unsigned char* buffTx, unsigned int lenTx, unsigned char* buffRx, unsigned int lenRx)
{
	fsmStruct->UARTn = UARTn;
	fsmStruct->bufferTx = buffTx;
	fsmStruct->lenBufferTx = lenTx;
	fsmStruct->ptrTxIn = 0;
	fsmStruct->ptrTxOut = 0;
	fsmStruct->bufferRx = buffRx;
	fsmStruct->lenBufferRx = lenRx;
	fsmStruct->ptrRxIn = 0;
	fsmStruct->ptrRxOut = 0;
	fsmStruct->estadoFSM = fsmUART_s_Idle;
}

void fsmUARTHandler (fsmUART* fsmStruct)
{
	(*fsmStruct->estadoFSM)(fsmStruct);
}

void fsmUART_s_Idle (fsmUART* fsmStruct)
{
	if (fsmUARTGetTxWaitingBytes(fsmStruct))
	{
		fsmStruct->estadoFSM = fsmUART_s_Tx;
	}
	else if (UARTReceivedData(fsmStruct->UARTn))
	{
		fsmStruct->estadoFSM = fsmUART_s_Rx;
	}
}

void fsmUART_s_Tx (fsmUART* fsmStruct)
{
	unsigned int bytesALeer;

	bytesALeer = fsmUARTGetTxWaitingBytes(fsmStruct);

	if (bytesALeer != 0)
	{
		if (bytesALeer > 60)
			bytesALeer = 60;

		while (bytesALeer)
		{
			while (UARTIsTransmitting(fsmStruct->UARTn));
			UARTSendByte(fsmStruct->UARTn, fsmStruct->bufferTx[fsmStruct->ptrTxOut]);
			fsmStruct->ptrTxOut ++;
			if (fsmStruct->ptrTxOut >= fsmStruct->lenBufferTx)
				fsmStruct->ptrTxOut = 0;
			bytesALeer --;
		}
	}

	if (UARTReceivedData(fsmStruct->UARTn))
	{
		fsmStruct->estadoFSM = fsmUART_s_Rx;
	}
	else
	{
		if (!fsmUARTGetTxWaitingBytes(fsmStruct))
		{
			fsmStruct->estadoFSM = fsmUART_s_Idle;
		}
	}
}

void fsmUART_s_Rx (fsmUART* fsmStruct)
{
	unsigned char bytesRecibidos = 0;

	while (bytesRecibidos <= 20 && UARTReceivedData(fsmStruct->UARTn) && fsmUARTGetRxFreeSpace (fsmStruct))
	{
		fsmStruct->bufferRx[fsmStruct->ptrRxIn] = UARTReceiveByte(fsmStruct->UARTn);
		bytesRecibidos ++;
		fsmStruct->ptrRxIn ++;
		if (fsmStruct->ptrRxIn >= fsmStruct->lenBufferRx)
			fsmStruct->ptrRxIn = 0;
	}

	if (fsmUARTGetTxWaitingBytes(fsmStruct))
	{
		fsmStruct->estadoFSM = fsmUART_s_Tx;
	}
	else
	{
		if (!UARTReceivedData(fsmStruct->UARTn))
		{
			fsmStruct->estadoFSM = fsmUART_s_Idle;
		}
	}
}



unsigned int fsmUARTGetTxWaitingBytes (fsmUART* fsmStruct)
{
	if (fsmStruct->ptrTxOut <= fsmStruct->ptrTxIn)
	{
		return (fsmStruct->ptrTxIn - fsmStruct->ptrTxOut);
	}
	else
	{
		return (fsmStruct->lenBufferTx - (fsmStruct->ptrTxOut - fsmStruct->ptrTxIn));
	}
}

unsigned int fsmUARTGetRxFreeSpace (fsmUART* fsmStruct)
{
	if (fsmStruct->ptrRxOut <= fsmStruct->ptrRxIn)
	{
		return (fsmStruct->lenBufferRx - (fsmStruct->ptrRxIn - fsmStruct->ptrRxOut));
	}
	else
	{
		return (fsmStruct->ptrRxOut - fsmStruct->ptrRxIn);
	}
}

unsigned int fsmUARTGetTxFreeSpace (fsmUART* fsmStruct)
{
	if (fsmStruct->ptrTxOut <= fsmStruct->ptrTxIn)
	{
		return (fsmStruct->lenBufferTx - (fsmStruct->ptrTxIn - fsmStruct->ptrTxOut));
	}
	else
	{
		return (fsmStruct->ptrTxOut - fsmStruct->ptrTxIn);
	}
}

unsigned int fsmUARTGetRxWaitingBytes (fsmUART* fsmStruct)
{
	if (fsmStruct->ptrRxOut <= fsmStruct->ptrRxIn)
	{
		return (fsmStruct->ptrRxIn - fsmStruct->ptrRxOut);
	}
	else
	{
		return (fsmStruct->lenBufferRx - (fsmStruct->ptrRxOut - fsmStruct->ptrRxIn));
	}
}

unsigned int fsmUARTGetRxData (fsmUART* fsmStruct, unsigned char* datos, unsigned int len)
{
	unsigned int bytesALeer, bytesLeidos;
	unsigned int i = 0;

	bytesALeer = fsmUARTGetRxWaitingBytes(fsmStruct);

	if (len != 0)
	{
		if (bytesALeer > len)
			bytesALeer = len;
	}

	bytesLeidos = bytesALeer;

	while (bytesALeer)
	{
		datos[i] = fsmStruct->bufferRx[fsmStruct->ptrRxOut];
		fsmStruct->ptrRxOut ++;
		if (fsmStruct->ptrRxOut >= fsmStruct->lenBufferRx)
			fsmStruct->ptrRxOut = 0;
		bytesALeer --;
		i ++;
	}

	return bytesLeidos;
}

unsigned int fsmUARTPutTxData (fsmUART* fsmStruct, unsigned char* datos, unsigned int len)
{
	unsigned int bytesDisponibles, bytesEscritos;
	unsigned int i = 0;

	bytesDisponibles = fsmUARTGetTxFreeSpace(fsmStruct);

	if (len != 0)
	{
		if (bytesDisponibles > len)
			bytesDisponibles = len;
	}

	bytesEscritos = bytesDisponibles;

	while (bytesDisponibles)
	{
		fsmStruct->bufferTx[fsmStruct->ptrTxIn] = datos[i];
		fsmStruct->ptrTxIn ++;
		if (fsmStruct->ptrTxIn >= fsmStruct->lenBufferTx)
			fsmStruct->ptrTxIn = 0;
		bytesDisponibles --;
		i ++;
	}

	return bytesEscritos;
}
