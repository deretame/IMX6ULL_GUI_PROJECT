#ifndef _PAGE_MANAGER_
#define _PAGE_MANAGER_

typedef struct PageAction
{
    char * name;
    void (*Run)(void * pParams);
    struct PageAction * pnext;
} PageAction, *pPageAction;

#endif