#ifndef __BATTERY_READ_H
#define __BATTERY_READ_H

#include "common.h"

#define VREFINT_CAL       ((uint16_t*) ((uint32_t) 0x1FF80078))
#define VDDA_VREFINT_CAL            ((uint32_t) 3000) 

uint16_t HW_AdcReadChannel( uint32_t Channel );
uint16_t getVoltage(void);
uint16_t ADCModel(uint32_t channel);
void stob(uint16_t* buf,int len);
#endif
