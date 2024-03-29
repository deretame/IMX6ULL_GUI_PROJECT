#include "include/config.h"
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
            printf("Usage: %s [-s Size] [-d display] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
            printf("Usage: %s -l\n", argv[0]);
            return -1;
            break;
        }
        }
    }

    if (!bList && (optind >= argc))
    {
        printf("Usage: %s [-s Size] [-d display] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
        printf("Usage: %s -l\n", argv[0]);
        return -1;
    }

    iError = DisplayInit();
    if (iError)
    {
        printf("DisplayInit error!\n");
        return -1;
    }

    iError = FontsInit();
    if (iError)
    {
        printf("FontsInit error!\n");
        return -1;
    }

    iError = EncodingInit();
    if (iError)
    {
        printf("EncodingInit error!\n");
        return -1;
    }

    iError = InputInit();
    if (iError)
    {
        printf("InputInit error!\n");
        return -1;
    }

    if (bList)
    {
        printf("supported display:\n");
        ShowDispOpr();

        printf("supported font:\n");
        ShowFontOpr();

        printf("supported encoding:\n");
        ShowEncodingOpr();

        printf("supported input:\n");
        ShowInputOpr();

        return 0;
    }

    strncpy(acTextFile, argv[optind], 128);
    acTextFile[127] = '\0';

    iError = OpenTextFile(acTextFile);
    if (iError)
    {
        printf("OpenTextFile error!\n");
        return -1;
    }

    iError = SetTextDetail(acHzkFile, acFreetypeFile, dwFontSize);
    if (iError)
    {
        printf("SetTextDetail error!\n");
        return -1;
    }

    DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    iError = SelectAndInitDisplay(acDisplay);
    if (iError)
    {
        printf("SelectAndInitDisplay error!\n");
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
        printf("Error to show first page\n");
        return -1;
    }

    printf("Enter 'n' to show next page, 'u' to show previous page, 'q' to exit: ");

    while (1)
    {

        if (0 == GetInputEvent(&tInputEvent))
        {
            if (tInputEvent.iVal == INPUT_VALUE_DOWN)
            {
                ShowNextPage();
                printf("\n");
            }
            else if (tInputEvent.iVal == INPUT_VALUE_UP)
            {
                ShowPrePage();
                printf("\n");
            }
            else if (tInputEvent.iVal == INPUT_VALUE_EXIT)
            {
                SCLBlack();
                printf("\n");
                return 0;
            }
        }
    }
    return 0;
}
