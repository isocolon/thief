#include "thief.h"

int New_Type [MAX_FUNCTION_KEY];
char New_FKey [MAX_FUNCTION_KEY] [MAX_FUNCTION_KEY_SIZE];

BOOL CALLBACK FunctionBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
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

			hFunction = GetDlgItem(hwnd, IDD_FUNCTION_FUNCTION);

			for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
			{
				New_Type [nI] = FKey.nType [nI];

				if((New_Type [nI] < 0) || (New_Type [nI] >= MAX_FMENU))
				{
					New_Type [nI] = FUNCTION_NONE;
				}

				if(TOOLBOX_NeedCommandValue(New_Type [nI]))
				{
					strcpy(New_FKey [nI], FKey.cFKey [nI]);
				}
				else
				{
					strcpy(New_FKey [nI], "");
				}
			}

			SendDlgItemMessage(hwnd, IDD_FUNCTION_LIST, LB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
			{
				if((nI + 1) >= 10)
				{
					sprintf(cTmp, "Function key %02d", nI + 2);
				}
				else
				{
					sprintf(cTmp, "Function key %02d", nI + 1);
				}
				SendDlgItemMessage(hwnd, IDD_FUNCTION_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);
			}
			SendDlgItemMessage(hwnd, IDD_FUNCTION_LIST, LB_SETCURSEL, 0, 0);

			SendMessage(hFunction, CB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_FMENU ; nI++)
			{
				strcpy(cTmp, FString [nI]);
				SendMessage(hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			strcpy(cTmp, FString [New_Type [0]]);
			if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_FUNCTION_COMMAND, WM_SETTEXT, 0, (LPARAM) New_FKey [0]);

			nSel = 0;
			EnableWindow(GetDlgItem(hwnd, IDD_FUNCTION_COMMAND), TOOLBOX_NeedCommandValue(New_Type [nSel]));

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_FUNCTION_LIST :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_FUNCTION_LIST, LB_GETCURSEL, 0, 0);

						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nF == CB_ERR)
						{
							New_Type [nSel] = FUNCTION_NONE;
						}
						else
						{
							New_Type [nSel] = nF;
						}

						if(TOOLBOX_NeedCommandValue(New_Type [nSel]))
						{
							SendDlgItemMessage(hwnd, IDD_FUNCTION_COMMAND, WM_GETTEXT, (WPARAM) MAX_FUNCTION_KEY_SIZE - 1, (LPARAM) cTmp);
							strcpy(New_FKey [nSel], cTmp);
						}
						else
						{
							strcpy(New_FKey [nSel], "");
						}

						strcpy(cTmp, FString [New_Type [nI]]);
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_FUNCTION_COMMAND, WM_SETTEXT, 0, (LPARAM) New_FKey [nI]);

						nSel = nI;
						EnableWindow(GetDlgItem(hwnd, IDD_FUNCTION_COMMAND), TOOLBOX_NeedCommandValue(New_Type [nSel]));
					}
					break;

				case IDD_FUNCTION_FUNCTION :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nF == CB_ERR)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_FUNCTION_COMMAND), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_FUNCTION_COMMAND), TOOLBOX_NeedCommandValue(nF));
						}
					}
					break;

				case IDOK :
					nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nF == CB_ERR)
					{
						New_Type [nSel] = FUNCTION_NONE;
					}
					else
					{
						New_Type [nSel] = nF;
					}

					if(TOOLBOX_NeedCommandValue(New_Type [nSel]))
					{
						SendDlgItemMessage(hwnd, IDD_FUNCTION_COMMAND, WM_GETTEXT, (WPARAM) MAX_FUNCTION_KEY_SIZE - 1, (LPARAM) cTmp);
						strcpy(New_FKey [nSel], cTmp);
					}
					else
					{
						strcpy(New_FKey [nSel], "");
					}

					for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
					{
						FKey.nType [nI] = New_Type [nI];
						strcpy(FKey.cFKey [nI], New_FKey [nI]);
						TOOLBOX_AllTrim(FKey.cFKey [nI]);

						if(! TOOLBOX_NeedCommandValue(FKey.nType [nI]))
						{
							strcpy(FKey.cFKey [nI], "");
						}
					}

					FKEY_Adjust();
					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_FUNCTION_DEFAULT :
					for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
					{
						New_Type [nI] = FUNCTION_NONE;
						strcpy(New_FKey [nI], "");
					}

					// set F1 as Ask for Help on Chess Server
					New_Type [0] = FUNCTION_QUESTION_BOX;

					// set F2 as Flip Board
					New_Type [1] = FUNCTION_FLIP_BOARD;

					// set F3 as Accept command
					New_Type [2] = FUNCTION_COMMAND;
					strcpy(New_FKey [2], ICS_ACCEPT_COMMAND);

					// set F4 as Decline command
					New_Type [3] = FUNCTION_COMMAND;
					strcpy(New_FKey [3], ICS_DECLINE_COMMAND);

					// set F5 as Flag command
					New_Type [4] = FUNCTION_COMMAND;
					strcpy(New_FKey [4], ICS_FLAG1_COMMAND);

					// set F6 as Abort command
					New_Type [5] = FUNCTION_COMMAND;
					strcpy(New_FKey [5], ICS_ABORT_COMMAND);

					// set F7 as Rematch command
					New_Type [6] = FUNCTION_COMMAND;
					strcpy(New_FKey [6], ICS_REMATCH_COMMAND);

					// set F8 as sit for piece drop move
					New_Type [7] = FUNCTION_SITPIECE_DROP_MOVE;

					// set F9 as Telnet last tell
					New_Type [8] = FUNCTION_TELNET_LAST_TELL;

					// set F11 as Toggle Maximize/Restore Telnet Console
					New_Type [9] = FUNCTION_TOGGLE_TELNET;

					// set F12 as Toggle Timer Command On/Off
					New_Type [10] = FUNCTION_TOGGLE_TIMER_ONOFF;

					nI = SendDlgItemMessage(hwnd, IDD_FUNCTION_LIST, LB_GETCURSEL, 0, 0);
					strcpy(cTmp, FString [New_Type [nI]]);
					if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					SendDlgItemMessage(hwnd, IDD_FUNCTION_COMMAND, WM_SETTEXT, 0, (LPARAM) New_FKey [nI]);

					nSel = nI;
					EnableWindow(GetDlgItem(hwnd, IDD_FUNCTION_COMMAND), TOOLBOX_NeedCommandValue(New_Type [nSel]));
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void FKEY_Init(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
	{
		FKey.nType [nI] = FUNCTION_NONE;
		strcpy(FKey.cFKey [nI], "");
	}

	// set F1 as Ask for Help on Chess Server
	FKey.nType [0] = FUNCTION_QUESTION_BOX;

	// set F2 as Flip Board
	FKey.nType [1] = FUNCTION_FLIP_BOARD;

	// set F3 as Accept command
	FKey.nType [2] = FUNCTION_COMMAND;
	strcpy(FKey.cFKey [2], ICS_ACCEPT_COMMAND);

	// set F4 as Decline command
	FKey.nType [3] = FUNCTION_COMMAND;
	strcpy(FKey.cFKey [3], ICS_DECLINE_COMMAND);

	// set F5 as Flag command
	FKey.nType [4] = FUNCTION_COMMAND;
	strcpy(FKey.cFKey [4], ICS_FLAG1_COMMAND);

	// set F6 as Abort command
	FKey.nType [5] = FUNCTION_COMMAND;
	strcpy(FKey.cFKey [5], ICS_ABORT_COMMAND);

	// set F7 as Rematch command
	FKey.nType [6] = FUNCTION_COMMAND;
	strcpy(FKey.cFKey [6], ICS_REMATCH_COMMAND);

	// set F8 as sit for piece drop move
	FKey.nType [7] = FUNCTION_SITPIECE_DROP_MOVE;

	// set F9 as Telnet last tell
	FKey.nType [8] = FUNCTION_TELNET_LAST_TELL;

	// set F11 as Toggle Maximize/Restore Telnet Console
	FKey.nType [9] = FUNCTION_TOGGLE_TELNET;

	// set F12 as Toggle Timer Command On/Off
	FKey.nType [10] = FUNCTION_TOGGLE_TIMER_ONOFF;
}

void FKEY_Adjust(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
	{
		if((FKey.nType [nI] < 0) || (FKey.nType [nI] >= MAX_FMENU))
		{
			FKey.nType [nI] = FUNCTION_NONE;
		}

		if(! TOOLBOX_NeedCommandValue(FKey.nType [nI]))
		{
			strcpy(FKey.cFKey [nI], "");
		}
	}
}

int FKEY_Command(int nI)
{
	if(FKey.nType [nI] == FUNCTION_NONE)
	{
		return 0;
	}
	return TOOLBOX_Command(FKey.nType [nI], FKey.cFKey [nI], User.bFKeyCommandAddHist);
}
