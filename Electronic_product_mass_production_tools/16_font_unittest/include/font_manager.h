#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

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

typedef struct FontBitMap
{
    Region tregion;
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

// freetype.c
void FreeTypeRegister(void);

// font_manager.c
void RegisterFont(pFontOpr ptFontOpr);
void FontsRegister(void);
int SelectAndInitFont(char * aFontOprName, char * aFontFileName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwcode, pFontBitMap ptFontBitMap);

#endif