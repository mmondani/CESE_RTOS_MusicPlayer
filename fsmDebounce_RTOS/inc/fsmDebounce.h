#ifndef FSMDEBOUNCE_H_
#define FSMDEBOUNCE_H_

#include "FreeRTOS.h"
#include "task.h"
#include "lpc_17xx_GPIO.h"

// **************************************************
// FSM para Debounce de un pulsador
typedef struct
{
	GPIO* pulsador;
	unsigned int start;
	unsigned int intervalo;
	unsigned int estadoPulsador;
	unsigned int flagPresionado;
	void (*estadoFSM) ();
}debouncePulsador;



void debounceInit (debouncePulsador* FSMpulsador, GPIO* pulsador, unsigned int intervalo);
void debounceHandler (debouncePulsador* FSMpulsador);
void debounceNoPresionado (debouncePulsador* FSMpulsador);
void debounceEspera (debouncePulsador* FSMpulsador);
void debouncePresionado (debouncePulsador* FSMpulsador);
unsigned int debounceEstaPresionado (debouncePulsador* FSMpulsador);
unsigned int debounceFlancoActivo (debouncePulsador* FSMpulsador);
unsigned int debounceFlancoPasivo (debouncePulsador* FSMpulsador);
// **************************************************



#endif
