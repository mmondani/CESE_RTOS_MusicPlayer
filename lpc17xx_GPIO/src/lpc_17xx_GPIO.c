#include "lpc_17xx_GPIO.h"

void GPIOInit (GPIO* pin, DireccionPin dir, EstadoPin estadoDefecto, unsigned int port, unsigned int bit)
{
	pin->direccion = dir;
	pin->estadoActivo = estadoDefecto;			// Cuando es una ENTRADA este campo es el estado activo del pin
												// Cuando es una SALIDA este campo es el estado idle del pin
	pin->puerto = port;
	pin->pin = bit;

	GPIO_SetDir(pin->puerto, 1UL << pin->pin, pin->direccion);

	if (pin->direccion == SALIDA)
		(pin->estadoActivo == BAJO)? GPIO_ClearValue(pin->puerto, 1UL << pin->pin) : GPIO_SetValue(pin->puerto, 1UL << pin->pin);
}

void GPIOactivarPin (GPIO* pin)
{
	GPIO_SetValue(pin->puerto, 1UL << pin->pin);
}

void GPIOpasivarPin (GPIO* pin)
{
	GPIO_ClearValue(pin->puerto, 1UL << pin->pin);
}

void GPIOToggle (GPIO* pin)
{
	if (pin->direccion == SALIDA)
	{
		if (((GPIO_ReadValue(pin->puerto) >> (1UL * pin->pin)) & 1UL) == 0)
			GPIOactivarPin (pin);
		else
			GPIOpasivarPin (pin);
	}
}

int GPIOestaActivo (GPIO* pin)
{
	unsigned int puerto = GPIO_ReadValue(pin->puerto);

	return (((puerto >> (1UL * pin->pin)) & 1UL) == pin->estadoActivo)? 1 : 0;
}
