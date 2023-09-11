#include "../include/config.h"
#include "../include/debug_manager.h"
#include <stdio.h>
#include <string.h>

static int StdoutDbgPrint(char * strData);

static T_DebugOPr g_tStdoutDebgOpr = {
    .name       = "stdout",
    .isCanUse   = 1,
    .DebugPrint = StdoutDbgPrint,
};

static int StdoutDbgPrint(char * strData)
{
    /* 直接把输出信息用printf打印即可 */
    printf("%s", strData);
    return strlen(strData);
}

int StdoutInit(void)
{
    return RegisterDebugOpr(&g_tStdoutDebgOpr);
}
