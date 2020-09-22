#include "thief.h"

BOOL CALLBACK PartnerAssistBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hType;
	char cTmp [2048];
	int nI;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			ASSIST_Adjust();

			hType = GetDlgItem(hwnd, IDD_ASSIST_TYPE);

			SendMessage(hType, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "Use Long Algebraic Notation");
			SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Use Short Algebraic Notation");
			SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

			if(User.bLongPartner)
			{
				strcpy(cTmp, "Use Long Algebraic Notation");
			}
			else
			{
				strcpy(cTmp, "Use Short Algebraic Notation");
			}
			if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			strcpy(cTmp, User.cPartnerMove1);
			SendDlgItemMessage(hwnd, IDD_ASSIST_1, WM_SETTEXT, 0, (LPARAM) cTmp);

			strcpy(cTmp, User.cPartnerMove2);
			SendDlgItemMessage(hwnd, IDD_ASSIST_2, WM_SETTEXT, 0, (LPARAM) cTmp);

			strcpy(cTmp, User.cPartnerMove3);
			SendDlgItemMessage(hwnd, IDD_ASSIST_3, WM_SETTEXT, 0, (LPARAM) cTmp);

			strcpy(cTmp, User.cPartnerMove4);
			SendDlgItemMessage(hwnd, IDD_ASSIST_4, WM_SETTEXT, 0, (LPARAM) cTmp);

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					nI = SendMessage(hType, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						User.bLongPartner = 0;
					}
					else if(nI == 0)
					{
						User.bLongPartner = 1;
					}
					else
					{
						User.bLongPartner = 0;
					}

					SendDlgItemMessage(hwnd, IDD_ASSIST_1, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					strcpy(User.cPartnerMove1, cTmp);

					SendDlgItemMessage(hwnd, IDD_ASSIST_2, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					strcpy(User.cPartnerMove2, cTmp);

					SendDlgItemMessage(hwnd, IDD_ASSIST_3, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					strcpy(User.cPartnerMove3, cTmp);

					SendDlgItemMessage(hwnd, IDD_ASSIST_4, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					strcpy(User.cPartnerMove4, cTmp);

					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_ASSIST_DEFAULT :
					strcpy(cTmp, "Use short algebraic notation");
					if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					strcpy(cTmp, DEFAULT_PARTNER_MOVE1);
					SendDlgItemMessage(hwnd, IDD_ASSIST_1, WM_SETTEXT, 0, (LPARAM) cTmp);

					strcpy(cTmp, DEFAULT_PARTNER_MOVE2);
					SendDlgItemMessage(hwnd, IDD_ASSIST_2, WM_SETTEXT, 0, (LPARAM) cTmp);

					strcpy(cTmp, DEFAULT_PARTNER_MOVE3);
					SendDlgItemMessage(hwnd, IDD_ASSIST_3, WM_SETTEXT, 0, (LPARAM) cTmp);

					strcpy(cTmp, DEFAULT_PARTNER_MOVE4);
					SendDlgItemMessage(hwnd, IDD_ASSIST_4, WM_SETTEXT, 0, (LPARAM) cTmp);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void ASSIST_Init(void)
{
	strcpy(User.cPartnerMove1, DEFAULT_PARTNER_MOVE1);
	strcpy(User.cPartnerMove2, DEFAULT_PARTNER_MOVE2);
	strcpy(User.cPartnerMove3, DEFAULT_PARTNER_MOVE3);
	strcpy(User.cPartnerMove4, DEFAULT_PARTNER_MOVE4);
}

void ASSIST_Adjust(void)
{
	TOOLBOX_AllTrim(User.cPartnerMove1);
	TOOLBOX_AllTrim(User.cPartnerMove2);
	TOOLBOX_AllTrim(User.cPartnerMove3);
	TOOLBOX_AllTrim(User.cPartnerMove4);
}
