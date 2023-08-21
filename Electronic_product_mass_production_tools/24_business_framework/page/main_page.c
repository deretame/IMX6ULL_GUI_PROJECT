#include "../include/page_manager.h"
#include <stdio.h>

static void MainPageRun(void * pParams);

static PageAction g_tMainPage = {
    .name = "main",
    .Run  = MainPageRun,
};

static void MainPageRun(void * pParams)
{
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

void MainPageRegister(void)
{
    PageRegister(&g_tMainPage);
}