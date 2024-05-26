#include "driverGLCD.h"

xQueueHandle queueMsgGLCD;

unsigned char rxBuffer[20];

void createDriverGLCD (fsmUART* periphUART)
{
	xTaskCreate(driverGLCD, (signed portCHAR*) "driver GLCD", 100, (void*)periphUART, tskIDLE_PRIORITY+1, NULL);

	queueMsgGLCD = xQueueCreate(45, sizeof (MensajeDriverGLCD));
}

void driverGLCD (void* uart)
{
	MensajeDriverGLCD mensaje;

	fsmUART* periphUART = (fsmUART*) uart;

	while (1)
	{
		xQueueReceive(queueMsgGLCD, (void *) &mensaje, portMAX_DELAY);

		if (!UARTIsTransmitting (periphUART->UARTn))
		{
			if (fsmUARTGetTxFreeSpace(periphUART) >= mensaje.longitud)
			{
				fsmUARTPutTxData(periphUART, mensaje.payload, mensaje.longitud);

				// Se espera la respuesta desde el GLCD. Hasta que no se recibe, la tarea cede su timeslice.
				while (fsmUARTGetRxWaitingBytes(periphUART) < 3)
				{
					taskYIELD();
				}
				fsmUARTGetRxData(periphUART, rxBuffer, 3);		// Se sacan del buffer de recepción la respuesta del GLCD
			}
			else
			{
				taskYIELD();
			}
		}
		else
		{
			taskYIELD();
		}
	}
}


unsigned char glcdSerialScollLine (fsmUART* fsmStruct, unsigned char linea)
{
	MensajeDriverGLCD slot;

	slot.longitud = 3;
	slot.payload[0] = CMD_SCROLL_LINE;
	slot.payload[1] = 1;
	slot.payload[2] = linea;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialClearLine (fsmUART* fsmStruct, unsigned char linea)
{
	MensajeDriverGLCD slot;

	slot.longitud = 3;
	slot.payload[0] = CMD_CLEAR_LINE;
	slot.payload[1] = 1;
	slot.payload[2] = linea;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialClearScreen (fsmUART* fsmStruct)
{
	MensajeDriverGLCD slot;

	slot.longitud = 2;
	slot.payload[0] = CMD_CLEAR_SCREEN;
	slot.payload[1] = 0;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}


unsigned char glcdSerialRender (fsmUART* fsmStruct)
{
	MensajeDriverGLCD slot;

	slot.longitud = 2;
	slot.payload[0] = CMD_PRINT_BUFF;
	slot.payload[1] = 0;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialPunto (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char color)
{
	MensajeDriverGLCD slot;

	slot.longitud = 5;
	slot.payload[0] = CMD_PUNTO;
	slot.payload[1] = 3;
	slot.payload[2] = x0;
	slot.payload[3] = y0;
	slot.payload[4] = color;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialLinea (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color)
{
	MensajeDriverGLCD slot;

	slot.longitud = 7;
	slot.payload[0] = CMD_LINEA;
	slot.payload[1] = 5;
	slot.payload[2] = x0;
	slot.payload[3] = y0;
	slot.payload[4] = x1;
	slot.payload[5] = y1;
	slot.payload[6] = color;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialCirculo (fsmUART* fsmStruct, unsigned char centroX, unsigned char centroY, unsigned char radio, unsigned char color, unsigned char relleno)
{
	MensajeDriverGLCD slot;

	slot.longitud = 7;
	slot.payload[0] = CMD_CIRCULO;
	slot.payload[1] = 5;
	slot.payload[2] = centroX;
	slot.payload[3] = centroY;
	slot.payload[4] = radio;
	slot.payload[5] = color;
	slot.payload[6] = relleno;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialRectangulo (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color, unsigned char relleno)
{
	MensajeDriverGLCD slot;

	slot.longitud = 8;
	slot.payload[0] = CMD_RECTANGULO;
	slot.payload[1] = 6;
	slot.payload[2] = x0;
	slot.payload[3] = y0;
	slot.payload[4] = x1;
	slot.payload[5] = y1;
	slot.payload[6] = color;
	slot.payload[7] = relleno;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialBorrarArea (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1)
{
	MensajeDriverGLCD slot;

	slot.longitud = 6;
	slot.payload[0] = CMD_BORRAR_AREA;
	slot.payload[1] = 4;
	slot.payload[2] = x0;
	slot.payload[3] = y0;
	slot.payload[4] = x1;
	slot.payload[5] = y1;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialPrintCaracter (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char caracter, unsigned char color)
{
	MensajeDriverGLCD slot;

	slot.longitud = 6;
	slot.payload[0] = CMD_PRINT_CARACTER;
	slot.payload[1] = 4;
	slot.payload[2] = x0;
	slot.payload[3] = y0;
	slot.payload[4] = caracter;
	slot.payload[5] = color;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdSerialPrintf (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char* texto, unsigned char color)
{
	MensajeDriverGLCD slot;
	unsigned int i;

	slot.payload[0] = CMD_PRINTF;

	slot.payload[2] = x0;
	slot.payload[3] = y0;
	for (i = 0; *texto != '\0'; i ++)
	{
		slot.payload[4+i] = *texto;
		texto++;
	}
	slot.payload[4+i] = '\0';
	slot.payload[4+i+1] = color;

	slot.longitud = (i+1) + 5;
	slot.payload[1] = slot.longitud - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdAgregarBotones (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdBoton* botones)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;

	slot.payload[0] = CMD_AGREGAR_BOTONES;

	slot.payload[2] = cantidadObjetos;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];
	for (i = 0; i < cantidadObjetos; i++)
	{
		*ptrPayload = botones[i].x0; ptrPayload++; longitudPayload++;
		*ptrPayload = botones[i].y0; ptrPayload++; longitudPayload++;
		*ptrPayload = botones[i].w; ptrPayload++; longitudPayload++;
		*ptrPayload = botones[i].h; ptrPayload++; longitudPayload++;
		*ptrPayload = botones[i].xTexto; ptrPayload++; longitudPayload++;
		*ptrPayload = botones[i].yTexto; ptrPayload++; longitudPayload++;
		for (j = 0; botones[i].texto[j] != '\0'; j++)
		{
			*ptrPayload = botones[i].texto[j]; ptrPayload++; longitudPayload++;
		}
		*ptrPayload = '\0'; ptrPayload++; longitudPayload++;

	}

	slot.longitud = longitudPayload;

	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdAgregarBarraProgreso (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdBarraProgreso* barras)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;

	slot.payload[0] = CMD_AGREGAR_BARRAS_PROGRESO;

	slot.payload[2] = cantidadObjetos;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];
	for (i = 0; i < cantidadObjetos; i++)
	{
		*ptrPayload = barras[i].x0; ptrPayload++; longitudPayload++;
		*ptrPayload = barras[i].y0; ptrPayload++; longitudPayload++;
		*ptrPayload = barras[i].w; ptrPayload++; longitudPayload++;
		for (j = 0; barras[i].texto[j] != '\0'; j++)
		{
			*ptrPayload = barras[i].texto[j]; ptrPayload++; longitudPayload++;
		}
		*ptrPayload = '\0'; ptrPayload++; longitudPayload++;

	}

	slot.longitud = longitudPayload;

	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdAgregarIndicadorVariable (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdIndicadorVariable* indicadores)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;

	slot.payload[0] = CMD_AGREGAR_INDICADOR_VARIABLE;

	slot.payload[2] = cantidadObjetos;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];
	for (i = 0; i < cantidadObjetos; i++)
	{
		*ptrPayload = indicadores[i].x0; ptrPayload++; longitudPayload++;
		*ptrPayload = indicadores[i].y0; ptrPayload++; longitudPayload++;
		*ptrPayload = indicadores[i].w; ptrPayload++; longitudPayload++;
		*ptrPayload = indicadores[i].borde; ptrPayload++; longitudPayload++;

		for (j = 0; indicadores[i].nombre[j] != '\0'; j++)
		{
			*ptrPayload = indicadores[i].nombre[j]; ptrPayload++; longitudPayload++;
		}
		*ptrPayload = '\0'; ptrPayload++; longitudPayload++;

		for (j = 0; indicadores[i].unidad[j] != '\0'; j++)
		{
			*ptrPayload = indicadores[i].unidad[j]; ptrPayload++; longitudPayload++;
		}
		*ptrPayload = '\0'; ptrPayload++; longitudPayload++;

	}

	slot.longitud = longitudPayload;

	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdAgregarGrafico (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdGrafico* graficos)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;
	unsigned char* ptrDato;

	slot.payload[0] = CMD_AGREGAR_GRAFICOS;

	slot.payload[2] = cantidadObjetos;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];
	for (i = 0; i < cantidadObjetos; i++)
	{
		*ptrPayload = graficos[i].graficos[0]; ptrPayload++; longitudPayload++;
		*ptrPayload = graficos[i].graficos[1]; ptrPayload++; longitudPayload++;
		*ptrPayload = graficos[i].graficos[2]; ptrPayload++; longitudPayload++;
		*ptrPayload = graficos[i].graficos[3]; ptrPayload++; longitudPayload++;
		*ptrPayload = graficos[i].x0; ptrPayload++; longitudPayload++;
		*ptrPayload = graficos[i].y0; ptrPayload++; longitudPayload++;
		*ptrPayload = graficos[i].w; ptrPayload++; longitudPayload++;
		*ptrPayload = graficos[i].h; ptrPayload++; longitudPayload++;

		ptrDato = (unsigned char*)&graficos[i].rangoVertical;
		for (j = 0; j < 4; j++)
		{
			*ptrPayload = *(ptrDato + 3 - j); ptrPayload++; longitudPayload++;
		}
		*ptrPayload = graficos[i].grilla; ptrPayload++; longitudPayload++;

	}

	slot.longitud = longitudPayload;

	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdAgregarSeries (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdSerie* series)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;
	unsigned char* ptrDato;

	slot.payload[0] = CMD_AGREGAR_SERIES;

	slot.payload[2] = cantidadObjetos;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];
	for (i = 0; i < cantidadObjetos; i++)
	{
		*ptrPayload = series[i].offset; ptrPayload++; longitudPayload++;

	}

	slot.longitud = longitudPayload;

	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdAgregarScrollBar (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdScrollBar* barras)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;
	unsigned char* ptrDato;

	slot.payload[0] = CMD_AGREGAR_SCROLL_BARS;

	slot.payload[2] = cantidadObjetos;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];
	for (i = 0; i < cantidadObjetos; i++)
	{
		ptrDato = (unsigned char*)&barras[i].max;
		for (j = 0; j < 2; j++)
		{
			*ptrPayload = *(ptrDato + 1 - j); ptrPayload++; longitudPayload++;
		}

		*ptrPayload = barras[i].w; ptrPayload++; longitudPayload++;
		*ptrPayload = barras[i].h; ptrPayload++; longitudPayload++;
		*ptrPayload = barras[i].vertical; ptrPayload++; longitudPayload++;
		*ptrPayload = barras[i].x0; ptrPayload++; longitudPayload++;
		*ptrPayload = barras[i].y0; ptrPayload++; longitudPayload++;

	}

	slot.longitud = longitudPayload;

	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}


unsigned char glcdActualizarBoton (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char nuevoEstado, unsigned char actualizar)
{
	MensajeDriverGLCD slot;

	slot.longitud = 5;
	slot.payload[0] = CMD_BOTON_ACTUALIZAR;
	slot.payload[1] = 3;
	slot.payload[2] = idObjeto;
	slot.payload[3] = nuevoEstado;
	slot.payload[4] = actualizar;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdActualizarBarraProgreso (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char porcentaje, unsigned char actualizar)
{
	MensajeDriverGLCD slot;

	slot.longitud = 5;
	slot.payload[0] = CMD_BARRA_ACTUALIZAR;
	slot.payload[1] = 3;
	slot.payload[2] = idObjeto;
	slot.payload[3] = porcentaje;
	slot.payload[4] = actualizar;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdActualizarIndicadorVariable (fsmUART* fsmStruct, unsigned char idObjeto, float nuevoValor, unsigned char actualizar)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;
	unsigned char* ptrDato;

	slot.payload[0] = CMD_INIDICADOR_ACTUALIZAR;
	slot.payload[2] = idObjeto;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];

	ptrDato = (unsigned char*)&nuevoValor;
	for (j = 0; j < 4; j++)
	{
		*ptrPayload = *(ptrDato + 3 - j); ptrPayload++; longitudPayload++;
	}

	*ptrPayload = actualizar; ptrPayload++; longitudPayload++;


	slot.longitud = longitudPayload;
	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdActualizarGrafico (fsmUART* fsmStruct, unsigned char idObjeto)
{
	MensajeDriverGLCD slot;

	slot.longitud = 3;
	slot.payload[0] = CMD_GRAFICO_ACTUALIZAR;
	slot.payload[1] = 1;
	slot.payload[2] = idObjeto;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdAgregarPuntoSerie (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char numeroSerie, signed char nuevoValor)
{
	MensajeDriverGLCD slot;

	slot.longitud = 5;
	slot.payload[0] = CMD_AGREGAR_PUNTO_SERIE;
	slot.payload[1] = 3;
	slot.payload[2] = idObjeto;
	slot.payload[3] = numeroSerie;
	slot.payload[4] = nuevoValor;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdActualizarScrollBar (fsmUART* fsmStruct, unsigned char idObjeto, unsigned int nuevoValor, unsigned char actualizar)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;
	unsigned char* ptrDato;

	slot.payload[0] = CMD_SCROLL_ACTUALIZAR;
	slot.payload[2] = idObjeto;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];

	ptrDato = (unsigned char*)&nuevoValor;
	for (j = 0; j < 2; j++)
	{
		*ptrPayload = *(ptrDato + 1 - j); ptrPayload++; longitudPayload++;
	}

	*ptrPayload = actualizar; ptrPayload++; longitudPayload++;


	slot.longitud = longitudPayload;
	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

unsigned char glcdDibujarIcono (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char x0, unsigned char y0)
{
	MensajeDriverGLCD slot;

	slot.longitud = 5;
	slot.payload[0] = CMD_DIBUJAR_ICONO;
	slot.payload[1] = 3;
	slot.payload[2] = idObjeto;
	slot.payload[3] = x0;
	slot.payload[4] = y0;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}

void glcdDibujarMessageBox (fsmUART* fsmStruct, glcdMessageBox* mensaje)
{
	glcdSerialBorrarArea (fsmStruct, mensaje->x0, mensaje->y0, mensaje->x0 + mensaje->w, mensaje->y0 + 20);
	glcdSerialRectangulo (fsmStruct, mensaje->x0 + 1, mensaje->y0 + 1, mensaje->x0 + mensaje->w - 1, mensaje->y0 + 20, 1, 0);
	glcdDibujarIcono (fsmStruct, mensaje->idIcono, mensaje->x0 + 3, mensaje->y0 + 3);
	glcdSerialPrintf (fsmStruct, mensaje->x0 + 22, mensaje->y0 + 8, mensaje->texto, 1);
}

void glcdActualizarIconMenu (fsmUART* fsmStruct, glcdIconMenu* menu, unsigned char actualizar)
{
	unsigned int cantidadCaracteres;
	unsigned char x0;
	unsigned int i;

	if (actualizar == 0)
	{
		glcdSerialBorrarArea (fsmStruct, 0, 0, 127, 63);

		for (cantidadCaracteres = 0; menu->titulo[cantidadCaracteres] != '\0'; cantidadCaracteres++);
		x0 = 64 - (2 * cantidadCaracteres);
		glcdSerialPrintf (fsmStruct, x0, 1, menu->titulo, 1);
		glcdSerialLinea (fsmStruct, 0, 8, 127, 8, 1);

	}
	else
	{
		glcdSerialBorrarArea (fsmStruct, 0, 10, 127, 50);
		glcdSerialBorrarArea (fsmStruct, 0, 58, 127, 63);
	}

	glcdSerialLinea (fsmStruct, 0, 55, 127, 55, 1);
	for (cantidadCaracteres = 0; menu->opciones[menu->selectedItem][cantidadCaracteres] != '\0'; cantidadCaracteres++);
	x0 = 64 - (2 * cantidadCaracteres);
	glcdSerialPrintf (fsmStruct, x0, 58, menu->opciones[menu->selectedItem], 1);

	glcdDibujarIcono (fsmStruct, menu->iconosID[menu->selectedItem], 54, 24);
	glcdSerialRectangulo (fsmStruct, 51, 21, 72, 42, 1, 0);
	if (menu->selectedItem >= 1)
	{
		glcdDibujarIcono (fsmStruct, menu->iconosID[menu->selectedItem - 1], 29, 24);
	}
	if (menu->selectedItem >= 2)
	{
		glcdDibujarIcono (fsmStruct, menu->iconosID[menu->selectedItem - 2], 4, 24);
	}

	if (menu->selectedItem <= (menu->cantidadIconos - 2))
	{
		glcdDibujarIcono (fsmStruct, menu->iconosID[menu->selectedItem + 1], 79, 24);
	}
	if (menu->selectedItem <= (menu->cantidadIconos - 3))
	{
		glcdDibujarIcono (fsmStruct, menu->iconosID[menu->selectedItem + 2], 104, 24);
	}
}

void glcdActualizarListBox (fsmUART* fsmStruct, glcdListBox* listBox, unsigned char actualizar)
{
	unsigned int i, j;
	unsigned char buffAux[40];

	if (actualizar == 0)
	{
		glcdSerialBorrarArea (fsmStruct, listBox->x0, listBox->y0, listBox->cantCaracteresMostrados * 4, listBox->cantLineasMostradas * 6 - 1 + 4);
		if (listBox->borde == 1)
			glcdSerialRectangulo (fsmStruct, listBox->x0, listBox->y0, listBox->cantCaracteresMostrados * 4, listBox->cantLineasMostradas * 6 - 1 + 4, 1, 0);


	}

	if (listBox->posCursor == listBox->posCursor_bk)	// No se movio solamente el cursor sino que se desplazan todas las
														// lineas o para arriba o para abajo.
	{
		glcdSerialBorrarArea (fsmStruct, listBox->x0 + 1, listBox->y0 + 1, listBox->cantCaracteresMostrados * 4 - 1, listBox->cantLineasMostradas * 6 - 1 + 4 - 1);

		if (listBox->posCursor == 0)		// Se dezplaza para abajo
		{
			glcdSerialRectangulo (fsmStruct, listBox->x0 + 1, listBox->y0 + 1, listBox->cantCaracteresMostrados * 4 - 1, listBox->y0 + 1 + 6, 1, 1);
			strncpy(buffAux, listBox->lineas[listBox->selectedItem].texto, listBox->cantCaracteresMostrados - 1);
			buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
			glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2, buffAux, 0);

			for (i = listBox->selectedItem + 1, j = 1; listBox->lineas[i].tipo != -1 && j < listBox->cantLineasMostradas && i < listBox->cantidadLineas; i++, j++)
			{
				strncpy(buffAux, listBox->lineas[i].texto, listBox->cantCaracteresMostrados - 1);
				buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
				glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2 + 6 * j, buffAux, 1);
			}
		}

		if (listBox->posCursor == (listBox->cantLineasMostradas - 1))		// Se dezplaza para arriba
		{
			glcdSerialRectangulo (fsmStruct, listBox->x0 + 1, listBox->y0 + 1 + 6 * (listBox->cantLineasMostradas - 1), listBox->cantCaracteresMostrados * 4 - 1, listBox->y0 + 1 + 6 * listBox->cantLineasMostradas, 1, 1);
			strncpy(buffAux, listBox->lineas[listBox->selectedItem].texto, listBox->cantCaracteresMostrados - 1);
			buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
			glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2 + 6 * (listBox->cantLineasMostradas - 1), buffAux, 0);

			for (i = listBox->selectedItem - 1, j = listBox->cantLineasMostradas - 2; listBox->lineas[i].tipo != -1 && j < listBox->cantLineasMostradas  && i < listBox->cantidadLineas; i--, j--)
			{
				strncpy(buffAux, listBox->lineas[i].texto, listBox->cantCaracteresMostrados - 1);
				buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
				glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2 + 6 * j, buffAux, 1);
			}
		}
	}
	else		// Se tiene que mover solamente el cursor desde posCursor_bk a posCursor
	{
		if (listBox->posCursor < listBox->posCursor_bk)
		{
			glcdSerialBorrarArea (fsmStruct, listBox->x0 + 1, listBox->y0 + 1 + 6 * listBox->posCursor, listBox->cantCaracteresMostrados * 4 - 1, listBox->y0 + 1 + 6 * listBox->posCursor + 6 + 6);
			glcdSerialRectangulo (fsmStruct, listBox->x0 + 1, listBox->y0 + 1 + 6 * listBox->posCursor, listBox->cantCaracteresMostrados * 4 - 1,  listBox->y0 + 1 + 6 * listBox->posCursor + 6, 1, 1);
			strncpy(buffAux, listBox->lineas[listBox->selectedItem].texto, listBox->cantCaracteresMostrados - 1);
			buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
			glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2 + 6 * listBox->posCursor, buffAux, 0);
			strncpy(buffAux, listBox->lineas[listBox->selectedItem + 1].texto, listBox->cantCaracteresMostrados - 1);
			buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
			glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2 + 6 * listBox->posCursor_bk, buffAux, 1);
		}
		else
		{
			glcdSerialBorrarArea (fsmStruct, listBox->x0 + 1, listBox->y0 + 1 + 6 * listBox->posCursor_bk, listBox->cantCaracteresMostrados * 4 - 1, listBox->y0 + 1 + 6 * listBox->posCursor_bk + 6 + 6);
			glcdSerialRectangulo (fsmStruct, listBox->x0 + 1, listBox->y0 + 1 + 6 * listBox->posCursor, listBox->cantCaracteresMostrados * 4 - 1,  listBox->y0 + 1 + 6 * listBox->posCursor + 6, 1, 1);
			strncpy(buffAux, listBox->lineas[listBox->selectedItem].texto, listBox->cantCaracteresMostrados - 1);
			buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
			glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2 + 6 * listBox->posCursor, buffAux, 0);
			strncpy(buffAux, listBox->lineas[listBox->selectedItem - 1].texto, listBox->cantCaracteresMostrados - 1);
			buffAux[listBox->cantCaracteresMostrados - 1] = '\0';
			glcdSerialPrintf (fsmStruct, listBox->x0 + 2, listBox->y0 + 2 + 6 * listBox->posCursor_bk, buffAux, 1);
		}

	}
}

void glcdActualizarTextBox (fsmUART* fsmStruct, glcdTextBox* textBox, unsigned char actualizar)
{
	unsigned int caracteresTexto;

	for (caracteresTexto = 0; textBox->texto[caracteresTexto] != '\0'; caracteresTexto++);

	glcdSerialBorrarArea (fsmStruct, textBox->x0, textBox->y0, textBox->x0 + 4 * caracteresTexto, textBox->y0 + 7);

	if (textBox->borde == 1)
		glcdSerialRectangulo (fsmStruct, textBox->x0, textBox->y0, textBox->x0 + 4 * caracteresTexto, textBox->y0 + 7, 1, 0);

	glcdSerialPrintf (fsmStruct, textBox->x0 + 1, textBox->y0 + 1, textBox->texto, 1);
}


void glcdActualizarBotonera (fsmUART* fsmStruct, glcdBotonera* botonera, unsigned char actualizar)
{
	unsigned int i;

	if (actualizar == 0)
	{
		for (i = 0; i < botonera->cantBotones; i++)
		{
			if (i == botonera->selectedButton)
				glcdActualizarBoton(fsmStruct, botonera->idBotones[i], 1, 0);
			else
				glcdActualizarBoton(fsmStruct, botonera->idBotones[i], 0, 0);
		}
	}
	else
	{
		glcdActualizarBoton(fsmStruct, botonera->idBotones[botonera->selectedButton_bk], 0, 1);
		glcdActualizarBoton(fsmStruct, botonera->idBotones[botonera->selectedButton], 1, 1);
	}
}


void glcdModificarScrollBarMax (fsmUART* fsmStruct, unsigned char idObjeto, unsigned int nuevoMaximo)
{
	MensajeDriverGLCD slot;
	unsigned int i, j;
	unsigned char* ptrPayload;
	unsigned char longitudPayload;
	unsigned char* ptrDato;

	slot.payload[0] = CMD_MODIF_SCROLL_BAR_MAX;
	slot.payload[2] = idObjeto;
	longitudPayload = 3;

	ptrPayload = &slot.payload[3];

	ptrDato = (unsigned char*)&nuevoMaximo;
	for (j = 0; j < 2; j++)
	{
		*ptrPayload = *(ptrDato + 1 - j); ptrPayload++; longitudPayload++;
	}


	slot.longitud = longitudPayload;
	slot.payload[1] = longitudPayload - 2;

	xQueueSendToBack(queueMsgGLCD, (void*)&slot, 0);

	return 0;
}
