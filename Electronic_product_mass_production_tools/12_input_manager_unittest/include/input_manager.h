#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <sys/time.h>

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET   2
#define TOUCHSCREEN      0
#define NETTINPUT        1

struct timeval;

typedef struct InputEvent
{
    struct timeval tTime;
    int iType;
    int iX;
    int iY;
    int iPressure;
    char str[1024];
} InputEvent, *pInputEvent;

typedef struct InputDevice
{
    char * name;
    int (*GetInputEvent)(pInputEvent ptInputEvent);
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    struct InputDevice * ptNext;
} InputDevice, *pInputDevice;

void RegisterInputDevice(pInputDevice ptInputOpr);

void InputInit(void);
void InputDeviceInit(void);
int GetInputEvent(pInputEvent ptInputEvent);

#endif   // !_INPUT_MANAGER_H_