#include "thief.h"

BOOL CALLBACK NewLoginBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hName, hServer, hHelper, hHandle, hPassword, hScript;
	FARPROC LpProc;
	char cName [_MAX_PATH], cHelper [_MAX_PATH], cHandle [_MAX_PATH], cPassword [_MAX_PATH], cScript [_MAX_PATH];
	char cTmp [_MAX_PATH], cTmp1 [_MAX_PATH], cTmpName [_MAX_PATH];
	FILE *Fv;
	int  nI, nS, nB;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

#ifdef KICS
			EnableWindow(GetDlgItem(hwnd, IDD_SERVER), SW_HIDE);
			EnableWindow(GetDlgItem(hwnd, IDD_SERVER_CONFIGURE), SW_HIDE);
#endif

			hName     = GetDlgItem(hwnd, IDD_NAME);
			hServer   = GetDlgItem(hwnd, IDD_SERVER);
			hHelper   = GetDlgItem(hwnd, IDD_HELPER);
			hHandle   = GetDlgItem(hwnd, IDD_HANDLE);
			hPassword = GetDlgItem(hwnd, IDD_PASSWORD);
			hScript   = GetDlgItem(hwnd, IDD_SCRIPT);

			SendDlgItemMessage(hwnd, IDD_NAME, WM_SETTEXT, (WPARAM) 0, (LPARAM) "");

			strcpy(cTmpName, "");

			SendMessage(hServer, CB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_SERVER ; nI++)
			{
				if(strlen(Server.cServer [nI]) == 0)
				{
					break;
				}

				switch(Server.nType [nI])
				{
					case SERVER_FICS :
						strcpy(cTmp1, "fics");
						break;

					case SERVER_ICC :
						strcpy(cTmp1, "icc");
						break;

					case SERVER_NONFICS :
						strcpy(cTmp1, "other");
						break;

					default :
						Server.nType [nI] = SERVER_FICS;
						strcpy(cTmp1, "fics");
						break;
				}

				sprintf(cTmp, "%s %s [%s] (%s)", Server.cServer [nI], Server.cPort [nI], Server.cPrompt [nI], cTmp1);
				SendMessage(hServer, CB_ADDSTRING, 0, (LPARAM) cTmp);

				if(nI == 0)
				{
					strcpy(cTmpName, cTmp);
				}
			}

			strcpy(cTmp, cTmpName);
			if(SendMessage(hServer, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hServer, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hServer, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_HELPER,   WM_SETTEXT, (WPARAM) 0, (LPARAM) "timeseal.exe");
			SendDlgItemMessage(hwnd, IDD_HANDLE,   WM_SETTEXT, (WPARAM) 0, (LPARAM) "");
			SendDlgItemMessage(hwnd, IDD_PASSWORD, WM_SETTEXT, (WPARAM) 0, (LPARAM) "");
			SendDlgItemMessage(hwnd, IDD_SCRIPT,   WM_SETTEXT, (WPARAM) 0, (LPARAM) "");

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_SERVER_CONFIGURE :
					System.bInDialogBox = 0;

					nS = SendMessage(hServer, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);

					LpProc = MakeProcInstance((FARPROC) ServerBoxWndProc, hInst);
					if(DialogBox(hInst, "ServerBox", hwnd, (DLGPROC) LpProc))
					{
						nB = 0;
						SendMessage(hServer, CB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							if(strlen(Server.cServer [nI]) == 0)
							{
								break;
							}

							nB = nB + 1;

							switch(Server.nType [nI])
							{
								case SERVER_FICS :
									strcpy(cTmp1, "fics");
									break;

								case SERVER_ICC :
									strcpy(cTmp1, "icc");
									break;

								case SERVER_NONFICS :
									strcpy(cTmp1, "other");
									break;

								default :
									Server.nType [nI] = SERVER_FICS;
									strcpy(cTmp1, "fics");
									break;
							}

							sprintf(cTmp, "%s %s [%s] (%s)", Server.cServer [nI], Server.cPort [nI], Server.cPrompt [nI], cTmp1);
							SendMessage(hServer, CB_ADDSTRING, 0, (LPARAM) cTmp);
						}

						if(nS == CB_ERR)
						{
							nS = 0;
						}
						else if(nS >= nB)
						{
							nS = 0;
						}

						switch(Server.nType [nS])
						{
							case SERVER_FICS :
								strcpy(cTmp1, "fics");
								break;
							case SERVER_ICC  :
								strcpy(cTmp1, "icc");
								break;
							case SERVER_NONFICS :
								strcpy(cTmp1, "other");
								break;
							default :
								strcpy(cTmp1, "fics");
								break;
						}
						sprintf(cTmp, "%s %s [%s] (%s)", Server.cServer [nS], Server.cPort [nS], Server.cPrompt [nS], cTmp1);
						if(SendMessage(hServer, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hServer, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hServer, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}
					}
					FreeProcInstance(LpProc);

					System.bInDialogBox = 1;
					break;

				case IDD_HELPER_BROWSE :
					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "exe", HELPER_FILT, "Helper File", NULL, NULL, cTmp, TOOLBOX_GetHelperPath());
					if(Fv != NULL)
					{
						fclose(Fv);

						nS = SendMessage(hServer, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nS == CB_ERR)
						{
							nS= 0;
						}

						strcpy(cTmp, TOOLBOX_GetHelperFilename(cTmp));

						SendDlgItemMessage(hwnd, IDD_HELPER, WM_SETTEXT, 0, (LPARAM) cTmp);
					}
					break;

				case IDD_SCRIPT_BROWSE :
					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "scr", SCRIPT_FILT, "Script File", NULL, NULL, cTmp, TOOLBOX_GetScriptPath());
					if(Fv != NULL)
					{
						fclose(Fv);
						strcpy(cTmp, TOOLBOX_GetScriptFilename(cTmp));
						SendDlgItemMessage(hwnd, IDD_SCRIPT, WM_SETTEXT, 0, (LPARAM) cTmp);
					}
					break;

				case IDOK :
					SendDlgItemMessage(hwnd, IDD_NAME, WM_GETTEXT, (WPARAM) _MAX_PATH, (LPARAM) cName);
					TOOLBOX_AllTrim(cName);

					if(strlen(cName) == 0)
					{
						TOOLBOX_Error("Missing Connection Name.");
						SetFocus(hName);
						return FALSE;
					}

					LOGIN_File(cName, cTmpName);

					if(TOOLBOX_CanOpenFile(cTmpName))
					{
						TOOLBOX_Error("Connection Name Already Exists.");
						SetFocus(hName);
						return FALSE;
					}

					nS = SendMessage(hServer, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nS == CB_ERR)
					{
						TOOLBOX_Error("Invalid Server.");
						SetFocus(hServer);
						return FALSE;
					}

					SendDlgItemMessage(hwnd, IDD_HELPER,   WM_GETTEXT, (WPARAM) _MAX_PATH, (LPARAM) cHelper);
					SendDlgItemMessage(hwnd, IDD_HANDLE,   WM_GETTEXT, (WPARAM) _MAX_PATH, (LPARAM) cHandle);
					SendDlgItemMessage(hwnd, IDD_PASSWORD, WM_GETTEXT, (WPARAM) _MAX_PATH, (LPARAM) cPassword);
					SendDlgItemMessage(hwnd, IDD_SCRIPT,   WM_GETTEXT, (WPARAM) _MAX_PATH, (LPARAM) cScript);

					TOOLBOX_AllTrim(cHelper);

					if(strlen(cHelper) == 0)
					{
						TOOLBOX_Beep();

						switch(Server.nType [nS])
						{
							case 0 :
								if(MessageBox(NULL, "Are You Sure You Want to Log Into FICS Without Timeseal.exe?", "Information", MB_YESNO|MB_ICONINFORMATION) == IDNO)
								{
									strcpy(cHelper, "timeseal.exe");
								}
								break;

							case 1 :
								if(MessageBox(NULL, "Are You Sure You Want to Log Into ICC Without Timestamp.exe?", "Information", MB_YESNO|MB_ICONINFORMATION) == IDNO)
								{
									strcpy(cHelper, "timestamp.exe");
								}
								break;

							case 2 :
								if(stricmp(Server.cServer [nS], "chess-square.com") == 0)
								{
								}
								else
								{
									if(MessageBox(NULL, "Are You Sure You Want to Log Into NONFICS Without Any Helper?", "Information", MB_YESNO|MB_ICONINFORMATION) == IDNO)
									{
										strcpy(cHelper, "timeseal.exe");
									}
								}
								break;

							default :
								if(MessageBox(NULL, "Are You Sure You Want to Log Into FICS Without Timeseal.exe?", "Information", MB_YESNO|MB_ICONINFORMATION) == IDNO)
								{
									strcpy(cHelper, "timeseal.exe");
								}
								break;
						}
					}
					else
					{
						switch(Server.nType [nS])
						{
							case 0 :
								if(stricmp(cHelper, "timestamp.exe") == 0)
								{
									strcpy(cHelper, "timeseal.exe");
								}
								break;

							case 1 :
								if(stricmp(cHelper, "timeseal.exe") == 0)
								{
									strcpy(cHelper, "timestamp.exe");
								}
								break;

							default :
								break;
						}
					}

					TOOLBOX_AllTrim(cHandle);
					TOOLBOX_AllTrim(cPassword);
					TOOLBOX_AllTrim(cScript);

					cHandle   [MAX_LOGIN_HANDLE_SIZE   - 1] = NULL_CHAR;
					cPassword [MAX_LOGIN_PASSWORD_SIZE - 1] = NULL_CHAR;

					if(strlen(cHandle) == 0)
					{
						TOOLBOX_Error("Missing Handle.");
						SetFocus(hHandle);
						return FALSE;
					}

					for(nI = 0 ; nI < ((int) strlen(cHandle)) ; nI++)
					{
						if((! isalnum(cHandle [nI])) && (cHandle [nI] != '-'))
						{
							TOOLBOX_Error("Invalid Handle.");
							SetFocus(hHandle);
							return FALSE;
						}
					}

					if(strlen(cScript) > 0)
					{
						if(! TOOLBOX_CanOpenFile(TOOLBOX_GetFullScriptFilename(cScript)))
						{
							TOOLBOX_Error("Script File Not Found.");
							SetFocus(hScript);
							return FALSE;
						}
					}

					strcpy(Login.cLoginFile, cTmpName);
					strcpy(Login.cLoginName, cName);
					strcpy(Login.cLoginAddress, Server.cServer [nS]);
					strcpy(Login.cLoginPort, Server.cPort [nS]);
					strcpy(Login.cLoginPrompt, Server.cPrompt [nS]);

					switch(Server.nType [nS])
					{
						case 0 :
							Login.nLoginType = SERVER_FICS;
							strcpy(Login.cLoginPrompt, FICS_PROMPT);
							break;

						case 1 :
							Login.nLoginType = SERVER_ICC;
							strcpy(Login.cLoginPrompt, ICC_PROMPT);
							break;

						case 2 :
							Login.nLoginType = SERVER_NONFICS;
							if(strlen(Login.cLoginPrompt) == 0)
							{
								strcpy(Login.cLoginPrompt, NONFICS_PROMPT);
							}
							break;

						default :
							Login.nLoginType = SERVER_FICS;
							strcpy(Login.cLoginPrompt, FICS_PROMPT);
							break;
					}

					strcpy(Login.cLoginHelper, cHelper);
					strcpy(Login.cLoginHandle, cHandle);
					strcpy(Login.cLoginPassword, cPassword);
					strcpy(Login.cLoginScript, cScript);

					LOGIN_Prepare();

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
