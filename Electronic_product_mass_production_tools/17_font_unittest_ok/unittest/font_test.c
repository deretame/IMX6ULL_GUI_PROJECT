#include "../include/disp_manager.h"
#include "../include/font_manager.h"
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
    int err;
    PDispBuff ptbuffer;
    FontBitMap tFontBitMap;
    char * str = "www.100ask.net";
    int i      = 0;
    int lcdx;
    int lcdy;
    int font_size;

    if (argc != 5)
    {
        printf("Usage: %s <font_file> <lcd_x> <lcd_y> <font_size>\n", argv[0]);
        return -1;
    }

    lcdx      = strtol(argv[2], NULL, 0);
    lcdy      = strtol(argv[3], NULL, 0);
    font_size = strtol(argv[4], NULL, 0);

    DisplayInit();

    SelectDefaultDispaly("fb");

    InitDefaultDispaly();
    ptbuffer = GetDispalyBuffer();

    FontsRegister();
    err = SelectAndInitFont("freetype", argv[1]);
    if (err)
    {
        printf("SelectAndInitFont error\n");
        return -1;
    }

    SetFontSize(font_size);
    while (str[i])
    {
        tFontBitMap.tregion.iLeftupx = lcdx;
        tFontBitMap.tregion.iLeftupy = lcdy;

        err = GetFontBitMap(str[i], &tFontBitMap);
        if (err)
        {
            printf("GetFontBitMap error\n");
            return -1;
        }

        // drow on buffer
        DrawFontBitMap(&tFontBitMap, 0xc26c75);

        // flush to lcd/web
        FlushDispalyRegion(&tFontBitMap.tregion, ptbuffer);

        lcdx = tFontBitMap.iNextOriginX;
        lcdy = tFontBitMap.iNextOriginY;

        i++;
    }

    return 0;
}