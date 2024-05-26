#include "driverUART.h"
#include "LPC17xx.h"
#include "lpc_17xx_UART.h"
#include "fsmUART.h"


fsmUART fsmUART3;

unsigned char bufferTx[100], bufferRx[100];		// Usados por el driver de la UART

void createDriverUART (void)
{
	ConfigUART configuracionUART3;


	configuracionUART3.baudrate = 256000;
	configuracionUART3.paridad = UART_PARITY_NONE;
	configuracionUART3.dataWidth = UART_DATABIT_8;
	configuracionUART3.stopBits = UART_STOPBIT_1;
	configuracionUART3.DMAEnable = 0;
	configuracionUART3.FIFOTrigger = UART_FIFO_TRGLEV0;
	UARTInit (LPC_UART3, configuracionUART3);


	fsmUARTInit(&fsmUART3, LPC_UART3, bufferTx, 100, bufferRx, 100);

	xTaskCreate(driverUART, (signed portCHAR*) "driver UART", 100, (void*)&fsmUART3, tskIDLE_PRIORITY+1, NULL);
}

void driverUART (void* uart)
{
	fsmUART* periphUART = (fsmUART*) uart;

	while (1)
	{
		fsmUARTHandler(periphUART);
		vTaskDelay(1/portTICK_RATE_MS);
	}
}
