#include "view_manager.h"


xQueueHandle queueMsgViewManager;

unsigned int vistaActual;
unsigned int vistaAnterior;

void initViewsAndObjects (void)
{
	unsigned int i;

//**************** BOTONES **************************************************

	glcdBotones[BOTON_ARRIBA_VISTA1].x0 = 0;
	glcdBotones[BOTON_ARRIBA_VISTA1].y0 = 0;
	glcdBotones[BOTON_ARRIBA_VISTA1].w = 6;
	glcdBotones[BOTON_ARRIBA_VISTA1].h = 6;
	glcdBotones[BOTON_ARRIBA_VISTA1].xTexto = 2;
	glcdBotones[BOTON_ARRIBA_VISTA1].yTexto = 1;
	strcpy(glcdBotones[BOTON_ARRIBA_VISTA1].texto, "+");
	glcdBotones[BOTON_ARRIBA_VISTA1].estado = 0;

	glcdBotones[BOTON_ABAJO_VISTA1].x0 = 0;
	glcdBotones[BOTON_ABAJO_VISTA1].y0 = 8;
	glcdBotones[BOTON_ABAJO_VISTA1].w = 6;
	glcdBotones[BOTON_ABAJO_VISTA1].h = 6;
	glcdBotones[BOTON_ABAJO_VISTA1].xTexto = 2;
	glcdBotones[BOTON_ABAJO_VISTA1].yTexto = 1;
	strcpy(glcdBotones[BOTON_ABAJO_VISTA1].texto, "-");
	glcdBotones[BOTON_ABAJO_VISTA1].estado = 0;

	glcdBotones[BOTON_PLAY_VISTA_REPRO].x0 = 50;
	glcdBotones[BOTON_PLAY_VISTA_REPRO].y0 = 50;
	glcdBotones[BOTON_PLAY_VISTA_REPRO].w = 8;
	glcdBotones[BOTON_PLAY_VISTA_REPRO].h = 8;
	glcdBotones[BOTON_PLAY_VISTA_REPRO].xTexto = 3;
	glcdBotones[BOTON_PLAY_VISTA_REPRO].yTexto = 2;
	strcpy(glcdBotones[BOTON_PLAY_VISTA_REPRO].texto, ">");
	glcdBotones[BOTON_PLAY_VISTA_REPRO].estado = 0;

	glcdBotones[BOTON_PAUSA_VISTA_REPRO].x0 = 60;
	glcdBotones[BOTON_PAUSA_VISTA_REPRO].y0 = 50;
	glcdBotones[BOTON_PAUSA_VISTA_REPRO].w = 8;
	glcdBotones[BOTON_PAUSA_VISTA_REPRO].h = 8;
	glcdBotones[BOTON_PAUSA_VISTA_REPRO].xTexto = 2;
	glcdBotones[BOTON_PAUSA_VISTA_REPRO].yTexto = 2;
	strcpy(glcdBotones[BOTON_PAUSA_VISTA_REPRO].texto, "&");
	glcdBotones[BOTON_PAUSA_VISTA_REPRO].estado = 0;

	glcdBotones[BOTON_MENU_VISTA_REPRO].x0 = 70;
	glcdBotones[BOTON_MENU_VISTA_REPRO].y0 = 50;
	glcdBotones[BOTON_MENU_VISTA_REPRO].w = 8;
	glcdBotones[BOTON_MENU_VISTA_REPRO].h = 8;
	glcdBotones[BOTON_MENU_VISTA_REPRO].xTexto = 2;
	glcdBotones[BOTON_MENU_VISTA_REPRO].yTexto = 2;
	strcpy(glcdBotones[BOTON_MENU_VISTA_REPRO].texto, "M");
	glcdBotones[BOTON_MENU_VISTA_REPRO].estado = 0;

	glcdBotones[BOTON_ARRIBA_VISTA_LISTA].x0 = 121;
	glcdBotones[BOTON_ARRIBA_VISTA_LISTA].y0 = 0;
	glcdBotones[BOTON_ARRIBA_VISTA_LISTA].w = 6;
	glcdBotones[BOTON_ARRIBA_VISTA_LISTA].h = 6;
	glcdBotones[BOTON_ARRIBA_VISTA_LISTA].xTexto = 1;
	glcdBotones[BOTON_ARRIBA_VISTA_LISTA].yTexto = 1;
	strcpy(glcdBotones[BOTON_ARRIBA_VISTA_LISTA].texto, "{");
	glcdBotones[BOTON_ARRIBA_VISTA_LISTA].estado = 0;

	glcdBotones[BOTON_ABAJO_VISTA_LISTA].x0 = 121;
	glcdBotones[BOTON_ABAJO_VISTA_LISTA].y0 = 33;
	glcdBotones[BOTON_ABAJO_VISTA_LISTA].w = 6;
	glcdBotones[BOTON_ABAJO_VISTA_LISTA].h = 6;
	glcdBotones[BOTON_ABAJO_VISTA_LISTA].xTexto = 1;
	glcdBotones[BOTON_ABAJO_VISTA_LISTA].yTexto = 1;
	strcpy(glcdBotones[BOTON_ABAJO_VISTA_LISTA].texto, "|");
	glcdBotones[BOTON_ABAJO_VISTA_LISTA].estado = 0;

//*********************************************************************************


//**************** SCROLL BARS ****************************************************

	glcdScrollBars[SCROLL_BAR_VISTA1].max = 150;
	glcdScrollBars[SCROLL_BAR_VISTA1].posicion = 0;
	glcdScrollBars[SCROLL_BAR_VISTA1].w = 4;
	glcdScrollBars[SCROLL_BAR_VISTA1].h = 45;
	glcdScrollBars[SCROLL_BAR_VISTA1].vertical = 1;
	glcdScrollBars[SCROLL_BAR_VISTA1].x0 = 1;
	glcdScrollBars[SCROLL_BAR_VISTA1].y0 = 16;

	glcdScrollBars[SCROLL_BAR_VISTA_LISTA].max = 8;
	glcdScrollBars[SCROLL_BAR_VISTA_LISTA].posicion = 0;
	glcdScrollBars[SCROLL_BAR_VISTA_LISTA].w = 6;
	glcdScrollBars[SCROLL_BAR_VISTA_LISTA].h = 23;
	glcdScrollBars[SCROLL_BAR_VISTA_LISTA].vertical = 1;
	glcdScrollBars[SCROLL_BAR_VISTA_LISTA].x0 = 121;
	glcdScrollBars[SCROLL_BAR_VISTA_LISTA].y0 = 8;

	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].max = 100;
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].posicion = 0;
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].w = 75;
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].h = 4;
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].vertical = 0;
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].x0 = 25;
	glcdScrollBars[SCROLL_BAR_TIEMPO_REPRODUCCION].y0 = 34;

//*********************************************************************************


//**************** MESSAGE BOX ****************************************************

	glcdMensajes[MESSAGE_ERROR_SD].x0 = 15;
	glcdMensajes[MESSAGE_ERROR_SD].y0 = 20;
	glcdMensajes[MESSAGE_ERROR_SD].w = 90;
	glcdMensajes[MESSAGE_ERROR_SD].idIcono = ICONO_ADVERTENCIA;
	strcpy(glcdMensajes[MESSAGE_ERROR_SD].texto, "ERROR TARJETA SD");

//*********************************************************************************


//**************** ICON MENU ******************************************************

	strcpy(glcdMenuIconos[MENU_PRINCIPAL].titulo, "PRINCIPAL");
	glcdMenuIconos[MENU_PRINCIPAL].cantidadIconos = 3;
	glcdMenuIconos[MENU_PRINCIPAL].iconosID[0] = ICONO_CARPETA;
	glcdMenuIconos[MENU_PRINCIPAL].iconosID[1] = ICONO_PLAY;
	glcdMenuIconos[MENU_PRINCIPAL].iconosID[2] = ICONO_ERROR;
	strcpy(glcdMenuIconos[MENU_PRINCIPAL].opciones[0], "Explorar");
	strcpy(glcdMenuIconos[MENU_PRINCIPAL].opciones[1], "Reproducir");
	strcpy(glcdMenuIconos[MENU_PRINCIPAL].opciones[2], "Salir");
	glcdMenuIconos[MENU_PRINCIPAL].selectedItem = 0;

//*********************************************************************************

//**************** LIST BOX *******************************************************
	glcdListas[LISTA_ITEMS].borde = 1;
	glcdListas[LISTA_ITEMS].x0 = 0;
	glcdListas[LISTA_ITEMS].y0 = 0;
	glcdListas[LISTA_ITEMS].posCursor = 0;
	glcdListas[LISTA_ITEMS].posCursor_bk = 0;
	glcdListas[LISTA_ITEMS].selectedItem = 0;
	glcdListas[LISTA_ITEMS].cantCaracteresMostrados = 29;
	glcdListas[LISTA_ITEMS].cantLineasMostradas = 6;
	glcdListas[LISTA_ITEMS].lineas = glcdItemLista1;
	glcdListas[LISTA_ITEMS].cantidadLineas = 9;

	glcdItemLista1[0].tipo = 0;
	strcpy(glcdItemLista1[0].texto, "Item 1");
	glcdItemLista1[1].tipo = 0;
	strcpy(glcdItemLista1[1].texto, "Item 2");
	glcdItemLista1[2].tipo = 0;
	strcpy(glcdItemLista1[2].texto, "Item 3");
	glcdItemLista1[3].tipo = 0;
	strcpy(glcdItemLista1[3].texto, "Item 4");
	glcdItemLista1[4].tipo = 0;
	strcpy(glcdItemLista1[4].texto, "Item 5");
	glcdItemLista1[5].tipo = 0;
	strcpy(glcdItemLista1[5].texto, "Item 6");
	glcdItemLista1[6].tipo = 0;
	strcpy(glcdItemLista1[6].texto, "Item 7");
	glcdItemLista1[7].tipo = 0;
	strcpy(glcdItemLista1[7].texto, "Item 8");
	glcdItemLista1[8].tipo = 0;
	strcpy(glcdItemLista1[8].texto, "Item 9");
	glcdItemLista1[9].tipo = -1;

//*********************************************************************************

//**************** BOTONERAS ******************************************************

	glcdBotoneras[BOTONERA_REPRO].cantBotones = 3;
	glcdBotoneras[BOTONERA_REPRO].idBotones[0] = BOTON_PLAY_VISTA_REPRO;
	glcdBotoneras[BOTONERA_REPRO].idBotones[1] = BOTON_PAUSA_VISTA_REPRO;
	glcdBotoneras[BOTONERA_REPRO].idBotones[2] = BOTON_MENU_VISTA_REPRO;
	glcdBotoneras[BOTONERA_REPRO].selectedButton = 0;
	glcdBotoneras[BOTONERA_REPRO].selectedButton_bk = 0;

//*********************************************************************************


//**************** TEXT BOX *******************************************************

	glcdTexBoxes[TEXTBOX_TIEMPO_TRASNCURRIDO].borde = 0;
	glcdTexBoxes[TEXTBOX_TIEMPO_TRASNCURRIDO].x0 = 25;
	glcdTexBoxes[TEXTBOX_TIEMPO_TRASNCURRIDO].y0 = 40;
	strcpy(glcdTexBoxes[TEXTBOX_TIEMPO_TRASNCURRIDO].texto, "0:00");

	glcdTexBoxes[TEXTBOX_TIEMPO_TOTAL].borde = 0;
	glcdTexBoxes[TEXTBOX_TIEMPO_TOTAL].x0 = 85;
	glcdTexBoxes[TEXTBOX_TIEMPO_TOTAL].y0 = 40;
	strcpy(glcdTexBoxes[TEXTBOX_TIEMPO_TOTAL].texto, "0:00");

	glcdTexBoxes[TEXTBOX_TITULO].borde = 0;
	glcdTexBoxes[TEXTBOX_TITULO].x0 = 64;
	glcdTexBoxes[TEXTBOX_TITULO].y0 = 5;
	glcdTexBoxes[TEXTBOX_TITULO].texto[0] = '\0';

	glcdTexBoxes[TEXTBOX_ARTISTA].borde = 0;
	glcdTexBoxes[TEXTBOX_ARTISTA].x0 = 64;
	glcdTexBoxes[TEXTBOX_ARTISTA].y0 = 15;
	glcdTexBoxes[TEXTBOX_ARTISTA].texto[0] = '\0';

	glcdTexBoxes[TEXTBOX_ALBUM].borde = 0;
	glcdTexBoxes[TEXTBOX_ALBUM].x0 = 64;
	glcdTexBoxes[TEXTBOX_ALBUM].y0 = 25;
	glcdTexBoxes[TEXTBOX_ALBUM].texto[0] = '\0';

//*********************************************************************************


//**************** VISTAS *********************************************************

	// Iniciazliación de los contenidos de las vistas
	for (i = 0; i < 20; i++)
	{
		viewsGLCD[VISTA_REPRO].objectID[i] = 0;
		viewsGLCD[VISTA_REPRO].objectType[i] = NINGUNO;
		viewsGLCD[VISTA_MENU].objectID[i] = 0;
		viewsGLCD[VISTA_MENU].objectType[i] = NINGUNO;
		viewsGLCD[VISTA_LISTA].objectID[i] = 0;
		viewsGLCD[VISTA_LISTA].objectType[i] = NINGUNO;
	}

	viewsGLCD[VISTA_MENU].objectID[0] = MENU_PRINCIPAL;
	viewsGLCD[VISTA_MENU].objectType[0] = ICON_MENU;

	viewsGLCD[VISTA_LISTA].objectID[0] = LISTA_ITEMS;
	viewsGLCD[VISTA_LISTA].objectType[0] = LIST_BOX;
	viewsGLCD[VISTA_LISTA].objectID[1] = BOTON_ARRIBA_VISTA_LISTA;
	viewsGLCD[VISTA_LISTA].objectType[1] = BOTON;
	viewsGLCD[VISTA_LISTA].objectID[2] = BOTON_ABAJO_VISTA_LISTA;
	viewsGLCD[VISTA_LISTA].objectType[2] = BOTON;
	viewsGLCD[VISTA_LISTA].objectID[3] = SCROLL_BAR_VISTA_LISTA;
	viewsGLCD[VISTA_LISTA].objectType[3] = SCROLL_BAR;

	viewsGLCD[VISTA_REPRO].objectID[0] = BOTON_ARRIBA_VISTA1;
	viewsGLCD[VISTA_REPRO].objectType[0] = BOTON;
	viewsGLCD[VISTA_REPRO].objectID[1] = BOTON_ABAJO_VISTA1;
	viewsGLCD[VISTA_REPRO].objectType[1] = BOTON;
	viewsGLCD[VISTA_REPRO].objectID[2] = SCROLL_BAR_VISTA1;
	viewsGLCD[VISTA_REPRO].objectType[2] = SCROLL_BAR;
	viewsGLCD[VISTA_REPRO].objectID[3] = BOTONERA_REPRO;
	viewsGLCD[VISTA_REPRO].objectType[3] = BOTONERA;
	viewsGLCD[VISTA_REPRO].objectID[4] = SCROLL_BAR_TIEMPO_REPRODUCCION;
	viewsGLCD[VISTA_REPRO].objectType[4] = SCROLL_BAR;
	viewsGLCD[VISTA_REPRO].objectID[5] = TEXTBOX_TIEMPO_TRASNCURRIDO;
	viewsGLCD[VISTA_REPRO].objectType[5] = TEXT_BOX;
	viewsGLCD[VISTA_REPRO].objectID[6] = TEXTBOX_TIEMPO_TOTAL;
	viewsGLCD[VISTA_REPRO].objectType[6] = TEXT_BOX;
	viewsGLCD[VISTA_REPRO].objectID[7] = TEXTBOX_TITULO;
	viewsGLCD[VISTA_REPRO].objectType[7] = TEXT_BOX;
	viewsGLCD[VISTA_REPRO].objectID[8] = TEXTBOX_ARTISTA;
	viewsGLCD[VISTA_REPRO].objectType[8] = TEXT_BOX;
	viewsGLCD[VISTA_REPRO].objectID[9] = TEXTBOX_ALBUM;
	viewsGLCD[VISTA_REPRO].objectType[9] = TEXT_BOX;

	vistaActual = VISTA_MENU;
	vistaAnterior = VISTA_MENU;

//*********************************************************************************


}

void createGLCDViewManager (fsmUART* periphUART)
{
	xTaskCreate(glcdViewManager, (signed portCHAR*) "View Manager", 100, (void*)periphUART, tskIDLE_PRIORITY+2, NULL);

	queueMsgViewManager = xQueueCreate(35, sizeof (MensajeColaViewManager));
}

void glcdViewManager(void* uart)
{
	fsmUART* periphUART = (fsmUART*)uart;
	unsigned int i;

	MensajeColaViewManager mensajeCola;

	if (CANT_GLCD_BOTONES)
		glcdAgregarBotones (&periphUART, CANT_GLCD_BOTONES, glcdBotones);

	if (CANT_GLCD_SCROLL_BARS)
		glcdAgregarScrollBar (&periphUART, CANT_GLCD_SCROLL_BARS, glcdScrollBars);

	while (1)
	{
		xQueueReceive(queueMsgViewManager, (void *) &mensajeCola, portMAX_DELAY);

		if (mensajeCola.evento == OBJECT_REFRESH)
		{
			if (mensajeCola.objectType == BOTON)
			{
				glcdActualizarBoton(periphUART, mensajeCola.objectID, glcdBotones[mensajeCola.objectID].estado, 1);
				glcdSerialRender(periphUART);
			}
			else if (mensajeCola.objectType == SCROLL_BAR)
			{
				glcdModificarScrollBarMax (periphUART, mensajeCola.objectID, glcdScrollBars[mensajeCola.objectID].max);
				glcdActualizarScrollBar (periphUART, mensajeCola.objectID, glcdScrollBars[mensajeCola.objectID].posicion, 1);
				glcdSerialRender(periphUART);
			}
			else if (mensajeCola.objectType == ICON_MENU)
			{
				glcdActualizarIconMenu (periphUART, &glcdMenuIconos[mensajeCola.objectID], 1);
				glcdSerialRender(periphUART);
			}
			else if (mensajeCola.objectType == LIST_BOX)
			{
				glcdActualizarListBox (periphUART, &glcdListas[mensajeCola.objectID], 1);
				glcdSerialRender(periphUART);
			}
			else if (mensajeCola.objectType == BOTONERA)
			{
				glcdActualizarBotonera (periphUART, &glcdBotoneras[mensajeCola.objectID], 1);
				glcdSerialRender(periphUART);
			}
			else if (mensajeCola.objectType == TEXT_BOX)
			{
				glcdActualizarTextBox (periphUART, &glcdTexBoxes[mensajeCola.objectID], 1);
				glcdSerialRender(periphUART);
			}
			// .......
		}

		else if (mensajeCola.evento == GENERAL_REFRESH)
		{
			unsigned int vistaAux;

			glcdSerialClearScreen (periphUART);

			vistaAux = vistaAnterior;
			vistaAnterior = vistaActual;
			vistaActual = vistaAux;

			for (i = 0; viewsGLCD[vistaActual].objectType[i] != NINGUNO; i++)
			{
				if (viewsGLCD[vistaActual].objectType[i] == BOTON)
				{
					glcdActualizarBoton(periphUART, viewsGLCD[vistaActual].objectID[i], glcdBotones[viewsGLCD[vistaActual].objectID[i]].estado, 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == SCROLL_BAR)
				{
					glcdModificarScrollBarMax (periphUART, viewsGLCD[vistaActual].objectID[i], glcdScrollBars[viewsGLCD[vistaActual].objectID[i]].max);
					glcdActualizarScrollBar (periphUART, viewsGLCD[vistaActual].objectID[i], glcdScrollBars[viewsGLCD[vistaActual].objectID[i]].posicion, 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == ICON_MENU)
				{
					glcdActualizarIconMenu (periphUART, &glcdMenuIconos[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == LIST_BOX)
				{
					glcdActualizarListBox (periphUART, &glcdListas[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == BOTONERA)
				{
					glcdActualizarBotonera (periphUART, &glcdBotoneras[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == TEXT_BOX)
				{
					glcdActualizarTextBox (periphUART, &glcdTexBoxes[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				// .......
			}

			glcdSerialRender(periphUART);
		}

		else if (mensajeCola.evento == CHANGE_VIEW)
		{
			glcdSerialClearScreen (periphUART);

			vistaAnterior = vistaActual;
			vistaActual = mensajeCola.objectID;

			for (i = 0; viewsGLCD[vistaActual].objectType[i] != NINGUNO; i++)
			{
				if (viewsGLCD[vistaActual].objectType[i] == BOTON)
				{
					glcdActualizarBoton(periphUART, viewsGLCD[vistaActual].objectID[i], glcdBotones[viewsGLCD[vistaActual].objectID[i]].estado, 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == SCROLL_BAR)
				{
					glcdModificarScrollBarMax (periphUART, viewsGLCD[vistaActual].objectID[i], glcdScrollBars[viewsGLCD[vistaActual].objectID[i]].max);
					glcdActualizarScrollBar (periphUART, viewsGLCD[vistaActual].objectID[i], glcdScrollBars[viewsGLCD[vistaActual].objectID[i]].posicion, 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == ICON_MENU)
				{
					glcdActualizarIconMenu (periphUART, &glcdMenuIconos[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == LIST_BOX)
				{
					glcdActualizarListBox (periphUART, &glcdListas[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == BOTONERA)
				{
					glcdActualizarBotonera (periphUART, &glcdBotoneras[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				else if (viewsGLCD[vistaActual].objectType[i] == TEXT_BOX)
				{
					glcdActualizarTextBox (periphUART, &glcdTexBoxes[viewsGLCD[vistaActual].objectID[i]], 0);
				}
				// .......
			}
			glcdSerialRender(periphUART);
		}

		else if (mensajeCola.evento == SHOW_MESSAGE_BOX)
		{
			vistaAnterior = vistaActual;
			vistaActual = VISTA_MESSAGE_DEFAULT;

			glcdDibujarMessageBox (periphUART, &glcdMensajes[mensajeCola.objectID]);

			glcdSerialRender(periphUART);
		}

		else if (mensajeCola.evento == SHOW_MESSAGE_BOX_BLOCKING)
		{
			vistaAnterior = vistaActual;
			vistaActual = VISTA_MESSAGE_DEFAULT_BLOCKING;

			glcdDibujarMessageBox (periphUART, &glcdMensajes[mensajeCola.objectID]);

			glcdSerialRender(periphUART);
		}

	}
}
