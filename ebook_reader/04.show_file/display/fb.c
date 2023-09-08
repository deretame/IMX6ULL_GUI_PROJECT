#include "../include/config.h"
#include "../include/disp_manager.h"
#include <fcntl.h>
#include <linux/fb.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

static int g_iFBFD;
static struct fb_var_screeninfo g_tVar; /* Current var */
static struct fb_fix_screeninfo g_tFix; /* Current fix */
static int g_iScreenSize;
static unsigned char * g_pucFbMem;
static int g_iLineWidth;
static int g_iPixelWidth;

static int FBDeviceInit(void);
static int FBShowPixel(int iPenX, int iPenY, unsigned int dwColor);
static int FBCleanScreen(unsigned int dwBackColor);

/* 构造，设置,注册一个结构体 */

static T_DispOpr g_tFBDispOpr = {
    .name        = "fb",
    .DeviceInit  = FBDeviceInit,
    .ShowPixel   = FBShowPixel,
    .CleanScreen = FBCleanScreen,
};

static int FBDeviceInit(void)
{
    g_iFBFD = open(FB_DEVICE_NAME, O_RDWR);
    if (g_iFBFD < 0)
    {
        DBG_PRINTF("can't open %s\n", FB_DEVICE_NAME);
        return -1;
    }

    if (ioctl(g_iFBFD, FBIOGET_VSCREENINFO, &g_tVar))
    {
        DBG_PRINTF("can't get var\n");
        return -1;
    }

    if (ioctl(g_iFBFD, FBIOGET_FSCREENINFO, &g_tFix))
    {
        DBG_PRINTF("can't get fix\n");
        return -1;
    }

    g_tFBDispOpr.iXres = g_tVar.xres;
    g_tFBDispOpr.iYres = g_tVar.yres;
    g_tFBDispOpr.iBpp  = g_tVar.bits_per_pixel;

    g_iScreenSize = g_tVar.xres * g_tVar.yres * g_tVar.bits_per_pixel / 8;
    g_iLineWidth  = g_tVar.xres * g_tVar.bits_per_pixel / 8;
    g_iPixelWidth = g_tVar.bits_per_pixel / 8;

    g_pucFbMem = (unsigned char *)mmap(NULL, g_iScreenSize, PROT_READ | PROT_WRITE, MAP_SHARED, g_iFBFD, 0);
    if (g_pucFbMem == (unsigned char *)-1)
    {
        DBG_PRINTF("can't mmap\n");
        return -1;
    }

    return 0;
}

/* dwColor =0xRRGGBB */
static int FBShowPixel(int iPenX, int iPenY, unsigned int dwColor)
{
    unsigned char * pucPen8 = g_pucFbMem + iPenY * g_iLineWidth + iPenX * g_iPixelWidth;
    unsigned short * pwPen16;
    unsigned int * pdwPen32;

    unsigned int red, green, blue;

    pwPen16  = (unsigned short *)pucPen8;
    pdwPen32 = (unsigned int *)pucPen8;

    switch (g_tFBDispOpr.iBpp)
    {
    case 8:
    {
        *pucPen8 = dwColor;
        break;
    }
    case 16:
    {
        /* 565 */
        red      = (dwColor >> 16) & 0xff;
        green    = (dwColor >> 8) & 0xff;
        blue     = (dwColor >> 0) & 0xff;
        dwColor  = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        *pwPen16 = dwColor;
        break;
    }
    case 32:
    {
        *pdwPen32 = dwColor;
        break;
    }
    default:
    {
        printf("can't surport %dbpp\n", g_tVar.bits_per_pixel);
        break;
    }
    }

    return 0;
}

static int FBCleanScreen(unsigned int dwBackColor)
{
    unsigned char * pucPen8 = g_pucFbMem;
    unsigned short * pwPen16;
    unsigned int * pdwPen32;

    unsigned int red, green, blue;
    int i;

    pwPen16  = (unsigned short *)pucPen8;
    pdwPen32 = (unsigned int *)pucPen8;

    switch (g_tFBDispOpr.iBpp)
    {
    case 8:
    {
        memset(g_pucFbMem, dwBackColor, g_iScreenSize);
        break;
    }
    case 16:
    {
        /* 565 */
        red         = (dwBackColor >> 16) & 0xff;
        green       = (dwBackColor >> 8) & 0xff;
        blue        = (dwBackColor >> 0) & 0xff;
        dwBackColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        for (i = 0; i < g_iScreenSize; i++)
        {
            *pwPen16 = dwBackColor;
            pwPen16++;
            i += 2;
        }

        break;
    }
    case 32:
    {
        for (i = 0; i < g_iScreenSize; i++)
        {
            *pdwPen32 = dwBackColor;
            pdwPen32++;
            i += 2;
        }
        break;
    }
    default:
    {
        printf("can't surport %dbpp\n", g_tVar.bits_per_pixel);
        break;
    }
    }

    return 0;
}

int FBInit(void)
{
    RegisterDispOpr(&g_tFBDispOpr);
    return 0;
}