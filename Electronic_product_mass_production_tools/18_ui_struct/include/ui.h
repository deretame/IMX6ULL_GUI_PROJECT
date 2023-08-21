#ifndef _UI_H_
#define _UI_H_

#include "disp_manager.h"
#include "input_manager.h"
#include "region.h"

typedef struct Button
{
    char * name;
    Region tRegion;
    int (*OnDrow)(Button ptButton, PDispBuff ptDispBuffer);
    int (*OnPressed)(Button ptButton, PDispBuff ptDispBuffer, pInputEvent ptInputEvent);
} Button, *pButton;

#endif