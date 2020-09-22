#include "thief.h"

int LEType [MAX_LOGIN_EVENT];
char LECmd [MAX_LOGIN_EVENT] [MAX_LOGIN_EVENT_COMMAND_SIZE];

BOOL CALLBACK LoginEventBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int nSel;
	static HWND hFunction;
	int nI, nF;
	char cTmp [2048];

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			hFunction = GetDlgItem(hwnd, IDD_LOGIN_EVENT1);

			for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
			{
				LEType [nI] = LEvent.nType [nI];

				if((LEType [nI] < 0) || (LEType [nI] >= MAX_FMENU))
				{
					LEType [nI] = FUNCTION_NONE;
				}

				if(TOOLBOX_NeedCommandValue(LEType [nI]))
				{
					strcpy(LECmd [nI], LEvent.cCommand [nI]);
				}
				else
				{
					strcpy(LECmd [nI], "");
				}
			}

			SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT, LB_RESETCONTENT, 0, 0);

			for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
			{
				sprintf(cTmp, "%02d. Login Event %d", nI + 1, nI + 1);
				SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT, LB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT, LB_SETCURSEL, 0, 0);

			SendMessage(hFunction, CB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_FMENU ; nI++)
			{
				strcpy(cTmp, FString [nI]);
				SendMessage(hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			strcpy(cTmp, FString [LEType [0]]);
			if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT2, WM_SETTEXT, 0, (LPARAM) LECmd [0]);

			nSel = 0;
			EnableWindow(GetDlgItem(hwnd, IDD_LOGIN_EVENT2), TOOLBOX_NeedCommandValue(LEType [nSel]));

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_LOGIN_EVENT :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT, LB_GETCURSEL, 0, 0);

						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nF == CB_ERR)
						{
							LEType [nSel] = FUNCTION_NONE;
						}
						else
						{
							LEType [nSel] = nF;
						}

						if(TOOLBOX_NeedCommandValue(LEType [nSel]))
						{
							SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT2, WM_GETTEXT, (WPARAM) MAX_LOGIN_EVENT_COMMAND_SIZE - 1, (LPARAM) cTmp);
							strcpy(LECmd [nSel], cTmp);
						}
						else
						{
							strcpy(LECmd [nSel], "");
						}

						strcpy(cTmp, FString [LEType [nI]]);
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT2, WM_SETTEXT, 0, (LPARAM) LECmd [nI]);

						nSel = nI;
						EnableWindow(GetDlgItem(hwnd, IDD_LOGIN_EVENT2), TOOLBOX_NeedCommandValue(LEType [nSel]));
					}
					break;

				case IDD_LOGIN_EVENT1 :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nF == CB_ERR)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_LOGIN_EVENT2), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_LOGIN_EVENT2), TOOLBOX_NeedCommandValue(nF));
						}
					}
					break;

				case IDOK :
					nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nF == CB_ERR)
					{
						LEType [nSel] = FUNCTION_NONE;
					}
					else
					{
						LEType [nSel] = nF;
					}

					if(TOOLBOX_NeedCommandValue(LEType [nSel]))
					{
						SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT2, WM_GETTEXT, (WPARAM) MAX_LOGIN_EVENT_COMMAND_SIZE - 1, (LPARAM) cTmp);
						strcpy(LECmd [nSel], cTmp);
					}
					else
					{
						strcpy(LECmd [nSel], "");
					}

					SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT2, WM_GETTEXT, (WPARAM) MAX_LOGIN_EVENT_COMMAND_SIZE - 1, (LPARAM) cTmp);

					strcpy(LECmd [nSel], cTmp);

					for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
					{
						LEvent.nType [nI] = LEType [nI];
						strcpy(LEvent.cCommand [nI], LECmd [nI]);
						TOOLBOX_AllTrim(LEvent.cCommand [nI]);

						if(! TOOLBOX_NeedCommandValue(LEvent.nType [nI]))
						{
							strcpy(LEvent.cCommand [nI], "");
						}
					}

					LOGINEVENT_Adjust();
					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_LOGIN_EVENT_DEFAULT :
					for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
					{
						LEType [nI] = FUNCTION_NONE;
						strcpy(LECmd [nI], "");
					}

					nI = SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT, LB_GETCURSEL, 0, 0);
					strcpy(cTmp, FString [LEType [nI]]);
					if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					SendDlgItemMessage(hwnd, IDD_LOGIN_EVENT2, WM_SETTEXT, 0, (LPARAM) LECmd [nI]);

					nSel = nI;
					EnableWindow(GetDlgItem(hwnd, IDD_LOGIN_EVENT2), TOOLBOX_NeedCommandValue(LEType [nSel]));
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void LOGINEVENT_Init(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
	{
		LEvent.nType [nI] = FUNCTION_NONE;
		strcpy(LEvent.cCommand [nI], "");
	}
}

void LOGINEVENT_Adjust(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
	{
		if((LEvent.nType [nI] < 0) || (LEvent.nType [nI] >= MAX_FMENU))
		{
			LEvent.nType [nI] = FUNCTION_NONE;
		}

		if(! TOOLBOX_NeedCommandValue(LEvent.nType [nI]))
		{
			strcpy(LEvent.cCommand [nI], "");
		}
	}
}

int LOGINEVENT_Command(int nI)
{
	if(LEvent.nType [nI] == FUNCTION_NONE)
	{
		return 0;
	}
	return TOOLBOX_Command(LEvent.nType [nI], LEvent.cCommand [nI], User.bLEventCommandAddHist);
}
