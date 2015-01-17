
/****

 Copyright (C) 1990 Texas Instruments Incorporated.

 Permission is granted to any individual or institution to use, copy, modify,
 and distribute this software, provided that this complete copyright and
 permission notice is maintained, intact, in all copies and supporting
 documentation.

 Texas Instruments Incorporated provides this software "as is" without express
 or implied warranty.

****/

/*
 * This file contains all new routines.
 */

#include <InterViews/PM/PM_lib.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#ifdef DEBUG
#include <stdio.h>
#endif

extern int saved_argc;
extern char** saved_argv;

long XColor::GetRgb() 
{
  long rgb = u.rgbColor;
  return rgb;
}

void XColor::SetRgb(LONG RgbColor)
{
  u.rgbColor = RgbColor;
}

/*
 * Put up a message box, terminate PM, and exit
 */
VOID ReportError(HAB hab, char *msg) 
{
  PERRINFO  perriBlk;
  PSZ       pszErrMsg;
  USHORT *  TempPtr;
  char *errMsg;
  
  if ((perriBlk = WinGetErrorInfo(hab)) != (PERRINFO)NULL){
    SELECTOROF(pszErrMsg) = SELECTOROF(perriBlk);
    SELECTOROF(TempPtr) = SELECTOROF(perriBlk);
    OFFSETOF(TempPtr) = perriBlk->offaoffszMsg;
    OFFSETOF(pszErrMsg) =  *TempPtr;
    errMsg = (char*)malloc(strlen((const char *)pszErrMsg) + strlen((const char *)msg) + 2);
    if (errMsg == 0) {
      PMMessageBox((char *)pszErrMsg);
    } else {
      strcpy(errMsg, msg);
      strcat(errMsg, (const char *)pszErrMsg);
      PMMessageBox(errMsg);
    }
    free(errMsg);
    WinFreeErrorInfo(perriBlk);
  } else {
    /*
     * We should never get here, but just in case.
     */
    PMMessageBox("FATAL ERROR");
  }
  WinTerminate(hab);
  exit(1);
}

void PMMessageBox(char* message)
{
  WinMessageBox(HWND_DESKTOP,
		(HWND)NULL,
		(PSZ)(message),
		(PSZ)NULL,
		0,
		MB_CUACRITICAL | MB_CANCEL );
}

#ifdef DEBUG

/*
 * Return true if any of the command line arguments match the code or zero.
 */
boolean debugP(int code) 
{
  if (saved_argc > 1) {
    int j;
    for (int i = 1; i < saved_argc; i++) {
      if (sscanf(saved_argv[i], "%d", &j)) {
	if (j == 0 || j == code) {
	  return true;
	}
      }
    }
  }
  return false;
}
	
void debug(char* msg)
{
  FILE *printer;

  printer = fopen("prn", "w");
  fprintf(printer, "%s\n", msg);
  fflush(printer);
  fclose(printer);
}

void DoPrintMsg(char *text, HWND hwnd, int msg)
{
  FILE *printer;
  char buffer[80];
  int id;

  printer = fopen("prn", "w");
  id = WinQueryWindowUShort(hwnd, IV_ID);

  switch (msg) {
  case WM_NULL:
    fprintf(printer, "%s id=%d  WM_NULL\n", text, id);
    fflush(printer);
    break;
  case WM_CREATE:
    fprintf(printer, "%s id=%d  WM_CREATE\n", text, id);
    fflush(printer);
    break;
  case WM_DESTROY:
    fprintf(printer, "%s id=%d  WM_DESTROY\n", text, id);
    fflush(printer);
    break;
  case WM_OTHERWINDOWDESTROYED:
    fprintf(printer, "%s id=%d  WM_OTHERWINDOWDESTROYED\n", text, id);
    fflush(printer);
    break;
  case WM_ENABLE:
    fprintf(printer, "%s id=%d  WM_ENABLE\n", text, id);
    fflush(printer);
    break;
  case WM_SHOW:
    fprintf(printer, "%s id=%d  WM_SHOW\n", text, id);
    fflush(printer);
    break;
  case WM_MOVE:
    fprintf(printer, "%s id=%d  WM_MOVE\n", text, id);
    fflush(printer);
    break;
  case WM_SIZE:
    fprintf(printer, "%s id=%d  WM_SIZE\n", text, id);
    fflush(printer);
    break;
  case WM_ADJUSTWINDOWPOS:
    fprintf(printer, "%s id=%d  WM_ADJUSTWINDOWPOS\n", text, id);
    fflush(printer);
    break;
  case WM_CALCVALIDRECTS:
    fprintf(printer, "%s id=%d  WM_CALCVALIDRECTS\n", text, id);
    fflush(printer);
    break;
  case WM_SETWINDOWPARAMS:
    fprintf(printer, "%s id=%d  WM_SETWINDOWPARAMS\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYWINDOWPARAMS:
    fprintf(printer, "%s id=%d  WM_QUERYWINDOWPARAMS\n", text, id);
    fflush(printer);
    break;
  case WM_HITTEST:
    fprintf(printer, "%s id=%d  WM_HITTEST\n", text, id);
    fflush(printer);
    break;
  case WM_ACTIVATE:
    fprintf(printer, "%s id=%d  WM_ACTIVATE\n", text, id);
    fflush(printer);
    break;
  case WM_SETFOCUS:
    fprintf(printer, "%s id=%d  WM_SETFOCUS\n", text, id);
    fflush(printer);
    break;
  case WM_SETSELECTION:
    fprintf(printer, "%s id=%d  WM_SETSELECTION\n", text, id);
    fflush(printer);
    break;
  case WM_COMMAND:
    fprintf(printer, "%s id=%d  WM_COMMAND\n", text, id);
    fflush(printer);
    break;
  case WM_SYSCOMMAND:
    fprintf(printer, "%s id=%d  WM_SYSCOMMAND\n", text, id);
    fflush(printer);
    break;
  case WM_HELP:
    fprintf(printer, "%s id=%d  WM_HELP\n", text, id);
    fflush(printer);
    break;
  case WM_PAINT:
    fprintf(printer, "%s id=%d  WM_PAINT\n", text, id);
    fflush(printer);
    break;
  case WM_TIMER:
    fprintf(printer, "%s id=%d  WM_TIMER\n", text, id);
    fflush(printer);
    break;
  case WM_SEM1:
    fprintf(printer, "%s id=%d  WM_SEM1\n", text, id);
    fflush(printer);
    break;
  case WM_SEM2:
    fprintf(printer, "%s id=%d  WM_SEM2\n", text, id);
    fflush(printer);
    break;
  case WM_SEM3:
    fprintf(printer, "%s id=%d  WM_SEM3\n", text, id);
    fflush(printer);
    break;
  case WM_SEM4:
    fprintf(printer, "%s id=%d  WM_SEM4\n", text, id);
    fflush(printer);
    break;
  case WM_CLOSE:
    fprintf(printer, "%s id=%d  WM_CLOSE\n", text, id);
    fflush(printer);
    break;
  case WM_QUIT:
    fprintf(printer, "%s id=%d  WM_QUIT\n", text, id);
    fflush(printer);
    break;
  case WM_SYSCOLORCHANGE:
    fprintf(printer, "%s id=%d  WM_SYSCOLORCHANGE\n", text, id);
    fflush(printer);
    break;
  case WM_SYSVALUECHANGED:
    fprintf(printer, "%s id=%d  WM_SYSVALUECHANGED\n", text, id);
    fflush(printer);
    break;
  case WM_CONTROL:
    fprintf(printer, "%s id=%d  WM_CONTROL\n", text, id);
    fflush(printer);
    break;
  case WM_VSCROLL:
    fprintf(printer, "%s id=%d  WM_VSCROLL\n", text, id);
    fflush(printer);
    break;
  case WM_HSCROLL:
    fprintf(printer, "%s id=%d  WM_HSCROLL\n", text, id);
    fflush(printer);
    break;
  case WM_INITMENU:
    fprintf(printer, "%s id=%d  WM_INITMENU\n", text, id);
    fflush(printer);
    break;
  case WM_MENUSELECT:
    fprintf(printer, "%s id=%d  WM_MENUSELECT\n", text, id);
    fflush(printer);
    break;
  case WM_MENUEND:
    fprintf(printer, "%s id=%d  WM_MENUEND\n", text, id);
    fflush(printer);
    break;
  case WM_DRAWITEM:
    fprintf(printer, "%s id=%d  WM_DRAWITEM\n", text, id);
    fflush(printer);
    break;
  case WM_MEASUREITEM:
    fprintf(printer, "%s id=%d  WM_MEASUREITEM\n", text, id);
    fflush(printer);
    break;
  case WM_CONTROLPOINTER:
//    fprintf(printer, "%s id=%d  WM_CONTROLPOINTER\n", text, id);
//    fflush(printer);
    break;
  case WM_CONTROLHEAP:
    fprintf(printer, "%s id=%d  WM_CONTROLHEAP\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYDLGCODE:
    fprintf(printer, "%s id=%d  WM_QUERYDLGCODE\n", text, id);
    fflush(printer);
    break;
  case WM_INITDLG:
    fprintf(printer, "%s id=%d  WM_INITDLG\n", text, id);
    fflush(printer);
    break;
  case WM_SUBSTITUTESTRING:
    fprintf(printer, "%s id=%d  WM_SUBSTITUTESTRING\n", text, id);
    fflush(printer);
    break;
  case WM_MATCHMNEMONIC:
    fprintf(printer, "%s id=%d  WM_MATCHMNEMONIC\n", text, id);
    fflush(printer);
    break;
  case WM_MOUSEMOVE:
//    fprintf(printer, "%s id=%d  WM_MOUSEMOVE\n", text, id);
//    fflush(printer);
    break;
  case WM_BUTTON1DOWN:
    fprintf(printer, "%s id=%d  WM_BUTTON1DOWN\n", text, id);
    fflush(printer);
    break;
  case WM_BUTTON1UP:
    fprintf(printer, "%s id=%d  WM_BUTTON1UP\n", text, id);
    fflush(printer);
    break;
  case WM_BUTTON1DBLCLK:
    fprintf(printer, "%s id=%d  WM_BUTTON1DBLCLK\n", text, id);
    fflush(printer);
    break;
  case WM_BUTTON2DOWN:
    fprintf(printer, "%s id=%d  WM_BUTTON2DOWN\n", text, id);
    fflush(printer);
    break;
  case WM_BUTTON2UP:    
    fprintf(printer, "%s id=%d  WM_BUTTON2UP\n", text, id);
    fflush(printer);
    break;
  case WM_BUTTON2DBLCLK:
    fprintf(printer, "%s id=%d  WM_BUTTON2DBLCLK\n", text, id);
    fflush(printer);
    break;
  case WM_BUTTON3UP:    
    fprintf(printer, "%s id=%d  WM_BUTTON3UP\n", text, id);
    fflush(printer);
    break;
  case WM_BUTTON3DBLCLK:
    fprintf(printer, "%s id=%d  WM_BUTTON3DBLCLK\n", text, id);
    fflush(printer);
    break;
  case WM_CHAR:
    fprintf(printer, "%s id=%d  WM_CHAR\n", text, id);
    fflush(printer);
    break;
  case WM_VIOCHAR:
    fprintf(printer, "%s id=%d  WM_VIOCHAR\n", text, id);
    fflush(printer);
    break;
  case WM_JOURNALNOTIFY:
    fprintf(printer, "%s id=%d  WM_JOURNALNOTIFY\n", text, id);
    fflush(printer);
    break;
  case WM_FLASHWINDOW:
    fprintf(printer, "%s id=%d  WM_FLASHWINDOW\n", text, id);
    fflush(printer);
    break;
  case WM_FORMATFRAME:
    fprintf(printer, "%s id=%d  WM_FORMATFRAME\n", text, id);
    fflush(printer);
    break;
  case WM_UPDATEFRAME:
    fprintf(printer, "%s id=%d  WM_UPDATEFRAME\n", text, id);
    fflush(printer);
    break;
  case WM_FOCUSCHANGE:
    fprintf(printer, "%s id=%d  WM_FOCUSCHANGE\n", text, id);
    fflush(printer);
    break;
  case WM_SETBORDERSIZE:
    fprintf(printer, "%s id=%d  WM_SETBORDERSIZE\n", text, id);
    fflush(printer);
    break;
  case WM_TRACKFRAME:
    fprintf(printer, "%s id=%d  WM_TRACKFRAME\n", text, id);
    fflush(printer);
    break;
  case WM_MINMAXFRAME:
    fprintf(printer, "%s id=%d  WM_MINMAXFRAME\n", text, id);
    fflush(printer);
    break;
  case WM_SETICON:
    fprintf(printer, "%s id=%d  WM_SETICON\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYICON:
    fprintf(printer, "%s id=%d  WM_QUERYICON\n", text, id);
    fflush(printer);
    break;
  case WM_SETACCELTABLE:
    fprintf(printer, "%s id=%d  WM_SETACCELTABLE\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYACCELTABLE:
    fprintf(printer, "%s id=%d  WM_QUERYACCELTABLE\n", text, id);
    fflush(printer);
    break;
  case WM_TRANSLATEACCEL:
    fprintf(printer, "%s id=%d  WM_TRANSLATEACCEL\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYTRACKINFO:
    fprintf(printer, "%s id=%d  WM_QUERYTRACKINFO\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYBORDERSIZE:
    fprintf(printer, "%s id=%d  WM_QUERYBORDERSIZE\n", text, id);
    fflush(printer);
    break;
  case WM_NEXTMENU:
    fprintf(printer, "%s id=%d  WM_NEXTMENU\n", text, id);
    fflush(printer);
    break;
  case WM_ERASEBACKGROUND:
    fprintf(printer, "%s id=%d  WM_ERASEBACKGROUND\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYFRAMEINFO:
    fprintf(printer, "%s id=%d  WM_QUERYFRAMEINFO\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYFOCUSCHAIN:
    fprintf(printer, "%s id=%d  WM_QUERYFOCUSCHAIN\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYFRAMECTLCOUNT:
    fprintf(printer, "%s id=%d  WM_QUERYFRAMECTLCOUNT\n", text, id);
    fflush(printer);
    break;
  case WM_QUERYCONVERTPOS:
    fprintf(printer, "%s id=%d  WM_QUERYCONVERTPOS\n", text, id);
    fflush(printer);
    break;
  case WM_ENTERNOTIFY:
    fprintf(printer, "%s id=%d  WM_ENTERNOTIFY\n", text, id);
    fflush(printer);
    break;
  case WM_LEAVENOTIFY:
    fprintf(printer, "%s id=%d  WM_LEAVENOTIFY\n", text, id);
    fflush(printer);
    break;
  default:
    fprintf(printer, "%s id=%d  Unknown-%x\n", text, id, msg);
    fflush(printer);
    break;
  }
  fclose(printer);
}
#endif
