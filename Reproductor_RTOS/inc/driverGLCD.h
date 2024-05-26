#ifndef DRIVERGLCD_H_
#define DRIVERGLCD_H_


// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "fsmUART.h"

//*************************************************************
// Elemento de la queue del driver GLCD

typedef struct
{
	unsigned int longitud;
	unsigned char payload[90];
}MensajeDriverGLCD;

//*************************************************************

//*************************************************************
// Objetos GLCD
typedef struct
{
	unsigned char x0, y0;
	unsigned char w, h;
	unsigned char xTexto, yTexto;
	unsigned char texto[5];
	unsigned char estado;
}glcdBoton;

typedef struct
{
	unsigned char x0, y0;
	unsigned char w;
	unsigned char texto[15];
	unsigned char porcentaje;
}glcdBarraProgreso;

typedef struct
{
	unsigned char x0, y0;
	unsigned char w;
	unsigned char borde;
	unsigned char nombre[8];
	signed char exponente;
	float valor;
	unsigned char unidad[3];
}glcdIndicadorVariable;

extern char submultiplos[];
extern char multiplos[];

typedef struct
{
	signed char y[100];
	unsigned char x;
	signed char offset;		// Valor de continua que desplaza verticalmente al gráfico
}glcdSerie;

typedef struct
{
	unsigned char graficos[4];
	unsigned char x0, y0;
	unsigned char w, h;
	float rangoVertical;
	unsigned char grilla;
}glcdGrafico;

typedef struct
{
	unsigned int max;
	unsigned int posicion;
	unsigned char w, h;
	unsigned char vertical;
	unsigned char x0, y0;
}glcdScrollBar;

typedef struct
{
	unsigned char w;
	unsigned char x0, y0;
	unsigned char idIcono;
	unsigned char texto[25];
}glcdMessageBox;

typedef struct
{
	unsigned char titulo[30];
	unsigned int cantidadIconos;
	unsigned char opciones[10][20];
	unsigned char iconosID[10];
	unsigned int selectedItem;
	//void (*callbacksOpciones[10]) (void);
}glcdIconMenu;

typedef struct
{
	int tipo;
	unsigned char texto[35];
}glcdListBoxItem;

typedef struct
{
	unsigned int borde;
	unsigned char x0, y0;
	unsigned int cantLineasMostradas;
	unsigned int cantCaracteresMostrados;
	unsigned int selectedItem;
	unsigned int posCursor, posCursor_bk;
	glcdListBoxItem* lineas;
	unsigned int cantidadLineas;
}glcdListBox;

typedef struct
{
	unsigned char x0, y0;
	unsigned int borde;
	unsigned char texto[40];
}glcdTextBox;

typedef struct
{
	unsigned int cantBotones;
	unsigned int idBotones[10];
	unsigned int selectedButton, selectedButton_bk;
}glcdBotonera;



//Iconos
enum {ICONO_PREGUNTA = 0, ICONO_ERROR, ICONO_ADVERTENCIA, ICONO_PLAY, ICONO_CARPETA};

//*************************************************************




//*************************************************************
// Command ID
#define CMD_SCROLL_LINE 				1
#define CMD_CLEAR_LINE 					2
#define CMD_CLEAR_SCREEN 				3
#define CMD_PUNTO 						4
#define CMD_LINEA 						5
#define CMD_CIRCULO 					6
#define CMD_RECTANGULO 					7
#define CMD_PRINT_CARACTER				8
#define CMD_BORRAR_AREA					9
#define CMD_PRINTF						10
#define CMD_PRINT_BUFF					11
#define CMD_AGREGAR_BOTONES				12
#define CMD_AGREGAR_BARRAS_PROGRESO		13
#define CMD_AGREGAR_INDICADOR_VARIABLE	14
#define CMD_AGREGAR_GRAFICOS			15
#define CMD_AGREGAR_SERIES				16
#define CMD_AGREGAR_SCROLL_BARS			17
#define CMD_BOTON_ACTUALIZAR			18
#define CMD_BARRA_ACTUALIZAR			19
#define CMD_INIDICADOR_ACTUALIZAR		20
#define CMD_GRAFICO_ACTUALIZAR			21
#define CMD_AGREGAR_PUNTO_SERIE			22
#define CMD_SCROLL_ACTUALIZAR			23
#define CMD_DIBUJAR_ICONO				24
#define CMD_MODIF_SCROLL_BAR_MAX		25

#define ICONO_PREGUNTA					0
#define ICONO_ERROR						1
#define ICONO_ADVERTENCIA				2
#define ICONO_PLAY						3
#define ICONO_CARPETA					4

//*************************************************************

/*typedef struct
{
	LPC_UART_TypeDef* UARTn;
	unsigned char* bufferTx;
	unsigned int lenBufferTx;
	unsigned int ptrTxIn;
	unsigned int ptrTxOut;
	unsigned char* bufferRx;
	unsigned int lenBufferRx;
	unsigned int ptrRxIn;
	unsigned int ptrRxOut;
	void (*estadoFSM) ();
}fsmUART;*/

//*************************************************************
// Funciones públicas

// Task
void createDriverGLCD (fsmUART* periphUART);
void driverGLCD (void* uart);
//


unsigned char glcdSerialScollLine (fsmUART* fsmStruct, unsigned char linea);
unsigned char glcdSerialClearLine (fsmUART* fsmStruct, unsigned char linea);
unsigned char glcdSerialClearScreen (fsmUART* fsmStruct);
unsigned char glcdSerialRender (fsmUART* fsmStruct);
unsigned char glcdSerialPunto (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char color);
unsigned char glcdSerialLinea (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
unsigned char glcdSerialCirculo (fsmUART* fsmStruct, unsigned char centroX, unsigned char centroY, unsigned char radio, unsigned char color, unsigned char relleno);
unsigned char glcdSerialRectangulo (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color, unsigned char relleno);
unsigned char glcdSerialBorrarArea (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
unsigned char glcdSerialPrintCaracter (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char caracter, unsigned char color);
unsigned char glcdSerialPrintf (fsmUART* fsmStruct, unsigned char x0, unsigned char y0, unsigned char* texto, unsigned char color);
unsigned char glcdAgregarBotones (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdBoton* botones);
unsigned char glcdAgregarBarraProgreso (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdBarraProgreso* barras);
unsigned char glcdAgregarIndicadorVariable (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdIndicadorVariable* indicadores);
unsigned char glcdAgregarGrafico (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdGrafico* graficos);
unsigned char glcdAgregarSeries (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdSerie* series);
unsigned char glcdAgregarScrollBar (fsmUART* fsmStruct, unsigned char cantidadObjetos, glcdScrollBar* barras);
unsigned char glcdActualizarBoton (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char nuevoEstado, unsigned char actualizar);
unsigned char glcdActualizarBarraProgreso (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char porcentaje, unsigned char actualizar);
unsigned char glcdActualizarIndicadorVariable (fsmUART* fsmStruct, unsigned char idObjeto, float nuevoValor, unsigned char actualizar);
unsigned char glcdActualizarGrafico (fsmUART* fsmStruct, unsigned char idObjeto);
unsigned char glcdAgregarPuntoSerie (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char numeroSerie, signed char nuevoValor);
unsigned char glcdActualizarScrollBar (fsmUART* fsmStruct, unsigned char idObjeto, unsigned int nuevoValor, unsigned char actualizar);
unsigned char glcdDibujarIcono (fsmUART* fsmStruct, unsigned char idObjeto, unsigned char x0, unsigned char y0);
void glcdDibujarMessageBox (fsmUART* fsmStruct, glcdMessageBox* mensaje);
void glcdActualizarIconMenu (fsmUART* fsmStruct, glcdIconMenu* menu, unsigned char actualizar);
void glcdActualizarListBox (fsmUART* fsmStruct, glcdListBox* listBox, unsigned char actualizar);
void glcdActualizarTextBox (fsmUART* fsmStruct, glcdTextBox* textBox, unsigned char actualizar);
void glcdActualizarBotonera (fsmUART* fsmStruct, glcdBotonera* botonera, unsigned char actualizar);
void glcdModificarScrollBarMax (fsmUART* fsmStruct, unsigned char idObjeto, unsigned int nuevoMaximo);
//*************************************************************

#endif
