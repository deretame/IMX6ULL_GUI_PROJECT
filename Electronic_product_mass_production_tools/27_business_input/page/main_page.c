#include "../include/config.h"
#include "../include/page_manager.h"
#include "../include/ui.h"
#include <math.h>
#include <stdio.h>

#define X_GAP 5
#define Y_GAP 5

static pButton g_tButtons[ITEMCONFIG_ITEM_MAX];

static void GenerateButtons(void);
static void MainPageRun(void * pParams);

static PageAction g_tMainPage = {
    .name = "main",
    .Run  = MainPageRun,
};

static void MainPageRun(void * pParams)
{
    int error;
    // 读取配置文件
    error = ParseConfigFile();
    if (error)
    {
        printf("ParseConfigFile error");
        return;
    }

    // 根据配置文件生成按钮以及界面
    GenerateButtons();

    while (10)
    {
        // 读取输入事件

        // 根据输入事件找到按钮

        // 调用按钮的OnPressed函数
    }
}

static void GenerateButtons(void)
{
    int n;
    int row, rows;
    int col, cols;
    int width, height;
    int n_per_line;
    PDispBuff ptDispBuff;
    int xres, yres;
    int start_x, start_y;
    int pre_start_x, pre_start_y;
    pButton pbutton;
    int i = 0;

    // 算出单个按钮的 width/height
    n = GetItemCfgCout();

    ptDispBuff = GetDispalyBuffer();
    xres       = ptDispBuff->ixres;
    yres       = ptDispBuff->iyres;
    width      = sqrt(1.0 / 0.618 * xres * yres / n);
    n_per_line = xres / width + 1;
    width      = xres / n_per_line;
    height     = width * 0.618;

    // 居中显示 : 计算出每个按钮的region
    start_x = (xres - width * n_per_line) / 2;
    rows    = n / n_per_line;
    if (rows * n_per_line < n)
        rows++;
    start_y = (yres - rows * height) / 2;

    // 计算每个按钮的region
    for (row = 0; (row < rows) && (i < n); row++)
    {
        pre_start_y = start_y + row * height;
        pre_start_x = start_x - width;
        for (col = 0; (col < n_per_line) && (i < n); n++)
        {
            pbutton                   = g_tButtons[i];
            pbutton->tRegion.iLeftupx = pre_start_x + width;
            pbutton->tRegion.iLeftupy = pre_start_y;
            pbutton->tRegion.iwidth   = width - X_GAP;
            pbutton->tRegion.ihight   = height - Y_GAP;
            pre_start_x               = pbutton->tRegion.iLeftupx;

            // InitButton
            InitButton(pbutton, GetItemCfgByIndex(i)->name, NULL, NULL, NULL);

            i++;
        }
    }

    // OnDrow
    for (i = 0; i < n; i++)
    {
        g_tButtons[i]->OnDrow(g_tButtons[i], ptDispBuff);
    }
}

void MainPageRegister(void)
{
    PageRegister(&g_tMainPage);
}
