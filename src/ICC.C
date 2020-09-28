#include "thief.h"

void ICC_ProcessBeforeLoginLine(char *cS)
{
	int      nI, nJ;
	char     cTmp [255], cTmp1 [255];
	COLORREF nC;

	if(cS [0] == '\\')
	{
		if(Telnet.bDisplayContinueLine)
		{
			Telnet.clrCurrent    = Telnet.clrLastLine;
			Telnet.nCurrentSound = SOUND_NONE;
			TELNET_Write(cS);
		}
		else
		{
			Telnet.bTelnetSkipLine = 1;
		}
		return;
	}
	Telnet.bDisplayContinueLine = 1;

//    if (cS [0] == '\\')
//        {
//        TELNET_Write (cS);
//        return;
//        }

	if(cS [0] == '\n')
	{
		TELNET_Write("\n");
		cS++;
	}

	if(cS [0] == BELL_CHAR)
	{
		cS++;
	}

	if(cS [1] == 'i' && cS [2] == 'c' && cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
	{
		cS += ICC_PROMPT_LENGTH;
	}

	while(*cS & 0x80)
	{
		cS++;
	}

	if(TELNET_MatchExp(cS, ICC_LOGIN))
	{
		if(Timeseal.nSocketLogin == 0)
		{
			nC = Telnet.clrCurrent;
			Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL];
			TELNET_Write(cS);

			strcpy(cTmp, Login.cLoginHandle);
			strcat(cTmp, "\n");
			Telnet.clrCurrent = clrColor [CLR_TELNET_USERTEXT];
			TELNET_ItalicWrite(cTmp);

			Telnet.clrCurrent = nC;

			TOOLBOX_WriteICS(Login.cLoginHandle);
			TOOLBOX_WriteICS("\n");

			Timeseal.nSocketLogin++;
			strcpy(Vars.cWhoAmI, Login.cLoginHandle);

			TOOLBOX_SetTelnetCaption();

			User.bIamAGuest = 0;
			TOOLBOX_CheckGuestMenu();
		}
		else
		{
			TOOLBOX_WriteSystem(ICS_CONN_LOST_DISPLAY);
			TOOLBOX_CloseSocket();
		}
		return;
	}

	if(TELNET_MatchExp(cS, ICC_PASSWORD))
	{
		strcpy(cTmp, cS);

		nJ = strlen(Login.cLoginPassword);

		Telnet.bFoundHandle = (nJ > 0);

		cTmp1 [0] = NULL_CHAR;

		for(nI = 0 ; nI < nJ ; nI++)
		{
			cTmp1 [nI]     = '*';
			cTmp1 [nI + 1] = NULL_CHAR;
		}

		strcat(cTmp, cTmp1);
		strcat(cTmp, "\n");

		nC = Telnet.clrCurrent;
		Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL];
		TELNET_Write(cTmp);
		Telnet.clrCurrent = nC;

		if(nJ > 0)
		{
			TOOLBOX_WriteICS(Login.cLoginPassword);
			TOOLBOX_WriteICS("\n");
			ICC_SendBeforeTwelve();
		}
		else
		{
			TOOLBOX_WriteSystem("\nMissing Password\n");
			TOOLBOX_WriteSystem(ICS_CONN_LOST_DISPLAY);
			TOOLBOX_CloseSocket();
		}
		return;
	}

	if(TELNET_MatchExp(cS, ICC_ENTER))
	{
		Telnet.bFoundHandle = 1;

		nJ = 0;
		for(nI = 36 ; nI < ((int) strlen(cS)) ; nI++)
		{
			if(cS [nI] == '"')
			{
				break;
			}
			Vars.cWhoAmI [nJ++] = cS [nI];
		}

		Vars.cWhoAmI [nJ] = NULL_CHAR;

		TOOLBOX_SetTelnetCaption();

		strcpy(cTmp, cS);
		strcat(cTmp, "\n");

		nC = Telnet.clrCurrent;
		Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL];
		TELNET_Write(cTmp);
		Telnet.clrCurrent = nC;
		ICC_SendBeforeTwelve();

		User.bIamAGuest = 1;
		TOOLBOX_CheckGuestMenu();
		return;
	}

	if(! Telnet.bFoundHandle)
	{
		if(TELNET_MatchExp(cS, ICC_LOGIN_GUEST))
		{
			Telnet.bFoundHandle = 1;

			nJ = 0;
			for(nI = 36 ; nI < ((int) strlen(cS)) ; nI++)
			{
				if(cS [nI] == '"')
				{
					break;
				}
				Vars.cWhoAmI [nJ++] = cS [nI];
			}
			Vars.cWhoAmI [nJ] = NULL_CHAR;

			TOOLBOX_SetTelnetCaption();

			strcpy(cTmp, cS);
			strcat(cTmp, "\n");

			nC = Telnet.clrCurrent;
			Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL];
			TELNET_Write(cTmp);
			Telnet.clrCurrent = nC;
			ICC_SendBeforeTwelve();

			User.bIamAGuest = 1;
			TOOLBOX_CheckGuestMenu();
			return;
		}
	}

	if(TELNET_MatchExp(cS, ICC_STYLE_12_SET) ||
			TELNET_MatchExp(cS, ICC_STYLE_12_SET1) ||
			TELNET_MatchExp(cS, ICC_TIMESTAMP_ACTIVATED) ||
			TELNET_MatchExp(cS, ICC_REATTACHING))
	{
		nC = Telnet.clrCurrent;
		Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL];
		TELNET_Write(cS);
		Telnet.clrCurrent = nC;

		Timeseal.bBeforeLogin = 0;
		return;
	}

	if(ICC_ColorIndex(cS))
	{
		TELNET_Write(cS);

		if(! EAR_Play(cS))
		{
			SOUND_Play();
		}
		Telnet.clrLastLine = Telnet.clrCurrent;
	}
	else
	{
		if(Telnet.bDisplayLine)
		{
			TELNET_Write(cS);
			SOUND_Play();
			Telnet.clrLastLine = Telnet.clrCurrent;
		}
	}

//    nC = Telnet.clrCurrent;
//    Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL];
//    TELNET_Write (cS);
//    Telnet.clrCurrent = nC;
}

void ICC_SendBeforeTwelve(void)
{
	TOOLBOX_WriteICS("\n");

	TOOLBOX_WriteICS(ICC_SET_INTERFACE_COMMAND);
	TOOLBOX_WriteICS(THIEF_VER);
	TOOLBOX_WriteICS("\n");

	TOOLBOX_WriteICS(ICC_SET_BELLOFF_COMMAND);
	TOOLBOX_WriteICS(ICC_SET_STYLE12_COMMAND);
}

void ICC_ProcessLine(char *cS)
{
	HDC hdc;
	int nF, nG, nI, nJ, nPartner, bPlaying, bPartner, bFound, nFlip;
	char *cP, *cQ, cTmp [2048], cNumber [30], cMove1 [500], cMove2 [500], cWRating [500], cWHandle [500], cBRating [500], cBHandle [500];

	if(cS [0] == BELL_CHAR)
	{
		cS++;
	}

	//
	// ICC sends line break before the aics% prompt therefore we need to
	// split into the linebreak and the rest of the chunk
	//
	if(cS [0] == '\n' && cS[1] != NULL_CHAR)
	{
		if(! Telnet.bTelnetSkipLine)
		{
			if(! Telnet.bLastIsTelnetEdit)
			{
				if((System.nIssuedMovesCommand > 0) && (System.nMoveListIndexNumber != -1))
				{
				}
				else
				{
					TELNET_Write("\n");
				}
			}
		}
		cS++;
	}

	Telnet.bLastIsTelnetEdit = 0;

	if(cS [1] == 'i' && cS [2] == 'c' && cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
	{
		cS += ICC_PROMPT_LENGTH;

		if(cS [1] == 'i' && cS [2] == 'c' && cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
		{
			cS += ICC_PROMPT_LENGTH;
		}

		if((cS [0] == NULL_CHAR) || (cS[0] == '\n' && cS [1] == NULL_CHAR))
		{
			Telnet.bTelnetSkipLine = 1;
			return;
		}
	}
	else
	{
		if((cS [0] == NULL_CHAR) || (cS [0] == '\n' && cS [1] == NULL_CHAR))
		{
			if(! Telnet.bTelnetSkipLine)
			{
				if((System.nIssuedMovesCommand > 0) && (System.nMoveListIndexNumber != -1))
				{
				}
				else
				{
					TELNET_Write(cS);
				}
			}
			return;
		}
	}

	if(cS [0] == '\n' && cS[1] != NULL_CHAR)
	{
		if((System.nIssuedMovesCommand > 0) && (System.nMoveListIndexNumber != -1))
		{
		}
		else
		{
			TELNET_Write("\n");
		}
		cS++;
	}

	if(Telnet.bHasESC)
	{
		nI = 0;
		nJ = 0;
		nG = strlen(cS);

		while(nI < nG)
		{
			if(cS [nI] == 27)
			{
				nI = nI + 4;
			}
			else
			{
				cTmp [nJ] = cS [nI];
				nI++;
				nJ++;
			}
		}

		cTmp [nJ] = NULL_CHAR;

		strcpy(cS, cTmp);

		Telnet.bHasESC = 0;
	}

	if(! strncmp(cS, ICC_12_INIT, 5))
	{
		if(ICC_ParseBoard(cS))
		{
			Telnet.bTelnetSkipLine = 1;
			return;
		}
		if(User.bEat12GameNotFoundLine)
		{
			Telnet.bTelnetSkipLine = 1;
			return;
		}
	}

	if(! strncmp(cS, ICC_B1_INIT, 5))
	{
		if(ICC_ParsePiece(cS))
		{
			Telnet.bTelnetSkipLine = 1;
			return;
		}
		if(User.bEatb1GameNotFoundLine)
		{
			Telnet.bTelnetSkipLine = 1;
			return;
		}
	}

	Telnet.bTelnetSkipLine = 0;

	if(! strncmp(cS, ICC_ILLEGAL_MOVE_1,  13) ||
			! strncmp(cS, ICC_ILLEGAL_MOVE_2,  16) ||
			! strncmp(cS, ICC_ILLEGAL_MOVE_3,  21) ||
			! strncmp(cS, ICC_NOT_YOUR_TURN_1, 20) ||
			! strncmp(cS, ICC_NOT_YOUR_TURN_2, 18))
	{
		if(Game [INDEX_PLAY].bPlaying)
		{
			TOOLBOX_WriteICS(ICC_REFRESH_COMMAND);
			TOOLBOX_WriteICS(Game [INDEX_PLAY].cHandle [INDEX_WHITE]);
			TOOLBOX_WriteICS("\n");
		}
		else
		{
			TOOLBOX_WriteICS(ICC_REFRESH_COMMAND1);
		}
		TELNET_NormalPrint(cS);
		return;
	}

	if(! strncmp(cS, ICC_PART_PLAYING_GAME, 29))
	{
		sscanf(cS + 29, "%d", &nG);
		Game [INDEX_PLAY].nGamePartner = nG;
		TELNET_NormalPrint(cS);
		return;
	}

	if(! strncmp(cS, ICC_STOP_OBS_GAME, 33))
	{
		TELNET_NormalPrint(cS);

		sscanf(cS + 33, "%d", &nG);

		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			if(Game [nI].nGameNumber == nG)
			{
				Game [nI].bFirstGame  = 0;
				Game [nI].nGameNumber = 0;
				Game [nI].bPlaying    = 0;
				Game [nI].bTickClock  = 1;

				if(IsWindow(Game [nI].hwnd))
				{
					SendMessage(Game [nI].hwnd, WM_CLOSE, 0, 0);
				}

				System.bUnobserve           = 0;
				System.nUnobGameNumber      = 0;
				strcpy(System.cUnobWname, "");
				strcpy(System.cUnobBname, "");

				if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
						(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
				{
					Game [INDEX_PLAY].nGamePartner = 0;
					Game [nI].nGamePartner = 0;

					STATE_ObserveEndGame(nI, 1, 1);
				}
				else
				{
					Game [nI].nGamePartner = 0;

					STATE_ObserveEndGame(nI, 0, 1);
				}
				break;
			}
		}
		return;
	}

	if(! strncmp(cS, ICC_STOP_OBS_GAME1, 14))
	{
		TELNET_NormalPrint(cS);

		sscanf(cS + 14, "%d", &nG);

		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			if(Game [nI].nGameNumber == nG)
			{
				Game [nI].bFirstGame  = 0;
				Game [nI].nGameNumber = 0;
				Game [nI].bPlaying    = 0;
				Game [nI].bTickClock  = 1;

				if(IsWindow(Game [nI].hwnd))
				{
					SendMessage(Game [nI].hwnd, WM_CLOSE, 0, 0);
				}

				System.bUnobserve           = 0;
				System.nUnobGameNumber      = 0;
				strcpy(System.cUnobWname, "");
				strcpy(System.cUnobBname, "");

				if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
						(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
				{
					Game [INDEX_PLAY].nGamePartner = 0;
					Game [nI].nGamePartner = 0;

					STATE_ObserveEndGame(nI, 1, 1);
				}
				else
				{
					Game [nI].nGamePartner = 0;

					STATE_ObserveEndGame(nI, 0, 1);
				}
				break;
			}
		}
		return;
	}

	if(! strncmp(cS, ICC_STOP_OBS_GAME2, 5))
	{
		if(strstr(cS, ICC_STOP_OBS_GAME21))
		{
			TELNET_NormalPrint(cS);

			sscanf(cS + 5, "%d", &nG);

			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(Game [nI].nGameNumber == nG)
				{
					Game [nI].bFirstGame  = 0;
					Game [nI].nGameNumber = 0;
					Game [nI].bPlaying    = 0;
					Game [nI].bTickClock  = 1;

					if(IsWindow(Game [nI].hwnd))
					{
						SendMessage(Game [nI].hwnd, WM_CLOSE, 0, 0);
					}

					System.bUnobserve           = 0;
					System.nUnobGameNumber      = 0;
					strcpy(System.cUnobWname, "");
					strcpy(System.cUnobBname, "");

					if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
							(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
					{
						Game [INDEX_PLAY].nGamePartner = 0;
						Game [nI].nGamePartner = 0;

						STATE_ObserveEndGame(nI, 1, 1);
					}
					else
					{
						Game [nI].nGamePartner = 0;

						STATE_ObserveEndGame(nI, 0, 1);
					}
					break;
				}
			}
			return;
		}
	}

	if(! strncmp(cS, ICC_GAME_NUMBER_INIT, 6))
	{
		TELNET_NormalPrint(cS);

		sscanf(cS + 6, "%d", &nG);

		cP = strchr(cS, ')');
		cQ = strchr(cS, '}');

		nF = -1;
		for(nI = 0 ; nI < MAX_GAME ; nI++)
		{
			if(Game [nI].nGameNumber == nG)
			{
				nF       = nI;
				bPlaying = Game [nI].bPlaying;
				nPartner = Game [nI].nGamePartner;

				Game [nI].bFirstGame  = 0;
				Game [nI].nGameNumber = 0;
				Game [nI].bPlaying    = 0;
				Game [nI].bTickClock  = 1;

				strcpy(Game [nI].cOrgResult, cS);

				if(nI == INDEX_PLAY)
				{
					System.bIsMyTurn = 0;

					hdc = GetDC(Game [nI].hwnd);
					BOARD_DrawBoard(nI, Game [nI].hwnd, hdc, DRAW_STATE_ERASE_ALL_TP);
					ReleaseDC(Game [nI].hwnd, hdc);

					Premove.nPremoveCount     = 0;
					Premove.nPremoveHead      = 0;
					Premove.nPremoveTail      = 0;
					Premove.bLastTP           = 0;
					Premove.ptLastTP [0].x    = -1;
					Premove.ptLastTP [0].y    = -1;
					Premove.ptLastTP [1].x    = -1;
					Premove.ptLastTP [1].y    = -1;
					Premove.bIllegalTP        = 0;
					Premove.ptIllegalTP [0].x = -1;
					Premove.ptIllegalTP [0].y = -1;
					Premove.ptIllegalTP [1].x = -1;
					Premove.ptIllegalTP [1].y = -1;

					Game [INDEX_PLAY].nGamePartner = 0;

					PostMessage(Game [nI].hwnd, WM_USER_MOUSELEAVE, 0, 0);
				}
				else
				{
					if(strlen(Vars.cPartner) > 0)
					{
						if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
								(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
						{
							Game [INDEX_PLAY].nGamePartner = 0;
							Game [nI].nGamePartner = 0;

							PostMessage(Game [nI].hwnd, WM_USER_MOUSELEAVE, 0, 0);
						}
						else
						{
							Game [nI].nGamePartner = 0;
						}
					}
					else
					{
						Game [nI].nGamePartner = 0;
					}
				}

				if(cP && cQ)
				{
					(void) CLOCK_StopClockTimer(nI);

					strncpy(Game [nI].cResult, cP + 1, cQ - cP + 1);

					cQ = strchr(Game [nI].cResult, '}');

					if(cQ != NULL)
					{
						*cQ = NULL_CHAR;
					}

					strcpy(Vars.cPartnerTell, "");

					TOOLBOX_AllTrim(Game [nI].cResult);

					hdc = GetDC(Game [nI].hwnd);
					BOARD_DrawResult(nI, hdc);
					ReleaseDC(Game [nI].hwnd, hdc);

					if(bPlaying)
					{
						if(nI == INDEX_PLAY)
						{
							if(! GAMESOUND_PlayBoard(cS))
							{
								GAMESOUND_Play(GAME_SOUND_PLAY_OVER);
							}

							for(nJ = 0 ; nJ < MAX_TIME_SOUND ; nJ++)
							{
								TimeSounds.bPlayed [nJ] = 0;
							}
							TimeSounds.nLastSecond = -500;

							Premove.nPremoveCount  = 0;
							Premove.nPremoveHead   = 0;
							Premove.nPremoveTail   = 0;
							Premove.bLastTP        = 0;
							Premove.ptLastTP [0].x = -1;
							Premove.ptLastTP [0].y = -1;
							Premove.ptLastTP [1].x = -1;
							Premove.ptLastTP [1].y = -1;
						}
						else
						{
							if(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
							{
								if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
										(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
								{
									if(! GAMESOUND_PartnerBoard(cS))
									{
										GAMESOUND_Play(GAME_SOUND_OBS_OVER1);
									}
								}
								else
								{
									GAMESOUND_Play(GAME_SOUND_OBS_OVER1);
								}
							}
							else
							{
								if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
										(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
								{
									if(! GAMESOUND_PartnerBoard(cS))
									{
										GAMESOUND_Play(GAME_SOUND_OBS_OVER2);
									}
								}
								else
								{
									GAMESOUND_Play(GAME_SOUND_OBS_OVER2);
								}
							}
						}
					}
				}

				if(nI == INDEX_PLAY)
				{
					bFound = 0;

					if(User.bICCPlayEndPartner)
					{
						if(bPlaying)
						{
							if(strlen(Vars.cPartner) > 0)
							{
								for(nI = 1 ; nI < MAX_GAME ; nI++)
								{
									if(Game [nI].bValid)
									{
										if(Game [nI].nGameNumber > 0)
										{
											if(Game [nI].nGameNumber == nPartner)
											{
												if(Game [nI].bPlaying)
												{
													if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
															(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
													{
														Game [nI].bFirstGame   = 0;
														Game [nI].bPlaying     = 0;
														Game [nI].nGameNumber  = 0;
														Game [nI].nGamePartner = 0;
														Game [nI].bTickClock   = 1;

														(void) CLOCK_StopClockTimer(nI);

														strcpy(Game [nI].cResult, Game [INDEX_PLAY].cResult);

														hdc = GetDC(Game [nI].hwnd);
														BOARD_DrawResult(nI, hdc);
														ReleaseDC(Game [nI].hwnd, hdc);

														bFound = nI;
														break;
													}
												}
											}
										}
									}
								}
							}
						}
					}

					STATE_PlayEndGame();

					if(bFound)
					{
						STATE_ObserveEndGame(bFound, 1, 0);
					}
				}
				else
				{
					if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
							(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
					{
						STATE_ObserveEndGame(nI, 1, 0);
					}
					else
					{
						STATE_ObserveEndGame(nI, 0, 0);
					}
				}
				break;
			}
		}

		if(nF != -1)
		{
			if(Game [nF].nGameType == GAMETYPE_BUGHOUSE)
			{
				if(strlen(Vars.cPartner) > 0)
				{
					if(nF == INDEX_PLAY)
					{
						for(nI = 1 ; nI < MAX_GAME ; nI++)
						{
							if(Game [nI].nGameNumber != 0)
							{
								if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
										(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
								{
									bPlaying = Game [nI].bPlaying;

									Game [nI].bFirstGame  = 0;
									Game [nI].nGameNumber = 0;
									Game [nI].bPlaying    = 0;
									Game [nI].bTickClock  = 1;

									PostMessage(Game [nI].hwnd, WM_USER_MOUSELEAVE, 0, 0);

									(void) CLOCK_StopClockTimer(nI);

									strcpy(Game [nI].cResult, Game [INDEX_PLAY].cResult);

									hdc = GetDC(Game [nI].hwnd);
									BOARD_DrawResult(nI, hdc);
									ReleaseDC(Game [nI].hwnd, hdc);

									if(bPlaying)
									{
										if(! GAMESOUND_PartnerBoard(cS))
										{
											GAMESOUND_Play(GAME_SOUND_OBS_OVER1);
										}
									}
									break;
								}
							}
						}
					}
					else
					{
						if((stricmp(Game [nF].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
								(stricmp(Game [nF].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
						{
							bPlaying = Game [INDEX_PLAY].bPlaying;

							Game [INDEX_PLAY].bFirstGame  = 0;
							Game [INDEX_PLAY].nGameNumber = 0;
							Game [INDEX_PLAY].bPlaying    = 0;
							Game [INDEX_PLAY].bTickClock  = 1;

							PostMessage(Game [INDEX_PLAY].hwnd, WM_USER_MOUSELEAVE, 0, 0);

							(void) CLOCK_StopClockTimer(INDEX_PLAY);

							strcpy(Game [INDEX_PLAY].cResult, Game [nF].cResult);

							hdc = GetDC(Game [INDEX_PLAY].hwnd);
							BOARD_DrawResult(INDEX_PLAY, hdc);
							ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);

							if(bPlaying)
							{
								if(! GAMESOUND_PlayBoard(cS))
								{
									GAMESOUND_Play(GAME_SOUND_PLAY_OVER);
								}

								for(nJ = 0 ; nJ < MAX_TIME_SOUND ; nJ++)
								{
									TimeSounds.bPlayed [nJ] = 0;
								}
								TimeSounds.nLastSecond = -500;

								Premove.nPremoveCount  = 0;
								Premove.nPremoveHead   = 0;
								Premove.nPremoveTail   = 0;
								Premove.bLastTP        = 0;
								Premove.ptLastTP [0].x = -1;
								Premove.ptLastTP [0].y = -1;
								Premove.ptLastTP [1].x = -1;
								Premove.ptLastTP [1].y = -1;
							}
						}
					}
				}
			}
		}
		return;
	}

	if(! strncmp(cS, ICC_STOP_EXA_GAME, 33))
	{
		sscanf(cS + 33, "%d", &nG);

		for(nI = 0 ; nI < MAX_GAME ; nI++)
		{
			if(Game [nI].nGameNumber == nG)
			{
				Game [nI].bFirstGame   = 0;
				Game [nI].nGameNumber  = 0;
				Game [nI].nGamePartner = 0;
				Game [nI].bPlaying     = 0;
				Game [nI].bTickClock   = 1;

				System.bUnobserve          = 0;
				System.nUnobGameNumber     = 0;
				strcpy(System.cUnobWname, "");
				strcpy(System.cUnobBname, "");

				if(nI == INDEX_PLAY)
				{
					(void) CLOCK_StopClockTimer(INDEX_PLAY);

					GAME_InitialSetup(INDEX_PLAY);

					TOOLBOX_InvalidateBoardWindow(0);

					if(! User.bShowMoveButton)
					{
						TOOLBOX_ShowMoveButtons(INDEX_PLAY, 0);
					}

					TOOLBOX_RedrawBoard(INDEX_PLAY);
					SetWindowText(Game [INDEX_PLAY].hwnd, TOOLBOX_GetGameWindowTitle(INDEX_PLAY));
				}
				break;
			}
		}
		TELNET_NormalPrint(cS);
		return;
	}

	if(! strncmp(cS, ICC_BUGHOUSE_START, 15))
	{
		//
		// Other board is guest23 (unreg) [white] guest73 (unreg) unrated wild(24) Bughouse 10 0
		//                (a)     (b)     (c)     (d)
		//
		// Other board is guest23 (unreg) [black] guest73 (unreg) unrated wild(24) Bughouse 10 0
		//                (a)     (b)     (c)     (d)
		//
		// Other board is guest380 (unreg) ZonJan (2016) unrated wild(24) Bughouse 10 0
		//                (a)     (b)      (c)    (d)
		//
		// Other board is ZonJan (2016) guest380 (unreg) unrated wild(24) Bughouse 10 0
		//                (a)    (b)    (c)      (d)
		//
		sscanf(cS + 15, "%s", &cWHandle);

		Icc.bFindICCPartner = 0;
		strcpy(Icc.cICCPartner, "");

		if(Game [INDEX_PLAY].bValid)
		{
			if(Game [INDEX_PLAY].nGameNumber > 0)
			{
				if(Game [INDEX_PLAY].bPlaying)
				{
					Icc.bFindICCPartner = 1;
					strcpy(Icc.cICCPartner, cWHandle);

					bPartner = 0;
					for(nI = 1 ; nI < MAX_GAME ; nI++)
					{
						if(Game [nI].bValid)
						{
							if(Game [nI].nGameNumber != 0)
							{
								if((stricmp(Game [nI].cHandle [INDEX_WHITE], cWHandle) == 0) ||
										(stricmp(Game [nI].cHandle [INDEX_BLACK], cWHandle) == 0))
								{
									bPartner = 1;
									break;
								}
							}
						}
					}
					if(! bPartner)
					{
						TOOLBOX_WriteICS(ICC_OBSERVE_COMMAND);
						TOOLBOX_WriteICS(cWHandle);
						TOOLBOX_WriteICS("\n");
					}
				}
			}
		}
		TELNET_NormalPrint(cS);
		return;
	}

	if(! strncmp(cS, ICC_ENTER_EXAM, 31))
	{
		ICC_ParseExamBoard(Game [INDEX_PLAY].cStyle12);
		TELNET_NormalPrint(cS);
		return;
	}

	if(cS [0] == '\\')
	{
		if(Telnet.bDisplayContinueLine)
		{
			Telnet.clrCurrent    = Telnet.clrLastLine;
			Telnet.nCurrentSound = SOUND_NONE;
			TELNET_Write(cS);
		}
		else
		{
			Telnet.bTelnetSkipLine = 1;
		}
		return;
	}
	Telnet.bDisplayContinueLine = 1;

//
// Following guest508.
//
	if(! strncmp(cS, ICC_HEARED_FOLLOW, 10))
	{
		sscanf(cS, "%s %s", &cTmp, &cWHandle);

		nI = strlen(cWHandle);
		if(cWHandle [nI - 1] == '.')
		{
			cWHandle [nI - 1] = NULL_CHAR;
		}
		cWHandle [ICS_HANDLE_LENGTH] = NULL_CHAR;

		// follow
		strcpy(Vars.cFollow,  cWHandle);
		strcpy(Vars.cPfollow, "");

		sprintf(Vars.cFollowGone, ICC_FOLLOW_GONE_STRING, cWHandle);
		Vars.nFollowGone = strlen(Vars.cFollowGone);

		sprintf(Vars.cFollowGone1, "");
		Vars.nFollowGone1 = 0;

		sprintf(Vars.cFollowOn, ICC_FOLLOW_ON_STRING, cWHandle);
		Vars.nFollowOn = strlen(Vars.cFollowOn);

		TELNET_NormalPrint(cS);
		TOOLBOX_SetTelnetCaption();
		return;
	}

//
// No longer following guest508.
//
	if(! strncmp(cS, ICC_HEARED_NO_FOLLOW, 20))
	{
		strcpy(Vars.cFollow,      "");
		strcpy(Vars.cPfollow,     "");
		strcpy(Vars.cFollowGone,  "");
		strcpy(Vars.cFollowGone1, "");
		strcpy(Vars.cFollowOn,    "");

		Vars.nFollowGone  = 0;
		Vars.nFollowGone1 = 0;
		Vars.nFollowOn    = 0;

		TELNET_NormalPrint(cS);
		TOOLBOX_SetTelnetCaption();
		return;
	}

//
// guest508, whom you are following, has disconnected.
//
	if(Vars.nFollowGone > 0)
	{
		if(! strncmp(cS, Vars.cFollowGone, Vars.nFollowGone))
		{
			strcpy(Vars.cFollow,      "");
			strcpy(Vars.cPfollow,     "");
			strcpy(Vars.cFollowGone,  "");
			strcpy(Vars.cFollowGone1, "");

			Vars.nFollowGone  = 0;
			Vars.nFollowGone1 = 0;

			TELNET_NormalPrint(cS);
			TOOLBOX_SetTelnetCaption();
			return;
		}
	}

//
// guest508, whom you are following, has connected.
//
	if(Vars.nFollowOn > 0)
	{
		if(! strncmp(cS, Vars.cFollowOn, Vars.nFollowOn))
		{
			sscanf(cS, "%s %s", &cTmp, &cWHandle);

			nI = strlen(cWHandle);
			if(cWHandle [nI - 1] == ',')
			{
				cWHandle [nI - 1] = NULL_CHAR;
			}
			cWHandle [ICS_HANDLE_LENGTH] = NULL_CHAR;

			// follow
			strcpy(Vars.cFollow,  cWHandle);
			strcpy(Vars.cPfollow, "");

			sprintf(Vars.cFollowGone, ICC_FOLLOW_GONE_STRING, cWHandle);
			Vars.nFollowGone = strlen(Vars.cFollowGone);

			sprintf(Vars.cFollowGone1, "");
			Vars.nFollowGone1 = 0;

			sprintf(Vars.cFollowOn, ICC_FOLLOW_ON_STRING, cWHandle);
			Vars.nFollowOn = strlen(Vars.cFollowOn);

			TELNET_NormalPrint(cS);
			TOOLBOX_SetTelnetCaption();
			return;
		}
	}

//
// Game 38: WhoAmI goes forward 1 move.
//
	if(User.bShowResult || User.bShowPtell)
	{
		if(! strncmp(cS, ICC_GAME_INFORMATION, 5))
		{
			if(strncmp(cS, ICC_HEARED_GAME_NOTIFY, 19))
			{
				sscanf(cS + 5, "%d", &nG);
				for(nI = 0 ; nI < MAX_GAME ; nI++)
				{
					if(Game [nI].nGameNumber == nG)
					{
						hdc = GetDC(Game [nI].hwnd);
						BOARD_DrawGinfo(nI, hdc, cS);
						ReleaseDC(Game [nI].hwnd, hdc);
						Telnet.bTelnetSkipLine = 1;
						return;
					}
				}
			}
		}
	}

//
// Promotion type set to Queen.
// Promotion type set to Knight.
// Promotion type set to Bishop.
// Promotion type set to Rook.
//
	if(! strncmp(cS, ICC_PROMOTE_PIECE, 22))
	{
		TELNET_NormalPrint(cS);

		System.bPromoteCommand = 1;
		System.bPromoteKnight  = 0;

		sscanf(cS + 22, "%s", &cTmp);

		if(! strncmp(cTmp, "Queen.", 6))
		{
			System.nPromoteWPiece = WHITE_QUEEN;
			System.nPromoteBPiece = BLACK_QUEEN;
		}
		else if(! strncmp(cTmp, "Knight.", 7))
		{
			System.nPromoteWPiece = WHITE_KNIGHT;
			System.nPromoteBPiece = BLACK_KNIGHT;
		}
		else if(! strncmp(cTmp, "Bishop.", 7))
		{
			System.nPromoteWPiece = WHITE_BISHOP;
			System.nPromoteBPiece = BLACK_BISHOP;
		}
		else if(! strncmp(cTmp, "Rook.", 5))
		{
			System.nPromoteWPiece = WHITE_ROOK;
			System.nPromoteBPiece = BLACK_ROOK;
		}
		else
		{
			System.nPromoteWPiece = WHITE_QUEEN;
			System.nPromoteBPiece = BLACK_QUEEN;
		}
		return;
	}

	if(System.nIssuedMovesCommand > 0)
	{
		if(strlen(cS) > 20)
		{
			//
			//guest564 vs. guest601 --- 2002.02.08 02:14:42
			//Unrated wild(23) match, initial time: 2 minutes, increment: 5 seconds
			//
			//guest24 vs. Battletorn (1458) --- 2002.02.08 02:16:27
			//Unrated wild(27) match, initial time: 2 minutes, increment: 12 seconds
			//
			//ghost (2692) vs. guest393 --- 2002.02.08 02:13:45
			//Unrated bullet match, initial time: 2 minutes, increment: 1 second
			//
			//taba (2086) vs. Faith (2169) --- 2002.02.08 02:16:43
			//Rated wild(23) match, initial time: 1 minute, increment: 0 seconds
			//
			//Unrated bullet match, initial time: 1 minute, increment: 0 seconds
			//Unrated blitz match, initial time: 5 minutes, increment: 0 seconds
			//Unrated standard match, initial time: 60 minutes, increment: 0 seconds
			//Unrated wild(1) match, initial time: 5 minutes, increment: 0 seconds
			//Unrated wild(23) match, initial time: 2 minutes, increment: 5 seconds
			//Unrated wild(27) match, initial time: 2 minutes, increment: 12 seconds
			//

			sscanf(cS, "%s %s %s %s %s", &cWHandle, &cWRating, &cTmp, &cBHandle, &cBRating);

			if(stricmp(cWRating, "vs.") == 0)
			{
				System.nMoveListIndexNumber = -1;

				for(nI = 0 ; nI < MAX_GAME ; nI++)
				{
					//guest564 vs. guest601 --- 2002.02.08 02:14:42
					if((stricmp(Game [nI].cHandle [INDEX_WHITE], cWHandle) == 0) &&
							(stricmp(Game [nI].cHandle [INDEX_BLACK], cTmp) == 0))
					{
						System.nMoveListIndexNumber = nI;

						Game [nI].bFirstGame = 0;

						if(stricmp(cBHandle, "---") == 0)
						{
							strcpy(Game [nI].cRating [INDEX_BLACK], "");
						}
						else
						{
							strcpy(Game [nI].cRating [INDEX_BLACK], cBHandle);
						}

						Telnet.bTelnetSkipLine = 1;
						//TELNET_NormalPrint (cS);
						return;
					}
				}
			}

			if(stricmp(cTmp, "vs.") == 0)
			{
				System.nMoveListIndexNumber = -1;

				for(nI = 0 ; nI < MAX_GAME ; nI++)
				{
					if((stricmp(Game [nI].cHandle [INDEX_WHITE], cWHandle) == 0) &&
							(stricmp(Game [nI].cHandle [INDEX_BLACK], cBHandle) == 0))
					{
						System.nMoveListIndexNumber = nI;

						Game [nI].bFirstGame = 0;

						strcpy(Game [nI].cRating [INDEX_WHITE], cWRating);

						if(stricmp(cBRating, "---") == 0)
						{
							strcpy(Game [nI].cRating [INDEX_BLACK], "");
						}
						else
						{
							strcpy(Game [nI].cRating [INDEX_BLACK], cBRating);
						}

						Telnet.bTelnetSkipLine = 1;
						//TELNET_NormalPrint (cS);
						return;
					}
				}
			}

			if(System.nMoveListIndexNumber >= 0)
			{
				nI = System.nMoveListIndexNumber;

				Game [nI].bFirstGame = 0;

				if(stricmp(cWHandle, "Rated") == 0)
				{
					Game [nI].nRated = 1;
					bFound           = 1;
				}
				else if(stricmp(cWHandle, "Unrated") == 0)
				{
					Game [nI].nRated = 0;
					bFound           = 1;
				}
				else
				{
					bFound = 0;
				}

				if(bFound)
				{
					strcpy(Game [nI].cOrgGameType, cWRating);

					if(nI == INDEX_PLAY)
					{
						System.bICCLastGameWasWild16 = 0;
					}

					if(stricmp(cWRating, "bullet") == 0)
					{
						strcpy(Game [nI].cGameType, cWRating);
						Game [nI].nGameType  = GAMETYPE_ICC_BULLET;
						Game [nI].bChessGame = 1;
						BOARD_ResetPromoteBoard(nI);
					}
					else if(stricmp(cWRating, "blitz") == 0)
					{
						strcpy(Game [nI].cGameType, cWRating);
						Game [nI].nGameType  = GAMETYPE_ICC_BLITZ;
						Game [nI].bChessGame = 1;
						BOARD_ResetPromoteBoard(nI);
					}
					else if(stricmp(cWRating, "standard") == 0)
					{
						strcpy(Game [nI].cGameType, cWRating);
						Game [nI].nGameType  = GAMETYPE_ICC_STANDARD;
						Game [nI].bChessGame = 1;
						BOARD_ResetPromoteBoard(nI);
					}
					else if(stricmp(cWRating, "5-minute") == 0)
					{
						strcpy(Game [nI].cGameType, cWRating);
						Game [nI].nGameType  = GAMETYPE_ICC_5_MINUTE;
						Game [nI].bChessGame = 1;
						BOARD_ResetPromoteBoard(nI);
					}
					else if(stricmp(cWRating, "1-minute") == 0)
					{
						strcpy(Game [nI].cGameType, cWRating);
						Game [nI].nGameType  = GAMETYPE_ICC_1_MINUTE;
						Game [nI].bChessGame = 1;
						BOARD_ResetPromoteBoard(nI);
					}
					else
					{
						//
						//wild(1)
						//wild(23)
						//
						if(cWRating [4] == '(')
						{

							if(cWRating [6] == ')')
							{
								Game [nI].nGameType  = ((int)(cWRating [5] - '0'));
								Game [nI].bChessGame = 0;
								sprintf(Game [nI].cGameType, "w%d", Game [nI].nGameType);
							}
							else if(cWRating [7] == ')')
							{
								if(cWRating [5] == '1')
								{
									Game [nI].nGameType  = 10 + ((int)(cWRating [6] - '0'));
									Game [nI].bChessGame = 0;
									sprintf(Game [nI].cGameType, "w%d", Game [nI].nGameType);

									if(nI == INDEX_PLAY)
									{
										if((Game [nI].nGameNumber > 0) && (Game [nI].bPlaying))
										{
											if(Game [nI].nGameType == GAMETYPE_ICC_WILD16)
											{
												System.bICCLastGameWasWild16 = Game [nI].nGameNumber;
											}
										}
									}
								}
								else if(cWRating [5] == '2')
								{
									Game [nI].nGameType  = 20 + ((int)(cWRating [6] - '0'));
									Game [nI].bChessGame = 0;
									sprintf(Game [nI].cGameType, "w%d", Game [nI].nGameType);
								}
								else
								{
									strcpy(Game [nI].cGameType, cWRating);
									Game [nI].nGameType  = GAMETYPE_CHESS;
									Game [nI].bChessGame = 1;
									BOARD_ResetPromoteBoard(nI);
								}
							}
							else
							{
								strcpy(Game [nI].cGameType, cWRating);
								Game [nI].nGameType  = GAMETYPE_CHESS;
								Game [nI].bChessGame = 1;
								BOARD_ResetPromoteBoard(nI);
							}
						}
						else
						{
							strcpy(Game [nI].cGameType, cWRating);
							Game [nI].nGameType  = GAMETYPE_CHESS;
							Game [nI].bChessGame = 1;
							BOARD_ResetPromoteBoard(nI);
						}
					}

					hdc = GetDC(Game [nI].hwnd);
					BOARD_DrawHandle(nI, hdc);
					BOARD_DrawGameType(nI, hdc);
					BOARD_DrawLastMove(nI, hdc);
					BOARD_DrawWhiteClock(nI, hdc);
					BOARD_DrawBlackClock(nI, hdc);
					ReleaseDC(Game [nI].hwnd, hdc);

					if(User.bShowGameTypeOnTitleBar)
					{
						SetWindowText(Game [nI].hwnd, TOOLBOX_GetGameWindowTitle(nI));
					}
					else
					{
						if(User.bShowValueOnTitleBar)
						{
							SetWindowText(Game [nI].hwnd, TOOLBOX_GetGameWindowTitle(nI));
						}
					}

					TOOLBOX_AdjustShowMoveButtons(nI);

					if(nI == INDEX_PLAY)
					{
						if(Game [nI].bPlaying)
						{
							if(Game [nI].nGameType == GAMETYPE_WILD5)
							{
								RECT rc;

								switch(User.nPlayWild5Flip)
								{
									case DEFAULT_WILD5_PLAY_TOP :
										if(Game [nI].bIPlayWhite)
										{
											Game [nI].bFlip = 0;
										}
										else
										{
											Game [nI].bFlip = 1;
										}
										break;

									case DEFAULT_WILD5_PLAY_BOTTOM :
										if(Game [nI].bIPlayWhite)
										{
											Game [nI].bFlip = 1;
										}
										else
										{
											Game [nI].bFlip = 0;
										}
										break;

									case DEFAULT_WILD5_PLAY_BBOTTOM :
										Game [nI].bFlip = 0;
										break;

									case DEFAULT_WILD5_PLAY_WBOTTOM :
										Game [nI].bFlip = 1;
										break;

									default :
										if(Game [nI].bIPlayWhite)
										{
											Game [nI].bFlip = 0;
										}
										else
										{
											Game [nI].bFlip = 1;
										}
										break;
								}

								GetClientRect(Game [nI].hwnd, &rc);

								hdc = GetDC(Game [nI].hwnd);

								switch(User.nBufferOrientation)
								{
									case DEFAULT_BUFFER_LEFT :
										BOARD_ResizeLeft(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									case DEFAULT_BUFFER_RIGHT :
										BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									case DEFAULT_BUFFER_TOPBOTTOML :
										BOARD_ResizeTopBottomL(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									case DEFAULT_BUFFER_TOPBOTTOMR :
										BOARD_ResizeTopBottomR(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									default :
										BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;
								}

								BOARD_LoadBitmaps(nI, hdc);
								BOARD_CheckFlip(nI);
								BOARD_DrawRepaint(nI, Game [nI].hwnd, hdc);

								ReleaseDC(Game [nI].hwnd, hdc);
							}
							else
							{
								RECT rc;

								GetClientRect(Game [nI].hwnd, &rc);

								hdc = GetDC(Game [nI].hwnd);

								switch(User.nBufferOrientation)
								{
									case DEFAULT_BUFFER_LEFT :
										BOARD_ResizeLeft(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									case DEFAULT_BUFFER_RIGHT :
										BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									case DEFAULT_BUFFER_TOPBOTTOML :
										BOARD_ResizeTopBottomL(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									case DEFAULT_BUFFER_TOPBOTTOMR :
										BOARD_ResizeTopBottomR(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;

									default :
										BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
										break;
								}

								BOARD_LoadBitmaps(nI, hdc);
								BOARD_CheckFlip(nI);
								BOARD_DrawRepaint(nI, Game [nI].hwnd, hdc);

								ReleaseDC(Game [nI].hwnd, hdc);
							}

							if(Game [nI].nGameType == GAMETYPE_BUGHOUSE)
							{
								GAMEEVENT_Command(GAME_EVENT_PLAY_BUG_START);
							}
							else
							{
								GAMEEVENT_Command(GAME_EVENT_PLAY_ONE_START);
							}

							if(Game [nI].bIPlayWhite)
							{
								MATCH_AdjustMatch(nI,
												  Game [nI].cHandle [INDEX_BLACK],
												  Game [nI].nInitialClock,
												  Game [nI].nIncrementClock,
												  Game [nI].nRated);
							}
							else
							{
								MATCH_AdjustMatch(nI,
												  Game [nI].cHandle [INDEX_WHITE],
												  Game [nI].nInitialClock,
												  Game [nI].nIncrementClock,
												  Game [nI].nRated);
							}

							System.nLastPlayGameType = Game [nI].nGameType;
						}
						else
						{
							RECT rc;

							GetClientRect(Game [nI].hwnd, &rc);

							hdc = GetDC(Game [nI].hwnd);

							switch(User.nBufferOrientation)
							{
								case DEFAULT_BUFFER_LEFT :
									BOARD_ResizeLeft(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
									break;

								case DEFAULT_BUFFER_RIGHT :
									BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
									break;

								case DEFAULT_BUFFER_TOPBOTTOML :
									BOARD_ResizeTopBottomL(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
									break;

								case DEFAULT_BUFFER_TOPBOTTOMR :
									BOARD_ResizeTopBottomR(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
									break;

								default :
									BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
									break;
							}

							BOARD_LoadBitmaps(nI, hdc);
							BOARD_CheckFlip(nI);
							BOARD_DrawRepaint(nI, Game [nI].hwnd, hdc);

							ReleaseDC(Game [nI].hwnd, hdc);

							GAMEEVENT_Command(GAME_EVENT_PLAY_ONE_START);

							System.nLastExamGameType = Game [nI].nGameType;
						}

						Game [nI].bIssuedEventCommand = 1;
					}
					else
					{
						RECT rc;

						GetClientRect(Game [nI].hwnd, &rc);

						hdc = GetDC(Game [nI].hwnd);

						switch(User.nBufferOrientation)
						{
							case DEFAULT_BUFFER_LEFT :
								BOARD_ResizeLeft(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
								break;

							case DEFAULT_BUFFER_RIGHT :
								BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
								break;

							case DEFAULT_BUFFER_TOPBOTTOML :
								BOARD_ResizeTopBottomL(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
								break;

							case DEFAULT_BUFFER_TOPBOTTOMR :
								BOARD_ResizeTopBottomR(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
								break;

							default :
								BOARD_ResizeRight(nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1);
								break;
						}

						BOARD_LoadBitmaps(nI, hdc);
						BOARD_CheckFlip(nI);
						BOARD_DrawRepaint(nI, Game [nI].hwnd, hdc);

						ReleaseDC(Game [nI].hwnd, hdc);

						if((Game [nI].nGameType == GAMETYPE_BUGHOUSE) && (Game [nI].nRelation != -2))
						{
							if((stricmp(Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) != 0) &&
									(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) != 0))
							{
								nG = Game [nI].nGameNumber;

								if(Icc.nSecondBugGame == nG)
								{
									Icc.bIssuedBugCommand = 0;
									Icc.nIssuedBugCommand = 0;
									Icc.bStartBugGameList = 0;
									Icc.nFirstBugGame     = 0;
									Icc.nSecondBugGame    = 0;
									Icc.bSecondBugFlip    = 0;
								}
								else
								{
									Icc.bIssuedBugCommand = 1;
									Icc.nIssuedBugCommand = nG;
									Icc.bStartBugGameList = 0;
									Icc.nFirstBugGame     = nG;
									Icc.nSecondBugGame    = 0;
									Icc.bSecondBugFlip    = (! Game [nI].bFlip);

									TOOLBOX_WriteICS(ICC_BUGHOUSE_COMMAND);

									GAMEEVENT_Command(GAME_EVENT_OBSERVE_BUG_ST);

									Game [nI].bIssuedEventCommand = 1;
								}
							}
						}
						else
						{
							GAMEEVENT_Command(GAME_EVENT_OBSERVE_ONE_ST);

							Game [nI].bIssuedEventCommand = 1;

							nFlip = 0;

							if(strlen(Vars.cFollow) > 0)
							{
								if(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
								{
									if(! Game [nI].bFlip)
									{
										nFlip = 1;
									}
								}
							}

							if(! nFlip)
							{
								if(strlen(Vars.cObserve) > 0)
								{
									if(TOOLBOX_MatchHandle(Game [nI].cHandle [INDEX_WHITE], Vars.cObserve))
									{
										strcpy(Vars.cObserve, "");

										if(Game [nI].bFlip)
										{
											nFlip = 1;
										}
									}
									else if(TOOLBOX_MatchHandle(Game [nI].cHandle [INDEX_BLACK], Vars.cObserve))
									{
										strcpy(Vars.cObserve, "");

										if(! Game [nI].bFlip)
										{
											nFlip = 1;
										}
									}
								}
							}

							if(nFlip)
							{
								PostMessage(hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0);
							}
						}

						System.nLastObserveGameType = Game [nI].nGameType;
					}

					Telnet.bTelnetSkipLine = 1;
					//TELNET_NormalPrint (cS);
					return;
				}
			}
		}

		if(System.nMoveListIndexNumber >= 0)
		{
			nI = System.nMoveListIndexNumber;

			if(! strncmp(cS, ICC_MOVELIST_END, 8))
			{
				strcpy(Game [nI].cOrgResult, cS);
				TOOLBOX_AllTrim(Game [nI].cOrgResult);

				if(Game [nI].bChessGame)
				{
					if(Game [nI].nMaxIndex >= 0)
					{
						BOARD_CopyPromoteBoard(nI);
					}

					BOARD_FindCapturedPieces(nI);
					BUGHOUSE_EndMoveList(nI);

					System.nIssuedMovesCommand  = System.nIssuedMovesCommand - 1;
					System.nMoveListIndexNumber = -1;
					Telnet.bTelnetSkipLine      = 1;

					if((nI == INDEX_PLAY) || (TOOLBOX_DisplayActualBoard(nI)))
					{
						hdc = GetDC(Game [nI].hwnd);
						if(nI == INDEX_PLAY)
						{
							BOARD_DrawBoard(nI, Game [nI].hwnd, hdc, DRAW_STATE_BUFFER);
						}
						else
						{
							BOARD_DrawBoard1(nI, Game [nI].hwnd, hdc, DRAW_STATE_BUFFER);
						}
						ReleaseDC(Game [nI].hwnd, hdc);
					}
				}
				else
				{
					BUGHOUSE_EndMoveList(nI);

					System.nIssuedMovesCommand  = System.nIssuedMovesCommand - 1;
					System.nMoveListIndexNumber = -1;
					Telnet.bTelnetSkipLine      = 1;
				}
				return;
			}

			if(! strncmp(cS, ICC_MOVELIST_NO_GAME, 22))
			{
				System.nIssuedMovesCommand  = System.nIssuedMovesCommand - 1;
				System.nMoveListIndexNumber = -1;
				Telnet.bTelnetSkipLine      = 1;
				//TELNET_NormalPrint (cS);
				return;
			}

			if(! strncmp(cS, ICC_MOVELIST_BEGIN, 6))
			{
				Telnet.bTelnetSkipLine = 1;
				return;
			}

			if(! strncmp(cS, ICC_MOVELIST_BEGIN1, 6))
			{
				Telnet.bTelnetSkipLine = 1;
				return;
			}

			if(cS [3] == '.')
			{
				//
				//  1.  Nf3      (0:02)    f6       (0:03)
				//  2.  e3       (0:09)    d5       (0:06)
				//  3.  d4       (0:05)    c6       (0:15)
				//  4.  c4       (0:02)    e5       (0:08)
				//  5.  cxd5     (0:06)    Bb4      (0:10)
				//  6.  Nc3      (0:03)
				//
				strcpy(cNumber,  "");
				strcpy(cMove1,   "");
				strcpy(cWRating, "");
				strcpy(cMove2,   "");
				strcpy(cBRating, "");
				sscanf(cS, "%s %s %s %s %s", &cNumber, &cMove1, &cWRating, &cMove2, &cBRating);

				cNumber [3] = NULL_CHAR;
				sscanf(cNumber, "%d", &nJ);
				nJ = nJ - 1;

				if(nJ < MAX_POSITION)
				{
					if(strlen(cMove1) > 0)
					{
						POSITION_AddMoveList(System.nMoveListIndexNumber, nJ, 0, cMove1, cWRating);
					}

					if(strlen(cMove2) > 0)
					{
						POSITION_AddMoveList(System.nMoveListIndexNumber, nJ, 1, cMove2, cBRating);
					}
				}

				Telnet.bTelnetSkipLine = 1;
				return;
			}
		}
	}

	if(Icc.bIssuedBugCommand)
	{
		if(! strncmp(cS, ICC_BUGHOUSE_LIST, 23))
		{

			//
			// Current bughouse games:
			//
			Icc.bStartBugGameList = 1;
		}
		else if(Icc.bStartBugGameList)
		{

			//
			// Current bughouse games:
			//    game 334 (Gnejs vs NimaTheGreat) with game 377 (TheJerk vs ZonJan)
			//    game 132 (Pequod vs AlexTheFat)  with game 195 (st4 vs guest883)
			//
			// Current bughouse games:
			//    game 167 (NimaTheGreat vs Gnejs) with game 271 (ZonJan vs TheJerk)
			//    game 132 (Pequod vs AlexTheFat)  with game 195 (st4 vs guest883)
			//
			// Current bughouse games:
			//    game 363 (NimaTheGreat vs Gnejs) with game 544 (ZonJan vs TheJerk)
			//    game 132 (Pequod vs AlexTheFat)  with game 195 (st4 vs guest883)
			//    a    b   c        d e            f    g    h
			//

			sscanf(cS, "%s %d %s %s %s %s %s %d", &cWHandle, &nF, &cWRating, &cWRating, &cWRating, &cWRating, &cBHandle, &nG);

			if((stricmp(cWHandle, "game") == 0) && (stricmp(cBHandle, "game") == 0))
			{
				if(((nF > 0) && (nF < 999)) && ((nG > 0) && (nG < 999)))
				{
					if(nF == Icc.nIssuedBugCommand)
					{
						Icc.bIssuedBugCommand = 0;
						Icc.nIssuedBugCommand = 0;
						Icc.bStartBugGameList = 0;

						bPartner = 0;
						for(nI = 0 ; nI < MAX_GAME ; nI++)
						{
							if(Game [nI].bValid)
							{
								if(Game [nI].nGameNumber == nG)
								{
									bPartner = 1;
									break;
								}
							}
						}

						if(bPartner)
						{
							Icc.nFirstBugGame  = 0;
							Icc.nSecondBugGame = 0;
							Icc.bSecondBugFlip = 0;
						}
						else
						{
							Icc.nFirstBugGame  = nF;
							Icc.nSecondBugGame = nG;

							TOOLBOX_WriteICS(ICC_OBSERVE_COMMAND);

							sprintf(cTmp, "%d", nG);
							TOOLBOX_WriteICS(cTmp);
							TOOLBOX_WriteICS("\n");
						}
					}
					else if(nG == Icc.nIssuedBugCommand)
					{
						Icc.bIssuedBugCommand = 0;
						Icc.nIssuedBugCommand = 0;
						Icc.bStartBugGameList = 0;

						bPartner = 0;
						for(nI = 0 ; nI < MAX_GAME ; nI++)
						{
							if(Game [nI].bValid)
							{
								if(Game [nI].nGameNumber == nF)
								{
									bPartner = 1;
									break;
								}
							}
						}

						if(bPartner)
						{
							Icc.nFirstBugGame  = 0;
							Icc.nSecondBugGame = 0;
							Icc.bSecondBugFlip = 0;
						}
						else
						{
							Icc.nFirstBugGame  = nG;
							Icc.nSecondBugGame = nF;

							TOOLBOX_WriteICS(ICC_OBSERVE_COMMAND);

							sprintf(cTmp, "%d", nF);
							TOOLBOX_WriteICS(cTmp);
							TOOLBOX_WriteICS("\n");
						}
					}
				}
				else
				{
					Icc.bIssuedBugCommand = 0;
					Icc.nIssuedBugCommand = 0;
					Icc.bStartBugGameList = 0;
					Icc.nFirstBugGame     = 0;
					Icc.nSecondBugGame    = 0;
					Icc.bSecondBugFlip    = 0;
				}
			}
			else
			{
				Icc.bIssuedBugCommand = 0;
				Icc.nIssuedBugCommand = 0;
				Icc.bStartBugGameList = 0;
				Icc.nFirstBugGame     = 0;
				Icc.nSecondBugGame    = 0;
				Icc.bSecondBugFlip    = 0;
			}
		}
	}

	if(ICC_ColorIndex(cS))
	{
		TELNET_Write(cS);

		if(! EAR_Play(cS))
		{
			SOUND_Play();
		}
		Telnet.clrLastLine = Telnet.clrCurrent;
	}
	else
	{
		if(Telnet.bDisplayLine)
		{
			TELNET_Write(cS);
			SOUND_Play();
			Telnet.clrLastLine = Telnet.clrCurrent;
		}
	}
}

int ICC_ParseBoard(char *cS)
{
	HDC hdc;
	MDICREATESTRUCT mdicreate;
	char cGarbage [100], cTurn, cwName [20], cbName [20], cVerboseMove [10], cTime4Move [10], cLastMove [10], *cP;
	int nN, nLP, bWCK, bWCQ, bBCK, bBCQ, nHalf, nRl, nIc, nIi, nMn, nX, nY;
	int bOrgFlip, bNewGame, bPartner, nI, nJ, nK, nFlip;
	int nA, nB, nC, nD, nOA;
	long nWc, nBc;
	long nTmpW, nTmpB, nLagThisMove [2], nTimeRemainder [2];

	sscanf(cS + 77,
		   "%c %d %d %d %d %d %d %d %s %s %d %d %d %s %s %d %d %d %s %s %s",
		   &cTurn,
		   &nLP, &bWCK, &bWCQ, &bBCK, &bBCQ,
		   &nHalf, &nN,
		   &cwName, &cbName,
		   &nRl, &nIc, &nIi,
		   &cGarbage, &cGarbage,
		   &nWc, &nBc, &nMn,
		   &cVerboseMove,
		   &cTime4Move,
		   &cLastMove);

	// playing game
	if(nRl == -1 ||     // -1 i am playing, it is my opponent's move
			nRl ==  1)      //  1 i am playing and it is my move
	{
		strcpy(Game [INDEX_PLAY].cStyle12, cS);

		System.bIsMyTurn = (nRl == 1);

		bOrgFlip = Game [INDEX_PLAY].bFlip;

		if(Game [INDEX_PLAY].nGameNumber == nN)
		{
			bNewGame = 0;
		}
		else
		{
			bNewGame = 1;

			if(System.bICCLastGameWasWild16 > 0)
			{
				if(System.bICCLastGameWasWild16 == nN)
				{
					BOARD_LoadBoard(INDEX_PLAY, cS + 5, cVerboseMove);

					hdc = GetDC(Game [INDEX_PLAY].hwnd);
					BOARD_DrawRepaint(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc);
					ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);

					System.bICCLastGameWasWild16 = 0;
					return 1;
				}

				System.bICCLastGameWasWild16 = 0;
			}

			Game [INDEX_PLAY].nGameType = System.nLastPlayGameType;
		}

		if(Game [INDEX_PLAY].nGameType == GAMETYPE_WILD5)
		{
			switch(User.nPlayWild5Flip)
			{
				case DEFAULT_WILD5_PLAY_TOP :
					if(stricmp(cwName, Vars.cWhoAmI) == 0)
					{
						Game [INDEX_PLAY].bIPlayWhite = 1;
						Game [INDEX_PLAY].bFlip       = 0;
					}
					else
					{
						Game [INDEX_PLAY].bIPlayWhite = 0;
						Game [INDEX_PLAY].bFlip       = 1;
					}
					break;

				case DEFAULT_WILD5_PLAY_BOTTOM :
					if(stricmp(cwName, Vars.cWhoAmI) == 0)
					{
						Game [INDEX_PLAY].bIPlayWhite = 1;
						Game [INDEX_PLAY].bFlip       = 1;
					}
					else
					{
						Game [INDEX_PLAY].bIPlayWhite = 0;
						Game [INDEX_PLAY].bFlip       = 0;
					}
					break;

				case DEFAULT_WILD5_PLAY_BBOTTOM :
					if(stricmp(cwName, Vars.cWhoAmI) == 0)
					{
						Game [INDEX_PLAY].bIPlayWhite = 1;
					}
					else
					{
						Game [INDEX_PLAY].bIPlayWhite = 0;
					}
					Game [INDEX_PLAY].bFlip = 0;
					break;

				case DEFAULT_WILD5_PLAY_WBOTTOM :
					if(stricmp(cwName, Vars.cWhoAmI) == 0)
					{
						Game [INDEX_PLAY].bIPlayWhite = 1;
					}
					else
					{
						Game [INDEX_PLAY].bIPlayWhite = 0;
					}
					Game [INDEX_PLAY].bFlip = 1;
					break;

				default :
					if(stricmp(cwName, Vars.cWhoAmI) == 0)
					{
						Game [INDEX_PLAY].bIPlayWhite = 1;
						Game [INDEX_PLAY].bFlip       = 0;
					}
					else
					{
						Game [INDEX_PLAY].bIPlayWhite = 0;
						Game [INDEX_PLAY].bFlip       = 1;
					}
					break;
			}
		}
		else
		{
			if(stricmp(cwName, Vars.cWhoAmI) == 0)
			{
				Game [INDEX_PLAY].bIPlayWhite = 1;
				Game [INDEX_PLAY].bFlip       = 0;
			}
			else
			{
				Game [INDEX_PLAY].bIPlayWhite = 0;
				Game [INDEX_PLAY].bFlip       = 1;
			}
		}

		Game [INDEX_PLAY].bValid          = 1;
		Game [INDEX_PLAY].nGameNumber     = nN;
		Game [INDEX_PLAY].nRelation       = nRl;
		Game [INDEX_PLAY].bWhitesMove     = (cTurn == 'W');
		Game [INDEX_PLAY].bLastWhitesMove = Game [INDEX_PLAY].bWhitesMove;
		Game [INDEX_PLAY].bPlaying        = 1;
		Game [INDEX_PLAY].bInitialMove    = (stricmp(cLastMove, ICS_INITIAL_MOVE_NAME) == 0);
		Game [INDEX_PLAY].nMoveNumber     = nMn;

		strcpy(Game [INDEX_PLAY].cHandle [INDEX_WHITE], cwName);
		strcpy(Game [INDEX_PLAY].cHandle [INDEX_BLACK], cbName);
		strcpy(Game [INDEX_PLAY].cResult,               "");

		Game [INDEX_PLAY].nLastDoublePushFile               = nLP;
		Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_WHITE] = bWCK;
		Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_WHITE] = bWCQ;
		Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_BLACK] = bBCK;
		Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_BLACK] = bBCQ;
		Game [INDEX_PLAY].nHalfMoves                        = nHalf;

		Game [INDEX_PLAY].nInitialClock   = nIc;
		Game [INDEX_PLAY].nIncrementClock = nIi;
		Game [INDEX_PLAY].bOnLagClock     = 0;

		if(bNewGame)
		{
			Game [INDEX_PLAY].bChessGame = 0;

			BOARD_LoadBoard(INDEX_PLAY, cS + 5, cVerboseMove);
		}
		else
		{
			if(User.bAnimatePlay)
			{
				if(! System.bIsMyTurn)
				{
					BOARD_LoadBoard(INDEX_PLAY, cS + 5, cVerboseMove);
				}
			}
			else
			{
				BOARD_LoadBoard(INDEX_PLAY, cS + 5, cVerboseMove);
			}
		}

		BOARD_GetHighlightFromLastMove(INDEX_PLAY, cVerboseMove, cLastMove);

		hdc = GetDC(Game [INDEX_PLAY].hwnd);

		if(bOrgFlip != Game [INDEX_PLAY].bFlip)
		{
			BOARD_CheckFlip(INDEX_PLAY);
		}

		if(Telnet.nTelnetState == WIN_MAXIMIZE)
		{
			wCoord [COORD_TELNET].s = WIN_SHOW;
			ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
		}

		if(bNewGame)
		{
			System.bPromoteCommand = 0;
			System.nPromoteWPiece  = 0;
			System.nPromoteBPiece  = 0;
			System.bPromoteKnight  = 0;

			Game [INDEX_PLAY].bFirstResize                 = 1;
			Game [INDEX_PLAY].bFirstGame                   = 1;
			Game [INDEX_PLAY].nGameType                    = System.nLastPlayGameType;
			Game [INDEX_PLAY].bTickClock                   = 1;
			Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
			Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc * 1000;

			strcpy(Game [INDEX_PLAY].cLastMove,             ICS_INITIAL_MOVE_NAME);
			strcpy(Game [INDEX_PLAY].cVerboseMove,          ICS_INITIAL_MOVE_NAME);
			strcpy(Game [INDEX_PLAY].cTime4Move,            ICS_INITIAL_TIME_MOVE);
			strcpy(Game [INDEX_PLAY].cRating [INDEX_WHITE], "");
			strcpy(Game [INDEX_PLAY].cRating [INDEX_BLACK], "");
			strcpy(Game [INDEX_PLAY].cOrgResult,            "");

			Game [INDEX_PLAY].bSavedGame = 0;
			Game [INDEX_PLAY].nRated     = ICS_INITIAL_RATED;

			strcpy(Game [INDEX_PLAY].cGameType, ICS_INITIAL_GAMETYPE_STRING);

			LagCmd.tLastLagCommand.nSec = 0;
			LagCmd.tLastLagCommand.nMs  = 0;

			Game [INDEX_PLAY].nClockTimerEvent       = 0;
			Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 0;
			Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 0;
			Game [INDEX_PLAY].tTickStartTM.nSec      = 0;
			Game [INDEX_PLAY].tTickStartTM.nMs       = 0;
			Game [INDEX_PLAY].nIntendedTickLength    = 0;
			Game [INDEX_PLAY].nLag [INDEX_WHITE]     = 0;
			Game [INDEX_PLAY].nLag [INDEX_BLACK]     = 0;

			Game [INDEX_PLAY].bIssuedEventCommand = 0;

			CLOCK_StartClocks(INDEX_PLAY, Game [INDEX_PLAY].hwnd);

			BOARD_LoadBuffer(INDEX_PLAY, INIT_BUFFER, INIT_BUFFER);

			BOARD_NullLastHighlight(INDEX_PLAY);
			BOARD_NullLastBoardBuffer(INDEX_PLAY);

			Premove.nPremoveCount     = 0;
			Premove.nPremoveHead      = 0;
			Premove.nPremoveTail      = 0;
			Premove.bLastTP           = 0;
			Premove.ptLastTP [0].x    = -1;
			Premove.ptLastTP [0].y    = -1;
			Premove.ptLastTP [1].x    = -1;
			Premove.ptLastTP [1].y    = -1;
			Premove.bIllegalTP        = 0;
			Premove.ptIllegalTP [0].x = -1;
			Premove.ptIllegalTP [0].y = -1;
			Premove.ptIllegalTP [1].x = -1;
			Premove.ptIllegalTP [1].y = -1;

			BOARD_DrawHandle(INDEX_PLAY, hdc);
			BOARD_DrawWhiteLag(INDEX_PLAY, hdc);
			BOARD_DrawBlackLag(INDEX_PLAY, hdc);
			BOARD_DrawWhiteClock(INDEX_PLAY, hdc);
			BOARD_DrawBlackClock(INDEX_PLAY, hdc);
			BOARD_DrawGameType(INDEX_PLAY, hdc);
			BOARD_DrawLastMove(INDEX_PLAY, hdc);
			BOARD_DrawResult(INDEX_PLAY, hdc);
			BOARD_DrawCoordinates(INDEX_PLAY, hdc);
			BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NEW_GAME);

			SetWindowText(Game [INDEX_PLAY].hwnd, TOOLBOX_GetGameWindowTitle(INDEX_PLAY));

			if(wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
			{
				wCoord [COORD_PLAY].s = WIN_SHOW;
				ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);
			}

			wCoord [COORD_PLAY].s = WIN_SHOW;
			SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);

			// start of own game
			GAMESOUND_Play(GAME_SOUND_PLAY_START);

			POSITION_FirstTime(INDEX_PLAY);

			sprintf(cGarbage, "%s %d\n", ICC_MOVELIST_COMMAND, nN);
			TOOLBOX_WriteICS(cGarbage);

			System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1;

			BUGHOUSE_FirstTime(INDEX_PLAY);

			DragInfo.nClicked = 0;

			if(! User.bShowMoveButton)
			{
				if(Game [INDEX_PLAY].bShowMoveButtons)
				{
					TOOLBOX_ShowMoveButtons(INDEX_PLAY, 0);
				}
			}

			if(User.bICCPlayCloseAll)
			{
				for(nI = 1 ; nI < MAX_GAME ; nI++)
				{
					if(IsWindow(Game [nI].hwnd))
					{
						Game [nI].bFirstGame  = 0;
						Game [nI].nGameNumber = 0;
						Game [nI].bPlaying    = 0;
						Game [nI].bTickClock  = 1;

						(void) CLOCK_StopClockTimer(nI);

						SendMessage(Game [nI].hwnd, WM_CLOSE, 0, 0);
					}
				}
			}

			if(Game [INDEX_PLAY].bIPlayWhite)
			{
				strcpy(Vars.cOpponent, cbName);
			}
			else
			{
				strcpy(Vars.cOpponent, cwName);
			}

			if(Censor [Login.nLoginType].bCensor)
			{
				if(CENSOR_Other(7, Vars.cOpponent))
				{
					sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
					TOOLBOX_WriteICS(cGarbage);

					System.bIssuedAbort = 1;
				}
				else
				{
					System.bIssuedAbort = 0;
				}
			}

			STATE_PlayNewGame();

			SetFocus(hwndWindow [HWND_TELNET_EDIT]);

			Icc.bFindICCPartner = 0;
			strcpy(Icc.cICCPartner, "");
		}
		else
		{
			strcpy(Game [INDEX_PLAY].cLastMove,    cLastMove);
			strcpy(Game [INDEX_PLAY].cVerboseMove, cVerboseMove);
			strcpy(Game [INDEX_PLAY].cTime4Move,   cTime4Move);

			if(User.bShowLagStat || User.bLagCommand)
			{
				if(Game[INDEX_PLAY].nIncrementClock)
				{
					Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
					Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc * 1000;
				}
				else
				{
					// assign current time remaining to variables
					nTmpW = Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE];
					nTmpB = Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK];

					// get number of milliseconds on clocks
					nTimeRemainder [INDEX_WHITE] = nTmpW % 1000;
					nTimeRemainder [INDEX_BLACK] = nTmpB % 1000;

					// calculate lag display for debugging / putting into lag
					nLagThisMove [INDEX_WHITE] = ((nWc * 1000) + nTimeRemainder [INDEX_WHITE]) - nTmpW;
					nLagThisMove [INDEX_BLACK] = ((nBc * 1000) + nTimeRemainder [INDEX_BLACK]) - nTmpB;

					// if we are near a boundary and there is some small lag, fix up
					if(nLagThisMove [INDEX_WHITE] == 1000 && nTimeRemainder [INDEX_WHITE] >= 700)
					{
						nTmpW += 1000;
						nTmpW -= nTimeRemainder[INDEX_WHITE];
						nTimeRemainder [INDEX_WHITE] = 0;
					}

					if(nLagThisMove [INDEX_BLACK] == 1000 && nTimeRemainder [INDEX_BLACK] >= 700)
					{
						nTmpB += 1000;
						nTmpB -= nTimeRemainder [INDEX_BLACK];
						nTimeRemainder [INDEX_BLACK] = 0;
					}

					// small negative lag, fix up
					if(nLagThisMove [INDEX_WHITE] <= -1000  && nTimeRemainder [INDEX_WHITE] <= 300)
					{
						nTmpW -= 300;
						nTimeRemainder [INDEX_WHITE] = 800;
					}

					if(nLagThisMove [INDEX_BLACK] <= -1000 && nTimeRemainder [INDEX_BLACK] <= 300)
					{
						nTmpB -= 300;
						nTimeRemainder [INDEX_BLACK] = 800;
					}

					// calculate lag display for debugging / putting into lag
					nLagThisMove [INDEX_WHITE] = ((nWc * 1000) + nTimeRemainder [INDEX_WHITE]) - nTmpW;
					nLagThisMove [INDEX_BLACK] = ((nBc * 1000) + nTimeRemainder [INDEX_BLACK]) - nTmpB;

					// put in the lag for this move
					if(nLagThisMove [INDEX_WHITE] > 0)
					{
						Game [INDEX_PLAY].nLag [INDEX_WHITE] += nLagThisMove [INDEX_WHITE];
					}

					if(nLagThisMove [INDEX_BLACK] > 0)
					{
						Game [INDEX_PLAY].nLag [INDEX_BLACK] += nLagThisMove [INDEX_BLACK];
					}

					Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
					Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc * 1000;
				}
			}
			else
			{
				Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
				Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc * 1000;
			}

			CLOCK_StartClocks(INDEX_PLAY, Game [INDEX_PLAY].hwnd);

			BOARD_DrawHandle(INDEX_PLAY, hdc);
			BOARD_DrawWhiteLag(INDEX_PLAY, hdc);
			BOARD_DrawBlackLag(INDEX_PLAY, hdc);
			BOARD_DrawWhiteClock(INDEX_PLAY, hdc);
			BOARD_DrawBlackClock(INDEX_PLAY, hdc);
			BOARD_DrawGameType(INDEX_PLAY, hdc);
			BOARD_DrawLastMove(INDEX_PLAY, hdc);

			if(User.bAnimatePlay)
			{
				if(System.bIsMyTurn)
				{
					ANIMATE_Move(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, User.nAnimateSpeedPlay);
					BOARD_LoadBoard(INDEX_PLAY, cS + 5, cVerboseMove);
				}
			}

			BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_BOARD);

			if(wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
			{
				wCoord [COORD_PLAY].s = WIN_SHOW;
				ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);

				SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);
				SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			}

			if(! System.bIsMyTurn)
			{
				if(strlen(F8Key.cSitPieceDropMove) > 0)
				{
					F8KEY_Init();
				}
			}

			POSITION_SaveGameMove(INDEX_PLAY);
		}

		if(Game [INDEX_PLAY].bPlaying)
		{
			if(! Game [INDEX_PLAY].bInitialMove)
			{
				if(Game [INDEX_PLAY].bIPlayWhite)
				{
					if(Game [INDEX_PLAY].bWhitesMove)
					{
						if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CHECK);
						}
						else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CAPTURE);
						}
						else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CASTLE);
						}
						else
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_MOVE);
						}
					}
					else
					{
						if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CAPTURE);
						}
						else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CASTLE);
						}
						else
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_MOVE);
						}
					}
				}
				else
				{
					if(Game [INDEX_PLAY].bWhitesMove)
					{
						if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CAPTURE);
						}
						else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CASTLE);
						}
						else
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_MOVE);
						}
					}
					else
					{
						if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CHECK);
						}
						else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CAPTURE);
						}
						else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_CASTLE);
						}
						else
						{
							GAMESOUND_Play(GAME_SOUND_PLAY_MOVE);
						}
					}
				}
			}
		}

		// has true premove turned on
		if(User.bTruePremove)
		{
			// it's my move
			if(System.bIsMyTurn)
			{
				// has alreay a true premove in the buffer
				if(Premove.nPremoveCount > 0)
				{
					nJ = 0;
					while(Premove.nPremoveCount > 0)
					{
						if(User.nTruePremove == PREMOVE_LR)
						{
							if(Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [8] == EMPTY_SQUARE)
							{
								nK = 1;
							}
							else
							{
								if(Game [INDEX_PLAY].nBoard [Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5]]
										[Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6]] == EMPTY_SQUARE)
								{
									if(Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [2] == (Game [INDEX_PLAY].bIPlayWhite ? WHITE_PAWN : BLACK_PAWN))
									{
										if(Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3] ==
												Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5])
										{
											nK = 1;
										}
										else
										{
											nK = 0;
										}
									}
									else
									{
										nK = 0;
									}
								}
								else
								{
									nK = 1;
								}
							}
						}
						else
						{
							nK = 1;
						}

						if(nK)
						{
							nOA = User.bAutoQueen;
							User.bAutoQueen = 0;

							if(CHESS_IsLegalMove(Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [1],
												 Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [2],
												 Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3],
												 Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4],
												 Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5],
												 Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6],
												 Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [7]))
							{
								// it's a legal move lets send it
								TOOLBOX_WriteICS(Premove.cPremoveBuffer [Premove.nPremoveTail]);

								User.bAutoQueen = nOA;

								// erase this true premove highlight
								Premove.bLastTP        = 1;
								Premove.ptLastTP [0].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3];
								Premove.ptLastTP [0].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4];
								Premove.ptLastTP [1].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5];
								Premove.ptLastTP [1].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6];

								// update true premove
								Premove.nPremoveCount = Premove.nPremoveCount - 1;
								Premove.nPremoveTail  = Premove.nPremoveTail  + 1;

								if(Premove.nPremoveTail >= MAX_TRUE_PREMOVE)
								{
									Premove.nPremoveTail = 0;
								}

								Game [INDEX_PLAY].bOnLagClock = 1;
								Game [INDEX_PLAY].bWhitesMove = (! Game [INDEX_PLAY].bWhitesMove);

								CLOCK_SwitchClocks(INDEX_PLAY);

								BOARD_DrawWhiteClock(INDEX_PLAY, hdc);
								BOARD_DrawBlackClock(INDEX_PLAY, hdc);
								BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NONE);

								ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);

								System.bIsMyTurn = 0;

								if(strlen(F8Key.cSitPieceDropMove) > 0)
								{
									F8KEY_Init();
								}
								return 1;
							}

							User.bAutoQueen = nOA;
						}

						// erase this true premove highlight
						Premove.bLastTP        = 1;
						Premove.ptLastTP [0].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3];
						Premove.ptLastTP [0].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4];
						Premove.ptLastTP [1].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5];
						Premove.ptLastTP [1].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6];

						// remember this illegal true premove
						if(nJ == 0)
						{
							nJ = 1;
							nA = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3];
							nB = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4];
							nC = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5];
							nD = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6];
						}

						// update true premove
						Premove.nPremoveCount = Premove.nPremoveCount - 1;
						Premove.nPremoveTail  = Premove.nPremoveTail  + 1;

						if(Premove.nPremoveTail >= MAX_TRUE_PREMOVE)
						{
							Premove.nPremoveTail = 0;
						}

						// draw board
						BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NONE);
					}

					Premove.nPremoveCount  = 0;
					Premove.nPremoveHead   = 0;
					Premove.nPremoveTail   = 0;
					Premove.bLastTP        = 0;
					Premove.ptLastTP [0].x = -1;
					Premove.ptLastTP [0].y = -1;
					Premove.ptLastTP [1].x = -1;
					Premove.ptLastTP [1].y = -1;

					if(nJ)
					{
						Premove.bIllegalTP        = 1;
						Premove.ptIllegalTP [0].x = nA;
						Premove.ptIllegalTP [0].y = nB;
						Premove.ptIllegalTP [1].x = nC;
						Premove.ptIllegalTP [1].y = nD;

						BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_DRAW_ILLEGAL_TP);
					}

					if(User.bIllegalTPBeep)
					{
						TOOLBOX_Beep();
					}
				}
			}
		}

		ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);
		return 1;
	}

	// examing game
	if(nRl == 2)
	{
		System.bIsMyTurn = 0;

		bOrgFlip = Game [INDEX_PLAY].bFlip;

		if(Game [INDEX_PLAY].nGameNumber == nN)
		{
			bNewGame = 0;
		}
		else
		{
			bNewGame = 1;

			if(strlen(Vars.cExam) > 0)
			{
				if(TOOLBOX_MatchHandle(cwName, Vars.cExam))
				{
					strcpy(Vars.cExam, "");
					Game [INDEX_PLAY].bFlip = 0;
				}
				else if(TOOLBOX_MatchHandle(cbName, Vars.cExam))
				{
					strcpy(Vars.cExam, "");
					Game [INDEX_PLAY].bFlip = 1;
				}
				else
				{
					Game [INDEX_PLAY].bFlip = 0;
				}
			}
			else
			{
				Game [INDEX_PLAY].bFlip = 0;
			}
		}

		Game [INDEX_PLAY].bValid          = 1;
		Game [INDEX_PLAY].nGameNumber     = nN;
		Game [INDEX_PLAY].nGamePartner    = 0;
		Game [INDEX_PLAY].nRelation       = nRl;
		Game [INDEX_PLAY].bWhitesMove     = (cTurn == 'W');
		Game [INDEX_PLAY].bLastWhitesMove = Game [INDEX_PLAY].bWhitesMove;
		Game [INDEX_PLAY].bPlaying        = 0;
		Game [INDEX_PLAY].bIPlayWhite     = (stricmp(cwName, Vars.cWhoAmI) == 0);
		Game [INDEX_PLAY].bInitialMove    = (stricmp(cLastMove, ICS_INITIAL_MOVE_NAME) == 0);
		Game [INDEX_PLAY].nMoveNumber     = nMn;

		strcpy(Game [INDEX_PLAY].cHandle [INDEX_WHITE], cwName);
		strcpy(Game [INDEX_PLAY].cHandle [INDEX_BLACK], cbName);
		strcpy(Game [INDEX_PLAY].cResult, "");

		Game [INDEX_PLAY].nLastDoublePushFile               = nLP;
		Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_WHITE] = bWCK;
		Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_WHITE] = bWCQ;
		Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_BLACK] = bBCK;
		Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_BLACK] = bBCQ;
		Game [INDEX_PLAY].nHalfMoves                        = nHalf;

		Game [INDEX_PLAY].nInitialClock                = nIc;
		Game [INDEX_PLAY].nIncrementClock              = nIi;
		Game [INDEX_PLAY].nClockTimerEvent             = 0;
		Game [INDEX_PLAY].bTickClock                   = 0;
		Game [INDEX_PLAY].bFlagged [INDEX_WHITE]       = 0;
		Game [INDEX_PLAY].bFlagged [INDEX_BLACK]       = 0;
		Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
		Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc * 1000;
		Game [INDEX_PLAY].tTickStartTM.nSec            = 0;
		Game [INDEX_PLAY].tTickStartTM.nMs             = 0;
		Game [INDEX_PLAY].nIntendedTickLength          = 0;
		Game [INDEX_PLAY].bOnLagClock                  = 0;
		Game [INDEX_PLAY].nNextLagCheck                = 1000;
		Game [INDEX_PLAY].nLag [INDEX_WHITE]           = 0;
		Game [INDEX_PLAY].nLag [INDEX_BLACK]           = 0;

		LagCmd.tLastLagCommand.nSec = 0;
		LagCmd.tLastLagCommand.nMs  = 0;

		hdc = GetDC(Game [INDEX_PLAY].hwnd);

		if(bOrgFlip != Game [INDEX_PLAY].bFlip)
		{
			BOARD_CheckFlip(INDEX_PLAY);
		}

		if(Telnet.nTelnetState == WIN_MAXIMIZE)
		{
			wCoord [COORD_TELNET].s = WIN_SHOW;
			ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
		}

		if(bNewGame)
		{
			System.bPromoteCommand = 0;
			System.nPromoteWPiece  = 0;
			System.nPromoteBPiece  = 0;
			System.bPromoteKnight  = 0;

			Game [INDEX_PLAY].bFirstResize = 1;
			Game [INDEX_PLAY].bFirstGame   = 1;
			Game [INDEX_PLAY].nGameType    = System.nLastExamGameType;
			Game [INDEX_PLAY].bChessGame   = 0;

			TOOLBOX_InvalidateBoardWindow(0);

			BOARD_LoadBoard(INDEX_PLAY, cS + 5, ICS_INITIAL_MOVE_NAME);

			BOARD_GetHighlightFromLastMove(INDEX_PLAY, cVerboseMove, cLastMove);

			strcpy(Game [INDEX_PLAY].cLastMove,             ICS_INITIAL_MOVE_NAME);
			strcpy(Game [INDEX_PLAY].cVerboseMove,          ICS_INITIAL_MOVE_NAME);
			strcpy(Game [INDEX_PLAY].cTime4Move,            ICS_INITIAL_TIME_MOVE);
			strcpy(Game [INDEX_PLAY].cRating [INDEX_WHITE], "");
			strcpy(Game [INDEX_PLAY].cRating [INDEX_BLACK], "");
			strcpy(Game [INDEX_PLAY].cOrgResult,            "");

			Game [INDEX_PLAY].bSavedGame = 0;
			Game [INDEX_PLAY].nRated     = ICS_INITIAL_RATED;

			strcpy(Game [INDEX_PLAY].cGameType, ICS_INITIAL_GAMETYPE_STRING);

			Game [INDEX_PLAY].bIssuedEventCommand = 0;

			CLOCK_StartClocks(INDEX_PLAY, Game [INDEX_PLAY].hwnd);

			BOARD_LoadBuffer(INDEX_PLAY, INIT_BUFFER, INIT_BUFFER);

			BOARD_NullLastHighlight(INDEX_PLAY);
			BOARD_NullLastBoardBuffer(INDEX_PLAY);

			Premove.nPremoveCount  = 0;
			Premove.nPremoveHead   = 0;
			Premove.nPremoveTail   = 0;
			Premove.bLastTP        = 0;
			Premove.ptLastTP [0].x = -1;
			Premove.ptLastTP [0].y = -1;
			Premove.ptLastTP [1].x = -1;
			Premove.ptLastTP [1].y = -1;

			BOARD_DrawHandle(INDEX_PLAY, hdc);
			BOARD_DrawWhiteLag(INDEX_PLAY, hdc);
			BOARD_DrawBlackLag(INDEX_PLAY, hdc);
			BOARD_DrawWhiteClock(INDEX_PLAY, hdc);
			BOARD_DrawBlackClock(INDEX_PLAY, hdc);
			BOARD_DrawGameType(INDEX_PLAY, hdc);
			BOARD_DrawLastMove(INDEX_PLAY, hdc);
			BOARD_DrawResult(INDEX_PLAY, hdc);
			BOARD_DrawCoordinates(INDEX_PLAY, hdc);
			BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NEW_GAME);

			POSITION_FirstTime(INDEX_PLAY);

			sprintf(cGarbage, "%s %d\n", ICC_MOVELIST_COMMAND, nN);
			TOOLBOX_WriteICS(cGarbage);

			System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1;

			BUGHOUSE_FirstTime(INDEX_PLAY);

			SetWindowText(Game [INDEX_PLAY].hwnd, TOOLBOX_GetGameWindowTitle(INDEX_PLAY));

			if(wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
			{
				wCoord [COORD_PLAY].s = WIN_SHOW;
				ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);
			}

			wCoord [COORD_PLAY].s = WIN_SHOW;
			SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);

			TOOLBOX_ShowMoveButtons(INDEX_PLAY, 1);

			SetFocus(hwndWindow [HWND_TELNET_EDIT]);

			Icc.bFindICCPartner = 0;
			strcpy(Icc.cICCPartner, "");
		}
		else
		{
			strcpy(Game [INDEX_PLAY].cLastMove,    cLastMove);
			strcpy(Game [INDEX_PLAY].cVerboseMove, cVerboseMove);
			strcpy(Game [INDEX_PLAY].cTime4Move,   cTime4Move);

			CLOCK_StartClocks(INDEX_PLAY, Game [INDEX_PLAY].hwnd);

			if(wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
			{
				wCoord [COORD_PLAY].s = WIN_SHOW;
				ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);

				SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);
				SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			}

			BOARD_DrawHandle(INDEX_PLAY, hdc);
			BOARD_DrawWhiteLag(INDEX_PLAY, hdc);
			BOARD_DrawBlackLag(INDEX_PLAY, hdc);
			BOARD_DrawWhiteClock(INDEX_PLAY, hdc);
			BOARD_DrawBlackClock(INDEX_PLAY, hdc);
			BOARD_DrawGameType(INDEX_PLAY, hdc);
			BOARD_DrawLastMove(INDEX_PLAY, hdc);

			if(User.bAnimateExam)
			{
				ANIMATE_Move(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, User.nAnimateSpeedExam);
			}
			BOARD_LoadBoard(INDEX_PLAY, cS + 5, cVerboseMove);

			BOARD_GetHighlightFromLastMove(INDEX_PLAY, cVerboseMove, cLastMove);

			BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_BOARD);
		}

		ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);

		if(! Game [INDEX_PLAY].bInitialMove)
		{
			if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
			{
				GAMESOUND_Play(GAME_SOUND_EXAM_CHECK);
			}
			else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
			{
				GAMESOUND_Play(GAME_SOUND_EXAM_CAPTURE);
			}
			else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
			{
				GAMESOUND_Play(GAME_SOUND_EXAM_CASTLE);
			}
			else
			{
				GAMESOUND_Play(GAME_SOUND_EXAM_MOVE);
			}
		}
		return 1;
	}

	if((nRl ==  0) ||                                       //  0 i am observing a game being played
			(nRl == -2) ||                                     // -2 i am observing game being examined
			((nRl == -3) && (System.nIssuedMovesCommand == 0))) // -3 isolated position, such as for "ref 3" or the "sposition" command
	{
		// observe game
		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			if(Game [nI].bValid)
			{
				if(Game [nI].nGameNumber == nN)
				{
					Game [nI].nRelation       = nRl;
					Game [nI].bWhitesMove     = (cTurn == 'W');
					Game [nI].bLastWhitesMove = Game [nI].bWhitesMove;
					Game [nI].bPlaying        = (nRl == 0);
					Game [nI].bInitialMove    = (strcmp(cLastMove, ICS_INITIAL_MOVE_NAME) == 0);
					Game [nI].nMoveNumber     = nMn;

					strcpy(Game [nI].cLastMove,             cLastMove);
					strcpy(Game [nI].cVerboseMove,          cVerboseMove);
					strcpy(Game [nI].cTime4Move,            cTime4Move);
					strcpy(Game [nI].cHandle [INDEX_WHITE], cwName);
					strcpy(Game [nI].cHandle [INDEX_BLACK], cbName);
					strcpy(Game [nI].cResult,               "");

					Game [nI].nLastDoublePushFile               = nLP;
					Game [nI].bCanCastleKingSide  [INDEX_WHITE] = bWCK;
					Game [nI].bCanCastleQueenSide [INDEX_WHITE] = bWCQ;
					Game [nI].bCanCastleKingSide  [INDEX_BLACK] = bBCK;
					Game [nI].bCanCastleQueenSide [INDEX_BLACK] = bBCQ;
					Game [nI].nHalfMoves                        = nHalf;

					Game [nI].nInitialClock   = nIc;
					Game [nI].nIncrementClock = nIi;

					if(Game [nI].bPlaying)
					{
						if(! Game [nI].nIncrementClock)
						{
							if(User.bShowLagStat || User.bLagCommand)
							{
								// assign current time remaining to variables
								nTmpW = Game [nI].nTimeRemaining [INDEX_WHITE];
								nTmpB = Game [nI].nTimeRemaining [INDEX_BLACK];

								// get number of milliseconds on clocks
								nTimeRemainder [INDEX_WHITE] = nTmpW % 1000;
								nTimeRemainder [INDEX_BLACK] = nTmpB % 1000;

								// calculate lag display for debugging / putting into lag
								nLagThisMove [INDEX_WHITE] = ((nWc * 1000) + nTimeRemainder [INDEX_WHITE]) - nTmpW;
								nLagThisMove [INDEX_BLACK] = ((nBc * 1000) + nTimeRemainder [INDEX_BLACK]) - nTmpB;

								// if we are near a boundary and there is some small lag, fix up
								if(nLagThisMove [INDEX_WHITE] == 1000 && nTimeRemainder [INDEX_WHITE] >= 700)
								{
									nTmpW += 1000;
									nTmpW -= nTimeRemainder [INDEX_WHITE];
									nTimeRemainder [INDEX_WHITE] = 0;
								}

								if(nLagThisMove [INDEX_BLACK] == 1000 && nTimeRemainder [INDEX_BLACK] >= 700)
								{
									nTmpB += 1000;
									nTmpB -= nTimeRemainder [INDEX_BLACK];
									nTimeRemainder [INDEX_BLACK] = 0;
								}

								// small negative lag, fix up
								if(nLagThisMove [INDEX_WHITE] <= -1000  && nTimeRemainder [INDEX_WHITE] <= 300)
								{
									nTmpW -= 300;
									nTimeRemainder [INDEX_WHITE] = 800;
								}

								if(nLagThisMove [INDEX_BLACK] <= -1000 && nTimeRemainder [INDEX_BLACK] <= 300)
								{
									nTmpW -= 300;
									nTimeRemainder [INDEX_BLACK] = 800;
								}

								// calculate lag display for debugging / putting into lag
								nLagThisMove [INDEX_WHITE] = ((nWc * 1000) + nTimeRemainder [INDEX_WHITE]) - nTmpW;
								nLagThisMove [INDEX_BLACK] = ((nBc * 1000) + nTimeRemainder [INDEX_BLACK]) - nTmpB;

								// put in the lag for this move
								if(nLagThisMove [INDEX_WHITE] > 0)
								{
									Game [nI].nLag [INDEX_WHITE] += nLagThisMove [INDEX_WHITE];
								}

								if(nLagThisMove [INDEX_BLACK] > 0)
								{
									Game [nI].nLag [INDEX_BLACK] += nLagThisMove [INDEX_BLACK];
								}
							}
						}
					}

					Game [nI].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
					Game [nI].nTimeRemaining [INDEX_BLACK] = nBc * 1000;

					if(nRl == -3)
					{
						Game [nI].bTickClock = 0;
					}
					else
					{
						Game [nI].bTickClock = 1;
					}

					CLOCK_StartClocks(nI, Game [nI].hwnd);

					if(Game [nI].bValid)
					{
						hdc = GetDC(Game [nI].hwnd);

						BOARD_DrawHandle(nI, hdc);
						BOARD_DrawWhiteLag(nI, hdc);
						BOARD_DrawBlackLag(nI, hdc);
						BOARD_DrawWhiteClock(nI, hdc);
						BOARD_DrawBlackClock(nI, hdc);
						BOARD_DrawGameType(nI, hdc);

						if(TOOLBOX_DisplayActualBoard(nI))
						{
							BOARD_DrawLastMove(nI, hdc);

							if(User.bAnimateObserve)
							{
								ANIMATE_Move(nI, Game [nI].hwnd, hdc, User.nAnimateSpeedObserve);
							}
							BOARD_LoadBoard(nI, cS + 5, cVerboseMove);

							BOARD_GetHighlightFromLastMove(nI, cVerboseMove, cLastMove);

							BOARD_DrawBoard1(nI, Game [nI].hwnd, hdc, DRAW_STATE_BOARD);
						}
						else
						{
							BOARD_DrawLastMove1(nI, hdc);

							BOARD_LoadBoard(nI, cS + 5, cVerboseMove);

							BOARD_GetHighlightFromLastMove(nI, cVerboseMove, cLastMove);
						}

						ReleaseDC(Game [nI].hwnd, hdc);

						if(nRl != -3)
						{
							if(! Game [nI].bInitialMove)
							{
								if(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
								{
									if(strlen(Vars.cPartner) > 0)
									{
										if((stricmp(Vars.cPartner, Game [nI].cHandle [INDEX_WHITE]) == 0) ||
												(stricmp(Vars.cPartner, Game [nI].cHandle [INDEX_BLACK]) == 0))
										{
											if(Game [INDEX_PLAY].bIPlayWhite)
											{
												if(Game [nI].bWhitesMove)
												{
													if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CAPTURE);
													}
													else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CASTLE);
													}
													else
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_MOVE);
													}
												}
												else
												{
													if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CHECK);
													}
													else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CAPTURE);
													}
													else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CASTLE);
													}
													else
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_MOVE);
													}
												}
											}
											else
											{
												if(Game [nI].bWhitesMove)
												{
													if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CHECK);
													}
													else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CAPTURE);
													}
													else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CASTLE);
													}
													else
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_MOVE);
													}
												}
												else
												{
													if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CAPTURE);
													}
													else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_CASTLE);
													}
													else
													{
														GAMESOUND_Play(GAME_SOUND_PARTNER_MOVE);
													}
												}
											}
										}
										else
										{
											if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
											{
												GAMESOUND_Play(GAME_SOUND_OBS_CHECK1);
											}
											else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
											{
												GAMESOUND_Play(GAME_SOUND_OBS_CAPTURE1);
											}
											else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
											{
												GAMESOUND_Play(GAME_SOUND_OBS_CASTLE1);
											}
											else
											{
												GAMESOUND_Play(GAME_SOUND_OBS_MOVE1);
											}
										}
									}
									else
									{
										if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
										{
											GAMESOUND_Play(GAME_SOUND_OBS_CHECK1);
										}
										else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
										{
											GAMESOUND_Play(GAME_SOUND_OBS_CAPTURE1);
										}
										else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
										{
											GAMESOUND_Play(GAME_SOUND_OBS_CASTLE1);
										}
										else
										{
											GAMESOUND_Play(GAME_SOUND_OBS_MOVE1);
										}
									}
								}
								else
								{
									if(strchr(cLastMove, ICS_KING_IN_CHECK_SYMBOL))
									{
										GAMESOUND_Play(GAME_SOUND_OBS_CHECK2);
									}
									else if(strchr(cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
									{
										GAMESOUND_Play(GAME_SOUND_OBS_CAPTURE2);
									}
									else if(strchr(cLastMove, ICS_CASTLE_SYMBOL))
									{
										GAMESOUND_Play(GAME_SOUND_OBS_CASTLE2);
									}
									else
									{
										GAMESOUND_Play(GAME_SOUND_OBS_MOVE2);
									}
								}
							}
						}
					}

					POSITION_SaveGameMove(nI);
					return 1;
				}
			}
		}

		//
		// if it's a game that we unobserved then eat this style 12 string
		//
		if(System.bUnobserve)
		{
			long nTmp;
			TIMEMARK now;

			CLOCK_GetTimeMark(&now);

			nTmp = CLOCK_SubtractTimeMarks(&now, &System.tUnobserve);
			if(nTmp <= 3)
			{
				if((System.nUnobGameNumber              == nN) &&
						(stricmp(System.cUnobWname, cwName) == 0) &&
						(stricmp(System.cUnobBname, cbName) == 0))
				{
					if(strlen(Vars.cObserve) == 0)
					{
						//
						// skip this second style 12 string
						//
						System.bUnobserve           = 0;
						System.nUnobGameNumber      = 0;
						strcpy(System.cUnobWname, "");
						strcpy(System.cUnobBname, "");

						sprintf(cGarbage, "%s %d\n", ICS_UNOBSERVE_COMMAND, nN);
						TOOLBOX_WriteICS(cGarbage);
						return 0;
					}
					else
					{
						if((! TOOLBOX_MatchHandle(System.cUnobWname, Vars.cObserve)) &&
								(! TOOLBOX_MatchHandle(System.cUnobBname, Vars.cObserve)))
						{
							//
							// skip this second style 12 string
							//
							System.bUnobserve           = 0;
							System.nUnobGameNumber      = 0;
							strcpy(System.cUnobWname, "");
							strcpy(System.cUnobBname, "");

							sprintf(cGarbage, "%s %d\n", ICS_UNOBSERVE_COMMAND, nN);
							TOOLBOX_WriteICS(cGarbage);
							return 0;
						}
					}
				}
			}

			System.bUnobserve           = 0;
			System.nUnobGameNumber      = 0;
			strcpy(System.cUnobWname, "");
			strcpy(System.cUnobBname, "");
		}

		// recycle observe window
		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			if(Game [nI].bValid)
			{
				if(Game [nI].nGameNumber == 0)
				{
					Game [nI].bFirstResize = 1;
					Game [nI].bFirstGame   = 1;
					Game [nI].bValid       = 1;
					Game [nI].nGamePartner = 0;
					Game [nI].nRelation    = nRl;

					nFlip    = 0;
					bOrgFlip = Game [nI].bFlip;

					if(Icc.bFindICCPartner)
					{
						if((stricmp(Icc.cICCPartner, cwName) == 0) ||
								(stricmp(Icc.cICCPartner, cbName) == 0))
						{
							Icc.bFindICCPartner = 0;
							strcpy(Icc.cICCPartner, "");

							if(Game [INDEX_PLAY].bIPlayWhite)
							{
								strcpy(cGarbage, cbName);
							}
							else
							{
								strcpy(cGarbage, cwName);
							}

							Game [INDEX_PLAY].nGamePartner = nN;
							Game [nI].nGamePartner         = Game [INDEX_PLAY].nGameNumber;

							if(stricmp(Vars.cPartner, cGarbage) != 0)
							{
								strcpy(Vars.cPartner, cGarbage);
								TOOLBOX_SetTelnetCaption();
								BUTTON_Refresh();
							}
						}
					}

					if(stricmp(Vars.cPartner, cwName) != 0 && stricmp(Vars.cPartner, cbName) != 0)
					{
						if(nN == Icc.nSecondBugGame)
						{
							Game [nI].bFlip = Icc.bSecondBugFlip;

							if(Icc.nFirstBugGame != 0)
							{
								for(nJ = 1 ; nJ < MAX_GAME ; nJ++)
								{
									if(Game [nJ].bValid)
									{
										if(Game [nJ].nGameNumber == Icc.nFirstBugGame)
										{
											Game [nJ].nGamePartner = nN;
											Game [nI].nGamePartner = Game [nJ].nGameNumber;

											if(strlen(Vars.cFollow) > 0)
											{
												if(stricmp(Game [nJ].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
												{
													if(! Game [nJ].bFlip)
													{
														nFlip = 1;
													}
												}
											}

											if(! nFlip)
											{
												if(strlen(Vars.cObserve) > 0)
												{
													if(TOOLBOX_MatchHandle(Game [nJ].cHandle [INDEX_WHITE], Vars.cObserve))
													{
														strcpy(Vars.cObserve, "");

														if(Game [nJ].bFlip)
														{
															nFlip = 1;
														}
													}
													else if(TOOLBOX_MatchHandle(Game [nJ].cHandle [INDEX_BLACK], Vars.cObserve))
													{
														strcpy(Vars.cObserve, "");

														if(! Game [nJ].bFlip)
														{
															nFlip = 1;
														}
													}
												}
											}
											break;
										}
									}
								}
							}

							if(! nFlip)
							{
								if(strlen(Vars.cFollow) > 0)
								{
									if(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
									{
										if(! Game [nI].bFlip)
										{
											nFlip = 1;
										}
									}
								}

								if(strlen(Vars.cObserve) > 0)
								{
									if(TOOLBOX_MatchHandle(Game [nI].cHandle [INDEX_WHITE], Vars.cObserve))
									{
										strcpy(Vars.cObserve, "");

										if(Game [nI].bFlip)
										{
											nFlip = 1;
										}
									}
									else if(TOOLBOX_MatchHandle(Game [nI].cHandle [INDEX_BLACK], Vars.cObserve))
									{
										strcpy(Vars.cObserve, "");

										if(! Game [nI].bFlip)
										{
											nFlip = 1;
										}
									}
								}
							}

							Icc.nFirstBugGame  = 0;
							Icc.bSecondBugFlip = 0;
						}
						else
						{
							if((Game [INDEX_PLAY].nGameNumber > 0) && (Game [INDEX_PLAY].bPlaying))
							{
								Game [nI].bFlip = ! Game [INDEX_PLAY].bFlip;
							}
							else
							{
								Game [nI].bFlip = 0;
							}
						}
					}
					else
					{
						if((Game [INDEX_PLAY].nGameNumber > 0) && (Game [INDEX_PLAY].bPlaying))
						{
							Game [nI].bFlip = ! Game [INDEX_PLAY].bFlip;
						}
						else
						{
							Game [nI].bFlip = 0;
						}
					}

					Game [nI].nGameNumber     = nN;
					Game [nI].nGameType       = System.nLastObserveGameType;
					Game [nI].bChessGame      = 0;
					Game [nI].bWhitesMove     = (cTurn == 'W');
					Game [nI].bLastWhitesMove = Game [nI].bWhitesMove;
					Game [nI].bPlaying        = (nRl == 0);
					Game [nI].bInitialMove    = (strcmp(cLastMove, ICS_INITIAL_MOVE_NAME) == 0);
					Game [nI].nMoveNumber     = nMn;

					strcpy(Game [nI].cLastMove,             cLastMove);
					strcpy(Game [nI].cVerboseMove,          cVerboseMove);
					strcpy(Game [nI].cTime4Move,            cTime4Move);
					strcpy(Game [nI].cHandle [INDEX_WHITE], cwName);
					strcpy(Game [nI].cHandle [INDEX_BLACK], cbName);
					strcpy(Game [nI].cRating [INDEX_WHITE], "");
					strcpy(Game [nI].cRating [INDEX_BLACK], "");

					Game [nI].nRated = ICS_INITIAL_RATED;

					strcpy(Game [nI].cGameType,  ICS_INITIAL_GAMETYPE_STRING);
					strcpy(Game [nI].cOrgResult, "");
					strcpy(Game [nI].cResult,    "");

					Game [nI].bSavedGame                        = 0;
					Game [nI].nLastDoublePushFile               = nLP;
					Game [nI].bCanCastleKingSide  [INDEX_WHITE] = bWCK;
					Game [nI].bCanCastleQueenSide [INDEX_WHITE] = bWCQ;
					Game [nI].bCanCastleKingSide  [INDEX_BLACK] = bBCK;
					Game [nI].bCanCastleQueenSide [INDEX_BLACK] = bBCQ;
					Game [nI].nHalfMoves                        = nHalf;

					Game [nI].nInitialClock                = nIc;
					Game [nI].nIncrementClock              = nIi;
					Game [nI].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
					Game [nI].nTimeRemaining [INDEX_BLACK] = nBc * 1000;
					Game [nI].nClockTimerEvent             = 0;
					Game [nI].bFlagged [INDEX_WHITE]       = 0;
					Game [nI].bFlagged [INDEX_BLACK]       = 0;

					if(nRl == -3)
					{
						Game [nI].bTickClock = 0;
					}
					else
					{
						Game [nI].bTickClock = 1;
					}

					Game [nI].tTickStartTM.nSec   = 0;
					Game [nI].tTickStartTM.nMs    = 0;
					Game [nI].nIntendedTickLength = 0;
					Game [nI].nNextLagCheck       = 1000;
					Game [nI].nLag [INDEX_WHITE]  = 0;
					Game [nI].nLag [INDEX_BLACK]  = 0;

					Game [nI].bIssuedEventCommand = 0;

					CLOCK_StartClocks(nI, Game [nI].hwnd);

					BOARD_LoadBoard(nI, cS + 5, ICS_INITIAL_MOVE_NAME);
					BOARD_LoadBuffer(nI, INIT_BUFFER, INIT_BUFFER);

					BOARD_GetHighlightFromLastMove(nI, cVerboseMove, cLastMove);

					BOARD_NullLastHighlight(nI);
					BOARD_NullLastBoardBuffer(nI);

					if(Game [nI].bValid)
					{
						hdc = GetDC(Game [nI].hwnd);

						if(bOrgFlip != Game [nI].bFlip)
						{
							BOARD_CheckFlip(nI);
						}

						BOARD_DrawHandle(nI, hdc);
						BOARD_DrawWhiteLag(nI, hdc);
						BOARD_DrawBlackLag(nI, hdc);
						BOARD_DrawWhiteClock(nI, hdc);
						BOARD_DrawBlackClock(nI, hdc);
						BOARD_DrawGameType(nI, hdc);
						BOARD_DrawLastMove(nI, hdc);
						BOARD_DrawResult(nI, hdc);
						BOARD_DrawCoordinates(nI, hdc);
						BOARD_DrawBoard1(nI, Game [nI].hwnd, hdc, DRAW_STATE_BOARD);

						ReleaseDC(Game [nI].hwnd, hdc);

						SetWindowText(Game [nI].hwnd, TOOLBOX_GetGameWindowTitle(nI));

						if(Telnet.nTelnetState == WIN_MAXIMIZE)
						{
							wCoord [COORD_TELNET].s = WIN_SHOW;
							ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
						}

						if(Game [nI].nState == WIN_MINIMIZE)
						{
							ShowWindow(Game [nI].hwnd, SW_RESTORE);
						}
						else
						{
							SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) Game [nI].hwnd, 0);
							SetFocus(hwndWindow [HWND_TELNET_EDIT]);
						}

						POSITION_FirstTime(nI);

						if(nRl != -3)
						{
							sprintf(cGarbage, "%s %d\n", ICC_MOVELIST_COMMAND, nN);
							TOOLBOX_WriteICS(cGarbage);

							System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1;
						}

						BUGHOUSE_FirstTime(nI);

						if(nFlip)
						{
							PostMessage(hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0);
						}
					}

					if(stricmp(Vars.cPartner, cwName) == 0)
					{
						if(IsWindow(Game [nI].hwnd))
						{
							MoveWindow(Game [nI].hwnd, wPartner.x, wPartner.y, wPartner.w, wPartner.h, TRUE);
							SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) Game [nI].hwnd, 0);
						}

						strcpy(Vars.cPartnerOpponent, cbName);

						if(Censor [Login.nLoginType].bCensor)
						{
							if(CENSOR_Other(7, Vars.cPartnerOpponent))
							{
								sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
								TOOLBOX_WriteICS(cGarbage);

								System.bIssuedAbort = 1;
							}
							else
							{
								System.bIssuedAbort = 0;
							}

							if(strlen(Vars.cPartner) > 0)
							{
								if(CENSOR_Partner(Vars.cPartner))
								{
									if(! System.bIssuedAbort)
									{
										sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
										TOOLBOX_WriteICS(cGarbage);

										System.bIssuedAbort = 1;
									}

									TOOLBOX_WriteICS(ICC_NO_PARTNER_COMMAND);
								}
							}
						}

						STATE_ObserveNewGame(nI, 1);
					}
					else if(stricmp(Vars.cPartner, cbName) == 0)
					{
						if(IsWindow(Game [nI].hwnd))
						{
							MoveWindow(Game [nI].hwnd, wPartner.x, wPartner.y, wPartner.w, wPartner.h, TRUE);
							SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) Game [nI].hwnd, 0);
						}

						strcpy(Vars.cPartnerOpponent, cwName);

						if(Censor [Login.nLoginType].bCensor)
						{
							if(CENSOR_Other(7, Vars.cPartnerOpponent))
							{
								sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
								TOOLBOX_WriteICS(cGarbage);

								System.bIssuedAbort = 1;
							}
							else
							{
								System.bIssuedAbort = 0;
							}

							if(strlen(Vars.cPartner) > 0)
							{
								if(CENSOR_Partner(Vars.cPartner))
								{
									if(! System.bIssuedAbort)
									{
										sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
										TOOLBOX_WriteICS(cGarbage);

										System.bIssuedAbort = 1;
									}

									TOOLBOX_WriteICS(ICC_NO_PARTNER_COMMAND);
								}
							}
						}

						STATE_ObserveNewGame(nI, 1);
					}
					else
					{
						STATE_ObserveNewGame(nI, 0);
					}
					return 1;
				}
			}
		}

		// new window
		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			if(! Game [nI].bValid)
			{
				Game [nI].bFirstResize = 1;
				Game [nI].bFirstGame   = 1;
				Game [nI].bValid       = 1;
				Game [nI].nGamePartner = 0;
				Game [nI].nRelation    = nRl;

				nFlip = 0;

				if(Icc.bFindICCPartner)
				{
					if((stricmp(Icc.cICCPartner, cwName) == 0) ||
							(stricmp(Icc.cICCPartner, cbName) == 0))
					{
						Icc.bFindICCPartner = 0;
						strcpy(Icc.cICCPartner, "");

						if(Game [INDEX_PLAY].bIPlayWhite)
						{
							strcpy(cGarbage, cbName);
						}
						else
						{
							strcpy(cGarbage, cwName);
						}

						Game [INDEX_PLAY].nGamePartner = nN;
						Game [nI].nGamePartner         = Game [INDEX_PLAY].nGameNumber;

						if(stricmp(Vars.cPartner, cGarbage) != 0)
						{
							strcpy(Vars.cPartner, cGarbage);
							TOOLBOX_SetTelnetCaption();
							BUTTON_Refresh();
						}
					}
				}

				if(stricmp(Vars.cPartner, cwName) != 0 && stricmp(Vars.cPartner, cbName) != 0)
				{
					if(nN == Icc.nSecondBugGame)
					{
						Game [nI].bFlip = Icc.bSecondBugFlip;

						if(Icc.nFirstBugGame != 0)
						{
							for(nJ = 1 ; nJ < MAX_GAME ; nJ++)
							{
								if(Game [nJ].bValid)
								{
									if(Game [nJ].nGameNumber == Icc.nFirstBugGame)
									{
										Game [nJ].nGamePartner = nN;
										Game [nI].nGamePartner = Game [nJ].nGameNumber;

										if(strlen(Vars.cFollow) > 0)
										{
											if(stricmp(Game [nJ].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
											{
												if(! Game [nJ].bFlip)
												{
													nFlip = 1;
												}
											}
										}

										if(! nFlip)
										{
											if(strlen(Vars.cObserve) > 0)
											{
												if(TOOLBOX_MatchHandle(Game [nJ].cHandle [INDEX_WHITE], Vars.cObserve))
												{
													strcpy(Vars.cObserve, "");

													if(Game [nJ].bFlip)
													{
														nFlip = 1;
													}
												}
												else if(TOOLBOX_MatchHandle(Game [nJ].cHandle [INDEX_BLACK], Vars.cObserve))
												{
													strcpy(Vars.cObserve, "");

													if(! Game [nJ].bFlip)
													{
														nFlip = 1;
													}
												}
											}
										}
										break;
									}
								}
							}
						}

						if(! nFlip)
						{
							if(strlen(Vars.cFollow) > 0)
							{
								if(stricmp(Game [nI].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
								{
									if(! Game [nI].bFlip)
									{
										nFlip = 1;
									}
								}
							}

							if(strlen(Vars.cObserve) > 0)
							{
								if(TOOLBOX_MatchHandle(Game [nI].cHandle [INDEX_WHITE], Vars.cObserve))
								{
									strcpy(Vars.cObserve, "");

									if(Game [nI].bFlip)
									{
										nFlip = 1;
									}
								}
								else if(TOOLBOX_MatchHandle(Game [nI].cHandle [INDEX_BLACK], Vars.cObserve))
								{
									strcpy(Vars.cObserve, "");

									if(! Game [nI].bFlip)
									{
										nFlip = 1;
									}
								}
							}
						}

						Icc.nFirstBugGame  = 0;
						Icc.bSecondBugFlip = 0;
					}
					else
					{
						if((Game [INDEX_PLAY].nGameNumber > 0) && (Game [INDEX_PLAY].bPlaying))
						{
							Game [nI].bFlip = ! Game [INDEX_PLAY].bFlip;
						}
						else
						{
							Game [nI].bFlip = 0;
						}
					}
				}
				else
				{
					if((Game [INDEX_PLAY].nGameNumber > 0) && (Game [INDEX_PLAY].bPlaying))
					{
						Game [nI].bFlip = ! Game [INDEX_PLAY].bFlip;
					}
					else
					{
						Game [nI].bFlip = 0;
					}
				}

				Game [nI].nGameNumber     = nN;
				Game [nI].nGameType       = System.nLastObserveGameType;
				Game [nI].bChessGame      = 0;
				Game [nI].bWhitesMove     = (cTurn == 'W');
				Game [nI].bLastWhitesMove = Game [nI].bWhitesMove;
				Game [nI].bPlaying        = (nRl == 0);
				Game [nI].bInitialMove    = (strcmp(cLastMove, ICS_INITIAL_MOVE_NAME) == 0);
				Game [nI].nMoveNumber     = nMn;

				strcpy(Game [nI].cLastMove,             cLastMove);
				strcpy(Game [nI].cVerboseMove,          cVerboseMove);
				strcpy(Game [nI].cTime4Move,            cTime4Move);
				strcpy(Game [nI].cHandle [INDEX_WHITE], cwName);
				strcpy(Game [nI].cHandle [INDEX_BLACK], cbName);
				strcpy(Game [nI].cRating [INDEX_WHITE], "");
				strcpy(Game [nI].cRating [INDEX_BLACK], "");

				Game [nI].nRated = ICS_INITIAL_RATED;

				strcpy(Game [nI].cGameType,  ICS_INITIAL_GAMETYPE_STRING);
				strcpy(Game [nI].cOrgResult, "");
				strcpy(Game [nI].cResult,    "");

				Game [nI].bSavedGame                        = 0;
				Game [nI].nLastDoublePushFile               = nLP;
				Game [nI].bCanCastleKingSide  [INDEX_WHITE] = bWCK;
				Game [nI].bCanCastleQueenSide [INDEX_WHITE] = bWCQ;
				Game [nI].bCanCastleKingSide  [INDEX_BLACK] = bBCK;
				Game [nI].bCanCastleQueenSide [INDEX_BLACK] = bBCQ;
				Game [nI].nHalfMoves                        = nHalf;

				Game [nI].nInitialClock                = nIc;
				Game [nI].nIncrementClock              = nIi;
				Game [nI].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
				Game [nI].nTimeRemaining [INDEX_BLACK] = nBc * 1000;
				Game [nI].nClockTimerEvent             = 0;
				Game [nI].bFlagged [INDEX_WHITE]       = 0;
				Game [nI].bFlagged [INDEX_BLACK]       = 0;

				if(nRl == -3)
				{
					Game [nI].bTickClock = 0;
				}
				else
				{
					Game [nI].bTickClock = 1;
				}

				Game [nI].tTickStartTM.nSec   = 0;
				Game [nI].tTickStartTM.nMs    = 0;
				Game [nI].nIntendedTickLength = 0;
				Game [nI].nNextLagCheck       = 1000;
				Game [nI].nLag [INDEX_WHITE]  = 0;
				Game [nI].nLag [INDEX_BLACK]  = 0;

				Game [nI].bIssuedEventCommand = 0;

				BOARD_LoadBoard(nI, cS + 5, ICS_INITIAL_MOVE_NAME);
				BOARD_LoadBuffer(nI, INIT_BUFFER, INIT_BUFFER);

				BOARD_GetHighlightFromLastMove(nI, cVerboseMove, cLastMove);

				BOARD_NullLastHighlight(nI);
				BOARD_NullLastBoardBuffer(nI);

				mdicreate.szClass = "MdiObserveChild";
				mdicreate.szTitle = "Observe";
				mdicreate.hOwner  = hInst;

				if(stricmp(Vars.cPartner, cwName) == 0)
				{
					mdicreate.x  = wPartner.x;
					mdicreate.y  = wPartner.y;
					mdicreate.cx = wPartner.w;
					mdicreate.cy = wPartner.h;

					bPartner = 1;

					strcpy(Vars.cPartnerOpponent, cbName);

					if(Censor [Login.nLoginType].bCensor)
					{
						if(CENSOR_Other(7, Vars.cPartnerOpponent))
						{
							sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
							TOOLBOX_WriteICS(cGarbage);

							System.bIssuedAbort = 1;
						}
						else
						{
							System.bIssuedAbort = 0;
						}

						if(strlen(Vars.cPartner) > 0)
						{
							if(CENSOR_Partner(Vars.cPartner))
							{
								if(! System.bIssuedAbort)
								{
									sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
									TOOLBOX_WriteICS(cGarbage);

									System.bIssuedAbort = 1;
								}

								TOOLBOX_WriteICS(ICC_NO_PARTNER_COMMAND);
							}
						}
					}
				}
				else if(stricmp(Vars.cPartner, cbName) == 0)
				{
					mdicreate.x  = wPartner.x;
					mdicreate.y  = wPartner.y;
					mdicreate.cx = wPartner.w;
					mdicreate.cy = wPartner.h;

					bPartner = 1;

					strcpy(Vars.cPartnerOpponent, cwName);

					if(Censor [Login.nLoginType].bCensor)
					{
						if(CENSOR_Other(7, Vars.cPartnerOpponent))
						{
							sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
							TOOLBOX_WriteICS(cGarbage);

							System.bIssuedAbort = 1;
						}
						else
						{
							System.bIssuedAbort = 0;
						}

						if(strlen(Vars.cPartner) > 0)
						{
							if(CENSOR_Partner(Vars.cPartner))
							{
								if(! System.bIssuedAbort)
								{
									sprintf(cGarbage, "%s\n", ICS_ABORT_COMMAND);
									TOOLBOX_WriteICS(cGarbage);

									System.bIssuedAbort = 1;
								}

								TOOLBOX_WriteICS(ICC_NO_PARTNER_COMMAND);
							}
						}
					}
				}
				else
				{
					mdicreate.x  = wCoord [nI].x;
					mdicreate.y  = wCoord [nI].y;
					mdicreate.cx = wCoord [nI].w;
					mdicreate.cy = wCoord [nI].h;

					bPartner = 0;
				}

				mdicreate.style  = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
				mdicreate.lParam = 0;

				System.nGameIndex = nI;

				Game [nI].hwnd = (HWND) SendMessage(hwndWindow [HWND_CLIENT], WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT) &mdicreate);

				if(Game [nI].bValid)
				{
					SetWindowText(Game [nI].hwnd, TOOLBOX_GetGameWindowTitle(nI));

					if(Telnet.nTelnetState == WIN_MAXIMIZE)
					{
						wCoord [COORD_TELNET].s = WIN_SHOW;
						ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
					}

					POSITION_FirstTime(nI);

					if(nRl != -3)
					{
						sprintf(cGarbage, "%s %d\n", ICC_MOVELIST_COMMAND, nN);
						TOOLBOX_WriteICS(cGarbage);

						System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1;
					}

					BUGHOUSE_FirstTime(nI);

					if(nFlip)
					{
						PostMessage(hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0);
					}
				}

				STATE_ObserveNewGame(nI, bPartner);
				return 1;
			}
		}
	}

	if(nRl == -3)       // -3 isolated position, such as for "ref 3" or the "sposition" command
	{
		if(System.nIssuedMovesCommand > 0)
		{
			if(System.nMoveListIndexNumber >= 0)
			{
				nI = System.nMoveListIndexNumber;
				if(Game [nI].nGameNumber == nN)
				{
					Game [nI].bErrorMoveList               = 0;
					Game [nI].bRelation4                   = 1;
					Game [nI].bClickedButton               = 0;
					Game [nI].nInitDoublePushFile          = nLP;
					Game [nI].bInitKingSide  [INDEX_WHITE] = bWCK;
					Game [nI].bInitQueenSide [INDEX_WHITE] = bWCQ;
					Game [nI].bInitKingSide  [INDEX_BLACK] = bBCK;
					Game [nI].bInitQueenSide [INDEX_BLACK] = bBCQ;
					Game [nI].nInitHalfMoves               = nHalf;
					Game [nI].nInitKingX     [INDEX_WHITE] = -1;
					Game [nI].nInitKingY     [INDEX_WHITE] = -1;
					Game [nI].nInitKingX     [INDEX_BLACK] = -1;
					Game [nI].nInitKingY     [INDEX_BLACK] = -1;
					Game [nI].nCurrentIndex                = -1;
					Game [nI].nCurrentColor                = -1;
					Game [nI].nMinIndex                    = MAX_POSITION;
					Game [nI].nMinColor                    = -1;
					Game [nI].nMaxIndex                    = -1;
					Game [nI].nMaxColor                    = -1;

					cP = cS + 5;
					for(nY = 7 ; nY >= 0 ; nY--)
					{
						for(nX = 0 ; nX < 8 ; nX++)
						{
							Game [nI].nInitBoard [nX] [nY] = BOARD_FICS_To_Int(*cP++);

							if(Game [nI].nInitBoard [nX] [nY] == WHITE_KING)
							{
								Game [nI].nInitKingX [INDEX_WHITE] = nX;
								Game [nI].nInitKingY [INDEX_WHITE] = nY;
							}
							else if(Game [nI].nInitBoard [nX] [nY] == BLACK_KING)
							{
								Game [nI].nInitKingX [INDEX_BLACK] = nX;
								Game [nI].nInitKingY [INDEX_BLACK] = nY;
							}
						}
						*cP++;
					}

					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						Game [nI].nInitBuffer [nX] = 0;
					}
					return 1;
				}
			}
		}
	}

	return 0;
}

void ICC_ParseExamBoard(char *cS)
{
	HDC hdc;
	char cGarbage [100], cTurn, cwName [20], cbName [20], cVerboseMove [10], cTime4Move [10], cLastMove [10];
	int nN, nLP, bWCK, bWCQ, bBCK, bBCQ, nHalf, nRl, nIc, nIi, nMn;
	int bOrgFlip, bNewGame;
	long nWc, nBc;

	sscanf(cS + 77,
		   "%c %d %d %d %d %d %d %d %s %s %d %d %d %s %s %d %d %d %s %s %s",
		   &cTurn,
		   &nLP, &bWCK, &bWCQ, &bBCK, &bBCQ,
		   &nHalf, &nN,
		   &cwName, &cbName,
		   &nRl, &nIc, &nIi,
		   &cGarbage, &cGarbage,
		   &nWc, &nBc, &nMn,
		   &cVerboseMove,
		   &cTime4Move,
		   &cLastMove);

	// examing game
	nRl = 2;

	System.bIsMyTurn = 0;

	bOrgFlip = Game [INDEX_PLAY].bFlip;

	if(Game [INDEX_PLAY].nGameNumber == nN)
	{
		bNewGame = 0;
	}
	else
	{
		bNewGame = 1;

		if(_stricmp(cwName, Vars.cWhoAmI) == 0)
		{
			Game [INDEX_PLAY].bFlip = 0;
		}
		else
		{
			Game [INDEX_PLAY].bFlip = 1;
		}
	}

	Game [INDEX_PLAY].bValid          = 1;
	Game [INDEX_PLAY].nGameNumber     = nN;
	Game [INDEX_PLAY].nGamePartner    = 0;
	Game [INDEX_PLAY].nRelation       = nRl;
	Game [INDEX_PLAY].bWhitesMove     = (cTurn == 'W');
	Game [INDEX_PLAY].bLastWhitesMove = Game [INDEX_PLAY].bWhitesMove;
	Game [INDEX_PLAY].bPlaying        = 0;
	Game [INDEX_PLAY].bIPlayWhite     = (stricmp(cwName, Vars.cWhoAmI) == 0);
	Game [INDEX_PLAY].bInitialMove    = (stricmp(cLastMove, ICS_INITIAL_MOVE_NAME) == 0);
	Game [INDEX_PLAY].nMoveNumber     = nMn;

	strcpy(Game [INDEX_PLAY].cHandle [INDEX_WHITE], cwName);
	strcpy(Game [INDEX_PLAY].cHandle [INDEX_BLACK], cbName);
	strcpy(Game [INDEX_PLAY].cResult, "");

	Game [INDEX_PLAY].nLastDoublePushFile               = nLP;
	Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_WHITE] = bWCK;
	Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_WHITE] = bWCQ;
	Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_BLACK] = bBCK;
	Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_BLACK] = bBCQ;
	Game [INDEX_PLAY].nHalfMoves                        = nHalf;

	Game [INDEX_PLAY].nInitialClock                = nIc;
	Game [INDEX_PLAY].nIncrementClock              = nIi;
	Game [INDEX_PLAY].nClockTimerEvent             = 0;
	Game [INDEX_PLAY].bTickClock                   = 0;
	Game [INDEX_PLAY].bFlagged [INDEX_WHITE]       = 0;
	Game [INDEX_PLAY].bFlagged [INDEX_BLACK]       = 0;
	Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc * 1000;
	Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc * 1000;
	Game [INDEX_PLAY].tTickStartTM.nSec            = 0;
	Game [INDEX_PLAY].tTickStartTM.nMs             = 0;
	Game [INDEX_PLAY].nIntendedTickLength          = 0;
	Game [INDEX_PLAY].bOnLagClock                  = 0;
	Game [INDEX_PLAY].nNextLagCheck                = 1000;
	Game [INDEX_PLAY].nLag [INDEX_WHITE]           = 0;
	Game [INDEX_PLAY].nLag [INDEX_BLACK]           = 0;

	LagCmd.tLastLagCommand.nSec = 0;
	LagCmd.tLastLagCommand.nMs  = 0;

	if(bNewGame)
	{
		Game [INDEX_PLAY].bChessGame = 0;
	}

	BOARD_LoadBoard(INDEX_PLAY, cS + 5, cVerboseMove);

	BOARD_GetHighlightFromLastMove(INDEX_PLAY, cVerboseMove, cLastMove);

	hdc = GetDC(Game [INDEX_PLAY].hwnd);

	if(bOrgFlip != Game [INDEX_PLAY].bFlip)
	{
		BOARD_CheckFlip(INDEX_PLAY);
	}

	if(Telnet.nTelnetState == WIN_MAXIMIZE)
	{
		wCoord [COORD_TELNET].s = WIN_SHOW;
		ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
	}

	if(bNewGame)
	{
		System.bPromoteCommand = 0;
		System.nPromoteWPiece  = 0;
		System.nPromoteBPiece  = 0;
		System.bPromoteKnight  = 0;

		Game [INDEX_PLAY].nGameType = System.nLastExamGameType;

		strcpy(Game [INDEX_PLAY].cLastMove,             ICS_INITIAL_MOVE_NAME);
		strcpy(Game [INDEX_PLAY].cVerboseMove,          ICS_INITIAL_MOVE_NAME);
		strcpy(Game [INDEX_PLAY].cTime4Move,            ICS_INITIAL_TIME_MOVE);
		strcpy(Game [INDEX_PLAY].cRating [INDEX_WHITE], "");
		strcpy(Game [INDEX_PLAY].cRating [INDEX_BLACK], "");
		strcpy(Game [INDEX_PLAY].cOrgResult,            "");

		Game [INDEX_PLAY].bSavedGame = 0;
		Game [INDEX_PLAY].nRated     = ICS_INITIAL_RATED;

		strcpy(Game [INDEX_PLAY].cGameType, ICS_INITIAL_GAMETYPE_STRING);

		Game [INDEX_PLAY].bIssuedEventCommand = 0;

		CLOCK_StartClocks(INDEX_PLAY, Game [INDEX_PLAY].hwnd);

		BOARD_LoadBuffer(INDEX_PLAY, INIT_BUFFER, INIT_BUFFER);

		BOARD_NullLastHighlight(INDEX_PLAY);
		BOARD_NullLastBoardBuffer(INDEX_PLAY);

		Premove.nPremoveCount  = 0;
		Premove.nPremoveHead   = 0;
		Premove.nPremoveTail   = 0;
		Premove.bLastTP        = 0;
		Premove.ptLastTP [0].x = -1;
		Premove.ptLastTP [0].y = -1;
		Premove.ptLastTP [1].x = -1;
		Premove.ptLastTP [1].y = -1;

		BOARD_DrawHandle(INDEX_PLAY, hdc);
		BOARD_DrawWhiteLag(INDEX_PLAY, hdc);
		BOARD_DrawBlackLag(INDEX_PLAY, hdc);
		BOARD_DrawWhiteClock(INDEX_PLAY, hdc);
		BOARD_DrawBlackClock(INDEX_PLAY, hdc);
		BOARD_DrawGameType(INDEX_PLAY, hdc);
		BOARD_DrawLastMove(INDEX_PLAY, hdc);
		BOARD_DrawResult(INDEX_PLAY, hdc);
		BOARD_DrawCoordinates(INDEX_PLAY, hdc);
		BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NEW_GAME);

		POSITION_FirstTime(INDEX_PLAY);

		sprintf(cGarbage, "%s %d\n", ICC_MOVELIST_COMMAND, nN);
		TOOLBOX_WriteICS(cGarbage);

		System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1;

		BUGHOUSE_FirstTime(INDEX_PLAY);

		SetWindowText(Game [INDEX_PLAY].hwnd, TOOLBOX_GetGameWindowTitle(INDEX_PLAY));

		if(wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
		{
			wCoord [COORD_PLAY].s = WIN_SHOW;
			ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);
		}

		wCoord [COORD_PLAY].s = WIN_SHOW;
		SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);

		TOOLBOX_ShowMoveButtons(INDEX_PLAY, 1);

		SetFocus(hwndWindow [HWND_TELNET_EDIT]);

		Icc.bFindICCPartner = 0;
		strcpy(Icc.cICCPartner, "");
	}
	else
	{
		strcpy(Game [INDEX_PLAY].cLastMove,    cLastMove);
		strcpy(Game [INDEX_PLAY].cVerboseMove, cVerboseMove);
		strcpy(Game [INDEX_PLAY].cTime4Move,   cTime4Move);

		CLOCK_StartClocks(INDEX_PLAY, Game [INDEX_PLAY].hwnd);

		BOARD_DrawHandle(INDEX_PLAY, hdc);
		BOARD_DrawWhiteLag(INDEX_PLAY, hdc);
		BOARD_DrawBlackLag(INDEX_PLAY, hdc);
		BOARD_DrawWhiteClock(INDEX_PLAY, hdc);
		BOARD_DrawBlackClock(INDEX_PLAY, hdc);
		BOARD_DrawGameType(INDEX_PLAY, hdc);
		BOARD_DrawLastMove(INDEX_PLAY, hdc);
		BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_BOARD);

		if(wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
		{
			wCoord [COORD_PLAY].s = WIN_SHOW;
			ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);

			SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);
			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
		}
	}

	ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);
}

int ICC_ParsePiece(char *cS)
{
	HDC hdc;
	char *cP1, *cP2;
	char cBugs1 [40], cBugs2 [40];
	int nN, nI;

	sscanf(cS, "<b1> game %d white %s black %s", &nN, &cBugs1, &cBugs2);

	for(nI = 0 ; nI < MAX_GAME ; nI++)
	{
		if(Game [nI].bValid)
		{
			if(Game [nI].nGameNumber == nN)
			{
				cP1 = strchr(cBugs1, ']');
				cP2 = strchr(cBugs2, ']');

				if((cP1 != NULL) && (cP2 != NULL))
				{
					*cP1 = NULL_CHAR;
					*cP2 = NULL_CHAR;

					BOARD_LoadBuffer(nI, cBugs1 + 1, cBugs2 + 1);

					if(Game [nI].bValid)
					{
						if((nI == INDEX_PLAY) || (TOOLBOX_DisplayActualBoard(nI)))
						{
							hdc = GetDC(Game [nI].hwnd);
							if(nI == INDEX_PLAY)
							{
								BOARD_DrawBoard(nI, Game [nI].hwnd, hdc, DRAW_STATE_BUFFER);
							}
							else
							{
								BOARD_DrawBoard1(nI, Game [nI].hwnd, hdc, DRAW_STATE_BUFFER);
							}
							ReleaseDC(Game [nI].hwnd, hdc);
						}

						//
						// update move list piece buffer
						//
						// icc sends the piece buffer update after the move is made
						// (after style 12 string) thus the piece buffer update
						// has to be made for the previous move.
						//
						POSITION_ICC_SaveGameBuffer(nI);
					}
				}

				if(nI == INDEX_PLAY)
				{
					STATE_PlayPieceBufferUpdate();
				}
				return 1;
			}
		}
	}
	return 0;
}

int ICC_ColorIndex(char *cS)
{
	HDC hdc;
	int nI, nJ;
	char cTmp [4096], cTmp1 [50], cTmp2 [50], *cP;

	if(Telnet.bSkipDisplayLine)
	{
		//--** indiantachyon is an abuser **--
		//--** indiantachyon is a computer **--
		if(! strncmp(cS, "--** ", 5))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bSkipDisplayLine     = 1;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}
		else
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bSkipDisplayLine     = 0;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}
	}

	Telnet.bDisplayLine = 1;

	if(Telnet.bLastIsChallenge)
	{
		if(User.bNoPlayAbuser)
		{
			//--** indiantachyon is an abuser **--
			if(! strncmp(cS, "--** ", 5))
			{
				if(strstr(cS, " is an abuser"))
				{
					Telnet.clrCurrent       = clrColor [CLR_TELNET_NORMAL];
					Telnet.nCurrentSound    = SOUND_NONE;
					Telnet.bLastIsChallenge = 0;
					TOOLBOX_WriteICS(ICC_DECLINE_COMMAND);
					return 0;
				}

				if(User.bNoPlayComputer)
				{
					//--** indiantachyon is a computer **--
					if(strstr(cS, " is a computer"))
					{
						Telnet.clrCurrent       = clrColor [CLR_TELNET_NORMAL];
						Telnet.nCurrentSound    = SOUND_NONE;
						Telnet.bLastIsChallenge = 0;
						TOOLBOX_WriteICS(ICC_DECLINE_COMMAND);
						return 0;
					}
				}
			}
		}
		else if(User.bNoPlayComputer)
		{
			//--** indiantachyon is a computer **--
			if(! strncmp(cS, "--** ", 5))
			{
				if(strstr(cS, " is a computer"))
				{
					Telnet.clrCurrent       = clrColor [CLR_TELNET_NORMAL];
					Telnet.nCurrentSound    = SOUND_NONE;
					Telnet.bLastIsChallenge = 0;
					TOOLBOX_WriteICS(ICC_DECLINE_COMMAND);
					return 0;
				}
			}
		}

		Telnet.bLastIsChallenge = 0;
	}

	if(cS [0] == '\\')
	{
		Telnet.clrCurrent    = Telnet.clrLastLine;
		Telnet.nCurrentSound = SOUND_NONE;
		return 0;
	}

	if(cS [0] == ' ')
	{
		//
		//   **ANNOUNCEMENT** from ROBOadmin: "help event2" for a list of all the
		//\   upcoming events on chessclub.com!
		//
		if(! strncmp(cS, ICC_ANNOUNCEMENT, 20))
		{
			if((User.bSuppressAnnouncement) || (Silence.bSilenceApply && Silence.bSilenceAnnouncement) ||
					(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayAnnouncement))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
			Telnet.clrCurrent = clrColor [CLR_TELNET_ANNOUNCEMENT];
		}
		else
		{
			Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL];
		}
		Telnet.nCurrentSound = SOUND_NONE;
		return 0;
	}

	if(cS [0] == ':')
	{
		Telnet.clrCurrent    = ICC_ColonColor(cS);
		Telnet.nCurrentSound = SOUND_NONE;
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_SAY))
	{
		strcpy(cTmp, TOOLBOX_GetICSHandle(cS, FALSE));

		if((Silence.bSilenceApply && Silence.bSilenceSay) ||
				(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlaySay))
		{
			if(SILENCE_Tell(cTmp, cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Say(cTmp, cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_SAY];
		Telnet.nCurrentSound = SOUND_SAY;

		strcpy(Vars.cLastSay, cS);
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_SHOUT))
	{
		if((Silence.bSilenceApply && Silence.bSilenceShout) ||
				(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayShout))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(5, TOOLBOX_GetICSHandle(cS, FALSE)))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(User.bSuppressCompShout)
		{
			if(TOOLBOX_IsComputerHandle(cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_SHOUT];
		Telnet.nCurrentSound = SOUND_SHOUT;
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_KIBITZ))
	{
		if(User.bSuppressCompKibitz)
		{
			if(TOOLBOX_IsComputerHandle(cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if((Silence.bSilenceApply && Silence.bSilenceKibitz) ||
				(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayKibitz))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}

		strcpy(cTmp, TOOLBOX_GetICSHandle(cS, FALSE));

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(3, cTmp))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_KIBITZ];
		Telnet.nCurrentSound = SOUND_KIBITZ;

		strcpy(Vars.cLastKib, cTmp);
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_WHISPER))
	{
		if(User.bSuppressCompWhisper)
		{
			if(TOOLBOX_IsComputerHandle(cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if((Silence.bSilenceApply && Silence.bSilenceWhisper) ||
				(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayWhisper))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}

		strcpy(cTmp, TOOLBOX_GetICSHandle(cS, FALSE));

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(4, cTmp))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_WHISPER];
		Telnet.nCurrentSound = SOUND_WHISPER;

		strcpy(Vars.cLastWhisper, cTmp);
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_SSHOUT))
	{

		if((Silence.bSilenceApply && Silence.bSilenceCShout) ||
				(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayCShout))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(5, TOOLBOX_GetICSHandle(cS, FALSE)))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(User.bSuppressCompShout)
		{
			if(TOOLBOX_IsComputerHandle(cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_CSHOUT];
		Telnet.nCurrentSound = SOUND_CSHOUT;
		return 0;
	}

	if(TELNET_IsChannel(cS, &nI))
	{

		if((Silence.bSilenceApply && Silence.bSilenceChTell) ||
				(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayChTell))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(6, TOOLBOX_GetICSHandle(cS, FALSE)))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(nI)
		{
			if(User.bSuppressCompChTell)
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_TELL))
	{
		strcpy(cTmp, TOOLBOX_GetICSHandle(cS, FALSE));

		if((Silence.bSilenceApply && Silence.bSilencePeTell) ||
				(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayPeTell))
		{
			if(SILENCE_Tell(cTmp, cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Tell(cTmp, cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(User.bIamAGuest)
		{
			if(User.bSuppressROBOWelcome)
			{
				if(! strncmp(cS, ICC_HEARED_ROBO_WELCOME, 27))
				{
					Telnet.bDisplayLine         = 0;
					Telnet.bDisplayContinueLine = 0;
					Telnet.bTelnetSkipLine      = 1;
					return 0;
				}
			}
		}

		if(User.bSuppressCompTell)
		{
			if(TOOLBOX_IsComputerHandle(cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_TELL];
		Telnet.nCurrentSound = SOUND_TELL;

		strcpy(Vars.cLastTell, cTmp);
		F9KEY_Add(Vars.cLastTell);

		//
		// ICC Partnership request/partner dumping/partner switching
		//
		// guest414(U) tells you: [automatic message] I chose you as my bughouse partner.
		// guest414(U) tells you: [automatic message] I'm no longer your partner.
		// guest414(U) tells you: [automatic message] I chose another partner.

		cP = strchr(cS, ':');
		if(cP != NULL)
		{
			if(stricmp(cP + 2, ICC_PARTNER_REQUEST) == 0)
			{
				Telnet.nCurrentSound = SOUND_PARTNER;
			}
			else if((stricmp(cP + 2, ICC_PARTNER_LEFT1) == 0) ||
					(stricmp(cP + 2, ICC_PARTNER_LEFT2) == 0))
			{
				strcpy(Vars.cPartner, "");
				strcpy(Vars.cPartnerTell, "");
				Game [INDEX_PLAY].nGamePartner = 0;

				TOOLBOX_SetTelnetCaption();
				BUTTON_Refresh();
			}
		}

		if(User.bEarPartner || User.bShowPtell)
		{
			if(strlen(Vars.cPartnerTell1) > 0)
			{
				strcpy(cTmp, cS);
				strupr(cTmp);

				if(TELNET_MatchExp(cTmp, Vars.cPartnerTell1) ||
						TELNET_MatchExp(cTmp, Vars.cPartnerTell2))
				{
					if(User.bShowPtell)
					{
						hdc = GetDC(Game [INDEX_PLAY].hwnd);
						BOARD_DrawPtell(INDEX_PLAY, hdc, cS);
						ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);
					}
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 1;
		}
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_PTELL))
	{
		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Ptell(Vars.cPartner, cS))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}

			if(strlen(Vars.cPartner) > 0)
			{
				if(CENSOR_Partner(Vars.cPartner))
				{
					TOOLBOX_WriteICS(ICC_NO_PARTNER_COMMAND);
				}
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_PTELL];
		Telnet.nCurrentSound = SOUND_PTELL;

		if(strlen(Vars.cPartner) > 0)
		{
			F9KEY_Add(Vars.cPartner);
			strcpy(Vars.cLastTell, Vars.cPartner);
		}

		if(User.bShowPtell)
		{
			hdc = GetDC(Game [INDEX_PLAY].hwnd);
			BOARD_DrawPtell(INDEX_PLAY, hdc, cS);
			ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);
		}
		return 1;
	}

	Telnet.bLastIsChallenge = 0;

	if(TELNET_MatchExp(cS, ICC_GOT_CHALLENGED))
	{
		//Challenge: heheman (----) cocoman (----) unrated crazyhouse 10 0.
		if(Censor [Login.nLoginType].bCensor)
		{
			char cWhite [50], cBlack [50];

			sscanf(cS, "%s %s %s %s", &cTmp, &cWhite, &cTmp, &cBlack);

			if(stricmp(cWhite, Vars.cWhoAmI) == 0)
			{
				if(CENSOR_Other(7, cBlack))
				{
					Telnet.bDisplayLine         = 0;
					Telnet.bDisplayContinueLine = 0;
					Telnet.bSkipDisplayLine     = 1;
					Telnet.bTelnetSkipLine      = 1;
					return 0;
				}
			}
			else
			{
				if(CENSOR_Other(7, cWhite))
				{
					Telnet.bDisplayLine         = 0;
					Telnet.bDisplayContinueLine = 0;
					Telnet.bSkipDisplayLine     = 1;
					Telnet.bTelnetSkipLine      = 1;
					return 0;
				}
			}
		}

		Telnet.bSkipDisplayLine = 0;
		Telnet.clrCurrent       = clrColor [CLR_TELNET_CHALLENGE];
		Telnet.nCurrentSound    = SOUND_CHALLENGE;
		Telnet.bLastIsChallenge = 1;
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_IT))
	{

		if((Silence.bSilenceApply && Silence.bSilenceShout) ||
				(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayShout))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			return 0;
		}

		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(5, TOOLBOX_GetICSHandle(cS + 4, FALSE)))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_SHOUT];
		Telnet.nCurrentSound = SOUND_SHOUT;
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_NOTIFICATION))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NOTIFICATION];
		Telnet.nCurrentSound = SOUND_NOTIFICATION;
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_ARRIVAL))
	{
		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(8, TOOLBOX_GetICSHandle(cS, FALSE)))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_ARRIVAL];
		Telnet.nCurrentSound = SOUND_ARRIVAL;
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_HEARED_DEPARTURE))
	{
		if(Censor [Login.nLoginType].bCensor)
		{
			if(CENSOR_Other(8, TOOLBOX_GetICSHandle(cS, FALSE)))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		Telnet.clrCurrent    = clrColor [CLR_TELNET_DEPARTURE];
		Telnet.nCurrentSound = SOUND_DEPARTURE;
		return 0;
	}

	if(! strncmp(cS, FICS_HEARED_GAME_NOTIFY, 19))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_GAME_NOTIFY];
		Telnet.nCurrentSound = SOUND_GAME_NOTIFY;

		if(User.bAutoObserveGNotify)
		{

			//Game notification: VABORIS (2422) vs. venomous (2401) rated crazyhouse 1 0: Game 4
			sscanf(cS, "%s %s %s %s %s %s", &cTmp, &cTmp, &cTmp1, &cTmp, &cTmp, &cTmp2);

			if((stricmp(cTmp1, Vars.cPartner) == 0) ||
					(stricmp(cTmp1, Vars.cFollow) == 0) ||
					(stricmp(cTmp2, Vars.cPartner) == 0) ||
					(stricmp(cTmp2, Vars.cFollow) == 0))
			{

				// do nothing
			}
			else
			{

				// observe but check to make sure neither of the handle is already being observed
				nJ = 0;
				for(nI = 0 ; nI < MAX_GAME ; nI++)
				{
					if(Game [nI].bValid)
					{
						if(Game [nI].nGameNumber > 0)
						{
							if(Game [nI].bPlaying)
							{
								if((stricmp(Game [nI].cHandle [INDEX_WHITE], cTmp1) == 0) ||
										(stricmp(Game [nI].cHandle [INDEX_BLACK], cTmp1) == 0) ||
										(stricmp(Game [nI].cHandle [INDEX_WHITE], cTmp2) == 0) ||
										(stricmp(Game [nI].cHandle [INDEX_BLACK], cTmp2) == 0))
								{
									nJ = 1;
									break;
								}
							}
						}
					}
				}

				if(nJ == 0)
				{
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{

							// playing
							TOOLBOX_WriteSystem("No Auto Game Notification Observe Due to Playing\n");
						}
						else
						{

							// examing
							TOOLBOX_WriteSystem("No Auto Game Notification Observe Due to Examing\n");
						}
					}
					else
					{

						// issue the observe command
						TOOLBOX_WriteICS(ICC_OBSERVE_COMMAND);
						TOOLBOX_WriteICS(cTmp1);
						TOOLBOX_WriteICS("\n");
					}
				}
			}
		}
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_ABORT_REQUEST))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
		Telnet.nCurrentSound = SOUND_ABORT_REQUEST;
		return 0;
	}

	if(User.bSuppressIgnoreFormula)
	{
		if(! strncmp(cS, ICC_HEARED_IGNORE_FORMULA, 20))
		{
			Telnet.bDisplayLine         = 0;
			Telnet.bDisplayContinueLine = 0;
			Telnet.bTelnetSkipLine      = 1;
			return 0;
		}
	}

//
//  "You no longer have a bughouse partner." // ok new
//  "Your partner has disconnected." // ok
//  "Your partner has just chosen a new partner." // no more
//  "Your partner has unset his/her partner variable." // no more
//  "Your partner has ended partnership." // ok
//  "Your partner has accepted a partnership with" //ok
//  "Your partner has left." // <-- ICC
//
	if(TELNET_MatchExp(cS, "You no longer have a bughouse partner")        ||
			TELNET_MatchExp(cS, "Your partner has ended partnership")           ||
			TELNET_MatchExp(cS, "Your partner has accepted a partnership with") ||
			TELNET_MatchExp(cS, "Your partner has disconnected")                ||
			TELNET_MatchExp(cS, "Your partner has left"))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
		Telnet.nCurrentSound = SOUND_NONE;

		strcpy(Vars.cPartner, "");
		strcpy(Vars.cPartnerTell, "");
		Game [INDEX_PLAY].nGamePartner = 0;

		TOOLBOX_SetTelnetCaption();

		BUTTON_Refresh();
		return 0;
	}

	if(TELNET_MatchExp(cS, ICC_GET_MEXED))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
		Telnet.nCurrentSound = SOUND_NONE;

		TOOLBOX_WriteICS(ICC_REFRESH_COMMAND1);

		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			if(IsWindow(Game [nI].hwnd))
			{
				Game [nI].bFirstGame  = 0;
				Game [nI].nGameNumber = 0;
				Game [nI].bPlaying    = 0;
				Game [nI].bTickClock  = 1;

				(void) CLOCK_StopClockTimer(nI);

				SendMessage(Game [nI].hwnd, WM_CLOSE, 0, 0);
			}
		}
		return 0;
	}

//
// Your opponent has added 300 seconds to your clock.
// 200 seconds were added to your opponents clock
//
	if(TELNET_MatchExp(cS, ICC_GOT_MORETIME_1) ||
			TELNET_MatchExp(cS, ICC_GOT_MORETIME_2))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
		Telnet.nCurrentSound = SOUND_NONE;

		TOOLBOX_WriteICS(ICC_REFRESH_COMMAND1);
		return 0;
	}

//
// Opponent has timeseal; checking if (s)he's lagging.
// Flag still pending from previous request; opponent is lagging.
// Opponent is not out of time, wait for server autoflag.
//
//  if (TELNET_MatchExp (cS, ICC_OPP_LAGGING_1) ||
//      TELNET_MatchExp (cS, ICC_OPP_LAGGING_2) ||
//      TELNET_MatchExp (cS, ICC_OPP_LAGGING_3))
//      {
//      Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
//      Telnet.nCurrentSound = SOUND_NONE;
//
//      Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 0;
//      Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 0;
//      return 0;
//      }

	if(! strncmp(cS, ICC_TIME_LEFT_0, 54))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
		Telnet.nCurrentSound = SOUND_NONE;
		return 0;
	}

	if(strstr(cS, ICC_TIME_LEFT_1) ||
			strstr(cS, ICC_TIME_LEFT_2) ||
			strstr(cS, ICC_TIME_LEFT_3) ||
			strstr(cS, ICC_TIME_LEFT_4))
	{
		Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
		Telnet.nCurrentSound = SOUND_NONE;

		Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 0;
		Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 0;
		return 0;
	}

	if(Censor [Login.nLoginType].bCensor)
	{
		if(Telnet.bTellTold)
		{
			if(! strncmp(cS, Telnet.cTellTold, Telnet.nTellTold))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bTellTold            = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(Telnet.bPtellTold)
		{
			if(! strncmp(cS, Telnet.cPtellTold, Telnet.nPtellTold))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bPtellTold           = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(Telnet.bSayTold)
		{
			if(! strncmp(cS, Telnet.cSayTold, Telnet.nSayTold))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bSayTold             = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}

		if(Telnet.bPartnerTold)
		{
			if(! strncmp(cS, Telnet.cPartnerTold, Telnet.nPartnerTold))
			{
				Telnet.bDisplayLine         = 0;
				Telnet.bDisplayContinueLine = 0;
				Telnet.bPartnerTold         = 0;
				Telnet.bTelnetSkipLine      = 1;
				return 0;
			}
		}
	}

	Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL];
	Telnet.nCurrentSound = SOUND_NONE;
	return 0;
}

COLORREF ICC_ColonColor(char *cS)
{
	COLORREF clr;
	int nL, nI;
	char cTmp [10];

	clr = clrColor [CLR_TELNET_COLON];

	if(! strncmp(cS, ":Notification: ", 15))
	{
		clr = clrColor [CLR_TELNET_COLON_NOTIFY];
	}
	else if((cS [0] == ':') && (cS [1] == NULL_CHAR))
	{
		clr = clrColor [CLR_TELNET_COLON_MAMER];
	}
	else
	{
		nL = 0;
		for(nI = 0 ; nI < MAX_MAMER_STRING ; nI++)
		{
			if(! strncmp(cS, MamerString [nI], MamerStringSize [nI]))
			{
				nL  = 1;
				clr = clrColor [CLR_TELNET_COLON_MAMER];
				break;
			}
		}

		if(! nL)
		{
			if(strstr(cS, " is observing tourney #")                 ||
					strstr(cS, " has been added to tourney #")            ||
					strstr(cS, " are observing tourney #")                ||
					strstr(cS, " has been invited to tourney #")          ||
					strstr(cS, " the king of KOTH #")                     ||
					strstr(cS, " the queen of KOTH #")                    ||
					strstr(cS, " managed the tourney.")                   ||
					strstr(cS, " is managing the tourney.")               ||
					strstr(cS, " variable has been set to")               ||
					strstr(cS, " has been made the manager of tourney #") ||
					strstr(cS, " has been forfeited from tourney #"))
			{
				clr = clrColor [CLR_TELNET_COLON_MAMER];
			}
			else if((cS [0] == ':') &&
					(cS [1] >= '0') &&
					(cS [1] <= '9') &&
					(cS [2] == '.'))
			{
				clr = clrColor [CLR_TELNET_COLON_MAMER];
			}
			else if((cS [0] == ':') &&
					(cS [1] >= '0') &&
					(cS [1] <= '9') &&
					(cS [2] >= '0') &&
					(cS [2] <= '9') &&
					(cS [3] == '.'))
			{
				clr = clrColor [CLR_TELNET_COLON_MAMER];
			}
			else if(TOOLBOX_IsQChannelTell(cS, cTmp))
			{
				clr = clrColor [CLR_TELNET_COLON_256];

				nL = strlen(cTmp);
				if(nL == 2)
				{
					//:knighttour(*)(C1): xxx
					if(cTmp [0] == 'C' && isdigit(cTmp [1]))
					{
						clr = clrColor [CLR_TELNET_COLON_C2];
					}
					else
						//:knighttour(*)(T1): xxx
						if(cTmp [0] == 'T' && isdigit(cTmp [1]))
						{
							clr = clrColor [CLR_TELNET_COLON_T3];
						}
				}
				else if(nL == 3)
				{
					//:knighttour(*)(C12): xxx
					if(cTmp [0] == 'C' && isdigit(cTmp [1]) && isdigit(cTmp [2]))
					{
						clr = clrColor [CLR_TELNET_COLON_C2];
					}
					else
						//:knighttour(*)(T12): xxx
						if(cTmp [0] == 'T' && isdigit(cTmp [1]) && isdigit(cTmp [2]))
						{
							clr = clrColor [CLR_TELNET_COLON_T3];
						}
				}
				else if(nL == 4)
				{
					//:knighttour(*)(C123): xxx
					if(cTmp [0] == 'C' && isdigit(cTmp [1]) && isdigit(cTmp [2]) && isdigit(cTmp [3]))
					{
						clr = clrColor [CLR_TELNET_COLON_C2];
					}
					else
						//:knighttour(*)(T123): xxx
						if(cTmp [0] == 'T' && isdigit(cTmp [1]) && isdigit(cTmp [2]) && isdigit(cTmp [3]))
						{
							clr = clrColor [CLR_TELNET_COLON_T3];
						}
				}
			}
			else
			{
				nL = strlen(cS);
				for(nI = 0 ; nI < nL ; nI++)
				{
					//:knighttour(TM) t-shouts: xxx
					if(cS [nI     ] == ' ' &&
							cS [nI +  1] == 't' &&
							cS [nI +  2] == '-' &&
							cS [nI +  3] == 's' &&
							cS [nI +  4] == 'h' &&
							cS [nI +  5] == 'o' &&
							cS [nI +  6] == 'u' &&
							cS [nI +  7] == 't' &&
							cS [nI +  8] == 's' &&
							cS [nI +  9] == ':' &&
							cS [nI + 10] == ' ')
					{
						clr = clrColor [CLR_TELNET_COLON_TSHOUTS];
						break;
					}
				}
			}
		}
	}
	return clr;
}
