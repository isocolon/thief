#include "thief.h"

BOOL CALLBACK SitPieceBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static HWND hMove ;
    char cTmp [2048] ;
    int nL, bOK ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            hMove = GetDlgItem (hwnd, IDD_SITPIECE_DROP_MOVE) ;

            strcpy (cTmp, F8Key.cSitPieceDropMove) ;
            SendDlgItemMessage (hwnd, IDD_SITPIECE_DROP_MOVE, WM_SETTEXT, (WPARAM) strlen (cTmp), (LPARAM) cTmp) ;

            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDOK :
                    SendDlgItemMessage (hwnd, IDD_SITPIECE_DROP_MOVE, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp) ;

                    TOOLBOX_AllTrim (cTmp) ;
                    cTmp [4] = NULL_CHAR ;

                    nL = strlen (cTmp) ;
                    if (nL > 0)
                        {
                        if (nL != 4)
                            {
                            TOOLBOX_Error ("Invalid Move String.") ;
                            SetFocus (hMove) ;
                            return FALSE ;
                            }

                        switch (cTmp [0])
                            {
                            case ICS_WHITE_QUEEN :
                            case ICS_BLACK_QUEEN :
                            case ICS_WHITE_ROOK :
                            case ICS_BLACK_ROOK :
                            case ICS_WHITE_BISHOP :
                            case ICS_BLACK_BISHOP :
                            case ICS_WHITE_KNIGHT :
                            case ICS_BLACK_KNIGHT :
                            case ICS_WHITE_PAWN :
                            case ICS_BLACK_PAWN :
                                bOK = 1 ;
                                break ;

                            default :
                                bOK = 0 ;
                                break ;
                            }

                        if (! bOK)
                            {
                            TOOLBOX_Error ("Invalid Piece.") ;
                            SetFocus (hMove) ;
                            return FALSE ;
                            }

                        if (cTmp [1] != '@')
                            {
                            TOOLBOX_Error ("Invalid Move String.") ;
                            SetFocus (hMove) ;
                            return FALSE ;
                            }

                        if (((cTmp [2] >= 'a') && (cTmp [2] <= 'h')) ||
                            ((cTmp [2] >= 'A') && (cTmp [2] <= 'H')))
                            {
                            }
                        else
                            {
                            TOOLBOX_Error ("Invalid Move String.") ;
                            SetFocus (hMove) ;
                            return FALSE ;
                            }

                        if ((cTmp [3] >= '1') && (cTmp [3] <= '8'))
                            {
                            }
                        else
                            {
                            TOOLBOX_Error ("Invalid Move String.") ;
                            SetFocus (hMove) ;
                            return FALSE ;
                            }
                        }

                    strcpy (F8Key.cSitPieceDropMove, cTmp) ;

                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;

                    sprintf (cTmp, "Sit For Piece Drop Move Set to %s\n", F8Key.cSitPieceDropMove) ;
                    TOOLBOX_WriteSystem (cTmp) ;
                    return TRUE ;

                case IDCANCEL :
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;
                }
            break ;
        }
    return FALSE ;
    }
