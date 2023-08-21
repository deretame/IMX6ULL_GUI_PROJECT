#ifndef _PAGE_MANAGER_
#define _PAGE_MANAGER_

#include "region.h"

typedef struct PageAction
{
    char * name;
    void (*Run)(void * pParams);
    struct PageAction * pnext;
} PageAction, *pPageAction;

#endif