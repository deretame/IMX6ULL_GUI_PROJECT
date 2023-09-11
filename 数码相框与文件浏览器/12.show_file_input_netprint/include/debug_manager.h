#ifndef _DEBUG_MANAGER_H
#define _DEBUG_MANAGER_H

#define APP_EMERG        "<0>" /* system is unusable			    */
#define APP_ALERT        "<1>" /* action must be taken immediately	*/
#define APP_CRIT         "<2>" /* critical conditions			    */
#define APP_ERR          "<3>" /* error conditions			        */
#define APP_WARNING      "<4>" /* warning conditions			    */
#define APP_NOTICE       "<5>" /* normal but significant condition	*/
#define APP_INFO         "<6>" /* informational			            */
#define APP_DEBUG        "<7>" /* debug-level messages			    */

#define DEFAULT_DBGLEVEL 4

typedef struct DebugOpr
{
    char * name;
    int isCanUse;
    int (*DebugInit)(void);
    int (*DebugExit)(void);
    int (*DebugPrint)(char * strData);
    struct DebugOpr * ptNext;
} T_DebugOPr, *PT_DebugOpr;

// debug_manager.c
int RegisterDebugOpr(PT_DebugOpr ptDebugOpr);
int SetDbgLevel(char * strBuf);
int SetDbgChanel(char * strBuf);
void ShowDebugOpr(void);
PT_DebugOpr GetDebugOpr(char * pcName);
int DebugPrint(const char * pcFormat, ...);
int InitDebugChanel(void);
int DebugInit(void);

// stdout.c
int StdoutInit(void);

// netprint.c
int NetPrintInit(void);

#endif