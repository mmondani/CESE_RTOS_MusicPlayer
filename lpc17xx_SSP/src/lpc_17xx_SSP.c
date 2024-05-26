#include "lpc_17xx_SSP.h"

void SSPInit (LPC_SSP_TypeDef* SSPn, ConfigSSP configuracion)
{
	SSP_CFG_Type SSP_Config;
	PINSEL_CFG_Type	pinConfiguracion;

	pinConfiguracion.Funcnum = 2;
	pinConfiguracion.OpenDrain = 0;
	pinConfiguracion.Pinmode = 0;
	pinConfiguracion.Portnum = 0;

	if (SSPn == LPC_SSP0)
	{
		pinConfiguracion.Pinnum = 15;		// SKC0
		PINSEL_ConfigPin(&pinConfiguracion);
		pinConfiguracion.Pinnum = 17;		// MISO0
		PINSEL_ConfigPin(&pinConfiguracion);
		pinConfiguracion.Pinnum = 18;		// MOSI0
		PINSEL_ConfigPin(&pinConfiguracion);
	}
	if (SSPn == LPC_SSP1)
	{
		pinConfiguracion.Pinnum = 7;		// SCK1
		PINSEL_ConfigPin(&pinConfiguracion);
		pinConfiguracion.Pinnum = 8;		// MISO1
		PINSEL_ConfigPin(&pinConfiguracion);
		pinConfiguracion.Pinnum = 9;		// MOSI1
		PINSEL_ConfigPin(&pinConfiguracion);
	}


	switch (configuracion.modo)
	{
		case MODE0_0:
			SSP_Config.CPHA = SSP_CPHA_FIRST;
			SSP_Config.CPOL = SSP_CPOL_LO;
			break;
		case MODE0_1:
			SSP_Config.CPHA = SSP_CPHA_FIRST;
			SSP_Config.CPOL = SSP_CPOL_HI;
			break;
		case MODE1_0:
			SSP_Config.CPHA = SSP_CPHA_SECOND;
			SSP_Config.CPOL = SSP_CPOL_LO;
			break;
		case MODE1_1:
			SSP_Config.CPHA = SSP_CPHA_SECOND;
			SSP_Config.CPOL = SSP_CPOL_HI;
			break;
	}

	switch (configuracion.databit)
	{
		case 4:
			SSP_Config.Databit = SSP_DATABIT_4;
			break;
		case 5:
			SSP_Config.Databit = SSP_DATABIT_5;
			break;
		case 6:
			SSP_Config.Databit = SSP_DATABIT_6;
			break;
		case 7:
			SSP_Config.Databit = SSP_DATABIT_7;
			break;
		case 8:
			SSP_Config.Databit = SSP_DATABIT_8;
			break;
		case 9:
			SSP_Config.Databit = SSP_DATABIT_9;
			break;
		case 10:
			SSP_Config.Databit = SSP_DATABIT_10;
			break;
		case 11:
			SSP_Config.Databit = SSP_DATABIT_11;
			break;
		case 12:
			SSP_Config.Databit = SSP_DATABIT_12;
			break;
		case 13:
			SSP_Config.Databit = SSP_DATABIT_13;
			break;
		case 14:
			SSP_Config.Databit = SSP_DATABIT_14;
			break;
		case 15:
			SSP_Config.Databit = SSP_DATABIT_15;
			break;
		case 16:
			SSP_Config.Databit = SSP_DATABIT_16;
			break;
		default:
			SSP_Config.Databit = SSP_DATABIT_8;
			break;
	}

	switch (configuracion.rol)
	{
		case SSP_MASTER:
			SSP_Config.Mode = SSP_MASTER_MODE;
			break;
		case SSP_SLAVE:
			SSP_Config.Mode = SSP_SLAVE_MODE;
			break;
	}

	switch (configuracion.frameFormat)
	{
		case SSP_SPI:
			SSP_Config.FrameFormat = SSP_FRAME_SPI;
			break;
		case SSP_TI:
			SSP_Config.FrameFormat = SSP_FRAME_TI;
			break;
		case SSP_MICROWIRE:
			SSP_Config.FrameFormat = SSP_FRAME_MICROWIRE;
			break;
	}

	SSP_Config.ClockRate = configuracion.clock;

	SSP_Init (SSPn, &SSP_Config);
	SSP_Cmd(SSPn, ENABLE);
}

unsigned int SSPisTxFIFOEmpty (LPC_SSP_TypeDef* SSPn)
{
	return SSP_GetStatus (SSPn, SSP_STAT_TXFIFO_EMPTY);
}

unsigned int SSPisTxFIFONotFull (LPC_SSP_TypeDef* SSPn)
{
	return SSP_GetStatus (SSPn, SSP_STAT_TXFIFO_NOTFULL);
}

unsigned int SSPisRxFIFONotEmpty (LPC_SSP_TypeDef* SSPn)
{
	return SSP_GetStatus (SSPn, SSP_STAT_RXFIFO_NOTEMPTY);
}

unsigned int SSPisRxFIFOFull (LPC_SSP_TypeDef* SSPn)
{
	return SSP_GetStatus (SSPn, SSP_STAT_RXFIFO_FULL);
}

unsigned int SSPisBusy (LPC_SSP_TypeDef* SSPn)
{
	return SSP_GetStatus (SSPn, SSP_STAT_BUSY);
}

void SSPSendByte (LPC_SSP_TypeDef* SSPn, unsigned char dato, unsigned int poll)
{
	SSP_SendData (SSPn, dato);
	if (poll)
	{
		while (SSPisBusy(SSPn));
	}
}

unsigned char SSPRecieveByte (LPC_SSP_TypeDef* SSPn)
{
	return SSP_ReceiveData (SSPn);
}

unsigned char SSPSendRecieveByte (LPC_SSP_TypeDef* SSPn, unsigned char dato)
{
	SSP_SendData (SSPn, dato);

	while (SSPisBusy(SSPn));

	return SSP_ReceiveData (SSPn);
}

void SSPSendBuffer (LPC_SSP_TypeDef * SSPn, const char *buff, unsigned int count)
{
	unsigned int i;

	for (i = 0; i < count; i++)
	{
		SSPSendByte (SSPn, *(buff+i), 1);
	}
}

void SSPReceiveBuffer (LPC_SSP_TypeDef * SSPn, unsigned char *buff, unsigned int count)
{
	unsigned int i;

	for (i = 0; i < count; i++)
	{
		*(buff+i) = SSPSendRecieveByte (SSPn, 0xFF);
	}
}
