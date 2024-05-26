#ifndef VIEW_INTERFACE_MATRIX_
#define VIEW_INTERFACE_MATRIX_

#include "common_enum.h"
#include "view_manager.h"
#include "driverSD.h"
#include "driverVS1063.h"

// Elemento de la cola de eventos de usuario
typedef struct
{
	unsigned int eventID;
	unsigned int parametro;
}MensajeEventoUsuario;

enum {USER_EVENTO_BOTON_PLAY = 0,USER_EVENTO_BOTON_PAUSA, USER_EVENTO_};



//     Matriz vistas - interfaces físicas
// ************************************************************************

void reaccion_vista_repro_botonArriba (unsigned char param);
void reaccion_vista_repro_botonAbajo (unsigned char param);
void reaccion_vista_repro_botonEnter (unsigned char param);
void reaccion_vista_repro_botonIzquierda (unsigned char param);
void reaccion_vista_repro_botonDerecha (unsigned char param);
void reaccion_vista_repro_timerRefresh (unsigned char param);
void reaccion_vista_message_default (unsigned char param);
void reaccion_vista_menu_botonIzquierda (unsigned char param);
void reaccion_vista_menu_botonDerecha (unsigned char param);
void reaccion_vista_menu_botonEnter (unsigned char param);
void reaccion_vista_lista_botonArriba (unsigned char param);
void reaccion_vista_lista_botonAbajo (unsigned char param);
void reaccion_vista_lista_botonIzquierda (unsigned char param);
void reaccion_vista_lista_botonEnter (unsigned char param);
void reaccion_NULL (unsigned char param);

#define CANTIDAD_EVENTOS			6
#define	CANTIDAD_VISTAS				5


// ************************************************************************


#endif
