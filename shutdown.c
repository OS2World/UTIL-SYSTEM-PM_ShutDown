/**
 **  PM Shutdown v0.1
 **
 **  (c) 1994 by Carsten Wimmer
 **
 **/

#define INCL_WINDIALOGS
#define INCL_WINFRAMEMGR
#define INCL_WINWORKPLACE
#define INCL_WINWINDOWMGR
#define INCL_WINTIMER
#include <os2.h>
#include <stdlib.h>
#include "shutdown.h"

#define APPNAME "PM Shutdown"
#define VERSION "v0.1"

MRESULT EXPENTRY shutdownProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

HWND hwndDlg;
HAB hab;
HMQ hmq;
ULONG TimerID, Seconds=1, TimeToGo;

void main(int argc, char *argv[])
{
  if( argc != 2 )
    return;

  TimeToGo = atol(argv[1]);

  if( !(hab = WinInitialize( 0 )) )
    return;

  if( !(hmq = WinCreateMsgQueue(hab, 0)) )
    return;

  hwndDlg = WinLoadDlg(HWND_DESKTOP, HWND_DESKTOP, shutdownProc, 0, ID_DIALOG, 0);

  WinProcessDlg(hwndDlg);

  WinDestroyWindow(hwndDlg);
  WinDestroyMsgQueue(hmq);
  WinTerminate(hab);

  return;
}

MRESULT EXPENTRY shutdownProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  char str[255];

  switch(msg)
  {
   case WM_INITDLG:
     sprintf(str, "%s %s (Timer: %ld out of %ld)", APPNAME, VERSION, Seconds, TimeToGo);
     WinSetWindowText(WinWindowFromID(hwnd, FID_TITLEBAR), (PSZ) str);
     TimerID = WinStartTimer(hab, hwnd, 50, 1000UL);
     break;
   case WM_COMMAND:
     switch( SHORT1FROMMP(mp1) )
     {
       case ID_BUTTON_OK:
         WinPostMsg(hwnd, WM_CLOSE, 0, 0);
         WinShutdownSystem(WinQueryAnchorBlock(HWND_DESKTOP), hmq);
         break;
       case ID_BUTTON_CANCEL:
         WinPostMsg(hwnd, WM_CLOSE, 0, 0);
         break;
       default:
         break;
     }
     break;
   case WM_TIMER:
     Seconds++;
     sprintf(str, "%s %s (Timer: %ld out of %ld)", APPNAME, VERSION, Seconds, TimeToGo);
     WinSetWindowText(WinWindowFromID(hwnd, FID_TITLEBAR), (PSZ) str);
     if( Seconds == TimeToGo ) {
       WinStopTimer(hab, hwnd, TimerID);
       WinPostMsg(hwnd, WM_COMMAND, MPFROMSHORT(ID_BUTTON_OK), 0);
     }
     break;
   case WM_CLOSE:
     WinDismissDlg(hwnd, TRUE);
     break;
   default:
     return(WinDefDlgProc(hwnd, msg, mp1, mp2));
  }

  return (MRESULT)0;
}

