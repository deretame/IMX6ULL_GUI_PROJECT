#include "../include/config.h"
#include "../include/debug_manager.h"
#include <stdarg.h>
#include <stdio.h>

static int StdoutDbgPrint(const char * pcFormat, ...);

static T_DebugOPr g_tStdoutDebgOpr = {
    .name       = "stdout",
    .isCanUse   = 1,
    .DebugPrint = StdoutDbgPrint,
};

static int StdoutDbgPrint(const char * pcFormat, ...)
{
    /* 直接把输出信息用printf打印即可 */
    va_list tArgs;
    int iNum;

    va_start(tArgs, pcFormat);
    iNum = vfprintf(stdout, pcFormat, tArgs);
    va_end(tArgs);

    return iNum;
}

int StdoutInit(void)
{
    return RegisterDebugOpr(&g_tStdoutDebgOpr);
}
