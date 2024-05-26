#include "temporizadores.h"



unsigned int getSystemTicks (void)
{
	return xTaskGetTickCount();
}



unsigned int expiroTemporizador (Temporizador timer)
{
	return (xTaskGetTickCount() - timer.start > timer.intervalo);
}

void initTemporizador (Temporizador* timer, unsigned int intervalo)
{
	timer->start = getSystemTicks();
	timer->intervalo = intervalo;
}

void restartTemporizador (Temporizador* timer)
{
	timer->start = getSystemTicks();
}
