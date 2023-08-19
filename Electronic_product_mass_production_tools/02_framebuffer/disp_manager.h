#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

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
    char * (*GetBuffer)(int * pXres, int * pYres, int * pBpp);
    int (*FlushRegion)(PRegion ptRegion, char * buffer);
    struct DispOpr * ptNext;
} DispOpr, *PDispOpr;

#endif