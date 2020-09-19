#include "thief.h"

BOOL CALLBACK RedClockBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static HWND hSecond, hTenth, h1Second ;
    char cTmp [2048] ;
    int  nI, nTmp ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            hSecond  = GetDlgItem (hwnd, IDD_REDCLOCK_1) ;
            hTenth   = GetDlgItem (hwnd, IDD_REDCLOCK_2) ;
            h1Second = GetDlgItem (hwnd, IDD_REDCLOCK_3) ;

            REDCLOCK_Adjust () ;

            sprintf (cTmp, "%d", RedClock.nRedClock) ;
            SendDlgItemMessage (hwnd, IDD_REDCLOCK_1, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp) ;

            SendMessage (hTenth, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Never Display Tenth of a Second") ;
            SendMessage (hTenth, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Always Display Tenth of a Second") ;
            SendMessage (hTenth, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Display Tenth of a Second When Time is Less Than one Minute") ;
            SendMessage (hTenth, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

			SendDlgItemMessage (hwnd, IDD_REDCLOCK_5, BM_SETCHECK, User.bColoredClocks			? BST_CHECKED : BST_UNCHECKED, 0) ;

            switch (RedClock.nShowTenthSecond)
                {
                case TENTH_NEVER :
                    strcpy (cTmp, "Never Display Tenth of a Second") ;
                    break ;

                case TENTH_ALWAYS :
                    strcpy (cTmp, "Always Display Tenth of a Second") ;
                    break ;

                case TENTH_ONE_MINUTE :
                    strcpy (cTmp, "Display Tenth of a Second When Time is Less Than one Minute") ;
                    break ;

                default :
                    RedClock.nShowTenthSecond = TENTH_NEVER ;
                    strcpy (cTmp, "Never Display Tenth of a Second") ;
                    break ;
                }

            if (SendMessage (hTenth, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hTenth, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hTenth, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendMessage (h1Second, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Never Display Tenth of a Second") ;
            SendMessage (h1Second, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Always Display Tenth of a Second") ;
            SendMessage (h1Second, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            if (RedClock.bShowOneSecond)
                {
                strcpy (cTmp, "Always Display Tenth of a Second") ;
                }
            else
                {
                strcpy (cTmp, "Never Display Tenth of a Second") ;
                }

            if (SendMessage (h1Second, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (h1Second, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (h1Second, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDOK :
					User.bColoredClocks = ((SendDlgItemMessage (hwnd, IDD_REDCLOCK_5, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    SendDlgItemMessage (hwnd, IDD_REDCLOCK_1, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp) ;
                    TOOLBOX_AllTrim (cTmp) ;
                    if (strlen (cTmp) == 0)
                        {
                        TOOLBOX_Error ("Missing Second.") ;
                        SetFocus (hSecond) ;
                        return FALSE ;
                        }

                    nTmp = 0 ;
                    for (nI = 0 ; nI < ((int) strlen (cTmp)) ; nI++)
                        {
                        if (nI == 0)
                            {
                            if (! isdigit (cTmp [nI]) && cTmp [nI] != '+' && cTmp [nI] != '-')
                                {
                                nTmp = 1 ;
                                break ;
                                }
                            }
                        else
                            {
                            if (! isdigit (cTmp [nI]))
                                {
                                nTmp = 1 ;
                                break ;
                                }
                            }
                        }
                    if (nTmp)
                        {
                        TOOLBOX_Error ("Invalid Second.") ;
                        SetFocus (hSecond) ;
                        return FALSE ;
                        }

                    nTmp = atoi (cTmp) ;
                    if ((nTmp < -300) || (nTmp > 300))
                        {
                        TOOLBOX_Error ("Second is Out of Range (-300 to 300).") ;
                        SetFocus (hSecond) ;
                        return FALSE ;
                        }

                    RedClock.nRedClock = nTmp ;

                    nI = SendMessage (hTenth, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        RedClock.nShowTenthSecond = TENTH_NEVER ;
                        }
                    else
                    if (nI == 0)
                        {
                        RedClock.nShowTenthSecond = TENTH_NEVER ;
                        }
                    else
                    if (nI == 1)
                        {
                        RedClock.nShowTenthSecond = TENTH_ALWAYS ;
                        }
                    else
                        {
                        RedClock.nShowTenthSecond = TENTH_ONE_MINUTE ;
                        }

                    nI = SendMessage (h1Second, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        RedClock.bShowOneSecond = 1 ;
                        }
                    else
                    if (nI == 0)
                        {
                        RedClock.bShowOneSecond = 0 ;
                        }
                    else
                    if (nI == 1)
                        {
                        RedClock.bShowOneSecond = 1 ;
                        }

                    REDCLOCK_Adjust () ;

                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDCANCEL :
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDD_REDCLOCK_4 :
					SendDlgItemMessage (hwnd, IDD_REDCLOCK_5, BM_SETCHECK, BST_CHECKED,   0) ;
                    sprintf (cTmp, "%d", DEFAULT_REDCLOCK_SECOND) ;
                    SendDlgItemMessage (hwnd, IDD_REDCLOCK_1, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp) ;

                    strcpy (cTmp, "Never Display Tenth of a Second") ;
                    if (SendMessage (hTenth, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hTenth, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hTenth, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    strcpy (cTmp, "Always Display Tenth of a Second") ;
                    if (SendMessage (h1Second, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (h1Second, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (h1Second, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }
                    return TRUE ;
                }
            break ;
        }
    return FALSE ;
    }

void REDCLOCK_Init (void)
    {
    RedClock.nRedClock        = DEFAULT_REDCLOCK_SECOND ;
    RedClock.nShowTenthSecond = DEFAULT_REDCLOCK_SECOND ;
    RedClock.bShowOneSecond   = DEFAULT_SHOW_ONESECOND ;

    REDCLOCK_Adjust () ;
    }

void REDCLOCK_Adjust (void)
    {
    if (RedClock.nRedClock < -300)
        {
        RedClock.nRedClock = -300 ;
        }
    else
    if (RedClock.nRedClock > 300)
        {
        RedClock.nRedClock = 300 ;
        }
    RedClock.nLRedClock = (long) (RedClock.nRedClock * 1000) ;

    switch (RedClock.nShowTenthSecond)
        {
        case TENTH_NEVER :
        case TENTH_ALWAYS :
        case TENTH_ONE_MINUTE :
            break ;

        default :
            RedClock.nShowTenthSecond = TENTH_NEVER ;
            break ;
        }

    if (RedClock.bShowOneSecond)
        {
        RedClock.bShowOneSecond = 1 ;
        }
    else
        {
        RedClock.bShowOneSecond = 0 ;
        }
    }
