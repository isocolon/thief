#include "thief.h"

LRESULT CALLBACK TelnetWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    HDC         hdc ;
    PAINTSTRUCT ps ;
    HFONT       hfOld ;
    SIZE        szLine ;
    int         nH ;

    switch (iMsg)
        {
        case WM_CHAR :
            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
            PostMessage (hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
            return 0 ;
            
        case WM_PAINT :
            BeginPaint (hwnd, &ps) ;
            EndPaint   (hwnd, &ps) ;
            SetFocus   (hwndWindow [HWND_TELNET_EDIT]) ;
            break ;

        case WM_TIMER :
            TIMERCMD_Timer () ;
            break ;

        case WM_USER_START_TIMER :
            TIMERCMD_Set (hwnd) ;
            return 0 ;

        case WM_USER_STOP_TIMER :
            TIMERCMD_Reset (hwnd) ;
            return 0 ;

        case WM_SIZE :
            if (IsIconic (hwnd))
                {
                wCoord [COORD_TELNET].s = WIN_HIDE ;
                Telnet.nTelnetState     = WIN_MINIMIZE ;
                }
            else
                {
                wCoord [COORD_TELNET].s = WIN_SHOW ;

                if (IsZoomed (hwnd))
                    {
                    Telnet.nTelnetState = WIN_MAXIMIZE ;
                    }
                else
                    {
                    Telnet.nTelnetState = WIN_NORMAL ;
                    }

                hdc   = GetDC (hwndWindow [HWND_TELNET_EDIT]) ;
                hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_TELNET].hfFont) ;
                GetTextExtentPoint32 (hdc, "W", 1, &szLine) ;
                SelectObject (hdc, hfOld) ;
                ReleaseDC (hwndWindow [HWND_TELNET_EDIT], hdc) ;

                nH = (szLine.cy * 7) >> 2 ;
                MoveWindow (hwndWindow [HWND_TELNET_TEXT], 0, 0, LOWORD (lParam), HIWORD (lParam) - nH, TRUE) ;
                MoveWindow (hwndWindow [HWND_TELNET_EDIT], 0, HIWORD (lParam) - nH, LOWORD (lParam), nH, TRUE) ;
                }

            if (User.bTelnetResizeGoBottom)
                {
                TELNET_GoBottomFillText () ;
                }
            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
            break ;

        case WM_MDIACTIVATE :
            if (lParam == (LPARAM) hwnd)
                {
                if (wCoord [COORD_TELNET].s == WIN_SHOW)
                    {
                    ShowWindow (hwnd, SW_SHOW) ;
                    }
                else
                    {
                    //ShowWindow (hwnd, SW_HIDE) ;
                    ShowWindow (hwnd, SW_MINIMIZE) ;
                    }
                }
            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
            return 0 ;

        case WM_CREATE :
            DeleteObject ((HBRUSH) SetClassLong (hwnd, GCL_HBRBACKGROUND,
                          (LONG) CreateSolidBrush (clrColor [CLR_TELNET_BACKGROUND]))) ;

            hwndWindow [HWND_TELNET_TEXT] = CreateWindow ("RICHEDIT", NULL,
                                                          WS_BORDER | WS_CHILD | WS_TABSTOP | WS_VSCROLL | ES_AUTOVSCROLL | ES_DISABLENOSCROLL | ES_LEFT | ES_MULTILINE | ES_NOHIDESEL | ES_READONLY,
                                                          0, 0, 0, 0, hwnd, NULL,
                                                          ((LPCREATESTRUCT) lParam) -> hInstance,
                                                          NULL) ;

            hwndWindow [HWND_TELNET_EDIT] = CreateWindow ("RICHEDIT", NULL,
                                                          WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL | ES_DISABLENOSCROLL | ES_NOHIDESEL,
                                                          0, 0, 0, 0, hwnd, NULL,
                                                          ((LPCREATESTRUCT) lParam) -> hInstance,
                                                          NULL) ;

            WndProcText = (WNDPROC) SetWindowLong (hwndWindow [HWND_TELNET_TEXT], GWL_WNDPROC, (LONG) TelnetTextSubclass) ;
            WndProcEdit = (WNDPROC) SetWindowLong (hwndWindow [HWND_TELNET_EDIT], GWL_WNDPROC, (LONG) TelnetEditSubclass) ;

            TELNET_NormalWrite ("\n") ;

            TELNET_Clear () ;
            return 0 ;

        case WM_CLOSE :
            wCoord [COORD_TELNET].s = WIN_HIDE ;
            //ShowWindow (hwnd, SW_HIDE) ;
            ShowWindow (hwnd, SW_MINIMIZE) ;
            SendMessage (hwndWindow [HWND_CLIENT], WM_MDINEXT, (LPARAM) NULL, 0) ;
            return 0 ;

        case WM_DESTROY :
            TIMERCMD_Reset (hwnd) ;

            DeleteObject ((HBRUSH) SetClassLong (hwnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush (WHITE_BRUSH))) ;
            return 0 ;
        }
    return DefMDIChildProc (hwnd, iMsg, wParam, lParam) ;
    }

LRESULT CALLBACK TelnetTextSubclass (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    CHARRANGE sel ;
    HMENU hMenu, hSubMenu ;
    POINT pt ;
    int nI, nJ ;
    char cTmp [THIEF_COMMAND_SIZE + 10] ;

    switch (iMsg)
        {	
        case WM_KEYDOWN :
            if ((GetKeyState (VK_CONTROL) & ~1))
                {
                switch (wParam)
                    {
                    case VK_PRIOR :
                        SendMessage (hwnd, EM_LINESCROLL, 0, -999999) ;
                        return 0 ;

                    case VK_NEXT :
                        sel.cpMin = 999999 ;
                        sel.cpMax = 999999 ;
                        SendMessage (hwnd, EM_EXSETSEL, 0, (LPARAM) &sel);
                        SendMessage (hwnd, EM_SCROLLCARET, 0, 0) ;
                        return 0 ;

                    default :
                        if (GetKeyState (VK_CONTROL) & 0x80)
                            {
                            if ((wParam >= 'a') && (wParam <= 'z'))
                                {
                                if (CONTROLKEY_Command ((int) (wParam - 'a')))
                                    {
                                    return 0 ;
                                    }
                                }
                            else
                            if ((wParam >= 'A') && (wParam <= 'Z'))
                                {
                                if (CONTROLKEY_Command ((int) (wParam - 'A')))
                                    {
                                    return 0 ;
                                    }
                                }
                            else
                            if ((wParam >= '0') && (wParam <= '9'))
                                {
                                if (CONTROLKEY_Command (((int) (wParam - '0')) + 26))
                                    {
                                    return 0 ;
                                    }
                                }
                            }
                        break ;
                    }
                }
            else
                {
                switch (wParam)
                    {
                    case VK_F1 :
                    case VK_F2 :
                    case VK_F3 :
                    case VK_F4 :
                    case VK_F5 :
                    case VK_F6 :
                    case VK_F7 :
                    case VK_F8 :
                    case VK_F9 :
                        if (FKEY_Command (wParam - VK_F1))
                            {
                            return 0 ;
                            }
                        break ;

                    case VK_F11 :
                    case VK_F12 :
                        if (FKEY_Command (wParam - VK_F2))
                            {
                            return 0 ;
                            }
                        break ;
                    }
                }
            break ;

        case WM_CHAR :
            if (wParam == '\t')
                {
                SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                }
            else
                {
                SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                SendMessage (hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
                }
            return 0 ;

        case WM_PASTE :
            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
            return SendMessage (hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;

        case WM_RBUTTONDOWN :
            if (! (GetKeyState (VK_SHIFT) & ~1))
                {
                // move selection here if it was empty
                POINT pt ;

                pt.x = LOWORD (lParam) ;
                pt.y = HIWORD (lParam) ;

                SendMessage (hwnd, EM_EXGETSEL, 0, (LPARAM) &sel) ;

                if (sel.cpMin == sel.cpMax)
                    {
                    sel.cpMin = SendMessage(hwnd, EM_CHARFROMPOS, 0, (LPARAM) &pt);     // doc is wrong
                    sel.cpMax = sel.cpMin ;
                    SendMessage (hwnd, EM_EXSETSEL, 0, (LPARAM) &sel) ;
                    }
                SendMessage (hwnd, EM_HIDESELECTION, FALSE, FALSE) ;
                }
            return 0 ;

        case WM_RBUTTONUP :
            if (GetKeyState (VK_SHIFT) & ~1)
                {
                SendMessage (hwndWindow [HWND_TELNET_TEXT], WM_COMMAND, MAKEWPARAM (IDM_TEXT_QUICKPASTE, 0), 0) ;
                }
            else
                {
				if (LUDENS)
					{
					TOOLBOX_GetTelnetHandle (TRUE) ;
					}
				else
					{
					TOOLBOX_GetTelnetHandle (FALSE) ;
					}

                nJ = 0 ;
                for (nI = 0 ; nI < MAX_RM_ITEM ; nI++)
                    {
                    if (strlen (RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [nI]) == 0)
                        {
                        break ;
                        }

                    if (TOOLBOX_IsSeparator (RIGHT_MOUSE_TELNET_TEXT, nI))
                        {
                        if (nJ == 0)
                            {
                            System.nRightMouseIndex = RIGHT_MOUSE_TELNET_TEXT ;

                            hMenu    = LoadMenu (hInst, "ONEITEMMENU") ;
                            hSubMenu = GetSubMenu (hMenu, 0) ;

                            DeleteMenu (hSubMenu, 0, MF_BYPOSITION) ;
                            }

                        AppendMenu (hSubMenu, MF_SEPARATOR, 0, 0) ;

                        nJ = nJ + 1 ;
                        }
                    else
                    if (TOOLBOX_OKRightMouse (RIGHT_MOUSE_TELNET_TEXT, nI))
                        {
                        if (nJ == 0)
                            {
                            System.nRightMouseIndex = RIGHT_MOUSE_TELNET_TEXT ;

                            hMenu    = LoadMenu (hInst, "ONEITEMMENU") ;
                            hSubMenu = GetSubMenu (hMenu, 0) ;

                            DeleteMenu (hSubMenu, 0, MF_BYPOSITION) ;
                            }

                        if (strchr (RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [nI], '%'))
                            {
                            TOOLBOX_ExpandAllMacro (cTmp, RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [nI], 0) ;
                            AppendMenu (hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), cTmp) ;
                            }
                        else
                            {
                            AppendMenu (hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [nI]) ;
                            }

                        if (FCheck [RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [nI]])
                            {
                            CheckMenuItem (hSubMenu, (WM_USER_RIGHT_MOUSE0 + nI), MF_CHECKED) ;
                            }

                        nJ = nJ + 1 ;
                        }
                    }
                if (nJ > 0)
                    {
                    pt.x = LOWORD (lParam) ;
                    pt.y = HIWORD (lParam) ;
                    TOOLBOX_MenuPopup (hwnd, pt, hMenu, -1) ;
                    }
                }
            return 0 ;

        case WM_LBUTTONDBLCLK :
            if (TOOLBOX_TelnetTextDoubleClickExecute ())
                {
                return 0 ;
                }
            break ;

        case WM_COMMAND :
            switch (LOWORD (wParam))
                {
                case IDM_TEXT_SELECTALL :
                    sel.cpMin = 0  ;
                    sel.cpMax = -1 ; // 999999?
                    SendMessage (hwnd, EM_EXSETSEL, 0, (LPARAM) &sel) ;
                    SetFocus (hwndWindow [HWND_TELNET_TEXT]) ;
                    return 0 ;

                case IDM_TEXT_COPY :
                    SendMessage (hwnd, EM_EXGETSEL, 0, (LPARAM) &sel) ;
                    if (sel.cpMin == sel.cpMax)
                        {
                        TOOLBOX_Beep () ;
                        }
                    else
                        {
                        SendMessage (hwnd, WM_COPY, 0, 0) ;
                        }
                    SetFocus (hwndWindow [HWND_TELNET_TEXT]) ;
                    return 0 ;

                case IDM_TEXT_QUICKPASTE :
                    SendMessage (hwnd, EM_EXGETSEL, 0, (LPARAM) &sel) ;
                    if (sel.cpMin == sel.cpMax)
                        {
                        TOOLBOX_Beep () ;
                        }
                    else
                        {
                        SendMessage (hwnd, WM_COPY, 0, 0) ;
                        SendMessage (hwndWindow [HWND_TELNET_EDIT], WM_PASTE, 0, 0) ;
                        }
                    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                    return 0 ;

                case IDM_TEXT_SAVETOFILE :
                    SendMessage (hwnd, EM_EXGETSEL, 0, (LPARAM) &sel) ;
                    if (sel.cpMin == sel.cpMax)
                        {
                        TOOLBOX_Beep () ;
                        }
                    else
                        {
                        if (! TOOLBOX_SaveTelnetMarkedText ())
                            {
                            TOOLBOX_Beep () ;
                            }
                        }
                    SetFocus (hwndWindow [HWND_TELNET_TEXT]) ;
                    return 0 ;

                case IDM_EDIT_SELECTALL :
                    sel.cpMin = 0  ;
                    sel.cpMax = -1 ; // 999999?
                    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
                    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                    return 0 ;

                case IDM_EDIT_COPY :
                    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
                    if (sel.cpMin == sel.cpMax)
                        {
                        TOOLBOX_Beep () ;
                        }
                    else
                        {
                        SendMessage (hwndWindow [HWND_TELNET_EDIT], WM_COPY, 0, 0) ;
                        }
                    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                    return 0 ;

                case IDM_EDIT_CUT :
                    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
                    if (sel.cpMin == sel.cpMax)
                        {
                        TOOLBOX_Beep () ;
                        }
                    else
                        {
                        SendMessage (hwndWindow [HWND_TELNET_EDIT], WM_CUT, 0, 0) ;
                        }
                    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                    return 0 ;

                case IDM_EDIT_PASTE :
                    if (IsClipboardFormatAvailable (CF_TEXT))
                        {
                        SendMessage (hwndWindow [HWND_TELNET_EDIT], WM_PASTE, 0, 0) ;
                        }
                    else
                        {
                        TOOLBOX_Beep () ;
                        }
                    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                    return 0 ;

				case IDM_TEXT_TELL :
                case WM_USER_RIGHT_MOUSE0 :
                case WM_USER_RIGHT_MOUSE1 :
                case WM_USER_RIGHT_MOUSE2 :
                case WM_USER_RIGHT_MOUSE3 :
                case WM_USER_RIGHT_MOUSE4 :
                case WM_USER_RIGHT_MOUSE5 :
                case WM_USER_RIGHT_MOUSE6 :
                case WM_USER_RIGHT_MOUSE7 :
                case WM_USER_RIGHT_MOUSE8 :
                case WM_USER_RIGHT_MOUSE9 :
                case WM_USER_RIGHT_MOUSE10 :
                case WM_USER_RIGHT_MOUSE11 :
                case WM_USER_RIGHT_MOUSE12 :
                case WM_USER_RIGHT_MOUSE13 :
                case WM_USER_RIGHT_MOUSE14 :
                case WM_USER_RIGHT_MOUSE15 :
                case WM_USER_RIGHT_MOUSE16 :
                case WM_USER_RIGHT_MOUSE17 :
                case WM_USER_RIGHT_MOUSE18 :
                case WM_USER_RIGHT_MOUSE19 :
                case WM_USER_RIGHT_MOUSE20 :
                case WM_USER_RIGHT_MOUSE21 :
                case WM_USER_RIGHT_MOUSE22 :
                case WM_USER_RIGHT_MOUSE23 :
                case WM_USER_RIGHT_MOUSE24 :
                case WM_USER_RIGHT_MOUSE25 :
                case WM_USER_RIGHT_MOUSE26 :
                case WM_USER_RIGHT_MOUSE27 :
                case WM_USER_RIGHT_MOUSE28 :
                case WM_USER_RIGHT_MOUSE29 :
				case WM_USER_RIGHT_MOUSE30 :
                case WM_USER_RIGHT_MOUSE31 :
                case WM_USER_RIGHT_MOUSE32 :
                case WM_USER_RIGHT_MOUSE33 :
                case WM_USER_RIGHT_MOUSE34 :
                case WM_USER_RIGHT_MOUSE35 :
                case WM_USER_RIGHT_MOUSE36 :
                case WM_USER_RIGHT_MOUSE37 :
                case WM_USER_RIGHT_MOUSE38 :
                case WM_USER_RIGHT_MOUSE39 :
                case WM_USER_RIGHT_MOUSE40 :
                case WM_USER_RIGHT_MOUSE41 :
                case WM_USER_RIGHT_MOUSE42 :
                case WM_USER_RIGHT_MOUSE43 :
                case WM_USER_RIGHT_MOUSE44 :
                case WM_USER_RIGHT_MOUSE45 :
                case WM_USER_RIGHT_MOUSE46 :
                case WM_USER_RIGHT_MOUSE47 :
                case WM_USER_RIGHT_MOUSE48 :
                case WM_USER_RIGHT_MOUSE49 :
                case WM_USER_RIGHT_MOUSE50 :
                    RIGHTMOUSE_Command (System.nRightMouseIndex, (LOWORD (wParam) - WM_USER_RIGHT_MOUSE0), User.bTelnetTextCommandAddHist) ;
                    return 0 ;
                }
            return 0 ;
        }
    return (*WndProcText) (hwnd, iMsg, wParam, lParam) ;
    }

LRESULT CALLBACK TelnetEditSubclass (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    HMENU hMenu, hSubMenu ;
    POINT pt ;
    int nI, nJ ;
    char cTmp [THIEF_COMMAND_SIZE + 10] ;

    switch (iMsg)
        {
        case WM_KEYDOWN :
            switch (wParam)
                {
                case VK_UP :
                    if (User.bUseArrowKeyMoveButton)
                        {
                        if (GetKeyState(VK_SHIFT) < 0)
                            {
                            if (! TOOLBOX_ShiftArrowKey (0))
                                {
                                HISTORY_Previous () ;
                                }
                            }
                        else
                            {
                            HISTORY_Previous () ;
                            }
                        }
                    else
                        {
                        HISTORY_Previous () ;
                        }
                    return 0 ;

                case VK_LEFT :
                    if (User.bUseArrowKeyMoveButton)
                        {
                        if (GetKeyState(VK_SHIFT) < 0)
                            {
                            if (TOOLBOX_ShiftArrowKey (1))
                                {
                                return 0 ;
                                }
                            }
                        }
                    break ;

                case VK_RIGHT :
                    if (User.bUseArrowKeyMoveButton)
                        {
                        if (GetKeyState(VK_SHIFT) < 0)
                            {
                            if (TOOLBOX_ShiftArrowKey (2))
                                {
                                return 0 ;
                                }
                            }
                        }
                    break ;

                case VK_DOWN :
                    if (User.bUseArrowKeyMoveButton)
                        {
                        if (GetKeyState(VK_SHIFT) < 0)
                            {
                            if (! TOOLBOX_ShiftArrowKey (3))
                                {
                                HISTORY_Next () ;
                                }
                            }
                        else
                            {
                            HISTORY_Next () ;
                            }
                        }
                    else
                        {
                        HISTORY_Next () ;
                        }
                    return 0 ;

                case VK_PRIOR :
                case VK_NEXT :
                    SendMessage (hwndWindow [HWND_TELNET_TEXT], iMsg, wParam, lParam) ;
                    return 0 ;

                case VK_F1 :
                case VK_F2 :
                case VK_F3 :
                case VK_F4 :
                case VK_F5 :
                case VK_F6 :
                case VK_F7 :
                case VK_F8 :
                case VK_F9 :
                    if (FKEY_Command (wParam - VK_F1))
                        {
                        return 0 ;
                        }
                    break ;

                case VK_F11 :
                case VK_F12 :
                    if (FKEY_Command (wParam - VK_F2))
                        {
                        return 0 ;
                        }
                    break ;

                default :
                    if (GetKeyState (VK_CONTROL) & 0x80)
                        {
                        if (wParam == '\r')
                            {
                            TELNET_EditEnter () ;
                            return 0 ;
                            }

                        if ((wParam >= 'a') && (wParam <= 'z'))
                            {
                            if (CONTROLKEY_Command ((int) (wParam - 'a')))
                                {
                                return 0 ;
                                }
                            }

                        if ((wParam >= 'A') && (wParam <= 'Z'))
                            {
                            if (CONTROLKEY_Command ((int) (wParam - 'A')))
                                {
                                return 0 ;
                                }
                            }

                        if ((wParam >= '0') && (wParam <= '9'))
                            {
                            if (CONTROLKEY_Command (((int) (wParam - '0')) + 26))
                                {
                                return 0 ;
                                }
                            }

                        if (wParam == 'v' || wParam == 'V')
                            {
                            SendMessage (hwnd, WM_PASTE, 0, 0) ;
                            return 0 ;
                            }

                        if (wParam == 'x' || wParam == 'X')
                            {
                            (*WndProcEdit) (hwnd, iMsg, wParam, lParam) ;
                            TELNET_SetEditFont () ;
                            return 0 ;
                            }
                        }
                    break ;
                }
            break ;

        case WM_CHAR :
            switch (wParam)
                {
                case '\r' :     // enter key
                    TELNET_EditEnter () ;
                    return 0 ;

                case '\033' :   // escape key
                    if (User.bEscCancelTruePremove)
                        {
                        if (Premove.nPremoveCount > 0)
                            {
                            BOARD_ResetTruePremove () ;
                            TOOLBOX_WriteICS (ICS_REFRESH_COMMAND) ;
                            }
                        }

                    if (User.bEscCancelSitDropMove)
                        {
                        F8KEY_Init () ;
                        }

                    if (User.bEscCancelPromoteKnight)
                        {
                        TOOLBOX_ResetPromotKnight () ;
                        }

                    if (DragInfo.nPc != EMPTY_SQUARE)
                        {
                        if (User.bEscCancelDragMove)
                            {
                            PostMessage (Game [INDEX_PLAY].hwnd, WM_USER_MOUSELEAVE, 0, 0) ;

                            for (nI = 1 ; nI < MAX_GAME ; nI++)
                                {
                                if (Game [nI].nGameNumber != 0)
                                    {
                                    if (Login.nLoginType == SERVER_ICC)
                                        {
                                        if (strlen (Vars.cPartner) > 0)
                                            {
                                            if ((stricmp (Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
                                                (stricmp (Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
                                                {
                                                PostMessage (Game [nI].hwnd, WM_USER_MOUSELEAVE, 0, 0) ;
                                                break ;
                                                }
                                            }
                                        }
                                    else
                                        {
                                        if (Game [nI].nGameNumber == Game [INDEX_PLAY].nGamePartner)
                                            {
                                            PostMessage (Game [nI].hwnd, WM_USER_MOUSELEAVE, 0, 0) ;
                                            break ;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    else
                        {
                        SetWindowText (hwndWindow [HWND_TELNET_EDIT], "") ;
                        }
                    return 0 ;

                default :
                    if (wParam > 127)
                        {
                        return 0 ;  // watch out for non ASCII characters
                        }
                    break ;
                }
            break ;

        case WM_PASTE :
            TOOLBOX_AdjustClipboardData (hwnd) ;
            (*WndProcEdit) (hwnd, iMsg, wParam, lParam) ;
            TELNET_SetEditFont () ;
            return 0 ;

        case WM_RBUTTONUP :
            if (GetKeyState (VK_SHIFT) & ~1)
                {
                SendMessage (hwndWindow [HWND_TELNET_TEXT], WM_COMMAND, MAKEWPARAM (IDM_TEXT_QUICKPASTE, 0), 0) ;
                }
            else
                {
                nJ = 0 ;
                for (nI = 0 ; nI < MAX_RM_ITEM ; nI++)
                    {
                    if (strlen (RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [nI]) == 0)
                        {
                        break ;
                        }

                    if (TOOLBOX_IsSeparator (RIGHT_MOUSE_TELNET_EDIT, nI))
                        {
                        if (nJ == 0)
                            {
                            System.nRightMouseIndex = RIGHT_MOUSE_TELNET_EDIT ;

                            hMenu    = LoadMenu (hInst, "ONEITEMMENU") ;
                            hSubMenu = GetSubMenu (hMenu, 0) ;

                            DeleteMenu (hSubMenu, 0, MF_BYPOSITION) ;
                            }

                        AppendMenu (hSubMenu, MF_SEPARATOR, 0, 0) ;

                        nJ = nJ + 1 ;
                        }
                    else
                    if (TOOLBOX_OKRightMouse (RIGHT_MOUSE_TELNET_EDIT, nI))
                        {
                        if (nJ == 0)
                            {
                            System.nRightMouseIndex = RIGHT_MOUSE_TELNET_EDIT ;

                            hMenu    = LoadMenu (hInst, "ONEITEMMENU") ;
                            hSubMenu = GetSubMenu (hMenu, 0) ;

                            DeleteMenu (hSubMenu, 0, MF_BYPOSITION) ;
                            }

                        if (strchr (RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [nI], '%'))
                            {
                            TOOLBOX_ExpandAllMacro (cTmp, RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [nI], 0) ;
                            AppendMenu (hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), cTmp) ;
                            }
                        else
                            {
                            AppendMenu (hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [nI]) ;
                            }

                        if (FCheck [RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType [nI]])
                            {
                            CheckMenuItem (hSubMenu, (WM_USER_RIGHT_MOUSE0 + nI), MF_CHECKED) ;
                            }

                        nJ = nJ + 1 ;
                        }
                    }
                if (nJ > 0)
                    {
                    pt.x = LOWORD (lParam) ;
                    pt.y = HIWORD (lParam) ;
                    TOOLBOX_MenuPopup (hwnd, pt, hMenu, -1) ;
                    }
                }
            return 0 ;

        case WM_COMMAND :
            switch (wParam)
                {
                case WM_USER_RIGHT_MOUSE0 :
                case WM_USER_RIGHT_MOUSE1 :
                case WM_USER_RIGHT_MOUSE2 :
                case WM_USER_RIGHT_MOUSE3 :
                case WM_USER_RIGHT_MOUSE4 :
                case WM_USER_RIGHT_MOUSE5 :
                case WM_USER_RIGHT_MOUSE6 :
                case WM_USER_RIGHT_MOUSE7 :
                case WM_USER_RIGHT_MOUSE8 :
                case WM_USER_RIGHT_MOUSE9 :
                case WM_USER_RIGHT_MOUSE10 :
                case WM_USER_RIGHT_MOUSE11 :
                case WM_USER_RIGHT_MOUSE12 :
                case WM_USER_RIGHT_MOUSE13 :
                case WM_USER_RIGHT_MOUSE14 :
                case WM_USER_RIGHT_MOUSE15 :
                case WM_USER_RIGHT_MOUSE16 :
                case WM_USER_RIGHT_MOUSE17 :
                case WM_USER_RIGHT_MOUSE18 :
                case WM_USER_RIGHT_MOUSE19 :
                case WM_USER_RIGHT_MOUSE20 :
                case WM_USER_RIGHT_MOUSE21 :
                case WM_USER_RIGHT_MOUSE22 :
                case WM_USER_RIGHT_MOUSE23 :
                case WM_USER_RIGHT_MOUSE24 :
                case WM_USER_RIGHT_MOUSE25 :
                case WM_USER_RIGHT_MOUSE26 :
                case WM_USER_RIGHT_MOUSE27 :
                case WM_USER_RIGHT_MOUSE28 :
                case WM_USER_RIGHT_MOUSE29 :
                case WM_USER_RIGHT_MOUSE30 :
                case WM_USER_RIGHT_MOUSE31 :
                case WM_USER_RIGHT_MOUSE32 :
                case WM_USER_RIGHT_MOUSE33 :
                case WM_USER_RIGHT_MOUSE34 :
                case WM_USER_RIGHT_MOUSE35 :
                case WM_USER_RIGHT_MOUSE36 :
                case WM_USER_RIGHT_MOUSE37 :
                case WM_USER_RIGHT_MOUSE38 :
                case WM_USER_RIGHT_MOUSE39 :
                case WM_USER_RIGHT_MOUSE40 :
                case WM_USER_RIGHT_MOUSE41 :
                case WM_USER_RIGHT_MOUSE42 :
                case WM_USER_RIGHT_MOUSE43 :
                case WM_USER_RIGHT_MOUSE44 :
                case WM_USER_RIGHT_MOUSE45 :
                case WM_USER_RIGHT_MOUSE46 :
                case WM_USER_RIGHT_MOUSE47 :
                case WM_USER_RIGHT_MOUSE48 :
                case WM_USER_RIGHT_MOUSE49 :
                case WM_USER_RIGHT_MOUSE50 :
                    RIGHTMOUSE_Command (System.nRightMouseIndex, (LOWORD (wParam) - WM_USER_RIGHT_MOUSE0), User.bTelnetEditCommandAddHist) ;
                    break ;
                }
            return 0 ;

        default :
            if (User.bMouseWheel)
                {
                if (iMsg == WM_MOUSEWHEEL)
                    {
                    PostMessage (hwndWindow [HWND_TELNET_TEXT], iMsg, wParam, lParam) ;
                    return 0 ;
                    }
                }
            break ;
        }
    return (*WndProcEdit) (hwnd, iMsg, wParam, lParam) ;
    }

void TELNET_Colorize (COLORREF clr)
    {
    Telnet.cfTelnet.dwMask      = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT ;
    Telnet.cfTelnet.crTextColor = clr ;
    Telnet.cfTelnet.dwEffects   = 0 ;

    if (hfFont [FONT_TELNET].bBold)
        {
        Telnet.cfTelnet.dwEffects |= CFE_BOLD ;
        }

    if (hfFont [FONT_TELNET].bItalic)
        {
        Telnet.cfTelnet.dwEffects |= CFE_ITALIC ;
        }

    if (hfFont [FONT_TELNET].bUnderLine)
        {
	Telnet.cfTelnet.dwEffects |= CFE_UNDERLINE ;
        }

    if (hfFont [FONT_TELNET].bStrikeOut)
        {
        Telnet.cfTelnet.dwEffects |= CFE_STRIKEOUT ;
        }
    }

void TELNET_SetEditColor (void)
    {
    CHARFORMAT cf ;

    cf.cbSize      = sizeof (CHARFORMAT) ;
    cf.dwMask      = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT ;
    cf.crTextColor = clrColor [CLR_TELNET_NORMAL] ;
    cf.dwEffects   = 0 ;

    if (hfFont [FONT_TELNET].bBold)
        {
        cf.dwEffects |= CFE_BOLD ;
        }

    if (hfFont [FONT_TELNET].bItalic)
        {
        cf.dwEffects |= CFE_ITALIC ;
        }

    if (hfFont [FONT_TELNET].bUnderLine)
        {
        cf.dwEffects |= CFE_UNDERLINE ;
        }

    if (hfFont [FONT_TELNET].bStrikeOut)
        {
        cf.dwEffects |= CFE_STRIKEOUT ;
        }

    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf) ;
    }

void TELNET_SetFont (void)
    {
    CHARFORMAT cfmt ;
    CHARRANGE  tmpsel, sel ;
    PARAFORMAT paraf ;

    cfmt.cbSize = sizeof (CHARFORMAT) ;
    cfmt.dwMask = CFM_FACE | CFM_SIZE | CFM_CHARSET ;
    strcpy (cfmt.szFaceName, hfFont [FONT_TELNET].Name) ;

    //
    // the 20.0 below is a magic number that is totally undocumented.
    // i deduced it by looking at the WordPad source code in the
    // MSVC++ samples directory.
    //
    cfmt.yHeight         = (int) (hfFont [FONT_TELNET].nPointSize * 20.0 + 0.5) ;
    cfmt.bCharSet        = hfFont [FONT_TELNET].lf.lfCharSet ;
    cfmt.bPitchAndFamily = hfFont [FONT_TELNET].lf.lfPitchAndFamily ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfmt) ;
    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfmt) ;

    // why are the following seemingly needed too?
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) &cfmt) ;
    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) &cfmt) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;

    tmpsel.cpMin = 0 ;
    tmpsel.cpMax = -1 ;     // 999999?

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &tmpsel) ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cfmt) ;

    //
    // trying putting this here too.  It still seems to tickle a RichEdit
    // bug: sometimes RichEdit indents the first line of a paragraph too.
    //
    paraf.cbSize        = sizeof (paraf) ;
    paraf.dwMask        = PFM_OFFSET | PFM_STARTINDENT ;
    paraf.dxStartIndent = 0 ;
    paraf.dxOffset      = WRAP_INDENT ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETPARAFORMAT, 0, (LPARAM) &paraf) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;

    TELNET_Colorize (clrColor [CLR_TELNET_NORMAL]) ;
    TELNET_SetEditColor () ;
    }

void TELNET_SetEditFont (void)
    {
    CHARFORMAT cfmt ;

    cfmt.cbSize = sizeof (CHARFORMAT) ;
    cfmt.dwMask = CFM_FACE | CFM_SIZE | CFM_CHARSET ;
    strcpy (cfmt.szFaceName, hfFont [FONT_TELNET].Name) ;

    //
    // the 20.0 below is a magic number that is totally undocumented.
    // i deduced it by looking at the WordPad source code in the
    // MSVC++ samples directory.
    //
    cfmt.yHeight         = (int) (hfFont [FONT_TELNET].nPointSize * 20.0 + 0.5) ;
    cfmt.bCharSet        = hfFont [FONT_TELNET].lf.lfCharSet ;
    cfmt.bPitchAndFamily = hfFont [FONT_TELNET].lf.lfPitchAndFamily ;

    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfmt) ;

    // why are the following seemingly needed too?
    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) &cfmt) ;

    TELNET_Colorize (clrColor [CLR_TELNET_NORMAL]) ;
    TELNET_SetEditColor () ;
    }

void TELNET_Clear (void)
    {
    SetWindowText (hwndWindow [HWND_TELNET_TEXT], "") ;
    SetWindowText (hwndWindow [HWND_TELNET_EDIT], "") ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrColor [CLR_TELNET_BACKGROUND]) ;
    SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrColor [CLR_TELNET_BACKGROUND]) ;

    TELNET_Colorize (clrColor [CLR_TELNET_NORMAL]) ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &Telnet.cfTelnet) ;

    TELNET_SetFont () ;

    Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;

    ShowWindow (hwndWindow [HWND_TELNET_TEXT], SW_SHOW) ;
    ShowWindow (hwndWindow [HWND_TELNET_EDIT], SW_SHOW) ;

    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

void TELNET_GoBottom (void)
    {
    CHARRANGE sel ;

    sel.cpMin = 999999 ;
    sel.cpMax = 999999 ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SCROLLCARET, 0, 0) ;
    }

void TELNET_ClearScreen (void)
    {

    TELNET_GoBottom();

	// EM_LINESCROLL does NOT work as documented, so we scroll up a page first before scrolling down
	SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SCROLL,SB_PAGEUP,0) ;
	SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_LINESCROLL, 0, 999999) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SCROLLCARET, 0, 0) ;

    }

void TELNET_GoBottomFillText (void)
    {
    CHARRANGE sel ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_LINESCROLL, 0, -999999) ;

    sel.cpMin = 999999 ;
    sel.cpMax = 999999 ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SCROLLCARET, 0, 0) ;
    }

void TELNET_Write (char *cD)
	{
	char cT[20],cTime[20];
	int nLength;
	
	nLength = strlen (cD) ;
	//insert time stamp
	if (nLength > 0)
		{
	  	if (User.bTimeStampTelnet)
			{
			if ((cD [nLength - 1] == '\n') && (cD[0] != '\\. '))
				{
				strcpy(cT,"[");
				_strtime (cTime);
				strncat(cT,cTime,5);
				strcat(cT,"] ");
				TELNET_Write1 (cT);
				}
			}
		}
	TELNET_Write1 (cD);
	}

void TELNET_Write1 (char *cData)
    {
    char *cP, *cQ, *cR ;
    int nC, nI ;

    if (User.bShowUnderline)
        {
        if (Login.nLoginType == SERVER_ICC)
            {
            if (Telnet.bMissinghttpwww)
                {
                if (cData [0] == '\\')
                    {
                    cP = cData ;
                    cQ = Telnet.cMarkBuffer ;
                    nC = 0 ;
                    while (*cP != NULL_CHAR)
                        {
                        if ((*cP == ' ') || (*cP == '(') ||
                            (*cP == ')') || (*cP == '"') ||
                            (*cP == '[') || (*cP == ']'))
                            {
                            *cQ++ = *cP++ ;

                            nC++ ;
                            }
                        else
                            {
                            if (cP == cData)
                                {
                                *cQ++ = *cP++ ;
                                }
                            else
                                {
                                break ;
                                }
                            }
                        }

                    if ((*cP == NULL_CHAR) || (nC > 3))
                        {
                        Telnet.bMissinghttpwww = 0 ;
                        cR = cData ;
                        }
                    else
                        {
                        if (strchr (cP, ' ') || strchr (cP, '(') ||
                            strchr (cP, ')') || strchr (cP, '"') ||
                            strchr (cP, '[') || strchr (cP, ']'))
                            {
                            while ((*cP != ' ') && (*cP != '(') &&
                                   (*cP != ')') && (*cP != '"') &&
                                   (*cP != '[') && (*cP != ']'))
                                {
                                *cQ++ = *cP++ ;
                                }
                            *cQ = NULL_CHAR ;
                            TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;

                            Telnet.bMissinghttpwww = 0 ;
                            cR = cP ;
                            }
                        else
                            {
                            TELNET_UnderlineWrite (cData) ;
                            Telnet.bMissinghttpwww = 1 ;
                            return ;
                            }
                        }
                    }
                else
                if (Telnet.bMissinghttpwww == 1)
                    {
                    if ((cData [0] == '\n') && (cData [1] == NULL_CHAR))
                        {
                        TELNET_NormalWrite (cData) ;
                        Telnet.bMissinghttpwww = 2 ;
                        return ;
                        }
                    else
                        {
                        Telnet.bMissinghttpwww = 0 ;
                        cR = cData ;
                        }
                    }
                else
                if (Telnet.bMissinghttpwww == 2)
                    {
                    if ((cData [0] == ' ') && (cData [1] == ' '))
                        {
                        cP = cData ;
                        cQ = Telnet.cMarkBuffer ;
                        nC = 0 ;
                        while (*cP != NULL_CHAR)
                            {
                            if (*cP == ' ')
                                {
                                *cQ++ = *cP++ ;

                                nC++ ;
                                }
                            else
                                {
                                break ;
                                }
                            }

                        if ((*cP == NULL_CHAR) || (nC > 2))
                            {
                            Telnet.bMissinghttpwww = 0 ;
                            cR = cData ;
                            }
                        else
                            {
                            if (strchr (cP, ' ') || strchr (cP, '(') ||
                                strchr (cP, ')') || strchr (cP, '"') ||
                                strchr (cP, '[') || strchr (cP, ']'))
                                {
                                while ((*cP != ' ') && (*cP != '(') &&
                                       (*cP != ')') && (*cP != '"') &&
                                       (*cP != '[') && (*cP != ']'))
                                    {
                                    *cQ++ = *cP++ ;
                                    }
                                *cQ = NULL_CHAR ;
                                TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;

                                Telnet.bMissinghttpwww = 0 ;
                                cR = cP ;
                                }
                            else
                                {
                                TELNET_UnderlineWrite (cData) ;
                                Telnet.bMissinghttpwww = 1 ;
                                return ;
                                }
                            }
                        }
                    else
                        {
                        Telnet.bMissinghttpwww = 0 ;
                        cR = cData ;
                        }
                    }
                else
                    {
                    Telnet.bMissinghttpwww = 0 ;
                    cR = cData ;
                    }
                }
            else
                {
                cR = cData ;
                }
            }
        else
            {
            if (Telnet.bMissinghttpwww)
                {
                if (cData [0] == '\\')
                    {
                    cP = cData ;
                    cQ = Telnet.cMarkBuffer ;
                    nC = 0 ;
                    while (*cP != NULL_CHAR)
                        {
                        if ((*cP == ' ') || (*cP == '(') ||
                            (*cP == ')') || (*cP == '"') ||
                            (*cP == '[') || (*cP == ']'))
                            {
                            *cQ++ = *cP++ ;

                            nC++ ;
                            }
                        else
                            {
                            if (cP == cData)
                                {
                                *cQ++ = *cP++ ;
                                }
                            else
                                {
                                break ;
                                }
                            }
                        }

                    if ((*cP == NULL_CHAR) || (nC > 3))
                        {
                        Telnet.bMissinghttpwww = 0 ;
                        cR = cData ;
                        }
                    else
                        {
                        if (strchr (cP, ' ') || strchr (cP, '(') ||
                            strchr (cP, ')') || strchr (cP, '"') ||
                            strchr (cP, '[') || strchr (cP, ']'))
                            {
                            while ((*cP != ' ') && (*cP != '(') &&
                                   (*cP != ')') && (*cP != '"') &&
                                   (*cP != '[') && (*cP != ']'))
                                {
                                *cQ++ = *cP++ ;
                                }
                            *cQ = NULL_CHAR ;
                            TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;

                            Telnet.bMissinghttpwww = 0 ;
                            cR = cP ;
                            }
                        else
                            {
                            TELNET_UnderlineWrite (cData) ;
                            Telnet.bMissinghttpwww = 1 ;
                            return ;
                            }
                        }
                    }
                else
                    {
                    Telnet.bMissinghttpwww = 0 ;
                    cR = cData ;
                    }
                }
            else
                {
                cR = cData ;
                }
            }

        if (strstr (cR, "http://"))
            {
            Telnet.bMissinghttpwww = 0 ;

            cP = cR ;

            do  {
                cQ = Telnet.cMarkBuffer ;
                while (strncmp (cP, "http://", 7))
                    {
                    *cQ++ = *cP++ ;
                    }
                *cQ = NULL_CHAR ;
                TELNET_NormalWrite (Telnet.cMarkBuffer) ;

                cQ = Telnet.cMarkBuffer ;
                while (*cP != NULL_CHAR)
                    {
                    if ((*cP == ' ') || (*cP == '(') ||
                        (*cP == ')') || (*cP == '"') ||
                        (*cP == '[') || (*cP == ']'))
                        {
                        break ;
                        }
                    else
                        {
                        *cQ++ = *cP++ ;
                        }
                    }
                *cQ = NULL_CHAR ;
                TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;

                if (*cP == NULL_CHAR)
                    {
                    Telnet.bMissinghttpwww = 1 ;
                    break ;
                    }

                if (*cP == ' ')
                    {
                    TELNET_NormalWrite (" ") ;
                    }
                else
                if (*cP == '(')
                    {
                    TELNET_NormalWrite ("(") ;
                    }
                else
                if (*cP == ')')
                    {
                    TELNET_NormalWrite (")") ;
                    }
                else
                if (*cP == '"')
                    {
                    TELNET_NormalWrite ("\"") ;
                    }
                else
                if (*cP == '[')
                    {
                    TELNET_NormalWrite ("[") ;
                    }
                else
                if (*cP == ']')
                    {
                    TELNET_NormalWrite ("]") ;
                    }

                cP++ ;
                }
            while (strstr (cP, "http://")) ;

            if (*cP != NULL_CHAR)
                {
                TELNET_NormalWrite (cP) ;
                }
            return ;
            }

        if (strstr (cR, "www."))
            {
            Telnet.bMissinghttpwww = 0 ;

            cP = cR ;

            do  {
                cQ = Telnet.cMarkBuffer ;
                while (strncmp (cP, "www.", 4))
                    {
                    *cQ++ = *cP++ ;
                    }
                *cQ = NULL_CHAR ;
                TELNET_NormalWrite (Telnet.cMarkBuffer) ;

                cQ = Telnet.cMarkBuffer ;
                while (*cP != NULL_CHAR)
                    {
                    if ((*cP == ' ') || (*cP == '(') ||
                        (*cP == ')') || (*cP == '"') ||
                        (*cP == '[') || (*cP == ']'))
                        {
                        break ;
                        }
                    else
                        {
                        *cQ++ = *cP++ ;
                        }
                    }
                *cQ = NULL_CHAR ;
                TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;

                if (*cP == NULL_CHAR)
                    {
                    Telnet.bMissinghttpwww = 1 ;
                    break ;
                    }

                if (*cP == ' ')
                    {
                    TELNET_NormalWrite (" ") ;
                    }
                else
                if (*cP == '(')
                    {
                    TELNET_NormalWrite ("(") ;
                    }
                else
                if (*cP == ')')
                    {
                    TELNET_NormalWrite (")") ;
                    }
                else
                if (*cP == '"')
                    {
                    TELNET_NormalWrite ("\"") ;
                    }
                else
                if (*cP == '[')
                    {
                    TELNET_NormalWrite ("[") ;
                    }
                else
                if (*cP == ']')
                    {
                    TELNET_NormalWrite ("]") ;
                    }

                cP++ ;
                }
            while (strstr (cP, "www.")) ;

            if (*cP != NULL_CHAR)
                {
                TELNET_NormalWrite (cP) ;
                }
            return ;
            }

        if (strchr (cR, '"'))
            {
            cP = cR ;

            do  {
                cQ = Telnet.cMarkBuffer ;
                while (*cP != '"')
                    {
                    *cQ++ = *cP++ ;
                    }

                if (Telnet.bMissingQuote && (cR [0] == '\\'))
                    {
                    *cQ = NULL_CHAR ;
                    TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;
                    Telnet.bMissingQuote = 0 ;
                    }
                else
                    {
                    *cQ++ = *cP++ ;
                    *cQ = NULL_CHAR ;
                    TELNET_NormalWrite (Telnet.cMarkBuffer) ;

                    cQ = Telnet.cMarkBuffer ;
                    while (*cP != NULL_CHAR)
                        {
                        if (*cP == '"')
                            {
                            break ;
                            }
                        else
                            {
                            *cQ++ = *cP++ ;
                            }
                        }
                    *cQ = NULL_CHAR ;

                    nC = 0 ;
                    for (nI = 0 ; nI < MAX_UNDERLINE_STRING ; nI++)
                        {
                        if (! strncmp (Telnet.cMarkBuffer, TelnetUnderLine [nI], TelnetUnderSize [nI]))
                            {
                            nC = 1 ;
                            break ;
                            }
                        }

                    if (nC)
                        {
                        TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;

                        if (*cP == NULL_CHAR)
                            {
                            Telnet.bMissingQuote = 1 ;
                            break ;
                            }
                        }
                    else
                        {
                        TELNET_NormalWrite (Telnet.cMarkBuffer) ;

                        if (*cP == NULL_CHAR)
                            {
                            Telnet.bMissingQuote = 0 ;
                            break ;
                            }
                        }
                    }

                if (*cP == '"')
                    {
                    TELNET_NormalWrite ("\"") ;
                    }

                cP++ ;
                }
            while (strchr (cP, '"')) ;

            if (*cP != NULL_CHAR)
                {
                TELNET_NormalWrite (cP) ;
                }
            return ;
            }

        if (Telnet.bMissingQuote)
            {
            if (cR [0] == '\\')
                {
                TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;
                return ;
                }

            Telnet.bMissingQuote = 0 ;
            }

        if (strchr (cR, '['))
            {
            cP = cR ;

            do  {
                cQ = Telnet.cMarkBuffer ;
                while (*cP != '[') 
                    {
                    *cQ++ = *cP++ ;
                    }

                if (Telnet.bMissingBracket && (cR [0] == '\\'))
                    {
                    *cQ = NULL_CHAR ;
                    TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;
                    Telnet.bMissingBracket = 0 ;
                    }
                else
                    {
                    *cQ++ = *cP++ ;
                    *cQ = NULL_CHAR ;
                    TELNET_NormalWrite (Telnet.cMarkBuffer) ;

                    cQ = Telnet.cMarkBuffer ;
                    while (*cP != NULL_CHAR)
                        {
                        if ((*cP == ']') || (*cP == '['))
                            {
                            break ;
                            }
                        else
                            {
                            *cQ++ = *cP++ ;
                            }
                        }
                    *cQ = NULL_CHAR ;

                    nC = 0 ;
                    for (nI = 0 ; nI < MAX_BRACKET_STRING ; nI++)
                        {
                        if (! strncmp (Telnet.cMarkBuffer, TelnetBracket [nI], TelnetBracketSize [nI]))
                            {
                            nC = 1 ;
                            break ;
                            }
                        }

                    if (nC)
                        {
                        TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;

                        if (*cP == NULL_CHAR)
                            {
                            Telnet.bMissingBracket = 1 ;
                            break ;
                            }
                        }
                    else
                        {
                        TELNET_NormalWrite (Telnet.cMarkBuffer) ;

                        if (*cP == NULL_CHAR)
                            {
                            Telnet.bMissingBracket = 0 ;
                            break ;
                            }
                        }
                    }

                if (*cP == ']')
                    {
                    TELNET_NormalWrite ("]") ;
                    }

                cP++ ;
                }

            while ( strchr (cP, '[')) ;
            if (*cP != NULL_CHAR)
                {
                TELNET_NormalWrite (cP) ;
                }
            return ;
            }

        if (Telnet.bMissingBracket)
            {
            if (cR [0] == '\\')
                {
                TELNET_UnderlineWrite (Telnet.cMarkBuffer) ;
                return ;
                }

            Telnet.bMissingBracket = 0 ;
            }

        TELNET_NormalWrite (cR) ;
        }
    else
        {
        TELNET_NormalWrite (cData) ;
        }
    }

void TELNET_NormalWrite (char *cData)
    {
    int       nLength, nTrim, nExlen, bTrim ;
    char      *cP, *cQ;
    POINT     pEnd ;
    RECT      rc ;
    CHARRANGE savesel, sel ;
	
    nLength = strlen (cData) ;

    if ((nLength > (CO_MAX - 100)) || (nLength == 0))
        {
        return ;
        }

    Telnet.bLastTelnetIsCR      = (cData [nLength - 1] == '\n') ;
    Telnet.cfTelnet.crTextColor = Telnet.clrCurrent ;

    cP = cData ;
    cQ = Telnet.cTelnetBuffer ;
    
    if (User.bLogTelnet)
        {
        LOG_Write (cP) ;
        }
    while (nLength--)
        {
        if (*cP == '\n')
            {
		    *cQ++ = '\r' ;
		    *cQ++ = '\n' ;
            }
        else
        if (*cP == BELL_CHAR)
            {
            cP++ ;
            }
        else
            {
            *cQ++ = *cP++ ;
            }
        }
    *cQ = NULL_CHAR ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, TRUE, FALSE) ;

    // save current selection
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &savesel) ;

    nExlen = GetWindowTextLength (hwndWindow [HWND_TELNET_TEXT]) ;

    // trim existing text if it's too long
    bTrim = (nExlen + (cQ - Telnet.cTelnetBuffer) > CO_MAX) ;
    if (bTrim)
        {
        nTrim = (CO_TRIM > (cQ - Telnet.cTelnetBuffer)) ? CO_TRIM : (cQ - Telnet.cTelnetBuffer) ;

        sel.cpMin = 0 ;
        sel.cpMax = nTrim ;

        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL,   0, (LPARAM) &sel) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_REPLACESEL, 0, (LPARAM) "") ;

        nExlen        -= nTrim ;
        savesel.cpMin -= nTrim ;
        savesel.cpMax -= nTrim ;

        if (nExlen < 0)
            {
            nExlen = 0 ;
            }
        if (savesel.cpMin < 0)
            {
            savesel.cpMin = 0 ;
            }
        if (savesel.cpMax < savesel.cpMin)
            {
            savesel.cpMax = savesel.cpMin ;
            }
        }

    // find out whether current end of text is visible
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETRECT, 0, (LPARAM) &rc) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_POSFROMCHAR, (WPARAM) &pEnd, nExlen) ;

    // append the new text
    sel.cpMin = nExlen ;
    sel.cpMax = nExlen ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &Telnet.cfTelnet) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_REPLACESEL, 0, (LPARAM) Telnet.cTelnetBuffer) ;

    if ((Telnet.bForceVisible) || (nExlen == 0) || (bTrim) ||
        (rc.left <= pEnd.x   &&
         pEnd.x  <  rc.right &&
         rc.top  <= pEnd.y   &&
         pEnd.y  <  rc.bottom))
        {
        //
        // scroll to make new end of text visible if old end of text
        // was visible or new text is an echo of user type in
        //
        sel.cpMin = 999999 ;
        sel.cpMax = 999999 ;

        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, FALSE, FALSE) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SCROLLCARET, 0, 0) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, TRUE, FALSE) ;
        }

    if ((savesel.cpMax == nExlen) || (Telnet.bForceVisible))
        {
        //
        // move insert point to new end of text if it was at the old
        // end of text or if the new text is an echo of user type in
        //
        sel.cpMin = 999999 ;
        sel.cpMax = 999999 ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
        }
    else
        {
        // restore previous selection
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &savesel) ;
        }

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, FALSE, FALSE) ;
    }

void TELNET_ItalicWrite (char *cData)
    {
    int       nLength, nTrim, nExlen, bTrim ;
    char      *cP, *cQ ;
    POINT     pEnd ;
    RECT      rc ;
    CHARRANGE savesel, sel ;
	
    nLength = strlen (cData) ;

    if ((nLength > (CO_MAX - 100)) || (nLength == 0))
        {
        return ;
        }

    Telnet.bLastTelnetIsCR      = (cData [nLength - 1] == '\n') ;
    Telnet.cfTelnet.crTextColor = Telnet.clrCurrent ;

    cP = cData ;
    cQ = Telnet.cTelnetBuffer ;
    
    if (User.bLogTelnet)
        {
        LOG_Write (cP) ;
        }

    while (nLength--)
        {
        if (*cP == '\n')
            {
            *cQ++ = '\r' ;
            *cQ++ = '\n' ;
            }
        else
        if (*cP == BELL_CHAR)
            {
            cP++ ;
            }
        else
            {
            *cQ++ = *cP++ ;
            }
        }

    *cQ = NULL_CHAR ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, TRUE, FALSE) ;

    // save current selection
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &savesel) ;

    nExlen = GetWindowTextLength (hwndWindow [HWND_TELNET_TEXT]) ;

    // trim existing text if it's too long
    bTrim = (nExlen + (cQ - Telnet.cTelnetBuffer) > CO_MAX) ;
    if (bTrim)
        {
        nTrim = (CO_TRIM > (cQ - Telnet.cTelnetBuffer)) ? CO_TRIM : (cQ - Telnet.cTelnetBuffer) ;

        sel.cpMin = 0 ;
        sel.cpMax = nTrim ;

        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL,   0, (LPARAM) &sel) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_REPLACESEL, 0, (LPARAM) "") ;

        nExlen        -= nTrim ;
        savesel.cpMin -= nTrim ;
        savesel.cpMax -= nTrim ;

        if (nExlen < 0)
            {
            nExlen = 0 ;
            }
        if (savesel.cpMin < 0)
            {
            savesel.cpMin = 0 ;
            }
        if (savesel.cpMax < savesel.cpMin)
            {
            savesel.cpMax = savesel.cpMin ;
            }
        }

    // find out whether current end of text is visible
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETRECT, 0, (LPARAM) &rc) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_POSFROMCHAR, (WPARAM) &pEnd, nExlen) ;

    // set to italic
    Telnet.cfItalic.dwMask      = Telnet.cfTelnet.dwMask ;
    Telnet.cfItalic.crTextColor = Telnet.cfTelnet.crTextColor ;
    Telnet.cfItalic.dwEffects   = Telnet.cfTelnet.dwEffects | CFE_ITALIC ;

    // append the new text
    sel.cpMin = nExlen ;
    sel.cpMax = nExlen ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &Telnet.cfItalic) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_REPLACESEL, 0, (LPARAM) Telnet.cTelnetBuffer) ;

    if ((Telnet.bForceVisible) || (nExlen == 0) || (bTrim) ||
        (rc.left <= pEnd.x   &&
         pEnd.x  <  rc.right &&
         rc.top  <= pEnd.y   &&
         pEnd.y  <  rc.bottom))
        {
        //
        // scroll to make new end of text visible if old end of text
        // was visible or new text is an echo of user type in
        //
        sel.cpMin = 999999 ;
        sel.cpMax = 999999 ;

        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, FALSE, FALSE) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SCROLLCARET, 0, 0) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, TRUE, FALSE) ;
        }

    if ((savesel.cpMax == nExlen) || (Telnet.bForceVisible))
        {
        //
        // move insert point to new end of text if it was at the old
        // end of text or if the new text is an echo of user type in
        //
        sel.cpMin = 999999 ;
        sel.cpMax = 999999 ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
        }
    else
        {
        // restore previous selection
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &savesel) ;
        }

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, FALSE, FALSE) ;
    }

void TELNET_UnderlineWrite (char *cData)
    {
    int       nLength, nTrim, nExlen, bTrim ;
    char      *cP, *cQ ;
    POINT     pEnd ;
    RECT      rc ;
    CHARRANGE savesel, sel ;
	
    nLength = strlen (cData) ;

    if ((nLength > (CO_MAX - 100)) || (nLength == 0))
        {
        return ;
        }

    Telnet.bLastTelnetIsCR      = (cData [nLength - 1] == '\n') ;
    Telnet.cfTelnet.crTextColor = Telnet.clrCurrent ;

    cP = cData ;
    cQ = Telnet.cTelnetBuffer ;
    
    if (User.bLogTelnet)
        {
        LOG_Write (cP) ;
        }

    while (nLength--)
        {
        if (*cP == '\n')
            {
	    *cQ++ = '\r' ;
	    *cQ++ = '\n' ;
            }
        else
        if (*cP == BELL_CHAR)
            {
            cP++ ;
            }
        else
            {
            *cQ++ = *cP++ ;
            }
        }

    *cQ = NULL_CHAR ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, TRUE, FALSE) ;

    // save current selection
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &savesel) ;

    nExlen = GetWindowTextLength (hwndWindow [HWND_TELNET_TEXT]) ;

    // trim existing text if it's too long
    bTrim = (nExlen + (cQ - Telnet.cTelnetBuffer) > CO_MAX) ;
    if (bTrim)
        {
        nTrim = (CO_TRIM > (cQ - Telnet.cTelnetBuffer)) ? CO_TRIM : (cQ - Telnet.cTelnetBuffer) ;

        sel.cpMin = 0 ;
        sel.cpMax = nTrim ;

        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL,   0, (LPARAM) &sel) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_REPLACESEL, 0, (LPARAM) "") ;

        nExlen        -= nTrim ;
        savesel.cpMin -= nTrim ;
        savesel.cpMax -= nTrim ;

        if (nExlen < 0)
            {
            nExlen = 0 ;
            }
        if (savesel.cpMin < 0)
            {
            savesel.cpMin = 0 ;
            }
        if (savesel.cpMax < savesel.cpMin)
            {
            savesel.cpMax = savesel.cpMin ;
            }
        }

    // find out whether current end of text is visible
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_GETRECT, 0, (LPARAM) &rc) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_POSFROMCHAR, (WPARAM) &pEnd, nExlen) ;

    // set to underline
    Telnet.cfUnderline.dwMask      = Telnet.cfTelnet.dwMask ;
    Telnet.cfUnderline.crTextColor = Telnet.cfTelnet.crTextColor ;
    Telnet.cfUnderline.dwEffects   = Telnet.cfTelnet.dwEffects | CFE_UNDERLINE ;

    // append the new text
    sel.cpMin = nExlen ;
    sel.cpMax = nExlen ;

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &Telnet.cfUnderline) ;
    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_REPLACESEL, 0, (LPARAM) Telnet.cTelnetBuffer) ;

    if ((Telnet.bForceVisible) || (nExlen == 0) || (bTrim) ||
        (rc.left <= pEnd.x   &&
         pEnd.x  <  rc.right &&
         rc.top  <= pEnd.y   &&
         pEnd.y  <  rc.bottom))
        {
        //
        // scroll to make new end of text visible if old end of text
        // was visible or new text is an echo of user type in
        //
        sel.cpMin = 999999 ;
        sel.cpMax = 999999 ;

        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, FALSE, FALSE) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_SCROLLCARET, 0, 0) ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, TRUE, FALSE) ;
        }

    if ((savesel.cpMax == nExlen) || (Telnet.bForceVisible))
        {
        //
        // move insert point to new end of text if it was at the old
        // end of text or if the new text is an echo of user type in
        //
        sel.cpMin = 999999 ;
        sel.cpMax = 999999 ;
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
        }
    else
        {
        // restore previous selection
        SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &savesel) ;
        }

    SendMessage (hwndWindow [HWND_TELNET_TEXT], EM_HIDESELECTION, FALSE, FALSE) ;
    }

void TELNET_NormalPrint (char *cS)

	{
    Telnet.bDisplayLine  = 1 ;
    Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
    Telnet.nCurrentSound = SOUND_NONE ;
    TELNET_Write (cS) ;
    Telnet.clrLastLine   = Telnet.clrCurrent ;
    }

void TELNET_EditEnter (void)
    {
    int nL,nCheck, bIllChar  ;
    COLORREF nC ;

    F9KEY_Clear () ;

    nL = GetWindowText(hwndWindow [HWND_TELNET_EDIT], Telnet.cEditBuffer, EDIT_LINE_SIZE - 1) ;

    HISTORY_Add (Telnet.cEditBuffer) ;

    TOOLBOX_CheckObserveExam (Telnet.cEditBuffer) ;
    
    nC = Telnet.clrCurrent ;
    Telnet.clrCurrent = clrColor [CLR_TELNET_USERTEXT] ;
    
    if (! Telnet.bLastTelnetIsCR)
        {
        Telnet.bForceVisible = 1 ;
		TELNET_NormalWrite ("\n") ;
        Telnet.bForceVisible = 0 ;
        }
    Telnet.bLastIsTelnetEdit = 1 ;

    bIllChar = 0;
	nCheck = strlen (Telnet.cEditBuffer);
    while (nCheck > 0)
		{
		nCheck--;
        if ((unsigned char)Telnet.cEditBuffer[nCheck] > 127)
			{
			bIllChar = 1;
			break;
			}
		}

    Telnet.cEditBuffer [nL    ] = '\n' ;
    Telnet.cEditBuffer [nL + 1] = NULL_CHAR ;
	Telnet.bForceVisible = 1 ;
	if (bIllChar)
		{
		TOOLBOX_Beep () ;
		TELNET_Write (SERVER_ILLEGALCHAR_MSG) ;
	}else{
		if ((_strnicmp(Telnet.cEditBuffer,"cls",3) == 0) && (strlen (Telnet.cEditBuffer) == 4)){
			TELNET_ClearScreen();
		}else{
			TOOLBOX_WriteICS (Telnet.cEditBuffer) ;		
		} 
	}

	TELNET_Write (Telnet.cEditBuffer) ;

	if (Telnet.cEditBuffer[0]='\\'){	    
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
		Telnet.clrLastLine   = Telnet.clrCurrent ;
	}

    Telnet.bForceVisible = 0 ;

    if (User.bEnterHighlight)
        {
        SendMessage (hwndWindow [HWND_TELNET_EDIT], EM_SETSEL, 0, (LPARAM) nL) ;
        }
    else
        {
        SetWindowText (hwndWindow [HWND_TELNET_EDIT], "") ;
        }
    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    }

int TELNET_MatchExp (char *cS, char *cP)
    {
    int bBracketOpen = 0; 
	
    while (isspace (*cS))
        {
        cS++ ;
        }

    while (1)
        {
        switch (*cP)
            {
            case 0 :
                return 1 ;

            case '$' :
                if (! isalpha (*cS))
                    {
                    return 0 ;
                    }

                while  
                    ((!isspace (*cS) || bBracketOpen) && ! (*cS == NULL_CHAR))
                    {
                    if (*cS == '(' || *cS == '[')
                        {
                        bBracketOpen = 1 ;
                        }

                    if (*cS == ')' || *cS == ']')
                        {
                        bBracketOpen = 0 ;
                        }

                    if (*cS == ':')
                        {
                        return 0 ;
                        }

                    cS++ ;
                    }
                    break ;

            case '@' :
                if (! isalpha (*cS))
                    {
                    return 0 ;
                    }

                while (isalpha (*cS))
                    {
                    cS++ ;
                    }
                break ;

            case '#' :
                if (! isdigit (*cS))
                    {
                    return 0 ;
                    }

                while (isdigit (*cS))
                    {
                    cS++ ;
                    }
                break ;

            default :
                if (*cS++ != *cP)
                    {
                    return 0 ;
                    }
            }
        cP++ ;
        }
    }

int TELNET_IsChannel (char *cS, int *bComp)
    {
    int  nS = 0, nC ;
    char cTmp [10] ;

    *bComp = 0 ;

    while (isspace (*cS))
        {
        cS++ ;
        }

    while (*cS != NULL_CHAR)
        {
        switch (nS)
            {
            case 0 :
                while (isalnum (*cS) || *cS == '-') // on ICC handles can also include numbers or the '-'
                    {
                    cS++ ;
                    }
                nS = 1 ;
                break ;

            case 1 :
                while (isalpha (*cS) || *cS == '(' || *cS == ')' || *cS == '*')
                    {
                    if ((*cS       == '(') &&
                        (*(cS + 1) == 'C') &&
                        (*(cS + 2) == ')'))
                        {
                        *bComp = 1 ;
                        }

                    cS++ ;
                    }

                if (! isdigit (*cS))
                    {
                    return 0 ;
                    }

                nS = 2 ;
                break ;

            case 2 :
                while (isdigit (*cS))
                    {
                    cS++ ;
                    }

                if (*cS == ')' && *(cS + 1) == ':')
                    {
                    Telnet.nCurrentSound = SOUND_NONE ;

                    if (*(cS - 2) == '(')
                        {
                        // one digit channel #
                        cTmp [0] = *(cS - 1) ;
                        cTmp [1] = NULL_CHAR ;

                        nC = atoi (cTmp) ;
                        if (bChannel [nC])
                            {
                            Telnet.clrCurrent = clrChannel [nC] ;
                            return 1 ;
                            }
                        }
                    else
                    if (*(cS - 3) == '(')
                        {
                        // two digit channel #
                        cTmp [0] = *(cS - 2) ;
                        cTmp [1] = *(cS - 1) ;
                        cTmp [2] = NULL_CHAR ;

                        nC = atoi (cTmp) ;
                        if (bChannel [nC])
                            {
                            Telnet.clrCurrent = clrChannel [nC] ;
                            return 1 ;
                            }
                        }
                    else
                    if (*(cS - 4) == '(')
                        {
                        // three digit channel #
                        cTmp [0] = *(cS - 3) ;
                        cTmp [1] = *(cS - 2) ;
                        cTmp [2] = *(cS - 1) ;
                        cTmp [3] = NULL_CHAR ;

                        nC = atoi (cTmp) ;
                        if (nC < MAX_CHANNEL_COLOR)
                            {
                            if (bChannel [nC])
                                {
                                Telnet.clrCurrent = clrChannel [nC] ;
                                return 1 ;
                                }
                            }
                        else
                            {
                            nC = 1000 ;
                            }
                        }
                    else
                        {
                        nC = 1000 ;
                        }

                    if (Telnet.nLastChannel != nC)
                        {
                        Telnet.nLastChannel = nC ;
                        if (Telnet.bLastChannel)
                            {
                            Telnet.bLastChannel = 0 ;
                            }
                        else
                            {
                            Telnet.bLastChannel = 1 ;
                            }
                        }

                    if (Telnet.bLastChannel)
                        {
                        Telnet.clrCurrent = clrColor [CLR_TELNET_CHANNEL] ;
                        }
                    else
                        {
                        Telnet.clrCurrent = clrColor [CLR_TELNET_CHANNELA] ;
                        }
                    return 1 ;
                    }
                return 0 ;
            }
        }
    return 0 ;
    }

void TELNET_CheckPressReturn (char *cS)
    {
    if (TELNET_MatchExp (cS, "(After logging in, do \"help register\" for more info on how to register.)"))
        {
        TOOLBOX_WriteICS ("\n") ;
        }
    }
