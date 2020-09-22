#include "thief.h"

#define ORG_NUM 200

POINT _ptPawn   [ORG_NUM] ;
POINT _ptRook   [ORG_NUM] ;
POINT _ptKnight [ORG_NUM] ;
POINT _ptBishop [ORG_NUM] ;
POINT _ptQueen  [ORG_NUM] ;
POINT _ptKing   [ORG_NUM] ;

int PAWN_NUM   = ORG_NUM ;
int ROOK_NUM   = ORG_NUM ;
int KNIGHT_NUM = ORG_NUM ;
int BISHOP_NUM = ORG_NUM ;
int QUEEN_NUM  = ORG_NUM ;
int KING_NUM   = ORG_NUM ;

int nss        = ORG_SIZE ;
int porgsize   = ORG_SIZE ;

POINT ptPawn   [ORG_NUM] ;
POINT ptRook   [ORG_NUM] ;
POINT ptKnight [ORG_NUM] ;
POINT ptBishop [ORG_NUM] ;
POINT ptQueen  [ORG_NUM] ;
POINT ptKing   [ORG_NUM] ;

void DRAW_LoadPieces(void)
{
	char Drive [_MAX_DRIVE] ;
	char Dir   [_MAX_DIR] ;
	char File  [_MAX_FNAME] ;
	char Ext   [_MAX_EXT] ;
	char Fn    [_MAX_PATH] ;

	int nI, cox, coy ;
	FILE *fv ;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext) ;

	nI = strlen(Dir) ;
	if(nI == 0)
	{
		strcat(Dir, "\\") ;
	}
	else
	{
		if(Dir [nI - 1] == '\\')
		{
			strcat(Dir, "") ;
		}
		else
		{
			strcat(Dir, "\\") ;
		}
	}

	// which pieceset ?
	switch(User.nPieceSet)
	{
		case VECTOR_TOUPS :
			SYS_CheckToups() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\TOUPS\\") ;
			break ;

		case VECTOR_NICOK :
			SYS_CheckNicok() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\NICOK\\") ;
			break ;

		case VECTOR_KBIRK :
			SYS_CheckKbirk() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\KBIRK\\") ;
			break ;

		case VECTOR_JHURT :
			SYS_CheckJhurt() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\JHURT\\") ;
			break ;

		case VECTOR_ECHUR :
			SYS_CheckEchur() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\ECHUR\\") ;
			break ;

		case VECTOR_DFONG :
			SYS_CheckDfong() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\DFONG\\") ;
			break ;

		case VECTOR_USER :
			SYS_CheckUser() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\USER\\") ;
			break ;

		default :
			User.nPieceSet = VECTOR_TOUPS ;
			SYS_CheckToups() ;
			strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\TOUPS\\") ;
			break ;
	}

	_makepath(Fn, Drive, Dir, "PAWN", ".INI") ;
	if((fv = fopen(Fn, "r")) == NULL)
	{
		CSet.ptPiece [WHITE_PAWN].x = ORG_SIZE ;
		CSet.ptPiece [WHITE_PAWN].y = ORG_SIZE ;
		CSet.ptPiece [BLACK_PAWN].x = ORG_SIZE ;
		CSet.ptPiece [BLACK_PAWN].y = ORG_SIZE ;
	}
	else
	{
		fscanf(fv, "%d", &porgsize) ;
		for(nI = 0 ; nI < ORG_NUM ; nI++)
		{
			if(fscanf(fv, "%d", &cox) == EOF)
			{
				PAWN_NUM = nI ;
				break ;
			}
			if(fscanf(fv, ",%d", &coy) == EOF)
			{
				PAWN_NUM = nI ;
				break ;
			}
			_ptPawn [nI].x = cox ;
			_ptPawn [nI].y = coy ;
		}
		fclose(fv) ;

		CSet.ptPiece [WHITE_PAWN].x = porgsize ;
		CSet.ptPiece [WHITE_PAWN].y = porgsize ;
		CSet.ptPiece [BLACK_PAWN].x = porgsize ;
		CSet.ptPiece [BLACK_PAWN].y = porgsize ;
	}

	_makepath(Fn, Drive, Dir, "ROOK", ".INI") ;
	if((fv = fopen(Fn, "r")) == NULL)
	{
		CSet.ptPiece [WHITE_ROOK].x = ORG_SIZE ;
		CSet.ptPiece [WHITE_ROOK].y = ORG_SIZE ;
		CSet.ptPiece [BLACK_ROOK].x = ORG_SIZE ;
		CSet.ptPiece [BLACK_ROOK].y = ORG_SIZE ;
	}
	else
	{
		fscanf(fv, "%d", &porgsize) ;
		for(nI = 0 ; nI < ORG_NUM ; nI++)
		{
			if(fscanf(fv, "%d", &cox) == EOF)
			{
				ROOK_NUM = nI ;
				break ;
			}
			if(fscanf(fv, ",%d", &coy) == EOF)
			{
				ROOK_NUM = nI ;
				break ;
			}
			_ptRook [nI].x = cox ;
			_ptRook [nI].y = coy ;
		}
		fclose(fv) ;

		CSet.ptPiece [WHITE_ROOK].x = porgsize ;
		CSet.ptPiece [WHITE_ROOK].y = porgsize ;
		CSet.ptPiece [BLACK_ROOK].x = porgsize ;
		CSet.ptPiece [BLACK_ROOK].y = porgsize ;
	}

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI") ;
	if((fv = fopen(Fn, "r")) == NULL)
	{
		CSet.ptPiece [WHITE_KNIGHT].x = ORG_SIZE ;
		CSet.ptPiece [WHITE_KNIGHT].y = ORG_SIZE ;
		CSet.ptPiece [BLACK_KNIGHT].x = ORG_SIZE ;
		CSet.ptPiece [BLACK_KNIGHT].y = ORG_SIZE ;
	}
	else
	{
		fscanf(fv, "%d", &porgsize) ;
		for(nI = 0 ; nI < ORG_NUM ; nI++)
		{
			if(fscanf(fv, "%d", &cox) == EOF)
			{
				KNIGHT_NUM = nI ;
				break ;
			}
			if(fscanf(fv, ",%d", &coy) == EOF)
			{
				KNIGHT_NUM = nI ;
				break ;
			}
			_ptKnight [nI].x = cox ;
			_ptKnight [nI].y = coy ;
		}
		fclose(fv) ;

		CSet.ptPiece [WHITE_KNIGHT].x = porgsize ;
		CSet.ptPiece [WHITE_KNIGHT].y = porgsize ;
		CSet.ptPiece [BLACK_KNIGHT].x = porgsize ;
		CSet.ptPiece [BLACK_KNIGHT].y = porgsize ;
	}

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI") ;
	if((fv = fopen(Fn, "r")) == NULL)
	{
		CSet.ptPiece [WHITE_BISHOP].x = ORG_SIZE ;
		CSet.ptPiece [WHITE_BISHOP].y = ORG_SIZE ;
		CSet.ptPiece [BLACK_BISHOP].x = ORG_SIZE ;
		CSet.ptPiece [BLACK_BISHOP].y = ORG_SIZE ;
	}
	else
	{
		fscanf(fv, "%d", &porgsize) ;
		for(nI = 0 ; nI < ORG_NUM ; nI++)
		{
			if(fscanf(fv, "%d", &cox) == EOF)
			{
				BISHOP_NUM = nI ;
				break ;
			}
			if(fscanf(fv, ",%d", &coy) == EOF)
			{
				BISHOP_NUM = nI ;
				break ;
			}
			_ptBishop [nI].x = cox ;
			_ptBishop [nI].y = coy ;
		}
		fclose(fv) ;

		CSet.ptPiece [WHITE_BISHOP].x = porgsize ;
		CSet.ptPiece [WHITE_BISHOP].y = porgsize ;
		CSet.ptPiece [BLACK_BISHOP].x = porgsize ;
		CSet.ptPiece [BLACK_BISHOP].y = porgsize ;
	}

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI") ;
	if((fv = fopen(Fn, "r")) == NULL)
	{
		CSet.ptPiece [WHITE_QUEEN].x = ORG_SIZE ;
		CSet.ptPiece [WHITE_QUEEN].y = ORG_SIZE ;
		CSet.ptPiece [BLACK_QUEEN].x = ORG_SIZE ;
		CSet.ptPiece [BLACK_QUEEN].y = ORG_SIZE ;
	}
	else
	{
		fscanf(fv, "%d", &porgsize) ;
		for(nI = 0 ; nI < ORG_NUM ; nI++)
		{
			if(fscanf(fv, "%d", &cox) == EOF)
			{
				QUEEN_NUM = nI ;
				break ;
			}
			if(fscanf(fv, ",%d", &coy) == EOF)
			{
				QUEEN_NUM = nI ;
				break ;
			}
			_ptQueen [nI].x = cox ;
			_ptQueen [nI].y = coy ;
		}
		fclose(fv) ;

		CSet.ptPiece [WHITE_QUEEN].x = porgsize ;
		CSet.ptPiece [WHITE_QUEEN].y = porgsize ;
		CSet.ptPiece [BLACK_QUEEN].x = porgsize ;
		CSet.ptPiece [BLACK_QUEEN].y = porgsize ;
	}

	_makepath(Fn, Drive, Dir, "KING", ".INI") ;
	if((fv = fopen(Fn, "r")) == NULL)
	{
		CSet.ptPiece [WHITE_KING].x = ORG_SIZE ;
		CSet.ptPiece [WHITE_KING].y = ORG_SIZE ;
		CSet.ptPiece [BLACK_KING].x = ORG_SIZE ;
		CSet.ptPiece [BLACK_KING].y = ORG_SIZE ;
	}
	else
	{
		fscanf(fv, "%d", &porgsize) ;
		for(nI = 0 ; nI < ORG_NUM ; nI++)
		{
			if(fscanf(fv, "%d", &cox) == EOF)
			{
				KING_NUM = nI ;
				break ;
			}
			if(fscanf(fv, ",%d", &coy) == EOF)
			{
				KING_NUM = nI ;
				break ;
			}
			_ptKing [nI].x = cox ;
			_ptKing [nI].y = coy ;
		}
		fclose(fv) ;

		CSet.ptPiece [WHITE_KING].x = porgsize ;
		CSet.ptPiece [WHITE_KING].y = porgsize ;
		CSet.ptPiece [BLACK_KING].x = porgsize ;
		CSet.ptPiece [BLACK_KING].y = porgsize ;
	}
}

void DRAW_SetSize(int nSize)
{
	int nMax, cox, coy, nI ;

	nss  = nSize ;
	nMax = nSize - 1 ;

	cox = coy = 0 ;
	for(nI = 0 ; nI < PAWN_NUM ; nI++)
	{
		cox = cox + _ptPawn [nI].x ;
		coy = coy - _ptPawn [nI].y ;
		ptPawn [nI].x = (cox * nss) / porgsize ;
		ptPawn [nI].y = (coy * nss) / porgsize ;
	}

	cox = coy = 0 ;
	for(nI = 0 ; nI < ROOK_NUM ; nI++)
	{
		cox = cox + _ptRook [nI].x ;
		coy = coy - _ptRook [nI].y ;
		ptRook [nI].x = (cox * nss) / porgsize ;
		ptRook [nI].y = (coy * nss) / porgsize ;
	}

	cox = coy = 0 ;
	for(nI = 0 ; nI < KNIGHT_NUM ; nI++)
	{
		cox = cox + _ptKnight [nI].x ;
		coy = coy - _ptKnight [nI].y ;

		ptKnight [nI].x = (cox * nss) / porgsize ;
		ptKnight [nI].y = (coy * nss) / porgsize ;
	}

	cox = coy = 0 ;
	for(nI = 0 ; nI < BISHOP_NUM ; nI++)
	{
		cox = cox + _ptBishop [nI].x ;
		coy = coy - _ptBishop [nI].y ;
		ptBishop [nI].x = (cox * nss) / porgsize ;
		ptBishop [nI].y = (coy * nss) / porgsize ;
	}

	cox = coy = 0 ;
	for(nI = 0 ; nI < QUEEN_NUM ; nI++)
	{
		cox = cox + _ptQueen [nI].x ;
		coy = coy - _ptQueen [nI].y ;
		ptQueen [nI].x = (cox * nss) / porgsize ;
		ptQueen [nI].y = (coy * nss) / porgsize ;
	}

	cox = coy = 0 ;
	for(nI = 0 ; nI < KING_NUM ; nI++)
	{
		cox = cox + _ptKing [nI].x ;
		coy = coy - _ptKing [nI].y ;
		ptKing [nI].x = (cox * nss) / porgsize ;
		ptKing [nI].y = (coy * nss) / porgsize ;
	}
}

void DRAW_WhiteSquare(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE]) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_BlackSquare(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK]) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_SquareW(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE_SQUARE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_SQUARE_FILL]) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_PawnW(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptPawn, PAWN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_RookW(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptRook, ROOK_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_KnightW(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptKnight, KNIGHT_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_BishopW(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptBishop, BISHOP_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_QueenW(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptQueen, QUEEN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_KingW(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptKing, KING_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_SquareB(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK_SQUARE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_SQUARE_FILL]) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_PawnB(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptPawn, PAWN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_RookB(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptRook, ROOK_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_KnightB(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptKnight, KNIGHT_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_BishopB(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptBishop, BISHOP_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_QueenB(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptQueen, QUEEN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_KingB(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptKing, KING_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_Piece(HDC hdc, int nPc)
{
	switch(nPc)
	{
		case WHITE_PAWN :
			DRAW_PawnW(hdc) ;
			return ;
		case BLACK_PAWN :
			DRAW_PawnB(hdc) ;
			return ;
		case WHITE_KNIGHT :
			DRAW_KnightW(hdc) ;
			return ;
		case BLACK_KNIGHT :
			DRAW_KnightB(hdc) ;
			return ;
		case WHITE_BISHOP :
			DRAW_BishopW(hdc) ;
			return ;
		case BLACK_BISHOP :
			DRAW_BishopB(hdc) ;
			return ;
		case WHITE_ROOK :
			DRAW_RookW(hdc) ;
			return ;
		case BLACK_ROOK :
			DRAW_RookB(hdc) ;
			return ;
		case WHITE_QUEEN :
			DRAW_QueenW(hdc) ;
			return ;
		case BLACK_QUEEN :
			DRAW_QueenB(hdc) ;
			return ;
		case WHITE_KING :
			DRAW_KingW(hdc) ;
			return ;
		case BLACK_KING :
			DRAW_KingB(hdc) ;
			return ;
	}
}

void DRAW_SquareBF(HDC hdc)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrColor [CLR_BUFFER_FILL]) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BUFFER_FILL]) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_PawnM(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_WhiteSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK]) ;
	Polygon(hdc, ptPawn, PAWN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_RookM(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_WhiteSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK]) ;
	Polygon(hdc, ptRook, ROOK_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_KnightM(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_WhiteSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK]) ;
	Polygon(hdc, ptKnight, KNIGHT_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_BishopM(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_WhiteSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK]) ;
	Polygon(hdc, ptBishop, BISHOP_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_QueenM(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_WhiteSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK]) ;
	Polygon(hdc, ptQueen, QUEEN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_KingM(HDC hdc)
{
	HPEN   hpOld ;
	HBRUSH hbOld ;

	DRAW_WhiteSquare(hdc) ;
	hpOld = (HPEN)   SelectObject(hdc, hpPen [PEN_BLACK]) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK]) ;
	Polygon(hdc, ptKing, KING_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
}

void DRAW_PieceMask(HDC hdc, int nPc)
{
	switch(nPc)
	{
		case WHITE_PAWN :
		case BLACK_PAWN :
			DRAW_PawnM(hdc) ;
			return ;

		case WHITE_KNIGHT :
		case BLACK_KNIGHT :
			DRAW_KnightM(hdc) ;
			return ;

		case WHITE_BISHOP :
		case BLACK_BISHOP :
			DRAW_BishopM(hdc) ;
			return ;

		case WHITE_ROOK :
		case BLACK_ROOK :
			DRAW_RookM(hdc) ;
			return ;

		case WHITE_QUEEN :
		case BLACK_QUEEN :
			DRAW_QueenM(hdc) ;
			return ;

		case WHITE_KING :
		case BLACK_KING :
			DRAW_KingM(hdc) ;
			return ;
	}
}

void DRAW_PawnW_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptPawn, PAWN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_RookW_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptRook, ROOK_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_KnightW_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptKnight, KNIGHT_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_BishopW_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptBishop, BISHOP_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_QueenW_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptQueen, QUEEN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_KingW_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
	Polygon(hdc, ptKing, KING_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_PawnB_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptPawn, PAWN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_RookB_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptRook, ROOK_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_KnightB_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptKnight, KNIGHT_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_BishopB_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptBishop, BISHOP_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_QueenB_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptQueen, QUEEN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_KingB_Outline(HDC hdc, COLORREF clrO)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrO) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
	Polygon(hdc, ptKing, KING_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
}

void DRAW_Piece_Outline(HDC hdc, int nPc, COLORREF clrO)
{
	switch(nPc)
	{
		case WHITE_PAWN :
			DRAW_PawnW_Outline(hdc, clrO) ;
			return ;
		case BLACK_PAWN :
			DRAW_PawnB_Outline(hdc, clrO) ;
			return ;
		case WHITE_KNIGHT :
			DRAW_KnightW_Outline(hdc, clrO) ;
			return ;
		case BLACK_KNIGHT :
			DRAW_KnightB_Outline(hdc, clrO) ;
			return ;
		case WHITE_BISHOP :
			DRAW_BishopW_Outline(hdc, clrO) ;
			return ;
		case BLACK_BISHOP :
			DRAW_BishopB_Outline(hdc, clrO) ;
			return ;
		case WHITE_ROOK :
			DRAW_RookW_Outline(hdc, clrO) ;
			return ;
		case BLACK_ROOK :
			DRAW_RookB_Outline(hdc, clrO) ;
			return ;
		case WHITE_QUEEN :
			DRAW_QueenW_Outline(hdc, clrO) ;
			return ;
		case BLACK_QUEEN :
			DRAW_QueenB_Outline(hdc, clrO) ;
			return ;
		case WHITE_KING :
			DRAW_KingW_Outline(hdc, clrO) ;
			return ;
		case BLACK_KING :
			DRAW_KingB_Outline(hdc, clrO) ;
			return ;
	}
}

void DRAW_SquareW1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_PawnW1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptPawn, PAWN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_RookW1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;

	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptRook, ROOK_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_KnightW1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptKnight, KNIGHT_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_BishopW1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptBishop, BISHOP_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_QueenW1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptQueen, QUEEN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_KingW1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptKing, KING_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_SquareB1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_PawnB1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptPawn, PAWN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_RookB1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptRook, ROOK_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_KnightB1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptKnight, KNIGHT_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_BishopB1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptBishop, BISHOP_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_QueenB1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptQueen, QUEEN_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_KingB1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	DRAW_BlackSquare(hdc) ;
	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Polygon(hdc, ptKing, KING_NUM) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_Piece1(HDC hdc, int nPc, COLORREF clrWP, COLORREF clrWB, COLORREF clrBP, COLORREF clrBB)
{
	switch(nPc)
	{
		case WHITE_PAWN :
			DRAW_PawnW1(hdc, clrWP, clrWB) ;
			return ;
		case BLACK_PAWN :
			DRAW_PawnB1(hdc, clrBP, clrBB) ;
			return ;
		case WHITE_KNIGHT :
			DRAW_KnightW1(hdc, clrWP, clrWB) ;
			return ;
		case BLACK_KNIGHT :
			DRAW_KnightB1(hdc, clrBP, clrBB) ;
			return ;
		case WHITE_BISHOP :
			DRAW_BishopW1(hdc, clrWP, clrWB) ;
			return ;
		case BLACK_BISHOP :
			DRAW_BishopB1(hdc, clrBP, clrBB) ;
			return ;
		case WHITE_ROOK :
			DRAW_RookW1(hdc, clrWP, clrWB) ;
			return ;
		case BLACK_ROOK :
			DRAW_RookB1(hdc, clrBP, clrBB) ;
			return ;
		case WHITE_QUEEN :
			DRAW_QueenW1(hdc, clrWP, clrWB) ;
			return ;
		case BLACK_QUEEN :
			DRAW_QueenB1(hdc, clrBP, clrBB) ;
			return ;
		case WHITE_KING :
			DRAW_KingW1(hdc, clrWP, clrWB) ;
			return ;
		case BLACK_KING :
			DRAW_KingB1(hdc, clrBP, clrBB) ;
			return ;
	}
}

void DRAW_SquareBF1(HDC hdc, COLORREF clrP, COLORREF clrB)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrP) ;
	hbTmp = CreateSolidBrush(clrB) ;
	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Rectangle(hdc, 0, 0, nss, nss) ;
	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_VectorPiece(HDC hdc, int nPc, COLORREF clrT, int nS)
{
	HPEN   hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	hpTmp = CreatePen(PS_SOLID, 0, clrT) ;
	hbTmp = CreateSolidBrush(clrT) ;

	hpOld = (HPEN)   SelectObject(hdc, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	Rectangle(hdc, 0, 0, nS, nS) ;

	switch(nPc)
	{
		case WHITE_PAWN :
			SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
			Polygon(hdc, ptPawn, PAWN_NUM) ;
			break ;

		case WHITE_ROOK :
			SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
			Polygon(hdc, ptRook, ROOK_NUM) ;
			break ;

		case WHITE_KNIGHT :
			SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
			Polygon(hdc, ptKnight, KNIGHT_NUM) ;
			break ;

		case WHITE_BISHOP :
			SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
			Polygon(hdc, ptBishop, BISHOP_NUM) ;
			break ;

		case WHITE_QUEEN :
			SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
			Polygon(hdc, ptQueen, QUEEN_NUM) ;
			break ;

		case WHITE_KING :
			SelectObject(hdc, hpPen [PEN_WHITE_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_WHITE_PIECE_FILL]) ;
			Polygon(hdc, ptKing, KING_NUM) ;
			break ;

		case BLACK_PAWN :
			SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
			Polygon(hdc, ptPawn, PAWN_NUM) ;
			break ;

		case BLACK_ROOK :
			SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
			Polygon(hdc, ptRook, ROOK_NUM) ;
			break ;

		case BLACK_KNIGHT :
			SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
			Polygon(hdc, ptKnight, KNIGHT_NUM) ;
			break ;

		case BLACK_BISHOP :
			SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
			Polygon(hdc, ptBishop, BISHOP_NUM) ;
			break ;

		case BLACK_QUEEN :
			SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
			Polygon(hdc, ptQueen, QUEEN_NUM) ;
			break ;

		case BLACK_KING :
			SelectObject(hdc, hpPen [PEN_BLACK_PIECE_OUTLINE]) ;
			SelectObject(hdc, hbBrush [BRUSH_BLACK_PIECE_FILL]) ;
			Polygon(hdc, ptKing, KING_NUM) ;
			break ;
	}

	SelectObject(hdc, hpOld) ;
	SelectObject(hdc, hbOld) ;

	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;
}

void DRAW_TransparentBlt(int nG, HDC hdc, int nP)
{
//
// Method #1 -- By Jeff Prosise
//
	HDC hdcImage, hdcAnd, hdcXor, hdcTemp ;
	HBITMAP htAnd, htXor, htTemp ;
	HBITMAP htOldImage, htOldAnd, htOldXor, htOldTemp ;
	int nImage, nAnd, nXor, nTemp ;

	hdcImage   = CreateCompatibleDC(hdc) ;
	htOldImage = (HBITMAP) SelectObject(hdcImage, Game [nG].hPiece [nP]) ;
	nImage     = SetMapMode(hdcImage, GetMapMode(hdc)) ;

	hdcAnd = CreateCompatibleDC(hdc) ;
	nAnd   = SetMapMode(hdcAnd, GetMapMode(hdc)) ;
	htAnd  = CreateBitmap(Game [nG].nss, Game [nG].nss, 1, 1, NULL) ;
	htOldAnd = (HBITMAP) SelectObject(hdcAnd, htAnd) ;

	SetBkColor(hdcImage, CSet.clrPiece [nP]) ;
	BitBlt(hdcAnd, 0, 0, Game [nG].nss, Game [nG].nss, hdcImage, 0, 0, SRCCOPY) ;

	hdcXor   = CreateCompatibleDC(hdc) ;
	nXor     = SetMapMode(hdcXor, GetMapMode(hdc)) ;
	htXor    = CreateCompatibleBitmap(hdcImage, Game [nG].nss, Game [nG].nss) ;
	htOldXor = (HBITMAP) SelectObject(hdcXor, htXor) ;

	BitBlt(hdcXor, 0, 0, Game [nG].nss, Game [nG].nss, hdcImage, 0, 0, SRCCOPY) ;
	BitBlt(hdcXor, 0, 0, Game [nG].nss, Game [nG].nss, hdcAnd,   0, 0, 0x220326) ;

	hdcTemp   = CreateCompatibleDC(hdc) ;
	nTemp     = SetMapMode(hdcTemp, GetMapMode(hdc)) ;
	htTemp    = CreateCompatibleBitmap(hdcImage, Game [nG].nss, Game [nG].nss) ;
	htOldTemp = (HBITMAP) SelectObject(hdcTemp, htTemp) ;

	BitBlt(hdcTemp, 0, 0, Game [nG].nss, Game [nG].nss, hdc,     0, 0, SRCCOPY) ;
	BitBlt(hdcTemp, 0, 0, Game [nG].nss, Game [nG].nss, hdcAnd,  0, 0, SRCAND) ;
	BitBlt(hdcTemp, 0, 0, Game [nG].nss, Game [nG].nss, hdcXor,  0, 0, SRCINVERT) ;
	BitBlt(hdc,     0, 0, Game [nG].nss, Game [nG].nss, hdcTemp, 0, 0, SRCCOPY) ;

	SetMapMode(hdcTemp,  nTemp) ;
	SetMapMode(hdcXor,   nXor) ;
	SetMapMode(hdcAnd,   nAnd) ;
	SetMapMode(hdcImage, nImage) ;

	SelectObject(hdcTemp,  htOldTemp) ;
	SelectObject(hdcXor,   htOldXor) ;
	SelectObject(hdcAnd,   htOldAnd) ;
	SelectObject(hdcImage, htOldImage) ;

	DeleteObject(hdcTemp) ;
	DeleteObject(hdcXor) ;
	DeleteObject(hdcAnd) ;
	DeleteObject(hdcImage) ;

	DeleteObject(htTemp) ;
	DeleteObject(htXor) ;
	DeleteObject(htAnd) ;
//

//
// Method #2 -- By Paul Reynolds
//
//  HDC hdcImage, hdcTrans ;
//  HBITMAP htTrans, htOldImage, htOldTrans ;
//  COLORREF clrOldBack, clrOldText ;
//
//  clrOldBack = (COLORREF) SetBkColor   (hdc, RGB (255, 255, 255)) ;
//  clrOldText = (COLORREF) SetTextColor (hdc, RGB (  0,   0,   0)) ;
//
//  hdcImage = CreateCompatibleDC (hdc) ;
//  hdcTrans = CreateCompatibleDC (hdc) ;
//
//  htOldImage = (HBITMAP) SelectObject (hdcImage, Game [nG].hPiece [nP]) ;
//  htTrans    = CreateBitmap (Game [nG].nss, Game [nG].nss, 1, 1, NULL) ;
//  htOldTrans = (HBITMAP) SelectObject (hdcTrans, htTrans) ;
//
//  SetBkColor (hdcImage, CSet.clrPiece [nP]) ;
//  BitBlt (hdcTrans, 0, 0, Game [nG].nss, Game [nG].nss, hdcImage, 0, 0, SRCCOPY) ;
//
//  BitBlt (hdc, 0, 0, Game [nG].nss, Game [nG].nss, hdcImage, 0, 0, SRCINVERT) ;
//  BitBlt (hdc, 0, 0, Game [nG].nss, Game [nG].nss, hdcTrans, 0, 0, SRCAND   ) ;
//  BitBlt (hdc, 0, 0, Game [nG].nss, Game [nG].nss, hdcImage, 0, 0, SRCINVERT) ;
//
//  SelectObject (hdcTrans, htOldTrans) ;
//  SelectObject (hdcImage, htOldImage) ;
//
//  SetBkColor   (hdc, clrOldBack) ;
//  SetTextColor (hdc, clrOldText) ;
//
//  DeleteObject (hdcTrans) ;
//  DeleteObject (hdcImage) ;
//  DeleteObject (htTrans ) ;
//

//
// Old way of doing it -- calling windows api (it has bugs!)
//
//  HDC hdcImage ;
//  HBITMAP htOldImage ;
//
//  hdcImage   = CreateCompatibleDC (hdc) ;
//  htOldImage = (HBITMAP) SelectObject (hdcImage, Game [nG].hPiece [nP]) ;
//  TransparentBlt (hdc, 0, 0, Game [nG].nss, Game [nG].nss, hdcImage, 0, 0, Game [nG].nss, Game [nG].nss, CSet.clrPiece [nP]) ;
//  SelectObject (hdcImage, htOldImage) ;
//  DeleteDC (hdcImage) ;
//
}
