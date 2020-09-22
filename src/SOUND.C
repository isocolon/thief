#include "thief.h"

void SOUND_Init(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_SOUND ; nI++)
	{
		Sounds.bPlay [nI] = 0;
		Sounds.bBeep [nI] = 0;
		strcpy(Sounds.cName [nI], "");
		Sounds.pData [nI] = NULL;
	}

	strcpy(Sounds.cName [SOUND_TELL],              "WHISTLE.WAV");
	strcpy(Sounds.cName [SOUND_PTELL],             "DRIP.WAV");
	strcpy(Sounds.cName [SOUND_SAY],               "WHISTLE.WAV");
	strcpy(Sounds.cName [SOUND_CHALLENGE],         "ALERT.WAV");
	strcpy(Sounds.cName [SOUND_ABORT_REQUEST],     "ABORTREQUEST.WAV");
	strcpy(Sounds.cName [SOUND_TAKEBACK_REQUEST],  "COWBELL.WAV");
	strcpy(Sounds.cName [SOUND_DRAW_REQUEST],		"COWBELL.WAV");
	strcpy(Sounds.cName [SOUND_ADJOURN_REQUEST],	"COWBELL.WAV");
	strcpy(Sounds.cName [SOUND_PARTNER],           "PARTNERSHIPREQUEST.WAV");
}

void SOUND_Load(void)
{
	int nI;
	FILE *Fv;
	struct stat stStat;

	SOUND_Destroy();

	for(nI = 0 ; nI < MAX_SOUND ; nI++)
	{
		if(Sounds.bBeep [nI])
		{
			Sounds.bPlay [nI] = 1;
		}
		else if(strlen(Sounds.cName [nI]) > 0)
		{
			Fv = fopen(TOOLBOX_GetFullWaveFilename(Sounds.cName [nI]), "rb");
			if(Fv != NULL)
			{
				if(fstat(fileno(Fv), &stStat) >= 0)
				{
					Sounds.pData [nI] = malloc(stStat.st_size);
					if(fread(Sounds.pData [nI], stStat.st_size, 1, Fv) >= 1)
					{
						Sounds.bPlay [nI] = 1;
					}
				}
				fclose(Fv);
			}
		}
	}
}

void SOUND_Play(void)
{
	if(User.bSound && User.bTelnetSound)
	{
		if(Sounds.bPlay [Telnet.nCurrentSound])
		{
			if(Sounds.bBeep [Telnet.nCurrentSound])
			{
				TOOLBOX_Beep();
			}
			else
			{
				PlaySound((char *) Sounds.pData [Telnet.nCurrentSound], NULL, SND_MEMORY | SND_ASYNC);
			}
		}
	}
}

void SOUND_Destroy(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_SOUND ; nI++)
	{
		Sounds.bPlay [nI] = 0;
		if(Sounds.pData [nI])
		{
			free(Sounds.pData [nI]);
		}
		Sounds.pData [nI] = NULL;
	}
}
