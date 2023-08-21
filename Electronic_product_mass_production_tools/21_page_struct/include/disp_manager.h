#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

#include "font_manager.h"
#include "region.h"

typedef struct DispBuff
{
    int ixres;
    int iyres;
    int iBpp;
    char * buff;
} DispBuff, *PDispBuff;

typedef struct DispOpr
{
    char * name;
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    int (*GetBuffer)(PDispBuff ptDispBuff);
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptDispBuff);
    struct DispOpr * ptNext;
} DispOpr, *PDispOpr;

// framebuffer
void FramebufferInit(void);

// disp_manager
int PutPixel(int x, int y, unsigned int dwcolor);
void RegisterDisplay(PDispOpr ptDispOpr);
int SelectDefaultDispaly(char * name);
int InitDefaultDispaly(void);
int FlushDispalyRegion(PRegion ptRegion, PDispBuff ptDispBuff);
void DisplayInit(void);
PDispBuff GetDispalyBuffer(void);
void DrawFontBitMap(pFontBitMap ptFontBitMap, unsigned int dwColor);
void DrawRegion(PRegion ptRegion, unsigned int dwColor);
void DrawTextInRegionCentral(char * name, PRegion ptRegion, unsigned int dwcolor);

#endif