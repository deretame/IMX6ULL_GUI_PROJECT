#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

typedef struct DispBuff
{
    int ixres;
    int iyres;
    int iBpp;
    char * buff;
} DispBuff, *PDispBuff;

typedef struct Region
{
    int iLeftupx;
    int iLeftupy;
    int iwidth;
    int ihight;
} Region, *PRegion;

typedef struct DispOpr
{
    char * name;
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    int (*GetBuffer)(PDispBuff ptDispBuff);
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptDispBuff);
    struct DispOpr * ptNext;
} DispOpr, *PDispOpr;

typedef struct FontBitMap
{
    Region tregion;
    int iCurOriginX;
    int iCurOriginY;
    int iNextOriginX;
    int iNextOriginY;
    unsigned char * pucBuffer;   // (point) unsigned char Bit Map
} FontBitMap, *pFontBitMap;

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
void DrawFontBitMap(FontBitMap ptFontBitMap, unsigned int dwColor);

#endif