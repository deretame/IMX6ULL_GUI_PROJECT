#include "../include/page_manager.h"
#include <stdio.h>

static void MainPageRun(void * pParams);

static PageAction g_tMainPage = {
    .name = "main",
    .Run  = MainPageRun,
};

static void MainPageRun(void * pParams)
{
    // 读取配置文件

    // 根据配置文件生成按钮以及界面
    while (10)
    {
        // 读取输入事件

        // 根据输入事件找到按钮

        // 调用按钮的OnPressed函数
    }
}

void MainPageRegister(void)
{
    PageRegister(&g_tMainPage);
}