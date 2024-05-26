#ifndef DELAYRIT_H_
#define DELAYRIT_H_

#include "LPC17xx.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_rit.h"

void delayRITInit (void);
void delayRIT1ms (unsigned int tiempo);
void delayRIT1us (unsigned int tiempo);



#endif
