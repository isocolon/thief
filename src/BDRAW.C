#include "thief.h"

#define BORG_NUM 200

POINT _ptBPawn   [BORG_NUM] ;
POINT _ptBRook   [BORG_NUM] ;
POINT _ptBKnight [BORG_NUM] ;
POINT _ptBBishop [BORG_NUM] ;
POINT _ptBQueen  [BORG_NUM] ;
POINT _ptBKing   [BORG_NUM] ;

int PAWN_NUMB   = BORG_NUM ;
int ROOK_NUMB   = BORG_NUM ;
int KNIGHT_NUMB = BORG_NUM ;
int BISHOP_NUMB = BORG_NUM ;
int QUEEN_NUMB  = BORG_NUM ;
int KING_NUMB   = BORG_NUM ;

int nbss      = ORG_SIZE ;
int porgsizeB = ORG_SIZE ;

POINT ptBPawn   [BORG_NUM] ;
POINT ptBRook   [BORG_NUM] ;
POINT ptBKnight [BORG_NUM] ;
POINT ptBBishop [BORG_NUM] ;
POINT ptBQueen  [BORG_NUM] ;
POINT ptBKing   [BORG_NUM] ;

void BDRAW_LoadPieces (void)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir [_MAX_DIR] ;
    char File [_MAX_FNAME] ;
    char Ext [_MAX_EXT] ;
    char Fn [_MAX_PATH] ;

    int nI, cox, coy ;
    FILE *fv ;

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    nI = strlen (Dir) ;
    if (nI == 0)
        {
        strcat (Dir, "\\") ;
        }
    else
        {
        if (Dir [nI - 1] == '\\')
            {
            strcat (Dir, "") ;
            }
        else
            {
            strcat (Dir, "\\") ;
            }
        }

    switch (BoardCfg.nPieceSet)
        {
        case VECTOR_TOUPS :
            SYS_CheckToups () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\TOUPS\\") ;
            break ;

        case VECTOR_NICOK :
            SYS_CheckNicok () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\NICOK\\") ;
            break ;

        case VECTOR_KBIRK :
            SYS_CheckKbirk () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\KBIRK\\") ;
            break ;

        case VECTOR_JHURT :
            SYS_CheckJhurt () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\JHURT\\") ;
            break ;

        case VECTOR_ECHUR :
            SYS_CheckEchur () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\ECHUR\\") ;
            break ;

        case VECTOR_DFONG :
            SYS_CheckDfong () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\DFONG\\") ;
            break ;

        case VECTOR_USER :
            SYS_CheckUser () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\USER\\") ;
            break ;

        default :
            BoardCfg.nPieceSet = VECTOR_TOUPS ;
            SYS_CheckToups () ;
            strcat (Dir, "BUGSTUFF\\PIECE\\VECTOR\\TOUPS\\") ;
            break ;
        }

    _makepath (Fn, Drive, Dir, "PAWN", ".INI") ;
    if ((fv = fopen (Fn, "r")) != NULL)
	{ 
        fscanf (fv, "%d", &porgsizeB) ;
        for (nI = 0 ; nI < BORG_NUM ; nI++)
            {
            if (fscanf (fv, "%d", &cox) == EOF)
                {
                PAWN_NUMB = nI ;
                break ;
                }
            if (fscanf (fv, ",%d", &coy) == EOF)
                {
                PAWN_NUMB = nI ;
                break ;
                }
            _ptBPawn [nI].x = cox ;
            _ptBPawn [nI].y = coy ;
            }
        fclose (fv) ;
        }

    _makepath (Fn, Drive, Dir, "ROOK", ".INI") ;
    if ((fv = fopen (Fn, "r")) != NULL)
	{ 
        fscanf (fv, "%d", &porgsizeB) ;
        for (nI = 0 ; nI < BORG_NUM ; nI++)
            {
            if (fscanf (fv, "%d", &cox) == EOF)
                {
                ROOK_NUMB = nI ;
                break ;
                }
            if (fscanf (fv, ",%d", &coy) == EOF)
                {
                ROOK_NUMB = nI ;
                break ;
                }
            _ptBRook [nI].x = cox ;
            _ptBRook [nI].y = coy ;
            }
        fclose (fv) ;
        }

    _makepath (Fn, Drive, Dir, "KNIGHT", ".INI") ;
    if ((fv = fopen (Fn, "r")) != NULL)
	{ 
        fscanf (fv, "%d", &porgsizeB) ;
        for (nI = 0 ; nI < BORG_NUM ; nI++)
            {
            if (fscanf (fv, "%d", &cox) == EOF)
                {
                KNIGHT_NUMB = nI ;
                break ;
                }
            if (fscanf (fv, ",%d", &coy) == EOF)
                {
                KNIGHT_NUMB = nI ;
                break ;
                }
            _ptBKnight [nI].x = cox ;
            _ptBKnight [nI].y = coy ;
            }
        fclose (fv) ;
        }

    _makepath (Fn, Drive, Dir, "BISHOP", ".INI") ;
    if ((fv = fopen (Fn, "r")) != NULL)
	{ 
        fscanf (fv, "%d", &porgsizeB) ;
        for (nI = 0 ; nI < BORG_NUM ; nI++)
            {
            if (fscanf (fv, "%d", &cox) == EOF)
                {
                BISHOP_NUMB = nI ;
                break ;
                }
            if (fscanf (fv, ",%d", &coy) == EOF)
                {
                BISHOP_NUMB = nI ;
                break ;
                }
            _ptBBishop [nI].x = cox ;
            _ptBBishop [nI].y = coy ;
            }
        fclose (fv) ;
        }

    _makepath (Fn, Drive, Dir, "QUEEN", ".INI") ;
    if ((fv = fopen (Fn, "r")) != NULL)
	{ 
        fscanf (fv, "%d", &porgsizeB) ;
        for (nI = 0 ; nI < BORG_NUM ; nI++)
            {
            if (fscanf (fv, "%d", &cox) == EOF)
                {
                QUEEN_NUMB = nI ;
                break ;
                }
            if (fscanf (fv, ",%d", &coy) == EOF)
                {
                QUEEN_NUMB = nI ;
                break ;
                }
            _ptBQueen [nI].x = cox ;
            _ptBQueen [nI].y = coy ;
            }
        fclose (fv) ;
        }

    _makepath (Fn, Drive, Dir, "KING", ".INI") ;
    if ((fv = fopen (Fn, "r")) != NULL)
	{ 
        fscanf (fv, "%d", &porgsizeB) ;
        for (nI = 0 ; nI < BORG_NUM ; nI++)
            {
            if (fscanf (fv, "%d", &cox) == EOF)
                {
                KING_NUMB = nI ;
                break ;
                }
            if (fscanf (fv, ",%d", &coy) == EOF)
                {
                KING_NUMB = nI ;
                break ;
                }
            _ptBKing [nI].x = cox ;
            _ptBKing [nI].y = coy ;
            }
        fclose (fv) ;
        }
    }

void BDRAW_SetSize (int nSize)
    {
    int nMax, cox, coy, nI ;

    nbss = nSize ;
    nMax = nSize - 1 ;

    cox = coy = 0 ;
    for (nI = 0 ; nI < PAWN_NUMB ; nI++)
        {
        cox = cox + _ptBPawn [nI].x ;
        coy = coy - _ptBPawn [nI].y ;
        ptBPawn [nI].x = (cox * nbss) / porgsizeB ;
        ptBPawn [nI].y = (coy * nbss) / porgsizeB ;
        }

    cox = coy = 0 ;
    for (nI = 0 ; nI < ROOK_NUMB ; nI++)
	{  
        cox = cox + _ptBRook [nI].x ;
        coy = coy - _ptBRook [nI].y ;
        ptBRook [nI].x = (cox * nbss) / porgsizeB ;
        ptBRook [nI].y = (coy * nbss) / porgsizeB ;
        }

    cox = coy = 0 ;
    for (nI = 0 ; nI < KNIGHT_NUMB ; nI++)
	{  
        cox = cox + _ptBKnight [nI].x ;
        coy = coy - _ptBKnight [nI].y ;
        ptBKnight [nI].x = (cox * nbss) / porgsizeB ;
        ptBKnight [nI].y = (coy * nbss) / porgsizeB ;
        }

    cox = coy = 0 ;
    for (nI = 0 ; nI < BISHOP_NUMB ; nI++)
        {
        cox = cox + _ptBBishop [nI].x ;
        coy = coy - _ptBBishop [nI].y ;
        ptBBishop [nI].x = (cox * nbss) / porgsizeB ;
        ptBBishop [nI].y = (coy * nbss) / porgsizeB ;
        }

    cox = coy = 0 ;
    for (nI = 0 ; nI < QUEEN_NUMB ; nI++)
	{  
        cox = cox + _ptBQueen [nI].x ;
        coy = coy - _ptBQueen [nI].y ;
        ptBQueen [nI].x = (cox * nbss) / porgsizeB ;
        ptBQueen [nI].y = (coy * nbss) / porgsizeB ;
        }

    cox = coy = 0 ;
    for (nI = 0 ; nI < KING_NUMB ; nI++)
	{  
        cox = cox + _ptBKing [nI].x ;
        coy = coy - _ptBKing [nI].y ;
        ptBKing [nI].x = (cox * nbss) / porgsizeB ;
        ptBKing [nI].y = (coy * nbss) / porgsizeB ;
        }
    }

void BDRAW_WhiteSquare (HDC hdc)
    {
    HPEN   hpOld ;
    HBRUSH hbOld ;

    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_WHITE]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_WHITE]) ;
    Rectangle (hdc, 0, 0, nbss, nbss) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_BlackSquare (HDC hdc)
    {
    HPEN   hpOld ;
    HBRUSH hbOld ;

    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_BLACK]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_BLACK]) ;
    Rectangle (hdc, 0, 0, nbss, nbss) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_PawnM (HDC hdc)
    {
    HPEN    hpOld ;
    HBRUSH  hbOld ;

    BDRAW_WhiteSquare (hdc) ;
    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_BLACK]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_BLACK]) ;
    Polygon (hdc, ptBPawn, PAWN_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_RookM (HDC hdc)
    {
    HPEN    hpOld ;
    HBRUSH  hbOld ;

    BDRAW_WhiteSquare (hdc) ;
    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_BLACK]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_BLACK]) ;
    Polygon (hdc, ptBRook, ROOK_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_KnightM (HDC hdc)
    {
    HPEN    hpOld ;
    HBRUSH  hbOld ;

    BDRAW_WhiteSquare (hdc) ;
    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_BLACK]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_BLACK]) ;
    Polygon (hdc, ptBKnight, KNIGHT_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_BishopM (HDC hdc)
    {
    HPEN    hpOld ;
    HBRUSH  hbOld ;

    BDRAW_WhiteSquare (hdc) ;
    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_BLACK]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_BLACK]) ;
    Polygon (hdc, ptBBishop, BISHOP_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_QueenM (HDC hdc)
    {
    HPEN    hpOld ;
    HBRUSH  hbOld ;

    BDRAW_WhiteSquare (hdc) ;
    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_BLACK]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_BLACK]) ;
    Polygon (hdc, ptBQueen, QUEEN_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_KingM (HDC hdc)
    {
    HPEN    hpOld ;
    HBRUSH  hbOld ;

    BDRAW_WhiteSquare (hdc) ;
    hpOld = (HPEN)   SelectObject (hdc, hpPen [PEN_BLACK]) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbBrush [BRUSH_BLACK]) ;
    Polygon (hdc, ptBKing, KING_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    }

void BDRAW_PieceMask (HDC hdc, int nPc)
    {
    switch (nPc)
        {
        case WHITE_PAWN :
        case BLACK_PAWN :
            BDRAW_PawnM (hdc) ;
            return ;

        case WHITE_KNIGHT :
        case BLACK_KNIGHT :
            BDRAW_KnightM (hdc) ;
            return ;

        case WHITE_BISHOP :
        case BLACK_BISHOP :
            BDRAW_BishopM (hdc) ;
            return ;

        case WHITE_ROOK :
        case BLACK_ROOK :
            BDRAW_RookM (hdc) ;
            return ;

        case WHITE_QUEEN :
        case BLACK_QUEEN :
            BDRAW_QueenM (hdc) ;
            return ;

        case WHITE_KING :
        case BLACK_KING :
            BDRAW_KingM (hdc) ;
            return ;
        }
    }

void BDRAW_SquareW1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Rectangle (hdc, 0, 0, nbss, nbss) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_PawnW1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBPawn, PAWN_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_RookW1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBRook, ROOK_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_KnightW1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBKnight, KNIGHT_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_BishopW1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBBishop, BISHOP_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_QueenW1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBQueen, QUEEN_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_KingW1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBKing, KING_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_SquareB1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Rectangle (hdc, 0, 0, nbss, nbss) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_PawnB1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBPawn, PAWN_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_RookB1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBRook, ROOK_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_KnightB1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBKnight, KNIGHT_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_BishopB1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBBishop, BISHOP_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_QueenB1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBQueen, QUEEN_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_KingB1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    BDRAW_BlackSquare (hdc) ;
    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Polygon (hdc, ptBKing, KING_NUMB) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_Piece1 (HDC hdc, int nPc, COLORREF clrWP, COLORREF clrWB,
                  COLORREF clrBP, COLORREF clrBB)
    {
    switch (nPc)
        {
        case WHITE_PAWN :   BDRAW_PawnW1 (hdc, clrWP, clrWB) ;   return ;
        case BLACK_PAWN :   BDRAW_PawnB1 (hdc, clrBP, clrBB) ;   return ;
        case WHITE_KNIGHT : BDRAW_KnightW1 (hdc, clrWP, clrWB) ; return ;
        case BLACK_KNIGHT : BDRAW_KnightB1 (hdc, clrBP, clrBB) ; return ;
        case WHITE_BISHOP : BDRAW_BishopW1 (hdc, clrWP, clrWB) ; return ;
        case BLACK_BISHOP : BDRAW_BishopB1 (hdc, clrBP, clrBB) ; return ;
        case WHITE_ROOK :   BDRAW_RookW1 (hdc, clrWP, clrWB) ;   return ;
        case BLACK_ROOK :   BDRAW_RookB1 (hdc, clrBP, clrBB) ;   return ;
        case WHITE_QUEEN :  BDRAW_QueenW1 (hdc, clrWP, clrWB) ;  return ;
        case BLACK_QUEEN :  BDRAW_QueenB1 (hdc, clrBP, clrBB) ;  return ;
        case WHITE_KING :   BDRAW_KingW1 (hdc, clrWP, clrWB) ;   return ;
        case BLACK_KING :   BDRAW_KingB1 (hdc, clrBP, clrBB) ;   return ;
        }
    }

void BDRAW_SquareBF1 (HDC hdc, COLORREF clrP, COLORREF clrB)
    {
    HPEN   hpTmp, hpOld ;
    HBRUSH hbTmp, hbOld ;

    hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
    hbTmp = CreateSolidBrush (clrB) ;
    hpOld = (HPEN)   SelectObject (hdc, hpTmp) ;
    hbOld = (HBRUSH) SelectObject (hdc, hbTmp) ;
    Rectangle (hdc, 0, 0, nbss, nbss) ;
    SelectObject (hdc, hpOld) ;
    SelectObject (hdc, hbOld) ;
    DeleteObject (hpTmp) ;
    DeleteObject (hbTmp) ;
    }

void BDRAW_TransparentBlt (HDC hdc, COLORREF clrTran)
    {
//
// Method #1 -- By Jeff Prosise
//
    HDC hdcImage, hdcAnd, hdcXor, hdcTemp ;
    HBITMAP htAnd, htXor, htTemp ;
    HBITMAP htOldImage, htOldAnd, htOldXor, htOldTemp ;
    int nImage, nAnd, nXor, nTemp ;

    hdcImage   = CreateCompatibleDC (hdc) ;
    htOldImage = (HBITMAP) SelectObject (hdcImage, BoardCfg.htTemp) ;
    nImage     = SetMapMode (hdcImage, GetMapMode (hdc)) ;

    hdcAnd = CreateCompatibleDC (hdc) ;
    nAnd   = SetMapMode (hdcAnd, GetMapMode (hdc)) ;
    htAnd  = CreateBitmap (BoardCfg.nss, BoardCfg.nss, 1, 1, NULL) ;
    htOldAnd = (HBITMAP) SelectObject (hdcAnd, htAnd) ;

    SetBkColor (hdcImage, clrTran) ;
    BitBlt (hdcAnd, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcImage, 0, 0, SRCCOPY) ;

    hdcXor   = CreateCompatibleDC (hdc) ;
    nXor     = SetMapMode (hdcXor, GetMapMode (hdc)) ;
    htXor    = CreateCompatibleBitmap (hdcImage, BoardCfg.nss, BoardCfg.nss) ;
    htOldXor = (HBITMAP) SelectObject (hdcXor, htXor) ;

    BitBlt (hdcXor, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcImage, 0, 0, SRCCOPY ) ;
    BitBlt (hdcXor, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcAnd,   0, 0, 0x220326) ;

    hdcTemp   = CreateCompatibleDC (hdc) ;
    nTemp     = SetMapMode (hdcTemp, GetMapMode (hdc)) ;
    htTemp    = CreateCompatibleBitmap (hdcImage, BoardCfg.nss, BoardCfg.nss) ;
    htOldTemp = (HBITMAP) SelectObject (hdcTemp, htTemp) ;

    BitBlt (hdcTemp, 0, 0, BoardCfg.nss, BoardCfg.nss, hdc,     0, 0, SRCCOPY  ) ;
    BitBlt (hdcTemp, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcAnd,  0, 0, SRCAND   ) ;
    BitBlt (hdcTemp, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcXor,  0, 0, SRCINVERT) ;
    BitBlt (hdc,     0, 0, BoardCfg.nss, BoardCfg.nss, hdcTemp, 0, 0, SRCCOPY  ) ;

    SetMapMode (hdcTemp,  nTemp ) ;
    SetMapMode (hdcXor,   nXor  ) ;
    SetMapMode (hdcAnd,   nAnd  ) ;
    SetMapMode (hdcImage, nImage) ;

    SelectObject (hdcTemp,  htOldTemp ) ;
    SelectObject (hdcXor,   htOldXor  ) ;
    SelectObject (hdcAnd,   htOldAnd  ) ;
    SelectObject (hdcImage, htOldImage) ;

    DeleteObject (hdcTemp ) ;
    DeleteObject (hdcXor  ) ;
    DeleteObject (hdcAnd  ) ;
    DeleteObject (hdcImage) ;

    DeleteObject (htTemp ) ;
    DeleteObject (htXor  ) ;
    DeleteObject (htAnd  ) ;
    }
