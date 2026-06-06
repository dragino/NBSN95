#ifndef _lwm2m_h
#define _lwm2m_h

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"



typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

#define Version                     1
#define Configmable                 0
#define Acknowledgement             2

/*-----------Option-----------*/
#define observe_optDelta            6
#define uri_Path_optDelta           11
#define conten_tFormat_optDelta  	12
#define uri_Query_optDelta  	    15
#define block_optDelta  		    23

/*-----------Other-----------*/
#define add_Server            "183.230.40.40:5683"


typedef enum
{
	GET = 0x01,
	POST,
	PUT,
	DELETE,
}COAPType;

typedef enum
{
	Created = 0x41,	//2.01
	Deleta,			//2.02
	Content = 0x45,	//2.05
	NotFound = 0x84	//4.04
}HTTPCODE;

typedef enum
{
	Uri_Path = 11,
	Content_Format = 12,
	Uri_Query = 15
}OPTIONNAME;

typedef enum
{
	Application_link_format = 0x28
}CONTENT_TYPE;

typedef enum
{	
	emptyMessage 	 = 0x00,
	conMessage 		 = 0x40,
	ackMessage 		 = 0x60,
	errorMessage 	 = 0xFF,
}CODEMessageType;

typedef struct
{
	uint8_t  head;
	uint8_t  code;
	uint16_t messageID;
	uint32_t token_l;	
    uint32_t token_h;
	uint8_t  optdesc;
}COAP;

char* connectMsg(char* buff,const char* uri);
char* sendDataMsg(char* buff, const char* payload);
char* recieveMsg(char* buff, const char* recieve_data);
char* deletetMsg(char* buff, const char* imei);

#endif
