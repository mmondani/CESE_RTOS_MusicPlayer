#ifndef VS1063_H_
#define VS1063_H_


#include "lpc_17xx_GPIO.h"
#include "lpc_17xx_SSP.h"

#define SPI_VS1063	LPC_SSP0


int VS1063Init(void);
unsigned int VS1063isBusy (void);
void VS1063WriteMem(unsigned int addr, unsigned int data);
unsigned int VS1063ReadMem(unsigned int addr);
void VS1063WriteSCI(unsigned char addr, unsigned int data);
unsigned int VS1063ReadSCI(unsigned char addr);
int VS1063WriteSDI(const unsigned char *data, unsigned char bytes);
void VS1063SineTest (void);


#endif
