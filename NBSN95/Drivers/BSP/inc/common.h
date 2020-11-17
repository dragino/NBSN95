/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H__
#define __COMMON_H__

#include "stm32l0xx_hal.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "usart.h"
#include "adc.h"
#include "iwdg.h"

#include "nbInit.h"
#include "at.h"
#include "time_server.h"
#include "ds18b20.h"
#include "sht20.h"
#include "sht31.h"
#include "ultrasound.h"
#include "lidar.h"
#include "weight.h"
#include "battery_read.h"

#include "upload.h"

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/**
 * @brief  initialises the 
 *
 * @note
 * @retval None
 */

#define version "v1.0.2"
#define stack 	"D-BC95-001"

#define COAP_PRO  0x01
#define UDP_PRO   0x02
#define MQTT_PRO  0x03

typedef enum
{
 model1 ='1',
 model2 ,
 model3 ,
 model4 ,
 model5 ,
 model6 ,	
}MODEL;

typedef struct
{
	uint32_t pwd[3];		   //System password
	uint8_t  pwd_flag;		 //Password correct flag
	uint8_t  mod;				   //mode
	uint8_t  uplink_flag;  //Send flag
	uint8_t  inmod;			   //Interrupt mode
	int 		 tdc;				   //Send cycle
	int 		 power_time;	 //Power on time 
	uint16_t uplink_count; //Number of postings
	uint8_t  protocol;		 //protocol
	uint8_t  cfm;					 //Confirm mode flag
}SYSTEM;

typedef struct
{
	uint8_t  add[25];
	uint8_t  uri[65];	
}USER;

typedef struct
{
	uint8_t  exit_flag;
	uint8_t  factor_number;
	float    factor;
	uint32_t exit_count;
	uint16_t temDs18b20_1;
	uint16_t temDs18b20_2;
	uint16_t temDs18b20_3;
	uint16_t humSHT;
	uint16_t temSHT;
	uint16_t batteryLevel_mV;
	uint16_t adc0;
	uint16_t adc1;
	uint16_t adc4;
	uint16_t distance;
	float GapValue;
	char data[37];
}SENSOR;


#ifdef __cplusplus
}
#endif

extern SYSTEM sys;
extern SENSOR sensor;
extern USER user;

void product_information_print(void);
void EX_GPIO_Init(uint8_t state);
void led_on(uint16_t time);
uint8_t uplink(void);

char* payLoadDeal(uint8_t model,char* payload);
void i2cDetection(void);

#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
