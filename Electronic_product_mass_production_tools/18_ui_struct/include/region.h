#ifndef _REGION_H_
#define _REGION_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef struct Region
{
    int iLeftupx;
    int iLeftupy;
    int iwidth;
    int ihight;
} Region, *PRegion;

#endif