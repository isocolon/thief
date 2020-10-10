#include "thief.h"

int  RM_Type [MAX_RM_ITEM];
char RM_Menu [MAX_RM_ITEM] [MAX_RM_MENU_SIZE];
char RM_oMenu [MAX_RM_ITEM] [MAX_RM_MENU_SIZE];
char RM_Command [MAX_RM_ITEM] [MAX_RM_COMMAND_SIZE];

BOOL CALLBACK RightMouseClickBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hMenu, hFunction;
	static int nSel, nCount, nMode;
	int nF, nI, nType;
	char cTmp [2048], cMenu [2048], cCommand [2048];

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			// get window
			hMenu     = GetDlgItem(hwnd, IDD_RM_MENU);
			hFunction = GetDlgItem(hwnd, IDD_RM_FUNCTION);

			// set window title
			switch(System.nRightMouseIndex)
			{
				case RIGHT_MOUSE_BUTTON :
					SetWindowText(hwnd, "Configure Right Mouse Button Click on Partner Communicator Buttons");
					break;

				case RIGHT_MOUSE_PLAY :
					SetWindowText(hwnd, "Configure Right Mouse Button Click on Playing Board");
					break;

				case RIGHT_MOUSE_OBSERVE :
					SetWindowText(hwnd, "Configure Right Mouse Button Click on Observe Board");
					break;

				case RIGHT_MOUSE_TELNET_TEXT :
					SetWindowText(hwnd, "Configure Right Mouse Button Click on Telnet Console Text");
					break;

				case RIGHT_MOUSE_TELNET_EDIT :
					SetWindowText(hwnd, "Configure Right Mouse Button Click on Telnet Console Command Line");
					break;

				case RIGHT_MOUSE_BOARD_HANDLE :
					SetWindowText(hwnd, "Configure Right Mouse Button Click on Handle on Game Board");
					break;

				default :
					System.nRightMouseIndex = RIGHT_MOUSE_BUTTON;
					SetWindowText(hwnd, "Configure Right Mouse Button Click on Partner Communicator Buttons");
					break;
			}

			// save into temporary arrays
			for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
			{
				RM_Type [nI] = RightMouse [System.nRightMouseIndex].nType [nI];
				strcpy(RM_Menu [nI], RightMouse [System.nRightMouseIndex].cMenu [nI]);
				strcpy(RM_oMenu [nI], RightMouse [System.nRightMouseIndex].cMenu [nI]);
				strcpy(RM_Command [nI], RightMouse [System.nRightMouseIndex].cCommand [nI]);

				if((RM_Type [nI] < 0) || (RM_Type [nI] >= MAX_FMENU))
				{
					RM_Type [nI] = FUNCTION_NONE;
				}

				if(! TOOLBOX_NeedCommandValue(RM_Type [nI]))
				{
					strcpy(RM_Command [nI], "");
				}
			}

			// show list
			nCount = 0;
			SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
			{
				if(strlen(RM_Menu [nI]) == 0)
				{
					break;
				}

				nCount = nCount + 1;

				sprintf(cTmp, "%02d. %s", nI + 1, RM_Menu [nI]);
				SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			// show item zero
			nSel = 0;

			SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) RM_Menu [nSel]);

			SendMessage(hFunction, CB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_FMENU ; nI++)
			{
				strcpy(cTmp, FString [nI]);
				SendMessage(hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			strcpy(cTmp, FString [RM_Type [nSel]]);
			if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) RM_Command [nSel]);

			SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);

			// disable all input items
			EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
			EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
			EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

			// disable "new" button if there are already to the maximum items
			if(nCount >= MAX_RM_ITEM)
			{
				EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
			}

			// disable "edit" and "delete" buttons if there are no items
			if(nCount <= 0)
			{
				EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
				EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
			}

			// assign input mode
			nMode = 0;

			// centralize window
			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_RM_LIST :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nSel = SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_GETCURSEL, 0, 0);

						SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) RM_Menu [nSel]);

						strcpy(cTmp, FString [RM_Type [nSel]]);
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) RM_Command [nSel]);

						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);
					}
					break;

				case IDD_RM_FUNCTION :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nF == CB_ERR)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND), TOOLBOX_NeedCommandValue(nF));
						}
					}
					break;

				case IDD_RM_NEW :
					// disable list box
					EnableWindow(GetDlgItem(hwnd, IDD_RM_LIST), 0);

					// hide "new", "edit", "delete", "default", "up", and "down" buttons
					ShowWindow(GetDlgItem(hwnd, IDD_RM_NEW),     SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_EDIT),    SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_DELETE),  SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_DEFAULT), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_UP),      SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_DOWN),    SW_HIDE);

					// enable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     1);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 1);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  1);

					// assign input mode
					nMode = 1;

					nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nF == CB_ERR)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND), TOOLBOX_NeedCommandValue(nF));
					}
					break;

				case IDD_RM_EDIT :
					nCount = 0;
					for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
					{
						if(strlen(RM_Menu [nI]) == 0)
						{
							break;
						}

						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Right Mouse Click Event to Delete.");
						return FALSE;
					}

					// disable list box
					EnableWindow(GetDlgItem(hwnd, IDD_RM_LIST), 0);

					// hide "new", "edit", "delete", "default", "up", and "down" buttons
					ShowWindow(GetDlgItem(hwnd, IDD_RM_NEW),     SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_EDIT),    SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_DELETE),  SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_DEFAULT), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_UP),      SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_RM_DOWN),    SW_HIDE);

					// enable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     1);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 1);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  1);

					// assign input mode
					nMode = 2;

					nSel = SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_GETCURSEL, 0, 0);

					EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND), TOOLBOX_NeedCommandValue(RM_Type [nSel]));
					break;

				case IDD_RM_DELETE :
					nCount = 0;
					for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
					{
						if(strlen(RM_Menu [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Right Mouse Click Event to Delete.");
						return FALSE;
					}

					if(nSel >= nCount)
					{
						TOOLBOX_Error("No Right Mouse Click Event Selected.");
						return FALSE;
					}

					for(nI = nSel ; nI < MAX_RM_ITEM ; nI++)
					{
						strcpy(RM_Menu [nI], RM_Menu [nI + 1]);
						RM_Type [nI] = RM_Type [nI + 1];
						strcpy(RM_Command [nI], RM_Command [nI + 1]);
					}

					// show list again
					nCount = 0;
					SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_RESETCONTENT, 0, 0);
					for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
					{
						if(strlen(RM_Menu [nI]) == 0)
						{
							break;
						}

						nCount = nCount + 1;

						sprintf(cTmp, "%02d. %s", nI + 1, RM_Menu [nI]);
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(RM_oMenu [nI], RM_Menu [nI]);
					}

					if(nCount > 0)
					{
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, 0, 0);

						// reset rest
						for(nI = nCount ; nI < MAX_RM_ITEM ; nI++)
						{
							strcpy(RM_Menu [nI], "");
							RM_Type [nI] = FUNCTION_NONE;
							strcpy(RM_Command [nI], "");
						}

						if(nSel >= nCount)
						{
							nSel = (nCount - 1);
						}

						// show item
						SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) RM_Menu [nSel]);

						strcpy(cTmp, FString [RM_Type [nSel]]);
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) RM_Command [nSel]);

						// select item
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);
					}
					else
					{
						nSel = 0;

						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, 0, 0);

						SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) "");

						strcpy(cTmp, FString [FUNCTION_NONE]);
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) "");
					}

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_RM_ITEM)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDD_RM_DEFAULT :
					// set it to default here...
					RIGHTMOUSE_Init1();

					// show list again
					nCount = 0;
					SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_RESETCONTENT, 0, 0);
					for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
					{
						if(strlen(RM_Menu [nI]) == 0)
						{
							break;
						}

						nCount = nCount + 1;

						sprintf(cTmp, "%02d. %s", nI + 1, RM_Menu [nI]);
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(RM_oMenu [nI], RM_Menu [nI]);
					}
					SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, 0, 0);

					// show item zero
					SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) RM_Menu [0]);

					strcpy(cTmp, FString [RM_Type [0]]);
					if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) RM_Command [0]);

					nSel = 0;

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_RM_ITEM)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDD_RM_UP :
					nCount = 0;
					for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
					{
						if(strlen(RM_Menu [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Right Mouse Click Event to Move.");
						return FALSE;
					}

					if(nSel > 0)
					{
						nType = RM_Type [nSel] ;
						strcpy(cMenu,    RM_Menu    [nSel]);
						strcpy(cCommand, RM_Command [nSel]);

						RM_Type [nSel] = RM_Type [nSel - 1];
						strcpy(RM_Menu    [nSel], RM_Menu    [nSel - 1]);
						strcpy(RM_Command [nSel], RM_Command [nSel - 1]);

						RM_Type [nSel - 1] = nType;
						strcpy(RM_Menu    [nSel - 1], cMenu);
						strcpy(RM_Command [nSel - 1], cCommand);

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if(strlen(RM_Menu [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							sprintf(cTmp, "%02d. %s", nI + 1, RM_Menu [nI]);
							SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(RM_oMenu [nI], RM_Menu [nI]);
						}

						nSel = nSel - 1;
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);
					}

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_RM_ITEM)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDD_RM_DOWN :
					nCount = 0;
					for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
					{
						if(strlen(RM_Menu [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Right Mouse Click Event to Move.");
						return FALSE;
					}

					if((nSel + 1) < nCount)
					{
						nType = RM_Type [nSel + 1] ;
						strcpy(cMenu,    RM_Menu    [nSel + 1]);
						strcpy(cCommand, RM_Command [nSel + 1]);

						RM_Type [nSel + 1] = RM_Type [nSel];
						strcpy(RM_Menu    [nSel + 1], RM_Menu    [nSel]);
						strcpy(RM_Command [nSel + 1], RM_Command [nSel]);

						RM_Type [nSel] = nType;
						strcpy(RM_Menu    [nSel], cMenu);
						strcpy(RM_Command [nSel], cCommand);

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if(strlen(RM_Menu [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							sprintf(cTmp, "%02d. %s", nI + 1, RM_Menu [nI]);
							SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(RM_oMenu [nI], RM_Menu [nI]);
						}

						nSel = nSel + 1;
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);
					}

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_RM_ITEM)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDOK :
					if(nMode == 1)
					{
						nCount = 0;
						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if(strlen(RM_Menu [nI]) == 0)
							{
								break;
							}
							nCount = nCount + 1;
						}

						if(nCount >= MAX_RM_ITEM)
						{
							sprintf(cTmp, "You Have Reached The Maximum Number of Right Mouse Click Events (%d).", MAX_RM_ITEM);
							TOOLBOX_Error(cTmp);
							return FALSE;
						}

						SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_GETTEXT, (WPARAM) MAX_RM_MENU_SIZE - 1, (LPARAM) cTmp);
						TOOLBOX_AllTrim(cTmp);
						if(strlen(cTmp) == 0)
						{
							TOOLBOX_Error("Missing Menu.");
							SetFocus(hMenu);
							return FALSE;
						}
						strcpy(cMenu, cTmp);

						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nF == CB_ERR)
						{
							nType = FUNCTION_NONE;
						}
						else
						{
							nType = nF;
						}

						if(TOOLBOX_NeedCommandValue(nType))
						{
							SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_GETTEXT, (WPARAM) MAX_RM_COMMAND_SIZE - 1, (LPARAM) cTmp);
							strcpy(cCommand, cTmp);
						}
						else
						{
							strcpy(cCommand, "");
						}

						// add it into the array
						strcpy(RM_Menu [nCount], cMenu);
						RM_Type [nCount] = nType;
						strcpy(RM_Command [nCount], cCommand);

						nCount = nCount + 1;

						// reset rest
						for(nI = nCount ; nI < MAX_RM_ITEM ; nI++)
						{
							strcpy(RM_Menu [nI], "");
							RM_Type [nI] = FUNCTION_NONE;
							strcpy(RM_Command [nI], "");
						}

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if(strlen(RM_Menu [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							sprintf(cTmp, "%02d. %s", nI + 1, RM_Menu [nI]);
							SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(RM_oMenu [nI], RM_Menu [nI]);
						}
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, 0, 0);

						nSel = nCount - 1;

						// show item just added
						SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) RM_Menu [nSel]);

						strcpy(cTmp, FString [RM_Type [nSel]]);
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) RM_Command [nSel]);

						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);

						// disable all input items
						EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

						// enable list box
						EnableWindow(GetDlgItem(hwnd, IDD_RM_LIST), 1);

						// show "new", "edit", "delete", "default", "up", and "down" buttons
						ShowWindow(GetDlgItem(hwnd, IDD_RM_NEW),     SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_EDIT),    SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DELETE),  SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DEFAULT), SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_UP),      SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DOWN),    SW_SHOW);

						// disable "new" button if there are already to the maximum items
						if(nCount >= MAX_RM_ITEM)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 1);
						}

						// disable "edit" and "delete" buttons if there are no items
						if(nCount <= 0)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
							EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   1);
							EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 1);
						}

						// assign input mode
						nMode = 0;
					}
					else if(nMode == 2)
					{
						nSel = SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_GETCURSEL, 0, 0);

						SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_GETTEXT, (WPARAM) MAX_RM_MENU_SIZE - 1, (LPARAM) cTmp);
						TOOLBOX_AllTrim(cTmp);
						if(strlen(cTmp) == 0)
						{
							TOOLBOX_Error("Missing Menu.");
							SetFocus(hMenu);
							return FALSE;
						}
						strcpy(RM_Menu [nSel], cTmp);

						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nF == CB_ERR)
						{
							RM_Type [nSel] = FUNCTION_NONE;
						}
						else
						{
							RM_Type [nSel] = nF;
						}

						if(TOOLBOX_NeedCommandValue(RM_Type [nSel]))
						{
							SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_GETTEXT, (WPARAM) MAX_RM_COMMAND_SIZE - 1, (LPARAM) cTmp);
							strcpy(RM_Command [nSel], cTmp);
						}
						else
						{
							strcpy(RM_Command [nSel], "");
						}

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if(strlen(RM_Menu [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							sprintf(cTmp, "%02d. %s", nI + 1, RM_Menu [nI]);
							SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(RM_oMenu [nI], RM_Menu [nI]);
						}
						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, 0, 0);

						// show item just added
						SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) RM_Menu [nSel]);

						strcpy(cTmp, FString [RM_Type [nSel]]);
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) RM_Command [nSel]);

						SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);

						// disable all input items
						EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

						// enable list box
						EnableWindow(GetDlgItem(hwnd, IDD_RM_LIST), 1);

						// show "new", "edit", "delete", "default", "up", and "down" buttons
						ShowWindow(GetDlgItem(hwnd, IDD_RM_NEW),     SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_EDIT),    SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DELETE),  SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DEFAULT), SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_UP),      SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DOWN),    SW_SHOW);

						// disable "new" button if there are already to the maximum items
						if(nCount >= MAX_RM_ITEM)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 1);
						}

						// disable "edit" and "delete" buttons if there are no items
						if(nCount <= 0)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
							EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   1);
							EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 1);
						}

						// assign input mode
						nMode = 0;
					}
					else
					{
						nCount = 0;
						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if(strlen(RM_Menu [nI]) == 0)
							{
								break;
							}
							nCount = nCount + 1;
						}

						if(nCount > 0)
						{
							nSel = SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_GETCURSEL, 0, 0);

							SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_GETTEXT, (WPARAM) MAX_RM_MENU_SIZE - 1, (LPARAM) cTmp);
							TOOLBOX_AllTrim(cTmp);
							if(strlen(cTmp) == 0)
							{
								TOOLBOX_Error("Missing Menu.");
								SetFocus(hMenu);
								return FALSE;
							}
							strcpy(RM_Menu [nSel], cTmp);

							nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
							if(nF == CB_ERR)
							{
								RM_Type [nSel] = FUNCTION_NONE;
							}
							else
							{
								RM_Type [nSel] = nF;
							}

							if(TOOLBOX_NeedCommandValue(RM_Type [nSel]))
							{
								SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_GETTEXT, (WPARAM) MAX_RM_COMMAND_SIZE - 1, (LPARAM) cTmp);
								strcpy(RM_Command [nSel], cTmp);
							}
							else
							{
								strcpy(RM_Command [nSel], "");
							}
						}

						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if((RM_Type [nI] < 0) || (RM_Type [nI] >= MAX_FMENU))
							{
								RM_Type [nI] = FUNCTION_NONE;
							}

							if(! TOOLBOX_NeedCommandValue(RM_Type [nI]))
							{
								strcpy(RM_Command [nI], "");
							}

							RightMouse [System.nRightMouseIndex].nType [nI] = RM_Type [nI];
							strcpy(RightMouse [System.nRightMouseIndex].cMenu [nI], RM_Menu [nI]);
							strcpy(RightMouse [System.nRightMouseIndex].cCommand [nI], RM_Command [nI]);
						}
						RIGHTMOUSE_Adjust();
						EndDialog(hwnd, TRUE);
						STATE_LeaveDialogBox();
						return TRUE;
					}
					break;

				case IDCANCEL :
					if(nMode == 0)
					{
						EndDialog(hwnd, TRUE);
						STATE_LeaveDialogBox();
						return TRUE;
					}
					else
					{
						nCount = 0;
						for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
						{
							if(strlen(RM_Menu [nI]) == 0)
							{
								break;
							}
							nCount = nCount + 1;
						}

						if(nCount > 0)
						{
							SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, 0, 0);

							if(nSel >= nCount)
							{
								nSel = (nCount - 1);
							}

							// show item
							SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) RM_Menu [nSel]);

							strcpy(cTmp, FString [RM_Type [nSel]]);
							if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
							{
								SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
								SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
							}

							SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) RM_Command [nSel]);

							// select item
							SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, (WPARAM) nSel, 0);
						}
						else
						{
							nSel = 0;

							SendDlgItemMessage(hwnd, IDD_RM_LIST, LB_SETCURSEL, 0, 0);

							SendDlgItemMessage(hwnd, IDD_RM_MENU, WM_SETTEXT, 0, (LPARAM) "");

							strcpy(cTmp, FString [FUNCTION_NONE]);
							if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
							{
								SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
								SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
							}

							SendDlgItemMessage(hwnd, IDD_RM_COMMAND, WM_SETTEXT, 0, (LPARAM) "");
						}

						// disable all input items
						EnableWindow(GetDlgItem(hwnd, IDD_RM_MENU),     0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_FUNCTION), 0);
						EnableWindow(GetDlgItem(hwnd, IDD_RM_COMMAND),  0);

						// enable list box
						EnableWindow(GetDlgItem(hwnd, IDD_RM_LIST), 1);

						// show "new", "edit", "delete", "default", "up", and "down" buttons
						ShowWindow(GetDlgItem(hwnd, IDD_RM_NEW),     SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_EDIT),    SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DELETE),  SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DEFAULT), SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_UP),      SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_RM_DOWN),    SW_SHOW);

						// disable "new" button if there are already to the maximum items
						if(nCount >= MAX_RM_ITEM)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_NEW), 1);
						}

						// disable "edit" and "delete" buttons if there are no items
						if(nCount <= 0)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   0);
							EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_RM_EDIT),   1);
							EnableWindow(GetDlgItem(hwnd, IDD_RM_DELETE), 1);
						}

						// assign input mode
						nMode = 0;
					}
					break;
			}
			break;
	}
	return FALSE;
}

void RIGHTMOUSE_Init(void)
{
	int nI, nJ;

	for(nI = 0 ; nI < MAX_RM ; nI++)
	{
		for(nJ = 0 ; nJ < MAX_RM_ITEM ; nJ++)
		{
			strcpy(RightMouse [nI].cMenu [nJ], "");
			RightMouse [nI].nType [nJ] = FUNCTION_NONE;
			strcpy(RightMouse [nI].cCommand [nJ], "");
		}
	}

	//
	// button
	//

	//
	// play
	//

	// reset board
	strcpy(RightMouse [RIGHT_MOUSE_PLAY].cMenu [0], "Reset Board");
	RightMouse [RIGHT_MOUSE_PLAY].nType [0] = FUNCTION_RESET_BOARD;

	// flip board
	strcpy(RightMouse [RIGHT_MOUSE_PLAY].cMenu [1], "Flip Board");
	RightMouse [RIGHT_MOUSE_PLAY].nType [1] = FUNCTION_FLIP_BOARD;

	// unexamine game
	strcpy(RightMouse [RIGHT_MOUSE_PLAY].cMenu [2], "Unexamine Game");
	RightMouse [RIGHT_MOUSE_PLAY].nType [2] = FUNCTION_UNEXAM_GAME;

	// close board
	strcpy(RightMouse [RIGHT_MOUSE_PLAY].cMenu [3], "Close Board");
	RightMouse [RIGHT_MOUSE_PLAY].nType [3] = FUNCTION_CLOSE_BOARD;

	//
	// observe
	//

	// reset board
	strcpy(RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [0], "Reset Board");
	RightMouse [RIGHT_MOUSE_OBSERVE].nType [0] = FUNCTION_RESET_BOARD;

	// flip board
	strcpy(RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [1], "Flip Board");
	RightMouse [RIGHT_MOUSE_OBSERVE].nType [1] = FUNCTION_FLIP_BOARD;

	// close board
	strcpy(RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [2], "Close Board");
	RightMouse [RIGHT_MOUSE_OBSERVE].nType [2] = FUNCTION_CLOSE_BOARD;

	//
	// telnet text
	//

	// finger
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [0], "%s %s", ICS_FINGER_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [0] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [0], "%s %s", ICS_FINGER_COMMAND, VAR_TELNETHANDLE);

	// variable
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [1] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_TELNETHANDLE);

	// ping
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [2], "%s %s", ICS_PING_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [2] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [2], "%s %s", ICS_PING_COMMAND, VAR_TELNETHANDLE);

	// match
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [3], "%s%s", ICS_MATCH_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [3] = FUNCTION_MATCH_BOX;

	// observe
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [4], "%s %s", ICS_OBSERVE_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [4] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [4], "%s %s", ICS_OBSERVE_COMMAND, VAR_TELNETHANDLE);

	// follow
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [5], "%s %s", ICS_FOLLOW_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [5] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [5], "%s %s", ICS_FOLLOW_COMMAND, VAR_TELNETHANDLE);

	// partner
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [6], "%s %s", ICS_PARTNER_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [6] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [6], "%s %s", ICS_PARTNER_COMMAND, VAR_TELNETHANDLE);

	// message
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [7], "%s %s", ICS_MESSAGE_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [7] = FUNCTION_MESSAGE_BOX;

	// history
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [8], "%s %s", ICS_HISTORY_COMMAND, VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [8] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [8], "%s %s", ICS_HISTORY_COMMAND, VAR_TELNETHANDLE);

	// edit %partner%
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [9], "Edit Partner");
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [9] = FUNCTION_EDIT_PARTNER_BOX;

	// copy
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [10], "Copy");
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [10] = FUNCTION_TELNET_TEXT_COPY;

	// copy and paste
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [11], "Copy and Paste");
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [11] = FUNCTION_TELNET_TEXT_CP;

	// select all
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [12], "Select All");
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [12] = FUNCTION_TELNET_TEXT_ALL;

	// save text to file
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [13], "Save Text to File");
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [13] = FUNCTION_TELNET_TEXT_SF;

	// tell to handle
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [14], "Tell %s", VAR_TELNETHANDLE);
	RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [14] = FUNCTION_TELNET_TELL_HANDLE;
	sprintf(RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [14], "Tell %s ", VAR_TELNETHANDLE);

	//
	// telnet edit
	//

	// cut
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [0], "Cut");
	RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType [0] = FUNCTION_TELNET_EDIT_CUT;

	// copy
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [1], "Copy");
	RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType [1] = FUNCTION_TELNET_EDIT_COPY;

	// paste
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [2], "Paste");
	RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType [2] = FUNCTION_TELNET_EDIT_PASTE;

	// select all
	strcpy(RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [3], "Select All");
	RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType [3] = FUNCTION_TELNET_EDIT_ALL;

	//
	// board handle
	//

	// all
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [0], "%s %s", ICS_ALL_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [0] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [0], "%s %s", ICS_ALL_COMMAND, VAR_BOARDHANDLE);

	// variable
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [1] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_BOARDHANDLE);

	// ping
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [2], "%s %s", ICS_PING_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [2] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [2], "%s %s", ICS_PING_COMMAND, VAR_BOARDHANDLE);

	// finger
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [3], "%s %s", ICS_FINGER_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [3] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [3], "%s %s", ICS_FINGER_COMMAND, VAR_BOARDHANDLE);

	// follow
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [4], "%s %s", ICS_FOLLOW_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [4] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [4], "%s %s", ICS_FOLLOW_COMMAND, VAR_BOARDHANDLE);

	// history
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [5], "%s %s", ICS_HISTORY_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [5] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [5], "%s %s", ICS_HISTORY_COMMAND, VAR_BOARDHANDLE);

	// observe
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [6], "%s %s", ICS_OBSERVE_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [6] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [6], "%s %s", ICS_OBSERVE_COMMAND, VAR_BOARDHANDLE);

	// unobserve
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [7], "%s %s", ICS_UNOBSERVE_COMMAND, VAR_BOARDHANDLE);
	RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [7] = FUNCTION_COMMAND;
	sprintf(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [7], "%s %s", ICS_UNOBSERVE_COMMAND, VAR_BOARDHANDLE);

	//
	// initialize FCheck array
	//
	for(nI = 0 ; nI < MAX_FMENU ; nI++)
	{
		FCheck [nI] = 0;
	}
}

void RIGHTMOUSE_Init1(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
	{
		strcpy(RM_Menu [nI], "");
		RM_Type [nI] = FUNCTION_NONE;
		strcpy(RM_Command [nI], "");
	}

	switch(System.nRightMouseIndex)
	{
		//
		// button
		//
		case RIGHT_MOUSE_BUTTON :
			break;

		//
		// play
		//
		case RIGHT_MOUSE_PLAY :
			// reset board
			strcpy(RM_Menu [0], "Reset Board");
			RM_Type [0] = FUNCTION_RESET_BOARD;

			// flip board
			strcpy(RM_Menu [1], "Flip Board");
			RM_Type [1] = FUNCTION_FLIP_BOARD;

			// unexamine game
			strcpy(RM_Menu [2], "Unexamine Game");
			RM_Type [2] = FUNCTION_UNEXAM_GAME;

			// close board
			strcpy(RM_Menu [3], "Close Board");
			RM_Type [3] = FUNCTION_CLOSE_BOARD;
			break;

		//
		// observe
		//
		case RIGHT_MOUSE_OBSERVE :
			// reset board
			strcpy(RM_Menu [0], "Reset Board");
			RM_Type [0] = FUNCTION_RESET_BOARD;

			// flip board
			strcpy(RM_Menu [1], "Flip Board");
			RM_Type [1] = FUNCTION_FLIP_BOARD;

			// close board
			strcpy(RM_Menu [2], "Close Board");
			RM_Type [2] = FUNCTION_CLOSE_BOARD;
			break;

		//
		// telnet text
		//
		case RIGHT_MOUSE_TELNET_TEXT :
			// finger
			sprintf(RM_Menu [0], "%s %s", ICS_FINGER_COMMAND, VAR_TELNETHANDLE);
			RM_Type [0] = FUNCTION_COMMAND;
			sprintf(RM_Command [0], "%s %s", ICS_FINGER_COMMAND, VAR_TELNETHANDLE);

			// variable
			sprintf(RM_Menu [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_TELNETHANDLE);
			RM_Type [1] = FUNCTION_COMMAND;
			sprintf(RM_Command [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_TELNETHANDLE);

			// ping
			sprintf(RM_Menu [2], "%s %s", ICS_PING_COMMAND, VAR_TELNETHANDLE);
			RM_Type [2] = FUNCTION_COMMAND;
			sprintf(RM_Command [2], "%s %s", ICS_PING_COMMAND, VAR_TELNETHANDLE);

			// match
			sprintf(RM_Menu [3], "%s%s", ICS_MATCH_COMMAND, VAR_TELNETHANDLE);
			RM_Type [3] = FUNCTION_MATCH_BOX;

			// observe
			sprintf(RM_Menu [4], "%s %s", ICS_OBSERVE_COMMAND, VAR_TELNETHANDLE);
			RM_Type [4] = FUNCTION_COMMAND;
			sprintf(RM_Command [4], "%s %s", ICS_OBSERVE_COMMAND, VAR_TELNETHANDLE);

			// follow
			sprintf(RM_Menu [5], "%s %s", ICS_FOLLOW_COMMAND, VAR_TELNETHANDLE);
			RM_Type [5] = FUNCTION_COMMAND;
			sprintf(RM_Command [5], "%s %s", ICS_FOLLOW_COMMAND, VAR_TELNETHANDLE);

			// partner
			sprintf(RM_Menu [6], "%s %s", ICS_PARTNER_COMMAND, VAR_TELNETHANDLE);
			RM_Type [6] = FUNCTION_COMMAND;
			sprintf(RM_Command [6], "%s %s", ICS_PARTNER_COMMAND, VAR_TELNETHANDLE);

			// message
			sprintf(RM_Menu [7], "%s %s", ICS_MESSAGE_COMMAND, VAR_TELNETHANDLE);
			RM_Type [7] = FUNCTION_MESSAGE_BOX;

			// history
			sprintf(RM_Menu [8], "%s %s", ICS_HISTORY_COMMAND, VAR_TELNETHANDLE);
			RM_Type [8] = FUNCTION_COMMAND;
			sprintf(RM_Command [8], "%s %s", ICS_HISTORY_COMMAND, VAR_TELNETHANDLE);

			// edit %partner%
			strcpy(RM_Menu [9], "Edit Partner");
			RM_Type [9] = FUNCTION_EDIT_PARTNER_BOX;

			// copy
			strcpy(RM_Menu [10], "Copy");
			RM_Type [10] = FUNCTION_TELNET_TEXT_COPY;

			// copy and paste
			strcpy(RM_Menu [11], "Copy and Paste");
			RM_Type [11] = FUNCTION_TELNET_TEXT_CP;

			// select all
			strcpy(RM_Menu [12], "Select All");
			RM_Type [12] = FUNCTION_TELNET_TEXT_ALL;

			// save text to file
			strcpy(RM_Menu [13], "Save Text to File");
			RM_Type [13] = FUNCTION_TELNET_TEXT_SF;

			// tell HANDLE
			sprintf(RM_Menu [14], "Tell %s", VAR_TELNETHANDLE);
			RM_Type [14] = FUNCTION_TELNET_TELL_HANDLE;
			sprintf(RM_Command [14], "Tell %s ", VAR_TELNETHANDLE);

			break;

		//
		// telnet edit
		//
		case RIGHT_MOUSE_TELNET_EDIT :
			// cut
			strcpy(RM_Menu [0], "Cut");
			RM_Type [0] = FUNCTION_TELNET_EDIT_CUT;

			// copy
			strcpy(RM_Menu [1], "Copy");
			RM_Type [1] = FUNCTION_TELNET_EDIT_COPY;

			// paste
			strcpy(RM_Menu [2], "Paste");
			RM_Type [2] = FUNCTION_TELNET_EDIT_PASTE;

			// select all
			strcpy(RM_Menu [3], "Select All");
			RM_Type [3] = FUNCTION_TELNET_EDIT_ALL;
			break;

		//
		// board handle
		//
		case RIGHT_MOUSE_BOARD_HANDLE :
			// all
			sprintf(RM_Menu [0], "%s %s", ICS_ALL_COMMAND, VAR_BOARDHANDLE);
			RM_Type [0] = FUNCTION_COMMAND;
			sprintf(RM_Command [0], "%s %s", ICS_ALL_COMMAND, VAR_BOARDHANDLE);

			// variable
			sprintf(RM_Menu [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_BOARDHANDLE);
			RM_Type [1] = FUNCTION_COMMAND;
			sprintf(RM_Command [1], "%s %s", ICS_VARIABLE_COMMAND, VAR_BOARDHANDLE);

			// ping
			sprintf(RM_Menu [2], "%s %s", ICS_PING_COMMAND, VAR_BOARDHANDLE);
			RM_Type [2] = FUNCTION_COMMAND;
			sprintf(RM_Command [2], "%s %s", ICS_PING_COMMAND, VAR_BOARDHANDLE);

			// finger
			sprintf(RM_Menu [3], "%s %s", ICS_FINGER_COMMAND, VAR_BOARDHANDLE);
			RM_Type [3] = FUNCTION_COMMAND;
			sprintf(RM_Command [3], "%s %s", ICS_FINGER_COMMAND, VAR_BOARDHANDLE);

			// follow
			sprintf(RM_Menu [4], "%s %s", ICS_FOLLOW_COMMAND, VAR_BOARDHANDLE);
			RM_Type [4] = FUNCTION_COMMAND;
			sprintf(RM_Command [4], "%s %s", ICS_FOLLOW_COMMAND, VAR_BOARDHANDLE);

			// history
			sprintf(RM_Menu [5], "%s %s", ICS_HISTORY_COMMAND, VAR_BOARDHANDLE);
			RM_Type [5] = FUNCTION_COMMAND;
			sprintf(RM_Command [5], "%s %s", ICS_HISTORY_COMMAND, VAR_BOARDHANDLE);

			// observe
			sprintf(RM_Menu [6], "%s %s", ICS_OBSERVE_COMMAND, VAR_BOARDHANDLE);
			RM_Type [6] = FUNCTION_COMMAND;
			sprintf(RM_Command [6], "%s %s", ICS_OBSERVE_COMMAND, VAR_BOARDHANDLE);

			// unobserve
			sprintf(RM_Menu [7], "%s %s", ICS_UNOBSERVE_COMMAND, VAR_BOARDHANDLE);
			RM_Type [7] = FUNCTION_COMMAND;
			sprintf(RM_Command [7], "%s %s", ICS_UNOBSERVE_COMMAND, VAR_BOARDHANDLE);
			break;
	}
}

void RIGHTMOUSE_Adjust(void)
{
	int nI, nJ;

	for(nI = 0 ; nI < MAX_RM ; nI++)
	{
		for(nJ = 0 ; nJ < MAX_RM_ITEM ; nJ++)
		{
			if((RightMouse [nI].nType [nJ] < 0) || (RightMouse [nI].nType [nJ] >= MAX_FMENU))
			{
				RightMouse [nI].nType [nJ] = FUNCTION_NONE;
			}

			if(! TOOLBOX_NeedCommandValue(RightMouse [nI].nType [nJ]))
			{
				strcpy(RightMouse [nI].cCommand [nJ], "");
			}
		}
	}
}

int RIGHTMOUSE_Command(int nI, int nJ, int bAdd)
{
	if(RightMouse [nI].nType [nJ] == FUNCTION_NONE)
	{
		return 0;
	}
	if(RightMouse [nI].nType [nJ] == FUNCTION_MATCH_BOX)
	{
		Match.bMatchMouse = 1;
	}
	if(RightMouse [nI].nType [nJ] == FUNCTION_MESSAGE_BOX)
	{
		User.bMessageMouse = 1;
	}
	return TOOLBOX_Command(RightMouse [nI].nType [nJ], RightMouse [nI].cCommand [nJ], bAdd);
}

void RIGHTMOUSE_AssignCheck(UINT nItem, int bValue)
{
	int nI;

	for(nI = 0 ; nI < MAX_FMENU ; nI++)
	{
		if(FMenu [nI] == nItem)
		{
			FCheck [nI] = bValue;
			break;
		}
	}
}

void RIGHTMOUSE_LoadCheck(void)
{
	RIGHTMOUSE_AssignCheck(IDM_LOG_TELNET,       User.bLogTelnet);
	RIGHTMOUSE_AssignCheck(IDM_AUTO_QUEEN,       User.bAutoQueen);
	RIGHTMOUSE_AssignCheck(IDM_AUTO_OBSERVE,     User.bAutoObserve);
	RIGHTMOUSE_AssignCheck(IDM_AUTO_REMATCH,     User.bAutoRematch);
	RIGHTMOUSE_AssignCheck(IDM_COORDINATES_SHOW, User.bShowCoord);
	RIGHTMOUSE_AssignCheck(IDM_SMART_MOVE,       User.bSmartMove);

	switch(User.nTruePremove)
	{
		case PREMOVE_NONE :
			User.bTruePremove = 0;
			User.nTruePremove = PREMOVE_NONE;
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_THIEF, 0);
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_LR,    0);
			break;

		case PREMOVE_THIEF :
			User.bTruePremove = 1;
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_THIEF, 1);
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_LR,    0);
			break;

		case PREMOVE_LR :
			User.bTruePremove = 1;
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_THIEF, 0);
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_LR,    1);
			break;

		default :
			User.bTruePremove = 0;
			User.nTruePremove = PREMOVE_NONE;
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_THIEF, 0);
			RIGHTMOUSE_AssignCheck(IDM_TRUE_PREMOVE_LR,    0);
			break;
	}

	RIGHTMOUSE_AssignCheck(IDM_RIGHT_MOUSE_PARTNER, User.bRightMousePartner);
	RIGHTMOUSE_AssignCheck(IDM_TEST_LEGAL_PLAY,     User.bTestLegalPlay);
	RIGHTMOUSE_AssignCheck(IDM_TEST_LEGAL_PARTNER,  User.bTestLegalPartner);
	RIGHTMOUSE_AssignCheck(IDM_ANIMATE_OBSERVE,     User.bAnimateObserve);
	RIGHTMOUSE_AssignCheck(IDM_ANIMATE_EXAM,        User.bAnimateExam);
	RIGHTMOUSE_AssignCheck(IDM_APPLY_SOUND,         User.bSound);
	RIGHTMOUSE_AssignCheck(IDM_APPLY_TELNET_SOUND,  User.bTelnetSound);
	RIGHTMOUSE_AssignCheck(IDM_APPLY_GAME_SOUND,    User.bGameSound);
	RIGHTMOUSE_AssignCheck(IDM_APPLY_EAR_SOUND,     User.bEarSound);
	RIGHTMOUSE_AssignCheck(IDM_FINAL_COUNTDOWN,     User.bFinalCountDown);
	RIGHTMOUSE_AssignCheck(IDM_LOG_GAMES,           User.bLogGame);
}