#include "thief.h"

BOOL CALLBACK SaveGameBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static HWND hSave ;
    FILE *Fv ;
    char cTmp [_MAX_PATH] ;
    int nI ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            hSave = GetDlgItem (hwnd, IDD_SAVEGAME_6) ;

            SendMessage (hSave, CB_RESETCONTENT, 0, 0) ;

            strcpy (cTmp, "Save as Daily Game File") ;
            SendMessage (hSave, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            strcpy (cTmp, "Save as One Game File") ;
            SendMessage (hSave, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

            if (User.bSaveDaily)
                {
                strcpy (cTmp, "Save as Daily Game File") ;
                }
            else
                {
                strcpy (cTmp, "Save as One Game File") ;
                }

            if (SendMessage (hSave, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                {
                SendMessage (hSave, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                SendMessage (hSave, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                }

            SendDlgItemMessage (hwnd, IDD_SAVEGAME_1, BM_SETCHECK, User.bSavePlayGame      ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_SAVEGAME_2, BM_SETCHECK, User.bSaveObserveGame   ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_SAVEGAME_3, BM_SETCHECK, User.bSaveUnobserveGame ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_SAVEGAME_4, BM_SETCHECK, User.bSaveAbortGame     ? BST_CHECKED : BST_UNCHECKED, 0) ;
            SendDlgItemMessage (hwnd, IDD_SAVEGAME_5, BM_SETCHECK, User.bSaveNoBFENBugGame ? BST_CHECKED : BST_UNCHECKED, 0) ;

            SendDlgItemMessage (hwnd, IDD_SAVEGAME_7, WM_SETTEXT, (WPARAM) strlen (User.cPGNViewer),  (LPARAM) User.cPGNViewer ) ;
            SendDlgItemMessage (hwnd, IDD_SAVEGAME_9, WM_SETTEXT, (WPARAM) strlen (User.cBPGNViewer), (LPARAM) User.cBPGNViewer) ;

            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDOK :
                    User.bSavePlayGame      = ((SendDlgItemMessage (hwnd, IDD_SAVEGAME_1, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bSaveObserveGame   = ((SendDlgItemMessage (hwnd, IDD_SAVEGAME_2, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bSaveUnobserveGame = ((SendDlgItemMessage (hwnd, IDD_SAVEGAME_3, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bSaveAbortGame     = ((SendDlgItemMessage (hwnd, IDD_SAVEGAME_4, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
                    User.bSaveNoBFENBugGame = ((SendDlgItemMessage (hwnd, IDD_SAVEGAME_5, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

                    SendDlgItemMessage (hwnd, IDD_SAVEGAME_7, WM_GETTEXT, (WPARAM) _MAX_PATH, (LPARAM) cTmp) ;
                    strcpy (User.cPGNViewer, cTmp) ;

                    SendDlgItemMessage (hwnd, IDD_SAVEGAME_9, WM_GETTEXT, (WPARAM) _MAX_PATH, (LPARAM) cTmp) ;
                    strcpy (User.cBPGNViewer, cTmp) ;

                    nI = SendMessage (hSave, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
                    if (nI == CB_ERR)
                        {
                        User.bSaveDaily = 1 ;
                        }
                    else
                        {
                        switch (nI)
                            {
                            case 0 :  User.bSaveDaily = 1 ; break ;
                            case 1 :  User.bSaveDaily = 0 ; break ;
                            default : User.bSaveDaily = 1 ; break ;
                            }
                        }

                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDCANCEL :
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDD_SAVEGAME_8 :
                    Fv = TOOLBOX_OpenFileDialog (hwnd, FALSE, NULL, "exe", PGN_VIEWER_FILT, "PGN Viewer", NULL, NULL, cTmp, Browse.cPGNBrowse) ;
                    if (Fv != NULL)
                        {
                        fclose (Fv) ;
                        SendDlgItemMessage (hwnd, IDD_SAVEGAME_7, WM_SETTEXT, 0, (LPARAM) cTmp) ;
                        TOOLBOX_GetPath (cTmp, Browse.cPGNBrowse) ;
                        INI_WriteSystem (INI_GetSysFilename ()) ;
                        }
                    break ;

                case IDD_SAVEGAME_10 :
                    Fv = TOOLBOX_OpenFileDialog (hwnd, FALSE, NULL, "exe", BPGN_VIEWER_FILT, "BPGN Viewer", NULL, NULL, cTmp, Browse.cBPGNBrowse) ;
                    if (Fv != NULL)
                        {
                        fclose (Fv) ;
                        SendDlgItemMessage (hwnd, IDD_SAVEGAME_9, WM_SETTEXT, 0, (LPARAM) cTmp) ;
                        TOOLBOX_GetPath (cTmp, Browse.cBPGNBrowse) ;
                        INI_WriteSystem (INI_GetSysFilename ()) ;
                        }
                    break ;

                case IDD_SAVEGAME_11 :
                    SendDlgItemMessage (hwnd, IDD_SAVEGAME_1, BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_SAVEGAME_2, BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_SAVEGAME_3, BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_SAVEGAME_4, BM_SETCHECK, BST_UNCHECKED, 0) ;
                    SendDlgItemMessage (hwnd, IDD_SAVEGAME_5, BM_SETCHECK, BST_CHECKED,   0) ;

                    strcpy (cTmp, "Save as Daily Game File") ;
                    if (SendMessage (hSave, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
                        {
                        SendMessage (hSave, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
                        SendMessage (hSave, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
                        }
                    return TRUE ;
                }
            break ;
        }
    return FALSE ;
    }

void SAVEGAME_GetFilename (int nG, int bExam, int bClose, int bUnknown, char *cFn)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    char cDate [20], cTmp [255] ;
    int nI ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    nI = strlen (Dir) ;
    if (nI == 0)
        {
        if (nG == INDEX_PLAY)
            {
            strcat (Dir, "\\GAME_LOGS\\MY_GAMES\\") ;
            }
        else
        if (bExam || bClose || bUnknown)
            {
            strcat (Dir, "\\GAME_LOGS\\OTHER_GAMES\\") ;
            }
        else
            {
            strcat (Dir, "\\GAME_LOGS\\OBSERVED_GAMES\\") ;
            }
        }
    else
        {
        if (Dir [nI - 1] == '\\')
            {
            if (nG == INDEX_PLAY)
                {
                strcat (Dir, "GAME_LOGS\\MY_GAMES\\") ;
                }
            else
            if (bExam || bClose || bUnknown)
                {
                strcat (Dir, "GAME_LOGS\\OTHER_GAMES\\") ;
                }
            else
                {
                strcat (Dir, "GAME_LOGS\\OBSERVED_GAMES\\") ;
                }
            }
        else
            {
            if (nG == INDEX_PLAY)
                {
                strcat (Dir, "\\GAME_LOGS\\MY_GAMES\\") ;
                }
            else
            if (bExam || bClose || bUnknown)
                {
                strcat (Dir, "\\GAME_LOGS\\OTHER_GAMES\\") ;
                }
            else
                {
                strcat (Dir, "\\GAME_LOGS\\OBSERVED_GAMES\\") ;
                }
            }
        }

    _strdate (cDate) ;  // MM/DD/YY

    strcpy (File, Login.cServerName) ;

    if (Game [nG].nGameType == GAMETYPE_BUGHOUSE)
        {
        strcpy (cTmp, "Bughouse") ;
        }
    else
    if (Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
        {
        strcpy (cTmp, "Crazyhouse") ;
        }
    else
        {
        strcpy (cTmp, Game [nG].cOrgGameType) ;
        cTmp [0] = toupper (cTmp [0]) ;
        }

    strcat (File, cTmp) ;

    if (User.bSaveDaily)
        {
        cTmp [0] = cDate [6] ;  // Y
        cTmp [1] = cDate [7] ;  // Y
        cTmp [2] = cDate [0] ;  // M
        cTmp [3] = cDate [1] ;  // M
        cTmp [4] = cDate [3] ;  // D
        cTmp [5] = cDate [4] ;  // D
        cTmp [6] = NULL_CHAR ;

        strcat (File, cTmp) ;
        }

    if ((Game [nG].nGameType == GAMETYPE_BUGHOUSE  ) ||
        (Game [nG].nGameType == GAMETYPE_CRAZYHOUSE))
        {
        strcpy (Ext, BPGN_EXT) ;
        }
    else
        {
        strcpy (Ext, PGN_EXT) ;
        }

    _makepath (cFn, Drive, Dir, File, Ext) ;
    }

char * SAVEGAME_GetFEN (int nG)
    {
    static char cBuffer [255] ;

    int nY, nX, nI, nP, nS ;
    char cTmp [255], cTmp1 [255] ;

    strcpy (cTmp, "") ;

    for (nY = 7 ; nY >= 0 ; nY--)
        {
        nI = 0 ;
        nS = 0 ;
        for (nX = 0 ; nX < 8 ; nX++)
            {
            nP = Game [nG].nInitBoard [nX] [nY] ;
            if (nP == EMPTY_SQUARE)
                {
                nS = nS + 1 ;
                }
            else
            if ((nP >= WHITE_PAWN) && (nP <= BLACK_KING))
                {
                if (nS > 0)
                    {
                    cTmp1 [nI++] = (char) (nS + '0') ;
                    nS = 0 ;
                    }
                cTmp1 [nI++] = ICSPiece [nP] ;
                }
            }
        if (nS > 0)
            {
            cTmp1 [nI++] = (char) (nS + '0') ;
            }

        if (nY > 0)
            {
            cTmp1 [nI++] = '/' ;
            }

        cTmp1 [nI] = NULL_CHAR ;

        strcat (cTmp, cTmp1) ;
        }

    if (Game [nG].bInitWhitesMove)
        {
        strcat (cTmp, " w ") ;
        }
    else
        {
        strcat (cTmp, " b ") ;
        }

    nI = 0 ;
    if (Game [nG].bInitKingSide [INDEX_WHITE])
        {
        cTmp1 [nI++] = 'K' ;
        }
    if (Game [nG].bInitQueenSide [INDEX_WHITE])
        {
        cTmp1 [nI++] = 'Q' ;
        }
    if (Game [nG].bInitKingSide [INDEX_BLACK])
        {
        cTmp1 [nI++] = 'k' ;
        }
    if (Game [nG].bInitQueenSide [INDEX_BLACK])
        {
        cTmp1 [nI++] = 'q' ;
        }
    if (nI == 0)
        {
        strcat (cTmp, "-") ;
        }
    else
        {
        cTmp1 [nI] = NULL_CHAR ;
        strcat (cTmp, cTmp1) ;
        }

    if (Game [nG].nInitDoublePushFile == -1)
        {
        strcat (cTmp, " - ") ;
        }
    else
        {
        cTmp1 [0] = ' ' ;
        cTmp1 [1] = (char) ('a' + Game [nG].nInitDoublePushFile) ;

        if (Game [nG].bInitWhitesMove)
            {
            cTmp1 [2] = '6' ;
            }
        else
            {
            cTmp1 [2] = '3' ;
            }
        cTmp1 [3] = ' ' ;
        cTmp1 [4] = NULL_CHAR ;

        strcat (cTmp, cTmp1) ;
        }

    sprintf (cTmp1, "%d ", Game [nG].nInitHalfMoves) ;
    strcat (cTmp, cTmp1) ;

    if (Game [nG].bInitInitialMove)
        {
        if (Game [nG].nInitMoveNumber == 0)
            {
            strcpy (cTmp1, "1") ;
            }
        else
            {
            sprintf (cTmp1, "%d", Game [nG].nInitMoveNumber) ;
            }
        }
    else
    if (Game [nG].bInitWhitesMove)
        {
        sprintf (cTmp1, "%d", Game [nG].nInitMoveNumber - 1) ;
        }
    else
        {
        sprintf (cTmp1, "%d", Game [nG].nInitMoveNumber) ;
        }

    strcat (cTmp, cTmp1) ;
    strcpy (cBuffer, cTmp) ;

    return cBuffer ;
    }

char * SAVEGAME_GetBFEN (int nI)
    {
    static char cBuffer [512] ;

    int nY, nX, nK, nS, nP, bP ;
    char cTmp [512], cTmp1 [512] ;

    //
    // main board
    //
    strcpy (cTmp, "") ;

    for (nY = 7 ; nY >= 0 ; nY--)
        {
        nK = 0 ;
        nS = 0 ;
        for (nX = 0 ; nX < 8 ; nX++)
            {
            nP = Bughouse [nI].nMainBoard   [nX] [nY] ;
            bP = Bughouse [nI].nMainPromote [nX] [nY] ;

            if (nP == EMPTY_SQUARE)
                {
                nS = nS + 1 ;
                }
            else
            if ((nP >= WHITE_PAWN) && (nP <= BLACK_KING))
                {
                if (nS > 0)
                    {
                    cTmp1 [nK++] = (char) (nS + '0') ;
                    nS = 0 ;
                    }
                cTmp1 [nK++] = ICSPiece [nP] ;

                if (bP)
                    {
                    cTmp1 [nK++] = '~' ;
                    }
                }
            }
        if (nS > 0)
            {
            cTmp1 [nK++] = (char) (nS + '0') ;
            }

        if (nY > 0)
            {
            cTmp1 [nK++] = '/' ;
            }

        cTmp1 [nK] = NULL_CHAR ;

        strcat (cTmp, cTmp1) ;
        }

    nK = 0 ;
    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
        {
        nP = Bughouse [nI].nMainBuffer [nX] ;

        if (nP > 0)
            {
            for (nY = 0 ; nY < nP ; nY++)
                {
                cTmp1 [nK++] = ICSPiece [nX] ;
                }
            }
        }

    if (nK > 0)
        {
        cTmp1 [nK] = NULL_CHAR ;

        strcat (cTmp, "/") ;
        strcat (cTmp, cTmp1) ;
        }

    if (Bughouse [nI].bMainWhitesMove)
        {
        strcat (cTmp, " w ") ;
        }
    else
        {
        strcat (cTmp, " b ") ;
        }

    nK = 0 ;
    if (Bughouse [nI].bMainCastleKingSide [INDEX_WHITE])
        {
        cTmp1 [nK++] = 'K' ;
        }
    if (Bughouse [nI].bMainCastleQueenSide [INDEX_WHITE])
        {
        cTmp1 [nK++] = 'Q' ;
        }
    if (Bughouse [nI].bMainCastleKingSide [INDEX_BLACK])
        {
        cTmp1 [nK++] = 'k' ;
        }
    if (Bughouse [nI].bMainCastleQueenSide [INDEX_BLACK])
        {
        cTmp1 [nK++] = 'q' ;
        }
    if (nK == 0)
        {
        strcat (cTmp, "-") ;
        }
    else
        {
        cTmp1 [nK] = NULL_CHAR ;
        strcat (cTmp, cTmp1) ;
        }

    if (Bughouse [nI].nMainLastDoublePushFile == -1)
        {
        strcat (cTmp, " - ") ;
        }
    else
        {
        cTmp1 [0] = ' ' ;
        cTmp1 [1] = (char) ('a' + Bughouse [nI].nMainLastDoublePushFile) ;

        if (Bughouse [nI].bMainWhitesMove)
            {
            cTmp1 [2] = '6' ;
            }
        else
            {
            cTmp1 [2] = '3' ;
            }
        cTmp1 [3] = ' ' ;
        cTmp1 [4] = NULL_CHAR ;

        strcat (cTmp, cTmp1) ;
        }

    sprintf (cTmp1,
             "%d %d",
             (int) (Bughouse [nI].nMainTimeRemaining [INDEX_WHITE] / 1000),
             (int) (Bughouse [nI].nMainTimeRemaining [INDEX_BLACK] / 1000)) ;

    strcat (cTmp,    cTmp1) ;
    strcpy (cBuffer, cTmp ) ;
    strcat (cBuffer, " | ") ;

    //
    // partner's board
    //
    strcpy (cTmp, "") ;

    for (nY = 7 ; nY >= 0 ; nY--)
        {
        nK = 0 ;
        nS = 0 ;
        for (nX = 0 ; nX < 8 ; nX++)
            {
            nP = Bughouse [nI].nPartnerBoard   [nX] [nY] ;
            bP = Bughouse [nI].nPartnerPromote [nX] [nY] ;

            if (nP == EMPTY_SQUARE)
                {
                nS = nS + 1 ;
                }
            else
            if ((nP >= WHITE_PAWN) && (nP <= BLACK_KING))
                {
                if (nS > 0)
                    {
                    cTmp1 [nK++] = (char) (nS + '0') ;
                    nS = 0 ;
                    }
                cTmp1 [nK++] = ICSPiece [nP] ;

                if (bP)
                    {
                    cTmp1 [nK++] = '~' ;
                    }
                }
            }
        if (nS > 0)
            {
            cTmp1 [nK++] = (char) (nS + '0') ;
            }

        if (nY > 0)
            {
            cTmp1 [nK++] = '/' ;
            }

        cTmp1 [nK] = NULL_CHAR ;

        strcat (cTmp, cTmp1) ;
        }

    nK = 0 ;
    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
        {
        nP = Bughouse [nI].nPartnerBuffer [nX] ;

        if (nP > 0)
            {
            for (nY = 0 ; nY < nP ; nY++)
                {
                cTmp1 [nK++] = ICSPiece [nX] ;
                }
            }
        }

    if (nK > 0)
        {
        cTmp1 [nK] = NULL_CHAR ;

        strcat (cTmp, "/") ;
        strcat (cTmp, cTmp1) ;
        }

    if (Bughouse [nI].bPartnerWhitesMove)
        {
        strcat (cTmp, " w ") ;
        }
    else
        {
        strcat (cTmp, " b ") ;
        }

    nK = 0 ;
    if (Bughouse [nI].bPartnerCastleKingSide [INDEX_WHITE])
        {
        cTmp1 [nK++] = 'K' ;
        }
    if (Bughouse [nI].bPartnerCastleQueenSide [INDEX_WHITE])
        {
        cTmp1 [nK++] = 'Q' ;
        }
    if (Bughouse [nI].bPartnerCastleKingSide [INDEX_BLACK])
        {
        cTmp1 [nK++] = 'k' ;
        }
    if (Bughouse [nI].bPartnerCastleQueenSide [INDEX_BLACK])
        {
        cTmp1 [nK++] = 'q' ;
        }
    if (nK == 0)
        {
        strcat (cTmp, "-") ;
        }
    else
        {
        cTmp1 [nK] = NULL_CHAR ;
        strcat (cTmp, cTmp1) ;
        }

    if (Bughouse [nI].nPartnerLastDoublePushFile == -1)
        {
        strcat (cTmp, " - ") ;
        }
    else
        {
        cTmp1 [0] = ' ' ;
        cTmp1 [1] = (char) ('a' + Bughouse [nI].nPartnerLastDoublePushFile) ;

        if (Bughouse [nI].bPartnerWhitesMove)
            {
            cTmp1 [2] = '6' ;
            }
        else
            {
            cTmp1 [2] = '3' ;
            }
        cTmp1 [3] = ' ' ;
        cTmp1 [4] = NULL_CHAR ;

        strcat (cTmp, cTmp1) ;
        }

    sprintf (cTmp1,
             "%d %d",
             (int) (Bughouse [nI].nPartnerTimeRemaining [INDEX_WHITE] / 1000),
             (int) (Bughouse [nI].nPartnerTimeRemaining [INDEX_BLACK] / 1000)) ;

    strcat (cTmp,    cTmp1) ;
    strcat (cBuffer, cTmp ) ;

    //
    // result
    //
    return cBuffer ;
    }

char * SAVEGAME_GetPGNHeader (int nG, char *cResult)
    {
    static char cBuffer [2048] ;

    char cDate [20], cTmp [1024], cTmp1 [1024], *cP, *cQ ;

    int nL ;

    switch (Login.nLoginType)
        {
        case SERVER_ICC :
            if (Game [nG].nRated == 1)
                {
                sprintf (cTmp, "[Event \"icc rated %s match\"]\n[Site \"icc\"]\n", Game [nG].cOrgGameType) ;
                }
            else
                {
                sprintf (cTmp, "[Event \"icc unrated %s match\"]\n[Site \"icc\"]\n", Game [nG].cOrgGameType) ;
                }
            break ;

        case SERVER_NONFICS :
            if (Game [nG].nRated == 1)
                {
                sprintf (cTmp, "[Event \"nonfics rated %s match\"]\n[Site \"nonfics\"]\n", Game [nG].cOrgGameType) ;
                }
            else
                {
                sprintf (cTmp, "[Event \"nonfics unrated %s match\"]\n[Site \"nonfics\"]\n", Game [nG].cOrgGameType) ;
                }
            break ;

        default :
            if (Game [nG].nRated == 1)
                {
                sprintf (cTmp, "[Event \"fics rated %s match\"]\n[Site \"fics\"]\n", Game [nG].cOrgGameType) ;
                }
            else
                {
                sprintf (cTmp, "[Event \"fics unrated %s match\"]\n[Site \"fics\"]\n", Game [nG].cOrgGameType) ;
                }
            break ;
        }
    strcpy (cBuffer, cTmp) ;

    _strdate (cDate) ;  // MM/DD/YY

    cTmp [0]  = '2' ;
    cTmp [1]  = '0' ;
    cTmp [2]  = cDate [6] ;  // Y
    cTmp [3]  = cDate [7] ;  // Y
    cTmp [4]  = '.' ;
    cTmp [5]  = cDate [0] ;  // M
    cTmp [6]  = cDate [1] ;  // M
    cTmp [7]  = '.' ;
    cTmp [8]  = cDate [3] ;  // D
    cTmp [9]  = cDate [4] ;  // D
    cTmp [10] = NULL_CHAR ;

    sprintf (cTmp1, "[Date \"%s\"]\n[White \"%s\"]\n[Black \"%s\"]\n",
             cTmp,
             Game [nG].cHandle [INDEX_WHITE],
             Game [nG].cHandle [INDEX_BLACK]) ;
    strcat (cBuffer, cTmp1) ;

    if (strlen (Game [nG].cRating [INDEX_WHITE]) == 0)
        {
        strcat (cBuffer, "[WhiteElo \"????\"]\n") ;
        }
    else
        {
        cP = strchr (Game [nG].cRating [INDEX_WHITE], '(') ;
        cQ = strchr (Game [nG].cRating [INDEX_WHITE], ')') ;
        if (cP && cQ)
            {
            strncpy (cTmp1, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cTmp1) ;
            if (cTmp1 [nL - 1] == ')')
                {
                cTmp1 [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cTmp1, Game [nG].cRating [INDEX_WHITE]) ;
            }

        sprintf (cTmp, "[WhiteElo \"%s\"]\n", cTmp1) ;
        strcat (cBuffer, cTmp) ;
        }

    if (strlen (Game [nG].cRating [INDEX_BLACK]) == 0)
        {
        strcat (cBuffer, "[BlackElo \"????\"]\n") ;
        }
    else
        {
        cP = strchr (Game [nG].cRating [INDEX_BLACK], '(') ;
        cQ = strchr (Game [nG].cRating [INDEX_BLACK], ')') ;
        if (cP && cQ)
            {
            strncpy (cTmp1, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cTmp1) ;
            if (cTmp1 [nL - 1] == ')')
                {
                cTmp1 [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cTmp1, Game [nG].cRating [INDEX_BLACK]) ;
            }

        sprintf (cTmp, "[BlackElo \"%s\"]\n", cTmp1) ;
        strcat (cBuffer, cTmp) ;
        }

    sprintf (cTmp, "[TimeControl \"%d+%d\"]\n", Game [nG].nInitialClock * 60, Game [nG].nIncrementClock) ;
    strcat (cBuffer, cTmp) ;
    strcat (cBuffer, "[Mode \"ICS\"]\n") ;

    if (strlen (Game [nG].cOrgResult) == 0)
        {
        strcat (cBuffer, "[Result \"*\"]\n") ;
        strcpy (cResult, "*") ;
        }
    else
        {
        cP = strchr (Game [nG].cOrgResult, '}') ;
        if (cP)
            {
            if (*(cP + 1) == ' ')
                {
                strcpy (cTmp, cP + 1) ;
                TOOLBOX_AllTrim (cTmp) ;

                nL = strlen (cTmp) ;
                if (cTmp [nL - 1] == '\n')
                    {
                    cTmp [nL - 1] = NULL_CHAR ;
                    }

                if (strstr (cTmp, RESULT_WHITE_WINS) ||
                    strstr (cTmp, RESULT_BLACK_WINS) ||
                    strstr (cTmp, RESULT_DRAW      ) ||
                    strstr (cTmp, RESULT_UNKNOWN   ))
                    {
                    strcat (cBuffer, "[Result \"") ;
                    strcat (cBuffer, cTmp) ;
                    strcat (cBuffer, "\"]\n") ;

                    strcpy (cResult, cTmp) ;
                    }
                else
                    {
                    strcat (cBuffer, "[Result \"*\"]\n") ;
                    strcpy (cResult, "*") ;
                    }
                }
            else
                {
                strcat (cBuffer, "[Result \"*\"]\n") ;
                strcpy (cResult, "*") ;
                }
            }
        else
            {
            strcat (cBuffer, "[Result \"*\"]\n") ;
            strcpy (cResult, "*") ;
            }
        }

    strcpy (cTmp, SAVEGAME_GetFEN (nG)) ;
    if (strcmp (cTmp, INIT_FEN) == 0)
        {
        strcat (cBuffer, "\n") ;
        }
    else
        {
        strcat (cBuffer, "[Setup \"1\"]\n[FEN \"") ;
        strcat (cBuffer, cTmp) ;
        strcat (cBuffer, "\"]\n\n") ;
        }

    return cBuffer ;
    }

void SAVEGAME_SavePGN (int nG, int bExam, int bClose)
    {
    char cFn [_MAX_PATH + 10], cResult [500], cLine [1024], cMove [500], *cP ;
    FILE *Fv ;
    int bHasResult, nI, nJ, nC, bHasMove, bDone ;

    if (System.bCDROMConnection)
        {
        return ;
        }

    //
    // don't save games with 0 moves
    //
    if ((Game [nG].nMinIndex < 0) || (Game [nG].nMaxIndex < 0))
        {
        return ;
        }

    strcpy (System.cSaveBuffer, SAVEGAME_GetPGNHeader (nG, cResult)) ;

    if (! bClose)
        {
        if (! User.bSaveAbortGame)
            {
            if (strcmp (cResult, "*") == 0)
                {
                return ;
                }
            }
        }

    if (strstr (Game [nG].cOrgResult, RESULT_WHITE_WINS) ||
        strstr (Game [nG].cOrgResult, RESULT_BLACK_WINS) ||
        strstr (Game [nG].cOrgResult, RESULT_DRAW      ) ||
        strstr (Game [nG].cOrgResult, RESULT_UNKNOWN   ))
        {
        SAVEGAME_GetFilename (nG, bExam, bClose, (strstr (Game [nG].cOrgResult, RESULT_UNKNOWN) ? 1 : 0), cFn) ;
        bHasResult = 1 ;            
        }
    else
        {
        SAVEGAME_GetFilename (nG, bExam, bClose, 1, cFn) ;
        bHasResult = 0 ;
        }

    if ((Game [nG].nMinIndex < 0) || (Game [nG].nMaxIndex < 0))
        {
        bHasMove = 0 ;
        nJ       = 0 ;
        }
    else
        {
        bHasMove = 1 ;
        nJ       = Game [nG].nMinIndex ;
        }

    if (bHasMove)
        {
        if (Game [nG].nMinColor < 0)
            {
            nC = 0 ;
            }
        else
            {
            nC = Game [nG].nMinColor ;
            }

        bDone = 0 ;

        if (Game [nG].bInitWhitesMove)
            {
            if (nC == 0)
                {
                // ok
                }
            else
                {
                // should be white's move (skip to next move)
                if (nJ < Game [nG].nMaxIndex)
                    {
                    if (nC == 0)
                        {
                        nC = 1 ;
                        }
                    else
                        {
                        nJ = nJ + 1 ;
                        nC = 0 ;
                        }
                    }
                else
                if (nJ == Game [nG].nMaxIndex)
                    {
                    if (Game [nG].nMaxColor == 0)
                        {
                        bDone = 1 ;
                        }
                    else
                        {
                        if (nC == 0)
                            {
                            nC = 1 ;
                            }
                        else
                            {
                            bDone = 1 ;
                            }
                        }
                    }
                else
                    {
                    bDone = 1 ;
                    }
                }
            }
        else
            {
            if (nC == 0)
                {
                // should be black's move (skip to next move)
                if (nJ < Game [nG].nMaxIndex)
                    {
                    if (nC == 0)
                        {
                        nC = 1 ;
                        }
                    else
                        {
                        nJ = nJ + 1 ;
                        nC = 0 ;
                        }
                    }
                else
                if (nJ == Game [nG].nMaxIndex)
                    {
                    if (Game [nG].nMaxColor == 0)
                        {
                        bDone = 1 ;
                        }
                    else
                        {
                        if (nC == 0)
                            {
                            nC = 1 ;
                            }
                        else
                            {
                            bDone = 1 ;
                            }
                        }
                    }
                else
                    {
                    bDone = 1 ;
                    }
                }
            else
                {
                // ok
                }
            }
            
        if (bDone)
            {
            // done
            }
        else
            {
            if (nC == 0)
                {
                sprintf (cLine, "%d. %s", nJ + 1, Game [nG].Position [nJ].cLastMove [nC]) ;
                }
            else
                {
                sprintf (cLine, "%d... %s", nJ + 1, Game [nG].Position [nJ].cLastMove [nC]) ;
                }

            while (1)
                {
                if (nJ < Game [nG].nMaxIndex)
                    {
                    if (nC == 0)
                        {
                        nC = 1 ;
                        }
                    else
                        {
                        nJ = nJ + 1 ;
                        nC = 0 ;
                        }
                    }
                else
                if (nJ == Game [nG].nMaxIndex)
                    {
                    if (Game [nG].nMaxColor == 0)
                        {
                        break ;
                        }
                    else
                        {
                        if (nC == 0)
                            {
                            nC = 1 ;
                            }
                        else
                            {
                            break ;
                            }
                        }
                    }
                else
                    {
                    break ;
                    }

                if (nC == 0)
                    {
                    sprintf (cMove, "%d. %s", nJ + 1, Game [nG].Position [nJ].cLastMove [nC]) ;
                    }
                else
                    {
                    sprintf (cMove, "%s", Game [nG].Position [nJ].cLastMove [nC]) ;
                    }

                if ((strlen (cLine) + 1 + strlen (cMove)) > 79)
                    {
                    strcat (System.cSaveBuffer, cLine) ;
                    strcat (System.cSaveBuffer, "\n") ;
                    strcpy (cLine, cMove) ;
                    }
                else
                    {
                    strcat (cLine, " ") ;
                    strcat (cLine, cMove) ;
                    }
                }

            strcat (System.cSaveBuffer, cLine) ;
            }

        strcat (System.cSaveBuffer, "\n\n") ;
        }

    if (bHasResult)
        {
        cP = strchr (Game [nG].cOrgResult, ')') ;
        if (cP)
            {
            if (*(cP + 1) == ' ')
                {
                strcpy (cLine, cP + 1) ;
                cLine [0] = '{' ;

                strcat (System.cSaveBuffer, cLine) ;

                nI = strlen (cLine) ;
                if (cLine [nI - 1] != '\n')
                    {
                    strcat (System.cSaveBuffer, "\n") ;
                    }
                }
            else
                {
                if (strlen (Game [nG].cOrgResult) == 0)
                    {
                    strcat (System.cSaveBuffer, "{*} *\n") ;
                    }
                else
                    {
                    strcat (System.cSaveBuffer, Game [nG].cOrgResult) ;

                    nI = strlen (Game [nG].cOrgResult) ;
                    if (Game [nG].cOrgResult [nI - 1] != '\n')
                        {
                        strcat (System.cSaveBuffer, "\n") ;
                        }
                    }
                }
            }
        else
            {
            if (strlen (Game [nG].cOrgResult) == 0)
                {
                strcat (System.cSaveBuffer, "{*} *") ;
                }
            else
                {
                strcat (System.cSaveBuffer, Game [nG].cOrgResult) ;
                }

            nI = strlen (Game [nG].cOrgResult) ;
            if (Game [nG].cOrgResult [nI - 1] != '\n')
                {
                strcat (System.cSaveBuffer, "\n") ;
                }
            }
        }
    else
        {
        strcat (System.cSaveBuffer, "{*} *\n") ;
        }

    Game [nG].bSavedGame = 1 ;

    Fv = fopen (cFn, "a") ;
    if (Fv == NULL)
        {
        sprintf (cLine, "Saving %s Failed\n", cFn) ;
        TOOLBOX_WriteSystem (cLine) ;
        }
    else
        {
        fprintf (Fv, "\n%s", System.cSaveBuffer) ;
        fclose (Fv) ;

        sprintf (cLine, "Saving %s Successful\n", cFn) ;
        TOOLBOX_WriteSystem (cLine) ;
        }
    }

char * SAVEGAME_GetZHHeader (int nG, char *cResult)
    {
    static char cBuffer [2048] ;

    char cDate [20], cTmp [1024], cTmp1 [1024], cWRating [255], cBRating [255], *cP, *cQ ;

    int nL ;

    switch (Login.nLoginType)
        {
        case SERVER_ICC :
            if (Game [nG].nRated == 1)
                {
                strcpy (cTmp, "[Event \"icc rated crazyhouse match\"]\n[Site \"icc\"]\n") ;
                }
            else
                {
                strcpy (cTmp, "[Event \"icc unrated crazyhouse match\"]\n[Site \"icc\"]\n") ;
                }
            break ;

        case SERVER_NONFICS :
            if (Game [nG].nRated == 1)
                {
                strcpy (cTmp, "[Event \"nonfics rated crazyhouse match\"]\n[Site \"nonfics\"]\n") ;
                }
            else
                {
                strcpy (cTmp, "[Event \"nonfics unrated crazyhouse match\"]\n[Site \"nonfics\"]\n") ;
                }
            break ;

        default :
            if (Game [nG].nRated == 1)
                {
                strcpy (cTmp, "[Event \"fics rated crazyhouse match\"]\n[Site \"fics\"]\n") ;
                }
            else
                {
                strcpy (cTmp, "[Event \"fics unrated crazyhouse match\"]\n[Site \"fics\"]\n") ;
                }
            break ;
        }
    strcpy (cBuffer, cTmp) ;

    _strdate (cDate) ;  // MM/DD/YY

    cTmp [0]  = '2' ;
    cTmp [1]  = '0' ;
    cTmp [2]  = cDate [6] ;  // Y
    cTmp [3]  = cDate [7] ;  // Y
    cTmp [4]  = '.' ;
    cTmp [5]  = cDate [0] ;  // M
    cTmp [6]  = cDate [1] ;  // M
    cTmp [7]  = '.' ;
    cTmp [8]  = cDate [3] ;  // D
    cTmp [9]  = cDate [4] ;  // D
    cTmp [10] = NULL_CHAR ;

    sprintf (cTmp1, "[Date \"%s\"]\n", cTmp) ;
    strcat (cBuffer, cTmp1) ;

    if (strlen (Game [nG].cRating [INDEX_WHITE]) == 0)
        {
        strcpy (cWRating, "") ;
        }
    else
        {
        cP = strchr (Game [nG].cRating [INDEX_WHITE], '(') ;
        cQ = strchr (Game [nG].cRating [INDEX_WHITE], ')') ;
        if (cP && cQ)
            {
            strncpy (cWRating, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cWRating) ;
            if (cWRating [nL - 1] == ')')
                {
                cWRating [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cWRating, Game [nG].cRating [INDEX_WHITE]) ;
            }
        }

    if (strlen (Game [nG].cRating [INDEX_BLACK]) == 0)
        {
        strcpy (cBRating, "") ;
        }
    else
        {
        cP = strchr (Game [nG].cRating [INDEX_BLACK], '(') ;
        cQ = strchr (Game [nG].cRating [INDEX_BLACK], ')') ;
        if (cP && cQ)
            {
            strncpy (cBRating, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cBRating) ;
            if (cBRating [nL - 1] == ')')
                {
                cBRating [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cBRating, Game [nG].cRating [INDEX_BLACK]) ;
            }
        }

    if (strlen (cWRating) == 0)
        {
        sprintf (cTmp, "[WhiteA \"%s\"]\n", Game [nG].cHandle [INDEX_WHITE]) ;
        }
    else
        {
        sprintf (cTmp, "[WhiteA \"%s\"][WhiteAElo \"%s\"]\n", Game [nG].cHandle [INDEX_WHITE], cWRating) ;
        }
    strcat (cBuffer, cTmp) ;

    if (strlen (cBRating) == 0)
        {
        sprintf (cTmp, "[BlackA \"%s\"]\n", Game [nG].cHandle [INDEX_BLACK]) ;
        }
    else
        {
        sprintf (cTmp, "[BlackA \"%s\"][BlackAElo \"%s\"]\n", Game [nG].cHandle [INDEX_BLACK], cBRating) ;
        }
    strcat (cBuffer, cTmp) ;

    strcat (cBuffer, "[WhiteB \"WhiteB\"][WhiteBElo \"----\"]\n") ;
    strcat (cBuffer, "[BlackB \"BlackB\"][BlackBElo \"----\"]\n") ;

    sprintf (cTmp, "[TimeControl \"%d+%d\"]\n", Game [nG].nInitialClock * 60, Game [nG].nIncrementClock) ;
    strcat (cBuffer, cTmp) ;

    if (strlen (Game [nG].cOrgResult) == 0)
        {
        strcat (cBuffer, "[Result \"*\"]\n") ;
        strcpy (cResult, "*") ;
        }
    else
        {
        cP = strchr (Game [nG].cOrgResult, '}') ;
        if (cP)
            {
            if (*(cP + 1) == ' ')
                {
                strcpy (cTmp, cP + 1) ;
                TOOLBOX_AllTrim (cTmp) ;

                nL = strlen (cTmp) ;
                if (cTmp [nL - 1] == '\n')
                    {
                    cTmp [nL - 1] = NULL_CHAR ;
                    }

                if (strstr (cTmp, RESULT_WHITE_WINS) ||
                    strstr (cTmp, RESULT_BLACK_WINS) ||
                    strstr (cTmp, RESULT_DRAW      ) ||
                    strstr (cTmp, RESULT_UNKNOWN   ))
                    {
                    strcat (cBuffer, "[Result \"") ;
                    strcat (cBuffer, cTmp) ;
                    strcat (cBuffer, "\"]\n") ;

                    strcpy (cResult, cTmp) ;
                    }
                else
                    {
                    strcat (cBuffer, "[Result \"*\"]\n") ;
                    strcpy (cResult, "*") ;
                    }
                }
            else
                {
                strcat (cBuffer, "[Result \"*\"]\n") ;
                strcpy (cResult, "*") ;
                }
            }
        else
            {
            strcat (cBuffer, "[Result \"*\"]\n") ;
            strcpy (cResult, "*") ;
            }
        }
    strcat (cBuffer, "\n") ;

    return cBuffer ;
    }

void SAVEGAME_SaveZH (int nG, int bExam, int bClose)
    {
    char cFn [_MAX_PATH + 10], cResult [500], cLine [1024], cMove [500], *cP ;
    FILE *Fv ;
    int bHasResult, nI, nJ, nC, bHasMove, nTime, nInc, nWTime, nBTime, nMn, nSc ;
    long nLTime ;

    if (System.bCDROMConnection)
        {
        return ;
        }

    //
    // don't save games with 0 moves
    //
    if ((Game [nG].nMinIndex < 0) || (Game [nG].nMaxIndex < 0))
        {
        return ;
        }

    strcpy (System.cSaveBuffer, SAVEGAME_GetZHHeader (nG, cResult)) ;

    if (! bClose)
        {
        if (! User.bSaveAbortGame)
            {
            if (strcmp (cResult, "*") == 0)
                {
                return ;
                }
            }
        }

    if (strstr (Game [nG].cOrgResult, RESULT_WHITE_WINS) ||
        strstr (Game [nG].cOrgResult, RESULT_BLACK_WINS) ||
        strstr (Game [nG].cOrgResult, RESULT_DRAW      ) ||
        strstr (Game [nG].cOrgResult, RESULT_UNKNOWN   ))
        {
        SAVEGAME_GetFilename (nG, bExam, bClose, (strstr (Game [nG].cOrgResult, RESULT_UNKNOWN) ? 1 : 0), cFn) ;
        bHasResult = 1 ;
        }
    else
        {
        SAVEGAME_GetFilename (nG, bExam, bClose, 1, cFn) ;
        bHasResult = 0 ;
        }

    if ((Game [nG].nMinIndex < 0) || (Game [nG].nMaxIndex < 0))
        {
        bHasMove = 0 ;
        nJ       = 0 ;
        }
    else
        {
        bHasMove = 1 ;
        nJ       = Game [nG].nMinIndex ;
        }

    if (bHasMove)
        {
        nTime = Game [nG].nInitialClock * 60 ;
        nInc  = Game [nG].nIncrementClock ;

        if ((nTime == 0) && (nInc > 0))
            {
            nTime = 10 ;
            }

        nWTime = nTime ;
        nBTime = nTime ;

        if (Game [nG].nMinColor < 0)
            {
            nC = 0 ;
            }
        else
            {
            nC = Game [nG].nMinColor ;
            }

        if (Game [nG].Position [nJ].nTime [nC] == -9999L)
            {
            nMn = 0 ;
            nSc = 0 ;
            sscanf (Game [nG].Position [nJ].cTime4Move [nC], "(%02d:%02d", &nMn, &nSc) ;

            if (nC == 0)
                {
                nWTime = (nWTime - ((nMn * 60) + nSc)) + nInc ;
                if (nWTime < 0)
                    {
                    nWTime = 0 ;
                    }

                sprintf (cLine, "%dA. %s {%d}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nWTime) ;
                }
            else
                {
                nBTime = (nBTime - ((nMn * 60) + nSc)) + nInc ;
                if (nBTime < 0)
                    {
                    nBTime = 0 ;
                    }

                sprintf (cLine, "%da. %s {%d}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nBTime) ;
                }
            }
        else
            {
            nLTime = (long) floor (((double) (Game [nG].Position [nJ].nTime [nC] + 999L)) / 1000.0) ;

            if (nC == 0)
                {
                sprintf (cLine, "%dA. %s {%ld}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nLTime) ;
                }
            else
                {
                sprintf (cLine, "%da. %s {%ld}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nLTime) ;
                }
            }

        while (1)
            {
            if (nJ < Game [nG].nMaxIndex)
                {
                if (nC == 0)
                    {
                    nC = 1 ;
                    }
                else
                    {
                    nJ = nJ + 1 ;
                    nC = 0 ;
                    }
                }
            else
            if (nJ == Game [nG].nMaxIndex)
                {
                if (Game [nG].nMaxColor == 0)
                    {
                    break ;
                    }
                else
                    {
                    if (nC == 0)
                        {
                        nC = 1 ;
                        }
                    else
                        {
                        break ;
                        }
                    }
                }
            else
                {
                break ;
                }

            if (Game [nG].Position [nJ].nTime [nC] == -9999L)
                {
                nMn = 0 ;
                nSc = 0 ;
                sscanf (Game [nG].Position [nJ].cTime4Move [nC], "(%02d:%02d", &nMn, &nSc) ;

                if (nC == 0)
                    {
                    nWTime = (nWTime - ((nMn * 60) + nSc)) + nInc ;
                    if (nWTime < 0)
                        {
                        nWTime = 0 ;
                        }

                    sprintf (cMove, "%dA. %s {%d}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nWTime) ;
                    }
                else
                    {
                    nBTime = (nBTime - ((nMn * 60) + nSc)) + nInc ;
                    if (nBTime < 0)
                        {
                        nBTime = 0 ;
                        }

                    sprintf (cMove, "%da. %s {%d}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nBTime) ;
                    }
                }
            else
                {
                nLTime = (long) floor (((double) (Game [nG].Position [nJ].nTime [nC] + 999L)) / 1000.0) ;

                if (nC == 0)
                    {
                    sprintf (cMove, "%dA. %s {%ld}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nLTime) ;
                    }
                else
                    {
                    sprintf (cMove, "%da. %s {%ld}", nJ + 1, Game [nG].Position [nJ].cLastMove [nC], nLTime) ;
                    }
                }

            if ((strlen (cLine) + 1 + strlen (cMove)) > 79)
                {
                strcat (System.cSaveBuffer, cLine) ;
                strcat (System.cSaveBuffer, "\n") ;
                strcpy (cLine, cMove) ;
                }
            else
                {
                strcat (cLine, " ") ;
                strcat (cLine, cMove) ;
                }
            }

        strcat (System.cSaveBuffer, cLine) ;
        strcat (System.cSaveBuffer, "\n\n") ;
        }

    if (bHasResult)
        {
        cP = strchr (Game [nG].cOrgResult, ')') ;
        if (cP)
            {
            if (*(cP + 1) == ' ')
                {
                strcpy (cLine, cP + 1) ;
                cLine [0] = '{' ;

                strcat (System.cSaveBuffer, cLine) ;

                nI = strlen (cLine) ;
                if (cLine [nI - 1] != '\n')
                    {
                    strcat (System.cSaveBuffer, "\n") ;
                    }
                }
            else
                {
                if (strlen (Game [nG].cOrgResult) == 0)
                    {
                    strcat (System.cSaveBuffer, "{*} *\n") ;
                    }
                else
                    {
                    strcat (System.cSaveBuffer, Game [nG].cOrgResult) ;

                    nI = strlen (Game [nG].cOrgResult) ;
                    if (Game [nG].cOrgResult [nI - 1] != '\n')
                        {
                        strcat (System.cSaveBuffer, "\n") ;
                        }
                    }
                }
            }
        else
            {
            if (strlen (Game [nG].cOrgResult) == 0)
                {
                strcat (System.cSaveBuffer, "{*} *") ;
                }
            else
                {
                strcat (System.cSaveBuffer, Game [nG].cOrgResult) ;
                }

            nI = strlen (Game [nG].cOrgResult) ;
            if (Game [nG].cOrgResult [nI - 1] != '\n')
                {
                strcat (System.cSaveBuffer, "\n") ;
                }
            }
        }
    else
        {
        strcat (System.cSaveBuffer, "{*} *\n") ;
        }

    Game [nG].bSavedGame = 1 ;

    Fv = fopen (cFn, "a") ;
    if (Fv == NULL)
        {
        sprintf (cLine, "Saving %s Failed\n", cFn) ;
        TOOLBOX_WriteSystem (cLine) ;
        }
    else
        {
        fprintf (Fv, "\n%s", System.cSaveBuffer) ;
        fclose (Fv) ;

        sprintf (cLine, "Saving %s Successful\n", cFn) ;
        TOOLBOX_WriteSystem (cLine) ;
        }
    }

char * SAVEGAME_GetBHHeader (int nI, int nJ, int nR, char *cResult)
    {
    static char cBuffer [2048] ;

    char cDate [20], cTmp [1024], cTmp1 [1024], cWRating [255], cBRating [255], *cP, *cQ ;
    char cHandle [4] [30], cMatch [10] [100], cA, cB ;

    int nK, nL, nM ;

    switch (Login.nLoginType)
        {
        case SERVER_ICC :
            if (Game [nI].nRated == 1)
                {
                strcpy (cTmp, "[Event \"icc rated bughouse match\"]\n[Site \"icc\"]\n") ;
                }
            else
                {
                strcpy (cTmp, "[Event \"icc unrated bughouse match\"]\n[Site \"icc\"]\n") ;
                }
            break ;

        case SERVER_NONFICS :
            if (Game [nI].nRated == 1)
                {
                strcpy (cTmp, "[Event \"nonfics rated bughouse match\"]\n[Site \"nonfics\"]\n") ;
                }
            else
                {
                strcpy (cTmp, "[Event \"nonfics unrated bughouse match\"]\n[Site \"nonfics\"]\n") ;
                }
            break ;

        default :
            if (Game [nI].nRated == 1)
                {
                strcpy (cTmp, "[Event \"fics rated bughouse match\"]\n[Site \"fics\"]\n") ;
                }
            else
                {
                strcpy (cTmp, "[Event \"fics unrated bughouse match\"]\n[Site \"fics\"]\n") ;
                }
            break ;
        }
    strcpy (cBuffer, cTmp) ;

    _strdate (cDate) ;  // MM/DD/YY

    cTmp [0]  = '2' ;
    cTmp [1]  = '0' ;
    cTmp [2]  = cDate [6] ;  // Y
    cTmp [3]  = cDate [7] ;  // Y
    cTmp [4]  = '.' ;
    cTmp [5]  = cDate [0] ;  // M
    cTmp [6]  = cDate [1] ;  // M
    cTmp [7]  = '.' ;
    cTmp [8]  = cDate [3] ;  // D
    cTmp [9]  = cDate [4] ;  // D
    cTmp [10] = NULL_CHAR ;

    sprintf (cTmp1, "[Date \"%s\"]\n", cTmp) ;
    strcat (cBuffer, cTmp1) ;

    if (strlen (Game [nI].cRating [INDEX_WHITE]) == 0)
        {
        strcpy (cWRating, "") ;
        }
    else
        {
        cP = strchr (Game [nI].cRating [INDEX_WHITE], '(') ;
        cQ = strchr (Game [nI].cRating [INDEX_WHITE], ')') ;
        if (cP && cQ)
            {
            strncpy (cWRating, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cWRating) ;
            if (cWRating [nL - 1] == ')')
                {
                cWRating [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cWRating, Game [nI].cRating [INDEX_WHITE]) ;
            }
        }

    if (strlen (Game [nI].cRating [INDEX_BLACK]) == 0)
        {
        strcpy (cBRating, "") ;
        }
    else
        {
        cP = strchr (Game [nI].cRating [INDEX_BLACK], '(') ;
        cQ = strchr (Game [nI].cRating [INDEX_BLACK], ')') ;
        if (cP && cQ)
            {
            strncpy (cBRating, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cBRating) ;
            if (cBRating [nL - 1] == ')')
                {
                cBRating [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cBRating, Game [nI].cRating [INDEX_BLACK]) ;
            }
        }

    if (strlen (cWRating) == 0)
        {
        sprintf (cTmp, "[WhiteA \"%s\"]\n", Game [nI].cHandle [INDEX_WHITE]) ;
        }
    else
        {
        sprintf (cTmp, "[WhiteA \"%s\"][WhiteAElo \"%s\"]\n", Game [nI].cHandle [INDEX_WHITE], cWRating) ;
        }
    strcat (cBuffer, cTmp) ;

    if (strlen (cBRating) == 0)
        {
        sprintf (cTmp, "[BlackA \"%s\"]\n", Game [nI].cHandle [INDEX_BLACK]) ;
        }
    else
        {
        sprintf (cTmp, "[BlackA \"%s\"][BlackAElo \"%s\"]\n", Game [nI].cHandle [INDEX_BLACK], cBRating) ;
        }
    strcat (cBuffer, cTmp) ;

    if (strlen (Game [nJ].cRating [INDEX_WHITE]) == 0)
        {
        strcpy (cWRating, "") ;
        }
    else
        {
        cP = strchr (Game [nJ].cRating [INDEX_WHITE], '(') ;
        cQ = strchr (Game [nJ].cRating [INDEX_WHITE], ')') ;
        if (cP && cQ)
            {
            strncpy (cWRating, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cWRating) ;
            if (cWRating [nL - 1] == ')')
                {
                cWRating [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cWRating, Game [nJ].cRating [INDEX_WHITE]) ;
            }
        }

    if (strlen (Game [nJ].cRating [INDEX_BLACK]) == 0)
        {
        strcpy (cBRating, "") ;
        }
    else
        {
        cP = strchr (Game [nJ].cRating [INDEX_BLACK], '(') ;
        cQ = strchr (Game [nJ].cRating [INDEX_BLACK], ')') ;
        if (cP && cQ)
            {
            strncpy (cBRating, cP + 1, (cQ - cP) + 1) ;

            nL = strlen (cBRating) ;
            if (cBRating [nL - 1] == ')')
                {
                cBRating [nL - 1] = NULL_CHAR ;
                }
            }
        else
            {
            strcpy (cBRating, Game [nJ].cRating [INDEX_BLACK]) ;
            }
        }

    if (strlen (cWRating) == 0)
        {
        sprintf (cTmp, "[WhiteB \"%s\"]\n", Game [nJ].cHandle [INDEX_WHITE]) ;
        }
    else
        {
        sprintf (cTmp, "[WhiteB \"%s\"][WhiteBElo \"%s\"]\n", Game [nJ].cHandle [INDEX_WHITE], cWRating) ;
        }
    strcat (cBuffer, cTmp) ;

    if (strlen (cBRating) == 0)
        {
        sprintf (cTmp, "[BlackB \"%s\"]\n", Game [nJ].cHandle [INDEX_BLACK]) ;
        }
    else
        {
        sprintf (cTmp, "[BlackB \"%s\"][BlackBElo \"%s\"]\n", Game [nJ].cHandle [INDEX_BLACK], cBRating) ;
        }
    strcat (cBuffer, cTmp) ;

    sprintf (cTmp, "[TimeControl \"%d+%d\"]\n", Game [nI].nInitialClock * 60, Game [nI].nIncrementClock) ;
    strcat (cBuffer, cTmp) ;

    //
    // double check bughouse game result and adjust it if necessary
    //
    // LOST STRING:
    // Grishezz checkmated} 0-1
    // RubberDuck resigns} 1-0
    // kunde forfeits on time} 0-1
    // kunde lost} 0-1
    // Romana67's partner checkmated} 0-1
    // Lethal-Predator's partner resigns} 1-0
    // kunde's partner forfeits on time} 0-1
    // kunde's partner lost} 0-1
    //
    // WON STRING:
    // sgs won} 0-1
    // sgs's partner won} 0-1
    //
    if (strstr (Game [nR].cOrgResult, RESULT_WHITE_WINS) ||
        strstr (Game [nR].cOrgResult, RESULT_BLACK_WINS))
        {
        strcpy (cHandle [0], Game [nI].cHandle [INDEX_WHITE]) ;
        strcpy (cHandle [1], Game [nI].cHandle [INDEX_BLACK]) ;
        strcpy (cHandle [2], Game [nJ].cHandle [INDEX_WHITE]) ;
        strcpy (cHandle [3], Game [nJ].cHandle [INDEX_BLACK]) ;

        for (nK = 0 ; nK < 4 ; nK++)
            {

            // Grishezz checkmated} 0-1
            sprintf (cMatch [0], "%s checkmated", cHandle [nK]) ;

            // RubberDuck resigns} 1-0
            sprintf (cMatch [1], "%s resigns", cHandle [nK]) ;

            // kunde forfeits on time} 0-1
            sprintf (cMatch [2], "%s forfeits on time", cHandle [nK]) ;

            // kunde lost} 0-1
            sprintf (cMatch [3], "%s lost", cHandle [nK]) ;

            // Romana67's partner checkmated} 0-1
            sprintf (cMatch [4], "%s's partner checkmated", cHandle [nK]) ;

            // Lethal-Predator's partner resigns} 1-0
            sprintf (cMatch [5], "%s's partner resigns", cHandle [nK]) ;

            // kunde's partner forfeits on time} 0-1
            sprintf (cMatch [6], "%s's partner forfeits on time", cHandle [nK]) ;

            // kunde's partner lost} 0-1
            sprintf (cMatch [7], "%s's partner lost", cHandle [nK]) ;

            // sgs won} 0-1
            sprintf (cMatch [8], "%s won", cHandle [nK]) ;

            // sgs's partner won} 0-1
            sprintf (cMatch [9], "%s's partner won", cHandle [nK]) ;

            for (nM = 0 ; nM < 10 ; nM++)
                {
                if (strstr (Game [nR].cOrgResult, cMatch [nM]))
                    {
                    if (nM >= 8)
                        {

                        // won string
                        cP = strchr (Game [nR].cOrgResult, '}') ;
                        if (cP)
                            {
                            if (*(cP + 1) == ' ')
                                {
                                if ((nK == 0) || (nK == 3))
                                    {
                                    cA = '1' ;
                                    cB = '0' ;
                                    }
                                else
                                    {
                                    cA = '0' ;
                                    cB = '1' ;
                                    }

                                *(cP + 2) = cA ;
                                *(cP + 3) = '-' ;
                                *(cP + 4) = cB ;
                                *(cP + 5) = NULL_CHAR ;
                                }
                            }
                        }
                    else
                        {

                        // lost string
                        cP = strchr (Game [nR].cOrgResult, '}') ;
                        if (cP)
                            {
                            if (*(cP + 1) == ' ')
                                {
                                if ((nK == 0) || (nK == 3))
                                    {
                                    cA = '0' ;
                                    cB = '1' ;
                                    }
                                else
                                    {
                                    cA = '1' ;
                                    cB = '0' ;
                                    }

                                *(cP + 2) = cA ;
                                *(cP + 3) = '-' ;
                                *(cP + 4) = cB ;
                                *(cP + 5) = NULL_CHAR ;
                                }
                            }
                        }

                    break ;
                    }
                }
            }
        }

    if (strlen (Game [nR].cOrgResult) == 0)
        {
        strcat (cBuffer, "[Result \"*\"]\n") ;
        strcpy (cResult, "*") ;
        }
    else
        {
        cP = strchr (Game [nR].cOrgResult, '}') ;
        if (cP)
            {
            if (*(cP + 1) == ' ')
                {
                strcpy (cTmp, cP + 1) ;
                TOOLBOX_AllTrim (cTmp) ;

                nL = strlen (cTmp) ;
                if (cTmp [nL - 1] == '\n')
                    {
                    cTmp [nL - 1] = NULL_CHAR ;
                    }

                if (strstr (cTmp, RESULT_WHITE_WINS) ||
                    strstr (cTmp, RESULT_BLACK_WINS) ||
                    strstr (cTmp, RESULT_DRAW      ) ||
                    strstr (cTmp, RESULT_UNKNOWN   ))
                    {
                    strcat (cBuffer, "[Result \"") ;
                    strcat (cBuffer, cTmp) ;
                    strcat (cBuffer, "\"]\n") ;

                    strcpy (cResult, cTmp) ;
                    }
                else
                    {
                    strcat (cBuffer, "[Result \"*\"]\n") ;
                    strcpy (cResult, "*") ;
                    }
                }
            else
                {
                strcat (cBuffer, "[Result \"*\"]\n") ;
                strcpy (cResult, "*") ;
                }
            }
        else
            {
            strcat (cBuffer, "[Result \"*\"]\n") ;
            strcpy (cResult, "*") ;
            }
        }

    if (! Bughouse [nI].bFromInitialPosition)
        {
        strcpy (cTmp, SAVEGAME_GetBFEN (nI)) ;

        strcat (cBuffer, "[Setup \"1\"]\n[FEN \"") ;
        strcat (cBuffer, cTmp) ;
        strcat (cBuffer, "\"]\n") ;
        }

    strcat (cBuffer, "\n") ;

    return cBuffer ;
    }

void SAVEGAME_SaveBH (int nI, int nJ, int bExam, int bClose)
    {
    char cFn [_MAX_PATH + 10], cResult [500], cLine [1024], cMove [500], *cP ;
    FILE *Fv ;
    int bHasResult ;
    int nR, nM, nK, nC, nL, nTime, nInc, nWTime1, nBTime1, nWTime2, nBTime2 ;
    int nG, nMn, nSc, nMM, nPM ;
    long nLTime ;

    if (System.bCDROMConnection)
        {
        return ;
        }

    //
    // don't save games with 0 moves
    //
    if (Bughouse [nI].nMove <= 0)
        {
        return ;
        }

    // FICS -> {Still in progress} *
    // ICC  -> {Game in progress} *
    if (strstr (Game [nI].cOrgResult, " in progress"))
        {
        nR = nJ ;
        }
    else
        {
        nR = nI ;
        }

    strcpy (System.cSaveBuffer, SAVEGAME_GetBHHeader (nI, nJ, nR, cResult)) ;

    if (! bClose)
        {
        if (! User.bSaveAbortGame)
            {
            if (strcmp (cResult, "*") == 0)
                {
                return ;
                }
            }
        }

    if (strstr (Game [nR].cOrgResult, RESULT_WHITE_WINS) ||
        strstr (Game [nR].cOrgResult, RESULT_BLACK_WINS) ||
        strstr (Game [nR].cOrgResult, RESULT_DRAW      ) ||
        strstr (Game [nR].cOrgResult, RESULT_UNKNOWN   ))
        {
        SAVEGAME_GetFilename (nI, bExam, bClose, (strstr (Game [nR].cOrgResult, RESULT_UNKNOWN) ? 1 : 0), cFn) ;
        bHasResult = 1 ;
        }
    else
        {
        SAVEGAME_GetFilename (nI, bExam, bClose, 1, cFn) ;
        bHasResult = 0 ;
		}

    if (Bughouse [nI].nMove > 0)
        {
        nInc = Game [nI].nIncrementClock ;

        if (Bughouse [nI].bFromInitialPosition)
            {
            nTime = Game [nI].nInitialClock * 60 ;

            if ((nTime == 0) && (nInc > 0))
                {
                nTime = 10 ;
                }

            nWTime1 = nTime ;
            nBTime1 = nTime ;
            nWTime2 = nTime ;
            nBTime2 = nTime ;
            }
        else
            {
            nWTime1 = (int) (Bughouse [nI].nMainTimeRemaining    [INDEX_WHITE] / 1000) ;
            nBTime1 = (int) (Bughouse [nI].nMainTimeRemaining    [INDEX_BLACK] / 1000) ;
            nWTime2 = (int) (Bughouse [nI].nPartnerTimeRemaining [INDEX_WHITE] / 1000) ;
            nBTime2 = (int) (Bughouse [nI].nPartnerTimeRemaining [INDEX_BLACK] / 1000) ;

            if (Bughouse [nI].bMainWhitesMove)
                {
                nMM = 0 ;
                }
            else
                {
                nMM = 1 ;
                }

            if (Bughouse [nI].bPartnerWhitesMove)
                {
                nPM = 0 ;
                }
            else
                {
                nPM = 1 ;
                }
            }

        strcpy (cLine, "") ;

        for (nM = 0 ; nM < Bughouse [nI].nMove ; nM++)
            {
            nG = Bughouse [nI].nIndex  [nM] ;
            nK = Bughouse [nI].nNumber [nM] ;
            nC = Bughouse [nI].nColor  [nM] ;

            if (nM == 0)
                {
                if (! Bughouse [nI].bFromInitialPosition)
                    {

                    // first time and not from initial position we have to make
                    // sure the first move is the right color to move
                    if (nG == nI)
                        {
                        if (nC == nMM)
                            {

                            // ok
                            }
                        else
                            {

                            // wrong color lets skip this record
                            continue ;
                            }
                        }
                    else
                        {
                        if (nC == nPM)
                            {

                            // ok
                            }
                        else
                            {

                            // wrong color lets skip this record
                            continue ;
                            }
                        }
                    }
                }

            if (Game [nG].Position [nK].nTime [nC] == -9999L)
                {
                nMn = 0 ;
                nSc = 0 ;
                sscanf (Game [nG].Position [nK].cTime4Move [nC], "(%02d:%02d", &nMn, &nSc) ;

                if (nG == nI)
                    {
                    if (nC == 0)
                        {
                        nWTime1 = (nWTime1 - ((nMn * 60) + nSc)) + nInc ;
                        if (nWTime1 < 0)
                            {
                            nWTime1 = 0 ;
                            }

                        sprintf (cMove, "%dA. %s {%d}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nWTime1) ;
                        }
                    else
                        {
                        nBTime1 = (nBTime1 - ((nMn * 60) + nSc)) + nInc ;
                        if (nBTime1 < 0)
                            {
                            nBTime1 = 0 ;
                            }

                        sprintf (cMove, "%da. %s {%d}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nBTime1) ;
                        }
                    }
                else
                    {
                    if (nC == 0)
                        {
                        nWTime2 = (nWTime2 - ((nMn * 60) + nSc)) + nInc ;
                        if (nWTime2 < 0)
                            {
                            nWTime2 = 0 ;
                            }

                        sprintf (cMove, "%dB. %s {%d}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nWTime2) ;
                        }
                    else
                        {
                        nBTime2 = (nBTime2 - ((nMn * 60) + nSc)) + nInc ;
                        if (nBTime2 < 0)
                            {
                            nBTime2 = 0 ;
                            }

                        sprintf (cMove, "%db. %s {%d}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nBTime2) ;
                        }
                    }
                }
            else
                {
                nLTime = (long) floor (((double) (Game [nG].Position [nK].nTime [nC] + 999L)) / 1000.0) ;
                    
                if (nG == nI)
                    {
                    if (nC == 0)
                        {
                        sprintf (cMove, "%dA. %s {%ld}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nLTime) ;
                        }
                    else
                        {
                        sprintf (cMove, "%da. %s {%ld}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nLTime) ;
                        }
                    }
                else
                    {
                    if (nC == 0)
                        {
                        sprintf (cMove, "%dB. %s {%ld}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nLTime) ;
                        }
                    else
                        {
                        sprintf (cMove, "%db. %s {%ld}", nK + 1, Game [nG].Position [nK].cLastMove [nC], nLTime) ;
                        }
                    }
                }

            if ((strlen (cLine) + 1 + strlen (cMove)) > 79)
                {
                strcat (System.cSaveBuffer, cLine) ;
                strcat (System.cSaveBuffer, "\n") ;
                strcpy (cLine, cMove) ;
                }
            else
                {
                if (nM > 0)
                    {
                    strcat (cLine, " ") ;
                    }
                strcat (cLine, cMove) ;
                }
            }

        strcat (System.cSaveBuffer, cLine) ;
        strcat (System.cSaveBuffer, "\n\n") ;
        }

    if (bHasResult)
        {
        cP = strchr (Game [nR].cOrgResult, ')') ;
        if (cP)
            {
            if (*(cP + 1) == ' ')
                {
                strcpy (cLine, cP + 1) ;
                cLine [0] = '{' ;

                strcat (System.cSaveBuffer, cLine) ;

                nL = strlen (cLine) ;
                if (cLine [nL - 1] != '\n')
                    {
                    strcat (System.cSaveBuffer, "\n") ;
                    }
                }
            else
                {
                if (strlen (Game [nR].cOrgResult) == 0)
                    {
                    strcat (System.cSaveBuffer, "{*} *\n") ;
                    }
                else
                    {
                    strcat (System.cSaveBuffer, Game [nR].cOrgResult) ;

                    nL = strlen (Game [nR].cOrgResult) ;
                    if (Game [nR].cOrgResult [nL - 1] != '\n')
                        {
                        strcat (System.cSaveBuffer, "\n") ;
                        }
                    }
                }
            }
        else
            {
            if (strlen (Game [nR].cOrgResult) == 0)
                {
                strcat (System.cSaveBuffer, "{*} *") ;
                }
            else
                {
                strcat (System.cSaveBuffer, Game [nR].cOrgResult) ;
                }

            nL = strlen (Game [nR].cOrgResult) ;
            if (Game [nR].cOrgResult [nL - 1] != '\n')
                {
                strcat (System.cSaveBuffer, "\n") ;
                }
            }
        }
    else
        {
        strcat (System.cSaveBuffer, "{*} *\n") ;
        }

    Game [nI].bSavedGame = 1 ;
    Game [nJ].bSavedGame = 1 ;

    Fv = fopen (cFn, "a") ;
    if (Fv == NULL)
        {
        sprintf (cLine, "Saving %s Failed\n", cFn) ;
        TOOLBOX_WriteSystem (cLine) ;
        }
    else
        {
        fprintf (Fv, "\n%s", System.cSaveBuffer) ;
        fclose (Fv) ;

        sprintf (cLine, "Saving %s Successful\n", cFn) ;
        TOOLBOX_WriteSystem (cLine) ;
        }
    }

void SAVEGAME_SaveGame (int nG, int bExam, int bClose)
    {
    int nI, nJ ;

    if (System.bCDROMConnection)
        {
        return ;
        }

    if ((Game [nG].nRelation ==  0) ||  //  0 i am observing a game being played
        (Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
        (Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
        {
        if (Game [nG].nGameType == GAMETYPE_BUGHOUSE)
            {
            nI = Bughouse [nG].nMainIndex ;

            if (User.bSaveNoBFENBugGame)
                {
                if (! Bughouse [nI].bFromInitialPosition)
                    {
                    return ;
                    }
                }
                
            nJ = Bughouse [nG].nPartnerIndex ;

            if ((nI >= 0) && (nJ >= 0))
                {
                if ((! Game [nI].bSavedGame) && (! Game [nJ].bSavedGame))
                    {
                    if ((Bughouse [nI].nMainIndex    >= 0) &&
                        (Bughouse [nI].nPartnerIndex >= 0))
                        {
                        SAVEGAME_SaveBH (nI, nJ, bExam, bClose) ;
                        }
                    }
                }
            }
        else
        if (Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
            {
            SAVEGAME_SaveZH (nG, bExam, bClose) ;
            }
        else
        if (Login.nLoginType == SERVER_ICC)
            {
            if ((Game [nG].nGameType == GAMETYPE_ICC_WILD6 ) ||
                (Game [nG].nGameType == GAMETYPE_ICC_WILD16))
                {
                return ;
                }
            else
                {
                SAVEGAME_SavePGN (nG, bExam, bClose) ;
                }
            }
        else
            {
            SAVEGAME_SavePGN (nG, bExam, bClose) ;
            }
        }
    }
