#include "thief.h"

LPGAMESOUNDS LpGameSounds ;

BOOL CALLBACK GameSoundBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int nSel ;
	int nI, nC ;
	FILE *Fv ;
	char cTmp [2048] ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			LpGameSounds = (LPGAMESOUNDS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(GAMESOUNDS)) ;

			for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
			{
				LpGameSounds->bBeep [nI] = GameSounds.bBeep [nI] ;
				strcpy(LpGameSounds->cName [nI], GameSounds.cName [nI]) ;
			}

			if(User.bGameSound)
			{
				SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_CHECKED, 0) ;
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_UNCHECKED, 0) ;
			}

			SendDlgItemMessage(hwnd, IDD_SOUND, LB_RESETCONTENT, 0, 0) ;

			sprintf(cTmp, "%02d. Playing Board Start", GAME_SOUND_PLAY_START + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Move", GAME_SOUND_PLAY_MOVE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Smart Move", GAME_SOUND_PLAY_MOVE_SMART + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board True Premove", GAME_SOUND_PLAY_MOVE_TP + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Capture", GAME_SOUND_PLAY_CAPTURE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Castle", GAME_SOUND_PLAY_CASTLE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. My King is in Check", GAME_SOUND_PLAY_CHECK + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board End", GAME_SOUND_PLAY_OVER + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner's Board Move", GAME_SOUND_PARTNER_MOVE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner's Board Capture", GAME_SOUND_PARTNER_CAPTURE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner's Board Castle", GAME_SOUND_PARTNER_CASTLE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner's King is in Check", GAME_SOUND_PARTNER_CHECK + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Move (While Playing)", GAME_SOUND_OBS_MOVE1 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Capture (While Playing)", GAME_SOUND_OBS_CAPTURE1 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Castle (While Playing)", GAME_SOUND_OBS_CASTLE1 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Check (While Playing)", GAME_SOUND_OBS_CHECK1 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board End (While Playing)", GAME_SOUND_OBS_OVER1 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Move (not Playing)", GAME_SOUND_OBS_MOVE2 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Capture (not Playing)", GAME_SOUND_OBS_CAPTURE2 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Castle (not Playing)", GAME_SOUND_OBS_CASTLE2 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Check (not Playing)", GAME_SOUND_OBS_CHECK2 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board End (not Playing)", GAME_SOUND_OBS_OVER2 + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. I Won", GAME_SOUND_I_WON + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. I Lost", GAME_SOUND_I_LOST + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. I Drew", GAME_SOUND_I_DREW + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. I Aborted", GAME_SOUND_I_ABORT + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner Won", GAME_SOUND_PARTNER_WON + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner Lost", GAME_SOUND_PARTNER_LOST + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner Drew", GAME_SOUND_PARTNER_DREW + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Partner Aborted", GAME_SOUND_PARTNER_ABORT + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Exam Move", GAME_SOUND_EXAM_MOVE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Exam Capture", GAME_SOUND_EXAM_CAPTURE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Exam Castle", GAME_SOUND_EXAM_CASTLE + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Exam Check", GAME_SOUND_EXAM_CHECK + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Game Abort", GAME_SOUND_GAME_ABORT + 1) ;
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			SendDlgItemMessage(hwnd, IDD_SOUND, LB_SETCURSEL, 0, 0) ;

			nSel = GAME_SOUND_PLAY_START ;

			if(LpGameSounds->bBeep [nSel])
			{
				SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
			}

			SendDlgItemMessage(hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpGameSounds->cName [nSel]) ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_SOUND :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_SOUND, LB_GETCURSEL, 0, 0) ;

						nC = SendDlgItemMessage(hwnd, IDD_BEEP, BM_GETSTATE, 0, 0) ;

						LpGameSounds->bBeep [nSel] = (nC == BST_CHECKED) ;

						SendDlgItemMessage(hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_GAME_SOUND_NAME, (LPARAM) cTmp) ;

						strcpy(LpGameSounds->cName [nSel], cTmp) ;

						if(LpGameSounds->bBeep [nI])
						{
							SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
						}
						else
						{
							SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
						}

						SendDlgItemMessage(hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpGameSounds->cName [nI]) ;

						nSel = nI ;
					}
					break ;

				case IDD_BROWSE :
					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "wav", SOUND_FILT, "Wave File", NULL, NULL, cTmp, TOOLBOX_GetWavePath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						strcpy(cTmp, TOOLBOX_GetWaveFilename(cTmp)) ;
						SendDlgItemMessage(hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) cTmp) ;
					}
					break ;

				case IDD_PLAY :
					SendDlgItemMessage(hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_GAME_SOUND_NAME, (LPARAM) cTmp) ;
					PlaySound(TOOLBOX_GetFullWaveFilename(cTmp), NULL, SND_FILENAME | SND_ASYNC) ;
					break ;

				case IDOK :
					nC = SendDlgItemMessage(hwnd, IDD_BEEP, BM_GETSTATE, 0, 0) ;

					LpGameSounds->bBeep [nSel] = (nC == BST_CHECKED) ;

					SendDlgItemMessage(hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_GAME_SOUND_NAME, (LPARAM) cTmp) ;

					strcpy(LpGameSounds->cName [nSel], cTmp) ;

					nI = SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_GETSTATE, 0, 0) ;
					if(nI == BST_CHECKED)
					{
						User.bGameSound = 1 ;
					}
					else
					{
						User.bGameSound = 0 ;
					}

					for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
					{
						GameSounds.bBeep [nI] = LpGameSounds->bBeep [nI] ;
						strcpy(GameSounds.cName [nI], TOOLBOX_GetWaveFilename(LpGameSounds->cName [nI])) ;
						TOOLBOX_AllTrim(GameSounds.cName [nI]) ;
					}

					HeapFree(GetProcessHeap(), 0, LpGameSounds) ;
					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					HeapFree(GetProcessHeap(), 0, LpGameSounds) ;
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_SND_DEFAULT :
					SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_CHECKED, 0) ;

					for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
					{
						LpGameSounds->bPlay [nI] = 0 ;
						LpGameSounds->bBeep [nI] = 0 ;
						strcpy(LpGameSounds->cName [nI], "") ;
						LpGameSounds->pData [nI] = NULL ;
					}

					strcpy(LpGameSounds->cName [GAME_SOUND_PLAY_START],      "FITEBELL.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PLAY_MOVE],       "MOVE.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PLAY_MOVE_SMART], "MOVE.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PLAY_OVER],       "ASTRING.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PLAY_CHECK],      "CHECK.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_OBS_MOVE2],       "MOVE.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_OBS_OVER2],       "ASTRING.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_I_WON],           "IWON.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_I_LOST],          "ILOST.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_I_DREW],          "IDREW.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_I_ABORT],         "IABORTED.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PARTNER_WON],     "PARTNERWON.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PARTNER_LOST],    "PARTNERLOST.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PARTNER_ABORT],   "PARTNERABORTED.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_PARTNER_DREW],    "GAMEDRAWN.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_EXAM_MOVE],       "EXAMMOVE.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_EXAM_CAPTURE],    "EXAMCAP.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_EXAM_CASTLE],     "EXAMMOVE.WAV") ;
					strcpy(LpGameSounds->cName [GAME_SOUND_GAME_ABORT],      "GAMEABORTED.WAV") ;

					nI = SendDlgItemMessage(hwnd, IDD_SOUND, LB_GETCURSEL, 0, 0) ;
					if(LpGameSounds->bBeep [nI])
					{
						SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
					}
					else
					{
						SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
					}

					SendDlgItemMessage(hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpGameSounds->cName [nI]) ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}
