#include "../include/config.h"
#include "../include/debug_manager.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_PORT    5678
#define PRINT_BUF_SIZE (16 * 1024)

static int g_iSocketServer;
static struct sockaddr_in g_tSocketServerAddr;
static struct sockaddr_in g_tSocketClientAddr;
static char * g_pcNetPrintBuf;
static int g_iHaveConnected = 0;
static int g_iReadPos       = 0;
static int g_iWritePos      = 0;

static pthread_t g_tSendTreadID;
static pthread_t g_tRecvTreadID;

static pthread_mutex_t g_tNetDbgSendMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tNetDbgSendConVar = PTHREAD_COND_INITIALIZER;

static int NetDbgInit(void);
static int NetDbgExit(void);
static int NetDbgPrint(char * strData);
static int GetDate(char * pcVal);
static int NetDbgInit(void);
static int NetDbgExit(void);
static int isFull(void);
static int isEmpty(void);
static int PutData(char cVal);

static T_DebugOPr g_tNetDbgOpr = {
    .name       = "netprint",
    .isCanUse   = 1,
    .DebugInit  = NetDbgInit,
    .DebugExit  = NetDbgExit,
    .DebugPrint = NetDbgPrint,
};

static void * NetDbgSendThreadFunction(void * pVoid)
{
    char strTmpBuf[512];
    char cVal;
    int i;
    int iAddrLen;

    while (1)
    {
        /* 平时是休眠状态 */
        pthread_mutex_lock(&g_tNetDbgSendMutex);
        pthread_cond_wait(&g_tNetDbgSendConVar, &g_tNetDbgSendMutex);
        pthread_mutex_unlock(&g_tNetDbgSendMutex);

        while (g_iHaveConnected && !isEmpty())
        {
            i = 0;

            /* 把环形缓冲区的数据取出来,最多取512字节 */
            while ((GetDate(&cVal) == 0) && (i < 512))
            {
                strTmpBuf[i] = cVal;
                i++;
            }
            /* 执行到这里表示被唤醒 */
            /* 用sendto函数打印给客户端 */
            iAddrLen = sizeof(struct sockaddr);
            sendto(g_iSocketServer, strTmpBuf, i, 0, (const struct sockaddr *)&g_tSocketClientAddr, iAddrLen);
        }
        pthread_mutex_unlock(&g_tNetDbgSendMutex);
    }
    return NULL;
}

static void * NetDbgRecvThreadFunction(void * pVoid)
{
    socklen_t iAddrLen;
    int iRecvLen;
    char ucRecvBuf[1000];
    struct sockaddr_in tSocketClientAddr;

    while (1)
    {
        iAddrLen = sizeof(struct sockaddr);
        iRecvLen = recvfrom(g_iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);

        if (ucRecvBuf > 0)
        {
            ucRecvBuf[iRecvLen] = '\0';
            /*
             * 解析数据
             * setclient           : 设置接收打印信息的客户端
             * dbglevel=0,1,2...   : 修改打印级别
             * stdout=0:           : 关闭stdout打印
             * stdout=1:           : 打开stdout打印
             * netprint=0:         : 关闭netprint打印
             * netprint=0:         : 打开netprint打印
             */

            if (strcmp(ucRecvBuf, "setclient") == 0)
            {
                g_tSocketClientAddr = tSocketClientAddr;
                g_iHaveConnected    = 1;
            }
            else if (strncmp(ucRecvBuf, "dbglevel=", 9))
            {
                SetDbgLevel(ucRecvBuf);
            }
            else
            {
                SetDbgChanel(ucRecvBuf);
            }
        }
    }
    return NULL;
}

static int NetDbgInit(void)
{
    /* socket初始化 */
    int iRet;

    g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_iSocketServer == -1)
    {
        printf("socket error!\n");
        return -1;
    }

    g_tSocketServerAddr.sin_family      = AF_INET;
    g_tSocketServerAddr.sin_port        = htons(SERVER_PORT); /* host to net, short */
    g_tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(g_tSocketServerAddr.sin_zero, 0, 8);

    iRet = bind(g_iSocketServer, (const struct sockaddr *)&g_tSocketServerAddr, sizeof(struct sockaddr));
    if (-1 == iRet)
    {
        printf("bind error!\n");
        return -1;
    }

    g_pcNetPrintBuf = malloc(PRINT_BUF_SIZE);
    if (g_pcNetPrintBuf == NULL)
    {
        close(g_iSocketServer);
        return -1;
    }

    /* 创建netprint发送线程:它用来发送打印消息给客户端 */
    pthread_create(&g_tSendTreadID, NULL, NetDbgSendThreadFunction, NULL);

    /* 创建netprint接收线程:用来接收控制信息,比如修改打印机别,打开或关闭打印 */
    pthread_create(&g_tRecvTreadID, NULL, NetDbgRecvThreadFunction, NULL);

    return 0;
}

static int NetDbgExit(void)
{
    /* 关闭socket,... */
    close(g_iSocketServer);
    free(g_pcNetPrintBuf);
    return 0;
}

static int isFull(void)
{
    return (((g_iWritePos + 1) % PRINT_BUF_SIZE) == g_iReadPos);
}

static int isEmpty(void)
{
    return (g_iWritePos == g_iReadPos);
}

static int PutData(char cVal)
{
    if (isFull())
        return -1;
    else
    {
        g_pcNetPrintBuf[g_iWritePos] = cVal;
        g_iWritePos                  = (g_iWritePos + 1) % PRINT_BUF_SIZE;

        return 0;
    }
}

static int GetDate(char * pcVal)
{
    if (isEmpty())
        return -1;
    else
    {
        *pcVal     = g_pcNetPrintBuf[g_iReadPos];
        g_iReadPos = (g_iReadPos + 1) % PRINT_BUF_SIZE;
        return 0;
    }
}

static int NetDbgPrint(char * strData)
{
    /* 把数据放入环形缓冲区 */
    int i;

    for (i = 0; i < strlen(strData); i++)
    {
        if (PutData(strData[i]) != 0)
            break;
    }

    /* 如果已经有客户端链接,就把数据通过网络发送给客户端 */
    /* 唤醒netprint的发送线程 */
    pthread_cond_signal(&g_tNetDbgSendConVar);

    return i;
}

int NetPrintInit(void)
{
    return RegisterDebugOpr(&g_tNetDbgOpr);
}