#include "thief.h"

void GAME_Init(void)
{
	int nI, nJ ;

	for(nI = 0 ; nI < (MAX_GAME + 1) ; nI++)
	{
		Game [nI].hWindow = NULL ;
		Game [nI].hSave   = NULL ;
		Game [nI].hTemp   = NULL ;

		for(nJ = 0 ; nJ < MAX_SQUARE ; nJ++)
		{
			Game [nI].hSquare [nJ] = NULL ;
		}

		for(nJ = 0 ; nJ < MAX_PIECE ; nJ++)
		{
			Game [nI].hPiece [nJ] = NULL ;
		}
	}
}

void GAME_Destroy(void)
{
	int nI, nJ ;

	for(nI = 0 ; nI < (MAX_GAME + 1) ; nI++)
	{
		if(Game [nI].hWindow)
		{
			DeleteObject(Game [nI].hWindow) ;
			Game [nI].hWindow = NULL ;
		}

		if(Game [nI].hSave)
		{
			DeleteObject(Game [nI].hSave) ;
			Game [nI].hSave = NULL ;
		}

		if(Game [nI].hTemp)
		{
			DeleteObject(Game [nI].hTemp) ;
			Game [nI].hTemp = NULL ;
		}

		for(nJ = 0 ; nJ < MAX_SQUARE ; nJ++)
		{
			if(Game [nI].hSquare [nJ])
			{
				DeleteObject(Game [nI].hSquare [nJ]) ;
				Game [nI].hSquare [nJ] = NULL ;
			}
		}

		for(nJ = 0 ; nJ < MAX_PIECE ; nJ++)
		{
			if(Game [nI].hPiece [nJ])
			{
				DeleteObject(Game [nI].hPiece [nJ]) ;
				Game [nI].hPiece [nJ] = NULL ;
			}
		}
	}
}

void GAME_InitAllGame(int bAll)
{
	int nI, nJ ;

	for(nI = 0 ; nI < (MAX_GAME + 1) ; nI++)
	{
		if(bAll)
		{
			Game [nI].hwnd   = NULL ;
			Game [nI].bValid = 0 ;
		}
		else
		{
			if(nI != INDEX_PLAY)
			{
				Game [nI].hwnd   = NULL ;
				Game [nI].bValid = 0 ;
			}
		}

		Game [nI].bFirstResize    = 1 ;
		Game [nI].bFirstGame      = 1 ;
		Game [nI].bFlip           = 0 ;
		Game [nI].nGameNumber     = 0 ;
		Game [nI].nGamePartner    = 0 ;
		Game [nI].nRelation       = -999 ;

		if(nI == INDEX_PLAY)
		{
			Game [nI].nGameType = System.nSysPlayGameType ;
		}
		else
		{
			Game [nI].nGameType = System.nSysObserveGameType ;
		}

		Game [nI].bChessGame      = 0 ;
		Game [nI].bWhitesMove     = 1 ;
		Game [nI].bLastWhitesMove = 1 ;
		Game [nI].bPlaying        = 0 ;
		Game [nI].bIPlayWhite     = 1 ;
		Game [nI].bInitialMove    = 1 ;
		Game [nI].nMoveNumber     = 1 ;

		strcpy(Game [nI].cLastMove,             ICS_INITIAL_MOVE_NAME) ;
		strcpy(Game [nI].cVerboseMove,          ICS_INITIAL_MOVE_NAME) ;
		strcpy(Game [nI].cTime4Move,            ICS_INITIAL_TIME_MOVE) ;
		strcpy(Game [nI].cHandle [INDEX_WHITE], ICS_INITIAL_WHITE_HANDLE) ;
		strcpy(Game [nI].cHandle [INDEX_BLACK], ICS_INITIAL_BLACK_HANDLE) ;
		strcpy(Game [nI].cRating [INDEX_WHITE], ICS_INITIAL_WHITE_RATING) ;
		strcpy(Game [nI].cRating [INDEX_BLACK], ICS_INITIAL_BLACK_RATING) ;

		Game [nI].nRated = ICS_INITIAL_RATED ;

		strcpy(Game [nI].cOrgGameType, ICS_INITIAL_GAMETYPE_STRING) ;
		strcpy(Game [nI].cGameType,    ICS_INITIAL_GAMETYPE_STRING) ;
		strcpy(Game [nI].cOrgResult,   "") ;
		strcpy(Game [nI].cResult,      "") ;

		BOARD_LoadBoard(nI, INIT_BOARD, ICS_INITIAL_MOVE_NAME) ;
		BOARD_LoadBuffer(nI, INIT_BUFFER, INIT_BUFFER) ;

		BOARD_SaveLastBoardBuffer(nI) ;

		Game [nI].nLastDoublePushFile               = -1 ;
		Game [nI].bCanCastleKingSide  [INDEX_WHITE] = 1 ;
		Game [nI].bCanCastleQueenSide [INDEX_WHITE] = 1 ;
		Game [nI].bCanCastleKingSide  [INDEX_BLACK] = 1 ;
		Game [nI].bCanCastleQueenSide [INDEX_BLACK] = 1 ;

		BOARD_NullHighlight(nI) ;
		BOARD_SaveLastHighlight(nI) ;

		Game [nI].nInitialClock                = 0 ;
		Game [nI].nIncrementClock              = 0 ;
		Game [nI].nClockTimerEvent             = 0 ;
		Game [nI].bTickClock                   = 1 ;
		Game [nI].bFlagged [INDEX_WHITE]       = 0 ;
		Game [nI].bFlagged [INDEX_BLACK]       = 0 ;
		Game [nI].nTimeRemaining [INDEX_WHITE] = 0 ;
		Game [nI].nTimeRemaining [INDEX_BLACK] = 0 ;

		Game [nI].tTickStartTM.nSec   = 0 ;
		Game [nI].tTickStartTM.nMs    = 0 ;
		Game [nI].nIntendedTickLength = 0 ;
		Game [nI].bOnLagClock         = 0 ;
		Game [nI].nNextLagCheck       = 1000 ;
		Game [nI].nLag [INDEX_WHITE]  = 0 ;
		Game [nI].nLag [INDEX_BLACK]  = 0 ;

		POSITION_Init(nI) ;

		for(nJ = 0 ; nJ < 13 ; nJ++)
		{
			Game [nI].ChessValue [nJ] = 0 ;
			Game [nI].BugValue   [nJ] = 0 ;
		}
	}
}

void GAME_InitialSetup(int nI)
{
	int nJ ;

	Game [nI].bFirstResize    = 1 ;
	Game [nI].bFirstGame      = 1 ;
	Game [nI].bValid          = 1 ;
	Game [nI].bFlip           = 0 ;
	Game [nI].nGameNumber     = 0 ;
	Game [nI].nGamePartner    = 0 ;
	Game [nI].nRelation       = -999 ;

	if((Game [nI].nGameType == GAMETYPE_BUGHOUSE) ||
			(Game [nI].nGameType == GAMETYPE_CRAZYHOUSE))
	{

		// change it to bughouse game type
		Game [nI].nGameType = GAMETYPE_BUGHOUSE ;
	}
	else
	{

		// change it to chess game type
		Game [nI].nGameType = GAMETYPE_CHESS ;
	}

	Game [nI].bChessGame      = 0 ;
	Game [nI].bWhitesMove     = 1 ;
	Game [nI].bLastWhitesMove = 1 ;
	Game [nI].bPlaying        = 0 ;
	Game [nI].bIPlayWhite     = 1 ;
	Game [nI].bInitialMove    = 1 ;
	Game [nI].nMoveNumber     = 1 ;

	strcpy(Game [nI].cLastMove,             ICS_INITIAL_MOVE_NAME) ;
	strcpy(Game [nI].cVerboseMove,          ICS_INITIAL_MOVE_NAME) ;
	strcpy(Game [nI].cTime4Move,            ICS_INITIAL_TIME_MOVE) ;
	strcpy(Game [nI].cHandle [INDEX_WHITE], ICS_INITIAL_WHITE_HANDLE) ;
	strcpy(Game [nI].cHandle [INDEX_BLACK], ICS_INITIAL_BLACK_HANDLE) ;
	strcpy(Game [nI].cRating [INDEX_WHITE], ICS_INITIAL_WHITE_RATING) ;
	strcpy(Game [nI].cRating [INDEX_BLACK], ICS_INITIAL_BLACK_RATING) ;

	Game [nI].nRated = ICS_INITIAL_RATED ;

	strcpy(Game [nI].cOrgGameType, ICS_INITIAL_GAMETYPE_STRING) ;
	strcpy(Game [nI].cGameType,    ICS_INITIAL_GAMETYPE_STRING) ;
	strcpy(Game [nI].cOrgResult,   "") ;
	strcpy(Game [nI].cResult,      "") ;

	BOARD_LoadBoard(nI, INIT_BOARD, ICS_INITIAL_MOVE_NAME) ;
	BOARD_LoadBuffer(nI, INIT_BUFFER, INIT_BUFFER) ;

	BOARD_SaveLastBoardBuffer(nI) ;

	Game [nI].nLastDoublePushFile               = -1 ;
	Game [nI].bCanCastleKingSide  [INDEX_WHITE] = 1 ;
	Game [nI].bCanCastleQueenSide [INDEX_WHITE] = 1 ;
	Game [nI].bCanCastleKingSide  [INDEX_BLACK] = 1 ;
	Game [nI].bCanCastleQueenSide [INDEX_BLACK] = 1 ;

	BOARD_NullHighlight(nI) ;
	BOARD_SaveLastHighlight(nI) ;

	Game [nI].nInitialClock                = 0 ;
	Game [nI].nIncrementClock              = 0 ;
	Game [nI].nClockTimerEvent             = 0 ;
	Game [nI].bTickClock                   = 1 ;
	Game [nI].bFlagged [INDEX_WHITE]       = 0 ;
	Game [nI].bFlagged [INDEX_BLACK]       = 0 ;
	Game [nI].nTimeRemaining [INDEX_WHITE] = 0 ;
	Game [nI].nTimeRemaining [INDEX_BLACK] = 0 ;

	Game [nI].tTickStartTM.nSec   = 0 ;
	Game [nI].tTickStartTM.nMs    = 0 ;
	Game [nI].nIntendedTickLength = 0 ;
	Game [nI].bOnLagClock         = 0 ;
	Game [nI].nNextLagCheck       = 1000 ;
	Game [nI].nLag [INDEX_WHITE]  = 0 ;
	Game [nI].nLag [INDEX_BLACK]  = 0 ;

	if(nI == INDEX_PLAY)
	{
		Premove.nPremoveCount = 0 ;
		Premove.nPremoveHead  = 0 ;
		Premove.nPremoveTail  = 0 ;

		SetWindowText(Game [INDEX_PLAY].hwnd, PLAY_BOARD_TITLE) ;
	}

	POSITION_Init(nI) ;

	for(nJ = 0 ; nJ < 13 ; nJ++)
	{
		Game [nI].ChessValue [nJ] = 0 ;
		Game [nI].BugValue   [nJ] = 0 ;
	}
}
