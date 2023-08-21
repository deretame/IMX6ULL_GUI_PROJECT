#ifndef _UI_H_
#define _UI_H_

#include "disp_manager.h"
#include "input_manager.h"
#include "region.h"

#define BUTTON_DEFAULT_COLOR 0xff0000
#define BUTTON_PRESSED_COLOR 0x00ff00
#define BUTTON_TEXT_COLOR    0x000000

struct Button;

typedef int (*ONDROW_FUNC)(struct Button * ptButton, PDispBuff ptDispBuffer);
typedef int (*ONPRESSED_FUNC)(struct Button * ptButton, PDispBuff ptDispBuffer, pInputEvent ptInputEvent);

typedef struct Button
{
    char * name;
    int status;
    Region tRegion;
    ONDROW_FUNC OnDrow;
    ONPRESSED_FUNC OnPressed;
} Button, *pButton;

void InitButton(pButton ptButton, char * name, PRegion ptRegion, ONDROW_FUNC OnDrow, ONPRESSED_FUNC OnPressed);

#endif