#include "thief.h"

BOOL CALLBACK VarsBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hText, hVar;
	char cTmp [2048];
	int nI, bError;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			hText = GetDlgItem(hwnd, IDD_VAR_TEXT);
			hVar  = GetDlgItem(hwnd, IDD_VAR1);

			switch(Vars.nEditVar)
			{
				case 0 :
					sprintf(cTmp, "Edit %s", VAR_WHOAMI);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_WHOAMI);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_HIDE);
					strcpy(cTmp, Vars.cWhoAmI);
					break;

				case 1 :
					sprintf(cTmp, "Edit %s", VAR_PARTNER);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_PARTNER);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cPartner);
					break;

				case 2 :
					sprintf(cTmp, "Edit %s", VAR_OPPONENT);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_OPPONENT);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cOpponent);
					break;

				case 3 :
					sprintf(cTmp, "Edit %s", VAR_PARTNEROPPONENT);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_PARTNEROPPONENT);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cPartnerOpponent);
					break;

				case 4 :
					sprintf(cTmp, "Edit %s", VAR_LASTTELL);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_LASTTELL);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cLastTell);
					break;

				case 5 :
					sprintf(cTmp, "Edit %s", VAR_LASTKIB);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_LASTKIB);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cLastKib);
					break;

				case 6 :
					sprintf(cTmp, "Edit %s", VAR_LASTWHISPER);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_LASTWHISPER);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cLastWhisper);
					break;

				case 7 :
					sprintf(cTmp, "Edit %s", VAR_LASTSAY);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_LASTSAY);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cLastSay);
					break;

				case 8 :
					sprintf(cTmp, "Edit %s", VAR_TELNETHANDLE);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_TELNETHANDLE);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cTelnetHandle);
					break;

				case 9 :
					sprintf(cTmp, "Edit %s", VAR_BOARDHANDLE);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_BOARDHANDLE);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cBoardHandle);
					break;

				case 10 :
					sprintf(cTmp, "Edit %s", VAR_USERHANDLE1);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_USERHANDLE1);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cUserHandle1);
					break;

				case 11 :
					sprintf(cTmp, "Edit %s", VAR_USERHANDLE2);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_USERHANDLE2);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cUserHandle2);
					break;

				default :
					Vars.nEditVar = 1;
					sprintf(cTmp, "Edit %s", VAR_PARTNER);
					SetWindowText(hwnd, cTmp);
					SetWindowText(hText, VAR_PARTNER);
					ShowWindow(GetDlgItem(hwnd, IDOK), SW_SHOW);
					strcpy(cTmp, Vars.cPartner);
					break;
			}

			SendDlgItemMessage(hwnd, IDD_VAR1, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp);

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					SendDlgItemMessage(hwnd, IDD_VAR1, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					cTmp [16] = NULL_CHAR;

					if(Vars.nEditVar != 0)
					{
						for(nI = 0 ; nI < ((int) strlen(cTmp)) ; nI++)
						{
							if(Login.nLoginType == SERVER_ICC)
							{
								if((nI > 0) && ((cTmp [nI] >= '0') && (cTmp [nI] <= '9')))
								{
									bError = 0;
								}
								else if((nI > 0) && (cTmp [nI] == '-'))
								{
									bError = 0;
								}
								else
									bError = (! isalpha(cTmp [nI]));
							}
							else
							{
								bError = (! isalpha(cTmp [nI]));
							}
							if(bError)
							{
								TOOLBOX_Error("Invalid Handle.");
								SetFocus(hVar);
								return FALSE;
							}
						}
					}

					switch(Vars.nEditVar)
					{
						case 0 :
							break;

						case 1 :
							if(strcmp(Vars.cPartner, cTmp) != 0)
							{
								strcpy(Vars.cPartner, cTmp);
								strcpy(Vars.cPartnerTell, "");
								TOOLBOX_SetTelnetCaption();
								BUTTON_Refresh();
							}
							break;

						case 2 :
							if(strcmp(Vars.cOpponent, cTmp) != 0)
							{
								strcpy(Vars.cOpponent, cTmp);
							}
							break;

						case 3 :
							if(strcmp(Vars.cPartnerOpponent, cTmp) != 0)
							{
								strcpy(Vars.cPartnerOpponent, cTmp);
							}
							break;

						case 4 :
							if(strcmp(Vars.cLastTell, cTmp) != 0)
							{
								strcpy(Vars.cLastTell, cTmp);
							}
							break;

						case 5 :
							if(strcmp(Vars.cLastKib, cTmp) != 0)
							{
								strcpy(Vars.cLastKib, cTmp);
							}
							break;

						case 6 :
							if(strcmp(Vars.cLastWhisper, cTmp) != 0)
							{
								strcpy(Vars.cLastWhisper, cTmp);
							}
							break;

						case 7 :
							if(strcmp(Vars.cLastSay, cTmp) != 0)
							{
								strcpy(Vars.cLastSay, cTmp);
							}
							break;

						case 8 :
							if(strcmp(Vars.cTelnetHandle, cTmp) != 0)
							{
								strcpy(Vars.cTelnetHandle, cTmp);
							}
							break;

						case 9 :
							if(strcmp(Vars.cBoardHandle, cTmp) != 0)
							{
								strcpy(Vars.cBoardHandle, cTmp);
							}
							break;

						case 10 :
							if(strcmp(Vars.cUserHandle1, cTmp) != 0)
							{
								strcpy(Vars.cUserHandle1, cTmp);
							}
							break;

						case 11 :
							if(strcmp(Vars.cUserHandle2, cTmp) != 0)
							{
								strcpy(Vars.cUserHandle2, cTmp);
							}
							break;
					}

					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void VARS_Init(void)
{
	strcpy(Vars.cWhoAmI,          "");
	strcpy(Vars.cPartner,         "");
	strcpy(Vars.cOpponent,        "");
	strcpy(Vars.cPartnerOpponent, "");
	strcpy(Vars.cLastTell,        "");
	strcpy(Vars.cLastKib,         "");
	strcpy(Vars.cLastWhisper,     "");
	strcpy(Vars.cLastSay,         "");
	strcpy(Vars.cTelnetHandle,    "");
	strcpy(Vars.cBoardHandle,     "");
	strcpy(Vars.cUserHandle1,     "");
	strcpy(Vars.cUserHandle2,     "");
	strcpy(Vars.cPartnerTell,     "");
	strcpy(Vars.cPartnerTell1,    "");
	strcpy(Vars.cPartnerTell2,    "");
	strcpy(Vars.cFollow,          "");
	strcpy(Vars.cPfollow,         "");
	strcpy(Vars.cFollowGone,      "");
	strcpy(Vars.cFollowGone1,     "");
	strcpy(Vars.cFollowOn,        "");
	strcpy(Vars.cObserve,         "");
	strcpy(Vars.cExam,            "");

	Vars.nFollowGone  = 0;
	Vars.nFollowGone1 = 0;
	Vars.nFollowOn    = 0;
	Vars.nEditVar     = 0;
}
