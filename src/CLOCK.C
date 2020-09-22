#include "thief.h"

#define FUDGE 25    /* 25ms = 1/40 sec should be plenty even for 50 Hz clocks */

int CLOCK_TimerRunning(int nG)
{
	return (Game [nG].nClockTimerEvent != 0) ;
}

int CLOCK_StopClockTimer(int nG)
{
	if(Game [nG].nClockTimerEvent == 0)
	{
		return 0 ;
	}
	KillTimer(Game [nG].hwnd, Game [nG].nClockTimerEvent) ;
	Game [nG].nClockTimerEvent = 0 ;
	return 1 ;
}

void CLOCK_StartClockTimer(int nG, long nMillSec)
{
	if(Game [nG].bTickClock)
	{
		Game [nG].nClockTimerEvent = SetTimer(Game [nG].hwnd, (UINT) TIMER_CLOCK_ID + nG, (UINT) nMillSec, NULL) ;
	}
	else
	{
		CLOCK_StopClockTimer(nG) ;
	}
}

void CLOCK_GetTimeMark(TIMEMARK *tm)
{
	struct timeb timeB ;

	ftime(&timeB) ;
	tm->nSec = (long) timeB.time ;
	tm->nMs  = (int)  timeB.millitm ;
}

long CLOCK_SubtractTimeMarks(TIMEMARK *tm2, TIMEMARK *tm1)
{
	return 1000L * (tm2->nSec - tm1->nSec) + (long)(tm2->nMs - tm1->nMs) ;
}

long CLOCK_NextTickLength(long nTimeRemaining)
{
	long nNominalTickLength, nNextTickLength ;

	if(Login.nLoginType == SERVER_FICS)
	{
		if(RedClock.bShowOneSecond)
		{
			if(nTimeRemaining > 0L && nTimeRemaining <= 1000L)
			{
				nNominalTickLength = 100L ;
			}
			else
			{
				if(RedClock.nShowTenthSecond == TENTH_NEVER)
				{
					nNominalTickLength = 1000L ;
				}
				else if(RedClock.nShowTenthSecond == TENTH_ALWAYS)
				{
					nNominalTickLength = 100L ;
				}
				else if(RedClock.nShowTenthSecond == TENTH_ONE_MINUTE)
				{
					if(nTimeRemaining > 0L && nTimeRemaining <= 60000L)
					{
						nNominalTickLength = 100L ;
					}
					else
					{
						nNominalTickLength = 1000L ;
					}
				}
				else
				{
					nNominalTickLength = 1000L ;
				}
			}
		}
		else
		{
			if(RedClock.nShowTenthSecond == TENTH_NEVER)
			{
				nNominalTickLength = 1000L ;
			}
			else if(RedClock.nShowTenthSecond == TENTH_ALWAYS)
			{
				nNominalTickLength = 100L ;
			}
			else if(RedClock.nShowTenthSecond == TENTH_ONE_MINUTE)
			{
				if(nTimeRemaining > 0L && nTimeRemaining <= 60000L)
				{
					nNominalTickLength = 100L ;
				}
				else
				{
					nNominalTickLength = 1000L ;
				}
			}
			else
			{
				nNominalTickLength = 1000L ;
			}
		}
	}
	else
	{
		if(RedClock.bShowOneSecond)
		{
			if(nTimeRemaining > 0L && nTimeRemaining <= 1000L)
			{
				nNominalTickLength = 100L ;
			}
			else
			{
				nNominalTickLength = 1000L ;
			}
		}
		else
		{
			nNominalTickLength = 1000L ;
		}
	}

	nNextTickLength = nTimeRemaining % nNominalTickLength ;

	if(nNextTickLength <= 0)
	{
		nNextTickLength += nNominalTickLength ;
	}
	return nNextTickLength ;
}

void CLOCK_ResetClocks(int nG)
{
	(void) CLOCK_StopClockTimer(nG) ;
	Game [nG].nTimeRemaining [INDEX_WHITE] = 0 ;
	Game [nG].nTimeRemaining [INDEX_BLACK] = 0 ;
}

void CLOCK_DecrementClocks(int nG, HWND hwnd)
{
	TIMEMARK now ;
	long     nLastTickLength, nFudge, nTimeRemaining ;
	int      nPGame, nI ;
	long     nLagBetweenBoards ;
	char     cTmp [512] ;

	if(! Game [nG].bValid || Game [nG].nGameNumber == 0)
	{
		return ;
	}

	if(Login.nLoginType != SERVER_FICS)
	{
		if(! Game [nG].bPlaying)
		{
			return ;
		}
	}

	CLOCK_GetTimeMark(&now) ;

	nLastTickLength = CLOCK_SubtractTimeMarks(&now, &Game [nG].tTickStartTM) ;

	if(User.bShowLagStat || User.bLagCommand)
	{
		if(Game [INDEX_PLAY].nGamePartner == 0)
		{
			nPGame = -1 ;

			if(Login.nLoginType == SERVER_NONFICS)
			{
				if(strlen(Vars.cPartner) > 0)
				{
					for(nI = 1 ; nI < MAX_GAME ; nI++)
					{
						if(Game [nI].bValid)
						{
							if(Game [nI].nGameNumber > 0)
							{
								if(Game [nI].bPlaying)
								{
									if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
											(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
									{
										Game [INDEX_PLAY].nGamePartner = Game [nI].nGameNumber ;
										Game [nI].nGamePartner = Game [INDEX_PLAY].nGameNumber ;

										nPGame = nI ;
										break ;
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			nPGame = TOOLBOX_FindGameByNumber(Game [INDEX_PLAY].nGamePartner) ;
		}

		// bug lag calculation
		if(User.bLagCommand && nG == INDEX_PLAY && ! Game [INDEX_PLAY].nIncrementClock && nPGame > 0)
		{
			if(Login.nLoginType == SERVER_FICS)
			{
				nLagBetweenBoards = abs(((Game [INDEX_PLAY].nLag [INDEX_WHITE] +
										  Game [nPGame    ].nLag [INDEX_BLACK]) -
										 (Game [nPGame    ].nLag [INDEX_WHITE] +
										  Game [INDEX_PLAY].nLag [INDEX_BLACK])) / 1000) ;
			}
			else
			{
				nLagBetweenBoards = abs(((Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] +
										  Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK]) -
										 (Game [nPGame    ].nTimeRemaining [INDEX_WHITE] +
										  Game [nPGame    ].nTimeRemaining [INDEX_BLACK])) / 1000) ;
			}

			if(nLagBetweenBoards >= LagCmd.nLagTolerance)
			{
				// check last lag command threshold
				if(LagCmd.tLastLagCommand.nSec == 0 && LagCmd.tLastLagCommand.nMs == 0)
				{
					nPGame = 1 ;
				}
				else
				{
					nPGame = (CLOCK_SubtractTimeMarks(&now, &LagCmd.tLastLagCommand) >= LagCmd.nLagLThreshold) ;
				}

				// write what the user sees
				if(nPGame)
				{
					LagCmd.tLastLagCommand = now ;

					if(LagCmd.nType != FUNCTION_NONE)
					{
						strcpy(cTmp, LagCmd.cLagDisplay) ;
						strcat(cTmp, "\n") ;

						TOOLBOX_WriteUser(cTmp) ;

						TOOLBOX_Command(LagCmd.nType, LagCmd.cLagCommand, User.bLagCommandAddHist) ;
					}
				}
			}
		}
	}

	// fudge if we woke up a little too soon
	nFudge = Game [nG].nIntendedTickLength - nLastTickLength ;
	if(nFudge < 0 || nFudge > FUDGE)
	{
		nFudge = 0 ;
	}
	if(Game [nG].bWhitesMove)
	{
		nTimeRemaining = Game [nG].nTimeRemaining [INDEX_WHITE] -= nLastTickLength ;
		BOARD_DisplayWhiteClock(nG, Game [nG].hwnd, Game [nG].nTimeRemaining [INDEX_WHITE] - nFudge) ;
	}
	else
	{
		nTimeRemaining = Game [nG].nTimeRemaining [INDEX_BLACK] -= nLastTickLength ;
		BOARD_DisplayBlackClock(nG, Game [nG].hwnd, Game [nG].nTimeRemaining [INDEX_BLACK] - nFudge) ;
	}
	CLOCK_CheckFlags() ;

	Game [nG].tTickStartTM        = now ;
	Game [nG].nIntendedTickLength = CLOCK_NextTickLength(nTimeRemaining - nFudge) + nFudge ;

	CLOCK_StartClockTimer(nG, Game [nG].nIntendedTickLength) ;

	if(User.bFinalCountDown)
	{
		TIMESOUND_Play(nG, nFudge) ;
	}
}

void CLOCK_SwitchClocks(int nG)
{
	TIMEMARK now ;
	long     nLastTickLength ;

	CLOCK_GetTimeMark(&now) ;

	if(CLOCK_StopClockTimer(nG))
	{
		nLastTickLength = CLOCK_SubtractTimeMarks(&now, &Game [nG].tTickStartTM) ;

		if(Game [nG].bWhitesMove)
		{
			Game [nG].nTimeRemaining [INDEX_WHITE] -= nLastTickLength ;
		}
		else
		{
			Game [nG].nTimeRemaining [INDEX_BLACK] -= nLastTickLength ;
		}
		CLOCK_CheckFlags() ;
	}

	if(! Game [nG].bValid || Game [nG].nGameNumber == 0)
	{
		return ;
	}

	if(Login.nLoginType != SERVER_FICS)
	{
		if(! Game [nG].bPlaying)
		{
			return ;
		}
	}

	Game [nG].tTickStartTM = now ;

	if(Game [nG].bWhitesMove)
	{
		Game [nG].nIntendedTickLength = CLOCK_NextTickLength(Game [nG].nTimeRemaining [INDEX_WHITE]) ;
	}
	else
	{
		Game [nG].nIntendedTickLength = CLOCK_NextTickLength(Game [nG].nTimeRemaining [INDEX_BLACK]) ;
	}

	CLOCK_StartClockTimer(nG, Game [nG].nIntendedTickLength) ;
}

void CLOCK_StopClocks(int nG, HWND hwnd)
{
	TIMEMARK now ;
	long nLastTickLength ;

	if(! CLOCK_StopClockTimer(nG))
	{
		return ;
	}

	if(! Game [nG].bValid || Game [nG].nGameNumber == 0)
	{
		return ;
	}

	if(Login.nLoginType != SERVER_FICS)
	{
		if(! Game [nG].bPlaying)
		{
			return ;
		}
	}

	CLOCK_GetTimeMark(&now) ;

	nLastTickLength = CLOCK_SubtractTimeMarks(&now, &Game [nG].tTickStartTM) ;

	if(Game [nG].bWhitesMove)
	{
		Game [nG].nTimeRemaining [INDEX_WHITE] -= nLastTickLength ;
		BOARD_DisplayWhiteClock(nG, Game [nG].hwnd, Game [nG].nTimeRemaining [INDEX_WHITE]) ;
	}
	else
	{
		Game [nG].nTimeRemaining [INDEX_BLACK] -= nLastTickLength ;
		BOARD_DisplayBlackClock(nG, Game [nG].hwnd, Game [nG].nTimeRemaining [INDEX_BLACK]) ;
	}

	CLOCK_CheckFlags() ;
}

void CLOCK_StartClocks(int nG, HWND hwnd)
{
	(void) CLOCK_StopClockTimer(nG) ;

	BOARD_DisplayWhiteClock(nG, Game [nG].hwnd, Game [nG].nTimeRemaining [INDEX_WHITE]) ;
	BOARD_DisplayBlackClock(nG, Game [nG].hwnd, Game [nG].nTimeRemaining [INDEX_BLACK]) ;

	if(! Game [nG].bValid || Game [nG].nGameNumber == 0)
	{
		return ;
	}

	if(Login.nLoginType != SERVER_FICS)
	{
		if(! Game [nG].bPlaying)
		{
			return ;
		}
	}

	CLOCK_CheckFlags() ;

	CLOCK_GetTimeMark(&Game [nG].tTickStartTM) ;

	if(Game [nG].bWhitesMove)
	{
		Game [nG].nIntendedTickLength = CLOCK_NextTickLength(Game [nG].nTimeRemaining [INDEX_WHITE]) ;
	}
	else
	{
		Game [nG].nIntendedTickLength = CLOCK_NextTickLength(Game [nG].nTimeRemaining [INDEX_BLACK]) ;
	}

	CLOCK_StartClockTimer(nG, Game [nG].nIntendedTickLength) ;
}

void CLOCK_CheckFlags(void)
{
	if(! Game [INDEX_PLAY].bValid || Game [INDEX_PLAY].nGameNumber == 0 || ! Game [INDEX_PLAY].bPlaying)
	{
		return ;
	}

	if(Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] <= 0)
	{
		if(! Game [INDEX_PLAY].bFlagged [INDEX_WHITE])
		{
			Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 1 ;

			if(User.bAutoFlag && ! Game [INDEX_PLAY].bIPlayWhite && ! Game [INDEX_PLAY].bFlagged [INDEX_BLACK])
			{
				TOOLBOX_WriteICS(ICS_FLAG_COMMAND) ;
				TOOLBOX_WriteUser(ICS_FLAG_COMMAND) ;
			}
		}
	}

	if(Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] <= 0)
	{
		if(! Game [INDEX_PLAY].bFlagged [INDEX_BLACK])
		{
			Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 1 ;

			if(User.bAutoFlag && Game [INDEX_PLAY].bIPlayWhite && ! Game [INDEX_PLAY].bFlagged [INDEX_WHITE])
			{
				TOOLBOX_WriteICS(ICS_FLAG_COMMAND) ;
				TOOLBOX_WriteUser(ICS_FLAG_COMMAND) ;
			}
		}
	}
}

char *CLOCK_TimeString(long nMs)
{
	int nType ;
	long nTime, nDay, nHour, nMinute, nSecond, nTenth ;
	char *cSign = "" ;

	static char cBuffer [32] ;

	if(Login.nLoginType == SERVER_FICS)
	{
		if(RedClock.bShowOneSecond)
		{
			if(nMs > 0L && nMs <= 900L)
			{
				nType = 1 ;
			}
			else
			{
				if(RedClock.nShowTenthSecond == TENTH_NEVER)
				{
					nType = 0 ;
				}
				else if(RedClock.nShowTenthSecond == TENTH_ALWAYS)
				{
					nType = 2 ;
				}
				else if(RedClock.nShowTenthSecond == TENTH_ONE_MINUTE)
				{
					if(nMs > 0L && nMs <= 59900L)
					{
						nType = 2 ;
					}
					else
					{
						nType = 0 ;
					}
				}
				else
				{
					nType = 0 ;
				}
			}
		}
		else
		{
			if(RedClock.nShowTenthSecond == TENTH_NEVER)
			{
				nType = 0 ;
			}
			else if(RedClock.nShowTenthSecond == TENTH_ALWAYS)
			{
				nType = 2 ;
			}
			else if(RedClock.nShowTenthSecond == TENTH_ONE_MINUTE)
			{
				if(nMs > 0L && nMs <= 59900L)
				{
					nType = 2 ;
				}
				else
				{
					nType = 0 ;
				}
			}
			else
			{
				nType = 0 ;
			}
		}
	}
	else
	{
		if(RedClock.bShowOneSecond)
		{
			if(nMs > 0L && nMs <= 900L)
			{
				nType = 1 ;
			}
			else
			{
				nType = 0 ;
			}
		}
		else
		{
			nType = 0 ;
		}
	}

	//
	// normal display
	//
	if(nType == 0)
	{
		nTime = (long) floor(((double)(nMs + 999L)) / 1000.0) ;
		if(nTime < 0)
		{
			cSign = "-" ;
			nTime = -nTime ;
		}

		nDay    = nTime / 86400L ;
		nTime   = nTime % 86400L ;
		nHour   = nTime / 3600L ;
		nTime   = nTime % 3600L ;
		nMinute = nTime / 60L ;
		nSecond = nTime % 60L ;

		if(nDay > 0)
		{
			sprintf(cBuffer, " %s%ld:%02ld:%02ld:%02ld ", cSign, nDay, nHour, nMinute, nSecond) ;
		}
		else if(nHour > 0)
		{
			sprintf(cBuffer, " %s%ld:%02ld:%02ld ", cSign, nHour, nMinute, nSecond) ;
		}
		else
		{
			sprintf(cBuffer, " %s%2ld:%02ld ", cSign, nMinute, nSecond) ;
		}
		return cBuffer ;
	}

	//
	// less than 1 second display
	//
	if(nType == 1)
	{
		sprintf(cBuffer, " 0.%1ld ", (nMs + 99L) / 100L) ;
		return cBuffer ;
	}

	//
	// tenth of a second display
	//
	if(nType == 2)
	{
		nTime = (long) floor(((double)(nMs + 99L)) / 100.0) ;
		if(nTime < 0)
		{
			cSign = "-" ;
			nTime = -nTime ;
		}

		nDay    = nTime / 864000L ;
		nTime   = nTime % 864000L ;
		nHour   = nTime / 36000L ;
		nTime   = nTime % 36000L ;
		nMinute = nTime / 600L ;
		nTime   = nTime % 600L ;
		nSecond = nTime / 10L ;
		nTenth  = nTime % 10L ;

		if(nDay > 0)
		{
			sprintf(cBuffer, " %s%ld:%02ld:%02ld:%02ld.%1ld ", cSign, nDay, nHour, nMinute, nSecond, nTenth) ;
		}
		else if(nHour > 0)
		{
			sprintf(cBuffer, " %s%ld:%02ld:%02ld.%1ld ", cSign, nHour, nMinute, nSecond, nTenth) ;
		}
		else
		{
			sprintf(cBuffer, " %s%2ld:%02ld.%1ld ", cSign, nMinute, nSecond, nTenth) ;
		}
		return cBuffer ;
	}

	return cBuffer ;
}
