#ifndef COMMON_ENUM_H_
#define COMMON_ENUM_H_

// ****************************************************************************************************************************
// Tipos de objetos GLCD
typedef enum
{
	NINGUNO = 0,\
	BOTON,\
	BARRA_PRGRESO,\
	INDICADOR_VARIABLE,\
	GRAFICO,\
	SERIE,\
	SCROLL_BAR,\
	ICON_MENU,\
	LIST_BOX,\
	BOTONERA,\
	TEXT_BOX
}glcdObjectType;
// ****************************************************************************************************************************


// ****************************************************************************************************************************
// Nombre de los objetos en el GLCD
enum {	BOTON_ARRIBA_VISTA1 = 0, BOTON_ABAJO_VISTA1,
		BOTON_ARRIBA_VISTA_LISTA, BOTON_ABAJO_VISTA_LISTA,
		BOTON_PLAY_VISTA_REPRO, BOTON_PAUSA_VISTA_REPRO,
		BOTON_MENU_VISTA_REPRO};									// Objetos BOTONES
enum {SCROLL_BAR_VISTA1 = 0, SCROLL_BAR_VISTA_LISTA,
	  SCROLL_BAR_TIEMPO_REPRODUCCION};								// Objetos SCROLL BARS
enum {MESSAGE_ERROR_SD = 0};										// Objetos MESSAGE BOX
enum {MENU_PRINCIPAL = 0};											// Objetos ICON_MENU
enum {BOTONERA_REPRO = 0};											// Objetos BOTONERA
enum {TEXTBOX_TIEMPO_TRASNCURRIDO = 0, TEXTBOX_TIEMPO_TOTAL,
	  TEXTBOX_TITULO, TEXTBOX_ARTISTA, TEXTBOX_ALBUM};				// objetos TEXT BOX
enum {LISTA_ITEMS = 0};												// Objetos LIST_BOX
enum {NONE_TYPE = 0, DIRECTORIO, ARCHIVO};								// Tipos de items de LISTA_ITEMS
// ****************************************************************************************************************************


// ****************************************************************************************************************************
// Tipos de eventos que puede recibir el View Manager
enum {OBJECT_REFRESH = 0, GENERAL_REFRESH, CHANGE_VIEW, SHOW_MESSAGE_BOX, SHOW_MESSAGE_BOX_BLOCKING};
// ****************************************************************************************************************************


// ****************************************************************************************************************************
// Enums para la matriz view-evento
enum {PULSADOR_ARRIBA = 0, PULSADOR_ABAJO, PULSADOR_IZQUIERDA, PULSADOR_DERECHA, PULSADOR_ENTER, TIMER_REFRESH_REPRO};			// Nombre de los eventos
enum {VISTA_MENU = 0, VISTA_LISTA, VISTA_REPRO, VISTA_MESSAGE_DEFAULT, VISTA_MESSAGE_DEFAULT_BLOCKING};		// Vistas
																											// Siempre que se usen Mesagge Box se debe agregar la vista VISTA_MESSAGE_DEFAULT, para asignarles
																											// las acciones correspondientes a los botones físicos cuando aparece un Message Box.
																											// La vista VISTA_MESSAGE_DEFAULT_BLOCKING no permite recuperarse de luego de mostrar el mensaje.
																											// Se debe resetear el micro.
// ****************************************************************************************************************************


#endif
