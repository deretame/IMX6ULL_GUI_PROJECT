#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "../include/region.h"

#define ITEMCONFIG_ITEM_MAX 30
#define CFG_FILE            "/etc/test_gui/gui.conf"

typedef struct ItemCfg
{
    int index;
    char name[100];
    int bCanBeTouch;
    char command[100];
} ItemCfg, *pItemCfg;

int ParseConfigFile(void);
int GetItemCfgCout(void);
pItemCfg GetItemCfgByIndex(int index);
pItemCfg GetItemCfgByName(char * name);

#endif