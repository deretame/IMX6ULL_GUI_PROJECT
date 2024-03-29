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
    int iCurOriginX;
    int iCurOriginY;
    int iNextOriginX;
    int iNextOriginY;
    unsigned char * pucBuffer;   // (point) unsigned char Bit Map
} FontBitMap, *pFontBitMap;

typedef struct FontOpr
{
    char * name;
    int (*FontInit)(char * aFineName);
    int (*SetFontSize)(int iFontSize);
    int (*GetFontBitMap)(unsigned int dwcode, pFontBitMap ptFontBitMap);
    struct FontOpr * ptnext;
} FontOpr, *pFontOpr;

#endif