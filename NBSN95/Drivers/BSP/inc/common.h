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

#define AT_VERSION_STRING 	"v1.2.7"
#define stack 							"D-BC95-003"

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
}model;

typedef struct Node
{
	uint8_t num;								//
	uint8_t data_valid_flag;		//Data valid flag 
	int Timestamp;							//Timestamp 
	char* data;									//DATA
	struct Node *next;					//Point to the next node 
}Node,*LinkedList;

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
	uint8_t			cum_flag;		 		//Cache upload mechanism flag
	uint8_t  		exit_flag;		 	//System external interrupt 
	uint8_t  		mod;				   	//mode
	uint8_t  		inmod;			   	//Interrupt mode
	uint32_t 		tdc;				   	//Send cycle
	uint16_t 		power_time;	 		//Power on time 
	uint16_t 		uplink_count; 	//Number of postings
	uint8_t  		protocol;		 		//protocol
	uint8_t  		cfm;					 	//Confirm mode flag
	uint16_t 		rxdl;				 		//Receiving time
	uint8_t    	list_flag;
	uint8_t     nband_flag;
	
	LinkedList 	list;
	USART				usart;
}SYSTEM;

typedef struct
{
	uint8_t  deui[16];
	uint8_t  add[70];
	uint8_t  add_ip[25];
	uint8_t  dns_add[16];
	uint8_t  apn[21];
	uint8_t  uri[128];
	uint8_t  client[41];
	uint8_t  uname[41];
	uint8_t  pwd[41];
	uint8_t  pubtopic[65];
	uint8_t  subtopic[65];
}USER;

typedef struct
{
	uint8_t  	exit_state;
	uint8_t  	singal;
	uint32_t 	exit_count;
	uint32_t 	time_stamp;
	int 			temDs18b20_1;
	int 			temDs18b20_2;
	int 			temDs18b20_3;
	int 			humSHT;
	int 			temSHT;
	uint16_t 	batteryLevel_mV;
	uint16_t 	adc0;
	uint16_t 	adc1;
	uint16_t 	adc4;
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
void led_on(uint16_t time);
uint8_t i2c_device_detection(void);

void txPayLoadDeal(SENSOR* Sensor,LinkedList L);
void txPayLoadDeal2(SENSOR* Sensor);
void rxPayLoadDeal(char* payload);
int hexToint(char *str);
uint16_t string_touint(void);
void StrToHex(char *pbDest, char *pszSrc, int nLen);
int countchar(char *str,char a);
void new_firmware_update(void);
uint8_t is_ipv4_addr(char *ip);
char* Int2String(int num,char *str);

LinkedList List_Init(LinkedList L);
LinkedList createList(void);
LinkedList printList(LinkedList L);
LinkedList sortList(LinkedList L);
LinkedList upLink_flash_clear(LinkedList L);
LinkedList txCachePayLoadDeal(SENSOR* Sensor,LinkedList L);
LinkedList upLink_fail_write(LinkedList L);
LinkedList upLink_fail_read(LinkedList L);

#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
