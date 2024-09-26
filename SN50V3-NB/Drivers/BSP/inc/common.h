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
#include "i2c.h"

#include "at.h"
#include "time_server.h"
#include "battery_read.h"
#include "ds18b20.h"
#include "sht20.h"
#include "sht31.h"
#include "lidar.h"
#include "ultrasound.h"
#include "weight.h"
#include "ult.h"
#include "maxsonar.h"
#include "ne117.h"
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

#define AT_VERSION_STRING 	"v1.2.4"
#define product_id 					 0x04

typedef enum
{
 model1 =1,
 model2 ,
 model3 ,
 model4 ,
 model5 ,
 model6 ,
 model7 ,	
 model8 ,
 model9 ,
 model10 ,
}model;

typedef struct
{
	uint16_t len;
	uint8_t* data;
}USART;

typedef struct
{
	uint8_t*		pwd;		   	 		//System password
	uint8_t			pwd_flag;		 		//Password correct flag
	uint8_t			fdr_flag;		 		//FDR flag
	uint8_t  		exit_flag;		 	//System external interrupt 
	uint8_t  		exit_flag_pa4;		 	//System external interrupt 
	uint8_t  		exit_flag_pa0;		 	//System external interrupt 
	uint8_t  		mod;				   	//mode
	uint8_t  		inmod;			   	//Interrupt mode
	uint8_t  		inmod_pa4;			   	//Interrupt mode
	uint8_t  		inmod_pa0;			   	//Interrupt mode
	uint32_t 		tdc;				   	//Send cycle
	uint16_t 		power_time;	 		//Power on time 
	uint16_t 		uplink_count; 	//Number of postings
	uint8_t  		protocol;		 		//protocol
	uint16_t 		rxdl;				 		//Receiving time
	uint8_t 		tr_time;				//Time interval of sensor recording data 
	uint8_t     csq_time;
  uint8_t     dns_time;	
  uint8_t     dns_timer;	
	uint8_t			sht_seq;
	uint8_t			sht_noud;
	USART				usart;
	uint8_t  		platform;		
	bool  		tlsmod;		 
	uint8_t  		cert;		
	uint8_t			log_seq;
	bool        clock_switch;
	uint16_t    strat_time;
}SYSTEM;

typedef struct
{
	uint8_t  deui[16];
	uint8_t  add[70];
	uint8_t  add_ip[50];
	uint8_t  dns_add[26];
	uint8_t  apn[41];
	uint8_t  client[129];
	uint8_t  uname[129];
	uint8_t  pwd[201];
	uint8_t  pubtopic[129];
	uint8_t  subtopic[129];
	uint8_t  uri1[129];
	uint8_t  uri2[129];
	uint8_t  uri3[129];
	uint8_t  uri4[129];	
}USER;

typedef struct
{
	uint8_t  	exit_state;
	uint8_t  	exit_level;
	uint8_t  	exit_state_pa4;
	uint8_t  	exit_level_pa4;
	uint8_t  	exit_state_pa0;
	uint8_t  	exit_level_pa0;	
	uint8_t  	singal;
	uint32_t 	exit_count;
	uint32_t 	exit_count_pa4;
	uint32_t 	exit_count_pa0;
	uint8_t 	count_mode;
	uint32_t 	time_stamp;
	int 			temDs18b20_1;
	int 			temDs18b20_2;
	int 			temDs18b20_3;
	int 			humSHT;
	int 			temSHT;
	uint16_t 	batteryLevel_mV;
	uint16_t 	adc1;
	uint16_t 	adc2;
	uint16_t 	adc3;
	uint16_t  distance;
	float 		GapValue;
	char*  		data;	
	uint16_t  data_len;
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
void EX_GPIO_Init_pa4(uint8_t state);
void EX_GPIO_Init_pa0(uint8_t state);
void led_on(uint16_t time);

void txPayLoadDeal(SENSOR* Sensor);
void txPayLoadDeal2(SENSOR* Sensor);
void rxPayLoadDeal(char* payload);
int hexToint(char *str);
uint16_t string_touint(void);
void StrToHex(char *pbDest, char *pszSrc, int nLen);
int countchar(char *str,char a);
void new_firmware_update(void);
uint8_t is_ipv4_addr(char *ip);
uint8_t is_ipv6_addr(char *ip);
char* Int2String(int num,char *str);
void  BSP_sensor_Init( void  );
void shtDataINIT(void);
void shtDataWrite(void);
void shtDataPrint(void);
void shtDataClear(void);
void DatalogPrint(void);
void DatalogClear(void);
void get_sensorvalue(void);
#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
