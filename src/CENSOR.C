#include "thief.h"

int CS_nItem, CS_nMode ;
char CS_cHandle  [MAX_CENSOR_ITEM] [ICS_HANDLE_LENGTH] ;
char CS_cCensor  [MAX_CENSOR_ITEM] [MAX_CENSOR_BIT] ;
char CS_oHandle  [MAX_CENSOR_ITEM] [ICS_HANDLE_LENGTH] ;
char CS_cTell    [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10] ;
char CS_cPtell   [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10] ;
char CS_cSay     [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10] ;
char CS_cPartner [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10] ;
char CS_cMessage [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10] ;

BOOL CALLBACK CensorBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static HWND hHandle ;
    static int nSel ;
    int nI ;
    char cTmp [2048] ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            // remember window
            hHandle = GetDlgItem (hwnd, IDD_CENSOR_HANDLE) ;

            // set window title
            switch (System.nCensorIndex)
                {
                case 0 :
                    SetWindowText (hwnd, "Configure FICS Censor List") ;
                    break ;

                case 1 :
                    SetWindowText (hwnd, "Configure ICC Censor List") ;
                    break ;

                case 2 :
                    SetWindowText (hwnd, "Configure Censor List for Other ICS") ;
                    break ;

                default :
                    System.nCensorIndex = 0 ;
                    SetWindowText (hwnd, "Configure FICS Censor List") ;
                    break ;
                }

            // save into temporary arrays
            CS_nItem = Censor [System.nCensorIndex].nItem ;
            for (nI = 0 ; nI < MAX_CENSOR_ITEM ; nI++)
                {
                strcpy (CS_cHandle  [nI], Censor [System.nCensorIndex].cHandle  [nI]) ;
                strcpy (CS_oHandle  [nI], Censor [System.nCensorIndex].cHandle  [nI]) ;
                strcpy (CS_cCensor  [nI], Censor [System.nCensorIndex].cCensor  [nI]) ;
                strcpy (CS_cTell    [nI], Censor [System.nCensorIndex].cTell    [nI]) ;
                strcpy (CS_cPtell   [nI], Censor [System.nCensorIndex].cPtell   [nI]) ;
                strcpy (CS_cSay     [nI], Censor [System.nCensorIndex].cSay     [nI]) ;
                strcpy (CS_cPartner [nI], Censor [System.nCensorIndex].cPartner [nI]) ;
				strcpy (CS_cMessage [nI], Censor [System.nCensorIndex].cMessage [nI]) ;
                }

            // adjust apply censor checkbox
            SendDlgItemMessage (hwnd, IDD_CENSOR_APPLY, BM_SETCHECK, Censor [System.nCensorIndex].bCensor ? BST_CHECKED : BST_UNCHECKED, 0) ;

            // show list
            SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_RESETCONTENT, 0, 0) ;
            for (nI = 0 ; nI < CS_nItem ; nI++)
                {
                sprintf (cTmp, "%02d. %s", nI + 1, CS_cHandle [nI]) ;
                SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
                }

            // initialize to first item
            nSel = 0 ;

            // show handle
            SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) CS_cHandle [nSel]) ;

            // show bits
            for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                {
                SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, (CS_cCensor [nSel] [nI] == '1') ? BST_CHECKED : BST_UNCHECKED, 0) ;
                }

            // show censor messages
            SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) Censor [System.nCensorIndex].cTell    [nSel]) ;
            SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) Censor [System.nCensorIndex].cPtell   [nSel]) ;
            SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) Censor [System.nCensorIndex].cSay     [nSel]) ;
            SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) Censor [System.nCensorIndex].cPartner [nSel]) ;
			SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) Censor [System.nCensorIndex].cMessage [nSel]) ;

            // select list item
            SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, (WPARAM) nSel, 0) ;

            // disable all input items
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_HANDLE),  0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT1),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT2),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT3),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT4),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT5),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT6),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT7),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT8),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT9),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT10),   0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT11),   0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_TELL),    0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PTELL),   0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_SAY),     0) ;
            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PARTNER), 0) ;
			EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_MESSAGE), 0) ;
            
            // disable "new" button if there are already to the maximum items
            if (CS_nItem >= MAX_CENSOR_ITEM)
                {
                EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 0) ;
                }

            // disable "edit" and "delete" buttons if there are no items
            if (CS_nItem <= 0)
                {
                EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   0) ;
                EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 0) ;
                }

            // assign input mode
            CS_nMode = 0 ;

            // centralize window
            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDD_CENSOR_LIST :
                    if (HIWORD (wParam) == LBN_SELCHANGE)
                        {
                        nSel = SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_GETCURSEL, 0, 0) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) CS_cHandle [nSel]) ;

                        for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                            {
                            SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, (CS_cCensor [nSel] [nI] == '1') ? BST_CHECKED : BST_UNCHECKED, 0) ;
                            }

                        SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) CS_cTell    [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) CS_cPtell   [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) CS_cSay     [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) CS_cPartner [nSel]) ;
						SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) CS_cMessage [nSel]) ;
                        }
                    break ;

                case IDD_CENSOR_NEW :
                    // disable check box
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_APPLY), 0) ;

                    // disable list box
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_LIST), 0) ;

                    // hide "new", "edit", and "delete" buttons
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW),    SW_HIDE) ;
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   SW_HIDE) ;
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), SW_HIDE) ;

                    // enable all input items
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_HANDLE),  1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT1),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT2),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT3),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT4),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT5),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT6),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT7),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT8),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT9),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT10),   1) ;
					EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT11),   1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_TELL),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PTELL),   1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_SAY),     1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PARTNER), 1) ;
					EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_MESSAGE), 1) ;

                    // assign input mode
                    CS_nMode = 1 ;
                    break ;

                case IDD_CENSOR_EDIT :
                    if (nSel >= CS_nItem)
                        {
                        TOOLBOX_Error ("No Handle Selected.") ;
                        return FALSE ;
                        }

                    // disable check box
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_APPLY), 0) ;

                    // disable list box
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_LIST), 0) ;

                    // hide "new", "edit", and "delete" buttons
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW),    SW_HIDE) ;
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   SW_HIDE) ;
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), SW_HIDE) ;

                    // enable all input items
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_HANDLE),  1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT1),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT2),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT3),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT4),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT5),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT6),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT7),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT8),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT9),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT10),   1) ;
					EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT11),   1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_TELL),    1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PTELL),   1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_SAY),     1) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PARTNER), 1) ;
					EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_MESSAGE), 1) ;

                    // assign input mode
                    CS_nMode = 2 ;
                    break ;

                case IDD_CENSOR_DELETE :
                    if (nSel >= CS_nItem)
                        {
                        TOOLBOX_Error ("No Handle Selected.") ;
                        return FALSE ;
                        }

                    for (nI = nSel ; nI < (MAX_CENSOR_ITEM - 1) ; nI++)
                        {
                        strcpy (CS_cHandle [nI], CS_cHandle [nI + 1]) ;
                        strcpy (CS_cCensor [nI], CS_cCensor [nI + 1]) ;
                        }

                    // clean up deleted item
                    strcpy (CS_cHandle [CS_nItem], "") ;
                    strcpy (CS_cCensor [CS_nItem], THIEF_CENSOR_BIT) ;

                    // show list again
                    CS_nItem = CS_nItem - 1 ;

                    // show list again
                    SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_RESETCONTENT, 0, 0) ;
                    for (nI = 0 ; nI < CS_nItem ; nI++)
                        {
                        sprintf (cTmp, "%02d. %s", nI + 1, CS_cHandle [nI]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

                        strcpy (CS_oHandle [nI], CS_cHandle [nI]) ;
                        }

                    if (CS_nItem > 0)
                        {
                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, 0, 0) ;

                        if (nSel >= CS_nItem)
                            {
                            nSel = (CS_nItem - 1) ;
                            }

                        // show item
                        SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) CS_cHandle [nSel]) ;

                        for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                            {
                            SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, (CS_cCensor [nSel] [nI] == '1') ? BST_CHECKED : BST_UNCHECKED, 0) ;
                            }

                        SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) CS_cTell    [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) CS_cPtell   [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) CS_cSay     [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) CS_cPartner [nSel]) ;
						SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) CS_cMessage [nSel]) ;

                        // select item
                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, (WPARAM) nSel, 0) ;
                        }
                    else
                        {
                        nSel = 0 ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, 0, 0) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) "") ;

                        for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                            {
                            SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, BST_CHECKED, 0) ;
                            }

                        SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_TELL_MESSAGE) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_PTELL_MESSAGE) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_SAY_MESSAGE) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_PARTNER_MESSAGE) ;
						SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_MESSAGE_MESSAGE) ;
                        }

                    // disable all input items
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_HANDLE),  0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT1),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT2),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT3),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT4),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT5),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT6),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT7),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT8),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT9),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT10),   0) ;
					EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT11),   0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_TELL),    0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PTELL),   0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_SAY),     0) ;
                    EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PARTNER), 0) ;
					EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_MESSAGE), 0) ;

                    // show "new", "edit", and "delete" buttons
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW),    SW_SHOW) ;
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   SW_SHOW) ;
                    ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), SW_SHOW) ;

                    // disable "new" button if there are already to the maximum items
                    if (CS_nItem >= MAX_CENSOR_ITEM)
                        {
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 0) ;
                        }

                    // disable "edit" and "delete" buttons if there are no items
                    if (CS_nItem <= 0)
                        {
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 0) ;
                        }

                    // assign input mode
                    CS_nMode = 0 ;
                    break ;

                case IDOK :
                    if (CS_nMode == 1)
                        {
                        SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR ;

                        if (strlen (cTmp) == 0)
                            {
                            TOOLBOX_Error ("Missing Handle.") ;
                            SetFocus (hHandle) ;
                            return FALSE ;
                            }

                        for (nI = 0 ; nI < ((int) strlen (cTmp)) ; nI++)
                            {
                            if ((! isalnum (cTmp [nI])) && (cTmp [nI] != '-'))
                                {
                                TOOLBOX_Error ("Invalid Handle.") ;
                                SetFocus (hHandle) ;
                                return FALSE ;
                                }
                            }

                        // add it into the array
                        strcpy (CS_cHandle [CS_nItem], cTmp) ;

                        for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                            {
                            if (SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_GETSTATE, 0, 0) == BST_CHECKED)
                                {
                                CS_cCensor [CS_nItem] [nI] = '1' ;
                                }
                            else
                                {
                                CS_cCensor [CS_nItem] [nI] = '0' ;
                                }
                            }

                        SendDlgItemMessage (hwnd, IDD_CENSOR_TELL, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cTell [CS_nItem], cTmp) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cPtell [CS_nItem], cTmp) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_SAY, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cSay [CS_nItem], cTmp) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cPartner [CS_nItem], cTmp) ;

						SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cMessage [CS_nItem], cTmp) ;

						CS_nItem = CS_nItem + 1 ;

                        // show list again
                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_RESETCONTENT, 0, 0) ;
                        for (nI = 0 ; nI < CS_nItem ; nI++)
                            {
                            sprintf (cTmp, "%02d. %s", nI + 1, CS_cHandle [nI]) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

                            strcpy (CS_oHandle [nI], CS_cHandle [nI]) ;
                            }

                        // show item just added
                        nSel = CS_nItem - 1 ;

                        // show item
                        SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) CS_cHandle [nSel]) ;

                        for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                            {
                            SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, (CS_cCensor [nSel] [nI] == '1') ? BST_CHECKED : BST_UNCHECKED, 0) ;
                            }

                        SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) CS_cTell    [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) CS_cPtell   [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) CS_cSay     [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) CS_cPartner [nSel]) ;
						SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) CS_cMessage [nSel]) ;

                        // select item
                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, (WPARAM) nSel, 0) ;

                        // disable all input items
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_HANDLE),  0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT1),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT2),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT3),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT4),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT5),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT6),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT7),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT8),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT9),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT10),   0) ;
						EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT11),   0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_TELL),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PTELL),   0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_SAY),     0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PARTNER), 0) ;
						EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_MESSAGE), 0) ;

                        // enable check box
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_APPLY), 1) ;

                        // enable list box
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_LIST), 1) ;

                        // show "new", "edit", and "delete" buttons
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW),    SW_SHOW) ;
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   SW_SHOW) ;
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), SW_SHOW) ;

                        // disable "new" button if there are already to the maximum items
                        if (CS_nItem >= MAX_CENSOR_ITEM)
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 0) ;
                            }
                        else
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 1) ;
                            }

                        // disable "edit" and "delete" buttons if there are no items
                        if (CS_nItem <= 0)
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   0) ;
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 0) ;
                            }
                        else
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   1) ;
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 1) ;
                            }

                        // assign input mode
                        CS_nMode = 0 ;
                        }
                    else
                    if (CS_nMode == 2)
                        {
                        nSel = SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_GETCURSEL, 0, 0) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR ;
                        if (strlen (cTmp) == 0)
                            {
                            TOOLBOX_Error ("Missing Handle.") ;
                            SetFocus (hHandle) ;
                            return FALSE ;
                            }

                        for (nI = 0 ; nI < ((int) strlen (cTmp)) ; nI++)
                            {
                            if ((! isalnum (cTmp [nI])) && (cTmp [nI] != '-'))
                                {
                                TOOLBOX_Error ("Invalid Handle.") ;
                                SetFocus (hHandle) ;
                                return FALSE ;
                                }
                            }

                        strcpy (CS_cHandle [nSel], cTmp) ;

                        for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                            {
                            if (SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_GETSTATE, 0, 0) == BST_CHECKED)
                                {
                                CS_cCensor [nSel] [nI] = '1' ;
                                }
                            else
                                {
                                CS_cCensor [nSel] [nI] = '0' ;
                                }
                            }

                        SendDlgItemMessage (hwnd, IDD_CENSOR_TELL, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cTell [nSel], cTmp) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cPtell [nSel], cTmp) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_SAY, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cSay [nSel], cTmp) ;

                        SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cPartner [nSel], cTmp) ;

						SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_GETTEXT, (WPARAM) CENSOR_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
                        TOOLBOX_AllTrim (cTmp) ;
                        strcpy (CS_cMessage [nSel], cTmp) ;

                        // show list again
                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_RESETCONTENT, 0, 0) ;
                        for (nI = 0 ; nI < CS_nItem ; nI++)
                            {
                            sprintf (cTmp, "%02d. %s", nI + 1, CS_cHandle [nI]) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

                            strcpy (CS_oHandle [nI], CS_cHandle [nI]) ;
                            }

                        // show item
                        SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) CS_cHandle [nSel]) ;

                        for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                            {
                            SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, (CS_cCensor [nSel] [nI] == '1') ? BST_CHECKED : BST_UNCHECKED, 0) ;
                            }

                        SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) CS_cTell    [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) CS_cPtell   [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) CS_cSay     [nSel]) ;
                        SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) CS_cPartner [nSel]) ;
						SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) CS_cMessage [nSel]) ;

                        // select item
                        SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, (WPARAM) nSel, 0) ;

                        // disable all input items
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_HANDLE),  0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT1),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT2),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT3),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT4),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT5),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT6),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT7),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT8),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT9),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT10),   0) ;
						EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT11),   0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_TELL),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PTELL),   0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_SAY),     0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PARTNER), 0) ;
						EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_MESSAGE), 0) ;

                        // enable check box
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_APPLY), 1) ;

                        // enable list box
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_LIST), 1) ;

                        // show "new", "edit", and "delete" buttons
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW),    SW_SHOW) ;
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   SW_SHOW) ;
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), SW_SHOW) ;

                        // disable "new" button if there are already to the maximum items
                        if (CS_nItem >= MAX_CENSOR_ITEM)
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 0) ;
                            }
                        else
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 1) ;
                            }

                        // disable "edit" and "delete" buttons if there are no items
                        if (CS_nItem <= 0)
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   0) ;
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 0) ;
                            }
                        else
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   1) ;
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 1) ;
                            }

                        // assign input mode
                        CS_nMode = 0 ;
                        }
                    else
                        {
                        if (SendDlgItemMessage (hwnd, IDD_CENSOR_APPLY, BM_GETSTATE, 0, 0) == BST_CHECKED)
                            {
                            Censor [System.nCensorIndex].bCensor = 1 ;
                            }
                        else
                            {
                            Censor [System.nCensorIndex].bCensor = 0 ;
                            }

                        for (nI = 0 ; nI < MAX_CENSOR_ITEM ; nI++)
                            {
                            strcpy (Censor [System.nCensorIndex].cHandle  [nI], "") ;
                            strcpy (Censor [System.nCensorIndex].cCensor  [nI], THIEF_CENSOR_BIT) ;
                            strcpy (Censor [System.nCensorIndex].cTell    [nI], THIEF_CENSOR_TELL_MESSAGE) ;
                            strcpy (Censor [System.nCensorIndex].cPtell   [nI], THIEF_CENSOR_PTELL_MESSAGE) ;
                            strcpy (Censor [System.nCensorIndex].cSay     [nI], THIEF_CENSOR_SAY_MESSAGE) ;
                            strcpy (Censor [System.nCensorIndex].cPartner [nI], THIEF_CENSOR_PARTNER_MESSAGE) ;
							strcpy (Censor [System.nCensorIndex].cMessage [nI], THIEF_CENSOR_MESSAGE_MESSAGE) ;
                            }

                        Censor [System.nCensorIndex].nItem = CS_nItem ;
                        for (nI = 0 ; nI < CS_nItem ; nI++)
                            {
                            strcpy (Censor [System.nCensorIndex].cHandle  [nI], CS_cHandle  [nI]) ;
                            strcpy (Censor [System.nCensorIndex].cCensor  [nI], CS_cCensor  [nI]) ;
                            strcpy (Censor [System.nCensorIndex].cTell    [nI], CS_cTell    [nI]) ;
                            strcpy (Censor [System.nCensorIndex].cPtell   [nI], CS_cPtell   [nI]) ;
                            strcpy (Censor [System.nCensorIndex].cSay     [nI], CS_cSay     [nI]) ;
                            strcpy (Censor [System.nCensorIndex].cPartner [nI], CS_cPartner [nI]) ;
							strcpy (Censor [System.nCensorIndex].cMessage [nI], CS_cMessage [nI]) ;
                            }

                        CENSOR_Adjust () ;
                        EndDialog (hwnd, TRUE) ;
                        STATE_LeaveDialogBox () ;
                        return TRUE ;
                        }
                    break ;

                case IDCANCEL :
                    if (CS_nMode == 0)
                        {
                        EndDialog (hwnd, TRUE) ;
                        STATE_LeaveDialogBox () ;
                        return TRUE ;
                        }
                    else
                        {
                        if (CS_nItem > 0)
                            {
                            SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, 0, 0) ;

                            if (nSel >= CS_nItem)
                                {
                                nSel = (CS_nItem - 1) ;
                                }

                            // show item
                            SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) CS_cHandle [nSel]) ;

                            for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                                {
                                SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, (CS_cCensor [nSel] [nI] == '1') ? BST_CHECKED : BST_UNCHECKED, 0) ;
                                }

                            SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) CS_cTell    [nSel]) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) CS_cPtell   [nSel]) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) CS_cSay     [nSel]) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) CS_cPartner [nSel]) ;
							SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) CS_cMessage [nSel]) ;

                            // select item
                            SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, (WPARAM) nSel, 0) ;
                            }
                        else
                            {
                            nSel = 0 ;

                            SendDlgItemMessage (hwnd, IDD_CENSOR_LIST, LB_SETCURSEL, 0, 0) ;

                            SendDlgItemMessage (hwnd, IDD_CENSOR_HANDLE, WM_SETTEXT, 0, (LPARAM) "") ;

                            for (nI = 0 ; nI < (MAX_CENSOR_BIT - 1) ; nI++)
                                {
                                SendDlgItemMessage (hwnd, IDD_CENSOR_BIT1 + nI, BM_SETCHECK, BST_CHECKED, 0) ;
                                }

                            SendDlgItemMessage (hwnd, IDD_CENSOR_TELL,    WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_TELL_MESSAGE) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_PTELL,   WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_PTELL_MESSAGE) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_SAY,     WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_SAY_MESSAGE) ;
                            SendDlgItemMessage (hwnd, IDD_CENSOR_PARTNER, WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_PARTNER_MESSAGE) ;
							SendDlgItemMessage (hwnd, IDD_CENSOR_MESSAGE, WM_SETTEXT, 0, (LPARAM) THIEF_CENSOR_MESSAGE_MESSAGE) ;
                            }

                        // disable all input items
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_HANDLE),  0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT1),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT2),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT3),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT4),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT5),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT6),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT7),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT8),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT9),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT10),   0) ;
						EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_BIT11),   0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_TELL),    0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PTELL),   0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_SAY),     0) ;
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_PARTNER), 0) ;
						EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_MESSAGE), 0) ;

                        // enable check box
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_APPLY), 1) ;

                        // enable list box
                        EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_LIST), 1) ;

                        // show "new", "edit", and "delete" buttons
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW),    SW_SHOW) ;
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   SW_SHOW) ;
                        ShowWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), SW_SHOW) ;

                        // disable "new" button if there are already to the maximum items
                        if (CS_nItem >= MAX_CENSOR_ITEM)
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 0) ;
                            }
                        else
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_NEW), 1) ;
                            }

                        // disable "edit" and "delete" buttons if there are no items
                        if (CS_nItem <= 0)
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   0) ;
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 0) ;
                            }
                        else
                            {
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_EDIT),   1) ;
                            EnableWindow (GetDlgItem (hwnd, IDD_CENSOR_DELETE), 1) ;
                            }

                        // assign input mode
                        CS_nMode = 0 ;
                        }
                    break ;
                }
            break ;
        }
    return FALSE ;
    }

void CENSOR_Init (void)
    {
    int nC, nI ;

    for (nC = 0 ; nC < MAX_CENSOR ; nC++)
        {
        Censor [nC].bCensor = DEFAULT_CENSOR ;
        Censor [nC].nItem   = 0 ;

        for (nI = 0 ; nI < MAX_CENSOR_ITEM ; nI++)
            {
            strcpy (Censor [nC].cHandle  [nI], "") ;
            strcpy (Censor [nC].cCensor  [nI], THIEF_CENSOR_BIT) ;
            strcpy (Censor [nC].cTell    [nI], THIEF_CENSOR_TELL_MESSAGE) ;
            strcpy (Censor [nC].cPtell   [nI], THIEF_CENSOR_PTELL_MESSAGE) ;
            strcpy (Censor [nC].cSay     [nI], THIEF_CENSOR_SAY_MESSAGE) ;
            strcpy (Censor [nC].cPartner [nI], THIEF_CENSOR_PARTNER_MESSAGE) ;
			strcpy (Censor [nC].cMessage [nI], THIEF_CENSOR_MESSAGE_MESSAGE) ;
            }
        }
    }

void CENSOR_Sort (void)
    {
    int nC, nI, nJ ;

    char cTmp [2048] ;

    for (nC = 0 ; nC < MAX_CENSOR ; nC++)
        {
        for (nI = 0 ; nI < Censor [nC].nItem ; nI++)
            {
            for (nJ = 0 ; nJ < Censor [nC].nItem ; nJ++)
                {
                if (nI != nJ)
                    {
                    if (tolower(Censor [nC].cHandle [nI] [0]) < tolower(Censor [nC].cHandle [nJ] [0]))
                        {
                        strcpy (cTmp, Censor [nC].cHandle [nI]) ;
                        strcpy (Censor [nC].cHandle [nI], Censor [nC].cHandle [nJ]) ;
                        strcpy (Censor [nC].cHandle [nJ], cTmp) ;

                        strcpy (cTmp, Censor [nC].cCensor [nI]) ;
                        strcpy (Censor [nC].cCensor [nI], Censor [nC].cCensor [nJ]) ;
                        strcpy (Censor [nC].cCensor [nJ], cTmp) ;

                        strcpy (cTmp, Censor [nC].cTell [nI]) ;
                        strcpy (Censor [nC].cTell [nI], Censor [nC].cTell [nJ]) ;
                        strcpy (Censor [nC].cTell [nJ], cTmp) ;

                        strcpy (cTmp, Censor [nC].cPtell [nI]) ;
                        strcpy (Censor [nC].cPtell [nI], Censor [nC].cPtell [nJ]) ;
                        strcpy (Censor [nC].cPtell [nJ], cTmp) ;

                        strcpy (cTmp, Censor [nC].cSay [nI]) ;
                        strcpy (Censor [nC].cSay [nI], Censor [nC].cSay [nJ]) ;
                        strcpy (Censor [nC].cSay [nJ], cTmp) ;

                        strcpy (cTmp, Censor [nC].cPartner [nI]) ;
                        strcpy (Censor [nC].cPartner [nI], Censor [nC].cPartner [nJ]) ;
                        strcpy (Censor [nC].cPartner [nJ], cTmp) ;

						strcpy (cTmp, Censor [nC].cMessage [nI]) ;
                        strcpy (Censor [nC].cMessage [nI], Censor [nC].cMessage [nJ]) ;
                        strcpy (Censor [nC].cMessage [nJ], cTmp) ;
                        }
                    }
                }
            }
        }
    }

void CENSOR_Index (void)
    {
    int nC, nI, nJ ;

    for (nC = 0 ; nC < MAX_CENSOR ; nC++)
        {
        for (nJ = 0 ; nJ < 26 ; nJ++)
            {
            Censor [nC].nStart [nJ] = -1 ;
            Censor [nC].nEnd   [nJ] = -1 ;
            }

        for (nI = 0 ; nI < Censor [nC].nItem ; nI++)
            {
            nJ = (int) (tolower (Censor [nC].cHandle [nI] [0]) - 'a') ;

            if (Censor [nC].nStart [nJ] == -1)
                {
                Censor [nC].nStart [nJ] = nI ;
                }

            Censor [nC].nEnd [nJ] = nI ;
            }
        }
    }

void CENSOR_Adjust (void)
    {
    int nC, nD, nI, nJ, nS ;
    char cTmp [2048] ;

    for (nC = 0 ; nC < MAX_CENSOR ; nC++)
        {
        nD = 0 ;

        for (nI = 0 ; nI < MAX_CENSOR_ITEM ; nI++)
            {
            strcpy (cTmp, Censor [nC].cHandle [nI]) ;
            TOOLBOX_AllTrim (cTmp) ;
            cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR ;

            for (nJ = 0 ; nJ < ((int) strlen (cTmp)) ; nJ++)
                {
                if ((! isalnum (cTmp [nJ])) && (cTmp [nJ] != '-'))
                    {
                    cTmp [nJ] = NULL_CHAR ;
                    break ;
                    }
                }

            if (strlen (cTmp) == 0)
                {
                strcpy (Censor [nC].cHandle  [nI], "") ;
                strcpy (Censor [nC].cCensor  [nI], THIEF_CENSOR_BIT) ;
                strcpy (Censor [nC].cTell    [nI], THIEF_CENSOR_TELL_MESSAGE) ;
                strcpy (Censor [nC].cPtell   [nI], THIEF_CENSOR_PTELL_MESSAGE) ;
                strcpy (Censor [nC].cSay     [nI], THIEF_CENSOR_SAY_MESSAGE) ;
                strcpy (Censor [nC].cPartner [nI], THIEF_CENSOR_PARTNER_MESSAGE) ;
				strcpy (Censor [nC].cMessage [nI], THIEF_CENSOR_MESSAGE_MESSAGE) ;
                }
            else
                {
                strcpy (Censor [nC].cHandle [nI], cTmp) ;

                strcpy (cTmp, Censor [nC].cCensor [nI]) ;
                cTmp [(MAX_CENSOR_BIT - 1)] = NULL_CHAR ;
                nS = strlen (cTmp) ;
                if (nS < (MAX_CENSOR_BIT - 1))
                    {
                    for (nJ = (nS - 1) ; nJ < (MAX_CENSOR_BIT - 1) ; nJ++)
                        {
                        cTmp [nJ] = '1' ;
                        }
                    }
                strcpy (Censor [nC].cCensor [nI], cTmp) ;

                nD = nD + 1 ;
                }
            }

        Censor [nC].nItem = nD ;
        }

    CENSOR_Sort  () ;
    CENSOR_Index () ;
    }

int CENSOR_Tell (char *cHandle, char *cS)
    {
    int nI, nJ, nS, nE ;
    char *cP, cTmp [512] ;

    nJ = (int) (tolower (cHandle [0]) - 'a') ;

    nS = Censor [Login.nLoginType].nStart [nJ] ;
    nE = Censor [Login.nLoginType].nEnd   [nJ] ;

    if ((nS == -1) || (nE == -1))
        {
        return 0 ;
        }

    for (nI = nS ; nI <= nE ; nI++)
        {
        if (stricmp (Censor [Login.nLoginType].cHandle [nI], cHandle) == 0)
            {
            if (Censor [Login.nLoginType].cCensor [nI] [0] != '0')
                {
                cP = strchr (cS, ':') ;
                if (cP != NULL)
                    {
                    if (strlen (Censor [Login.nLoginType].cTell [nI]) > 0)
                        {
                        if (strncmp (cP, THIEF_CENSOR_PREFIX1, THIEF_CENSOR_PREFIX1_SIZE))
                            {
                            // no it's not an auto message
                            sprintf (cTmp,
                                     "%s %s %s%s\n",
                                     ICS_CENSOR_TELL_COMMAND,
                                     cHandle,
                                     THIEF_CENSOR_PREFIX,
                                     Censor [Login.nLoginType].cTell [nI]) ;

                            TOOLBOX_WriteICS (cTmp) ;

                            Telnet.bTellTold = 1 ;
                            sprintf (Telnet.cTellTold, "(told %s", cHandle) ;
                            Telnet.nTellTold = strlen (Telnet.cTellTold) ;
                            }
                        else
                            {
                            // yes it's an auto message
                            Telnet.bTellTold = 0 ;
                            }
                        }
                    else
                        {
                        Telnet.bTellTold = 0 ;
                        }
                    }
                else
                    {
                    Telnet.bTellTold = 0 ;
                    }
                return 1 ;
                }
            else
                {
                Telnet.bTellTold = 0 ;
                }
            break ;
            }
        }
    return 0 ;
    }

int CENSOR_Ptell (char *cHandle, char *cS)
    {
    int nI, nJ, nS, nE ;
    char *cP, cTmp [512] ;

    nJ = (int) (tolower (cHandle [0]) - 'a') ;

    nS = Censor [Login.nLoginType].nStart [nJ] ;
    nE = Censor [Login.nLoginType].nEnd   [nJ] ;

    if ((nS == -1) || (nE == -1))
        {
        return 0 ;
        }

    for (nI = nS ; nI <= nE ; nI++)
        {
        if (stricmp (Censor [Login.nLoginType].cHandle [nI], cHandle) == 0)
            {
            if (Censor [Login.nLoginType].cCensor [nI] [1] != '0')
                {
                cP = strchr (cS, ':') ;
                if (cP != NULL)
                    {
                    if (strlen (Censor [Login.nLoginType].cPtell [nI]) > 0)
                        {
                        if (strncmp (cP, THIEF_CENSOR_PREFIX1, THIEF_CENSOR_PREFIX1_SIZE))
                            {
                            // no it's not an auto message
                            sprintf (cTmp,
                                     "%s %s %s%s\n",
                                     ICS_CENSOR_TELL_COMMAND,
                                     cHandle,
                                     THIEF_CENSOR_PREFIX,
                                     Censor [Login.nLoginType].cPtell [nI]) ;

                            TOOLBOX_WriteICS (cTmp) ;

                            Telnet.bPtellTold = 1 ;
                            sprintf (Telnet.cPtellTold, "(told %s", cHandle) ;
                            Telnet.nPtellTold = strlen (Telnet.cPtellTold) ;
                            }
                        else
                            {
                            // yes it's an auto message
                            Telnet.bPtellTold = 0 ;
                            }
                        }
                    else
                        {
                        Telnet.bPtellTold = 0 ;
                        }
                    }
                else
                    {
                    Telnet.bPtellTold = 0 ;
                    }
                return 1 ;
                }
            else
                {
                Telnet.bPtellTold = 0 ;
                }
            break ;
            }
        }
    return 0 ;
    }

int CENSOR_Say (char *cHandle, char *cS)
    {
    int nI, nJ, nS, nE ;
    char *cP, cTmp [512] ;

    nJ = (int) (tolower (cHandle [0]) - 'a') ;

    nS = Censor [Login.nLoginType].nStart [nJ] ;
    nE = Censor [Login.nLoginType].nEnd   [nJ] ;

    if ((nS == -1) || (nE == -1))
        {
        return 0 ;
        }

    for (nI = nS ; nI <= nE ; nI++)
        {
        if (stricmp (Censor [Login.nLoginType].cHandle [nI], cHandle) == 0)
            {
            if (Censor [Login.nLoginType].cCensor [nI] [2] != '0')
                {
                cP = strchr (cS, ':') ;
                if (cP != NULL)
                    {
                    if (strlen (Censor [Login.nLoginType].cSay [nI]) > 0)
                        {
                        if (strncmp (cP, THIEF_CENSOR_PREFIX1, THIEF_CENSOR_PREFIX1_SIZE))
                            {
                            // no it's not an auto message
                            sprintf (cTmp,
                                     "%s %s %s%s\n",
                                     ICS_CENSOR_TELL_COMMAND,
                                     cHandle,
                                     THIEF_CENSOR_PREFIX,
                                     Censor [Login.nLoginType].cSay [nI]) ;

                            TOOLBOX_WriteICS (cTmp) ;

                            Telnet.bSayTold = 1 ;
                            sprintf (Telnet.cSayTold, "(told %s", cHandle) ;
                            Telnet.nSayTold = strlen (Telnet.cSayTold) ;
                            }
                        else
                            {
                            // yes it's an auto message
                            Telnet.bSayTold = 0 ;
                            }
                        }
                    else
                        {
                        Telnet.bSayTold = 0 ;
                        }
                    }
                else
                    {
                    Telnet.bSayTold = 0 ;
                    }
                return 1 ;
                }
            else
                {
                Telnet.bSayTold = 0 ;
                }
            break ;
            }
        }
    return 0 ;
    }

int CENSOR_Partner (char *cHandle)
    {
    int nI, nJ, nS, nE ;
    char cTmp [512] ;

    nJ = (int) (tolower (cHandle [0]) - 'a') ;

    nS = Censor [Login.nLoginType].nStart [nJ] ;
    nE = Censor [Login.nLoginType].nEnd   [nJ] ;

    if ((nS == -1) || (nE == -1))
        {
        return 0 ;
        }

    for (nI = nS ; nI <= nE ; nI++)
        {
        if (stricmp (Censor [Login.nLoginType].cHandle [nI], cHandle) == 0)
            {
            if (Censor [Login.nLoginType].cCensor [nI] [9] != '0')
                {
                if (strlen (Censor [Login.nLoginType].cPartner [nI]) > 0)
                    {
                    sprintf (cTmp,
                             "%s %s %s%s\n",
                             ICS_CENSOR_TELL_COMMAND,
                             cHandle,
                             THIEF_CENSOR_PREFIX,
                             Censor [Login.nLoginType].cPartner [nI]) ;

                    TOOLBOX_WriteICS (cTmp) ;

                    Telnet.bPartnerTold = 1 ;
                    sprintf (Telnet.cPartnerTold, "(told %s", cHandle) ;
                    Telnet.nPartnerTold = strlen (Telnet.cPartnerTold) ;
                    }
                else
                    {
                    Telnet.bPartnerTold = 0 ;
                    }
                return 1 ;
                }
            else
                {
                Telnet.bPartnerTold = 0 ;
                }
            break ;
            }
        }
    return 0 ;
    }

// 1.24 
int CENSOR_Message (char *cHandle)
	{
    int nI, nJ, nS, nE ;
    char cTmp [512] ;

	nJ = (int) (tolower (cHandle [0]) - 'a') ;
    nS = Censor [Login.nLoginType].nStart [nJ] ;
    nE = Censor [Login.nLoginType].nEnd   [nJ] ;

    if ((nS == -1) || (nE == -1))
        {
        return 0 ;
        }

    for (nI = nS ; nI <= nE ; nI++)
        {
        if (stricmp (Censor [Login.nLoginType].cHandle [nI], cHandle) == 0)
            {
            if (Censor [Login.nLoginType].cCensor [nI] [10] != '0')
                {
                sprintf (cTmp,
                             "%s %s %s%s\n",
                             ICS_CENSOR_TELL_COMMAND,
                             cHandle,
                             THIEF_CENSOR_PREFIX,
                             Censor [Login.nLoginType].cMessage [nI]) ;

                TOOLBOX_WriteICS (cTmp) ;
				sprintf (cTmp,
                             "%s %s\n",
                             ICS_CLEARMESSAGE_COMMAND,
                             cHandle) ;
                TOOLBOX_WriteICS (cTmp) ;
				Telnet.nAutoClearedMessage++;

                Telnet.bSayTold = 1 ;
                sprintf (Telnet.cSayTold, "(told %s", cHandle) ;
                Telnet.nSayTold = strlen (Telnet.cSayTold) ;
				return 1;
                }
            break ;
            }
        }
    return 0 ;
    }

int CENSOR_Other (int nX, char *cHandle)
    {
    int nI, nJ, nS, nE ;

    nJ = (int) (tolower (cHandle [0]) - 'a') ;

    nS = Censor [Login.nLoginType].nStart [nJ] ;
    nE = Censor [Login.nLoginType].nEnd   [nJ] ;

    if ((nS == -1) || (nE == -1))
        {
        return 0 ;
        }

    for (nI = nS ; nI <= nE ; nI++)
        {
        if (stricmp (Censor [Login.nLoginType].cHandle [nI], cHandle) == 0)
            {
            if (Censor [Login.nLoginType].cCensor [nI] [nX] != '0')
                {
                return 1 ;
                }
            break ;
            }
        }
    return 0 ;
    }

int CENSOR_ClearMessages (char *cLine)
	{
	int nI, nT , nU ;
	char cGarbage1 [20] , cTmp [512] ;

	//You have x messages (y unread).
	//You have x message (y unread).
    if (Censor [Login.nLoginType].bCensor)
       {

        sscanf (cLine, "You have %d %s (%d", &nT, &cGarbage1, &nU) ;
		
        if (nU > 0)
			{

		    if (Censor [System.nCensorIndex].nItem == 0)
		    {
			return 0 ;
			}

			for (nI = 0 ; nI < Censor [System.nCensorIndex].nItem ; nI++)
				{
				// 1.25 fix, only delete if message censor enabled
				if (Censor [Login.nLoginType].cCensor [nI] [10] != '0'){
					sprintf (cTmp,
								"%s %s\n",
								ICS_CLEARMESSAGE_COMMAND,
								Censor [Login.nLoginType].cHandle [nI]) ;
					TOOLBOX_WriteICS (cTmp) ;
					Telnet.nAutoClearedMessage++;
					}
				}
			}
		}
	return 0 ;
	}