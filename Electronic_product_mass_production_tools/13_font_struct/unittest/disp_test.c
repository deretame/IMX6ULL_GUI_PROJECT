#include "../include/disp_manager.h"
#include "../include/fonts.h"
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
    Region    region;
    PDispBuff ptbuffer;

    DisplayInit();

    SelectDefaultDispaly("fb");

    InitDefaultDispaly();

    lcd_put_ascii(100, 100, 'A'); /*在屏幕中间显示8*16的字母A*/
    sleep(3);
    lcd_put_ascii(100, 100, 'B'); /*在屏幕中间显示8*16的字母B*/
    sleep(3);
    lcd_put_ascii(100, 100, 'C'); /*在屏幕中间显示8*16的字母C*/
    sleep(3);
    lcd_put_ascii(100, 100, 'D'); /*在屏幕中间显示8*16的字母D*/
    sleep(3);
    lcd_put_ascii(100, 100, 'E'); /*在屏幕中间显示8*16的字母E*/
    sleep(3);

    region.iLeftupx = 100;
    region.iLeftupy = 100;
    region.iwidth   = 8;
    region.ihight   = 16;

    ptbuffer = GetDispalyBuffer();

    FlushDispalyRegion(&region, ptbuffer);

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