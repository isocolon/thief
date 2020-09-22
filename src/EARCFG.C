#include "thief.h"

LPEARS LpEars ;

BOOL CALLBACK EarBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
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

			LpEars = (LPEARS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(EARS)) ;
			for(nI = 0 ; nI < MAX_EAR ; nI++)
			{
				LpEars->bBeep [nI] = Ears.bBeep [nI] ;
				strcpy(LpEars->cTell [nI], Ears.cTell [nI]) ;
				strcpy(LpEars->cName [nI], Ears.cName [nI]) ;
			}

			if(User.bEarSound)
			{
				SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_CHECKED, 0) ;
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_UNCHECKED, 0) ;
			}

			SendDlgItemMessage(hwnd, IDD_SOUND, LB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < MAX_EAR ; nI++)
			{
				sprintf(cTmp, "%02d", nI + 1) ;
				SendDlgItemMessage(hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}
			SendDlgItemMessage(hwnd, IDD_SOUND, LB_SETCURSEL, 0, 0) ;

			SendDlgItemMessage(hwnd, IDD_MATCH, WM_SETTEXT, 0, (LPARAM) LpEars->cTell [0]) ;

			if(LpEars->bBeep [0])
			{
				SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
			}

			SendDlgItemMessage(hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpEars->cName [0]) ;

			nSel = 0 ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_SOUND :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_SOUND, LB_GETCURSEL, 0, 0) ;

						SendDlgItemMessage(hwnd, IDD_MATCH, WM_GETTEXT, (WPARAM) MAX_EAR_TELL, (LPARAM) cTmp) ;

						strcpy(LpEars->cTell [nSel], cTmp) ;

						nC = SendDlgItemMessage(hwnd, IDD_BEEP, BM_GETSTATE, 0, 0) ;

						LpEars->bBeep [nSel] = (nC == BST_CHECKED) ;

						SendDlgItemMessage(hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_EAR_NAME, (LPARAM) cTmp) ;

						strcpy(LpEars->cName [nSel], cTmp) ;

						SendDlgItemMessage(hwnd, IDD_MATCH, WM_SETTEXT, 0, (LPARAM) LpEars->cTell [nI]) ;

						if(LpEars->bBeep [nI])
						{
							SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
						}
						else
						{
							SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
						}

						SendDlgItemMessage(hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpEars->cName [nI]) ;

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
					SendDlgItemMessage(hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_EAR_NAME, (LPARAM) cTmp) ;
					PlaySound(TOOLBOX_GetFullWaveFilename(cTmp), NULL, SND_FILENAME | SND_ASYNC) ;
					break ;

				case IDOK :
					SendDlgItemMessage(hwnd, IDD_MATCH, WM_GETTEXT, (WPARAM) MAX_EAR_TELL, (LPARAM) cTmp) ;

					strcpy(LpEars->cTell [nSel], cTmp) ;

					nC = SendDlgItemMessage(hwnd, IDD_BEEP, BM_GETSTATE, 0, 0) ;

					LpEars->bBeep [nSel] = (nC == BST_CHECKED) ;

					SendDlgItemMessage(hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_EAR_NAME, (LPARAM) cTmp) ;

					strcpy(LpEars->cName [nSel], cTmp) ;

					nI = SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_GETSTATE, 0, 0) ;
					if(nI == BST_CHECKED)
					{
						User.bEarSound = 1 ;
					}
					else
					{
						User.bEarSound = 0 ;
					}

					for(nI = 0 ; nI < MAX_EAR ; nI++)
					{
						Ears.bBeep [nI] = LpEars->bBeep [nI] ;
						strcpy(Ears.cTell [nI], LpEars->cTell [nI]) ;
						strcpy(Ears.cName [nI], TOOLBOX_GetWaveFilename(LpEars->cName [nI])) ;
						TOOLBOX_AllTrim(Ears.cTell [nI]) ;
						TOOLBOX_AllTrim(Ears.cName [nI]) ;
					}

					HeapFree(GetProcessHeap(), 0, LpEars) ;
					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					HeapFree(GetProcessHeap(), 0, LpEars) ;
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_SND_DEFAULT :
					SendDlgItemMessage(hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_CHECKED, 0) ;

					for(nI = 0 ; nI < MAX_EAR ; nI++)
					{
						LpEars->bPlay [nI] = 0 ;
						LpEars->bBeep [nI] = 0 ;
						strcpy(LpEars->cTell [nI], "") ;
						strcpy(LpEars->cName [nI], "") ;
						LpEars->pData [nI] = NULL ;
					}

					// initialize old ear default
					strcpy(LpEars->cTell [0], "Sit") ;
					strcpy(LpEars->cName [0], "SIT.WAV") ;

					strcpy(LpEars->cTell [1], "Go") ;
					strcpy(LpEars->cName [1], "GO.WAV") ;

					strcpy(LpEars->cTell [2], "Fast") ;
					strcpy(LpEars->cName [2], "PLAYFAST.WAV") ;

					strcpy(LpEars->cTell [3], "Time") ;
					strcpy(LpEars->cName [3], "LOOKTIME.WAV") ;

					strcpy(LpEars->cTell [4], "++") ;
					strcpy(LpEars->cName [4], "YESTRADE.WAV") ;

					strcpy(LpEars->cTell [5], "----") ;
					strcpy(LpEars->cName [5], "NOTRADE.WAV") ;

					strcpy(LpEars->cTell [6], "+Q") ;
					strcpy(LpEars->cName [6], "QGOOD.WAV") ;

					strcpy(LpEars->cTell [7], "-Q") ;
					strcpy(LpEars->cName [7], "NOQ.WAV") ;

					strcpy(LpEars->cTell [8], "+R") ;
					strcpy(LpEars->cName [8], "RGOOD.WAV") ;

					strcpy(LpEars->cTell [9], "-R") ;
					strcpy(LpEars->cName [9], "NOR.WAV") ;

					strcpy(LpEars->cTell [10], "+N") ;
					strcpy(LpEars->cName [10], "NGOOD.WAV") ;

					strcpy(LpEars->cTell [11], "-N") ;
					strcpy(LpEars->cName [11], "NON.WAV") ;

					strcpy(LpEars->cTell [12], "+B") ;
					strcpy(LpEars->cName [12], "BGOOD.WAV") ;

					strcpy(LpEars->cTell [13], "-B") ;
					strcpy(LpEars->cName [13], "NOB.WAV") ;

					strcpy(LpEars->cTell [14], "+P") ;
					strcpy(LpEars->cName [14], "PGOOD.WAV") ;

					strcpy(LpEars->cTell [15], "-P") ;
					strcpy(LpEars->cName [15], "NOP.WAV") ;

					strcpy(LpEars->cTell [16], "OK") ;
					strcpy(LpEars->cName [16], "OK.WAV") ;

					strcpy(LpEars->cTell [17], "OK now") ;
					strcpy(LpEars->cName [17], "OKNOW.WAV") ;

					strcpy(LpEars->cTell [18], "Hard") ;
					strcpy(LpEars->cName [18], "HARD2GET.WAV") ;

					strcpy(LpEars->cTell [19], "Coming") ;
					strcpy(LpEars->cName [19], "COMING.WAV") ;

					strcpy(LpEars->cTell [20], "Maybe") ;
					strcpy(LpEars->cName [20], "MAYBE.WAV") ;

					strcpy(LpEars->cTell [21], "I sit") ;
					strcpy(LpEars->cName [21], "ISIT.WAV") ;

					strcpy(LpEars->cTell [22], "Mates me") ;
					strcpy(LpEars->cName [22], "MATESME.WAV") ;

					strcpy(LpEars->cTell [23], "Mates him") ;
					strcpy(LpEars->cName [23], "MATESHIM.WAV") ;

					strcpy(LpEars->cTell [24], "I dead") ;
					strcpy(LpEars->cName [24], "IMDEAD.WAV") ;

					strcpy(LpEars->cTell [25], "Opp dead") ;
					strcpy(LpEars->cName [25], "OPPMATED.WAV") ;

					strcpy(LpEars->cTell [26], "Yes") ;
					strcpy(LpEars->cName [26], "YES.WAV") ;

					strcpy(LpEars->cTell [27], "No") ;
					strcpy(LpEars->cName [27], "NO.WAV") ;

					strcpy(LpEars->cTell [28], "Tell Me Go") ;
					strcpy(LpEars->cName [28], "TELLMEGO.WAV") ;

					strcpy(LpEars->cTell [29], "Sit Until I Tell U Go") ;
					strcpy(LpEars->cName [29], "TELLUGO.WAV") ;

					strcpy(LpEars->cTell [30], "Keep Checking") ;
					strcpy(LpEars->cName [30], "KEEPCHECKING.WAV") ;

					strcpy(LpEars->cTell [31], "Nevermind") ;
					strcpy(LpEars->cName [31], "NEVERMIND.WAV") ;

					// include new FICS ear default
					strcpy(LpEars->cTell [32], "+") ;
					strcpy(LpEars->cName [32], "+.WAV") ;

					strcpy(LpEars->cTell [33], "+++") ;
					strcpy(LpEars->cName [33], "+++.WAV") ;

					strcpy(LpEars->cTell [34], "-") ;
					strcpy(LpEars->cName [34], "-.WAV") ;

					strcpy(LpEars->cTell [35], "- -") ;
					strcpy(LpEars->cName [35], "-_-.WAV") ;

					strcpy(LpEars->cTell [36], "- - -") ;
					strcpy(LpEars->cName [36], "-_-_-.WAV") ;

					strcpy(LpEars->cTell [37], "Pawn") ;
					strcpy(LpEars->cName [37], "PAWN.WAV") ;

					strcpy(LpEars->cTell [38], "Bishop") ;
					strcpy(LpEars->cName [38], "BISHOP.WAV") ;

					strcpy(LpEars->cTell [39], "Knight") ;
					strcpy(LpEars->cName [39], "KNIGHT.WAV") ;

					strcpy(LpEars->cTell [40], "Rook") ;
					strcpy(LpEars->cName [40], "ROOK.WAV") ;

					strcpy(LpEars->cTell [41], "Queen") ;
					strcpy(LpEars->cName [41], "QUEEN.WAV") ;

					strcpy(LpEars->cTell [42], "I lose...") ;
					strcpy(LpEars->cName [42], "ILOSE.WAV") ;

					strcpy(LpEars->cTell [43], "Can I go?") ;
					strcpy(LpEars->cName [43], "CANIGO.WAV") ;

					strcpy(LpEars->cTell [44], "Take") ;
					strcpy(LpEars->cName [44], "TAKE.WAV") ;

					strcpy(LpEars->cTell [45], "What u want?") ;
					strcpy(LpEars->cName [45], "WHATUWANT.WAV") ;

					strcpy(LpEars->cTell [46], "No!") ;
					strcpy(LpEars->cName [46], "NO.WAV") ;

					strcpy(LpEars->cTell [47], "I Mated") ;
					strcpy(LpEars->cName [47], "IMATED.WAV") ;

					strcpy(LpEars->cTell [48], "Opp Mated") ;
					strcpy(LpEars->cName [48], "OPPMATED.WAV") ;

					// new SuperGrover sounds
					strcpy(LpEars->cTell [49], "We up") ;
					strcpy(LpEars->cName [49], "WEUP.WAV") ;

					strcpy(LpEars->cTell [50], "We down") ;
					strcpy(LpEars->cName [50], "WEDOWN.WAV") ;

					strcpy(LpEars->cTell [51], "U get") ;
					strcpy(LpEars->cName [51], "YOUGET.WAV") ;

					strcpy(LpEars->cTell [52], "He gets") ;
					strcpy(LpEars->cName [52], "HEGETS.WAV") ;

					strcpy(LpEars->cTell [53], "Watchout") ;
					strcpy(LpEars->cName [53], "WATCHOUT.WAV") ;

					strcpy(LpEars->cTell [54], "Feed me") ;
					strcpy(LpEars->cName [54], "FEEDME.WAV") ;

					strcpy(LpEars->cTell [55], "Lag") ;
					strcpy(LpEars->cName [55], "LAG.WAV") ;

					strcpy(LpEars->cTell [56], "Lag cheating fuckers") ;
					strcpy(LpEars->cName [56], "LAGCHEATINGFUCKERS.WAV") ;

					strcpy(LpEars->cTell [57], "Mother fucking lag") ;
					strcpy(LpEars->cName [57], "MOTHERLAG.WAV") ;

					strcpy(LpEars->cTell [58], "Bitch is dead") ;
					strcpy(LpEars->cName [58], "BITCHDEAD.WAV") ;

					strcpy(LpEars->cTell [59], "Bite me") ;
					strcpy(LpEars->cName [59], "BITEME.WAV") ;

					strcpy(LpEars->cTell [60], "Cake") ;
					strcpy(LpEars->cName [60], "CAKE.WAV") ;

					strcpy(LpEars->cTell [61], "Eat shit") ;
					strcpy(LpEars->cName [61], "EATSHIT.WAV") ;

					strcpy(LpEars->cTell [62], "Fuck u") ;
					strcpy(LpEars->cName [62], "FUKYOUMOTHER.WAV") ;

					strcpy(LpEars->cTell [63], "I finger focked") ;
					strcpy(LpEars->cName [63], "IFINGERFOCKED.WAV") ;

					strcpy(LpEars->cTell [64], "I finger focking") ;
					strcpy(LpEars->cName [64], "IFINGERFOCKING.WAV") ;

					strcpy(LpEars->cTell [65], "Move faster bitch") ;
					strcpy(LpEars->cName [65], "MOVEFASTBITCH.WAV") ;

					strcpy(LpEars->cTell [66], "Son of a bitch") ;
					strcpy(LpEars->cName [66], "SONOFABITCH.WAV") ;

					strcpy(LpEars->cTell [67], "So easy") ;
					strcpy(LpEars->cName [67], "SOEASY.WAV") ;

					strcpy(LpEars->cTell [68], "She dead") ;
					strcpy(LpEars->cName [68], "SHEDEAD.WAV") ;

					strcpy(LpEars->cTell [69], "They suck") ;
					strcpy(LpEars->cName [69], "THEYSUCK.WAV") ;

					// load into input fields
					nI = SendDlgItemMessage(hwnd, IDD_SOUND, LB_GETCURSEL, 0, 0) ;

					SendDlgItemMessage(hwnd, IDD_MATCH, WM_SETTEXT, 0, (LPARAM) LpEars->cTell [nI]) ;

					if(LpEars->bBeep [nI])
					{
						SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
					}
					else
					{
						SendDlgItemMessage(hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
					}

					SendDlgItemMessage(hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpEars->cName [nI]) ;
					return TRUE ;

				case IDD_EAR_OLDFICS :
					HeapFree(GetProcessHeap(), 0, LpEars) ;

					User.bEarSound = 1 ;

					BUTTON_OldFICSEAR() ;
					BUTTON_Refresh() ;
					BUTTON_RefreshLabel() ;

					EAR_Destroy() ;
					EAR_OldFICSEAR() ;
					EAR_Load() ;

					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_EAR_NEWFICS :
					HeapFree(GetProcessHeap(), 0, LpEars) ;

					User.bEarSound = 1 ;

					BUTTON_NewFICSEAR() ;
					BUTTON_Refresh() ;
					BUTTON_RefreshLabel() ;

					EAR_Destroy() ;
					EAR_NewFICSEAR() ;
					EAR_Load() ;

					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}
