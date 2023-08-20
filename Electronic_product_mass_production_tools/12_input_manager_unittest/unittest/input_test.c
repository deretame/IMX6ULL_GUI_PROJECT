#include "../include/disp_manager.h"
#include "../include/fonts.h"
#include "../include/input_manager.h"
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int                      fd_fb;
struct fb_var_screeninfo var; /* Current var */
int                      screen_size;
unsigned char *          fbmem;
unsigned int             line_width;
unsigned int             pixel_width;

/***********************************************************************
 * 函数名称： lcd_put_ascii
 * 功能描述： 在LCD指定位置上显示一个8*16的字符
 * 输入参数： x坐标，y坐标，ascii码
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2020/05/12	     V1.0	  zh(angenao)	      创建
 ***********************************************************************/
void lcd_put_ascii(int x, int y, unsigned char c);

int main(int argc, char ** argv)
{
    int ret;
    InputEvent event;

    InputInit();
    InputDeviceInit();
    while (1)
    {
        ret = GetInputEvent(&event);
        if (ret)
        {
            printf("GetInputEvent ERROR!\n");
            return -1;
        }
        else
        {
            if (event.iType == INPUT_TYPE_NET)
            {
                printf("Type     = %d\n", event.iType);
                printf("str      = %s\n", event.str);
                printf("time     = %ld\n", event.tTime.tv_sec);
            }
            else if (event.iType == INPUT_TYPE_TOUCH)
            {
                printf("Type     = %d\n", event.iType);
                printf("x        = %d\n", event.iX);
                printf("y        = %d\n", event.iY);
                printf("pressure = %d\n", event.iPressure);
                printf("time     = %ld\n", event.tTime.tv_sec);
                printf("\n");
            }
        }
    }

    return 0;
}

void lcd_put_ascii(int x, int y, unsigned char c)
{
    unsigned char * dots = (unsigned char *)&fontdata_8x16[c * 16];
    int             i, b;
    unsigned char   byte;

    for (i = 0; i < 16; i++)
    {
        byte = dots[i];
        for (b = 7; b >= 0; b--)
        {
            if (byte & (1 << b))
            {
                /* show */
                PutPixel(x + 7 - b, y + i, 0xffffff); /* 白 */
            }
            else
            {
                /* hide */
                PutPixel(x + 7 - b, y + i, 0); /* 黑 */
            }
        }
    }
}