#include "thief.h"

BOOL CALLBACK LagCommandBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hTolerance, hThreshold, hFunction;
	char cTmp [2048];
	int  nI, nTmp, nTmp1;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			hTolerance = GetDlgItem(hwnd, IDD_LAG_TOLERANCE);
			hThreshold = GetDlgItem(hwnd, IDD_LAG_THRESHOLD);
			hFunction  = GetDlgItem(hwnd, IDD_LAG_FUNCTION);

			LAGCMD_Adjust();

			if(User.bLagCommand)
			{
				SendDlgItemMessage(hwnd, IDD_LAG_COMMAND_APPLY, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_LAG_COMMAND_APPLY, BM_SETCHECK, BST_UNCHECKED, 0);
			}

			sprintf(cTmp, "%d", LagCmd.nLagTolerance);
			SendDlgItemMessage(hwnd, IDD_LAG_TOLERANCE, WM_SETTEXT, 0, (LPARAM) cTmp);

			sprintf(cTmp, "%d", LagCmd.nLagThreshold);
			SendDlgItemMessage(hwnd, IDD_LAG_THRESHOLD, WM_SETTEXT, 0, (LPARAM) cTmp);

			SendMessage(hFunction, CB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_FMENU ; nI++)
			{
				strcpy(cTmp, FString [nI]);
				SendMessage(hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			strcpy(cTmp, FString [LagCmd.nType]);
			if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_LAG_COMMAND, WM_SETTEXT, 0, (LPARAM) LagCmd.cLagCommand);
			SendDlgItemMessage(hwnd, IDD_LAG_DISPLAY, WM_SETTEXT, 0, (LPARAM) LagCmd.cLagDisplay);

			EnableWindow(GetDlgItem(hwnd, IDD_LAG_COMMAND), TOOLBOX_NeedCommandValue(LagCmd.nType));

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_LAG_FUNCTION :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nI = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nI == CB_ERR)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_LAG_COMMAND), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_LAG_COMMAND), TOOLBOX_NeedCommandValue(nI));
						}
					}
					break;

				case IDOK :
					SendDlgItemMessage(hwnd, IDD_LAG_TOLERANCE, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					if(strlen(cTmp) == 0)
					{
						TOOLBOX_Error("Missing Lag Tolerance.");
						SetFocus(hTolerance);
						return FALSE;
					}

					nTmp = atoi(cTmp);
					if((nTmp < 1) || (nTmp > 300))
					{
						TOOLBOX_Error("Lag Tolerance is Out of Range (1 to 300 seconds).");
						SetFocus(hTolerance);
						return FALSE;
					}

					SendDlgItemMessage(hwnd, IDD_LAG_THRESHOLD, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					if(strlen(cTmp) == 0)
					{
						TOOLBOX_Error("Missing Lag Threshold.");
						SetFocus(hThreshold);
						return FALSE;
					}

					nTmp1 = atoi(cTmp);
					if((nTmp1 < 1) || (nTmp1 > 300))
					{
						TOOLBOX_Error("Lag Threshold is Out of Range (1 to 300 seconds).");
						SetFocus(hThreshold);
						return FALSE;
					}

					nI = SendDlgItemMessage(hwnd, IDD_LAG_COMMAND_APPLY, BM_GETSTATE, 0, 0);
					if(nI == BST_CHECKED)
					{
						User.bLagCommand = 1;
					}
					else
					{
						User.bLagCommand = 0;
					}

					LagCmd.nLagTolerance = nTmp;
					LagCmd.nLagThreshold = nTmp1;

					nI = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						LagCmd.nType = FUNCTION_NONE;
					}
					else
					{
						LagCmd.nType = nI;
					}

					if(TOOLBOX_NeedCommandValue(LagCmd.nType))
					{
						SendDlgItemMessage(hwnd, IDD_LAG_COMMAND, WM_GETTEXT, (WPARAM) MAX_LAG_COMMAND_SIZE - 1, (LPARAM) cTmp);
						TOOLBOX_AllTrim(cTmp);
						strcpy(LagCmd.cLagCommand, cTmp);
					}
					else
					{
						strcpy(LagCmd.cLagCommand, "");
					}

					SendDlgItemMessage(hwnd, IDD_LAG_DISPLAY, WM_GETTEXT, (WPARAM) MAX_LAG_DISPLAY_SIZE, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					strcpy(LagCmd.cLagDisplay, cTmp);

					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_LAG_DEFAULT :
					SendDlgItemMessage(hwnd, IDD_LAG_COMMAND_APPLY, BM_SETCHECK, BST_CHECKED, 0);

					sprintf(cTmp, "%d", ICS_LAG_TOLERANCE);
					SendDlgItemMessage(hwnd, IDD_LAG_TOLERANCE, WM_SETTEXT, 0, (LPARAM) cTmp);

					sprintf(cTmp, "%d", ICS_LAG_CHECK_THRESHOLD);
					SendDlgItemMessage(hwnd, IDD_LAG_THRESHOLD, WM_SETTEXT, 0, (LPARAM) cTmp);

					strcpy(cTmp, FString [ICS_LAG_FUNCTION]);
					if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					strcpy(cTmp, ICS_LAG_COMMAND);
					SendDlgItemMessage(hwnd, IDD_LAG_COMMAND, WM_SETTEXT, 0, (LPARAM) cTmp);

					strcpy(cTmp, ICS_LAG_DISPLAY);
					SendDlgItemMessage(hwnd, IDD_LAG_DISPLAY, WM_SETTEXT, 0, (LPARAM) cTmp);

					EnableWindow(GetDlgItem(hwnd, IDD_LAG_COMMAND), TOOLBOX_NeedCommandValue(ICS_LAG_FUNCTION));
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void LAGCMD_Adjust(void)
{
	if((LagCmd.nType >= 0) && (LagCmd.nType < MAX_FMENU))
	{
		if(! TOOLBOX_NeedCommandValue(LagCmd.nType))
		{
			strcpy(LagCmd.cLagCommand, "");
		}
	}
	else
	{
		LagCmd.nType = FUNCTION_COMMAND;
	}

	if(LagCmd.nLagTolerance < 1)
	{
		LagCmd.nLagTolerance = 1;
	}
	else if(LagCmd.nLagTolerance > 300)
	{
		LagCmd.nLagTolerance = 300;
	}

	if(LagCmd.nLagThreshold < 1)
	{
		LagCmd.nLagThreshold = 1;
	}
	else if(LagCmd.nLagThreshold > 300)
	{
		LagCmd.nLagThreshold = 300;
	}
	LagCmd.nLagLThreshold = LagCmd.nLagThreshold * 1000;
}
