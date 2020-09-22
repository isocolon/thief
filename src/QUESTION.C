#include "thief.h"

BOOL CALLBACK QuestionBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hQuestion ;
	char cTmp [4096 + 10], cTmp1 [6000 + 10] ;
	int nS ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			hQuestion = GetDlgItem(hwnd, IDD_QUESTION) ;
			SendDlgItemMessage(hwnd, IDD_QUESTION, WM_SETTEXT, 0, (LPARAM) "") ;
			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					SendDlgItemMessage(hwnd, IDD_QUESTION, WM_GETTEXT, (WPARAM) 4096, (LPARAM) cTmp) ;
					TOOLBOX_AllTrim(cTmp) ;
					if(strlen(cTmp) > 0)
					{
						switch(Login.nLoginType)
						{
							case SERVER_FICS :
								if(User.bIamAGuest)
								{
									sprintf(cTmp1, "%s%s\n", FICS_QUESTION4_COMMAND, cTmp) ;
								}
								else
								{
									sprintf(cTmp1, "%s%s\n", FICS_QUESTION1_COMMAND, cTmp) ;
								}
								break ;

							case SERVER_ICC :
								sprintf(cTmp1, "%s%s\n", ICC_QUESTION_COMMAND, cTmp) ;
								break ;

							case SERVER_NONFICS :
								sprintf(cTmp1, "%s%s\n", NONFICS_QUESTION_COMMAND, cTmp) ;
								break ;

							default :
								if(User.bIamAGuest)
								{
									sprintf(cTmp1, "%s%s\n", FICS_QUESTION4_COMMAND, cTmp) ;
								}
								else
								{
									sprintf(cTmp1, "%s%s\n", FICS_QUESTION1_COMMAND, cTmp) ;
								}
								break ;
						}

						if(Timeseal.bSocketIsOpen)
						{
							TOOLBOX_WriteICS(cTmp1) ;
						}

						TOOLBOX_WriteUser(cTmp1) ;

						nS = strlen(cTmp1) ;
						if(cTmp1 [nS - 1] == '\n')
						{
							cTmp1 [nS - 1] = NULL_CHAR ;
						}

						if(User.bQuestionCommandAddHist)
						{
							HISTORY_Add(cTmp1) ;
						}

						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}

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
