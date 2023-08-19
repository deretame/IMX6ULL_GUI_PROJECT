#include "../include/input_manager.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static char g_buf[1000];
static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tConVar = PTHREAD_COND_INITIALIZER;
static pInputDevice g_InputDevs = NULL;

static void * input_recv_thread_func(void * data);

void RegisterDisplay(pInputDevice ptInputOpr)
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
    /* 无数据休眠 */

    /* 返回数据 */
}

static void * input_recv_thread_func(void * data)
{
    pInputDevice tInputDev = (pInputDevice)data;
    InputEvent tEvent;
    int ret;

    while (1)
    {
        /* 读取数据 */
        ret = tInputDev->GetInputEvent(&tEvent);
        if (!ret)
        {
            /* 保存数据 */

            /* 唤醒等待数据的线程 */
            pthread_mutex_lock(&g_tMutex);
            pthread_cond_wait(&g_tConVar, &g_tMutex);

            pthread_mutex_unlock(&g_tMutex);
        }
    }

    return NULL;
}