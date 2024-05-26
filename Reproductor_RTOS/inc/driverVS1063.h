#ifndef DRIVERVS1063_H_
#define DRIVERVS1063_H_

// ******************************************
// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
// ******************************************

// ******************************************
// VS1063
#include "vs1063.h"
#include "vs10xx_uc.h"
// ******************************************

#include "common_enum.h"
#include "driverSD.h"

//*************************************************************
// Elemento de la cola del task de control

typedef struct
{
	unsigned int commandID;
	unsigned char parametro;
}MensajeControlVS1063;

enum {VS_CONTROL_CHG_VOLUME = 0, VS_CONTROL_PLAY, VS_CONTROL_PAUSE, VS_CONTROL_STOP};
//*************************************************************

//*************************************************************
// Elemento de la cola de eventos

typedef struct
{
	unsigned int eventID;
	unsigned int parametro;
}MensajeEventoVS1063;

enum {VS_EVENTO_START_PLAYING = 0, VS_EVENTO_STOP_PLAYING, VS_EVENTO_PAUSE_PLAYING, VS_EVENTO_VOLUME_CHANGE,
	VS_EVENTO_SONG_DATA};
//*************************************************************

//*************************************************************
// Estructura que contiene información del tema reproduciéndose

typedef struct
{
	unsigned int tagID3Available;
	unsigned char nombreArchivo[31];
	unsigned char titulo[31];
	unsigned char artista[31];
	unsigned char album[31];
	unsigned char anio[5];
}estructuraSongData;

//*************************************************************

//*************************************************************
// Task
void createDriverVS1063 (void);
void driverVS1063Control (void* param);
void driverVS1063Escritura (void* param);
//*************************************************************

//*************************************************************
// Funciones públicas
void vs1063GetSongData_nombreArchivo (unsigned char *nombreArchivo);
void vs1063GetSongData_titulo (unsigned char *titulo);
void vs1063GetSongData_artista (unsigned char *artista);
void vs1063GetSongData_album (unsigned char *album);
void vs1063GetSongData_anio (unsigned char *anio);
//*************************************************************

#endif
