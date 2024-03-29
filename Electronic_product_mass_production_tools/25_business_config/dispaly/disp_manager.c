#include "../include/disp_manager.h"
#include "../include/font_manager.h"
#include <stdio.h>
#include <string.h>

/* 管理底层的LCD,WEB */
static PDispOpr g_DispDevs    = NULL;
static PDispOpr g_DispDefault = NULL;
static DispBuff g_tDispBuff;
static int line_width;
static int pixel_width;

int PutPixel(int x, int y, unsigned int dwcolor)
{
    unsigned char * pen_8 = (unsigned char *)g_tDispBuff.buff + y * line_width + x * pixel_width;
    unsigned short * pen_16;
    unsigned int * pen_32;

    unsigned int red, green, blue;

    pen_16 = (unsigned short *)pen_8;
    pen_32 = (unsigned int *)pen_8;

    switch (g_tDispBuff.iBpp)
    {
    case 8:
    {
        *pen_8 = dwcolor;
        break;
    }
    case 16:
    {
        /* 565 */
        red     = (dwcolor >> 16) & 0xff;
        green   = (dwcolor >> 8) & 0xff;
        blue    = (dwcolor >> 0) & 0xff;
        dwcolor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        *pen_16 = dwcolor;
        break;
    }
    case 32:
    {
        *pen_32 = dwcolor;
        break;
    }
    default:
    {
        printf("can't surport %dbpp\n", g_tDispBuff.iBpp);
        break;
    }
    }
    return 0;
}

void RegisterDisplay(PDispOpr ptDispOpr)
{
    ptDispOpr->ptNext = g_DispDevs;
    g_DispDevs        = ptDispOpr;
}

int SelectDefaultDispaly(char * name)
{
    PDispOpr pTmp = g_DispDevs;
    while (pTmp)
    {
        if (strcmp(pTmp->name, name) == 0)
        {
            g_DispDefault = pTmp;
            return 0;
        }
        pTmp = pTmp->ptNext;
    }
    return -1;
}

int InitDefaultDispaly(void)
{
    int err;

    err = g_DispDevs->DeviceInit();
    if (err)
    {
        printf("DeviceInit fail");
        return -1;
    }

    err = g_DispDevs->GetBuffer(&g_tDispBuff);
    if (err)
    {
        printf("GetBuffer fail");
        return -1;
    }

    line_width  = g_tDispBuff.ixres * g_tDispBuff.iBpp / 8;
    pixel_width = g_tDispBuff.iBpp / 8;

    return 0;
}

PDispBuff GetDispalyBuffer(void)
{
    return &g_tDispBuff;
}

int FlushDispalyRegion(PRegion ptRegion, PDispBuff ptDispBuff)
{
    g_DispDefault->FlushRegion(ptRegion, ptDispBuff);
    return 0;
}

void DisplayInit(void)
{
    FramebufferInit();
}

void DrawFontBitMap(pFontBitMap ptFontBitMap, unsigned int dwColor)
{
    int i, j, p, q;
    int x     = ptFontBitMap->tregion.iLeftupx;
    int y     = ptFontBitMap->tregion.iLeftupy;
    int x_max = x + ptFontBitMap->tregion.iwidth;
    int y_max = y + ptFontBitMap->tregion.ihight;
    int width = ptFontBitMap->tregion.iwidth;

    unsigned char * buffer = ptFontBitMap->pucBuffer;
    // printf("x = %d, y = %d\n", x, y);

    for (j = y, q = 0; j < y_max; j++, q++)
    {
        for (i = x, p = 0; i < x_max; i++, p++)
        {
            if (i < 0 || j < 0 ||
                i >= g_tDispBuff.ixres || j >= g_tDispBuff.iyres)
                continue;
            if (buffer[q * width + p])
                PutPixel(i, j, dwColor);
        }
    }
}

void DrawRegion(PRegion ptRegion, unsigned int dwColor)
{
    int x     = ptRegion->iLeftupx;
    int y     = ptRegion->iLeftupy;
    int width = ptRegion->iwidth;
    int hight = ptRegion->ihight;

    int i, j;

    for (j = y; j < y + hight; j++)
    {
        for (i = x; i < x + width; i++)
        {
            PutPixel(i, j, dwColor);
        }
    }
}

void DrawTextInRegionCentral(char * name, PRegion ptRegion, unsigned int dwcolor)
{
    int error;
    int i         = 0;
    int n         = strlen(name);
    int iFontSize = ptRegion->iwidth / n / 2;
    FontBitMap tFontBitMap;

    int iOriginX, iOriginY;

    if (iFontSize > ptRegion->ihight)
        iFontSize = ptRegion->ihight;

    iOriginX = (ptRegion->iwidth - n * iFontSize) / 2 + ptRegion->iLeftupx;
    iOriginY = (ptRegion->ihight - iFontSize) / 2 + iFontSize + ptRegion->iLeftupy;

    SetFontSize(iFontSize);

    while (name[i])
    {
        tFontBitMap.iCurOriginX = iOriginX;
        tFontBitMap.iCurOriginY = iOriginY;

        error = GetFontBitMap(name[i], &tFontBitMap);
        if (error)
        {
            printf("GetFontBitMap error\n");
            return;
        }

        // drow on buffer
        DrawFontBitMap(&tFontBitMap, dwcolor);

        // flush to lcd/web
        // FlushDispalyRegion(&tFontBitMap.tregion, ptBuffer);

        iOriginX = tFontBitMap.iNextOriginX;
        iOriginY = tFontBitMap.iNextOriginY;

        i++;
    }
}