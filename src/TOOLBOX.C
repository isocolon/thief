#include "thief.h"

int TOOLBOX_IsAlpha (char Temp)
    {
    if ((isalnum (Temp)) || (Temp == '.') || (Temp == ':') || (Temp == '-') || (Temp == '/') || (System.bIncludeSpace && (Temp != '\"')))
        {
        return 1 ;
        }
    return 0 ;
    }

int TOOLBOX_IsAlpha1 (char Temp, BOOL Ludens)
    {
    if (Login.nLoginType == SERVER_ICC)
        {
        return (isalnum (Temp) || (Temp == '-')) ;
        }
    if (Ludens)
		{
// 1.22		return ((isdigit (Temp) || (Temp == '.'))) ;
		return ((isalnum (Temp) || (Temp == '.') || (Temp == '@'))) ;
		}
	else
		{
		return (isalnum (Temp)) ;
		}
    }

void TOOLBOX_Write (char *ln)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ooo   [_MAX_PATH] ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;
    strcpy (Ext, OUT_EXT) ;
    _makepath (Ooo, Drive, Dir, File, Ext) ;

    if (System.bCDROMConnection)
        {
        return ;
        }

    Fv = fopen (Ooo, "a") ;
    if (Fv != NULL)
        {
        fprintf (Fv, "%s", ln) ;
        fclose (Fv) ;
        }
    }

void TOOLBOX_Writeln (char *ln)
    {
    FILE *Fv ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Ooo   [_MAX_PATH] ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;
    strcpy (Ext, OUT_EXT) ;
    _makepath (Ooo, Drive, Dir, File, Ext) ;

    if (System.bCDROMConnection)
        {
        return ;
        }

    Fv = fopen (Ooo, "a") ;
    if (Fv != NULL)
        {
        fprintf (Fv, "%s\n", ln) ;
        fclose (Fv) ;
        }
    }

void TOOLBOX_Beep (void)
    {
    if (User.bSound)
        {
        MessageBeep (0xFFFFFFFF) ;
        }
    }

void TOOLBOX_Error (char *s)
    {
    TOOLBOX_Beep () ;
    MessageBox (NULL, s, "Error", MB_ICONERROR) ;
    }

void TOOLBOX_SaveWindowCoord (HWND hwnd, int nG)
    {
    WINDOWPLACEMENT wp ;

    if (IsWindow (hwnd))
        {
        if (hwnd == hwndWindow [HWND_FRAME])
            {
            if (IsZoomed (hwnd))
                {
                ShowWindow (hwnd, SW_RESTORE) ;

                wp.length = sizeof (WINDOWPLACEMENT) ;
                GetWindowPlacement(hwnd, &wp) ;

                wCoord [nG].x = wp.rcNormalPosition.left ;
                wCoord [nG].y = wp.rcNormalPosition.top ;
                wCoord [nG].w = wp.rcNormalPosition.right  - wp.rcNormalPosition.left ;
                wCoord [nG].h = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top  ;

                ShowWindow (hwnd, SW_SHOWMAXIMIZED) ;
                }
            else
                {
                wp.length = sizeof (WINDOWPLACEMENT) ;
                GetWindowPlacement(hwnd, &wp) ;

                wCoord [nG].x = wp.rcNormalPosition.left ;
                wCoord [nG].y = wp.rcNormalPosition.top ;
                wCoord [nG].w = wp.rcNormalPosition.right  - wp.rcNormalPosition.left ;
                wCoord [nG].h = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top  ;
                }
            }
        else
            {
            wp.length = sizeof (WINDOWPLACEMENT) ;
            GetWindowPlacement(hwnd, &wp) ;

            wCoord [nG].x = wp.rcNormalPosition.left ;
            wCoord [nG].y = wp.rcNormalPosition.top ;
            wCoord [nG].w = wp.rcNormalPosition.right  - wp.rcNormalPosition.left ;
            wCoord [nG].h = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top  ;
            }
        }
    }

void TOOLBOX_GetWindowCoord (HWND hwnd, int *x, int *y, int *w, int *h)
    {
    WINDOWPLACEMENT wp ;

    if (IsWindow (hwnd))
        {
        if (hwnd == hwndWindow [HWND_FRAME])
            {
            if (IsZoomed (hwnd))
                {
                ShowWindow (hwnd, SW_RESTORE) ;

                wp.length = sizeof (WINDOWPLACEMENT) ;
                GetWindowPlacement(hwnd, &wp) ;

                *x = wp.rcNormalPosition.left ;
                *y = wp.rcNormalPosition.top ;
                *w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1 ;
                *h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1 ;

                ShowWindow (hwnd, SW_SHOWMAXIMIZED) ;
                }
            else
                {
                wp.length = sizeof (WINDOWPLACEMENT) ;
                GetWindowPlacement(hwnd, &wp) ;

                *x = wp.rcNormalPosition.left ;
                *y = wp.rcNormalPosition.top ;
                *w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1 ;
                *h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1 ;
                }
            }
        else
            {
            wp.length = sizeof (WINDOWPLACEMENT) ;
            GetWindowPlacement(hwnd, &wp) ;

            *x = wp.rcNormalPosition.left ;
            *y = wp.rcNormalPosition.top ;
            *w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1 ;
            *h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1 ;
            }
        }
    }

void TOOLBOX_TryAllWindowCoord (void)
    {
    int nI ;

    if (IsWindow (hwndWindow [HWND_FRAME]))
        {
        if (System.bMaximizeMain)
            {
            if (IsZoomed (hwndWindow [HWND_FRAME]))
                {
                }
            else
                {
                ShowWindow (hwndWindow [HWND_FRAME], SW_SHOWMAXIMIZED) ;
                }
            }
        else
            {
            if (IsIconic (hwndWindow [HWND_FRAME]) ||
                IsZoomed (hwndWindow [HWND_FRAME]))
                {
                ShowWindow (hwndWindow [HWND_FRAME], SW_RESTORE) ;
                }

            MoveWindow (hwndWindow [HWND_FRAME],
                        wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
                        wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
                        TRUE) ;
            }
        }

    if (IsWindow (hwndWindow [HWND_TELNET]))
        {
        if (IsIconic (hwndWindow [HWND_TELNET]) && wCoord [COORD_TELNET].s == WIN_HIDE)
            {
            }
        else
            {
            nI = wCoord [COORD_TELNET].s ;

            if (IsZoomed (hwndWindow [HWND_TELNET]) ||
                IsIconic (hwndWindow [HWND_TELNET]))
                {
                ShowWindow (hwndWindow [HWND_TELNET], SW_RESTORE) ;
                }

            MoveWindow (hwndWindow [HWND_TELNET],
                        wCoord [COORD_TELNET].x, wCoord [COORD_TELNET].y,
                        wCoord [COORD_TELNET].w, wCoord [COORD_TELNET].h,
                        TRUE) ;

            wCoord [COORD_TELNET].s = nI ;

            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE,
                         (WPARAM) (HWND) hwndWindow [HWND_TELNET], 0) ;
            }
        }

    if (IsWindow (hwndWindow [HWND_BUTTON]))
        {
        if (IsIconic (hwndWindow [HWND_BUTTON]) && wCoord [COORD_BUTTON].s == WIN_HIDE)
            {
            }
        else
            {
            nI = wCoord [COORD_BUTTON].s ;

            if (IsZoomed (hwndWindow [HWND_BUTTON]) ||
                IsIconic (hwndWindow [HWND_BUTTON]))
                {
                ShowWindow (hwndWindow [HWND_BUTTON], SW_RESTORE) ;
                }

            MoveWindow (hwndWindow [HWND_BUTTON],
                        wCoord [COORD_BUTTON].x, wCoord [COORD_BUTTON].y,
                        wCoord [COORD_BUTTON].w, wCoord [COORD_BUTTON].h,
                        TRUE) ;

            wCoord [COORD_BUTTON].s = nI ;

            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE,
                         (WPARAM) (HWND) hwndWindow [HWND_BUTTON], 0) ;
            }
        }

    if (IsWindow (hwndWindow [HWND_PLAY]))
        {
        if (IsIconic (hwndWindow [HWND_PLAY]) && wCoord [COORD_PLAY].s == WIN_HIDE)
            {
            }
        else
            {
            nI = wCoord [COORD_PLAY].s ;

            if (IsZoomed (hwndWindow [HWND_PLAY]) ||
                IsIconic (hwndWindow [HWND_PLAY]))
                {
                ShowWindow (hwndWindow [HWND_PLAY], SW_RESTORE) ;
                }

            MoveWindow (hwndWindow [HWND_PLAY],
                        wCoord [COORD_PLAY].x, wCoord [COORD_PLAY].y,
                        wCoord [COORD_PLAY].w, wCoord [COORD_PLAY].h,
                        TRUE) ;

            wCoord [COORD_PLAY].s = nI ;

            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE,
                         (WPARAM) (HWND) hwndWindow [HWND_PLAY], 0) ;
            }
        }

    for (nI = 1 ; nI < MAX_GAME ; nI++)
        {
        if (IsWindow (Game [nI].hwnd))
            {
            if (IsIconic (Game [nI].hwnd) || IsZoomed (Game [nI].hwnd))
                {
                ShowWindow (Game [nI].hwnd, SW_RESTORE) ;
                }

            MoveWindow (Game [nI].hwnd,
                        wCoord [nI].x, wCoord [nI].y,
                        wCoord [nI].w, wCoord [nI].h,
                        TRUE) ;

            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) Game [nI].hwnd, 0) ;
            }
        }
    }

void TOOLBOX_LoadAllWindowCoord (void)
    {
    int nI, nX, nY, nW, nH ;

    if (IsWindow (hwndWindow [HWND_FRAME]))
        {
        if (System.bMaximizeMain)
            {
            if (IsZoomed (hwndWindow [HWND_FRAME]))
                {
                }
            else
                {
                ShowWindow (hwndWindow [HWND_FRAME], SW_SHOWMAXIMIZED) ;
                }
            }
        else
            {
            TOOLBOX_GetWindowCoord (hwndWindow [HWND_FRAME], &nX, &nY, &nW, &nH) ;

            if ((nX != wCoord [COORD_MAIN].x) ||
                (nY != wCoord [COORD_MAIN].y) ||
                (nW != wCoord [COORD_MAIN].w) ||
                (nH != wCoord [COORD_MAIN].h))
                {
                if (IsIconic (hwndWindow [HWND_FRAME]) ||
                    IsZoomed (hwndWindow [HWND_FRAME]))
                    {
                    ShowWindow (hwndWindow [HWND_FRAME], SW_RESTORE) ;
                    }

                MoveWindow (hwndWindow [HWND_FRAME],
                            wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
                            wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
                            TRUE) ;
                }
            }
        }

    if (IsWindow (hwndWindow [HWND_TELNET]))
        {
        if (IsIconic (hwndWindow [HWND_TELNET]) && wCoord [COORD_TELNET].s == WIN_HIDE)
            {
            }
        else
            {
            TOOLBOX_GetWindowCoord (hwndWindow [HWND_TELNET], &nX, &nY, &nW, &nH) ;

            if ((nX != wCoord [COORD_TELNET].x) ||
                (nY != wCoord [COORD_TELNET].y) ||
                (nW != wCoord [COORD_TELNET].w) ||
                (nH != wCoord [COORD_TELNET].h))
                {
                nI = wCoord [COORD_TELNET].s ;

                if (IsZoomed (hwndWindow [HWND_TELNET]) ||
                    IsIconic (hwndWindow [HWND_TELNET]))
                    {
                    ShowWindow (hwndWindow [HWND_TELNET], SW_RESTORE) ;
                    }

                MoveWindow (hwndWindow [HWND_TELNET],
                            wCoord [COORD_TELNET].x, wCoord [COORD_TELNET].y,
                            wCoord [COORD_TELNET].w, wCoord [COORD_TELNET].h,
                            TRUE) ;

                wCoord [COORD_TELNET].s = nI ;

                SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE,
                             (WPARAM) (HWND) hwndWindow [HWND_TELNET], 0) ;
                }
            }
        }

    if (IsWindow (hwndWindow [HWND_BUTTON]))
        {
        if (IsIconic (hwndWindow [HWND_BUTTON]) && wCoord [COORD_BUTTON].s == WIN_HIDE)
            {
            }
        else
            {
            TOOLBOX_GetWindowCoord (hwndWindow [HWND_BUTTON], &nX, &nY, &nW, &nH) ;

            if ((nX != wCoord [COORD_BUTTON].x) ||
                (nY != wCoord [COORD_BUTTON].y) ||
                (nW != wCoord [COORD_BUTTON].w) ||
                (nH != wCoord [COORD_BUTTON].h))
                {
                nI = wCoord [COORD_BUTTON].s ;

                if (IsZoomed (hwndWindow [HWND_BUTTON]) ||
                    IsIconic (hwndWindow [HWND_BUTTON]))
                    {
                    ShowWindow (hwndWindow [HWND_BUTTON], SW_RESTORE) ;
                    }

                MoveWindow (hwndWindow [HWND_BUTTON],
                            wCoord [COORD_BUTTON].x, wCoord [COORD_BUTTON].y,
                            wCoord [COORD_BUTTON].w, wCoord [COORD_BUTTON].h,
                            TRUE) ;

                wCoord [COORD_BUTTON].s = nI ;

                SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE,
                             (WPARAM) (HWND) hwndWindow [HWND_BUTTON], 0) ;
                }
            }
        }

    if (IsWindow (hwndWindow [HWND_PLAY]))
        {
        if (IsIconic (hwndWindow [HWND_PLAY]) && wCoord [COORD_PLAY].s == WIN_HIDE)
            {
            }
        else
            {
            TOOLBOX_GetWindowCoord (hwndWindow [HWND_PLAY], &nX, &nY, &nW, &nH) ;

            if ((nX != wCoord [COORD_PLAY].x) ||
                (nY != wCoord [COORD_PLAY].y) ||
                (nW != wCoord [COORD_PLAY].w) ||
                (nH != wCoord [COORD_PLAY].h))
                {
                nI = wCoord [COORD_PLAY].s ;

                if (IsZoomed (hwndWindow [HWND_PLAY]) ||
                    IsIconic (hwndWindow [HWND_PLAY]))
                    {
                    ShowWindow (hwndWindow [HWND_PLAY], SW_RESTORE) ;
                    }

                MoveWindow (hwndWindow [HWND_PLAY],
                            wCoord [COORD_PLAY].x, wCoord [COORD_PLAY].y,
                            wCoord [COORD_PLAY].w, wCoord [COORD_PLAY].h,
                            TRUE) ;

                wCoord [COORD_PLAY].s = nI ;

                SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE,
                             (WPARAM) (HWND) hwndWindow [HWND_PLAY], 0) ;
                }
            }
        }

    for (nI = 1 ; nI < MAX_GAME ; nI++)
        {
        if (IsWindow (Game [nI].hwnd))
            {
            TOOLBOX_GetWindowCoord (Game [nI].hwnd, &nX, &nY, &nW, &nH) ;

            if ((nX != wCoord [nI].x) ||
                (nY != wCoord [nI].y) ||
                (nW != wCoord [nI].w) ||
                (nH != wCoord [nI].h))
                {
                if (IsIconic (Game [nI].hwnd) || IsZoomed (Game [nI].hwnd))
                    {
                    ShowWindow (Game [nI].hwnd, SW_RESTORE) ;
                    }

                MoveWindow (Game [nI].hwnd,
                            wCoord [nI].x, wCoord [nI].y,
                            wCoord [nI].w, wCoord [nI].h,
                            TRUE) ;

                SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) Game [nI].hwnd, 0) ;
                }
            }
        }
    }

void TOOLBOX_SaveAllWindowCoord (void)
    {
    int nI ;

    TOOLBOX_SaveWindowCoord (hwndWindow [HWND_FRAME],  COORD_MAIN) ;
    TOOLBOX_SaveWindowCoord (hwndWindow [HWND_TELNET], COORD_TELNET) ;
    TOOLBOX_SaveWindowCoord (hwndWindow [HWND_BUTTON], COORD_BUTTON) ;
    TOOLBOX_SaveWindowCoord (hwndWindow [HWND_PLAY],   COORD_PLAY) ;

    for (nI = 1 ; nI < MAX_GAME ; nI++)
        {
        TOOLBOX_SaveWindowCoord (Game [nI].hwnd, nI) ;
        }
    }

void TOOLBOX_ReadINICoord (WCOORD *wc, char *cFn, char *cGroup, char *cName)
    {
    char cTmp [255] ;

    sprintf (cTmp, "%d %d %d %d", wc->x, wc->y, wc->w, wc->h) ;
    GetPrivateProfileString (cGroup, cName, cTmp, cTmp, 41, cFn) ;

    sscanf (cTmp, "%d %d %d %d", &wc->x, &wc->y, &wc->w, &wc->h) ;
    }

void TOOLBOX_WriteINICoord (FILE *Fv, WCOORD *wc, char *cName)
    {
    char cTmp [255] ;

    sprintf (cTmp, "%d %d %d %d", wc->x, wc->y, wc->w, wc->h) ;
    fprintf (Fv, "%s=%s\n", cName, cTmp) ;
    }

void TOOLBOX_ReadINIColor (char *Ini, COLORREF *clr, char *cGroup, char *cName)
    {
    char cTmp [255] ;

    sprintf (cTmp, "%lx", (*clr)) ;
    GetPrivateProfileString (cGroup, cName, cTmp, cTmp, 41, Ini) ;
    sscanf (cTmp, "%lx", clr) ;
    }

int TOOLBOX_ReadINIColor1 (char *Ini, COLORREF *clr, char *cGroup, char *cName)
    {
    char cTmp1 [255], cTmp2 [255] ;

    strcpy (cTmp1, "ZZZZZZ") ;
    strcpy (cTmp2, "ZZZZZZ") ;

    GetPrivateProfileString (cGroup, cName, cTmp1, cTmp2, 41, Ini) ;

    if (strcmp (cTmp2, "ZZZZZZ") == 0)
        {
        sscanf ("000000", "%lx", clr) ;
        return 0 ;
        }

    sscanf (cTmp2, "%lx", clr) ;
    return 1 ;
    }

void TOOLBOX_WriteINIColor (FILE *Fv, COLORREF clr, char *cName)
    {
    fprintf (Fv, "%s=%06lx\n", cName, clr) ;
    }

void TOOLBOX_ReadINIFont (char *Ini, int nI, char *cGroup, char *cName)
    {
    int  nJ, nL ;
    char cTmp [255], cTmp1 [255] ;

    sprintf (cTmp, "%s,%d,%d,%d,%d,%d",
                   hfFont [nI].Name,
                   hfFont [nI].nPointSize,
                   hfFont [nI].bBold      ? 1 : 0,
                   hfFont [nI].bItalic    ? 1 : 0,
                   hfFont [nI].bUnderLine ? 1 : 0,
                   hfFont [nI].bStrikeOut ? 1 : 0) ;

    GetPrivateProfileString (cGroup, cName, cTmp, cTmp, 81, Ini) ;

    nL = strlen (cTmp) ;
    for (nJ = 0 ; nJ < nL ; nJ++)
        {
        if (cTmp [nJ] == ',')
            {
            break ;
            }
        cTmp1 [nJ] = cTmp [nJ] ;
        }
    cTmp1 [nJ] = NULL_CHAR ;

    sscanf (cTmp + nJ, ",%d,%d,%d,%d,%d",
                       &hfFont [nI].nPointSize,
                       &hfFont [nI].bBold,
                       &hfFont [nI].bItalic,
                       &hfFont [nI].bUnderLine,
                       &hfFont [nI].bStrikeOut) ;

    strcpy (hfFont [nI].Name, cTmp1) ;

    hfFont [nI].bBold      = hfFont [nI].bBold      ? 1 : 0 ;
    hfFont [nI].bItalic    = hfFont [nI].bItalic    ? 1 : 0 ;
    hfFont [nI].bUnderLine = hfFont [nI].bUnderLine ? 1 : 0 ;
    hfFont [nI].bStrikeOut = hfFont [nI].bStrikeOut ? 1 : 0 ;
    }

void TOOLBOX_WriteINIFont (FILE *Fv, int nI, char *cName)
    {
    char cTmp [255] ;

    sprintf (cTmp, "%s,%d,%d,%d,%d,%d",
                    hfFont [nI].Name,
                    hfFont [nI].nPointSize,
                    hfFont [nI].bBold      ? 1 : 0,
                    hfFont [nI].bItalic    ? 1 : 0,
                    hfFont [nI].bUnderLine ? 1 : 0,
                    hfFont [nI].bStrikeOut ? 1 : 0) ;

    fprintf (Fv, "%s=%s\n", cName, cTmp) ;
    }

void TOOLBOX_MakeFont (int nI)
    {
    FONT_CreateFont (nI,
                     hfFont [nI].Name,       hfFont [nI].nPointSize,
                     hfFont [nI].bBold,      hfFont [nI].bItalic,
                     hfFont [nI].bUnderLine, hfFont [nI].bStrikeOut) ;
    }

void TOOLBOX_ConnectedMenu (void)
    {
    int nI ;

    EnableMenuItem (hMenu [MENU_INIT], IDM_NEW_CONNECT,   MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_CONNECT,       MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_RECONNECT,     MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_QUICK_CONNECT, MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_CDROM_CONNECT, MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_DISCONNECT,    MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_RUN_SCRIPT,    MF_ENABLED) ;

    for (nI = 0 ; nI < Login.nLoginString ; nI++)
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_EXTRA + nI, MF_GRAYED) ;
        }

    EnableMenuItem (hMenu [MENU_INIT], IDM_SEEK_GAME,     MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MATCH_PLAYER,  MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_REMATCH,       MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_RESUME_GAME,   MF_ENABLED) ;

    EnableMenuItem (hMenu [MENU_INIT], IDM_TELL_USER,          MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MESSAGE_USER,       MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_RATING_NOTES,    MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_VARS,            MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_PING,            MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_HISTORY,         MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_STORED,          MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_JOURNAL,         MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_BUG_PLAY,      MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_BUG_PARTNERED, MF_ENABLED) ;

    if (Login.nLoginType == SERVER_ICC)
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_BUG_OPEN, MF_GRAYED) ;
        }
    else
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_BUG_OPEN, MF_ENABLED) ;
        }

    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_PLAYERS,     MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_ALL_PLAYERS, MF_ENABLED) ;

#ifndef KICS
    if (Login.nLoginType == SERVER_FICS)
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_FICS_REGISTER, MF_ENABLED) ;
        }
    else
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_FICS_REGISTER, MF_GRAYED) ;
        }
#endif

    EnableMenuItem (hMenu [MENU_INIT], IDM_ASK_QUESTION, MF_ENABLED) ;

    TOOLBOX_SetTimerCommandMenu () ;
    }

void TOOLBOX_DisconnectedMenu (void)
    {
    int nI ;

    EnableMenuItem (hMenu [MENU_INIT], IDM_NEW_CONNECT, MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_CONNECT,     MF_ENABLED) ;

    if (strlen (System.cLoginFile) == 0)
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_RECONNECT, MF_GRAYED) ;
        }
    else
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_RECONNECT, MF_ENABLED) ;
        }

    EnableMenuItem (hMenu [MENU_INIT], IDM_QUICK_CONNECT, MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_CDROM_CONNECT, MF_ENABLED) ;

    EnableMenuItem (hMenu [MENU_INIT], IDM_DISCONNECT, MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_RUN_SCRIPT, MF_GRAYED) ;

    for (nI = 0 ; nI < Login.nLoginString ; nI++)
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_EXTRA + nI, MF_ENABLED) ;
        }

    EnableMenuItem (hMenu [MENU_INIT], IDM_SEEK_GAME,     MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MATCH_PLAYER,  MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_REMATCH,       MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_RESUME_GAME,   MF_GRAYED) ;

    EnableMenuItem (hMenu [MENU_INIT], IDM_TELL_USER,          MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MESSAGE_USER,       MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_RATING_NOTES,    MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_VARS,            MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_PING,            MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_HISTORY,         MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_STORED,          MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_JOURNAL,         MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_BUG_PLAY,      MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_BUG_PARTNERED, MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_BUG_OPEN,      MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_PLAYERS,       MF_GRAYED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_LIST_ALL_PLAYERS,   MF_GRAYED) ;
#ifndef KICS
    EnableMenuItem (hMenu [MENU_INIT], IDM_FICS_REGISTER,      MF_ENABLED) ;
#endif
    EnableMenuItem (hMenu [MENU_INIT], IDM_ASK_QUESTION, MF_GRAYED) ;
        
    User.bIamAGuest = 0 ;
    }

void TOOLBOX_CheckGuestMenu (void)
    {
    EnableMenuItem (hMenu [MENU_INIT], IDM_MESSAGE_USER, User.bIamAGuest ? MF_GRAYED : MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_STORED,    User.bIamAGuest ? MF_GRAYED : MF_ENABLED) ;
    EnableMenuItem (hMenu [MENU_INIT], IDM_MY_JOURNAL,   User.bIamAGuest ? MF_GRAYED : MF_ENABLED) ;
    }

void TOOLBOX_CheckAllMenu (void)
    {
    CheckMenuItem (hMenu [MENU_INIT], IDM_LOG_TELNET,       User.bLogTelnet      ? MF_CHECKED : MF_UNCHECKED) ;
    CheckMenuItem (hMenu [MENU_INIT], IDM_LOG_GAMES,        User.bLogGame        ? MF_CHECKED : MF_UNCHECKED) ;
    CheckMenuItem (hMenu [MENU_INIT], IDM_APPLY_SOUND,      User.bSound          ? MF_CHECKED : MF_UNCHECKED) ;
	CheckMenuItem (hMenu [MENU_INIT], IDM_EXIT_PROMPT,      User.bSuppressExitPrompt ? MF_CHECKED : MF_UNCHECKED) ;
    CheckMenuItem (hMenu [MENU_INIT], IDM_SAVE_SET_EXIT,    User.bSaveOnExit     ? MF_CHECKED : MF_UNCHECKED) ;
	CheckMenuItem (hMenu [MENU_INIT], IDM_MINIMIZE_TRAY,     User.bMinimizetoTray ? MF_CHECKED : MF_UNCHECKED) ;
    CheckMenuItem (hMenu [MENU_INIT], IDM_SHOW_BITMAP_SIZE, User.bShowBitmapSize ? MF_CHECKED : MF_UNCHECKED) ;
    CheckMenuItem (hMenu [MENU_INIT], IDM_SHOW_MENU_PATH,   User.bShowMenuPath   ? MF_CHECKED : MF_UNCHECKED) ;

    RIGHTMOUSE_LoadCheck () ;
    }

void TOOLBOX_SetTimerCommandMenu (void)
    {
    if (Timeseal.bSocketIsOpen)
        {
        if (TimerCmd.bTimerCommand)
            {
            EnableMenuItem (hMenu [MENU_INIT], IDM_START_TIMER, MF_GRAYED) ;
            EnableMenuItem (hMenu [MENU_INIT], IDM_STOP_TIMER,  MF_ENABLED) ;
            }
        else
            {
            EnableMenuItem (hMenu [MENU_INIT], IDM_START_TIMER, MF_ENABLED) ;
            EnableMenuItem (hMenu [MENU_INIT], IDM_STOP_TIMER,  MF_GRAYED) ;
            }
        }
    else
        {
        EnableMenuItem (hMenu [MENU_INIT], IDM_START_TIMER, MF_GRAYED) ;
        EnableMenuItem (hMenu [MENU_INIT], IDM_STOP_TIMER,  MF_GRAYED) ;
        }
    }

void TOOLBOX_RefreshWindowColor (void)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_GAME ; nI++)
        {
        if (IsWindow (Game [nI].hwnd))
            {
            DeleteObject ((HBRUSH) SetClassLong (Game [nI].hwnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush (clrColor [CLR_WINDOW_COLOR]))) ;
            }
        }
    }

void TOOLBOX_InvalidateBoardWindow (int bReLoad)
    {
    int nI ;
    RECT rc ;
    HDC hdc ;

    for (nI = 0 ; nI < MAX_GAME ; nI++)
        {
        if (IsWindow (Game [nI].hwnd))
            {
            hdc = GetDC (Game [nI].hwnd) ;

            if (IsIconic (Game [nI].hwnd))
                {
                if (nI == INDEX_PLAY)
                    {
                    if (bReLoad)
                        {
                        CSET_Load (hdc) ;
                        }
                    }
                }
            else
                {
                GetClientRect (Game [nI].hwnd, &rc) ;

                if (nI == INDEX_PLAY)
                    {
                    if (bReLoad)
                        {
                        CSET_Load (hdc) ;
                        }
                    }

                switch (User.nBufferOrientation)
                    {
                    case DEFAULT_BUFFER_LEFT :
                        BOARD_ResizeLeft (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                        break ;

                    case DEFAULT_BUFFER_RIGHT :
                        BOARD_ResizeRight (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                        break ;

                    case DEFAULT_BUFFER_TOPBOTTOML :
                        BOARD_ResizeTopBottomL (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                        break ;

                    case DEFAULT_BUFFER_TOPBOTTOMR :
                        BOARD_ResizeTopBottomR (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                        break ;

                    default :
                        BOARD_ResizeRight (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                        break ;
                    }

                BOARD_LoadBitmaps (nI, hdc) ;
                BOARD_CheckFlip (nI) ;
                BOARD_DrawRepaint (nI, Game [nI].hwnd, hdc) ;
                }

            ReleaseDC (Game [nI].hwnd, hdc) ;
            }
        }
    }

void TOOLBOX_InvalidateBoardWindow1 (int bReLoad)
    {
    int nI ;
    RECT rc ;
    HDC hdc ;

    for (nI = 0 ; nI < MAX_GAME ; nI++)
        {
        if (IsWindow (Game [nI].hwnd))
            {
            hdc = GetDC (Game [nI].hwnd) ;

            if (IsIconic (Game [nI].hwnd))
                {
                if (nI == INDEX_PLAY)
                    {
                    if (bReLoad)
                        {
                        CSET_Load (hdc) ;
                        }
                    }
                }
            else
                {
                if (IsZoomed (Game [nI].hwnd))
                    {
                    ShowWindow (Game [nI].hwnd, SW_RESTORE) ;
                    }

                GetClientRect (Game [nI].hwnd, &rc) ;

                if (nI == INDEX_PLAY)
                    {
                    if (bReLoad)
                        {
                        CSET_Load (hdc) ;
                        }
                    }

                SendMessage (Game [nI].hwnd, WM_SIZE, SIZE_RESTORED, (LPARAM) ((rc.bottom << 16) | rc.right)) ;
                InvalidateRect (Game [nI].hwnd, NULL, TRUE) ;
                }

            ReleaseDC (Game [nI].hwnd, hdc) ;
            }
        }
    }

void TOOLBOX_CloseObserveWindow (void)
    {
    int nI ;

    for (nI = 1 ; nI < MAX_GAME ; nI++)
        {
        if (IsWindow (Game [nI].hwnd))
            {
            SendMessage (Game [nI].hwnd, WM_CLOSE, 0, 0) ;
            }
        }
    }

int TOOLBOX_CenterWindow (HWND hwndChild, HWND hwndParent)
    {
    RECT rChild, rParent ;
    int  wChild, hChild, wParent, hParent ;
    int  wScreen, hScreen, xNew, yNew ;
    HDC  hdc ;

    // get the Height and Width of the child window
    GetWindowRect (hwndChild, &rChild) ;
    wChild = rChild.right - rChild.left ;
    hChild = rChild.bottom - rChild.top ;

    // get the Height and Width of the parent window
    GetWindowRect (hwndParent, &rParent) ;
    wParent = rParent.right - rParent.left ;
    hParent = rParent.bottom - rParent.top ;

    // get the display limits
    hdc     = GetDC (hwndChild) ;
    wScreen = GetDeviceCaps (hdc, HORZRES) ;
    hScreen = GetDeviceCaps (hdc, VERTRES) ;
    ReleaseDC(hwndChild, hdc) ;

    // calculate new X position, then adjust for screen
    xNew = rParent.left + ((wParent - wChild) >> 1) ;
    if (xNew < 0)
        {
        xNew = 0 ;
        }
    else
        {
        if ((xNew + wChild) > wScreen)
            {
            xNew = wScreen - wChild ;
            }
        }

    // calculate new Y position, then adjust for screen
    yNew = rParent.top  + ((hParent - hChild) >> 1) ;
    if (yNew < 0)
        {
        yNew = 0 ;
        }
    else
        {
        if ((yNew + hChild) > hScreen)
            {
            yNew = hScreen - hChild ;
            }
        }
    return SetWindowPos (hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ;
    }

FILE * TOOLBOX_OpenFileDialog (HWND hwnd, BOOL write, char *defName,
                               char *defExt, char *nameFilt, char *dlgTitle,
                               UINT *number, char fileTitle [MSG_SIZE],
                               char fileName [MSG_SIZE], char Init [_MAX_PATH])
    {
    OPENFILENAME openFileName ;
    char         buf1 [MSG_SIZE] ;
    FILE         *Fv ;

    if (fileName == NULL)
        {
        fileName = buf1 ;
        }

    if (defName == NULL)
        {
        strcpy(fileName, "*.") ;
        strcat(fileName, defExt) ;
        }
    else
        {
        strcpy(fileName, defName) ;
        }

    if (fileTitle)
        {
        strcpy(fileTitle, "") ;
        }

    if (number)
        {
        *number = 0 ;
        }

    openFileName.lStructSize       = sizeof (OPENFILENAME) ;
    openFileName.hwndOwner         = hwnd ;
    openFileName.hInstance         = NULL ;
    openFileName.lpstrFilter       = nameFilt ;
    openFileName.lpstrCustomFilter = NULL ;
    openFileName.nMaxCustFilter    = 0 ;
    openFileName.nFilterIndex      = 1 ;
    openFileName.lpstrFile         = fileName ;
    openFileName.nMaxFile          = MSG_SIZE ;
    openFileName.lpstrFileTitle    = fileTitle ;
    openFileName.nMaxFileTitle     = (fileTitle ? MSG_SIZE : 0) ;
    openFileName.lpstrInitialDir   = Init ;
    openFileName.lpstrTitle        = dlgTitle ;
    openFileName.Flags             = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
                                    (write ? 0 : OFN_FILEMUSTEXIST) |
                                    (number ? OFN_ENABLETEMPLATE |
                                              OFN_ENABLEHOOK: 0) |
                                    OFN_EXPLORER ;
    openFileName.nFileOffset       = 0 ;
    openFileName.nFileExtension    = 0 ;
    openFileName.lpstrDefExt       = defExt ;
    openFileName.lCustData         = (LONG) number ;
    openFileName.lpfnHook          = NULL ;
    openFileName.lpTemplateName    = NULL ;

    if (write ? GetSaveFileName (&openFileName) : GetOpenFileName (&openFileName))
        {
        // open the file
        Fv = fopen (openFileName.lpstrFile, write ? "a" : "rb") ;
        if (Fv == NULL)
            {
            return NULL ;
            }
        }
    else
        {
        return FALSE ;
        }
    return Fv ;
    }

void TOOLBOX_AllTrim (char *cSt)
    {
    int nLp = 0, nRp = 0, nI, nJ ;

    for (nI = 0 ; cSt [ nI ] != NULL_CHAR ; nI++)
        {
        if (cSt [ nI ] != ' ')
            {
            nLp = nI ;
            break ;
            }
        }

    nJ = 0 ;
    for (nI = nLp ; cSt [ nI ] != NULL_CHAR ; nI++)
        {
        cSt [ nJ++ ] = cSt [ nI ] ;
        if (cSt [ nI ] != ' ')
            {
            nRp = nJ ;
            }
        }
    cSt [ nRp ] = NULL_CHAR ;
    }

int TOOLBOX_CanOpenFile (char *Fn)
    {
    FILE *Fv ;

    Fv = fopen (Fn, "r") ;
    if (Fv == NULL)
        {
        return 0 ;
        }
    else
        {
        fclose (Fv) ;
        return 1 ;
        }
    }

void TOOLBOX_SetTelnetCaption (void)
    {
    char cTmp [100] ;

    if (Timeseal.bSocketIsOpen)
        {
        if (strlen (Vars.cWhoAmI) == 0)
            {
            strcpy (cTmp, Login.cServerName) ;
            }
        else
            {
            if (strlen (Vars.cPartner) == 0)
                {
                if (strlen (Vars.cFollow) == 0)
                    {
                    if (strlen (Vars.cPfollow) == 0)
                        {
                        sprintf (cTmp, "%s (%s)", Login.cServerName, Vars.cWhoAmI) ;
                        }
                    else
                        {
                        sprintf (cTmp, "%s (%s) <%s's partner>", Login.cServerName, Vars.cWhoAmI, Vars.cPfollow) ;
                        }
                    }
                else
                    {
                    sprintf (cTmp, "%s (%s) <%s>", Login.cServerName, Vars.cWhoAmI, Vars.cFollow) ;
                    }
                }
            else
                {
                if (strlen (Vars.cFollow) == 0)
                    {
                    if (strlen (Vars.cPfollow) == 0)
                        {
                        sprintf (cTmp, "%s (%s + %s)", Login.cServerName, Vars.cWhoAmI, Vars.cPartner) ;
                        }
                    else
                        {
                        sprintf (cTmp, "%s (%s + %s) <%s's partner>", Login.cServerName, Vars.cWhoAmI, Vars.cPartner, Vars.cPfollow) ;
                        }
                    }
                else
                    {
                    sprintf (cTmp, "%s (%s + %s) <%s>", Login.cServerName, Vars.cWhoAmI, Vars.cPartner, Vars.cFollow) ;
                    }
                }
            }
        }
    else
        {
        strcpy (cTmp, TELNET_TITLE) ;
        }
    SetWindowText (hwndWindow [HWND_TELNET], cTmp) ;
    }

void TOOLBOX_WriteICS (char *cSt)
    {
    if (Login.bLoginHelper)
        {
        PMI_TS_Write (cSt) ;
        }
    else
        {
        send (Timeseal.skSocket, cSt, strlen (cSt), NO_FLAGS_SET) ;
        }
    }

void TOOLBOX_WriteUser (char *cS)
    {
    int bF ;
    COLORREF nC ;

    bF = Telnet.bForceVisible ;
    nC = Telnet.clrCurrent ;
    Telnet.clrCurrent    = clrColor [CLR_TELNET_USERTEXT] ;
    Telnet.bForceVisible = 1 ;
    if (Telnet.bLastTelnetIsCR)
        {
	TELNET_NormalWrite (cS) ;
        }
    else
        {
	TELNET_NormalWrite ("\n") ;
	TELNET_NormalWrite (cS) ;
        }
    Telnet.bForceVisible = bF ;
    Telnet.bLastIsTelnetEdit = 1 ;
    Telnet.clrCurrent = nC ;
    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

void TOOLBOX_WriteSystem (char *cS)
    {
    int bF ;
    COLORREF nC ;

    if (IsWindow (hwndWindow [HWND_TELNET_TEXT]))
        {
        bF = Telnet.bForceVisible ;
        nC = Telnet.clrCurrent ;
        Telnet.clrCurrent    = clrColor [CLR_TELNET_USERTEXT] ;
        Telnet.bForceVisible = 1 ;
        if (Telnet.bLastTelnetIsCR)
            {
	    TELNET_ItalicWrite (cS) ;
            }
        else
            {
	    TELNET_ItalicWrite ("\n") ;
	    TELNET_ItalicWrite (cS) ;
            }
        Telnet.bForceVisible = bF ;
        Telnet.bLastIsTelnetEdit = 1 ;
        Telnet.clrCurrent = nC ;
        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
        }
    }

void TOOLBOX_CloseSocket (void)
    {
    char cTmp [_MAX_PATH + 100] ;

    System.bIsMyTurn = 0 ;

    User.bIamAGuest = 0 ;

    Timeseal.bSocketIsOpen = 0 ;
    Timeseal.nSocketLogin  = 0 ;
    Timeseal.bBeforeLogin  = 1 ;
    Timeseal.bSocketLine   = 0 ;
    strcpy (Timeseal.cSocketLine, "") ;

    Telnet.bFoundHandle    = 0 ;
    Telnet.bTelnetSkipLine = 0 ;
    Telnet.clrCurrent      = clrColor [CLR_TELNET_NORMAL] ;

    Fics.bPobserve = 1 ;
    Fics.nPobserve = 0 ;
    strcpy (Fics.cPobserve, "") ;

    if (Login.bLoginHelper)
        {
        PMI_TS_Destroy() ;
        }
    else
        {
        closesocket (Timeseal.skSocket) ;
        }

    TOOLBOX_DisconnectedMenu () ;
    TOOLBOX_SetTimerCommandMenu () ;
    TOOLBOX_SetTelnetCaption () ;

    TIMERCMD_Clear () ;
    VARS_Init () ;
    HISTORY_Init () ;
    F9KEY_Init () ;
    STATE_Logout () ;

    if (User.bLogTelnet)
        {
        if (LOG_End ())
            {
            sprintf (cTmp, "Closing %s Successful\n", Log.cFn) ;
            TOOLBOX_WriteSystem (cTmp) ;
            }
        LOG_Init () ;
        }
    }

int TOOLBOX_ReadScriptFile (char *cFn)
    {
    FILE *Fv ;
    char cTmp [2048] ;
    int  nL ;

    if (strlen (cFn) == 0)
        {
        return 0 ;
        }

    Fv = fopen (cFn, "r") ;
    if (Fv == NULL)
        {
        return 0 ;
        }

    while (fgets (cTmp, 2048, Fv) != NULL)
        {
        TOOLBOX_AllTrim (cTmp) ;

        nL = strlen (cTmp) ;
        if (nL > 0)
            {
            if (cTmp [0] != ';')
                {
                if (cTmp [nL - 1] != '\n')
                    {
                    cTmp [nL    ] = '\n' ;
                    cTmp [nL + 1] = NULL_CHAR ;
                    }

                TOOLBOX_WriteICS    (cTmp) ;
                TOOLBOX_WriteSystem (cTmp) ;
                }
            }
        }
    fclose (Fv) ;

    sprintf (cTmp, "Running Script %s Successful\n", cFn) ;
    TOOLBOX_WriteSystem (cTmp) ;
    return 1 ;
    }

int TOOLBOX_FindGameByNumber (int nG)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_GAME ; nI++)
        {
        if ((Game [nI].bValid) && (Game [nI].nGameNumber == nG))
            {
            return nI ;
            }
	}
    return -1 ;
    }

int TOOLBOX_ExpandHandleMacro (char *cD, char *cS, char *cH, char *cA)
    {
    int  nH, nJ, nK, nL, nM, bF = 0 ;
    char cTmp [2048] ;

    nL = strlen (cS) ;
    if (nL <= 0)
        {
        strcpy (cD, "") ;
        }
    else
        {
        strcpy (cTmp, cS) ;
        strlwr (cTmp) ;

        if (strstr (cTmp, cH))
            {
            nH = strlen (cH) ;
            for (nJ = 0 ; nJ < nL ; nJ++)
                {
                if (! strncmp ((cTmp + nJ), cH, nH))
                    {
                    for (nK = 0 ; nK < nJ ; nK++)
                        {
                        cD [nK] = cS [nK] ;
                        }
                    cD [nJ] = NULL_CHAR ;

                    strcat (cD, cA) ;

                    nM = strlen (cD) ;

                    for (nK = nJ + nH ; nK < nL ; nK++)
                        {
                        cD [nM++] = cS [nK] ;
                        }
                    cD [nM] = NULL_CHAR ;

                    bF = 1 ;
                    break ;
                    }
                }
            }
        if (! bF)
            {
            strcpy (cD, cS) ;
            }
        }
    return bF ;
    }

void TOOLBOX_ExpandAllMacro (char *cD, char *cS, int bChecknR)
    {
    int nI ;
    char cTmpA [2048], cTmpB [2048], cResult [2048] ;

    TOOLBOX_ExpandHandleMacro (cTmpA,   cS,    VAR_WHOAMI,          Vars.cWhoAmI) ;
    TOOLBOX_ExpandHandleMacro (cTmpB,   cTmpA, VAR_PARTNER,         Vars.cPartner) ;
    TOOLBOX_ExpandHandleMacro (cTmpA,   cTmpB, VAR_OPPONENT,        Vars.cOpponent) ;
    TOOLBOX_ExpandHandleMacro (cTmpB,   cTmpA, VAR_PARTNEROPPONENT, Vars.cPartnerOpponent) ;
    TOOLBOX_ExpandHandleMacro (cTmpA,   cTmpB, VAR_LASTTELL,        Vars.cLastTell) ;
    TOOLBOX_ExpandHandleMacro (cTmpB,   cTmpA, VAR_LASTKIB,         Vars.cLastKib) ;
    TOOLBOX_ExpandHandleMacro (cTmpA,   cTmpB, VAR_LASTWHISPER,     Vars.cLastWhisper) ;
    TOOLBOX_ExpandHandleMacro (cTmpB,   cTmpA, VAR_LASTSAY,         Vars.cLastKib) ;
    TOOLBOX_ExpandHandleMacro (cTmpA,   cTmpB, VAR_TELNETHANDLE,    Vars.cTelnetHandle) ;
    TOOLBOX_ExpandHandleMacro (cTmpB,   cTmpA, VAR_BOARDHANDLE,     Vars.cBoardHandle) ;
    TOOLBOX_ExpandHandleMacro (cTmpA,   cTmpB, VAR_USERHANDLE1,     Vars.cUserHandle1) ;
    TOOLBOX_ExpandHandleMacro (cResult, cTmpA, VAR_USERHANDLE2,     Vars.cUserHandle2) ;

    if (bChecknR)
        {
        nI = strlen (cResult) ;

        //
        // if "nR" is at the end of the string don't send a line break to ICS
        //
//        if ((cResult [nI - 2] == 'n') && (cResult [nI - 1] == 'R'))
//            {
//            cResult [nI - 2] = NULL_CHAR ;
//            cResult [nI - 1] = NULL_CHAR ;
//            }
//        else
            {
            cResult [nI]     = '\n' ;
            cResult [nI + 1] = NULL_CHAR ;
            }
        }
    strcpy (cD, cResult) ;
    }

void TOOLBOX_SetupFICSMenu (void)
    {
    HMENU hM ;
    int   nI ;

    if (Login.nLoginString > 0)
        {
        hM = GetSubMenu (hMenu [MENU_INIT], 0) ;

        AppendMenu (hM, MF_SEPARATOR, 0, NULL) ;

        for (nI = 0 ; nI < Login.nLoginString ; nI++)
            {
            AppendMenu (hM, MF_STRING, IDM_EXTRA + nI, Login.cLoginString [nI]) ;
            }
        }
    }

void TOOLBOX_AddFICSMenu (char *cS)
    {
    HMENU hM ;
    char cTmp [MAX_LOGIN_SIZE] ;
    int nI, bF = 0 ;

    strcpy (cTmp, TOOLBOX_GetConFilename (cS)) ;
    TOOLBOX_AllTrim (cTmp) ;

    for (nI = 0 ; nI < Login.nLoginString ; nI++)
        {
        if (stricmp (cTmp, TOOLBOX_GetConFilename (Login.cLoginString [nI])) == 0)
            {
            bF = 1 ;
            break ;
            }
        }
    if (bF)
        {
        return ;
        }

    hM = GetSubMenu (hMenu [MENU_INIT], 0) ;

    if (Login.nLoginString == 0)
        {
        AppendMenu (hM, MF_SEPARATOR, 0, NULL) ;
        }

    if (Login.nLoginString >= (MAX_LOGIN - 1))
        {
        for (nI = 0 ; nI < (MAX_LOGIN - 1) ; nI++)
            {
            strcpy (Login.cLoginString [nI], Login.cLoginString [nI + 1]) ;
            }

        strcpy (Login.cLoginString [MAX_LOGIN - 1], cTmp) ;

        for (nI = 0 ; nI < MAX_LOGIN ; nI++)
            {
            ModifyMenu (hM,
                        IDM_EXTRA + nI,
                        MF_BYCOMMAND | MF_STRING | MF_ENABLED,
                        IDM_EXTRA + nI, Login.cLoginString [nI]) ;
            }
        }
    else
        {
        strcpy (Login.cLoginString [Login.nLoginString], cTmp) ;

        AppendMenu (hM,
                    MF_STRING,
                    IDM_EXTRA + Login.nLoginString,
                    Login.cLoginString [Login.nLoginString]) ;

        ++Login.nLoginString ;
        }

    INI_WriteSystem (INI_GetSysFilename ()) ;
    }

void TOOLBOX_ShowMoveButtons (int nG, int bShow)
    {
    if (bShow)
        {
        if (IsWindow (Game [nG].hwnd1))
            {
            InvalidateRect (Game [nG].hwnd1, NULL, TRUE) ;
            InvalidateRect (Game [nG].hwnd2, NULL, TRUE) ;
            InvalidateRect (Game [nG].hwnd3, NULL, TRUE) ;
            InvalidateRect (Game [nG].hwnd4, NULL, TRUE) ;

            ShowWindow (Game [nG].hwnd1, SW_SHOW) ;
            ShowWindow (Game [nG].hwnd2, SW_SHOW) ;
            ShowWindow (Game [nG].hwnd3, SW_SHOW) ;
            ShowWindow (Game [nG].hwnd4, SW_SHOW) ;
            }
        Game [nG].bShowMoveButtons = 1 ;
        }
    else
        {
        if (IsWindow (Game [nG].hwnd1))
            {
            ShowWindow (Game [nG].hwnd1, SW_HIDE) ;
            ShowWindow (Game [nG].hwnd2, SW_HIDE) ;
            ShowWindow (Game [nG].hwnd3, SW_HIDE) ;
            ShowWindow (Game [nG].hwnd4, SW_HIDE) ;
            }
        Game [nG].bShowMoveButtons = 0 ;
        }
    }

void TOOLBOX_ProcessCmdLine (PSTR szCmdLine)
    {
    char cCmd [_MAX_PATH], cTmp [_MAX_PATH] ;
    char Fn [_MAX_PATH], PP [10] [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    int nL, nI, nB, nC, nQ ;

    for (nI = 0 ; nI < 10 ; nI++)
        {
        strcpy (PP [nI], "") ;
        }

    strcpy (cCmd, szCmdLine) ;
    TOOLBOX_AllTrim (cCmd) ;
    nL = strlen (cCmd) ;

    nI = 0 ;
    nB = 0 ;
    nC = 0 ;
    nQ = 0 ;

    while (nI < nL)
        {
        if (nQ == 1)
            {
            if (cCmd [nI] == '"')
                {
                cTmp [nB] = NULL_CHAR ;

                nQ = 0 ;
                nB = 0 ;

                if (strlen (cTmp) > 0)
                    {
                    strcpy (PP [nC], cTmp) ;

                    nC = nC + 1 ;

                    if (nC >= 10)
                        {
                        break ;
                        }
                    }
                }
            else
                {
                cTmp [nB] = cCmd [nI] ;

                nB = nB + 1 ;
                }
            }
        else
            {
            if (cCmd [nI] == '"')
                {
                nQ = 1 ;
                }
            else
            if (cCmd [nI] == ' ')
                {
                cTmp [nB] = NULL_CHAR ;

                nB = 0 ;

                if (strlen (cTmp) > 0)
                    {
                    strcpy (PP [nC], cTmp) ;

                    nC = nC + 1 ;

                    if (nC >= 10)
                        {
                        break ;
                        }
                    }
                }
            else
                {
                cTmp [nB] = cCmd [nI] ;

                nB = nB + 1 ;
                }
            }

        nI = nI + 1 ;
        }

    if (nB > 0)
        {
        cTmp [nB] = NULL_CHAR ;

        if (strlen (cTmp) > 0)
            {
            strcpy (PP [nC], cTmp) ;

            nC = nC + 1 ;
            }
        }

    strcpy (Fn, "") ;

    for (nI = 0 ; nI < 10 ; nI++)
        {
        if (strlen (PP [nI]) > 0)
            {
            _splitpath (PP [nI], Drive, Dir, File, Ext) ;

            if (stricmp (Ext, LOGIN_EXT) == 0)
                {
                strcpy (Fn, PP [nI]) ;
                TOOLBOX_AllTrim (Fn) ;
                break ;
                }
            }
        }

    _splitpath (Fn, Drive, Dir, File, Ext) ;

    if (stricmp (Ext, LOGIN_EXT) != 0)
        {
        return ;
        }

    strcpy (Login.cLoginFile, Fn) ;

    if (LOGIN_Load (Login.cLoginFile,     Login.cLoginName,
                    Login.cLoginAddress,  Login.cLoginPort,
                    Login.cLoginHelper,   &Login.nLoginType,
                    Login.cLoginPrompt,   Login.cLoginHandle,
                    Login.cLoginPassword, Login.cLoginScript))
        {
        LOGIN_Prepare () ;
        LOGIN_Login () ;
        }

    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

void TOOLBOX_GetTelnetHandle (BOOL Ludens)
    {
    CHARRANGE sel ;
    char cTmp [255] ;
	
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
    if (sel.cpMin == sel.cpMax)
        {
        if (User.bRMNoHLSetTelnetHandle)
            {
            TOOLBOX_GetCclientHandle (sel, cTmp, Ludens) ;

            TOOLBOX_AllTrim (cTmp) ;
            strcpy (cTmp, TOOLBOX_GetICSHandle (cTmp, Ludens)) ;
            if (!Ludens)
				{ 
				cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR ;
				}
			else
				{
				// remove trailing . 
				if (strlen(cTmp) > 0) 
					{
					if (cTmp[strlen(cTmp)-1] == '.') 
						{
						cTmp[strlen(cTmp)-1] = NULL_CHAR;
						}
					}
			    if (strlen(cTmp) > 0) 
					{
					if (cTmp[0] == '.') 
						{
						cTmp[0] = ' ';
						TOOLBOX_AllTrim (cTmp) ;
						}
					}
				}

            strcpy (Vars.cTelnetHandle, cTmp) ;
            }
        }
    else
		// highlighted text
        {
        if ((sel.cpMax - sel.cpMin) > MSG_SIZE)
            {
            sel.cpMax = sel.cpMin + 50 ;
            }

        {
        char cSel [CO_MAX + 1 + 10] ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETSELTEXT, 0, (LPARAM) cSel) ;
        if (!Ludens)
			{ // if text highlighted cut text to max handle size only in non-admin version
			cSel [ICS_HANDLE_LENGTH] = NULL_CHAR ;
			}
        strcpy (cTmp, cSel) ;
        }

        TOOLBOX_AllTrim (cTmp) ;
		if (!Ludens) 
			{ // choose handle when highlighted text only in non-admin version
			strcpy (cTmp, TOOLBOX_GetICSHandle (cTmp, FALSE)) ;
			cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR ;
			}

        strcpy (Vars.cTelnetHandle, cTmp) ;
        }
    }

void TOOLBOX_GetCclientHandle (CHARRANGE sel, char *cHandle, BOOL Ludens)
    {
    TEXTRANGE tr ;
    char cBefore [MSG_SIZE + 1], cAfter [MSG_SIZE + 1], cTmp [MSG_SIZE + 1] ;
    int nL, nI ;

    // get before
    if (sel.cpMin == 0)
        {
        strcpy (cBefore, "") ;
        }
    else
        {
        tr.chrg.cpMin = sel.cpMin - 100 ;
        if (tr.chrg.cpMin < 0)
            {
            tr.chrg.cpMin = 0 ;
            }

        tr.chrg.cpMax = sel.cpMin ;
        tr.lpstrText  = cBefore ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETTEXTRANGE, 0, (LPARAM) &tr) ;
        }

    // get After
    tr.chrg.cpMin = sel.cpMin ;
    tr.chrg.cpMax = sel.cpMin + 100 ;
    tr.lpstrText  = cAfter ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETTEXTRANGE, 0, (LPARAM) &tr) ;

    // process
    strcpy (cTmp, "") ;

    if (TOOLBOX_IsAlpha1 (cAfter [0], Ludens))
        {
        nL = strlen (cBefore) ;
        if (nL > 0)
            {
            for (nI = (nL - 1) ; nI >= 0 ; nI--)
                {
                if (! TOOLBOX_IsAlpha1 (cBefore [nI], Ludens))
                    {
                    if (nI != (nL - 1))
                        {
                        strcpy (cTmp, cBefore + nI + 1) ;
                        break ;
                        }
                    }
                }
            }
        strcat (cTmp, cAfter) ;
        }
    else
    if ((strlen (cAfter) > 0) && TOOLBOX_IsAlpha1 (cAfter [1], Ludens))
        {
        strcpy (cTmp, cAfter + 1) ;
        }
    else
    if ((strlen (cAfter) > 0) && TOOLBOX_IsAlpha1 (cAfter [2], Ludens))
        {
        strcpy (cTmp, cAfter + 2) ;
        }
    else
        {
        nL = strlen (cBefore) ;
        if (nL > 0)
            {
            for (nI = (nL - 1) ; nI >= 0 ; nI--)
                {
                if (! TOOLBOX_IsAlpha1 (cBefore [nI], Ludens))
                    {
                    if (nI != (nL - 1))
                        {
                        strcpy (cTmp, cBefore + nI + 1) ;
                        break ;
                        }
                    }
                }
            }
        }

    // final process
    nL = strlen (cTmp) ;
    for (nI = 0 ; nI < nL ; nI++)
        {
        if (! TOOLBOX_IsAlpha1 (cTmp [nI], Ludens))
            {
            cTmp [nI] = NULL_CHAR ;
            break ;
            }
        }

    // copy result
    strcpy (cHandle, cTmp) ;
    }

VOID APIENTRY TOOLBOX_MenuPopup (HWND hwnd, POINT pt, HMENU hmenu, UINT def)
    {
    HMENU hmenuTrackPopup ;     /* floating pop-up menu  */

    //
    // get the first pop-up menu in the menu template. This is the
    // menu that TrackPopupMenu displays.
    //
    hmenuTrackPopup = GetSubMenu (hmenu, 0) ;

    SetMenuDefaultItem (hmenuTrackPopup, def, FALSE) ;

    //
    // TrackPopupMenu () uses screen coordinates, so convert the
    // coordinates of the mouse click to screen coordinates
    //
    ClientToScreen (hwnd, (LPPOINT) &pt) ;
		
    // draw and track the floating pop-up menu
    TrackPopupMenu (hmenuTrackPopup, TPM_CENTERALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL) ;
	
    // destroy the menu
    DestroyMenu (hmenu) ;
    }

void TOOLBOX_GetPath (char *F, char *P)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    _splitpath (F, Drive, Dir, File, Ext) ;
    _makepath (P, Drive, Dir, "", "") ;
    }

void TOOLBOX_WriteInfoOnOff (int bVal, char *cS)
    {
    char cTmp [512] ;

    sprintf (cTmp, "%s %s\n", cS, (bVal ? "On" : "Off")) ;
    TOOLBOX_WriteSystem (cTmp) ;
    }

int TOOLBOX_Command (int nT, char *cCmd, int bAdd)
    {
    int nS, nFrom, nTo, nI ;
    char cTmp [2048], cTmp1 [2048] ;
    char cVar [12] [2048], cVal [12] [2048] ;
    CHARRANGE TSel ;
    HDC hdc ;

    // none
    if (nT == FUNCTION_NONE)
        {
        return 0 ;
        }

    // command
    if (nT == FUNCTION_COMMAND)
        {
        if (strlen (cCmd) == 0)
            {
            return 0 ;
            }

        TOOLBOX_ExpandAllMacro (cTmp, cCmd, 1) ;

        if (Timeseal.bSocketIsOpen)
            {
            TOOLBOX_WriteICS (cTmp) ;
            }

        TOOLBOX_WriteUser (cTmp) ;

        nS = strlen (cTmp) ;
        if (cTmp [nS - 1] == '\n')
            {
            cTmp [nS - 1] = NULL_CHAR ;
            }

        if (bAdd)
            {
            HISTORY_Add (cTmp) ;
            }

        TOOLBOX_CheckObserveExam (cTmp) ;

        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
        return 1 ;
        }

    // sit for piece drop move
    if (nT == FUNCTION_SITPIECE_DROP_MOVE)
        {
        F8KEY_Set () ;
        return 1 ;
        }

    // telnet last tell
    if (nT == FUNCTION_TELNET_LAST_TELL)
        {
        F9KEY_Set () ;
        return 1 ;
        }

    // tell handle command
    if (nT == FUNCTION_TELNET_TELL_HANDLE)
        {

		TOOLBOX_GetTelnetHandle (FALSE) ;
/*		if ((strlen(Vars.cTelnetHandle) == 0)) 
			{
			return 0;
			} */
	
		TSel.cpMin = 999999 ;
        TSel.cpMax = 999999 ;

		sprintf(cTmp, "TELL %s ",Vars.cTelnetHandle);
	    SetWindowText (hwndWindow [HWND_TELNET_EDIT], cTmp) ;
        SendMessage   (hwndWindow [HWND_TELNET_EDIT], EM_EXSETSEL, 0, (LPARAM) &TSel) ;
        SetFocus      (hwndWindow [HWND_TELNET_EDIT]) ;
		return 1;
		}
	
	// telnet edit command
    if (nT == FUNCTION_TELNET_EDIT_CMD)
        {
        if (strlen (cCmd) == 0)
            {
            return 0 ;
            }

        TOOLBOX_ExpandAllMacro (cTmp, cCmd, 1) ;

        nS = strlen (cTmp) ;
        if (cTmp [nS - 1] == '\n')
            {
            cTmp [nS - 1] = NULL_CHAR ;
            nS = strlen (cTmp) ;
            }

        if (cTmp [nS - 1] != ' ')
            {
            cTmp [nS    ] = ' ' ;
            cTmp [nS + 1] = NULL_CHAR ;
            }

        TSel.cpMin = 999999 ;
        TSel.cpMax = 999999 ;
    
        SetWindowText (hwndWindow [HWND_TELNET_EDIT], cTmp) ;
        SendMessage   (hwndWindow [HWND_TELNET_EDIT], EM_EXSETSEL, 0, (LPARAM) &TSel) ;
        SetFocus      (hwndWindow [HWND_TELNET_EDIT]) ;
        return 1 ;
        }

    // copy variable
    if (nT == FUNCTION_COPY_VARIABLE)
        {
        sscanf (cCmd, "%s %s", &cTmp, &cTmp1) ;

        TOOLBOX_AllTrim (cTmp) ;
        TOOLBOX_AllTrim (cTmp1) ;

        if ((strlen (cTmp) == 0) || (strlen (cTmp1) == 0))
            {
            return 0 ;
            }

        strcpy (cVar [0],  VAR_WHOAMI) ;
        strcpy (cVar [1],  VAR_PARTNER) ;
        strcpy (cVar [2],  VAR_OPPONENT) ;
        strcpy (cVar [3],  VAR_PARTNEROPPONENT) ;
        strcpy (cVar [4],  VAR_LASTTELL) ;
        strcpy (cVar [5],  VAR_LASTKIB) ;
        strcpy (cVar [6],  VAR_LASTSAY) ;
        strcpy (cVar [7],  VAR_LASTWHISPER) ;
        strcpy (cVar [8],  VAR_TELNETHANDLE) ;
        strcpy (cVar [9],  VAR_BOARDHANDLE) ;
        strcpy (cVar [10], VAR_USERHANDLE1) ;
        strcpy (cVar [11], VAR_USERHANDLE2) ;

        nFrom = -1 ;
        nTo   = -1 ;

        for (nI = 0 ; nI < 12 ; nI++)
            {
            if (stricmp (cTmp, cVar [nI]) == 0)
                {
                nFrom = nI ;
                }
            if (stricmp (cTmp1, cVar [nI]) == 0)
                {
                nTo = nI ;
                }
            }
        
        if ((nFrom == -1) || (nTo == -1) || (nFrom == nTo))
            {
            return 0 ;
            }

        strcpy (cVal [0],  Vars.cWhoAmI) ;
        strcpy (cVal [1],  Vars.cPartner) ;
        strcpy (cVal [2],  Vars.cOpponent) ;
        strcpy (cVal [3],  Vars.cPartnerOpponent) ;
        strcpy (cVal [4],  Vars.cLastTell) ;
        strcpy (cVal [5],  Vars.cLastKib) ;
        strcpy (cVal [6],  Vars.cLastSay) ;
        strcpy (cVal [7],  Vars.cLastWhisper) ;
        strcpy (cVal [8],  Vars.cTelnetHandle) ;
        strcpy (cVal [9],  Vars.cBoardHandle) ;
        strcpy (cVal [10], Vars.cUserHandle1) ;
        strcpy (cVal [11], Vars.cUserHandle2) ;

        strcpy (cVal [nTo], cVal [nFrom]) ;

        strcpy (Vars.cWhoAmI,          cVal [0] ) ;
        strcpy (Vars.cPartner,         cVal [1] ) ;
        strcpy (Vars.cOpponent,        cVal [2] ) ;
        strcpy (Vars.cPartnerOpponent, cVal [3] ) ;
        strcpy (Vars.cLastTell,        cVal [4] ) ;
        strcpy (Vars.cLastKib,         cVal [5] ) ;
        strcpy (Vars.cLastSay,         cVal [6] ) ;
        strcpy (Vars.cLastWhisper,     cVal [7] ) ;
        strcpy (Vars.cTelnetHandle,    cVal [8] ) ;
        strcpy (Vars.cBoardHandle,     cVal [9] ) ;
        strcpy (Vars.cUserHandle1,     cVal [10]) ;
        strcpy (Vars.cUserHandle2,     cVal [11]) ;
        return 1 ;
        }

    // minimize thief
    if (nT == FUNCTION_MINIMIZE_THIEF)
        {
        ShowWindow (hwndWindow [HWND_FRAME], SW_SHOWMINIMIZED) ;
        return 1 ;
        }

    // internet browser
    if (nT == FUNCTION_INTERNET_BROWSER)
        {
        ShellExecute (NULL, "open", cCmd, NULL, "", SW_SHOW) ;
        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
        return 1 ;
        }

    // run script
    if (nT == FUNCTION_RUN_SCRIPT)
        {
        if (Timeseal.bSocketIsOpen)
            {
            if (strlen (cCmd) == 0)
                {
                PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_RUN_SCRIPT, (LPARAM) 0) ;
                return 1 ;
                }
            else
                {
                if (TOOLBOX_ReadScriptFile (TOOLBOX_GetFullScriptFilename (cCmd)))
                    {
                    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                    return 1 ;
                    }
                }
            }
        TOOLBOX_Beep () ;
        return 0 ;
        }

    // load profile
    if (nT == FUNCTION_LOAD_PROFILE)
        {
        if (strlen (cCmd) == 0)
            {
            PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_LOAD_PROFILE, (LPARAM) 0) ;
            return 1 ;
            }
        else
            {
            if (TOOLBOX_LoadProfile (TOOLBOX_GetFullProfileFilename (cCmd)))
                {
                return 1 ;
                }
            }
        TOOLBOX_Beep () ;
        return 0 ;
        }

    // toggle thief/lightning rulez true premove style
    if (nT == FUNCTION_TOGGLE_TP)
        {
        switch (User.nTruePremove)
            {
            case PREMOVE_NONE :
                PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_TRUE_PREMOVE_THIEF, (LPARAM) 0) ;
                break ;

            case PREMOVE_THIEF :
                PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_TRUE_PREMOVE_LR, (LPARAM) 0) ;
                break ;

            case PREMOVE_LR :
                PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_TRUE_PREMOVE_THIEF, (LPARAM) 0) ;
                break ;
            }
        return 1 ;
        }
        
    // pawn drop move
    if (nT == FUNCTION_DROP_PAWN)
        {
        if ((Game [LeftMouseDrop.nI].nGameType != GAMETYPE_BUGHOUSE  ) &&
            (Game [LeftMouseDrop.nI].nGameType != GAMETYPE_CRAZYHOUSE))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nI < 0) || (LeftMouseDrop.nI >= MAX_GAME))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nX < 0) || (LeftMouseDrop.nX > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nY <= 0) || (LeftMouseDrop.nY >= 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if (LeftMouseDrop.nI == INDEX_PLAY)
            {
            if (Game [LeftMouseDrop.nI].nGameNumber > 0)
                {
                ReleaseCapture() ;

                if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                    {
                    Game [LeftMouseDrop.nI].nCurrentIndex = Game [LeftMouseDrop.nI].nMaxIndex ;
                    Game [LeftMouseDrop.nI].nCurrentColor = Game [LeftMouseDrop.nI].nMaxColor ;

                    hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                    BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                    ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                    }

                if (Game [LeftMouseDrop.nI].bPlaying)
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bIPlayWhite ? WHITE_PAWN : BLACK_PAWN))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                else
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bWhitesMove ? WHITE_PAWN : BLACK_PAWN))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                }
            else
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
        if ((Game [INDEX_PLAY].nGameNumber > 0)       &&
            (Game [INDEX_PLAY].bPlaying)              &&
            (Game [LeftMouseDrop.nI].nGameNumber > 0) &&
            (Game [LeftMouseDrop.nI].bPlaying)        &&
            (Game [INDEX_PLAY].nGamePartner == Game [LeftMouseDrop.nI].nGameNumber))
            {
            ReleaseCapture() ;

            if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                {
                hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                }

            if (! BOARD_RightMouseDropPiece1 (LeftMouseDrop.nI, LeftMouseDrop.nX, LeftMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_PAWN : BLACK_PAWN))
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        // reset left mouse drop
        LeftMouseDrop.nI = -1 ;
        LeftMouseDrop.nX = -1 ;
        LeftMouseDrop.nY = -1 ;
        return 1 ;
        }

    // knight drop move
    if (nT == FUNCTION_DROP_KNIGHT)
        {
        if ((Game [LeftMouseDrop.nI].nGameType != GAMETYPE_BUGHOUSE  ) &&
             (Game [LeftMouseDrop.nI].nGameType != GAMETYPE_CRAZYHOUSE))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nI < 0) || (LeftMouseDrop.nI >= MAX_GAME))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nX < 0) || (LeftMouseDrop.nX > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nY < 0) || (LeftMouseDrop.nY > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if (LeftMouseDrop.nI == INDEX_PLAY)
            {
            if (Game [LeftMouseDrop.nI].nGameNumber > 0)
                {
                ReleaseCapture() ;

                if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                    {
                    hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                    BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                    ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                    }

                if (Game [LeftMouseDrop.nI].bPlaying)
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bIPlayWhite ? WHITE_KNIGHT : BLACK_KNIGHT))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                else
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bWhitesMove ? WHITE_KNIGHT : BLACK_KNIGHT))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                }
            else
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
        if ((Game [INDEX_PLAY].nGameNumber > 0)       &&
            (Game [INDEX_PLAY].bPlaying)              &&
            (Game [LeftMouseDrop.nI].nGameNumber > 0) &&
            (Game [LeftMouseDrop.nI].bPlaying)        &&
            (Game [INDEX_PLAY].nGamePartner == Game [LeftMouseDrop.nI].nGameNumber))
            {
            ReleaseCapture() ;

            if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                {
                hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                }

            if (! BOARD_RightMouseDropPiece1 (LeftMouseDrop.nI, LeftMouseDrop.nX, LeftMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_KNIGHT : BLACK_KNIGHT))
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        // reset left mouse drop
        LeftMouseDrop.nI = -1 ;
        LeftMouseDrop.nX = -1 ;
        LeftMouseDrop.nY = -1 ;
        return 1 ;
        }

    // bishop drop move
    if (nT == FUNCTION_DROP_BISHOP)
        {
        if ((Game [LeftMouseDrop.nI].nGameType != GAMETYPE_BUGHOUSE  ) &&
            (Game [LeftMouseDrop.nI].nGameType != GAMETYPE_CRAZYHOUSE))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nI < 0) || (LeftMouseDrop.nI >= MAX_GAME))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nX < 0) || (LeftMouseDrop.nX > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nY < 0) || (LeftMouseDrop.nY > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if (LeftMouseDrop.nI == INDEX_PLAY)
            {
            if (Game [LeftMouseDrop.nI].nGameNumber > 0)
                {
                ReleaseCapture() ;

                if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                    {
                    hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                    BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                    ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                    }

                if (Game [LeftMouseDrop.nI].bPlaying)
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bIPlayWhite ? WHITE_BISHOP : BLACK_BISHOP))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                else
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bWhitesMove ? WHITE_BISHOP : BLACK_BISHOP))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                }
            else
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
        if ((Game [INDEX_PLAY].nGameNumber > 0)       &&
            (Game [INDEX_PLAY].bPlaying)              &&
            (Game [LeftMouseDrop.nI].nGameNumber > 0) &&
            (Game [LeftMouseDrop.nI].bPlaying)        &&
            (Game [INDEX_PLAY].nGamePartner == Game [LeftMouseDrop.nI].nGameNumber))
            {
            ReleaseCapture() ;

            if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                {
                hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                }

            if (! BOARD_RightMouseDropPiece1 (LeftMouseDrop.nI, LeftMouseDrop.nX, LeftMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_BISHOP : BLACK_BISHOP))
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        // reset left mouse drop
        LeftMouseDrop.nI = -1 ;
        LeftMouseDrop.nX = -1 ;
        LeftMouseDrop.nY = -1 ;
        return 1 ;
        }

    // rook drop move
    if (nT == FUNCTION_DROP_ROOK)
        {
        if ((Game [LeftMouseDrop.nI].nGameType != GAMETYPE_BUGHOUSE  ) &&
            (Game [LeftMouseDrop.nI].nGameType != GAMETYPE_CRAZYHOUSE))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nI < 0) || (LeftMouseDrop.nI >= MAX_GAME))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nX < 0) || (LeftMouseDrop.nX > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nY < 0) || (LeftMouseDrop.nY > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if (LeftMouseDrop.nI == INDEX_PLAY)
            {
            if (Game [LeftMouseDrop.nI].nGameNumber > 0)
                {
                ReleaseCapture() ;

                if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                    {
                    hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                    BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                    ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                    }

                if (Game [LeftMouseDrop.nI].bPlaying)
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bIPlayWhite ? WHITE_ROOK : BLACK_ROOK))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                else
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bWhitesMove ? WHITE_ROOK : BLACK_ROOK))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                }
            else
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
        if ((Game [INDEX_PLAY].nGameNumber > 0)       &&
            (Game [INDEX_PLAY].bPlaying)              &&
            (Game [LeftMouseDrop.nI].nGameNumber > 0) &&
            (Game [LeftMouseDrop.nI].bPlaying)        &&
            (Game [INDEX_PLAY].nGamePartner == Game [LeftMouseDrop.nI].nGameNumber))
            {
            ReleaseCapture() ;

            if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                {
                hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                }

            if (! BOARD_RightMouseDropPiece1 (LeftMouseDrop.nI, LeftMouseDrop.nX, LeftMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_ROOK : BLACK_ROOK))
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        // reset left mouse drop
        LeftMouseDrop.nI = -1 ;
        LeftMouseDrop.nX = -1 ;
        LeftMouseDrop.nY = -1 ;
        return 1 ;
        }

    // queen drop move
    if (nT == FUNCTION_DROP_QUEEN)
        {
        if ((Game [LeftMouseDrop.nI].nGameType != GAMETYPE_BUGHOUSE  ) &&
            (Game [LeftMouseDrop.nI].nGameType != GAMETYPE_CRAZYHOUSE))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nI < 0) || (LeftMouseDrop.nI >= MAX_GAME))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nX < 0) || (LeftMouseDrop.nX > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if ((LeftMouseDrop.nY < 0) || (LeftMouseDrop.nY > 7))
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        if (LeftMouseDrop.nI == INDEX_PLAY)
            {
            if (Game [LeftMouseDrop.nI].nGameNumber > 0)
                {
                ReleaseCapture() ;

                if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                    {
                    hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                    BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                    ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                    }

                if (Game [LeftMouseDrop.nI].bPlaying)
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bIPlayWhite ? WHITE_QUEEN : BLACK_QUEEN))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                else
                    {
                    if (! BOARD_RightMouseDropPiece (LeftMouseDrop.nX, LeftMouseDrop.nY, Game [LeftMouseDrop.nI].bWhitesMove ? WHITE_QUEEN : BLACK_QUEEN))
                        {
                        TOOLBOX_Beep () ;
                        return 0 ;
                        }
                    }
                }
            else
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
        if ((Game [INDEX_PLAY].nGameNumber > 0)       &&
            (Game [INDEX_PLAY].bPlaying)              &&
            (Game [LeftMouseDrop.nI].nGameNumber > 0) &&
            (Game [LeftMouseDrop.nI].bPlaying)        &&
            (Game [INDEX_PLAY].nGamePartner == Game [LeftMouseDrop.nI].nGameNumber))
            {
            ReleaseCapture() ;

            if ((DragInfo.nPc != EMPTY_SQUARE) || (! TOOLBOX_DisplayActualBoard (LeftMouseDrop.nI)))
                {
                hdc = GetDC (Game [LeftMouseDrop.nI].hwnd) ;
                BOARD_RestoreDragMove (LeftMouseDrop.nI, hdc) ;
                ReleaseDC (Game [LeftMouseDrop.nI].hwnd, hdc) ;
                }

            if (! BOARD_RightMouseDropPiece1 (LeftMouseDrop.nI, LeftMouseDrop.nX, LeftMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_QUEEN : BLACK_QUEEN))
                {
                TOOLBOX_Beep () ;
                return 0 ;
                }
            }
        else
            {
            TOOLBOX_Beep () ;
            return 0 ;
            }

        // reset left mouse drop
        LeftMouseDrop.nI = -1 ;
        LeftMouseDrop.nX = -1 ;
        LeftMouseDrop.nY = -1 ;
        return 1 ;
        }

    // menu commands
    if ((nT >= 0) && (nT < MAX_FMENU))
        {
        if ((FMenu [nT] == IDM_NONE) || (FMenu [nT] == IDM_TITLE))
            {
            return 0 ;
            }
        else
            {
            PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, FMenu [nT], (LPARAM) 0) ;
            return 1 ;
            }
        }
    else
        {
        return 0 ;
        }
    }

char * TOOLBOX_GetICSHandle (char *cS, BOOL Ludens)
    {
    int nS, nI ;
    static char cBuffer [2048] ;

    nS = strlen (cS) ;
    strcpy (cBuffer, "") ;
    for (nI = 0 ; nI <= nS ; nI++)
        {
        if (cS [nI] == '('  ||
            cS [nI] == ' '  ||
            cS [nI] == ':'  ||
            cS [nI] == '\t' ||
            cS [nI] == '\r' ||
            cS [nI] == '\n' ||
            cS [nI] == ']'  ||
            cS [nI] == '>')
            {
            cBuffer [nI] = NULL_CHAR ;
            break ;
            }
        else
            {
            if (Login.nLoginType == SERVER_ICC)
                {
                if ((! isalnum (cS [nI])) && (cS [nI] != '-'))
                    {
                    cBuffer [nI] = NULL_CHAR ;
                    break ;
                    }
                }
            else
				{ 
// new in 1.22: allow rightclick on numbers for responding to seeks or offers)
// 1.22 line:   if ((! isalnum (cS [nI])) &&  !(Ludens && (isdigit(cS[nI]) || cS[nI] == '.')))
				if ((! isalpha (cS [nI])) &&  !(Ludens && (isalnum(cS[nI]) || cS[nI] == '.' || cS[nI] == '@')))
                    {
                    cBuffer [nI] = NULL_CHAR ;
                    break ;
                    }
                }

            if ((nI >= 20) &&  !(Ludens))
                {
                cBuffer [nI] = NULL_CHAR ;
                break ;
                }
            else
                {
                cBuffer [nI] = cS [nI] ;
                }
            }
        }
    return cBuffer ;
    }

char * TOOLBOX_GetICSHandleFromMessage (char *cS)
    {
    int nS, nI ;
    static char cBuffer [2048] ;

	nS = strlen (cS) ;
    strcpy (cBuffer, "") ;
	//Thieftest at Fri Feb 11, 15:04 PST 2011: bla
	for (nI = 0 ; nI <= nS ; nI++)
		{
		if (!isalpha(cS [nI]))
			{
			cBuffer [nI] = NULL_CHAR ;
			break ;
			}
		else
			{
			cBuffer [nI] = cS [nI] ;
			}
		}
    return cBuffer ;
	}

char * TOOLBOX_GetICSHandleFromMessageDeleted (char *cS)
    {
    int nS, nI ;
    static char cBuffer [2048] ; 

	nS = strlen (cS) ;
    strcpy (cBuffer, "") ;
	//Messages from Thieftest cleared.
	for (nI = 14 ; nI <= nS ; nI++)
		{
		if (!isalpha(cS [nI]))
			{
			cBuffer [nI-14] = NULL_CHAR ;
			break ;
			}
		else
			{
			cBuffer [nI-14] = cS [nI] ;
			}
		}
    return cBuffer ;
	}

char * TOOLBOX_GetICSHandleFromNoMessage (char *cS)
    {
    int nS, nI ;
    static char cBuffer [2048] ; 

	nS = strlen (cS) ;
    strcpy (cBuffer, "") ;
	//You have no messages from Thieftest.
	for (nI = 26 ; nI <= nS ; nI++)
		{
		if (!isalpha(cS [nI]))
			{
			cBuffer [nI-26] = NULL_CHAR ;
			break ;
			}
		else
			{
			cBuffer [nI-26] = cS [nI] ;
			}
		}
    return cBuffer ;
	}

int TOOLBOX_IsComputerHandle (char *cS)
    {
    int bR, nS, nI ;

    bR = 0 ;
    nS = strlen (cS) ;
    for (nI = 0 ; nI < nS ; nI++)
        {
        if (cS [nI    ] == '(' &&
            cS [nI + 1] == 'C' &&
            cS [nI + 2] == ')')
            {
            bR = 1 ;
            break ;
            }
        else
        if (cS [nI] == ' ')
            {
            break ;
            }
        }
    return bR ;
    }

int TOOLBOX_IsSeparator (int nI, int nJ)
    {
    return (RightMouse [nI].cMenu [nJ] [0] == '-') ;
    }

int TOOLBOX_OKRightMouse (int nI, int nJ)
    {
    CHARRANGE sel ;

    switch (RightMouse [nI].nType [nJ])
        {
        case FUNCTION_SITPIECE_DROP_MOVE :
            if (Game [INDEX_PLAY].nGameType == GAMETYPE_BUGHOUSE)
                {
                if (Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
                    {
                    return 1 ;
                    }
                }
            return 0 ;

        case FUNCTION_RESET_BOARD :
            if (IsWindow (System.hwndLastGame))
                {
                if (Game [INDEX_PLAY].hwnd == System.hwndLastGame)
                    {
                    if (Game [INDEX_PLAY].nGameNumber > 0)
                        {
                        return 0 ;
                        }
                    else
                        {
                        return 1 ;
                        }
                    }
                else
                    {
                    for (nI = 1 ; nI < MAX_GAME ; nI++)
                        {
                        if (Game [nI].hwnd == System.hwndLastGame)
                            {
                            if (Game [nI].bValid && Game [nI].nGameNumber > 0)
                                {
                                return 0 ;
                                }
                            else
                                {
                                return 1 ;
                                }
                            }
                        }
                    }
                }
            return 0 ;

        case FUNCTION_FLIP_BOARD :
            if (IsWindow (System.hwndLastGame))
                {
                if (Game [INDEX_PLAY].hwnd == System.hwndLastGame)
                    {
                    if (Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
//                    if (Game [INDEX_PLAY].nGameNumber > 0)
                        {
                        return 0 ;
                        }
                    else
                        {
                        return 1 ;
                        }
                    }
                else
                    {
                    for (nI = 1 ; nI < MAX_GAME ; nI++)
                        {
                        if (Game [nI].hwnd == System.hwndLastGame)
                            {
                            return 1 ;
                            }
                        }
                    }
                }
            return 0 ;

        case FUNCTION_CLOSE_BOARD :
            if (IsWindow (System.hwndLastGame))
                {
                if (Game [INDEX_PLAY].hwnd == System.hwndLastGame)
                    {
                    if (Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
                        {
                        return 0 ;
                        }
                    else
                        {
                        return 1 ;
                        }
                    }
                else
                    {
                    for (nI = 1 ; nI < MAX_GAME ; nI++)
                        {
                        if (Game [nI].hwnd == System.hwndLastGame)
                            {
                            return 1 ;
                            }
                        }
                    }
                }
            return 0 ;

        case FUNCTION_UNEXAM_GAME :
            if (IsWindow (System.hwndLastGame))
                {
                if (Game [INDEX_PLAY].hwnd == System.hwndLastGame)
                    {
                    if (Game [INDEX_PLAY].nGameNumber > 0)
                        {
                        if (Game [INDEX_PLAY].bPlaying)
                            {
                            return 0 ;
                            }
                        else
                            {
                            return 1 ;
                            }
                        }
                    }
                }
            return 0 ;

        case FUNCTION_TELNET_TEXT_COPY :
            SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
            if (sel.cpMin == sel.cpMax)
                {
                return 0 ;
                }
            else
                {
                return 1 ;
                }
            return 0 ;

        case FUNCTION_TELNET_TEXT_CP :
            SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
            if (sel.cpMin == sel.cpMax)
                {
                return 0 ;
                }
            else
                {
                return 1 ;
                }
            return 0 ;

        case FUNCTION_TELNET_TEXT_SF :
            SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
            if (sel.cpMin == sel.cpMax)
                {
                return 0 ;
                }
            else
                {
                return 1 ;
                }
            return 0 ;

        case FUNCTION_TELNET_EDIT_COPY :
            SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
            if (sel.cpMin == sel.cpMax)
                {
                return 0 ;
                }
            else
                {
                return 1 ;
                }
            return 0 ;

        case FUNCTION_TELNET_EDIT_CUT :
            SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
            if (sel.cpMin == sel.cpMax)
                {
                return 0 ;
                }
            else
                {
                return 1 ;
                }
            return 0 ;

        case FUNCTION_TELNET_EDIT_PASTE :
            if (IsClipboardFormatAvailable (CF_TEXT))
                {
                return 1 ;
                }
            else
                {
                return 0 ;
                }
            return 0 ;

        case FUNCTION_COPY_VARIABLE :
            return 1 ;

        case FUNCTION_MATCH_BOX :
            if (Timeseal.bSocketIsOpen)
                {
                if (Game [INDEX_PLAY].nGameNumber > 0)
                    {
                    if (Game [INDEX_PLAY].bPlaying)
                        {
                        return 0 ;
                        }
                    else
                    if (Login.nLoginType == SERVER_ICC)
                        {
                        return 1 ;
                        }
                    else
                        {
                        return 0 ;
                        }
                    }
                else
                    {
                    return 1 ;
                    }
                }
            else
                {
                return 0 ;
                }
            return 0 ;

        case FUNCTION_SEEK_BOX :
            if (Timeseal.bSocketIsOpen)
                {
                if (Game [INDEX_PLAY].nGameNumber > 0)
                    {
                    if (Game [INDEX_PLAY].bPlaying)
                        {
                        return 0 ;
                        }
                    else
                    if (Login.nLoginType == SERVER_ICC)
                        {
                        return 1 ;
                        }
                    else
                        {
                        return 0 ;
                        }
                    }
                else
                    {
                    return 1 ;
                    }
                }
            else
                {
                return 0 ;
                }
            return 0 ;

        case FUNCTION_MESSAGE_BOX :
            if (Timeseal.bSocketIsOpen)
                {
                if (User.bIamAGuest)
                    {
                    return 0 ;
                    }
                return 1 ;
                }
            return 0 ;

        case FUNCTION_QUESTION_BOX :
            if (Timeseal.bSocketIsOpen)
                {
                return 1 ;
                }
            return 0 ;

        case FUNCTION_DROP_PAWN :
        case FUNCTION_DROP_KNIGHT :
        case FUNCTION_DROP_BISHOP :
        case FUNCTION_DROP_ROOK :
        case FUNCTION_DROP_QUEEN :
            return 0 ;

        case FUNCTION_RUN_SCRIPT :
            if (Timeseal.bSocketIsOpen)
                {
                return 1 ;
                }
            return 0 ;

        case FUNCTION_LOAD_PROFILE :
            return 1 ;
        }
    return 1 ;
    }

int TOOLBOX_SaveTelnetMarkedText (void)
    {
    CHARRANGE sel ;
    FILE *Fv ;
    char cFn [_MAX_PATH], *cP, cDate [20], cTime [20] ;

    Fv = TOOLBOX_OpenFileDialog (hwndWindow [HWND_TELNET_TEXT], TRUE, NULL, "log", LOG_FILT, "Save log file", NULL, NULL, cFn, Browse.cLogBrowse) ;
    if (Fv == NULL)
        {
        return 0 ;
        }
    fclose (Fv) ;
    TOOLBOX_GetPath (cFn, Browse.cLogBrowse) ;
    INI_WriteSystem (INI_GetSysFilename ()) ;
    
    if (strlen (cFn) == 0)
        {
        return 0 ;
        }

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
    if (sel.cpMin == sel.cpMax)
        {
        return 0 ;
        }

    Fv = fopen (cFn, "a") ;
    if (Fv == NULL)
        {
        return 0 ;
        }

    {
    char cSel [CO_MAX + 1 + 10] ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETSELTEXT, 0, (LPARAM) cSel) ;

    for (cP = cSel ; *cP != NULL_CHAR ; cP++)
        {
        if ((*cP == '\r') && (*(cP + 1) == '\n'))
            {
            *cP = ' ' ;
            }
        }

    _strdate (cDate) ;  // MM/DD/YY
    _strtime (cTime) ;  // HH:MM:SS

    fprintf (Fv, "\n--[%s  %s]--\n", cDate, cTime) ;
    fprintf (Fv, "%s\n", cSel) ;
    }

    fclose (Fv) ;
    return 1 ;
    }

int TOOLBOX_TelnetTextDoubleClickExecute (void)
    {
    CHARRANGE sel ;
    TEXTRANGE tr ;
    char cBefore [MSG_SIZE + 10], cAfter [MSG_SIZE + 10], 
		 cBeforeTemp [MSG_SIZE + 10], cAfterTemp [MSG_SIZE + 10], cLine [MSG_SIZE + 10] ;
    char cTmp [MSG_SIZE + 10], cTmpB [MSG_SIZE + 10], cTmpA [MSG_SIZE + 10] ;
    int nB, nA, nL, nI, nJ, nS, bE ;
    char *cP, *cQ ;

    // get current selection
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;

    // get before
    if (sel.cpMin == 0)
        {
        strcpy (cBefore, "") ;
        }
    else
        {
        tr.chrg.cpMin = sel.cpMin - 100 ;
        if (tr.chrg.cpMin < 0)
            {
            tr.chrg.cpMin = 0 ;
            }

        tr.chrg.cpMax = sel.cpMin ;
        tr.lpstrText  = cBefore ;

        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETTEXTRANGE, 0, (LPARAM) &tr) ;
        }

    // get after
    tr.chrg.cpMin = sel.cpMin ;
    tr.chrg.cpMax = sel.cpMin + 100 ;
    tr.lpstrText  = cAfter ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETTEXTRANGE, 0, (LPARAM) &tr) ;
	strcpy (cBeforeTemp,cBefore);
	strcpy (cAfterTemp,cAfter);

    //
    // check for "www." and "http://"
    //
    nB = strlen (cBefore) ;
    nA = strlen (cAfter) ;

    // initialize actual line
    strcpy (cLine, "") ;

    // get the actual line (before)
    if (nB > 0)
        {
        nS = -1 ;
        nI = nB - 1 ;
        while (nI >= 0)
            {
            if (cBefore [nI] == ' ')
                {
                if (nI >= 6)
                    {
                    if ((cBefore [nI - 1] == ' ' ) &&
                        (cBefore [nI - 2] == ' ' ) &&
                        (cBefore [nI - 3] == '\\') &&
                        (cBefore [nI - 4] == '\n') &&
                        (cBefore [nI - 5] == '\r'))
                        {
                        nI = nI - 6 ;
                        continue ;
                        }
                    }

                if (Login.nLoginType == SERVER_ICC)
                    {
                    if (nI >= 4)
                        {
                        if ((cBefore [nI - 1] == ' ' ) &&
                            (cBefore [nI - 2] == '\n') &&
                            (cBefore [nI - 3] == '\r'))
                            {
                            nI = nI - 4 ;
                            continue ;
                            }
                        }
                    }
                nS = nI + 1 ;
                break ;
                }

            if ((cBefore [nI] == '(') || (cBefore [nI] == ')') ||
                (cBefore [nI] == '"') || (cBefore [nI] == '[') ||
                (cBefore [nI] == ']'))
                {
                nS = nI + 1 ;
                break ;
                }

            nI = nI - 1 ;
            }

        if (nS == -1)
            {
            strcpy (cLine, cBefore) ;
            }
        else
        if (nS > nB)
            {
            strcpy (cLine, "") ;
            }
        else
            {
            nJ = 0 ;
            for (nI = nS ; nI < nB ; nI++)
                {
                cLine [nJ++] = cBefore [nI] ;
                }
            cLine [nJ] = NULL_CHAR ;
            }
        }

    // get the actual line (after)
    if (nA > 0)
        {
        nS = -1 ;
        nI = 0 ;
        while (nI < nA)
            {
            if (cAfter [nI] == '\r')
                {
                if ((nI + 6) < nA)
                    {
                    if ((cAfter [nI + 1] == '\n') &&
                        (cAfter [nI + 2] == '\\') &&
                        (cAfter [nI + 3] == ' ' ) &&
                        (cAfter [nI + 4] == ' ' ) &&
                        (cAfter [nI + 5] == ' ' ))
                        {
                        nI = nI + 6 ;
                        continue ;
                        }
                    }

                if (Login.nLoginType == SERVER_ICC)
                    {
                    if (nI >= 4)
                        {
                        if ((cAfter [nI + 1] == '\n') &&
                            (cAfter [nI + 2] == ' ' ) &&
                            (cAfter [nI + 3] == ' ' ))
                            {
                            nI = nI + 4 ;
                            continue ;
                            }
                        }
                    }
                nS = nI - 1 ;
                break ;
                }

            if ((cAfter [nI] == '(') || (cAfter [nI] == ')') ||
                (cAfter [nI] == ' ') || (cAfter [nI] == '"') ||
                (cAfter [nI] == '[') || (cAfter [nI] == ']'))
                {
                nS = nI - 1 ;
                break ;
                }

            nI = nI + 1 ;
            }

        if (nS == -1)
            {
            strcat (cLine, cAfter) ;
            }
        else
        if (nS > nA)
            {
            }
        else
            {
            cAfter [nS + 1] = NULL_CHAR ;
            strcat (cLine, cAfter) ;
            }
        }

    strcpy (cTmp, cLine) ;

    // remove "\r\n\\   "
    while (strstr (cTmp, "\r\n\\   "))
        {
        cP = cTmp ;
        cQ = cTmpB ;
        nI = 0 ;
        while (strncmp (cP, "\r\n\\   ", 6))
            {
            *cQ++ = *cP++ ;
            nI = nI + 1 ;
            }
        *cQ = NULL_CHAR ;

        strcpy (cTmpA, (cP + 6)) ;

        strcpy (cTmp, cTmpB) ;
        strcat (cTmp, cTmpA) ;

        if ((nB >= nI) && (nB <= (nI + 5)))
            {
            nB = nI ;
            }
        else
        if (nB > (nI + 5))
            {
            nB = nB - 6 ;
            }
        }

    if (Login.nLoginType == SERVER_ICC)
        {
        // remove "\r\n  "
        while (strstr (cTmp, "\r\n  "))
            {
            cP = cTmp ;
            cQ = cTmpB ;
            nI = 0 ;
            while (strncmp (cP, "\r\n  ", 4))
                {
                *cQ++ = *cP++ ;
                nI = nI + 1 ;
                }
            *cQ = NULL_CHAR ;

            strcpy (cTmpA, (cP + 4)) ;

            strcpy (cTmp, cTmpB) ;
            strcat (cTmp, cTmpA) ;

            if ((nB >= nI) && (nB <= (nI + 3)))
                {
                nB = nI ;
                }
            else
            if (nB > (nI + 3))
                {
                nB = nB - 4 ;
                }
            }
        }

    // scan for "www." and "http://"
    nL = strlen (cTmp) ;
    if (nB >= nL)
        {
        nB = nL - 1 ;
        }
    nS = -1 ;
    nI = nB ;
    cP = (cTmp + nB) ;
    while (nI >= 0)
        {
        if (! strncmp (cP, "www.", 4))
            {
            // check for http://www.
            if (nI >= 7)
                {
                if ((cTmp [nI - 1] == '/') &&
                    (cTmp [nI - 2] == '/') &&
                    (cTmp [nI - 3] == ':') &&
                    (cTmp [nI - 4] == 'p') &&
                    (cTmp [nI - 5] == 't') &&
                    (cTmp [nI - 6] == 't') &&
                    (cTmp [nI - 7] == 'h'))
                    {
                    nS = nI - 7 ;
                    }
                else
                    {
                    nS = nI ;
                    }
                }
            else
                {
                nS = nI ;
                }
            break ;
            }
        else
        if (! strncmp (cP, "http://", 7))
            {
            nS = nI ;
            break ;
            }
        else
            {
            cP = cP - 1 ;
            nI = nI - 1 ;
            }
        }

    // found
    if ((nS != -1) && (nS < nL))
        {
        bE = 0 ;
        nJ = 0 ;
        for (nI = nS ; nI < nB ; nI++)
            {
            if ((cTmp [nI] == '\n') || (cTmp [nI] == '\r'))
                {
                bE = 1 ;
                break ;
                }
            else
            if ((cTmp [nI] == ' ') ||
                (cTmp [nI] == '(') ||
                (cTmp [nI] == ')') ||
                (cTmp [nI] == '"'))
                {
                bE = 1 ;
                break ;
                }
            else
                {
                cTmpB [nJ] = cTmp [nI] ;
                nJ = nJ + 1 ;
                }
            }
        cTmpB [nJ] = NULL_CHAR ;

        if (bE)
            {
            strcpy (cTmpA, "") ;
            }
        else
            {
            nJ = 0 ;
            for (nI = nB ; nI < nL ; nI++)
                {
                if ((cTmp [nI] == '\n') || (cTmp [nI] == '\r'))
                    {
                    break ;
                    }
                else
                if ((cTmp [nI] == ' ') ||
                    (cTmp [nI] == '(') ||
                    (cTmp [nI] == ')') ||
                    (cTmp [nI] == '"'))
                    {
                    break ;
                    }
                else
                    {
                    cTmpA [nJ] = cTmp [nI] ;
                    nJ = nJ + 1 ;
                    }
                }
            cTmpA [nJ] = NULL_CHAR ;
            }

        strcpy (cTmp, cTmpB) ;
        strcat (cTmp, cTmpA) ;

        nJ = strlen (cTmp) ;
        if (nJ > 0)
            {
            for (nI = 0 ; nI < nJ ; nI++)
                {
                if (cTmp [nI] == '(' || cTmp [nI] == ')'  ||
                    cTmp [nI] == '[' || cTmp [nI] == ']'  ||
                    cTmp [nI] == '{' || cTmp [nI] == '}'  ||
                    cTmp [nI] == ' ' || cTmp [nI] == '\t' ||
                    cTmp [nI] == '"')
                    {
                    cTmp [nI] = NULL_CHAR ;
                    break ;
                    }
                }
            ShellExecute (NULL, "open", cTmp, NULL, "", SW_SHOW) ;
            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
            return 1 ;
            }
        }

    // make sure before and after has characters
	strcpy (cBefore, cBeforeTemp);
	strcpy (cAfter, cAfterTemp);
    nB = strlen (cBefore) ;
    nA = strlen (cAfter) ;

    // nothing to do
    if ((nB == 0) && (nA == 0))
        {
        // restore back to original selection
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
        return 0 ;
        }

    //
    // check for '"'
    //
    strcpy (cTmp, cBefore) ;
    strcat (cTmp, cAfter) ;

    // remove "\r\n\\   "
    while (strstr (cTmp, "\r\n\\   "))
        {
        cP = cTmp ;
        cQ = cTmpB ;
        nI = 0 ;
        while (strncmp (cP, "\r\n\\   ", 6))
            {
            *cQ++ = *cP++ ;
            nI = nI + 1 ;
            }
        *cQ = NULL_CHAR ;

        strcpy (cTmpA, (cP + 6)) ;

        strcpy (cTmp, cTmpB) ;
        strcat (cTmp, cTmpA) ;

        if ((nB >= nI) && (nB <= (nI + 5)))
            {
            nB = nI ;
            }
        else
        if (nB > (nI + 5))
            {
            nB = nB - 6 ;
            }
        }

    // scan for '"' and '['
    nL = strlen (cTmp) ;
    if (nB >= nL)
        {
        nB = nL - 1 ;
        }
    nS = -1 ;
    nI = nB ;
    while (nI >= 0)
        {
        if (cTmp [nI] == '"' || cTmp [nI] == '[')
            {
            nS = nI + 1 ;
            break ;
            }
        else
            {
            nI = nI - 1 ;
            }
        }

    // found
    if ((nS != -1) && (nS < nL))
        {
        bE = 0 ;
        nJ = 0 ;
        for (nI = nS ; nI < nL ; nI++)
            {
            if ((cTmp [nI] == '\n') || (cTmp [nI] == '\r'))
                {
                break ;
                }
            else
            if (cTmp [nI] == '"' || cTmp [nI] == ']')
                {
                bE = 1 ;
                break ;
                }
            else
                {
                cTmpB [nJ] = cTmp [nI] ;
                nJ = nJ + 1 ;
                }
            }
        cTmpB [nJ] = NULL_CHAR ;

        // found
        if (bE)
            {
            if (strlen (cTmpB) > 0)
                {
                // scan for "http://" and "www."
                if ((! strncmp (cTmpB, "http://", 7)) ||
                    (! strncmp (cTmpB, "www.",    4)))
                    {
                    nJ = strlen (cTmpB) ;
                    if (nJ > 0)
                        {
                        for (nI = 0 ; nI < nJ ; nI++)
                            {
                            if (cTmpB [nI] == '(' || cTmpB [nI] == ')'  ||
                                cTmpB [nI] == '[' || cTmpB [nI] == ']'  ||
                                cTmpB [nI] == '{' || cTmpB [nI] == '}'  ||
                                cTmpB [nI] == ' ' || cTmpB [nI] == '\t' ||
                                cTmpB [nI] == '"')
                                {
                                cTmpB [nI] = NULL_CHAR ;
                                break ;
                                }
                            }
                        ShellExecute (NULL, "open", cTmpB, NULL, "", SW_SHOW) ;
                        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                        return 1 ;
                        }
                    }
                else
                    {
                    bE = 0 ;
                    for (nI = 0 ; nI < MAX_UNDERLINE_STRING ; nI++)
                        {
                        if (! strncmp (cTmpB, TelnetUnderLine [nI], TelnetUnderSize [nI]))
                            {
                            bE = 1 ;
                            break ;
                            }
                        }

                    if (bE)
                        {
                        strcpy (cTmp,  cTmpB) ;
                        strcpy (cTmpA, cTmpB) ;
                        strcat (cTmpA, "\n") ;

                        if (Timeseal.bSocketIsOpen)
                            {
                            TOOLBOX_WriteICS (cTmpA) ;
                            }

                        TOOLBOX_WriteUser (cTmpA) ;

                        if (User.bTelnetTextCommandAddHist)
                            {
                            HISTORY_Add (cTmp) ;
                            }

                        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                        return 1 ;
                        }

                    bE = 0 ;
                    for (nI = 0 ; nI < MAX_BRACKET_STRING ; nI++)
                        {
                        if (! strncmp (cTmpB, TelnetBracket [nI], TelnetBracketSize [nI]))
                            {
                            bE = 1 ;
                            break ;
                            }
                        }

                    if (bE)
                        {
                        strcpy (cTmp,  cTmpB) ;
                        strcpy (cTmpA, cTmpB) ;
                        strcat (cTmpA, "\n") ;

                        if (Timeseal.bSocketIsOpen)
                            {
                            TOOLBOX_WriteICS (cTmpA) ;
                            }

                        TOOLBOX_WriteUser (cTmpA) ;

                        if (User.bTelnetTextCommandAddHist)
                            {
                            HISTORY_Add (cTmp) ;
                            }

                        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                        return 1 ;
                        }
                    }
                }

            // restore back to original selection
            SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
            return 0 ;
            }
        }


    // restore back to original selection
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
    return 0 ;
    }

char * TOOLBOX_QuotesAroundFilename (char *cFn)
    {
    static char cBuffer [_MAX_PATH + 10] ;

    int nL ;

    nL = strlen (cFn) ;
    if (nL == 0)
        {
        strcpy (cBuffer, "") ;
        return cBuffer ;
        }

    if ((cFn [0] == '"') && (cFn [nL - 1] == '"'))
        {
        strcpy (cBuffer, cFn) ;
        return cBuffer ;
        }

    strcpy (cBuffer, "\"") ;
    strcat (cBuffer, cFn) ;
    strcat (cBuffer, "\"") ;

    return cBuffer ;
    }

void TOOLBOX_OpenNotePad (char *cFn)
    {
    char cTmp [2048] ;

    if (TOOLBOX_CanOpenFile (cFn))
        {
        ShellExecute (NULL, "open", User.cNotepad, TOOLBOX_QuotesAroundFilename (cFn), "", SW_SHOW) ;
        }
    else
        {
        sprintf (cTmp, "Opening %s Failed.", cFn) ;
        TOOLBOX_Error (cTmp) ;
        }
    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

void TOOLBOX_OpenGame (char *cFn)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    char cTmp [2048] ;

    _splitpath (cFn, Drive, Dir, File, Ext) ;

    if (strlen (cFn) > 0)
        {
        if (TOOLBOX_CanOpenFile (cFn))
            {
            if (stricmp (Ext, PGN_EXT) == 0)
                {
                if (strlen (User.cPGNViewer) == 0)
                    {
                    TOOLBOX_Error ("Where is Your PGN Viewer?  Please Use [Log Game Settings] to Locate it.") ;
                    PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, (WPARAM) IDM_LOG_GAME_SETTINGS, (LPARAM) 0) ;
                    }
                else
                    {
                    ShellExecute (NULL, "open", User.cPGNViewer, TOOLBOX_QuotesAroundFilename (cFn), "", SW_SHOW) ;
                    }
                }
            else
            if (stricmp (Ext, BPGN_EXT) == 0)
                {
                if (strlen (User.cBPGNViewer) == 0)
                    {
                    TOOLBOX_Error ("Where is Your BPGN Viewer?  Please Use [Log Game Settings] to Locate it.") ;
                    PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, (WPARAM) IDM_LOG_GAME_SETTINGS, (LPARAM) 0) ;
                    }
                else
                    {
                    ShellExecute (NULL, "open", User.cBPGNViewer, TOOLBOX_QuotesAroundFilename (cFn), "", SW_SHOW) ;
                    }
                }
            else
                {
                TOOLBOX_Error ("Expecting .PGN or .BPGN File.") ;
                }
            }
        else
            {
            sprintf (cTmp, "Opening %s Failed.", cFn) ;
            TOOLBOX_Error (cTmp) ;
            }
        }
    else
        {
        TOOLBOX_Error ("Missing File to Open.") ;
        }
    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

void TOOLBOX_RedrawBoard (int nG)
    {
    HDC hdc ;

    if (IsWindow (Game [nG].hwnd))
        {
        if (! IsIconic (Game [nG].hwnd))
            {
            hdc = GetDC (Game [nG].hwnd) ;
            BOARD_CheckFlip (nG) ;
            BOARD_DrawRepaint (nG, Game [nG].hwnd, hdc) ;
            ReleaseDC (Game [nG].hwnd, hdc) ;
            }
        }
    }

void TOOLBOX_IssueISet (int bMs, int bSP, int bTP, int bSM)
    {
    if (Login.nLoginType == SERVER_FICS)
        {
        if (bMs)
            {
            // iset ms 1
            TOOLBOX_WriteICS (FICS_SET_ISET_MS_1_COMMAND) ;
            }

        if (bSP)
            {
            // iset startpos 1
            TOOLBOX_WriteICS (FICS_SET_ISET_SP_1_COMMAND) ;
            }

        if (bTP)
            {
            // iset premove 1/0
			if (User.bTruePremove)
				{            
				TOOLBOX_WriteICS (FICS_SET_ISET_TP_1_COMMAND) ;
				}
			else
				{
				TOOLBOX_WriteICS (FICS_SET_ISET_TP_0_COMMAND) ;
				}
            }

        if (bSM)
            {
			if (User.bSmartMove)
            // iset smartmove 1/0
				{ 
				TOOLBOX_WriteICS (FICS_SET_ISET_SM_1_COMMAND) ;
				}
			else
				{
				TOOLBOX_WriteICS (FICS_SET_ISET_SM_0_COMMAND) ;
				}
            }
        }
    }

void TOOLBOX_CheckObserveExam (char *cS)
    {
    if (! Game [INDEX_PLAY].bPlaying)
        {
        char cTmp [4096 + 10], cTmp1 [4096 + 10] ;

        sscanf (cS, "%s %s", &cTmp, cTmp1) ;

        if ((strlen (cTmp) > 0) && (strlen (cTmp1) > 0))
            {
            if ((cTmp [0] == 'o' || cTmp [0] == 'O') &&
                (cTmp [1] == 'b' || cTmp [1] == 'B'))
                {
                cTmp1 [ICS_HANDLE_LENGTH] = NULL_CHAR ;
                strcpy (Vars.cObserve, cTmp1) ;
                return ;
                }

            if ((cTmp [0] == 'e' || cTmp [0] == 'E') &&
                (cTmp [1] == 'x' || cTmp [1] == 'X'))
                {
                cTmp1 [ICS_HANDLE_LENGTH] = NULL_CHAR ;
                strcpy (Vars.cExam, cTmp1) ;
                return ;
                }
            }
        }
    }

void TOOLBOX_LowerString (char *cS)
    {
    int nL, nI ;

    nL = strlen (cS) ;
    for (nI = 0 ; nI < nL ; nI++)
        {
        cS [nI] = tolower (cS[nI]) ;
        }
    }

int TOOLBOX_MatchHandle (char *cM, char *cH)
    {
    int nM, nH ;
    char cTmp1 [50], cTmp2 [50] ;

    nM = strlen (cM) ;
    nH = strlen (cH) ;

    if (nM > nH)
        {
        strcpy (cTmp1, cM) ;
        strcpy (cTmp2, cH) ;

        TOOLBOX_LowerString (cTmp1) ;
        TOOLBOX_LowerString (cTmp2) ;

        if (! strncmp (cTmp1, cTmp2, nH))
            {
            return 1 ;
            }
        }
    else
    if (nM < nH)
        {
        }
    else
        {
        return (stricmp (cM, cH) == 0) ;
        }

    return 0 ;
    }

char * TOOLBOX_GetGameTypeString (int nG)
    {
    static char cBuffer [255] ;

    if (strlen (Game [nG].cGameType) > 0)
        {
        if (Game [nG].nRated == 0)
            {
            sprintf (cBuffer, "%d %d u %s", Game [nG].nInitialClock, Game [nG].nIncrementClock, Game [nG].cGameType) ;
            }
        else
        if (Game [nG].nRated == 1)
            {
            sprintf (cBuffer, "%d %d r %s", Game [nG].nInitialClock, Game [nG].nIncrementClock, Game [nG].cGameType) ;
            }
        else
            {
            sprintf (cBuffer, "%d %d %s", Game [nG].nInitialClock, Game [nG].nIncrementClock, Game [nG].cGameType) ;
            }
        }
    else
        {
        sprintf (cBuffer, "%d %d", Game [nG].nInitialClock, Game [nG].nIncrementClock) ;
        }
    return cBuffer ;
    }

int TOOLBOX_ShiftArrowKey (int nB)
    {
    HWND hwnd ;
    int nJ, nI ;

    hwnd = (HWND) SendMessage (hwndWindow [HWND_CLIENT], WM_MDIGETACTIVE, 0, 0) ;
    if (! IsWindow (hwnd))
        {
        return 0 ;
        }

    if (! Timeseal.bSocketIsOpen)
        {
        return 0 ;
        }

    nJ = -1 ;

    for (nI = 1 ; nI < MAX_GAME ; nI++)
        {
        if (hwnd == Game [nI].hwnd)
            {
            if (Game [nI].bShowMoveButtons)
                {
                if (Game [nI].bValid)
                    {
                    OBSERVE_Command (hwnd, nB) ;
                    }
                return 1 ;
                }
            else
                {
                return 0 ;
                }
            }
        else
            {
            if (nJ == -1)
                {
                if (Game [nI].bShowMoveButtons)
                    {
                    if (Game [nI].bValid)
                        {
                        nJ = nI ;
                        }
                    }
                }
            }
        }

    if (Game [INDEX_PLAY].bShowMoveButtons)
        {
        PLAY_Command (nB) ;
        return 1 ;
        }
    else
    if (nJ > 0)
        {
        OBSERVE_Command (Game [nJ].hwnd, nB) ;
        return 1 ;
        }
    else
        {
        return 0 ;
        }
    }

int TOOLBOX_OKMoveButton (int nI)
    {
    if ((Game [nI].nRelation ==  0) ||  //  0 i am observing a game being played
        (Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
        (Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
        {
        if (Login.nLoginType == SERVER_ICC)
            {
//          if ((Game [nI].nGameType == GAMETYPE_ICC_WILD1 ) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD2 ) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD3 ) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD4 ) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD6 ) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD16) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD20) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD21) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD22) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD27) ||
//              (Game [nI].nGameType == GAMETYPE_ICC_WILD28))
//              {
//              return 0 ;
//              }

            if ((Game [nI].nGameType == GAMETYPE_ICC_WILD6 ) ||
                (Game [nI].nGameType == GAMETYPE_ICC_WILD16) ||
                (Game [nI].nGameType == GAMETYPE_ICC_WILD20) ||
                (Game [nI].nGameType == GAMETYPE_ICC_WILD21) ||
                (Game [nI].nGameType == GAMETYPE_ICC_WILD22) ||
                (Game [nI].nGameType == GAMETYPE_ICC_WILD27) ||
                (Game [nI].nGameType == GAMETYPE_ICC_WILD28))
                {
                return 0 ;
                }
            }
        else
            {
//            if (Game [nI].nGameType == GAMETYPE_FICS_WILDFR)
//                {
//                return 0 ;
//                }
            }
        return 1 ;
        }
    return 0 ;
    }

int TOOLBOX_DisplayActualBoard (int nI)
    {
    if (! TOOLBOX_OKMoveButton (nI))
        {
        return 1 ;
        }

    if (Game [nI].bClickedButton)
        {
        if (Game [nI].nMaxIndex == -2)
            {
            return 1 ;
            }
        else
        if ((Game [nI].nCurrentIndex == Game [nI].nMaxIndex) &&
            (Game [nI].nCurrentColor == Game [nI].nMaxColor))
            {
            return 1 ;
            }
        else
            {
            return 0 ;
            }
        }
    else
        {
        return 1 ;
        }
    }

void TOOLBOX_IssueLoginEvent (void)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
        {
        LOGINEVENT_Command (nI) ;
        }
    }

void TOOLBOX_AdjustShowMoveButtons (int nG)
    {
    if (User.bShowMoveButton)
        {
        if (Game [nG].nGameNumber == 0)
            {
            if (Game [nG].bInitialMove)
                {
                TOOLBOX_ShowMoveButtons (nG, 0) ;
                }
            else
                {
                if (Game [nG].bFirstGame)
                    {
                    TOOLBOX_ShowMoveButtons (nG, 0) ;
                    }
                else
                    {
                    if (TOOLBOX_OKMoveButton (nG))
                        {
                        TOOLBOX_ShowMoveButtons (nG, 1) ;
                        }
                    else
                        {
                        TOOLBOX_ShowMoveButtons (nG, 0) ;
                        }
                    }
                }
            }
        else
            {
            if (Game [nG].bPlaying)
                {
                if (Game [nG].bFirstGame)
                    {
                    TOOLBOX_ShowMoveButtons (nG, 0) ;
                    }
                else
                    {
                    if (TOOLBOX_OKMoveButton (nG))
                        {
                        TOOLBOX_ShowMoveButtons (nG, 1) ;
                        }
                    else
                        {
                        TOOLBOX_ShowMoveButtons (nG, 0) ;
                        }
                    }
                }
            else
                {
                if (nG == INDEX_PLAY)
                    {
                    TOOLBOX_ShowMoveButtons (nG, 1) ;
                    }
                else
                    {
                    if (TOOLBOX_OKMoveButton (nG))
                        {
                        TOOLBOX_ShowMoveButtons (nG, 1) ;
                        }
                    else
                        {
                        TOOLBOX_ShowMoveButtons (nG, 0) ;
                        }
                    }
                }
            }
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                TOOLBOX_ShowMoveButtons (nG, 1) ;
                }
            else
                {
                TOOLBOX_ShowMoveButtons (nG, 0) ;
                }
            }
        else
            {
            TOOLBOX_ShowMoveButtons (nG, 0) ;
            }
        }
    }

int TOOLBOX_NeedCommandValue (int nType)
    {
    return ((nType == FUNCTION_COMMAND         ) ||
            (nType == FUNCTION_TELNET_EDIT_CMD ) ||
            (nType == FUNCTION_INTERNET_BROWSER) ||
            (nType == FUNCTION_COPY_VARIABLE   ) ||
            (nType == FUNCTION_RUN_SCRIPT      ) ||
            (nType == FUNCTION_LOAD_PROFILE    ) ||
			(nType == FUNCTION_TELNET_TELL_HANDLE ));
    }

void TOOLBOX_Issue_Command (char *cFICS, char *cICC, char *cNONFICS, int bAdd)
    {
    char cTmp [2048] ;
    int nS ;

    if (Timeseal.bSocketIsOpen)
        {
        switch (Login.nLoginType)
            {
            case SERVER_FICS :
                strcpy (cTmp, cFICS) ;
                break ;

            case SERVER_ICC :
                strcpy (cTmp, cICC) ;
                break ;

            case SERVER_NONFICS :
                strcpy (cTmp, cNONFICS) ;
                break ;

            default :
                strcpy (cTmp, cFICS) ;
                break ;
            }
        TOOLBOX_WriteICS  (cTmp) ;
        TOOLBOX_WriteUser (cTmp) ;

        if (bAdd)
            {
            nS = strlen (cTmp) ;
            if (cTmp [nS - 1] == '\n')
                {
                cTmp [nS - 1] = NULL_CHAR ;
                }

            HISTORY_Add (cTmp) ;
            }
        }
    else
        {
        TOOLBOX_Beep () ;
        }
    }

char * TOOLBOX_GetMyGamePath (void)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cTmp  [_MAX_DIR] ;

    int nL ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    nL = strlen (Dir) ;
    if (nL == 0)
        {
        strcpy (cTmp, "\\GAME_LOGS\\MY_GAMES\\") ;
        }
    else
        {
        if (Dir [nL - 1] == '\\')
            {
            strcpy (cTmp, "GAME_LOGS\\MY_GAMES\\") ;
            }
        else
            {
            strcpy (cTmp, "\\GAME_LOGS\\MY_GAMES\\") ;
            }
        }
    strcat (Dir, cTmp) ;
    _makepath (cBuffer, Drive, Dir, "", "") ;

    return cBuffer ;
    }

char * TOOLBOX_GetObservedGamePath (void)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cTmp  [_MAX_DIR] ;

    int nL ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    nL = strlen (Dir) ;
    if (nL == 0)
        {
        strcpy (cTmp, "\\GAME_LOGS\\OBSERVED_GAMES\\") ;
        }
    else
        {
        if (Dir [nL - 1] == '\\')
            {
            strcpy (cTmp, "GAME_LOGS\\OBSERVED_GAMES\\") ;
            }
        else
            {
            strcpy (cTmp, "\\GAME_LOGS\\OBSERVED_GAMES\\") ;
            }
        }
    strcat (Dir, cTmp) ;
    _makepath (cBuffer, Drive, Dir, "", "") ;

    return cBuffer ;
    }

char * TOOLBOX_GetOtherGamePath (void)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cTmp  [_MAX_DIR] ;

    int nL ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    nL = strlen (Dir) ;
    if (nL == 0)
        {
        strcpy (cTmp, "\\GAME_LOGS\\OTHER_GAMES\\") ;
        }
    else
        {
        if (Dir [nL - 1] == '\\')
            {
            strcpy (cTmp, "GAME_LOGS\\OTHER_GAMES\\") ;
            }
        else
            {
            strcpy (cTmp, "\\GAME_LOGS\\OTHER_GAMES\\") ;
            }
        }
    strcat (Dir, cTmp) ;
    _makepath (cBuffer, Drive, Dir, "", "") ;

    return cBuffer ;
    }

char * TOOLBOX_GetWavePath (void)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cTmp  [_MAX_DIR] ;

    int nL ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    nL = strlen (Dir) ;
    if (nL == 0)
        {
        strcpy (cTmp, "\\WAVE\\") ;
        }
    else
        {
        if (Dir [nL - 1] == '\\')
            {
            strcpy (cTmp, "WAVE\\") ;
            }
        else
            {
            strcpy (cTmp, "\\WAVE\\") ;
            }
        }
    strcat (Dir, cTmp) ;
    _makepath (cBuffer, Drive, Dir, "", "") ;

    return cBuffer ;
    }

char * TOOLBOX_GetFullWaveFilename (char *cWave)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cFn   [_MAX_FNAME] ;
    char cTmp  [_MAX_DIR] ;

    int nL ;

    if (strlen (cWave) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cWave, Drive, Dir, cFn, Ext) ;

        _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

        nL = strlen (Dir) ;
        if (nL == 0)
            {
            strcpy (cTmp, "\\WAVE\\") ;
            }
        else
            {
            if (Dir [nL - 1] == '\\')
                {
                strcpy (cTmp, "WAVE\\") ;
                }
            else
                {
                strcpy (cTmp, "\\WAVE\\") ;
                }
            }

        strcat (Dir, cTmp) ;
        strcpy (Ext, WAV_EXT) ;
        _makepath (cBuffer, Drive, Dir, cFn, Ext) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetWaveFilename (char *cWave)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    if (strlen (cWave) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cWave, Drive, Dir, File, Ext) ;

        strcpy (cBuffer, File) ;
        strcat (cBuffer, WAV_EXT) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetHelperPath (void)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    _splitpath (TOOLBOX_GetInstDirPath(), Drive, Dir, File, Ext) ;

    _makepath (cBuffer, Drive, Dir, "", "") ;

    return cBuffer ;
    }

char * TOOLBOX_GetFullHelperFilename (char *cHelper)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cTmp  [_MAX_DIR] ;
    char cEEE  [_MAX_EXT] ;

    if (strlen (cHelper) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cHelper, Drive, Dir, cTmp, cEEE) ;

        _splitpath (TOOLBOX_GetInstDirPath(), Drive, Dir, File, Ext) ;

        _makepath (cBuffer, Drive, Dir, cTmp, cEEE) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetHelperFilename (char *cHelper)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    if (strlen (cHelper) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cHelper, Drive, Dir, File, Ext) ;

        strcpy (cBuffer, File) ;
        strcat (cBuffer, Ext) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetScriptPath (void)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    _makepath (cBuffer, Drive, Dir, "", "") ;

    return cBuffer ;
    }

char * TOOLBOX_GetFullScriptFilename (char *cScript)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cTmp  [_MAX_DIR] ;
    char cEEE  [_MAX_EXT] ;

    if (strlen (cScript) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cScript, Drive, Dir, cTmp, cEEE) ;

        _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

        _makepath (cBuffer, Drive, Dir, cTmp, cEEE) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetScriptFilename (char *cScript)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    if (strlen (cScript) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cScript, Drive, Dir, File, Ext) ;

        strcpy (cBuffer, File) ;
        strcat (cBuffer, Ext) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetFullConFilename (char *cCon)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cTmp  [_MAX_DIR] ;
    char cEEE  [_MAX_EXT] ;

    if (strlen (cCon) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cCon, Drive, Dir, cTmp, cEEE) ;

        _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

        _makepath (cBuffer, Drive, Dir, cTmp, cEEE) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetConFilename (char *cCon)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    if (strlen (cCon) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cCon, Drive, Dir, File, Ext) ;

        strcpy (cBuffer, File) ;
        strcat (cBuffer, Ext) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetFullProfileFilename (char *cProf)
    {
    static char cBuffer [_MAX_PATH] ;

    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char cFn   [_MAX_FNAME] ;
    char cTmp  [_MAX_DIR] ;

    int nL ;

    if (strlen (cProf) == 0)
        {
        strcpy (cBuffer, "") ;
        }
    else
        {
        _splitpath (cProf, Drive, Dir, cFn, Ext) ;

        _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

        nL = strlen (Dir) ;
        if (nL == 0)
            {
            strcpy (cTmp, "\\PROFILE\\") ;
            }
        else
            {
            if (Dir [nL - 1] == '\\')
                {
                strcpy (cTmp, "PROFILE\\") ;
                }
            else
                {
                strcpy (cTmp, "\\PROFILE\\") ;
                }
            }

        strcat (Dir, cTmp) ;
        strcpy (Ext, INI_EXT) ;
        _makepath (cBuffer, Drive, Dir, cFn, Ext) ;
        }
    return cBuffer ;
    }

char * TOOLBOX_GetInstDirPath (void)
    {
    static char cTmp  [_MAX_PATH] ;

	char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

	GetModuleFileName(NULL, cTmp, MAX_PATH);
	_splitpath (cTmp, Drive, Dir, File, Ext) ;
	_makepath (cTmp, Drive, Dir, NULL, NULL) ;
    return cTmp;
    }

char * TOOLBOX_GetMyDocumentPath (void)
    {
    static char cBuffer [_MAX_PATH] ;

 	SHGetSpecialFolderPath(HWND_DESKTOP, cBuffer, THIEF_DOCUMENT_PATH, FALSE);
	strcat(cBuffer,"\\");
	strcat(cBuffer,THIEF_NAME);
    return cBuffer ;
    }

void TOOLBOX_SortLoginString (void)
    {
    int nI, nJ ;

    char cLS [MAX_LOGIN_SIZE + 10] ;

    for (nI = 0 ; nI < Login.nLoginString ; nI++)
        {
        for (nJ = 0 ; nJ < Login.nLoginString ; nJ++)
            {
            if (nI != nJ)
                {
                if (stricmp (Login.cLoginString [nI], Login.cLoginString [nJ]) < 0)
                    {
                    strcpy (cLS, Login.cLoginString [nI]) ;
                    strcpy (Login.cLoginString [nI], Login.cLoginString [nJ]) ;
                    strcpy (Login.cLoginString [nJ], cLS) ;
                    }
                }
            }
        }
    }

int TOOLBOX_LoadProfile (char *cFn)
    {
    RECT rc ;
    char cTmp1 [2048] ;

    if (strlen (cFn) == 0)
        {
        return 0 ;
        }

    CENSOR_Init () ;
	SILENCE_Init () ;

    INI_ReadSetup (cFn) ;
    INI_ReadSystem (cFn) ;
    LAYOUT_ProfileRestore (cFn) ;

    DeleteObject (hfFont [FONT_HANDLE].     hfFont) ;
    DeleteObject (hfFont [FONT_CLOCK].      hfFont) ;
    DeleteObject (hfFont [FONT_GAMETYPE].   hfFont) ;
    DeleteObject (hfFont [FONT_LASTMOVE].   hfFont) ;
    DeleteObject (hfFont [FONT_LAGSTAT].    hfFont) ;
    DeleteObject (hfFont [FONT_RESULT].     hfFont) ;
    DeleteObject (hfFont [FONT_TELNET].     hfFont) ;
    DeleteObject (hfFont [FONT_BUTTON].     hfFont) ;
    DeleteObject (hfFont [FONT_COORDINATES].hfFont) ;

    TOOLBOX_MakeFont (FONT_HANDLE) ;
    TOOLBOX_MakeFont (FONT_CLOCK) ;
    TOOLBOX_MakeFont (FONT_GAMETYPE) ;
    TOOLBOX_MakeFont (FONT_LASTMOVE) ;
    TOOLBOX_MakeFont (FONT_LAGSTAT) ;
    TOOLBOX_MakeFont (FONT_RESULT) ;
    TOOLBOX_MakeFont (FONT_TELNET) ;
    TOOLBOX_MakeFont (FONT_BUTTON) ;
    TOOLBOX_MakeFont (FONT_COORDINATES) ;

    SendMessage (hwndWindow [HWND_BUTTON], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM (TRUE, 0)) ;
    BUTTON_Refresh () ;
    BUTTON_RefreshLabel () ;
    GetClientRect (hwndWindow [HWND_BUTTON], &rc) ;
    SendMessage (hwndWindow [HWND_BUTTON], WM_SIZE, SIZE_RESTORED, (LPARAM) ((rc.bottom << 16) | rc.right)) ;

    SOUND_Load () ;
    EAR_Load () ;
    GAMESOUND_Load () ;
    TIMESOUND_Load () ;
    DRAW_LoadPieces () ;
    RIGHTMOUSE_LoadCheck () ;
    TOOLBOX_CheckAllMenu () ;

    SYS_ChangeColor () ;
    TOOLBOX_RefreshWindowColor () ;
    if (User.bActualSize)
        {
        TOOLBOX_InvalidateBoardWindow1 (1) ;
        }
    else
        {
        TOOLBOX_InvalidateBoardWindow (1) ;
        }

    TELNET_SetFont () ;
    TELNET_SetEditColor () ;
    Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrColor [CLR_TELNET_BACKGROUND]) ;
    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrColor [CLR_TELNET_BACKGROUND]) ;

    if (Timeseal.bSocketIsOpen)
        {
        if (User.bLogTelnet)
            {
            if (LOG_Start ())
                {
                sprintf (cFn, "Creating %s Successful\n", Log.cFn) ;
                }
            else
                {
                sprintf (cFn, "Creating %s Failed\n", Log.cFn) ;
                }
            TOOLBOX_WriteSystem (cFn) ;
            }
        else
            {
            if (LOG_End ())
                {
                sprintf (cFn, "Closing %s Successful\n", Log.cFn) ;
                TOOLBOX_WriteSystem (cFn) ;
                }
            LOG_Init () ;
            }
        }
    TOOLBOX_IssueISet (1, 1, 1, 1) ;

    sprintf (cTmp1, "Loading Profile %s Successful\n", cFn) ;
    TOOLBOX_WriteSystem (cTmp1) ;

    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    return 1 ;
    }

int TOOLBOX_ShowBuffer (int nG)
    {
    if (Game [nG].nGameType == GAMETYPE_CRAZYHOUSE || Game [nG].nGameType == GAMETYPE_BUGHOUSE)
        {
        return 1 ;
        }
    else
        {
        return User.nShowCapturedChessPiece ;
        }
    }

int TOOLBOX_ShowBuffer1 (int nGT)
    {
    if (nGT == GAMETYPE_CRAZYHOUSE || nGT == GAMETYPE_BUGHOUSE)
        {
        return 1 ;
        }
    else
        {
        return User.nShowCapturedChessPiece ;
        }
    }

void TOOLBOX_AdjustClipboardData (HWND hwnd)
    {
    HGLOBAL hGMem, hGNew ;
    PSTR    pGMem, pGNew ;
    int     nL, nI ;
    char    *cS, *cD ;

    OpenClipboard (hwnd) ;

    hGMem = GetClipboardData (CF_TEXT) ;

    if (hGMem != NULL)
        {
        pGMem = (PSTR) GlobalLock (hGMem) ;
        nL    = strlen (pGMem) ;

        if (nL > 0)
            {
            hGNew = GlobalAlloc (GHND, nL + 1) ;

            if (hGNew != NULL)
                {
                pGNew = (PSTR) GlobalLock (hGNew) ;

                cS = pGMem ;
                cD = pGNew ;

                for (nI = 0 ; nI < nL ; nI++)
                    {
                    if (Login.nLoginType == SERVER_ICC)
                        {
                        if ((*cS       == ' ')  &&
                            (*(cS + 1) == '\r') &&
                            (*(cS + 2) == '\n') &&
                            (*(cS + 3) == '\\') &&
                            (*(cS + 4) == ' ')  &&
                            (*(cS + 5) == ' ')  &&
                            (*(cS + 6) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\r') &&
                            (*(cS + 1) == '\n') &&
                            (*(cS + 2) == '\\') &&
                            (*(cS + 3) == ' ')  &&
                            (*(cS + 4) == ' ')  &&
                            (*(cS + 5) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\n') &&
                            (*(cS + 1) == '\\') &&
                            (*(cS + 2) == ' ')  &&
                            (*(cS + 3) == ' ')  &&
                            (*(cS + 4) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\r') &&
                            (*(cS + 1) == '\n') &&
                            (*(cS + 2) == '\\') &&
                            (*(cS + 3) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == ' ')  &&
                            (*(cS + 1) == '\r') &&
                            (*(cS + 2) == '\n') &&
                            (*(cS + 3) == ' ')  &&
                            (*(cS + 4) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\r') &&
                            (*(cS + 1) == '\n') &&
                            (*(cS + 2) == ' ')  &&
                            (*(cS + 3) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\n') &&
                            (*(cS + 1) == ' ')  &&
                            (*(cS + 2) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\r') &&
                            (*(cS + 1) == '\n') &&
                            (*(cS + 2) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if (*cS == '\r')
                            {
                            *cS++ ;
                            }
                        if (*cS == '\n')
                            {
                            *cS++ ;
                            }
                        else
                            {
                            *cD++ = *cS++ ;
                            }
                        }
                    else
                        {
                        if ((*cS       == ' ')  &&
                            (*(cS + 1) == '\r') &&
                            (*(cS + 2) == '\n') &&
                            (*(cS + 3) == '\\') &&
                            (*(cS + 4) == ' ')  &&
                            (*(cS + 5) == ' ')  &&
                            (*(cS + 6) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\r') &&
                            (*(cS + 1) == '\n') &&
                            (*(cS + 2) == '\\') &&
                            (*(cS + 3) == ' ')  &&
                            (*(cS + 4) == ' ')  &&
                            (*(cS + 5) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\n') &&
                            (*(cS + 1) == '\\') &&
                            (*(cS + 2) == ' ')  &&
                            (*(cS + 3) == ' ')  &&
                            (*(cS + 4) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if ((*cS       == '\r') &&
                            (*(cS + 1) == '\n') &&
                            (*(cS + 2) == '\\') &&
                            (*(cS + 3) == ' '))
                            {
                            *cS++ ;
                            *cS++ ;
                            *cS++ ;
                            }
                        else
                        if (*cS == '\r')
                            {
                            *cS++ ;
                            }
                        if (*cS == '\n')
                            {
                            *cS++ ;
                            }
                        else
                            {
                            *cD++ = *cS++ ;
                            }
                        }
                    }
                GlobalUnlock (hGNew) ;

                EmptyClipboard () ;
                SetClipboardData (CF_TEXT, hGNew) ;
                }
            }
        GlobalUnlock (hGMem) ;
        }
    CloseClipboard () ;
    }

int TOOLBOX_IsQChannelTell (char *cS, char *cC)
    {
    int bR, nS, nI ;

    bR = 0 ;
    nS = strlen (cS) ;
    for (nI = 0 ; nI < nS ; nI++)
        {
        if (cS [nI    ] == ')' &&
            cS [nI + 1] == ':' &&
            cS [nI + 2] == ' ')
            {
            // (C2) (T2) (12)
            if (nI >= 3)
                {
                if (cS [nI - 3] == '(')
                    {
                    bR = 1 ;
                    cC [0] = cS [nI - 2] ;
                    cC [1] = cS [nI - 1] ;
                    cC [2] = NULL_CHAR ;
                    break ;
                    }
                }

            // (C12) (T12) (123)
            if (nI >= 4)
                {
                if (cS [nI - 4] == '(')
                    {
                    bR = 1 ;
                    cC [0] = cS [nI - 3] ;
                    cC [1] = cS [nI - 2] ;
                    cC [2] = cS [nI - 1] ;
                    cC [3] = NULL_CHAR ;
                    break ;
                    }
                }

            // (C123) (T123)
            if (nI >= 5)
                {
                if (cS [nI - 5] == '(')
                    {
                    bR = 1 ;
                    cC [0] = cS [nI - 4] ;
                    cC [1] = cS [nI - 3] ;
                    cC [2] = cS [nI - 2] ;
                    cC [3] = cS [nI - 1] ;
                    cC [4] = NULL_CHAR ;
                    break ;
                    }
                }
            }
        }
    return bR ;
    }

void TOOLBOX_DisplayBitmapSize (int nG)
    {
    int nss, nI ;
    char cTmp [512] ;

    if (User.bShowBitmapSize)
        {
        if (IsWindow (Game [nG].hwnd))
            {
            if (! IsIconic (Game [nG].hwnd))
                {
                nss = 0 ;

                for (nI = 0 ; nI < MAX_PIECE ; nI++)
                    {
                    if (CSet.ptPiece [nI].x > nss)
                        {
                        nss = CSet.ptPiece [nI].x ;
                        }

                    if (CSet.ptPiece [nI].y > nss)
                        {
                        nss = CSet.ptPiece [nI].y ;
                        }
                    }

                sprintf (cTmp, "Original=%d  Current=%d\n", nss, Game [nG].nss) ;
                TOOLBOX_WriteSystem (cTmp) ;
                }
            }
        }
    }

void TOOLBOX_DisplayMenuPath (char *cP)
    {
    int nS ;
    char cTmp [512] ;

    if (User.bShowMenuPath)
        {
        nS = strlen (cP) ;
        if (nS > 0)
            {
            strcpy (cTmp, cP) ;

            if (cTmp [nS - 1] != '\n')
                {
                strcat (cTmp, "\n") ;
                }

            TOOLBOX_WriteSystem (cTmp) ;
            }
        }
    }

void TOOLBOX_ResetPromotKnight (void)
    {
    if (System.bPromoteKnight)
        {
        System.bPromoteKnight = 0 ;
        TOOLBOX_WriteSystem ("Auto Knight Promotion is Off\n") ;
        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
        }
    }

char * TOOLBOX_GetGameWindowTitle (int nG)
    {
    static char cBuffer [255] ;
    char cValue [30] ;

    if (nG == INDEX_PLAY)
        {
        if (Game [nG].nGameNumber == 0)
            {
            if ((strcmp (Game [nG].cHandle [INDEX_WHITE], ICS_INITIAL_WHITE_HANDLE) == 0) &&
                (strcmp (Game [nG].cHandle [INDEX_BLACK], ICS_INITIAL_BLACK_HANDLE) == 0))
                {
                strcpy (cBuffer, PLAY_BOARD_TITLE) ;
                }
            else
                {
                if (User.bShowValueOnTitleBar)
                    {
                    if (Game [nG].nGameType == GAMETYPE_BUGHOUSE ||
                        Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
                        {
                        sprintf (cValue, "(%d-%d) ",
                                         Game [nG].BugValue [WHITE_PAWN] +
                                         Game [nG].BugValue [WHITE_ROOK] +
                                         Game [nG].BugValue [WHITE_KNIGHT] +
                                         Game [nG].BugValue [WHITE_BISHOP] +
                                         Game [nG].BugValue [WHITE_QUEEN],
                                         Game [nG].BugValue [BLACK_PAWN] +
                                         Game [nG].BugValue [BLACK_ROOK] +
                                         Game [nG].BugValue [BLACK_KNIGHT] +
                                         Game [nG].BugValue [BLACK_BISHOP] +
                                         Game [nG].BugValue [BLACK_QUEEN]) ;
                        }
                    else
                        {
                        sprintf (cValue, "(%d-%d) ",
                                         Game [nG].ChessValue [WHITE_PAWN] +
                                         Game [nG].ChessValue [WHITE_ROOK] +
                                         Game [nG].ChessValue [WHITE_KNIGHT] +
                                         Game [nG].ChessValue [WHITE_BISHOP] +
                                         Game [nG].ChessValue [WHITE_QUEEN],
                                         Game [nG].ChessValue [BLACK_PAWN] +
                                         Game [nG].ChessValue [BLACK_ROOK] +
                                         Game [nG].ChessValue [BLACK_KNIGHT] +
                                         Game [nG].ChessValue [BLACK_BISHOP] +
                                         Game [nG].ChessValue [BLACK_QUEEN]) ;
                        }
                    }
                else
                    {
                    strcpy (cValue, "") ;
                    }

                if (User.bShowGameTypeOnTitleBar)
                    {
                    sprintf (cBuffer, "%s %d (%s vs. %s) %s%s",
                             PLAY_BOARD_TITLE,
                             Game [nG].nGameNumber,
                             Game [nG].cHandle [INDEX_WHITE],
                             Game [nG].cHandle [INDEX_BLACK],
                             cValue,
                             TOOLBOX_GetGameTypeString (nG)) ;
                    }
                else
                    {
                    sprintf (cBuffer, "%s %d (%s vs. %s) %s",
                             PLAY_BOARD_TITLE,
                             Game [nG].nGameNumber,
                             Game [nG].cHandle [INDEX_WHITE],
                             Game [nG].cHandle [INDEX_BLACK],
                             cValue) ;
                    }
                }
            }
        else
            {
            if (User.bShowValueOnTitleBar)
                {
                if (Game [nG].nGameType == GAMETYPE_BUGHOUSE ||
                    Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
                    {
                    sprintf (cValue, "(%d-%d) ",
                                     Game [nG].BugValue [WHITE_PAWN] +
                                     Game [nG].BugValue [WHITE_ROOK] +
                                     Game [nG].BugValue [WHITE_KNIGHT] +
                                     Game [nG].BugValue [WHITE_BISHOP] +
                                     Game [nG].BugValue [WHITE_QUEEN],
                                     Game [nG].BugValue [BLACK_PAWN] +
                                     Game [nG].BugValue [BLACK_ROOK] +
                                     Game [nG].BugValue [BLACK_KNIGHT] +
                                     Game [nG].BugValue [BLACK_BISHOP] +
                                     Game [nG].BugValue [BLACK_QUEEN]) ;
                    }
                else
                    {
                    sprintf (cValue, "(%d-%d) ",
                                     Game [nG].ChessValue [WHITE_PAWN] +
                                     Game [nG].ChessValue [WHITE_ROOK] +
                                     Game [nG].ChessValue [WHITE_KNIGHT] +
                                     Game [nG].ChessValue [WHITE_BISHOP] +
                                     Game [nG].ChessValue [WHITE_QUEEN],
                                     Game [nG].ChessValue [BLACK_PAWN] +
                                     Game [nG].ChessValue [BLACK_ROOK] +
                                     Game [nG].ChessValue [BLACK_KNIGHT] +
                                     Game [nG].ChessValue [BLACK_BISHOP] +
                                     Game [nG].ChessValue [BLACK_QUEEN]) ;
                    }
                }
            else
                {
                strcpy (cValue, "") ;
                }

            if (User.bShowGameTypeOnTitleBar)
                {
                sprintf (cBuffer, "%s %d (%s vs. %s) %s%s",
                         PLAY_BOARD_TITLE,
                         Game [nG].nGameNumber,
                         Game [nG].cHandle [INDEX_WHITE],
                         Game [nG].cHandle [INDEX_BLACK],
                         cValue,
                         TOOLBOX_GetGameTypeString (nG)) ;
                }
            else
                {
                sprintf (cBuffer, "%s %d (%s vs. %s) %s",
                         PLAY_BOARD_TITLE,
                         Game [nG].nGameNumber,
                         Game [nG].cHandle [INDEX_WHITE],
                         Game [nG].cHandle [INDEX_BLACK],
                         cValue) ;
                }
            }
        }
    else
        {
        if (User.bShowValueOnTitleBar)
            {
            if (Game [nG].nGameType == GAMETYPE_BUGHOUSE ||
                Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
                {
                sprintf (cValue, "(%d-%d) ",
                                 Game [nG].BugValue [WHITE_PAWN] +
                                 Game [nG].BugValue [WHITE_ROOK] +
                                 Game [nG].BugValue [WHITE_KNIGHT] +
                                 Game [nG].BugValue [WHITE_BISHOP] +
                                 Game [nG].BugValue [WHITE_QUEEN],
                                 Game [nG].BugValue [BLACK_PAWN] +
                                 Game [nG].BugValue [BLACK_ROOK] +
                                 Game [nG].BugValue [BLACK_KNIGHT] +
                                 Game [nG].BugValue [BLACK_BISHOP] +
                                 Game [nG].BugValue [BLACK_QUEEN]) ;
                }
            else
                {
                sprintf (cValue, "(%d-%d) ",
                                 Game [nG].ChessValue [WHITE_PAWN] +
                                 Game [nG].ChessValue [WHITE_ROOK] +
                                 Game [nG].ChessValue [WHITE_KNIGHT] +
                                 Game [nG].ChessValue [WHITE_BISHOP] +
                                 Game [nG].ChessValue [WHITE_QUEEN],
                                 Game [nG].ChessValue [BLACK_PAWN] +
                                 Game [nG].ChessValue [BLACK_ROOK] +
                                 Game [nG].ChessValue [BLACK_KNIGHT] +
                                 Game [nG].ChessValue [BLACK_BISHOP] +
                                 Game [nG].ChessValue [BLACK_QUEEN]) ;
                }
            }
        else
            {
            strcpy (cValue, "") ;
            }

        if (User.bShowGameTypeOnTitleBar)
            {
            sprintf (cBuffer, "%d (%s vs. %s) %s%s",
                     Game [nG].nGameNumber,
                     Game [nG].cHandle [INDEX_WHITE],
                     Game [nG].cHandle [INDEX_BLACK],
                     cValue,
                     TOOLBOX_GetGameTypeString (nG)) ;
            }
        else
            {
            sprintf (cBuffer, "%d (%s vs. %s) %s",
                     Game [nG].nGameNumber,
                     Game [nG].cHandle [INDEX_WHITE],
                     Game [nG].cHandle [INDEX_BLACK],
                     cValue) ;
            }
        }
    return cBuffer ;
    }

void TOOLBOX_ShowTaskIcon (HWND hwnd, BOOL bShow)
{
    NOTIFYICONDATA tsym; // in diese Struktur kommen die Daten des Icons

    ZeroMemory (&tsym, sizeof (NOTIFYICONDATA));

    if (bShow)
    {
        tsym.cbSize = sizeof (NOTIFYICONDATA);
        tsym.hWnd   = hwnd;
        tsym.uID    = ID_ICON1;
        tsym.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
        tsym.uCallbackMessage = WM_ICONCLICK;
        tsym.hIcon  = hIcon;
        strcpy (tsym.szTip, THIEF_VER);
		strcat (tsym.szTip, " ");
		if (strlen(Vars.cWhoAmI) > 0)
			{
			strcat (tsym.szTip, " ");
			strcat (tsym.szTip, Vars.cWhoAmI);
			}
		else
			{
			strcat (tsym.szTip, "not connected");
			}
        Shell_NotifyIcon (NIM_ADD, &tsym);
    }


    else
    {
        tsym.cbSize = sizeof (NOTIFYICONDATA);
        tsym.hWnd   = hwnd;
        tsym.uID    = ID_ICON1;
        tsym.uFlags = 0;
        Shell_NotifyIcon (NIM_DELETE, &tsym);
    }
        return;
}