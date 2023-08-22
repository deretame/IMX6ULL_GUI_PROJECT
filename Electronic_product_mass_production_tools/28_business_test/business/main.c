#include "../include/disp_manager.h"
#include "../include/font_manager.h"
#include "../include/input_manager.h"
#include "../include/page_manager.h"
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
    // PDispBuff ptBuffer;

    if (argc != 2)
    {
        printf("Usage : %s <font_file>\n", argv[0]);
        return -1;
    }

    // 初始化显示系统
    DisplayInit();

    SelectDefaultDispaly("fb");

    InitDefaultDispaly();

    // ptBuffer = GetDispalyBuffer();

    // 初始化输入系统
    InputInit();

    InputDeviceInit();

    // 初始化文字系统
    FontsRegister();

    error = SelectAndInitFont("freetype", argv[1]);
    if (error)
    {
        printf("SelectAndInitFont err\n");
        return -1;
    }

    // 初始化页面系统
    PagesRegister();

    // 运行业务系统的主界面
    Page("main")->Run(NULL);

    return 0;
}