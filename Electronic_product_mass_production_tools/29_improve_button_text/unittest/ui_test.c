#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/disp_manager.h"
#include "../include/font_manager.h"
#include "../include/ui.h"

int main(int argc, char ** argv)
{
    PDispBuff ptBuffer;
    int error;
    Button tButton;
    Region tRegion;

    if (argc != 2)
    {
        printf("Usage: %s <font_size>\n", argv[0]);
        return -1;
    }

    DisplayInit();

    SelectDefaultDisplay("fb");

    InitDefaultDisplay();

    ptBuffer = GetDisplayBuffer();

    FontsRegister();

    error = SelectAndInitFont("freetype", argv[1]);
    if (error)
    {
        printf("SelectAndInitFont err\n");
        return -1;
    }

    tRegion.iLeftUpX = 200;
    tRegion.iLeftUpY = 200;
    tRegion.iWidth   = 300;
    tRegion.iHeigh   = 100;

    InitButton(&tButton, "test", &tRegion, NULL, NULL);
    tButton.OnDraw(&tButton, ptBuffer);
    while (1)
    {
        tButton.OnPressed(&tButton, ptBuffer, NULL);
        sleep(2);
    }

    return 0;
}
