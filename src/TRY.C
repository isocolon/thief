#include "thief.h"

BOOL CALLBACK TryBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hScreen, hTaskbar, hPlay;
	int nL, nS, nT, nP;
	char cTmp [2048];

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			hScreen  = GetDlgItem(hwnd, IDD_TRY_SCREEN);
			hTaskbar = GetDlgItem(hwnd, IDD_TRY_TASKBAR);
			hPlay    = GetDlgItem(hwnd, IDD_TRY_PLAY);

			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "01. Play/Observe One Game 1");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "02. Play/Observe One Game 2");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "03. Play/Observe One Game 3");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "04. Play Bughouse 1");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "05. Play Bughouse 2");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "06. Play Bughouse 3");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "07. Observe Bughouse 1");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "08. Observe Bughouse 2");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "09. Simul Bughouse Left");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "10. Simul Bughouse Right");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "11. Observe Three Games");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "12. Observe Four Games");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "13. Observe Five Games");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "14. Observe Six Games");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "15. Observe Eight Games");
			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);


			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_SETCURSEL, 0, 0);

			if((System.nTryList < 0) || (System.nTryList > 12))
			{
				System.nTryList = 0;
			}

			SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_SETCURSEL, (WPARAM) System.nTryList, 0);

			SendMessage(hScreen, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "Use Maximum Screen Resolution as Main Window");
			SendMessage(hScreen, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Use Current Main Window Size");
			SendMessage(hScreen, CB_ADDSTRING, 0, (LPARAM) cTmp);

			switch(System.nTryScreen)
			{
				case 0 :
					strcpy(cTmp, "Use Maximum Screen Resolution as Main Window");
					break;

				case 1 :
					strcpy(cTmp, "Use Current Main Window Size");
					break;

				default :
					strcpy(cTmp, "Use Maximum Screen Resolution as Main Window");
					System.nTryScreen = 0;
					break;
			}

			if(SendMessage(hScreen, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hScreen, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hScreen, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendMessage(hTaskbar, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "Main Window Stops at Taskbar Boundry");
			SendMessage(hTaskbar, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Main Window Overlaps Taskbar");
			SendMessage(hTaskbar, CB_ADDSTRING, 0, (LPARAM) cTmp);

			switch(System.nTryTaskbar)
			{
				case 0 :
					strcpy(cTmp, "Main Window Stops at Taskbar Boundry");
					break;

				case 1 :
					strcpy(cTmp, "Main Window Overlaps Taskbar");
					break;

				default :
					strcpy(cTmp, "Main Window Stops at Taskbar Boundry");
					System.nTryTaskbar = 0;
					break;
			}

			if(SendMessage(hTaskbar, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hTaskbar, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hTaskbar, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendMessage(hPlay, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "Move Playing Board to The Left");
			SendMessage(hPlay, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Move Playing Board to The Right");
			SendMessage(hPlay, CB_ADDSTRING, 0, (LPARAM) cTmp);

			switch(System.nTryPlay)
			{
				case 0 :
					strcpy(cTmp, "Move Playing Board to The Left");
					break;

				case 1 :
					strcpy(cTmp, "Move Playing Board to The Right");
					break;

				default :
					strcpy(cTmp, "Move Playing Board to The Left");
					System.nTryPlay = 0;
					break;
			}

			if(SendMessage(hPlay, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hPlay, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hPlay, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					nL = SendDlgItemMessage(hwnd, IDD_TRY_LIST, LB_GETCURSEL, 0, 0);
					System.nTryList = nL;

					nS = SendMessage(hScreen, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nS == CB_ERR)
					{
						nS = 0;
					}
					System.nTryScreen = nS;
					nS = (nS == 0);

					nT = SendMessage(hTaskbar, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nT == CB_ERR)
					{
						nT = 0;
					}
					System.nTryTaskbar = nT;
					nT = (nT == 1);

					nP = SendMessage(hPlay, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nP == CB_ERR)
					{
						nP = 0;
					}
					System.nTryPlay = nP;
					nP = (nP == 0);

					EndDialog(hwnd, TRUE);
					INI_WriteSystem(INI_GetSysFilename());
					STATE_LeaveDialogBox();

					switch(nL)
					{
						case 0 :    // play/observe one game 1
							LAYOUT_Try_14(nS, nT, nP);
							break;

						case 1 :    // play/observe one game 1
							LAYOUT_Try_8(nS, nT, nP);
							break;

						case 2 :    // play/observe one game 2
							LAYOUT_Try_9(nS, nT, nP);
							break;

						case 3 :    // play bughouse 1
							LAYOUT_Try_1(nS, nT, nP);
							break;

						case 4 :    // play bughouse 2
							LAYOUT_Try_2(nS, nT, nP);
							break;

						case 5 :    // play bughouse 3
							LAYOUT_Try_3(nS, nT, nP);
							break;

						case 6 :    // observe bughouse 1
							LAYOUT_Try_4(nS, nT, nP);
							break;

						case 7 :    // observer bughouse 2
							LAYOUT_Try_5(nS, nT, nP);
							break;

						case 8 :    // simul bughouse left
							LAYOUT_Try_6(nS, nT, nP);
							break;

						case 9 :    // simul bughouse right
							LAYOUT_Try_7(nS, nT, nP);
							break;

						case 10 :   // observe three games
							LAYOUT_Try_10(nS, nT, nP);
							break;

						case 11 :   // observe four games
							LAYOUT_Try_11(nS, nT, nP);
							break;

						case 12 :   // observe five games
							LAYOUT_Try_12(nS, nT, nP);
							break;

						case 13 :   // observe six games
							LAYOUT_Try_13(nS, nT, nP);
							break;

						case 14 :   // observe eight games
							LAYOUT_Try_15(nS, nT, nP);
							break;
					}

					TOOLBOX_CheckAllMenu();

					System.bOkToResizePlay = 1;
					TOOLBOX_TryAllWindowCoord();
					System.bOkToResizePlay = 0;
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
