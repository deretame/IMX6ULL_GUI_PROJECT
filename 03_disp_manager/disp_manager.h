#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

typedef struct DispBuff
{
    int    ixres;
    int    iyres;
    int    iBpp;
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

int  PutPixel(int x, int y, unsigned int dwcolor);
void RegisterDisplay(PDispOpr ptDispOpr);
int  SelectDefaultDispaly(char * name);
void FramebufferInit(void);
int  InitDefaultDispaly(void);
int  FlushDispalyRegion(PRegion ptRegion, PDispBuff ptDispBuff);

#endif