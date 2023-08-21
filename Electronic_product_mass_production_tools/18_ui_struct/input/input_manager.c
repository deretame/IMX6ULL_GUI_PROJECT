#include "../include/input_manager.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tConVar = PTHREAD_COND_INITIALIZER;
static pInputDevice g_InputDevs = NULL;

static void * input_recv_thread_func(void * data);

// 实现环形缓冲区
#define BUFFER_LEN 20
static int g_iread  = 0;
static int g_iwrite = 0;
static InputEvent g_atInputEvents[BUFFER_LEN];

static int isInputBufferFull(void)
{
    return (g_iread == ((g_iwrite + 1) % BUFFER_LEN));
}

static int isInputBufferEmpty(void)
{
    return (g_iread == g_iwrite);
}

static void PutInputEventToBuffer(pInputEvent ptInputEvent)
{
    if (!isInputBufferFull())
    {
        g_atInputEvents[g_iwrite] = *ptInputEvent;
        g_iwrite                  = (g_iwrite + 1) % BUFFER_LEN;
    }
}

static int GetInputEventFromBuffer(pInputEvent ptInputEvent)
{
    if (!isInputBufferEmpty())
    {
        *ptInputEvent = g_atInputEvents[g_iread];
        g_iread       = (g_iread + 1) % BUFFER_LEN;
        return 1;
    }
    return 0;
}

// 完成实现

void RegisterInputDevice(pInputDevice ptInputOpr)
{
    ptInputOpr->ptNext = g_InputDevs;
    g_InputDevs        = ptInputOpr;
}

void InputInit(void)
{
    /* 注册触摸屏 */
    extern void TouchScreenDevRegister(void);
    TouchScreenDevRegister();
    /* 注册网络输入 */
    extern void NetinputDevRegister(void);
    NetinputDevRegister();
}

void InputDeviceInit(void)
{
    int ret;
    pthread_t tid;
    pInputDevice pTemp = g_InputDevs;

    /* 初始化每个输入设备,并创建线程 */
    while (pTemp)
    {
        /* 初始化设备 */
        ret = pTemp->DeviceInit();

        /* 创建线程 */
        if (!ret)
        {
            ret = pthread_create(&tid, NULL, input_recv_thread_func, pTemp);
        }

        pTemp = pTemp->ptNext;
    }
}

int GetInputEvent(pInputEvent ptInputEvent)
{
    InputEvent tEvent;
    int ret;

    /* 无数据休眠 */
    pthread_mutex_lock(&g_tMutex);
    if (GetInputEventFromBuffer(&tEvent))
    {
        *ptInputEvent = tEvent;
        pthread_mutex_unlock(&g_tMutex);
        return 0;
    }
    else
    {
        // 休眠等待
        pthread_cond_wait(&g_tConVar, &g_tMutex);
        if (GetInputEventFromBuffer(&tEvent))
        {
            *ptInputEvent = tEvent;
            ret = 0;
        }
        else
        {
            ret = -1;
        }
        pthread_mutex_unlock(&g_tMutex);
    }
    return ret;
}

static void * input_recv_thread_func(void * data)
{
    pInputDevice ptInputDev = (pInputDevice)data;
    InputEvent tEvent;
    int ret;

    while (1)
    {
        /* 读取数据 */
        ret = ptInputDev->GetInputEvent(&tEvent);
        if (!ret)
        {
            /* 保存数据 */
            pthread_mutex_lock(&g_tMutex);
            PutInputEventToBuffer(&tEvent);

            /* 唤醒等待数据的线程 */
            pthread_cond_signal(&g_tConVar);
            pthread_mutex_unlock(&g_tMutex);
        }
    }

    return NULL;
}