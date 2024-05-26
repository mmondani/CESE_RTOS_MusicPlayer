#ifndef LPC_17XX_SSP_H_
#define LPC_17XX_SSP_H_


#include "LPC17xx.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_pinsel.h"

/*Modos SPI */
//-----------------------
//| MODE | CPHA | CPOL |
//| 0,0  |  0   |   0  |
//| 0,1  |  1   |   0  |
//| 1,0  |  0   |   1  |
//| 1,1  |  1   |   1  |
typedef enum {MODE0_0, MODE0_1, MODE1_0, MODE1_1}SPIModes;
typedef enum {SSP_MASTER, SSP_SLAVE}SPIMaster_slave;
typedef enum {SSP_SPI, SSP_TI, SSP_MICROWIRE}SSPFrameFormat;

typedef struct
{
	unsigned int databit;		// 4 .. 16
	SPIModes modo;
	SPIMaster_slave rol;
	SSPFrameFormat frameFormat;
	unsigned int clock;
}ConfigSSP;

void SSPInit (LPC_SSP_TypeDef* SSPn, ConfigSSP configuracion);
unsigned int SSPisTxFIFOEmpty (LPC_SSP_TypeDef* SSPn);
unsigned int SSPisTxFIFONotFull (LPC_SSP_TypeDef* SSPn);
unsigned int SSPisRxFIFONotEmpty (LPC_SSP_TypeDef* SSPn);
unsigned int SSPisRxFIFOFull (LPC_SSP_TypeDef* SSPn);
unsigned int SSPisBusy (LPC_SSP_TypeDef* SSPn);
void SSPSendByte (LPC_SSP_TypeDef* SSPn, unsigned char dato, unsigned int poll);
unsigned char SSPRecieveByte (LPC_SSP_TypeDef* SSPn);
unsigned char SSPSendRecieveByte (LPC_SSP_TypeDef* SSPn, unsigned char dato);
void SSPSendBuffer (LPC_SSP_TypeDef * SSPn, const char *buff, unsigned int count);
void SSPReceiveBuffer (LPC_SSP_TypeDef * SSPn, unsigned char *buff, unsigned int count);


#endif
