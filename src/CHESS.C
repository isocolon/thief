#include "thief.h"

#define ALGE_CASTLE_KING_SIDE_MOVE  ICS_CASTLE_KING_SIDE_MOVE
#define ALGE_CASTLE_QUEEN_SIDE_MOVE ICS_CASTLE_QUEEN_SIDE_MOVE

int _KnightMoveX [8];
int _KnightMoveY [8];

int _KingMoveX [8];
int _KingMoveY [8];

char cAlgePiece [12];

int nTempBoard [8] [8];
int nTempBuffer [12];
int nTempPromote [8] [8];
int bTempWhitesMove;
int nTempLastDoublePushFile;
int bTempCanCastleKingSide  [2];
int bTempCanCastleQueenSide [2];
int nTempKingX [2];
int nTempKingY [2];
int bTempInitialMove;
int bInTruePremove;
int bInIsLegalMoveString;

void CHESS_Init(void)
{
	_KnightMoveX [0] = +1;
	_KnightMoveY [0] = +2;
	_KnightMoveX [1] = +2;
	_KnightMoveY [1] = +1;
	_KnightMoveX [2] = +2;
	_KnightMoveY [2] = -1;
	_KnightMoveX [3] = +1;
	_KnightMoveY [3] = -2;
	_KnightMoveX [4] = -1;
	_KnightMoveY [4] = -2;
	_KnightMoveX [5] = -2;
	_KnightMoveY [5] = -1;
	_KnightMoveX [6] = -2;
	_KnightMoveY [6] = +1;
	_KnightMoveX [7] = -1;
	_KnightMoveY [7] = +2;

	_KingMoveX   [0] = +0;
	_KingMoveY   [0] = +1;
	_KingMoveX   [1] = +1;
	_KingMoveY   [1] = +1;
	_KingMoveX   [2] = +1;
	_KingMoveY   [2] =  0;
	_KingMoveX   [3] = +1;
	_KingMoveY   [3] = -1;
	_KingMoveX   [4] =  0;
	_KingMoveY   [4] = -1;
	_KingMoveX   [5] = -1;
	_KingMoveY   [5] = -1;
	_KingMoveX   [6] = -1;
	_KingMoveY   [6] =  0;
	_KingMoveX   [7] = -1;
	_KingMoveY   [7] = +1;

	cAlgePiece [WHITE_PAWN]   = ICS_WHITE_PAWN;
	cAlgePiece [WHITE_ROOK]   = ICS_WHITE_ROOK;
	cAlgePiece [WHITE_KNIGHT] = ICS_WHITE_KNIGHT;
	cAlgePiece [WHITE_BISHOP] = ICS_WHITE_BISHOP;
	cAlgePiece [WHITE_QUEEN]  = ICS_WHITE_QUEEN;
	cAlgePiece [BLACK_PAWN]   = ICS_WHITE_PAWN;
	cAlgePiece [BLACK_ROOK]   = ICS_WHITE_ROOK;
	cAlgePiece [BLACK_KNIGHT] = ICS_WHITE_KNIGHT;
	cAlgePiece [BLACK_BISHOP] = ICS_WHITE_BISHOP;
	cAlgePiece [BLACK_QUEEN]  = ICS_WHITE_QUEEN;
	cAlgePiece [WHITE_KING]   = ICS_WHITE_KING;
	cAlgePiece [BLACK_KING]   = ICS_WHITE_KING;

	bInTruePremove       = 0;
	bInIsLegalMoveString = 0;
}

int CHESS_IsEmptySquare(int nG, int nX, int nY)
{
	return (Game [nG].nBoard [nX] [nY] == EMPTY_SQUARE);
}

int CHESS_IsWhitePiece(int nG, int nX, int nY)
{
	switch(Game [nG].nBoard [nX] [nY])
	{
		case WHITE_PAWN :
			return 1;
		case WHITE_ROOK :
			return 1;
		case WHITE_KNIGHT :
			return 1;
		case WHITE_BISHOP :
			return 1;
		case WHITE_QUEEN :
			return 1;
		case WHITE_KING :
			return 1;
	}
	return 0;
}

int CHESS_IsBlackPiece(int nG, int nX, int nY)
{
	switch(Game [nG].nBoard [nX] [nY])
	{
		case BLACK_PAWN :
			return 1;
		case BLACK_ROOK :
			return 1;
		case BLACK_KNIGHT :
			return 1;
		case BLACK_BISHOP :
			return 1;
		case BLACK_QUEEN :
			return 1;
		case BLACK_KING :
			return 1;
	}
	return 0;
}

int CHESS_IsSameColor(int nG, int nX, int nY, int nC)
{
	if(bInTruePremove)
	{
		return 0;
	}
	if(nC == INDEX_WHITE)
	{
		return (CHESS_IsWhitePiece(nG, nX, nY));
	}
	else
	{
		return (CHESS_IsBlackPiece(nG, nX, nY));
	}
}

int CHESS_ReversePiece(int nPc)
{
	switch(nPc)
	{
		case WHITE_PAWN :
			return BLACK_PAWN;
		case WHITE_ROOK :
			return BLACK_ROOK;
		case WHITE_KNIGHT :
			return BLACK_KNIGHT;
		case WHITE_BISHOP :
			return BLACK_BISHOP;
		case WHITE_QUEEN :
			return BLACK_QUEEN;
		case WHITE_KING :
			return BLACK_KING;
		case BLACK_PAWN :
			return WHITE_PAWN;
		case BLACK_ROOK :
			return WHITE_ROOK;
		case BLACK_KNIGHT :
			return WHITE_KNIGHT;
		case BLACK_BISHOP :
			return WHITE_BISHOP;
		case BLACK_QUEEN :
			return WHITE_QUEEN;
		case BLACK_KING :
			return WHITE_KING;
	}

	return EMPTY_SQUARE;
}

void CHESS_CopyTempMove(int nI)
{
	Moves.nFromX    [Moves.nMove] = TempMoves.nFromX    [nI];
	Moves.nFromY    [Moves.nMove] = TempMoves.nFromY    [nI];
	Moves.nToX      [Moves.nMove] = TempMoves.nToX      [nI];
	Moves.nToY      [Moves.nMove] = TempMoves.nToY      [nI];
	Moves.nMoveType [Moves.nMove] = TempMoves.nMoveType [nI];
	Moves.nCapture  [Moves.nMove] = TempMoves.nCapture  [nI];
	Moves.nDrop     [Moves.nMove] = TempMoves.nDrop     [nI];
	Moves.nPromote  [Moves.nMove] = TempMoves.nPromote  [nI];

	Moves.nMove = Moves.nMove + 1;
}

void CHESS_CopyMoveTemp(int nI, int nM)
{
	TempMoves.nFromX    [nI] = Moves.nFromX    [nM];
	TempMoves.nFromY    [nI] = Moves.nFromY    [nM];
	TempMoves.nToX      [nI] = Moves.nToX      [nM];
	TempMoves.nToY      [nI] = Moves.nToY      [nM];
	TempMoves.nMoveType [nI] = Moves.nMoveType [nM];
	TempMoves.nCapture  [nI] = Moves.nCapture  [nM];
	TempMoves.nDrop     [nI] = Moves.nDrop     [nM];
	TempMoves.nPromote  [nI] = Moves.nPromote  [nM];
}

void CHESS_AddTempMove(int nFX, int nFY, int nTX, int nTY, int nType, int nCapture, int nDrop, int nPromote)
{
	TempMoves.nFromX    [TempMoves.nMove] = nFX;
	TempMoves.nFromY    [TempMoves.nMove] = nFY;
	TempMoves.nToX      [TempMoves.nMove] = nTX;
	TempMoves.nToY      [TempMoves.nMove] = nTY;
	TempMoves.nMoveType [TempMoves.nMove] = nType;
	TempMoves.nCapture  [TempMoves.nMove] = nCapture;
	TempMoves.nDrop     [TempMoves.nMove] = nDrop;
	TempMoves.nPromote  [TempMoves.nMove] = nPromote;

	TempMoves.nMove = TempMoves.nMove + 1;
}

int CHESS_CheckBQ(int nG, int nC, int nOC, int nSX, int nSY, int nB, int nQ, int *nCount, int *bDirect)
{
	int bCheck, nI, nX, nY;

	bCheck = 0;

	// upper left
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nX = nSX - nI;
		nY = nSY - nI;

		if(nX < 0 || nY < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nX] [nY] == nB || Game [nG].nBoard [nX] [nY] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nOC))
		{
			break;
		}
	}

	// upper right
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nX = nSX + nI;
		nY = nSY - nI;

		if(nX > 7 || nY < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nX] [nY] == nB || Game [nG].nBoard [nX] [nY] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nOC))
		{
			break;
		}
	}

	// lower left
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nX = nSX - nI;
		nY = nSY + nI;

		if(nX < 0 || nY > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nX] [nY] == nB || Game [nG].nBoard [nX] [nY] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nOC))
		{
			break;
		}
	}

	// lower right
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nX = nSX + nI;
		nY = nSY + nI;

		if(nX > 7 || nY > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nX] [nY] == nB || Game [nG].nBoard [nX] [nY] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nY, nOC))
		{
			break;
		}
	}

	return bCheck;
}

int CHESS_CheckRQ(int nG, int nC, int nOC, int nSX, int nSY, int nR, int nQ, int *nCount, int *bDirect)
{
	int bCheck, nI, nN;

	bCheck = 0;

	// up
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nSY - nI;

		if(nN < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nSX, nN, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nSX] [nN] == nR || Game [nG].nBoard [nSX] [nN] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nSX, nN, nOC))
		{
			break;
		}
	}

	// down
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nSY + nI;

		if(nN > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nSX, nN, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nSX] [nN] == nR || Game [nG].nBoard [nSX] [nN] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nSX, nN, nOC))
		{
			break;
		}
	}

	// left
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nSX - nI;

		if(nN < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nN, nSY, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nN] [nSY] == nR || Game [nG].nBoard [nN] [nSY] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nN, nSY, nOC))
		{
			break;
		}
	}

	// right
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nSX + nI;

		if(nN > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nN, nSY, nC))
		{
			break;
		}

		if(Game [nG].nBoard [nN] [nSY] == nR || Game [nG].nBoard [nN] [nSY] == nQ)
		{
			bCheck  = 1;
			*nCount = *nCount + 1;

			if(nI == 1)
			{
				*bDirect = 1;
			}
			break;
		}

		if(CHESS_IsSameColor(nG, nN, nSY, nOC))
		{
			break;
		}
	}

	return bCheck;
}

int CHESS_CheckN(int nG, int nN, int nSX, int nSY, int *nCount)
{
	int bCheck, nI, nX, nY;

	bCheck = 0;

	for(nI = 0 ; nI < 8 ; nI++)
	{
		nX = nSX + _KnightMoveX [nI];
		nY = nSY + _KnightMoveY [nI];

		if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
		{
			if(Game [nG].nBoard [nX] [nY] == nN)
			{
				bCheck  = 1;
				*nCount = *nCount + 1;
				break;
			}
		}
	}

	return bCheck;
}

int CHESS_CheckWP(int nG, int nSX, int nSY, int *nCount)
{
	int nX, nY;

	nX = nSX - 1;
	nY = nSY - 1;

	if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
	{
		if(Game [nG].nBoard [nX] [nY] == WHITE_PAWN)
		{
			*nCount = *nCount + 1;
			return 1;
		}
	}

	nX = nSX + 1;
	nY = nSY - 1;

	if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
	{
		if(Game [nG].nBoard [nX] [nY] == WHITE_PAWN)
		{
			*nCount = *nCount + 1;
			return 1;
		}
	}

	return 0;
}

int CHESS_CheckBP(int nG, int nSX, int nSY, int *nCount)
{
	int nX, nY;

	nX = nSX - 1;
	nY = nSY + 1;

	if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
	{
		if(Game [nG].nBoard [nX] [nY] == BLACK_PAWN)
		{
			*nCount = *nCount + 1;
			return 1;
		}
	}

	nX = nSX + 1;
	nY = nSY + 1;

	if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
	{
		if(Game [nG].nBoard [nX] [nY] == BLACK_PAWN)
		{
			*nCount = *nCount + 1;
			return 1;
		}
	}

	return 0;
}

int CHESS_CheckK(int nG, int nK, int nSX, int nSY, int *nCount)
{
	int nI, nX, nY;

	for(nI = 0 ; nI < 8 ; nI++)
	{
		nX = nSX + _KingMoveX [nI];
		nY = nSY + _KingMoveY [nI];

		if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
		{
			if(Game [nG].nBoard [nX] [nY] == nK)
			{
				*nCount = *nCount + 1;
				return 1;
			}
		}
	}

	return 0;
}

int CHESS_SquareInCheck(int nG, int nC, int nSX, int nSY, int *nCount, int *bDirect)
{
	*nCount  = 0;
	*bDirect = 0;

	if((nSX < 0) || (nSX > 7) || (nSY < 0) || (nSY > 7))
	{
		return 0;
	}

	if(nC == INDEX_WHITE)
	{
		if(CHESS_CheckBQ(nG, INDEX_WHITE, INDEX_BLACK, nSX, nSY, BLACK_BISHOP, BLACK_QUEEN, nCount, bDirect))
		{
			if((*nCount > 1) || *bDirect)
			{
				return 1;
			}
		}

		if(CHESS_CheckRQ(nG, INDEX_WHITE, INDEX_BLACK, nSX, nSY, BLACK_ROOK, BLACK_QUEEN, nCount, bDirect))
		{
			if((*nCount > 1) || *bDirect)
			{
				return 1;
			}
		}

		if(CHESS_CheckN(nG, BLACK_KNIGHT, nSX, nSY, nCount))
		{
			*bDirect = 1;
			return 1;
		}

		if(CHESS_CheckBP(nG, nSX, nSY, nCount))
		{
			*bDirect = 1;
			return 1;
		}

		if(Game [nG].nGameType != GAMETYPE_FICS_ATOMIC)
		{
			if(CHESS_CheckK(nG, BLACK_KING, nSX, nSY, nCount))
			{
				*bDirect = 1;
				return 1;
			}
		}
	}
	else
	{
		if(CHESS_CheckBQ(nG, INDEX_BLACK, INDEX_WHITE, nSX, nSY, WHITE_BISHOP, WHITE_QUEEN, nCount, bDirect))
		{
			if((*nCount > 1) || *bDirect)
			{
				return 1;
			}
		}

		if(CHESS_CheckRQ(nG, INDEX_BLACK, INDEX_WHITE, nSX, nSY, WHITE_ROOK, WHITE_QUEEN, nCount, bDirect))
		{
			if((*nCount > 1) || *bDirect)
			{
				return 1;
			}
		}

		if(CHESS_CheckN(nG, WHITE_KNIGHT, nSX, nSY, nCount))
		{
			*bDirect = 1;
			return 1;
		}

		if(CHESS_CheckWP(nG, nSX, nSY, nCount))
		{
			*bDirect = 1;
			return 1;
		}

		if(Game [nG].nGameType != GAMETYPE_FICS_ATOMIC)
		{
			if(CHESS_CheckK(nG, WHITE_KING, nSX, nSY, nCount))
			{
				*bDirect = 1;
				return 1;
			}
		}
	}

	return (*nCount > 0);
}

void CHESS_GenKnight(int nG, int nX, int nY, int nC)
{
	int nI, nA, nB;

	for(nI = 0 ; nI < 8 ; nI++)
	{
		nA = nX + _KnightMoveX [nI];
		nB = nY + _KnightMoveY [nI];

		if(nA >= 0 && nA <= 7 && nB >= 0 && nB <= 7)
		{
			if(! CHESS_IsSameColor(nG, nA, nB, nC))
			{
				if(CHESS_IsEmptySquare(nG, nA, nB))
				{
					CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
				}
				else
				{
					CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, Game [nG].nBoard [nA] [nB], EMPTY_SQUARE, EMPTY_SQUARE);
				}
			}
		}
	}
}

void CHESS_GenKing(int nG, int nX, int nY, int nC)
{
	int nI, nA, nB;

	for(nI = 0 ; nI < 8 ; nI++)
	{
		nA = nX + _KingMoveX [nI];
		nB = nY + _KingMoveY [nI];

		if(nA >= 0 && nA <= 7 && nB >= 0 && nB <= 7)
		{
			if(! CHESS_IsSameColor(nG, nA, nB, nC))
			{
				if(CHESS_IsEmptySquare(nG, nA, nB))
				{
					CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
				}
				else
				{
					if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
					{
						if(bInTruePremove)
						{
							CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, Game [nG].nBoard [nA] [nB], EMPTY_SQUARE, EMPTY_SQUARE);
						}
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, Game [nG].nBoard [nA] [nB], EMPTY_SQUARE, EMPTY_SQUARE);
					}
				}
			}
		}
	}
}

void CHESS_GenDiagonal(int nG, int nX, int nY, int nC)
{
	int nI, nA, nB;

	// upper left
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nA = nX - nI;
		nB = nY - nI;

		if(nA < 0 || nB < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nA, nB, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nA, nB))
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, Game [nG].nBoard [nA] [nB], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}

	// upper right
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nA = nX + nI;
		nB = nY - nI;

		if(nA > 7 || nB < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nA, nB, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nA, nB))
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, Game [nG].nBoard [nA] [nB], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}

	// lower left
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nA = nX - nI;
		nB = nY + nI;

		if(nA < 0 || nB > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nA, nB, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nA, nB))
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, Game [nG].nBoard [nA] [nB], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}

	// lower right
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nA = nX + nI;
		nB = nY + nI;

		if(nA > 7 || nB > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nA, nB, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nA, nB))
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nA, nB, MOVE_NORMAL, Game [nG].nBoard [nA] [nB], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}
}

void CHESS_GenHorizontal(int nG, int nX, int nY, int nC)
{
	int nI, nN;

	// up
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nY - nI;

		if(nN < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nN, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nX, nN))
		{
			CHESS_AddTempMove(nX, nY, nX, nN, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nX, nN, MOVE_NORMAL, Game [nG].nBoard [nX] [nN], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}

	// down
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nY + nI;

		if(nN > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nX, nN, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nX, nN))
		{
			CHESS_AddTempMove(nX, nY, nX, nN, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nX, nN, MOVE_NORMAL, Game [nG].nBoard [nX] [nN], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}

	// left
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nX - nI;

		if(nN < 0)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nN, nY, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nN, nY))
		{
			CHESS_AddTempMove(nX, nY, nN, nY, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nN, nY, MOVE_NORMAL, Game [nG].nBoard [nN] [nY], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}

	// right
	for(nI = 1 ; nI < 8 ; nI++)
	{
		nN = nX + nI;

		if(nN > 7)
		{
			break;
		}

		if(CHESS_IsSameColor(nG, nN, nY, nC))
		{
			break;
		}

		if(CHESS_IsEmptySquare(nG, nN, nY))
		{
			CHESS_AddTempMove(nX, nY, nN, nY, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}
		else
		{
			CHESS_AddTempMove(nX, nY, nN, nY, MOVE_NORMAL, Game [nG].nBoard [nN] [nY], EMPTY_SQUARE, EMPTY_SQUARE);

			if(! bInTruePremove)
			{
				break;
			}
		}
	}
}

void CHESS_GenWhitePawn(int nG, int nX, int nY)
{
	// single pawn push
	if(bInTruePremove || CHESS_IsEmptySquare(nG, nX, nY + 1))
	{
		if(nY == 6)
		{
			// promotion
			if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
			   (Game [nG].nGameType == GAMETYPE_GIVEAWAY))
			{
				if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY))
				{
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, System.nPromoteWPiece);
				}
				else
				{
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_KING);

					if(bInIsLegalMoveString || (! User.bAutoQueen))
					{
						CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_QUEEN);
						CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_ROOK);
						CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_BISHOP);
					}
				}
			}
			else if(Game [nG].nGameType == GAMETYPE_LOSER)
			{
				if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteWPiece != WHITE_KING))
				{
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, System.nPromoteWPiece);
				}
				else
				{
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_QUEEN);
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_ROOK);
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_KNIGHT);
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_BISHOP);
				}
			}
			else
			{
				if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteWPiece != WHITE_KING))
				{
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, System.nPromoteWPiece);
				}
				else
				{
					CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_QUEEN);

					if(bInIsLegalMoveString || (! User.bAutoQueen))
					{
						CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_ROOK);
						CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX, 7, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, WHITE_BISHOP);
					}
				}
			}
		}
		else
		{
			// single pawn push
			CHESS_AddTempMove(nX, nY, nX, nY + 1, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}

		// at home position
		if(nY == 1)
		{
			// double pawn push
			if(bInTruePremove || CHESS_IsEmptySquare(nG, nX, 3))
			{
				CHESS_AddTempMove(nX, nY, nX, 3, MOVE_DBLPUSH, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}

	// capture to the left
	if(nX > 0)
	{
		if(bInTruePremove || CHESS_IsBlackPiece(nG, nX - 1, nY + 1))
		{
			if(nY == 6)
			{
				// promotion capture
				if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
				   (Game [nG].nGameType == GAMETYPE_GIVEAWAY))
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY))
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, System.nPromoteWPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_KING);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_QUEEN);
							CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_ROOK);
							CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_BISHOP);
						}
					}
				}
				else if(Game [nG].nGameType == GAMETYPE_LOSER)
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteWPiece != WHITE_KING))
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, System.nPromoteWPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_QUEEN);
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_ROOK);
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_BISHOP);
					}
				}
				else
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteWPiece != WHITE_KING))
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, System.nPromoteWPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_QUEEN);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_ROOK);
							CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX - 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [7], EMPTY_SQUARE, WHITE_BISHOP);
						}
					}
				}
			}
			else
			{
				// just capture
				CHESS_AddTempMove(nX, nY, nX - 1, nY + 1, MOVE_NORMAL, Game [nG].nBoard [nX - 1] [nY + 1], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}

	// capture to the right
	if(nX < 7)
	{
		if(bInTruePremove || CHESS_IsBlackPiece(nG, nX + 1, nY + 1))
		{
			if(nY == 6)
			{
				// promotion capture
				if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
						(Game [nG].nGameType == GAMETYPE_GIVEAWAY))
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY))
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, System.nPromoteWPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_KING);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_QUEEN);
							CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_ROOK);
							CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_BISHOP);
						}
					}
				}
				else if(Game [nG].nGameType == GAMETYPE_LOSER)
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteWPiece != WHITE_KING))
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, System.nPromoteWPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_QUEEN);
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_ROOK);
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_BISHOP);
					}
				}
				else
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteWPiece != WHITE_KING))
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, System.nPromoteWPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_QUEEN);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_ROOK);
							CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX + 1, 7, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [7], EMPTY_SQUARE, WHITE_BISHOP);
						}
					}
				}
			}
			else
			{
				// just capture
				CHESS_AddTempMove(nX, nY, nX + 1, nY + 1, MOVE_NORMAL, Game [nG].nBoard [nX + 1] [nY + 1], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}

	// en pasant
	if(nY == 4)
	{
		// to the left
		if(nX > 0)
		{
			if(bInTruePremove || (Game [nG].nLastDoublePushFile == nX - 1))
			{
				CHESS_AddTempMove(nX, nY, nX - 1, 5, MOVE_ENPASANT, Game [nG].nBoard [nX - 1] [4], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}

		// to the right
		if(nX < 7)
		{
			if(bInTruePremove || (Game [nG].nLastDoublePushFile == nX + 1))
			{
				CHESS_AddTempMove(nX, nY, nX + 1, 5, MOVE_ENPASANT, Game [nG].nBoard [nX + 1] [4], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}
}

void CHESS_GenWhiteKing(int nG, int nX, int nY)
{
	int nCount = 0, bDirect = 0;

	// king moves
	CHESS_GenKing(nG, nX, nY, INDEX_WHITE);

	// certain wild games can not castle
	if((Game [nG].nGameType == GAMETYPE_WILD5) ||
			(Game [nG].nGameType == GAMETYPE_SUICIDE) ||
			(Game [nG].nGameType == GAMETYPE_GIVEAWAY) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD7) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD15) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD18) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD19) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD28))
	{
		return;
	}

	// o-o
	if(Game [nG].bCanCastleKingSide [INDEX_WHITE])
	{
		// in original position
		if(nX == 4 && nY == 0)
		{
			if(bInTruePremove)
			{
				CHESS_AddTempMove(4, 0, 6, 0, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
			}
			else
			{
				if(Game [nG].nBoard [5] [0] == EMPTY_SQUARE &&
						Game [nG].nBoard [6] [0] == EMPTY_SQUARE &&
						Game [nG].nBoard [7] [0] == WHITE_ROOK)
				{
					if(! CHESS_SquareInCheck(nG, INDEX_WHITE, 4, 0, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_WHITE, 5, 0, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_WHITE, 6, 0, &nCount, &bDirect))
					{
						CHESS_AddTempMove(4, 0, 6, 0, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
					}
				}
			}
		}

		//
		// only on ICC
		//
		if(Login.nLoginType == SERVER_ICC)
		{
			if(nX == 3 && nY == 0)
			{
				if(bInTruePremove)
				{
					CHESS_AddTempMove(3, 0, 1, 0, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
				}
				else
				{
					if(Game [nG].nBoard [2] [0] == EMPTY_SQUARE &&
							Game [nG].nBoard [1] [0] == EMPTY_SQUARE &&
							Game [nG].nBoard [0] [0] == WHITE_ROOK)
					{
						if(! CHESS_SquareInCheck(nG, INDEX_WHITE, 3, 0, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_WHITE, 2, 0, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_WHITE, 1, 0, &nCount, &bDirect))
						{
							CHESS_AddTempMove(3, 0, 1, 0, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
						}
					}
				}
			}
		}
	}

	// o-o-o
	if(Game [nG].bCanCastleQueenSide [INDEX_WHITE])
	{
		// in original position
		if(nX == 4 && nY == 0)
		{
			if(bInTruePremove)
			{
				CHESS_AddTempMove(4, 0, 2, 0, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
			}
			else
			{
				if(Game [nG].nBoard [3] [0] == EMPTY_SQUARE &&
						Game [nG].nBoard [2] [0] == EMPTY_SQUARE &&
						Game [nG].nBoard [1] [0] == EMPTY_SQUARE &&
						Game [nG].nBoard [0] [0] == WHITE_ROOK)
				{
					if(! CHESS_SquareInCheck(nG, INDEX_WHITE, 4, 0, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_WHITE, 3, 0, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_WHITE, 2, 0, &nCount, &bDirect))
					{
						CHESS_AddTempMove(4, 0, 2, 0, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
					}
				}
			}
		}

		//
		// only on ICC
		//
		if(Login.nLoginType == SERVER_ICC)
		{
			if(nX == 3 && nY == 0)
			{
				if(bInTruePremove)
				{
					CHESS_AddTempMove(3, 0, 5, 0, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
				}
				else
				{
					if(Game [nG].nBoard [4] [0] == EMPTY_SQUARE &&
							Game [nG].nBoard [5] [0] == EMPTY_SQUARE &&
							Game [nG].nBoard [6] [0] == EMPTY_SQUARE &&
							Game [nG].nBoard [7] [0] == WHITE_ROOK)
					{
						if(! CHESS_SquareInCheck(nG, INDEX_WHITE, 3, 0, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_WHITE, 4, 0, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_WHITE, 5, 0, &nCount, &bDirect))
						{
							CHESS_AddTempMove(3, 0, 5, 0, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
						}
					}
				}
			}
		}
	}
}

void CHESS_GenBlackPawn(int nG, int nX, int nY)
{
	// single pawn push
	if(bInTruePremove || CHESS_IsEmptySquare(nG, nX, nY - 1))
	{
		if(nY == 1)
		{
			// promotion
			if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
					(Game [nG].nGameType == GAMETYPE_GIVEAWAY))
			{
				if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY))
				{
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, System.nPromoteBPiece);
				}
				else
				{
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_KING);

					if(bInIsLegalMoveString || (! User.bAutoQueen))
					{
						CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_QUEEN);
						CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_ROOK);
						CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_BISHOP);
					}
				}
			}
			else if(Game [nG].nGameType == GAMETYPE_LOSER)
			{
				if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteBPiece != BLACK_KING))
				{
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, System.nPromoteBPiece);
				}
				else
				{
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_QUEEN);
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_ROOK);
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_KNIGHT);
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_BISHOP);
				}
			}
			else
			{
				if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteBPiece != BLACK_KING))
				{
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, System.nPromoteBPiece);
				}
				else
				{
					CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_QUEEN);

					if(bInIsLegalMoveString || (! User.bAutoQueen))
					{
						CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_ROOK);
						CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX, 0, MOVE_PROMOTE, EMPTY_SQUARE, EMPTY_SQUARE, BLACK_BISHOP);
					}
				}
			}
		}
		else
		{
			// single pawn push
			CHESS_AddTempMove(nX, nY, nX, nY - 1, MOVE_NORMAL, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
		}

		// at home position
		if(nY == 6)
		{
			// double pawn push
			if(bInTruePremove || CHESS_IsEmptySquare(nG, nX, 4))
			{
				CHESS_AddTempMove(nX, nY, nX, 4, MOVE_DBLPUSH, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}

	// capture to the left
	if(nX > 0)
	{
		if(bInTruePremove || CHESS_IsWhitePiece(nG, nX - 1, nY - 1))
		{
			if(nY == 1)
			{
				// promotion capture
				if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
						(Game [nG].nGameType == GAMETYPE_GIVEAWAY))
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY))
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, System.nPromoteBPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_KING);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_QUEEN);
							CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_ROOK);
							CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_BISHOP);
						}
					}
				}
				else if(Game [nG].nGameType == GAMETYPE_LOSER)
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteBPiece != BLACK_KING))
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, System.nPromoteBPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_QUEEN);
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_ROOK);
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_BISHOP);
					}
				}
				else
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteBPiece != BLACK_KING))
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, System.nPromoteBPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_QUEEN);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_ROOK);
							CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX - 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX - 1] [0], EMPTY_SQUARE, BLACK_BISHOP);
						}
					}
				}
			}
			else
			{
				// just capture
				CHESS_AddTempMove(nX, nY, nX - 1, nY - 1, MOVE_NORMAL, Game [nG].nBoard [nX - 1] [nY - 1], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}

	// capture to the right
	if(nX < 7)
	{
		if(bInTruePremove || CHESS_IsWhitePiece(nG, nX + 1, nY - 1))
		{
			if(nY == 1)
			{
				// promotion capture
				if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
						(Game [nG].nGameType == GAMETYPE_GIVEAWAY))
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY))
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, System.nPromoteBPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_KING);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_QUEEN);
							CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_ROOK);
							CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_BISHOP);
						}
					}
				}
				else if(Game [nG].nGameType == GAMETYPE_LOSER)
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteBPiece != BLACK_KING))
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, System.nPromoteBPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_QUEEN);
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_ROOK);
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_KNIGHT);
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_BISHOP);
					}
				}
				else
				{
					if(System.bPromoteCommand && User.bAutoQueen && (nG == INDEX_PLAY) && (System.nPromoteBPiece != BLACK_KING))
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, System.nPromoteBPiece);
					}
					else
					{
						CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_QUEEN);

						if(bInIsLegalMoveString || (! User.bAutoQueen))
						{
							CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_ROOK);
							CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_KNIGHT);
							CHESS_AddTempMove(nX, nY, nX + 1, 0, MOVE_PROMOTE, Game [nG].nBoard [nX + 1] [0], EMPTY_SQUARE, BLACK_BISHOP);
						}
					}
				}
			}
			else
			{
				// just capture
				CHESS_AddTempMove(nX, nY, nX + 1, nY - 1, MOVE_NORMAL, Game [nG].nBoard [nX + 1] [nY - 1], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}

	// en pasant
	if(nY == 3)
	{
		// to the left
		if(nX > 0)
		{
			if(bInTruePremove || (Game [nG].nLastDoublePushFile == nX - 1))
			{
				CHESS_AddTempMove(nX, nY, nX - 1, 2, MOVE_ENPASANT, Game [nG].nBoard [nX - 1] [3], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}

		// to the right
		if(nX < 7)
		{
			if(bInTruePremove || (Game [nG].nLastDoublePushFile == nX + 1))
			{
				CHESS_AddTempMove(nX, nY, nX + 1, 2, MOVE_ENPASANT, Game [nG].nBoard [nX + 1] [3], EMPTY_SQUARE, EMPTY_SQUARE);
			}
		}
	}
}

void CHESS_GenBlackKing(int nG, int nX, int nY)
{
	int nCount = 0, bDirect = 0;

	// king moves
	CHESS_GenKing(nG, nX, nY, INDEX_BLACK);

	// certain wild games can not castle
	if((Game [nG].nGameType == GAMETYPE_WILD5) ||
			(Game [nG].nGameType == GAMETYPE_SUICIDE) ||
			(Game [nG].nGameType == GAMETYPE_GIVEAWAY) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD7) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD15) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD18) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD19) ||
			(Game [nG].nGameType == GAMETYPE_ICC_WILD28))
	{
		return;
	}

	// o-o
	if(Game [nG].bCanCastleKingSide [INDEX_BLACK])
	{
		// in original position
		if(nX == 4 && nY == 7)
		{
			if(bInTruePremove)
			{
				CHESS_AddTempMove(4, 7, 6, 7, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
			}
			else
			{
				if(Game [nG].nBoard [5] [7] == EMPTY_SQUARE &&
						Game [nG].nBoard [6] [7] == EMPTY_SQUARE &&
						Game [nG].nBoard [7] [7] == BLACK_ROOK)
				{
					if(! CHESS_SquareInCheck(nG, INDEX_BLACK, 4, 7, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_BLACK, 5, 7, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_BLACK, 6, 7, &nCount, &bDirect))
					{
						CHESS_AddTempMove(4, 7, 6, 7, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
					}
				}
			}
		}

		//
		// only on ICC
		//
		if(Login.nLoginType == SERVER_ICC)
		{
			if(nX == 3 && nY == 7)
			{
				if(bInTruePremove)
				{
					CHESS_AddTempMove(3, 7, 1, 7, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
				}
				else
				{
					if(Game [nG].nBoard [2] [7] == EMPTY_SQUARE &&
							Game [nG].nBoard [1] [7] == EMPTY_SQUARE &&
							Game [nG].nBoard [0] [7] == BLACK_ROOK)
					{
						if(! CHESS_SquareInCheck(nG, INDEX_BLACK, 3, 7, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_BLACK, 2, 7, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_BLACK, 1, 7, &nCount, &bDirect))
						{
							CHESS_AddTempMove(3, 7, 1, 7, MOVE_OO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
						}
					}
				}
			}
		}
	}

	// o-o-o
	if(Game [nG].bCanCastleQueenSide [INDEX_BLACK])
	{
		// in original position
		if(nX == 4 && nY == 7)
		{
			if(bInTruePremove)
			{
				CHESS_AddTempMove(4, 7, 2, 7, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
			}
			else
			{
				if(Game [nG].nBoard [3] [7] == EMPTY_SQUARE &&
						Game [nG].nBoard [2] [7] == EMPTY_SQUARE &&
						Game [nG].nBoard [1] [7] == EMPTY_SQUARE &&
						Game [nG].nBoard [0] [7] == BLACK_ROOK)
				{
					if(! CHESS_SquareInCheck(nG, INDEX_BLACK, 4, 7, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_BLACK, 3, 7, &nCount, &bDirect) &&
							! CHESS_SquareInCheck(nG, INDEX_BLACK, 2, 7, &nCount, &bDirect))
					{
						CHESS_AddTempMove(4, 7, 2, 7, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
					}
				}
			}
		}

		//
		// only on ICC
		//
		if(Login.nLoginType == SERVER_ICC)
		{
			if(nX == 3 && nY == 7)
			{
				if(bInTruePremove)
				{
					CHESS_AddTempMove(3, 7, 5, 7, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
				}
				else
				{
					if(Game [nG].nBoard [4] [7] == EMPTY_SQUARE &&
							Game [nG].nBoard [5] [7] == EMPTY_SQUARE &&
							Game [nG].nBoard [6] [7] == EMPTY_SQUARE &&
							Game [nG].nBoard [7] [7] == BLACK_ROOK)
					{
						if(! CHESS_SquareInCheck(nG, INDEX_BLACK, 3, 7, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_BLACK, 4, 7, &nCount, &bDirect) &&
								! CHESS_SquareInCheck(nG, INDEX_BLACK, 5, 7, &nCount, &bDirect))
						{
							CHESS_AddTempMove(3, 7, 5, 7, MOVE_OOO, EMPTY_SQUARE, EMPTY_SQUARE, EMPTY_SQUARE);
						}
					}
				}
			}
		}
	}
}

void CHESS_BackupPosition(int nG)
{
	int nX, nY;

	// backup board
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			nTempBoard   [nX] [nY] = Game [nG].nBoard        [nX] [nY];
			nTempPromote [nX] [nY] = Game [nG].nPromoteBoard [nX] [nY];
		}
	}

	// backup piece pile
	for(nX = WHITE_PAWN ; nX <= BLACK_QUEEN ; nX++)
	{
		nTempBuffer [nX] = Game [nG].nBuffer [nX];
	}

	// backup whites move
	bTempWhitesMove = Game [nG].bWhitesMove;

	// backup last pawn double push file number
	nTempLastDoublePushFile = Game [nG].nLastDoublePushFile;

	// backup can castle flags
	bTempCanCastleKingSide  [INDEX_WHITE] = Game [nG].bCanCastleKingSide  [INDEX_WHITE];
	bTempCanCastleKingSide  [INDEX_BLACK] = Game [nG].bCanCastleKingSide  [INDEX_BLACK];
	bTempCanCastleQueenSide [INDEX_WHITE] = Game [nG].bCanCastleQueenSide [INDEX_WHITE];
	bTempCanCastleQueenSide [INDEX_BLACK] = Game [nG].bCanCastleQueenSide [INDEX_BLACK];

	// backup king positions
	nTempKingX [INDEX_WHITE] = Game [nG].nKingX [INDEX_WHITE];
	nTempKingY [INDEX_WHITE] = Game [nG].nKingY [INDEX_WHITE];
	nTempKingX [INDEX_BLACK] = Game [nG].nKingX [INDEX_BLACK];
	nTempKingY [INDEX_BLACK] = Game [nG].nKingY [INDEX_BLACK];

	// backup initial move
	bTempInitialMove = Game [nG].bInitialMove;
}

void CHESS_RestorePosition(int nG)
{
	int nX, nY;

	// restore board
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			Game [nG].nBoard        [nX] [nY] = nTempBoard   [nX] [nY];
			Game [nG].nPromoteBoard [nX] [nY] = nTempPromote [nX] [nY];
		}
	}

	// restore piece pile
	for(nX = WHITE_PAWN ; nX <= BLACK_QUEEN ; nX++)
	{
		Game [nG].nBuffer [nX] = nTempBuffer [nX];
	}

	// restore whites move
	Game [nG].bWhitesMove = bTempWhitesMove;

	// temporary last pawn double push file number
	Game [nG].nLastDoublePushFile = nTempLastDoublePushFile;

	// restore can castle flags
	Game [nG].bCanCastleKingSide  [INDEX_WHITE] = bTempCanCastleKingSide  [INDEX_WHITE];
	Game [nG].bCanCastleKingSide  [INDEX_BLACK] = bTempCanCastleKingSide  [INDEX_BLACK];
	Game [nG].bCanCastleQueenSide [INDEX_WHITE] = bTempCanCastleQueenSide [INDEX_WHITE];
	Game [nG].bCanCastleQueenSide [INDEX_BLACK] = bTempCanCastleQueenSide [INDEX_BLACK];

	// restore king positions
	Game [nG].nKingX [INDEX_WHITE] = nTempKingX [INDEX_WHITE];
	Game [nG].nKingY [INDEX_WHITE] = nTempKingY [INDEX_WHITE];
	Game [nG].nKingX [INDEX_BLACK] = nTempKingX [INDEX_BLACK];
	Game [nG].nKingY [INDEX_BLACK] = nTempKingY [INDEX_BLACK];

	// restore initial move
	Game [nG].bInitialMove = bTempInitialMove;
}

void CHESS_MakeMove(int nG, int nFX, int nFY, int nTX, int nTY, int nType, int nCapture, int nDrop, int nPromote)
{
	int nCP;

	switch(nType)
	{
		// normal move (including capture)
		case MOVE_NORMAL :
			if(nCapture == EMPTY_SQUARE)
			{
				// no capture
				Game [nG].nBoard [nTX] [nTY] = Game [nG].nBoard [nFX] [nFY];
				Game [nG].nBoard [nFX] [nFY] = EMPTY_SQUARE;
			}
			else
			{
				// capture
				Game [nG].nBoard [nTX] [nTY] = Game [nG].nBoard [nFX] [nFY];
				Game [nG].nBoard [nFX] [nFY] = EMPTY_SQUARE;

				// crazyhouse piece buffer update
				if(Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
				{
					if(Game [nG].nPromoteBoard [nTX] [nTY])
					{
						switch(nCapture)
						{
							case WHITE_PAWN :
							case WHITE_KNIGHT :
							case WHITE_BISHOP :
							case WHITE_ROOK :
							case WHITE_QUEEN :
								nCP = WHITE_PAWN;
								break;

							case BLACK_PAWN :
							case BLACK_KNIGHT :
							case BLACK_BISHOP :
							case BLACK_ROOK :
							case BLACK_QUEEN :
								nCP = BLACK_PAWN;
								break;

							default :
								nCP = nCapture;
								break;
						}
					}
					else
					{
						nCP = nCapture;
					}

					switch(nCP)
					{
						case WHITE_PAWN :
							Game [nG].nBuffer [BLACK_PAWN  ]++;
							break;
						case WHITE_KNIGHT :
							Game [nG].nBuffer [BLACK_KNIGHT]++;
							break;
						case WHITE_BISHOP :
							Game [nG].nBuffer [BLACK_BISHOP]++;
							break;
						case WHITE_ROOK :
							Game [nG].nBuffer [BLACK_ROOK  ]++;
							break;
						case WHITE_QUEEN :
							Game [nG].nBuffer [BLACK_QUEEN ]++;
							break;

						case BLACK_PAWN :
							Game [nG].nBuffer [WHITE_PAWN  ]++;
							break;
						case BLACK_KNIGHT :
							Game [nG].nBuffer [WHITE_KNIGHT]++;
							break;
						case BLACK_BISHOP :
							Game [nG].nBuffer [WHITE_BISHOP]++;
							break;
						case BLACK_ROOK :
							Game [nG].nBuffer [WHITE_ROOK  ]++;
							break;
						case BLACK_QUEEN :
							Game [nG].nBuffer [WHITE_QUEEN ]++;
							break;
					}
				}
				else if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
				{
					CHESS_AtomicCapture(nG, nTX, nTY);
				}
				else if(Game [nG].bChessGame)   // chess game buffer update
				{
					if(Game [nG].nPromoteBoard [nTX] [nTY])
					{
						switch(nCapture)
						{
							case WHITE_PAWN :
							case WHITE_KNIGHT :
							case WHITE_BISHOP :
							case WHITE_ROOK :
							case WHITE_QUEEN :
								nCP = WHITE_PAWN;
								break;

							case BLACK_PAWN :
							case BLACK_KNIGHT :
							case BLACK_BISHOP :
							case BLACK_ROOK :
							case BLACK_QUEEN :
								nCP = BLACK_PAWN;
								break;

							default :
								nCP = nCapture;
								break;
						}
					}
					else
					{
						nCP = nCapture;
					}

					switch(nCP)
					{
						case WHITE_PAWN :
							Game [nG].nBuffer [WHITE_PAWN  ]++;
							break;
						case WHITE_KNIGHT :
							Game [nG].nBuffer [WHITE_KNIGHT]++;
							break;
						case WHITE_BISHOP :
							Game [nG].nBuffer [WHITE_BISHOP]++;
							break;
						case WHITE_ROOK :
							Game [nG].nBuffer [WHITE_ROOK  ]++;
							break;
						case WHITE_QUEEN :
							Game [nG].nBuffer [WHITE_QUEEN ]++;
							break;

						case BLACK_PAWN :
							Game [nG].nBuffer [BLACK_PAWN  ]++;
							break;
						case BLACK_KNIGHT :
							Game [nG].nBuffer [BLACK_KNIGHT]++;
							break;
						case BLACK_BISHOP :
							Game [nG].nBuffer [BLACK_BISHOP]++;
							break;
						case BLACK_ROOK :
							Game [nG].nBuffer [BLACK_ROOK  ]++;
							break;
						case BLACK_QUEEN :
							Game [nG].nBuffer [BLACK_QUEEN ]++;
							break;
					}
				}
			}

			if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
			{

				// adjust promote board
				Game [nG].nPromoteBoard [nTX] [nTY] = 0;
				Game [nG].nPromoteBoard [nFX] [nFY] = 0;
			}
			else
			{

				// adjust promote board
				Game [nG].nPromoteBoard [nTX] [nTY] = Game [nG].nPromoteBoard [nFX] [nFY];
				Game [nG].nPromoteBoard [nFX] [nFY] = 0;
			}

			// adjust can castle flags and king position (if applicable)
			if(Game [nG].nBoard [nTX] [nTY] == WHITE_KING)
			{
				Game [nG].bCanCastleKingSide  [INDEX_WHITE] = 0;
				Game [nG].bCanCastleQueenSide [INDEX_WHITE] = 0;

				if(Game [nG].nGameType == GAMETYPE_ICC_WILD9)
				{
					BOARD_FindICCWild9Kings(nG);
				}
				else
				{
					Game [nG].nKingX [INDEX_WHITE] = nTX;
					Game [nG].nKingY [INDEX_WHITE] = nTY;
				}
			}
			else if(Game [nG].nBoard [nTX] [nTY] == WHITE_ROOK)
			{
				if(nFX == 7 && nFY == 0)
				{
					Game [nG].bCanCastleKingSide [INDEX_WHITE] = 0;
				}
				else if(nFX == 0 && nFY == 0)
				{
					Game [nG].bCanCastleQueenSide [INDEX_WHITE] = 0;
				}
			}
			else if(Game [nG].nBoard [nTX] [nTY] == BLACK_KING)
			{
				Game [nG].bCanCastleKingSide  [INDEX_BLACK] = 0;
				Game [nG].bCanCastleQueenSide [INDEX_BLACK] = 0;

				if(Game [nG].nGameType == GAMETYPE_ICC_WILD9)
				{
					BOARD_FindICCWild9Kings(nG);
				}
				else
				{
					Game [nG].nKingX [INDEX_BLACK] = nTX;
					Game [nG].nKingY [INDEX_BLACK] = nTY;
				}
			}
			else if(Game [nG].nBoard [nTX] [nTY] == BLACK_ROOK)
			{
				if(nFX == 7 && nFY == 7)
				{
					Game [nG].bCanCastleKingSide [INDEX_BLACK] = 0;
				}
				else if(nFX == 0 && nFY == 7)
				{
					Game [nG].bCanCastleQueenSide [INDEX_BLACK] = 0;
				}
			}

			// adjust last double push file
			Game [nG].nLastDoublePushFile = -1;
			break;

		// drop move
		case MOVE_DROP :
			Game [nG].nBoard [nTX] [nTY] = nDrop;
			Game [nG].nBuffer [nDrop]    = Game [nG].nBuffer [nDrop] - 1;

			// adjust promote board
			Game [nG].nPromoteBoard [nTX] [nTY] = 0;

			// adjust last double push file
			Game [nG].nLastDoublePushFile = -1;
			break;

		// o-o
		case MOVE_OO :
			Game [nG].nBoard [nTX] [nTY] = Game [nG].nBoard [nFX] [nFY];
			Game [nG].nBoard [nFX] [nFY] = EMPTY_SQUARE;

			// adjust promote board
			Game [nG].nPromoteBoard [nTX] [nTY] = 0;
			Game [nG].nPromoteBoard [nFX] [nFY] = 0;

			if(nFX == 4)
			{
				Game [nG].nBoard [5] [nFY] = Game [nG].nBoard [7] [nFY];
				Game [nG].nBoard [7] [nFY] = EMPTY_SQUARE;

				// adjust promote board
				Game [nG].nPromoteBoard [5] [nFY] = 0;
				Game [nG].nPromoteBoard [7] [nFY] = 0;
			}
			else if(nFX == 3)
			{
				Game [nG].nBoard [2] [nFY] = Game [nG].nBoard [0] [nFY];
				Game [nG].nBoard [0] [nFY] = EMPTY_SQUARE;

				// adjust promote board
				Game [nG].nPromoteBoard [2] [nFY] = 0;
				Game [nG].nPromoteBoard [0] [nFY] = 0;
			}

			// adjust can castle flags and king position
			if(Game [nG].nBoard [nTX] [nTY] == WHITE_KING)
			{
				Game [nG].bCanCastleKingSide  [INDEX_WHITE] = 0;
				Game [nG].bCanCastleQueenSide [INDEX_WHITE] = 0;

				if(Game [nG].nGameType == GAMETYPE_ICC_WILD9)
				{
					BOARD_FindICCWild9Kings(nG);
				}
				else
				{
					Game [nG].nKingX [INDEX_WHITE] = nTX;
					Game [nG].nKingY [INDEX_WHITE] = nTY;
				}
			}
			else if(Game [nG].nBoard [nTX] [nTY] == BLACK_KING)
			{
				Game [nG].bCanCastleKingSide  [INDEX_BLACK] = 0;
				Game [nG].bCanCastleQueenSide [INDEX_BLACK] = 0;

				if(Game [nG].nGameType == GAMETYPE_ICC_WILD9)
				{
					BOARD_FindICCWild9Kings(nG);
				}
				else
				{
					Game [nG].nKingX [INDEX_BLACK] = nTX;
					Game [nG].nKingY [INDEX_BLACK] = nTY;
				}
			}

			// adjust last double push file
			Game [nG].nLastDoublePushFile = -1;
			break;

		// o-o-o
		case MOVE_OOO :
			Game [nG].nBoard [nTX] [nTY] = Game [nG].nBoard [nFX] [nFY];
			Game [nG].nBoard [nFX] [nFY] = EMPTY_SQUARE;

			// adjust promote board
			Game [nG].nPromoteBoard [nTX] [nTY] = 0;
			Game [nG].nPromoteBoard [nFX] [nFY] = 0;

			if(nFX == 4)
			{
				Game [nG].nBoard [3] [nFY] = Game [nG].nBoard [0] [nFY];
				Game [nG].nBoard [0] [nFY] = EMPTY_SQUARE;

				// adjust promote board
				Game [nG].nPromoteBoard [3] [nFY] = 0;
				Game [nG].nPromoteBoard [0] [nFY] = 0;
			}
			else if(nFX == 3)
			{
				Game [nG].nBoard [4] [nFY] = Game [nG].nBoard [7] [nFY];
				Game [nG].nBoard [7] [nFY] = EMPTY_SQUARE;

				// adjust promote board
				Game [nG].nPromoteBoard [4] [nFY] = 0;
				Game [nG].nPromoteBoard [7] [nFY] = 0;
			}

			// adjust can castle flags and king position
			if(Game [nG].nBoard [nTX] [nTY] == WHITE_KING)
			{
				Game [nG].bCanCastleKingSide  [INDEX_WHITE] = 0;
				Game [nG].bCanCastleQueenSide [INDEX_WHITE] = 0;

				if(Game [nG].nGameType == GAMETYPE_ICC_WILD9)
				{
					BOARD_FindICCWild9Kings(nG);
				}
				else
				{
					Game [nG].nKingX [INDEX_WHITE] = nTX;
					Game [nG].nKingY [INDEX_WHITE] = nTY;
				}
			}
			else if(Game [nG].nBoard [nTX] [nTY] == BLACK_KING)
			{
				Game [nG].bCanCastleKingSide  [INDEX_BLACK] = 0;
				Game [nG].bCanCastleQueenSide [INDEX_BLACK] = 0;

				if(Game [nG].nGameType == GAMETYPE_ICC_WILD9)
				{
					BOARD_FindICCWild9Kings(nG);
				}
				else
				{
					Game [nG].nKingX [INDEX_BLACK] = nTX;
					Game [nG].nKingY [INDEX_BLACK] = nTY;
				}
			}

			// adjust last double push file
			Game [nG].nLastDoublePushFile = -1;
			break;

		// double pawn push
		case MOVE_DBLPUSH :
			Game [nG].nBoard [nTX] [nTY] = Game [nG].nBoard [nFX] [nFY];
			Game [nG].nBoard [nFX] [nFY] = EMPTY_SQUARE;

			// adjust promote board
			Game [nG].nPromoteBoard [nTX] [nTY] = 0;
			Game [nG].nPromoteBoard [nFX] [nFY] = 0;

			// adjust last double push file
			Game [nG].nLastDoublePushFile = nTX;
			break;

		// en pasant
		case MOVE_ENPASANT :
			Game [nG].nBoard [nTX] [nTY] = Game [nG].nBoard [nFX] [nFY];
			Game [nG].nBoard [nFX] [nFY] = EMPTY_SQUARE;
			Game [nG].nBoard [nTX] [nFY] = EMPTY_SQUARE;

			// adjust promote board
			Game [nG].nPromoteBoard [nTX] [nTY] = 0;
			Game [nG].nPromoteBoard [nFX] [nFY] = 0;
			Game [nG].nPromoteBoard [nTX] [nFY] = 0;

			// adjust last double push file
			Game [nG].nLastDoublePushFile = -1;

			// crazyhouse piece buffer update
			if(Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
			{
				switch(nCapture)
				{
					case WHITE_PAWN :
						Game [nG].nBuffer [BLACK_PAWN]++;
						break;

					case BLACK_PAWN :
						Game [nG].nBuffer [WHITE_PAWN]++;
						break;
				}
			}
			else if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
			{
				CHESS_AtomicCapture(nG, nTX, nTY);
			}
			else if(Game [nG].bChessGame)   // chess game buffer update
			{
				switch(nCapture)
				{
					case WHITE_PAWN :
						Game [nG].nBuffer [WHITE_PAWN]++;
						break;

					case BLACK_PAWN :
						Game [nG].nBuffer [BLACK_PAWN]++;
						break;
				}
			}
			break;

		// promotion
		case MOVE_PROMOTE :
			Game [nG].nBoard [nFX] [nFY] = EMPTY_SQUARE;
			Game [nG].nBoard [nTX] [nTY] = nPromote;

			if(nCapture != EMPTY_SQUARE)
			{
				// crazyhouse piece buffer update
				if(Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
				{
					if(Game [nG].nPromoteBoard [nTX] [nTY])
					{
						switch(nCapture)
						{
							case WHITE_PAWN :
							case WHITE_KNIGHT :
							case WHITE_BISHOP :
							case WHITE_ROOK :
							case WHITE_QUEEN :
								nCP = WHITE_PAWN;
								break;

							case BLACK_PAWN :
							case BLACK_KNIGHT :
							case BLACK_BISHOP :
							case BLACK_ROOK :
							case BLACK_QUEEN :
								nCP = BLACK_PAWN;
								break;

							default :
								nCP = nCapture;
								break;
						}
					}
					else
					{
						nCP = nCapture;
					}

					switch(nCP)
					{
						case WHITE_PAWN :
							Game [nG].nBuffer [BLACK_PAWN  ]++;
							break;
						case WHITE_KNIGHT :
							Game [nG].nBuffer [BLACK_KNIGHT]++;
							break;
						case WHITE_BISHOP :
							Game [nG].nBuffer [BLACK_BISHOP]++;
							break;
						case WHITE_ROOK :
							Game [nG].nBuffer [BLACK_ROOK  ]++;
							break;
						case WHITE_QUEEN :
							Game [nG].nBuffer [BLACK_QUEEN ]++;
							break;

						case BLACK_PAWN :
							Game [nG].nBuffer [WHITE_PAWN  ]++;
							break;
						case BLACK_KNIGHT :
							Game [nG].nBuffer [WHITE_KNIGHT]++;
							break;
						case BLACK_BISHOP :
							Game [nG].nBuffer [WHITE_BISHOP]++;
							break;
						case BLACK_ROOK :
							Game [nG].nBuffer [WHITE_ROOK  ]++;
							break;
						case BLACK_QUEEN :
							Game [nG].nBuffer [WHITE_QUEEN ]++;
							break;
					}
				}
				else if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
				{
					CHESS_AtomicCapture(nG, nTX, nTY);
				}
				else if(Game [nG].bChessGame)   // chess game buffer update
				{
					if(Game [nG].nPromoteBoard [nTX] [nTY])
					{
						switch(nCapture)
						{
							case WHITE_PAWN :
							case WHITE_KNIGHT :
							case WHITE_BISHOP :
							case WHITE_ROOK :
							case WHITE_QUEEN :
								nCP = WHITE_PAWN;
								break;

							case BLACK_PAWN :
							case BLACK_KNIGHT :
							case BLACK_BISHOP :
							case BLACK_ROOK :
							case BLACK_QUEEN :
								nCP = BLACK_PAWN;
								break;

							default :
								nCP = nCapture;
								break;
						}
					}
					else
					{
						nCP = nCapture;
					}

					switch(nCP)
					{
						case WHITE_PAWN :
							Game [nG].nBuffer [WHITE_PAWN  ]++;
							break;
						case WHITE_KNIGHT :
							Game [nG].nBuffer [WHITE_KNIGHT]++;
							break;
						case WHITE_BISHOP :
							Game [nG].nBuffer [WHITE_BISHOP]++;
							break;
						case WHITE_ROOK :
							Game [nG].nBuffer [WHITE_ROOK  ]++;
							break;
						case WHITE_QUEEN :
							Game [nG].nBuffer [WHITE_QUEEN ]++;
							break;

						case BLACK_PAWN :
							Game [nG].nBuffer [BLACK_PAWN  ]++;
							break;
						case BLACK_KNIGHT :
							Game [nG].nBuffer [BLACK_KNIGHT]++;
							break;
						case BLACK_BISHOP :
							Game [nG].nBuffer [BLACK_BISHOP]++;
							break;
						case BLACK_ROOK :
							Game [nG].nBuffer [BLACK_ROOK  ]++;
							break;
						case BLACK_QUEEN :
							Game [nG].nBuffer [BLACK_QUEEN ]++;
							break;
					}
				}
			}

			if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)  // atomic capture update
			{

				// adjust promote board
				Game [nG].nPromoteBoard [nFX] [nFY] = 0;
				Game [nG].nPromoteBoard [nTX] [nTY] = 0;
			}
			else
			{

				// adjust promote board
				Game [nG].nPromoteBoard [nFX] [nFY] = 0;
				Game [nG].nPromoteBoard [nTX] [nTY] = 1;
			}

			// adjust last double push file
			Game [nG].nLastDoublePushFile = -1;
			break;
	}

	// adjust initial move flag
	Game [nG].bInitialMove = 0;

	// adjust whites move flag
	Game [nG].bWhitesMove = (! Game [nG].bWhitesMove);
}

void CHESS_GenMove(int nG, int bIncludeDropMove, int bLegalPieceDrop)
{
	int nC, nO, nX, nY, nI, nCount, bDirect, bHasCaptureMove;

	// set temporary move to zero
	TempMoves.nMove = 0;

	// white's move
	if(Game [nG].bWhitesMove)
	{
		// set white to move
		nC = INDEX_WHITE;

		// scan through entire board for white pieces
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				switch(Game [nG].nBoard [nX] [nY])
				{
					case WHITE_PAWN:
						CHESS_GenWhitePawn(nG, nX, nY);
						break;

					case WHITE_ROOK:
						CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_KNIGHT :
						CHESS_GenKnight(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_BISHOP :
						CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_QUEEN :
						CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
						CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_KING :
						CHESS_GenWhiteKing(nG, nX, nY);
						break;
				}
			}
		}

		if(bIncludeDropMove)
		{
			// generate drop move
			for(nY = 0 ; nY < 8 ; nY++)
			{
				for(nX = 0 ; nX < 8 ; nX++)
				{
					if(CHESS_IsEmptySquare(nG, nX, nY))
					{
						if((nY > 0) && (nY < 7))
						{
							if(bLegalPieceDrop)
							{
								if(Game [nG].nBuffer [WHITE_PAWN] > 0)
								{
									CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_PAWN, EMPTY_SQUARE);
								}
							}
							else
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_PAWN, EMPTY_SQUARE);
							}
						}

						if(bLegalPieceDrop)
						{
							if(Game [nG].nBuffer [WHITE_ROOK] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_ROOK, EMPTY_SQUARE);
							}

							if(Game [nG].nBuffer [WHITE_KNIGHT] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_KNIGHT, EMPTY_SQUARE);
							}

							if(Game [nG].nBuffer [WHITE_BISHOP] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_BISHOP, EMPTY_SQUARE);
							}

							if(Game [nG].nBuffer [WHITE_QUEEN] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_QUEEN, EMPTY_SQUARE);
							}
						}
						else
						{
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_ROOK,   EMPTY_SQUARE);
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_KNIGHT, EMPTY_SQUARE);
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_BISHOP, EMPTY_SQUARE);
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, WHITE_QUEEN,  EMPTY_SQUARE);
						}
					}
				}
			}
		}
	}
	else
	{
		// set black to move
		nC = INDEX_BLACK;

		// scan through entire board for black pieces
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				switch(Game [nG].nBoard [nX] [nY])
				{
					case BLACK_PAWN:
						CHESS_GenBlackPawn(nG, nX, nY);
						break;

					case BLACK_ROOK:
						CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_KNIGHT :
						CHESS_GenKnight(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_BISHOP :
						CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_QUEEN :
						CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
						CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_KING :
						CHESS_GenBlackKing(nG, nX, nY);
						break;
				}
			}
		}

		if(bIncludeDropMove)
		{
			// generate drop move
			for(nY = 0 ; nY < 8 ; nY++)
			{
				for(nX = 0 ; nX < 8 ; nX++)
				{
					if(CHESS_IsEmptySquare(nG, nX, nY))
					{
						if((nY > 0) && (nY < 7))
						{
							if(bLegalPieceDrop)
							{
								if(Game [nG].nBuffer [BLACK_PAWN] > 0)
								{
									CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_PAWN, EMPTY_SQUARE);
								}
							}
							else
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_PAWN, EMPTY_SQUARE);
							}
						}

						if(bLegalPieceDrop)
						{
							if(Game [nG].nBuffer [BLACK_ROOK] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_ROOK, EMPTY_SQUARE);
							}

							if(Game [nG].nBuffer [BLACK_KNIGHT] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_KNIGHT, EMPTY_SQUARE);
							}

							if(Game [nG].nBuffer [BLACK_BISHOP] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_BISHOP, EMPTY_SQUARE);
							}

							if(Game [nG].nBuffer [BLACK_QUEEN] > 0)
							{
								CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_QUEEN, EMPTY_SQUARE);
							}
						}
						else
						{
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_ROOK,   EMPTY_SQUARE);
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_KNIGHT, EMPTY_SQUARE);
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_BISHOP, EMPTY_SQUARE);
							CHESS_AddTempMove(-1, -1, nX, nY, MOVE_DROP, EMPTY_SQUARE, BLACK_QUEEN,  EMPTY_SQUARE);
						}
					}
				}
			}
		}
	}

	// reset legal move count
	Moves.nMove = 0;

	// reset last match index
	Moves.nLastMatch = -1;

	if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
			(Game [nG].nGameType == GAMETYPE_GIVEAWAY))
	{
		// check to see if there are capture moves
		bHasCaptureMove = 0;
		for(nI = 0 ; nI < TempMoves.nMove ; nI++)
		{
			if((TempMoves.nCapture [nI] >= WHITE_PAWN) && (TempMoves.nCapture [nI] <= BLACK_KING))
			{
				bHasCaptureMove = 1;
				break;
			}
		}

		// if there are capture moves then only copy capturing moves else copy all moves
		if(bHasCaptureMove)
		{
			for(nI = 0 ; nI < TempMoves.nMove ; nI++)
			{
				if((TempMoves.nCapture [nI] >= WHITE_PAWN) && (TempMoves.nCapture [nI] <= BLACK_KING))
				{
					// save only capture move as legal moves
					CHESS_CopyTempMove(nI);
				}
			}
		}
		else
		{
			for(nI = 0 ; nI < TempMoves.nMove ; nI++)
			{
				// save all moves as legal moves
				CHESS_CopyTempMove(nI);
			}
		}
	}
	else if(Game [nG].nGameType == GAMETYPE_LOSER)
	{
		// filter out illegal moves
		bHasCaptureMove = 0;
		nCount          = 0;
		bDirect         = 0;
		for(nI = 0 ; nI < TempMoves.nMove ; nI++)
		{
			// backup position
			CHESS_BackupPosition(nG);

			// make move
			CHESS_MakeMove(nG,
						   TempMoves.nFromX    [nI],
						   TempMoves.nFromY    [nI],
						   TempMoves.nToX      [nI],
						   TempMoves.nToY      [nI],
						   TempMoves.nMoveType [nI],
						   TempMoves.nCapture  [nI],
						   TempMoves.nDrop     [nI],
						   TempMoves.nPromote  [nI]);

			// check to see if it's a legal move
			if(! CHESS_SquareInCheck(nG,
									 nC,
									 Game [nG].nKingX [nC],
									 Game [nG].nKingY [nC],
									 &nCount,
									 &bDirect))
			{
				if((TempMoves.nCapture [nI] >= WHITE_PAWN) && (TempMoves.nCapture [nI] <= BLACK_QUEEN))
				{
					if(! bHasCaptureMove)
					{
						bHasCaptureMove = 1;

						// reset legal move count
						Moves.nMove = 0;

						// reset last match index
						Moves.nLastMatch = -1;
					}

					// save as a legal move
					CHESS_CopyTempMove(nI);
				}
				else if(! bHasCaptureMove)
				{
					// save as a legal move
					CHESS_CopyTempMove(nI);
				}
			}

			// restore position
			CHESS_RestorePosition(nG);
		}
	}
	else if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
	{
		// get opponent color index
		nO = (nC == INDEX_WHITE ? INDEX_BLACK : INDEX_WHITE);

		// filter out illegal moves
		nCount  = 0;
		bDirect = 0;
		for(nI = 0 ; nI < TempMoves.nMove ; nI++)
		{
			// backup position
			CHESS_BackupPosition(nG);

			// make move
			CHESS_MakeMove(nG,
						   TempMoves.nFromX    [nI],
						   TempMoves.nFromY    [nI],
						   TempMoves.nToX      [nI],
						   TempMoves.nToY      [nI],
						   TempMoves.nMoveType [nI],
						   TempMoves.nCapture  [nI],
						   TempMoves.nDrop     [nI],
						   TempMoves.nPromote  [nI]);

			// if our king is gone then it's an illegal move
			if(Game [nG].nKingX [nC] == -1 || Game [nG].nKingY [nC] == -1)
			{
			}
			else
			{
				//
				// if the opponent king is gone then there is no need to check
				// if our king is in check
				//
				if(Game [nG].nKingX [nO] == -1 || Game [nG].nKingY [nO] == -1)
				{
					// save as a legal move
					CHESS_CopyTempMove(nI);
				}
				else
				{
					// our king is in check
					if(CHESS_SquareInCheck(nG,
										   nC,
										   Game [nG].nKingX [nC],
										   Game [nG].nKingY [nC],
										   &nCount,
										   &bDirect))
					{
						// if opponent's king is around our king then it's a legal move
						if(CHESS_OpponentKingIsNear(nG,
													nC,
													Game [nG].nKingX [nC],
													Game [nG].nKingY [nC]))
						{
							// save as a legal move
							CHESS_CopyTempMove(nI);
						}
					}
					else
					{
						// save as a legal move
						CHESS_CopyTempMove(nI);
					}
				}
			}

			// restore position
			CHESS_RestorePosition(nG);
		}
	}
	else
	{
		// filter out illegal moves
		nCount  = 0;
		bDirect = 0;
		for(nI = 0 ; nI < TempMoves.nMove ; nI++)
		{
			// backup position
			CHESS_BackupPosition(nG);

			// make move
			CHESS_MakeMove(nG,
						   TempMoves.nFromX    [nI],
						   TempMoves.nFromY    [nI],
						   TempMoves.nToX      [nI],
						   TempMoves.nToY      [nI],
						   TempMoves.nMoveType [nI],
						   TempMoves.nCapture  [nI],
						   TempMoves.nDrop     [nI],
						   TempMoves.nPromote  [nI]);

			// check to see if it's a legal move
			if(! CHESS_SquareInCheck(nG,
									 nC,
									 Game [nG].nKingX [nC],
									 Game [nG].nKingY [nC],
									 &nCount,
									 &bDirect))
			{
				// save as a legal move
				CHESS_CopyTempMove(nI);
			}

			// restore position
			CHESS_RestorePosition(nG);
		}
	}
}

int CHESS_SmartMoveSource(int nG, int nX, int nY)
{
	int nOA, bPC, nPW, nPB;
	int nC, nO, nI, nCount, bDirect, nCC, nJ;

	// not in true premove
	bInTruePremove = 0;

	// not in CHESS_IsLegalMoveString ()
	bInIsLegalMoveString = 0;

	// if the user does not have smart move turned on then return as no smart move found
	if((! User.bSmartMove) || (! User.bSmartMove1))
	{
		return 0;
	}

	// if it's ICC's wild 16 then no smart move
	if(Game [nG].nGameType == GAMETYPE_ICC_WILD16)
	{
		return 0;
	}

	if(System.bPromoteKnight)
	{
		nOA = User.bAutoQueen;
		bPC = System.bPromoteCommand;
		nPW = System.nPromoteWPiece;
		nPB = System.nPromoteBPiece;

		User.bAutoQueen        = 1;
		System.bPromoteCommand = 1;
		System.nPromoteWPiece  = WHITE_KNIGHT;
		System.nPromoteBPiece  = BLACK_KNIGHT;
	}

	// set temporary move to zero
	TempMoves.nMove = 0;

	// reset legal move count
	Moves.nMove = 0;

	// reset last match index
	Moves.nLastMatch = -1;

	// generate move
	if(Game [nG].nGameType == GAMETYPE_LOSER)
	{
		CHESS_GenMove(nG, 0, 1);

		// no move generated
		if(Moves.nMove == 0)
		{
			if(System.bPromoteKnight)
			{
				User.bAutoQueen        = nOA;
				System.bPromoteCommand = bPC;
				System.nPromoteWPiece  = nPW;
				System.nPromoteBPiece  = nPB;
			}
			return 0;
		}

		// see if we find only one move for the piece
		nJ  = -1;
		nCC = 0;
		for(nI = 0 ; nI < Moves.nMove ; nI++)
		{
			if((Moves.nFromX [nI] == nX) && (Moves.nFromY [nI] == nY))
			{
				nJ  = nI;
				nCC = nCC + 1;
				if(nCC > 1)
				{
					break;
				}
			}
		}

		if(nCC == 1)    // only one legal loser move
		{
			// reset legal move count
			Moves.nMove = 0;

			// reset last match index
			Moves.nLastMatch = -1;

			// copy move
			CHESS_CopyMoveTemp(0, nJ);
			CHESS_CopyTempMove(0);
		}
		else            // there is at least one other capture move --> no smart move
		{
			if(System.bPromoteKnight)
			{
				User.bAutoQueen        = nOA;
				System.bPromoteCommand = bPC;
				System.nPromoteWPiece  = nPW;
				System.nPromoteBPiece  = nPB;
			}
			return 0;
		}
	}
	else
	{
		switch(Game [nG].nBoard [nX] [nY])
		{
			case WHITE_PAWN:
				nC = INDEX_WHITE;
				CHESS_GenWhitePawn(nG, nX, nY);
				break;

			case WHITE_ROOK:
				nC = INDEX_WHITE;
				CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_KNIGHT :
				nC = INDEX_WHITE;
				CHESS_GenKnight(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_BISHOP :
				nC = INDEX_WHITE;
				CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_QUEEN :
				nC = INDEX_WHITE;
				CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
				CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_KING :
				nC = INDEX_WHITE;
				CHESS_GenWhiteKing(nG, nX, nY);
				break;

			case BLACK_PAWN:
				nC = INDEX_BLACK;
				CHESS_GenBlackPawn(nG, nX, nY);
				break;

			case BLACK_ROOK:
				nC = INDEX_BLACK;
				CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_KNIGHT :
				nC = INDEX_BLACK;
				CHESS_GenKnight(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_BISHOP :
				nC = INDEX_BLACK;
				CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_QUEEN :
				nC = INDEX_BLACK;
				CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
				CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_KING :
				nC = INDEX_BLACK;
				CHESS_GenBlackKing(nG, nX, nY);
				break;

			default :
				if(System.bPromoteKnight)
				{
					User.bAutoQueen        = nOA;
					System.bPromoteCommand = bPC;
					System.nPromoteWPiece  = nPW;
					System.nPromoteBPiece  = nPB;
				}
				return 0;
		}

		// no move generated
		if(TempMoves.nMove == 0)
		{
			if(System.bPromoteKnight)
			{
				User.bAutoQueen        = nOA;
				System.bPromoteCommand = bPC;
				System.nPromoteWPiece  = nPW;
				System.nPromoteBPiece  = nPB;
			}
			return 0;
		}

		if((Game [nG].nGameType == GAMETYPE_SUICIDE) ||
				(Game [nG].nGameType == GAMETYPE_GIVEAWAY))
		{
			// count the # of capture moves
			nCC = 0;
			for(nI = 0 ; nI < TempMoves.nMove ; nI++)
			{
				if((TempMoves.nCapture [nI] >= WHITE_PAWN) && (TempMoves.nCapture [nI] <= BLACK_KING))
				{
					nCC = nCC + 1;
					if(nCC > 1)
					{
						break;
					}
				}
			}

			if(nCC == 0)    // no capture move
			{
				// if there are no moves or more than one move then there is no smart move
				if((TempMoves.nMove == 0) || (TempMoves.nMove > 1))
				{
					if(System.bPromoteKnight)
					{
						User.bAutoQueen        = nOA;
						System.bPromoteCommand = bPC;
						System.nPromoteWPiece  = nPW;
						System.nPromoteBPiece  = nPB;
					}
					return 0;
				}

				// generate all legal moves (except drop move)
				CHESS_GenMove(nG, 0, 1);

				// no moves
				if(Moves.nMove == 0)
				{
					if(System.bPromoteKnight)
					{
						User.bAutoQueen        = nOA;
						System.bPromoteCommand = bPC;
						System.nPromoteWPiece  = nPW;
						System.nPromoteBPiece  = nPB;
					}
					return 0;
				}

				// count the # of capture moves
				nCC = 0;
				for(nI = 0 ; nI < Moves.nMove ; nI++)
				{
					if((Moves.nCapture [nI] >= WHITE_PAWN) && (Moves.nCapture [nI] <= BLACK_KING))
					{
						nCC = 1;
						break;
					}
				}

				// there is at least one other capture move --> no smart move
				if(nCC == 1)
				{
					if(System.bPromoteKnight)
					{
						User.bAutoQueen        = nOA;
						System.bPromoteCommand = bPC;
						System.nPromoteWPiece  = nPW;
						System.nPromoteBPiece  = nPB;
					}
					return 0;
				}

				// set temporary move to zero
				TempMoves.nMove = 0;

				// reset legal move count
				Moves.nMove = 0;

				// reset last match index
				Moves.nLastMatch = -1;

				// generate move for the piece
				switch(Game [nG].nBoard [nX] [nY])
				{
					case WHITE_PAWN:
						nC = INDEX_WHITE;
						CHESS_GenWhitePawn(nG, nX, nY);
						break;

					case WHITE_ROOK:
						nC = INDEX_WHITE;
						CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_KNIGHT :
						nC = INDEX_WHITE;
						CHESS_GenKnight(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_BISHOP :
						nC = INDEX_WHITE;
						CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_QUEEN :
						nC = INDEX_WHITE;
						CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
						CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
						break;

					case WHITE_KING :
						nC = INDEX_WHITE;
						CHESS_GenWhiteKing(nG, nX, nY);
						break;

					case BLACK_PAWN:
						nC = INDEX_BLACK;
						CHESS_GenBlackPawn(nG, nX, nY);
						break;

					case BLACK_ROOK:
						nC = INDEX_BLACK;
						CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_KNIGHT :
						nC = INDEX_BLACK;
						CHESS_GenKnight(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_BISHOP :
						nC = INDEX_BLACK;
						CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_QUEEN :
						nC = INDEX_BLACK;
						CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
						CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
						break;

					case BLACK_KING :
						nC = INDEX_BLACK;
						CHESS_GenBlackKing(nG, nX, nY);
						break;

					default :
						if(System.bPromoteKnight)
						{
							User.bAutoQueen        = nOA;
							System.bPromoteCommand = bPC;
							System.nPromoteWPiece  = nPW;
							System.nPromoteBPiece  = nPB;
						}
						return 0;
				}

				// make sure there is only one move for this piece generated
				if(TempMoves.nMove != 1)
				{
					if(System.bPromoteKnight)
					{
						User.bAutoQueen        = nOA;
						System.bPromoteCommand = bPC;
						System.nPromoteWPiece  = nPW;
						System.nPromoteBPiece  = nPB;
					}
					return 0;
				}

				CHESS_CopyTempMove(0);
			}
			else if(nCC == 1)   // only one capture move
			{
				for(nI = 0 ; nI < TempMoves.nMove ; nI++)
				{
					if((TempMoves.nCapture [nI] >= WHITE_PAWN) && (TempMoves.nCapture [nI] <= BLACK_KING))
					{
						// save the only move as legal move
						CHESS_CopyTempMove(nI);
						break;
					}
				}
			}
			else            // more than one capture move (no smart move)
			{
				if(System.bPromoteKnight)
				{
					User.bAutoQueen        = nOA;
					System.bPromoteCommand = bPC;
					System.nPromoteWPiece  = nPW;
					System.nPromoteBPiece  = nPB;
				}
				return 0;
			}
		}
		else if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
		{
			// get opponent color index
			nO = (nC == INDEX_WHITE ? INDEX_BLACK : INDEX_WHITE);

			// filter out illegal moves
			nCount  = 0;
			bDirect = 0;
			for(nI = 0 ; nI < TempMoves.nMove ; nI++)
			{
				// backup position
				CHESS_BackupPosition(nG);

				// make move
				CHESS_MakeMove(nG,
							   TempMoves.nFromX    [nI],
							   TempMoves.nFromY    [nI],
							   TempMoves.nToX      [nI],
							   TempMoves.nToY      [nI],
							   TempMoves.nMoveType [nI],
							   TempMoves.nCapture  [nI],
							   TempMoves.nDrop     [nI],
							   TempMoves.nPromote  [nI]);

				// if our king is gone then it's an illegal move
				if(Game [nG].nKingX [nC] == -1 || Game [nG].nKingY [nC] == -1)
				{
				}
				else
				{
					//
					// if the opponent king is gone then there is no need to check
					// if our king is in check
					//
					if(Game [nG].nKingX [nO] == -1 || Game [nG].nKingY [nO] == -1)
					{
						// save as legal move
						CHESS_CopyTempMove(nI);

						// if there is already one move saved then it's not smart move
						if(Moves.nMove >= 2)
						{
							// restore position
							CHESS_RestorePosition(nG);
							break;
						}
					}
					else
					{
						// our king is in check
						if(CHESS_SquareInCheck(nG,
											   nC,
											   Game [nG].nKingX [nC],
											   Game [nG].nKingY [nC],
											   &nCount,
											   &bDirect))
						{
							// if opponent's king is around our king then it's a legal move
							if(CHESS_OpponentKingIsNear(nG,
														nC,
														Game [nG].nKingX [nC],
														Game [nG].nKingY [nC]))
							{
								// save as legal move
								CHESS_CopyTempMove(nI);

								// if there is already one move saved then it's not smart move
								if(Moves.nMove >= 2)
								{
									// restore position
									CHESS_RestorePosition(nG);
									break;
								}
							}
						}
						else
						{
							// save as legal move
							CHESS_CopyTempMove(nI);

							// if there is already one move saved then it's not smart move
							if(Moves.nMove >= 2)
							{
								// restore position
								CHESS_RestorePosition(nG);
								break;
							}
						}
					}
				}

				// restore position
				CHESS_RestorePosition(nG);
			}
		}
		else
		{
			// filter out illegal moves
			nCount  = 0;
			bDirect = 0;
			for(nI = 0 ; nI < TempMoves.nMove ; nI++)
			{
				// backup position
				CHESS_BackupPosition(nG);

				// make move
				CHESS_MakeMove(nG,
							   TempMoves.nFromX    [nI],
							   TempMoves.nFromY    [nI],
							   TempMoves.nToX      [nI],
							   TempMoves.nToY      [nI],
							   TempMoves.nMoveType [nI],
							   TempMoves.nCapture  [nI],
							   TempMoves.nDrop     [nI],
							   TempMoves.nPromote  [nI]);

				// check to see if it's a legal move
				if(! CHESS_SquareInCheck(nG,
										 nC,
										 Game [nG].nKingX [nC],
										 Game [nG].nKingY [nC],
										 &nCount,
										 &bDirect))
				{
					// save as legal move
					CHESS_CopyTempMove(nI);

					// if there is already one move saved then it's not smart move
					if(Moves.nMove >= 2)
					{
						// restore position
						CHESS_RestorePosition(nG);
						break;
					}
				}

				// restore position
				CHESS_RestorePosition(nG);
			}
		}
	}

	if(System.bPromoteKnight)
	{
		User.bAutoQueen        = nOA;
		System.bPromoteCommand = bPC;
		System.nPromoteWPiece  = nPW;
		System.nPromoteBPiece  = nPB;
	}
	return (Moves.nMove == 1);
}

int CHESS_SmartMoveDestination(int nG, int nX, int nY)
{
	int nOA, bPC, nPW, nPB;
	int nC, nO, nI;

	// not in true premove
	bInTruePremove = 0;

	// not in CHESS_IsLegalMoveString ()
	bInIsLegalMoveString = 0;

	// if the user does not have smart move turned on then return as no smart move found
	if((! User.bSmartMove) || (! User.bSmartMove2))
	{
		return 0;
	}

	// if it's ICC's wild 16 then no smart move
	if(Game [nG].nGameType == GAMETYPE_ICC_WILD16)
	{
		return 0;
	}

	if(System.bPromoteKnight)
	{
		nOA = User.bAutoQueen;
		bPC = System.bPromoteCommand;
		nPW = System.nPromoteWPiece;
		nPB = System.nPromoteBPiece;

		User.bAutoQueen        = 1;
		System.bPromoteCommand = 1;
		System.nPromoteWPiece  = WHITE_KNIGHT;
		System.nPromoteBPiece  = BLACK_KNIGHT;
	}

	// generate all legal moves (except drop move)
	CHESS_GenMove(nG, 0, 1);

	// reset last match index
	Moves.nLastMatch = -1;

	// no move generated
	if(Moves.nMove == 0)
	{
		if(System.bPromoteKnight)
		{
			User.bAutoQueen        = nOA;
			System.bPromoteCommand = bPC;
			System.nPromoteWPiece  = nPW;
			System.nPromoteBPiece  = nPB;
		}
		return 0;
	}

	// go through all the legal moves and match for destination square x,y
	nC = 0;
	for(nI = 0 ; nI < Moves.nMove ; nI++)
	{
		if((Moves.nToX [nI] == nX) && (Moves.nToY [nI] == nY))
		{
			if(nC == 0)
			{
				nO = nI;
			}
			else
			{
				if(System.bPromoteKnight)
				{
					User.bAutoQueen        = nOA;
					System.bPromoteCommand = bPC;
					System.nPromoteWPiece  = nPW;
					System.nPromoteBPiece  = nPB;
				}
				return 0;
			}

			nC = nC + 1;
		}
	}

	if(System.bPromoteKnight)
	{
		User.bAutoQueen        = nOA;
		System.bPromoteCommand = bPC;
		System.nPromoteWPiece  = nPW;
		System.nPromoteBPiece  = nPB;
	}

	if(nC == 1)
	{
		Moves.nLastMatch = nO;
		return 1;
	}
	return 0;
}

char *CHESS_GetNotation(int nG, int nIndex)
{
	static char cBuffer [32];
	int  nP, bA, bB, bC, nI;
	char cTmp [32];

	// must be legal move index
	if((nIndex < 0) || (nIndex >= Moves.nMove))
	{
		strcpy(cBuffer, "");
		return cBuffer;
	}

	// drop move
	if(Moves.nMoveType [nIndex] == MOVE_DROP)
	{
		sprintf(cBuffer,
				"%c@%c%d",
				cAlgePiece [Moves.nDrop [nIndex]],
				Moves.nToX [nIndex] + 'a',
				Moves.nToY [nIndex] + 1);

		return cBuffer;
	}

	// o-o move
	if(Moves.nMoveType [nIndex] == MOVE_OO)
	{
		strcpy(cBuffer, ALGE_CASTLE_KING_SIDE_MOVE);
		return cBuffer;
	}

	// o-o-o move
	if(Moves.nMoveType [nIndex] == MOVE_OOO)
	{
		strcpy(cBuffer, ALGE_CASTLE_QUEEN_SIDE_MOVE);
		return cBuffer;
	}

	// pawn double push
	if(Moves.nMoveType [nIndex] == MOVE_DBLPUSH)
	{
		sprintf(cBuffer,
				"%c%d",
				Moves.nToX [nIndex] + 'a',
				Moves.nToY [nIndex] + 1);

		return cBuffer;
	}

	// en pasant
	if(Moves.nMoveType [nIndex] == MOVE_ENPASANT)
	{
		sprintf(cBuffer,
				"%cx%c%d",
				Moves.nFromX [nIndex] + 'a',
				Moves.nToX   [nIndex] + 'a',
				Moves.nToY   [nIndex] + 1);

		return cBuffer;
	}

	// promotion
	if(Moves.nMoveType [nIndex] == MOVE_PROMOTE)
	{
		if(Moves.nCapture [nIndex] == EMPTY_SQUARE)
		{
			sprintf(cBuffer,
					"%c%d=%c",
					Moves.nToX [nIndex] + 'a',
					Moves.nToY [nIndex] + 1,
					cAlgePiece [Moves.nPromote [nIndex]]);
		}
		else
		{
			sprintf(cBuffer,
					"%cx%c%d=%c",
					Moves.nFromX [nIndex] + 'a',
					Moves.nToX   [nIndex] + 'a',
					Moves.nToY   [nIndex] + 1,
					cAlgePiece [Moves.nPromote [nIndex]]);
		}
		return cBuffer;
	}

	// normal moves
	if(Game [nG].nBoard [Moves.nFromX [nIndex]] [Moves.nFromY [nIndex]] == EMPTY_SQUARE)
	{
		nP = Game [nG].nBoard [Moves.nToX [nIndex]] [Moves.nToY [nIndex]];
	}
	else
	{
		nP = Game [nG].nBoard [Moves.nFromX [nIndex]] [Moves.nFromY [nIndex]];
	}

	if((nP == WHITE_PAWN) || (nP == BLACK_PAWN))
	{
		if(Moves.nCapture [nIndex] == EMPTY_SQUARE)
		{
			sprintf(cBuffer,
					"%c%d",
					Moves.nToX [nIndex] + 'a',
					Moves.nToY [nIndex] + 1);
		}
		else
		{
			sprintf(cBuffer,
					"%cx%c%d",
					Moves.nFromX [nIndex] + 'a',
					Moves.nToX   [nIndex] + 'a',
					Moves.nToY   [nIndex] + 1);
		}
	}
	else
	{
		bA = 0;
		bB = 0;
		bC = 0;

		for(nI = 0 ; nI < Moves.nMove ; nI++)
		{
			if((nI != nIndex) && (Moves.nMoveType [nI] == MOVE_NORMAL))
			{
				if(Game [nG].nBoard [Moves.nFromX [nI]] [Moves.nFromY [nI]] == nP)
				{
					if((Moves.nToX [nI] == Moves.nToX [nIndex]) &&
							(Moves.nToY [nI] == Moves.nToY [nIndex]))
					{
						bC = 1;

						if(Moves.nFromX [nI] == Moves.nFromX [nIndex])
						{
							bA = 1;

							if(bA && bB)
							{
								break;
							}
						}

						if(Moves.nFromY [nI] == Moves.nFromY [nIndex])
						{
							bB = 1;

							if(bA && bB)
							{
								break;
							}
						}
					}
				}
			}
		}

		if(bA && bB)
		{
			sprintf(cTmp,
					"%c%c%d",
					cAlgePiece [nP],
					Moves.nFromX [nIndex] + 'a',
					Moves.nFromY [nIndex] + 1);
		}
		else if(bA)
		{
			sprintf(cTmp,
					"%c%d",
					cAlgePiece [nP],
					Moves.nFromY [nIndex] + 1);
		}
		else if(bB || bC)
		{
			sprintf(cTmp,
					"%c%c",
					cAlgePiece [nP],
					Moves.nFromX [nIndex] + 'a');
		}
		else
		{
			sprintf(cTmp, "%c", cAlgePiece [nP]);
		}

		if(Moves.nCapture [nIndex] == EMPTY_SQUARE)
		{
			sprintf(cBuffer,
					"%s%c%d",
					cTmp,
					Moves.nToX [nIndex] + 'a',
					Moves.nToY [nIndex] + 1);
		}
		else
		{
			sprintf(cBuffer,
					"%sx%c%d",
					cTmp,
					Moves.nToX [nIndex] + 'a',
					Moves.nToY [nIndex] + 1);
		}
	}

	return cBuffer;
}

char *CHESS_GetLongNotation(int nG, int nIndex)
{
	static char cBuffer [32];

	// must be legal move index
	if((nIndex < 0) || (nIndex >= Moves.nMove))
	{
		strcpy(cBuffer, "");
		return cBuffer;
	}

	// drop move
	if(Moves.nMoveType [nIndex] == MOVE_DROP)
	{
		sprintf(cBuffer,
				"%c@%c%d",
				cAlgePiece [Moves.nDrop [nIndex]],
				Moves.nToX [nIndex] + 'a',
				Moves.nToY [nIndex] + 1);

		return cBuffer;
	}

	// o-o move
	if(Moves.nMoveType [nIndex] == MOVE_OO)
	{
		strcpy(cBuffer, ALGE_CASTLE_KING_SIDE_MOVE);
		return cBuffer;
	}

	// o-o-o move
	if(Moves.nMoveType [nIndex] == MOVE_OOO)
	{
		strcpy(cBuffer, ALGE_CASTLE_QUEEN_SIDE_MOVE);
		return cBuffer;
	}

	// pawn double push
	if(Moves.nMoveType [nIndex] == MOVE_DBLPUSH)
	{
		sprintf(cBuffer,
				"%c%d%c%d",
				Moves.nFromX [nIndex] + 'a',
				Moves.nFromY [nIndex] + 1,
				Moves.nToX   [nIndex] + 'a',
				Moves.nToY   [nIndex] + 1);

		return cBuffer;
	}

	// en pasant
	if(Moves.nMoveType [nIndex] == MOVE_ENPASANT)
	{
		sprintf(cBuffer,
				"%c%d%c%d",
				Moves.nFromX [nIndex] + 'a',
				Moves.nFromY [nIndex] + 1,
				Moves.nToX   [nIndex] + 'a',
				Moves.nToY   [nIndex] + 1);

		return cBuffer;
	}

	// promotion
	if(Moves.nMoveType [nIndex] == MOVE_PROMOTE)
	{
		sprintf(cBuffer,
				"%c%d%c%d=%c",
				Moves.nFromX [nIndex] + 'a',
				Moves.nFromY [nIndex] + 1,
				Moves.nToX   [nIndex] + 'a',
				Moves.nToY   [nIndex] + 1,
				cAlgePiece [Moves.nPromote [nIndex]]);

		return cBuffer;
	}

	// normal moves
	sprintf(cBuffer,
			"%c%d%c%d",
			Moves.nFromX [nIndex] + 'a',
			Moves.nFromY [nIndex] + 1,
			Moves.nToX   [nIndex] + 'a',
			Moves.nToY   [nIndex] + 1);

	return cBuffer;
}

int CHESS_IsLegalMoveString(int nG, char *cMove)
{
	int  bL, nI, nP;
	char cTmp [30];

	// not in true premove
	bInTruePremove = 0;

	// in CHESS_IsLegalMoveString ()
	bInIsLegalMoveString = 1;

	// generate all legal moves
	if(Game [nG].nGameType == GAMETYPE_CRAZYHOUSE)
	{
		CHESS_GenMove(nG, 1, 1);
	}
	else if(Game [nG].nGameType == GAMETYPE_BUGHOUSE)
	{
		CHESS_GenMove(nG, 1, 0);
	}
	else
	{
		CHESS_GenMove(nG, 0, 1);
	}

	// no move generated
	if(Moves.nMove == 0)
	{
		bInIsLegalMoveString = 0;
		return 0;
	}

	// scan to see if move string match any moves
	bL = 0;
	for(nI = 0 ; nI < Moves.nMove ; nI++)
	{

		// drop move
		if(Moves.nMoveType [nI] == MOVE_DROP)
		{
			// check moves like Q@f7
			sprintf(cTmp,
					"%c@%c%d",
					cAlgePiece [Moves.nDrop [nI]],
					Moves.nToX [nI] + 'a',
					Moves.nToY [nI] + 1);

			if(stricmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}
		}

		// o-o move
		if(Moves.nMoveType [nI] == MOVE_OO)
		{
			// check moves like o-o
			if(stricmp(ALGE_CASTLE_KING_SIDE_MOVE, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			if(Game [nG].bWhitesMove)
			{
				if((stricmp("e1g1",  cMove) == 0) ||
						(stricmp("ke1g1", cMove) == 0))
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}
			}
			else
			{
				if((stricmp("e8g8",  cMove) == 0) ||
						(stricmp("ke8g8", cMove) == 0))
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}
			}
		}

		// o-o-o move
		if(Moves.nMoveType [nI] == MOVE_OOO)
		{
			// check moves like o-o-o
			if(stricmp(ALGE_CASTLE_QUEEN_SIDE_MOVE, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			if(Game [nG].bWhitesMove)
			{
				if((stricmp("e1c1",  cMove) == 0) ||
						(stricmp("ke1c1", cMove) == 0))
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}
			}
			else
			{
				if((stricmp("e8c8",  cMove) == 0) ||
						(stricmp("ke8c8", cMove) == 0))
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}
			}
		}

		// pawn double push
		if(Moves.nMoveType [nI] == MOVE_DBLPUSH)
		{
			// check moves like e4
			sprintf(cTmp,
					"%c%d",
					Moves.nToX [nI] + 'a',
					Moves.nToY [nI] + 1);

			if(strcmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			// check moves like Pe4
			sprintf(cTmp,
					"P%c%d",
					Moves.nToX [nI] + 'a',
					Moves.nToY [nI] + 1);

			if(stricmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			// check moves like e2e4
			sprintf(cTmp,
					"%c%d%c%d",
					Moves.nFromX [nI] + 'a',
					Moves.nFromY [nI] + 1,
					Moves.nToX   [nI] + 'a',
					Moves.nToY   [nI] + 1);

			if(strcmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			// check moves like Pe2e4
			sprintf(cTmp,
					"P%c%d%c%d",
					Moves.nFromX [nI] + 'a',
					Moves.nFromY [nI] + 1,
					Moves.nToX   [nI] + 'a',
					Moves.nToY   [nI] + 1);

			if(stricmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}
		}

		// pawn en pasant
		if(Moves.nMoveType [nI] == MOVE_ENPASANT)
		{
			// check moves like exd6
			sprintf(cTmp,
					"%cx%c%d",
					Moves.nFromX [nI] + 'a',
					Moves.nToX   [nI] + 'a',
					Moves.nToY   [nI] + 1);

			if(strcmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			// check moves like Pexd6
			sprintf(cTmp,
					"P%cx%c%d",
					Moves.nFromX [nI] + 'a',
					Moves.nToX   [nI] + 'a',
					Moves.nToY   [nI] + 1);

			if(stricmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			// check moves like e5d6
			sprintf(cTmp,
					"%c%d%c%d",
					Moves.nFromX [nI] + 'a',
					Moves.nFromY [nI] + 1,
					Moves.nToX   [nI] + 'a',
					Moves.nToY   [nI] + 1);

			if(strcmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			// check moves like e5xd6
			sprintf(cTmp,
					"%c%dx%c%d",
					Moves.nFromX [nI] + 'a',
					Moves.nFromY [nI] + 1,
					Moves.nToX   [nI] + 'a',
					Moves.nToY   [nI] + 1);

			if(strcmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}

			// check moves like Pe5xd6
			sprintf(cTmp,
					"P%c%dx%c%d",
					Moves.nFromX [nI] + 'a',
					Moves.nFromY [nI] + 1,
					Moves.nToX   [nI] + 'a',
					Moves.nToY   [nI] + 1);

			if(stricmp(cTmp, cMove) == 0)
			{
				bL = 1;
				Moves.nLastMatch = nI;
				break;
			}
		}

		// pawn promote
		if(Moves.nMoveType [nI] == MOVE_PROMOTE)
		{
			if(Moves.nCapture [nI] == EMPTY_SQUARE)
			{
				// check moves like e8=Q
				sprintf(cTmp,
						"%c%d=%c",
						Moves.nToX [nI] + 'a',
						Moves.nToY [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like e8Q
				sprintf(cTmp,
						"%c%d%c",
						Moves.nToX [nI] + 'a',
						Moves.nToY [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like e7e8=Q
				sprintf(cTmp,
						"%c%d%c%d=%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like e7e8Q
				sprintf(cTmp,
						"%c%d%c%d%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pe8=Q
				sprintf(cTmp,
						"P%c%d=%c",
						Moves.nToX [nI] + 'a',
						Moves.nToY [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pe8Q
				sprintf(cTmp,
						"P%c%d%c",
						Moves.nToX [nI] + 'a',
						Moves.nToY [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pe7e8=Q
				sprintf(cTmp,
						"P%c%d%c%d=%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pe7e8Q
				sprintf(cTmp,
						"P%c%d%c%d%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}
			}
			else
			{
				// check moves like exf8=Q
				sprintf(cTmp,
						"%cx%c%d=%c",
						Moves.nFromX [nI] + 'a',
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like exf8Q
				sprintf(cTmp,
						"%cx%c%d%c",
						Moves.nFromX [nI] + 'a',
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like e7xf8=Q
				sprintf(cTmp,
						"%c%dx%c%d=%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like e7xf8Q
				sprintf(cTmp,
						"%c%dx%c%d%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like e7f8=Q
				sprintf(cTmp,
						"%c%d%c%d=%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like e7f8Q
				sprintf(cTmp,
						"%c%d%c%d%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pexf8=Q
				sprintf(cTmp,
						"P%cx%c%d=%c",
						Moves.nFromX [nI] + 'a',
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pexf8Q
				sprintf(cTmp,
						"P%cx%c%d%c",
						Moves.nFromX [nI] + 'a',
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pe7xf8=Q
				sprintf(cTmp,
						"P%c%dx%c%d=%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}

				// check moves like Pe7xf8Q
				sprintf(cTmp,
						"P%c%dx%c%d%c",
						Moves.nFromX [nI] + 'a',
						Moves.nFromY [nI] + 1,
						Moves.nToX   [nI] + 'a',
						Moves.nToY   [nI] + 1,
						cAlgePiece [Moves.nPromote [nI]]);

				if(stricmp(cTmp, cMove) == 0)
				{
					bL = 1;
					Moves.nLastMatch = nI;
					break;
				}
			}
		}

		// normal moves
		if(Moves.nMoveType [nI] == MOVE_NORMAL)
		{
			nP = Game [nG].nBoard [Moves.nFromX [nI]] [Moves.nFromY [nI]];

			if((nP == WHITE_PAWN) || (nP == BLACK_PAWN))
			{
				if(Moves.nCapture [nI] == EMPTY_SQUARE)
				{
					// check moves like e6
					sprintf(cTmp,
							"%c%d",
							Moves.nToX [nI] + 'a',
							Moves.nToY [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Pe6
					sprintf(cTmp,
							"P%c%d",
							Moves.nToX [nI] + 'a',
							Moves.nToY [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like e5e6
					sprintf(cTmp,
							"%c%d%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Pe5e6
					sprintf(cTmp,
							"P%c%d%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}
				}
				else
				{
					// check moves like exf5
					sprintf(cTmp,
							"%cx%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Pexf5
					sprintf(cTmp,
							"P%cx%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like e4xf5
					sprintf(cTmp,
							"%c%dx%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like e4f5
					sprintf(cTmp,
							"%c%d%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Pe4xf5
					sprintf(cTmp,
							"P%c%dx%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}
				}
			}
			else
			{
				if(Moves.nCapture [nI] == EMPTY_SQUARE)
				{
					// check moves like Ng1f3
					sprintf(cTmp,
							"%c%c%d%c%d",
							cAlgePiece [nP],
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like g1f3
					sprintf(cTmp,
							"%c%d%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Ngf3
					sprintf(cTmp,
							"%c%c%c%d",
							cAlgePiece [nP],
							Moves.nFromX [nI] + 'a',
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like N1f3
					sprintf(cTmp,
							"%c%d%c%d",
							cAlgePiece [nP],
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Nf3
					sprintf(cTmp,
							"%c%c%d",
							cAlgePiece [nP],
							Moves.nToX [nI] + 'a',
							Moves.nToY [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}
				}
				else
				{
					// check moves like Ng1xf3
					sprintf(cTmp,
							"%c%c%dx%c%d",
							cAlgePiece [nP],
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like g1f3
					sprintf(cTmp,
							"%c%d%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like g1xf3
					sprintf(cTmp,
							"%c%dx%c%d",
							Moves.nFromX [nI] + 'a',
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Ngxf3
					sprintf(cTmp,
							"%c%cx%c%d",
							cAlgePiece [nP],
							Moves.nFromX [nI] + 'a',
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like N1xf3
					sprintf(cTmp,
							"%c%dx%c%d",
							cAlgePiece [nP],
							Moves.nFromY [nI] + 1,
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);

					if(stricmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}

					// check moves like Nxf3
					sprintf(cTmp,
							"%cx%c%d",
							cAlgePiece [nP],
							Moves.nToX   [nI] + 'a',
							Moves.nToY   [nI] + 1);
// 1.25 fix (MrBug): board buttons for observe board would not work for games like
// 1.d3 c5 2.Be3 e6 3.b4 b6 4.bxc5 Na6 5.c6
// case insensitive comparison does not handle pawn/bishop disambiguity
// correctly, eg matching Bxc5 instead of correct move bxc5	in the above example
// We don't need to care about cases like nxd4 because we only process ICS move lists
//
//                    if (stricmp (cTmp, cMove) == 0)
					if(strcmp(cTmp, cMove) == 0)
					{
						bL = 1;
						Moves.nLastMatch = nI;
						break;
					}
				}
			}
		}
	}

	bInIsLegalMoveString = 0;

	return bL;
}

int CHESS_IsLegalMove(int nG, int nPc, int nX, int nY, int nX1, int nY1, int nPo)
{
	int nC, nI, nCount, bDirect;

	// not in true premove
	bInTruePremove = 0;

	// not in CHESS_IsLegalMoveString ()
	bInIsLegalMoveString = 0;

	// make sure destination square index is valid
	if((nX1 < 0) || (nX1 > 7) || (nY1 < 0) || (nY1 > 7))
	{
		return 0;
	}

	// if it's ICC's wild 16 then use true premove to check for legal move
	if(Game [nG].nGameType == GAMETYPE_ICC_WILD16)
	{
		return CHESS_IsLegalTruePremove(nG, nPc, nX, nY, nX1, nY1, nPo);
	}

	if((Game [nG].nGameType == GAMETYPE_LOSER) ||
			(Game [nG].nGameType == GAMETYPE_SUICIDE) ||
			(Game [nG].nGameType == GAMETYPE_GIVEAWAY) ||
			(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC))
	{
		// generate all legal moves (except drop move)
		CHESS_GenMove(nG, 0, 1);
	}
	else
	{
		// set temporary move to zero
		TempMoves.nMove = 0;

		if((nX == -1) || (nY == -1))    // drop move
		{
			if((nPc == WHITE_PAWN) || (nPc == BLACK_PAWN))
			{
				if((nY1 == 0) || (nY1 == 7))
				{
					return 0;
				}
			}

			if(! CHESS_IsEmptySquare(nG, nX1, nY1))
			{
				return 0;
			}

			if(Game [nG].nBuffer [nPc] <= 0)
			{
				return 0;
			}

			if((nPc == WHITE_PAWN) || (nPc == WHITE_ROOK) || (nPc == WHITE_KNIGHT) ||
			   (nPc == WHITE_BISHOP) || (nPc == WHITE_QUEEN) || (nPc == WHITE_KING))
			{
				nC = INDEX_WHITE;
			}
			else
			{
				nC = INDEX_BLACK;
			}

			CHESS_AddTempMove(-1, -1, nX1, nY1, MOVE_DROP, EMPTY_SQUARE, nPc, EMPTY_SQUARE);
		}
		else                            // board move
		{
			// make sure source square index is valid
			if((nX < 0) || (nX > 7) || (nY < 0) || (nY > 7))
			{
				return 0;
			}

			switch(nPc)
			{
				case WHITE_PAWN:
					nC = INDEX_WHITE;
					CHESS_GenWhitePawn(nG, nX, nY);
					break;

				case WHITE_ROOK:
					nC = INDEX_WHITE;
					CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
					break;

				case WHITE_KNIGHT :
					nC = INDEX_WHITE;
					CHESS_GenKnight(nG, nX, nY, INDEX_WHITE);
					break;

				case WHITE_BISHOP :
					nC = INDEX_WHITE;
					CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
					break;

				case WHITE_QUEEN :
					nC = INDEX_WHITE;
					CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
					CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
					break;

				case WHITE_KING :
					nC = INDEX_WHITE;
					CHESS_GenWhiteKing(nG, nX, nY);
					break;

				case BLACK_PAWN:
					nC = INDEX_BLACK;
					CHESS_GenBlackPawn(nG, nX, nY);
					break;

				case BLACK_ROOK:
					nC = INDEX_BLACK;
					CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
					break;

				case BLACK_KNIGHT :
					nC = INDEX_BLACK;
					CHESS_GenKnight(nG, nX, nY, INDEX_BLACK);
					break;

				case BLACK_BISHOP :
					nC = INDEX_BLACK;
					CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
					break;

				case BLACK_QUEEN :
					nC = INDEX_BLACK;
					CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
					CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
					break;

				case BLACK_KING :
					nC = INDEX_BLACK;
					CHESS_GenBlackKing(nG, nX, nY);
					break;
			}
		}

		// no move generated
		if(TempMoves.nMove == 0)
		{
			return 0;
		}

		// reset legal move count
		Moves.nMove = 0;

		// reset last match index
		Moves.nLastMatch = -1;

		// filter out illegal moves
		nCount  = 0;
		bDirect = 0;
		for(nI = 0 ; nI < TempMoves.nMove ; nI++)
		{
			// backup position
			CHESS_BackupPosition(nG);

			// make move
			CHESS_MakeMove(nG,
						   TempMoves.nFromX    [nI],
						   TempMoves.nFromY    [nI],
						   TempMoves.nToX      [nI],
						   TempMoves.nToY      [nI],
						   TempMoves.nMoveType [nI],
						   TempMoves.nCapture  [nI],
						   TempMoves.nDrop     [nI],
						   TempMoves.nPromote  [nI]);

			// check to see if it's a legal move
			if(! CHESS_SquareInCheck(nG,
									 nC,
									 Game [nG].nKingX [nC],
									 Game [nG].nKingY [nC],
									 &nCount,
									 &bDirect))
			{
				// save as a legal move
				CHESS_CopyTempMove(nI);
			}

			// restore position
			CHESS_RestorePosition(nG);
		}
	}

	// no legal moves
	if(Moves.nMove == 0)
	{
		return 0;
	}

	// search for move
	if((nX == -1) || (nY == -1))    // drop move
	{
		for(nI = 0 ; nI < Moves.nMove ; nI++)
		{
			if(Moves.nMoveType [nI] == MOVE_DROP)
			{
				if((Moves.nDrop [nI] == nPc) &&
						(Moves.nToX  [nI] == nX1) &&
						(Moves.nToY  [nI] == nY1))
				{
					Moves.nLastMatch = nI;
					return 1;
				}
			}
		}
	}
	else                            // board move
	{
		if(nPo == EMPTY_SQUARE)     // non-promotion board move
		{
			for(nI = 0 ; nI < Moves.nMove ; nI++)
			{
				if((Moves.nMoveType [nI] != MOVE_DROP) &&
						(Moves.nMoveType [nI] != MOVE_PROMOTE))
				{
					if((Moves.nFromX [nI] == nX) &&
							(Moves.nFromY [nI] == nY) &&
							(Moves.nToX   [nI] == nX1) &&
							(Moves.nToY   [nI] == nY1))
					{
						Moves.nLastMatch = nI;
						return 1;
					}
				}
			}
		}
		else                        // promotion board move
		{
			for(nI = 0 ; nI < Moves.nMove ; nI++)
			{
				if(Moves.nMoveType [nI] == MOVE_PROMOTE)
				{
					if((Moves.nPromote [nI] == nPo) &&
							(Moves.nFromX   [nI] == nX) &&
							(Moves.nFromY   [nI] == nY) &&
							(Moves.nToX     [nI] == nX1) &&
							(Moves.nToY     [nI] == nY1))
					{
						Moves.nLastMatch = nI;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int CHESS_IsLegalTruePremove(int nG, int nPc, int nX, int nY, int nX1, int nY1, int nPo)
{
	int nI;

	// make sure destination square index is valid
	if((nX1 < 0) || (nX1 > 7) || (nY1 < 0) || (nY1 > 7))
	{
		return 0;
	}

	// set temporary move to zero
	TempMoves.nMove = 0;

	if((nX == -1) || (nY == -1))    // drop move
	{
		if((nPc == WHITE_PAWN) || (nPc == BLACK_PAWN))
		{
			if((nY1 == 0) || (nY1 == 7))
			{
				return 0;
			}
		}

		if(Game [nG].nBuffer [nPc] <= 0)
		{
			return 0;
		}

		CHESS_AddTempMove(-1, -1, nX1, nY1, MOVE_DROP, EMPTY_SQUARE, nPc, EMPTY_SQUARE);
	}
	else                            // board move
	{
		// make sure source square index is valid
		if((nX < 0) || (nX > 7) || (nY < 0) || (nY > 7))
		{
			return 0;
		}

		// in true premove
		bInTruePremove = 1;

		// not in CHESS_IsLegalMoveString ()
		bInIsLegalMoveString = 0;

		switch(nPc)
		{
			case WHITE_PAWN:
				CHESS_GenWhitePawn(nG, nX, nY);
				break;

			case WHITE_ROOK:
				CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_KNIGHT :
				CHESS_GenKnight(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_BISHOP :
				CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_QUEEN :
				CHESS_GenDiagonal(nG, nX, nY, INDEX_WHITE);
				CHESS_GenHorizontal(nG, nX, nY, INDEX_WHITE);
				break;

			case WHITE_KING :
				CHESS_GenWhiteKing(nG, nX, nY);
				break;

			case BLACK_PAWN:
				CHESS_GenBlackPawn(nG, nX, nY);
				break;

			case BLACK_ROOK:
				CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_KNIGHT :
				CHESS_GenKnight(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_BISHOP :
				CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_QUEEN :
				CHESS_GenDiagonal(nG, nX, nY, INDEX_BLACK);
				CHESS_GenHorizontal(nG, nX, nY, INDEX_BLACK);
				break;

			case BLACK_KING :
				CHESS_GenBlackKing(nG, nX, nY);
				break;
		}

		// not in true premove
		bInTruePremove = 0;
	}

	// no move generated
	if(TempMoves.nMove == 0)
	{
		return 0;
	}

	// reset legal move count
	Moves.nMove = 0;

	// reset last match index
	Moves.nLastMatch = -1;

	for(nI = 0 ; nI < TempMoves.nMove ; nI++)
	{
		// save all moves as legal moves
		CHESS_CopyTempMove(nI);
	}

	// no generated move
	if(Moves.nMove == 0)
	{
		return 0;
	}

	// search for move
	if((nX == -1) || (nY == -1))    // drop move
	{
		for(nI = 0 ; nI < Moves.nMove ; nI++)
		{
			if(Moves.nMoveType [nI] == MOVE_DROP)
			{
				if((Moves.nDrop [nI] == nPc) &&
						(Moves.nToX  [nI] == nX1) &&
						(Moves.nToY  [nI] == nY1))
				{
					Moves.nLastMatch = nI;
					return 1;
				}
			}
		}
	}
	else                            // board move
	{
		if(nPo == EMPTY_SQUARE)     // non-promotion board move
		{
			for(nI = 0 ; nI < Moves.nMove ; nI++)
			{
				if((Moves.nMoveType [nI] != MOVE_DROP) &&
						(Moves.nMoveType [nI] != MOVE_PROMOTE))
				{
					if((Moves.nFromX [nI] == nX) &&
							(Moves.nFromY [nI] == nY) &&
							(Moves.nToX   [nI] == nX1) &&
							(Moves.nToY   [nI] == nY1))
					{
						Moves.nLastMatch = nI;
						return 1;
					}
				}
			}
		}
		else                        // promotion board move
		{
			for(nI = 0 ; nI < Moves.nMove ; nI++)
			{
				if(Moves.nMoveType [nI] == MOVE_PROMOTE)
				{
					if((Moves.nPromote [nI] == nPo) &&
							(Moves.nFromX   [nI] == nX) &&
							(Moves.nFromY   [nI] == nY) &&
							(Moves.nToX     [nI] == nX1) &&
							(Moves.nToY     [nI] == nY1))
					{
						Moves.nLastMatch = nI;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int CHESS_IsLegalExamDropMove(int nG, int nPc, int nX1, int nY1)
{
	int nC, nI, nCount, bDirect;

	// not in true premove
	bInTruePremove = 0;

	// not in CHESS_IsLegalMoveString ()
	bInIsLegalMoveString = 0;

	// make sure destination square index is valid
	if((nX1 < 0) || (nX1 > 7) || (nY1 < 0) || (nY1 > 7))
	{
		return 0;
	}

	// set temporary move to zero
	TempMoves.nMove = 0;

	// drop move
	if((nPc == WHITE_PAWN) || (nPc == BLACK_PAWN))
	{
		if((nY1 == 0) || (nY1 == 7))
		{
			return 0;
		}
	}

	if(! CHESS_IsEmptySquare(nG, nX1, nY1))
	{
		return 0;
	}

	if((nPc == WHITE_PAWN) || (nPc == WHITE_ROOK) || (nPc == WHITE_KNIGHT) ||
			(nPc == WHITE_BISHOP) || (nPc == WHITE_QUEEN) || (nPc == WHITE_KING))
	{
		nC = INDEX_WHITE;
	}
	else
	{
		nC = INDEX_BLACK;
	}

	CHESS_AddTempMove(-1, -1, nX1, nY1, MOVE_DROP, EMPTY_SQUARE, nPc, EMPTY_SQUARE);

	// no move generated
	if(TempMoves.nMove == 0)
	{
		return 0;
	}

	// reset legal move count
	Moves.nMove = 0;

	// reset last match index
	Moves.nLastMatch = -1;

	// filter out illegal moves
	nCount  = 0;
	bDirect = 0;
	for(nI = 0 ; nI < TempMoves.nMove ; nI++)
	{
		// backup position
		CHESS_BackupPosition(nG);

		// make move
		CHESS_MakeMove(nG,
					   TempMoves.nFromX    [nI],
					   TempMoves.nFromY    [nI],
					   TempMoves.nToX      [nI],
					   TempMoves.nToY      [nI],
					   TempMoves.nMoveType [nI],
					   TempMoves.nCapture  [nI],
					   TempMoves.nDrop     [nI],
					   TempMoves.nPromote  [nI]);

		// check to see if it's a legal move
		if(! CHESS_SquareInCheck(nG,
								 nC,
								 Game [nG].nKingX [nC],
								 Game [nG].nKingY [nC],
								 &nCount,
								 &bDirect))
		{
			// save as a legal move
			CHESS_CopyTempMove(nI);
		}

		// restore position
		CHESS_RestorePosition(nG);
	}

	// no legal moves
	if(Moves.nMove == 0)
	{
		return 0;
	}

	// search for move
	for(nI = 0 ; nI < Moves.nMove ; nI++)
	{
		if(Moves.nMoveType [nI] == MOVE_DROP)
		{
			if((Moves.nDrop [nI] == nPc) &&
					(Moves.nToX  [nI] == nX1) &&
					(Moves.nToY  [nI] == nY1))
			{
				Moves.nLastMatch = nI;
				return 1;
			}
		}
	}
	return 0;
}

int CHESS_IsLegalPartnerBoardMove(int nG, int nPc, int nX, int nY, int nX1, int nY1, int nPo)
{
	int nI;

	// not in true premove
	bInTruePremove = 0;

	// not in CHESS_IsLegalMoveString ()
	bInIsLegalMoveString = 0;

	// make sure destination square index is valid
	if((nX1 < 0) || (nX1 > 7) || (nY1 < 0) || (nY1 > 7))
	{
		return 0;
	}

	// generate all legal moves (except drop move)
	CHESS_GenMove(nG, 0, 1);

	// no legal moves
	if(Moves.nMove == 0)
	{
		return 0;
	}

	// search for move
	if((nX == -1) || (nY == -1))    // drop move
	{
		for(nI = 0 ; nI < Moves.nMove ; nI++)
		{
			if(Moves.nMoveType [nI] == MOVE_DROP)
			{
				if((Moves.nDrop [nI] == nPc) &&
						(Moves.nToX  [nI] == nX1) &&
						(Moves.nToY  [nI] == nY1))
				{
					Moves.nLastMatch = nI;
					return 1;
				}
			}
		}
	}
	else                            // board move
	{
		if(nPo == EMPTY_SQUARE)     // non-promotion board move
		{
			for(nI = 0 ; nI < Moves.nMove ; nI++)
			{
				if((Moves.nMoveType [nI] != MOVE_DROP) &&
						(Moves.nMoveType [nI] != MOVE_PROMOTE))
				{
					if((Moves.nFromX [nI] == nX) &&
							(Moves.nFromY [nI] == nY) &&
							(Moves.nToX   [nI] == nX1) &&
							(Moves.nToY   [nI] == nY1))
					{
						Moves.nLastMatch = nI;
						return 1;
					}
				}
			}
		}
		else                        // promotion board move
		{
			for(nI = 0 ; nI < Moves.nMove ; nI++)
			{
				if(Moves.nMoveType [nI] == MOVE_PROMOTE)
				{
					if((Moves.nPromote [nI] == nPo) &&
							(Moves.nFromX   [nI] == nX) &&
							(Moves.nFromY   [nI] == nY) &&
							(Moves.nToX     [nI] == nX1) &&
							(Moves.nToY     [nI] == nY1))
					{
						Moves.nLastMatch = nI;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int CHESS_GenKingLegalSquare(int nG, int nC)
{
	int nX, nY, nI, nJ, nA, nB, nKing, bFound, nCount, bDirect;

	nX = Game [nG].nKingX [nC];
	nY = Game [nG].nKingY [nC];

	if((nX < 0) || (nY < 0))
	{
		return 0;
	}

	nKing = Game [nG].nBoard [nX] [nY];
	Game [nG].nBoard [nX] [nY] = EMPTY_SQUARE;

	bFound  = 0;
	nCount  = 0;
	bDirect = 0;

	if(Game [nG].nGameType == GAMETYPE_FICS_ATOMIC)
	{
		for(nI = 0 ; nI < 8 ; nI++)
		{
			nA = nX + _KingMoveX [nI];
			nB = nY + _KingMoveY [nI];

			if(nA >= 0 && nA <= 7 && nB >= 0 && nB <= 7)
			{
				if(Game [nG].nBoard [nA] [nB] == EMPTY_SQUARE)
				{
					if(CHESS_SquareInCheck(nG,
										   nC,
										   nA,
										   nB,
										   &nCount,
										   &bDirect))
					{
						if(CHESS_OpponentKingIsNear(nG, nC, nA, nB))
						{
							Game [nG].ptKing [nC] [nI].x = nA;
							Game [nG].ptKing [nC] [nI].y = nB;

							for(nJ = 0 ; nJ < 8 ; nJ++)
							{
								if((Game [nG].ptLastKing [nC] [nJ].x == nA) &&
										(Game [nG].ptLastKing [nC] [nJ].y == nB))
								{
									Game [nG].ptLastKing [nC] [nJ].x = -1;
									Game [nG].ptLastKing [nC] [nJ].y = -1;
									break;
								}
							}

							bFound = 1;
						}
					}
					else
					{
						Game [nG].ptKing [nC] [nI].x = nA;
						Game [nG].ptKing [nC] [nI].y = nB;

						for(nJ = 0 ; nJ < 8 ; nJ++)
						{
							if((Game [nG].ptLastKing [nC] [nJ].x == nA) &&
							   (Game [nG].ptLastKing [nC] [nJ].y == nB))
							{
								Game [nG].ptLastKing [nC] [nJ].x = -1;
								Game [nG].ptLastKing [nC] [nJ].y = -1;
								break;
							}
						}

						bFound = 1;
					}
				}
			}
		}
	}
	else
	{
		for(nI = 0 ; nI < 8 ; nI++)
		{
			nA = nX + _KingMoveX [nI];
			nB = nY + _KingMoveY [nI];

			if(nA >= 0 && nA <= 7 && nB >= 0 && nB <= 7)
			{
				if(! CHESS_IsSameColor(nG, nA, nB, nC))
				{
					if(! CHESS_SquareInCheck(nG,
											 nC,
											 nA,
											 nB,
											 &nCount,
											 &bDirect))
					{
						Game [nG].ptKing [nC] [nI].x = nA;
						Game [nG].ptKing [nC] [nI].y = nB;

						for(nJ = 0 ; nJ < 8 ; nJ++)
						{
							if((Game [nG].ptLastKing [nC] [nJ].x == nA) &&
									(Game [nG].ptLastKing [nC] [nJ].y == nB))
							{
								Game [nG].ptLastKing [nC] [nJ].x = -1;
								Game [nG].ptLastKing [nC] [nJ].y = -1;
								break;
							}
						}

						bFound = 1;
					}
				}
			}
		}
	}

	Game [nG].nBoard [nX] [nY] = nKing;

	return bFound;
}

void CHESS_AtomicCapture(int nG, int nSX, int nSY)
{
	int nI, nX, nY;

	Game [nG].nBoard [nSX] [nSY] = EMPTY_SQUARE;

	for(nI = 0 ; nI < 8 ; nI++)
	{
		nX = nSX + _KingMoveX [nI];
		nY = nSY + _KingMoveY [nI];

		if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
		{
			if((Game [nG].nBoard [nX] [nY] == EMPTY_SQUARE) ||
			   (Game [nG].nBoard [nX] [nY] == WHITE_PAWN) ||
			   (Game [nG].nBoard [nX] [nY] == BLACK_PAWN))
			{
			}
			else
			{
				if(Game [nG].nBoard [nX] [nY] == WHITE_KING)
				{
					Game [nG].nKingX [INDEX_WHITE] = -1;
					Game [nG].nKingY [INDEX_WHITE] = -1;
				}
				else if(Game [nG].nBoard [nX] [nY] == BLACK_KING)
				{
					Game [nG].nKingX [INDEX_BLACK] = -1;
					Game [nG].nKingY [INDEX_BLACK] = -1;
				}

				Game [nG].nBoard [nX] [nY] = EMPTY_SQUARE;
			}
		}
	}
}

int CHESS_OpponentKingIsNear(int nG, int nC, int nSX, int nSY)
{
	int nK, nI, nX, nY;

	nK = (nC == INDEX_WHITE ? BLACK_KING : WHITE_KING);

	for(nI = 0 ; nI < 8 ; nI++)
	{
		nX = nSX + _KingMoveX [nI];
		nY = nSY + _KingMoveY [nI];

		if(nX >= 0 && nX <= 7 && nY >= 0 && nY <= 7)
		{
			if(Game [nG].nBoard [nX] [nY] == nK)
			{
				return 1;
			}
		}
	}
	return 0;
}