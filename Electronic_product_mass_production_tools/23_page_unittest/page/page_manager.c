#include "../include/page_manager.h"
#include <string.h>

pPageAction g_ptPages = NULL;

void PagesRegister(void);

void PageRegister(pPageAction ptPageAction)
{
    ptPageAction->pnext = g_ptPages;
    g_ptPages           = ptPageAction;
}

pPageAction Page(char * name)
{
    pPageAction ptTmp = g_ptPages;

    while (ptTmp)
    {
        if (strcmp(ptTmp->name, name) == 0)
            return ptTmp;
        ptTmp = ptTmp->pnext;
    }

    return NULL;
}

void PagesRegister(void)
{
    MainPageRegister();
}