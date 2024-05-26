#ifndef FSMBLINKLED_H_
#define FSMBLINKLED_H_

#include "FreeRTOS.h"
#include "task.h"
#include "lpc_17xx_GPIO.h"

// **************************************************
// FSM para Parpadear LED
typedef struct
{
	GPIO* led;
	unsigned int parpadear;
	unsigned int start;
	unsigned int intervalo;
	unsigned int repeticiones;
	void (*estadoFSM) ();
}parpadearLED;


void parpadearInit (parpadearLED* FSMparpadear, GPIO* led);
void parpadearHandler (parpadearLED* FSMparpadear);
void parpadearApagado (parpadearLED* FSMparpadear);
void parpadearPrendido (parpadearLED* FSMparpadear);
void parpadearStart (parpadearLED* FSMparpadear, unsigned int intervalo, unsigned int repeticiones);
void parpadearStop (parpadearLED* FSMparpadear);
// **************************************************

#endif
