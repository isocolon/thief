#include "thief.h"

void HISTORY_Init (void)
    {
    int nI ;

    History.nHistoryI = 0 ;
    History.nHistoryP = 0 ;

    for (nI = 0 ; nI < MAX_HISTORY ; nI++)
        {
        strcpy (History.cHistoryT [nI], "") ;
        }
    }

void HISTORY_Previous (void)
    {
    int nP ;
    CHARRANGE TSel ;

    nP = (History.nHistoryP - 1 + MAX_HISTORY) % MAX_HISTORY ;
    if (nP == History.nHistoryI || strlen(History.cHistoryT [nP]) == 0)
        {
        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
        return ;
        }

    History.nHistoryP = nP ;

    SetWindowText (hwndWindow [HWND_TELNET_EDIT], History.cHistoryT [History.nHistoryP]) ;

    TSel.cpMin = 999999 ;
    TSel.cpMax = 999999 ;
    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_EXSETSEL, 0, (LPARAM) &TSel) ;
    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

void HISTORY_Next (void)
    {
    CHARRANGE  TSel ;

    if (History.nHistoryP == History.nHistoryI)
        {
        SetWindowText (hwndWindow [HWND_TELNET_EDIT], "") ;
        }
    else
        {
        History.nHistoryP = (History.nHistoryP + 1) % MAX_HISTORY ;

        SetWindowText (hwndWindow [HWND_TELNET_EDIT], History.cHistoryT [History.nHistoryP]) ;

        TSel.cpMin = 999999 ;
        TSel.cpMax = 999999 ;
        SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_EXSETSEL, 0, (LPARAM) &TSel) ;
        }
    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

void HISTORY_Add (char *s)
    {
    if (strlen (s) > 0)
        {
        if (User.bEnterHighlight)
            {
            History.nHistoryI = (History.nHistoryI + 1) % MAX_HISTORY ;

            strcpy (History.cHistoryT [History.nHistoryI], s) ;
            }
        else
            {
            strcpy (History.cHistoryT [History.nHistoryI], s) ;

            History.nHistoryI = (History.nHistoryI + 1) % MAX_HISTORY ;
            }
        History.nHistoryP = History.nHistoryI ;
        }
    }
