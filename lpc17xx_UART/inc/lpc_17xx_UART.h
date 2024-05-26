#ifndef LPC_17XX_UART_H_
#define LPC_17XX_UART_H_


#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"

typedef struct
{
	unsigned int baudrate;
	UART_PARITY_Type paridad;
	UART_DATABIT_Type dataWidth;
	UART_STOPBIT_Type stopBits;
	unsigned int DMAEnable;
	UART_FITO_LEVEL_Type FIFOTrigger;
}ConfigUART;


void UARTInit (LPC_UART_TypeDef* UARTn, ConfigUART configuracion);
unsigned int UARTReceivedData (LPC_UART_TypeDef* UARTn);
unsigned int UARTIsTransmitting (LPC_UART_TypeDef* UARTn);
void UARTSendByte (LPC_UART_TypeDef* UARTn, char data);
void UARTSend (LPC_UART_TypeDef* UARTn, unsigned char* txbuf, unsigned int buflen, TRANSFER_BLOCK_Type flag);
char UARTReceiveByte (LPC_UART_TypeDef* UARTn);
void UARTReceive (LPC_UART_TypeDef* UARTn, unsigned char* rxbuf, unsigned int buflen, TRANSFER_BLOCK_Type flag);
void printLineUART (LPC_UART_TypeDef * UARTn, unsigned char * buffer);

#endif
