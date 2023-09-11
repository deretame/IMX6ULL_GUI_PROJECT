#include "include/config.h"
#include "include/debug_manager.h"
#include "include/disp_manager.h"
#include "include/draw.h"
#include "include/encoding_manager.h"
#include "include/fonts_manager.h"
#include "include/input_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ./show_file [-s Size] [-f freetype_font_file] [-h HZK] <text_file> */
int main(int argc, char ** argv)
{
    int iError;
    unsigned int dwFontSize = 16;
    char acHzkFile[128];
    char acFreetypeFile[128];
    char acTextFile[128];

    char acDisplay[128];

    int bList = 0;

    T_InputEvent tInputEvent;

    acHzkFile[0]      = '\0';
    acFreetypeFile[0] = '\0';
    acTextFile[0]     = '\0';

    strcpy(acDisplay, "fb");

    while ((iError = getopt(argc, argv, "ls:f:h:d:")) != -1)
    {
        switch (iError)
        {
        case 'l':
        {
            bList = 1;
            break;
        }
        case 's':
        {
            dwFontSize = strtoul(optarg, NULL, 0);
            break;
        }
        case 'f':
        {
            strncpy(acFreetypeFile, optarg, 128);
            acFreetypeFile[127] = '\0';
            break;
        }
        case 'h':
        {
            strncpy(acHzkFile, optarg, 128);
            acHzkFile[127] = '\0';
            break;
        }
        case 'd':
        {
            strncpy(acDisplay, optarg, 128);
            acDisplay[127] = '\0';
            break;
        }
        default:
        {
            DebugPrint("Usage: %s [-s Size] [-d display] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
            DebugPrint("Usage: %s -l\n", argv[0]);
            return -1;
            break;
        }
        }
    }

    if (!bList && (optind >= argc))
    {
        DebugPrint("Usage: %s [-s Size] [-d display] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
        DebugPrint("Usage: %s -l\n", argv[0]);
        return -1;
    }

    iError = DisplayInit();
    if (iError)
    {
        DebugPrint("DisplayInit error!\n");
        return -1;
    }

    iError = FontsInit();
    if (iError)
    {
        DebugPrint("FontsInit error!\n");
        return -1;
    }

    iError = EncodingInit();
    if (iError)
    {
        DebugPrint("EncodingInit error!\n");
        return -1;
    }

    iError = DebugInit();
    if (iError)
    {
        DebugPrint("DebugInit error!\n");
        return -1;
    }

    iError = InputInit();
    if (iError)
    {
        DebugPrint("InputInit error!\n");
        return -1;
    }

    if (bList)
    {
        DebugPrint("supported display:\n");
        ShowDispOpr();

        DebugPrint("supported font:\n");
        ShowFontOpr();

        DebugPrint("supported encoding:\n");
        ShowEncodingOpr();

        DebugPrint("supported input:\n");
        ShowInputOpr();

        DebugPrint("supported debug chanel:\n");
        ShowDebugOpr();

        return 0;
    }

    InitDebugChanel();

    strncpy(acTextFile, argv[optind], 128);
    acTextFile[127] = '\0';

    iError = OpenTextFile(acTextFile);
    if (iError)
    {
        DebugPrint("OpenTextFile error!\n");
        return -1;
    }

    iError = SetTextDetail(acHzkFile, acFreetypeFile, dwFontSize);
    if (iError)
    {
        DebugPrint("SetTextDetail error!\n");
        return -1;
    }

    DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    iError = SelectAndInitDisplay(acDisplay);
    if (iError)
    {
        DebugPrint("SelectAndInitDisplay error!\n");
        return -1;
    }

    iError = AllInputDevicesInit();
    if (iError)
    {
        DBG_PRINTF("Error AllInputDevicesInit\n");
        return -1;
    }

    DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    iError = ShowNextPage();
    DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    if (iError)
    {
        DebugPrint("Error to show first page\n");
        return -1;
    }

    DebugPrint("Enter 'n' to show next page, 'u' to show previous page, 'q' to exit: ");
    DebugPrint("\n");

    while (1)
    {

        if (0 == GetInputEvent(&tInputEvent))
        {
            if (tInputEvent.iVal == INPUT_VALUE_DOWN)
            {
                ShowNextPage();
                DebugPrint("\n");
            }
            else if (tInputEvent.iVal == INPUT_VALUE_UP)
            {
                ShowPrePage();
                DebugPrint("\n");
            }
            else if (tInputEvent.iVal == INPUT_VALUE_EXIT)
            {
                CSLBlack();
                DebugPrint("\n");
                return 0;
            }
        }
    }
    return 0;
}
