#include "driverVS1063.h"
#include "lpc_17xx_GPIO.h"
#include "lpc_17xx_SSP.h"

//**************************************************************
// Funciones locales
unsigned int isID3TagAvailable(unsigned char* buffer);
void parserTagID3 (unsigned char* buffer, estructuraSongData* data);
unsigned int unsynchsafe (int input);
//**************************************************************

extern GPIO DREQ_VS1063;
extern xQueueHandle queueBufferLecturaSD;

xQueueHandle queueMsgDriverVS1063;
xQueueHandle queueEventosVS1063;
xSemaphoreHandle mutexAccesoVS1063;

void (*mutexHolderTaskVS)(void* param);
unsigned int reproduciendoArchivo;
unsigned int primeraVuelta;

estructuraSongData songData;

void createDriverVS1063 (void)
{
	ConfigSSP SSPConfiguracion0;


	SSPConfiguracion0.clock = 3000000;
	SSPConfiguracion0.databit = 8;
	SSPConfiguracion0.frameFormat = SSP_SPI;
	SSPConfiguracion0.rol = SSP_MASTER;
	SSPConfiguracion0.modo = MODE0_0;
	SSPInit(SPI_VS1063, SSPConfiguracion0);

	VS1063Init();

	SSP_Cmd(SPI_VS1063, DISABLE);

	SSPConfiguracion0.clock = 7000000;
	SSPConfiguracion0.databit = 8;
	SSPConfiguracion0.frameFormat = SSP_SPI;
	SSPConfiguracion0.rol = SSP_MASTER;
	SSPConfiguracion0.modo = MODE0_0;
	SSPInit(SPI_VS1063, SSPConfiguracion0);



	xTaskCreate(driverVS1063Control, (signed portCHAR*) "drv Control VS", 200, NULL, tskIDLE_PRIORITY+3, NULL);
	xTaskCreate(driverVS1063Escritura, (signed portCHAR*) "drv lect VS", 700, NULL, tskIDLE_PRIORITY+1, NULL);

	queueMsgDriverVS1063 = xQueueCreate(5, sizeof (MensajeControlVS1063));
	queueEventosVS1063 = xQueueCreate(15, sizeof (MensajeEventoVS1063));
	mutexAccesoVS1063 = xSemaphoreCreateMutex();

	mutexHolderTaskVS = NULL;

	reproduciendoArchivo = 0;

	songData.tagID3Available = 0;
}

void driverVS1063Control (void* param)
{
	MensajeControlVS1063 mensajeControl;
	MensajeEventoVS1063 mensajeEvento;
	unsigned int playMode, sciMode, bytesPendientes, dump;

	// Toma el mutex ya que hasta que no se indique PLAY
	// el task de escritura no tiene nada que hacer.
	xSemaphoreTake(mutexAccesoVS1063, portMAX_DELAY);
	mutexHolderTaskVS = driverVS1063Control;

	while (1)
	{
		// Bloquea hasta recibir un mensaje para controlar el VS1063
		xQueueReceive(queueMsgDriverVS1063, (void*)&mensajeControl, portMAX_DELAY);

		if (mutexHolderTaskVS != driverVS1063Control)
		{
			// Solamente pide el mutex si no lo tiene ya en su poder
			xSemaphoreTake(mutexAccesoVS1063, portMAX_DELAY);
			mutexHolderTaskVS = driverVS1063Control;
		}



		if (mensajeControl.commandID == VS_CONTROL_CHG_VOLUME)
		{
			VS1063WriteSCI(SCI_VOL, mensajeControl.parametro*0x0101);

			// Se genera el EVENTO de cambio de volumen
			mensajeEvento.eventID = VS_EVENTO_VOLUME_CHANGE;
			mensajeEvento.parametro = mensajeControl.parametro;
			xQueueSendToBack(queueEventosVS1063, &mensajeEvento, 0);
		}
		else if (mensajeControl.commandID == VS_CONTROL_PLAY)
		{
			reproduciendoArchivo = 1;
			primeraVuelta = 1;

			playMode = VS1063ReadMem (PAR_PLAY_MODE);
			playMode &= ~PAR_PLAY_MODE_PAUSE_ENA;
			VS1063WriteMem(PAR_PLAY_MODE, playMode);

			// Se genera el EVENTO de comienzo de reproducción
			mensajeEvento.eventID = VS_EVENTO_START_PLAYING;
			xQueueSendToBack(queueEventosVS1063, &mensajeEvento, 0);
		}
		else if (mensajeControl.commandID == VS_CONTROL_PAUSE)
		{
			reproduciendoArchivo = 0;

			playMode = VS1063ReadMem (PAR_PLAY_MODE);
			playMode |= PAR_PLAY_MODE_PAUSE_ENA;
			VS1063WriteMem(PAR_PLAY_MODE, playMode);

			// Se genera el EVENTO de reproduccion pausada
			mensajeEvento.eventID = VS_EVENTO_PAUSE_PLAYING;
			xQueueSendToBack(queueEventosVS1063, &mensajeEvento, 0);
		}
		else if (mensajeControl.commandID == VS_CONTROL_STOP)
		{
			sciMode = VS1063ReadSCI(SCI_MODE);
			VS1063WriteSCI(SCI_MODE, sciMode | SM_CANCEL);

			// Se espera hasta que el bit SM_CANCEL sea borrado por el VS1063
			while (VS1063ReadSCI(SCI_MODE) & SM_CANCEL);

			// Se vacía la cola queueBufferLecturaSD para limpiar los bytes
			// que quedan de la última canción
			bytesPendientes = uxQueueMessagesWaiting(queueBufferLecturaSD);
			while (bytesPendientes)
			{
				xQueueReceive(queueBufferLecturaSD, (void*)&dump, portMAX_DELAY);
				bytesPendientes --;
			}

			// Se genera el EVENTO de reproduccion finalizada
			mensajeEvento.eventID = VS_EVENTO_STOP_PLAYING;
			xQueueSendToBack(queueEventosVS1063, &mensajeEvento, 0);
		}


		if (reproduciendoArchivo == 1)
		{
			// Entrega el mutex para que la tarea de escritura
			// empiece a emviar la data al VS1063
			mutexHolderTaskVS = NULL;
			xSemaphoreGive(mutexAccesoVS1063);
		}

	}

}

void driverVS1063Escritura (void* param)
{
	unsigned int i;
	unsigned char buffLocal[512];
	unsigned int bytesWaiting;
	MensajeEventoVS1063 mensajeEvento;
	MensajeBloqueMemoriaSD mensajeBufferLectura;

	while (1)
	{
		// Solo puede escribir en el VS1063 cuando no se está
		// utilizando la interfaz de control. La tarea de control no
		// entrega el mutex hasta que no haya recibido el mensaje de PLAY
		xSemaphoreTake(mutexAccesoVS1063, portMAX_DELAY);
		mutexHolderTaskVS = driverVS1063Escritura;



// ******************* Exclusión mutua ***************************************

		bytesWaiting = uxQueueMessagesWaiting(queueBufferLecturaSD);
		if (bytesWaiting > 2 && !VS1063isBusy())
		{
			// Bloquea hasta que haya bytes para enviar al VS1063
			xQueueReceive(queueBufferLecturaSD, (void*)&mensajeBufferLectura, portMAX_DELAY);

			if (isID3TagAvailable(mensajeBufferLectura.ptrBloque))
			{
				parserTagID3 (mensajeBufferLectura.ptrBloque, &songData);
				songData.tagID3Available = 1;

				// Se genera el EVENTO de data ID3 disponible
				mensajeEvento.eventID = VS_EVENTO_SONG_DATA;
				xQueueSendToBack(queueEventosVS1063, &mensajeEvento, 0);
			}

			for (i = 0; i < mensajeBufferLectura.cantBytes / 32; i ++)
			{
				VS1063WriteSDI (&mensajeBufferLectura.ptrBloque[32 * i], 32);
			}
		}

		/*bytesWaiting = uxQueueMessagesWaiting(queueBufferLecturaSD);
		if (bytesWaiting > 512 && !VS1063isBusy())
		{
			for (i = 0; i < 512; i ++)
			{
				// Bloquea hasta que haya bytes para enviar al VS1063
				xQueueReceive(queueBufferLecturaSD, (void*)&buffLocal[i], portMAX_DELAY);
			}

			if (isID3TagAvailable(buffLocal))
			{
				parserTagID3 (buffLocal, &songData);
				songData.tagID3Available = 1;

				// Se genera el EVENTO de data ID3 disponible
				mensajeEvento.eventID = VS_EVENTO_SONG_DATA;
				xQueueSendToBack(queueEventosVS1063, &mensajeEvento, 0);
			}

			for (i = 0; i < 16; i ++)
			{
				VS1063WriteSDI (&buffLocal[32 * i], 32);
			}
		}
		*/

/*		if (bytesWaiting > 6000 && primeraVuelta)
		{
			primeraVuelta = 0;
		}

		if (bytesWaiting > 3000 && !VS1063isBusy() && !primeraVuelta)
		{
			for (i = 0; i < 3000 && !VS1063isBusy(); i ++)
			{
				xQueueReceive(queueBufferLecturaSD, (void*)&buffLocal[i%32], portMAX_DELAY);		// Bloquea hasta que haya bytes para enviar al VS1063

				if (i % 32 == 0)
				{
					VS1063WriteSDI (buffLocal, 32);
				}
			}

		}
*/

// ***************************************************************************

		mutexHolderTaskVS = NULL;
		xSemaphoreGive(mutexAccesoVS1063);

	}
}


unsigned int isID3TagAvailable(unsigned char* buffer)
{
	if (buffer[0] == 'I' && buffer[1] == 'D' && buffer[2] == '3')
		return 1;

	return 0;
}

void parserTagID3 (unsigned char* buffer, estructuraSongData* data)
{
	unsigned int synchSafeHeaderSize, synchSafeFrameSize;
	int headerSize, frameSize;
	unsigned int artistaEncontrado = 0, tituloEncontrado = 0, anioEncontrado = 0, albumEncontrado = 0;
	unsigned int i;
	unsigned char* ptrFrames;
	unsigned char tag[5];

	synchSafeHeaderSize = buffer[9];
	synchSafeHeaderSize |= (buffer[8] << 8);
	synchSafeHeaderSize |= (buffer[7] << 16);
	synchSafeHeaderSize |= (buffer[6] << 24);
	headerSize = unsynchsafe (synchSafeHeaderSize);

	ptrFrames = &buffer[10];

	while (headerSize > 0)
	{
		tag[0] = *ptrFrames; ptrFrames++; headerSize--;
		tag[1] = *ptrFrames; ptrFrames++; headerSize--;
		tag[2] = *ptrFrames; ptrFrames++; headerSize--;
		tag[3] = *ptrFrames; ptrFrames++; headerSize--;
		tag[4] = '\0';


		synchSafeFrameSize = (*ptrFrames << 24); ptrFrames++; headerSize--;
		synchSafeFrameSize |= (*ptrFrames << 16); ptrFrames++; headerSize--;
		synchSafeFrameSize |= (*ptrFrames << 8); ptrFrames++; headerSize--;
		synchSafeFrameSize |= *ptrFrames; ptrFrames++; headerSize--;
		frameSize = unsynchsafe (synchSafeFrameSize); headerSize--;

		// Dejo el puntero apuntando al payload del tag (el header de cada tag tiene 10 bytes)
		ptrFrames++; ptrFrames++; ptrFrames++;
		headerSize--; headerSize--; headerSize--;

		if (!strcmp(tag, "TIT2"))
		{
			// Titulo de la cancion
			strncpy(songData.titulo, ptrFrames, frameSize);
			songData.titulo[frameSize] = '\0';

			tituloEncontrado = 1;
		}
		else if (!strcmp(tag, "TALB"))
		{
			// Album de la cancion
			strncpy(songData.album, ptrFrames, frameSize);
			songData.album[frameSize] = '\0';

			albumEncontrado = 1;
		}
		else if (!strcmp(tag, "TPE1"))
		{
			// Banda/intérprete de la cancion
			strncpy(songData.artista, ptrFrames, frameSize);
			songData.artista[frameSize] = '\0';

			artistaEncontrado = 1;
		}
		else if (!strcmp(tag, "TDRL"))
		{
			// Año de salida
			strncpy(songData.anio, ptrFrames, frameSize);
			songData.anio[frameSize] = '\0';

			anioEncontrado = 1;
		}

		ptrFrames += (frameSize - 1);
		headerSize -= (frameSize - 1);
	}

	if (tituloEncontrado == 0)
		songData.titulo[0] = '\0';
	if (albumEncontrado == 0)
		songData.album[0] = '\0';
	if (artistaEncontrado == 0)
		songData.artista[0] = '\0';
	if (anioEncontrado == 0)
		songData.anio[0] = '\0';
}

unsigned int unsynchsafe (int input)
{
	int output = 0, mask = 0x7F000000;

	while (mask)
	{
		output >>= 1;
		output |= input & mask;
		mask >>= 8;
	}

	return output;
}


void vs1063GetSongData_nombreArchivo (unsigned char *nombreArchivo)
{

}

void vs1063GetSongData_titulo (unsigned char *titulo)
{
	if (songData.tagID3Available)
		strcpy(titulo, songData.titulo);
	else
		*titulo = '\0';
}

void vs1063GetSongData_artista (unsigned char *artista)
{
	if (songData.tagID3Available)
		strcpy(artista, songData.artista);
	else
		*artista = '\0';
}

void vs1063GetSongData_album (unsigned char *album)
{
	if (songData.tagID3Available)
		strcpy(album, songData.album);
	else
		*album = '\0';
}

void vs1063GetSongData_anio (unsigned char *anio)
{
	if (songData.tagID3Available)
		strcpy(anio, songData.anio);
	else
		*anio = '\0';
}
