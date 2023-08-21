#include "../include/input_manager.h"
#include <stdio.h>
#include <tslib.h>

struct InputDevice * ptNext;
struct tsdev * g_ts;

static int TouchScreenGetInputEvent(pInputEvent ptInputEvent);
static int TouchScreenDeviceInit(void);
static int TouchScreenDeviceExit(void);

static InputDevice g_tRouchScreenDev = {
    .name          = "touchscreen",
    .GetInputEvent = TouchScreenGetInputEvent,
    .DeviceInit    = TouchScreenDeviceInit,
    .DeviceExit    = TouchScreenDeviceExit,
};

static int TouchScreenGetInputEvent(pInputEvent ptInputEvent)
{
    struct ts_sample samp;
    int ret;

    ret = ts_read(g_ts, &samp, 1);
    if (ret != 1)
        return -1;

    ptInputEvent->iType     = INPUT_TYPE_TOUCH;
    ptInputEvent->iX        = samp.x;
    ptInputEvent->iY        = samp.y;
    ptInputEvent->iPressure = samp.pressure;
    ptInputEvent->tTime     = samp.tv;

    return 0;
}

static int TouchScreenDeviceInit(void)
{
    g_ts = ts_setup(NULL, 0);
    if (!g_ts)
    {
        printf("ts_setup err\n");
        return -1;
    }

    return 0;
}

static int TouchScreenDeviceExit(void)
{
    ts_close(g_ts);
    return 0;
}

void TouchScreenDevRegister(void)
{
    RegisterInputDevice(&g_tRouchScreenDev);
}