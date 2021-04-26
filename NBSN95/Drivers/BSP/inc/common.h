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
#include "battery_read.h"
#include "ds18b20.h"
#include "sht20.h"
#include "sht31.h"
#include "lidar.h"
#include "ultrasound.h"
#include "weight.h"

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

#define version 	"v1.1.1"
#define version_s "111"
#define stack 		"D-BC95-002"

#define COAP_PRO  0x01
#define UDP_PRO   0x02
#define MQTT_PRO  0x03
#define TCP_PRO   0x04

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
	uint8_t  inmod;			   //Interrupt mode
	int 		 tdc;				   //Send cycle
	int 		 power_time;	 //Power on time 
	uint16_t uplink_count; //Number of postings
	uint8_t  protocol;		 //protocol
	uint8_t  cfm;					 //Confirm mode flag
	uint16_t rxdl;				 //Receiving time
}SYSTEM;

typedef struct
{
	uint8_t  deui[15];
	uint8_t  add[25];
	uint8_t  uri[128];
	uint8_t  client[41];	
	uint8_t  uname[41];
	uint8_t  pwd[41];
	uint8_t  pubtopic[65];
	uint8_t  subtopic[65];
}USER;

typedef struct
{
	uint8_t  exit_flag;
	uint8_t  singal;
	uint32_t exit_count;
	int temDs18b20_1;
	int temDs18b20_2;
	int temDs18b20_3;
	int humSHT;
	int temSHT;
	uint16_t batteryLevel_mV;
	uint16_t adc0;
	uint16_t adc1;
	uint16_t adc4;
	uint16_t distance;
	float GapValue;
	float factor;
	char data[250];	
	int  data_len;
}SENSOR;


#ifdef __cplusplus
}
#endif

extern SYSTEM sys;
extern SENSOR sensor;
extern USER user;

void product_information_print(void);
void reboot_information_print(void);
void EX_GPIO_Init(uint8_t state);
void led_on(uint16_t time);
void i2c_device_detection(void);

void txPayLoadDeal(SENSOR* Sensor);
void rxPayLoadDeal(char* payload);
int hexToint(char *str);
#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
