#ifndef _PAGE_MANAGER_
#define _PAGE_MANAGER_

#include "region.h"

typedef struct PageAction
{
    char * name;
    void (*Run)(void * pParams);
    struct PageAction * pnext;
} PageAction, *pPageAction;

// page_manager.c
void PageRegister(pPageAction ptPageAction);
void PagesRegister(void);
pPageAction Page(char * name);

// main_page.c
void MainPageRegister(void);

#endif