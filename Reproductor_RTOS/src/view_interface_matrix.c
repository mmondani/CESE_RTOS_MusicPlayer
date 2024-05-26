#include "view_interface_matrix.h"
#include "reproductor.h"

extern xQueueHandle queueMsgViewManager;
extern xQueueHandle queueMsgDriverSD;
extern xQueueHandle queueBufferLecturaSD;
extern xQueueHandle queueMsgDriverVS1063;
extern xQueueHandle queueEventosUsuario;

extern screenView viewsGLCD[];
extern glcdBoton glcdBotones[];
extern glcdScrollBar glcdScrollBars[];
extern glcdMessageBox glcdMensajes[];
extern glcdIconMenu glcdMenuIconos[];
extern glcdListBox glcdListas[];
extern glcdListBoxItem glcdItemLista1[];
extern glcdBotonera glcdBotoneras[];
extern glcdTextBox glcdTexBoxes[];


extern estructuraReproductor reproductorMusica;

void (*reaccionesSistema [CANTIDAD_VISTAS][CANTIDAD_EVENTOS])(unsigned char param) ={
																							{reaccion_NULL, reaccion_NULL, reaccion_vista_menu_botonIzquierda, reaccion_vista_menu_botonDerecha, reaccion_vista_menu_botonEnter, reaccion_NULL},
																							{reaccion_vista_lista_botonArriba, reaccion_vista_lista_botonAbajo, reaccion_vista_lista_botonIzquierda, reaccion_NULL, reaccion_vista_lista_botonEnter, reaccion_NULL},
																							{reaccion_vista_repro_botonArriba, reaccion_vista_repro_botonAbajo, reaccion_vista_repro_botonIzquierda, reaccion_vista_repro_botonDerecha, reaccion_vista_repro_botonEnter, reaccion_vista_repro_timerRefresh},
																							{reaccion_NULL, reaccion_NULL, reaccion_NULL, reaccion_NULL, reaccion_vista_message_default, reaccion_NULL},
																							{reaccion_NULL, reaccion_NULL, reaccion_NULL, reaccion_NULL, reaccion_NULL, reaccion_NULL}
																						};

int volumen = 60;

void reaccion_vista_repro_botonArriba (unsigned char param)
{
	MensajeColaViewManager mensajeCola;
	MensajeControlVS1063 mensajeControlVS1063;

	glcdBotones[0].estado = param;

	mensajeCola.evento = OBJECT_REFRESH;
	mensajeCola.objectID = BOTON_ARRIBA_VISTA1;
	mensajeCola.objectType = BOTON;

	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);


	if (param == 1)
	{
		if (volumen >= 5)
		{
			volumen -= 5;
			glcdScrollBars[0].posicion = volumen;

			mensajeControlVS1063.commandID = VS_CONTROL_CHG_VOLUME;
			mensajeControlVS1063.parametro = volumen;
			xQueueSendToBack(queueMsgDriverVS1063, (void*)&mensajeControlVS1063, 0);

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = SCROLL_BAR_VISTA1;
			mensajeCola.objectType = SCROLL_BAR;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_repro_botonAbajo (unsigned char param)
{
	MensajeColaViewManager mensajeCola;
	MensajeControlVS1063 mensajeControlVS1063;

	glcdBotones[1].estado = param;

	mensajeCola.evento = OBJECT_REFRESH;
	mensajeCola.objectID = BOTON_ABAJO_VISTA1;
	mensajeCola.objectType = BOTON;

	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);

	if (param == 1)
	{
		if (volumen <= (glcdScrollBars[0].max - 5))
		{
			volumen += 5;
			glcdScrollBars[0].posicion = volumen;

			mensajeControlVS1063.commandID = VS_CONTROL_CHG_VOLUME;
			mensajeControlVS1063.parametro = volumen;
			xQueueSendToBack(queueMsgDriverVS1063, (void*)&mensajeControlVS1063, 0);

			glcdScrollBars[0].posicion = volumen;

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = SCROLL_BAR_VISTA1;
			mensajeCola.objectType = SCROLL_BAR;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_repro_botonEnter (unsigned char param)
{
	MensajeColaViewManager mensajeCola;
	MensajeEventoUsuario mensajeEventosUsuario;

	if (param == 1)
	{
		if (glcdBotoneras[0].selectedButton == 0)
		{
			mensajeEventosUsuario.eventID = USER_EVENTO_BOTON_PLAY;
			xQueueSendToBack(queueEventosUsuario, (void*)&mensajeEventosUsuario, 0);
		}
		else if(glcdBotoneras[0].selectedButton == 1)
		{
			mensajeEventosUsuario.eventID = USER_EVENTO_BOTON_PAUSA;
			xQueueSendToBack(queueEventosUsuario, (void*)&mensajeEventosUsuario, 0);
		}
		else if(glcdBotoneras[0].selectedButton == 2)
		{
			mensajeCola.evento = CHANGE_VIEW;
			mensajeCola.objectID = VISTA_MENU;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_repro_botonIzquierda (unsigned char param)
{
	MensajeColaViewManager mensajeCola;

	if (param == 1)
	{
		if (glcdBotoneras[0].selectedButton > 0)
		{
			glcdBotoneras[0].selectedButton_bk = glcdBotoneras[0].selectedButton;
			glcdBotoneras[0].selectedButton --;

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = BOTONERA_REPRO;
			mensajeCola.objectType = BOTONERA;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_repro_botonDerecha (unsigned char param)
{
	MensajeColaViewManager mensajeCola;

	if (param == 1)
	{
		if (glcdBotoneras[0].selectedButton < (glcdBotoneras[0].cantBotones - 1))
		{
			glcdBotoneras[0].selectedButton_bk = glcdBotoneras[0].selectedButton;
			glcdBotoneras[0].selectedButton ++;

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = BOTONERA_REPRO;
			mensajeCola.objectType = BOTONERA;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_repro_timerRefresh (unsigned char param)
{
	unsigned int tiempoTranscurrido_segundos, tiempoTranscurrido_minutos;
	unsigned char buffAux[40];
	MensajeColaViewManager mensajeCola;

	// Se actualizan los objetos del GLCD relacionados con el tiempo transcurrido de reproducción

// TIEMPO TRANSCURRIDO
	// De milisegundos a segundos
	tiempoTranscurrido_segundos = reproductorMusica.playingTime_ticks / 1000;

	// Se obtienen los minutos enteros transcurridos
	tiempoTranscurrido_minutos = (tiempoTranscurrido_segundos * 100) / 60;
	tiempoTranscurrido_minutos /= 100;

	// Se obtienen los segundos transcurridos
	//tiempoTranscurrido_segundos = (tiempoTranscurrido_segundos * 100) / 60;
	tiempoTranscurrido_segundos -= (tiempoTranscurrido_minutos * 60);
	//tiempoTranscurrido_segundos = (tiempoTranscurrido_segundos * 60) / 100;

	sprintf(buffAux, "%02d:%02d", tiempoTranscurrido_minutos, tiempoTranscurrido_segundos);

	strcpy(glcdTexBoxes[TEXTBOX_TIEMPO_TRASNCURRIDO].texto, buffAux);

	mensajeCola.evento = OBJECT_REFRESH;
	mensajeCola.objectID = TEXTBOX_TIEMPO_TRASNCURRIDO;
	mensajeCola.objectType = TEXT_BOX;
	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);


// TIEMPO TOTAL (ya esta en segundos)
	// Se obtienen los minutos enteros transcurridos
	tiempoTranscurrido_segundos = reproductorMusica.totalTime;
	tiempoTranscurrido_minutos = (tiempoTranscurrido_segundos * 100) / 60;
	tiempoTranscurrido_minutos /= 100;

	// Se obtienen los segundos transcurridos
	//iempoTranscurrido_segundos = (tiempoTranscurrido_segundos * 100) / 60;
	tiempoTranscurrido_segundos -= (tiempoTranscurrido_minutos * 60);
	//tiempoTranscurrido_segundos = (tiempoTranscurrido_segundos * 60) / 100;

	sprintf(buffAux, "%02d:%02d", tiempoTranscurrido_minutos, tiempoTranscurrido_segundos);

	strcpy(glcdTexBoxes[TEXTBOX_TIEMPO_TOTAL].texto, buffAux);

	mensajeCola.evento = OBJECT_REFRESH;
	mensajeCola.objectID = TEXTBOX_TIEMPO_TOTAL;
	mensajeCola.objectType = TEXT_BOX;
	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);


// ACTUALIZACIÓN DEL SCROLL BAR
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].max = reproductorMusica.totalTime;
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].posicion = reproductorMusica.playingTime_ticks / 1000;

	mensajeCola.evento = OBJECT_REFRESH;
	mensajeCola.objectID = SCROLL_BAR_TIEMPO_REPRODUCCION;
	mensajeCola.objectType = SCROLL_BAR;
	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);


// ACTUALIZACION TEXTBOXES CON DATA DE LA CANCION
	unsigned char x0;
	unsigned int cantidadCaracteres;

	if (reproductorMusica.actualizarSongData == 1)
	{
		strcpy (glcdTexBoxes[TEXTBOX_TITULO].texto, reproductorMusica.titulo);
		for (cantidadCaracteres = 0; glcdTexBoxes[TEXTBOX_TITULO].texto[cantidadCaracteres] != '\0'; cantidadCaracteres++);
		x0 = 64 - (2 * cantidadCaracteres);
		glcdTexBoxes[TEXTBOX_TITULO].x0 = x0;

		mensajeCola.evento = OBJECT_REFRESH;
		mensajeCola.objectID = TEXTBOX_TITULO;
		mensajeCola.objectType = TEXT_BOX;
		xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);


		strcpy (glcdTexBoxes[TEXTBOX_ARTISTA].texto, reproductorMusica.artista);
		for (cantidadCaracteres = 0; glcdTexBoxes[TEXTBOX_ARTISTA].texto[cantidadCaracteres] != '\0'; cantidadCaracteres++);
		x0 = 64 - (2 * cantidadCaracteres);
		glcdTexBoxes[TEXTBOX_ARTISTA].x0 = x0;

		mensajeCola.evento = OBJECT_REFRESH;
		mensajeCola.objectID = TEXTBOX_ARTISTA;
		mensajeCola.objectType = TEXT_BOX;
		xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);


		strcpy (glcdTexBoxes[TEXTBOX_ALBUM].texto, reproductorMusica.album);
		for (cantidadCaracteres = 0; glcdTexBoxes[TEXTBOX_ALBUM].texto[cantidadCaracteres] != '\0'; cantidadCaracteres++);
		x0 = 64 - (2 * cantidadCaracteres);
		glcdTexBoxes[TEXTBOX_ALBUM].x0 = x0;

		mensajeCola.evento = OBJECT_REFRESH;
		mensajeCola.objectID = TEXTBOX_ALBUM;
		mensajeCola.objectType = TEXT_BOX;
		xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);

		reproductorMusica.actualizarSongData = 0;
	}
}

void reaccion_vista_message_default (unsigned char param)
{
	MensajeColaViewManager mensajeCola;


	if (param == 1)		// Chequeo que se haya apretado el boton y no que se haya soltado
	{
		mensajeCola.evento = GENERAL_REFRESH;

		xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
	}
}

void reaccion_vista_menu_botonIzquierda (unsigned char param)
{
	MensajeColaViewManager mensajeCola;

	if (param == 1)
	{
		if (glcdMenuIconos[0].selectedItem > 0)
		{
			glcdMenuIconos[0].selectedItem --;

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = MENU_PRINCIPAL;
			mensajeCola.objectType = ICON_MENU;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_menu_botonDerecha (unsigned char param)
{
	MensajeColaViewManager mensajeCola;

	if (param == 1)
	{
		if (glcdMenuIconos[0].selectedItem < (glcdMenuIconos[0].cantidadIconos - 1))
		{
			glcdMenuIconos[0].selectedItem ++;

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = MENU_PRINCIPAL;
			mensajeCola.objectType = ICON_MENU;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_menu_botonEnter (unsigned char param)
{
	MensajeColaViewManager mensajeCola;
	MensajeControlSD mensajeColaControlSD;


	if (param == 1)
	{
		if (glcdMenuIconos[0].selectedItem == 0)
		{
			mensajeColaControlSD.commandID = SD_CONTROL_SCAN_DIR;
			xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeColaControlSD, 0);


			glcdListas[0].posCursor = 0;
			glcdListas[0].posCursor_bk = 0;
			glcdListas[0].selectedItem = 0;
			glcdScrollBars[1].posicion = 0;


			mensajeCola.evento = CHANGE_VIEW;
			mensajeCola.objectID = VISTA_LISTA;
			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
		else if (glcdMenuIconos[0].selectedItem == 1)
		{
			mensajeCola.evento = CHANGE_VIEW;
			mensajeCola.objectID = VISTA_REPRO;

			glcdBotones[0].estado = 0;
			glcdBotones[1].estado = 0;

			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
		else if (glcdMenuIconos[0].selectedItem == 2)
		{

		}
	}
}

void reaccion_vista_lista_botonArriba (unsigned char param)
{
	MensajeColaViewManager mensajeCola;

	glcdBotones[2].estado = param;

	mensajeCola.evento = OBJECT_REFRESH;
	mensajeCola.objectID = BOTON_ARRIBA_VISTA_LISTA;
	mensajeCola.objectType = BOTON;
	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);

	if (param == 1)
	{
		glcdListas[0].posCursor_bk = glcdListas[0].posCursor;



		if (glcdListas[0].selectedItem > 0)
		{
			if (glcdListas[0].posCursor > 0)
				glcdListas[0].posCursor --;
			else
				glcdListas[0].posCursor = 0;

			glcdListas[0].selectedItem --;

			glcdScrollBars[1].posicion = glcdListas[0].selectedItem;

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = LISTA_ITEMS;
			mensajeCola.objectType = LIST_BOX;
			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = SCROLL_BAR_VISTA_LISTA;
			mensajeCola.objectType = SCROLL_BAR;
			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_lista_botonAbajo (unsigned char param)
{
	MensajeColaViewManager mensajeCola;

	glcdBotones[3].estado = param;

	mensajeCola.evento = OBJECT_REFRESH;
	mensajeCola.objectID = BOTON_ABAJO_VISTA_LISTA;
	mensajeCola.objectType = BOTON;
	xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);

	if (param == 1)
	{
		glcdListas[0].posCursor_bk = glcdListas[0].posCursor;

		if (glcdListas[0].selectedItem < glcdListas[0].cantidadLineas - 1)
		{
			if (glcdListas[0].posCursor < (glcdListas[0].cantLineasMostradas - 1))
				glcdListas[0].posCursor ++;
			else
				glcdListas[0].posCursor = glcdListas[0].cantLineasMostradas - 1;

			glcdListas[0].selectedItem ++;

			glcdScrollBars[1].posicion = glcdListas[0].selectedItem;

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = LISTA_ITEMS;
			mensajeCola.objectType = LIST_BOX;
			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);

			mensajeCola.evento = OBJECT_REFRESH;
			mensajeCola.objectID = SCROLL_BAR_VISTA_LISTA;
			mensajeCola.objectType = SCROLL_BAR;
			xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
		}
	}
}

void reaccion_vista_lista_botonIzquierda (unsigned char param)
{
	MensajeColaViewManager mensajeCola;

	if (param == 1)
	{
		mensajeCola.evento = CHANGE_VIEW;
		mensajeCola.objectID = VISTA_MENU;

		xQueueSendToBack(queueMsgViewManager, (void*)&mensajeCola, 0);
	}
}

void reaccion_vista_lista_botonEnter (unsigned char param)
{
	MensajeColaViewManager mensajeCola;
	MensajeControlSD mensajeColaControlSD;
	MensajeControlVS1063 mensajeControlVS1063;

	if (param == 1)
	{
		if (glcdListas[0].lineas[glcdListas[0].selectedItem].tipo == DIRECTORIO)
		{
			mensajeColaControlSD.commandID = SD_CONTROL_CHANGE_DIR;
			strcpy(mensajeColaControlSD.parametro, glcdListas[0].lineas[glcdListas[0].selectedItem].texto);
			xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeColaControlSD, 0);

			mensajeColaControlSD.commandID = SD_CONTROL_SCAN_DIR;
			xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeColaControlSD, 0);
		}
		else if (glcdListas[0].lineas[glcdListas[0].selectedItem].tipo == ARCHIVO)
		{
			mensajeColaControlSD.commandID = SD_CONTROL_OPEN_FILE;
			strcpy(mensajeColaControlSD.parametro, glcdListas[0].lineas[glcdListas[0].selectedItem].texto);
			xQueueSendToBack(queueMsgDriverSD, (void*)&mensajeColaControlSD, 0);
		}
	}
}


void reaccion_NULL (unsigned char param)
{
	// A esta función apuntan todas las combinaciones VISTA - INTERFAZ que no tienen que realizar
	// ninguna acción.
}
