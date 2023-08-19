#include "disp_manager.h"
#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int                      fd_fb;
static struct fb_var_screeninfo var; /* Current var */
static int                      screen_size;
static unsigned char *          fb_base;
static unsigned int             line_width;
static unsigned int             pixel_width;

static int    FbDeviceInit(void);
static int    FbDeviceExit(void);
static char * FbGetBuffer(int * pXres, int * pYres, int * pBpp);
static int    FbFlushRegion(PRegion ptRegion, char * buffer);

struct DispOpr g_tFramebufferOpr = {
    .name        = "fb",
    .DeviceInit  = FbDeviceInit,
    .DeviceExit  = FbDeviceExit,
    .GetBuffer   = FbGetBuffer,
    .FlushRegion = FbFlushRegion,
};

static int FbDeviceInit(void)
{
    int i;

    fd_fb = open("/dev/fb0", O_RDWR);
    if (fd_fb < 0)
    {
        printf("can't open /dev/fb0\n");
        return -1;
    }
    if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
    {
        printf("can't get var\n");
        return -1;
    }

    line_width  = var.xres * var.bits_per_pixel / 8;
    pixel_width = var.bits_per_pixel / 8;
    screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
    fb_base     = (unsigned char *)mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
    if (fb_base == (unsigned char *)-1)
    {
        printf("can't mmap\n");
        return -1;
    }

    return 0;
}

static int FbDeviceExit(void)
{
    munmap(fb_base, screen_size);
    close(fd_fb);
    return 0;
}

/*
 * 可以返回LCD的frambueffer,以后上层APP可以直接操作LCD,可以不用FbFlushRegion
 * 也可以malloc返回一块无关的buffer,要使用FbFlushRegion
 */
static char * FbGetBuffer(int * pXres, int * pYres, int * pBpp)
{
    *pXres = var.xres;
    *pYres = var.yres;
    *pBpp  = var.bits_per_pixel;
    return (char *)fb_base;
}

static int FbFlushRegion(PRegion ptRegion, char * buffer)
{
    return 0;
}