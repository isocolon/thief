#include "thief.h"

void POSITION_Init(int nI)
{
	Game [nI].bFirstGame        = 1 ;
	Game [nI].bErrorMoveList    = 0 ;
	Game [nI].bRelation4        = 0 ;
	Game [nI].bClickedButton    = 0 ;
	Game [nI].nCurrentIndex     = -2 ;
	Game [nI].nCurrentColor     = -2 ;
	Game [nI].nMinIndex         = MAX_POSITION ;
	Game [nI].nMinColor         = -2 ;
	Game [nI].nMaxIndex         = -2 ;
	Game [nI].nMaxColor         = -2 ;
	Game [nI].nMoveListMaxIndex = -2 ;
	Game [nI].nMoveListMaxColor = -2 ;
}

void POSITION_FirstTime(int nI)
{
	int nX, nY ;

	Game [nI].bErrorMoveList               = 0 ;
	Game [nI].bRelation4                   = 0 ;
	Game [nI].bClickedButton               = 0 ;
	Game [nI].nInitDoublePushFile          = Game [nI].nLastDoublePushFile ;
	Game [nI].bInitKingSide  [INDEX_WHITE] = Game [nI].bCanCastleKingSide  [INDEX_WHITE] ;
	Game [nI].bInitQueenSide [INDEX_WHITE] = Game [nI].bCanCastleQueenSide [INDEX_WHITE] ;
	Game [nI].bInitKingSide  [INDEX_BLACK] = Game [nI].bCanCastleKingSide  [INDEX_BLACK] ;
	Game [nI].bInitQueenSide [INDEX_BLACK] = Game [nI].bCanCastleQueenSide [INDEX_BLACK] ;
	Game [nI].nInitHalfMoves               = Game [nI].nHalfMoves ;
	Game [nI].nInitKingX     [INDEX_WHITE] = Game [nI].nKingX              [INDEX_WHITE] ;
	Game [nI].nInitKingY     [INDEX_WHITE] = Game [nI].nKingY              [INDEX_WHITE] ;
	Game [nI].nInitKingX     [INDEX_BLACK] = Game [nI].nKingX              [INDEX_BLACK] ;
	Game [nI].nInitKingY     [INDEX_BLACK] = Game [nI].nKingY              [INDEX_BLACK] ;

	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			Game [nI].nInitBoard    [nX] [nY] = Game [nI].nBoard [nX] [nY] ;
			Game [nI].nPromoteBoard [nX] [nY] = 0 ;
		}
	}

	for(nX = 0 ; nX < MAX_BUFFER ; nX++)
	{
		Game [nI].nInitBuffer [nX] = Game [nI].nBuffer [nX] ;
	}

	Game [nI].bInitInitialMove = Game [nI].bInitialMove ;
	Game [nI].bInitWhitesMove  = Game [nI].bWhitesMove ;
	Game [nI].nInitMoveNumber  = Game [nI].nMoveNumber ;

	strcpy(Game [nI].cInitLastMove,  Game [nI].cLastMove) ;
	strcpy(Game [nI].cInitTime4Move, Game [nI].cTime4Move) ;

	Game [nI].nCurrentIndex     = -1 ;
	Game [nI].nCurrentColor     = -1 ;
	Game [nI].nMinIndex         = MAX_POSITION ;
	Game [nI].nMinColor         = -1 ;
	Game [nI].nMaxIndex         = -1 ;
	Game [nI].nMaxColor         = -1 ;
	Game [nI].nMoveListMaxIndex = -1 ;
	Game [nI].nMoveListMaxColor = -1 ;
}

void POSITION_AddMoveList(int nI, int nJ, int nC, char *cMove, char *cTm4Move)
{
	int nL, nX, nY ;
	int bReset, nWK, nWQ, nBK, nBQ ;
	char cPosition [255], cTmpMove [50] ;

	// make sure it's ok to press move buttons for this game
	if(! TOOLBOX_OKMoveButton(nI))
	{
		return ;
	}

	// make sure it's not bigger or equal to MAX_POSITION
	if(nJ >= MAX_POSITION)
	{
		return ;
	}

	// update maximum move list number and color
	Game [nI].nMoveListMaxIndex = nJ ;
	Game [nI].nMoveListMaxColor = nC ;

	// it errored before then we will skip it
	if(Game [nI].bErrorMoveList)
	{
		// copy last move stuff
		strcpy(Game [nI].Position [nJ].cLastMove  [nC], cMove) ;
		strcpy(Game [nI].Position [nJ].cTime4Move [nC], cTm4Move) ;

		// set time remaining stuff to -9999L
		Game [nI].Position [nJ].nTime [nC] = -9999L ;
		return ;
	}

	// starting position --> index 0, white to move
	if((nJ == 0) && (nC == 0))
	{
		// assign game type
		Game [TEMP_GAME_INDEX].nGameType = Game [nI].nGameType ;

		// check game type
		if(Login.nLoginType == SERVER_ICC)
		{
			switch(Game [TEMP_GAME_INDEX].nGameType)
			{
				case GAMETYPE_CHESS :
				case GAMETYPE_ICC_BULLET :
				case GAMETYPE_ICC_BLITZ :
				case GAMETYPE_ICC_STANDARD :
				case GAMETYPE_ICC_5_MINUTE :
				case GAMETYPE_ICC_1_MINUTE :
				case GAMETYPE_LOSER :
				case GAMETYPE_CRAZYHOUSE :
				case GAMETYPE_BUGHOUSE :
				case GAMETYPE_GIVEAWAY :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD1 :
				case GAMETYPE_ICC_WILD2 :
				case GAMETYPE_ICC_WILD3 :
				case GAMETYPE_ICC_WILD4 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				case GAMETYPE_WILD5 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 0 ;
						nWQ    = 0 ;
						nBK    = 0 ;
						nBQ    = 0 ;
						strcpy(cPosition, INIT_WILD5_BOARD) ;
					}
					break ;

				case GAMETYPE_ICC_WILD6 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD7 :
					bReset = 1 ;
					nWK    = 0 ;
					nWQ    = 0 ;
					nBK    = 0 ;
					nBQ    = 0 ;
					strcpy(cPosition, INIT_ICC_WILD7_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD8 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_ICC_WILD8_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD9 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_ICC_WILD9_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD10 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_ICC_WILD10_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD11 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_ICC_WILD11_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD12 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 0 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_ICC_WILD12_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD13 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_ICC_WILD13_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD14 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 0 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_ICC_WILD14_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD15 :
					bReset = 1 ;
					nWK    = 0 ;
					nWQ    = 0 ;
					nBK    = 0 ;
					nBQ    = 0 ;
					strcpy(cPosition, INIT_ICC_WILD15_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD16 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				case GAMETYPE_ICC_WILD18 :
					bReset = 1 ;
					nWK    = 0 ;
					nWQ    = 0 ;
					nBK    = 0 ;
					nBQ    = 0 ;
					strcpy(cPosition, INIT_ICC_WILD18_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD19 :
					bReset = 1 ;
					nWK    = 0 ;
					nWQ    = 0 ;
					nBK    = 0 ;
					nBQ    = 0 ;
					strcpy(cPosition, INIT_ICC_WILD19_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD20 :
				case GAMETYPE_ICC_WILD21 :
				case GAMETYPE_ICC_WILD22 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD25 :
				case GAMETYPE_ICC_WILD27 :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_BOARD) ;
					break ;

				case GAMETYPE_ICC_WILD28 :
					bReset = 1 ;
					nWK    = 0 ;
					nWQ    = 0 ;
					nBK    = 0 ;
					nBQ    = 0 ;
					strcpy(cPosition, INIT_ICC_WILD28_BOARD) ;
					break ;

				default :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_BOARD) ;
					break ;
			}
		}
		else
		{
			switch(Game [TEMP_GAME_INDEX].nGameType)
			{
				case GAMETYPE_CHESS :
				case GAMETYPE_FICS_LIGHTNING :
				case GAMETYPE_FICS_BLITZ :
				case GAMETYPE_FICS_STANDARD :
				case GAMETYPE_SUICIDE :
				case GAMETYPE_CRAZYHOUSE :
				case GAMETYPE_BUGHOUSE :
				case GAMETYPE_FICS_ATOMIC :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_BOARD) ;
					break ;

				case GAMETYPE_FICS_WILD0 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_FICS_WILD0_BOARD) ;
					}
					break ;

				case GAMETYPE_FICS_WILD1 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				case GAMETYPE_FICS_WILD2 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				case GAMETYPE_FICS_WILD3 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				case GAMETYPE_FICS_WILD4 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				case GAMETYPE_WILD5 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 0 ;
						nWQ    = 0 ;
						nBK    = 0 ;
						nBQ    = 0 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				case GAMETYPE_FICS_WILD8 :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_FICS_WILD8_BOARD) ;
					}
					break ;

				case GAMETYPE_FICS_WILD8A :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_FICS_WILD8A_BOARD) ;
					}
					break ;

				case GAMETYPE_FICS_WILDFR :
					if(Game [nI].bRelation4)
					{
						bReset = 0 ;
					}
					else
					{
						bReset = 1 ;
						nWK    = 1 ;
						nWQ    = 1 ;
						nBK    = 1 ;
						nBQ    = 1 ;
						strcpy(cPosition, INIT_BOARD) ;
					}
					break ;

				default :
					bReset = 1 ;
					nWK    = 1 ;
					nWQ    = 1 ;
					nBK    = 1 ;
					nBQ    = 1 ;
					strcpy(cPosition, INIT_BOARD) ;
					break ;
			}
		}

		if(bReset)
		{
			Game [TEMP_GAME_INDEX].bWhitesMove                       = 1 ;
			Game [TEMP_GAME_INDEX].bInitialMove                      = 1 ;
			Game [TEMP_GAME_INDEX].nMoveNumber                       = 1 ;
			Game [TEMP_GAME_INDEX].nLastDoublePushFile               = -1 ;
			Game [TEMP_GAME_INDEX].bCanCastleKingSide  [INDEX_WHITE] = nWK ;
			Game [TEMP_GAME_INDEX].bCanCastleQueenSide [INDEX_WHITE] = nWQ ;
			Game [TEMP_GAME_INDEX].bCanCastleKingSide  [INDEX_BLACK] = nBK ;
			Game [TEMP_GAME_INDEX].bCanCastleQueenSide [INDEX_BLACK] = nBQ ;
			Game [TEMP_GAME_INDEX].nHalfMoves                        = 0 ;
			Game [TEMP_GAME_INDEX].bChessGame                        = Game [nI].bChessGame ;

			BOARD_LoadBoard(TEMP_GAME_INDEX, cPosition, ICS_INITIAL_MOVE_NAME) ;
			BOARD_LoadBuffer(TEMP_GAME_INDEX, INIT_BUFFER, INIT_BUFFER) ;
			BOARD_ResetPromoteBoard(TEMP_GAME_INDEX) ;

			Game [nI].nInitDoublePushFile          = Game [TEMP_GAME_INDEX].nLastDoublePushFile ;
			Game [nI].bInitKingSide  [INDEX_WHITE] = Game [TEMP_GAME_INDEX].bCanCastleKingSide  [INDEX_WHITE] ;
			Game [nI].bInitQueenSide [INDEX_WHITE] = Game [TEMP_GAME_INDEX].bCanCastleQueenSide [INDEX_WHITE] ;
			Game [nI].bInitKingSide  [INDEX_BLACK] = Game [TEMP_GAME_INDEX].bCanCastleKingSide  [INDEX_BLACK] ;
			Game [nI].bInitQueenSide [INDEX_BLACK] = Game [TEMP_GAME_INDEX].bCanCastleQueenSide [INDEX_BLACK] ;
			Game [nI].nInitHalfMoves               = Game [TEMP_GAME_INDEX].nHalfMoves ;
			Game [nI].nInitKingX     [INDEX_WHITE] = Game [TEMP_GAME_INDEX].nKingX              [INDEX_WHITE] ;
			Game [nI].nInitKingY     [INDEX_WHITE] = Game [TEMP_GAME_INDEX].nKingY              [INDEX_WHITE] ;
			Game [nI].nInitKingX     [INDEX_BLACK] = Game [TEMP_GAME_INDEX].nKingX              [INDEX_BLACK] ;
			Game [nI].nInitKingY     [INDEX_BLACK] = Game [TEMP_GAME_INDEX].nKingY              [INDEX_BLACK] ;

			for(nY = 0 ; nY < 8 ; nY++)
			{
				for(nX = 0 ; nX < 8 ; nX++)
				{
					Game [nI].nInitBoard    [nX] [nY] = Game [TEMP_GAME_INDEX].nBoard [nX] [nY] ;
					Game [nI].nPromoteBoard [nX] [nY] = 0 ;
				}
			}

			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [nI].nInitBuffer [nX] = Game [TEMP_GAME_INDEX].nBuffer [nX] ;
			}

			Game [nI].bInitInitialMove = 1 ;
			Game [nI].bInitWhitesMove  = 1 ;
			Game [nI].nInitMoveNumber  = 1 ;

			strcpy(Game [nI].cInitLastMove,  ICS_INITIAL_MOVE_NAME) ;
			strcpy(Game [nI].cInitTime4Move, ICS_INITIAL_TIME_MOVE) ;
		}
		else
		{
			Game [TEMP_GAME_INDEX].bChessGame                        = Game [nI].bChessGame ;
			Game [TEMP_GAME_INDEX].nLastDoublePushFile               = Game [nI].nInitDoublePushFile ;
			Game [TEMP_GAME_INDEX].bCanCastleKingSide  [INDEX_WHITE] = Game [nI].bInitKingSide  [INDEX_WHITE] ;
			Game [TEMP_GAME_INDEX].bCanCastleQueenSide [INDEX_WHITE] = Game [nI].bInitQueenSide [INDEX_WHITE] ;
			Game [TEMP_GAME_INDEX].bCanCastleKingSide  [INDEX_BLACK] = Game [nI].bInitKingSide  [INDEX_BLACK] ;
			Game [TEMP_GAME_INDEX].bCanCastleQueenSide [INDEX_BLACK] = Game [nI].bInitQueenSide [INDEX_BLACK] ;
			Game [TEMP_GAME_INDEX].nHalfMoves                        = Game [nI].nHalfMoves ;
			Game [TEMP_GAME_INDEX].nKingX              [INDEX_WHITE] = Game [nI].nInitKingX     [INDEX_WHITE] ;
			Game [TEMP_GAME_INDEX].nKingY              [INDEX_WHITE] = Game [nI].nInitKingY     [INDEX_WHITE] ;
			Game [TEMP_GAME_INDEX].nKingX              [INDEX_BLACK] = Game [nI].nInitKingX     [INDEX_BLACK] ;
			Game [TEMP_GAME_INDEX].nKingY              [INDEX_BLACK] = Game [nI].nInitKingY     [INDEX_BLACK] ;

			for(nY = 0 ; nY < 8 ; nY++)
			{
				for(nX = 0 ; nX < 8 ; nX++)
				{
					Game [TEMP_GAME_INDEX].nBoard        [nX] [nY] = Game [nI].nInitBoard [nX] [nY] ;
					Game [TEMP_GAME_INDEX].nPromoteBoard [nX] [nY] = 0 ;
				}
			}

			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [TEMP_GAME_INDEX].nBuffer [nX] = Game [nI].nInitBuffer [nX] ;
			}
		}
	}

	// remove the last '+' or '#' symbol from the move string
	strcpy(cTmpMove, cMove) ;

	nL = strlen(cTmpMove) ;
	if((cTmpMove [nL - 1] == '+') || (cTmpMove [nL - 1] == '#'))
	{
		cTmpMove [nL - 1] = NULL_CHAR ;
	}

	// assign who's turn to move
	Game [TEMP_GAME_INDEX].bWhitesMove = (nC == 0) ;

	// issue the move to the chess legal move engine
	if(! CHESS_IsLegalMoveString(TEMP_GAME_INDEX, cTmpMove))
	{
		POSITION_FirstTime(nI) ;
		Game [nI].bErrorMoveList = 1 ;

//      if (nC == 0)
//          {
//          sprintf (cPosition, "Errored on %d. %s\n\n", nJ + 1, cMove) ;
//          }
//      else
//          {
//          sprintf (cPosition, "Errored on %d... %s\n\n", nJ + 1, cMove) ;
//          }
//      TOOLBOX_WriteSystem (cPosition) ;

		return ;
	}

	// execute the move
	CHESS_MakeMove(TEMP_GAME_INDEX,
				   Moves.nFromX    [Moves.nLastMatch],
				   Moves.nFromY    [Moves.nLastMatch],
				   Moves.nToX      [Moves.nLastMatch],
				   Moves.nToY      [Moves.nLastMatch],
				   Moves.nMoveType [Moves.nLastMatch],
				   Moves.nCapture  [Moves.nLastMatch],
				   Moves.nDrop     [Moves.nLastMatch],
				   Moves.nPromote  [Moves.nLastMatch]) ;

	// copy position
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			Game [nI].Position [nJ].nBoard [nC] [nX] [nY] = Game [TEMP_GAME_INDEX].nBoard [nX] [nY] ;
		}
	}

	// copy buffer
	for(nX = 0 ; nX < MAX_BUFFER ; nX++)
	{
		Game [nI].Position [nJ].nBuffer [nC] [nX] = Game [TEMP_GAME_INDEX].nBuffer [nX] ;
	}

	// copy last move stuff
	strcpy(Game [nI].Position [nJ].cLastMove  [nC], cMove) ;
	strcpy(Game [nI].Position [nJ].cTime4Move [nC], cTm4Move) ;

	// set time remaining stuff to -9999L
	Game [nI].Position [nJ].nTime [nC] = -9999L ;

	// assign minimum
	if(Game [nI].nMinIndex == MAX_POSITION)
	{
		Game [nI].nMinIndex = nJ ;
		Game [nI].nMinColor = nC ;
	}
	else if(nJ < Game [nI].nMinIndex)
	{
		Game [nI].nMinIndex = nJ ;
		Game [nI].nMinColor = nC ;
	}
	else if(nJ == Game [nI].nMinIndex)
	{
		if(nC < Game [nI].nMinColor)
		{
			Game [nI].nMinColor = nC ;
		}
	}

	// assign maximum
	if((Game [nI].nMaxIndex == -1) || (Game [nI].nMaxIndex == -2))
	{
		Game [nI].nMaxIndex = nJ ;
		Game [nI].nMaxColor = nC ;
	}
	else if(nJ > Game [nI].nMaxIndex)
	{
		Game [nI].nMaxIndex = nJ ;
		Game [nI].nMaxColor = nC ;
	}
	else if(nJ == Game [nI].nMaxIndex)
	{
		if(nC > Game [nI].nMaxColor)
		{
			Game [nI].nMaxColor = nC ;
		}
	}

	// assign current
	Game [nI].nCurrentIndex = nJ ;
	Game [nI].nCurrentColor = nC ;
}

void POSITION_SaveGameMove(int nI)
{
	int nJ, nC, nX, nY ;

	// make sure it's ok to press move buttons for this game
	if(! TOOLBOX_OKMoveButton(nI))
	{
		return ;
	}

	// if it's initial position then initialize the current position as the first position
	if(Game [nI].nMaxIndex == -2)
	{
		POSITION_FirstTime(nI) ;
		return ;
	}

	// make sure it's not bigger or equal to MAX_POSITION
	nJ = (Game [nI].nMoveNumber - 1) ;
	if(nJ >= MAX_POSITION)
	{
		POSITION_FirstTime(nI) ;
		return ;
	}

	// if it's the initial position then set it as the initial position
	if(Game [nI].bInitialMove)
	{
		POSITION_FirstTime(nI) ;
		return ;
	}

	if(nJ == 0)
	{
		if(Game [nI].bLastWhitesMove)
		{
			nC = 1 ;
		}
		else
		{
			nC = 0 ;
		}
	}
	else
	{
		if(Game [nI].bLastWhitesMove)
		{
			nJ = nJ - 1 ;
			nC = 1 ;
		}
		else
		{
			nC = 0 ;
		}
	}

	// copy position
	for(nY = 7 ; nY >= 0 ; nY--)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			Game [nI].Position [nJ].nBoard [nC] [nX] [nY] = Game [nI].nBoard [nX] [nY] ;
		}
	}

	// copy buffer
	for(nX = 0 ; nX < MAX_BUFFER ; nX++)
	{
		Game [nI].Position [nJ].nBuffer [nC] [nX] = Game [nI].nBuffer [nX] ;
	}

	// update maximum move list number and color
	Game [nI].nMoveListMaxIndex = nJ ;
	Game [nI].nMoveListMaxColor = nC ;

	// copy last move stuff
	strcpy(Game [nI].Position [nJ].cLastMove  [nC], Game [nI].cLastMove) ;
	strcpy(Game [nI].Position [nJ].cTime4Move [nC], Game [nI].cTime4Move) ;

	// copy time remaining stuff
	Game [nI].Position [nJ].nTime [nC] = Game [nI].nTimeRemaining [nC] ;

	// assign minimum
	if(Game [nI].nMinIndex == MAX_POSITION)
	{
		Game [nI].nMinIndex = nJ ;
		Game [nI].nMinColor = nC ;
	}
	else if(nJ < Game [nI].nMinIndex)
	{
		Game [nI].nMinIndex = nJ ;
		Game [nI].nMinColor = nC ;
	}
	else if(nJ == Game [nI].nMinIndex)
	{
		if(nC < Game [nI].nMinColor)
		{
			Game [nI].nMinColor = nC ;
		}
	}

	// assign maximum
	Game [nI].nMaxIndex = nJ ;
	Game [nI].nMaxColor = nC ;

	// assign current
	if(Game [nI].bClickedButton)
	{
		if(Game [nI].nCurrentIndex != -1)
		{
			if((Game [nI].nCurrentIndex == -2)                 ||
					(Game [nI].nCurrentIndex < Game [nI].nMinIndex) ||
					((Game [nI].nCurrentIndex == Game [nI].nMinIndex) &&
					 (Game [nI].nCurrentColor <  Game [nI].nMinColor)))
			{
				Game [nI].nCurrentIndex = Game [nI].nMinIndex ;
				Game [nI].nCurrentColor = Game [nI].nMinColor ;
			}
			else if((Game [nI].nCurrentIndex > Game [nI].nMaxIndex) ||
					((Game [nI].nCurrentIndex == Game [nI].nMaxIndex) &&
					 (Game [nI].nCurrentColor >  Game [nI].nMaxColor)))
			{
				Game [nI].nCurrentIndex = Game [nI].nMaxIndex ;
				Game [nI].nCurrentColor = Game [nI].nMaxColor ;
			}
		}
	}
	else
	{
		Game [nI].nCurrentIndex = nJ ;
		Game [nI].nCurrentColor = nC ;
	}

	// save it into bughouse moves (if applicable)
	BUGHOUSE_SaveGameMove(nI, nJ, nC) ;
}

void POSITION_FICS_SaveGameBuffer(int nI, int bNext)
{
	int nJ, nC, nX ;

	//
	// update move list position buffer
	//
	// <b1> game 71 white [] black []           (PREVIOUS)
	//
	// <b1> game 71 white [N] black [] <- WN    (NEXT)
	//
	nJ = (Game [nI].nMoveNumber - 1) ;
	if(nJ >= MAX_POSITION)
	{
		POSITION_FirstTime(nI) ;
	}
	else
	{
		if(bNext)
		{
			//
			// update piece buffer on the next move
			//
			if(Game [nI].bLastWhitesMove)
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
		{
			//
			// update piece buffer on the previous move
			//
			if(nJ == 0)
			{
				if(Game [nI].bLastWhitesMove)
				{
					nC = 1 ;
				}
				else
				{
					nC = 0 ;
				}
			}
			else
			{
				if(Game [nI].bLastWhitesMove)
				{
					nJ = nJ - 1 ;
					nC = 1 ;
				}
				else
				{
					nC = 0 ;
				}
			}
		}

		if((nJ == 0) && (nC == 0))
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [nI].nInitBuffer [nX] = Game [nI].nBuffer [nX] ;
			}
		}
		else
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [nI].Position [nJ].nBuffer [nC] [nX] = Game [nI].nBuffer [nX] ;
			}
		}
	}
}

void POSITION_ICC_SaveGameBuffer(int nI)
{
	int nJ, nC, nX ;

	//
	// update move list piece buffer
	//
	// icc sends the piece buffer update after the move is made
	// (after style 12 string) thus the piece buffer update has to be made
	// for the previous move.
	//
	nJ = (Game [nI].nMoveNumber - 1) ;
	if(nJ >= MAX_POSITION)
	{
		POSITION_FirstTime(nI) ;
	}
	else
	{
		if(nJ == 0)
		{
			if(Game [nI].bLastWhitesMove)
			{
				nC = 1 ;
			}
			else
			{
				nC = 0 ;
			}
		}
		else
		{
			if(Game [nI].bLastWhitesMove)
			{
				nJ = nJ - 1 ;
				nC = 1 ;
			}
			else
			{
				nC = 0 ;
			}
		}

		if((nJ == 0) && (nC == 0))
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [nI].nInitBuffer [nX] = Game [nI].nBuffer [nX] ;
			}
		}
		else
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [nI].Position [nJ].nBuffer [nC] [nX] = Game [nI].nBuffer [nX] ;
			}
		}
	}
}

void POSITION_NONFICS_SaveGameBuffer(int nI, int bNext)
{
	int nJ, nC, nX ;

	//
	// update move list position buffer
	//
	// <b1> game 71 white [] black []           (PREVIOUS)
	//
	// <b1> game 71 white [N] black [] <- WN    (NEXT)
	//
	nJ = (Game [nI].nMoveNumber - 1) ;
	if(nJ >= MAX_POSITION)
	{
		POSITION_FirstTime(nI) ;
	}
	else
	{
		if(bNext)
		{
			//
			// update piece buffer on the next move
			//
			if(Game [nI].bLastWhitesMove)
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
		{
			//
			// update piece buffer on the previous move
			//
			if(nJ == 0)
			{
				if(Game [nI].bLastWhitesMove)
				{
					nC = 1 ;
				}
				else
				{
					nC = 0 ;
				}
			}
			else
			{
				if(Game [nI].bLastWhitesMove)
				{
					nJ = nJ - 1 ;
					nC = 1 ;
				}
				else
				{
					nC = 0 ;
				}
			}
		}

		if((nJ == 0) && (nC == 0))
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [nI].nInitBuffer [nX] = Game [nI].nBuffer [nX] ;
			}
		}
		else
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				Game [nI].Position [nJ].nBuffer [nC] [nX] = Game [nI].nBuffer [nX] ;
			}
		}
	}
}
