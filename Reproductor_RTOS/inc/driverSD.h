#ifndef DRIVERSD_H_
#define DRIVERSD_H_

// ******************************************
// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
// ******************************************

// ******************************************
// FATfs
#include "define_fatfs.h"
#include "ff.h"
#include "delayRIT.h"
// ******************************************

#include "driverGLCD.h"
#include "view_manager.h"
#include "common_enum.h"

//*************************************************************
// Elemento de la cola del task de control

typedef struct
{
	unsigned int commandID;
	unsigned char parametro[40];
}MensajeControlSD;

enum {SD_CONTROL_OPEN_FILE = 0, SD_CONTROL_CLOSE_FILE, SD_CONTROL_READ_FILE, SD_CONTROL_CHANGE_DIR, SD_CONTROL_SCAN_DIR};
//*************************************************************

//*************************************************************
// Elemento de la cola de eventos

typedef struct
{
	unsigned int eventID;
	unsigned int parametro;
}MensajeEventoSD;

enum {SD_EVENTO_FILE_OPENED = 0,SD_EVENTO_END_OF_FILE, SD_EVENTO_SCAN_COMPLETE, SD_EVENTO_ERROR};
//*************************************************************

//*************************************************************
// Elemento de la cola de buffer de lectura

typedef struct
{
	unsigned int cantBytes;
	unsigned char* ptrBloque;
}MensajeBloqueMemoriaSD;
//*************************************************************


//*************************************************************
// Task
void createDriverSD (void);
void driverSDControl (void* param);
void driverSDLectura (void* param);
void handlerItemScanDir (FILINFO* Finfo, unsigned int numeroItem);
//*************************************************************


#endif
