#include "thief.h"

BOOL CALLBACK TimerBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hMinute, hFunction ;
	char cTmp [2048] ;
	int nI ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			hMinute   = GetDlgItem(hwnd, IDD_TIMER_MINUTE) ;
			hFunction = GetDlgItem(hwnd, IDD_TIMER_FUNCTION) ;

			TIMERCMD_Adjust() ;

			sprintf(cTmp, "%d", TimerCmd.nTimerMinute) ;
			SendDlgItemMessage(hwnd, IDD_TIMER_MINUTE, WM_SETTEXT, 0, (LPARAM) cTmp) ;

			SendMessage(hFunction, CB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < MAX_FMENU ; nI++)
			{
				strcpy(cTmp, FString [nI]) ;
				SendMessage(hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}

			strcpy(cTmp, FString [TimerCmd.nType]) ;
			if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
				SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
			}

			TOOLBOX_AllTrim(TimerCmd.cTimerCommand) ;
			SendDlgItemMessage(hwnd, IDD_TIMER_COMMAND, WM_SETTEXT, 0, (LPARAM) TimerCmd.cTimerCommand) ;

			TOOLBOX_AllTrim(TimerCmd.cICCTimerCommand) ;
			SendDlgItemMessage(hwnd, IDD_TIMER_ICC_COMMAND, WM_SETTEXT, 0, (LPARAM) TimerCmd.cICCTimerCommand) ;

			EnableWindow(GetDlgItem(hwnd, IDD_TIMER_COMMAND),     TOOLBOX_NeedCommandValue(TimerCmd.nType)) ;
			EnableWindow(GetDlgItem(hwnd, IDD_TIMER_ICC_COMMAND), TOOLBOX_NeedCommandValue(TimerCmd.nType)) ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_TIMER_FUNCTION :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nI = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
						if(nI == CB_ERR)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_TIMER_COMMAND),     0) ;
							EnableWindow(GetDlgItem(hwnd, IDD_TIMER_ICC_COMMAND), 0) ;
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_TIMER_COMMAND),     TOOLBOX_NeedCommandValue(nI)) ;
							EnableWindow(GetDlgItem(hwnd, IDD_TIMER_ICC_COMMAND), TOOLBOX_NeedCommandValue(nI)) ;
						}
					}
					break ;

				case IDOK :
					SendDlgItemMessage(hwnd, IDD_TIMER_MINUTE, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp) ;

					TOOLBOX_AllTrim(cTmp) ;
					if(strlen(cTmp) == 0)
					{
						TOOLBOX_Error("Missing Minute.") ;
						SetFocus(hMinute) ;
						return FALSE ;
					}

					nI = atoi(cTmp) ;
					if((nI < 1) || (nI > 50))
					{
						TOOLBOX_Error("Minute is Out of Range (1 to 50).") ;
						SetFocus(hMinute) ;
						return FALSE ;
					}
					TimerCmd.nTimerMinute = nI ;

					nI = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
					if(nI == CB_ERR)
					{
						TimerCmd.nType = FUNCTION_NONE ;
					}
					else
					{
						TimerCmd.nType = nI ;
					}

					if(TOOLBOX_NeedCommandValue(TimerCmd.nType))
					{
						SendDlgItemMessage(hwnd, IDD_TIMER_COMMAND, WM_GETTEXT, (WPARAM) MAX_TIMER_LINE_SIZE - 1, (LPARAM) cTmp) ;
						TOOLBOX_AllTrim(cTmp) ;
						strcpy(TimerCmd.cTimerCommand, cTmp) ;

						SendDlgItemMessage(hwnd, IDD_TIMER_ICC_COMMAND, WM_GETTEXT, (WPARAM) MAX_TIMER_LINE_SIZE - 1, (LPARAM) cTmp) ;
						TOOLBOX_AllTrim(cTmp) ;
						strcpy(TimerCmd.cICCTimerCommand, cTmp) ;
					}
					else
					{
						strcpy(TimerCmd.cTimerCommand,    "") ;
						strcpy(TimerCmd.cICCTimerCommand, "") ;
					}

					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_TIMER_DEFAULT :
					sprintf(cTmp, "%d", DEFAULT_TIMER_MINUTE) ;
					SendDlgItemMessage(hwnd, IDD_TIMER_MINUTE, WM_SETTEXT, 0, (LPARAM) cTmp) ;

					strcpy(cTmp, FString [DEFAULT_TIMER_FUNCTION]) ;
					if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
						SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
					}

					strcpy(cTmp, DEFAULT_TIMER_STRING) ;
					TOOLBOX_AllTrim(cTmp) ;
					SendDlgItemMessage(hwnd, IDD_TIMER_COMMAND, WM_SETTEXT, 0, (LPARAM) cTmp) ;

					strcpy(cTmp, DEFAULT_ICC_TIMER_STRING) ;
					TOOLBOX_AllTrim(cTmp) ;
					SendDlgItemMessage(hwnd, IDD_TIMER_ICC_COMMAND, WM_SETTEXT, 0, (LPARAM) cTmp) ;

					EnableWindow(GetDlgItem(hwnd, IDD_TIMER_COMMAND),     TOOLBOX_NeedCommandValue(DEFAULT_TIMER_FUNCTION)) ;
					EnableWindow(GetDlgItem(hwnd, IDD_TIMER_ICC_COMMAND), TOOLBOX_NeedCommandValue(DEFAULT_TIMER_FUNCTION)) ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}
