#ifndef __UPLOAD_H__
#define __UPLOAD_H__

#include "nbInit.h"

NBState sockCreat(void);
NBState dataSendUDP(char* payload);
NBState sockClosd(void);
#endif
