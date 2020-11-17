#ifndef __UPLOAD_H__
#define __UPLOAD_H__

#include "nbInit.h"
#include "COAP_Statck.h"

NBState sockCreat(void);
NBState dataSendPut(char* payload);
NBState dataSendUDP(char* payload);
NBState sockClosd(void);
#endif
