#include "thief.h"

void GAMESOUND_Init(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
	{
		GameSounds.bPlay [nI] = 0 ;
		GameSounds.bBeep [nI] = 0 ;
		strcpy(GameSounds.cName [nI], "") ;
		GameSounds.pData [nI] = NULL ;
	}

	strcpy(GameSounds.cName [GAME_SOUND_PLAY_START],      "FITEBELL.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_MOVE],       "MOVE.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_MOVE_SMART], "MOVE.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_OVER],       "ASTRING.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_CHECK],      "CHECK.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_OBS_MOVE2],       "MOVE.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_OBS_OVER2],       "ASTRING.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_I_WON],           "IWON.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_I_LOST],          "ILOST.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_I_DREW],          "IDREW.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_I_ABORT],         "IABORTED.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_WON],     "PARTNERWON.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_LOST],    "PARTNERLOST.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_ABORT],   "PARTNERABORTED.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_DREW],    "GAMEDRAWN.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_EXAM_MOVE],       "EXAMMOVE.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_EXAM_CAPTURE],    "EXAMCAP.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_EXAM_CASTLE],     "EXAMMOVE.WAV") ;
	strcpy(GameSounds.cName [GAME_SOUND_GAME_ABORT],      "GAMEABORTED.WAV") ;
}

void GAMESOUND_Load(void)
{
	int nI ;
	FILE *Fv ;
	struct stat stStat ;

	GAMESOUND_Destroy() ;

	for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
	{
		if(GameSounds.bBeep [nI])
		{
			GameSounds.bPlay [nI] = 1 ;
		}
		else if(strlen(GameSounds.cName [nI]) > 0)
		{
			Fv = fopen(TOOLBOX_GetFullWaveFilename(GameSounds.cName [nI]), "rb") ;
			if(Fv != NULL)
			{
				if(fstat(fileno(Fv), &stStat) >= 0)
				{
					GameSounds.pData [nI] = malloc(stStat.st_size) ;
					if(fread(GameSounds.pData [nI], stStat.st_size, 1, Fv) >= 1)
					{
						GameSounds.bPlay [nI] = 1 ;
					}
				}
				fclose(Fv) ;
			}
		}
	}
}

void GAMESOUND_Play(int nI)
{
	if(User.bSound && User.bGameSound)
	{
		if(GameSounds.bPlay [nI])
		{
			if(GameSounds.bBeep [nI])
			{
				TOOLBOX_Beep() ;
			}
			else
			{
				PlaySound((char *) GameSounds.pData [nI], NULL, SND_MEMORY | SND_ASYNC) ;
			}
		}
	}
}

void GAMESOUND_Destroy(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
	{
		GameSounds.bPlay [nI] = 0 ;
		if(GameSounds.pData [nI])
		{
			free(GameSounds.pData [nI]) ;
		}
		GameSounds.pData [nI] = NULL ;
	}
}

int GAMESOUND_PlayBoard(char *cS)
{
	char *cQ ;
	char cTmp [255], cMatch [255] ;
	int nL ;

	if(! User.bGameSound)
	{
		return 0 ;
	}

	cQ = strchr(cS, '}') ;
	if(cQ == NULL)
	{
		return 0 ;
	}

	strcpy(cTmp, cQ) ;
	nL = strlen(cTmp) ;
	if(cTmp [nL - 1] == '\n')
	{
		cTmp [nL - 1] = NULL_CHAR ;
	}

	//discard all lines from partner boards here:
	// kunde's partner forfeits on time} 0-1
	// kunde's partner lost} 0-1
	if(strstr(cS, "'s partner"))
	{
		return 1;
	}


	if((strcmp(cTmp, "} 1-0") == 0) ||
			(strcmp(cTmp, "} 0-1") == 0))
	{

		//
		// double check game result
		//
		// LOST STRING:
		// Grishezz checkmated} 0-1
		// RubberDuck resigns} 1-0
		// kunde forfeits on time} 0-1
		// kunde lost} 0-1
		// Romana67's partner checkmated} 0-1
		// Lethal-Predator's partner resigns} 1-0
		// kunde's partner forfeits on time} 0-1
		// kunde's partner lost} 0-1
		//
		// WON STRING:
		// sgs won} 0-1
		// sgs's partner won} 0-1
		//

		// Grishezz checkmated} 0-1
		sprintf(cMatch, "%s checkmated", Vars.cWhoAmI) ;
		if(strstr(_strlwr(cS), cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s checkmated", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// RubberDuck resigns} 1-0
		sprintf(cMatch, "%s resigns", Vars.cWhoAmI) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s resigns", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde forfeits on time} 0-1
		sprintf(cMatch, "%s forfeits on time", Vars.cWhoAmI) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s forfeits on time", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde lost} 0-1
		sprintf(cMatch, "%s lost", Vars.cWhoAmI) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s lost", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// Romana67's partner checkmated} 0-1
		sprintf(cMatch, "%s's partner checkmated", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner checkmated", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// Lethal-Predator's partner resigns} 1-0
		sprintf(cMatch, "%s's partner resigns", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner resigns", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde's partner forfeits on time} 0-1
		sprintf(cMatch, "%s's partner forfeits on time", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner forfeits on time", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde's partner lost} 0-1
		sprintf(cMatch, "%s's partner lost", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner lost", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// sgs won} 0-1
		sprintf(cMatch, "%s won", Vars.cWhoAmI) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s won", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// sgs's partner won} 0-1
		sprintf(cMatch, "%s's partner won", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_WON])
			{
				GAMESOUND_Play(GAME_SOUND_I_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner won", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_I_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}
	}
	else if(strcmp(cTmp, "} 1/2-1/2") == 0)
	{
		sprintf(cMatch, "(%s vs. %s)", Vars.cWhoAmI, Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_DREW])
			{
				GAMESOUND_Play(GAME_SOUND_I_DREW) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "(%s vs. %s)", Vars.cOpponent, Vars.cWhoAmI) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_I_DREW])
			{
				GAMESOUND_Play(GAME_SOUND_I_DREW) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}
	}
	else if((strcmp(cTmp, "Game adjourned by mutual agreement} *") == 0) ||
			(strcmp(cTmp, "lost connection; game adjourned} *") == 0))
	{
		if(GameSounds.bPlay [GAME_SOUND_PLAY_OVER])
		{
			GAMESOUND_Play(GAME_SOUND_PLAY_OVER) ;
			return 1 ;
		}
		else
		{
			return 0 ;
		}
	}
	else if(strcmp(cTmp, "} *") == 0)
	{
		// fix for game end sound events in 1.24
		//own board aborted:
		//{Game 267 (ThiefTest vs. bugtestII) Game aborted on move 1} *
		// we had white
		sprintf(cMatch, "(%s vs. %s) Game aborted", Vars.cWhoAmI, Vars.cOpponent) ;
		if(strstr(_strlwr(cS), _strlwr(cMatch)))
		{
			if(Game [INDEX_PLAY].nAbortStatus == 4)
			{
				if(GameSounds.bPlay [GAME_SOUND_GAME_ABORT])
				{
					GAMESOUND_Play(GAME_SOUND_GAME_ABORT) ;
					return 1 ;
				}
			}
			if(GameSounds.bPlay [GAME_SOUND_I_ABORT])
			{
				GAMESOUND_Play(GAME_SOUND_I_ABORT) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}
		// we had black
		sprintf(cMatch, "(%s vs. %s) Game aborted", Vars.cOpponent, Vars.cWhoAmI) ;
		if(strstr(_strlwr(cS), _strlwr(cMatch)))
		{
			if(Game [INDEX_PLAY].nAbortStatus == 4)
			{
				if(GameSounds.bPlay [GAME_SOUND_GAME_ABORT])
				{
					GAMESOUND_Play(GAME_SOUND_GAME_ABORT) ;
					return 1 ;
				}
			}
			if(GameSounds.bPlay [GAME_SOUND_I_ABORT])
			{
				GAMESOUND_Play(GAME_SOUND_I_ABORT) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}
		//discard all lines from partner boards here:
		//{Game 25 (xxx vs. yyy) Partners' game aborted} *
		if(strstr(_strlwr(cS), "partners' game aborted"))
		{
			return 1;
		}
	}
	return 0 ;
}

int GAMESOUND_PartnerBoard(char *cS)
{
	char *cQ ;
	char cTmp [255], cMatch [255] ;
	int  nL ;

	if(! User.bGameSound)
	{
		return 0 ;
	}

	cQ = strchr(cS, '}') ;
	if(cQ == NULL)
	{
		return 0 ;
	}

	strcpy(cTmp, cQ) ;

	nL = strlen(cTmp) ;
	if(cTmp [nL - 1] == '\n')
	{
		cTmp [nL - 1] = NULL_CHAR ;
	}

	//discard all lines from partner boards here:
	// kunde's partner forfeits on time} 0-1
	// kunde's partner lost} 0-1
	if(strstr(cS, "'s partner"))
	{
		return 1;
	}

	if((strcmp(cTmp, "} 1-0") == 0) ||
			(strcmp(cTmp, "} 0-1") == 0))
	{

		//
		// double check game result
		//
		// LOST STRING:
		// Grishezz checkmated} 0-1
		// RubberDuck resigns} 1-0
		// kunde forfeits on time} 0-1
		// kunde lost} 0-1
		// Romana67's partner checkmated} 0-1
		// Lethal-Predator's partner resigns} 1-0
		// kunde's partner forfeits on time} 0-1
		// kunde's partner lost} 0-1
		//
		// WON STRING:
		// sgs won} 0-1
		// sgs's partner won} 0-1
		//

		// Grishezz checkmated} 0-1
		sprintf(cMatch, "%s checkmated", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s checkmated", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// RubberDuck resigns} 1-0
		sprintf(cMatch, "%s resigns", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s resigns", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde forfeits on time} 0-1
		sprintf(cMatch, "%s forfeits on time", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s forfeits on time", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde lost} 0-1
		sprintf(cMatch, "%s lost", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s lost", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// Romana67's partner checkmated} 0-1
		sprintf(cMatch, "%s's partner checkmated", Vars.cWhoAmI) ;
		if(strstr(_strlwr(cS), _strlwr(cMatch)))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner checkmated", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// Lethal-Predator's partner resigns} 1-0
		sprintf(cMatch, "%s's partner resigns", Vars.cWhoAmI) ;
		if(strstr(_strlwr(cS), _strlwr(cMatch)))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner resigns", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde's partner forfeits on time} 0-1
		sprintf(cMatch, "%s's partner forfeits on time", Vars.cWhoAmI) ;
		if(strstr(_strlwr(cS), _strlwr(cMatch)))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner forfeits on time", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// kunde's partner lost} 0-1
		sprintf(cMatch, "%s's partner lost", Vars.cWhoAmI) ;
		if(strstr(_strlwr(cS), _strlwr(cMatch)))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner lost", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// sgs won} 0-1
		sprintf(cMatch, "%s won", Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s won", Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		// sgs's partner won} 0-1
		sprintf(cMatch, "%s's partner won", Vars.cWhoAmI) ;
		if(strstr(_strlwr(cS), _strlwr(cMatch)))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_WON])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_WON) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "%s's partner won", Vars.cOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_LOST])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_LOST) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}
	}
	else if(strcmp(cTmp, "} 1/2-1/2") == 0)
	{
		sprintf(cMatch, "(%s vs. %s)", Vars.cPartner, Vars.cPartnerOpponent) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_DREW])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_DREW) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}

		sprintf(cMatch, "(%s vs. %s)", Vars.cPartnerOpponent, Vars.cPartner) ;
		if(strstr(cS, cMatch))
		{
			if(GameSounds.bPlay [GAME_SOUND_PARTNER_DREW])
			{
				GAMESOUND_Play(GAME_SOUND_PARTNER_DREW) ;
				return 1 ;
			}
			else
			{
				return 0 ;
			}
		}
	}
	else
		// fix for game end sound events in 1.24
		if(strcmp(cTmp, "} *") == 0)
		{
			sprintf(cMatch, "(%s vs. %s) Game aborted on move", Vars.cPartner, Vars.cPartnerOpponent) ;
			if(strstr(cS, cMatch))
			{
				if(GameSounds.bPlay [GAME_SOUND_PARTNER_ABORT])
				{
					GAMESOUND_Play(GAME_SOUND_PARTNER_ABORT) ;
					return 1 ;
				}
				else
				{
					return 0 ;
				}
			}

			sprintf(cMatch, "(%s vs. %s) Game aborted on move", Vars.cPartnerOpponent, Vars.cPartner) ;
			if(strstr(cS, cMatch))
			{
				if(GameSounds.bPlay [GAME_SOUND_PARTNER_ABORT])
				{
					GAMESOUND_Play(GAME_SOUND_PARTNER_ABORT) ;
					return 1 ;
				}
				else
				{
					return 0 ;
				}
			}
			//discard all lines from partner boards here:
			//{Game 25 (xxx vs. yyy) Partners' game aborted} *
			if(strstr(cS, "Partners' game aborted"))
			{
				return 1;
			}
		}
	return 0 ;
}
