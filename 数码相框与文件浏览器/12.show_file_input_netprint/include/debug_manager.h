#ifndef _DEBUG_MANAGER_H
#define _DEBUG_MANAGER_H

typedef struct DebugOpr
{
    char * name;
    int isCanUse;
    int (*DebugInit)(void);
    int (*DebugExit)(void);
    int (*DebugPrint)(const char * pcFormat, ...);
    struct DebugOpr * ptNext;
} T_DebugOPr, *PT_DebugOpr;

// debug_manager.c
int RegisterDebugOpr(PT_DebugOpr ptDebugOpr);
int SetDbgLevel(char * strBuf);
int SetDbgChanel(char * strBuf);

// stdout.c
int StdoutInit(void);

// netprint.c
int NetPrintInit(void);

#endif