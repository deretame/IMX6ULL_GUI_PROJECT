#ifndef _VIDEO_MANAGERE_H_
#define _VIDEO_MANAGERE_H_

#include "config.h"

typedef struct VideoDevice
{
    int iFd;
    int iPixelFoamt;
    int iWidth;
    int iHeight;

    /* 函数 */
} T_VideoDevice, *PT_VideoDevice;

typedef struct VideoBuf
{
} T_VideoBuf, *PT_VideoBuf;

typedef struct VideoOpr
{
    int (*InitDevice)(char * strDevName, PT_VideoDevice ptVideoDevice);
    int (*ExitDevice)(PT_VideoDevice ptVideoDevice);
    int (*GetFram)(PT_VideoDevice ptVideoDevice);
    int (*PutFram)();
} T_VideoOpr, *PT_VideoOpr;

#endif