#include "thief.h"

void BUGHOUSE_Init(void)
{
	int nG ;

	for(nG = 0 ; nG < MAX_GAME ; nG++)
	{
		Bughouse [nG].nMainIndex    = -1 ;
		Bughouse [nG].nPartnerIndex = -1 ;
		Bughouse [nG].nMove         = 0 ;
	}
}

void BUGHOUSE_FirstTime(int nG)
{
	Bughouse [nG].nMainIndex    = -1 ;
	Bughouse [nG].nPartnerIndex = -1 ;
	Bughouse [nG].nMove         = 0 ;
}

void BUGHOUSE_EndMoveList(int nG)
{
	int nI, nJ, nK, nY, nX, nC, nM, bDrop, bSide1, bSide2 ;

	// not a bughouse game
	if(Game [nG].nGameType != GAMETYPE_BUGHOUSE)
	{
		return ;
	}

	// copy promote board from Game [TEMP_GAME_INDEX] into Game [nG]
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			Game [nG].nPromoteBoard [nX] [nY] = Game [TEMP_GAME_INDEX].nPromoteBoard [nX] [nY] ;
		}
	}

	// it's a bughouse game but don't have partner's game number yet
	if(Game [nG].nGamePartner == 0)
	{
		BUGHOUSE_FirstTime(nG) ;
		return ;
	}

	// look for partner's game number
	nJ = -1 ;
	for(nK = 0 ; nK < MAX_GAME ; nK++)
	{
		if(Game [nK].bPlaying)
		{
			if(Game [nK].nGameNumber == Game [nG].nGamePartner)
			{
				nJ = nK ;
				break ;
			}
		}
	}

	// partner's game number not found
	if(nJ == -1)
	{
		BUGHOUSE_FirstTime(nG) ;
		return ;
	}

	//
	// the other board has not been set so lets decide which board is main and partner
	//
	if(nG == INDEX_PLAY)
	{
		// set the other board as the partner board
		nI = nG ;   // nI = main board
//      nJ = nJ ;   // nJ = partner board
	}
	else
	{
		// set the other board as the main board
		nI = nJ ;   // nI = main board
		nJ = nG ;   // nJ = partner board
	}

	Bughouse [nI].nMainIndex    = nI ;
	Bughouse [nI].nPartnerIndex = nJ ;

	Bughouse [nJ].nMainIndex    = nI ;
	Bughouse [nJ].nPartnerIndex = nJ ;

	// no moves from the move lists then start from initial position
	if((Game [nI].nMoveListMaxIndex < 0) && (Game [nJ].nMoveListMaxIndex < 0))
	{
		Bughouse [nI].nMainMove     = -1 ;
		Bughouse [nI].nMainColor    = -1 ;
		Bughouse [nI].nPartnerMove  = -1 ;
		Bughouse [nI].nPartnerColor = -1 ;

		Bughouse [nI].nMove = 0 ;
		Bughouse [nJ].nMove = 0 ;

		Bughouse [nI].bFromInitialPosition = 1 ;
		Bughouse [nJ].bFromInitialPosition = 1 ;
		return ;
	}

	// all the moves are on the main board
	if((Game [nI].nMoveListMaxIndex >= 0) && (Game [nJ].nMoveListMaxIndex < 0))
	{
		Bughouse [nI].nMainMove     = -1 ;
		Bughouse [nI].nMainColor    = -1 ;
		Bughouse [nI].nPartnerMove  = -1 ;
		Bughouse [nI].nPartnerColor = -1 ;

		Bughouse [nI].nMove = 0 ;
		Bughouse [nJ].nMove = 0 ;

		Bughouse [nI].bFromInitialPosition = 1 ;
		Bughouse [nJ].bFromInitialPosition = 1 ;

		nM = 0 ;
		nK = 0 ;
		nC = 0 ;

		while(nK <= Game [nI].nMoveListMaxIndex)
		{
			Bughouse [nI].nIndex  [nM] = nI ;
			Bughouse [nI].nNumber [nM] = nK ;
			Bughouse [nI].nColor  [nM] = nC ;

			Bughouse [nI].nMainMove  = nK ;
			Bughouse [nI].nMainColor = nC ;

			nM = nM + 1 ;

			if(nK == Game [nI].nMoveListMaxIndex)
			{
				if(nC == Game [nI].nMoveListMaxColor)
				{
					break ;
				}
				else
				{
					nC = 1 ;
				}
			}
			else
			{
				if(nC == 0)
				{
					nC = 1 ;
				}
				else
				{
					nK = nK + 1 ;
					nC = 0 ;
				}
			}
		}

		Bughouse [nI].nMove = nM ;
		return ;
	}

	// all the moves are on the partner board
	if((Game [nI].nMoveListMaxIndex < 0) && (Game [nJ].nMoveListMaxIndex >= 0))
	{
		Bughouse [nI].nMainMove     = -1 ;
		Bughouse [nI].nMainColor    = -1 ;
		Bughouse [nI].nPartnerMove  = -1 ;
		Bughouse [nI].nPartnerColor = -1 ;

		Bughouse [nI].nMove = 0 ;
		Bughouse [nJ].nMove = 0 ;

		Bughouse [nI].bFromInitialPosition = 1 ;
		Bughouse [nJ].bFromInitialPosition = 1 ;

		nM = 0 ;
		nK = 0 ;
		nC = 0 ;

		while(nK <= Game [nJ].nMoveListMaxIndex)
		{
			Bughouse [nI].nIndex  [nM] = nJ ;
			Bughouse [nI].nNumber [nM] = nK ;
			Bughouse [nI].nColor  [nM] = nC ;

			Bughouse [nI].nPartnerMove  = nK ;
			Bughouse [nI].nPartnerColor = nC ;

			nM = nM + 1 ;

			if(nK == Game [nJ].nMoveListMaxIndex)
			{
				if(nC == Game [nJ].nMoveListMaxColor)
				{
					break ;
				}
				else
				{
					nC = 1 ;
				}
			}
			else
			{
				if(nC == 0)
				{
					nC = 1 ;
				}
				else
				{
					nK = nK + 1 ;
					nC = 0 ;
				}
			}
		}

		Bughouse [nI].nMove = nM ;
		return ;
	}

	//
	// moves are on both boards
	// see if less than MAX_NO_DROP_MOVE moves on both board and no drop moves
	// then can start from initial position
	//
	if((Game [nI].nMoveListMaxIndex < MAX_NO_DROP_MOVE) &&
			(Game [nJ].nMoveListMaxIndex < MAX_NO_DROP_MOVE))
	{
		bDrop = 0 ;
		nK    = 0 ;
		nC    = 0 ;
		while(nK <= Game [nI].nMoveListMaxIndex)
		{
			if(Game [nI].Position [nK].cLastMove [nC] [1] == '@')
			{
				bDrop = 1 ;
				break ;
			}

			if(nK == Game [nI].nMoveListMaxIndex)
			{
				if(nC == Game [nI].nMoveListMaxColor)
				{
					break ;
				}
				else
				{
					nC = 1 ;
				}
			}
			else
			{
				if(nC == 0)
				{
					nC = 1 ;
				}
				else
				{
					nK = nK + 1 ;
					nC = 0 ;
				}
			}
		}

		if(! bDrop)
		{
			nK = 0 ;
			nC = 0 ;
			while(nK <= Game [nJ].nMoveListMaxIndex)
			{
				if(Game [nJ].Position [nK].cLastMove [nC] [1] == '@')
				{
					bDrop = 1 ;
					break ;
				}

				if(nK == Game [nJ].nMoveListMaxIndex)
				{
					if(nC == Game [nJ].nMoveListMaxColor)
					{
						break ;
					}
					else
					{
						nC = 1 ;
					}
				}
				else
				{
					if(nC == 0)
					{
						nC = 1 ;
					}
					else
					{
						nK = nK + 1 ;
						nC = 0 ;
					}
				}
			}
		}

		// start from initial position
		if(! bDrop)
		{
			Bughouse [nI].nMainMove     = -1 ;
			Bughouse [nI].nMainColor    = -1 ;
			Bughouse [nI].nPartnerMove  = -1 ;
			Bughouse [nI].nPartnerColor = -1 ;

			Bughouse [nI].bFromInitialPosition = 1 ;
			Bughouse [nJ].bFromInitialPosition = 1 ;

			bSide1 = 0 ;
			bSide2 = 0 ;
			nM     = 0 ;
			nK     = 0 ;
			nC     = 0 ;
			while(nK < MAX_NO_DROP_MOVE)
			{
				if(nK < Game [nI].nMoveListMaxIndex)
				{
					Bughouse [nI].nIndex  [nM] = nI ;
					Bughouse [nI].nNumber [nM] = nK ;
					Bughouse [nI].nColor  [nM] = nC ;

					Bughouse [nI].nMainMove  = nK ;
					Bughouse [nI].nMainColor = nC ;

					nM = nM + 1 ;
				}
				else if(nK == Game [nI].nMoveListMaxIndex)
				{
					Bughouse [nI].nIndex  [nM] = nI ;
					Bughouse [nI].nNumber [nM] = nK ;
					Bughouse [nI].nColor  [nM] = nC ;

					Bughouse [nI].nMainMove  = nK ;
					Bughouse [nI].nMainColor = nC ;

					nM = nM + 1 ;

					if(nC == Game [nI].nMoveListMaxColor)
					{
						bSide1 = 1 ;
					}
				}
				else
				{
					bSide1 = 1 ;
				}

				if(nK < Game [nJ].nMoveListMaxIndex)
				{
					Bughouse [nI].nIndex  [nM] = nJ ;
					Bughouse [nI].nNumber [nM] = nK ;
					Bughouse [nI].nColor  [nM] = nC ;

					Bughouse [nI].nPartnerMove  = nK ;
					Bughouse [nI].nPartnerColor = nC ;

					nM = nM + 1 ;
				}
				else if(nK == Game [nJ].nMoveListMaxIndex)
				{
					Bughouse [nI].nIndex  [nM] = nJ ;
					Bughouse [nI].nNumber [nM] = nK ;
					Bughouse [nI].nColor  [nM] = nC ;

					Bughouse [nI].nPartnerMove  = nK ;
					Bughouse [nI].nPartnerColor = nC ;

					nM = nM + 1 ;

					if(nC == Game [nJ].nMoveListMaxColor)
					{
						bSide2 = 1 ;
					}
				}
				else
				{
					bSide2 = 1 ;
				}

				if(bSide1 && bSide2)
				{
					break ;
				}
				else
				{
					if(nC == 0)
					{
						nC = 1 ;
					}
					else
					{
						nK = nK + 1 ;
						nC = 0 ;
					}
				}
			}

			Bughouse [nI].nMove = nM ;
			Bughouse [nJ].nMove = 0 ;
			return ;
		}
	}

	//
	// start from this position
	//
	Bughouse [nI].nMainMove     = -1 ;
	Bughouse [nI].nMainColor    = -1 ;
	Bughouse [nI].nPartnerMove  = -1 ;
	Bughouse [nI].nPartnerColor = -1 ;

	Bughouse [nI].nMove = 0 ;
	Bughouse [nJ].nMove = 0 ;

	Bughouse [nI].bFromInitialPosition = 0 ;
	Bughouse [nJ].bFromInitialPosition = 0 ;

	// copy board and promote board
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			Bughouse [nI].nMainBoard      [nX] [nY] = Game [nI].nBoard        [nX] [nY] ;
			Bughouse [nI].nMainPromote    [nX] [nY] = Game [nI].nPromoteBoard [nX] [nY] ;
			Bughouse [nI].nPartnerBoard   [nX] [nY] = Game [nJ].nBoard        [nX] [nY] ;
			Bughouse [nI].nPartnerPromote [nX] [nY] = Game [nJ].nPromoteBoard [nX] [nY] ;
		}
	}

	// copy buffer
	for(nX = 0 ; nX < MAX_BUFFER ; nX++)
	{
		Bughouse [nI].nMainBuffer    [nX] = Game [nI].nBuffer [nX] ;
		Bughouse [nI].nPartnerBuffer [nX] = Game [nJ].nBuffer [nX] ;
	}

	// copy last double push file and can castle king/queen side
	Bughouse [nI].nMainLastDoublePushFile               = Game [nI].nLastDoublePushFile ;
	Bughouse [nI].bMainCastleKingSide     [INDEX_WHITE] = Game [nI].bCanCastleKingSide  [INDEX_WHITE] ;
	Bughouse [nI].bMainCastleKingSide     [INDEX_BLACK] = Game [nI].bCanCastleKingSide  [INDEX_BLACK] ;
	Bughouse [nI].bMainCastleQueenSide    [INDEX_WHITE] = Game [nI].bCanCastleQueenSide [INDEX_WHITE] ;
	Bughouse [nI].bMainCastleQueenSide    [INDEX_BLACK] = Game [nI].bCanCastleQueenSide [INDEX_BLACK] ;

	Bughouse [nI].nPartnerLastDoublePushFile            = Game [nJ].nLastDoublePushFile ;
	Bughouse [nI].bPartnerCastleKingSide  [INDEX_WHITE] = Game [nJ].bCanCastleKingSide  [INDEX_WHITE] ;
	Bughouse [nI].bPartnerCastleKingSide  [INDEX_BLACK] = Game [nJ].bCanCastleKingSide  [INDEX_BLACK] ;
	Bughouse [nI].bPartnerCastleQueenSide [INDEX_WHITE] = Game [nJ].bCanCastleQueenSide [INDEX_WHITE] ;
	Bughouse [nI].bPartnerCastleQueenSide [INDEX_BLACK] = Game [nJ].bCanCastleQueenSide [INDEX_BLACK] ;

	// copy time remaining
	Bughouse [nI].nMainTimeRemaining    [INDEX_WHITE] = Game [nI].nTimeRemaining [INDEX_WHITE] ;
	Bughouse [nI].nMainTimeRemaining    [INDEX_BLACK] = Game [nI].nTimeRemaining [INDEX_BLACK] ;
	Bughouse [nI].nPartnerTimeRemaining [INDEX_WHITE] = Game [nJ].nTimeRemaining [INDEX_WHITE] ;
	Bughouse [nI].nPartnerTimeRemaining [INDEX_BLACK] = Game [nJ].nTimeRemaining [INDEX_BLACK] ;

	// copy white's move
	Bughouse [nI].bMainWhitesMove    = Game [nI].bLastWhitesMove ;
	Bughouse [nI].bPartnerWhitesMove = Game [nJ].bLastWhitesMove ;
}

void BUGHOUSE_SaveGameMove(int nG, int nJ, int nC)
{
	int nI, nM ;

	if((Game [nG].nRelation ==  0) ||   //  0 i am observing a game being played
			(Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
			(Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
	{
		// must be a bughouse game
		if(Game [nG].nGameType == GAMETYPE_BUGHOUSE)
		{
			// make sure there is a main board index
			if(Bughouse [nG].nMainIndex >= 0)
			{
				// get main board index
				nI = Bughouse [nG].nMainIndex ;

				// make sure move number and color is bigger than the last one
				if(nI == nG)
				{
					if(nJ < Bughouse [nI].nMainMove)
					{
						return ;
					}

					if(nJ == Bughouse [nI].nMainMove)
					{
						if(nC <= Bughouse [nI].nMainColor)
						{
							return ;
						}
					}

					// remember current move and color
					Bughouse [nI].nMainMove  = nJ ;
					Bughouse [nI].nMainColor = nC ;
				}
				else
				{
					if(nJ < Bughouse [nI].nPartnerMove)
					{
						return ;
					}

					if(nJ == Bughouse [nI].nPartnerMove)
					{
						if(nC <= Bughouse [nI].nPartnerColor)
						{
							return ;
						}
					}

					// remember current move and color
					Bughouse [nI].nPartnerMove  = nJ ;
					Bughouse [nI].nPartnerColor = nC ;
				}

				// save current move
				nM = Bughouse [nI].nMove ;

				Bughouse [nI].nMove        = nM + 1 ;
				Bughouse [nI].nIndex  [nM] = nG ;
				Bughouse [nI].nNumber [nM] = nJ ;
				Bughouse [nI].nColor  [nM] = nC ;
			}
		}
	}
}
