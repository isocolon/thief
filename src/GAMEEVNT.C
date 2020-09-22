#include "thief.h"

int GEType [MAX_GAME_EVENT] ;
char GECmd [MAX_GAME_EVENT] [MAX_GAME_EVENT_COMMAND_SIZE] ;

BOOL CALLBACK GameEventBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int nSel ;
	static HWND hFunction ;
	int nI, nF ;
	char cTmp [2048] ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			hFunction = GetDlgItem(hwnd, IDD_GAME_EVENT1) ;

			for(nI = 0 ; nI < MAX_GAME_EVENT ; nI++)
			{
				GEType [nI] = GEvent.nType [nI] ;

				if((GEType [nI] < 0) || (GEType [nI] >= MAX_FMENU))
				{
					GEType [nI] = FUNCTION_NONE ;
				}

				if(TOOLBOX_NeedCommandValue(GEType [nI]))
				{
					strcpy(GECmd [nI], GEvent.cCommand [nI]) ;
				}
				else
				{
					strcpy(GECmd [nI], "") ;
				}
			}

			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_RESETCONTENT, 0, 0) ;

			sprintf(cTmp, "%02d. Playing Board Game Start 1", GAME_EVENT_PLAY_START1 + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Game Start 2", GAME_EVENT_PLAY_START2 + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Bughouse Game Start", GAME_EVENT_PLAY_BUG_START + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board One Game Start", GAME_EVENT_PLAY_ONE_START + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Game Over 1", GAME_EVENT_PLAY_OVER1 + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Playing Board Game Over 2", GAME_EVENT_PLAY_OVER2 + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Game Start", GAME_EVENT_OBSERVE_START + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Bughouse Game Start", GAME_EVENT_OBSERVE_BUG_ST + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board One Game Start", GAME_EVENT_OBSERVE_ONE_ST + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			sprintf(cTmp, "%02d. Observe Board Game Over", GAME_EVENT_OBSERVE_OVER + 1) ;
			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

			SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_SETCURSEL, 0, 0) ;

			SendMessage(hFunction, CB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < MAX_FMENU ; nI++)
			{
				strcpy(cTmp, FString [nI]) ;
				SendMessage(hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}

			strcpy(cTmp, FString [GEType [0]]) ;
			if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
				SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
			}

			SendDlgItemMessage(hwnd, IDD_GAME_EVENT2, WM_SETTEXT, 0, (LPARAM) GECmd [0]) ;

			nSel = 0 ;
			EnableWindow(GetDlgItem(hwnd, IDD_GAME_EVENT2), TOOLBOX_NeedCommandValue(GEType [nSel])) ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_GAME_EVENT :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_GETCURSEL, 0, 0) ;

						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
						if(nF == CB_ERR)
						{
							GEType [nSel] = FUNCTION_NONE ;
						}
						else
						{
							GEType [nSel] = nF ;
						}

						if(TOOLBOX_NeedCommandValue(GEType [nSel]))
						{
							SendDlgItemMessage(hwnd, IDD_GAME_EVENT2, WM_GETTEXT, (WPARAM) MAX_GAME_EVENT_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
							strcpy(GECmd [nSel], cTmp) ;
						}
						else
						{
							strcpy(GECmd [nSel], "") ;
						}

						strcpy(cTmp, FString [GEType [nI]]) ;
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
						}

						SendDlgItemMessage(hwnd, IDD_GAME_EVENT2, WM_SETTEXT, 0, (LPARAM) GECmd [nI]) ;

						nSel = nI ;
						EnableWindow(GetDlgItem(hwnd, IDD_GAME_EVENT2), TOOLBOX_NeedCommandValue(GEType [nSel])) ;
					}
					break ;

				case IDD_GAME_EVENT1 :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
						if(nF == CB_ERR)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_GAME_EVENT2), 0) ;
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_GAME_EVENT2), TOOLBOX_NeedCommandValue(nF)) ;
						}
					}
					break ;

				case IDOK :
					nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
					if(nF == CB_ERR)
					{
						GEType [nSel] = FUNCTION_NONE ;
					}
					else
					{
						GEType [nSel] = nF ;
					}

					if(TOOLBOX_NeedCommandValue(GEType [nSel]))
					{
						SendDlgItemMessage(hwnd, IDD_GAME_EVENT2, WM_GETTEXT, (WPARAM) MAX_GAME_EVENT_COMMAND_SIZE - 1, (LPARAM) cTmp) ;
						strcpy(GECmd [nSel], cTmp) ;
					}
					else
					{
						strcpy(GECmd [nSel], "") ;
					}

					SendDlgItemMessage(hwnd, IDD_GAME_EVENT2, WM_GETTEXT, (WPARAM) MAX_GAME_EVENT_COMMAND_SIZE - 1, (LPARAM) cTmp) ;

					strcpy(GECmd [nSel], cTmp) ;

					for(nI = 0 ; nI < MAX_GAME_EVENT ; nI++)
					{
						GEvent.nType [nI] = GEType [nI] ;
						strcpy(GEvent.cCommand [nI], GECmd [nI]) ;
						TOOLBOX_AllTrim(GEvent.cCommand [nI]) ;

						if(! TOOLBOX_NeedCommandValue(GEvent.nType [nI]))
						{
							strcpy(GEvent.cCommand [nI], "") ;
						}
					}

					GAMEEVENT_Adjust() ;
					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_GAME_EVENT_DEFAULT :
					for(nI = 0 ; nI < MAX_GAME_EVENT ; nI++)
					{
						GEType [nI] = FUNCTION_NONE ;
						strcpy(GECmd [nI], "") ;
					}

					nI = SendDlgItemMessage(hwnd, IDD_GAME_EVENT, LB_GETCURSEL, 0, 0) ;
					strcpy(cTmp, FString [GEType [nI]]) ;
					if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
						SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
					}

					SendDlgItemMessage(hwnd, IDD_GAME_EVENT2, WM_SETTEXT, 0, (LPARAM) GECmd [nI]) ;

					nSel = nI ;
					EnableWindow(GetDlgItem(hwnd, IDD_GAME_EVENT2), TOOLBOX_NeedCommandValue(GEType [nSel])) ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}

void GAMEEVENT_Init(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_GAME_EVENT ; nI++)
	{
		GEvent.nType [nI] = FUNCTION_NONE ;
		strcpy(GEvent.cCommand [nI], "") ;
	}
}

void GAMEEVENT_Adjust(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_GAME_EVENT ; nI++)
	{
		if((GEvent.nType [nI] < 0) || (GEvent.nType [nI] >= MAX_FMENU))
		{
			GEvent.nType [nI] = FUNCTION_NONE ;
		}

		if(! TOOLBOX_NeedCommandValue(GEvent.nType [nI]))
		{
			strcpy(GEvent.cCommand [nI], "") ;
		}
	}
}

int GAMEEVENT_Command(int nI)
{
	if(GEvent.nType [nI] == FUNCTION_NONE)
	{
		return 0 ;
	}
	return TOOLBOX_Command(GEvent.nType [nI], GEvent.cCommand [nI], User.bGEventCommandAddHist) ;
}
