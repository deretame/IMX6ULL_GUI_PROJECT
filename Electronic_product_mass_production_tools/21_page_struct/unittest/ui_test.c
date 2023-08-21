#include "../include/disp_manager.h"
#include "../include/font_manager.h"
#include "../include/input_manager.h"
#include "../include/ui.h"
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

int main(int argc, char ** argv)
{
    int error;
    PDispBuff ptBuffer;
    Button tButton;
    Region tRegion;

    if (argc != 2)
    {
        printf("Usage : %s <font_file>\n", argv[0]);
        return -1;
    }

    DisplayInit();

    SelectDefaultDispaly("fb");

    InitDefaultDispaly();

    ptBuffer = GetDispalyBuffer();

    FontsRegister();

    error = SelectAndInitFont("freetype", argv[1]);
    if (error)
    {
        printf("SelectAndInitFont err\n");
        return -1;
    }

    tRegion.iLeftupx = 100;
    tRegion.iLeftupy = 100;
    tRegion.iwidth   = 300;
    tRegion.ihight   = 100;
    InitButton(&tButton, "test", &tRegion, NULL, NULL);

    tButton.OnDrow(&tButton, ptBuffer);

    while (1)
    {
        tButton.OnPressed(&tButton, ptBuffer, NULL);
        sleep(2);
    }

    return 0;
}