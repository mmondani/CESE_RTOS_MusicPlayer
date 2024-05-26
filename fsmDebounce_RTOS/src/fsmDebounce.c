#include "fsmDebounce.h"



void debounceInit (debouncePulsador* FSMpulsador, GPIO* pulsador, unsigned int intervalo)
{
	FSMpulsador->pulsador = pulsador;
	FSMpulsador->estadoFSM = debounceNoPresionado;
	FSMpulsador->intervalo = intervalo;
	FSMpulsador->estadoPulsador = !pulsador->estadoActivo;
	FSMpulsador->flagPresionado = 0;
}

void debounceHandler (debouncePulsador* FSMpulsador)
{
	(*FSMpulsador->estadoFSM)(FSMpulsador);
}

void debounceNoPresionado (debouncePulsador* FSMpulsador)
{
	if (GPIOestaActivo(FSMpulsador->pulsador))
	{
		FSMpulsador->start = xTaskGetTickCount();
		FSMpulsador->estadoFSM = debounceEspera;
	}
}

void debounceEspera (debouncePulsador* FSMpulsador)
{
	if (xTaskGetTickCount() - FSMpulsador->start > FSMpulsador->intervalo)
	{
		if (GPIOestaActivo(FSMpulsador->pulsador))
		{
			FSMpulsador->estadoFSM = debouncePresionado;
			FSMpulsador->estadoPulsador = FSMpulsador->pulsador->estadoActivo;
		}
		else
		{
			FSMpulsador->estadoPulsador = !FSMpulsador->pulsador->estadoActivo;
			FSMpulsador->estadoFSM = debounceNoPresionado;
		}
	}
}

void debouncePresionado (debouncePulsador* FSMpulsador)
{
	if (!GPIOestaActivo(FSMpulsador->pulsador))
	{
		FSMpulsador->start = xTaskGetTickCount();
		FSMpulsador->estadoFSM = debounceEspera;
	}
}


unsigned int debounceEstaPresionado (debouncePulsador* FSMpulsador)
{
	return FSMpulsador->estadoPulsador == FSMpulsador->pulsador->estadoActivo;
}

unsigned int debounceFlancoActivo (debouncePulsador* FSMpulsador)
{
	if (debounceEstaPresionado (FSMpulsador) && FSMpulsador->flagPresionado == 0)
	{
		FSMpulsador->flagPresionado = 1;

		return 1;
	}

	return 0;
}

unsigned int debounceFlancoPasivo (debouncePulsador* FSMpulsador)
{
	if (!debounceEstaPresionado (FSMpulsador) && FSMpulsador->flagPresionado == 1)
	{
		FSMpulsador->flagPresionado = 0;

		return 1;
	}

	return 0;
}
