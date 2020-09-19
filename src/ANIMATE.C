#include "thief.h"

#define ANIMATE_FACTOR  4

void ANIMATE_Init (void)
    {
    AnimateInfo.bFromBoard  = 0 ;
    AnimateInfo.ptFrom.x    = -1 ;
    AnimateInfo.ptFrom.y    = -1 ;
    AnimateInfo.ptCurrent.x = -1 ;
    AnimateInfo.ptCurrent.y = -1 ;
    AnimateInfo.ptLast.x    = -1 ;
    AnimateInfo.ptLast.y    = -1 ;
    AnimateInfo.nIndex      = -1 ;
    AnimateInfo.nPc         = EMPTY_SQUARE ;
    }

void ANIMATE_Tween (POINT *ptStart, POINT *ptMid, POINT *ptFinish, int nFactor, POINT *ptFrames, int *nFrames)
    {
    int nI, nFraction = 1, nCount = 0 ;

    // slow in, stepping 1/16th, then 1/8th, ...
    for (nI = 0; nI < nFactor ; nI++)
        {
        nFraction = nFraction *  2 ;
        }

    for (nI = 0; nI < nFactor ; nI++)
        {
        ptFrames [nCount].x = ptStart->x + (ptMid->x - ptStart->x) / nFraction ;
        ptFrames [nCount].y = ptStart->y + (ptMid->y - ptStart->y) / nFraction ;

        nCount    = nCount + 1 ;
        nFraction = nFraction / 2 ;
        }
  
    // middle point
    ptFrames [nCount] = *ptMid ;
    nCount = nCount + 1 ;
  
    // slow out, stepping 1/2, then 1/4, ...
    nFraction = 2 ;
    for (nI = 0; nI < nFactor ; nI++)
        {
        ptFrames[nCount].x = ptFinish->x - (ptFinish->x - ptMid->x) / nFraction ;
        ptFrames[nCount].y = ptFinish->y - (ptFinish->y - ptMid->y) / nFraction ;
        nCount    = nCount + 1 ;
        nFraction = nFraction * 2 ;
        }

    // assign frame count
    *nFrames = nCount ;
    }

int ANIMATE_AnimateMove (int nG, HWND hwnd, HDC hdc, int nPc, int nFromX, int nFromY, int nToX, int nToY, int nSpeed)
    {
    POINT ptStart, ptFinish, ptMid ;
    POINT ptFrames [30] ;
    int bBoardMove, nPx, nPy, nFrames, nI, nSleep ;

    // reset animate info
    ANIMATE_Init () ;

    // missing piece
    if (nPc == EMPTY_SQUARE)
        {
        return 0 ;
        }

    // invalid destination square
    if ((nToX < 0) || (nToX > 7) || (nToY < 0) || (nToY > 7))
        {
        return 0 ;
        }

    // determine if it's board move
    bBoardMove = ((nFromX >= 0) && (nFromY >= 0)) ;

    if (bBoardMove)
        {
        // board move

        // invalid source square
        if ((nFromX < 0) || (nFromX > 7) || (nFromY < 0) || (nFromY > 7))
            {
            return 0 ;
            }

        // source square must contain the animate piece
        if (Game [nG].nBoard [nFromX] [nFromY] != nPc)
            {
            return 0 ;
            }

        // check to make sure the piece does not capture its own piece
        if (BOARD_IsWhitePiece (nPc))
            {
            if (BOARD_IsWhitePiece (Game [nG].nBoard [nToX] [nToY]))
                {
                return 0 ;
                }
            }
        else
            {
            if (BOARD_IsBlackPiece (Game [nG].nBoard [nToX] [nToY]))
                {
                return 0 ;
                }
            }

        // assign start
        BOARD_SquareToPosition (nG, nFromX, nFromY, &nPx, &nPy) ;
        ptStart.x = nPx ;
        ptStart.y = nPy ;

        // assign animate board move
        AnimateInfo.bFromBoard = 1 ;
        AnimateInfo.ptFrom.x   = nFromX ;
        AnimateInfo.ptFrom.y   = nFromY ;
        }
    else
        {
        // drop move

        // invalid drop piece
        if ((nPc < WHITE_PAWN) || (nPc > BLACK_QUEEN))
            {
            return 0 ;
            }

        // make sure if the drop move is a pawn it can't go on first or eighth rank
        if ((nPc == WHITE_PAWN) || (nPc == BLACK_PAWN))
            {
            if ((nToY == 1) || (nToY == 7))
                {
                return 0 ;
                }
            }

        // maks sure the drop move's destination square is empty
        if (Game [nG].nBoard [nToX] [nToY] != EMPTY_SQUARE)
            {
            return 0 ;
            }

        // assign start
        ptStart.x = Game [nG].rBuffer.left + Game [nG].ptBuffer [nPc].x ;
        ptStart.y = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nPc].y ;

        // assign animate drop move
        AnimateInfo.bFromBoard = 0 ;
        AnimateInfo.ptFrom.x   = -1 ;
        AnimateInfo.ptFrom.y   = -1 ;
        }

    // assign animate information
    AnimateInfo.nIndex = nG ;
    AnimateInfo.nPc    = nPc ;

    // assign finish
    BOARD_SquareToPosition (nG, nToX, nToY, &nPx, &nPy) ;
    ptFinish.x = nPx ;
    ptFinish.y = nPy ;

    if (bBoardMove)
        {
        // all pieces except knights move in straight line
        if ((nPc == WHITE_KNIGHT) || (nPc == BLACK_KNIGHT))
            {
            // knight: make diagonal movement then straight
            if (abs(nToY - nFromY) < abs(nToX - nFromX))
                {
                ptMid.x = ptStart.x + (ptFinish.x - ptStart.x) / 2 ;
                ptMid.y = ptFinish.y ;
                }
            else
                {
                ptMid.x = ptFinish.x ;
                ptMid.y = ptStart.y + (ptFinish.y - ptStart.y) / 2 ;
                }
            }
        else
            {
            ptMid.x = ptStart.x + (ptFinish.x - ptStart.x) / 2 ;
            ptMid.y = ptStart.y + (ptFinish.y - ptStart.y) / 2 ;
            }
        }
    else
        {
        ptMid.x = ptStart.x + (ptFinish.x - ptStart.x) / 2 ;
        ptMid.y = ptStart.y + (ptFinish.y - ptStart.y) / 2 ;
        }

    // don't use as many ptFrames for very short moves
    if ((! bBoardMove) || (abs (nToY - nFromY) + abs (nToX - nFromX) > 2))
        {
        ANIMATE_Tween (&ptStart, &ptMid, &ptFinish, ANIMATE_FACTOR, ptFrames, &nFrames) ;
        }
    else
        {
        ANIMATE_Tween (&ptStart, &ptMid, &ptFinish, ANIMATE_FACTOR - 1, ptFrames, &nFrames) ;
        }

    // determine sleep time
    nSleep = AnimateSpeed [nSpeed] ;

    // animate frames
    if (nG == INDEX_PLAY)
        {
        for (nI = 0; nI < nFrames ; nI++)
            {
            AnimateInfo.ptCurrent = ptFrames [nI] ;

            BOARD_DrawAnimateBoard (nG, hwnd, hdc) ;

            AnimateInfo.ptLast = AnimateInfo.ptCurrent ;
            Sleep (nSleep) ;
            }
        }
    else
        {
        for (nI = 0; nI < nFrames ; nI++)
            {
            AnimateInfo.ptCurrent = ptFrames [nI] ;

            BOARD_DrawAnimateBoard1 (nG, hwnd, hdc) ;

            AnimateInfo.ptLast = AnimateInfo.ptCurrent ;
            Sleep (nSleep) ;
            }
        }

    // animate last finish frame
    AnimateInfo.ptCurrent = ptFinish ;

    if (nG == INDEX_PLAY)
        {
        BOARD_DrawAnimateBoard (nG, hwnd, hdc) ;
        }
    else
        {
        BOARD_DrawAnimateBoard1 (nG, hwnd, hdc) ;
        }
    return 1 ;
    }

void ANIMATE_Move (int nG, HWND hwnd, HDC hdc, int nSpeed)
    {
    int nFromX, nFromY, nToX, nToY, nPc ;

    // check to make sure the Game [nG].hWindow is not NULL
    if (Game [nG].hWindow == NULL)
        {
        ANIMATE_Init () ;
        return ;
        }

    // initial position
    if (Game [nG].bInitialMove)
        {
        ANIMATE_Init () ;
        return ;
        }

    // o-o
    if (stricmp (Game [nG].cVerboseMove, ICS_CASTLE_KING_SIDE_MOVE) == 0)
        {
        if (Game [nG].bWhitesMove)
            {
            if (Game [nG].nBoard [4] [7] == BLACK_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [4] [7], 4, 7, 6, 7, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [7] [7], 7, 7, 5, 7, nSpeed))
                        {
                        }
                    }
                }
            else
            if (Game [nG].nBoard [3] [7] == BLACK_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [3] [7], 3, 7, 1, 7, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [0] [7], 0, 7, 2, 7, nSpeed))
                        {
                        }
                    }
                }
            }
        else
            {
            if (Game [nG].nBoard [4] [0] == WHITE_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [4] [0], 4, 0, 6, 0, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [7] [0], 7, 0, 5, 0, nSpeed))
                        {
                        }
                    }
                }
            else
            if (Game [nG].nBoard [3] [0] == WHITE_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [3] [0], 3, 0, 1, 0, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [0] [0], 0, 0, 2, 0, nSpeed))
                        {
                        }
                    }
                }
            }
        ANIMATE_Init () ;
        return ;
        }

    // o-o-o
    if (stricmp (Game [nG].cVerboseMove, ICS_CASTLE_QUEEN_SIDE_MOVE) == 0)
        {
        if (Game [nG].bWhitesMove)
            {
            if (Game [nG].nBoard [4] [7] == BLACK_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [4] [7], 4, 7, 2, 7, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [0] [7], 0, 7, 3, 7, nSpeed))
                        {
                        }
                    }
                }
            else
            if (Game [nG].nBoard [3] [7] == BLACK_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [3] [7], 3, 7, 5, 7, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [7] [7], 7, 7, 4, 7, nSpeed))
                        {
                        }
                    }
                }
            }
        else
            {
            if (Game [nG].nBoard [4] [0] == WHITE_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [4] [0], 4, 0, 2, 0, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [0] [0], 0, 0, 3, 0, nSpeed))
                        {
                        }
                    }
                }
            else
            if (Game [nG].nBoard [3] [0] == WHITE_KING)
                {
                if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [3] [0], 3, 0, 5, 0, nSpeed))
                    {
                    if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [7] [0], 7, 0, 4, 0, nSpeed))
                        {
                        }
                    }
                }
            }
        ANIMATE_Init () ;
        return ;
        }

    // board move
    if (! strchr (Game [nG].cVerboseMove, '@'))
        {
        nFromX = (int) (Game [nG].cVerboseMove [2] - 'a') ;
        nFromY = (int) (Game [nG].cVerboseMove [3] - '1') ;
        nToX   = (int) (Game [nG].cVerboseMove [5] - 'a') ;
        nToY   = (int) (Game [nG].cVerboseMove [6] - '1') ;

        if (ANIMATE_AnimateMove (nG, hwnd, hdc, Game [nG].nBoard [nFromX] [nFromY], nFromX, nFromY, nToX, nToY, nSpeed))
            {
            }

        ANIMATE_Init () ;
        return ;
        }

    // drop move
    switch (Login.nLoginType)
        {
        case SERVER_FICS :
            nToX = (int) (Game [nG].cVerboseMove [5] - 'a') ;
            nToY = (int) (Game [nG].cVerboseMove [6] - '1') ;
            break ;

        case SERVER_ICC :
            nToX = (int) (Game [nG].cVerboseMove [2] - 'a') ;
            nToY = (int) (Game [nG].cVerboseMove [3] - '1') ;
            break ;

        case SERVER_NONFICS :
            nToX = (int) (Game [nG].cVerboseMove [5] - 'a') ;
            nToY = (int) (Game [nG].cVerboseMove [6] - '1') ;
            break ;

        default :
            nToX = (int) (Game [nG].cVerboseMove [5] - 'a') ;
            nToY = (int) (Game [nG].cVerboseMove [6] - '1') ;
            break ;
        }

    switch (Game [nG].cVerboseMove [0])
        {
        case ICS_BLACK_PAWN :
        case ICS_WHITE_PAWN :
            nPc = (Game [nG].bWhitesMove ? BLACK_PAWN : WHITE_PAWN) ;
            break ;

        case ICS_WHITE_KNIGHT :
        case ICS_BLACK_KNIGHT :
            nPc = (Game [nG].bWhitesMove ? BLACK_KNIGHT : WHITE_KNIGHT) ;
            break ;

        case ICS_WHITE_BISHOP :
        case ICS_BLACK_BISHOP :
            nPc = (Game [nG].bWhitesMove ? BLACK_BISHOP : WHITE_BISHOP) ;
            break ;

        case ICS_WHITE_ROOK :
        case ICS_BLACK_ROOK :
            nPc = (Game [nG].bWhitesMove ? BLACK_ROOK : WHITE_ROOK) ;
            break ;

        case ICS_WHITE_QUEEN :
        case ICS_BLACK_QUEEN :
            nPc = (Game [nG].bWhitesMove ? BLACK_QUEEN : WHITE_QUEEN) ;
            break ;

        default :
            return ;
        }

    if (ANIMATE_AnimateMove (nG, hwnd, hdc, nPc, -1, -1, nToX, nToY, nSpeed))
        {
        }

    ANIMATE_Init () ;
    }
