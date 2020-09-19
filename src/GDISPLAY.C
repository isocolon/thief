#include "thief.h"

BOOL CALLBACK GameDisplayBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static HWND hCapture, hPlayM, hPlayO, hExam, hObserve, hBuffer ;
    char cTmp [2048] ;
    int  nI ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            hCapture = GetDlgItem (hwnd, IDD_GDISPLAY_14) ;
            hPlayM   = GetDlgItem (hwnd, IDD_GDISPLAY_15) ;
            hPlayO   = GetDlgItem (hwnd, IDD_GDISPLAY_16) ;
            hExam    = GetDlgItem (hwnd, IDD_GDISPLAY_17) ;
            hObserve = GetDlgItem (hwnd, IDD_GDISPLAY_18) ;
            hBuffer  = GetDlgItem (hwnd, IDD_GDISPLAY_19) ;

            SendDlgItemMessage (hwnd, IDD_GDISPLAY_1,  BM_SETCHECK, User.bShowResult             ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_2,  BM_SETCHECK, User.bShowRating             ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_3,  BM_SETCHECK, User.bShowLagStat            ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_4,  BM_SETCHECK, User.bShowPtell              ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_5,  BM_SETCHECK, User.bShowCoord              ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_6,  BM_SETCHECK, User.bShowGameType           ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_7,  BM_SETCHECK, User.bShowGameTypeOnTitleBar ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_8,  BM_SETCHECK, User.bShowValueOnTitleBar    ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_9,  BM_SETCHECK, User.bShowLastMove           ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_10, BM_SETCHECK, User.bShowLastMoveTenth      ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_11, BM_SETCHECK, User.bShowHighlight          ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_12, BM_SETCHECK, User.bShowTPHighlight        ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_GDISPLAY_13, BM_SETCHECK, User.bShowClockOnTop         ? BST_CHECKED : BST_UNCHECKED, 0) ;

            SendMessage (hCapture, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Don't Show Buffer and Don't Show Captured Piece") ;
            SendMessage (hCapture, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Show Buffer and Don't Show Captured Piece") ;
            SendMessage (hCapture, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Show Buffer and Show Captured Piece") ;
            SendMessage (hCapture, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            switch (User.nShowCapturedChessPiece)
                {
                case 0 :
                    strcpy (cTmp, "Don't Show Buffer and Don't Show Captured Piece") ;
                    break ;

                case 1 :
                    strcpy (cTmp, "Show Buffer and Don't Show Captured Piece") ;
                    break ;

                case 2 :
                    strcpy (cTmp, "Show Buffer and Show Captured Piece") ;
                    break ;

                default :
                    User.nShowCapturedChessPiece = 0 ;
                    strcpy (cTmp, "Don't Show Buffer and Don't Show Captured Piece") ;
                    break ;
                }

            if (SendMessage (hCapture, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hCapture, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hCapture, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendMessage (hPlayM, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Don't Show My King Square Highlight") ;
            SendMessage (hPlayM, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Always Show My King Square Highlight") ;
            SendMessage (hPlayM, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Show My King Square Highlight (Check & Click)") ;
            SendMessage (hPlayM, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            switch (User.nShowKingHighlightPM)
                {
                case 0 :
                    strcpy (cTmp, "Don't Show My King Square Highlight") ;
                    break ;

                case 1 :
                    strcpy (cTmp, "Always Show My King Square Highlight") ;
                    break ;

                case 2 :
                    strcpy (cTmp, "Show My King Square Highlight (Check & Click)") ;
                    break ;

                default :
                    User.nShowKingHighlightPM = 0 ;
                    strcpy (cTmp, "Don't Show My King Square Highlight") ;
                    break ;
                }

            if (SendMessage (hPlayM, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hPlayM, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hPlayM, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendMessage (hPlayO, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Don't Show Opponent King Square Highlight") ;
            SendMessage (hPlayO, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Always Show Opponent King Square Highlight") ;
            SendMessage (hPlayO, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Show Opponent King Square Highlight (Check & Click)") ;
            SendMessage (hPlayO, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            switch (User.nShowKingHighlightPO)
                {
                case 0 :
                    strcpy (cTmp, "Don't Show Opponent King Square Highlight") ;
                    break ;

                case 1 :
                    strcpy (cTmp, "Always Show Opponent King Square Highlight") ;
                    break ;

                case 2 :
                    strcpy (cTmp, "Show Opponent King Square Highlight (Check & Click)") ;
                    break ;

                default :
                    User.nShowKingHighlightPO = 0 ;
                    strcpy (cTmp, "Don't Show Opponent King Square Highlight") ;
                    break ;
                }

            if (SendMessage (hPlayO, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hPlayO, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hPlayO, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendMessage (hExam, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Don't Show Exam King Square Highlight") ;
            SendMessage (hExam, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Always Show Exam King Square Highlight") ;
            SendMessage (hExam, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Show Exam King Square Highlight (Check & Click)") ;
            SendMessage (hExam, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            switch (User.nShowKingHighlightE)
                {
                case 0 :
                    strcpy (cTmp, "Don't Show Exam King Square Highlight") ;
                    break ;

                case 1 :
                    strcpy (cTmp, "Always Show Exam King Square Highlight") ;
                    break ;

                case 2 :
                    strcpy (cTmp, "Show Exam King Square Highlight (Check & Click)") ;
                    break ;

                default :
                    User.nShowKingHighlightE = 0 ;
                    strcpy (cTmp, "Don't Show Exam King Square Highlight") ;
                    break ;
                }

            if (SendMessage (hExam, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hExam, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hExam, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendMessage (hObserve, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Don't Show Observe King Square Highlight") ;
            SendMessage (hObserve, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Always Show Observe King Square Highlight") ;
            SendMessage (hObserve, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Show Observe King Square Highlight (Check)") ;
            SendMessage (hObserve, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            switch (User.nShowKingHighlightO)
                {
                case 0 :
                    strcpy (cTmp, "Don't Show Observe King Square Highlight") ;
                    break ;

                case 1 :
                    strcpy (cTmp, "Always Show Observe King Square Highlight") ;
                    break ;

                case 2 :
                    strcpy (cTmp, "Show Observe King Square Highlight (Check)") ;
                    break ;

                default :
                    User.nShowKingHighlightO = 0 ;
                    strcpy (cTmp, "Don't Show Observe King Square Highlight") ;
                    break ;
                }

            if (SendMessage (hObserve, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hObserve, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hObserve, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendMessage (hBuffer, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Left") ;
            SendMessage (hBuffer, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Right") ;
            SendMessage (hBuffer, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Top and Bottom (Left)") ;
            SendMessage (hBuffer, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Top and Bottom (Right)") ;
            SendMessage (hBuffer, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            switch (User.nBufferOrientation)
                {
                case DEFAULT_BUFFER_LEFT :
                    strcpy (cTmp, "Left") ;
                    break ;

                case DEFAULT_BUFFER_RIGHT :
                    strcpy (cTmp, "Right") ;
                    break ;

                case DEFAULT_BUFFER_TOPBOTTOML:
                    strcpy (cTmp, "Top and Bottom (Left)") ;
                    break ;

                case DEFAULT_BUFFER_TOPBOTTOMR :
                    strcpy (cTmp, "Top and Bottom (Right)") ;
                    break ;

                default :
                    User.nBufferOrientation = DEFAULT_BUFFER_RIGHT ;
                    strcpy (cTmp, "Right") ;
                    break ;
                }

            if (SendMessage (hBuffer, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hBuffer, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hBuffer, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDOK :
                    User.bShowResult             = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_1,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowRating             = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_2,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowLagStat            = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_3,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowPtell              = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_4,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowCoord              = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_5,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowGameType           = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_6,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowGameTypeOnTitleBar = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_7,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowValueOnTitleBar    = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_8,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowLastMove           = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_9,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowLastMoveTenth      = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_10, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowHighlight          = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_11, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowTPHighlight        = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_12, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bShowClockOnTop         = ((SendDlgItemMessage (hwnd, IDD_GDISPLAY_13, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

                    nI = SendMessage (hCapture, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        User.nShowCapturedChessPiece = 0 ;
                        }
                    else
                        {
                        User.nShowCapturedChessPiece = nI ;
                        }

                    nI = SendMessage (hPlayM, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        User.nShowKingHighlightPM = 0 ;
                        }
                    else
                        {
                        User.nShowKingHighlightPM = nI ;
                        }

                    nI = SendMessage (hPlayO, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        User.nShowKingHighlightPO = 0 ;
                        }
                    else
                        {
                        User.nShowKingHighlightPO = nI ;
                        }

                    nI = SendMessage (hExam, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        User.nShowKingHighlightE = 0 ;
                        }
                    else
                        {
                        User.nShowKingHighlightE = nI ;
                        }

                    nI = SendMessage (hObserve, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        User.nShowKingHighlightO = 0 ;
                        }
                    else
                        {
                        User.nShowKingHighlightO = nI ;
                        }

                    User.bShowKingHighlight = (User.nShowKingHighlightPM ||
                                               User.nShowKingHighlightPO ||
                                               User.nShowKingHighlightE  ||
                                               User.nShowKingHighlightO) ;

                    nI = SendMessage (hBuffer, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        User.nBufferOrientation = DEFAULT_BUFFER_RIGHT ;
                        }
                    else
                        {
                        User.nBufferOrientation = nI ;
                        }

                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDCANCEL :
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDD_GDISPLAY_20 :
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_1,  BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_2,  BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_3,  BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_4,  BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_5,  BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_6,  BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_7,  BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_8,  BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_9,  BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_10, BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_11, BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_12, BM_SETCHECK, BST_CHECKED,   0) ;
                    SendDlgItemMessage (hwnd, IDD_GDISPLAY_13, BM_SETCHECK, BST_UNCHECKED, 0) ;

                    strcpy (cTmp, "Don't Show Buffer and Don't Show Captured Piece") ;
                    if (SendMessage (hCapture, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hCapture, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hCapture, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    strcpy (cTmp, "Don't Show My King Square Highlight") ;
                    if (SendMessage (hPlayM, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hPlayM, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hPlayM, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    strcpy (cTmp, "Don't Show Opponent King Square Highlight") ;
                    if (SendMessage (hPlayO, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hPlayO, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hPlayO, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    strcpy (cTmp, "Don't Show Exam King Square Highlight") ;
                    if (SendMessage (hExam, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hExam, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hExam, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    strcpy (cTmp, "Don't Show Observe King Square Highlight") ;
                    if (SendMessage (hObserve, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hObserve, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hObserve, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }

                    strcpy (cTmp, "Right") ;
                    if (SendMessage (hBuffer, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hBuffer, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hBuffer, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }
                    return TRUE ;
                }
            break ;
        }
    return FALSE ;
    }
