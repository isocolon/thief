#include "thief.h"

BOOL CALLBACK MoveButtonBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    int nI ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            if (User.bShowMoveButton)
                {
                SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_1, BM_SETCHECK, BST_CHECKED, 0) ;
                }
            else
                {
                SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_1, BM_SETCHECK, BST_UNCHECKED, 0) ;
                }

            if (User.bUseArrowKeyMoveButton)
                {
                SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_2, BM_SETCHECK, BST_CHECKED, 0) ;
                }
            else
                {
                SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_2, BM_SETCHECK, BST_UNCHECKED, 0) ;
                }

            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDOK :
                    nI = SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_1, BM_GETSTATE, 0, 0) ;
                    if (nI == BST_CHECKED)
                        {
                        User.bShowMoveButton = 1 ;
                        }
                    else
                        {
                        User.bShowMoveButton = 0 ;
                        }

                    nI = SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_2, BM_GETSTATE, 0, 0) ;
                    if (nI == BST_CHECKED)
                        {
                        User.bUseArrowKeyMoveButton = 1 ;
                        }
                    else
                        {
                        User.bUseArrowKeyMoveButton = 0 ;
                        }

                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDCANCEL :
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDD_MOVEBUTTON_3 :
                    SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_1, BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_MOVEBUTTON_2, BM_SETCHECK, BST_UNCHECKED, 0) ;
                    return TRUE ;
                }
            break ;
        }
    return FALSE ;
    }
