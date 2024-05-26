#include "lpc_17xx_UART.h"

void UARTInit (LPC_UART_TypeDef* UARTn, ConfigUART configuracion)
{
	UART_CFG_Type		UARTConfiguracion;
	UART_FIFO_CFG_Type	UARTFIFOConfiguracion;
	PINSEL_CFG_Type		PinConfiguracion;

	if (UARTn == LPC_UART0)
	{
		PinConfiguracion.Funcnum = 1;
		PinConfiguracion.OpenDrain = 0;
		PinConfiguracion.Pinmode = 0;
		PinConfiguracion.Pinnum = 2;
		PinConfiguracion.Portnum = 0;
		PINSEL_ConfigPin(&PinConfiguracion);
		PinConfiguracion.Pinnum = 3;
		PINSEL_ConfigPin(&PinConfiguracion);
	}
	else if (UARTn == LPC_UART1)
	{
		PinConfiguracion.Funcnum = 1;
		PinConfiguracion.OpenDrain = 0;
		PinConfiguracion.Pinmode = 0;
		PinConfiguracion.Pinnum = 15;
		PinConfiguracion.Portnum = 0;
		PINSEL_ConfigPin(&PinConfiguracion);
		PinConfiguracion.Pinnum = 16;
		PINSEL_ConfigPin(&PinConfiguracion);
	}
	else if (UARTn == LPC_UART2)
	{
		PinConfiguracion.Funcnum = 1;
		PinConfiguracion.OpenDrain = 0;
		PinConfiguracion.Pinmode = 0;
		PinConfiguracion.Pinnum = 10;
		PinConfiguracion.Portnum = 0;
		PINSEL_ConfigPin(&PinConfiguracion);
		PinConfiguracion.Pinnum = 11;
		PINSEL_ConfigPin(&PinConfiguracion);
	}
	else if (UARTn == LPC_UART3)
	{
		PinConfiguracion.Funcnum = 2;
		PinConfiguracion.OpenDrain = 0;
		PinConfiguracion.Pinmode = 0;
		PinConfiguracion.Pinnum = 0;
		PinConfiguracion.Portnum = 0;
		PINSEL_ConfigPin(&PinConfiguracion);
		PinConfiguracion.Pinnum = 1;
		PINSEL_ConfigPin(&PinConfiguracion);
	}

	UARTConfiguracion.Baud_rate = configuracion.baudrate;
	UARTConfiguracion.Databits = configuracion.dataWidth;
	UARTConfiguracion.Parity = configuracion.paridad;
	UARTConfiguracion.Stopbits = configuracion.stopBits;
	UART_Init(UARTn, &UARTConfiguracion);


	if (configuracion.DMAEnable == 0)
		UARTFIFOConfiguracion.FIFO_DMAMode = DISABLE;
	else
		UARTFIFOConfiguracion.FIFO_DMAMode = ENABLE;

	UARTFIFOConfiguracion.FIFO_Level = configuracion.FIFOTrigger;
	UARTFIFOConfiguracion.FIFO_ResetRxBuf = ENABLE;
	UARTFIFOConfiguracion.FIFO_ResetTxBuf = ENABLE;
	UART_FIFOConfig(UARTn, &UARTFIFOConfiguracion);

	UART_TxCmd(UARTn, ENABLE);
}


unsigned int UARTReceivedData (LPC_UART_TypeDef* UARTn)
{
	if (UARTn->LSR & UART_LSR_RDR){
		return 1;
	} else {
		return 0;
	}
}

unsigned int UARTIsTransmitting (LPC_UART_TypeDef* UARTn)
{
	return (UART_CheckBusy(UARTn));
}

void UARTSendByte (LPC_UART_TypeDef* UARTn, char data)
{
	UART_SendByte(UARTn, data);
}

void UARTSend (LPC_UART_TypeDef* UARTn, unsigned char* txbuf, unsigned int buflen, TRANSFER_BLOCK_Type flag)
{
	UART_Send(UARTn, txbuf, buflen, flag);
}

char UARTReceiveByte (LPC_UART_TypeDef* UARTn)
{
	return (UART_ReceiveByte(UARTn));
}

void UARTReceive (LPC_UART_TypeDef* UARTn, unsigned char* rxbuf, unsigned int buflen, TRANSFER_BLOCK_Type flag)
{
	UART_Receive(UARTn, rxbuf, buflen, flag);
}

void printLineUART (LPC_UART_TypeDef * UARTn, unsigned char * buffer)
{
	unsigned int i;

	i = 0;

	while (*(buffer + i) != '\0')
	{
		UARTSendByte (UARTn, *(buffer + i));
		while (UARTIsTransmitting (UARTn));
		i++;
	}
}
