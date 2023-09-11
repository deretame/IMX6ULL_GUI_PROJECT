#include "../include/debug_manager.h"
#include "../include/config.h"
#include <string.h>

static PT_DebugOpr g_ptDebugOprHead;
static int g_iDbgLevelLimint = 8;

int RegisterDebugOpr(PT_DebugOpr ptDebugOpr)
{
    PT_DebugOpr ptTmp;

    if (!g_ptDebugOprHead)
    {
        g_ptDebugOprHead   = ptDebugOpr;
        ptDebugOpr->ptNext = NULL;
    }
    else
    {
        ptTmp = g_ptDebugOprHead;
        while (ptTmp->ptNext)
        {
            ptTmp = ptTmp->ptNext;
        }
        ptTmp->ptNext      = ptDebugOpr;
        ptDebugOpr->ptNext = NULL;
    }

    return 0;
}

void ShowDebugOpr(void)
{
    int i             = 0;
    PT_DebugOpr ptTmp = g_ptDebugOprHead;

    while (ptTmp)
    {
        printf("%02d %s\n", i++, ptTmp->name);
        ptTmp = ptTmp->ptNext;
    }
}

PT_DebugOpr GetDebugOpr(char * pcName)
{
    PT_DebugOpr ptTmp = g_ptDebugOprHead;

    while (ptTmp)
    {
        if (strcmp(ptTmp->name, pcName) == 0)
        {
            return ptTmp;
        }
        ptTmp = ptTmp->ptNext;
    }
    return NULL;
}

/* strBuf = "dbglevel=<0-7>" */
int SetDbgLevel(char * strBuf)
{
    g_iDbgLevelLimint = strBuf[9] - '0';
    return 0;
}

/*
 * stdout=0:           : 关闭stdout打印
 * stdout=1:           : 打开stdout打印
 * netprint=0:         : 关闭netprint打印
 * netprint=0:         : 打开netprint打印
 */
int SetDbgChanel(char * strBuf)
{
    char * pStrTmp;
    char strName[100];
    PT_DebugOpr ptTmp;

    pStrTmp = strstr(strBuf, "=");
    if (!pStrTmp)
        return -1;
    else
    {
        strncpy(strName, strBuf, pStrTmp - strBuf);
        strName[pStrTmp - strBuf] = '\0';

        ptTmp = GetDebugOpr(strName);
        if (!ptTmp)
            return -1;

        if (pStrTmp[1] == '0')
            ptTmp->isCanUse = 0;
        else
            ptTmp->isCanUse = 1;

        return 0;
    }
}

int DebugInit(void)
{
    int iError;

    iError = StdoutInit();

    return iError;
}
