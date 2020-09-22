#include "thief.h"

LPTIMESOUNDS TmSounds;

BOOL CALLBACK FinalBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int nSel;
	int nI, nC;
	FILE *Fv;
	char cTmp [2048];

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			TmSounds = (LPTIMESOUNDS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(TIMESOUNDS));

			strcpy(TmSounds->cName [ 0], TimeSounds.cName [10]);
			strcpy(TmSounds->cName [ 1], TimeSounds.cName [ 9]);
			strcpy(TmSounds->cName [ 2], TimeSounds.cName [ 8]);
			strcpy(TmSounds->cName [ 3], TimeSounds.cName [ 7]);
			strcpy(TmSounds->cName [ 4], TimeSounds.cName [ 6]);
			strcpy(TmSounds->cName [ 5], TimeSounds.cName [ 5]);
			strcpy(TmSounds->cName [ 6], TimeSounds.cName [ 4]);
			strcpy(TmSounds->cName [ 7], TimeSounds.cName [ 3]);
			strcpy(TmSounds->cName [ 8], TimeSounds.cName [ 2]);
			strcpy(TmSounds->cName [ 9], TimeSounds.cName [ 1]);
			strcpy(TmSounds->cName [10], TimeSounds.cName [ 0]);
			strcpy(TmSounds->cName [11], TimeSounds.cName [21]);
			strcpy(TmSounds->cName [12], TimeSounds.cName [20]);
			strcpy(TmSounds->cName [13], TimeSounds.cName [19]);
			strcpy(TmSounds->cName [14], TimeSounds.cName [18]);
			strcpy(TmSounds->cName [15], TimeSounds.cName [17]);
			strcpy(TmSounds->cName [16], TimeSounds.cName [16]);
			strcpy(TmSounds->cName [17], TimeSounds.cName [15]);
			strcpy(TmSounds->cName [18], TimeSounds.cName [14]);
			strcpy(TmSounds->cName [19], TimeSounds.cName [13]);
			strcpy(TmSounds->cName [20], TimeSounds.cName [12]);
			strcpy(TmSounds->cName [21], TimeSounds.cName [11]);

			if(User.bFinalCountDown)
			{
				SendDlgItemMessage(hwnd, IDD_FINAL_APPLY, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_FINAL_APPLY, BM_SETCHECK, BST_UNCHECKED, 0);
			}

			SendDlgItemMessage(hwnd, IDD_FINAL_SOUND, LB_RESETCONTENT, 0, 0);

			nC = 0;
			for(nI = 10 ; nI >= 0 ; nI--)
			{
				nC = nC + 1;
				if(nI <= 1)
				{
					sprintf(cTmp, "%02d. My Board %d Second", nC, nI);
				}
				else
				{
					sprintf(cTmp, "%02d. My Board %d Seconds", nC, nI);
				}
				SendDlgItemMessage(hwnd, IDD_FINAL_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			for(nI = 10 ; nI >= 0 ; nI--)
			{
				nC = nC + 1;
				if(nI <= 1)
				{
					sprintf(cTmp, "%02d. Partner's Board %d Second", nC, nI);
				}
				else
				{
					sprintf(cTmp, "%02d. Partner's Board %d Seconds", nC, nI);
				}
				SendDlgItemMessage(hwnd, IDD_FINAL_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_FINAL_SOUND, LB_SETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_SETTEXT, 0, (LPARAM) TmSounds->cName [0]);
			nSel = 0;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_FINAL_SOUND :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_FINAL_SOUND, LB_GETCURSEL, 0, 0);

						SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_GETTEXT, (WPARAM) MAX_TIME_SOUND_NAME, (LPARAM) cTmp);
						strcpy(TmSounds->cName [nSel], cTmp);

						SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_SETTEXT, 0, (LPARAM) TmSounds->cName [nI]);
						nSel = nI;
					}
					break;

				case IDD_FINAL_BROWSE :
					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "wav", SOUND_FILT, "Wave File", NULL, NULL, cTmp, TOOLBOX_GetWavePath());
					if(Fv != NULL)
					{
						fclose(Fv);
						strcpy(cTmp, TOOLBOX_GetWaveFilename(cTmp));
						SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_SETTEXT, 0, (LPARAM) cTmp);
					}
					break;

				case IDD_FINAL_PLAY :
					SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_GETTEXT, (WPARAM) MAX_TIME_SOUND_NAME, (LPARAM) cTmp);
					PlaySound(TOOLBOX_GetFullWaveFilename(cTmp), NULL, SND_FILENAME | SND_ASYNC);
					break;

				case IDOK :
					nI = SendDlgItemMessage(hwnd, IDD_FINAL_APPLY, BM_GETSTATE, 0, 0);
					if(nI == BST_CHECKED)
					{
						User.bFinalCountDown = 1;
					}
					else
					{
						User.bFinalCountDown = 0;
					}

					SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_GETTEXT, (WPARAM) MAX_TIME_SOUND_NAME, (LPARAM) cTmp);
					strcpy(TmSounds->cName [nSel], cTmp);

					strcpy(TimeSounds.cName [10], TOOLBOX_GetWaveFilename(TmSounds->cName [ 0]));
					strcpy(TimeSounds.cName [ 9], TOOLBOX_GetWaveFilename(TmSounds->cName [ 1]));
					strcpy(TimeSounds.cName [ 8], TOOLBOX_GetWaveFilename(TmSounds->cName [ 2]));
					strcpy(TimeSounds.cName [ 7], TOOLBOX_GetWaveFilename(TmSounds->cName [ 3]));
					strcpy(TimeSounds.cName [ 6], TOOLBOX_GetWaveFilename(TmSounds->cName [ 4]));
					strcpy(TimeSounds.cName [ 5], TOOLBOX_GetWaveFilename(TmSounds->cName [ 5]));
					strcpy(TimeSounds.cName [ 4], TOOLBOX_GetWaveFilename(TmSounds->cName [ 6]));
					strcpy(TimeSounds.cName [ 3], TOOLBOX_GetWaveFilename(TmSounds->cName [ 7]));
					strcpy(TimeSounds.cName [ 2], TOOLBOX_GetWaveFilename(TmSounds->cName [ 8]));
					strcpy(TimeSounds.cName [ 1], TOOLBOX_GetWaveFilename(TmSounds->cName [ 9]));
					strcpy(TimeSounds.cName [ 0], TOOLBOX_GetWaveFilename(TmSounds->cName [10]));
					strcpy(TimeSounds.cName [21], TOOLBOX_GetWaveFilename(TmSounds->cName [11]));
					strcpy(TimeSounds.cName [20], TOOLBOX_GetWaveFilename(TmSounds->cName [12]));
					strcpy(TimeSounds.cName [19], TOOLBOX_GetWaveFilename(TmSounds->cName [13]));
					strcpy(TimeSounds.cName [18], TOOLBOX_GetWaveFilename(TmSounds->cName [14]));
					strcpy(TimeSounds.cName [17], TOOLBOX_GetWaveFilename(TmSounds->cName [15]));
					strcpy(TimeSounds.cName [16], TOOLBOX_GetWaveFilename(TmSounds->cName [16]));
					strcpy(TimeSounds.cName [15], TOOLBOX_GetWaveFilename(TmSounds->cName [17]));
					strcpy(TimeSounds.cName [14], TOOLBOX_GetWaveFilename(TmSounds->cName [18]));
					strcpy(TimeSounds.cName [13], TOOLBOX_GetWaveFilename(TmSounds->cName [19]));
					strcpy(TimeSounds.cName [12], TOOLBOX_GetWaveFilename(TmSounds->cName [20]));
					strcpy(TimeSounds.cName [11], TOOLBOX_GetWaveFilename(TmSounds->cName [21]));

					HeapFree(GetProcessHeap(), 0, TmSounds);
					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					HeapFree(GetProcessHeap(), 0, TmSounds);
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_FINAL_DEFAULT1 :
					SendDlgItemMessage(hwnd, IDD_FINAL_APPLY, BM_SETCHECK, BST_CHECKED, 0);

					for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
					{
						TmSounds->bPlay   [nI] = 0;
						TmSounds->bPlayed [nI] = 0;
						TmSounds->nLastSecond  = -500;
						strcpy(TmSounds->cName [nI], "");
						TmSounds->pData [nI] = NULL;
					}

					strcpy(TmSounds->cName [ 0],  "PLAY10.WAV");
					strcpy(TmSounds->cName [ 1],  "PLAY9.WAV");
					strcpy(TmSounds->cName [ 2],  "PLAY8.WAV");
					strcpy(TmSounds->cName [ 3],  "PLAY7.WAV");
					strcpy(TmSounds->cName [ 4],  "PLAY6.WAV");
					strcpy(TmSounds->cName [ 5],  "PLAY5.WAV");
					strcpy(TmSounds->cName [ 6],  "PLAY4.WAV");
					strcpy(TmSounds->cName [ 7],  "PLAY3.WAV");
					strcpy(TmSounds->cName [ 8],  "PLAY2.WAV");
					strcpy(TmSounds->cName [ 9],  "PLAY1.WAV");
					strcpy(TmSounds->cName [10],  "PLAY0.WAV");

					strcpy(TmSounds->cName [11],  "PART10.WAV");
					strcpy(TmSounds->cName [12],  "PART9.WAV");
					strcpy(TmSounds->cName [13],  "PART8.WAV");
					strcpy(TmSounds->cName [14],  "PART7.WAV");
					strcpy(TmSounds->cName [15],  "PART6.WAV");
					strcpy(TmSounds->cName [16],  "PART5.WAV");
					strcpy(TmSounds->cName [17],  "PART4.WAV");
					strcpy(TmSounds->cName [18],  "PART3.WAV");
					strcpy(TmSounds->cName [19],  "PART2.WAV");
					strcpy(TmSounds->cName [20],  "PART1.WAV");
					strcpy(TmSounds->cName [21],  "PART0.WAV");

					nI = SendDlgItemMessage(hwnd, IDD_FINAL_SOUND, LB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_SETTEXT, 0, (LPARAM) TmSounds->cName [nI]);
					return TRUE;

				case IDD_FINAL_DEFAULT2 :
					SendDlgItemMessage(hwnd, IDD_FINAL_APPLY, BM_SETCHECK, BST_CHECKED, 0);

					for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
					{
						TmSounds->bPlay   [nI] = 0;
						TmSounds->bPlayed [nI] = 0;
						TmSounds->nLastSecond  = -500;
						strcpy(TmSounds->cName [nI], "");
						TmSounds->pData [nI] = NULL;
					}

					strcpy(TmSounds->cName [ 0], "ALARM.WAV");
					strcpy(TmSounds->cName [11], "THUD.WAV");

					nI = SendDlgItemMessage(hwnd, IDD_FINAL_SOUND, LB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hwnd, IDD_FINAL_EDIT, WM_SETTEXT, 0, (LPARAM) TmSounds->cName [nI]);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void TIMESOUND_Init(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
	{
		TimeSounds.bPlay   [nI] = 0;
		TimeSounds.bPlayed [nI] = 0;
		TimeSounds.pData   [nI] = NULL;
	}
	TimeSounds.nLastSecond = -500;

	strcpy(TimeSounds.cName [ 0],  "PLAY0.WAV");
	strcpy(TimeSounds.cName [ 1],  "PLAY1.WAV");
	strcpy(TimeSounds.cName [ 2],  "PLAY2.WAV");
	strcpy(TimeSounds.cName [ 3],  "PLAY3.WAV");
	strcpy(TimeSounds.cName [ 4],  "PLAY4.WAV");
	strcpy(TimeSounds.cName [ 5],  "PLAY5.WAV");
	strcpy(TimeSounds.cName [ 6],  "PLAY6.WAV");
	strcpy(TimeSounds.cName [ 7],  "PLAY7.WAV");
	strcpy(TimeSounds.cName [ 8],  "PLAY8.WAV");
	strcpy(TimeSounds.cName [ 9],  "PLAY9.WAV");
	strcpy(TimeSounds.cName [10],  "PLAY10.WAV");

	strcpy(TimeSounds.cName [11],  "PART0.WAV");
	strcpy(TimeSounds.cName [12],  "PART1.WAV");
	strcpy(TimeSounds.cName [13],  "PART2.WAV");
	strcpy(TimeSounds.cName [14],  "PART3.WAV");
	strcpy(TimeSounds.cName [15],  "PART4.WAV");
	strcpy(TimeSounds.cName [16],  "PART5.WAV");
	strcpy(TimeSounds.cName [17],  "PART6.WAV");
	strcpy(TimeSounds.cName [18],  "PART7.WAV");
	strcpy(TimeSounds.cName [19],  "PART8.WAV");
	strcpy(TimeSounds.cName [20],  "PART9.WAV");
	strcpy(TimeSounds.cName [21],  "PART10.WAV");
}

void TIMESOUND_Load(void)
{
	int         nI;
	FILE        *Fv;
	struct stat stStat;

	TIMESOUND_Destroy();

	for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
	{
		if(strlen(TimeSounds.cName [nI]) > 0)
		{
			Fv = fopen(TOOLBOX_GetFullWaveFilename(TimeSounds.cName [nI]), "rb");
			if(Fv != NULL)
			{
				if(fstat(fileno(Fv), &stStat) >= 0)
				{
					TimeSounds.pData [nI] = malloc(stStat.st_size);

					if(fread(TimeSounds.pData [nI], stStat.st_size, 1, Fv) >= 1)
					{
						TimeSounds.bPlay [nI] = 1;
					}
				}
				fclose(Fv);
			}
		}
	}
}

void TIMESOUND_Play(int nG, long nFudge)
{
	long nMs, nSecond;
	int nI, bTenth;

	if(nG == INDEX_PLAY)
	{
		if(Game [nG].bIPlayWhite)
		{
			if(! Game [nG].bWhitesMove)
			{
				return;
			}
			nMs = Game [nG].nTimeRemaining [INDEX_WHITE] - nFudge;
		}
		else
		{
			if(Game [nG].bWhitesMove)
			{
				return;
			}
			nMs = Game [nG].nTimeRemaining [INDEX_BLACK] - nFudge;
		}

		nI = 0;
	}
	else if(Login.nLoginType == SERVER_FICS)
	{
		if(Game [INDEX_PLAY].nGamePartner == Game [nG].nGameNumber)
		{
			if(Game [INDEX_PLAY].bIPlayWhite)
			{
				if(Game [nG].bWhitesMove)
				{
					return;
				}
				nMs = Game [nG].nTimeRemaining [INDEX_BLACK] - nFudge;
			}
			else
			{
				if(! Game [nG].bWhitesMove)
				{
					return;
				}
				nMs = Game [nG].nTimeRemaining [INDEX_WHITE] - nFudge;
			}

			nI = PARTNER_TIME_SOUND_START;
		}
		else
		{
			return;
		}
	}
	else if((strlen(Vars.cPartner) > 0) &&
			((stricmp(Game [nG].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
			 (stricmp(Game [nG].cHandle [INDEX_BLACK], Vars.cPartner) == 0)))
	{
		if(Game [INDEX_PLAY].bIPlayWhite)
		{
			if(Game [nG].bWhitesMove)
			{
				return;
			}
			nMs = Game [nG].nTimeRemaining [INDEX_BLACK] - nFudge;
		}
		else
		{
			if(! Game [nG].bWhitesMove)
			{
				return;
			}
			nMs = Game [nG].nTimeRemaining [INDEX_WHITE] - nFudge;
		}

		nI = PARTNER_TIME_SOUND_START;
	}
	else
	{
		return;
	}

	nSecond = (long) floor(((double)(nMs + 999L)) / 1000.0);

	if(nSecond <= 10 && nSecond >= 0)
	{
		nSecond = nSecond + nI;

		switch(RedClock.nShowTenthSecond)
		{
			case TENTH_ALWAYS :
			case TENTH_ONE_MINUTE :
				bTenth = 1;
				break;

			default :
				bTenth = 0;
				break;
		}

		if((Game [nG].nIncrementClock) && bTenth)
		{
			if(TimeSounds.bPlay [nSecond] && (TimeSounds.nLastSecond != nSecond))
			{
				if(User.bSound)
				{
					PlaySound((char *) TimeSounds.pData [nSecond], NULL, SND_MEMORY | SND_ASYNC);
				}
				TimeSounds.nLastSecond = nSecond;
			}
		}
		else
		{
			if(TimeSounds.bPlay [nSecond] && ! TimeSounds.bPlayed [nSecond])
			{
				if(User.bSound)
				{
					PlaySound((char *) TimeSounds.pData [nSecond], NULL, SND_MEMORY | SND_ASYNC);
				}
				if(nSecond == 0)
				{
					TimeSounds.bPlayed [nSecond] = 1;
				}
				else if(! Game [nG].nIncrementClock)
				{
					TimeSounds.bPlayed [nSecond] = 1;
				}
			}
		}
	}
	else
	{
		TimeSounds.nLastSecond = nSecond;
	}
}

void TIMESOUND_Destroy(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
	{
		TimeSounds.bPlay   [nI] = 0;
		TimeSounds.bPlayed [nI] = 0;

		if(TimeSounds.pData [nI])
		{
			free(TimeSounds.pData [nI]);
		}

		TimeSounds.pData [nI] = NULL;
	}
}
