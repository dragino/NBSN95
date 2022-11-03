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

#define NEWLINE   	"\n"  										  /* Line break */

#define AT  				"AT"                     		/* state */
#define ATE  				"ATE"                     	/* Echo Mode */
#define CSQ   			"+CSQ"               				/* singel */
#define NRB					"+NRB"											/* restart */
#define CIMI				"+CIMI"											/* IMSI number */
#define NBAND				"+NBAND"										/* Frequency band */
#define CGMM			  "+CGMM"										  /* Request Manufacturer Model */

#define CGATT				"+CGATT"										/* Connect to the network*/
#define CFUN				"+CFUN"											/* Set the UE function */
#define NCONFIG 		"+NCONFIG"									/* Configure UE Behaviour */
#define CEREG				"+CEREG"										/* Query whether the network has been activated. */
#define NSONMI			"+NSONMI"										/* Disable indication messages unsolicited result code. */
#define CSCON				"+CSCON"										/* Whether to connect to the network. */
#define CPSMS				"+CPSMS"										/* PSM Setting. */
#define CCLK				"+CCLK"										  /* GET time. */
#define CGDCONT			"+CGDCONT"									/* Define a PDP Context. */
#define CGPADDR 		"+CGPADDR"									/* GET IP. */
#define CGSN 				"+CGSN=1"										/* GET IMEI. */
#define QDNS 				"+QDNS"											/* QDNS. */
#define QIDNSCFG		"+QIDNSCFG"									/* QDNS config. */

#define NSOCL				"+NSOCL"										/* closed socked */
#define NSOCLI			"+NSOCLI"										/* Socket Close Indicator(Response Only),TCP only  */
#define NSOCR				"+NSOCR"										/* Create SOCKET */
#define NSOCO				"+NSOCO"										/* Connect Command (TCP Only) */
#define NSOSD				"+NSOSD"										/* Send Command (TCP Only) */
#define NSOST				"+NSOST"										/* SendTo Command (UDP Only) */
#define NSOSTF			"+NSOSTF"										/* SendTo Command with Flags (UDP Only)*/
#define NSORF				"+NSORF" 										/* Read message*/
#define NSOSTR  		"+NSOSTR"                   /* TCP/UDP send instruction command */

#define QMTCFG  		"+QMTCFG"										/* MQTT configuration parameters */
#define QMTOPEN			"+QMTOPEN"				  				/* Create MQTT connection */
#define QMTCONN 		"+QMTCONN"                  /* Connect a client to MQTT server */
#define QMTPUB 			"+QMTPUB"                   /* Publish Messages */
#define QMTSUB 			"+QMTSUB"                   /* Subscribe to topics */
#define QMTRECV 		"+QMTRECV"									/* Receive Data */
#define QMTDISC 		"+QMTDISC"									/* Disconnect a client from MQTT server */
#define QMTSTAT			"+QMTSTAT"									/* URC to Indicate State Change in MQTT Link Layer */

#define QCOAPCREATE "+QCOAPCREATE"							/* Create the CoAP context. */
#define QCOAPCFG 		"+QCOAPCFG"									/* Configure to show the CoAP option of sender. */
#define QCOAPOPTION "+QCOAPOPTION"							/* Configure CoAP Options */
#define QCOAPSEND   "+QCOAPSEND"								/* Send Data */
#define QCOAPDEL    "+QCOAPDEL"									/* Delete the CoAP context */
#define QCOAPURC		"+QCOAPURC"

#define QCOAPOPEN   "+QCOAPOPEN"							/* COAP Configure server address. */
#define QCOAPCLOSE  "+QCOAPCLOSE"							/* Delete a CoAP Context. */
typedef enum
{
	_AT = 0,					 	//
	_ATE,						//Set Command Echo Mode
	_AT_IMEI,			 	//(AT+CGSN=1)
	_AT_IMSI,      	//(AT+CIMI)
	_AT_CFUN,			 	//AT+CFUN
	_AT_NCONFIG,		//Configure UE Behaviour
	_AT_NBAND,		 	//AT+NBAND
	_AT_CGMM,       // Request Manufacturer Model 
	_AT_CCLK,				//AT+CCLK?
	_AT_CGDCONT,		//SET APN
	_AT_CPSMS,     	//Power Saving ModeSetting
	_AT_CSQ,       	//Singal
	_AT_QDNSCFG,    //DNSCFG
	_AT_QDNS,       //DNS
/*COAP*/
	_AT_COAP_CONFIG,//CoAP Configuration Command
	_AT_COAP_OPEN,	//OPEN COAP PORT
	_AT_COAP_OPTION,//Configure CoAP Options
	_AT_COAP_SEND_CONFIG,  //SEND DATA
	_AT_COAP_SEND,  //SEND DATA
	_AT_COAP_READ,  //READ DATA	
	_AT_COAP_CLOSE, //CLOSE UDP PORT
	_AT_COAP_URI,
/*COAP BC95-GV*/
  _AT_COAP_OPEN_CONFIG_95GV,
	_AT_COAP_OPTION_95GV,
	_AT_COAP_SEND_CONFIG_95GV,
	_AT_COAP_CLOSE_BC95GV,
	_AT_COAP_URI_95GV,
/*UDP*/
	_AT_UDP_OPEN,		//OPEN UDP PORT
	_AT_UDP_SEND,   //SEND DATA
	_AT_UDP_READ,   //READ DATA	
	_AT_UDP_CLOSE,  //CLOSE UDP PORT
	_AT_UDP_URI,
/*MQTT*/
	_AT_MQTT_Config,//MQTT configuration
	_AT_MQTT_OPEN,  //OPEN MQTT PORT
	_AT_MQTT_CONN,  //Connect a client to MQTT server
	_AT_MQTT_SUB,   //Subscribe to MQTT topics
	_AT_MQTT_PUB,   //Pubscribe MQTT topic
	_AT_MQTT_SEND,  //SEND DATA
	_AT_MQTT_READ,  //READ DATA	
	_AT_MQTT_CLOSE, //CLOSE MQTT PORT
	_AT_MQTT_URI,
/*TCP*/	
	_AT_TCP_OPEN,		//OPEN TCP PORT
	_AT_TCP_CONN,		//Connect to server
	_AT_TCP_SEND,   //SEND DATA
	_AT_TCP_READ,   //READ DATA	
	_AT_TCP_CLOSE,  //CLOSE TCP PORT	
	_AT_TCP_URI,
	
	_AT_NRB,			  //restart
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
	NB_NBAND_SET,
	NB_NBAND_NOSET,
	NB_OTHER,
	NB_STA_SUCC,
	NB_35_SUCC,
	NB_95g_SUCC,
	NB_95_SUCC,
	NB_QCOAPOPEN_SUCC,
	NB_QCOAPCLOSE_SUCC
}NB_TaskStatus;


extern NB nb;

NB_TaskStatus nb_null_run(const char* param);
NB_TaskStatus nb_at_run(const char* param);
NB_TaskStatus nb_cgsn_get(const char* param);
NB_TaskStatus nb_ate_run(const char* param);
NB_TaskStatus nb_cimi_get(const char* param);
NB_TaskStatus nb_cfun_run(const char* param);
NB_TaskStatus nb_cfun_set(const char* param);
NB_TaskStatus nb_cfun_get(const char* param);
NB_TaskStatus nb_nconfig_run(const char* param);

NB_TaskStatus nb_nband_run(const char* param);
NB_TaskStatus nb_nband_set(const char* param);
NB_TaskStatus nb_nband_get(const char* param);

NB_TaskStatus nb_cgmm_run(const char* param);
NB_TaskStatus nb_cgmm_get(const char* param);

NB_TaskStatus nb_cclk_run(const char* param);
NB_TaskStatus nb_cclk_get(const char* param);

NB_TaskStatus nb_cgdcont_run(const char* param);
NB_TaskStatus nb_cgdcont_set(const char* param);
NB_TaskStatus nb_cgdcont_get(const char* param);

NB_TaskStatus nb_cpsms_run(const char* param);
NB_TaskStatus nb_cpsms_set(const char* param);
NB_TaskStatus nb_cpsms_get(const char* param);

NB_TaskStatus nb_qdnscfg_run(const char* param);
NB_TaskStatus nb_qdnscfg_set(const char* param);

NB_TaskStatus nb_qdns_run(const char* param);
NB_TaskStatus nb_qdns_set(const char* param);
NB_TaskStatus nb_qdns_get(const char* param);

NB_TaskStatus nb_csq_get(const char* param);
NB_TaskStatus nb_nrb_run(const char* param);

NB_TaskStatus nb_COAP_open_run(const char* param);
NB_TaskStatus nb_COAP_open_set(const char* param);
NB_TaskStatus nb_COAP_open_get(const char* param);

NB_TaskStatus nb_COAP_config_run(const char* param);
NB_TaskStatus nb_COAP_config_set(const char* param);
NB_TaskStatus nb_COAP_config_get(const char* param);

NB_TaskStatus nb_COAP_option_run(const char* param);
NB_TaskStatus nb_COAP_option_set(const char* param);
NB_TaskStatus nb_COAP_option_get(const char* param);

NB_TaskStatus nb_COAP_send_config_run(const char* param);
NB_TaskStatus nb_COAP_send_config_set(const char* param);

NB_TaskStatus nb_COAP_send_run(const char* param);
NB_TaskStatus nb_COAP_send_set(const char* param);

NB_TaskStatus nb_COAP_read_run(const char* param);
NB_TaskStatus nb_COAP_read_set(const char* param);
NB_TaskStatus nb_COAP_read_get(const char* param);

NB_TaskStatus nb_COAP_close_run(const char* param);
NB_TaskStatus nb_COAP_close_set(const char* param);
NB_TaskStatus nb_COAP_close_get(const char* param);

NB_TaskStatus nb_COAP_uri_run(const char* param);

NB_TaskStatus nb_COAP_open_config95gv_run(const char* param);
NB_TaskStatus nb_COAP_open_config95gv_set(const char* param);

NB_TaskStatus nb_COAP_option95gv_run(const char* param);
NB_TaskStatus nb_COAP_option95gv_set(const char* param);
NB_TaskStatus nb_COAP_option95gv_get(const char* param);

NB_TaskStatus nb_COAP_send_config95gv_run(const char* param);
NB_TaskStatus nb_COAP_send_config95gv_set(const char* param);

NB_TaskStatus nb_COAP_closebc95gv_run(const char* param);
NB_TaskStatus nb_COAP_closebc95gv_set(const char* param);
NB_TaskStatus nb_COAP_closebc95gv_get(const char* param);

NB_TaskStatus nb_COAP_uri95gv_run(const char* param);

NB_TaskStatus nb_UDP_open_run(const char* param);
NB_TaskStatus nb_UDP_open_set(const char* param);
NB_TaskStatus nb_UDP_open_get(const char* param);

NB_TaskStatus nb_UDP_send_run(const char* param);
NB_TaskStatus nb_UDP_send_set(const char* param);

NB_TaskStatus nb_UDP_read_run(const char* param);
NB_TaskStatus nb_UDP_read_set(const char* param);
NB_TaskStatus nb_UDP_read_get(const char* param);

NB_TaskStatus nb_UDP_close_run(const char* param);
NB_TaskStatus nb_UDP_close_set(const char* param);
NB_TaskStatus nb_UDP_close_get(const char* param);

NB_TaskStatus nb_UDP_uri_run(const char* param);

NB_TaskStatus nb_MQTT_config_set(const char* param);

NB_TaskStatus nb_MQTT_open_run(const char* param);
NB_TaskStatus nb_MQTT_open_set(const char* param);

NB_TaskStatus nb_MQTT_conn_run(const char* param);
NB_TaskStatus nb_MQTT_conn_set(const char* param);

NB_TaskStatus nb_MQTT_sub_run(const char* param);
NB_TaskStatus nb_MQTT_sub_set(const char* param);

NB_TaskStatus nb_MQTT_pub_run(const char* param);
NB_TaskStatus nb_MQTT_pub_set(const char* param);

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

NB_TaskStatus nb_TCP_conn_run(const char* param);
NB_TaskStatus nb_TCP_conn_set(const char* param);
NB_TaskStatus nb_TCP_conn_get(const char* param);

NB_TaskStatus nb_TCP_send_run(const char* param);
NB_TaskStatus nb_TCP_send_set(const char* param);

NB_TaskStatus nb_TCP_read_run(const char* param);
NB_TaskStatus nb_TCP_read_set(const char* param);
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
/**************** CFUN	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CFUN,
		.len_string 		= 0,
		.time_out 			= 2000,
    .run 						= nb_cfun_run,
		.set						= nb_cfun_set,
		.get						= nb_cfun_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** NCONFIG	****************/
	{		
    .ATSendStr 			= AT NCONFIG "=AUTOCONNECT,TRUE" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_NCONFIG,
		.len_string 		= sizeof(AT NCONFIG "=AUTOCONNECT,TRUE" NEWLINE) - 1,
		.time_out 			= 300,
    .run 						= nb_nconfig_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** NBAND	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_NBAND,
		.len_string 		= 0,
		.time_out 			= 300,
    .run 						= nb_nband_run,
		.set						= nb_nband_set,
		.get						= nb_nband_get,
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
		.time_out 			= 500,
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
		.time_out 			= 300,
		.try_num        = 4,
    .run 						= nb_qdns_run,
		.set						= nb_qdns_set,
		.get						= nb_qdns_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** COAP_QCOAPCFG	****************/
	{		
    .ATSendStr 			= AT QCOAPCFG "=\"Showrspopt\",1" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_CONFIG,
		.len_string 		= sizeof(AT QCOAPCFG "=\"Showrspopt\",1" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_COAP_config_run,
		.set						= nb_COAP_config_set,
		.get						= nb_COAP_config_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** COAP_OPEN	****************/
	{
    .ATSendStr 			= AT QCOAPCREATE "=56830" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPEN,
		.len_string 		= sizeof(AT QCOAPCREATE "=56830" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_COAP_open_run,
		.set						= nb_COAP_open_set,
		.get						= nb_COAP_open_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** COAP_QCOAPOPTION	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_COAP_option_run,
		.set						= nb_COAP_option_set,
		.get						= nb_COAP_option_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** COAP_SEND_CONFIG	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_SEND_CONFIG,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_COAP_send_config_run,
		.set						= nb_COAP_send_config_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** COAP_SEND	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_SEND,
		.len_string 		= 0,
		.time_out 			= 1000,
		.try_num        = 4,
    .run 						= nb_COAP_send_run,
		.set						= nb_COAP_send_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** COAP_READ	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_READ,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_COAP_read_run,
		.set						= nb_COAP_read_set,
		.get						= nb_COAP_read_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** COAP_CLOSED	****************/
	{		
    .ATSendStr 			= AT QCOAPDEL NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_CLOSE,
		.len_string 		= sizeof(AT QCOAPDEL NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_COAP_close_run,
		.set						= nb_COAP_close_set,
		.get						= nb_COAP_close_get,
		.nb_cmd_status  = NB_IDIE,
  },		
/**************** COAP_URI	****************/

	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_COAP_URI,
		.len_string 		= 0,
		.time_out 			= 0,
		.try_num        = 0,
    .run 						= nb_COAP_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
	
/**************** _AT_COAP_OPEN_CONFIG_95GV	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPEN_CONFIG_95GV,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_COAP_open_config95gv_run,
		.set						= nb_COAP_open_config95gv_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },	
/**************** COAP_QCOAPOPTION_95GV	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION_95GV,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_COAP_option95gv_run,
		.set						= nb_COAP_option95gv_set,
		.get						= nb_COAP_option95gv_get,
		.nb_cmd_status  = NB_IDIE,
  },	
/**************** COAP_SEND_CONFIG_95GV	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_SEND_CONFIG_95GV,
		.len_string 		= 0,
		.time_out 			= 1000,
		.try_num        = 3,
    .run 						= nb_COAP_send_config95gv_run,
		.set						= nb_COAP_send_config95gv_set,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },	
/**************** COAP_CLOSED_BC95GV	****************/
	{		
    .ATSendStr 			= AT QCOAPCLOSE "=0" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_CLOSE_BC95GV,
		.len_string 		= sizeof(AT QCOAPCLOSE "=0" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_COAP_closebc95gv_run,
		.set						= nb_COAP_closebc95gv_set,
		.get						= nb_COAP_closebc95gv_get,
		.nb_cmd_status  = NB_IDIE,	
		 },
/**************** _AT_COAP_URI_95GV	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_COAP_URI_95GV,
		.len_string 		= 0,
		.time_out 			= 0,
		.try_num        = 0,
    .run 						= nb_COAP_uri95gv_run,
		.set						= nb_null_run,
		.get						= nb_null_run,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** UDP_OPEN	****************/
	{
    .ATSendStr 			= AT NSOCR "=DGRAM,17,0,1" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_OPEN,
		.len_string 		= sizeof(AT NSOCR "=DGRAM,17,0,1" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_UDP_open_run,
		.set						= nb_UDP_open_set,
		.get						= nb_UDP_open_get,
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
		.set						= nb_UDP_read_set,
		.get						= nb_UDP_read_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** UDP_CLOSED	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_CLOSE,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_UDP_close_run,
		.set						= nb_UDP_close_set,
		.get						= nb_UDP_close_get,
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
/**************** MQTT configuration	****************/
	{		
    .ATSendStr 			= AT QMTCFG "=\"version\",0,4" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_Config,
		.len_string 		= sizeof(AT QMTCFG "=\"version\",0,4" NEWLINE) - 1,
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
		.time_out 			= 500,
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
    .ATSendStr 			= AT NSOCR "=STREAM,6,0,1" NEWLINE,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_OPEN,
		.len_string 		= sizeof(AT NSOCR "=STREAM,6,0,1" NEWLINE) - 1,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_TCP_open_run,
		.set						= nb_TCP_open_set,
		.get						= nb_TCP_open_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** TCP_CONN	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_CONN,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 4,
    .run 						= nb_TCP_conn_run,
		.set						= nb_TCP_conn_set,
		.get						= nb_TCP_conn_get,
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
		.set						= nb_TCP_read_set,
		.get						= nb_TCP_read_get,
		.nb_cmd_status  = NB_IDIE,
  },
/**************** TCP_CLOSED	****************/
	{		
    .ATSendStr 			= NULL,
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_CLOSE,
		.len_string 		= 0,
		.time_out 			= 500,
		.try_num        = 2,
    .run 						= nb_TCP_close_run,
		.set						= nb_TCP_close_set,
		.get						= nb_TCP_close_get,
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
/**************** NRB	****************/
	{
    .ATSendStr 			= AT NRB NEWLINE,
		.ATRecStrOK  		= "REBOOTING",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_NRB,
		.len_string 		= sizeof(AT NRB NEWLINE) - 1,
		.time_out 			= 10000,
    .run 						= nb_nrb_run,
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
