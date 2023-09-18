#ifndef _VIDEO_MANAGER_H_
#define _VIDEO_MANAGER_H_

#include "config.h"
#include "pic_operation.h"

struct VideoDevice;
typedef struct VideoDevice T_VideoDevice, *PT_VideoDevice;
struct VideoOpr;
typedef struct VideoOpr T_VideoOpr, *PT_VideoOpr;

typedef struct VideoDevice
{
    int iFd;
    int iPixelFormat;
    int iWidth;
    int iHeight;
    PT_VideoOpr ptOpr;
} T_VideoDevice, *PT_VideoDevice;

typedef struct VideoBuf
{
    T_PixelDatas tPixelDatas;
    int iPixelFormat;
} T_VideoBuf, *PT_VideoBuf;

typedef struct VideoOpr
{
    int (*InitDevice)(char * strDevName, PT_VideoDevice ptVideoDevice);
    int (*ExitDevice)(PT_VideoDevice ptVideoDevice);
    int (*GetFrame)(PT_VideoDevice ptVideoDevice, PT_VideoBuf ptVideoBuf);
    int (*PutFrame)(PT_VideoDevice ptVideoDevice, PT_VideoBuf ptVideoBuf);
    int (*StartDevice)(PT_VideoDevice ptVideoDevice);
    int (*StopDevice)(PT_VideoDevice ptVideoDevice);
} T_VideoOpr, *PT_VideoOpr;

#endif   // _VIDEO_MANAGER_H_