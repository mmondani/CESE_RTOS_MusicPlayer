#ifndef FSMUART_H_
#define FSMUART_H_

#include "lpc_17xx_UART.h"
#include "LPC17xx.h"

typedef struct
{
	LPC_UART_TypeDef* UARTn;
	unsigned char* bufferTx;
	unsigned int lenBufferTx;
	unsigned int ptrTxIn;
	unsigned int ptrTxOut;
	unsigned char* bufferRx;
	unsigned int lenBufferRx;
	unsigned int ptrRxIn;
	unsigned int ptrRxOut;
	void (*estadoFSM) ();
}fsmUART;

//*************************************************************
// Funciones públicas

void fsmUARTInit (fsmUART* fsmStruct, LPC_UART_TypeDef* UARTn, unsigned char* buffTx, unsigned int lenTx, unsigned char* buffRx, unsigned int lenRx);
void fsmUARTHandler (fsmUART* fsmStruct);
unsigned int fsmUARTGetTxFreeSpace (fsmUART* fsmStruct);
unsigned int fsmUARTGetRxWaitingBytes (fsmUART* fsmStruct);
unsigned int fsmUARTGetRxData (fsmUART* fsmStruct, unsigned char* datos, unsigned int len);
unsigned int fsmUARTPutTxData (fsmUART* fsmStruct, unsigned char* datos, unsigned int len);
//*************************************************************




#endif
