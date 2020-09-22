#include "thief.h"

BOOL CALLBACK TellBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hRecipient, hTell ;
	char cTmp [6000 + 10], cRec [1024 + 10], cTell [4096 + 10] ;
	int nS ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			hRecipient = GetDlgItem(hwnd, IDD_TELL_RECIPIENT) ;
			hTell      = GetDlgItem(hwnd, IDD_TELL) ;

			strcpy(cTmp, Vars.cLastTell) ;

			cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR ;

			SendDlgItemMessage(hwnd, IDD_TELL_RECIPIENT, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp) ;
			SendDlgItemMessage(hwnd, IDD_TELL,           WM_SETTEXT, (WPARAM) 0, (LPARAM) "") ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					SendDlgItemMessage(hwnd, IDD_TELL_RECIPIENT, WM_GETTEXT, (WPARAM) 255, (LPARAM) cRec) ;
					SendDlgItemMessage(hwnd, IDD_TELL,           WM_GETTEXT, (WPARAM) 4096, (LPARAM) cTell) ;

					TOOLBOX_AllTrim(cRec) ;
					TOOLBOX_AllTrim(cTell) ;

					cRec [ICS_HANDLE_LENGTH] = NULL_CHAR ;

					if(strlen(cRec) == 0)
					{
						TOOLBOX_Error("Missing Recipient.") ;
						SetFocus(hRecipient) ;
						return FALSE ;
					}

					sprintf(cTmp, "%s %s %s", ICS_TELL_COMMAND, cRec, cTell) ;
					TOOLBOX_AllTrim(cTmp) ;
					strcat(cTmp, "\n") ;

					if(Timeseal.bSocketIsOpen)
					{
						TOOLBOX_WriteICS(cTmp) ;
					}

					TOOLBOX_WriteUser(cTmp) ;

					nS = strlen(cTmp) ;
					if(cTmp [nS - 1] == '\n')
					{
						cTmp [nS - 1] = NULL_CHAR ;
					}

					if(User.bTellCommandAddHist)
					{
						HISTORY_Add(cTmp) ;
					}

					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;

					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}
