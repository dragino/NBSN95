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

#define stack 							"D-BG95-002"
#define COAP_PRO  0x01
#define UDP_PRO   0x02
#define MQTT_PRO  0x03
#define TCP_PRO   0x04
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
#define CSQ   			"+CSQ"               				/* singel */
#define QGPS   			"+QGPS"  
#define QRST			  "+QRST"											/* restart */
#define CIMI				"+CIMI"											/* IMSI number */
#define CGMM			  "+CGMM"										  /* Request Manufacturer Model */

#define CGATT				"+CGATT"										/* Connect to the network*/
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
#define CGSN 				"+CGSN"										/* GET IMEI. */
#define QNWINFO 				"+QNWINFO"		
#define QICSGP 				"+QICSGP"		
#define QIDEACT 				"+QIDEACT"		
#define QIACT 				"+QIACT"		
#define QDNS 				"+QIDNSGIP"											/* QDNS. */
#define QIDNSCFG		"+QIDNSCFG"									/* QDNS config. */
#define QGPSLOC		"+QGPSLOC"	
#define QGPSEND		"+QGPSEND"	
#define QCOAPHEADER "+QCOAPHEADER"							/* Create the CoAP context. */
#define QCOAPCFG 		"+QCOAPCFG"									/* Configure to show the CoAP option of sender. */
#define QCOAPOPTION "+QCOAPOPTION"							/* Configure CoAP Options */
#define QCOAPSEND   "+QCOAPSEND"								/* Send Data */
#define QCOAPOPEN   "+QCOAPOPEN"							/* COAP Configure server address. */
#define QCOAPRECV		"+QCOAPRECV"
#define QCOAPCLOSE  "+QCOAPCLOSE"							/* Delete a CoAP Context. */

#define QICFG				  "+QICFG"										
#define QICLOSE				"+QICLOSE"										/* closed socked */
#define QIOPEN				"+QIOPEN"										/* Create SOCKET */
#define QISEND				"+QISEND"										/* Send Command (TCP and UDP) */
#define QISENDEX				"+QISENDEX"										/* Send Command (TCP and UDP) */
#define QIRD				"+QIRD"		

#define QMTCFG  		"+QMTCFG"										/* MQTT configuration parameters */
#define QMTOPEN			"+QMTOPEN"				  				/* Create MQTT connection */
#define QMTCONN 		"+QMTCONN"                  /* Connect a client to MQTT server */
#define QMTPUBEX 			"+QMTPUB"                   /* Publish Messages */
#define QMTPUB 			"+QMTPUB"                   /* Publish Messages */
#define QMTSUB 			"+QMTSUB"                   /* Subscribe to topics */
#define QMTRECV 		"+QMTRECV"									/* Receive Data */
#define QMTDISC 		"+QMTDISC"									/* Disconnect a client from MQTT server */
#define QMTCLOSE 		"+QMTCLOSE"									/* Disconnect a client from MQTT server */
#define QMTSTAT			"+QMTSTAT"									/* URC to Indicate State Change in MQTT Link Layer */
#define QMTSTAT5			"+QMTSTAT: 0,5"		

#define QSSLCFG			"+QSSLCFG"									/* Manage server and client authentication. */
#define CACERT			"+QSSLCFG"	
#define CLICERT			"+QSSLCFG"	
#define CLIKEY			"+QSSLCFG"	
#define QCFG		  	"+QCFG"	
typedef enum
{
	_AT = 0,					 	//
	_ATE,						//Set Command Echo Mode
	_AT_IMEI,			 	//(AT+CGSN=1)
	_AT_IMSI,      	//(AT+CIMI)
	_AT_QICFG,	
	_AT_CGMM,       // Request Manufacturer Model 
	_AT_QBAND, 
	_AT_IOTM, 
	_AT_CCLK,				//AT+CCLK?
	_AT_CCLK2,				//AT+CCLK?
	_AT_CGDCONT,		//SET APN
	_AT_QGPS,       	
	_AT_CSQ,       	//Singal
	_AT_QNWINFO,
	_AT_QICSGP,       	
	_AT_QIDEACT,       
	_AT_QIACT,       	//Activate a PDP Context
	_AT_QDNSCFG,    //DNSCFG
	_AT_QDNS,       //DNS
	_AT_QGPSLOC, 
	_AT_QGPSEND,
/*COAP*/
	_AT_COAP_CONFIG,//CoAP Configuration Command
	_AT_COAP_OPEN,	//OPEN COAP PORT
	_AT_COAP_HEADER,	
	_AT_COAP_OPTION1,//Configure CoAP Options
	_AT_COAP_OPTION2,//Configure CoAP Options
	_AT_COAP_OPTION3,//Configure CoAP Options
	_AT_COAP_OPTION4,//Configure CoAP Options
	_AT_COAP_OPTION5,//Configure CoAP Options
	_AT_COAP_OPTION6,//Configure CoAP Options
	_AT_COAP_OPTION7,//Configure CoAP Options
	_AT_COAP_OPTION8,//Configure CoAP Options
	_AT_COAP_SEND_CONFIG,  //SEND DATA
	_AT_COAP_SEND,  //SEND DATA
	_AT_COAP_READ,  //READ DATA	
	_AT_COAP_CLOSE, //CLOSE UDP PORT
	_AT_COAP_URI,	
/*UDP*/
	_AT_UDP_OPEN,		//OPEN UDP PORT
	_AT_UDP_SEND,   //SEND DATA
	_AT_UDP_DATA,  //SEND DATA
	_AT_UDP_SENDEX,   //SEND DATA
	_AT_UDP_QIRD,  
	_AT_UDP_READ,   //READ DATA	
	_AT_UDP_CLOSE,  //CLOSE UDP PORT
	_AT_UDP_URI,
/*MQTT*/
	_AT_QSSLCFG,//MQTT configuration
	_AT_QMTCFG_SSL,//Use SSL/TLS TCP secure connection
	_AT_CACERT,
	_AT_CLICERT,
	_AT_CLIKEY,
	_AT_MQTT_Config,//MQTT configuration
	_AT_MQTT_OPEN,  //OPEN MQTT PORT
	_AT_MQTT_CONN,  //Connect a client to MQTT server
	_AT_MQTT_SUB,   //Subscribe to MQTT topics
	_AT_MQTT_PUBEX,   //Pubscribe MQTT topic
	_AT_MQTT_PUB1,   //Pubscribe MQTT topicThingspeak
	_AT_MQTT_PUB2,   //Pubscribe MQTT topicDatacake
	_AT_MQTT_PUB3,   //Pubscribe MQTT topicThingboard
	_AT_MQTT_PUB5,   //Pubscribe MQTT JSON
	_AT_MQTT_SEND,  //SEND DATA
	_AT_MQTT_READ,  //READ DATA	
	_AT_MQTT_DISC, //CLOSE MQTT PORT
	_AT_MQTT_CLOSE, //CLOSE MQTT PORT
	_AT_MQTT_URI,
/*TCP*/	
	_AT_TCP_OPEN,		//OPEN TCP PORT
	_AT_TCP_SEND,   //SEND DATA
	_AT_TCP_DATA,  //SEND DATA
	_AT_TCP_SENDEX,   //SEND DATA
	_AT_TCP_QIRD,  
	_AT_TCP_READ,   //READ DATA	
	_AT_TCP_CLOSE,  //CLOSE TCP PORT	
	_AT_TCP_URI,
	
	_AT_QRST,			  //restart
	_AT_URI,
	_AT_ERROR,
  _AT_IDLE,
	_AT_FLAG_INIT,
	
	_AT_UPLOAD_START,
	_AT_UPLOAD_SUCC,
	_AT_UPLOAD_FAIL,
	_AT_UPLOAD_END,
	_AT_CFUNOFF,
	_AT_QSCLK,			
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
	NB_NO_GPS,	
	NB_CMD_SUCC,
	NB_CMD_FAIL,
	NB_OPEN_SUCC,
	NB_OPEN_FAIL,
	NB_CONN_SUCC,
	NB_CONN_FAIL,
	NB_SEND_SUCC,
  NB_NSOCO_SUCC,
	NB_SEND_FAIL,
	NB_CLOSE_SUCC,
	NB_SUB_SUCC,
	NB_SUB_FAIL,
	NB_PUB_SUCC,
	NB_PUB_FAIL,
	NB_READ_NODATA,
	NB_READ_DATA,
  NB_DISC_SUCC,
	NB_CLOSE_FAIL,
	NB_CMD_OFF,
	NB_CMD_ON,
	NB_QBAND_SET,
	NB_QBAND_NOSET,
	NB_OTHER,
	NB_STA_SUCC,
	NB_RD_SUCC,
	NB_M1_SUCC,
	NB_M2_SUCC,
	NB_M3_SUCC,
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
NB_TaskStatus nb_cfunoff_run(const char* param);
NB_TaskStatus nb_cfunoff_set(const char* param);
NB_TaskStatus nb_cfunoff_get(const char* param);
NB_TaskStatus nb_qsclk_run(const char* param);
NB_TaskStatus nb_qsclk_set(const char* param);
NB_TaskStatus nb_qsclk_get(const char* param);

NB_TaskStatus nb_qicfg_run(const char* param);
NB_TaskStatus nb_qicfg_set(const char* param);
NB_TaskStatus nb_qicfg_get(const char* param);

NB_TaskStatus nb_cgmm_run(const char* param);
NB_TaskStatus nb_cgmm_get(const char* param);

NB_TaskStatus nb_qband_run(const char* param);
NB_TaskStatus nb_qband_set(const char* param);
NB_TaskStatus nb_qband_get(const char* param);

NB_TaskStatus nb_iotm_run(const char* param);
NB_TaskStatus nb_iotm_set(const char* param);
NB_TaskStatus nb_iotm_get(const char* param);

NB_TaskStatus nb_cclk_run(const char* param);
NB_TaskStatus nb_cclk_get(const char* param);

NB_TaskStatus nb_cclk2_run(const char* param);
NB_TaskStatus nb_cclk2_get(const char* param);

NB_TaskStatus nb_qnwinfo_run(const char* param);
NB_TaskStatus nb_qnwinfo_get(const char* param);

NB_TaskStatus nb_cgdcont_run(const char* param);
NB_TaskStatus nb_cgdcont_set(const char* param);
NB_TaskStatus nb_cgdcont_get(const char* param);

NB_TaskStatus nb_cpsms_run(const char* param);
NB_TaskStatus nb_cpsms_set(const char* param);
NB_TaskStatus nb_cpsms_get(const char* param);

NB_TaskStatus nb_qicsgp_run(const char* param);
NB_TaskStatus nb_qicsgp_set(const char* param);
NB_TaskStatus nb_qicsgp_get(const char* param);

NB_TaskStatus nb_qideact_run(const char* param);
NB_TaskStatus nb_qideact_set(const char* param);
NB_TaskStatus nb_qideact_get(const char* param);

NB_TaskStatus nb_qiact_run(const char* param);
NB_TaskStatus nb_qiact_set(const char* param);
NB_TaskStatus nb_qiact_get(const char* param);

NB_TaskStatus nb_qdnscfg_run(const char* param);
NB_TaskStatus nb_qdnscfg_set(const char* param);

NB_TaskStatus nb_qdns_run(const char* param);
NB_TaskStatus nb_qdns_set(const char* param);
NB_TaskStatus nb_qdns_get(const char* param);

NB_TaskStatus nb_qgps_run(const char* param);
NB_TaskStatus nb_qgps_set(const char* param);

NB_TaskStatus nb_qgpsloc_run(const char* param);
NB_TaskStatus nb_qgpsloc_get(const char* param);

NB_TaskStatus nb_qgpssend_run(const char* param);
NB_TaskStatus nb_qgpssend_set(const char* param);

NB_TaskStatus nb_csq_get(const char* param);
NB_TaskStatus nb_qrst_run(const char* param);

NB_TaskStatus nb_COAP_open_run(const char* param);
NB_TaskStatus nb_COAP_open_set(const char* param);
NB_TaskStatus nb_COAP_open_get(const char* param);

NB_TaskStatus nb_COAP_config_run(const char* param);
NB_TaskStatus nb_COAP_config_set(const char* param);
NB_TaskStatus nb_COAP_config_get(const char* param);

NB_TaskStatus nb_COAP_header_run(const char* param);
NB_TaskStatus nb_COAP_header_set(const char* param);
NB_TaskStatus nb_COAP_header_get(const char* param);

NB_TaskStatus nb_COAP_option1_run(const char* param);
NB_TaskStatus nb_COAP_option1_set(const char* param);
NB_TaskStatus nb_COAP_option1_get(const char* param);

NB_TaskStatus nb_COAP_option2_run(const char* param);
NB_TaskStatus nb_COAP_option2_set(const char* param);
NB_TaskStatus nb_COAP_option2_get(const char* param);

NB_TaskStatus nb_COAP_option3_run(const char* param);
NB_TaskStatus nb_COAP_option3_set(const char* param);
NB_TaskStatus nb_COAP_option3_get(const char* param);

NB_TaskStatus nb_COAP_option4_run(const char* param);
NB_TaskStatus nb_COAP_option4_set(const char* param);
NB_TaskStatus nb_COAP_option4_get(const char* param);

NB_TaskStatus nb_COAP_option5_run(const char* param);
NB_TaskStatus nb_COAP_option5_set(const char* param);
NB_TaskStatus nb_COAP_option5_get(const char* param);

NB_TaskStatus nb_COAP_option6_run(const char* param);
NB_TaskStatus nb_COAP_option6_set(const char* param);
NB_TaskStatus nb_COAP_option6_get(const char* param);

NB_TaskStatus nb_COAP_option7_run(const char* param);
NB_TaskStatus nb_COAP_option7_set(const char* param);
NB_TaskStatus nb_COAP_option7_get(const char* param);

NB_TaskStatus nb_COAP_option8_run(const char* param);
NB_TaskStatus nb_COAP_option8_set(const char* param);
NB_TaskStatus nb_COAP_option8_get(const char* param);

NB_TaskStatus nb_COAP_send_config_run(const char* param);
NB_TaskStatus nb_COAP_send_config_set(const char* param);

NB_TaskStatus nb_COAP_send_run(const char* param);
NB_TaskStatus nb_COAP_send_set(const char* param);

NB_TaskStatus nb_COAP_read_run(const char* param);
NB_TaskStatus nb_COAP_read_get(const char* param);

NB_TaskStatus nb_COAP_close_run(const char* param);
NB_TaskStatus nb_COAP_close_set(const char* param);
NB_TaskStatus nb_COAP_close_get(const char* param);

NB_TaskStatus nb_COAP_uri_run(const char* param);

NB_TaskStatus nb_UDP_open_run(const char* param);
NB_TaskStatus nb_UDP_open_set(const char* param);
NB_TaskStatus nb_UDP_open_get(const char* param);

NB_TaskStatus nb_UDP_send_run(const char* param);
NB_TaskStatus nb_UDP_send_set(const char* param);

NB_TaskStatus nb_UDP_data_run(const char* param);
NB_TaskStatus nb_UDP_data_set(const char* param);

NB_TaskStatus nb_UDP_sendex_run(const char* param);
NB_TaskStatus nb_UDP_sendex_set(const char* param);

NB_TaskStatus nb_UDP_qird_run(const char* param);
NB_TaskStatus nb_UDP_qird_set(const char* param);

NB_TaskStatus nb_UDP_read_run(const char* param);
NB_TaskStatus nb_UDP_read_get(const char* param);

NB_TaskStatus nb_UDP_close_run(const char* param);
NB_TaskStatus nb_UDP_close_set(const char* param);

NB_TaskStatus nb_UDP_uri_run(const char* param);

NB_TaskStatus nb_QSSLCFG_run(const char* param);
NB_TaskStatus nb_QSSLCFG_set(const char* param);


NB_TaskStatus nb_QMTCFGSSL_run(const char* param);
NB_TaskStatus nb_QMTCFGSSL_set(const char* param);

NB_TaskStatus nb_CACERT_run(const char* param);
NB_TaskStatus nb_CACERT_set(const char* param);

NB_TaskStatus nb_CLICERT_run(const char* param);
NB_TaskStatus nb_CLICERT_set(const char* param);

NB_TaskStatus nb_CLIKEY_run(const char* param);
NB_TaskStatus nb_CLIKEY_set(const char* param);

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

NB_TaskStatus nb_MQTT_disc_run(const char* param);
NB_TaskStatus nb_MQTT_disc_set(const char* param);

NB_TaskStatus nb_MQTT_close_run(const char* param);
NB_TaskStatus nb_MQTT_close_set(const char* param);

NB_TaskStatus nb_MQTT_uri_run(const char* param);

NB_TaskStatus nb_TCP_open_run(const char* param);
NB_TaskStatus nb_TCP_open_set(const char* param);
NB_TaskStatus nb_TCP_open_get(const char* param);

NB_TaskStatus nb_TCP_send_run(const char* param);
NB_TaskStatus nb_TCP_send_set(const char* param);

NB_TaskStatus nb_TCP_data_run(const char* param);
NB_TaskStatus nb_TCP_data_set(const char* param);

NB_TaskStatus nb_TCP_sendex_run(const char* param);
NB_TaskStatus nb_TCP_sendex_set(const char* param);

NB_TaskStatus nb_TCP_qird_run(const char* param);
NB_TaskStatus nb_TCP_qird_set(const char* param);

NB_TaskStatus nb_TCP_read_run(const char* param);
NB_TaskStatus nb_TCP_read_get(const char* param);

NB_TaskStatus nb_TCP_close_run(const char* param);
NB_TaskStatus nb_TCP_close_set(const char* param);
NB_TaskStatus nb_TCP_close_get(const char* param);

NB_TaskStatus nb_TCP_uri_run(const char* param);

struct NBTASK 
{

	const char 	*ATRecStrOK;                       			/*< Pre-correctly received data */
	const char 	*ATRecStrError;                      		/*< Pre-correctly received data */
	const int 	cmd_num;																/*< CMD number*/

	uint16_t 		time_out;																/*< Instruction timeout,unit: ms*/

  NB_TaskStatus (*run)(const char* param);      			/*< Function entry */
	NB_TaskStatus (*set)(const char* param);      			/*< Function entry */
	NB_TaskStatus (*get)(const char* param);      			/*< Function entry */
	

};

static const struct NBTASK NBTask[] =
{
/**************** AT	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = NULL,
		.cmd_num        = _AT,

		.time_out 			= 300,

    .run 						= nb_at_run,
		.set						= nb_null_run,
		.get						= nb_null_run,

  },
/**************** ATE	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _ATE,

		.time_out 			= 300,
    .run 						= nb_ate_run,
		.set						= nb_null_run,
		.get						= nb_null_run,

  },
/**************** CGSN	****************/
	{		

		.ATRecStrOK     = "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_IMSI,

		.time_out 			= 300,
    .run 						= nb_null_run,
		.set						= nb_null_run,
		.get						= nb_cgsn_get,

  },
/**************** CIMI	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_IMEI,

		.time_out 			= 300,
    .run 						= nb_null_run,
		.set						= nb_null_run,
		.get						= nb_cimi_get,

  },
/****************QICFG	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QICFG,

		.time_out 			= 500,
    .run 						= nb_qicfg_run,
		.set						= nb_qicfg_set,
		.get						= nb_qicfg_get,

  },
/**************** CGMM	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CGMM,

		.time_out 			= 300,
    .run 						= nb_cgmm_run,
		.set						= nb_null_run,
		.get						= nb_cgmm_get,

  },
/****************QBAND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QBAND,

		.time_out 			= 500,
    .run 						= nb_qband_run,
		.set						= nb_qband_set,
		.get						= nb_qband_get,

  },	
/****************IOTMOD	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_IOTM,

		.time_out 			= 500,
    .run 						= nb_iotm_run,
		.set						= nb_iotm_set,
		.get						= nb_iotm_get,

  },
/**************** CCLK	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CCLK,

		.time_out 			= 300,
    .run 						= nb_cclk_run,
		.set						= nb_null_run,
		.get						= nb_cclk_get,

  },
/**************** CCLK2	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CCLK2,

		.time_out 			= 300,
    .run 						= nb_cclk2_run,
		.set						= nb_null_run,
		.get						= nb_cclk2_get,

  },
/**************** CGDCONT	****************/
	{

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CGDCONT,

		.time_out 			= 300,
    .run 						= nb_cgdcont_run,
		.set						= nb_cgdcont_set,
		.get						= nb_cgdcont_get,

  },
/**************** QGPS	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QGPS,

		.time_out 			= 300,

    .run 						= nb_qgps_run,
		.set						= nb_qgps_set,
		.get						= nb_null_run,

  },
/**************** CSQ	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CSQ,

		.time_out 			= 300,

    .run 						= nb_null_run,
		.set						= nb_null_run,
		.get						= nb_csq_get,

  },
/**************** QNWINFO	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QNWINFO,

		.time_out 			= 300,
    .run 						= nb_qnwinfo_run,
		.set						= nb_null_run,
		.get						= nb_qnwinfo_get,

  },
/**************** QICSGP	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QICSGP,

		.time_out 			= 500,

    .run 						= nb_qicsgp_run,
		.set						= nb_qicsgp_set,
		.get						= nb_qicsgp_get,

  },
/**************** QIDEACT	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QIDEACT,

		.time_out 			= 500,

    .run 						= nb_qideact_run,
		.set						= nb_qideact_set,
		.get						= nb_qideact_get,

  },
/**************** QIACT	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QIACT,

		.time_out 			= 500,

    .run 						= nb_qiact_run,
		.set						= nb_qiact_set,
		.get						= nb_qiact_get,

  },	
/**************** QDNSCFG	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QDNSCFG,

		.time_out 			= 300,

    .run 						= nb_qdnscfg_run,
		.set						= nb_qdnscfg_set,
		.get						= nb_null_run,

  },
/**************** QDNS	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QDNS,

		.time_out 			= 2000,

    .run 						= nb_qdns_run,
		.set						= nb_qdns_set,
		.get						= nb_qdns_get,

  },
/**************** QGPSLOC	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QGPSLOC,

		.time_out 			= 1000,

    .run 						= nb_qgpsloc_run,
		.set						= nb_null_run,
		.get						= nb_qgpsloc_get,

  },
/**************** QGPSEND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QGPSEND,

		.time_out 			= 500,

    .run 						= nb_qgpssend_run,
		.set						= nb_qgpssend_set,
		.get						= nb_null_run,

  },
/**************** COAP_QCOAPCFG	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_CONFIG,

		.time_out 			= 500,

    .run 						= nb_COAP_config_run,
		.set						= nb_COAP_config_set,
		.get						= nb_COAP_config_get,

  },
/**************** COAP_OPEN	****************/
	{

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPEN,

		.time_out 			= 500,

    .run 						= nb_COAP_open_run,
		.set						= nb_COAP_open_set,
		.get						= nb_COAP_open_get,

  },
/**************** COAP_QCOAPHEADER	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_HEADER,

		.time_out 			= 500,

    .run 						= nb_COAP_header_run,
		.set						= nb_COAP_header_set,
		.get						= nb_COAP_header_get,

  },
/**************** COAP_QCOAPOPTION1	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION1,

		.time_out 			= 500,

    .run 						= nb_COAP_option1_run,
		.set						= nb_COAP_option1_set,
		.get						= nb_COAP_option1_get,

  },
/**************** COAP_QCOAPOPTION2	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION2,

		.time_out 			= 500,

    .run 						= nb_COAP_option2_run,
		.set						= nb_COAP_option2_set,
		.get						= nb_COAP_option2_get,

  },
/**************** COAP_QCOAPOPTION3	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION3,

		.time_out 			= 500,

    .run 						= nb_COAP_option3_run,
		.set						= nb_COAP_option3_set,
		.get						= nb_COAP_option3_get,

  },
/**************** COAP_QCOAPOPTION4	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION4,

		.time_out 			= 500,

    .run 						= nb_COAP_option4_run,
		.set						= nb_COAP_option4_set,
		.get						= nb_COAP_option4_get,

  },
/**************** COAP_QCOAPOPTION5	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION5,

		.time_out 			= 500,

    .run 						= nb_COAP_option5_run,
		.set						= nb_COAP_option5_set,
		.get						= nb_COAP_option5_get,

  },
/**************** COAP_QCOAPOPTION6	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION6,

		.time_out 			= 500,

    .run 						= nb_COAP_option6_run,
		.set						= nb_COAP_option6_set,
		.get						= nb_COAP_option6_get,

  },
/**************** COAP_QCOAPOPTION7	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION7,

		.time_out 			= 500,

    .run 						= nb_COAP_option7_run,
		.set						= nb_COAP_option7_set,
		.get						= nb_COAP_option7_get,

  },
/**************** COAP_QCOAPOPTION8	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION8,

		.time_out 			= 500,

    .run 						= nb_COAP_option8_run,
		.set						= nb_COAP_option8_set,
		.get						= nb_COAP_option8_get,

  },	
/**************** COAP_SEND_CONFIG	****************/
	{		

		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_SEND_CONFIG,

		.time_out 			= 500,

    .run 						= nb_COAP_send_config_run,
		.set						= nb_COAP_send_config_set,
		.get						= nb_null_run,

  },
/**************** COAP_SEND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_SEND,

		.time_out 			= 1000,

    .run 						= nb_COAP_send_run,
		.set						= nb_COAP_send_set,
		.get						= nb_null_run,

  },
/**************** COAP_READ	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_READ,

		.time_out 			= 500,

    .run 						= nb_COAP_read_run,
		.set						= nb_null_run,
		.get						= nb_COAP_read_get,

  },
/**************** COAP_CLOSED	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_CLOSE,

		.time_out 			= 500,

    .run 						= nb_COAP_close_run,
		.set						= nb_COAP_close_set,
		.get						= nb_COAP_close_get,

  },		
/**************** COAP_URI	****************/

	{		

		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_COAP_URI,

		.time_out 			= 0,

    .run 						= nb_COAP_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,

  },	
/**************** UDP_OPEN	****************/
	{

		.ATRecStrOK  		= "+QIOPEN: 0,0",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_OPEN,

		.time_out 			= 1500,

    .run 						= nb_UDP_open_run,
		.set						= nb_UDP_open_set,
		.get						= nb_null_run,

  },
	
/**************** UDP_SEND	****************/
	{		

		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_SEND,

		.time_out 			= 1000,

    .run 						= nb_UDP_send_run,
		.set						= nb_UDP_send_set,
		.get						= nb_null_run,

  },
/**************** UDP_DATA	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_DATA,

		.time_out 			= 1000,
    .run 						= nb_UDP_data_run,
		.set						= nb_UDP_data_set,
		.get						= nb_null_run,

  },
/**************** UDP_SENDEX	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_SENDEX,

		.time_out 			= 1000,

    .run 						= nb_UDP_sendex_run,
		.set						= nb_UDP_sendex_set,
		.get						= nb_null_run,

  },
/**************** UDP_QIRD	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_QIRD,

		.time_out 			= 1000,

    .run 						= nb_UDP_qird_run,
		.set						= nb_UDP_qird_set,
		.get						= nb_null_run,

  },
/**************** UDP_READ	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_READ,

		.time_out 			= 500,

    .run 						= nb_UDP_read_run,
		.set						= nb_null_run,
		.get						= nb_UDP_read_get,

  },
/**************** UDP_CLOSED	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_CLOSE,

		.time_out 			= 500,

    .run 						= nb_UDP_close_run,
		.set						= nb_UDP_close_set,
		.get						= nb_null_run,

  },
/**************** UDP_URI	****************/
	{		

		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_UDP_URI,

		.time_out 			= 0,

    .run 						= nb_UDP_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,

  },
/**************** QSSLCFG	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QSSLCFG,

		.time_out 			= 300,

    .run 						= nb_QSSLCFG_run,
		.set						= nb_QSSLCFG_set,
		.get						= nb_null_run,

  },	
/**************** QMTCFGSSL	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QMTCFG_SSL,

		.time_out 			= 300,

    .run 						= nb_QMTCFGSSL_run,
		.set						= nb_QMTCFGSSL_set,
		.get						= nb_null_run,

  },
/**************** CACERT	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CACERT,

		.time_out 			= 300,

    .run 						= nb_CACERT_run,
		.set						= nb_CACERT_set,
		.get						= nb_null_run,

  },
/**************** CLICERT	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CLICERT,

		.time_out 			= 300,

    .run 						= nb_CLICERT_run,
		.set						= nb_CLICERT_set,
		.get						= nb_null_run,

  },
/**************** CLIKEY	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CLIKEY,

		.time_out 			= 300,

    .run 						= nb_CLIKEY_run,
		.set						= nb_CLIKEY_set,
		.get						= nb_null_run,

  },
/**************** MQTT configuration	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_Config,

		.time_out 			= 200,

    .run 						= nb_null_run,
		.set						= nb_MQTT_config_set,
		.get						= nb_null_run,

  },
/**************** MQTT_OPEN	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_OPEN,

		.time_out 			= 1200,

    .run 						= nb_MQTT_open_run,
		.set						= nb_MQTT_open_set,
		.get						= nb_null_run,

  },
/**************** MQTT_CONN	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_CONN,

		.time_out 			= 500,

    .run 						= nb_MQTT_conn_run,
		.set						= nb_MQTT_conn_set,
		.get						= nb_null_run,

  },
/**************** MQTT_SUB	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_SUB,

		.time_out 			= 500,
    .run 						= nb_MQTT_sub_run,
		.set						= nb_MQTT_sub_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB	****************/
	{		

		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUBEX,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub_run,
		.set						= nb_MQTT_pub_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB1	****************/
	{		
 
		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB1,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub1_run,
		.set						= nb_MQTT_pub1_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB2	****************/
	{		

		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB2,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub2_run,
		.set						= nb_MQTT_pub2_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB3	****************/
	{		

		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB3,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub3_run,
		.set						= nb_MQTT_pub3_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB5	****************/
	{		

		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB5,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub5_run,
		.set						= nb_MQTT_pub5_set,
		.get						= nb_null_run,

  },
/**************** MQTT_SEND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_SEND,

		.time_out 			= 1000,
    .run 						= nb_MQTT_send_run,
		.set						= nb_MQTT_send_set,
		.get						= nb_null_run,

  },
/**************** MQTT_READ	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_READ,

		.time_out 			= 500,
    .run 						= nb_MQTT_data_read_run,
		.set						= nb_MQTT_data_read_set,
		.get						= nb_null_run,

  },
/**************** MQTT_DISC	****************/
	{		
 
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_DISC,

		.time_out 			= 500,

    .run 						= nb_MQTT_disc_run,
		.set						= nb_MQTT_disc_set,
		.get						= nb_null_run,

  },
/**************** MQTT_CLOSE	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_CLOSE,

		.time_out 			= 500,

    .run 						= nb_MQTT_close_run,
		.set						= nb_MQTT_close_set,
		.get						= nb_null_run,

  },
/**************** MQTT_URI	****************/
	{		

		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_MQTT_URI,

		.time_out 			= 0,

    .run 						= nb_MQTT_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,

  },
/**************** TCP_OPEN	****************/
	{		

		.ATRecStrOK  		= "+QIOPEN: 0,0",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_OPEN,

		.time_out 			= 1500,

    .run 						= nb_TCP_open_run,
		.set						= nb_TCP_open_set,
		.get						= nb_null_run,

  },
/**************** TCP_SEND	****************/
	{		

		.ATRecStrOK  		= ">",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_SEND,

		.time_out 			= 1000,

    .run 						= nb_TCP_send_run,
		.set						= nb_TCP_send_set,
		.get						= nb_null_run,

  },
/**************** TCP_DATA	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_DATA,

		.time_out 			= 1000,
    .run 						= nb_TCP_data_run,
		.set						= nb_TCP_data_set,
		.get						= nb_null_run,

  },
/**************** TCP_SENDEX	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_SENDEX,

		.time_out 			= 1000,

    .run 						= nb_TCP_sendex_run,
		.set						= nb_TCP_sendex_set,
		.get						= nb_null_run,

  },
/**************** TCP_QIRD	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_QIRD,

		.time_out 			= 1000,

    .run 						= nb_TCP_qird_run,
		.set						= nb_TCP_qird_set,
		.get						= nb_null_run,

  },
/**************** TCP_READ	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_READ,

		.time_out 			= 500,

    .run 						= nb_TCP_read_run,
		.set						= nb_null_run,
		.get						= nb_TCP_read_get,

  },
/**************** TCP_CLOSED	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_CLOSE,

		.time_out 			= 4000,

    .run 						= nb_TCP_close_run,
		.set						= nb_TCP_close_set,
		.get						= nb_null_run,

  },
/**************** TCP_URI	****************/
	{

		.ATRecStrOK  		= NULL,
		.ATRecStrError  = NULL,
		.cmd_num        = _AT_TCP_URI,

		.time_out 			= 0,

    .run 						= nb_TCP_uri_run,
		.set						= nb_null_run,
		.get						= nb_null_run,

  },
};

#ifdef __cplusplus
}
#endif
void stored_datalog(void);
NB_TaskStatus nb_at_send(const struct NBTASK *NB_Task);
ATCmdNum NBTASK(uint8_t *task);
#endif 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
