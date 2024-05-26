#include "fsmBlinkLed.h"



void parpadearInit (parpadearLED* FSMparpadear, GPIO* led)
{
	FSMparpadear->led = led;
	FSMparpadear->estadoFSM = parpadearApagado;
	FSMparpadear->intervalo = 0;
	FSMparpadear->repeticiones = 0;
	FSMparpadear->parpadear = 0;
}

void parpadearHandler (parpadearLED* FSMparpadear)
{
	(*FSMparpadear->estadoFSM)(FSMparpadear);
}

void parpadearApagado (parpadearLED* FSMparpadear)
{
	if (FSMparpadear->parpadear && FSMparpadear->repeticiones && (xTaskGetTickCount() - FSMparpadear->start > FSMparpadear->intervalo))
	{
		FSMparpadear->estadoFSM = parpadearPrendido;
		FSMparpadear->start = xTaskGetTickCount();
		GPIOactivarPin(FSMparpadear->led);
	}
}

void parpadearPrendido (parpadearLED* FSMparpadear)
{
	if (FSMparpadear->parpadear && FSMparpadear->repeticiones && (xTaskGetTickCount() - FSMparpadear->start > FSMparpadear->intervalo))
	{
		FSMparpadear->estadoFSM = parpadearApagado;
		FSMparpadear->start = xTaskGetTickCount();
		FSMparpadear->repeticiones --;
		if (FSMparpadear->repeticiones == 0)
		{
			FSMparpadear->parpadear = 0;
		}
		GPIOpasivarPin(FSMparpadear->led);
	}
}

void parpadearStart (parpadearLED* FSMparpadear, unsigned int intervalo, unsigned int repeticiones)
{
	if (!FSMparpadear->parpadear)
	{
		FSMparpadear->start= xTaskGetTickCount();
		FSMparpadear->intervalo = intervalo;
		FSMparpadear->parpadear = 1;
		FSMparpadear->repeticiones = repeticiones;
		FSMparpadear->estadoFSM = parpadearApagado;
	}
}

void parpadearStop (parpadearLED* FSMparpadear)
{
	FSMparpadear->parpadear = 0;
	FSMparpadear->estadoFSM = parpadearApagado;
	GPIOpasivarPin(FSMparpadear->led);
}
