#include "reproductor.h"
#include "view_interface_matrix.h"
#include "driverSD.h"
#include "driverVS1063.h"
#include "view_manager.h"
#include "common_enum.h"


extern xQueueHandle queueMsgGLCD;
extern xQueueHandle queueMsgViewManager;
extern xQueueHandle queueMsgDriverSD;
extern xQueueHandle queueMsgDriverVS1063;
extern xQueueHandle queueBufferLecturaSD;
extern xQueueHandle queueEventosSD;
extern xQueueHandle queueEventosVS1063;
extern xQueueHandle queueEventosUsuario;

extern glcdBoton glcdBotones[];
extern glcdScrollBar glcdScrollBars[];
extern glcdMessageBox glcdMensajes[];
extern glcdIconMenu glcdMenuIconos[];
extern glcdListBox glcdListas[];
extern glcdListBoxItem glcdItemLista1[];


void createReproductor (void)
{
	xTaskCreate(musicPlayer, (signed portCHAR*) "player", 100, NULL, tskIDLE_PRIORITY+4, NULL);
	queueEventosUsuario = xQueueCreate(15, sizeof(MensajeEventoUsuario));


	// Inicialización de la máquina de estados del reproductor
	reproductorMusica.estado = REPRODUCTOR_STOP;
	reproductorMusica.actualizarSongData = 1;
}


void musicPlayer (void* param)
{
	portTickType ticker = xTaskGetTickCount();
	MensajeEventoUsuario mensajeEventosUsuario;
	MensajeEventoSD mensajeEventosSD;
	MensajeEventoVS1063 mensajeEventosVS1063;
	MensajeControlVS1063 mensajeVS1063Control;
	MensajeControlSD mensajeSDControl;
	MensajeColaViewManager mensajeViewManager;

	while (1)
	{
		// Revisa si alguna de las colas de eventos tiene mensajes pendientes.
		// Si los tiene, procesa el mensaje de acuerdo al ID del evento.

		if (uxQueueMessagesWaiting(queueEventosUsuario))
		{
			xQueueReceive(queueEventosUsuario, (void*)&mensajeEventosUsuario, 0);

			if (mensajeEventosUsuario.eventID == USER_EVENTO_BOTON_PLAY)
			{
				if (reproductorMusica.estado == REPRODUCTOR_PAUSE)
				{
					mensajeVS1063Control.commandID = VS_CONTROL_PLAY;
					xQueueSendToBack(queueMsgDriverVS1063, &mensajeVS1063Control, portMAX_DELAY);
				}
			}
			else if (mensajeEventosUsuario.eventID == USER_EVENTO_BOTON_PAUSA)
			{
				if (reproductorMusica.estado == REPRODUCTOR_PLAY)
				{
					mensajeVS1063Control.commandID = VS_CONTROL_PAUSE;
					xQueueSendToBack(queueMsgDriverVS1063, &mensajeVS1063Control, portMAX_DELAY);
				}
			}
		}
		if (uxQueueMessagesWaiting(queueEventosSD))
		{
			xQueueReceive(queueEventosSD, (void*)&mensajeEventosSD, 0);

			if (mensajeEventosSD.eventID == SD_EVENTO_FILE_OPENED)
			{
				if (reproductorMusica.estado == REPRODUCTOR_STOP)
				{
					mensajeSDControl.commandID = SD_CONTROL_READ_FILE;
					xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeSDControl, 0);

					mensajeVS1063Control.commandID = VS_CONTROL_PLAY;
					xQueueSendToBack(queueMsgDriverVS1063, (void*)&mensajeVS1063Control, 0);
				}
				else if (reproductorMusica.estado == REPRODUCTOR_PLAY || reproductorMusica.estado == REPRODUCTOR_PAUSE)
				{
					reproductorMusica.estado = REPRODUCTOR_STOPPING;
					reproductorMusica.archivoPendiente = 1;

					mensajeVS1063Control.commandID = VS_CONTROL_STOP;
					xQueueSendToBack(queueMsgDriverVS1063, (void*)&mensajeVS1063Control, 0);
				}

				// Se guarda el tamaño del archivo abierto, el cual va a ser usado para calcular
				// la duración de la canción, usando el bitrate
				reproductorMusica.filesSize = mensajeEventosSD.parametro;
				reproductorMusica.totalTime = reproductorMusica.filesSize / (32000 / 8);		// En segundos

			}
			else if (mensajeEventosSD.eventID == SD_EVENTO_END_OF_FILE)
			{
				reproductorMusica.estado = REPRODUCTOR_STOPPING;

				mensajeSDControl.commandID = SD_CONTROL_CLOSE_FILE;
				xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeSDControl, 0);

				mensajeVS1063Control.commandID = VS_CONTROL_STOP;
				xQueueSendToBack(queueMsgDriverVS1063, (void*)&mensajeVS1063Control, 0);
			}
			else if (mensajeEventosSD.eventID == SD_EVENTO_ERROR)
			{
				// Cuando se produce un error en alguna operación de la SD, se
				// muestra un mensaje que no se puede ocultar.
				mensajeViewManager.evento = SHOW_MESSAGE_BOX_BLOCKING;
				mensajeViewManager.objectID = MESSAGE_ERROR_SD;
				xQueueSendToBack(queueMsgViewManager, (void*)&mensajeViewManager, 0);
			}
			else if (mensajeEventosSD.eventID == SD_EVENTO_SCAN_COMPLETE)
			{
				glcdListas[0].posCursor = 0;
				glcdListas[0].posCursor_bk = 0;
				glcdListas[0].selectedItem = 0;
				glcdScrollBars[1].posicion = 0;
				glcdBotones[2].estado = 0;
				glcdBotones[3].estado = 0;

				mensajeViewManager.evento = CHANGE_VIEW;
				mensajeViewManager.objectID = VISTA_LISTA;
				xQueueSendToBack(queueMsgViewManager, (void*)&mensajeViewManager, 0);
			}
		}
		if (uxQueueMessagesWaiting(queueEventosVS1063))
		{
			xQueueReceive(queueEventosVS1063, (void*)&mensajeEventosVS1063, 0);

			if (mensajeEventosVS1063.eventID == VS_EVENTO_START_PLAYING)
			{
				if (reproductorMusica.estado == REPRODUCTOR_STOP)
				{
					// Se resetea el contador de playing time
					reproductorMusica.playingTime_ticks = 0;

					// Se muestra la vista de reproducción
					glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].posicion = 0;
					glcdTexBoxes[TEXTBOX_TITULO].texto[0] = '\0';
					glcdTexBoxes[TEXTBOX_ARTISTA].texto[0] = '\0';
					glcdTexBoxes[TEXTBOX_ALBUM].texto[0] = '\0';

					mensajeViewManager.evento = CHANGE_VIEW;
					mensajeViewManager.objectID = VISTA_REPRO;
					xQueueSendToBack(queueMsgViewManager, (void*)&mensajeViewManager, 0);
				}

				reproductorMusica.estado = REPRODUCTOR_PLAY;
			}
			else if (mensajeEventosVS1063.eventID == VS_EVENTO_STOP_PLAYING)
			{
				reproductorMusica.estado = REPRODUCTOR_STOP;

				if (reproductorMusica.archivoPendiente == 1)
				{
					reproductorMusica.archivoPendiente = 0;

					mensajeSDControl.commandID = SD_CONTROL_READ_FILE;
					xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeSDControl, 0);

					mensajeVS1063Control.commandID = VS_CONTROL_PLAY;
					xQueueSendToBack(queueMsgDriverVS1063, (void*)&mensajeVS1063Control, 0);
				}
			}
			else if (mensajeEventosVS1063.eventID == VS_EVENTO_PAUSE_PLAYING)
			{
				reproductorMusica.estado = REPRODUCTOR_PAUSE;
			}
			else if (mensajeEventosVS1063.eventID == VS_EVENTO_VOLUME_CHANGE)
			{
				reproductorMusica.volumen = mensajeEventosVS1063.parametro;
			}
			else if (mensajeEventosVS1063.eventID == VS_EVENTO_SONG_DATA)
			{
				vs1063GetSongData_titulo (reproductorMusica.titulo);
				vs1063GetSongData_artista (reproductorMusica.artista);
				vs1063GetSongData_album (reproductorMusica.album);
				vs1063GetSongData_anio (reproductorMusica.anio);

				// Hay nuevos datos de la canción que se deben transferir a los objetos GLCD que la muestran
				reproductorMusica.actualizarSongData = 1;
			}
		}



		vTaskDelayUntil(&ticker, 10/portTICK_RATE_MS);
	}
}
