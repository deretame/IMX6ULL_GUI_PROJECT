#include "../include/font_manager.h"
#include <stdio.h>
#include <string.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

static pFontOpr g_ptFonts          = NULL;
static pFontOpr g_ptDefaultFontOpr = NULL;

void RegisterFont(pFontOpr ptFontOpr);
void FontsRegister(void);
int SelectAndInitFont(char * aFontOprName, char * aFontFileName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwcode, pFontBitMap ptFontBitMap);

void RegisterFont(pFontOpr ptFontOpr)
{
    ptFontOpr->ptnext = g_ptFonts;
    g_ptFonts         = ptFontOpr;
}

void FontsRegister(void)
{
    extern void FreeTypeRegister(void);
    FreeTypeRegister();
}

int SelectAndInitFont(char * aFontOprName, char * aFontFileName)
{
    pFontOpr ptTmp = g_ptFonts;
    while (ptTmp)
    {
        if (strcmp(ptTmp->name, aFontOprName) == 0)
            break;
        ptTmp = ptTmp->ptnext;
    }

    if (!ptTmp)
        return -1;

    g_ptDefaultFontOpr = ptTmp;
    return ptTmp->FontInit(aFontFileName);
}

int SetFontSize(int iFontSize)
{
    return g_ptDefaultFontOpr->SetFontSize(iFontSize);
}

int GetFontBitMap(unsigned int dwcode, pFontBitMap ptFontBitMap)
{
    return g_ptDefaultFontOpr->GetFontBitMap(dwcode, ptFontBitMap);
}