#ifndef TEMPORIZADOR_H_
#define TEMPORIZADOR_H_


#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"


// **************************************************
// Temporizadores
typedef struct
{
	unsigned int start;
	unsigned int intervalo;
}Temporizador;

void initTemporizador (Temporizador* timer, unsigned int intervalo);
void restartTemporizador (Temporizador* timer);
unsigned int expiroTemporizador (Temporizador timer);



void initGlobalSystemTick (void);
unsigned int getSystemTicks (void);
// **************************************************


#endif
