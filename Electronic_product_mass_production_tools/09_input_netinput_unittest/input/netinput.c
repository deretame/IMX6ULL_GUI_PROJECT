#include "../include/input_manager.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

#define SERVER_PORT 8888

static int iSocketServer;

static int NetInputGetInputEvent(pInputEvent ptInputEvent);
static int NetInputDeviceInit(void);
static int NetInputDeviceExit(void);

static InputDevice g_tRouchScreenDev = {
    .name          = "NetInput",
    .GetInputEvent = NetInputGetInputEvent,
    .DeviceInit    = NetInputDeviceInit,
    .DeviceExit    = NetInputDeviceExit,
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
            printf("str   = %s\n", event.str);
            printf("event = %d\n", event.iType);
            printf("time  = %ld\n", event.tTime.tv_sec);
        }
    }
    g_tRouchScreenDev.DeviceExit();

    return 0;
}

static int NetInputGetInputEvent(pInputEvent ptInputEvent)
{
    int iAddrLen;
    int iRecvLen;
    unsigned char ucRecvBuf[1000];
    struct sockaddr_in tSocketClientAddr;

    iAddrLen = sizeof(struct sockaddr);
    iRecvLen = recvfrom(iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, (socklen_t *)&iAddrLen);
    if (iRecvLen > 0)
    {
        ucRecvBuf[iRecvLen] = '\0';
        // printf("Get Msg From %s : %s\n", inet_ntoa(tSocketClientAddr.sin_addr), ucRecvBuf);
        ptInputEvent->iType = INPUT_TYPE_NET;
        gettimeofday(&ptInputEvent->tTime, NULL);
        strncpy(ptInputEvent->str, (char *)ucRecvBuf, 1000);
        ptInputEvent->str[999] = '\0';
        return 0;
    }

    return -1;
}

static int NetInputDeviceInit(void)
{
    int iRet;
    struct sockaddr_in tSocketServerAddr;

    iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == iSocketServer)
    {
        printf("socket error!\n");
        return -1;
    }

    tSocketServerAddr.sin_family      = AF_INET;
    tSocketServerAddr.sin_port        = htons(SERVER_PORT); /* host to net, short */
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        printf("bind error!\n");
        return -1;
    }

    return 0;
}

static int NetInputDeviceExit(void)
{
    close(iSocketServer);
    return 0;
}
