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

int main(int argc, char ** argv)
{
    int ret;
    InputEvent event;

    g_tRouchScreenDev.DeviceInit();

    while (1)
    {
        ret = g_tRouchScreenDev.GetInputEvent(&event);
        if (ret)
        {
            printf("GetInputEvent ERROR!\n");
            g_tRouchScreenDev.DeviceExit();
            return -1;
        }
        else
        {
            printf("event    = %d\n", event.iType);
            printf("x        = %d\n", event.iX);
            printf("y        = %d\n", event.iY);
            printf("pressure = %d\n", event.iPressure);
        }
    }
    g_tRouchScreenDev.DeviceExit();

    return 0;
}

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