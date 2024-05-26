/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

// ******************************************
// Libreria estandar C
#include <stdio.h>	//Uso sprintf
#include <string.h>	//Uso strlen
// ******************************************

// ******************************************
// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
// ******************************************


// ******************************************
// Librerias Periféricos
#include "lpc_17xx_GPIO.h"
#include "lpc_17xx_SSP.h"
#include "fsmDebounce.h"
#include "driverUART.h"
#include "driverSD.h"
#include "driverVS1063.h"
#include "reproductor.h"
#include "temporizadores.h"
// ******************************************


// ******************************************
// GLCD
#include "driverGLCD.h"
#include "view_interface_matrix.h"
#include "view_manager.h"
#include "common_enum.h"
// ******************************************


// ******************************************
// Tasks
void eventManager (void* param);
void blinkLed (void* led);
void musicPlayer (void* param);
// ******************************************


GPIO ledOK;
GPIO pinCardDetect;
GPIO pulsadorArriba, pulsadorAbajo, pulsadorIzquierda, pulsadorDerecha, pulsadorEnter;
debouncePulsador debouncePulsadorArriba, debouncePulsadorAbajo, debounceIzquierda, debounceDerecha, debounceEnter, debounceCardDetect;

GPIO xCS_VS1063, xDCS_VS1063, DREQ_VS1063, RST_VS1063;



extern xQueueHandle queueMsgGLCD;
extern xQueueHandle queueMsgViewManager;
extern xQueueHandle queueMsgDriverSD;
extern xQueueHandle queueMsgDriverVS1063;
extern xQueueHandle queueBufferLecturaSD;
extern xQueueHandle queueEventosSD;
extern xQueueHandle queueEventosVS1063;
extern xQueueHandle queueEventosUsuario;


extern screenView viewsGLCD[];
extern glcdBoton glcdBotones[];
extern glcdScrollBar glcdScrollBars[];
extern glcdMessageBox glcdMensajes[];
extern glcdIconMenu glcdMenuIconos[];
extern glcdListBox glcdListas[];
extern glcdListBoxItem glcdItemLista1[];

extern fsmUART fsmUART3;;

extern void (*reaccionesSistema [CANTIDAD_VISTAS][CANTIDAD_EVENTOS])(unsigned char param);
extern unsigned int vistaActual;
extern unsigned int vistaAnterior;

Temporizador timerRefreshVistaReproductor;

extern estructuraReproductor reproductorMusica;

int main ( void )
{
	MensajeColaViewManager mensajeViewManager;

// *******************************************************************************************************
// INICIALIZACION
	GPIOInit(&ledOK, SALIDA, BAJO, 0, 22);
	GPIOInit(&pulsadorArriba, ENTRADA, ALTO, 2, 6);
	GPIOInit(&pulsadorAbajo, ENTRADA, ALTO, 2, 8);
	GPIOInit(&pulsadorIzquierda, ENTRADA, ALTO, 2, 12);
	GPIOInit(&pulsadorDerecha, ENTRADA, ALTO, 2, 11);
	GPIOInit(&pulsadorEnter, ENTRADA, ALTO, 2, 7);

	GPIOInit(&CS_SD, SALIDA, ALTO, SD_CS_PORT, SD_CS_PIN);
	GPIOInit(&pinCardDetect, ENTRADA, BAJO, 2, 4);

	GPIOInit(&xCS_VS1063, SALIDA, ALTO, 1, 30);
	GPIOInit(&xDCS_VS1063, SALIDA, ALTO, 0, 3);
	GPIOInit(&DREQ_VS1063, ENTRADA, ALTO, 0, 27);
	GPIOInit(&RST_VS1063, SALIDA, ALTO, 1, 31);


	debounceInit (&debouncePulsadorArriba, &pulsadorArriba, 20);
	debounceInit (&debouncePulsadorAbajo, &pulsadorAbajo, 20);
	debounceInit (&debounceIzquierda, &pulsadorIzquierda, 20);
	debounceInit (&debounceDerecha, &pulsadorDerecha, 20);
	debounceInit (&debounceEnter, &pulsadorEnter, 20);
	debounceInit (&debounceCardDetect, &pinCardDetect, 80);

// *******************************************************************************************************


	xTaskCreate(eventManager, (signed portCHAR*) "eventMng", 300, NULL, tskIDLE_PRIORITY+4, NULL);
	xTaskCreate(blinkLed, (signed portCHAR*) "Blink OK", 25, (void*)&ledOK, tskIDLE_PRIORITY+1, NULL);
	createDriverUART();
	createDriverGLCD(&fsmUART3);
	initViewsAndObjects();
	createGLCDViewManager(&fsmUART3);
	createDriverSD();
	createDriverVS1063();
	createReproductor();

	mensajeViewManager.evento = CHANGE_VIEW;
	mensajeViewManager.objectID = VISTA_MENU;
	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeViewManager, 0);

	initTemporizador(&timerRefreshVistaReproductor, 400);


	vTaskStartScheduler();
	return 0;
}

void eventManager (void* param)
{
	portTickType ticker = xTaskGetTickCount();
	MensajeColaViewManager mensajeCola;


	while(1)
	{
		debounceHandler(&debouncePulsadorArriba);
		debounceHandler(&debouncePulsadorAbajo);
		debounceHandler(&debounceIzquierda);
		debounceHandler(&debounceDerecha);
		debounceHandler(&debounceEnter);
		debounceHandler(&debounceCardDetect);


		if (debounceFlancoActivo(&debouncePulsadorArriba))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_ARRIBA])(1);
		}
		if (debounceFlancoPasivo(&debouncePulsadorArriba))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_ARRIBA])(0);
		}


		if (debounceFlancoActivo(&debouncePulsadorAbajo))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_ABAJO])(1);
		}
		if (debounceFlancoPasivo(&debouncePulsadorAbajo))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_ABAJO])(0);
		}


		if (debounceFlancoActivo(&debounceIzquierda))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_IZQUIERDA])(1);
		}
		if (debounceFlancoPasivo(&debounceIzquierda))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_IZQUIERDA])(0);
		}


		if (debounceFlancoActivo(&debounceDerecha))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_DERECHA])(1);
		}
		if (debounceFlancoPasivo(&debounceDerecha))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_DERECHA])(0);
		}


		if (debounceFlancoActivo(&debounceEnter))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_ENTER])(1);
		}
		if (debounceFlancoPasivo(&debounceEnter))
		{
			(*reaccionesSistema[vistaActual][PULSADOR_ENTER])(0);
		}

		debounceFlancoActivo(&debounceCardDetect);
		if (debounceFlancoPasivo(&debounceCardDetect))
		{
			mensajeCola.evento = SHOW_MESSAGE_BOX_BLOCKING;
			mensajeCola.objectID = MESSAGE_ERROR_SD;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}


		if (expiroTemporizador (timerRefreshVistaReproductor))
		{
			restartTemporizador(&timerRefreshVistaReproductor);
			(*reaccionesSistema[vistaActual][TIMER_REFRESH_REPRO])(1);
		}


		vTaskDelayUntil(&ticker, 20/portTICK_RATE_MS);
	}
}


/*
 Tarea para mostrar un heartbeat
 */
void blinkLed (void* led)
{
	GPIO* blinky = (GPIO*)led;

	while (1)
	{
		GPIOactivarPin(blinky);
		vTaskDelay(500/portTICK_RATE_MS);
		GPIOpasivarPin(blinky);
		vTaskDelay(500/portTICK_RATE_MS);
	}
}



//No borrar! Son necesarios para que compile la aplicacion
void vApplicationTickHook ( void )
{
	// Aumenta constantemente los ticks del playing time.
	// Cuando se comienza a reproducir se resetea a cero esta variable
	if (reproductorMusica.estado == REPRODUCTOR_PLAY)
		reproductorMusica.playingTime_ticks ++;
}

//No borrar! Son necesarios para que compile la aplicacion
void vApplicationIdleHook ( void )
{

}

