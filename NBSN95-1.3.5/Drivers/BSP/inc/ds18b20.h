#ifndef __DS18B20_H
#define __DS18B20_H

#include "common.h"

void DS18B20_delay(uint16_t time);
void DS18B20_Mode_IPU(uint8_t num);
void DS18B20_Mode_Out_PP(uint8_t num);
void DS18B20_IoDeInit(uint8_t num);
void DS18B20_Rst(uint8_t num);
uint8_t DS18B20_Presence(uint8_t num);
uint8_t DS18B20_ReadBit(uint8_t num);
uint8_t DS18B20_ReadByte(uint8_t num);
void DS18B20_WriteBit(uint8_t dat,uint8_t num);
void DS18B20_WriteByte(uint8_t dat,uint8_t num);
void DS18B20_SkipRom(uint8_t num);
float DS18B20_GetTemp_SkipRom (uint8_t num);

#endif
