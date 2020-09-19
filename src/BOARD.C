#include "thief.h"

POINT _ptNumber [3] = { {2, 1}, {4, 5}, {6, 9} } ;

int BOARD_FICS_To_Int (char cP)
    {
    switch (cP)
        {
        case ICS_EMPTY_SQUARE : return EMPTY_SQUARE ;
        case ICS_WHITE_PAWN :   return WHITE_PAWN ;
        case ICS_BLACK_PAWN :   return BLACK_PAWN ;
        case ICS_WHITE_KNIGHT : return WHITE_KNIGHT ;
        case ICS_BLACK_KNIGHT : return BLACK_KNIGHT ;
        case ICS_WHITE_BISHOP : return WHITE_BISHOP ;
        case ICS_BLACK_BISHOP : return BLACK_BISHOP ;
        case ICS_WHITE_ROOK :   return WHITE_ROOK ;
        case ICS_BLACK_ROOK :   return BLACK_ROOK ;
        case ICS_WHITE_QUEEN :  return WHITE_QUEEN ;
        case ICS_BLACK_QUEEN :  return BLACK_QUEEN ;
        case ICS_WHITE_KING :   return WHITE_KING ;
        case ICS_BLACK_KING :   return BLACK_KING ;
        }
    return EMPTY_SQUARE ;
    }

int BOARD_FICS_To_White_Int (char cP)
    {
    switch (cP)
        {
        case ICS_WHITE_PAWN :   return WHITE_PAWN ;
        case ICS_WHITE_KNIGHT : return WHITE_KNIGHT ;
        case ICS_WHITE_BISHOP : return WHITE_BISHOP ;
        case ICS_WHITE_ROOK :   return WHITE_ROOK ;
        case ICS_WHITE_QUEEN :  return WHITE_QUEEN ;
        }
    return EMPTY_SQUARE ;
    }

int BOARD_FICS_To_Black_Int (char cP)
    {
    switch (cP)
        {
        case ICS_WHITE_PAWN :   return BLACK_PAWN ;
        case ICS_WHITE_KNIGHT : return BLACK_KNIGHT ;
        case ICS_WHITE_BISHOP : return BLACK_BISHOP ;
        case ICS_WHITE_ROOK :   return BLACK_ROOK ;
        case ICS_WHITE_QUEEN :  return BLACK_QUEEN ;
        }
    return EMPTY_SQUARE ;
    }

int BOARD_IsWhitePiece (int nP)
    {
    return (((nP >= WHITE_PAWN) && (nP <= WHITE_QUEEN)) || (nP == WHITE_KING)) ;
    }

int BOARD_IsBlackPiece (int nP)
    {
    return (((nP >= BLACK_PAWN) && (nP <= BLACK_QUEEN)) || (nP == BLACK_KING)) ;
    }

int BOARD_IsSameColor (int nP, int nC)
    {
    if (nC == INDEX_WHITE)
        {
        return (((nP >= WHITE_PAWN) && (nP <= WHITE_QUEEN)) || (nP == WHITE_KING)) ;
        }
    else
        {
        return (((nP >= BLACK_PAWN) && (nP <= BLACK_QUEEN)) || (nP == BLACK_KING)) ;
        }
    }

void BOARD_FindKings (int nG)
    {
    int nX, nY ;

    Game [nG].nKingX [INDEX_WHITE] = -1 ;
    Game [nG].nKingY [INDEX_WHITE] = -1 ;

    Game [nG].nKingX [INDEX_BLACK] = -1 ;
    Game [nG].nKingY [INDEX_BLACK] = -1 ;

    for (nY = 7 ; nY >= 0 ; nY--)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            if (Game [nG].nBoard [nX] [nY] == WHITE_KING)
                {
                Game [nG].nKingX [INDEX_WHITE] = nX ;
                Game [nG].nKingY [INDEX_WHITE] = nY ;
                }
            else
            if (Game [nG].nBoard [nX] [nY] == BLACK_KING)
                {
                Game [nG].nKingX [INDEX_BLACK] = nX ;
                Game [nG].nKingY [INDEX_BLACK] = nY ;
                }
            }
        }

    if (Game [nG].bCanCastleKingSide  [INDEX_WHITE] ||
        Game [nG].bCanCastleQueenSide [INDEX_WHITE] )
        {
        if (Game [nG].nKingX [INDEX_WHITE] == 4 &&
            Game [nG].nKingY [INDEX_WHITE] == 0)
            {
            if (Game [nG].nBoard [7] [0] != WHITE_ROOK)
                {
                Game [nG].bCanCastleKingSide [INDEX_WHITE] = 0 ;
                }

            if (Game [nG].nBoard [0] [0] != WHITE_ROOK)
                {
                Game [nG].bCanCastleQueenSide [INDEX_WHITE] = 0 ;
                }
            }
        else
            {
            Game [nG].bCanCastleKingSide  [INDEX_WHITE] = 0 ;
            Game [nG].bCanCastleQueenSide [INDEX_WHITE] = 0 ;
            }
        }

    if (Game [nG].bCanCastleKingSide  [INDEX_BLACK] ||
        Game [nG].bCanCastleQueenSide [INDEX_BLACK] )
        {
        if (Game [nG].nKingX [INDEX_BLACK] == 4 &&
            Game [nG].nKingY [INDEX_BLACK] == 7)
            {
            if (Game [nG].nBoard [7] [7] != BLACK_ROOK)
                {
                Game [nG].bCanCastleKingSide [INDEX_BLACK] = 0 ;
                }

            if (Game [nG].nBoard [0] [7] != BLACK_ROOK)
                {
                Game [nG].bCanCastleQueenSide [INDEX_BLACK] = 0 ;
                }
            }
        else
            {
            Game [nG].bCanCastleKingSide  [INDEX_BLACK] = 0 ;
            Game [nG].bCanCastleQueenSide [INDEX_BLACK] = 0 ;
            }
        }
    }

void BOARD_FindICCWild9Kings (int nG)
    {
    int nX, nY, nCWX, nCWY, nCBX, nCBY ;

    if (Game [nG].nGameType == GAMETYPE_ICC_WILD9)
        {
        nCWX = -1 ;
        nCWY = -1 ;
        nCBX = -1 ;
        nCBY = -1 ;

        for (nY = 7 ; nY >= 0 ; nY--)
            {
            for (nX = 0 ; nX < 8 ; nX++)
                {
                if (Game [nG].nBoard [nX] [nY] == WHITE_KING)
                    {
                    if (nCWX == -1)
                        {
                        nCWX = nX ;
                        nCWY = nY ;
                        }
                    else
                    if (nX < nCWX)
                        {
                        nCWX = nX ;
                        nCWY = nY ;
                        }
                    else
                    if ((nX == nCWX) && (nY < nCWY))
                        {
                        nCWX = nX ;
                        nCWY = nY ;
                        }
                    }
                else
                if (Game [nG].nBoard [nX] [nY] == BLACK_KING)
                    {
                    if (nCBX == -1)
                        {
                        nCBX = nX ;
                        nCBY = nY ;
                        }
                    else
                    if (nX < nCBX)
                        {
                        nCBX = nX ;
                        nCBY = nY ;
                        }
                    else
                    if ((nX == nCBX) && (nY < nCBY))
                        {
                        nCBX = nX ;
                        nCBY = nY ;
                        }
                    }
                }
            }

        Game [nG].nKingX [INDEX_WHITE] = nCWX ;
        Game [nG].nKingY [INDEX_WHITE] = nCWY ;

        Game [nG].nKingX [INDEX_BLACK] = nCBX ;
        Game [nG].nKingY [INDEX_BLACK] = nCBY ;
        }
    }

void BOARD_UpdatePromoteBoard (int nG, int bWhitesMove, char *cVerboseMove)
    {
    int nFromX, nFromY, nToX, nToY ;

    // if initial position then return
    if (Game [nG].bInitialMove)
        {
        return ;
        }

    // if cVerboseMove is the initial position then return
    if (stricmp (cVerboseMove, ICS_INITIAL_MOVE_NAME) == 0)
        {
        return ;
        }

    // o-o
    if (stricmp (cVerboseMove, ICS_CASTLE_KING_SIDE_MOVE) == 0)
        {
        if (bWhitesMove)
            {
            Game [nG].nPromoteBoard [4] [7] = 0 ;
            Game [nG].nPromoteBoard [6] [7] = 0 ;
            }
        else
            {
            Game [nG].nPromoteBoard [4] [0] = 0 ;
            Game [nG].nPromoteBoard [6] [0] = 0 ;
            }
        return ;
        }

    // o-o-o
    if (stricmp (cVerboseMove, ICS_CASTLE_QUEEN_SIDE_MOVE) == 0)
        {
        if (bWhitesMove)
            {
            Game [nG].nPromoteBoard [4] [7] = 0 ;
            Game [nG].nPromoteBoard [2] [7] = 0 ;
            }
        else
            {
            Game [nG].nPromoteBoard [4] [0] = 0 ;
            Game [nG].nPromoteBoard [2] [0] = 0 ;
            }
        return ;
        }

    // board move
    if (! strchr (cVerboseMove, '@'))
        {
        nFromX = (int) (cVerboseMove [2] - 'a') ;
        nFromY = (int) (cVerboseMove [3] - '1') ;
        nToX   = (int) (cVerboseMove [5] - 'a') ;
        nToY   = (int) (cVerboseMove [6] - '1') ;

        if (strchr (cVerboseMove, '='))
            {
            // promotion move
            Game [nG].nPromoteBoard [nToX  ] [nToY  ] = 1 ;
            Game [nG].nPromoteBoard [nFromX] [nFromY] = 0 ;
            }
        else
            {
            // regular move
            Game [nG].nPromoteBoard [nToX  ] [nToY  ] = Game [nG].nPromoteBoard [nFromX] [nFromY] ;
            Game [nG].nPromoteBoard [nFromX] [nFromY] = 0 ;
            }
        return ;
        }

    // drop move
    switch (Login.nLoginType)
        {
        case SERVER_FICS :
            nToX = (int) (cVerboseMove [5] - 'a') ;
            nToY = (int) (cVerboseMove [6] - '1') ;
            break ;

        case SERVER_ICC :
            nToX = (int) (cVerboseMove [2] - 'a') ;
            nToY = (int) (cVerboseMove [3] - '1') ;
            break ;

        case SERVER_NONFICS :
            nToX = (int) (cVerboseMove [5] - 'a') ;
            nToY = (int) (cVerboseMove [6] - '1') ;
            break ;

        default :
            nToX = (int) (cVerboseMove [5] - 'a') ;
            nToY = (int) (cVerboseMove [6] - '1') ;
            break ;
        }

    Game [nG].nPromoteBoard [nToX] [nToY] = 0 ;
    }

void BOARD_LoadBoard (int nG, char *cP, char *cVerboseMove)
    {
    int nX, nY ;

    if (Game [nG].bChessGame)
        {
        BOARD_UpdatePromoteBoard (nG, Game [nG].bWhitesMove, cVerboseMove) ;

        for (nX = 0 ; nX < MAX_BUFFER ; nX++)
            {
            Game [nG].nBuffer [nX] = ChessPiece [nX] ;
            }
        }

    Game [nG].ChessValue [WHITE_PAWN]   = 0 ;
    Game [nG].ChessValue [WHITE_ROOK]   = 0 ;
    Game [nG].ChessValue [WHITE_KNIGHT] = 0 ;
    Game [nG].ChessValue [WHITE_BISHOP] = 0 ;
    Game [nG].ChessValue [WHITE_QUEEN]  = 0 ;

    Game [nG].ChessValue [BLACK_PAWN]   = 0 ;
    Game [nG].ChessValue [BLACK_ROOK]   = 0 ;
    Game [nG].ChessValue [BLACK_KNIGHT] = 0 ;
    Game [nG].ChessValue [BLACK_BISHOP] = 0 ;
    Game [nG].ChessValue [BLACK_QUEEN]  = 0 ;

    Game [nG].BugValue [WHITE_PAWN]   = 0 ;
    Game [nG].BugValue [WHITE_ROOK]   = 0 ;
    Game [nG].BugValue [WHITE_KNIGHT] = 0 ;
    Game [nG].BugValue [WHITE_BISHOP] = 0 ;
    Game [nG].BugValue [WHITE_QUEEN]  = 0 ;

    Game [nG].BugValue [BLACK_PAWN]   = 0 ;
    Game [nG].BugValue [BLACK_ROOK]   = 0 ;
    Game [nG].BugValue [BLACK_KNIGHT] = 0 ;
    Game [nG].BugValue [BLACK_BISHOP] = 0 ;
    Game [nG].BugValue [BLACK_QUEEN]  = 0 ;

    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
        {
        Game [nG].BugValue [nX] = Game [nG].nBuffer [nX] * BugValue [nX] ;
        }

    Game [nG].nKingX [INDEX_WHITE] = -1 ;
    Game [nG].nKingY [INDEX_WHITE] = -1 ;

    Game [nG].nKingX [INDEX_BLACK] = -1 ;
    Game [nG].nKingY [INDEX_BLACK] = -1 ;

    for (nY = 7 ; nY >= 0 ; nY--)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            Game [nG].nBoard [nX] [nY] = BOARD_FICS_To_Int (*cP++) ;

            Game [nG].ChessValue [Game [nG].nBoard [nX] [nY]] = Game [nG].ChessValue [Game [nG].nBoard [nX] [nY]] + ChessValue [Game [nG].nBoard [nX] [nY]] ;
            Game [nG].BugValue   [Game [nG].nBoard [nX] [nY]] = Game [nG].BugValue   [Game [nG].nBoard [nX] [nY]] + BugValue   [Game [nG].nBoard [nX] [nY]] ;

            if (Game [nG].nBoard [nX] [nY] == WHITE_KING)
                {
                Game [nG].nKingX [INDEX_WHITE] = nX ;
                Game [nG].nKingY [INDEX_WHITE] = nY ;
                }
            else
            if (Game [nG].nBoard [nX] [nY] == BLACK_KING)
                {
                Game [nG].nKingX [INDEX_BLACK] = nX ;
                Game [nG].nKingY [INDEX_BLACK] = nY ;
                }
            else
            if (Game [nG].bChessGame)
                {
                if (Game [nG].nBoard [nX] [nY] != EMPTY_SQUARE)
                    {
                    if (Game [nG].nPromoteBoard [nX] [nY])
                        {
                        if (BOARD_IsWhitePiece (Game [nG].nBoard [nX] [nY]))
                            {
                            --Game [nG].nBuffer [WHITE_PAWN] ;
                            }
                        else
                            {
                            --Game [nG].nBuffer [BLACK_PAWN] ;
                            }
                        }
                    else
                        {
                        --Game [nG].nBuffer [Game [nG].nBoard [nX] [nY]] ;
                        }
                    }
                }
            }
        *cP++ ;
        }

    if (Game [nG].nGameType == GAMETYPE_ICC_WILD9)
        {
        BOARD_FindICCWild9Kings (nG) ;
        }

    if (User.bShowValueOnTitleBar)
        {
        SetWindowText (Game [nG].hwnd, TOOLBOX_GetGameWindowTitle (nG)) ;
        }
    }

void BOARD_LoadBuffer (int nG, char *cW, char *cB)
    {
    int nI,nX, nY ;

    Game [nG].BugValue [WHITE_PAWN]   = 0 ;
    Game [nG].BugValue [WHITE_ROOK]   = 0 ;
    Game [nG].BugValue [WHITE_KNIGHT] = 0 ;
    Game [nG].BugValue [WHITE_BISHOP] = 0 ;
    Game [nG].BugValue [WHITE_QUEEN]  = 0 ;

    Game [nG].BugValue [BLACK_PAWN]   = 0 ;
    Game [nG].BugValue [BLACK_ROOK]   = 0 ;
    Game [nG].BugValue [BLACK_KNIGHT] = 0 ;
    Game [nG].BugValue [BLACK_BISHOP] = 0 ;
    Game [nG].BugValue [BLACK_QUEEN]  = 0 ;

    for (nY = 0 ; nY < 8 ; nY++)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            Game [nG].BugValue [Game [nG].nBoard [nX] [nY]] = Game [nG].BugValue [Game [nG].nBoard [nX] [nY]] + BugValue [Game [nG].nBoard [nX] [nY]] ;
            }
        }

    Game [nG].nBuffer [WHITE_PAWN]   = 0 ;
    Game [nG].nBuffer [WHITE_ROOK]   = 0 ;
    Game [nG].nBuffer [WHITE_KNIGHT] = 0 ;
    Game [nG].nBuffer [WHITE_BISHOP] = 0 ;
    Game [nG].nBuffer [WHITE_QUEEN]  = 0 ;

    Game [nG].nBuffer [BLACK_PAWN]   = 0 ;
    Game [nG].nBuffer [BLACK_ROOK]   = 0 ;
    Game [nG].nBuffer [BLACK_KNIGHT] = 0 ;
    Game [nG].nBuffer [BLACK_BISHOP] = 0 ;
    Game [nG].nBuffer [BLACK_QUEEN]  = 0 ;

    for (nI = 0 ; nI < ((int) strlen (cW)) ; nI++)
        {
        switch (cW [nI])
            {
            case ICS_WHITE_PAWN :
                ++Game [nG].nBuffer [WHITE_PAWN] ;
                Game [nG].BugValue [WHITE_PAWN] = Game [nG].BugValue [WHITE_PAWN] + BugValue [WHITE_PAWN] ;
                break ;

            case ICS_WHITE_ROOK :
                ++Game [nG].nBuffer [WHITE_ROOK] ;
                Game [nG].BugValue [WHITE_ROOK] = Game [nG].BugValue [WHITE_ROOK] + BugValue [WHITE_ROOK] ;
                break ;

            case ICS_WHITE_KNIGHT :
                ++Game [nG].nBuffer [WHITE_KNIGHT] ;
                Game [nG].BugValue [WHITE_KNIGHT] = Game [nG].BugValue [WHITE_KNIGHT] + BugValue [WHITE_KNIGHT] ;
                break ;

            case ICS_WHITE_BISHOP :
                ++Game [nG].nBuffer [WHITE_BISHOP] ;
                Game [nG].BugValue [WHITE_BISHOP] = Game [nG].BugValue [WHITE_BISHOP] + BugValue [WHITE_BISHOP] ;
                break ;

            case ICS_WHITE_QUEEN :
                ++Game [nG].nBuffer [WHITE_QUEEN] ;
                Game [nG].BugValue [WHITE_QUEEN] = Game [nG].BugValue [WHITE_QUEEN] + BugValue [WHITE_QUEEN] ;
                break ;
            }
        }

    for (nI = 0 ; nI < ((int) strlen (cB)) ; nI++)
        {
        switch (cB [nI])
            {
            case ICS_WHITE_PAWN :
                ++Game [nG].nBuffer [BLACK_PAWN] ;
                Game [nG].BugValue [BLACK_PAWN] = Game [nG].BugValue [BLACK_PAWN] + BugValue [BLACK_PAWN] ;
                break ;

            case ICS_WHITE_ROOK :
                ++Game [nG].nBuffer [BLACK_ROOK] ;
                Game [nG].BugValue [BLACK_ROOK] = Game [nG].BugValue [BLACK_ROOK] + BugValue [BLACK_ROOK] ;
                break ;

            case ICS_WHITE_KNIGHT :
                ++Game [nG].nBuffer [BLACK_KNIGHT] ;
                Game [nG].BugValue [BLACK_KNIGHT] = Game [nG].BugValue [BLACK_KNIGHT] + BugValue [BLACK_KNIGHT] ;
                break ;

            case ICS_WHITE_BISHOP :
                ++Game [nG].nBuffer [BLACK_BISHOP] ;
                Game [nG].BugValue [BLACK_BISHOP] = Game [nG].BugValue [BLACK_BISHOP] + BugValue [BLACK_BISHOP] ;
                break ;

            case ICS_WHITE_QUEEN :
                ++Game [nG].nBuffer [BLACK_QUEEN] ;
                Game [nG].BugValue [BLACK_QUEEN] = Game [nG].BugValue [BLACK_QUEEN] + BugValue [BLACK_QUEEN] ;
                break ;
            }
        }

    if (User.bShowValueOnTitleBar)
        {
        SetWindowText (Game [nG].hwnd, TOOLBOX_GetGameWindowTitle (nG)) ;
        }
    }

void BOARD_FindCapturedPieces (int nG)
    {
    int nX, nY ;

    BOARD_UpdatePromoteBoard (nG, Game [nG].bLastWhitesMove, Game [nG].cVerboseMove) ;

    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
        {
        Game [nG].nBuffer [nX] = ChessPiece [nX] ;
        }

    for (nY = 7 ; nY >= 0 ; nY--)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            if (Game [nG].nBoard [nX] [nY] == WHITE_KING)
                {
                }
            else
            if (Game [nG].nBoard [nX] [nY] == BLACK_KING)
                {
                }
            else
            if (Game [nG].bChessGame)
                {
                if (Game [nG].nBoard [nX] [nY] != EMPTY_SQUARE)
                    {
                    if (Game [nG].nPromoteBoard [nX] [nY])
                        {
                        if (BOARD_IsWhitePiece (Game [nG].nBoard [nX] [nY]))
                            {
                            --Game [nG].nBuffer [WHITE_PAWN] ;
                            }
                        else
                            {
                            --Game [nG].nBuffer [BLACK_PAWN] ;
                            }
                        }
                    else
                        {
                        --Game [nG].nBuffer [Game [nG].nBoard [nX] [nY]] ;
                        }
                    }
                }
            }
        }
    }

void BOARD_SaveLastBoardBuffer (int nG)
    {
    int nX, nY ;

    for (nY = 0 ; nY < 8 ; nY++)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            Game [nG].nLastBoard [nX] [nY] = Game [nG].nBoard [nX] [nY] ;
            }
        }

    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
        {
        Game [nG].nLastBuffer [nX] = Game [nG].nBuffer [nX] ;
        }
    }

void BOARD_NullLastBoardBuffer (int nG)
    {
    int nX, nY ;

    for (nY = 0 ; nY < 8 ; nY++)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            Game [nG].nLastBoard [nX] [nY] = -2 ;
            }
        }

    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
        {
        Game [nG].nLastBuffer [nX] = -2 ;
        }
    }

void BOARD_NullHighlight (int nG)
    {
    int nI ;

    Game [nG].ptHighlight [0].x = -2 ;
    Game [nG].ptHighlight [0].y = -2 ;
    Game [nG].ptHighlight [1] = Game [nG].ptHighlight [0] ;

    for (nI = 0 ; nI < 8 ; nI++)
        {
        Game [nG].ptKing [INDEX_WHITE] [nI] = Game [nG].ptHighlight [0] ;
        Game [nG].ptKing [INDEX_BLACK] [nI] = Game [nG].ptHighlight [0] ;
        }
    }

void BOARD_SaveLastHighlight (int nG)
    {
    int nI ;

    Game [nG].ptLastHighlight [0] = Game [nG].ptHighlight [0] ;
    Game [nG].ptLastHighlight [1] = Game [nG].ptHighlight [1] ;

    for (nI = 0 ; nI < 8 ; nI++)
        {
        Game [nG].ptLastKing [INDEX_WHITE] [nI] = Game [nG].ptKing [INDEX_WHITE] [nI] ;
        Game [nG].ptLastKing [INDEX_BLACK] [nI] = Game [nG].ptKing [INDEX_BLACK] [nI] ;
        }
    }

void BOARD_NullLastHighlight (int nG)
    {
    int nI ;

    Game [nG].ptLastHighlight [0].x = -2 ;
    Game [nG].ptLastHighlight [0].y = -2 ;
    Game [nG].ptLastHighlight [1] = Game [nG].ptLastHighlight [0] ;

    for (nI = 0 ; nI < 8 ; nI++)
        {
        Game [nG].ptLastKing [INDEX_WHITE] [nI] = Game [nG].ptLastHighlight [0] ;
        Game [nG].ptLastKing [INDEX_BLACK] [nI] = Game [nG].ptLastHighlight [0] ;
        }
    }

void BOARD_ResetPromoteBoard (int nG)
    {
    int nX, nY ;

    for (nY = 0 ; nY < 8 ; nY++)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            Game [nG].nPromoteBoard [nX] [nY] = 0 ;
            }
        }
    }

void BOARD_CopyPromoteBoard (int nG)
    {
    int nX, nY ;

    // copy promote board from Game [TEMP_GAME_INDEX] into Game [nG]
    for (nY = 0 ; nY < 8 ; nY++)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            Game [nG].nPromoteBoard [nX] [nY] = Game [TEMP_GAME_INDEX].nPromoteBoard [nX] [nY] ;
            }
        }
    }

void BOARD_ResizeNoBuffer (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nWC, nBC, nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    if (User.bShowClockOnTop)
        {
        BOARD_ResizeNoBuffer1 (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (Game [nG].nGameType == GAMETYPE_WILD5)
        {
        nWC = INDEX_BLACK ;
        nBC = INDEX_WHITE ;
        }
    else
        {
        nWC = INDEX_WHITE ;
        nBC = INDEX_BLACK ;
        }

    nxClient = nxClient - 2 ;
    nyClient = nyClient - 2 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    if (szHandle.cy > szClock.cy)
        {
        nH = szHandle.cy ;
        }
    else
        {
        nH = szClock.cy ;
        }

    if (User.bShowLagStat)
        {
        if (szLag.cy > nH)
            {
            nH = szLag.cy ;
            }
        }

    nT = nH + 2 ;
    nB = 2 + nH ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = nB + (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = nB + (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = nB + (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient >> 3 ;               // x square size

    if ((nss << 3) > nyClient)          // y board size
        {
        nss = nyClient >> 3 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    Game [nG].rBuffer.left   = -500 ;
    Game [nG].rBuffer.top    = -500 ;
    Game [nG].rBuffer.right  = 0 ;
    Game [nG].rBuffer.bottom = 0 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = -500 ;
    Game [nG].rBuffer1.top    = -500 ;
    Game [nG].rBuffer1.right  = 0 ;
    Game [nG].rBuffer1.bottom = 0 ;

    // assign board rect
    if (User.bShowCoord)
        {
        Game [nG].rBoard.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBoard.left = 0 ;
        }
    Game [nG].rBoard.top    = nT ;
    Game [nG].rBoard.right  = Game [nG].rBoard.left + (nss << 3) + 1 ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top  + (nss << 3) + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBoard.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // assign handle and clock rect
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBoard.bottom ;
        }

    if (Game [nG].bFlip)
        {
        Game [nG].rHandle [nWC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nWC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nBC].top = nB ;
        Game [nG].rClock  [nBC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nWC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nBC].top = nB ;
            }
        }
    else
        {
        Game [nG].rHandle [nBC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nBC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nWC].top = nB ;
        Game [nG].rClock  [nWC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nBC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nWC].top = nB ;
            }
        }

    nB = nB + nH ;
    nT = nT + (nH + 2) ;

    // assign remaining handle rect
    Game [nG].rHandle [nWC].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [nBC].left   = Game [nG].rBoard.left + 1 ;

    Game [nG].rHandle [nWC].right  = Game [nG].rHandle [nWC].left + szHandle.cx ;
    Game [nG].rHandle [nBC].right  = Game [nG].rHandle [nBC].left + szHandle.cx ;

    if (Game [nG].rHandle [nWC].right > Game [nG].rBoard.right)
        {
        Game [nG].rHandle [nWC].right = Game [nG].rBoard.right ;
        Game [nG].rHandle [nBC].right = Game [nG].rBoard.right ;
        }

    Game [nG].rHandle [nWC].bottom = Game [nG].rHandle [nWC].top  + szHandle.cy ;
    Game [nG].rHandle [nBC].bottom = Game [nG].rHandle [nBC].top  + szHandle.cy ;

    // assign remaining clock rect
    if (User.bShowLagStat)
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx - szLag.cx ;
        }
    else
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx ;
        }

    Game [nG].rClock [nBC].left = Game [nG].rClock [nWC].left ;

    Game [nG].rClock [nWC].right  = Game [nG].rClock [nWC].left + szClock.cx ;
    Game [nG].rClock [nBC].right  = Game [nG].rClock [nBC].left + szClock.cx ;

    Game [nG].rClock [nWC].bottom = Game [nG].rClock [nWC].top  + szClock.cy ;
    Game [nG].rClock [nBC].bottom = Game [nG].rClock [nBC].top  + szClock.cy ;

    // assign remaining lag stat, last move, and result rectangles
    if (User.bShowLagStat)
        {
        Game [nG].rLag [nWC].bottom = Game [nG].rLag [nWC].top + szLag.cy ;
        Game [nG].rLag [nWC].left   = Game [nG].rClock [nWC].right + 1 ;
        Game [nG].rLag [nWC].right  = Game [nG].rLag [nWC].left + szLag.cx ;

        Game [nG].rLag [nBC].bottom = Game [nG].rLag [nBC].top + szLag.cy ;
        Game [nG].rLag [nBC].left   = Game [nG].rLag [nWC].left ;
        Game [nG].rLag [nBC].right  = Game [nG].rLag [nWC].right ;
        }

    nB = nB + 2 ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBoard.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nss ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + (nss * 6) ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nss ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + (nss * 6) ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            if (User.bShowLagStat)
                {
                nI = Game [nG].rBoard.right - szLag.cx - szLag.cx ;

                if (nI < Game [nG].rClock [nWC].left)
                    {
                    nI = Game [nG].rClock [nWC].left ;
                    }
                }
            else
                {
                nI = Game [nG].rClock [nWC].left ;
                }

            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - nI) >> 2 ;
            nH = nU ;

            if (User.bShowLastMove)
                {
                if (Game [nG].rLastMove.right > (nI - 1))
                    {
                    Game [nG].rLastMove.right = (nI - 1) ;
                    }
                }

            if (User.bShowGameType)
                {
                if (Game [nG].rGameType.right > (nI - 1))
                    {
                    Game [nG].rGameType.right = (nI - 1) ;
                    }
                }
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeNoBuffer1 (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    nxClient = nxClient - 2 ;
    nyClient = nyClient - 2 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "W:%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    nT = (2 + szHandle.cy) ;

    if (User.bShowLagStat)
        {
        if (szLag.cy > szClock.cy)
            {
            nH = szLag.cy ;
            }
        else
            {
            nH = szClock.cy ;
            }
        nT = nT + (2 + nH) ;
        }
    else
        {
        nH = szClock.cy ;
        nT = nT + (2 + nH) ;
        }

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient >> 3 ;               // x square size

    if ((nss << 3) > nyClient)          // y board size
        {
        nss = nyClient >> 3 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    Game [nG].rBuffer.left   = -500 ;
    Game [nG].rBuffer.top    = -500 ;
    Game [nG].rBuffer.right  = 0 ;
    Game [nG].rBuffer.bottom = 0 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = -500 ;
    Game [nG].rBuffer1.top    = -500 ;
    Game [nG].rBuffer1.right  = 0 ;
    Game [nG].rBuffer1.bottom = 0 ;

    // assign board rect
    if (User.bShowCoord)
        {
        Game [nG].rBoard.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBoard.left = 0 ;
        }
    Game [nG].rBoard.top    = nT ;
    Game [nG].rBoard.right  = Game [nG].rBoard.left + (nss << 3) + 1 ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top  + (nss << 3) + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBoard.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // get top and bottom starting value
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBoard.bottom ;
        }

    nB = nB + 2 ;

    // assign handle rect
    Game [nG].rHandle [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [INDEX_WHITE].top    = nT ;
    Game [nG].rHandle [INDEX_WHITE].right  = Game [nG].rHandle [INDEX_WHITE].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_WHITE].bottom = Game [nG].rHandle [INDEX_WHITE].top  + szHandle.cy ;

    if (User.bShowLagStat)
        {
        nC = Game [nG].rBoard.right - szClock.cx - 1 - szLag.cx ;
        nX = nC ;
        }
    else
        {
        nC = Game [nG].rBoard.right - szClock.cx ;
        nX = Game [nG].rBoard.right - (szHandle.cx >> 1) ;
        }

    Game [nG].rHandle [INDEX_BLACK].left   = nX ;
    Game [nG].rHandle [INDEX_BLACK].top    = nT ;
    Game [nG].rHandle [INDEX_BLACK].right  = Game [nG].rHandle [INDEX_BLACK].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_BLACK].bottom = Game [nG].rHandle [INDEX_BLACK].top  + szHandle.cy ;

    nT = nT + (szHandle.cy + 2) ;

    // assign clock rect
    Game [nG].rClock [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rClock [INDEX_WHITE].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_WHITE].right  = Game [nG].rClock [INDEX_WHITE].left + szClock.cx ;
    Game [nG].rClock [INDEX_WHITE].bottom = Game [nG].rClock [INDEX_WHITE].top  + szClock.cy ;

    Game [nG].rClock [INDEX_BLACK].left   = nC ;
    Game [nG].rClock [INDEX_BLACK].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_BLACK].right  = Game [nG].rClock [INDEX_BLACK].left + szClock.cx ;
    Game [nG].rClock [INDEX_BLACK].bottom = Game [nG].rClock [INDEX_BLACK].top  + szClock.cy ;

    // assign lag status
    if (User.bShowLagStat)
        {
        Game [nG].rLag [INDEX_WHITE].left   = Game [nG].rClock [INDEX_WHITE].right + 1 ;
        Game [nG].rLag [INDEX_WHITE].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_WHITE].right  = Game [nG].rLag [INDEX_WHITE].left + szLag.cx ;
        Game [nG].rLag [INDEX_WHITE].bottom = Game [nG].rLag [INDEX_WHITE].top  + szLag.cy ;

        Game [nG].rLag [INDEX_BLACK].left   = Game [nG].rBoard.right - szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_BLACK].right  = Game [nG].rLag [INDEX_BLACK].left + szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].bottom = Game [nG].rLag [INDEX_BLACK].top  + szLag.cy ;
        }

    nT = nT + (nH + 2) ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBoard.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nss ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + (nss * 6) ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nss ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + (nss * 6) ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            if (User.bShowLagStat)
                {
                nI = Game [nG].rBoard.right - szLag.cx - szLag.cx ;

                if (nI < Game [nG].rClock [INDEX_BLACK].left)
                    {
                    nI = Game [nG].rClock [INDEX_BLACK].left ;
                    }
                }
            else
                {
                nI = Game [nG].rClock [INDEX_BLACK].left ;
                }

            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - nI) >> 2 ;
            nH = nU ;

            if (User.bShowLastMove)
                {
                if (Game [nG].rLastMove.right > (nI - 1))
                    {
                    Game [nG].rLastMove.right = (nI - 1) ;
                    }
                }

            if (User.bShowGameType)
                {
                if (Game [nG].rGameType.right > (nI - 1))
                    {
                    Game [nG].rGameType.right = (nI - 1) ;
                    }
                }
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeLeft (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nWC, nBC, nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    if (! TOOLBOX_ShowBuffer (nG))
        {
        BOARD_ResizeNoBuffer (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (User.bShowClockOnTop)
        {
        BOARD_ResizeLeft1 (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (Game [nG].nGameType == GAMETYPE_WILD5)
        {
        nWC = INDEX_BLACK ;
        nBC = INDEX_WHITE ;
        }
    else
        {
        nWC = INDEX_WHITE ;
        nBC = INDEX_BLACK ;
        }

    nxClient = nxClient - 4 ;
    nyClient = nyClient - 2 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    if (szHandle.cy > szClock.cy)
        {
        nH = szHandle.cy ;
        }
    else
        {
        nH = szClock.cy ;
        }

    if (User.bShowLagStat)
        {
        if (szLag.cy > nH)
            {
            nH = szLag.cy ;
            }
        }

    nT = nH + 2 ;
    nB = 2 + nH ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = nB + (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = nB + (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = nB + (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient / 10 ;               // x square size

    if ((nss << 3) > nyClient)          // y board size
        {
        nss = nyClient >> 3 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    Game [nG].rBuffer.left   = 0 ;
    Game [nG].rBuffer.top    = nT ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + nss + nss + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBuffer.top  + (nss << 3) + 1 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBuffer.top ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer.bottom ;

    // assign board rect
    Game [nG].rBoard.left   = Game [nG].rBuffer.right + 1 ;
    Game [nG].rBoard.top    = nT ;
    Game [nG].rBoard.right  = Game [nG].rBoard.left + (nss << 3) + 1 ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top  + (nss << 3) + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBoard.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = Game [nG].rBoard.right + 4 ;
            Game [nG].rVCoord [nI].right  = Game [nG].rVCoord [nI].left + szCoord.cx ;
            }
        }

    // assign handle and clock rect
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBoard.bottom ;
        }

    if (Game [nG].bFlip)
        {
        Game [nG].rHandle [nWC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nWC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nBC].top = nB ;
        Game [nG].rClock  [nBC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nWC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nBC].top = nB ;
            }
        }
    else
        {
        Game [nG].rHandle [nBC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nBC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nWC].top = nB ;
        Game [nG].rClock  [nWC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nBC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nWC].top = nB ;
            }
        }

    nB = nB + nH ;
    nT = nT + (nH + 2) ;

    // assign remaining handle rect
    Game [nG].rHandle [nWC].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [nBC].left   = Game [nG].rBoard.left + 1 ;

    Game [nG].rHandle [nWC].right  = Game [nG].rHandle [nWC].left + szHandle.cx ;
    Game [nG].rHandle [nBC].right  = Game [nG].rHandle [nBC].left + szHandle.cx ;

    if (Game [nG].rHandle [nWC].right > Game [nG].rBoard.right)
        {
        Game [nG].rHandle [nWC].right = Game [nG].rBoard.right ;
        Game [nG].rHandle [nBC].right = Game [nG].rBoard.right ;
        }

    Game [nG].rHandle [nWC].bottom = Game [nG].rHandle [nWC].top  + szHandle.cy ;
    Game [nG].rHandle [nBC].bottom = Game [nG].rHandle [nBC].top  + szHandle.cy ;

    // assign remaining clock rect
    if (User.bShowLagStat)
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx - szLag.cx ;
        }
    else
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx ;
        }

    Game [nG].rClock [nBC].left = Game [nG].rClock [nWC].left ;

    Game [nG].rClock [nWC].right  = Game [nG].rClock [nWC].left + szClock.cx ;
    Game [nG].rClock [nBC].right  = Game [nG].rClock [nBC].left + szClock.cx ;

    Game [nG].rClock [nWC].bottom = Game [nG].rClock [nWC].top  + szClock.cy ;
    Game [nG].rClock [nBC].bottom = Game [nG].rClock [nBC].top  + szClock.cy ;

    // assign remaining lag stat, last move, and result rectangles
    if (User.bShowLagStat)
        {
        Game [nG].rLag [nWC].bottom = Game [nG].rLag [nWC].top + szLag.cy ;
        Game [nG].rLag [nWC].left   = Game [nG].rClock [nWC].right + 1 ;
        Game [nG].rLag [nWC].right  = Game [nG].rLag [nWC].left + szLag.cx ;

        Game [nG].rLag [nBC].bottom = Game [nG].rLag [nBC].top + szLag.cy ;
        Game [nG].rLag [nBC].left   = Game [nG].rLag [nWC].left ;
        Game [nG].rLag [nBC].right  = Game [nG].rLag [nWC].right ;
        }

    nB = nB + 2 ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top  = 0 ;
    Game [nG].rActual.left = 0 ;

    if (User.bShowCoord)
        {
        Game [nG].rActual.right = Game [nG].rVCoord [0].right + 1 ;
        }
    else
        {
        Game [nG].rActual.right = Game [nG].rBoard.right + 1 ;
        }

    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nss ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + (nss * 6) ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nss ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + (nss * 6) ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            nI = Game [nG].rBuffer.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBuffer.right - nI) >> 2 ;
            nH = nU ;
            }
        else
            {
            nI = Game [nG].rBuffer.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBuffer.right - Game [nG].rBuffer.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeLeft1 (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    nxClient = nxClient - 4 ;
    nyClient = nyClient - 2 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "W:%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    nT = (2 + szHandle.cy) ;

    if (User.bShowLagStat)
        {
        if (szLag.cy > szClock.cy)
            {
            nH = szLag.cy ;
            }
        else
            {
            nH = szClock.cy ;
            }
        nT = nT + (2 + nH) ;
        }
    else
        {
        nH = szClock.cy ;
        nT = nT + (2 + nH) ;
        }

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient / 10 ;               // x square size

    if ((nss << 3) > nyClient)          // y board size
        {
        nss = nyClient >> 3 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    Game [nG].rBuffer.left   = 0 ;
    Game [nG].rBuffer.top    = nT ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + nss + nss + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBuffer.top  + (nss << 3) + 1 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBuffer.top ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer.bottom ;

    // assign board rect
    Game [nG].rBoard.left   = Game [nG].rBuffer.right + 1 ;
    Game [nG].rBoard.top    = nT ;
    Game [nG].rBoard.right  = Game [nG].rBoard.left + (nss << 3) + 1 ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top  + (nss << 3) + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBoard.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = Game [nG].rBoard.right + 4 ;
            Game [nG].rVCoord [nI].right  = Game [nG].rVCoord [nI].left + szCoord.cx ;
            }
        }

    // get top and bottom starting value
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBoard.bottom ;
        }

    nB = nB + 2 ;

    // assign handle rect
    Game [nG].rHandle [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [INDEX_WHITE].top    = nT ;
    Game [nG].rHandle [INDEX_WHITE].right  = Game [nG].rHandle [INDEX_WHITE].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_WHITE].bottom = Game [nG].rHandle [INDEX_WHITE].top  + szHandle.cy ;

    if (User.bShowLagStat)
        {
        nC = Game [nG].rBoard.right - szClock.cx - 1 - szLag.cx ;
        nX = nC ;
        }
    else
        {
        nC = Game [nG].rBoard.right - szClock.cx ;
        nX = Game [nG].rBoard.right - (szHandle.cx >> 1) ;
        }

    Game [nG].rHandle [INDEX_BLACK].left   = nX ;
    Game [nG].rHandle [INDEX_BLACK].top    = nT ;
    Game [nG].rHandle [INDEX_BLACK].right  = Game [nG].rHandle [INDEX_BLACK].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_BLACK].bottom = Game [nG].rHandle [INDEX_BLACK].top  + szHandle.cy ;

    nT = nT + (szHandle.cy + 2) ;

    // assign clock rect
    Game [nG].rClock [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rClock [INDEX_WHITE].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_WHITE].right  = Game [nG].rClock [INDEX_WHITE].left + szClock.cx ;
    Game [nG].rClock [INDEX_WHITE].bottom = Game [nG].rClock [INDEX_WHITE].top  + szClock.cy ;

    Game [nG].rClock [INDEX_BLACK].left   = nC ;
    Game [nG].rClock [INDEX_BLACK].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_BLACK].right  = Game [nG].rClock [INDEX_BLACK].left + szClock.cx ;
    Game [nG].rClock [INDEX_BLACK].bottom = Game [nG].rClock [INDEX_BLACK].top  + szClock.cy ;

    // assign lag status
    if (User.bShowLagStat)
        {
        Game [nG].rLag [INDEX_WHITE].left   = Game [nG].rClock [INDEX_WHITE].right + 1 ;
        Game [nG].rLag [INDEX_WHITE].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_WHITE].right  = Game [nG].rLag [INDEX_WHITE].left + szLag.cx ;
        Game [nG].rLag [INDEX_WHITE].bottom = Game [nG].rLag [INDEX_WHITE].top  + szLag.cy ;

        Game [nG].rLag [INDEX_BLACK].left   = Game [nG].rBoard.right - szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_BLACK].right  = Game [nG].rLag [INDEX_BLACK].left + szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].bottom = Game [nG].rLag [INDEX_BLACK].top  + szLag.cy ;
        }

    nT = nT + (nH + 2) ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top  = 0 ;
    Game [nG].rActual.left = 0 ;

    if (User.bShowCoord)
        {
        Game [nG].rActual.right = Game [nG].rVCoord [0].right + 1 ;
        }
    else
        {
        Game [nG].rActual.right = Game [nG].rBoard.right + 1 ;
        }

    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nss ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + (nss * 6) ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nss ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + (nss * 6) ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            nI = Game [nG].rBuffer.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBuffer.right - nI) >> 2 ;
            nH = nU ;
            }
        else
            {
            nI = Game [nG].rBuffer.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBuffer.right - Game [nG].rBuffer.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeRight (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nWC, nBC, nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    if (! TOOLBOX_ShowBuffer (nG))
        {
        BOARD_ResizeNoBuffer (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (User.bShowClockOnTop)
        {
        BOARD_ResizeRight1 (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (Game [nG].nGameType == GAMETYPE_WILD5)
        {
        nWC = INDEX_BLACK ;
        nBC = INDEX_WHITE ;
        }
    else
        {
        nWC = INDEX_WHITE ;
        nBC = INDEX_BLACK ;
        }

    nxClient = nxClient - 4 ;
    nyClient = nyClient - 2 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    if (szHandle.cy > szClock.cy)
        {
        nH = szHandle.cy ;
        }
    else
        {
        nH = szClock.cy ;
        }

    if (User.bShowLagStat)
        {
        if (szLag.cy > nH)
            {
            nH = szLag.cy ;
            }
        }

    nT = nH + 2 ;
    nB = 2 + nH ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = nB + (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = nB + (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = nB + (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient / 10 ;               // x square size

    if ((nss << 3) > nyClient)          // y board size
        {
        nss = nyClient >> 3 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign board rect
    if (User.bShowCoord)
        {
        Game [nG].rBoard.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBoard.left = 0 ;
        }
    Game [nG].rBoard.top    = nT ;
    Game [nG].rBoard.right  = Game [nG].rBoard.left + (nss << 3) + 1 ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top  + (nss << 3) + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBoard.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // assign buffer rect
    Game [nG].rBuffer.left   = Game [nG].rBoard.right + 1 ;
    Game [nG].rBuffer.top    = Game [nG].rBoard.top ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + nss + nss + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBoard.bottom ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBuffer.top ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer.bottom ;

    // assign handle and clock rect
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBoard.bottom ;
        }

    if (Game [nG].bFlip)
        {
        Game [nG].rHandle [nWC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nWC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nBC].top = nB ;
        Game [nG].rClock  [nBC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nWC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nBC].top = nB ;
            }
        }
    else
        {
        Game [nG].rHandle [nBC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nBC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nWC].top = nB ;
        Game [nG].rClock  [nWC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nBC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nWC].top = nB ;
            }
        }

    nB = nB + nH ;
    nT = nT + (nH + 2) ;

    // assign remaining handle rect
    Game [nG].rHandle [nWC].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [nBC].left   = Game [nG].rBoard.left + 1 ;

    Game [nG].rHandle [nWC].right  = Game [nG].rHandle [nWC].left + szHandle.cx ;
    Game [nG].rHandle [nBC].right  = Game [nG].rHandle [nBC].left + szHandle.cx ;

    if (Game [nG].rHandle [nWC].right > Game [nG].rBoard.right)
        {
        Game [nG].rHandle [nWC].right = Game [nG].rBoard.right ;
        Game [nG].rHandle [nBC].right = Game [nG].rBoard.right ;
        }

    Game [nG].rHandle [nWC].bottom = Game [nG].rHandle [nWC].top  + szHandle.cy ;
    Game [nG].rHandle [nBC].bottom = Game [nG].rHandle [nBC].top  + szHandle.cy ;

    // assign remaining clock rect
    Game [nG].rClock [nWC].left   = Game [nG].rBoard.right - szClock.cx ;
    Game [nG].rClock [nBC].left   = Game [nG].rClock [nWC].left ;

    Game [nG].rClock [nWC].right  = Game [nG].rClock [nWC].left + szClock.cx ;
    Game [nG].rClock [nBC].right  = Game [nG].rClock [nBC].left + szClock.cx ;

    Game [nG].rClock [nWC].bottom = Game [nG].rClock [nWC].top  + szClock.cy ;
    Game [nG].rClock [nBC].bottom = Game [nG].rClock [nBC].top  + szClock.cy ;

    // assign remaining lag stat, last move, and result rectangles
    if (User.bShowLagStat)
        {
        Game [nG].rLag [nWC].bottom = Game [nG].rLag [nWC].top + szLag.cy ;
        Game [nG].rLag [nWC].left   = Game [nG].rClock [nWC].right + 1 ;
        Game [nG].rLag [nWC].right  = Game [nG].rLag [nWC].left + szLag.cx ;

        Game [nG].rLag [nBC].bottom = Game [nG].rLag [nBC].top + szLag.cy ;
        Game [nG].rLag [nBC].left   = Game [nG].rLag [nWC].left ;
        Game [nG].rLag [nBC].right  = Game [nG].rLag [nWC].right ;
        }

    nB = nB + 2 ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBuffer.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nss ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + (nss * 6) ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nss ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + (nss * 6) ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            nI = Game [nG].rBoard.right + 1 ;
            nT = nButtonY ;
            nB = (nXC - nI) >> 2 ;
            nH = nU ;
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeRight1 (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    nxClient = nxClient - 4 ;
    nyClient = nyClient - 2 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "W:%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    nT = (2 + szHandle.cy) ;

    if (User.bShowLagStat)
        {
        if (szLag.cy > szClock.cy)
            {
            nH = szLag.cy ;
            }
        else
            {
            nH = szClock.cy ;
            }
        nT = nT + (2 + nH) ;
        }
    else
        {
        nH = szClock.cy ;
        nT = nT + (2 + nH) ;
        }

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient / 10 ;               // x square size

    if ((nss << 3) > nyClient)          // y board size
        {
        nss = nyClient >> 3 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign board rect
    if (User.bShowCoord)
        {
        Game [nG].rBoard.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBoard.left = 0 ;
        }
    Game [nG].rBoard.top    = nT ;
    Game [nG].rBoard.right  = Game [nG].rBoard.left + (nss << 3) + 1 ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top  + (nss << 3) + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBoard.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // assign buffer rect
    Game [nG].rBuffer.left   = Game [nG].rBoard.right + 1 ;
    Game [nG].rBuffer.top    = Game [nG].rBoard.top ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + nss + nss + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBoard.bottom ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBuffer.top ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer.bottom ;

    // get top and bottom starting value
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBoard.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBoard.bottom ;
        }

    nB = nB + 2 ;

    // assign handle rect
    Game [nG].rHandle [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [INDEX_WHITE].top    = nT ;
    Game [nG].rHandle [INDEX_WHITE].right  = Game [nG].rHandle [INDEX_WHITE].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_WHITE].bottom = Game [nG].rHandle [INDEX_WHITE].top  + szHandle.cy ;

    Game [nG].rHandle [INDEX_BLACK].left   = Game [nG].rBoard.right - szClock.cx ;
    Game [nG].rHandle [INDEX_BLACK].top    = nT ;
    Game [nG].rHandle [INDEX_BLACK].right  = Game [nG].rHandle [INDEX_BLACK].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_BLACK].bottom = Game [nG].rHandle [INDEX_BLACK].top  + szHandle.cy ;

    nT = nT + (szHandle.cy + 2) ;

    // assign clock rect
    Game [nG].rClock [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rClock [INDEX_WHITE].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_WHITE].right  = Game [nG].rClock [INDEX_WHITE].left + szClock.cx ;
    Game [nG].rClock [INDEX_WHITE].bottom = Game [nG].rClock [INDEX_WHITE].top  + szClock.cy ;

    Game [nG].rClock [INDEX_BLACK].left   = Game [nG].rBoard.right - szClock.cx ;
    Game [nG].rClock [INDEX_BLACK].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_BLACK].right  = Game [nG].rClock [INDEX_BLACK].left + szClock.cx ;
    Game [nG].rClock [INDEX_BLACK].bottom = Game [nG].rClock [INDEX_BLACK].top  + szClock.cy ;

    // assign lag status
    if (User.bShowLagStat)
        {
        Game [nG].rLag [INDEX_WHITE].left   = Game [nG].rClock [INDEX_WHITE].right + 1 ;
        Game [nG].rLag [INDEX_WHITE].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_WHITE].right  = Game [nG].rLag [INDEX_WHITE].left + szLag.cx ;
        Game [nG].rLag [INDEX_WHITE].bottom = Game [nG].rLag [INDEX_WHITE].top  + szLag.cy ;

        Game [nG].rLag [INDEX_BLACK].left   = Game [nG].rBoard.right + 1 ;
        Game [nG].rLag [INDEX_BLACK].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_BLACK].right  = Game [nG].rLag [INDEX_BLACK].left + szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].bottom = Game [nG].rLag [INDEX_BLACK].top  + szLag.cy ;
        }

    nT = nT + (nH + 2) ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBuffer.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nss ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + (nss * 6) ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nss ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nss + nss ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nss ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + (nss * 6) ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + (nss * 7) ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + (nss * 5) ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + (nss * 6) ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            nI = Game [nG].rBoard.right + 1 ;
            nT = nButtonY ;
            nB = (nXC - nI) >> 2 ;
            nH = nU ;
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeTopBottomL (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nWC, nBC, nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    if (! TOOLBOX_ShowBuffer (nG))
        {
        BOARD_ResizeNoBuffer (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (User.bShowClockOnTop)
        {
        BOARD_ResizeTopBottomL1 (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (Game [nG].nGameType == GAMETYPE_WILD5)
        {
        nWC = INDEX_BLACK ;
        nBC = INDEX_WHITE ;
        }
    else
        {
        nWC = INDEX_WHITE ;
        nBC = INDEX_BLACK ;
        }

    nxClient = nxClient - 2 ;
    nyClient = nyClient - 6 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    if (szHandle.cy > szClock.cy)
        {
        nH = szHandle.cy ;
        }
    else
        {
        nH = szClock.cy ;
        }

    if (User.bShowLagStat)
        {
        if (szLag.cy > nH)
            {
            nH = szLag.cy ;
            }
        }

    nT = nH + 2 ;
    nB = 2 + nH ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = nB + (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = nB + (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = nB + (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient >> 3 ;               // x square size

    if ((nss * 10) > nyClient)          // y board size
        {
        nss = nyClient / 10 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    if (User.bShowCoord)
        {
        Game [nG].rBuffer.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBuffer.left = 0 ;
        }
    Game [nG].rBuffer.top    = nT ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + (nss << 3) + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBuffer.top  + nss + 1 ;

    // assign board rect
    Game [nG].rBoard.left   = Game [nG].rBuffer.left ;
    Game [nG].rBoard.top    = Game [nG].rBuffer.bottom + 1 ;
    Game [nG].rBoard.right  = Game [nG].rBuffer.right ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top + (nss << 3) + 1 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBoard.bottom + 1 ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer1.top + nss + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBuffer1.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // assign handle and clock rect
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBuffer1.bottom ;
        }

    if (Game [nG].bFlip)
        {
        Game [nG].rHandle [nWC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nWC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nBC].top = nB ;
        Game [nG].rClock  [nBC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nWC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nBC].top = nB ;
            }
        }
    else
        {
        Game [nG].rHandle [nBC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nBC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nWC].top = nB ;
        Game [nG].rClock  [nWC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nBC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nWC].top = nB ;
            }
        }

    nB = nB + nH ;
    nT = nT + (nH + 2) ;

    // assign remaining handle rect
    Game [nG].rHandle [nWC].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [nBC].left   = Game [nG].rBoard.left + 1 ;

    Game [nG].rHandle [nWC].right  = Game [nG].rHandle [nWC].left + szHandle.cx ;
    Game [nG].rHandle [nBC].right  = Game [nG].rHandle [nBC].left + szHandle.cx ;

    if (Game [nG].rHandle [nWC].right > Game [nG].rBoard.right)
        {
        Game [nG].rHandle [nWC].right = Game [nG].rBoard.right ;
        Game [nG].rHandle [nBC].right = Game [nG].rBoard.right ;
        }

    Game [nG].rHandle [nWC].bottom = Game [nG].rHandle [nWC].top  + szHandle.cy ;
    Game [nG].rHandle [nBC].bottom = Game [nG].rHandle [nBC].top  + szHandle.cy ;

    // assign remaining clock rect
    if (User.bShowLagStat)
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx - szLag.cx ;
        }
    else
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx ;
        }
    Game [nG].rClock [nBC].left = Game [nG].rClock [nWC].left ;

    Game [nG].rClock [nWC].right  = Game [nG].rClock [nWC].left + szClock.cx ;
    Game [nG].rClock [nBC].right  = Game [nG].rClock [nBC].left + szClock.cx ;

    Game [nG].rClock [nWC].bottom = Game [nG].rClock [nWC].top  + szClock.cy ;
    Game [nG].rClock [nBC].bottom = Game [nG].rClock [nBC].top  + szClock.cy ;

    // assign remaining lag stat, last move, and result rectangles
    if (User.bShowLagStat)
        {
        Game [nG].rLag [nWC].bottom = Game [nG].rLag [nWC].top + szLag.cy ;
        Game [nG].rLag [nWC].left   = Game [nG].rClock [nWC].right + 1 ;
        Game [nG].rLag [nWC].right  = Game [nG].rLag [nWC].left + szLag.cx ;

        Game [nG].rLag [nBC].bottom = Game [nG].rLag [nBC].top + szLag.cy ;
        Game [nG].rLag [nBC].left   = Game [nG].rLag [nWC].left ;
        Game [nG].rLag [nBC].right  = Game [nG].rLag [nWC].right ;
        }

    nB = nB + 2 ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBoard.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    nB = Game [nG].rBuffer1.top - Game [nG].rBuffer.top ;

    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nB ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nB ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            if (User.bShowLagStat)
                {
                nI = Game [nG].rBoard.right - szLag.cx - szLag.cx ;

                if (nI < Game [nG].rClock [nWC].left)
                    {
                    nI = Game [nG].rClock [nWC].left ;
                    }
                }
            else
                {
                nI = Game [nG].rClock [nWC].left ;
                }

            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - nI) >> 2 ;
            nH = nU ;

            if (User.bShowLastMove)
                {
                if (Game [nG].rLastMove.right > (nI - 1))
                    {
                    Game [nG].rLastMove.right = (nI - 1) ;
                    }
                }

            if (User.bShowGameType)
                {
                if (Game [nG].rGameType.right > (nI - 1))
                    {
                    Game [nG].rGameType.right = (nI - 1) ;
                    }
                }
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeTopBottomL1 (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    nxClient = nxClient - 2 ;
    nyClient = nyClient - 6 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "W:%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    nT = (2 + szHandle.cy) ;

    if (User.bShowLagStat)
        {
        if (szLag.cy > szClock.cy)
            {
            nH = szLag.cy ;
            }
        else
            {
            nH = szClock.cy ;
            }
        nT = nT + (2 + nH) ;
        }
    else
        {
        nH = szClock.cy ;
        nT = nT + (2 + nH) ;
        }

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient >> 3 ;               // x square size

    if ((nss * 10) > nyClient)          // y board size
        {
        nss = nyClient / 10 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    if (User.bShowCoord)
        {
        Game [nG].rBuffer.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBuffer.left = 0 ;
        }
    Game [nG].rBuffer.top    = nT ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + (nss << 3) + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBuffer.top  + nss + 1 ;

    // assign board rect
    Game [nG].rBoard.left   = Game [nG].rBuffer.left ;
    Game [nG].rBoard.top    = Game [nG].rBuffer.bottom + 1 ;
    Game [nG].rBoard.right  = Game [nG].rBuffer.right ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top + (nss << 3) + 1 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBoard.bottom + 1 ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer1.top + nss + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBuffer1.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // get top and bottom starting value
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBuffer1.bottom ;
        }

    nB = nB + 2 ;

    // assign handle rect
    Game [nG].rHandle [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [INDEX_WHITE].top    = nT ;
    Game [nG].rHandle [INDEX_WHITE].right  = Game [nG].rHandle [INDEX_WHITE].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_WHITE].bottom = Game [nG].rHandle [INDEX_WHITE].top  + szHandle.cy ;

    if (User.bShowLagStat)
        {
        nC = Game [nG].rBoard.right - szClock.cx - 1 - szLag.cx ;
        nX = nC ;
        }
    else
        {
        nC = Game [nG].rBoard.right - szClock.cx ;
        nX = Game [nG].rBoard.right - (szHandle.cx >> 1) ;
        }

    Game [nG].rHandle [INDEX_BLACK].left   = nX ;
    Game [nG].rHandle [INDEX_BLACK].top    = nT ;
    Game [nG].rHandle [INDEX_BLACK].right  = Game [nG].rHandle [INDEX_BLACK].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_BLACK].bottom = Game [nG].rHandle [INDEX_BLACK].top  + szHandle.cy ;

    nT = nT + (szHandle.cy + 2) ;

    // assign clock rect
    Game [nG].rClock [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rClock [INDEX_WHITE].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_WHITE].right  = Game [nG].rClock [INDEX_WHITE].left + szClock.cx ;
    Game [nG].rClock [INDEX_WHITE].bottom = Game [nG].rClock [INDEX_WHITE].top  + szClock.cy ;

    Game [nG].rClock [INDEX_BLACK].left   = nC ;
    Game [nG].rClock [INDEX_BLACK].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_BLACK].right  = Game [nG].rClock [INDEX_BLACK].left + szClock.cx ;
    Game [nG].rClock [INDEX_BLACK].bottom = Game [nG].rClock [INDEX_BLACK].top  + szClock.cy ;

    // assign lag status
    if (User.bShowLagStat)
        {
        Game [nG].rLag [INDEX_WHITE].left   = Game [nG].rClock [INDEX_WHITE].right + 1 ;
        Game [nG].rLag [INDEX_WHITE].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_WHITE].right  = Game [nG].rLag [INDEX_WHITE].left + szLag.cx ;
        Game [nG].rLag [INDEX_WHITE].bottom = Game [nG].rLag [INDEX_WHITE].top  + szLag.cy ;

        Game [nG].rLag [INDEX_BLACK].left   = Game [nG].rBoard.right - szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_BLACK].right  = Game [nG].rLag [INDEX_BLACK].left + szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].bottom = Game [nG].rLag [INDEX_BLACK].top  + szLag.cy ;
        }

    nT = nT + (nH + 2) ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBoard.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    nB = Game [nG].rBuffer1.top - Game [nG].rBuffer.top ;

    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nB ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = 1 ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = 1 ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = 1 + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = 1 + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = 1 + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = 1 + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nB ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            if (User.bShowLagStat)
                {
                nI = Game [nG].rBoard.right - szLag.cx - szLag.cx ;

                if (nI < Game [nG].rClock [INDEX_BLACK].left)
                    {
                    nI = Game [nG].rClock [INDEX_BLACK].left ;
                    }
                }
            else
                {
                nI = Game [nG].rClock [INDEX_BLACK].left ;
                }

            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - nI) >> 2 ;
            nH = nU ;

            if (User.bShowLastMove)
                {
                if (Game [nG].rLastMove.right > (nI - 1))
                    {
                    Game [nG].rLastMove.right = (nI - 1) ;
                    }
                }

            if (User.bShowGameType)
                {
                if (Game [nG].rGameType.right > (nI - 1))
                    {
                    Game [nG].rGameType.right = (nI - 1) ;
                    }
                }
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeTopBottomR (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nWC, nBC, nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    if (! TOOLBOX_ShowBuffer (nG))
        {
        BOARD_ResizeNoBuffer (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (User.bShowClockOnTop)
        {
        BOARD_ResizeTopBottomR1 (nG, hdc, nxClient, nyClient) ;
        return ;
        }

    if (Game [nG].nGameType == GAMETYPE_WILD5)
        {
        nWC = INDEX_BLACK ;
        nBC = INDEX_WHITE ;
        }
    else
        {
        nWC = INDEX_WHITE ;
        nBC = INDEX_BLACK ;
        }

    nxClient = nxClient - 2 ;
    nyClient = nyClient - 6 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    if (szHandle.cy > szClock.cy)
        {
        nH = szHandle.cy ;
        }
    else
        {
        nH = szClock.cy ;
        }

    if (User.bShowLagStat)
        {
        if (szLag.cy > nH)
            {
            nH = szLag.cy ;
            }
        }

    nT = nH + 2 ;
    nB = 2 + nH ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = nB + (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = nB + (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = nB + (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient >> 3 ;               // x square size

    if ((nss * 10) > nyClient)          // y board size
        {
        nss = nyClient / 10 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    if (User.bShowCoord)
        {
        Game [nG].rBuffer.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBuffer.left = 0 ;
        }
    Game [nG].rBuffer.top    = nT ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + (nss << 3) + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBuffer.top  + nss + 1 ;

    // assign board rect
    Game [nG].rBoard.left   = Game [nG].rBuffer.left ;
    Game [nG].rBoard.top    = Game [nG].rBuffer.bottom + 1 ;
    Game [nG].rBoard.right  = Game [nG].rBuffer.right ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top + (nss << 3) + 1 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBoard.bottom + 1 ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer1.top + nss + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBuffer1.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // assign handle and clock rect
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBuffer1.bottom ;
        }

    if (Game [nG].bFlip)
        {
        Game [nG].rHandle [nWC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nWC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nBC].top = nB ;
        Game [nG].rClock  [nBC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nWC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nBC].top = nB ;
            }
        }
    else
        {
        Game [nG].rHandle [nBC].top = (nT + nH) - szHandle.cy ;
        Game [nG].rClock  [nBC].top = (nT + nH) - szClock.cy ;

        nB = nB + 2 ;

        Game [nG].rHandle [nWC].top = nB ;
        Game [nG].rClock  [nWC].top = nB ;

        if (User.bShowLagStat)
            {
            Game [nG].rLag [nBC].top = (nT + nH) - szLag.cy ;
            Game [nG].rLag [nWC].top = nB ;
            }
        }

    nB = nB + nH ;
    nT = nT + (nH + 2) ;

    // assign remaining handle rect
    Game [nG].rHandle [nWC].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [nBC].left   = Game [nG].rBoard.left + 1 ;

    Game [nG].rHandle [nWC].right  = Game [nG].rHandle [nWC].left + szHandle.cx ;
    Game [nG].rHandle [nBC].right  = Game [nG].rHandle [nBC].left + szHandle.cx ;

    if (Game [nG].rHandle [nWC].right > Game [nG].rBoard.right)
        {
        Game [nG].rHandle [nWC].right = Game [nG].rBoard.right ;
        Game [nG].rHandle [nBC].right = Game [nG].rBoard.right ;
        }

    Game [nG].rHandle [nWC].bottom = Game [nG].rHandle [nWC].top  + szHandle.cy ;
    Game [nG].rHandle [nBC].bottom = Game [nG].rHandle [nBC].top  + szHandle.cy ;

    // assign remaining clock rect
    if (User.bShowLagStat)
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx - szLag.cx ;
        }
    else
        {
        Game [nG].rClock [nWC].left = Game [nG].rBoard.right - szClock.cx ;
        }
    Game [nG].rClock [nBC].left = Game [nG].rClock [nWC].left ;

    Game [nG].rClock [nWC].right  = Game [nG].rClock [nWC].left + szClock.cx ;
    Game [nG].rClock [nBC].right  = Game [nG].rClock [nBC].left + szClock.cx ;

    Game [nG].rClock [nWC].bottom = Game [nG].rClock [nWC].top  + szClock.cy ;
    Game [nG].rClock [nBC].bottom = Game [nG].rClock [nBC].top  + szClock.cy ;

    // assign remaining lag stat, last move, and result rectangles
    if (User.bShowLagStat)
        {
        Game [nG].rLag [nWC].bottom = Game [nG].rLag [nWC].top + szLag.cy ;
        Game [nG].rLag [nWC].left   = Game [nG].rClock [nWC].right + 1 ;
        Game [nG].rLag [nWC].right  = Game [nG].rLag [nWC].left + szLag.cx ;

        Game [nG].rLag [nBC].bottom = Game [nG].rLag [nBC].top + szLag.cy ;
        Game [nG].rLag [nBC].left   = Game [nG].rLag [nWC].left ;
        Game [nG].rLag [nBC].right  = Game [nG].rLag [nWC].right ;
        }

    nB = nB + 2 ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBoard.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    nB = Game [nG].rBuffer1.top - Game [nG].rBuffer.top ;

    nI = 1 + nss + nss + nss ;

    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = nI ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = nI ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nB ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = nI ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = nI ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nB ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            if (User.bShowLagStat)
                {
                nI = Game [nG].rBoard.right - szLag.cx - szLag.cx ;

                if (nI < Game [nG].rClock [nWC].left)
                    {
                    nI = Game [nG].rClock [nWC].left ;
                    }
                }
            else
                {
                nI = Game [nG].rClock [nWC].left ;
                }

            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - nI) >> 2 ;
            nH = nU ;

            if (User.bShowLastMove)
                {
                if (Game [nG].rLastMove.right > (nI - 1))
                    {
                    Game [nG].rLastMove.right = (nI - 1) ;
                    }
                }

            if (User.bShowGameType)
                {
                if (Game [nG].rGameType.right > (nI - 1))
                    {
                    Game [nG].rGameType.right = (nI - 1) ;
                    }
                }
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_ResizeTopBottomR1 (int nG, HDC hdc, int nxClient, int nyClient)
    {
    HFONT hfOld ;
    SIZE  szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szCoord ;
    int   nH, nT, nB, nU, nI, nss, nC, nX, nXC, bShowB, nButtonY ;
    char  cTmp [100] ;

    nxClient = nxClient - 2 ;
    nyClient = nyClient - 6 ;

    nXC = nxClient ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;
    GetTextExtentPoint32 (hdc, "Lag:00:00", 9, &szLag) ;

    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;

    if (User.bShowRating)
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW (9999)", 23, &szHandle) ;
        }
    else
        {
        GetTextExtentPoint32 (hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;
        }

    SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    sprintf (cTmp, "W:%s", CLOCK_TimeString (23 * 60 * 60 * 1000L)) ;
    GetTextExtentPoint32 (hdc, cTmp, strlen (cTmp), &szClock) ;

    SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
    GetTextExtentPoint32 (hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
    GetTextExtentPoint32 (hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szLMX) ;

    SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;
    GetTextExtentPoint32 (hdc, "W", 1, &szResult) ;

    SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;
    GetTextExtentPoint32 (hdc, "8", 1, &szCoord) ;

    SelectObject (hdc, hfOld) ;

    if (User.bShowMoveButton)
        {
        bShowB = 1 ;
        }
    else
        {
        if (nG == INDEX_PLAY)
            {
            if ((Game [nG].nGameNumber > 0) && (! Game [nG].bPlaying))
                {
                bShowB = 1 ;
                }
            else
                {
                bShowB = 0 ;
                }
            }
        else
            {
            bShowB = 0 ;
            }
        }

    nT = (2 + szHandle.cy) ;

    if (User.bShowLagStat)
        {
        if (szLag.cy > szClock.cy)
            {
            nH = szLag.cy ;
            }
        else
            {
            nH = szClock.cy ;
            }
        nT = nT + (2 + nH) ;
        }
    else
        {
        nH = szClock.cy ;
        nT = nT + (2 + nH) ;
        }

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            if (szGameType.cy > szLastMove.cy)
                {
                nU = szGameType.cy ;
                }
            else
                {
                nU = szLastMove.cy ;
                }
            }
        else
            {
            nU = szLastMove.cy ;
            }
        nB = (2 + nU) ;
        }
    else
    if (User.bShowGameType)
        {
        nU = szGameType.cy ;
        nB = (2 + nU) ;
        }
    else
        {
        if (szGameType.cy > szLastMove.cy)
            {
            nU = szGameType.cy ;
            }
        else
            {
            nU = szLastMove.cy ;
            }

        if (bShowB)
            {
            nB = (2 + nU) ;
            }
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + (2 + szResult.cy) ;
        }

    if (User.bShowCoord)
        {
        nB = nB + (1 + szCoord.cy) ;

        nxClient = nxClient - szCoord.cx - 4 ;
        }

    nyClient = nyClient - (nT + nB) ;

    nss = nxClient >> 3 ;               // x square size

    if ((nss * 10) > nyClient)          // y board size
        {
        nss = nyClient / 10 ;           // x square size
        }

    Game [nG].nss  = nss ;              // assign square size
    Game [nG].nhss = nss >> 1 ;         // assign half square size
    Game [nG].ndss = nss << 1 ;         // assign double square size

    // assign buffer rect
    if (User.bShowCoord)
        {
        Game [nG].rBuffer.left = 4 + szCoord.cx ;
        }
    else
        {
        Game [nG].rBuffer.left = 0 ;
        }
    Game [nG].rBuffer.top    = nT ;
    Game [nG].rBuffer.right  = Game [nG].rBuffer.left + (nss << 3) + 1 ;
    Game [nG].rBuffer.bottom = Game [nG].rBuffer.top  + nss + 1 ;

    // assign board rect
    Game [nG].rBoard.left   = Game [nG].rBuffer.left ;
    Game [nG].rBoard.top    = Game [nG].rBuffer.bottom + 1 ;
    Game [nG].rBoard.right  = Game [nG].rBuffer.right ;
    Game [nG].rBoard.bottom = Game [nG].rBoard.top + (nss << 3) + 1 ;

    // assign buffer rect 1
    Game [nG].rBuffer1.left   = Game [nG].rBuffer.left ;
    Game [nG].rBuffer1.top    = Game [nG].rBoard.bottom + 1 ;
    Game [nG].rBuffer1.right  = Game [nG].rBuffer.right ;
    Game [nG].rBuffer1.bottom = Game [nG].rBuffer1.top + nss + 1 ;

    // assign coordinates rects
    if (User.bShowCoord)
        {
        nX = (szCoord.cx >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.left + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rHCoord [nI].top    = Game [nG].rBuffer1.bottom + 1 ;
            Game [nG].rHCoord [nI].bottom = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
            Game [nG].rHCoord [nI].left   = nC ;
            Game [nG].rHCoord [nI].right  = nC + szCoord.cx ;
            }

        nX = (szCoord.cy >> 1) ;

        for (nI = 0 ; nI < 8 ; nI++)
            {
            nC = ((Game [nG].rBoard.top + (nss * nI) + Game [nG].nhss) - nX) ;

            Game [nG].rVCoord [nI].top    = nC ;
            Game [nG].rVCoord [nI].bottom = nC + szCoord.cy ;
            Game [nG].rVCoord [nI].left   = 2 ;
            Game [nG].rVCoord [nI].right  = 2 + szCoord.cx ;
            }
        }

    // get top and bottom starting value
    nT = 0 ;

    if (User.bShowCoord)
        {
        nB = Game [nG].rBuffer1.bottom + 1 + szCoord.cy ;
        }
    else
        {
        nB = Game [nG].rBuffer1.bottom ;
        }

    nB = nB + 2 ;

    // assign handle rect
    Game [nG].rHandle [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rHandle [INDEX_WHITE].top    = nT ;
    Game [nG].rHandle [INDEX_WHITE].right  = Game [nG].rHandle [INDEX_WHITE].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_WHITE].bottom = Game [nG].rHandle [INDEX_WHITE].top  + szHandle.cy ;

    if (User.bShowLagStat)
        {
        nC = Game [nG].rBoard.right - szClock.cx - 1 - szLag.cx ;
        nX = nC ;
        }
    else
        {
        nC = Game [nG].rBoard.right - szClock.cx ;
        nX = Game [nG].rBoard.right - (szHandle.cx >> 1) ;
        }

    Game [nG].rHandle [INDEX_BLACK].left   = nX ;
    Game [nG].rHandle [INDEX_BLACK].top    = nT ;
    Game [nG].rHandle [INDEX_BLACK].right  = Game [nG].rHandle [INDEX_BLACK].left + szHandle.cx ;
    Game [nG].rHandle [INDEX_BLACK].bottom = Game [nG].rHandle [INDEX_BLACK].top  + szHandle.cy ;

    nT = nT + (szHandle.cy + 2) ;

    // assign clock rect
    Game [nG].rClock [INDEX_WHITE].left   = Game [nG].rBoard.left + 1 ;
    Game [nG].rClock [INDEX_WHITE].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_WHITE].right  = Game [nG].rClock [INDEX_WHITE].left + szClock.cx ;
    Game [nG].rClock [INDEX_WHITE].bottom = Game [nG].rClock [INDEX_WHITE].top  + szClock.cy ;

    Game [nG].rClock [INDEX_BLACK].left   = nC ;
    Game [nG].rClock [INDEX_BLACK].top    = (nT + nH) - szClock.cy ;
    Game [nG].rClock [INDEX_BLACK].right  = Game [nG].rClock [INDEX_BLACK].left + szClock.cx ;
    Game [nG].rClock [INDEX_BLACK].bottom = Game [nG].rClock [INDEX_BLACK].top  + szClock.cy ;

    // assign lag status
    if (User.bShowLagStat)
        {
        Game [nG].rLag [INDEX_WHITE].left   = Game [nG].rClock [INDEX_WHITE].right + 1 ;
        Game [nG].rLag [INDEX_WHITE].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_WHITE].right  = Game [nG].rLag [INDEX_WHITE].left + szLag.cx ;
        Game [nG].rLag [INDEX_WHITE].bottom = Game [nG].rLag [INDEX_WHITE].top  + szLag.cy ;

        Game [nG].rLag [INDEX_BLACK].left   = Game [nG].rBoard.right - szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].top    = (nT + nH) - szLag.cy ;
        Game [nG].rLag [INDEX_BLACK].right  = Game [nG].rLag [INDEX_BLACK].left + szLag.cx ;
        Game [nG].rLag [INDEX_BLACK].bottom = Game [nG].rLag [INDEX_BLACK].top  + szLag.cy ;
        }

    nT = nT + (nH + 2) ;

    nButtonY = nB ;

    if (User.bShowLastMove)
        {
        if (User.bShowGameType)
            {
            Game [nG].rGameType.top    = nB ;
            Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
            Game [nG].rGameType.left   = Game [nG].rBoard.left ;
            Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

            if (Game [nG].rGameType.right > Game [nG].rBoard.right)
                {
                Game [nG].rGameType.right = Game [nG].rBoard.right ;
                }
            }

        Game [nG].rLastMove.top    = nB ;
        Game [nG].rLastMove.bottom = Game [nG].rLastMove.top + szLastMove.cy ;

        nI = ((Game [nG].rBoard.right - Game [nG].rBoard.left) + 1) >> 1 ;
        Game [nG].rLastMove.left   = (Game [nG].rBoard.left + nI) - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx - szLMX.cx ;
        Game [nG].rLastMove.right  = Game [nG].rLastMove.left + szLastMove.cx ;

        if (Game [nG].rLastMove.right > Game [nG].rBoard.right)
            {
            Game [nG].rLastMove.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (User.bShowGameType)
        {
        Game [nG].rGameType.top    = nB ;
        Game [nG].rGameType.bottom = Game [nG].rGameType.top + szGameType.cy ;
        Game [nG].rGameType.left   = Game [nG].rBoard.left ;
        Game [nG].rGameType.right  = Game [nG].rGameType.left + szGameType.cx ;

        if (Game [nG].rGameType.right > Game [nG].rBoard.right)
            {
            Game [nG].rGameType.right = Game [nG].rBoard.right ;
            }

        nB = nB + nU ;
        }
    else
    if (bShowB)
        {
        nB = nB + nU ;
        }

    if (User.bShowResult || User.bShowPtell)
        {
        nB = nB + 2 ;

        Game [nG].rResult.top    = nB ;
        Game [nG].rResult.bottom = Game [nG].rResult.top + szResult.cy ;
        Game [nG].rResult.left   = 0 ;
        Game [nG].rResult.right  = nXC - Game [nG].rResult.left ;

        nB = nB + szResult.cy ;
        }

    // assign actual board size
    Game [nG].rActual.top    = 0 ;
    Game [nG].rActual.left   = 0 ;
    Game [nG].rActual.right  = Game [nG].rBoard.right + 1 ;
    Game [nG].rActual.bottom = nB + 1 ;

    // assign buffer pieces starting x and y coordinate within buffer
    nB = Game [nG].rBuffer1.top - Game [nG].rBuffer.top ;

    nI = 1 + nss + nss + nss ;

    if (Game [nG].bFlip)
        {
        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = nI ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 ;

        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = nI ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 + nB ;
        }
    else
        {
        // assign black pieces
        Game [nG].ptBuffer [BLACK_PAWN].x   = nI ;
        Game [nG].ptBuffer [BLACK_PAWN].y   = 1 ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = 1 ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = 1 ;

        Game [nG].ptBuffer [BLACK_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_ROOK].y   = 1 ;

        Game [nG].ptBuffer [BLACK_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [BLACK_QUEEN].y  = 1 ;

        // assign white pieces
        Game [nG].ptBuffer [WHITE_PAWN].x   = nI ;
        Game [nG].ptBuffer [WHITE_PAWN].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = nI + nss ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = nI + nss + nss ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = 1 + nB ;

        Game [nG].ptBuffer [WHITE_ROOK].x   = nI + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_ROOK].y   = 1 + nB ;

        Game [nG].ptBuffer [WHITE_QUEEN].x  = nI + nss + nss + nss + nss ;
        Game [nG].ptBuffer [WHITE_QUEEN].y  = 1 + nB ;
        }

    // assign buffer piece number coordinates
    for (nI = 0 ; nI < 3 ; nI++)
        {
        Game [nG].ptNumber [nI].x = 1 + ((_ptNumber [nI].x * nss) / ORG_SIZE) ;
        Game [nG].ptNumber [nI].y = (_ptNumber [nI].y * nss) / ORG_SIZE ;
        }

    // adjust buttons
    if (bShowB)
        {
        if (User.bShowLastMove || User.bShowGameType)
            {
            if (User.bShowLagStat)
                {
                nI = Game [nG].rBoard.right - szLag.cx - szLag.cx ;

                if (nI < Game [nG].rClock [INDEX_BLACK].left)
                    {
                    nI = Game [nG].rClock [INDEX_BLACK].left ;
                    }
                }
            else
                {
                nI = Game [nG].rClock [INDEX_BLACK].left ;
                }

            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - nI) >> 2 ;
            nH = nU ;

            if (User.bShowLastMove)
                {
                if (Game [nG].rLastMove.right > (nI - 1))
                    {
                    Game [nG].rLastMove.right = (nI - 1) ;
                    }
                }

            if (User.bShowGameType)
                {
                if (Game [nG].rGameType.right > (nI - 1))
                    {
                    Game [nG].rGameType.right = (nI - 1) ;
                    }
                }
            }
        else
            {
            nI = Game [nG].rBoard.left + 1 ;
            nT = nButtonY ;
            nB = (Game [nG].rBoard.right - Game [nG].rBoard.left) >> 2 ;
            nH = nU ;
            }

        MoveWindow (Game [nG].hwnd1, nI,                nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd2, nI + nB,           nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd3, nI + nB + nB,      nT, nB, nH, FALSE) ;
        MoveWindow (Game [nG].hwnd4, nI + nB + nB + nB, nT, nB, nH, FALSE) ;
        }
    }

void BOARD_CheckFlip (int nG)
    {
    RECT rc ;
    int  bF, nX, nY ;

    if (Game [nG].bFlip)
        {
        bF = (Game [nG].ptBuffer [WHITE_PAWN].y > Game [nG].ptBuffer [BLACK_PAWN].y) ;
        }
    else
        {
        bF = (Game [nG].ptBuffer [BLACK_PAWN].y > Game [nG].ptBuffer [WHITE_PAWN].y) ;
        }

    if (bF)
        {
        // switch clock
        if (! User.bShowClockOnTop)
            {
            CopyRect (&rc, &Game [nG].rClock [INDEX_WHITE]) ;
            CopyRect (&Game [nG].rClock [INDEX_WHITE], &Game [nG].rClock [INDEX_BLACK]) ;
            CopyRect (&Game [nG].rClock [INDEX_BLACK], &rc) ;

            // switch lag stat
            CopyRect (&rc, &Game [nG].rLag [INDEX_WHITE]) ;
            CopyRect (&Game [nG].rLag [INDEX_WHITE], &Game [nG].rLag [INDEX_BLACK]) ;
            CopyRect (&Game [nG].rLag [INDEX_BLACK], &rc) ;

            // switch handle
            CopyRect (&rc, &Game [nG].rHandle [INDEX_WHITE]) ;
            CopyRect (&Game [nG].rHandle [INDEX_WHITE], &Game [nG].rHandle [INDEX_BLACK]) ;
            CopyRect (&Game [nG].rHandle [INDEX_BLACK], &rc) ;
            }

        // switch pawn
        nX = Game [nG].ptBuffer [WHITE_PAWN].x ;
        nY = Game [nG].ptBuffer [WHITE_PAWN].y ;

        Game [nG].ptBuffer [WHITE_PAWN].x = Game [nG].ptBuffer [BLACK_PAWN].x ;
        Game [nG].ptBuffer [WHITE_PAWN].y = Game [nG].ptBuffer [BLACK_PAWN].y ;

        Game [nG].ptBuffer [BLACK_PAWN].x = nX ;
        Game [nG].ptBuffer [BLACK_PAWN].y = nY ;

        // switch knight
        nX = Game [nG].ptBuffer [WHITE_KNIGHT].x ;
        nY = Game [nG].ptBuffer [WHITE_KNIGHT].y ;

        Game [nG].ptBuffer [WHITE_KNIGHT].x = Game [nG].ptBuffer [BLACK_KNIGHT].x ;
        Game [nG].ptBuffer [WHITE_KNIGHT].y = Game [nG].ptBuffer [BLACK_KNIGHT].y ;

        Game [nG].ptBuffer [BLACK_KNIGHT].x = nX ;
        Game [nG].ptBuffer [BLACK_KNIGHT].y = nY ;

        // switch bishop
        nX = Game [nG].ptBuffer [WHITE_BISHOP].x ;
        nY = Game [nG].ptBuffer [WHITE_BISHOP].y ;

        Game [nG].ptBuffer [WHITE_BISHOP].x = Game [nG].ptBuffer [BLACK_BISHOP].x ;
        Game [nG].ptBuffer [WHITE_BISHOP].y = Game [nG].ptBuffer [BLACK_BISHOP].y ;

        Game [nG].ptBuffer [BLACK_BISHOP].x = nX ;
        Game [nG].ptBuffer [BLACK_BISHOP].y = nY ;

        // switch rook
        nX = Game [nG].ptBuffer [WHITE_ROOK].x ;
        nY = Game [nG].ptBuffer [WHITE_ROOK].y ;

        Game [nG].ptBuffer [WHITE_ROOK].x = Game [nG].ptBuffer [BLACK_ROOK].x ;
        Game [nG].ptBuffer [WHITE_ROOK].y = Game [nG].ptBuffer [BLACK_ROOK].y ;

        Game [nG].ptBuffer [BLACK_ROOK].x = nX ;
        Game [nG].ptBuffer [BLACK_ROOK].y = nY ;

        // switch queen
        nX = Game [nG].ptBuffer [WHITE_QUEEN].x ;
        nY = Game [nG].ptBuffer [WHITE_QUEEN].y ;

        Game [nG].ptBuffer [WHITE_QUEEN].x = Game [nG].ptBuffer [BLACK_QUEEN].x ;
        Game [nG].ptBuffer [WHITE_QUEEN].y = Game [nG].ptBuffer [BLACK_QUEEN].y ;

        Game [nG].ptBuffer [BLACK_QUEEN].x = nX ;
        Game [nG].ptBuffer [BLACK_QUEEN].y = nY ;
        }
    }

void BOARD_LoadBitmaps (int nG, HDC hdc)
    {
    HDC hdcMem, hdcMem1 ;
    HBITMAP htOld, htOld1 ;
    int nI, nS ;
    RECT rc ;

    // get window size
    GetClientRect (Game [nG].hwnd, &rc) ;

    // destroy bitmaps
    BOARD_Destroy (nG) ;

    // create save background bitmap (drag info)
    Game [nG].hSave = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;

    // create temporary bitmap
    Game [nG].hTemp = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;

    if (User.bVectorSquare || User.bVectorPiece)
        {
        DRAW_SetSize (Game [nG].nss) ;
        }

    if (User.bVectorSquare)
        {
        // create memory hdc
        hdcMem = CreateCompatibleDC (hdc) ;
        SetPolyFillMode (hdcMem, WINDING) ;

        // white square
        Game [nG].hSquare [INDEX_WHITE] = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;
        htOld = (HBITMAP) SelectObject (hdcMem, Game [nG].hSquare [INDEX_WHITE]) ;
        DRAW_SquareW (hdcMem) ;

        // black square
        Game [nG].hSquare [INDEX_BLACK] = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;
        SelectObject (hdcMem, Game [nG].hSquare [INDEX_BLACK]) ;
        DRAW_SquareB (hdcMem) ;

        // buffer square
        Game [nG].hSquare [INDEX_BUFFER] = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;
        SelectObject (hdcMem, Game [nG].hSquare [INDEX_BUFFER]) ;
        DRAW_SquareBF (hdcMem) ;

        // done
        SelectObject (hdcMem, htOld) ;
        DeleteDC (hdcMem) ;
        }
    else
        {
        hdcMem  = CreateCompatibleDC (hdc) ;
        hdcMem1 = CreateCompatibleDC (hdc) ;

        for (nI = 0 ; nI < MAX_SQUARE ; nI++)
            {
            Game [nG].hSquare [nI] = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;
            if (nI == 0)
                {
                htOld  = (HBITMAP) SelectObject (hdcMem,  Game [nG].hSquare [nI]) ;
                htOld1 = (HBITMAP) SelectObject (hdcMem1, CSet.hSquare [nI]) ;
                nS     = SetStretchBltMode (hdcMem, COLORONCOLOR) ;
                }
            else
                {
                SelectObject (hdcMem,  Game [nG].hSquare [nI]) ;
                SelectObject (hdcMem1, CSet.hSquare [nI]) ;
                SetStretchBltMode (hdcMem, COLORONCOLOR) ;
                }
            StretchBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdcMem1, 0, 0, CSet.ptSquare [nI].x, CSet.ptSquare [nI].y, SRCCOPY) ;
            }

        SetStretchBltMode (hdcMem, nS) ;

        SelectObject (hdcMem,  htOld ) ;
        SelectObject (hdcMem1, htOld1) ;

        DeleteDC (hdcMem ) ;
        DeleteDC (hdcMem1) ;
        }

    if (User.bVectorPiece)
        {
        // create memory hdc
        hdcMem = CreateCompatibleDC (hdc) ;
        SetPolyFillMode (hdcMem, WINDING) ;

        // pieces
        for (nI = 0 ; nI < MAX_PIECE ; nI++)
            {
            Game [nG].hPiece [nI] = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;
            if (nI == 0)
                {
                htOld = (HBITMAP) SelectObject (hdcMem, Game [nG].hPiece [nI]) ;
                }
            else
                {
                SelectObject (hdcMem, Game [nG].hPiece [nI]) ;
                }
            DRAW_VectorPiece (hdcMem, nI, CSet.clrPiece [nI], Game [nG].nss) ;
            }

        // done
        SelectObject (hdcMem, htOld) ;
        DeleteDC (hdcMem) ;
        }
    else
        {
        hdcMem  = CreateCompatibleDC (hdc) ;
        hdcMem1 = CreateCompatibleDC (hdc) ;

        for (nI = 0 ; nI < MAX_PIECE ; nI++)
            {
            Game [nG].hPiece [nI] = CreateCompatibleBitmap (hdc, Game [nG].nss, Game [nG].nss) ;
            if (nI == 0)
                {
                htOld  = (HBITMAP) SelectObject (hdcMem,  Game [nG].hPiece [nI]) ;
                htOld1 = (HBITMAP) SelectObject (hdcMem1, CSet.hPiece [nI]) ;
                nS     = SetStretchBltMode (hdcMem, COLORONCOLOR) ;
                }
            else
                {
                SelectObject (hdcMem,  Game [nG].hPiece [nI]) ;
                SelectObject (hdcMem1, CSet.hPiece [nI]) ;
                SetStretchBltMode (hdcMem, COLORONCOLOR) ;
                }
            StretchBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdcMem1, 0, 0, CSet.ptPiece [nI].x, CSet.ptPiece [nI].y, SRCCOPY) ;
            }

        SetStretchBltMode (hdcMem, nS) ;

        SelectObject (hdcMem,  htOld ) ;
        SelectObject (hdcMem1, htOld1) ;

        DeleteDC (hdcMem ) ;
        DeleteDC (hdcMem1) ;
        }
    }

void BOARD_Destroy (int nG)
    {
    int nI ;

    // delete window bitmap
    if (Game [nG].hWindow)
        {
        DeleteObject (Game [nG].hWindow) ;
        Game [nG].hWindow = NULL ;
        }

    // delete save background bitmap (drag info)
    if (Game [nG].hSave)
        {
        DeleteObject (Game [nG].hSave) ;
        Game [nG].hSave = NULL ;
        }

    // delete temporary bitmap
    if (Game [nG].hTemp)
        {
        DeleteObject (Game [nG].hTemp) ;
        Game [nG].hTemp = NULL ;
        }

    // delete square bitmap
    for (nI = 0 ; nI < MAX_SQUARE ; nI++)
        {
        if (Game [nG].hSquare [nI])
            {
            DeleteObject (Game [nG].hSquare [nI]) ;
            Game [nG].hSquare [nI] = NULL ;
            }
        }

    // delete piece bitmap
    for (nI = 0 ; nI < MAX_PIECE ; nI++)
        {
        if (Game [nG].hPiece [nI])
            {
            DeleteObject (Game [nG].hPiece [nI]) ;
            Game [nG].hPiece [nI] = NULL ;
            }
        }
    }

void BOARD_DrawRepaint (int nG, HWND hwnd, HDC hdc)
    {
    RECT rc ;
    HDC hdcWindow ;
    HBITMAP htOld ;

    if (! IsIconic (hwnd))
        {
        GetClientRect (hwnd, &rc) ;

        if (Game [nG].hWindow == NULL)
            {
            Game [nG].hWindow = CreateCompatibleBitmap (hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
            }

        hdcWindow = CreateCompatibleDC (hdc) ;
        htOld     = (HBITMAP) SelectObject (hdcWindow, Game [nG].hWindow) ;

        BOARD_DrawWindowBackground (hwnd, hdcWindow) ;

        if (TOOLBOX_DisplayActualBoard (nG))
            {
            BOARD_DrawFullPosition (nG, hwnd, hdcWindow) ;
            BOARD_DrawHandle       (nG, hdcWindow) ;
            BOARD_DrawWhiteClock   (nG, hdcWindow) ;
            BOARD_DrawBlackClock   (nG, hdcWindow) ;
            BOARD_DrawWhiteLag     (nG, hdcWindow) ;
            BOARD_DrawBlackLag     (nG, hdcWindow) ;
            BOARD_DrawGameType     (nG, hdcWindow) ;
            BOARD_DrawLastMove     (nG, hdcWindow) ;
            BOARD_DrawResult       (nG, hdcWindow) ;
            BOARD_DrawCoordinates  (nG, hdcWindow) ;
            }
        else
            {
            BOARD_DrawFullPosition1 (nG, hwnd, hdcWindow) ;
            BOARD_DrawHandle        (nG, hdcWindow) ;
            BOARD_DrawWhiteClock    (nG, hdcWindow) ;
            BOARD_DrawBlackClock    (nG, hdcWindow) ;
            BOARD_DrawWhiteLag      (nG, hdcWindow) ;
            BOARD_DrawBlackLag      (nG, hdcWindow) ;
            BOARD_DrawGameType      (nG, hdcWindow) ;
            BOARD_DrawLastMove1     (nG, hdcWindow) ;
            BOARD_DrawResult        (nG, hdcWindow) ;
            BOARD_DrawCoordinates   (nG, hdcWindow) ;
            }

        BitBlt (hdc, 0, 0, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1, hdcWindow, 0, 0, SRCCOPY) ;

        SelectObject (hdcWindow, htOld) ;
        DeleteObject (hdcWindow) ;

        if (nG == INDEX_PLAY)
            {
            if (User.nMoveType == DRAG_MOVE)
                {
                if (DragInfo.nPc != EMPTY_SQUARE)
                    {
                    if (DragInfo.nIndex == nG)
                        {
                        BOARD_DrawBoard (nG, hwnd, hdc, DRAW_STATE_NONE) ;
                        }
                    }
                }
            }
        else
            {
            if (Game [nG].nGameNumber == 0)
                {
                }
            else
                {
                if (Game [nG].nGameNumber == Game [INDEX_PLAY].nGamePartner)
                    {
                    if (User.nMoveType == DRAG_MOVE)
                        {
                        if (DragInfo.nPc != EMPTY_SQUARE)
                            {
                            if (DragInfo.nIndex == nG)
                                {
                                BOARD_DrawBoard1 (nG, hwnd, hdc, DRAW_STATE_NONE) ;
                                }
                            }
                        }
                    }
                }
            }

        TOOLBOX_AdjustShowMoveButtons (nG) ;
        }
    }

void BOARD_DrawWindowBackground (HWND hwnd, HDC hdc)
    {
    RECT rc ;
    HPEN hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    GetClientRect (hwnd, &rc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrColor [CLR_WINDOW_COLOR]) ;
    hbTmp = CreateSolidBrush (clrColor [CLR_WINDOW_COLOR]) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Rectangle (hdc, rc.left, rc.top, rc.right, rc.bottom) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BOARD_Draw0 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y) ;
    }

void BOARD_Draw1 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [1].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [1].x, Game [nG].ptNumber [2].y) ;
    }

void BOARD_Draw2 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [1].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [1].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    }

void BOARD_Draw3 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [2].y) ;

    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [1].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [1].y) ;
    }

void BOARD_Draw4 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [1].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [1].y) ;

    MoveToEx (hdc, Game [nG].ptNumber [1].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [1].x, Game [nG].ptNumber [2].y) ;
    }

void BOARD_Draw5 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [1].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [1].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [2].y) ;
    }

void BOARD_Draw6 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [1].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [1].y) ;
    }

void BOARD_Draw7 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    }

void BOARD_Draw8 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [2].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y) ;

    MoveToEx (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [1].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [1].y) ;
    }

void BOARD_Draw9 (int nG, HDC hdc)
    {
    MoveToEx (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [1].y, NULL) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [1].y) ;
    LineTo (hdc, Game [nG].ptNumber [0].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [0].y) ;
    LineTo (hdc, Game [nG].ptNumber [2].x, Game [nG].ptNumber [2].y) ;
    }

void BOARD_Draw11 (int nG, HDC hdc)
    {
    MoveToEx (hdc, 1, Game [nG].ptNumber [0].y, NULL) ;
    LineTo (hdc, 1, Game [nG].ptNumber [2].y) ;
    }

void BOARD_DrawNumber (int nG, HDC hdc, int nN)
    {
    HPEN hpOld ;

    if (nN > 1)
        {
        hpOld = (HPEN) SelectObject (hdc, hpPen [PEN_PIECE_NUMBER_OUTLINE]) ;
        if (nN > 9)
            {
            BOARD_Draw11 (nG, hdc) ;
            nN = nN - 10 ;
            }
        switch (nN)
            {
            case 0 : BOARD_Draw0 (nG, hdc) ; break ;
            case 1 : BOARD_Draw1 (nG, hdc) ; break ;
            case 2 : BOARD_Draw2 (nG, hdc) ; break ;
            case 3 : BOARD_Draw3 (nG, hdc) ; break ;
            case 4 : BOARD_Draw4 (nG, hdc) ; break ;
            case 5 : BOARD_Draw5 (nG, hdc) ; break ;
            case 6 : BOARD_Draw6 (nG, hdc) ; break ;
            case 7 : BOARD_Draw7 (nG, hdc) ; break ;
            case 8 : BOARD_Draw8 (nG, hdc) ; break ;
            case 9 : BOARD_Draw9 (nG, hdc) ; break ;
            }
        SelectObject (hdc, hpOld) ;
        }
    }

void BOARD_DrawHandle (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     cTmp [50] ;

    hfOld = (HFONT)    SelectObject (hdc, hfFont [FONT_HANDLE].hfFont) ;
    clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_HANDLE]) ;
    clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;

    if (User.bShowRating)
        {
        if (strlen (Game [nG].cRating [INDEX_WHITE]) == 0)
            {
            strcpy (cTmp, Game [nG].cHandle [INDEX_WHITE]) ;
            }
        else
            {
            sprintf (cTmp, "%s %s",
                           Game [nG].cHandle [INDEX_WHITE],
                           Game [nG].cRating [INDEX_WHITE]) ;
            }

        ExtTextOut(hdc,
                   Game [nG].rHandle [INDEX_WHITE].left,
                   Game [nG].rHandle [INDEX_WHITE].top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rHandle [INDEX_WHITE],
                   cTmp,
                   strlen (cTmp),
                   NULL) ;

        if (strlen (Game [nG].cRating [INDEX_BLACK]) == 0)
            {
            strcpy (cTmp, Game [nG].cHandle [INDEX_BLACK]) ;
            }
        else
            {
            sprintf (cTmp, "%s %s",
                           Game [nG].cHandle [INDEX_BLACK],
                           Game [nG].cRating [INDEX_BLACK]) ;
            }

        ExtTextOut(hdc,
                   Game [nG].rHandle [INDEX_BLACK].left,
                   Game [nG].rHandle [INDEX_BLACK].top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rHandle [INDEX_BLACK],
                   cTmp,
                   strlen (cTmp),
                   NULL) ;
        }
    else
        {
        ExtTextOut(hdc,
                   Game [nG].rHandle [INDEX_WHITE].left,
                   Game [nG].rHandle [INDEX_WHITE].top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rHandle [INDEX_WHITE],
                   Game [nG].cHandle [INDEX_WHITE],
                   strlen (Game [nG].cHandle [INDEX_WHITE]),
                   NULL) ;

        ExtTextOut(hdc,
                   Game [nG].rHandle [INDEX_BLACK].left,
                   Game [nG].rHandle [INDEX_BLACK].top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rHandle [INDEX_BLACK],
                   Game [nG].cHandle [INDEX_BLACK],
                   strlen (Game [nG].cHandle [INDEX_BLACK]),
                   NULL) ;
        }

    SetBkColor   (hdc, clrOB) ;
    SetTextColor (hdc, clrOT) ;
    SelectObject (hdc, hfOld) ;
    }

void BOARD_DrawWhiteClock (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     Time [100] ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    if (User.bShowClockOnTop)
        {
        sprintf (Time, "W:%s", CLOCK_TimeString (Game [nG].nTimeRemaining [INDEX_WHITE])) ;
        }
    else
        {
        sprintf (Time, "%s", CLOCK_TimeString (Game [nG].nTimeRemaining [INDEX_WHITE])) ;
        }
    if (Game [nG].bWhitesMove)
        {
        if (Game [nG].bPlaying && Game [nG].nTimeRemaining [INDEX_WHITE] <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_ON]) ;
            }
        clrOB = SetBkColor (hdc, clrColor [BOARD_DetermineColorUpDown(nG,INDEX_WHITE,Game [nG].nTimeRemaining [INDEX_WHITE] )]) ;
        }
    else
        {
        if (Game [nG].bPlaying && Game [nG].nTimeRemaining [INDEX_WHITE] <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_OFF]) ;
            }
        clrOB = SetBkColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
        }

    ExtTextOut(hdc,
               Game [nG].rClock [INDEX_WHITE].left,
               Game [nG].rClock [INDEX_WHITE].top,
               ETO_CLIPPED | ETO_OPAQUE,
               &Game [nG].rClock [INDEX_WHITE], Time, strlen (Time),
               NULL) ;

    SetTextColor (hdc, clrOT) ;
    SetBkColor   (hdc, clrOB) ;
    SelectObject (hdc, hfOld) ;
    }

void BOARD_DrawBlackClock (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     Time [100] ;

    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    if (User.bShowClockOnTop)
        {
        sprintf (Time, "B:%s", CLOCK_TimeString (Game [nG].nTimeRemaining [INDEX_BLACK])) ;
        }
    else
        {
        sprintf (Time, "%s", CLOCK_TimeString (Game [nG].nTimeRemaining [INDEX_BLACK])) ;
        }

    if (Game [nG].bWhitesMove)
        {
        if (Game [nG].bPlaying && Game [nG].nTimeRemaining [INDEX_BLACK] <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_OFF]) ;
            }
        clrOB = SetBkColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
        }
    else
        {
        if (Game [nG].bPlaying && Game [nG].nTimeRemaining [INDEX_BLACK] <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_ON]) ;
            }
        clrOB = SetBkColor (hdc, clrColor [BOARD_DetermineColorUpDown(nG,INDEX_BLACK,Game [nG].nTimeRemaining [INDEX_BLACK])]) ;
        }

    ExtTextOut(hdc,
               Game [nG].rClock [INDEX_BLACK].left,
               Game [nG].rClock [INDEX_BLACK].top,
               ETO_CLIPPED | ETO_OPAQUE,
               &Game [nG].rClock [INDEX_BLACK], Time, strlen (Time),
               NULL) ;

    SetTextColor (hdc, clrOT) ;
    SetBkColor   (hdc, clrOB) ;
    SelectObject (hdc, hfOld) ;
    }

int BOARD_DetermineColorUpDown (int nGC, int nCIndex, long nCTimeRemaining)
	{
	int		 nPGameC, ReturnColor;

	// new since Thief 1.2: Colored Bughouse Clocks showing Teams up/down

	ReturnColor = CLR_CLOCK_ON_BACKGROUND;
	if ((Game[nGC].nGameType == GAMETYPE_BUGHOUSE) && User.bColoredClocks)
			{
			nPGameC = TOOLBOX_FindGameByNumber (Game [nGC].nGamePartner) ;
			if (nPGameC >= 0)
				{
				if (nCTimeRemaining != Game [nPGameC].nTimeRemaining [nCIndex])
					{
					if (nCTimeRemaining > Game [nPGameC].nTimeRemaining [nCIndex])
						{
						ReturnColor = CLR_CLOCK_TIMEUP;
						}
					else
						{
						ReturnColor = CLR_CLOCK_TIMEDOWN;
						}
					}
				}
			}
	return ReturnColor;
	}

void BOARD_DisplayWhiteClock (int nG, HWND hwnd, long nTimeRemaining)
    {
    HDC      hdc ;
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     Time [100] ;

    hdc   = GetDC (hwnd) ;
    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    if (User.bShowClockOnTop)
        {
        sprintf (Time, "W:%s", CLOCK_TimeString (nTimeRemaining)) ;
        }
    else
        {
        sprintf (Time, "%s", CLOCK_TimeString (nTimeRemaining)) ;
        }

    if (Game [nG].bWhitesMove)
		{
        if (Game [nG].bPlaying && nTimeRemaining <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_ON]) ;
            }
//		clrOB = SetBkColor (hdc, clrColor [CLR_CLOCK_ON_BACKGROUND]) ;
		clrOB = SetBkColor (hdc, clrColor [BOARD_DetermineColorUpDown(nG,INDEX_WHITE,nTimeRemaining)]) ;
		}
    else
        {
        if (Game [nG].bPlaying && nTimeRemaining <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_OFF]) ;
            }
        clrOB = SetBkColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
        }

    ExtTextOut(hdc,
               Game [nG].rClock [INDEX_WHITE].left,
               Game [nG].rClock [INDEX_WHITE].top,
               ETO_CLIPPED | ETO_OPAQUE,
               &Game [nG].rClock [INDEX_WHITE], Time, strlen (Time),
               NULL) ;

    SetTextColor (hdc, clrOT) ;
    SetBkColor   (hdc, clrOB) ;
    SelectObject (hdc, hfOld) ;

    ReleaseDC (hwnd, hdc) ;
    }

void BOARD_DisplayBlackClock (int nG, HWND hwnd, long nTimeRemaining)
    {
    HDC      hdc ;
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     Time [100] ;

    hdc   = GetDC (hwnd) ;
    hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_CLOCK].hfFont) ;

    if (User.bShowClockOnTop)
        {
        sprintf (Time, "B:%s", CLOCK_TimeString (nTimeRemaining)) ;
        }
    else
        {
        sprintf (Time, "%s", CLOCK_TimeString (nTimeRemaining)) ;
        }

    if (Game [nG].bWhitesMove)
        {
        if (Game [nG].bPlaying && nTimeRemaining <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_OFF]) ;
            }
        clrOB = SetBkColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
        }
    else
        {
        if (Game [nG].bPlaying && nTimeRemaining <= RedClock.nLRedClock)
            {
            clrOT = SetTextColor (hdc, clrRed) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_ON]) ;
            }

//		clrOB = SetBkColor (hdc, clrColor [CLR_CLOCK_ON_BACKGROUND]) ;
		clrOB = SetBkColor (hdc, clrColor [BOARD_DetermineColorUpDown(nG,INDEX_BLACK,nTimeRemaining)]) ;
		}
    ExtTextOut(hdc,
               Game [nG].rClock [INDEX_BLACK].left,
               Game [nG].rClock [INDEX_BLACK].top,
               ETO_CLIPPED | ETO_OPAQUE,
               &Game [nG].rClock [INDEX_BLACK], Time, strlen (Time),
               NULL) ;

    SetTextColor (hdc, clrOT) ;
    SetBkColor   (hdc, clrOB) ;
    SelectObject (hdc, hfOld) ;

    ReleaseDC (hwnd, hdc) ;
    }

void BOARD_DrawWhiteLag (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     Time [15] ;
    int      nSec ;

    if (User.bShowLagStat)
        {
        hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;

        nSec = Game [nG].nLag [INDEX_WHITE] / 1000 ;
        sprintf (Time, "Lag:%02d:%02d", nSec / 60, nSec % 60) ;

        if (stricmp (Game [nG].cHandle [INDEX_WHITE], Vars.cWhoAmI) == 0 && Game [nG].bOnLagClock)
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_ON]) ;
            clrOB = SetBkColor   (hdc, clrColor [CLR_CLOCK_ON_BACKGROUND]) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_LAGSTAT]) ;
            clrOB = SetBkColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
            }

        ExtTextOut(hdc,
                   Game [nG].rLag [INDEX_WHITE].left,
                   Game [nG].rLag [INDEX_WHITE].top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rLag [INDEX_WHITE], Time, strlen (Time),
                   NULL) ;

        SetTextColor (hdc, clrOT) ;
        SetBkColor   (hdc, clrOB) ;
        SelectObject (hdc, hfOld) ;
        }
    }

void BOARD_DrawBlackLag (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     Time [15] ;
    int      nSec ;

    if (User.bShowLagStat)
        {
        hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_LAGSTAT].hfFont) ;

        nSec = Game [nG].nLag [INDEX_BLACK] / 1000 ;
        sprintf (Time, "Lag:%02d:%02d", nSec / 60, nSec % 60) ;

        if (stricmp (Game [nG].cHandle [INDEX_BLACK], Vars.cWhoAmI) == 0 && Game [nG].bOnLagClock)
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_CLOCK_ON]) ;
            clrOB = SetBkColor   (hdc, clrColor [CLR_CLOCK_ON_BACKGROUND]) ;
            }
        else
            {
            clrOT = SetTextColor (hdc, clrColor [CLR_LAGSTAT]) ;
            clrOB = SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;
            }

        ExtTextOut(hdc,
                   Game [nG].rLag [INDEX_BLACK].left,
                   Game [nG].rLag [INDEX_BLACK].top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rLag [INDEX_BLACK], Time, strlen (Time),
                   NULL) ;

        SetTextColor (hdc, clrOT) ;
        SetBkColor   (hdc, clrOB) ;
        SelectObject (hdc, hfOld) ;
        }
    }

void BOARD_DrawGameType (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char cTmp [128] ;

    if (User.bShowGameType)
        {
        hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_GAMETYPE].hfFont) ;
        clrOT = SetTextColor (hdc, clrColor [CLR_GAMETYPE]) ;
        clrOB = SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;

        strcpy (cTmp, TOOLBOX_GetGameTypeString (nG)) ;

        ExtTextOut(hdc,
                   Game [nG].rGameType.left,
                   Game [nG].rGameType.top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rGameType, cTmp, strlen (cTmp),
                   NULL) ;

        SetTextColor (hdc, clrOT) ;
        SetBkColor   (hdc, clrOB) ;
        SelectObject (hdc, hfOld) ;
        }
    }

void BOARD_DrawLastMove (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     *cP, cTM [30], cTmp [50] ;

    if (User.bShowLastMove)
        {
        strcpy (cTM, Game [nG].cTime4Move) ;

        if (Login.nLoginType == SERVER_FICS)
            {
            cP = strchr (cTM, '.') ;
            if (cP != NULL)
                {
                if (User.bShowLastMoveTenth)
                    {
                    *cP++ ;
                    *cP++ ;
                    }
                *cP++ = ')' ;
                *cP   = NULL_CHAR ;
                }
            }

        hfOld = (HFONT)    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
        clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_LASTMOVE]) ;
        clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;

        if (Game [nG].bInitialMove)
            {
            sprintf (cTmp,
                     "%d. %s %s",
                     Game [nG].nMoveNumber,
                     Game [nG].cLastMove,
                     cTM) ;
            }
        else
        if (Game [nG].bWhitesMove)
            {
            sprintf (cTmp,
                     "%d... %s %s",
                     Game [nG].nMoveNumber - 1,
                     Game [nG].cLastMove,
                     cTM) ;
            }
        else
            {
            sprintf (cTmp,
                     "%d. %s %s",
                     Game [nG].nMoveNumber,
                     Game [nG].cLastMove,
                     cTM) ;
            }

        ExtTextOut(hdc,
                   Game [nG].rLastMove.left,
                   Game [nG].rLastMove.top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rLastMove,
                   cTmp,
                   strlen (cTmp),
                   NULL) ;

        SetTextColor (hdc, clrOT) ;
        SetBkColor   (hdc, clrOB) ;
        SelectObject (hdc, hfOld) ;
        }
    }

void BOARD_DrawLastMove1 (int nI, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     *cP, cTM [30], cTmp [50] ;
    int      nJ, nC ;

    if (User.bShowLastMove)
        {
        hfOld = (HFONT)    SelectObject (hdc, hfFont [FONT_LASTMOVE].hfFont) ;
        clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_LASTMOVE]) ;
        clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;

        nJ = Game [nI].nCurrentIndex ;
        nC = Game [nI].nCurrentColor ;

        if (nJ == -1)
            {
            strcpy (cTM, Game [nI].cInitTime4Move) ;

            if (Login.nLoginType == SERVER_FICS)
                {
                cP = strchr (cTM, '.') ;
                if (cP != NULL)
                    {
                    if (User.bShowLastMoveTenth)
                        {
                        *cP++ ;
                        *cP++ ;
                        }
                    *cP++ = ')' ;
                    *cP   = NULL_CHAR ;
                    }
                }

            if (Game [nI].bInitInitialMove)
                {
                if (Game [nI].nInitMoveNumber == 0)
                    {
                    sprintf (cTmp,
                             "%d. %s %s",
                             Game [nI].nInitMoveNumber + 1,
                             Game [nI].cInitLastMove,
                             cTM) ;
                    }
                else
                    {
                    sprintf (cTmp,
                             "%d. %s %s",
                             Game [nI].nInitMoveNumber,
                             Game [nI].cInitLastMove,
                             cTM) ;
                    }
                }
            else
            if (Game [nI].bInitWhitesMove)
                {
                sprintf (cTmp,
                         "%d... %s %s",
                         Game [nI].nInitMoveNumber - 1,
                         Game [nI].cInitLastMove,
                         cTM) ;
                }
            else
                {
                sprintf (cTmp,
                         "%d. %s %s",
                         Game [nI].nInitMoveNumber,
                         Game [nI].cInitLastMove,
                         cTM) ;
                }
            }
        else
            {
            strcpy (cTM, Game [nI].Position [nJ].cTime4Move [nC]) ;

            if (Login.nLoginType == SERVER_FICS)
                {
                cP = strchr (cTM, '.') ;
                if (cP != NULL)
                    {
                    if (User.bShowLastMoveTenth)
                        {
                        *cP++ ;
                        *cP++ ;
                        }
                    *cP++ = ')' ;
                    *cP   = NULL_CHAR ;
                    }
                }

            if (nC == 0)
                {
                sprintf (cTmp,
                         "%d. %s %s",
                         nJ + 1,
                         Game [nI].Position [nJ].cLastMove [nC],
                         cTM) ;
                }
            else
                {
                sprintf (cTmp,
                         "%d... %s %s",
                         nJ + 1,
                         Game [nI].Position [nJ].cLastMove [nC],
                         cTM) ;
                }
            }

        ExtTextOut(hdc,
                   Game [nI].rLastMove.left,
                   Game [nI].rLastMove.top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nI].rLastMove,
                   cTmp,
                   strlen (cTmp),
                   NULL) ;

        SetTextColor (hdc, clrOT) ;
        SetBkColor   (hdc, clrOB) ;
        SelectObject (hdc, hfOld) ;
        }
    }

void BOARD_DrawResult (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;

    if (nG == INDEX_PLAY)
        {
        if (User.bShowResult || User.bShowPtell)
            {
            hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;

            if (strlen (Game [nG].cResult) == 0)
                {
                if (User.bShowPtell)
                    {
                    if (strlen (Vars.cPartnerTell) == 0)
                        {
                        clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
                        clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;

                        ExtTextOut(hdc,
                                   Game [nG].rResult.left,
                                   Game [nG].rResult.top,
                                   ETO_CLIPPED | ETO_OPAQUE,
                                   &Game [nG].rResult,
                                   "",
                                   0,
                                   NULL) ;
                        }
                    else
                        {
                        clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_PTELL_FOREGROUND]) ;
                        clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_PTELL_BACKGROUND]) ;

                        ExtTextOut(hdc,
                                   Game [nG].rResult.left,
                                   Game [nG].rResult.top,
                                   ETO_CLIPPED | ETO_OPAQUE,
                                   &Game [nG].rResult,
                                   Vars.cPartnerTell,
                                   strlen (Vars.cPartnerTell),
                                   NULL) ;
                        }
                    }
                else
                    {
                    clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
                    clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;

                    ExtTextOut(hdc,
                               Game [nG].rResult.left,
                               Game [nG].rResult.top,
                               ETO_CLIPPED | ETO_OPAQUE,
                               &Game [nG].rResult,
                               "",
                               0,
                               NULL) ;
                    }
                }
            else
                {
                clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_RESULT_FOREGROUND]) ;
                clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_RESULT_BACKGROUND]) ;

                ExtTextOut(hdc,
                           Game [nG].rResult.left,
                           Game [nG].rResult.top,
                           ETO_CLIPPED | ETO_OPAQUE,
                           &Game [nG].rResult,
                           Game [nG].cResult,
                           strlen (Game [nG].cResult),
                           NULL) ;
                }

            SetTextColor (hdc, clrOT) ;
            SetBkColor   (hdc, clrOB) ;
            SelectObject (hdc, hfOld) ;
            }
        }
    else
        {
        if (User.bShowResult)
            {
            hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_RESULT].hfFont) ;

            if (strlen (Game [nG].cResult) == 0)
                {
                clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_WINDOW_COLOR]) ;
                clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;
                }
            else
                {
                clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_RESULT_FOREGROUND]) ;
                clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_RESULT_BACKGROUND]) ;
                }

            ExtTextOut(hdc,
                       Game [nG].rResult.left,
                       Game [nG].rResult.top,
                       ETO_CLIPPED | ETO_OPAQUE,
                       &Game [nG].rResult,
                       Game [nG].cResult,
                       strlen (Game [nG].cResult),
                       NULL) ;

            SetTextColor (hdc, clrOT) ;
            SetBkColor   (hdc, clrOB) ;
            SelectObject (hdc, hfOld) ;
            }
        }
    }

void BOARD_DrawPtell (int nG, HDC hdc, char *cS)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    char     *cP ;
    int      nS ;

    cP = strchr (cS, ':') ;

    if (cP != NULL)
        {
        hfOld = (HFONT)    SelectObject (hdc, hfFont   [FONT_RESULT].hfFont) ;
        clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_PTELL_FOREGROUND]) ;
        clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_PTELL_BACKGROUND]) ;

        strcpy (Vars.cPartnerTell, cP + 2) ;

        nS = strlen (Vars.cPartnerTell) ;

        if (Vars.cPartnerTell [nS - 1] == '\n')
            {
            nS = nS - 1 ;
            Vars.cPartnerTell [nS] = NULL_CHAR ;

            ExtTextOut(hdc,
                       Game [nG].rResult.left,
                       Game [nG].rResult.top,
                       ETO_CLIPPED | ETO_OPAQUE,
                       &Game [nG].rResult,
                       Vars.cPartnerTell,
                       nS,
                       NULL) ;
            }
        else
            {
            ExtTextOut(hdc,
                       Game [nG].rResult.left,
                       Game [nG].rResult.top,
                       ETO_CLIPPED | ETO_OPAQUE,
                       &Game [nG].rResult,
                       Vars.cPartnerTell,
                       nS,
                       NULL) ;
            }

        strcpy (Game [INDEX_PLAY].cResult, "") ;

        SetTextColor (hdc, clrOT) ;
        SetBkColor   (hdc, clrOB) ;
        SelectObject (hdc, hfOld) ;
        }
    }

void BOARD_DrawGinfo (int nG, HDC hdc, char *cS)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;
    int      nS ;

    hfOld = (HFONT)    SelectObject (hdc, hfFont   [FONT_RESULT].hfFont) ;
    clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_GINFO_FOREGROUND]) ;
    clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_GINFO_BACKGROUND]) ;

    nS = strlen (cS) ;

    if (cS [nS - 1] == '\n')
        {
        nS = nS - 1 ;
        cS [nS] = NULL_CHAR ;

        ExtTextOut(hdc,
                   Game [nG].rResult.left,
                   Game [nG].rResult.top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rResult,
                   cS,
                   nS,
                   NULL) ;
        }
    else
        {
        ExtTextOut(hdc,
                   Game [nG].rResult.left,
                   Game [nG].rResult.top,
                   ETO_CLIPPED | ETO_OPAQUE,
                   &Game [nG].rResult,
                   cS,
                   nS,
                   NULL) ;
        }

    strcpy (Game [INDEX_PLAY].cResult, "") ;

    SetTextColor (hdc, clrOT) ;
    SetBkColor   (hdc, clrOB) ;
    SelectObject (hdc, hfOld) ;
    }

void BOARD_DrawCoordinates (int nG, HDC hdc)
    {
    HFONT    hfOld ;
    COLORREF clrOT, clrOB ;

    if (User.bShowCoord)
        {
        hfOld = (HFONT) SelectObject (hdc, hfFont [FONT_COORDINATES].hfFont) ;

        clrOT = (COLORREF) SetTextColor (hdc, clrColor [CLR_COORDINATES_FOREGROUND]) ;
        clrOB = (COLORREF) SetBkColor   (hdc, clrColor [CLR_WINDOW_COLOR]) ;

        if (Game [nG].bFlip)
            {
            // 87654321
            ExtTextOut(hdc, Game [nG].rVCoord [0].left, Game [nG].rVCoord [0].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [0], "1", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [1].left, Game [nG].rVCoord [1].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [1], "2", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [2].left, Game [nG].rVCoord [2].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [2], "3", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [3].left, Game [nG].rVCoord [3].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [3], "4", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [4].left, Game [nG].rVCoord [4].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [4], "5", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [5].left, Game [nG].rVCoord [5].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [5], "6", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [6].left, Game [nG].rVCoord [6].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [6], "7", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [7].left, Game [nG].rVCoord [7].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [7], "8", 1, NULL) ;

            // abcdefgh
            ExtTextOut(hdc, Game [nG].rHCoord [0].left, Game [nG].rHCoord [0].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [0], "h", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [1].left, Game [nG].rHCoord [1].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [1], "g", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [2].left, Game [nG].rHCoord [2].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [2], "f", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [3].left, Game [nG].rHCoord [3].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [3], "e", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [4].left, Game [nG].rHCoord [4].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [4], "d", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [5].left, Game [nG].rHCoord [5].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [5], "c", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [6].left, Game [nG].rHCoord [6].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [6], "b", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [7].left, Game [nG].rHCoord [7].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [7], "a", 1, NULL) ;
            }
        else
            {
            // 12345678
            ExtTextOut(hdc, Game [nG].rVCoord [0].left, Game [nG].rVCoord [0].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [0], "8", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [1].left, Game [nG].rVCoord [1].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [1], "7", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [2].left, Game [nG].rVCoord [2].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [2], "6", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [3].left, Game [nG].rVCoord [3].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [3], "5", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [4].left, Game [nG].rVCoord [4].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [4], "4", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [5].left, Game [nG].rVCoord [5].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [5], "3", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [6].left, Game [nG].rVCoord [6].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [6], "2", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rVCoord [7].left, Game [nG].rVCoord [7].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rVCoord [7], "1", 1, NULL) ;

            // abcdefgh
            ExtTextOut(hdc, Game [nG].rHCoord [0].left, Game [nG].rHCoord [0].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [0], "a", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [1].left, Game [nG].rHCoord [1].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [1], "b", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [2].left, Game [nG].rHCoord [2].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [2], "c", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [3].left, Game [nG].rHCoord [3].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [3], "d", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [4].left, Game [nG].rHCoord [4].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [4], "e", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [5].left, Game [nG].rHCoord [5].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [5], "f", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [6].left, Game [nG].rHCoord [6].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [6], "g", 1, NULL) ;
            ExtTextOut(hdc, Game [nG].rHCoord [7].left, Game [nG].rHCoord [7].top, ETO_CLIPPED | ETO_OPAQUE, &Game [nG].rHCoord [7], "h", 1, NULL) ;
            }

        SetTextColor (hdc, clrOT) ;
        SetBkColor   (hdc, clrOB) ;
        SelectObject (hdc, hfOld) ;
        }
    }

void BOARD_SquareToPosition (int nG, int nBx, int nBy,
                             int *nPx, int *nPy)
    {
    if (Game [nG].bFlip)
        {
        *nPx = Game [nG].rBoard.left + 1 + (ReverseCoord [nBx] * Game [nG].nss) ;
        *nPy = Game [nG].rBoard.top  + 1 + (nBy * Game [nG].nss) ;
        }
    else
        {
        *nPx = Game [nG].rBoard.left + 1 + (nBx * Game [nG].nss) ;
        *nPy = Game [nG].rBoard.top  + 1 + (ReverseCoord [nBy] * Game [nG].nss) ;
        }
    }

void BOARD_DrawHighlight (int nG, int nBx, int nBy, HDC hdc, int nI)
    {
    HPEN hpOld ;
    int nPx, nPy ;

    BOARD_SquareToPosition (nG, nBx, nBy, &nPx, &nPy) ;

    hpOld = (HPEN) SelectObject (hdc, hpPen [nI]) ;

    MoveToEx (hdc, nPx + 1, nPy + 1, NULL) ;
    LineTo (hdc, nPx + Game [nG].nss - 2, nPy + 1) ;
    LineTo (hdc, nPx + Game [nG].nss - 2, nPy + Game [nG].nss - 2) ;
    LineTo (hdc, nPx + 1, nPy + Game [nG].nss - 2) ;
    LineTo (hdc, nPx + 1, nPy + 1) ;

    MoveToEx (hdc, nPx + 2, nPy + 2, NULL) ;
    LineTo (hdc, nPx + Game [nG].nss - 3, nPy + 2) ;
    LineTo (hdc, nPx + Game [nG].nss - 3, nPy + Game [nG].nss - 3) ;
    LineTo (hdc, nPx + 2, nPy + Game [nG].nss - 3) ;
    LineTo (hdc, nPx + 2, nPy + 2) ;

    SelectObject (hdc, hpOld) ;
    }

void BOARD_DrawBoardPiece (int nG, HDC hdc, int nBx, int nBy, int nP)
    {
    HDC hdcMem, hdcMem1 ;
    HBITMAP htOld, htOld1 ;
    int nPx, nPy ;

    BOARD_SquareToPosition (nG, nBx, nBy, &nPx, &nPy) ;

    hdcMem  = CreateCompatibleDC (hdc) ;
    hdcMem1 = CreateCompatibleDC (hdc) ;

    htOld  = (HBITMAP) SelectObject (hdcMem,  Game [nG].hTemp) ;
    htOld1 = (HBITMAP) SelectObject (hdcMem1, Game [nG].hSquare [ColorBoard [nBx] [nBy]]) ;

    BitBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdcMem1, 0, 0, SRCCOPY) ;

    if (nP != EMPTY_SQUARE)
        {
        DRAW_TransparentBlt (nG, hdcMem, nP) ;
        }

    BitBlt (hdc, nPx, nPy, Game [nG].nss, Game [nG].nss, hdcMem, 0, 0, SRCCOPY) ;

    SelectObject (hdcMem,  htOld ) ;
    SelectObject (hdcMem1, htOld1) ;

    DeleteDC (hdcMem ) ;
    DeleteDC (hdcMem1) ;
    }

void BOARD_DrawBufferPiece (int nG, HDC hdc, int nP, int nN)
    {
    HDC hdcMem, hdcMem1 ;
    HBITMAP htOld, htOld1 ;

    hdcMem  = CreateCompatibleDC (hdc) ;
    hdcMem1 = CreateCompatibleDC (hdc) ;

    htOld  = (HBITMAP) SelectObject (hdcMem,  Game [nG].hTemp) ;
    htOld1 = (HBITMAP) SelectObject (hdcMem1, Game [nG].hSquare [INDEX_BUFFER]) ;

    BitBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdcMem1, 0, 0, SRCCOPY) ;

    if (nN > 0)
        {
        DRAW_TransparentBlt (nG, hdcMem, nP) ;
        BOARD_DrawNumber    (nG, hdcMem, nN) ;
        }

    BitBlt (hdc, Game [nG].rBuffer.left + Game [nG].ptBuffer [nP].x, Game [nG].rBuffer.top + Game [nG].ptBuffer [nP].y, Game [nG].nss, Game [nG].nss, hdcMem, 0, 0, SRCCOPY) ;

    SelectObject (hdcMem,  htOld ) ;
    SelectObject (hdcMem1, htOld1) ;

    DeleteDC (hdcMem ) ;
    DeleteDC (hdcMem1) ;
    }

void BOARD_DrawChessBufferPiece (int nG, HDC hdc, int nP, int nN)
    {
    HDC hdcMem, hdcMem1 ;
    HBITMAP htOld, htOld1 ;

    hdcMem  = CreateCompatibleDC (hdc) ;
    hdcMem1 = CreateCompatibleDC (hdc) ;

    htOld  = (HBITMAP) SelectObject (hdcMem,  Game [nG].hTemp) ;
    htOld1 = (HBITMAP) SelectObject (hdcMem1, Game [nG].hSquare [INDEX_BUFFER]) ;

    BitBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdcMem1, 0, 0, SRCCOPY) ;

    if (nN > 0)
        {
        DRAW_TransparentBlt (nG, hdcMem, nP) ;
        BOARD_DrawNumber    (nG, hdcMem, nN) ;
        }

    nP = ReverseChessPiece [nP] ;

    BitBlt (hdc, Game [nG].rBuffer.left + Game [nG].ptBuffer [nP].x, Game [nG].rBuffer.top + Game [nG].ptBuffer [nP].y, Game [nG].nss, Game [nG].nss, hdcMem, 0, 0, SRCCOPY) ;

    SelectObject (hdcMem,  htOld ) ;
    SelectObject (hdcMem1, htOld1) ;

    DeleteDC (hdcMem ) ;
    DeleteDC (hdcMem1) ;
    }

void BOARD_DrawEmptyBufferPiece (int nG, HDC hdc, int nP)
    {
    HDC hdcMem, hdcMem1 ;
    HBITMAP htOld, htOld1 ;

    hdcMem  = CreateCompatibleDC (hdc) ;
    hdcMem1 = CreateCompatibleDC (hdc) ;

    htOld  = (HBITMAP) SelectObject (hdcMem,  Game [nG].hTemp) ;
    htOld1 = (HBITMAP) SelectObject (hdcMem1, Game [nG].hSquare [INDEX_BUFFER]) ;

    BitBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdcMem1, 0, 0, SRCCOPY) ;

    BitBlt (hdc, Game [nG].rBuffer.left + Game [nG].ptBuffer [nP].x, Game [nG].rBuffer.top + Game [nG].ptBuffer [nP].y, Game [nG].nss, Game [nG].nss, hdcMem, 0, 0, SRCCOPY) ;

    SelectObject (hdcMem,  htOld ) ;
    SelectObject (hdcMem1, htOld1) ;

    DeleteDC (hdcMem ) ;
    DeleteDC (hdcMem1) ;
    }

void BOARD_DrawDragPiece (int nG, HDC hdc)
    {
    HDC hdcMem ;
    HBITMAP htOld ;

    if (DragInfo.nPc != EMPTY_SQUARE)
        {
        // create memory bitmap
        hdcMem = CreateCompatibleDC (hdc) ;
        htOld  = (HBITMAP) SelectObject (hdcMem, Game [nG].hTemp) ;

        // copy background
        BitBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdc, DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, SRCCOPY) ;

        // transparent draw
        DRAW_TransparentBlt (nG, hdcMem, DragInfo.nPc) ;

        // copy result into target
        BitBlt (hdc, DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, Game [nG].nss, Game [nG].nss, hdcMem, 0, 0, SRCCOPY) ;

        // clean up
        SelectObject (hdcMem, htOld) ;
        DeleteDC     (hdcMem) ;
        }
    }

void BOARD_DrawAnimatePiece (int nG, HDC hdc)
    {
    HDC hdcMem ;
    HBITMAP htOld ;

    if (AnimateInfo.nPc != EMPTY_SQUARE)
        {
        // create memory bitmap
        hdcMem = CreateCompatibleDC (hdc) ;
        htOld  = (HBITMAP) SelectObject (hdcMem, Game [nG].hTemp) ;

        // copy background
        BitBlt (hdcMem, 0, 0, Game [nG].nss, Game [nG].nss, hdc, AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, SRCCOPY) ;

        // transparent draw
        DRAW_TransparentBlt (nG, hdcMem, AnimateInfo.nPc) ;

        // copy result into target
        BitBlt (hdc, AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, Game [nG].nss, Game [nG].nss, hdcMem, 0, 0, SRCCOPY) ;

        // clean up
        SelectObject (hdcMem, htOld) ;
        DeleteDC     (hdcMem) ;
        }
    }

void BOARD_DrawFullPosition (int nG, HWND hwnd, HDC hdc)
    {
    HDC hdcWindow, hdcTemp ;
    HBITMAP htWindow, htOldWindow, htOldTemp ;
    HPEN hpOldWindow ;
    HBRUSH hbOldWindow ;
    RECT rc ;

    int nW, nH, nW1, nH1 ;
    int nY, nX, nI, nOT ;
    int bB ;

    // create memory dc
    hdcWindow = CreateCompatibleDC (hdc) ;
    hdcTemp   = CreateCompatibleDC (hdc) ;

    // get client rect
    GetClientRect (hwnd, &rc) ;

    // create memory window bitmap to be drawn to
    htWindow = CreateCompatibleBitmap (hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;

    // assign board and buffer sizes
    nW  = (Game [nG].rBoard.right  - Game [nG].rBoard.left ) + 1 ;
    nH  = (Game [nG].rBoard.bottom - Game [nG].rBoard.top  ) + 1 ;
    nW1 = (Game [nG].rBuffer.right - Game [nG].rBuffer.left) + 1 ;

    // draw board outline
    htOldWindow = (HBITMAP) SelectObject (hdcWindow, htWindow) ;
    hpOldWindow = (HPEN)    SelectObject (hdcWindow, hpPen [PEN_BOARD_OUTLINE]) ;
    Rectangle (hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, Game [nG].rBoard.left + nW, Game [nG].rBoard.top + nH) ;

    // draw buffer outline and fill buffer
    bB = TOOLBOX_ShowBuffer (nG) ;

    if (bB)
        {
        hbOldWindow = (HBRUSH) SelectObject (hdcWindow, hbBrush [BRUSH_BUFFER_FILL]) ;
        SelectObject (hdcWindow, hpPen [PEN_BUFFER_OUTLINE]) ;

        if ((User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOML) ||
            (User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOMR))
            {
            nH1 = (Game [nG].rBuffer.bottom - Game [nG].rBuffer.top) + 1 ;
            Rectangle (hdcWindow, Game [nG].rBuffer.left,  Game [nG].rBuffer.top,  Game [nG].rBuffer.left  + nW1, Game [nG].rBuffer.top  + nH1) ;
            Rectangle (hdcWindow, Game [nG].rBuffer1.left, Game [nG].rBuffer1.top, Game [nG].rBuffer1.left + nW1, Game [nG].rBuffer1.top + nH1) ;
            }
        else
            {
            Rectangle (hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, Game [nG].rBuffer.left + nW1, Game [nG].rBuffer.top + nH) ;
            }

        SelectObject (hdcWindow, hbOldWindow) ;
        }

    // draw board
    for (nY = 0 ; nY < 8 ; nY++)
        {
        for (nX = 0 ; nX < 8 ; nX++)
            {
            BOARD_DrawBoardPiece (nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
            }
        }

    // draw buffer
    if (bB)
        {
        if (Game [nG].bChessGame)
            {
            if (User.nShowCapturedChessPiece == 2)
                {
                if ((Game [nG].nRelation ==  0) ||  //  0 i am observing a game being played
                    (Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
                    (Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
                    {
                    for (nI = 0 ; nI < MAX_BUFFER ; nI++)
                        {
                        BOARD_DrawChessBufferPiece (nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;
                        }
                    }
                else
                    {
                    for (nI = 0 ; nI < MAX_BUFFER ; nI++)
                        {
                        BOARD_DrawEmptyBufferPiece (nG, hdcWindow, nI) ;
                        }
                    }
                }
            else
                {
                for (nI = 0 ; nI < MAX_BUFFER ; nI++)
                    {
                    BOARD_DrawEmptyBufferPiece (nG, hdcWindow, nI) ;
                    }
                }
            }
        else
            {
            for (nI = 0 ; nI < MAX_BUFFER ; nI++)
                {
                BOARD_DrawBufferPiece (nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;
                }
            }
        }

    // draw between buffers
    htOldTemp = (HBITMAP) SelectObject (hdcTemp, Game [nG].hSquare [INDEX_BUFFER]) ;

    if (bB)
        {
        if (User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOML)
            {
            nX = Game [nG].rBuffer.left + Game [nG].ptBuffer [WHITE_QUEEN].x + Game [nG].nss ;
            for (nI = 0 ; nI < 3 ; nI++)
                {
                BitBlt (hdcWindow, nX, Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_QUEEN].y, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, nX, Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_QUEEN].y, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                nX = nX + Game [nG].nss ;
                }
            }
        else
        if (User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOMR)
            {
            nX = Game [nG].rBuffer.left + 1 ;
            for (nI = 0 ; nI < 3 ; nI++)
                {
                BitBlt (hdcWindow, nX, Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_QUEEN].y, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, nX, Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_QUEEN].y, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                nX = nX + Game [nG].nss ;
                }
            }
        else
            {
            if (Game [nG].bFlip)
                {
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [WHITE_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_BISHOP].y,                                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [WHITE_BISHOP].x, Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_BISHOP].y + Game [nG].nss,                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [WHITE_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_BISHOP].y + Game [nG].nss,                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [WHITE_BISHOP].x, Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_BISHOP].y + Game [nG].nss + Game [nG].nss, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [WHITE_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_BISHOP].y + Game [nG].nss + Game [nG].nss, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [BLACK_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_BISHOP].y,                                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                }
            else
                {
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [BLACK_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_BISHOP].y,                                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [BLACK_BISHOP].x, Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_BISHOP].y + Game [nG].nss,                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [BLACK_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_BISHOP].y + Game [nG].nss,                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [BLACK_BISHOP].x, Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_BISHOP].y + Game [nG].nss + Game [nG].nss, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [BLACK_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [BLACK_BISHOP].y + Game [nG].nss + Game [nG].nss, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nG].rBuffer.left + Game [nG].ptBuffer [WHITE_QUEEN].x,  Game [nG].rBuffer.top + Game [nG].ptBuffer [WHITE_BISHOP].y,                                 Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;
                }
            }
        }

    // draw from/to square highlight
    if (User.bShowHighlight)
        {
        if (Game [nG].ptHighlight [0].x >= 0)
            {
            BOARD_DrawHighlight (nG, Game [nG].ptHighlight [0].x, Game [nG].ptHighlight [0].y, hdcWindow, PEN_HIGHLIGHT) ;
            }
        if (Game [nG].ptHighlight [1].x >= 0)
            {
            BOARD_DrawHighlight (nG, Game [nG].ptHighlight [1].x, Game [nG].ptHighlight [1].y, hdcWindow, PEN_HIGHLIGHT) ;
            }
        }

    // draw king legal square highlight
    if (User.bShowKingHighlight)
        {
        for (nI = 0 ; nI < 8 ; nI++)
            {
            if (Game [nG].ptKing [INDEX_WHITE] [nI].x >= 0)
                {
                BOARD_DrawHighlight (nG, Game [nG].ptKing [INDEX_WHITE] [nI].x, Game [nG].ptKing [INDEX_WHITE] [nI].y, hdcWindow, PEN_KING_HIGHLIGHT) ;
                }
            if (Game [nG].ptKing [INDEX_BLACK] [nI].x >= 0)
                {
                BOARD_DrawHighlight (nG, Game [nG].ptKing [INDEX_BLACK] [nI].x, Game [nG].ptKing [INDEX_BLACK] [nI].y, hdcWindow, PEN_KING_HIGHLIGHT) ;
                }
            }
        }

    if (nG == INDEX_PLAY)
        {
        // draw illegal true premove and true premove square highlight
        if (User.bTruePremove)
            {
            // illegal true premove
            if (Premove.bIllegalTP)
                {
                if ((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
                    {
                    BOARD_DrawHighlight (nG, Premove.ptIllegalTP [0].x, Premove.ptIllegalTP [0].y, hdcWindow, PEN_ILLEGAL_TRUEPREMOVE) ;
                    }
                if ((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
                    {
                    BOARD_DrawHighlight (nG, Premove.ptIllegalTP [1].x, Premove.ptIllegalTP [1].y, hdcWindow, PEN_ILLEGAL_TRUEPREMOVE) ;
                    }
                }

            if (User.bShowTPHighlight)
                {
                // true premove
                nOT = Premove.nPremoveTail ;
                for (nI = 0 ; nI < Premove.nPremoveCount ; nI++)
                    {
                    if (Premove.nPremoveLegalBuffer [nOT] [3] >= 0)
                        {
                        BOARD_DrawHighlight (nG, Premove.nPremoveLegalBuffer [nOT] [3], Premove.nPremoveLegalBuffer [nOT] [4], hdcWindow, PEN_TRUEPREMOVE) ;
                        }
                    if (Premove.nPremoveLegalBuffer [nOT] [5] >= 0)
                        {
                        BOARD_DrawHighlight (nG, Premove.nPremoveLegalBuffer [nOT] [5], Premove.nPremoveLegalBuffer [nOT] [6], hdcWindow, PEN_TRUEPREMOVE) ;
                        }

                    nOT = nOT + 1 ;
                    if (nOT >= MAX_TRUE_PREMOVE)
                        {
                        nOT = 0 ;
                        }
                    }
                }
            }
        }

    // clean up
    SelectObject (hdcWindow, htOldWindow) ;
    SelectObject (hdcWindow, hpOldWindow) ;
    DeleteDC     (hdcWindow) ;

    // copy into target
    SelectObject (hdcTemp, htWindow) ;

    if (bB)
        {
        switch (User.nBufferOrientation)
            {
            case DEFAULT_BUFFER_LEFT :
                BitBlt (hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, nH, hdcTemp, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
                break ;

            case DEFAULT_BUFFER_RIGHT :
                BitBlt (hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcTemp, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
                break ;

            case DEFAULT_BUFFER_TOPBOTTOML :
            case DEFAULT_BUFFER_TOPBOTTOMR :
                BitBlt (hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW, (Game [nG].rBuffer1.bottom - Game [nG].rBuffer.top) + 1, hdcTemp, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
                break ;

            default :
                BitBlt (hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcTemp, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
                break ;
            }
        }
    else
        {
        BitBlt (hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW, nH, hdcTemp, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
        }

    SelectObject (hdcTemp, htOldTemp) ;

    // clean up
    DeleteDC (hdcTemp) ;
    DeleteObject (htWindow) ;

    // assign last move highlight
    Game [nG].ptLastHighlight [0] = Game [nG].ptHighlight [0] ;
    Game [nG].ptLastHighlight [1] = Game [nG].ptHighlight [1] ;

    // assign last king legal square highlight
    if (User.bShowKingHighlight)
        {
        for (nI = 0 ; nI < 8 ; nI++)
            {
            Game [nG].ptLastKing [INDEX_WHITE] [nI] = Game [nG].ptKing [INDEX_WHITE] [nI] ;
            Game [nG].ptLastKing [INDEX_BLACK] [nI] = Game [nG].ptKing [INDEX_BLACK] [nI] ;
            }
        }

    // save last board and buffer
    BOARD_SaveLastBoardBuffer (nG) ;

    // display window title
    SetWindowText (hwnd, TOOLBOX_GetGameWindowTitle (nG)) ;
    }

void BOARD_DrawFullPosition1 (int nI, HWND hwnd, HDC hdc)
    {
    HDC hdcWindow, hdcTemp ;
    HBITMAP htWindow, htOldWindow, htOldTemp ;
    HPEN hpOldWindow ;
    HBRUSH hbOldWindow ;
    RECT rc ;

    int nW, nH, nW1, nH1 ;
    int nJ, nC, nY, nX ;
    int bB ;

    // create memory dc
    hdcWindow = CreateCompatibleDC (hdc) ;
    hdcTemp   = CreateCompatibleDC (hdc) ;

    // get client rect
    GetClientRect (hwnd, &rc) ;

    // create memory window bitmap to be drawn to
    htWindow = CreateCompatibleBitmap (hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;

    // assign board and buffer sizes
    nW  = (Game [nI].rBoard.right  - Game [nI].rBoard.left ) + 1 ;
    nH  = (Game [nI].rBoard.bottom - Game [nI].rBoard.top  ) + 1 ;
    nW1 = (Game [nI].rBuffer.right - Game [nI].rBuffer.left) + 1 ;

    // draw board outline
    htOldWindow = (HBITMAP) SelectObject (hdcWindow, htWindow) ;
    hpOldWindow = (HPEN)    SelectObject (hdcWindow, hpPen [PEN_BOARD_OUTLINE]) ;
    Rectangle (hdcWindow, Game [nI].rBoard.left, Game [nI].rBoard.top, Game [nI].rBoard.left + nW, Game [nI].rBoard.top + nH) ;

    // draw buffer outline and fill buffer
    bB = TOOLBOX_ShowBuffer (nI) ;

    if (bB)
        {
        hbOldWindow = (HBRUSH) SelectObject (hdcWindow, hbBrush [BRUSH_BUFFER_FILL]) ;
        SelectObject (hdcWindow, hpPen [PEN_BUFFER_OUTLINE]) ;

        if ((User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOML) ||
            (User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOMR))
            {
            nH1 = (Game [nI].rBuffer.bottom - Game [nI].rBuffer.top) + 1 ;
            Rectangle (hdcWindow, Game [nI].rBuffer.left,  Game [nI].rBuffer.top,  Game [nI].rBuffer.left  + nW1, Game [nI].rBuffer.top  + nH1) ;
            Rectangle (hdcWindow, Game [nI].rBuffer1.left, Game [nI].rBuffer1.top, Game [nI].rBuffer1.left + nW1, Game [nI].rBuffer1.top + nH1) ;
            }
        else
            {
            Rectangle (hdcWindow, Game [nI].rBuffer.left, Game [nI].rBuffer.top, Game [nI].rBuffer.left + nW1, Game [nI].rBuffer.top + nH) ;
            }

        SelectObject (hdcWindow, hbOldWindow) ;
        }

    nJ = Game [nI].nCurrentIndex ;
    nC = Game [nI].nCurrentColor ;

    if (nJ == -1)
        {
        // draw board
        for (nY = 0 ; nY < 8 ; nY++)
            {
            for (nX = 0 ; nX < 8 ; nX++)
                {
                BOARD_DrawBoardPiece (nI, hdcWindow, nX, nY, Game [nI].nInitBoard [nX] [nY]) ;
                }
            }

        // draw buffer
        if (bB)
            {
            if (Game [nI].bChessGame)
                {
                if (User.nShowCapturedChessPiece == 2)
                    {
                    if ((Game [nI].nRelation ==  0) ||  //  0 i am observing a game being played
                        (Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
                        (Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
                        {
                        for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                            {
                            BOARD_DrawChessBufferPiece (nI, hdcWindow, nX, Game [nI].nInitBuffer [nX]) ;
                            }
                        }
                    else
                        {
                        for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                            {
                            BOARD_DrawEmptyBufferPiece (nI, hdcWindow, nX) ;
                            }
                        }
                    }
                else
                    {
                    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                        {
                        BOARD_DrawEmptyBufferPiece (nI, hdcWindow, nX) ;
                        }
                    }
                }
            else
                {
                for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                    {
                    BOARD_DrawBufferPiece (nI, hdcWindow, nX, Game [nI].nInitBuffer [nX]) ;
                    }
                }
            }
        }
    else
        {
        // draw board
        for (nY = 0 ; nY < 8 ; nY++)
            {
            for (nX = 0 ; nX < 8 ; nX++)
                {
                BOARD_DrawBoardPiece (nI, hdcWindow, nX, nY, Game [nI].Position [nJ].nBoard [nC] [nX] [nY]) ;
                }
            }

        // draw buffer
        if (bB)
            {
            if (Game [nI].bChessGame)
                {
                if (User.nShowCapturedChessPiece == 2)
                    {
                    if ((Game [nI].nRelation ==  0) ||  //  0 i am observing a game being played
                        (Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
                        (Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
                        {
                        for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                            {
                            BOARD_DrawChessBufferPiece (nI, hdcWindow, nX, Game [nI].Position [nJ].nBuffer [nC] [nX]) ;
                            }
                        }
                    else
                        {
                        for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                            {
                            BOARD_DrawEmptyBufferPiece (nI, hdcWindow, nX) ;
                            }
                        }
                    }
                else
                    {
                    for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                        {
                        BOARD_DrawEmptyBufferPiece (nI, hdcWindow, nX) ;
                        }
                    }
                }
            else
                {
                for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                    {
                    BOARD_DrawBufferPiece (nI, hdcWindow, nX, Game [nI].Position [nJ].nBuffer [nC] [nX]) ;
                    }
                }
            }
        }

    // draw between buffers
    htOldTemp = (HBITMAP) SelectObject (hdcTemp, Game [nI].hSquare [INDEX_BUFFER]) ;

    if (bB)
        {
        if (User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOML)
            {
            nX = Game [nI].rBuffer.left + Game [nI].ptBuffer [WHITE_QUEEN].x + Game [nI].nss ;
            for (nY = 0 ; nY < 3 ; nY++)
                {
                BitBlt (hdcWindow, nX, Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_QUEEN].y, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, nX, Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_QUEEN].y, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                nX = nX + Game [nI].nss ;
                }
            }
        else
        if (User.nBufferOrientation == DEFAULT_BUFFER_TOPBOTTOMR)
            {
            nX = Game [nI].rBuffer.left + 1 ;
            for (nY = 0 ; nY < 3 ; nY++)
                {
                BitBlt (hdcWindow, nX, Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_QUEEN].y, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, nX, Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_QUEEN].y, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                nX = nX + Game [nI].nss ;
                }
            }
        else
            {
            if (Game [nI].bFlip)
                {
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [WHITE_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_BISHOP].y,                                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [WHITE_BISHOP].x, Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_BISHOP].y + Game [nI].nss,                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [WHITE_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_BISHOP].y + Game [nI].nss,                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [WHITE_BISHOP].x, Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_BISHOP].y + Game [nI].nss + Game [nI].nss, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [WHITE_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_BISHOP].y + Game [nI].nss + Game [nI].nss, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [BLACK_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_BISHOP].y,                                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                }
            else
                {
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [BLACK_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_BISHOP].y,                                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [BLACK_BISHOP].x, Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_BISHOP].y + Game [nI].nss,                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [BLACK_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_BISHOP].y + Game [nI].nss,                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [BLACK_BISHOP].x, Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_BISHOP].y + Game [nI].nss + Game [nI].nss, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [BLACK_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [BLACK_BISHOP].y + Game [nI].nss + Game [nI].nss, Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                BitBlt (hdcWindow, Game [nI].rBuffer.left + Game [nI].ptBuffer [WHITE_QUEEN].x,  Game [nI].rBuffer.top + Game [nI].ptBuffer [WHITE_BISHOP].y,                                 Game [nI].nss, Game [nI].nss, hdcTemp, 0, 0, SRCCOPY) ;
                }
            }
        }

    // clean up
    SelectObject (hdcWindow, htOldWindow) ;
    SelectObject (hdcWindow, hpOldWindow) ;
    DeleteDC     (hdcWindow) ;

    // copy into target
    SelectObject (hdcTemp, htWindow) ;

    if (bB)
        {
        switch (User.nBufferOrientation)
            {
            case DEFAULT_BUFFER_LEFT :
                BitBlt (hdc, Game [nI].rBuffer.left, Game [nI].rBuffer.top, nW + nW1, nH, hdcTemp, Game [nI].rBuffer.left, Game [nI].rBuffer.top, SRCCOPY) ;
                break ;

            case DEFAULT_BUFFER_RIGHT :
                BitBlt (hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW + nW1, nH, hdcTemp, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
                break ;

            case DEFAULT_BUFFER_TOPBOTTOML :
            case DEFAULT_BUFFER_TOPBOTTOMR :
                BitBlt (hdc, Game [nI].rBuffer.left, Game [nI].rBuffer.top, nW, (Game [nI].rBuffer1.bottom - Game [nI].rBuffer.top) + 1, hdcTemp, Game [nI].rBuffer.left, Game [nI].rBuffer.top, SRCCOPY) ;
                break ;

            default :
                BitBlt (hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW + nW1, nH, hdcTemp, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
                break ;
            }
        }
    else
        {
        BitBlt (hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW, nH, hdcTemp, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
        }

    SelectObject (hdcTemp, htOldTemp) ;

    // clean up
    DeleteDC (hdcTemp) ;
    DeleteObject (htWindow) ;

    // display window title
    SetWindowText (hwnd, TOOLBOX_GetGameWindowTitle (nI)) ;
    }

int BOARD_OnMouseDown (int nG, HWND hwnd, HDC hdc, int bBoard, int nMx, int nMy, int nX, int nY)
    {
    int nBx, nBy, nI ;

    if ((nG != INDEX_PLAY) || (DragInfo.nPc != EMPTY_SQUARE))
        {
        if (User.nMoveType == CLICK_MOVE)
            {
            return 0 ;
            }
        BOARD_RestoreDragMove (nG, hdc) ;
        return 1 ;
        }

    if (bBoard)                 // board
        {
        if (Game [nG].bFlip)
            {
            nBx = ReverseCoord [nX] ;
            nBy = nY ;
            }
        else
            {
            nBx = nX ;
            nBy = ReverseCoord [nY] ;
            }

        if (DragInfo.ptFrom.x == nBx && DragInfo.ptFrom.y == nBy)
            {
            DragInfo.nIndex   = -1 ;
            DragInfo.nPc      = EMPTY_SQUARE ;
            DragInfo.ptFrom.x = -1 ;
            DragInfo.ptFrom.y = -1 ;
            DragInfo.nClicked = 0 ;
            return 1 ;
            }

        // assign left mouse drop
        LeftMouseDrop.nI = nG ;
        LeftMouseDrop.nX = nBx ;
        LeftMouseDrop.nY = nBy ;

        if (Game [nG].nBoard [nBx] [nBy] == EMPTY_SQUARE)
            {
            // smart move (destination square)
            if ((User.bSmartMove) && (User.bSmartMove2))
                {
                if (System.bIsMyTurn)
                    {
                    if (CHESS_SmartMoveDestination (nG, nBx, nBy))
                        {
                        BOARD_MakeSmartMove (hdc,
                                             Moves.nFromX [Moves.nLastMatch],
                                             Moves.nFromY [Moves.nLastMatch],
                                             Moves.nToX [Moves.nLastMatch],
                                             Moves.nToY [Moves.nLastMatch],
                                             Moves.nPromote [Moves.nLastMatch]) ;
                        return 1 ;
                        }
                    }
                }
            return 2 ;
            }

        if (Game [nG].bPlaying)
            {
            if (Game [nG].bIPlayWhite)
                {
                if (BOARD_IsBlackPiece (Game [nG].nBoard [nBx] [nBy]))
                    {
                    // smart move (destination square)
                    if ((User.bSmartMove) && (User.bSmartMove2))
                        {
                        if (System.bIsMyTurn)
                            {
                            if (CHESS_SmartMoveDestination (nG, nBx, nBy))
                                {
                                BOARD_MakeSmartMove (hdc,
                                                     Moves.nFromX [Moves.nLastMatch],
                                                     Moves.nFromY [Moves.nLastMatch],
                                                     Moves.nToX [Moves.nLastMatch],
                                                     Moves.nToY [Moves.nLastMatch],
                                                     Moves.nPromote [Moves.nLastMatch]) ;
                                return 1 ;
                                }
                            }
                        }

                    // clicked on opponent's black king -- generate legal king square
                    if (User.bShowKingHighlight)
                        {
                        if (User.nShowKingHighlightPO == 2 ||
                            User.nShowKingHighlightE  == 2)
                            {
                            if (Game [nG].nBoard [nBx] [nBy] == BLACK_KING)
                                {
                                if (CHESS_GenKingLegalSquare (nG, INDEX_BLACK))
                                    {
                                    BOARD_DrawBoard (nG, Game [nG].hwnd, hdc, DRAW_STATE_NONE) ;
                                    }
                                }
                            }
                        }
                    return 1 ;
                    }
                }
            else
                {
                if (BOARD_IsWhitePiece (Game [nG].nBoard [nBx] [nBy]))
                    {
                    // smart move (destination square)
                    if ((User.bSmartMove) && (User.bSmartMove2))
                        {
                        if (System.bIsMyTurn)
                            {
                            if (CHESS_SmartMoveDestination (nG, nBx, nBy))
                                {
                                BOARD_MakeSmartMove (hdc,
                                                     Moves.nFromX [Moves.nLastMatch],
                                                     Moves.nFromY [Moves.nLastMatch],
                                                     Moves.nToX [Moves.nLastMatch],
                                                     Moves.nToY [Moves.nLastMatch],
                                                     Moves.nPromote [Moves.nLastMatch]) ;
                                return 1 ;
                                }
                            }
                        }

                    // clicked on opponent's white king -- generate legal king square
                    if (User.bShowKingHighlight)
                        {
                        if (User.nShowKingHighlightPO == 2 ||
                            User.nShowKingHighlightE  == 2)
                            {
                            if (Game [nG].nBoard [nBx] [nBy] == WHITE_KING)
                                {
                                if (CHESS_GenKingLegalSquare (nG, INDEX_WHITE))
                                    {
                                    BOARD_DrawBoard (nG, Game [nG].hwnd, hdc, DRAW_STATE_NONE) ;
                                    }
                                }
                            }
                        }
                    return 1 ;
                    }
                }
            }
        else
            {
            if (Game [nG].bWhitesMove)
                {
                if (BOARD_IsBlackPiece (Game [nG].nBoard [nBx] [nBy]))
                    {
                    // smart move (destination square)
                    if ((User.bSmartMove) && (User.bSmartMove2))
                        {
                        if (System.bIsMyTurn)
                            {
                            if (CHESS_SmartMoveDestination (nG, nBx, nBy))
                                {
                                BOARD_MakeSmartMove (hdc,
                                                     Moves.nFromX [Moves.nLastMatch],
                                                     Moves.nFromY [Moves.nLastMatch],
                                                     Moves.nToX [Moves.nLastMatch],
                                                     Moves.nToY [Moves.nLastMatch],
                                                     Moves.nPromote [Moves.nLastMatch]) ;
                                return 1 ;
                                }
                            }
                        }

                    // clicked on opponent's black king -- generate legal king square
                    if (User.bShowKingHighlight)
                        {
                        if (User.nShowKingHighlightPO == 2 ||
                            User.nShowKingHighlightE  == 2)
                            {
                            if (Game [nG].nBoard [nBx] [nBy] == BLACK_KING)
                                {
                                if (CHESS_GenKingLegalSquare (nG, INDEX_BLACK))
                                    {
                                    BOARD_DrawBoard (nG, Game [nG].hwnd, hdc, DRAW_STATE_NONE) ;
                                    }
                                }
                            }
                        }
                    return 1 ;
                    }
                }
            else
                {
                if (BOARD_IsWhitePiece (Game [nG].nBoard [nBx] [nBy]))
                    {
                    // smart move (destination square)
                    if ((User.bSmartMove) && (User.bSmartMove2))
                        {
                        if (System.bIsMyTurn)
                            {
                            if (CHESS_SmartMoveDestination (nG, nBx, nBy))
                                {
                                BOARD_MakeSmartMove (hdc,
                                                     Moves.nFromX [Moves.nLastMatch],
                                                     Moves.nFromY [Moves.nLastMatch],
                                                     Moves.nToX [Moves.nLastMatch],
                                                     Moves.nToY [Moves.nLastMatch],
                                                     Moves.nPromote [Moves.nLastMatch]) ;
                                return 1 ;
                                }
                            }
                        }

                    // clicked on opponent's white king -- generate legal king square
                    if (User.bShowKingHighlight)
                        {
                        if (User.nShowKingHighlightPO == 2 ||
                            User.nShowKingHighlightE  == 2)
                            {
                            if (Game [nG].nBoard [nBx] [nBy] == WHITE_KING)
                                {
                                if (CHESS_GenKingLegalSquare (nG, INDEX_WHITE))
                                    {
                                    BOARD_DrawBoard (nG, Game [nG].hwnd, hdc, DRAW_STATE_NONE) ;
                                    }
                                }
                            }
                        }
                    return 1 ;
                    }
                }
            }

        // smart move (source square)
        if ((User.bSmartMove) && (User.bSmartMove1))
            {
            if (System.bIsMyTurn)
                {
                if (CHESS_SmartMoveSource (nG, nBx, nBy))
                    {
                    BOARD_MakeSmartMove (hdc,
                                         nBx,
                                         nBy,
                                         Moves.nToX [0],
                                         Moves.nToY [0],
                                         Moves.nPromote [0]) ;
                    return 1 ;
                    }
                }
            }

        // clicked on king -- generate legal king square
        if (User.bShowKingHighlight)
            {
            if (User.nShowKingHighlightPM == 2 ||
                User.nShowKingHighlightE  == 2)
                {
                if (Game [nG].nBoard [nBx] [nBy] == WHITE_KING)
                    {
                    if (CHESS_GenKingLegalSquare (nG, INDEX_WHITE))
                        {
                        BOARD_DrawBoard (nG, Game [nG].hwnd, hdc, DRAW_STATE_NONE) ;
                        }
                    }
                else
                if (Game [nG].nBoard [nBx] [nBy] == BLACK_KING)
                    {
                    if (CHESS_GenKingLegalSquare (nG, INDEX_BLACK))
                        {
                        BOARD_DrawBoard (nG, Game [nG].hwnd, hdc, DRAW_STATE_NONE) ;
                        }
                    }
                }
            }

        // setup drag information
        DragInfo.nIndex     = nG ;
        DragInfo.bFromBoard = 1 ;
        DragInfo.ptFrom.x   = nBx ;
        DragInfo.ptFrom.y   = nBy ;

        if (User.bAutoCenterDragPiece)
            {
            DragInfo.ptCurrent.x = nMx - Game [nG].nhss ;
            DragInfo.ptCurrent.y = nMy - Game [nG].nhss ;
            }
        else
            {
            BOARD_SquareToPosition (nG, nBx, nBy, &DragInfo.ptCurrent.x, &DragInfo.ptCurrent.y) ;

            DragInfo.ptDist.x = nMx - DragInfo.ptCurrent.x ;
            DragInfo.ptDist.y = nMy - DragInfo.ptCurrent.y ;
            }

        DragInfo.ptLast   = DragInfo.ptCurrent ;
        DragInfo.nPc      = Game [nG].nBoard [nBx] [nBy] ;
        DragInfo.nClicked = 1 ;

        if (User.nMoveType == DRAG_MOVE)
            {
            BOARD_DrawBoard (nG, hwnd, hdc, DRAW_STATE_MOUSE_DOWN) ;
            }
        }
    else                        // buffer
        {
        // reset left mouse drop
        LeftMouseDrop.nI = -1 ;
        LeftMouseDrop.nX = -1 ;
        LeftMouseDrop.nY = -1 ;

        for (nI = 0 ; nI < MAX_BUFFER ; nI++)
            {
            if (Game [nG].nBuffer [nI] > 0                                     &&
                nMx >= Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x                &&
                nMx <= Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x + Game [nG].nss &&
                nMy >= Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y                &&
                nMy <= Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y + Game [nG].nss)
                {
                if (Game [nG].bPlaying)
                    {
                    if (Game [nG].bIPlayWhite)
                        {
                        if (BOARD_IsBlackPiece (nI))
                            {
                            break ;
                            }
                        }
                    else
                        {
                        if (BOARD_IsWhitePiece (nI))
                            {
                            break ;
                            }
                        }
                    }
                else
                    {
                    if (Game [nG].bWhitesMove)
                        {
                        if (BOARD_IsBlackPiece (nI))
                            {
                            break ;
                            }
                        }
                    else
                        {
                        if (BOARD_IsWhitePiece (nI))
                            {
                            break ;
                            }
                        }
                    }

                // setup drag information
                DragInfo.nIndex     = nG ;
                DragInfo.bFromBoard = 0 ;
                DragInfo.ptFrom.x   = -1 ;
                DragInfo.ptFrom.y   = -1 ;

                if (User.bAutoCenterDragPiece)
                    {
                    DragInfo.ptCurrent.x = nMx - Game [nG].nhss ;
                    DragInfo.ptCurrent.y = nMy - Game [nG].nhss ;
                    }
                else
                    {
                    DragInfo.ptCurrent.x = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
                    DragInfo.ptCurrent.y = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;

                    DragInfo.ptDist.x = nMx - DragInfo.ptCurrent.x ;
                    DragInfo.ptDist.y = nMy - DragInfo.ptCurrent.y ;
                    }

                DragInfo.ptLast   = DragInfo.ptCurrent ;
                DragInfo.nPc      = nI ;
                DragInfo.nClicked = 1 ;

                if (User.nMoveType == DRAG_MOVE)
                    {
                    BOARD_DrawBoard (nG, hwnd, hdc, DRAW_STATE_MOUSE_DOWN) ;
                    }
                break ;
                }
            }
        }
    return (DragInfo.nPc == EMPTY_SQUARE) ;
    }

void BOARD_OnMouseMove (int nG, HWND hwnd, HDC hdc, int nMx, int nMy)
    {
    if (User.bAutoCenterDragPiece)
        {
        DragInfo.ptCurrent.x = nMx - Game [nG].nhss ;
        DragInfo.ptCurrent.y = nMy - Game [nG].nhss ;
        }
    else
        {
        DragInfo.ptCurrent.x = nMx - DragInfo.ptDist.x ;
        DragInfo.ptCurrent.y = nMy - DragInfo.ptDist.y ;
        }

    if (User.nMoveType == DRAG_MOVE)
        {
        BOARD_DrawBoard (nG, hwnd, hdc, DRAW_STATE_MOUSE_MOVE) ;
        }

    DragInfo.ptLast = DragInfo.ptCurrent ;
    }

int BOARD_OnMouseUp (int nG, HWND hwnd, HDC hdc, int nX, int nY, int *bRefresh)
    {
    int nBx, nBy, nPpc, bP = 0, nOA ;
    char cTmp [50] ;
    RECT rc ;

    if ((nG != INDEX_PLAY) || (nX < 0 || nX > 7 || nY < 0 || nY > 7))
        {
        *bRefresh = 0 ;
        F8KEY_Init () ;
        TOOLBOX_ResetPromotKnight () ;
        return 0 ;
        }

    if (Game [nG].bFlip)
        {
        nBx = ReverseCoord [nX] ;
        nBy = nY ;
        }
    else
        {
        nBx = nX ;
        nBy = ReverseCoord [nY] ;
        }

    if (DragInfo.bFromBoard)
        {
        if (nBx == DragInfo.ptFrom.x && nBy == DragInfo.ptFrom.y)
            {
            *bRefresh = 0 ;
            return 0 ;
            }

        if (System.bIsMyTurn || (! Game [nG].bPlaying))
            {
            if (Game [nG].nBoard [nBx] [nBy] != EMPTY_SQUARE)
                {
                if (BOARD_IsWhitePiece (DragInfo.nPc))
                    {
                    if (BOARD_IsWhitePiece (Game [nG].nBoard [nBx] [nBy]))
                        {
                        *bRefresh = 0 ;
                        F8KEY_Init () ;
                        TOOLBOX_ResetPromotKnight () ;
                        return 0 ;
                        }
                    }
                else
                    {
                    if (BOARD_IsBlackPiece (Game [nG].nBoard [nBx] [nBy]))
                        {
                        *bRefresh = 0 ;
                        F8KEY_Init () ;
                        TOOLBOX_ResetPromotKnight () ;
                        return 0 ;
                        }
                    }
                }
            }

        if (DragInfo.nPc == WHITE_PAWN)
            {
            if (nBy == 0)
                {
                *bRefresh = 0 ;
                F8KEY_Init () ;
                TOOLBOX_ResetPromotKnight () ;
                return 0 ;
                }

            if (nBy == 7)
                {
                if (DragInfo.ptFrom.y != 6)
                    {
                    *bRefresh = 0 ;
                    F8KEY_Init () ;
                    TOOLBOX_ResetPromotKnight () ;
                    return 0 ;
                    }

                if (System.bPromoteKnight)
                    {
                    DragInfo.nPc = WHITE_KNIGHT ;
                    bP = 1 ;
                    }
                else
                    {
                    if (User.bAutoQueen &&
                        ((Game [nG].nGameType != GAMETYPE_LOSER) ||
                        ((Game [nG].nGameType == GAMETYPE_LOSER) && (System.bPromoteCommand))))
                        {
                        if ((Game [nG].nGameType == GAMETYPE_SUICIDE ) ||
                            (Game [nG].nGameType == GAMETYPE_GIVEAWAY))
                            {
                            if (System.bPromoteCommand)
                                {
                                DragInfo.nPc = System.nPromoteWPiece ;
                                }
                            else
                                {
                                DragInfo.nPc = WHITE_KING ;
                                }
                            }
                        else
                            {
                            if (System.bPromoteCommand)
                                {
                                if (System.nPromoteWPiece == WHITE_KING)
                                    {
                                    DragInfo.nPc = WHITE_QUEEN ;
                                    }
                                else
                                    {
                                    DragInfo.nPc = System.nPromoteWPiece ;
                                    }
                                }
                            else
                                {
                                DragInfo.nPc = WHITE_QUEEN ;
                                }
                            }
                        bP = 1 ;
                        }
                    else
                        {
                        GetWindowRect (hwnd, &rc) ;

                        nPpc = PROMOTE_Pawn (hwnd, Game [nG].hGame, rc.left + Game [nG].rBoard.left + (nX * Game [nG].nss) + 1, rc.top + Game [nG].rBoard.top + (nY * Game [nG].nss) + 1) ;
                        if ((! nPpc) || (DragInfo.nPc == EMPTY_SQUARE))
                            {
                            *bRefresh = 0 ;
                            F8KEY_Init () ;
                            TOOLBOX_ResetPromotKnight () ;
                            return 0 ;
                            }

                        switch (nPpc)
                            {
                            case IDD_QUEEN :
                                DragInfo.nPc = WHITE_QUEEN ;
                                bP = 1 ;
                                break ;

                            case IDD_ROOK :
                                DragInfo.nPc = WHITE_ROOK ;
                                bP = 1 ;
                                break ;

                            case IDD_BISHOP :
                                DragInfo.nPc = WHITE_BISHOP ;
                                bP = 1 ;
                                break ;

                            case IDD_KNIGHT :
                                DragInfo.nPc = WHITE_KNIGHT ;
                                bP = 1 ;
                                break ;

                            case IDD_KING :
                                DragInfo.nPc = WHITE_KING ;
                                bP = 1 ;
                                break ;

                            default :
                                *bRefresh = 0 ;
                                F8KEY_Init () ;
                                TOOLBOX_ResetPromotKnight () ;
                                return 0 ;
                            }
                        }
                    }
                }
            }
        else
        if (DragInfo.nPc == BLACK_PAWN)
            {
            if (nBy == 7)
                {
                *bRefresh = 0 ;
                F8KEY_Init () ;
                TOOLBOX_ResetPromotKnight () ;
                return 0 ;
                }

            if (nBy == 0)
                {
                if (DragInfo.ptFrom.y != 1)
                    {
                    *bRefresh = 0 ;
                    F8KEY_Init () ;
                    TOOLBOX_ResetPromotKnight () ;
                    return 0 ;
                    }

                if (System.bPromoteKnight)
                    {
                    DragInfo.nPc = BLACK_KNIGHT ;
                    bP = 1 ;
                    }
                else
                    {
                    if (User.bAutoQueen &&
                        ((Game [nG].nGameType != GAMETYPE_LOSER) ||
                        ((Game [nG].nGameType == GAMETYPE_LOSER) && (System.bPromoteCommand))))
                        {
                        if ((Game [nG].nGameType == GAMETYPE_SUICIDE ) ||
                            (Game [nG].nGameType == GAMETYPE_GIVEAWAY))
                            {
                            if (System.bPromoteCommand)
                                {
                                DragInfo.nPc = System.nPromoteBPiece ;
                                }
                            else
                                {
                                DragInfo.nPc = BLACK_KING ;
                                }
                            }
                        else
                            {
                            if (System.bPromoteCommand)
                                {
                                if (System.nPromoteBPiece == BLACK_KING)
                                    {
                                    DragInfo.nPc = BLACK_QUEEN ;
                                    }
                                else
                                    {
                                    DragInfo.nPc = System.nPromoteBPiece ;
                                    }
                                }
                            else
                                {
                                DragInfo.nPc = BLACK_QUEEN ;
                                }
                            }
                        bP = 1 ;
                        }
                    else
                        {
                        GetWindowRect (hwnd, &rc) ;

                        nPpc = PROMOTE_Pawn (hwnd, Game [nG].hGame, rc.left + Game [nG].rBoard.left + (nX * Game [nG].nss) + 1, rc.top + Game [nG].rBoard.top + (nY * Game [nG].nss) + 1) ;
                        if ((! nPpc) || (DragInfo.nPc == EMPTY_SQUARE))
                            {
                            *bRefresh = 0 ;
                            F8KEY_Init () ;
                            TOOLBOX_ResetPromotKnight () ;
                            return 0 ;
                            }

                        switch (nPpc)
                            {
                            case IDD_QUEEN :
                                DragInfo.nPc = BLACK_QUEEN ;
                                bP = 1 ;
                                break ;

                            case IDD_ROOK :
                                DragInfo.nPc = BLACK_ROOK ;
                                bP = 1 ;
                                break ;

                            case IDD_BISHOP :
                                DragInfo.nPc = BLACK_BISHOP ;
                                bP = 1 ;
                                break ;

                            case IDD_KNIGHT :
                                DragInfo.nPc = BLACK_KNIGHT ;
                                bP = 1 ;
                                break ;

                            case IDD_KING :
                                DragInfo.nPc = BLACK_KING ;
                                bP = 1 ;
                                break ;

                            default :
                                *bRefresh = 0 ;
                                F8KEY_Init () ;
                                TOOLBOX_ResetPromotKnight () ;
                                return 0 ;
                            }
                        }
                    }
                }
            }
        }
    else
        {
        if (DragInfo.nPc == WHITE_PAWN || DragInfo.nPc == BLACK_PAWN)
            {
            if (nBy == 0 || nBy == 7)
                {
                *bRefresh = 0 ;
                F8KEY_Init () ;
                TOOLBOX_ResetPromotKnight () ;
                return 0 ;
                }
            }

        if (System.bIsMyTurn || (! Game [nG].bPlaying))
            {
            if (Game [nG].nBoard [nBx] [nBy] != EMPTY_SQUARE)
                {
                *bRefresh = 0 ;
                F8KEY_Init () ;
                TOOLBOX_ResetPromotKnight () ;
                return 0 ;
                }
            }
        }

    if (DragInfo.nPc == EMPTY_SQUARE)
        {
        *bRefresh = 0 ;
        F8KEY_Init () ;
        TOOLBOX_ResetPromotKnight () ;
        return 0 ;
        }
    else
        {
        // make move
        nPpc = DragInfo.nPc ;

        if (DragInfo.bFromBoard)
            {
            if (bP)
                {
                if (System.bPromoteKnight)
                    {
                    TOOLBOX_ResetPromotKnight () ;

                    nOA = User.bAutoQueen ;
                    User.bAutoQueen = 0 ;

                    if (System.bIsMyTurn || (! Game [nG].bPlaying))
                        {
                        if (User.bTestLegalPlay)
                            {
                            if (! CHESS_IsLegalMove (nG,
                                                     Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y],
                                                     DragInfo.ptFrom.x,
                                                     DragInfo.ptFrom.y,
                                                     nBx,
                                                     nBy,
                                                     nPpc))
                                {
                                User.bAutoQueen = nOA ;
                                *bRefresh = 1 ;
                                return 0 ;
                                }
                            }
                        }
                    else
                        {
                        if (! CHESS_IsLegalTruePremove (nG,
                                                        Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y],
                                                        DragInfo.ptFrom.x,
                                                        DragInfo.ptFrom.y,
                                                        nBx,
                                                        nBy,
                                                        nPpc))
                            {
                            User.bAutoQueen = nOA ;
                            *bRefresh = 1 ;
                            return 0 ;
                            }
                        }

                    User.bAutoQueen = nOA ;
                    }
                else
                    {
                    if (System.bIsMyTurn || (! Game [nG].bPlaying))
                        {
                        if (User.bTestLegalPlay)
                            {
                            if (! CHESS_IsLegalMove (nG,
                                                     Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y],
                                                     DragInfo.ptFrom.x,
                                                     DragInfo.ptFrom.y,
                                                     nBx,
                                                     nBy,
                                                     nPpc))
                                {
                                *bRefresh = 1 ;
                                return 0 ;
                                }
                            }
                        }
                    else
                        {
                        if (! CHESS_IsLegalTruePremove (nG,
                                                        Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y],
                                                        DragInfo.ptFrom.x,
                                                        DragInfo.ptFrom.y,
                                                        nBx,
                                                        nBy,
                                                        nPpc))
                            {
                            *bRefresh = 1 ;
                            return 0 ;
                            }
                        }
                    }

                sprintf (cTmp, "%c%d%c%d=%c\n",
                         DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                         nBx + 'a', nBy + 1, ICSPiece [nPpc]) ;
                }
            else
                {
                if (System.bIsMyTurn || (! Game [nG].bPlaying))
                    {
                    if (User.bTestLegalPlay)
                        {
                        if (! CHESS_IsLegalMove (nG,
                                                 nPpc,
                                                 DragInfo.ptFrom.x,
                                                 DragInfo.ptFrom.y,
                                                 nBx,
                                                 nBy,
                                                 EMPTY_SQUARE))
                            {
                            *bRefresh = 1 ;
                            return 0 ;
                            }
                        }
                    }
                else
                    {
                    if (! CHESS_IsLegalTruePremove (nG,
                                                    nPpc,
                                                    DragInfo.ptFrom.x,
                                                    DragInfo.ptFrom.y,
                                                    nBx,
                                                    nBy,
                                                    EMPTY_SQUARE))
                        {
                        *bRefresh = 1 ;
                        return 0 ;
                        }
                    }

                sprintf (cTmp, "%c%d%c%d\n",
                         DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                         nBx + 'a', nBy + 1) ;
                }

            if (System.bIsMyTurn)
                {
                TOOLBOX_WriteICS (cTmp) ;
                if (Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
                    {
                    if (Game [nG].nBoard [nBx] [nBy] == EMPTY_SQUARE)
                        {
                        Game [nG].nBoard [nBx] [nBy] = nPpc ;
                        Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
                        }
                    else
                        {
                        Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
                        CHESS_AtomicCapture (nG, nBx, nBy) ;
                        }
                    }
                else
                    {
                    Game [nG].nBoard [nBx] [nBy] = nPpc ;
                    Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
                    }
                }
            else
                {
                if (User.bTruePremove && Game [nG].bPlaying)
                    {
                    if (Premove.nPremoveCount >= MAX_TRUE_PREMOVE)
                        {
                        Premove.nPremoveTail = Premove.nPremoveTail + 1 ;

                        if (Premove.nPremoveTail >= MAX_TRUE_PREMOVE)
                            {
                            Premove.nPremoveTail = 0 ;
                            }

                        Premove.bLastTP        = 1 ;
                        Premove.ptLastTP [0].x = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [3] ;
                        Premove.ptLastTP [0].y = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [4] ;
                        Premove.ptLastTP [1].x = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [5] ;
                        Premove.ptLastTP [1].y = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [6] ;
                        }

                    strcpy (Premove.cPremoveBuffer [Premove.nPremoveHead], cTmp) ;

                    if (bP)
                        {
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [1] = nG ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [2] = Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [3] = DragInfo.ptFrom.x ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [4] = DragInfo.ptFrom.y ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [5] = nBx ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [6] = nBy ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [7] = nPpc ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [8] = Game [nG].nBoard [nBx] [nBy] ;
                        }
                    else
                        {
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [1] = nG ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [2] = nPpc ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [3] = DragInfo.ptFrom.x ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [4] = DragInfo.ptFrom.y ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [5] = nBx ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [6] = nBy ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [7] = EMPTY_SQUARE ;
                        Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [8] = Game [nG].nBoard [nBx] [nBy] ;
                        }

                    if (Premove.nPremoveCount < MAX_TRUE_PREMOVE)
                        {
                        Premove.nPremoveCount = Premove.nPremoveCount + 1 ;
                        }

                    Premove.nPremoveHead = Premove.nPremoveHead + 1 ;

                    if (Premove.nPremoveHead >= MAX_TRUE_PREMOVE)
                        {
                        Premove.nPremoveHead = 0 ;
                        }

                    BOARD_RestoreDragMove (nG, hdc) ;

                    GAMESOUND_Play (GAME_SOUND_PLAY_MOVE_TP) ;

                    *bRefresh = 0 ;
                    F8KEY_Init () ;
                    TOOLBOX_ResetPromotKnight () ;
                    return 1 ;
                    }
                else
                    {
                    if (Game [nG].nGameNumber != 0)
                        {
                        // TOOLBOX_WriteICS (cTmp) ;

                        // new in 1.23: not our move, discard the move:
						if (Game [nG].bPlaying) {
							TOOLBOX_WriteSystem("It is not your move\n");
							*bRefresh = 1 ;
							return 0;
						}else{
							TOOLBOX_WriteICS (cTmp) ;
						}

                        }
                    if (Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
                        {
                        if (Game [nG].nBoard [nBx] [nBy] == EMPTY_SQUARE)
                            {
                            Game [nG].nBoard [nBx] [nBy] = nPpc ;
                            Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
                            }
                        else
                            {
                            Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
                            CHESS_AtomicCapture (nG, nBx, nBy) ;
                            }
                        }
                    else
                        {
                        Game [nG].nBoard [nBx] [nBy] = nPpc ;
                        Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
                        }
                    }
                }

            if (System.bIsMyTurn || Game [nG].nGameNumber == 0)
                {
                if (Game [nG].bWhitesMove)
                    {
                    if (nPpc == WHITE_PAWN)
                        {
                        if (nBx == Game [nG].nLastDoublePushFile)
                            {
                            Game [nG].nLastDoublePushFile = -1 ;

                            if (nBy == 5 && Game [nG].nBoard [nBx] [nBy - 1] == BLACK_PAWN)
                                {
                                Game [nG].nBoard [nBx] [nBy - 1] = EMPTY_SQUARE ;

                                if (Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
                                    {
                                    CHESS_AtomicCapture (nG, nBx, nBy) ;
                                    }
                                }
                            }
                        else
                            {
                            Game [nG].nLastDoublePushFile = -1 ;

                            if (DragInfo.ptFrom.y == 1 && nBy == 3)
                                {
                                Game [nG].nLastDoublePushFile = nBx ;
                                }
                            }
                        }
                    else
                    if (nPpc == WHITE_KING)
                        {
                        if (Game [nG].bCanCastleKingSide [INDEX_WHITE])
                            {
                            if (DragInfo.ptFrom.x               == 4 &&
                                DragInfo.ptFrom.y               == 0 &&
                                nBx                             == 6 &&
                                nBy                             == 0 &&
                                Game [nG].nBoard [5] [0] == EMPTY_SQUARE &&
                                Game [nG].nBoard [7] [0] == WHITE_ROOK)
                                {
                                Game [nG].nBoard [5] [0] = WHITE_ROOK ;
                                Game [nG].nBoard [7] [0] = EMPTY_SQUARE ;
                                }
                            }

                        if (Game [nG].bCanCastleQueenSide [INDEX_WHITE])
                            {
                            if (DragInfo.ptFrom.x               == 4 &&
                                DragInfo.ptFrom.y               == 0 &&
                                nBx                             == 2 &&
                                nBy                             == 0 &&
                                Game [nG].nBoard [0] [0] == WHITE_ROOK   &&
                                Game [nG].nBoard [1] [0] == EMPTY_SQUARE &&
                                Game [nG].nBoard [3] [0] == EMPTY_SQUARE)
                                {
                                Game [nG].nBoard [3] [0] = WHITE_ROOK ;
                                Game [nG].nBoard [0] [0] = EMPTY_SQUARE ;
                                }
                            }
                        }
                    }
                else
                    {
                    if (nPpc == BLACK_PAWN)
                        {
                        if (nBx == Game [nG].nLastDoublePushFile)
                            {
                            Game [nG].nLastDoublePushFile = -1 ;

                            if (nBy == 2 && Game [nG].nBoard [nBx] [nBy + 1] == WHITE_PAWN)
                                {
                                Game [nG].nBoard [nBx] [nBy + 1] = EMPTY_SQUARE ;

                                if (Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
                                    {
                                    CHESS_AtomicCapture (nG, nBx, nBy) ;
                                    }
                                }
                            }
                        else
                            {
                            Game [nG].nLastDoublePushFile = -1 ;

                            if (DragInfo.ptFrom.y == 6 && nBy == 4)
                                {
                                Game [nG].nLastDoublePushFile = nBx ;
                                }
                            }
                        }
                    else
                    if (nPpc == BLACK_KING)
                        {
                        if (Game [nG].bCanCastleKingSide [INDEX_BLACK])
                            {
                            if (DragInfo.ptFrom.x               == 4 &&
                                DragInfo.ptFrom.y               == 7 &&
                                nBx                             == 6 &&
                                nBy                             == 7 &&
                                Game [nG].nBoard [5] [7] == EMPTY_SQUARE &&
                                Game [nG].nBoard [7] [7] == BLACK_ROOK)
                                {
                                Game [nG].nBoard [5] [7] = BLACK_ROOK ;
                                Game [nG].nBoard [7] [7] = EMPTY_SQUARE ;
                                }
                            }

                        if (Game [nG].bCanCastleQueenSide [INDEX_BLACK])
                            {
                            if (DragInfo.ptFrom.x               == 4 &&
                                DragInfo.ptFrom.y               == 7 &&
                                nBx                             == 2 &&
                                nBy                             == 7 &&
                                Game [nG].nBoard [0] [7] == BLACK_ROOK   &&
                                Game [nG].nBoard [1] [7] == EMPTY_SQUARE &&
                                Game [nG].nBoard [3] [7] == EMPTY_SQUARE)
                                {
                                Game [nG].nBoard [3] [7] = BLACK_ROOK ;
                                Game [nG].nBoard [0] [7] = EMPTY_SQUARE ;
                                }
                            }
                        }
                    }

                if (Game [nG].nGameNumber == 0)
                    {
                    Game [nG].ptHighlight [0].x = DragInfo.ptFrom.x ;
                    Game [nG].ptHighlight [0].y = DragInfo.ptFrom.y ;

                    Game [nG].ptHighlight [1].x = nBx ;
                    Game [nG].ptHighlight [1].y = nBy ;
                    }
                }
            }
        else
            {
            if (System.bIsMyTurn || (! Game [nG].bPlaying))
                {
                if (User.bTestLegalPlay)
                    {
                    if (! CHESS_IsLegalMove (nG,
                                             nPpc,
                                             -1,
                                             -1,
                                             nBx,
                                             nBy,
                                             EMPTY_SQUARE))
                        {
                        *bRefresh = 1 ;
                        return 0 ;
                        }
                    }
                }
            else
                {
                if (! CHESS_IsLegalTruePremove (nG,
                                                nPpc,
                                                -1,
                                                -1,
                                                nBx,
                                                nBy,
                                                EMPTY_SQUARE))
                    {
                    *bRefresh = 1 ;
                    return 0 ;
                    }
                }

            sprintf (cTmp, "%c@%c%d\n", ICSPiece [nPpc], nBx + 'a', nBy + 1) ;

            if (System.bIsMyTurn)
                {
                TOOLBOX_WriteICS (cTmp) ;

                Game [nG].nBoard [nBx] [nBy] = nPpc ;
                --Game [nG].nBuffer [nPpc] ;
                }
            else
                {
                if (User.bTruePremove && Game [nG].bPlaying)
                    {
                    if (Premove.nPremoveCount >= MAX_TRUE_PREMOVE)
                        {
                        Premove.nPremoveTail = Premove.nPremoveTail + 1 ;

                        if (Premove.nPremoveTail >= MAX_TRUE_PREMOVE)
                            {
                            Premove.nPremoveTail = 0 ;
                            }

                        Premove.bLastTP        = 1 ;
                        Premove.ptLastTP [0].x = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [3] ;
                        Premove.ptLastTP [0].y = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [4] ;
                        Premove.ptLastTP [1].x = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [5] ;
                        Premove.ptLastTP [1].y = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [6] ;
                        }

                    strcpy (Premove.cPremoveBuffer [Premove.nPremoveHead], cTmp) ;

                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [1] = nG ;
                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [2] = nPpc ;
                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [3] = -1 ;
                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [4] = -1 ;
                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [5] = nBx ;
                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [6] = nBy ;
                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [7] = EMPTY_SQUARE ;
                    Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [8] = EMPTY_SQUARE ;

                    if (Premove.nPremoveCount < MAX_TRUE_PREMOVE)
                        {
                        Premove.nPremoveCount = Premove.nPremoveCount + 1 ;
                        }

                    Premove.nPremoveHead = Premove.nPremoveHead + 1 ;

                    if (Premove.nPremoveHead >= MAX_TRUE_PREMOVE)
                        {
                        Premove.nPremoveHead = 0 ;
                        }

                    BOARD_RestoreDragMove (nG, hdc) ;

                    GAMESOUND_Play (GAME_SOUND_PLAY_MOVE_TP) ;

                    *bRefresh = 0 ;
                    F8KEY_Init () ;
                    TOOLBOX_ResetPromotKnight () ;
                    return 1 ;
                    }
                else
                    {
                    if (Game [nG].nGameNumber == 0)
                        {
                        Game [nG].ptHighlight [0].x = -1 ;
                        Game [nG].ptHighlight [0].y = -1 ;

                        Game [nG].ptHighlight [1].x = nBx ;
                        Game [nG].ptHighlight [1].y = nBy ;
                        }
                    else
                        {
                        TOOLBOX_WriteICS (cTmp) ;
                        }

                    Game [nG].nBoard [nBx] [nBy] = nPpc ;
                    --Game [nG].nBuffer [nPpc] ;
                    }
                }
            }

        Game [nG].bWhitesMove = (! Game [nG].bWhitesMove) ;

        if (Game [nG].bPlaying)
            {
            Game [nG].bOnLagClock = 1 ;
            }
        else
        if (Game [nG].nGameNumber == 0)
            {
            BOARD_DrawWhiteClock (nG, hdc) ;
            BOARD_DrawBlackClock (nG, hdc) ;
            BOARD_FindKings (nG) ;

            GAMESOUND_Play (GAME_SOUND_PLAY_MOVE) ;
            }

        CLOCK_SwitchClocks (nG) ;

        BOARD_DrawWhiteClock (nG, hdc) ;
        BOARD_DrawBlackClock (nG, hdc) ;

        BOARD_DrawBoard (nG, hwnd, hdc, DRAW_STATE_MOUSE_UP) ;

        System.bIsMyTurn = 0 ;

        F8KEY_Init () ;
        TOOLBOX_ResetPromotKnight () ;
        }

    *bRefresh = 0 ;
    return 1 ;
    }

void BOARD_RestoreDragMove (int nG, HDC hdc)
    {
    Game [nG].nCurrentIndex  = Game [nG].nMaxIndex ;
    Game [nG].nCurrentColor  = Game [nG].nMaxColor ;
    Game [nG].bClickedButton = 0 ;

    ReleaseCapture() ;

    if (nG == INDEX_PLAY)
        {
        BOARD_DrawBoard (nG, Game [nG].hwnd, hdc, DRAW_STATE_RESTORE) ;
        }
    else
        {
        BOARD_DrawBoard1 (nG, Game [nG].hwnd, hdc, DRAW_STATE_RESTORE) ;
        }
    }

int BOARD_OnMouseDown1 (int nG, HDC hdc, int bBoard, int nMx, int nMy, int nX, int nY)
    {
    int nBx, nBy, nI ;

    if ((nG == INDEX_PLAY) || (DragInfo.nPc != EMPTY_SQUARE))
        {
        BOARD_RestoreDragMove (nG, hdc) ;
        return 1 ;
        }

    if (bBoard)                 // board
        {
        if (Game [nG].bFlip)
            {
            nBx = ReverseCoord [nX] ;
            nBy = nY ;
            }
        else
            {
            nBx = nX ;
            nBy = ReverseCoord [nY] ;
            }

        if (DragInfo.ptFrom.x == nBx && DragInfo.ptFrom.y == nBy)
            {
            DragInfo.nIndex   = -1 ;
            DragInfo.nPc      = EMPTY_SQUARE ;
            DragInfo.ptFrom.x = -1 ;
            DragInfo.ptFrom.y = -1 ;
            DragInfo.nClicked = 0 ;
            return 1 ;
            }

        // assign left mouse drop
        LeftMouseDrop.nI = nG ;
        LeftMouseDrop.nX = nBx ;
        LeftMouseDrop.nY = nBy ;

        if (Game [nG].nBoard [nBx] [nBy] == EMPTY_SQUARE)
            {
            return 2 ;
            }

        // setup drag information
        DragInfo.nIndex     = nG ;
        DragInfo.bFromBoard = 1 ;
        DragInfo.ptFrom.x   = nBx ;
        DragInfo.ptFrom.y   = nBy ;

        if (User.bAutoCenterDragPiece)
            {
            DragInfo.ptCurrent.x = nMx - Game [nG].nhss ;
            DragInfo.ptCurrent.y = nMy - Game [nG].nhss ;
            }
        else
            {
            BOARD_SquareToPosition (nG, nBx, nBy, &DragInfo.ptCurrent.x, &DragInfo.ptCurrent.y) ;

            DragInfo.ptDist.x = nMx - DragInfo.ptCurrent.x ;
            DragInfo.ptDist.y = nMy - DragInfo.ptCurrent.y ;
            }

        DragInfo.ptLast   = DragInfo.ptCurrent ;
        DragInfo.nPc      = Game [nG].nBoard [nBx] [nBy] ;
        DragInfo.nClicked = 1 ;

        if (User.nMoveType == DRAG_MOVE)
            {
            BOARD_DrawBoard1 (nG, Game [nG].hwnd, hdc, DRAW_STATE_MOUSE_DOWN) ;
            }
        }
    else                        // buffer
        {
        // reset left mouse drop
        LeftMouseDrop.nI = -1 ;
        LeftMouseDrop.nX = -1 ;
        LeftMouseDrop.nY = -1 ;

        for (nI = 0 ; nI < MAX_BUFFER ; nI++)
            {
            if (Game [nG].nBuffer [nI] > 0                                             &&
                nMx >= Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x                &&
                nMx <= Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x + Game [nG].nss &&
                nMy >= Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y                &&
                nMy <= Game [nG].rBuffer.top +  Game [nG].ptBuffer [nI].y + Game [nG].nss)
                {

                // setup drag information
                DragInfo.nIndex     = nG ;
                DragInfo.bFromBoard = 0 ;
                DragInfo.ptFrom.x   = -1 ;
                DragInfo.ptFrom.y   = -1 ;

                if (User.bAutoCenterDragPiece)
                    {
                    DragInfo.ptCurrent.x = nMx - Game [nG].nhss ;
                    DragInfo.ptCurrent.y = nMy - Game [nG].nhss ;
                    }
                else
                    {
                    DragInfo.ptCurrent.x = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
                    DragInfo.ptCurrent.y = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;

                    DragInfo.ptDist.x = nMx - DragInfo.ptCurrent.x ;
                    DragInfo.ptDist.y = nMy - DragInfo.ptCurrent.y ;
                    }

                DragInfo.ptLast   = DragInfo.ptCurrent ;
                DragInfo.nPc      = nI ;
                DragInfo.nClicked = 1 ;

                if (User.nMoveType == DRAG_MOVE)
                    {
                    BOARD_DrawBoard1 (nG, Game [nG].hwnd, hdc, DRAW_STATE_MOUSE_DOWN) ;
                    }
                break ;
                }
            }
        }
    return (DragInfo.nPc == EMPTY_SQUARE) ;
    }

void BOARD_OnMouseMove1 (int nG, HDC hdc, int nMx, int nMy)
    {
    if (User.bAutoCenterDragPiece)
        {
        DragInfo.ptCurrent.x = nMx - Game [nG].nhss ;
        DragInfo.ptCurrent.y = nMy - Game [nG].nhss ;
        }
    else
        {
        DragInfo.ptCurrent.x = nMx - DragInfo.ptDist.x ;
        DragInfo.ptCurrent.y = nMy - DragInfo.ptDist.y ;
        }

    if (User.nMoveType == DRAG_MOVE)
        {
        BOARD_DrawBoard1 (nG, Game [nG].hwnd, hdc, DRAW_STATE_MOUSE_MOVE) ;
        }

    DragInfo.ptLast = DragInfo.ptCurrent ;
    }

int BOARD_OnMouseUp1 (int nG, HWND hwnd, HDC hdc, int nX, int nY)
    {
    int nBx, nBy, nPpc, bP = 0 ;
    char cTmp [255], cTmp1 [255] ;
    RECT rc ;

    if ((nG == INDEX_PLAY) || (nX < 0 || nX > 7 || nY < 0 || nY > 7))
        {
        return 0 ;
        }

    if (Game [nG].bFlip)
        {
        nBx = ReverseCoord [nX] ;
        nBy = nY ;
        }
    else
        {
        nBx = nX ;
        nBy = ReverseCoord [nY] ;
        }

    if (DragInfo.bFromBoard)
        {
        if (nBx == DragInfo.ptFrom.x && nBy == DragInfo.ptFrom.y)
            {
            return 0 ;
            }

        if (Game [nG].nBoard [nBx] [nBy] != EMPTY_SQUARE)
            {
            if (BOARD_IsWhitePiece (DragInfo.nPc))
                {
                if (BOARD_IsWhitePiece (Game [nG].nBoard [nBx] [nBy]))
                    {
                    return 0 ;
                    }
                }
            else
                {
                if (BOARD_IsBlackPiece (Game [nG].nBoard [nBx] [nBy]))
                    {
                    return 0 ;
                    }
                }
            }

        if (DragInfo.nPc == WHITE_PAWN)
            {
            if (nBy == 0)
                {
                return 0 ;
                }

            if (nBy == 7)
                {
                if (DragInfo.ptFrom.y != 6)
                    {
                    return 0 ;
                    }

                if (User.bAutoQueen)
                    {
                    DragInfo.nPc = WHITE_QUEEN ;
                    bP = 1 ;
                    }
                else
                    {
                    GetWindowRect (hwnd, &rc) ;

                    nPpc = PROMOTE_Pawn (hwnd, Game [nG].hGame, rc.left + Game [nG].rBoard.left + (nX * Game [nG].nss) + 1, rc.top + Game [nG].rBoard.top + (nY * Game [nG].nss) + 1) ;
                    if ((! nPpc) || (DragInfo.nPc == EMPTY_SQUARE))
                        {
                        return 0 ;
                        }

                    switch (nPpc)
                        {
                        case IDD_QUEEN :
                            DragInfo.nPc = WHITE_QUEEN ;
                            bP = 1 ;
                            break ;

                        case IDD_ROOK :
                            DragInfo.nPc = WHITE_ROOK ;
                            bP = 1 ;
                            break ;

                        case IDD_BISHOP :
                            DragInfo.nPc = WHITE_BISHOP ;
                            bP = 1 ;
                            break ;

                        case IDD_KNIGHT :
                            DragInfo.nPc = WHITE_KNIGHT ;
                            bP = 1 ;
                            break ;

                        case IDD_KING :
                            DragInfo.nPc = WHITE_KING ;
                            bP = 1 ;
                            break ;

                        default :
                            return 0 ;
                        }
                    }
                }
            }
        else
        if (DragInfo.nPc == BLACK_PAWN)
            {
            if (nBy == 7)
                {
                return 0 ;
                }

            if (nBy == 0)
                {
                if (DragInfo.ptFrom.y != 1)
                    {
                    return 0 ;
                    }

                if (User.bAutoQueen)
                    {
                    DragInfo.nPc = BLACK_QUEEN ;
                    bP = 1 ;
                    }
                else
                    {
                    GetWindowRect (hwnd, &rc) ;

                    nPpc = PROMOTE_Pawn (hwnd, Game [nG].hGame, rc.left + Game [nG].rBoard.left + (nX * Game [nG].nss) + 1, rc.top + Game [nG].rBoard.top + (nY * Game [nG].nss) + 1) ;
                    if ((! nPpc) || (DragInfo.nPc == EMPTY_SQUARE))
                        {
                        return 0 ;
                        }

                    switch (nPpc)
                        {
                        case IDD_QUEEN :
                            DragInfo.nPc = BLACK_QUEEN ;
                            bP = 1 ;
                            break ;

                        case IDD_ROOK :
                            DragInfo.nPc = BLACK_ROOK ;
                            bP = 1 ;
                            break ;

                        case IDD_BISHOP :
                            DragInfo.nPc = BLACK_BISHOP ;
                            bP = 1 ;
                            break ;

                        case IDD_KNIGHT :
                            DragInfo.nPc = BLACK_KNIGHT ;
                            bP = 1 ;
                            break ;

                        case IDD_KING :
                            DragInfo.nPc = BLACK_KING ;
                            bP = 1 ;
                            break ;

                        default :
                            return 0 ;
                        }
                    }
                }
            }
        }
    else
        {
        if (DragInfo.nPc == WHITE_PAWN || DragInfo.nPc == BLACK_PAWN)
            {
            if (nBy == 0 || nBy == 7)
                {
                return 0 ;
                }
            }

        //if (Game [nG].nBoard [nBx] [nBy] != EMPTY_SQUARE)
        //    {
        //    return 0 ;
        //    }
        }

    if (DragInfo.nPc == EMPTY_SQUARE)
        {
        return 0 ;
        }
    else
        {
        // make move
        nPpc = DragInfo.nPc ;

        if (DragInfo.bFromBoard)
            {
            if (User.bTestLegalPartner)
                {
                if (bP)
                    {
                    if (CHESS_IsLegalPartnerBoardMove (nG,
                                                       Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y],
                                                       DragInfo.ptFrom.x,
                                                       DragInfo.ptFrom.y,
                                                       nBx,
                                                       nBy,
                                                       nPpc))
                        {
                        if (Game [INDEX_PLAY].bIPlayWhite)
                            {
                            if (BOARD_IsBlackPiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }
                        else
                            {
                            if (BOARD_IsWhitePiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }

                        if (User.bLongPartner)
                            {
                            if (strlen (cTmp1) == 0)
                                {
                                sprintf (cTmp, "ptell %s\n", CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
                                }
                            else
                                {
                                sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
                                }
                            }
                        else
                            {
                            if (strlen (cTmp1) == 0)
                                {
                                sprintf (cTmp, "ptell %s\n", CHESS_GetNotation (nG, Moves.nLastMatch)) ;
                                }
                            else
                                {
                                sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetNotation (nG, Moves.nLastMatch)) ;
                                }
                            }
                        }
                    else
                        {
                        if (Game [INDEX_PLAY].bIPlayWhite)
                            {
                            if (BOARD_IsBlackPiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }
                        else
                            {
                            if (BOARD_IsWhitePiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }

                        if (strlen (cTmp1) == 0)
                            {
                            sprintf (cTmp, "ptell %c%d%c%d=%c\n",
                                     DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                     nBx + 'a', nBy + 1, ICSPiece [nPpc]) ;
                            }
                        else
                            {
                            sprintf (cTmp, "ptell %s %c%d%c%d=%c\n", cTmp1,
                                     DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                     nBx + 'a', nBy + 1, ICSPiece [nPpc]) ;
                            }
                        }
                    }
                else
                    {
                    if (CHESS_IsLegalPartnerBoardMove (nG,
                                                       nPpc,
                                                       DragInfo.ptFrom.x,
                                                       DragInfo.ptFrom.y,
                                                       nBx,
                                                       nBy,
                                                       EMPTY_SQUARE))
                        {
                        if (Game [INDEX_PLAY].bIPlayWhite)
                            {
                            if (BOARD_IsBlackPiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }
                        else
                            {
                            if (BOARD_IsWhitePiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }

                        if (User.bLongPartner)
                            {
                            if (strlen (cTmp1) == 0)
                                {
                                sprintf (cTmp, "ptell %s\n", CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
                                }
                            else
                                {
                                sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
                                }
                            }
                        else
                            {
                            if (strlen (cTmp1) == 0)
                                {
                                sprintf (cTmp, "ptell %s\n", CHESS_GetNotation (nG, Moves.nLastMatch)) ;
                                }
                            else
                                {
                                sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetNotation (nG, Moves.nLastMatch)) ;
                                }
                            }
                        }
                    else
                        {
                        if (Game [INDEX_PLAY].bIPlayWhite)
                            {
                            if (BOARD_IsBlackPiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }
                        else
                            {
                            if (BOARD_IsWhitePiece (nPpc))
                                {
                                strcpy (cTmp1, User.cPartnerMove1) ;
                                }
                            else
                                {
                                strcpy (cTmp1, User.cPartnerMove3) ;
                                }
                            }

                        if (strlen (cTmp1) == 0)
                            {
                            sprintf (cTmp, "ptell %c%d%c%d\n",
                                     DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                     nBx + 'a', nBy + 1) ;
                            }
                        else
                            {
                            sprintf (cTmp, "ptell %s %c%d%c%d\n", cTmp1,
                                     DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                     nBx + 'a', nBy + 1) ;
                            }
                        }
                    }
                }
            else
                {
                if (Game [INDEX_PLAY].bIPlayWhite)
                    {
                    if (BOARD_IsBlackPiece (nPpc))
                        {
                        strcpy (cTmp1, User.cPartnerMove1) ;
                        }
                    else
                        {
                        strcpy (cTmp1, User.cPartnerMove3) ;
                        }
                    }
                else
                    {
                    if (BOARD_IsWhitePiece (nPpc))
                        {
                        strcpy (cTmp1, User.cPartnerMove1) ;
                        }
                    else
                        {
                        strcpy (cTmp1, User.cPartnerMove3) ;
                        }
                    }

                if (bP)
                    {
                    if (strlen (cTmp1) == 0)
                        {
                        sprintf (cTmp, "ptell %c%d%c%d=%c\n",
                                 DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                 nBx + 'a', nBy + 1, ICSPiece [nPpc]) ;
                        }
                    else
                        {
                        sprintf (cTmp, "ptell %s %c%d%c%d=%c\n", cTmp1,
                                 DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                 nBx + 'a', nBy + 1, ICSPiece [nPpc]) ;
                        }
                    }
                else
                    {
                    if (strlen (cTmp1) == 0)
                        {
                        sprintf (cTmp, "ptell %c%d%c%d\n",
                                 DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                 nBx + 'a', nBy + 1) ;
                        }
                    else
                        {
                        sprintf (cTmp, "ptell %s %c%d%c%d\n", cTmp1,
                                 DragInfo.ptFrom.x + 'a', DragInfo.ptFrom.y + 1,
                                 nBx + 'a', nBy + 1) ;
                        }
                    }
                }
            }
        else
            {
            //if (User.bTestLegalPartner)
            //    {
            //    if (! CHESS_IsLegalPartnerBoardMove (nG,
            //                                         nPpc,
            //                                         -1,
            //                                         -1,
            //                                         nBx,
            //                                         nBy,
            //                                         EMPTY_SQUARE))
            //        {
            //        return 0 ;
            //        }
            //
            //    if (Game [INDEX_PLAY].bIPlayWhite)
            //        {
            //        if (BOARD_IsBlackPiece (nPpc))
            //            {
            //            strcpy (cTmp1, User.cPartnerMove1) ;
            //            }
            //        else
            //            {
            //            strcpy (cTmp1, User.cPartnerMove3) ;
            //            }
            //        }
            //    else
            //        {
            //        if (BOARD_IsWhitePiece (nPpc))
            //            {
            //            strcpy (cTmp1, User.cPartnerMove1) ;
            //            }
            //        else
            //            {
            //            strcpy (cTmp1, User.cPartnerMove3) ;
            //            }
            //        }
            //
            //    if (User.bLongPartner)
            //        {
            //       if (strlen (cTmp1) == 0)
            //            {
            //            sprintf (cTmp, "ptell %s\n", CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
            //            }
            //        else
            //            {
            //            sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
            //            }
            //        }
            //    else
            //        {
            //        if (strlen (cTmp1) == 0)
            //            {
            //            sprintf (cTmp, "ptell %s\n", CHESS_GetNotation (nG, Moves.nLastMatch)) ;
            //            }
            //        else
            //            {
            //            sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetNotation (nG, Moves.nLastMatch)) ;
            //            }
            //        }
            //    }
            //else
            //    {
            //    if (Game [INDEX_PLAY].bIPlayWhite)
            //        {
            //        if (BOARD_IsBlackPiece (nPpc))
            //            {
            //            strcpy (cTmp1, User.cPartnerMove1) ;
            //            }
            //        else
            //            {
            //            strcpy (cTmp1, User.cPartnerMove3) ;
            //            }
            //        }
            //    else
            //        {
            //        if (BOARD_IsWhitePiece (nPpc))
            //            {
            //            strcpy (cTmp1, User.cPartnerMove1) ;
            //            }
            //        else
            //            {
            //            strcpy (cTmp1, User.cPartnerMove3) ;
            //            }
            //        }
            //
            //    if (strlen (cTmp1) == 0)
            //        {
            //        sprintf (cTmp, "ptell %c@%c%d\n", ICSPiece [nPpc], nBx + 'a', nBy + 1) ;
            //        }
            //    else
            //        {
            //        sprintf (cTmp, "ptell %s %c@%c%d\n", cTmp1, ICSPiece [nPpc], nBx + 'a', nBy + 1) ;
            //        }
            //    }

            if (Game [INDEX_PLAY].bIPlayWhite)
                {
                if (BOARD_IsBlackPiece (nPpc))
                    {
                    strcpy (cTmp1, User.cPartnerMove1) ;
                    }
                else
                    {
                    strcpy (cTmp1, User.cPartnerMove3) ;
                    }
                }
            else
                {
                if (BOARD_IsWhitePiece (nPpc))
                    {
                    strcpy (cTmp1, User.cPartnerMove1) ;
                    }
                else
                    {
                    strcpy (cTmp1, User.cPartnerMove3) ;
                    }
                }

            if (strlen (cTmp1) == 0)
                {
                sprintf (cTmp, "ptell %c@%c%d\n", ICSPiece [nPpc], nBx + 'a', nBy + 1) ;
                }
            else
                {
                sprintf (cTmp, "ptell %s %c@%c%d\n", cTmp1, ICSPiece [nPpc], nBx + 'a', nBy + 1) ;
                }
            }

        TOOLBOX_WriteICS  (cTmp) ;
        TOOLBOX_WriteUser (cTmp) ;

        BOARD_RestoreDragMove (nG, hdc) ;
        }

    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    return 1 ;
    }

void BOARD_GetHighlightFromLastMove (int nG, char *cS, char *cL)
    {
    BOARD_NullHighlight (nG) ;

    // initial position
    if (Game [nG].bInitialMove)
        {
        return ;
        }

    // king is in check get legal king square
    if (User.bShowKingHighlight)
        {
        if (nG == INDEX_PLAY)
            {
            if (Game [nG].bPlaying)
                {
                if (Game [nG].bIPlayWhite)
                    {
                    if (User.nShowKingHighlightPM == 1)
                        {
                        CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                        }
                    else
                    if (User.nShowKingHighlightPM == 2)
                        {
                        if (Game [nG].bWhitesMove)
                            {
                            if (strchr (cL, ICS_KING_IN_CHECK_SYMBOL))
                                {
                                CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                                }
                            }
                        }

                    if (User.nShowKingHighlightPO == 1)
                        {
                        CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                        }
                    else
                    if (User.nShowKingHighlightPO == 2)
                        {
                        if (! Game [nG].bWhitesMove)
                            {
                            if (strchr (cL, ICS_KING_IN_CHECK_SYMBOL))
                                {
                                CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                                }
                            }
                        }
                    }
                else
                    {
                    if (User.nShowKingHighlightPM == 1)
                        {
                        CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                        }
                    else
                    if (User.nShowKingHighlightPM == 2)
                        {
                        if (! Game [nG].bWhitesMove)
                            {
                            if (strchr (cL, ICS_KING_IN_CHECK_SYMBOL))
                                {
                                CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                                }
                            }
                        }

                    if (User.nShowKingHighlightPO == 1)
                        {
                        CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                        }
                    else
                    if (User.nShowKingHighlightPO == 2)
                        {
                        if (Game [nG].bWhitesMove)
                            {
                            if (strchr (cL, ICS_KING_IN_CHECK_SYMBOL))
                                {
                                CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                                }
                            }
                        }
                    }
                }
            else
                {
                if (User.nShowKingHighlightE == 1)
                    {
                    CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                    CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                    }
                else
                if (User.nShowKingHighlightE == 2)
                    {
                    if (strchr (cL, ICS_KING_IN_CHECK_SYMBOL))
                        {
                        if (Game [nG].bWhitesMove)
                            {
                            CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                            }
                        else
                            {
                            CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                            }
                        }
                    }
                }
            }
        else
            {
            if (User.nShowKingHighlightO == 1)
                {
                CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                }
            else
            if (User.nShowKingHighlightO == 2)
                {
                if (strchr (cL, ICS_KING_IN_CHECK_SYMBOL))
                    {
                    if (Game [nG].bWhitesMove)
                        {
                        CHESS_GenKingLegalSquare (nG, INDEX_WHITE) ;
                        }
                    else
                        {
                        CHESS_GenKingLegalSquare (nG, INDEX_BLACK) ;
                        }
                    }
                }
            }
        }

    // o-o
    if (stricmp (cS, ICS_CASTLE_KING_SIDE_MOVE) == 0)
        {
        Game [nG].ptHighlight [0].x = 4 ;
        Game [nG].ptHighlight [1].x = 6 ;

        if (Game [nG].bWhitesMove)
            {
            Game [nG].ptHighlight [0].y = 7 ;
            Game [nG].ptHighlight [1].y = 7 ;
            }
        else
            {
            Game [nG].ptHighlight [0].y = 0 ;
            Game [nG].ptHighlight [1].y = 0 ;
            }
        return ;
        }

    // o-o-o
    if (stricmp (cS, ICS_CASTLE_QUEEN_SIDE_MOVE) == 0)
        {
        Game [nG].ptHighlight [0].x = 4 ;
        Game [nG].ptHighlight [1].x = 2 ;

        if (Game [nG].bWhitesMove)
            {
            Game [nG].ptHighlight [0].y = 7 ;
            Game [nG].ptHighlight [1].y = 7 ;
            }
        else
            {
            Game [nG].ptHighlight [0].y = 0 ;
            Game [nG].ptHighlight [1].y = 0 ;
            }
        return ;
        }

    // drop move?
    if (strchr (cS, '@'))
        {
        switch (Login.nLoginType)
            {
            case SERVER_FICS :
                Game [nG].ptHighlight [1].x = (int) (cS [5] - 'a') ;
                Game [nG].ptHighlight [1].y = (int) (cS [6] - '1') ;
                break ;

            case SERVER_ICC :
                Game [nG].ptHighlight [1].x = (int) (cS [2] - 'a') ;
                Game [nG].ptHighlight [1].y = (int) (cS [3] - '1') ;
                break ;

            case SERVER_NONFICS :
                Game [nG].ptHighlight [1].x = (int) (cS [5] - 'a') ;
                Game [nG].ptHighlight [1].y = (int) (cS [6] - '1') ;
                break ;

            default :
                Game [nG].ptHighlight [1].x = (int) (cS [5] - 'a') ;
                Game [nG].ptHighlight [1].y = (int) (cS [6] - '1') ;
                break ;
            }
        return ;
        }

    // board move
    Game [nG].ptHighlight [0].x = (int) (cS [2] - 'a') ;
    Game [nG].ptHighlight [0].y = (int) (cS [3] - '1') ;

    Game [nG].ptHighlight [1].x = (int) (cS [5] - 'a') ;
    Game [nG].ptHighlight [1].y = (int) (cS [6] - '1') ;
    }

void BOARD_ResetTruePremove (void)
    {
    HDC hdc ;

    Premove.bLastTP        = 0 ;
    Premove.ptLastTP [0].x = -1 ;
    Premove.ptLastTP [0].y = -1 ;
    Premove.ptLastTP [1].x = -1 ;
    Premove.ptLastTP [1].y = -1 ;

    if (Premove.nPremoveCount == 0)
        {
        if (Premove.bIllegalTP)
            {
            hdc = GetDC (Game [INDEX_PLAY].hwnd) ;
            BOARD_DrawBoard (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_ERASE_ALL_TP) ;
            ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
            }
        }
    else
        {
        hdc = GetDC (Game [INDEX_PLAY].hwnd) ;
        BOARD_DrawBoard (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_ERASE_ALL_TP) ;
        ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
        Premove.nPremoveCount = 0 ;
        Premove.nPremoveHead  = 0 ;
        Premove.nPremoveTail  = 0 ;
        }

    Premove.bIllegalTP        = 0 ;
    Premove.ptIllegalTP [0].x = -1 ;
    Premove.ptIllegalTP [0].y = -1 ;
    Premove.ptIllegalTP [1].x = -1 ;
    Premove.ptIllegalTP [1].y = -1 ;
    }

void BOARD_MakeSmartMove (HDC hdc, int nFx, int nFy, int nTx, int nTy, int nPp)
    {
    char cTmp [50] ;
    int nPc ;

    if (nPp == EMPTY_SQUARE)
        {
        sprintf (cTmp, "%c%d%c%d\n", nFx + 'a', nFy + 1, nTx + 'a', nTy + 1) ;

        nPc = Game [INDEX_PLAY].nBoard [nFx] [nFy] ;
        }
    else
        {
        sprintf (cTmp, "%c%d%c%d=%c\n", nFx + 'a', nFy + 1, nTx + 'a', nTy + 1, ICSPiece [nPp]) ;

        nPc = nPp ;
        }

    TOOLBOX_WriteICS (cTmp) ;

    if (Game [INDEX_PLAY].nGameType == GAMETYPE_FICS_ATOMIC)
        {
        if (Game [INDEX_PLAY].nBoard [nTx] [nTy] == EMPTY_SQUARE)
            {
            Game [INDEX_PLAY].nBoard [nFx] [nFy] = EMPTY_SQUARE ;
            Game [INDEX_PLAY].nBoard [nTx] [nTy] = nPc ;
            }
        else
            {
            Game [INDEX_PLAY].nBoard [nFx] [nFy] = EMPTY_SQUARE ;
            CHESS_AtomicCapture (INDEX_PLAY, nTx, nTy) ;
            }
        }
    else
        {
        Game [INDEX_PLAY].nBoard [nFx] [nFy] = EMPTY_SQUARE ;
        Game [INDEX_PLAY].nBoard [nTx] [nTy] = nPc ;
        }

    if (Game [INDEX_PLAY].bWhitesMove)
        {
        if (nPc == WHITE_PAWN)
            {
            if (nTx == Game [INDEX_PLAY].nLastDoublePushFile)
                {
                Game [INDEX_PLAY].nLastDoublePushFile = -1 ;

                if (nTy == 5 && Game [INDEX_PLAY].nBoard [nTx] [nTy - 1] == BLACK_PAWN)
                    {
                    Game [INDEX_PLAY].nBoard [nTx] [nTy - 1] = EMPTY_SQUARE ;

                    if (Game [INDEX_PLAY].nGameType == GAMETYPE_FICS_ATOMIC)
                        {
                        CHESS_AtomicCapture (INDEX_PLAY, nTx, nTy) ;
                        }
                    }
                }
            else
                {
                Game [INDEX_PLAY].nLastDoublePushFile = -1 ;

                if (nFy == 1 && nTy == 3)
                    {
                    Game [INDEX_PLAY].nLastDoublePushFile = nTx ;
                    }
                }
            }
        else
        if (nPc == WHITE_KING)
            {
            if (Game [INDEX_PLAY].bCanCastleKingSide [INDEX_WHITE])
                {
                if (nFx                              == 4            &&
                    nFy                              == 0            &&
                    nTx                              == 6            &&
                    nTy                              == 0            &&
                    Game [INDEX_PLAY].nBoard [5] [0] == EMPTY_SQUARE &&
                    Game [INDEX_PLAY].nBoard [7] [0] == WHITE_ROOK)
                    {
                    Game [INDEX_PLAY].nBoard [5] [0] = WHITE_ROOK ;
                    Game [INDEX_PLAY].nBoard [7] [0] = EMPTY_SQUARE ;
                    }
                }

            if (Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_WHITE])
                {
                if (nFx                              == 4            &&
                    nFy                              == 0            &&
                    nTx                              == 2            &&
                    nTy                              == 0            &&
                    Game [INDEX_PLAY].nBoard [0] [0] == WHITE_ROOK   &&
                    Game [INDEX_PLAY].nBoard [1] [0] == EMPTY_SQUARE &&
                    Game [INDEX_PLAY].nBoard [3] [0] == EMPTY_SQUARE)
                    {
                    Game [INDEX_PLAY].nBoard [3] [0] = WHITE_ROOK ;
                    Game [INDEX_PLAY].nBoard [0] [0] = EMPTY_SQUARE ;
                    }
                }
            }
        }
    else
        {
        if (nPc == BLACK_PAWN)
            {
            if (nTx == Game [INDEX_PLAY].nLastDoublePushFile)
                {
                Game [INDEX_PLAY].nLastDoublePushFile = -1 ;

                if (nTy == 2 && Game [INDEX_PLAY].nBoard [nTx] [nTy + 1] == WHITE_PAWN)
                    {
                    Game [INDEX_PLAY].nBoard [nTx] [nTy + 1] = EMPTY_SQUARE ;

                    if (Game [INDEX_PLAY].nGameType == GAMETYPE_FICS_ATOMIC)
                        {
                        CHESS_AtomicCapture (INDEX_PLAY, nTx, nTy) ;
                        }
                    }
                }
            else
                {
                Game [INDEX_PLAY].nLastDoublePushFile = -1 ;

                if (nFy == 6 && nTy == 4)
                    {
                    Game [INDEX_PLAY].nLastDoublePushFile = nTx ;
                    }
                }
            }
        else
        if (nPc == BLACK_KING)
            {
            if (Game [INDEX_PLAY].bCanCastleKingSide [INDEX_BLACK])
                {
                if (nFx                              == 4            &&
                    nFy                              == 7            &&
                    nTx                              == 6            &&
                    nTy                              == 7            &&
                    Game [INDEX_PLAY].nBoard [5] [7] == EMPTY_SQUARE &&
                    Game [INDEX_PLAY].nBoard [7] [7] == BLACK_ROOK)
                    {
                    Game [INDEX_PLAY].nBoard [5] [7] = BLACK_ROOK ;
                    Game [INDEX_PLAY].nBoard [7] [7] = EMPTY_SQUARE ;
                    }
                }

            if (Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_BLACK])
                {
                if (nFx                              == 4            &&
                    nFy                              == 7            &&
                    nTx                              == 2            &&
                    nTy                              == 7            &&
                    Game [INDEX_PLAY].nBoard [0] [7] == BLACK_ROOK   &&
                    Game [INDEX_PLAY].nBoard [1] [7] == EMPTY_SQUARE &&
                    Game [INDEX_PLAY].nBoard [3] [7] == EMPTY_SQUARE)
                    {
                    Game [INDEX_PLAY].nBoard [3] [7] = BLACK_ROOK ;
                    Game [INDEX_PLAY].nBoard [0] [7] = EMPTY_SQUARE ;
                    }
                }
            }
        }

    Game [INDEX_PLAY].bWhitesMove = (! Game [INDEX_PLAY].bWhitesMove) ;

    if (Game [INDEX_PLAY].bPlaying)
        {
        Game [INDEX_PLAY].bOnLagClock = 1 ;
        }
    else
    if (Game [INDEX_PLAY].nGameNumber == 0)
        {
        BOARD_DrawWhiteClock (INDEX_PLAY, hdc) ;
        BOARD_DrawBlackClock (INDEX_PLAY, hdc) ;
        BOARD_FindKings (INDEX_PLAY) ;
        }

    CLOCK_SwitchClocks (INDEX_PLAY) ;

    BOARD_DrawWhiteClock (INDEX_PLAY, hdc) ;
    BOARD_DrawBlackClock (INDEX_PLAY, hdc) ;

    BOARD_DrawBoard (INDEX_PLAY, hwndWindow [HWND_PLAY], hdc, DRAW_STATE_MOUSE_UP) ;

    System.bIsMyTurn = 0 ;

    F8KEY_Init () ;
    TOOLBOX_ResetPromotKnight () ;

    GAMESOUND_Play (GAME_SOUND_PLAY_MOVE_SMART) ;
    }

int BOARD_RightMouseDropPiece (int nBx, int nBy, int nPc)
    {
    HDC hdc ;
    char cTmp [150] ;

    if (nPc == EMPTY_SQUARE)
        {
        F8KEY_Init () ;
        TOOLBOX_ResetPromotKnight () ;
        return 0 ;
        }

    if (nPc == WHITE_PAWN || nPc == BLACK_PAWN)
        {
        if (nBy == 0 || nBy == 7)
            {
            F8KEY_Init () ;
            TOOLBOX_ResetPromotKnight () ;
            return 0 ;
            }
        }

    if (Game [INDEX_PLAY].nGameNumber > 0)
        {
        if (Game [INDEX_PLAY].bPlaying)
            {
            if (Game [INDEX_PLAY].nBuffer [nPc] == 0)
                {
                switch (nPc)
                    {
                    case WHITE_PAWN :
                    case BLACK_PAWN :
                        sprintf (F8Key.cSitPieceDropMove, "%c@%c%d", ICS_WHITE_PAWN,   nBx + 'a', nBy + 1) ;
                        break ;

                    case WHITE_KNIGHT :
                    case BLACK_KNIGHT :
                        sprintf (F8Key.cSitPieceDropMove, "%c@%c%d", ICS_WHITE_KNIGHT, nBx + 'a', nBy + 1) ;
                        break ;

                    case WHITE_BISHOP :
                    case BLACK_BISHOP :
                        sprintf (F8Key.cSitPieceDropMove, "%c@%c%d", ICS_WHITE_BISHOP, nBx + 'a', nBy + 1) ;
                        break ;

                    case WHITE_ROOK :
                    case BLACK_ROOK :
                        sprintf (F8Key.cSitPieceDropMove, "%c@%c%d", ICS_WHITE_ROOK,   nBx + 'a', nBy + 1) ;
                        break ;

                    case WHITE_QUEEN :
                    case BLACK_QUEEN :
                        sprintf (F8Key.cSitPieceDropMove, "%c@%c%d", ICS_WHITE_QUEEN,  nBx + 'a', nBy + 1) ;
                        break ;

                    default :
                        F8KEY_Init () ;
                        TOOLBOX_ResetPromotKnight () ;
                        return 0 ;
                    }

                sprintf (cTmp, "Sit For Piece Drop Move Set to %s\n", F8Key.cSitPieceDropMove) ;
                TOOLBOX_WriteSystem (cTmp) ;
                return 1 ;
                }
            }
        }

    if (System.bIsMyTurn || (! Game [INDEX_PLAY].bPlaying))
        {
        if (Game [INDEX_PLAY].nBoard [nBx] [nBy] != EMPTY_SQUARE)
            {
            F8KEY_Init () ;
            TOOLBOX_ResetPromotKnight () ;
            return 0 ;
            }
        }

    // make move
    if (System.bIsMyTurn)
        {
        if (User.bTestLegalPlay)
            {
            if (! CHESS_IsLegalMove (INDEX_PLAY,
                                     nPc,
                                     -1,
                                     -1,
                                     nBx,
                                     nBy,
                                     EMPTY_SQUARE))
                {
                F8KEY_Init () ;
                TOOLBOX_ResetPromotKnight () ;
                return 0 ;
                }
            }
        }
    else
        {
        if (Game [INDEX_PLAY].bPlaying)
            {
            if (! CHESS_IsLegalTruePremove (INDEX_PLAY,
                                            nPc,
                                            -1,
                                            -1,
                                            nBx,
                                            nBy,
                                            EMPTY_SQUARE))
                {
                F8KEY_Init () ;
                TOOLBOX_ResetPromotKnight () ;
                return 0 ;
                }
            }
        else
            {
            if (! CHESS_IsLegalExamDropMove (INDEX_PLAY,
                                             nPc,
                                             nBx,
                                             nBy))
                {
                F8KEY_Init () ;
                TOOLBOX_ResetPromotKnight () ;
                return 0 ;
                }
            }
        }

    hdc = GetDC (hwndWindow [HWND_PLAY]) ;

    sprintf (cTmp, "%c@%c%d\n", ICSPiece [nPc], nBx + 'a', nBy + 1) ;

    if (System.bIsMyTurn)
        {
        TOOLBOX_WriteICS (cTmp) ;

        Game [INDEX_PLAY].nBoard [nBx] [nBy] = nPc ;
        --Game [INDEX_PLAY].nBuffer [nPc] ;
        }
    else
        {
        if (User.bTruePremove && Game [INDEX_PLAY].bPlaying)
            {
            if (Premove.nPremoveCount >= MAX_TRUE_PREMOVE)
                {
                Premove.nPremoveTail = Premove.nPremoveTail + 1 ;

                if (Premove.nPremoveTail >= MAX_TRUE_PREMOVE)
                    {
                    Premove.nPremoveTail = 0 ;
                    }

                Premove.bLastTP        = 1 ;
                Premove.ptLastTP [0].x = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [3] ;
                Premove.ptLastTP [0].y = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [4] ;
                Premove.ptLastTP [1].x = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [5] ;
                Premove.ptLastTP [1].y = Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [6] ;
                }

            strcpy (Premove.cPremoveBuffer [Premove.nPremoveHead], cTmp) ;

            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [1] = INDEX_PLAY ;
            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [2] = nPc ;
            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [3] = -1 ;
            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [4] = -1 ;
            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [5] = nBx ;
            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [6] = nBy ;
            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [7] = EMPTY_SQUARE ;
            Premove.nPremoveLegalBuffer [Premove.nPremoveHead] [8] = EMPTY_SQUARE ;

            if (Premove.nPremoveCount < MAX_TRUE_PREMOVE)
                {
                Premove.nPremoveCount = Premove.nPremoveCount + 1 ;
                }

            Premove.nPremoveHead = Premove.nPremoveHead + 1 ;

            if (Premove.nPremoveHead >= MAX_TRUE_PREMOVE)
                {
                Premove.nPremoveHead = 0 ;
                }

            BOARD_RestoreDragMove (INDEX_PLAY, hdc) ;

            GAMESOUND_Play (GAME_SOUND_PLAY_MOVE_TP) ;

            ReleaseDC (hwndWindow [HWND_PLAY], hdc) ;

            F8KEY_Init () ;
            TOOLBOX_ResetPromotKnight () ;
            return 1 ;
            }
        else
            {
            if (Game [INDEX_PLAY].nGameNumber == 0)
                {
                Game [INDEX_PLAY].ptHighlight [0].x = -1 ;
                Game [INDEX_PLAY].ptHighlight [0].y = -1 ;

                Game [INDEX_PLAY].ptHighlight [1].x = nBx ;
                Game [INDEX_PLAY].ptHighlight [1].y = nBy ;
                }
            else
                {
                TOOLBOX_WriteICS (cTmp) ;
                }

            Game [INDEX_PLAY].nBoard [nBx] [nBy] = nPc ;
            --Game [INDEX_PLAY].nBuffer [nPc] ;
            }
        }

    Game [INDEX_PLAY].bWhitesMove = (! Game [INDEX_PLAY].bWhitesMove) ;

    if (Game [INDEX_PLAY].bPlaying)
        {
        Game [INDEX_PLAY].bOnLagClock = 1 ;
        }
    else
    if (Game [INDEX_PLAY].nGameNumber == 0)
        {
        BOARD_DrawWhiteClock (INDEX_PLAY, hdc) ;
        BOARD_DrawBlackClock (INDEX_PLAY, hdc) ;
        BOARD_FindKings (INDEX_PLAY) ;

        GAMESOUND_Play (GAME_SOUND_PLAY_MOVE) ;
        }

    CLOCK_SwitchClocks (INDEX_PLAY) ;

    BOARD_DrawWhiteClock (INDEX_PLAY, hdc) ;
    BOARD_DrawBlackClock (INDEX_PLAY, hdc) ;

    BOARD_DrawBoard (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_MOUSE_UP) ;

    ReleaseDC (hwndWindow [HWND_PLAY], hdc) ;

    System.bIsMyTurn = 0 ;

    F8KEY_Init () ;
    TOOLBOX_ResetPromotKnight () ;
    return 1 ;
    }

int BOARD_RightMouseDropPiece1 (int nG, int nBx, int nBy, int nPc)
    {
    char cTmp [255], cTmp1 [255] ;

    if (nG == INDEX_PLAY)
        {
        return 0 ;
        }

    if (nPc == EMPTY_SQUARE)
        {
        return 0 ;
        }

    if (nPc == WHITE_PAWN || nPc == BLACK_PAWN)
        {
        if (nBy == 0 || nBy == 7)
            {
            return 0 ;
            }
        }

    //if (Game [nG].nBoard [nBx] [nBy] != EMPTY_SQUARE)
    //    {
    //    return 0 ;
    //    }

    //if (User.bTestLegalPartner)
    //    {
    //    if (Game [nG].nBuffer [nPc] > 0)
    //        {
    //        if (! CHESS_IsLegalMove (nG,
    //                                 nPc,
    //                                 -1,
    //                                 -1,
    //                                 nBx,
    //                                 nBy,
    //                                 EMPTY_SQUARE))
    //            {
    //            return 0 ;
    //            }
    //        }
    //
    //    if (Game [INDEX_PLAY].bIPlayWhite)
    //        {
    //        if (BOARD_IsBlackPiece (nPc))
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove1) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove2) ;
    //                }
    //            }
    //        else
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove3) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove4) ;
    //                }
    //            }
    //        }
    //    else
    //        {
    //        if (BOARD_IsWhitePiece (nPc))
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove1) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove2) ;
    //                }
    //            }
    //        else
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove3) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove4) ;
    //                }
    //            }
    //        }
    //
    //    if (Game [nG].nBuffer [nPc] > 0)
    //        {
    //        if (User.bLongPartner)
    //            {
    //            if (strlen (cTmp1) == 0)
    //                {
    //                sprintf (cTmp, "ptell %s\n", CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
    //                }
    //            else
    //                {
    //                sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetLongNotation (nG, Moves.nLastMatch)) ;
    //                }
    //            }
    //        else
    //            {
    //            if (strlen (cTmp1) == 0)
    //                {
    //                sprintf (cTmp, "ptell %s\n", CHESS_GetNotation (nG, Moves.nLastMatch)) ;
    //                }
    //            else
    //                {
    //                sprintf (cTmp, "ptell %s %s\n", cTmp1, CHESS_GetNotation (nG, Moves.nLastMatch)) ;
    //                }
    //            }
    //        }
    //    else
    //        {
    //        if (strlen (cTmp1) == 0)
    //            {
    //            sprintf (cTmp, "ptell %c@%c%d\n", ICSPiece [nPc], nBx + 'a', nBy + 1) ;
    //            }
    //        else
    //            {
    //            sprintf (cTmp, "ptell %s %c@%c%d\n", cTmp1, ICSPiece [nPc], nBx + 'a', nBy + 1) ;
    //            }
    //        }
    //    }
    //else
    //    {
    //    if (Game [INDEX_PLAY].bIPlayWhite)
    //        {
    //        if (BOARD_IsBlackPiece (nPc))
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove1) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove2) ;
    //                }
    //            }
    //        else
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove3) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove4) ;
    //                }
    //            }
    //        }
    //    else
    //        {
    //        if (BOARD_IsWhitePiece (nPc))
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove1) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove2) ;
    //                }
    //            }
    //        else
    //            {
    //            if (Game [nG].nBuffer [nPc] > 0)
    //                {
    //                strcpy (cTmp1, User.cPartnerMove3) ;
    //                }
    //            else
    //                {
    //                strcpy (cTmp1, User.cPartnerMove4) ;
    //                }
    //            }
    //        }
    //
    //    if (strlen (cTmp1) == 0)
    //        {
    //        sprintf (cTmp, "ptell %c@%c%d\n", ICSPiece [nPc], nBx + 'a', nBy + 1) ;
    //        }
    //    else
    //        {
    //        sprintf (cTmp, "ptell %s %c@%c%d\n", cTmp1, ICSPiece [nPc], nBx + 'a', nBy + 1) ;
    //        }
    //    }

    if (Game [INDEX_PLAY].bIPlayWhite)
        {
        if (BOARD_IsBlackPiece (nPc))
            {
            if (Game [nG].nBuffer [nPc] > 0)
                {
                strcpy (cTmp1, User.cPartnerMove1) ;
                }
            else
                {
                strcpy (cTmp1, User.cPartnerMove2) ;
                }
            }
        else
            {
            if (Game [nG].nBuffer [nPc] > 0)
                {
                strcpy (cTmp1, User.cPartnerMove3) ;
                }
            else
                {
                strcpy (cTmp1, User.cPartnerMove4) ;
                }
            }
        }
    else
        {
        if (BOARD_IsWhitePiece (nPc))
            {
            if (Game [nG].nBuffer [nPc] > 0)
                {
                strcpy (cTmp1, User.cPartnerMove1) ;
                }
            else
                {
                strcpy (cTmp1, User.cPartnerMove2) ;
                }
            }
        else
            {
            if (Game [nG].nBuffer [nPc] > 0)
                {
                strcpy (cTmp1, User.cPartnerMove3) ;
                }
            else
                {
                strcpy (cTmp1, User.cPartnerMove4) ;
                }
            }
        }

    if (strlen (cTmp1) == 0)
        {
        sprintf (cTmp, "ptell %c@%c%d\n", ICSPiece [nPc], nBx + 'a', nBy + 1) ;
        }
    else
        {
        sprintf (cTmp, "ptell %s %c@%c%d\n", cTmp1, ICSPiece [nPc], nBx + 'a', nBy + 1) ;
        }

    TOOLBOX_WriteICS  (cTmp) ;
    TOOLBOX_WriteUser (cTmp) ;

    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    return 1 ;
    }
