#ifndef __lowpower_H
#define __lowpower_H

#include "usart.h"

void LPM_EnterStopMode(void (*Clock_Config)(void));
void LPM_DisableStopMode(void);

#endif
