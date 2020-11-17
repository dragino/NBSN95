/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NB_INIT_H__
#define __NB_INIT_H__

typedef enum
{
	fail = 0,
	success 
}NBState;

#include "common.h"

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

#define AT  		"AT"                     		/* state */
#define CSQ   	"+CSQ"               				/* singel */
#define NRB			"+NRB"											/* restart */
#define CIMI		"+CIMI"											/* CIMI */
#define NABND		"+NBAND"										/* Frequency band */
#define ATE  		"ATE"                     	/* Print echo */

#define CGATT		"+CGATT"										/* Connect to the network*/
#define CFUN		"+CFUN"											/* Set the UE function */
#define CEREG		"+CEREG"										/* Query whether the network has been activated. */
#define NSONMI	"+NSONMI"										/* Disable indication messages unsolicited result code. */
#define CSCON		"+CSCON"										/* Whether to connect to the network. */
#define CPSMS		"+CPSMS"										/* PSM Setting. */
#define CCLK		"+CCLK"										  /* GET time. */
#define CGPADDR "+CGPADDR"									/* GET IP. */
#define CGSN 		"+CGSN"											/* GET IMEI. */

#define NSOCL		"+NSOCL"										/* closed socked */
#define NSOCR		"+NSOCR"										/* Create SOCKET */
#define NSOST		"+NSOST"										/* Send UDP Message */
#define NSOSTF	"+NSOSTF"										/* Send UDP Message With RCC Flag*/
#define NSORF		"+NSORF" 										/* Read message*/
#define UDP			"=DGRAM,17,56000,1\n"				/* Create UDP connection */

typedef struct
{
	uint8_t socket;							 //The port number
	uint8_t net_flag;						 //Network access flag
	uint8_t imei_flag;					 //Whether to scan the flag of the card
	uint8_t recieve_flag;				 //Serial reception completion flag
	uint8_t rxlen ;							 //Receive data length
	char    recieve_data[300];	 //Receive data 
	char    imei[30];						 //IMSI number
}NB;

#ifdef __cplusplus
}
#endif
extern NB nb;
NBState nb_send(char* data,char* buff);
NBState nb_Init(void);


NBState nb_netAccess(void);
NBState nb_CIMI(void);
NBState nb_Reboot(void);
NBState nb_CGSN(void);
NBState nb_ATE(void);
#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
