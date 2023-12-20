/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NB_INIT_H__
#define __NB_INIT_H__

typedef enum
{
	fail = 0,
	success ,
	send,
	running,
	no_status,
	invalid,
	effective 
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

#define NB_RX_SIZE  512  										    /* NB Serial port receive buffer size  */

#define NEWLINE   	"\r\n"  										  /* Line break */

#define AT  				"AT"                     		/* state */
#define ATE  				"ATE"                     	/* Echo Mode */
#define QCFGEV  	  "+QCFG"                     	/* Echo Mode */
#define CSQ   			"+CSQ"               				/* singel */
#define QRST			  "+QRST"											/* restart */
#define QRST2			  "+QRST"											/* restart */
#define CIMI				"+CIMI"											/* IMSI number */
#define QBAND				"+QBAND"										/* Frequency band */
#define CGMM			  "+CGMM"										  /* Request Manufacturer Model */

#define CGATT				"+CGATT"										/* Connect to the network*/
#define CFUNSTA				"+CFUN"											/* Set the UE function */
#define CFUNEND				"+CFUN"											/* Set the UE function */
#define CFUNOFF			"+CFUN"											/* Set the UE function */
#define QSCLK		    "+QSCLK"										/* Enable light sleep and deep sleep */
#define QSCLKOFF	    "+QSCLK"										/* Enable light sleep and deep sleep */
#define CEREG				"+CEREG"										/* Query whether the network has been activated. */
#define NSONMI			"+NSONMI"										/* Disable indication messages unsolicited result code. */
#define CSCON				"+CSCON"										/* Whether to connect to the network. */
#define CPSMS				"+CPSMS"										/* PSM Setting. */
#define CCLK				"+CCLK"										  /* GET time. */
#define CCLK2				"+CCLK"										  /* GET time. */
#define CGDCONT			"+CGDCONT"									/* Define a PDP Context. */
#define CGPADDR 		"+CGPADDR"									/* GET IP. */
#define CGSN 				"+CGSN=1"										/* GET IMEI. */
#define QDNS 				"+QIDNSGIP"											/* QDNS. */
#define QIDNSCFG		"+QIDNSCFG"									/* QDNS config. */

#define QICFG				  "+QICFG"										
#define QICLOSE				"+QICLOSE"										/* closed socked */
#define QIOPEN				"+QIOPEN"										/* Create SOCKET */
#define QISEND				"+QISEND"										/* Send Command (TCP and UDP) */

#define QMTCFG  		"+QMTCFG"										/* MQTT configuration parameters */
#define QMTOPEN			"+QMTOPEN"				  				/* Create MQTT connection */
#define QMTCONN 		"+QMTCONN"                  /* Connect a client to MQTT server */
#define QMTPUB 			"+QMTPUB"                   /* Publish Messages */
#define QMTSUB 			"+QMTSUB"                   /* Subscribe to topics */
#define QMTRECV 		"+QMTRECV"									/* Receive Data */
#define QMTDISC 		"+QMTDISC"									/* Disconnect a client from MQTT server */
#define QMTSTAT			"+QMTSTAT"									/* URC to Indicate State Change in MQTT Link Layer */

#define QSSLCFG			"+QSSLCFG"									/* Manage server and client authentication. */

typedef enum
{
	_AT = 0,					 	//
	_ATE,						//Set Command Echo Mode
	_AT_QCFGEV,				//Disable the reporting of deep sleep event URC.
	_AT_IMEI,			 	//(AT+CGSN=1)
	_AT_IMSI,      	//(AT+CIMI)
	_AT_QICFG,	
	_AT_CFUNSTA,			 	//AT+CFUN	
	_AT_CFUNOFF,			 	//AT+CFUN
	_AT_CFUNEND,			 	//AT+CFUN
	_AT_QSCLK,			 	//AT+QSCLK
	_AT_QSCLKOFF,			 	//AT+QSCLK	
	_AT_QBAND,		 	//AT+NBAND
	_AT_CGMM,       // Request Manufacturer Model 
	_AT_CCLK,				//AT+CCLK?
	_AT_CCLK2,				//AT+CCLK?
	_AT_CGDCONT,		//SET APN
	_AT_CPSMS,     	//Power Saving ModeSetting
	_AT_CSQ,       	//Singal
	_AT_QDNSCFG,    //DNSCFG
	_AT_QDNS,       //DNS

/*UDP*/
	_AT_UDP_OPEN,		//OPEN UDP PORT
	_AT_UDP_SEND,   //SEND DATA
	_AT_UDP_READ,   //READ DATA	
	_AT_UDP_CLOSE,  //CLOSE UDP PORT
	_AT_UDP_URI,
/*MQTT*/
	_AT_QSSLCFG,//MQTT configuration
	_AT_QMTCFG_SSL,//Use SSL/TLS TCP secure connection
	_AT_MQTT_Config,//MQTT configuration
	_AT_MQTT_OPEN,  //OPEN MQTT PORT
	_AT_MQTT_CONN,  //Connect a client to MQTT server
	_AT_MQTT_SUB,   //Subscribe to MQTT topics
	_AT_MQTT_PUB,   //Pubscribe MQTT topic
	_AT_MQTT_PUB1,   //Pubscribe MQTT topicThingspeak
	_AT_MQTT_PUB2,   //Pubscribe MQTT topicDatacake
	_AT_MQTT_PUB3,   //Pubscribe MQTT topicThingboard
	_AT_MQTT_PUB5,   //Pubscribe MQTT JSON
	_AT_MQTT_SEND,  //SEND DATA
	_AT_MQTT_READ,  //READ DATA	
	_AT_MQTT_CLOSE, //CLOSE MQTT PORT
	_AT_MQTT_URI,
/*TCP*/	
	_AT_TCP_OPEN,		//OPEN TCP PORT
	_AT_TCP_SEND,   //SEND DATA
	_AT_TCP_READ,   //READ DATA	
	_AT_TCP_CLOSE,  //CLOSE TCP PORT	
	_AT_TCP_URI,
	
	_AT_QRST,			  //restart
	_AT_QRST2,			  //restart
	_AT_URI,
	_AT_ERROR,
  _AT_IDLE,
	_AT_FLAG_INIT,
	
	_AT_UPLOAD_START,
	_AT_UPLOAD_SUCC,
	_AT_UPLOAD_FAIL,
	_AT_UPLOAD_END,
}ATCmdNum;

typedef struct
{
	uint8_t  	socket;							 				 //The port number
	uint8_t 	singal;											 //Signal strength, always less than 99 
	uint8_t  	net_flag;						 				 //Network access flag
	uint8_t  	uplink_flag;								 //Send flag 
	uint8_t   dns_flag;										 //DNS flag 
	uint8_t  	recieve_flag;				 				 //Serial reception completion flag
	uint8_t		recieve_ack_flag;
	uint8_t   resend_count;
	uint8_t		imsi[20];						 				 //IMSI number
	uint8_t		imei[20];						 				 //IMEI number
	USART			usart;
}NB;

typedef enum   //BC95-G Status flag
{
	NB_IDIE = 0,
	NB_SEND,
	NB_RECIEVE,
	NB_BUSY,
	NB_ERROR,
	NB_ACCESS,
	NB_NO_TIME,
	NB_CMD_SUCC,
	NB_CMD_FAIL,
	NB_OPEN_SUCC,
	NB_OPEN_FAIL,
	NB_CONN_SUCC,
	NB_CONN_FAIL,
	NB_SEND_SUCC,
  NB_NSOCO_SUCC,
	NB_SEND_FAIL,
	NB_SUB_SUCC,
	NB_SUB_FAIL,
	NB_PUB_SUCC,
	NB_PUB_FAIL,
	NB_READ_NODATA,
	NB_READ_DATA,
	NB_CLOSE_SUCC,
	NB_CLOSE_FAIL,
	NB_CMD_OFF,
	NB_CMD_ON,
	NB_QBAND_SET,
	NB_QBAND_NOSET,
	NB_OTHER,
	NB_STA_SUCC,
}NB_TaskStatus;


extern NB nb;

NB_TaskStatus nb_null_run(const char* param);
NB_TaskStatus nb_at_run(const char* param);
NB_TaskStatus nb_cgsn_get(const char* param);
NB_TaskStatus nb_ate_run(const char* param);
NB_TaskStatus nb_cimi_get(const char* param);

NB_TaskStatus nb_cfunsta_run(const char* param);
NB_TaskStatus nb_cfunsta_set(const char* param);
NB_TaskStatus nb_cfunsta_get(const char* param);

NB_TaskStatus nb_cfun_run(const char* param);
NB_TaskStatus nb_cfun_set(const char* param);
NB_TaskStatus nb_cfun_get(const char* param);
NB_TaskStatus nb_cfunoff_run(const char* param);
NB_TaskStatus nb_cfunoff_set(const char* param);
NB_TaskStatus nb_cfunoff_get(const char* param);
NB_TaskStatus nb_qsclk_run(const char* param);
NB_TaskStatus nb_qsclk_set(const char* param);
NB_TaskStatus nb_qsclk_get(const char* param);

NB_TaskStatus nb_qicfg_run(const char* param);
NB_TaskStatus nb_qicfg_set(const char* param);
NB_TaskStatus nb_qicfg_get(const char* param);

NB_TaskStatus nb_qsclkoff_run(const char* param);
NB_TaskStatus nb_qsclkoff_set(const char* param);
NB_TaskStatus nb_qsclkoff_get(const char* param);

NB_TaskStatus nb_qband_run(const char* param);
NB_TaskStatus nb_qband_set(const char* param);
NB_TaskStatus nb_qband_get(const char* param);

NB_TaskStatus nb_cgmm_run(const char* param);
NB_TaskStatus nb_cgmm_get(const char* param);

NB_TaskStatus nb_cclk_run(const char* param);
NB_TaskStatus nb_cclk_get(const char* param);

NB_TaskStatus nb_cclk2_run(const char* param);
NB_TaskStatus nb_cclk2_get(const char* param);

NB_TaskStatus nb_cgdcont_run(const char* param);
NB_TaskStatus nb_cgdcont_set(const char* param);
NB_TaskStatus nb_cgdcont_get(const char* param);

NB_TaskStatus nb_cpsms_run(const char* param);
NB_TaskStatus nb_cpsms_set(const char* param);
NB_TaskStatus nb_cpsms_get(const char* param);

NB_TaskStatus nb_qdnscfg_run(const char* param);
NB_TaskStatus nb_qdnscfg_set(const char* param);

NB_TaskStatus nb_qcfgev_run(const char* param);
NB_TaskStatus nb_qcfgev_set(const char* param);

NB_TaskStatus nb_qdns_run(const char* param);
NB_TaskStatus nb_qdns_set(const char* param);
NB_TaskStatus nb_qdns_get(const char* param);

NB_TaskStatus nb_csq_get(const char* param);
NB_TaskStatus nb_qrst_run(const char* param);
NB_TaskStatus nb_qrst2_run(const char* param);

NB_TaskStatus nb_UDP_open_run(const char* param);
NB_TaskStatus nb_UDP_open_set(const char* param);
NB_TaskStatus nb_UDP_open_get(const char* param);

NB_TaskStatus nb_UDP_send_run(const char* param);
NB_TaskStatus nb_UDP_send_set(const char* param);

NB_TaskStatus nb_UDP_read_run(const char* param);
NB_TaskStatus nb_UDP_read_get(const char* param);

NB_TaskStatus nb_UDP_close_run(const char* param);
NB_TaskStatus nb_UDP_close_set(const char* param);

NB_TaskStatus nb_UDP_uri_run(const char* param);

NB_TaskStatus nb_QSSLCFG_run(const char* param);
NB_TaskStatus nb_QSSLCFG_set(const char* param);


NB_TaskStatus nb_QMTCFGSSL_run(const char* param);
NB_TaskStatus nb_QMTCFGSSL_set(const char* param);

NB_TaskStatus nb_MQTT_config_set(const char* param);

NB_TaskStatus nb_MQTT_open_run(const char* param);
NB_TaskStatus nb_MQTT_open_set(const char* param);

NB_TaskStatus nb_MQTT_conn_run(const char* param);
NB_TaskStatus nb_MQTT_conn_set(const char* param);

NB_TaskStatus nb_MQTT_sub_run(const char* param);
NB_TaskStatus nb_MQTT_sub_set(const char* param);

NB_TaskStatus nb_MQTT_pub_run(const char* param);
NB_TaskStatus nb_MQTT_pub_set(const char* param);

NB_TaskStatus nb_MQTT_pub1_run(const char* param);
NB_TaskStatus nb_MQTT_pub1_set(const char* param);

NB_TaskStatus nb_MQTT_pub2_run(const char* param);
NB_TaskStatus nb_MQTT_pub2_set(const char* param);

NB_TaskStatus nb_MQTT_pub3_run(const char* param);
NB_TaskStatus nb_MQTT_pub3_set(const char* param);

NB_TaskStatus nb_MQTT_pub5_run(const char* param);
NB_TaskStatus nb_MQTT_pub5_set(const char* param);

NB_TaskStatus nb_MQTT_send_run(const char* param);
NB_TaskStatus nb_MQTT_send_set(const char* param);

NB_TaskStatus nb_MQTT_data_read_run(const char* param);
NB_TaskStatus nb_MQTT_data_read_set(const char* param);

NB_TaskStatus nb_MQTT_close_run(const char* param);
NB_TaskStatus nb_MQTT_close_set(const char* param);

NB_TaskStatus nb_MQTT_uri_run(const char* param);

NB_TaskStatus nb_TCP_open_run(const char* param);
NB_TaskStatus nb_TCP_open_set(const char* param);
NB_TaskStatus nb_TCP_open_get(const char* param);

NB_TaskStatus nb_TCP_send_run(const char* param);
NB_TaskStatus nb_TCP_send_set(const char* param);

NB_TaskStatus nb_TCP_read_run(const char* param);
NB_TaskStatus nb_TCP_read_get(const char* param);

NB_TaskStatus nb_TCP_close_run(const char* param);
NB_TaskStatus nb_TCP_close_set(const char* param);
NB_TaskStatus nb_TCP_close_get(const char* param);

NB_TaskStatus nb_TCP_uri_run(const char* param);

struct NBTASK 
{
  char 				*ATSendStr;															/*< send command */
	const char 	*ATRecStrOK;                       			/*< Pre-correctly received data */
	const char 	*ATRecStrError;                      		/*< Pre-correctly received data */
	const int 	cmd_num;																/*< CMD number*/
	int 				len_string;															/*< length of the command string, not including the final \0 */
	uint16_t 		time_out;																/*< Instruction timeout,unit: ms*/
	uint8_t 		try_num;																/*< Number of attempts */
  NB_TaskStatus (*run)(const char* param);      			/*< Function entry */
	NB_TaskStatus (*set)(const char* param);      			/*< Function entry */
	NB_TaskStatus (*get)(const char* param);      			/*< Function entry */
	
	NB_TaskStatus  nb_cmd_status;
};

static struct NBTASK NBTask[] =
{
/**************** AT	****************/
	{		
    .ATSendStr 	 		= AT NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = NULL,
		.cmd_num        = _AT,
		.len_string 		= sizeof(AT  NEWLINE) - 1,
		.time_out 			= 300,
		.try_num        = 3,
    .run 						= nb_at_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** ATE	****************/
	{		
    .ATSendStr 	 		= ATE"0" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _ATE,
		.len_string 		= sizeof(ATE"0" NEWLINE) - 1,
		.time_out 			= 300,
    .run 						= nb_ate_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** QCFGEV	****************/
	{		
    .ATSendStr 	 		= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QCFGEV,
		.len_string 		= 0,
		.time_out 			= 300,
		.try_num        = 2,		
    .run 						= nb_qcfgev_run,
		.set						= nb_qcfgev_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CGSN	****************/
	{		
    .ATSendStr      = AT CGSN NEWLINE,
		.ATRecStrOK     = "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_IMSI,
		.len_string 		= sizeof(AT CGSN NEWLINE) - 1,
		.time_out 			= 300,
    .run 						= nb_null_run,
		.set						= nb_null_run,
		.get						= nb_cgsn_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CIMI	****************/
	{		
    .ATSendStr 			= AT CIMI NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_IMEI,
		.len_string 		= sizeof(AT CIMI NEWLINE) - 1,
		.time_out 			= 300,
    .run 						= nb_null_run,
		.set						= nb_null_run,
		.get						= nb_cimi_get,
		.nb_cmd_status  = NB_IDIE,
  },
/****************QICFG	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QICFG,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_qicfg_run,
		.set						= nb_qicfg_set,
		.get						= nb_qicfg_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CFUNSTA	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CFUNSTA,
		.len_string 		= 0,
		.time_out 			= 2000,
    .run 						= nb_cfunsta_run,
		.set						= nb_cfunsta_set,
		.get						= nb_cfunsta_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CFUNOFF	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CFUNOFF,
		.len_string 		= 0,
		.time_out 			= 2000,
    .run 						= nb_cfunoff_run,
		.set						= nb_cfunoff_set,
		.get						= nb_cfunoff_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CFUNEND	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CFUNEND,
		.len_string 		= 0,
		.time_out 			= 2000,
    .run 						= nb_cfun_run,
		.set						= nb_cfun_set,
		.get						= nb_cfun_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** QSCLK	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QSCLK,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_qsclk_run,
		.set						= nb_qsclk_set,
		.get						= nb_qsclk_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** QSCLKOFF	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QSCLKOFF,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_qsclkoff_run,
		.set						= nb_qsclkoff_set,
		.get						= nb_qsclkoff_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** QBAND	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QBAND,
		.len_string 		= 0,
		.time_out 			= 300,
    .run 						= nb_qband_run,
		.set						= nb_qband_set,
		.get						= nb_qband_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CGMM	****************/
	{		
    .ATSendStr 			=  AT CGMM NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CGMM,
		.len_string 		= sizeof(AT CGMM  NEWLINE) - 1,
		.time_out 			= 300,
    .run 						= nb_cgmm_run,
		.set						= nb_null_run,
		.get						= nb_cgmm_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CCLK	****************/
	{		
    .ATSendStr 			= AT CCLK "?" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CCLK,
		.len_string 		= sizeof(AT CCLK "?" NEWLINE) - 1,
		.time_out 			= 300,
    .run 						= nb_cclk_run,
		.set						= nb_null_run,
		.get						= nb_cclk_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CCLK2	****************/
	{		
    .ATSendStr 			= AT CCLK2 "?" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CCLK2,
		.len_string 		= sizeof(AT CCLK2 "?" NEWLINE) - 1,
		.time_out 			= 300,
    .run 						= nb_cclk2_run,
		.set						= nb_null_run,
		.get						= nb_cclk2_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CGDCONT	****************/
	{
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CGDCONT,
		.len_string 		= 0,
		.time_out 			= 300,
    .run 						= nb_cgdcont_run,
		.set						= nb_cgdcont_set,
		.get						= nb_cgdcont_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CPSMS	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CPSMS,
		.len_string 		= 0,
		.time_out 			= 300,
		.try_num        = 2,
    .run 						= nb_cpsms_run,
		.set						= nb_cpsms_set,
		.get						= nb_cpsms_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** CSQ	****************/
	{		
    .ATSendStr 			= AT CSQ NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CSQ,
		.len_string 		= sizeof(AT CSQ NEWLINE) - 1,
		.time_out 			= 300,
		.try_num        = 4,
    .run 						= nb_null_run,
		.set						= nb_null_run,
		.get						= nb_csq_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** QDNSCFG	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QDNSCFG,
		.len_string 		= 0,
		.time_out 			= 300,
		.try_num        = 4,
    .run 						= nb_qdnscfg_run,
		.set						= nb_qdnscfg_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** QDNS	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QDNS,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_qdns_run,
		.set						= nb_qdns_set,
		.get						= nb_qdns_get,
		.nb_cmd_status  = NB_IDIE,
  },

/**************** UDP_OPEN	****************/
	{
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_OPEN,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_UDP_open_run,
		.set						= nb_UDP_open_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
	
/**************** UDP_SEND	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_SEND,
		.len_string 		= 0,
		.time_out 			= 1000,
		.try_num        = 4,
    .run 						= nb_UDP_send_run,
		.set						= nb_UDP_send_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },

/**************** UDP_READ	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_READ,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_UDP_read_run,
		.set						= nb_null_run,
		.get						= nb_UDP_read_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** UDP_CLOSED	****************/
	{		
    .ATSendStr 			= AT QICLOSE"=0" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_CLOSE,
		.len_string 		= sizeof(AT QICLOSE"=0" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_UDP_close_run,
		.set						= nb_UDP_close_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** UDP_URI	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_UDP_URI,
		.len_string 		= 0,
		.time_out 			= 0,
		.try_num        = 0,
    .run 						= nb_UDP_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** QSSLCFG	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QSSLCFG,
		.len_string 		= 0,
		.time_out 			= 300,
		.try_num        = 4,
    .run 						= nb_QSSLCFG_run,
		.set						= nb_QSSLCFG_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },	
/**************** QMTCFGSSL	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QMTCFG_SSL,
		.len_string 		= 0,
		.time_out 			= 300,
		.try_num        = 4,
    .run 						= nb_QMTCFGSSL_run,
		.set						= nb_QMTCFGSSL_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT configuration	****************/
	{		
    .ATSendStr 			= AT QMTCFG "=\"version\",0,1" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_Config,
		.len_string 		= sizeof(AT QMTCFG "=\"version\",0,1" NEWLINE) - 1,
		.time_out 			= 200,
		.try_num        = 4,
    .run 						= nb_null_run,
		.set						= nb_MQTT_config_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_OPEN	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_OPEN,
		.len_string 		= 0,
		.time_out 			= 1200,
		.try_num        = 4,
    .run 						= nb_MQTT_open_run,
		.set						= nb_MQTT_open_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_CONN	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_CONN,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_MQTT_conn_run,
		.set						= nb_MQTT_conn_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_SUB	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_SUB,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_MQTT_sub_run,
		.set						= nb_MQTT_sub_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_PUB	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_MQTT_pub_run,
		.set						= nb_MQTT_pub_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_PUB1	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB1,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_MQTT_pub1_run,
		.set						= nb_MQTT_pub1_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_PUB2	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB2,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_MQTT_pub2_run,
		.set						= nb_MQTT_pub2_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_PUB3	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB3,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_MQTT_pub3_run,
		.set						= nb_MQTT_pub3_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_PUB5	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB5,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_MQTT_pub5_run,
		.set						= nb_MQTT_pub5_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_SEND	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_SEND,
		.len_string 		= 0,
		.time_out 			= 1000,
    .run 						= nb_MQTT_send_run,
		.set						= nb_MQTT_send_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_READ	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_READ,
		.len_string 		= 0,
		.time_out 			= 500,
    .run 						= nb_MQTT_data_read_run,
		.set						= nb_MQTT_data_read_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_CLOSE	****************/
	{		
    .ATSendStr 			= AT QMTDISC"=0" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_CLOSE,
		.len_string 		= sizeof(AT QMTDISC"=0" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_MQTT_close_run,
		.set						= nb_MQTT_close_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** MQTT_URI	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_MQTT_URI,
		.len_string 		= 0,
		.time_out 			= 0,
		.try_num        = 0,
    .run 						= nb_MQTT_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** TCP_OPEN	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_OPEN,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_TCP_open_run,
		.set						= nb_TCP_open_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** TCP_SEND	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_SEND,
		.len_string 		= 0,
		.time_out 			= 1000,
		.try_num        = 4,
    .run 						= nb_TCP_send_run,
		.set						= nb_TCP_send_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** TCP_READ	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_READ,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_TCP_read_run,
		.set						= nb_null_run,
		.get						= nb_TCP_read_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** TCP_CLOSED	****************/
	{		
    .ATSendStr 			= AT QICLOSE"=0" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_CLOSE,
		.len_string 		= sizeof(AT QICLOSE"=0" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_TCP_close_run,
		.set						= nb_TCP_close_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** TCP_URI	****************/
	{
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_TCP_URI,
		.len_string 		= 0,
		.time_out 			= 0,
		.try_num        = 0,
    .run 						= nb_TCP_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/****************QRST	****************/
	{
    .ATSendStr 			= AT QRST "=1" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QRST,
		.len_string 		= sizeof(AT QRST "=1" NEWLINE) - 1,
		.time_out 			= 2000,
    .run 						= nb_qrst_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/****************QRST2	****************/
	{
    .ATSendStr 			= AT QRST2 "=1" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QRST2,
		.len_string 		= sizeof(AT QRST2 "=1" NEWLINE) - 1,
		.time_out 			= 2000,
    .run 						= nb_qrst2_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
};

#ifdef __cplusplus
}
#endif

NB_TaskStatus nb_at_send(struct NBTASK *NB_Task);
ATCmdNum NBTASK(uint8_t *task);
#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
