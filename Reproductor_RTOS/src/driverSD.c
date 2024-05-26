#include "driverSD.h"
#include "lpc_17xx_SSP.h"
#include "delayRIT.h"

xQueueHandle queueMsgDriverSD;
xQueueHandle queueBufferLecturaSD;
xQueueHandle queueEventosSD;
xSemaphoreHandle mutexAccesoSD;

FATFS Fatfs; /* File system object */
FIL Fil; /* File object */
DIR dir; /* Directory object */
FILINFO fno; /* File information object */
unsigned char filePath[] = "song2.mp3";
unsigned char dirPath[30]={'\0'};

void die(FRESULT rc);
DWORD get_fattime(void);



extern glcdListBox glcdListas[];
extern glcdScrollBar glcdScrollBars[];
extern xQueueHandle queueMsgViewManager;


void (*mutexHolderTask)(void* param);		// Puntero usado para saber cual de las tareas tiene el mutex
unsigned int leyendoArchivoSD;

unsigned char bufferLecturaSD[20][256];
unsigned char ptrBloqueMemoria;

void createDriverSD (void)
{
	ConfigSSP SSPConfiguracion1;
	FRESULT rc; /* Result code */


	SSPConfiguracion1.clock = 20000000;
	SSPConfiguracion1.databit = 8;
	SSPConfiguracion1.frameFormat = SSP_SPI;
	SSPConfiguracion1.rol = SSP_MASTER;
	SSPConfiguracion1.modo = MODE0_0;
	SSPInit(SD_SPI, SSPConfiguracion1);

	// Configura el RIT para generar los delays necesarios de FATfs
	delayRITInit();

	// Montar el sistema de archivos (no falla nunca)
	rc = f_mount(0, &Fatfs);
	if (rc)
		die(rc);


	xTaskCreate(driverSDControl, (signed portCHAR*) "drv Control SD", 200, NULL, tskIDLE_PRIORITY+3, NULL);
	xTaskCreate(driverSDLectura, (signed portCHAR*) "drv lect SD", 300, NULL, tskIDLE_PRIORITY+1, NULL);

	queueMsgDriverSD = xQueueCreate(5, sizeof (MensajeControlSD));
	//queueBufferLecturaSD = xQueueCreate(5000, sizeof (unsigned char));
	queueBufferLecturaSD = xQueueCreate(15, sizeof (MensajeBloqueMemoriaSD));
	queueEventosSD = xQueueCreate(15, sizeof (MensajeEventoSD));
	mutexAccesoSD = xSemaphoreCreateMutex();

	mutexHolderTask = NULL;

	leyendoArchivoSD = 0;

	ptrBloqueMemoria = 0;
}

void driverSDControl (void* param)
{
	MensajeControlSD mensajeControl;
	MensajeEventoSD mensajeEvento;
	DIR dirs;
	FRESULT res;
	FILINFO fileInfo;
	unsigned int numeroItem = 0;
	static char lfn[_MAX_LFN + 1];   /* Buffer para el LFN (long file name) */
	fileInfo.lfname = lfn;
	fileInfo.lfsize = sizeof (lfn);

	// Toma el mutex ya que hasta que no se abra un archivo
	// el task de lectura no tiene nada que hacer.
	xSemaphoreTake(mutexAccesoSD, portMAX_DELAY);
	mutexHolderTask = driverSDControl;

	while (1)
	{
		// Bloquea hasta recibir un mensaje para trabajar con el sistema
		// de archivos de la SD.
		xQueueReceive(queueMsgDriverSD, (void*)&mensajeControl, portMAX_DELAY);

		// Solamente pide el mutex si no lo tiene ya en su poder
		if (mutexHolderTask != driverSDControl)
		{
			xSemaphoreTake(mutexAccesoSD, portMAX_DELAY);
			mutexHolderTask = driverSDControl;
		}

		if (mensajeControl.commandID == SD_CONTROL_SCAN_DIR)
		{
			// Se escanean todos los elementos del directorio actual (incluidos . y ..)
			res = f_opendir(&dirs, "");
			if (res)
				die(res);

			numeroItem = 0;

			while (((res = f_readdir(&dirs, &fileInfo)) == FR_OK) && fileInfo.fname[0])
			{
				//if (_FS_RPATH && Finfo.fname[0] == '.') continue;

				handlerItemScanDir (&fileInfo, numeroItem);
				numeroItem ++;
			}
			handlerItemScanDir (NULL, numeroItem);

			// Se genera el EVENTO de finalización del escaneo del directorio actual.
			mensajeEvento.eventID = SD_EVENTO_SCAN_COMPLETE;
			xQueueSendToBack(queueEventosSD, &mensajeEvento, 0);
		}
		else if (mensajeControl.commandID == SD_CONTROL_CHANGE_DIR)
		{
			res = f_chdir(mensajeControl.parametro);
			if (res)
				die(res);

		}
		else if (mensajeControl.commandID == SD_CONTROL_OPEN_FILE)
		{
			res = f_open(&Fil, mensajeControl.parametro, FA_READ);
			if (res)
				die(res);

			// Se genera el EVENTO de archivo abierto (incluye el tamaño del archivo).
			mensajeEvento.eventID = SD_EVENTO_FILE_OPENED;
			mensajeEvento.parametro = f_size(&Fil);
			xQueueSendToBack(queueEventosSD, &mensajeEvento, 0);
		}
		else if (mensajeControl.commandID == SD_CONTROL_CLOSE_FILE)
		{
			leyendoArchivoSD = 0;
			f_close(&Fil);
		}
		else if (mensajeControl.commandID == SD_CONTROL_READ_FILE)
		{
			leyendoArchivoSD = 1;
		}


		if (leyendoArchivoSD == 1)
		{
			mutexHolderTask = NULL;
			xSemaphoreGive(mutexAccesoSD);
		}
	}
}

void driverSDLectura (void* param)
{
	MensajeControlSD mensajeColaControlSD;
	MensajeEventoSD mensajeEvento;
	MensajeBloqueMemoriaSD mensajeBufferLectura;
	FRESULT res;
	unsigned int bytesRead = 0, i;
	//unsigned char buffLocal[256];

	while (1)
	{
		// La tarea de control solo libera el mutex si hay algún archivo para leer
		xSemaphoreTake(mutexAccesoSD, portMAX_DELAY);
		mutexHolderTask = driverSDLectura;


// ******************* Exclusión mutua ***************************************

		//res = f_read(&Fil, &buffLocal, 256, &bytesRead);
		res = f_read(&Fil, bufferLecturaSD[ptrBloqueMemoria], 256, &bytesRead);

		if (res)
			die(res);

		if (!bytesRead)
		{
			// No hay bytes leidos --> Se alcanzó el final del archivo
			mensajeColaControlSD.commandID = SD_CONTROL_CLOSE_FILE;
			xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeColaControlSD, 0);

			// Se genera el EVENTO de fin de archivo
			mensajeEvento.eventID = SD_EVENTO_END_OF_FILE;
			xQueueSendToBack(queueEventosSD, &mensajeEvento, 0);
		}

// ***************************************************************************

		mutexHolderTask = NULL;
		xSemaphoreGive(mutexAccesoSD);

		if (bytesRead)
		{
			mensajeBufferLectura.cantBytes = bytesRead;
			mensajeBufferLectura.ptrBloque = bufferLecturaSD[ptrBloqueMemoria];

			xQueueSendToBack(queueBufferLecturaSD, &mensajeBufferLectura, portMAX_DELAY);
		}

		ptrBloqueMemoria ++;
		if (ptrBloqueMemoria > 19)
		{
			ptrBloqueMemoria = 0;
		}

		/*for (i = 0; i < bytesRead; i++)
		{
			xQueueSendToBack(queueBufferLecturaSD, &buffLocal[i], portMAX_DELAY);
		}*/
	}
}

/*
 Función que debe ser implementada de acuerdo a la aplicación concreta en la
 que se use el driver.
 */
void handlerItemScanDir (FILINFO* fileInfo, unsigned int numeroItem)
{
	char *fn;

	if (fileInfo != NULL)
	{
		if (numeroItem < 30)		// La LISTA_ITEM tiene 30 items
		{
#if _USE_LFN
			fn = ((fileInfo->lfname[0])? fileInfo->lfname : fileInfo->fname);
#else
			fn = fileInfo->fname;
#endif

			if (fileInfo->fattrib & AM_DIR)		// Es un directorio
			{
				glcdListas[0].lineas[numeroItem].tipo = DIRECTORIO;
			}
			else								// Es un archivo
			{
				glcdListas[0].lineas[numeroItem].tipo = ARCHIVO;
			}
			strcpy(glcdListas[0].lineas[numeroItem].texto, fn);
		}
	}
	else
	{
		glcdListas[0].lineas[numeroItem].tipo = NONE_TYPE;

		// Indico la cantidad total de lineas cargadas en el list box
		glcdListas[0].cantidadLineas = numeroItem;

		// Modifico el máximo del scoll bar asociado al ListBox de acuerdo
		// a la cantidad de elementos que va a contener la lista.
		glcdScrollBars[1].max = numeroItem - 1;
	}
}


/*
 Función a la que se llama si ocurrió un error en cualquiera de las
 operaciones con las SD
 */
void die(FRESULT rc)
{
	MensajeColaViewManager mensajeCola;
	MensajeEventoSD mensajeEvento;

	printf ("Failed with rc=%u.\n\r", rc);


	// Cuando falla alguna operación de la SD se muestra un mensaje de
	// error en el GLCD del cual no se puede salir.

	// Se genera el EVENTO de error de SD
	mensajeEvento.eventID = SD_EVENTO_ERROR;
	xQueueSendToBack(queueEventosSD, &mensajeEvento, 0);


	/*mensajeCola.evento = SHOW_MESSAGE_BOX_BLOCKING;
	mensajeCola.objectID = MESSAGE_ERROR_SD;

	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);*/


	for (;;)
		;
}


DWORD get_fattime(void) {
	return ((DWORD) (2012 - 1980) << 25) /* Year = 2012 */
	| ((DWORD) 1 << 21) /* Month = 1 */
	| ((DWORD) 1 << 16) /* Day_m = 1*/
	| ((DWORD) 0 << 11) /* Hour = 0 */
	| ((DWORD) 0 << 5) /* Min = 0 */
	| ((DWORD) 0 >> 1); /* Sec = 0 */

}
