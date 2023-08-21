#include "../include/disp_manager.h"
#include "../include/input_manager.h"
#include "../include/ui.h"

static int DefaultOnDrow(struct Button * ptButton, PDispBuff ptDispBuffer);
static int DefaultOnPressed(struct Button * ptButton, PDispBuff ptDispBuffer, pInputEvent ptInputEvent);
void InitButton(pButton ptButton, char * name, PRegion ptRegion, ONDROW_FUNC OnDrow, ONPRESSED_FUNC OnPressed);

static int DefaultOnDrow(struct Button * ptButton, PDispBuff ptDispBuffer)
{
    // 绘制底色
    DrawRegion(&ptButton->tRegion, BUTTON_DEFAULT_COLOR);

    // 居中写文字
    DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);

    // flush to lcd
    FlushDispalyRegion(&ptButton->tRegion, ptDispBuffer);

    return 0;
}
static int DefaultOnPressed(struct Button * ptButton, PDispBuff ptDispBuffer, pInputEvent ptInputEvent)
{
    unsigned int dwcolor = BUTTON_DEFAULT_COLOR;

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

void InitButton(pButton ptButton, char * name, PRegion ptRegion, ONDROW_FUNC OnDrow, ONPRESSED_FUNC OnPressed)
{
    ptButton->name      = name;
    ptButton->status    = 0;
    ptButton->tRegion   = *ptRegion;
    ptButton->OnDrow    = OnDrow ? OnDrow : DefaultOnDrow;
    ptButton->OnPressed = OnPressed ? OnPressed : DefaultOnPressed;
}