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

#define stack 							"D-BC660K-003"
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

#define QCOAPCFG		 "+QCOAPCFG"
#define QCOAPOPEN		 "+QCOAPOPEN"
#define QCOAPHEAD		 "+QCOAPHEAD"
#define QCOAPOPTION		"+QCOAPOPTION"
#define QCOAPSEND		"+QCOAPSEND"
#define QCOAPCLOSE		"+QCOAPCLOSE"

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

/*COAP*/
  _AT_COAP_CONFIG,
	_AT_COAP_OPEN,
	_AT_COAP_QCOAPHEAD,
	_AT_COAP_OPTION1,
	_AT_COAP_OPTION2,
	_AT_COAP_OPTION3,
	_AT_COAP_OPTION4,
  _AT_COAP_SEND_CONFIG,
	_AT_COAP_SEND,
	_AT_COAP_SEND_HEX,
  _AT_COAP_READ,
	_AT_COAP_CLOSE,
	_AT_COAP_URI,
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
  NB_QCOAPOPEN_SUCC,
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

NB_TaskStatus nb_COAP_config_set(const char* param);

NB_TaskStatus nb_COAP_open_run(const char* param);
NB_TaskStatus nb_COAP_open_set(const char* param);

NB_TaskStatus nb_COAP_head_run(const char* param);
NB_TaskStatus nb_COAP_head_set(const char* param);

NB_TaskStatus nb_COAP_option1_run(const char* param);
NB_TaskStatus nb_COAP_option1_set(const char* param);	

NB_TaskStatus nb_COAP_option2_run(const char* param);
NB_TaskStatus nb_COAP_option2_set(const char* param);	

NB_TaskStatus nb_COAP_option3_run(const char* param);
NB_TaskStatus nb_COAP_option3_set(const char* param);	

NB_TaskStatus nb_COAP_option4_run(const char* param);
NB_TaskStatus nb_COAP_option4_set(const char* param);	

NB_TaskStatus nb_COAP_send_config_run(const char* param);
NB_TaskStatus nb_COAP_send_config_set(const char* param);
	
NB_TaskStatus nb_COAP_send_run(const char* param);
NB_TaskStatus nb_COAP_send_set(const char* param);
	
NB_TaskStatus nb_COAP_send_hex_run(const char* param);
NB_TaskStatus nb_COAP_send_hex_set(const char* param);

NB_TaskStatus nb_COAP_read_run(const char* param);
NB_TaskStatus nb_COAP_read_get(const char* param);	

NB_TaskStatus nb_COAP_close_run(const char* param);
NB_TaskStatus nb_COAP_close_set(const char* param);	

NB_TaskStatus nb_COAP_uri_run(const char* param);
	
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
/**************** QCFGEV	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QCFGEV,

		.time_out 			= 300,
		
    .run 						= nb_qcfgev_run,
		.set						= nb_qcfgev_set,
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
/**************** CFUNSTA	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CFUNSTA,

		.time_out 			= 2000,
    .run 						= nb_cfunsta_run,
		.set						= nb_cfunsta_set,
		.get						= nb_cfunsta_get,

  },
/**************** CFUNOFF	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CFUNOFF,

		.time_out 			= 2000,
    .run 						= nb_cfunoff_run,
		.set						= nb_cfunoff_set,
		.get						= nb_cfunoff_get,

  },
/**************** CFUNEND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CFUNEND,

		.time_out 			= 2000,
    .run 						= nb_cfun_run,
		.set						= nb_cfun_set,
		.get						= nb_cfun_get,

  },
/**************** QSCLK	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QSCLK,

		.time_out 			= 500,
    .run 						= nb_qsclk_run,
		.set						= nb_qsclk_set,
		.get						= nb_qsclk_get,

  },
/**************** QSCLKOFF	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QSCLKOFF,

		.time_out 			= 500,
    .run 						= nb_qsclkoff_run,
		.set						= nb_qsclkoff_set,
		.get						= nb_qsclkoff_get,

  },
/**************** QBAND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QBAND,

		.time_out 			= 300,
    .run 						= nb_qband_run,
		.set						= nb_qband_set,
		.get						= nb_qband_get,

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
/**************** CPSMS	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_CPSMS,

		.time_out 			= 300,

    .run 						= nb_cpsms_run,
		.set						= nb_cpsms_set,
		.get						= nb_cpsms_get,

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

		.time_out 			= 1000,

    .run 						= nb_qdns_run,
		.set						= nb_qdns_set,
		.get						= nb_qdns_get,

  },
/**************** COAP_QCOAPCFG	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_CONFIG,

		.time_out 			= 500,

    .run 						= nb_null_run,
		.set						= nb_COAP_config_set,
		.get						= nb_null_run,

  },
/**************** COAP_OPEN	****************/
	{

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPEN,

		.time_out 			= 1500,

    .run 						= nb_COAP_open_run,
		.set						= nb_COAP_open_set,
		.get						= nb_null_run,

  },
/**************** COAP_QCOAPHEADER	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_QCOAPHEAD,

		.time_out 			= 500,

    .run 						= nb_COAP_head_run,
		.set						= nb_COAP_head_set,
		.get						= nb_null_run,

  },
/**************** COAP_QCOAPOPTION1	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION1,

		.time_out 			= 500,

    .run 						= nb_COAP_option1_run,
		.set						= nb_COAP_option1_set,
		.get						= nb_null_run,

  },
/**************** COAP_QCOAPOPTION2	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION2,

		.time_out 			= 500,

    .run 						= nb_COAP_option2_run,
		.set						= nb_COAP_option2_set,
		.get						= nb_null_run,

  },
/**************** COAP_QCOAPOPTION3	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION3,

		.time_out 			= 500,

    .run 						= nb_COAP_option3_run,
		.set						= nb_COAP_option3_set,
		.get						= nb_null_run,

  },
/**************** COAP_QCOAPOPTION4	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_OPTION4,

		.time_out 			= 500,

    .run 						= nb_COAP_option4_run,
		.set						= nb_COAP_option4_set,
		.get						= nb_null_run,

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
/**************** COAP_SEND_HEX	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_COAP_SEND_HEX,

		.time_out 			= 1000,

    .run 						= nb_COAP_send_hex_run,
		.set						= nb_COAP_send_hex_set,
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
		.get						= nb_null_run,

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

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_OPEN,

		.time_out 			= 500,

    .run 						= nb_UDP_open_run,
		.set						= nb_UDP_open_set,
		.get						= nb_null_run,

  },
	
/**************** UDP_SEND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_UDP_SEND,

		.time_out 			= 1000,

    .run 						= nb_UDP_send_run,
		.set						= nb_UDP_send_set,
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
		.cmd_num        = _AT_MQTT_PUB,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub_run,
		.set						= nb_MQTT_pub_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB1	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB1,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub1_run,
		.set						= nb_MQTT_pub1_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB2	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB2,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub2_run,
		.set						= nb_MQTT_pub2_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB3	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_MQTT_PUB3,

		.time_out 			= 500,
    .run 						= nb_MQTT_pub3_run,
		.set						= nb_MQTT_pub3_set,
		.get						= nb_null_run,

  },
/**************** MQTT_PUB5	****************/
	{		
 
		.ATRecStrOK  		= "OK",
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
 
		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_OPEN,

		.time_out 			= 500,

    .run 						= nb_TCP_open_run,
		.set						= nb_TCP_open_set,
		.get						= nb_null_run,

  },
/**************** TCP_SEND	****************/
	{		

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_TCP_SEND,

		.time_out 			= 1000,

    .run 						= nb_TCP_send_run,
		.set						= nb_TCP_send_set,
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

		.time_out 			= 500,

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
/****************QRST	****************/
	{

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QRST,

		.time_out 			= 2000,
    .run 						= nb_qrst_run,
		.set						= nb_null_run,
		.get						= nb_null_run,

  },
/****************QRST2	****************/
	{

		.ATRecStrOK  		= "OK",
		.ATRecStrError  = "ERROR",
		.cmd_num        = _AT_QRST2,

		.time_out 			= 2000,
    .run 						= nb_qrst2_run,
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
