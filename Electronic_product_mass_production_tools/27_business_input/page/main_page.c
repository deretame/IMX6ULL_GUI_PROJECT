#include "../include/config.h"
#include "../include/input_manager.h"
#include "../include/page_manager.h"
#include "../include/ui.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define X_GAP 5
#define Y_GAP 5

static pButton g_tButtons[ITEMCONFIG_ITEM_MAX];
static int g_tButtonCnt;

static pButton GetButtonByInputEvent(pInputEvent ptInputEvent);
static pButton GetButtonByname(char * name);
static int IsTouchPointInRegion(int iX, int iY, PRegion ptRegion);
static void GenerateButtons(void);
static void MainPageRun(void * pParams);
static int MainOnPressed(struct Button * ptButton, PDispBuff ptDispBuffer, pInputEvent ptInputEvent);

static PageAction g_tMainPage = {
    .name = "main",
    .Run  = MainPageRun,
};

static void MainPageRun(void * pParams)
{
    int error;
    InputEvent tInputEvent;
    pButton ptButton;
    PDispBuff ptDispBuffer = GetDispalyBuffer();

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
        error = GetInputEvent(&tInputEvent);
        if (error)
        {
            printf("GetInputEvent error");
            continue;
        }

        // 根据输入事件找到按钮
        ptButton = GetButtonByInputEvent(&tInputEvent);
        if (!ptButton)
            continue;

        // 调用按钮的OnPressed函数
        ptButton->OnPressed(ptButton, ptDispBuffer, &tInputEvent);
    }
}

static pButton GetButtonByInputEvent(pInputEvent ptInputEvent)
{
    int i;
    char name[100];

    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        for (i = 0; i < g_tButtonCnt; i++)
        {
            if (IsTouchPointInRegion(ptInputEvent->iType, ptInputEvent->iY, &g_tButtons[i]->tRegion))
                return g_tButtons[i];
        }
    }
    else if (ptInputEvent->iType == INPUT_TYPE_NET)
    {
        for (i = 0; i < g_tButtonCnt; i++)
        {
            sscanf(ptInputEvent->str, "%s", name);
            return GetButtonByname(name);
        }
    }

    return NULL;
}

static pButton GetButtonByname(char * name)
{
    int i;
    for (i = 0; i < g_tButtonCnt; i++)
    {
        if (strcmp(name, g_tButtons[i]->name) == 0)
            return g_tButtons[i];
    }

    return NULL;
}

static int IsTouchPointInRegion(int iX, int iY, PRegion ptRegion)
{
    if (iX < ptRegion->iLeftupx || iX > ptRegion->iLeftupx + ptRegion->iwidth)
        return 0;

    if (iY < ptRegion->iLeftupy || iY > ptRegion->iLeftupy + ptRegion->ihight)
        return 0;

    return 1;
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
    g_tButtonCnt = n = GetItemCfgCout();

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
            InitButton(pbutton, GetItemCfgByIndex(i)->name, NULL, NULL, MainOnPressed);

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

static int MainOnPressed(struct Button * ptButton, PDispBuff ptDispBuffer, pInputEvent ptInputEvent)
{
    unsigned int dwcolor = BUTTON_DEFAULT_COLOR;

    // 1.0 对于触摸屏事件
    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        // 1.1 分辨能否被点击
        if (GetItemCfgByName(ptButton->name)->bCanBeTouch == 0)
            return -1;

        // 1.2 修改颜色
        ptButton->status = !ptButton->status;
    }

    // 2.0 对于网络类事件

    // 2.1 根据传入的字符串修改颜色

    ptButton->status = !ptButton->status;
    if (ptButton->status)
        dwcolor = BUTTON_PRESSED_COLOR;

    // 绘制底色
    DrawRegion(&ptButton->tRegion, dwcolor);

    // 居中写文字
    DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);

    // flush to lcd
    FlushDispalyRegion(&ptButton->tRegion, ptDispBuffer);

    return 0;
}