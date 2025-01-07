#ifndef _coap_statck_h_
#define _coap_statck_h_

#define Version     							1

#define Configmable 							0
#define Acknowledgement 					2

/*-----------Option-----------*/
#define observe_optDelta  				6
#define uri_Path_optDelta  				11
#define conten_tFormat_optDelta  	12
#define uri_Query_optDelta  			15
#define block_optDelta  					23

typedef enum
{
	GET = 1,
	POST,
	PUT,
	DELETE
}COAPType;

typedef enum
{	
	emptyMessage 	 = 0x00,
	conMessage 		 = 0x40,
	ackMessage 		 = 0x60,
	errorMessage 	 = 0xFF,
}CODEMessageType;

#include "stdio.h"
#include "string.h"
#include "common.h"
#include "COAP_PUT.h"
#include "COAP_Msgback.h"
#include "COAP_Get.h"

typedef struct
{
	uint8_t  head;
	uint8_t  code;
	uint16_t messageID;
	uint32_t token;
	
}COAP;

#endif
