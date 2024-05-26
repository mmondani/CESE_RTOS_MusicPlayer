#ifndef DRIVERUART_H_
#define DRIVERUART_H_

// ******************************************
// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
// ******************************************


//*************************************************************
// Task
void createDriverUART (void);
void driverUART (void* uart);
//*************************************************************


#endif
