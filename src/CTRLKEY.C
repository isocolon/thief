#include "thief.h"

int NewC_Type [MAX_CONTROL_KEY] ;
char NewC_FKey [MAX_CONTROL_KEY] [MAX_CONTROL_KEY_SIZE] ;

BOOL CALLBACK ControlKeyBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static int nSel ;
    static HWND hFunction ;
    static int nKey [MAX_CONTROL_KEY] ;

    int nI, nJ, nF ;
    char c, cTmp [2048] ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            hFunction = GetDlgItem (hwnd, IDD_CONTROL_FUNCTION) ;

            for (nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
                {
                NewC_Type [nI] = ControlKey.nType [nI] ;

                if ((NewC_Type [nI] < 0) || (NewC_Type [nI] >= MAX_FMENU))
                    {
                    NewC_Type [nI] = FUNCTION_NONE ;
                    }

                if (TOOLBOX_NeedCommandValue (NewC_Type [nI]))
                    {
                    strcpy (NewC_FKey [nI], ControlKey.cFKey [nI]) ;
                    }
                else
                    {
                    strcpy (NewC_FKey [nI], "") ;
                    }
                }

            nJ = 0 ;

            SendDlgItemMessage (hwnd, IDD_CONTROL_LIST, LB_RESETCONTENT, 0, 0) ;
            for (nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
                {
                if (nI < 26)
                    {
                    c = (char) (nI + 'a') ;
                    if ((c == 'a') || (c == 'c') || (c == 'v') || (c == 'x'))
                        {
                        nKey [nI] = -1 ;
                        }
                    else
                        {
                        nKey [nJ] = nI ;
                        sprintf (cTmp, "%02d. Control %c", nJ + 1, (char) (nI + 'A')) ;
                        SendDlgItemMessage (hwnd, IDD_CONTROL_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
                        nJ = nJ + 1 ;
                        }
                    }
                else
                    {
                    nKey [nJ] = nI ;
                    sprintf (cTmp, "%02d. Control %c", nJ + 1, (char) ((nI - 26) + '0')) ;
                    SendDlgItemMessage (hwnd, IDD_CONTROL_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
                    nJ = nJ + 1 ;
                    }
                }
            SendDlgItemMessage (hwnd, IDD_CONTROL_LIST, LB_SETCURSEL, 0, 0) ;

            SendMessage (hFunction, CB_RESETCONTENT, 0, 0) ;
            for (nI = 0 ; nI < MAX_FMENU ; nI++)
                {
                strcpy (cTmp, FString [nI]) ;
                SendMessage (hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp) ;
                }

            nSel = nKey [0] ;

            strcpy (cTmp, FString [NewC_Type [nSel]]) ;
            if (SendMessage (hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendDlgItemMessage (hwnd, IDD_CONTROL_COMMAND, WM_SETTEXT, 0, (LPARAM) NewC_FKey [nSel]) ;

            EnableWindow (GetDlgItem (hwnd, IDD_CONTROL_COMMAND), TOOLBOX_NeedCommandValue (NewC_Type [nSel])) ;
            
            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDD_CONTROL_LIST :
                    if (HIWORD (wParam) == LBN_SELCHANGE)
                        {
                        nJ = SendDlgItemMessage (hwnd, IDD_CONTROL_LIST, LB_GETCURSEL, 0, 0) ;
                        nI = nKey [nJ] ;

                        nF = SendMessage (hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                        if (nF == CB_ERR)
                            {
                            NewC_Type [nSel] = FUNCTION_NONE ;
                            }
                        else
                            {
                            NewC_Type [nSel] = nF ;
                            }

                        if (TOOLBOX_NeedCommandValue (NewC_Type [nSel]))
                            {
                            SendDlgItemMessage (hwnd, IDD_CONTROL_COMMAND, WM_GETTEXT, (WPARAM) MAX_CONTROL_KEY_SIZE - 1, (LPARAM) cTmp) ;
                            strcpy (NewC_FKey [nSel], cTmp) ;
                            }
                        else
                            {
                            strcpy (NewC_FKey [nSel], "") ;
                            }

                        strcpy (cTmp, FString [NewC_Type [nI]]) ;
                        if (SendMessage (hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                            {
                            SendMessage (hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                            SendMessage (hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                            }

                        SendDlgItemMessage (hwnd, IDD_CONTROL_COMMAND, WM_SETTEXT, 0, (LPARAM) NewC_FKey [nI]) ;

                        nSel = nI ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CONTROL_COMMAND), TOOLBOX_NeedCommandValue (NewC_Type [nSel])) ;
                        }
                    break ;

                case IDD_CONTROL_FUNCTION :
                    if (HIWORD (wParam) == CBN_SELCHANGE)
                        {
                        nF = SendMessage (hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                        if (nF == CB_ERR)
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CONTROL_COMMAND), 0) ;
                            }
                        else
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CONTROL_COMMAND), TOOLBOX_NeedCommandValue (nF)) ;
                            }
                        }
                    break ;

                case IDOK :
                    nF = SendMessage (hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nF == CB_ERR)
                        {
                        NewC_Type [nSel] = FUNCTION_NONE ;
                        }
                    else
                        {
                        NewC_Type [nSel] = nF ;
                        }

                    if (TOOLBOX_NeedCommandValue (NewC_Type [nSel]))
                        {
                        SendDlgItemMessage (hwnd, IDD_CONTROL_COMMAND, WM_GETTEXT, (WPARAM) MAX_CONTROL_KEY_SIZE - 1, (LPARAM) cTmp) ;
                        strcpy (NewC_FKey [nSel], cTmp) ;
                        }
                    else
                        {
                        strcpy (NewC_FKey [nSel], "") ;
                        }

                    for (nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
                        {
                        ControlKey.nType [nI] = NewC_Type [nI] ;
                        strcpy (ControlKey.cFKey [nI], NewC_FKey [nI]) ;
                        TOOLBOX_AllTrim (ControlKey.cFKey [nI]) ;

                        if (! TOOLBOX_NeedCommandValue (ControlKey.nType [nI]))
                            {
                            strcpy (ControlKey.cFKey [nI], "") ;
                            }
                        }

                    CONTROLKEY_Adjust () ;
                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDCANCEL :
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDD_CONTROL_DEFAULT :
                    for (nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
                        {
                        NewC_Type [nI] = FUNCTION_NONE ;
                        strcpy (NewC_FKey [nI], "") ;
                        }

                    // set CTRL T as maximize telnet console
                    NewC_Type [(int) ('T' - 'A')] = FUNCTION_MAXIMIZE_TELNET ;

                    // set CTRL B as restore telnet console
                    NewC_Type [(int) ('B' - 'A')] = FUNCTION_RESTORE_TELNET ;

                    // set CTRL N as reset board
                    NewC_Type [(int) ('N' - 'A')] = FUNCTION_RESET_BOARD ;

                    // set CTRL F as flip board
                    NewC_Type [(int) ('F' - 'A')] = FUNCTION_FLIP_BOARD ;

                    nJ = SendDlgItemMessage (hwnd, IDD_CONTROL_LIST, LB_GETCURSEL, 0, 0) ;
                    nI = nKey [nJ] ;

                    strcpy (cTmp, FString [NewC_Type [nI]]) ;
                    if (SendMessage (hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    SendDlgItemMessage (hwnd, IDD_CONTROL_COMMAND, WM_SETTEXT, 0, (LPARAM) NewC_FKey [nI]) ;

                    nSel = nI ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CONTROL_COMMAND), TOOLBOX_NeedCommandValue (NewC_Type [nSel])) ;
                    return TRUE ;

                case IDD_CONTROL_DEFAULT1 :
                    for (nI = 27 ; nI <= 34 ; nI++)
                        {
                        NewC_Type [nI] = FUNCTION_NONE ;
                        strcpy (NewC_FKey [nI], "") ;
                        }

                    // set CTRL 1 to 8 to restore layout #1 to #8
                    NewC_Type [27] = FUNCTION_RESTORE_LAYOUT1 ;
                    NewC_Type [28] = FUNCTION_RESTORE_LAYOUT2 ;
                    NewC_Type [29] = FUNCTION_RESTORE_LAYOUT3 ;
                    NewC_Type [30] = FUNCTION_RESTORE_LAYOUT4 ;
                    NewC_Type [31] = FUNCTION_RESTORE_LAYOUT5 ;
                    NewC_Type [32] = FUNCTION_RESTORE_LAYOUT6 ;
                    NewC_Type [33] = FUNCTION_RESTORE_LAYOUT7 ;
                    NewC_Type [34] = FUNCTION_RESTORE_LAYOUT8 ;

                    nJ = SendDlgItemMessage (hwnd, IDD_CONTROL_LIST, LB_GETCURSEL, 0, 0) ;
                    nI = nKey [nJ] ;

                    strcpy (cTmp, FString [NewC_Type [nI]]) ;
                    if (SendMessage (hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    SendDlgItemMessage (hwnd, IDD_CONTROL_COMMAND, WM_SETTEXT, 0, (LPARAM) NewC_FKey [nI]) ;

                    nSel = nI ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CONTROL_COMMAND), TOOLBOX_NeedCommandValue (NewC_Type [nSel])) ;
                    return TRUE ;
                }
            break ;
        }
    return FALSE ;
    }

void CONTROLKEY_Init (void)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
        {
        ControlKey.nType [nI] = FUNCTION_NONE ;
        strcpy (ControlKey.cFKey [nI], "") ;
        }

    // set CTRL T as maximize telnet console
    ControlKey.nType [(int) ('T' - 'A')] = FUNCTION_MAXIMIZE_TELNET ;

    // set CTRL B as restore telnet console
    ControlKey.nType [(int) ('B' - 'A')] = FUNCTION_RESTORE_TELNET ;

    // set CTRL N as reset board
    ControlKey.nType [(int) ('N' - 'A')] = FUNCTION_RESET_BOARD ;

    // set CTRL F as flip board
    ControlKey.nType [(int) ('F' - 'A')] = FUNCTION_FLIP_BOARD ;
    }

void CONTROLKEY_Adjust (void)
    {
    int nI ;
    char c ;

    for (nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
        {
        if ((ControlKey.nType [nI] < 0) || (ControlKey.nType [nI] >= MAX_FMENU))
            {
            ControlKey.nType [nI] = FUNCTION_NONE ;
            }

        c = (char) (nI + 'a') ;
        if ((c == 'a') || (c == 'c') || (c == 'v') || (c == 'x'))
            {
            ControlKey.nType [nI] = FUNCTION_NONE ;
            strcpy (ControlKey.cFKey [nI], "") ;
            }
        else
            {
            if (! TOOLBOX_NeedCommandValue (ControlKey.nType [nI]))
                {
                strcpy (ControlKey.cFKey [nI], "") ;
                }
            }
        }
    }

int CONTROLKEY_Command (int nI)
    {
    char c ;

    if (ControlKey.nType [nI] == FUNCTION_NONE)
        {
        return 0 ;
        }

    c = (char) (nI + 'a') ;
    if ((c == 'a') || (c == 'c') || (c == 'v') || (c == 'x'))
        {
        return 0 ;
        }

    return TOOLBOX_Command (ControlKey.nType [nI], ControlKey.cFKey [nI], User.bCKeyCommandAddHist) ;
    }
