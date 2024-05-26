#ifndef REPRODUCTOR_H_
#define REPRODUCTOR_H_


// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "fsmUART.h"


// ******************************************
// Reproductor
typedef enum {
	REPRODUCTOR_PLAY = 0, REPRODUCTOR_PAUSE, REPRODUCTOR_STOPPING, REPRODUCTOR_STOP
}estadoReproductor;

typedef struct
{
	unsigned int volumen;
	estadoReproductor estado;
	unsigned int filesSize;
	unsigned char titulo[31];
	unsigned char artista[31];
	unsigned char album[31];
	unsigned char anio[5];
	unsigned int playingTime_ticks;
	unsigned int totalTime;
	unsigned int archivoPendiente;
	unsigned int actualizarSongData;
}estructuraReproductor;

estructuraReproductor reproductorMusica;


xQueueHandle queueEventosUsuario;
// ******************************************


//*************************************************************
// Task
void createReproductor (void);
void musicPlayer (void* param);
//*************************************************************


#endif
