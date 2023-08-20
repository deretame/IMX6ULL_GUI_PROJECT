#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef struct FontBitMap
{
    int iLeftUpX;
    int iLeftUpY;
    int iWidth;
    int iRows;
} FontBitMap, *pFontBitMap;

#endif