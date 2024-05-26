#ifndef LPC_17XX_GPIO_H_
#define LPC_17XX_GPIO_H_

#include "lpc17xx_gpio.h"

typedef enum {BAJO = 0, ALTO} EstadoPin;
typedef enum {ENTRADA = 0, SALIDA} DireccionPin;

typedef struct
{
	DireccionPin direccion;
	EstadoPin estadoActivo;
	unsigned int puerto;
	unsigned int pin;
}GPIO;

void GPIOInit (GPIO* pin, DireccionPin dir, EstadoPin estadoDefecto, unsigned int port, unsigned int bit);
void GPIOactivarPin (GPIO* pin);
void GPIOpasivarPin (GPIO* pin);
void GPIOToggle (GPIO* pin);
int GPIOestaActivo (GPIO* pin);



#endif
