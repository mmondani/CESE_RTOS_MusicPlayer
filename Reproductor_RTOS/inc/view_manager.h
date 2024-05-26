#ifndef VIEW_MANAGER_H_
#define VIEW_MANAGER_H_

#include <stdio.h>	//Uso sprintf
#include <string.h>	//Uso strlen
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "common_enum.h"
#include "driverGLCD.h"

//     Vistas y View Manager
// ************************************************************************

typedef struct
{
	glcdObjectType objectType[20];
	unsigned int objectID[20];
}screenView;


typedef struct
{
	unsigned char evento;
	unsigned char objectID;
	glcdObjectType objectType;
}MensajeColaViewManager;


#define CANT_GLCD_VIEWS				3
#define CANT_GLCD_BOTONES			9
#define CANT_GLCD_PROGRESS_BARS		0
#define	CANT_GLCD_INDICADOR_VAR		0
#define	CANT_GLCD_GRAFICOS			0
#define	CANT_GLCD_SERIES			0
#define CANT_GLCD_SCROLL_BARS		3
#define CANT_GLCD_MSG_BOX			1
#define	CANT_GLCD_ICON_MENU			1
#define CANT_GLCD_LIST_BOX			1
#define CANT_GLCD_TEXT_BOX			5


screenView viewsGLCD[CANT_GLCD_VIEWS];
glcdBoton glcdBotones[CANT_GLCD_BOTONES];
glcdScrollBar glcdScrollBars[CANT_GLCD_SCROLL_BARS];
glcdMessageBox glcdMensajes[CANT_GLCD_MSG_BOX];
glcdIconMenu glcdMenuIconos[CANT_GLCD_ICON_MENU];
glcdListBox glcdListas[CANT_GLCD_LIST_BOX];
glcdListBoxItem glcdItemLista1[20];
glcdTextBox glcdTexBoxes[CANT_GLCD_TEXT_BOX];
glcdBotonera glcdBotoneras[1];
// ************************************************************************


// ************************************************************************
// Task
void createGLCDViewManager (fsmUART* periphUART);
void glcdViewManager(void* uart);
void initViewsAndObjects (void);
// ************************************************************************


#endif
