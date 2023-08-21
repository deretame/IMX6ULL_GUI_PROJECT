#include <fcntl.h>
#include <ft2build.h>
#include <linux/fb.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "../include/font_manager.h"

static FT_Face g_face;
static int g_iDefaultFontSize = 24;

static int FreeTypeFontInit(char * aFineName);
static int FreeTypeSetFontSize(int iFontSize);
static int FreeTypeGetFontBitMap(unsigned int dwcode, pFontBitMap ptFontBitMap);

static FontOpr g_tFreeTypeOpr = {
    .name          = "freetype",
    .FontInit      = FreeTypeFontInit,
    .SetFontSize   = FreeTypeSetFontSize,
    .GetFontBitMap = FreeTypeGetFontBitMap,
};

static int FreeTypeFontInit(char * aFineName)
{
    int error;
    FT_Library library;

    error = FT_Init_FreeType(&library); /* initialize library */
    if (error)
    {
        printf("FT_Init_FreeType error\n");
        return -1;
    }

    error = FT_New_Face(library, aFineName, 0, &g_face); /* create face object */
    if (error)
    {
        printf("FT_New_Face error\n");
        return -1;
    }

    FT_Set_Pixel_Sizes(g_face, g_iDefaultFontSize, 0);

    return 0;
}

static int FreeTypeSetFontSize(int iFontSize)
{
    FT_Set_Pixel_Sizes(g_face, iFontSize, 0);
    return 0;
}

static int FreeTypeGetFontBitMap(unsigned int dwcode, pFontBitMap ptFontBitMap)
{
    int error;
    FT_Vector pen;
    FT_GlyphSlot slot = g_face->glyph;

    pen.x = ptFontBitMap->iCurOriginX * 64; /* 单位: 1/64像素 */
    pen.y = ptFontBitMap->iCurOriginY * 64; /* 单位: 1/64像素 */

    /* 转换：transformation */
    FT_Set_Transform(g_face, 0, &pen);

    /* 加载位图: load glyph image into the slot (erase previous one) */
    error = FT_Load_Char(g_face, dwcode, FT_LOAD_RENDER);
    if (error)
    {
        printf("FT_Load_Char error\n");
        return -1;
    }

    ptFontBitMap->pucBuffer = slot->bitmap.buffer;

    ptFontBitMap->tregion.iLeftupx = slot->bitmap_left;
    ptFontBitMap->tregion.iLeftupx = ptFontBitMap->iCurOriginY * 2 - slot->bitmap_top;
    ptFontBitMap->tregion.iwidth   = slot->bitmap.width;
    ptFontBitMap->tregion.ihight   = slot->bitmap.rows;
    ptFontBitMap->iNextOriginX     = ptFontBitMap->iCurOriginX + slot->advance.x / 64;
    ptFontBitMap->iNextOriginY     = ptFontBitMap->iCurOriginY;

    return 0;
}

void FreeTypeRegister(void)
{
    RegisterFont(&g_tFreeTypeOpr);
}
