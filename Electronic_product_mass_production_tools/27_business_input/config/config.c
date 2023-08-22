#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ItemCfg g_tItemCfgs[ITEMCONFIG_ITEM_MAX];
static int g_itemCfgCount = 0;

int ParseConfigFile(void)
{
    FILE * fp;
    char buf[100];
    char * p = buf;

    // 1.0 打开文件
    fp = fopen(CFG_FILE, "r");
    if (!fp)
    {
        printf("%s open error", CFG_FILE);
        return -1;
    }

    // 2.1 读出每一行的内容
    while (fgets(buf, 100, fp))
    {
        buf[99] = '\0';

        // 2.2 去掉开头的空格或者TAB
        while (*p == ' ' || *p == '\t')
            p++;

        // 2.3 忽略注释
        if (*p == '#')
            continue;

        // 2.4 处理
        g_tItemCfgs[g_itemCfgCount].command[0] = '\0';
        sscanf(p, "%s %d %s", g_tItemCfgs[g_itemCfgCount].name, &g_tItemCfgs[g_itemCfgCount].bCanBeTouch, g_tItemCfgs[g_itemCfgCount].command);
        g_itemCfgCount++;
    }

    return 0;
}

int GetItemCfgCout(void)
{
    return g_itemCfgCount;
}

pItemCfg GetItemCfgByIndex(int index)
{
    if (index < g_itemCfgCount)
        return &g_tItemCfgs[index];
    else
        return NULL;
}

pItemCfg GetItemCfgByName(char * name)
{
    int i;

    for (i = 0; i < g_itemCfgCount; i++)
    {
        if (strcmp(g_tItemCfgs[i].name, name) == 0)
            return &g_tItemCfgs[i];
    }

    return NULL;
}