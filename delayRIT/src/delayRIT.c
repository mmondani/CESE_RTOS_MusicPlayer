#include "delayRIT.h"

unsigned int tiempo_delay;

void RIT_IRQHandler(void)
{
	RIT_GetIntStatus(LPC_RIT);
	if (tiempo_delay > 0)
		tiempo_delay --;
}

void delayRITInit (void)
{
	RIT_Init(LPC_RIT);
	RIT_Cmd (LPC_RIT, DISABLE);
	NVIC_EnableIRQ(RIT_IRQn);
}

void delayRIT1ms (unsigned int tiempo)
{
	unsigned int clock_rate, cmp_value;

	/*tiempo_delay = tiempo;

	clock_rate = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_RIT);
	cmp_value = (clock_rate /1000) * tiempo;

	LPC_RIT->RICOMPVAL = cmp_value;
	LPC_RIT->RICTRL |= (1<<1);*/

	RIT_TimerConfig(LPC_RIT, 1);

	RIT_Cmd (LPC_RIT, ENABLE);
	while (tiempo_delay != 0);
	RIT_Cmd (LPC_RIT, DISABLE);

}

void delayRIT1us (unsigned int tiempo)
{
	unsigned int clock_rate, cmp_value;

	tiempo_delay = tiempo;

	clock_rate = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_RIT);
	cmp_value = (clock_rate /1000000) * tiempo;

	LPC_RIT->RICOMPVAL = cmp_value;
	LPC_RIT->RICTRL |= (1<<1);

	RIT_Cmd (LPC_RIT, ENABLE);
	while (tiempo_delay != 0);
	RIT_Cmd (LPC_RIT, DISABLE);
}
