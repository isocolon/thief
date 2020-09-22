#include "thief.h"

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	CLIENTCREATESTRUCT clientcreate ;
	MDICREATESTRUCT    mdicreate ;
	HWND               hwndChild ;
	FARPROC            LpProc ;
	CHARRANGE          sel ;
	FILE               *Fv ;
	char               cTmp [_MAX_PATH + 100], *cStart, *cEnd ;
	int                nStatus ;
	// these vars are for Tray Icon handling
	static UINT		   suTaskbarRestart;
	POINT			   pt;
	HMENU			   htrayMenu, hpopup;

	switch(iMsg)
	{
		case WM_CREATE :
			LOGIN_Check() ;
			SYS_InitCoord(hwnd) ;

			clientcreate.hWindowMenu  = hMenu [MENU_INIT_WINDOW] ;
			clientcreate.idFirstChild = IDM_CHILD ;

			hwndWindow [HWND_CLIENT] =
				CreateWindow("MDICLIENT", NULL,
							 WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE | MDIS_ALLCHILDSTYLES, // Georg
							 0, 0, 0, 0, hwnd, (HMENU) 1, hInst,
							 (LPSTR) &clientcreate) ;

			TOOLBOX_DisconnectedMenu() ;
			TOOLBOX_CheckAllMenu() ;
			TOOLBOX_SetTimerCommandMenu() ;

#ifdef KICS
			EnableMenuItem(hMenu [MENU_INIT], IDM_FICS_REGISTER,          MF_GRAYED) ;
			EnableMenuItem(hMenu [MENU_INIT], IDM_READ_THE_BUGBOARD,      MF_GRAYED) ;
			EnableMenuItem(hMenu [MENU_INIT], IDM_READ_THE_FICS_FORUM,    MF_GRAYED) ;
#endif

			GAME_InitialSetup(INDEX_PLAY) ;

			mdicreate.szClass = "MdiPlayChild" ;
			mdicreate.szTitle = PLAY_BOARD_TITLE ;
			mdicreate.hOwner  = hInst ;
			mdicreate.x       = wCoord [COORD_PLAY].x ;
			mdicreate.y       = wCoord [COORD_PLAY].y ;
			mdicreate.cx      = wCoord [COORD_PLAY].w ;
			mdicreate.cy      = wCoord [COORD_PLAY].h ;
			mdicreate.style   = WS_OVERLAPPEDWINDOW | WS_VISIBLE ;
			mdicreate.lParam  = 0 ;

			hwndWindow [HWND_PLAY] =
				(HWND) SendMessage(hwndWindow [HWND_CLIENT], WM_MDICREATE, 0,
								   (LPARAM)(LPMDICREATESTRUCT) &mdicreate) ;

			Game [INDEX_PLAY].hwnd = hwndWindow [HWND_PLAY] ;

			mdicreate.szClass = "MdiButtonChild" ;
			mdicreate.szTitle = BUTTON_TITLE ;
			mdicreate.hOwner  = hInst ;
			mdicreate.x       = wCoord [COORD_BUTTON].x ;
			mdicreate.y       = wCoord [COORD_BUTTON].y ;
			mdicreate.cx      = wCoord [COORD_BUTTON].w ;
			mdicreate.cy      = wCoord [COORD_BUTTON].h ;
			mdicreate.style   = WS_OVERLAPPEDWINDOW | WS_VISIBLE ;
			mdicreate.lParam  = 0 ;

			hwndWindow [HWND_BUTTON] =
				(HWND) SendMessage(hwndWindow [HWND_CLIENT], WM_MDICREATE, 0,
								   (LPARAM)(LPMDICREATESTRUCT) &mdicreate) ;

			mdicreate.szClass = "MdiTelnetChild" ;
			mdicreate.szTitle = TELNET_TITLE ;
			mdicreate.hOwner  = hInst ;
			mdicreate.x       = wCoord [COORD_TELNET].x ;
			mdicreate.y       = wCoord [COORD_TELNET].y ;
			mdicreate.cx      = wCoord [COORD_TELNET].w ;
			mdicreate.cy      = wCoord [COORD_TELNET].h ;
			mdicreate.style   = WS_OVERLAPPEDWINDOW | WS_VISIBLE ;
			mdicreate.lParam  = 0 ;

			hwndWindow [HWND_TELNET] =
				(HWND) SendMessage(hwndWindow [HWND_CLIENT], WM_MDICREATE, 0,
								   (LPARAM)(LPMDICREATESTRUCT) &mdicreate) ;

			TOOLBOX_SetTelnetCaption() ;

			// Load Task_Icon;
			hIcon  = LoadIcon(hInst, THIEF_ICO) ;

			suTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));

			return 0 ;

		case WSA_READ :
			if(Timeseal.TSInput.nCount == 0)
			{
				TOOLBOX_CloseSocket() ;
				TOOLBOX_WriteSystem(ICS_CONN_LOST_DISPLAY1) ;
				return 0 ;
			}

			strcpy(Timeseal.cSocketBuffer, Timeseal.TSInput.cBuffer) ;

			cStart = Timeseal.cSocketBuffer ;
			cEnd   = Timeseal.cSocketBuffer ;

			if(Timeseal.bBeforeLogin)
			{
				while(Timeseal.bSocketIsOpen)
				{
					while(*cEnd != 1  &&
							*cEnd != 13 &&
							*cEnd != NULL_CHAR)
					{
						TELNET_CheckPressReturn(cEnd) ;
						cEnd++ ;
					}

					if(*cEnd == NULL_CHAR)
					{
						if(Timeseal.bBeforeLogin)
						{
							switch(Login.nLoginType)
							{
								case SERVER_FICS :
									FICS_ProcessBeforeLoginLine(cStart) ;
									break ;
								case SERVER_ICC :
									ICC_ProcessBeforeLoginLine(cStart) ;
									break ;
								case SERVER_NONFICS :
									NONFICS_ProcessBeforeLoginLine(cStart) ;
									break ;
								default :
									NONFICS_ProcessBeforeLoginLine(cStart) ;
									break ;
							}
						}
						else
						{
							switch(Login.nLoginType)
							{
								case SERVER_FICS :
									FICS_ProcessLine(cStart) ;
									break ;
								case SERVER_ICC :
									ICC_ProcessLine(cStart) ;
									break ;
								case SERVER_NONFICS :
									NONFICS_ProcessLine(cStart) ;
									break ;
								default :
									NONFICS_ProcessLine(cStart) ;
									break ;
							}
						}
						break ;
					}

					if(cEnd != cStart)
					{
						*cEnd = NULL_CHAR ;

						if(Timeseal.bBeforeLogin)
						{
							switch(Login.nLoginType)
							{
								case SERVER_FICS :
									FICS_ProcessBeforeLoginLine(cStart) ;
									break ;
								case SERVER_ICC :
									ICC_ProcessBeforeLoginLine(cStart) ;
									break ;
								case SERVER_NONFICS :
									NONFICS_ProcessBeforeLoginLine(cStart) ;
									break ;
								default :
									NONFICS_ProcessBeforeLoginLine(cStart) ;
									break ;
							}
						}
						else
						{
							switch(Login.nLoginType)
							{
								case SERVER_FICS :
									FICS_ProcessLine(cStart) ;
									break ;
								case SERVER_ICC :
									ICC_ProcessLine(cStart) ;
									break ;
								case SERVER_NONFICS :
									NONFICS_ProcessLine(cStart) ;
									break ;
								default :
									NONFICS_ProcessLine(cStart) ;
									break ;
							}
						}
					}

					cStart = cEnd + 1 ;
					cEnd   = cStart ;
				}

				if(! Timeseal.bBeforeLogin)
				{
					strcpy(Timeseal.cSocketLine, "") ;
					Timeseal.bSocketLine = 0 ;
					TOOLBOX_SetTelnetCaption() ;
					TOOLBOX_ReadScriptFile(TOOLBOX_GetFullScriptFilename(Login.cLoginScript)) ;
					TOOLBOX_IssueLoginEvent() ;
				}
			}
			else
			{
				while(Timeseal.bSocketIsOpen)
				{
					while(*cEnd != 13 && *cEnd != NULL_CHAR)
					{
						if(*cEnd == 27)
						{
							Telnet.bHasESC = 1 ;
						}

						cEnd++ ;
					}

					if(*cEnd == NULL_CHAR)
					{
						if(Timeseal.bSocketLine)
						{
							strcat(Timeseal.cSocketLine, cStart) ;
						}
						else
						{
							strcpy(Timeseal.cSocketLine, cStart) ;
							Timeseal.bSocketLine = 1 ;
						}
						break ;
					}

					if(Timeseal.bSocketLine)
					{
						*cEnd = NULL_CHAR ;

						strcat(Timeseal.cSocketLine, cStart) ;
						Timeseal.bSocketLine = 0 ;

						switch(Login.nLoginType)
						{
							case SERVER_FICS :
								FICS_ProcessLine(Timeseal.cSocketLine) ;
								break ;
							case SERVER_ICC :
								ICC_ProcessLine(Timeseal.cSocketLine) ;
								break ;
							case SERVER_NONFICS :
								NONFICS_ProcessLine(Timeseal.cSocketLine) ;
								break ;
							default :
								NONFICS_ProcessLine(Timeseal.cSocketLine) ;
								break ;
						}
					}
					else
					{
						if(cEnd != cStart)
						{
							*cEnd = NULL_CHAR ;

							switch(Login.nLoginType)
							{
								case SERVER_FICS :
									FICS_ProcessLine(cStart) ;
									break ;
								case SERVER_ICC :
									ICC_ProcessLine(cStart) ;
									break ;
								case SERVER_NONFICS :
									NONFICS_ProcessLine(cStart) ;
									break ;
								default :
									NONFICS_ProcessLine(cStart) ;
									break ;
							}
						}
					}

					cStart = cEnd + 1 ;
					cEnd   = cStart ;
				}
			}

			Timeseal.TSInput.cNext = Timeseal.TSInput.cBuffer ;
			break ;

		case WSA_READ1 :
			if(WSAGETSELECTEVENT(lParam) == FD_READ)
			{
				nStatus = recv((SOCKET) wParam, Timeseal.cSocketBuffer, MAX_SOCKET_BUFFER_SIZE, NO_FLAGS_SET) ;
				if(nStatus)
				{
					Timeseal.cSocketBuffer [ nStatus ] = NULL_CHAR ;

					if(nStatus >= MAX_SOCKET_BUFFER_SIZE)
					{
						TOOLBOX_WriteSystem("Read Buffer Overflow\n") ;
					}

					cStart = Timeseal.cSocketBuffer ;
					cEnd   = Timeseal.cSocketBuffer ;

					if(Timeseal.bBeforeLogin)
					{
						while(Timeseal.bSocketIsOpen)
						{
							while(*cEnd != 1  &&
									*cEnd != 13 &&
									*cEnd != NULL_CHAR)
							{
								TELNET_CheckPressReturn(cEnd) ;
								cEnd++ ;
							}

							if(*cEnd == NULL_CHAR)
							{
								if(Timeseal.bBeforeLogin)
								{
									switch(Login.nLoginType)
									{
										case SERVER_FICS :
											FICS_ProcessBeforeLoginLine(cStart) ;
											break ;
										case SERVER_ICC :
											ICC_ProcessBeforeLoginLine(cStart) ;
											break ;
										case SERVER_NONFICS :
											NONFICS_ProcessBeforeLoginLine(cStart) ;
											break ;
										default :
											NONFICS_ProcessBeforeLoginLine(cStart) ;
											break ;
									}
								}
								else
								{
									switch(Login.nLoginType)
									{
										case SERVER_FICS :
											FICS_ProcessLine(cStart) ;
											break ;
										case SERVER_ICC :
											ICC_ProcessLine(cStart) ;
											break ;
										case SERVER_NONFICS :
											NONFICS_ProcessLine(cStart) ;
											break ;
										default :
											NONFICS_ProcessLine(cStart) ;
											break ;
									}
								}
								break ;
							}

							if(cEnd != cStart)
							{
								*cEnd = NULL_CHAR ;

								if(Timeseal.bBeforeLogin)
								{
									switch(Login.nLoginType)
									{
										case SERVER_FICS :
											FICS_ProcessBeforeLoginLine(cStart) ;
											break ;
										case SERVER_ICC :
											ICC_ProcessBeforeLoginLine(cStart) ;
											break ;
										case SERVER_NONFICS :
											NONFICS_ProcessBeforeLoginLine(cStart) ;
											break ;
										default :
											NONFICS_ProcessBeforeLoginLine(cStart) ;
											break ;
									}
								}
								else
								{
									switch(Login.nLoginType)
									{
										case SERVER_FICS :
											FICS_ProcessLine(cStart) ;
											break ;
										case SERVER_ICC :
											ICC_ProcessLine(cStart) ;
											break ;
										case SERVER_NONFICS :
											NONFICS_ProcessLine(cStart) ;
											break ;
										default :
											NONFICS_ProcessLine(cStart) ;
											break ;
									}
								}
							}

							cStart = cEnd + 1 ;
							cEnd   = cStart ;
						}

						if(! Timeseal.bBeforeLogin)
						{
							strcpy(Timeseal.cSocketLine, "") ;
							Timeseal.bSocketLine = 0 ;
							TOOLBOX_SetTelnetCaption() ;
							TOOLBOX_ReadScriptFile(TOOLBOX_GetFullScriptFilename(Login.cLoginScript)) ;
							TOOLBOX_IssueLoginEvent() ;
						}
					}
					else
					{
						while(Timeseal.bSocketIsOpen)
						{
							while(*cEnd != 13 && *cEnd != NULL_CHAR)
							{
								cEnd++ ;
							}

							if(*cEnd == NULL_CHAR)
							{
								if(Timeseal.bSocketLine)
								{
									strcat(Timeseal.cSocketLine, cStart) ;
								}
								else
								{
									strcpy(Timeseal.cSocketLine, cStart) ;
									Timeseal.bSocketLine = 1 ;
								}
								break ;
							}

							if(Timeseal.bSocketLine)
							{
								*cEnd = NULL_CHAR ;

								strcat(Timeseal.cSocketLine, cStart) ;
								Timeseal.bSocketLine = 0 ;

								switch(Login.nLoginType)
								{
									case SERVER_FICS :
										FICS_ProcessLine(Timeseal.cSocketLine) ;
										break ;
									case SERVER_ICC :
										ICC_ProcessLine(Timeseal.cSocketLine) ;
										break ;
									case SERVER_NONFICS :
										NONFICS_ProcessLine(Timeseal.cSocketLine) ;
										break ;
									default :
										NONFICS_ProcessLine(Timeseal.cSocketLine) ;
										break ;
								}
							}
							else
							{
								if(cEnd != cStart)
								{
									*cEnd = NULL_CHAR ;

									switch(Login.nLoginType)
									{
										case SERVER_FICS :
											FICS_ProcessLine(cStart) ;
											break ;
										case SERVER_ICC :
											ICC_ProcessLine(cStart) ;
											break ;
										case SERVER_NONFICS :
											NONFICS_ProcessLine(cStart) ;
											break ;
										default :
											NONFICS_ProcessLine(cStart) ;
											break ;
									}
								}
							}

							cStart = cEnd + 1 ;
							cEnd   = cStart ;
						}
					}
				}
				else
				{
					TOOLBOX_WriteSystem("Connection Broken\n") ;
				}
			}
			else
			{
				TOOLBOX_CloseSocket() ;
				TOOLBOX_WriteSystem(ICS_CONN_LOST_DISPLAY) ;
			}
			break ;

		case WM_KEYDOWN :
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
			return 0;

		case WM_CHAR :
			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
			return 0;

		case WM_ICONCLICK:
			switch(lParam)
			{
				case WM_LBUTTONDBLCLK:
					ShowWindow(hwndWindow [HWND_FRAME], SW_RESTORE);
					SetForegroundWindow(hwndWindow [HWND_FRAME]);
					TOOLBOX_ShowTaskIcon(hwndWindow [HWND_FRAME], FALSE);
					break;

				case WM_RBUTTONUP:
					GetCursorPos(&pt);
					htrayMenu = LoadMenu(hInst, "TRAYMENU") ;
					hpopup = GetSubMenu(htrayMenu, 0);
					/*  SetForegroundWindow and the ensuing null PostMessage is a
						workaround for a Windows 95 bug (see KB article Q135788),
						SetForegroundWindow also causes our MessageBox to pop up in front
						of any other application's windows. */
					SetForegroundWindow(hwndWindow [HWND_FRAME]);
					/*  We specifiy TPM_RETURNCMD, so TrackPopupMenu returns the menu
						selection instead of returning immediately and getting a
						WM_COMMAND with the selection. */
					switch(TrackPopupMenu(hpopup,   // Popup menu to track
										  TPM_RETURNCMD |    // Return menu code
										  TPM_RIGHTBUTTON, // Track right mouse button?
										  pt.x, pt.y,        // screen coordinates
										  0,                 // reserved
										  hwnd,              // owner
										  NULL))             // LPRECT user can click in
						// without dismissing menu
					{
						case IDM_EXIT_THIEF:
							SendMessage(hwnd, WM_CLOSE, 0, 0) ;
							break;

						case IDM_RESTORE_THIEF:
							ShowWindow(hwndWindow [HWND_FRAME], SW_RESTORE);
							SetForegroundWindow(hwndWindow [HWND_FRAME]);
							TOOLBOX_ShowTaskIcon(hwndWindow [HWND_FRAME], FALSE);
							break;
					}
					PostMessage(hwnd, 0, 0, 0); // see above
					DestroyMenu(htrayMenu); // Delete loaded menu and reclaim its resources
					break;

			}
			return 0;

		case WM_SIZE:
			if((wParam == SIZE_MINIMIZED) && (User.bMinimizetoTray))
			{
				TOOLBOX_ShowTaskIcon(hwndWindow [HWND_FRAME], TRUE);
				ShowWindow(hwndWindow [HWND_FRAME], SW_HIDE);
				return 0;
			}
			break;

		case WM_COMMAND :
			switch(wParam)
			{
				case IDM_NEW_CONNECT :
					System.bQuickConnection = 0 ;
					System.bCDROMConnection = 0 ;

					TOOLBOX_DisplayMenuPath("[File]->[New Connection]") ;

					LpProc = MakeProcInstance((FARPROC) NewLoginBoxWndProc, hInst) ;

					Login.nLoginType = SERVER_FICS ;
					strcpy(Login.cLoginFile,    "") ;
					strcpy(Login.cLoginName,    "") ;
#ifndef  KICS
					strcpy(Login.cLoginAddress, "freechess.org") ;
					strcpy(Login.cLoginPort,     "5000") ;
					strcpy(Login.cLoginPrompt,   FICS_PROMPT) ;
#endif

#ifdef  KICS
					strcpy(Login.cLoginAddress, "kics.freechess.org") ;
					strcpy(Login.cLoginPort,     "6000") ;
					strcpy(Login.cLoginPrompt,   FICS_PROMPT) ;
#endif

					strcpy(Login.cLoginHelper,   "timeseal.exe") ;
					strcpy(Login.cLoginHandle,   "") ;
					strcpy(Login.cLoginPassword, "") ;
					strcpy(Login.cLoginScript,   "") ;

					if(DialogBox(hInst, "NewLoginBox", hwnd, (DLGPROC) LpProc))
					{
						LOGIN_Save(Login.cLoginFile,     Login.cLoginName,
								   Login.cLoginAddress,  Login.cLoginPort,
								   Login.cLoginHelper,   &Login.nLoginType,
								   Login.cLoginPrompt,   Login.cLoginHandle,
								   Login.cLoginPassword, Login.cLoginScript) ;
						LOGIN_Login() ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_CONNECT :
					System.bQuickConnection = 0 ;
					System.bCDROMConnection = 0 ;

					TOOLBOX_DisplayMenuPath("[File]->[Open Connection]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "con",
												CONNECTION_FILT,
												"Open Connection",
												NULL, NULL, cTmp, Browse.cConnectionBrowse) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cConnectionBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						LpProc = MakeProcInstance((FARPROC) OldLoginBoxWndProc, hInst) ;

						strcpy(Login.cLoginFile, cTmp) ;

						if(LOGIN_Load(Login.cLoginFile,     Login.cLoginName,
									  Login.cLoginAddress,  Login.cLoginPort,
									  Login.cLoginHelper,   &Login.nLoginType,
									  Login.cLoginPrompt,   Login.cLoginHandle,
									  Login.cLoginPassword, Login.cLoginScript))
						{
							if(DialogBox(hInst, "OldLoginBox", hwnd, (DLGPROC) LpProc))
							{
								LOGIN_Save(Login.cLoginFile,     Login.cLoginName,
										   Login.cLoginAddress,  Login.cLoginPort,
										   Login.cLoginHelper,   &Login.nLoginType,
										   Login.cLoginPrompt,   Login.cLoginHandle,
										   Login.cLoginPassword, Login.cLoginScript) ;

								LOGIN_Login() ;
							}
							FreeProcInstance(LpProc) ;
						}
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RECONNECT :
					System.bQuickConnection = 0 ;
					System.bCDROMConnection = 0 ;

					TOOLBOX_DisplayMenuPath("[File]->[Reconnect]") ;

					if(strlen(System.cLoginFile) > 0)
					{
						sprintf(cTmp, "Reconnecting %s\n", System.cLoginFile) ;
						TOOLBOX_WriteSystem(cTmp) ;

						strcpy(Login.cLoginFile, System.cLoginFile) ;

						if(LOGIN_Load(Login.cLoginFile,     Login.cLoginName,
									  Login.cLoginAddress,  Login.cLoginPort,
									  Login.cLoginHelper,   &Login.nLoginType,
									  Login.cLoginPrompt,   Login.cLoginHandle,
									  Login.cLoginPassword, Login.cLoginScript))
						{
							LOGIN_Prepare() ;
							LOGIN_Login() ;
						}

						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					return 0 ;

				case IDM_DISCONNECT :
					System.bQuickConnection = 0 ;
					System.bCDROMConnection = 0 ;

					TOOLBOX_DisplayMenuPath("[File]->[Close Connection]") ;

					TOOLBOX_WriteICS(ICS_QUIT_COMMAND) ;
					TOOLBOX_WriteSystem(ICS_CONN_LOST_DISPLAY) ;

					TOOLBOX_CloseSocket() ;
					TOOLBOX_CloseObserveWindow() ;
					GAME_InitAllGame(0) ;
					GAME_InitialSetup(INDEX_PLAY) ;

					TOOLBOX_InvalidateBoardWindow(0) ;
					TOOLBOX_RedrawBoard(INDEX_PLAY) ;
					SetWindowText(hwndWindow [HWND_PLAY], PLAY_BOARD_TITLE) ;

					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_QUICK_CONNECT :
					System.bQuickConnection = 1 ;
					System.bCDROMConnection = 0 ;

					TOOLBOX_DisplayMenuPath("[File]->[Quick Connection]") ;

					LpProc = MakeProcInstance((FARPROC) OldLoginBoxWndProc, hInst) ;

					Login.nLoginType = SERVER_FICS ;
					strcpy(Login.cLoginFile,    "") ;
					strcpy(Login.cLoginName,    "") ;
#ifndef  KICS
					strcpy(Login.cLoginAddress, "freechess.org") ;
					strcpy(Login.cLoginPort,     "5000") ;
					strcpy(Login.cLoginPrompt,   FICS_PROMPT) ;
#endif
#ifdef  KICS
					strcpy(Login.cLoginAddress, "kics.freechess.org") ;
					strcpy(Login.cLoginPort,     "6000") ;
					strcpy(Login.cLoginPrompt,   FICS_PROMPT) ;
#endif
					strcpy(Login.cLoginHelper,   "timeseal.exe") ;
					strcpy(Login.cLoginHandle,   "") ;
					strcpy(Login.cLoginPassword, "") ;
					strcpy(Login.cLoginScript,   "") ;

					if(DialogBox(hInst, "OldLoginBox", hwnd, (DLGPROC) LpProc))
					{
						LOGIN_Login() ;
					}
					else
					{
						System.bQuickConnection = 0 ;
						System.bCDROMConnection = 0 ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_CDROM_CONNECT :
					System.bQuickConnection = 0 ;
					System.bCDROMConnection = 1 ;

					TOOLBOX_DisplayMenuPath("[File]->[CDROM Connection]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "con",
												CONNECTION_FILT,
												"Open CDROM Connection",
												NULL, NULL, cTmp, Browse.cConnectionBrowse) ;
					if(Fv == NULL)
					{
						System.bQuickConnection = 0 ;
						System.bCDROMConnection = 0 ;
					}
					else
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cConnectionBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						LpProc = MakeProcInstance((FARPROC) OldLoginBoxWndProc, hInst) ;

						strcpy(Login.cLoginFile, cTmp) ;

						if(LOGIN_Load(Login.cLoginFile,     Login.cLoginName,
									  Login.cLoginAddress,  Login.cLoginPort,
									  Login.cLoginHelper,   &Login.nLoginType,
									  Login.cLoginPrompt,   Login.cLoginHandle,
									  Login.cLoginPassword, Login.cLoginScript))
						{
							if(DialogBox(hInst, "OldLoginBox", hwnd, (DLGPROC) LpProc))
							{
								LOGIN_Save(Login.cLoginFile,     Login.cLoginName,
										   Login.cLoginAddress,  Login.cLoginPort,
										   Login.cLoginHelper,   &Login.nLoginType,
										   Login.cLoginPrompt,   Login.cLoginHandle,
										   Login.cLoginPassword, Login.cLoginScript) ;

								LOGIN_Login() ;
							}
							else
							{
								System.bQuickConnection = 0 ;
								System.bCDROMConnection = 0 ;
							}
							FreeProcInstance(LpProc) ;
						}
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SAVE_PROFILE :
					TOOLBOX_DisplayMenuPath("[File]->[Save Personal Configuration Profile]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, TRUE, NULL, "ini",
												INI_FILT,
												"Save Personal Configuration Profile",
												NULL, NULL, cTmp, Browse.cProfileBrowse) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cProfileBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;

						if(strlen(cTmp) > 0)
						{
							INI_WriteSetup(cTmp) ;
							INI_WriteSystem(cTmp) ;
							LAYOUT_ActualSave(cTmp) ;

							{
								char cTmp1 [2048] ;
								sprintf(cTmp1, "Saving Personal Configuration Profile %s Successful\n", cTmp) ;
								TOOLBOX_WriteSystem(cTmp1) ;
							}

						}
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_PROFILE :
					TOOLBOX_DisplayMenuPath("[File]->[Load Profile]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "ini",
												INI_FILT,
												"Load Profile",
												NULL, NULL, cTmp, Browse.cProfileBrowse) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cProfileBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;

						if(strlen(cTmp) > 0)
						{
							TOOLBOX_LoadProfile(cTmp) ;
						}
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOG_TELNET :
					TOOLBOX_DisplayMenuPath("[File]->[Log Telnet Console]") ;

					User.bLogTelnet = ! User.bLogTelnet ;
					TOOLBOX_WriteInfoOnOff(User.bLogTelnet, "Log Telnet Console is") ;
					TOOLBOX_CheckAllMenu() ;

					if(Timeseal.bSocketIsOpen)
					{
						if(User.bLogTelnet)
						{
							if(LOG_Start())
							{
								sprintf(cTmp, "Creating %s Successful\n", Log.cFn) ;
							}
							else
							{
								sprintf(cTmp, "Creating %s Failed\n", Log.cFn) ;
							}
							TOOLBOX_WriteSystem(cTmp) ;
						}
						else
						{
							if(LOG_End())
							{
								sprintf(cTmp, "Closing %s Successful\n", Log.cFn) ;
								TOOLBOX_WriteSystem(cTmp) ;
							}
							LOG_Init() ;
						}
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LOG :
					TOOLBOX_DisplayMenuPath("[File]->[Open Telnet Console Log]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "log",
												LOG_FILT,
												"Open Telnet Console Log",
												NULL, NULL, cTmp, Browse.cLogBrowse) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cLogBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenNotePad(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_THIEF_INI :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Thief Setting File]->[Edit Thief.ini]") ;

					TOOLBOX_OpenNotePad(INI_GetIniFilename()) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_THIEF_SYS :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Thief Setting File]->[Edit Thief.sys]") ;

					TOOLBOX_OpenNotePad(INI_GetSysFilename()) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_DEFAULT_LAYOUT :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Default Layout]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(0)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT1 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #1]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(1)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT2 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #2]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(2)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT3 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #3]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(3)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT4 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #4]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(4)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT5 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #5]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(5)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT6 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #6]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(6)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT7 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #7]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(7)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_LAYOUT8 :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Layout File]->[Edit Layout #8]") ;

					TOOLBOX_OpenNotePad(LAYOUT_File(8)) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_OPEN_SCRIPT :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Script File]->[Edit Script File]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "scr",
												SCRIPT_FILT,
												"Edit Script File",
												NULL, NULL, cTmp, TOOLBOX_GetScriptPath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						//TOOLBOX_OpenNotePad (TOOLBOX_GetFullScriptFilename (cTmp)) ;
						TOOLBOX_OpenNotePad(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RUN_SCRIPT :
					TOOLBOX_DisplayMenuPath("[File]->[Edit Script File]->[Run Script File]") ;

					if(! Timeseal.bSocketIsOpen)
					{
						TOOLBOX_Beep() ;
						return 0 ;
					}
					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "scr",
												SCRIPT_FILT,
												"Run Scrip File",
												NULL, NULL, cTmp, TOOLBOX_GetScriptPath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						//TOOLBOX_ReadScriptFile (TOOLBOX_GetFullScriptFilename (cTmp)) ;
						TOOLBOX_ReadScriptFile(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EXIT :
					TOOLBOX_DisplayMenuPath("[File]->[Exit]") ;

					SendMessage(hwnd, WM_CLOSE, 0, 0) ;
					return 0 ;

				case IDM_EXTRA :
				case IDM_EXTRA1 :
				case IDM_EXTRA2 :
				case IDM_EXTRA3 :
				case IDM_EXTRA4 :
				case IDM_EXTRA5 :
				case IDM_EXTRA6 :
				case IDM_EXTRA7 :
				case IDM_EXTRA8 :
				case IDM_EXTRA9 :
					LpProc = MakeProcInstance((FARPROC) OldLoginBoxWndProc, hInst) ;

					strcpy(Login.cLoginFile, Login.cLoginString [LOWORD(wParam) - IDM_EXTRA]) ;

					if(LOGIN_Load(Login.cLoginFile,     Login.cLoginName,
								  Login.cLoginAddress,  Login.cLoginPort,
								  Login.cLoginHelper,   &Login.nLoginType,
								  Login.cLoginPrompt,   Login.cLoginHandle,
								  Login.cLoginPassword, Login.cLoginScript))
					{
						if(DialogBox(hInst, "OldLoginBox", hwnd, (DLGPROC) LpProc))
						{
							LOGIN_Save(Login.cLoginFile,     Login.cLoginName,
									   Login.cLoginAddress,  Login.cLoginPort,
									   Login.cLoginHelper,   &Login.nLoginType,
									   Login.cLoginPrompt,   Login.cLoginHandle,
									   Login.cLoginPassword, Login.cLoginScript) ;

							LOGIN_Login() ;
						}
						FreeProcInstance(LpProc) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_COPY_PASTE :
					TOOLBOX_DisplayMenuPath("[Edit]->[Copy and Paste]") ;

					SendMessage(hwndWindow [HWND_TELNET_TEXT], WM_COMMAND, MAKEWPARAM(IDM_TEXT_QUICKPASTE, 0), 0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_COPY :
					TOOLBOX_DisplayMenuPath("[Edit]->[Copy]") ;

					SendMessage(hwndWindow [HWND_TELNET_EDIT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
					if(sel.cpMin == sel.cpMax)
					{
						SendMessage(hwndWindow [HWND_TELNET_TEXT], EM_EXGETSEL, 0, (LPARAM) &sel) ;
						if(sel.cpMin == sel.cpMax)
						{
							TOOLBOX_Beep() ;
						}
						else
						{
							SendMessage(hwndWindow [HWND_TELNET_TEXT], WM_COPY, 0, 0) ;
						}
					}
					else
					{
						SendMessage(hwndWindow [HWND_TELNET_EDIT], WM_COPY, 0, 0) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_PASTE :
					TOOLBOX_DisplayMenuPath("[Edit]->[Paste]") ;

					SendMessage(hwndWindow [HWND_TELNET_TEXT], WM_COMMAND, MAKEWPARAM(IDM_EDIT_PASTE, 0), 0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TEXT_TELL :

				case IDM_SELECT_ALL :
					TOOLBOX_DisplayMenuPath("[Edit]->[Select All]") ;

					sel.cpMin = 0  ;
					sel.cpMax = 999999 ;
					SendMessage(hwndWindow [HWND_TELNET_TEXT], EM_EXSETSEL, 0, (LPARAM) &sel) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SEEK_GAME :
					if(! Timeseal.bSocketIsOpen)
					{
						TOOLBOX_Beep() ;
						return 0 ;
					}
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{
							TOOLBOX_Beep() ;
							return 0 ;
						}
						else if(Login.nLoginType != SERVER_ICC)
						{
							TOOLBOX_Beep() ;
							return 0 ;
						}
					}

					TOOLBOX_DisplayMenuPath("[Game]->[Seek a Game...]") ;

					LpProc = MakeProcInstance((FARPROC) SeekBoxWndProc, hInst) ;
					DialogBox(hInst, "SeekBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MATCH_PLAYER :
					if(! Timeseal.bSocketIsOpen)
					{
						Match.bMatchMouse = 0 ;
						TOOLBOX_Beep() ;
						return 0 ;
					}
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{
							Match.bMatchMouse = 0 ;
							TOOLBOX_Beep() ;
							return 0 ;
						}
						else if(Login.nLoginType != SERVER_ICC)
						{
							Match.bMatchMouse = 0 ;
							TOOLBOX_Beep() ;
							return 0 ;
						}
					}

					TOOLBOX_DisplayMenuPath("[Game]->[Match Player...]") ;

					LpProc = MakeProcInstance((FARPROC) MatchBoxWndProc, hInst) ;
					DialogBox(hInst, "MatchBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_REMATCH :
					TOOLBOX_DisplayMenuPath("[Game]->[Rematch]") ;

					TOOLBOX_Issue_Command(FICS_REMATCH_COMMAND,
										  ICC_REMATCH_COMMAND,
										  NONFICS_REMATCH_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RESUME_GAME :
					TOOLBOX_DisplayMenuPath("[Game]->[Resume Game]") ;

					TOOLBOX_Issue_Command(FICS_RESUME_COMMAND,
										  ICC_RESUME_COMMAND,
										  NONFICS_RESUME_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_FLIP_BOARD :
					if(IsWindow(System.hwndLastGame))
					{
						TOOLBOX_DisplayMenuPath("[Game]->[Flip Board]") ;

						PostMessage(System.hwndLastGame, WM_USER_FLIP_BOARD, (WPARAM) 0, (LPARAM) 0) ;
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOG_GAMES :
					TOOLBOX_DisplayMenuPath("[Game]->[Log Games]") ;

					User.bLogGame = ! User.bLogGame ;
					TOOLBOX_WriteInfoOnOff(User.bLogGame, "Log Games is") ;

					if(User.bLogGame)
					{
						if((! User.bSavePlayGame) &&
								(! User.bSaveObserveGame) &&
								(! User.bSaveUnobserveGame) &&
								(! User.bSaveAbortGame))
						{
							User.bSavePlayGame      = 1 ;
							User.bSaveObserveGame   = 1 ;
							User.bSaveUnobserveGame = 0 ;
							User.bSaveAbortGame     = 1 ;
							User.bSaveNoBFENBugGame = 1 ;
						}
					}

					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOG_GAME_SETTINGS :
					TOOLBOX_DisplayMenuPath("[Game]->[Log Game Settings...]") ;

					LpProc = MakeProcInstance((FARPROC) SaveGameBoxWndProc, hInst) ;
					DialogBox(hInst, "SaveGameBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_MY_BPGN :
					TOOLBOX_DisplayMenuPath("[Game]->[Load Saved BPGN Games]->[My Games]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "bpgn",
												BPGN_FILT,
												"Load My BPGN Games",
												NULL, NULL, cTmp, TOOLBOX_GetMyGamePath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSaveBPGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_OBSERVED_BPGN :
					TOOLBOX_DisplayMenuPath("[Game]->[Load Saved BPGN Games]->[Observed Games]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "bpgn",
												BPGN_FILT,
												"Load Observed BPGN Games",
												NULL, NULL, cTmp, TOOLBOX_GetObservedGamePath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSaveBPGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_OTHER_BPGN :
					TOOLBOX_DisplayMenuPath("[Game]->[Load Saved BPGN Games]->[Other Games]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "bpgn",
												BPGN_FILT,
												"Load Other BPGN Games",
												NULL, NULL, cTmp, TOOLBOX_GetOtherGamePath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSaveBPGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_MY_PGN :
					TOOLBOX_DisplayMenuPath("[Game]->[Load Saved PGN Games]->[My Games]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "pgn",
												PGN_FILT,
												"Load My PGN Games",
												NULL, NULL, cTmp, TOOLBOX_GetMyGamePath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSavePGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_OBSERVED_PGN :
					TOOLBOX_DisplayMenuPath("[Game]->[Load Saved PGN Games]->[Observed Games]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "pgn",
												PGN_FILT,
												"Load Observed PGN Games",
												NULL, NULL, cTmp, TOOLBOX_GetObservedGamePath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSavePGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_OTHER_PGN :
					TOOLBOX_DisplayMenuPath("[Game]->[Load Saved PGN Games]->[Other Games]") ;

					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "pgn",
												PGN_FILT,
												"Load Other PGN Games",
												NULL, NULL, cTmp, TOOLBOX_GetOtherGamePath()) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSavePGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TELL_USER :
					if(Timeseal.bSocketIsOpen)
					{
						TOOLBOX_DisplayMenuPath("[Actions]->[Send Tells to a User]") ;

						LpProc = MakeProcInstance((FARPROC) TellBoxWndProc, hInst) ;
						DialogBox(hInst, "TellBox", hwnd, (DLGPROC) LpProc) ;
						FreeProcInstance(LpProc) ;
						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					else
					{
						TOOLBOX_Beep() ;
						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					return 0 ;

				case IDM_MESSAGE_USER :
					if(Timeseal.bSocketIsOpen)
					{
						if(User.bIamAGuest)
						{
							User.bMessageMouse = 0 ;
							TOOLBOX_Beep() ;
							SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
						}
						else
						{
							TOOLBOX_DisplayMenuPath("[Actions]->[Message a User]") ;

							LpProc = MakeProcInstance((FARPROC) MessageBoxWndProc, hInst) ;
							DialogBox(hInst, "MessageBox", hwnd, (DLGPROC) LpProc) ;
							FreeProcInstance(LpProc) ;
							SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
						}
					}
					else
					{
						User.bMessageMouse = 0 ;
						TOOLBOX_Beep() ;
						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					return 0 ;

				case IDM_MY_RATING_NOTES :
					TOOLBOX_DisplayMenuPath("[Actions]->[Show My Rating & Notes]") ;

					TOOLBOX_Issue_Command(FICS_FINGER_COMMAND,
										  ICC_FINGER_COMMAND,
										  NONFICS_FINGER_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MY_VARS :
					TOOLBOX_DisplayMenuPath("[Actions]->[Show My Variables]") ;

					TOOLBOX_Issue_Command(FICS_VARS_COMMAND,
										  ICC_VARS_COMMAND,
										  NONFICS_VARS_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MY_PING :
					TOOLBOX_DisplayMenuPath("[Actions]->[Show My Lag/Ping Time]") ;

					TOOLBOX_Issue_Command(FICS_PING_COMMAND,
										  ICC_PING_COMMAND,
										  NONFICS_PING_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MY_HISTORY :
					TOOLBOX_DisplayMenuPath("[Actions]->[Show My Recent Games]") ;

					TOOLBOX_Issue_Command(FICS_HISTORY_COMMAND,
										  ICC_HISTORY_COMMAND,
										  NONFICS_HISTORY_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MY_STORED :
					if(User.bIamAGuest)
					{
						TOOLBOX_Beep() ;
					}
					else
					{
						TOOLBOX_DisplayMenuPath("[Actions]->[Show My Adjourned Games]") ;

						TOOLBOX_Issue_Command(FICS_STORED_COMMAND,
											  ICC_STORED_COMMAND,
											  NONFICS_STORED_COMMAND, 1) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MY_JOURNAL :
					if(User.bIamAGuest)
					{
						TOOLBOX_Beep() ;
					}
					else
					{
						TOOLBOX_DisplayMenuPath("[Actions]->[Show My Personal Journal Games]") ;

						sprintf(cTmp, "%s %s\n", ICC_JOURNAL_COMMAND, Vars.cWhoAmI) ;

						TOOLBOX_Issue_Command(FICS_JOURNAL_COMMAND,
											  cTmp,
											  NONFICS_JOURNAL_COMMAND, 1) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LIST_BUG_PLAY :
					TOOLBOX_DisplayMenuPath("[Actions]->[List Bughouse Games Being Played]") ;

					TOOLBOX_Issue_Command(FICS_BUG_PLAY_COMMAND,
										  ICC_BUG_PLAY_COMMAND,
										  NONFICS_BUG_PLAY_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LIST_BUG_PARTNERED :
					TOOLBOX_DisplayMenuPath("[Actions]->[List Bughouse Players Partnered]") ;

					TOOLBOX_Issue_Command(FICS_BUG_PARTNERED_COMMAND,
										  ICC_BUG_PARTNERED_COMMAND,
										  NONFICS_BUG_PARTNERED_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LIST_BUG_OPEN :
					TOOLBOX_DisplayMenuPath("[Actions]->[List Players Open for Bughouse Partnership]") ;

					TOOLBOX_Issue_Command(FICS_BUG_OPEN_COMMAND,
										  ICC_BUG_OPEN_COMMAND,
										  NONFICS_BUG_OPEN_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LIST_PLAYERS :
					TOOLBOX_DisplayMenuPath("[Actions]->[List Players Open for Chess Matches]") ;

					TOOLBOX_Issue_Command(FICS_LIST_PLAYERS_COMMAND,
										  ICC_LIST_PLAYERS_COMMAND,
										  NONFICS_LIST_PLAYERS_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LIST_ALL_PLAYERS :
					TOOLBOX_DisplayMenuPath("[Actions]->[List All Players Online]") ;

					TOOLBOX_Issue_Command(FICS_ALL_PLAYERS_COMMAND,
										  ICC_ALL_PLAYERS_COMMAND,
										  NONFICS_ALL_PLAYERS_COMMAND, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_START_TIMER :
					TOOLBOX_DisplayMenuPath("[Actions]->[Start Timer Command]") ;

					TOOLBOX_WriteInfoOnOff(! TimerCmd.bTimerCommand, "Timer Command is") ;
					TIMERCMD_Start() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_STOP_TIMER :
					TOOLBOX_DisplayMenuPath("[Actions]->[Stop Timer Command]") ;

					TOOLBOX_WriteInfoOnOff(! TimerCmd.bTimerCommand, "Timer Command is") ;
					TIMERCMD_Stop() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_FICS_REGISTER :
					TOOLBOX_DisplayMenuPath("[Actions]->[Register for a New FICS Account]") ;

					ShellExecute(NULL, "open", FICS_REGISTRATION_URL, NULL, "", SW_SHOW) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_READ_THE_BUGBOARD :
					TOOLBOX_DisplayMenuPath("[Actions]->[Read The Bugboard]") ;

					ShellExecute(NULL, "open", THE_BUGBOARD_URL, NULL, "", SW_SHOW) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_READ_THE_FICS_FORUM :
					TOOLBOX_DisplayMenuPath("[Actions]->[Read The FICS Forum]") ;

					ShellExecute(NULL, "open", THE_FICS_FORUM_URL, NULL, "", SW_SHOW) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_GAME_OPTION :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Game Configuration]->[Game Options...]") ;

					LpProc = MakeProcInstance((FARPROC) GameOptionBoxWndProc, hInst) ;
					if(DialogBox(hInst, "GameOptionBox", hwnd, (DLGPROC) LpProc))
					{
						Fics.bPobserve = 1 ;
						Fics.nPobserve = 0 ;
						strcpy(Fics.cPobserve, "") ;

						if(User.bActualSize)
						{
							TOOLBOX_InvalidateBoardWindow1(0) ;
						}
						else
						{
							TOOLBOX_InvalidateBoardWindow(0) ;
						}
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_GAME_DISPLAY :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Game Configuration]->[Game Display...]") ;

					LpProc = MakeProcInstance((FARPROC) GameDisplayBoxWndProc, hInst) ;
					if(DialogBox(hInst, "GameDisplayBox", hwnd, (DLGPROC) LpProc))
					{
						BOARD_ResetTruePremove() ;
						F8KEY_Init() ;
						TOOLBOX_InvalidateBoardWindow1(0) ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_BOARD_CONFIGURE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Game Configuration]->[Board Appearance...]") ;

					LpProc = MakeProcInstance((FARPROC) BoardBoxWndProc, hInst) ;
					if(DialogBox(hInst, "BoardBox", hwnd, (DLGPROC) LpProc))
					{
						SYS_ChangeColor() ;
						TOOLBOX_RefreshWindowColor() ;
						TOOLBOX_InvalidateBoardWindow(1) ;
					}
					else
					{
						TOOLBOX_InvalidateBoardWindow(0) ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_CLOCK_DISPLAY :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Game Configuration]->[Clock Display...]") ;

					LpProc = MakeProcInstance((FARPROC) RedClockBoxWndProc, hInst) ;
					if(DialogBox(hInst, "RedClockBox", hwnd, (DLGPROC) LpProc))
					{
						TOOLBOX_InvalidateBoardWindow(0) ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MOVE_INPUT :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Move-Input Configuration]->[Move-Input...]") ;

					LpProc = MakeProcInstance((FARPROC) MoveInputBoxWndProc, hInst) ;
					if(DialogBox(hInst, "MoveInputBox", hwnd, (DLGPROC) LpProc))
					{
						DragInfo.nClicked = 0 ;
						TRUEPREMOVE_Adjust() ;
						TOOLBOX_IssueISet(0, 0, 1, 1) ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MOVE_BUTTON :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Move-Input Configuration]->[Move Arrow Buttons...]") ;

					LpProc = MakeProcInstance((FARPROC) MoveButtonBoxWndProc, hInst) ;
					if(DialogBox(hInst, "MoveButtonBox", hwnd, (DLGPROC) LpProc))
					{
						TOOLBOX_InvalidateBoardWindow(0) ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_PARTNER_ASSIST :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Move-Input Configuration]->[Observe Board Partner Assist...]") ;

					LpProc = MakeProcInstance((FARPROC) PartnerAssistBoxWndProc, hInst) ;
					DialogBox(hInst, "PartnerAssistBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TELNET_OPTION :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Telnet Console Configuration]->[Telnet Console Options...]") ;

					LpProc = MakeProcInstance((FARPROC) TelnetOptionBoxWndProc, hInst) ;
					if(DialogBox(hInst, "TelnetOptionBox", hwnd, (DLGPROC) LpProc))
					{
						if(User.bMouseWheel)
						{
							SetFocus(hwndWindow [HWND_TELNET_TEXT]) ;
							PostMessage(hwndWindow [HWND_TELNET_TEXT], (UINT) 0, (WPARAM) 0, (LPARAM) 0) ;
							TELNET_GoBottom() ;
						}
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TELNET_CONFIGURE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Telnet Console Configuration]->[Telnet Console Colors...]") ;

					LpProc = MakeProcInstance((FARPROC) TelnetBoxWndProc, hInst) ;
					if(DialogBox(hInst, "TelnetBox", hwnd, (DLGPROC) LpProc))
					{
						TELNET_SetFont() ;
						TELNET_SetEditColor() ;
						Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
						SendMessage(hwndWindow [HWND_TELNET_TEXT], EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrColor [CLR_TELNET_BACKGROUND]) ;
						SendMessage(hwndWindow [HWND_TELNET_EDIT], EM_SETBKGNDCOLOR, FALSE, (LPARAM) clrColor [CLR_TELNET_BACKGROUND]) ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_APPLY_SOUND :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Sound Configuration]->[Apply Sound]") ;

					User.bSound = ! User.bSound ;
					TOOLBOX_WriteInfoOnOff(User.bSound, "Apply Sound is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SOUND_TELNET :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Sound Configuration]->[Telnet Console Sounds...]") ;

					LpProc = MakeProcInstance((FARPROC) SoundBoxWndProc, hInst) ;
					if(DialogBox(hInst, "SoundBox", hwnd, (DLGPROC) LpProc))
					{
						SOUND_Load() ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SOUND_GAME :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Sound Configuration]->[Game Sounds...]") ;

					LpProc = MakeProcInstance((FARPROC) GameSoundBoxWndProc, hInst) ;
					if(DialogBox(hInst, "GameSoundBox", hwnd, (DLGPROC) LpProc))
					{
						GAMESOUND_Load() ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SOUND_EAR :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Sound Configuration]->[Partner Communicator Sounds...]") ;

					LpProc = MakeProcInstance((FARPROC) EarBoxWndProc, hInst) ;
					if(DialogBox(hInst, "EarBox", hwnd, (DLGPROC) LpProc))
					{
						EAR_Load() ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SOUND_FINAL :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Sound Configuration]->[Final Countdown...]") ;

					LpProc = MakeProcInstance((FARPROC) FinalBoxWndProc, hInst) ;
					if(DialogBox(hInst, "FinalCountDownBox", hwnd, (DLGPROC) LpProc))
					{
						TIMESOUND_Load() ;
					}
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOGEVENT_CONFIGURE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Login Event...]") ;

					LpProc = MakeProcInstance((FARPROC) LoginEventBoxWndProc, hInst) ;
					DialogBox(hInst, "LoginEventBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_GAMEEVENT_CONFIGURE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Game Event...]") ;

					LpProc = MakeProcInstance((FARPROC) GameEventBoxWndProc, hInst) ;
					DialogBox(hInst, "GameEventBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RM_BUTTON :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Right Mouse Button Click on Partner Communicator Buttons...]") ;

					System.nRightMouseIndex = RIGHT_MOUSE_BUTTON ;
					LpProc = MakeProcInstance((FARPROC) RightMouseClickBoxWndProc, hInst) ;
					DialogBox(hInst, "RightMouseClickBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RM_PLAY :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Right Mouse Button Click on Playing Board...]") ;

					System.nRightMouseIndex = RIGHT_MOUSE_PLAY ;
					LpProc = MakeProcInstance((FARPROC) RightMouseClickBoxWndProc, hInst) ;
					DialogBox(hInst, "RightMouseClickBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RM_OBSERVE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Right Mouse Button Click on Observe Board...]") ;

					System.nRightMouseIndex = RIGHT_MOUSE_OBSERVE ;
					LpProc = MakeProcInstance((FARPROC) RightMouseClickBoxWndProc, hInst) ;
					DialogBox(hInst, "RightMouseClickBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RM_TELNET_TEXT :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Right Mouse Button Click on Telnet Console Text...]") ;

					System.nRightMouseIndex = RIGHT_MOUSE_TELNET_TEXT ;
					LpProc = MakeProcInstance((FARPROC) RightMouseClickBoxWndProc, hInst) ;
					DialogBox(hInst, "RightMouseClickBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RM_TELNET_EDIT :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Right Mouse Button Click on Telnet Console Command Line...]") ;

					System.nRightMouseIndex = RIGHT_MOUSE_TELNET_EDIT ;
					LpProc = MakeProcInstance((FARPROC) RightMouseClickBoxWndProc, hInst) ;
					DialogBox(hInst, "RightMouseClickBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RM_BOARD_HANDLE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Event Settings and Mouse Configuration]->[Right Mouse Button Click on Handle on Game Board...]") ;

					System.nRightMouseIndex = RIGHT_MOUSE_BOARD_HANDLE ;
					LpProc = MakeProcInstance((FARPROC) RightMouseClickBoxWndProc, hInst) ;
					DialogBox(hInst, "RightMouseClickBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_WHOAMI :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %whoami%]") ;

					Vars.nEditVar = 0 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_PARTNER :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %partner%]") ;

					Vars.nEditVar = 1 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_OPPONENT :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %opponent%]") ;

					Vars.nEditVar = 2 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_PARTNEROPP :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %partneropp%]") ;

					Vars.nEditVar = 3 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_LASTTELL :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %lasttell%]") ;

					Vars.nEditVar = 4 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_LASTKIB :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %lastkib%]") ;

					Vars.nEditVar = 5 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_LASTWHISPER :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %lastwhisper%]") ;

					Vars.nEditVar = 6 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_LASTSAY :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %lastsay%]") ;

					Vars.nEditVar = 7 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_TELNETHANDLE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %telnethandle%]") ;

					Vars.nEditVar = 8 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_BOARDHANDLE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %boardhandle%]") ;

					Vars.nEditVar = 9 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_USERHANDLE1 :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %userhandle1%]") ;

					Vars.nEditVar = 10 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EDIT_USERHANDLE2 :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Edit Variables]->[Edit %userhandle2%]") ;

					Vars.nEditVar = 11 ;
					LpProc = MakeProcInstance((FARPROC) VarsBoxWndProc, hInst) ;
					DialogBox(hInst, "VarsBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TIMER :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Timer Command...]") ;

					LpProc = MakeProcInstance((FARPROC) TimerBoxWndProc, hInst) ;
					DialogBox(hInst, "TimerBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SILENCE :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Silence Command...]") ;

					LpProc = MakeProcInstance((FARPROC) SilenceBoxWndProc, hInst) ;
					DialogBox(hInst, "SilenceBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_BUTTON_CONFIG :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Partner Communicator Buttons...]") ;

					LpProc = MakeProcInstance((FARPROC) ButtonBoxWndProc, hInst) ;
					DialogBox(hInst, "ButtonBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_FUNCTION :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Function Keys...]") ;

					LpProc = MakeProcInstance((FARPROC) FunctionBoxWndProc, hInst) ;
					DialogBox(hInst, "FunctionKeyBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_CONTROL_KEY :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Control Keys...]") ;

					LpProc = MakeProcInstance((FARPROC) ControlKeyBoxWndProc, hInst) ;
					DialogBox(hInst, "ControlKeyBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LAG_COMMAND :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Lag Command...]") ;

					LpProc = MakeProcInstance((FARPROC) LagCommandBoxWndProc, hInst) ;
					DialogBox(hInst, "LagCommandBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_FICS_CENSOR :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Personal Censor Filter]->[Configure FICS Censor List...]") ;

					System.nCensorIndex = 0 ;
					LpProc = MakeProcInstance((FARPROC) CensorBoxWndProc, hInst) ;
					DialogBox(hInst, "CensorBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_ICC_CENSOR :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Personal Censor Filter]->[Configure ICC Censor List...]") ;

					System.nCensorIndex = 1 ;
					LpProc = MakeProcInstance((FARPROC) CensorBoxWndProc, hInst) ;
					DialogBox(hInst, "CensorBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_NONFICS_CENSOR :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Customize Personal Censor Filter]->[Configure Censor List for Other ICS...]") ;

					System.nCensorIndex = 2 ;
					LpProc = MakeProcInstance((FARPROC) CensorBoxWndProc, hInst) ;
					DialogBox(hInst, "CensorBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_EXIT_PROMPT :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Exit Prompt]") ;

					User.bSuppressExitPrompt = ! User.bSuppressExitPrompt ;
					TOOLBOX_WriteInfoOnOff(User.bSuppressExitPrompt, "No Exit Prompt when connected is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SAVE_SET_NOW :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Save Settings Now]") ;

					TOOLBOX_WriteSystem("Save Settings Now\n") ;
					TOOLBOX_SaveAllWindowCoord() ;
					INI_WriteSetup(INI_GetIniFilename()) ;
					INI_WriteSystem(INI_GetSysFilename()) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SAVE_SET_EXIT :
					TOOLBOX_DisplayMenuPath("[Preferences]->[Save Settings on Exit]") ;

					User.bSaveOnExit = ! User.bSaveOnExit ;
					TOOLBOX_WriteInfoOnOff(User.bSaveOnExit, "Save Settings on Exit is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MAXIMIZE_TELNET :
					TOOLBOX_DisplayMenuPath("[Window]->[Maximize Telnet Console]") ;

					wCoord [COORD_TELNET].s = WIN_SHOW ;
					ShowWindow(hwndWindow [HWND_TELNET], SW_SHOWMAXIMIZED) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RESTORE_TELNET :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Telnet Console]") ;

					wCoord [COORD_TELNET].s = WIN_SHOW ;
					ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_MINIMIZE_TRAY :
					TOOLBOX_DisplayMenuPath("[Window]->[Minimize to Tray]") ;
					User.bMinimizetoTray = ! User.bMinimizetoTray ;
					TOOLBOX_WriteInfoOnOff(User.bMinimizetoTray, "Minimize to Tray is") ;
					TOOLBOX_CheckAllMenu() ;
					return 0 ;

				case IDM_SHOW_BITMAP_SIZE :
					TOOLBOX_DisplayMenuPath("[Window]->[Show Bitmap Size During Game Window Resize]") ;

					User.bShowBitmapSize = ! User.bShowBitmapSize ;
					TOOLBOX_WriteInfoOnOff(User.bShowBitmapSize, "Show Bitmap Size During Game Window Resize is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SHOW_MENU_PATH :
					TOOLBOX_DisplayMenuPath("[Window]->[Show Menu Path in Telnet Console Window]") ;

					User.bShowMenuPath = ! User.bShowMenuPath ;
					TOOLBOX_WriteInfoOnOff(User.bShowMenuPath, "Show Menu Path in Telnet Console Window is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TRY_LAYOUT :
					TOOLBOX_DisplayMenuPath("[Window]->[Try Window Layout]") ;

					LpProc = MakeProcInstance((FARPROC) TryBoxWndProc, hInst) ;
					DialogBox(hInst, "TryLayoutBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE0 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Default Layout]") ;

					LAYOUT_Restore(0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE1 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #1]") ;

					LAYOUT_Restore(1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE2 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #2]") ;

					LAYOUT_Restore(2) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE3 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #3]") ;

					LAYOUT_Restore(3) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE4 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #4]") ;

					LAYOUT_Restore(4) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE5 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #5]") ;

					LAYOUT_Restore(5) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE6 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #6]") ;

					LAYOUT_Restore(6) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE7 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #7]") ;

					LAYOUT_Restore(7) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_RESTORE8 :
					TOOLBOX_DisplayMenuPath("[Window]->[Restore Window Layout]->[Restore Layout #8]") ;

					LAYOUT_Restore(8) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE0 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Default Layout]") ;

					TOOLBOX_WriteSystem("Save as Default Layout\n") ;
					LAYOUT_Save(0) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE1 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #1]") ;

					TOOLBOX_WriteSystem("Save as Layout #1\n") ;
					LAYOUT_Save(1) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE2 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #2]") ;

					TOOLBOX_WriteSystem("Save as Layout #2\n") ;
					LAYOUT_Save(2) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE3 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #3]") ;

					TOOLBOX_WriteSystem("Save as Layout #3\n") ;
					LAYOUT_Save(3) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE4 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #4]") ;

					TOOLBOX_WriteSystem("Save as Layout #4\n") ;
					LAYOUT_Save(4) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE5 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #5]") ;

					TOOLBOX_WriteSystem("Save as Layout #5\n") ;
					LAYOUT_Save(5) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE6 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #6]") ;

					TOOLBOX_WriteSystem("Save as Layout #6\n") ;
					LAYOUT_Save(6) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE7 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #7]") ;

					TOOLBOX_WriteSystem("Save as Layout #7\n") ;
					LAYOUT_Save(7) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_WINDOW_SAVE8 :
					TOOLBOX_DisplayMenuPath("[Window]->[Save Window Layout]->[Save as Layout #8]") ;

					TOOLBOX_WriteSystem("Save as Layout #8\n") ;
					LAYOUT_Save(8) ;
					LAYOUT_LoadVirtual() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_HELP_TOPIC :
					TOOLBOX_DisplayMenuPath("[Help]->[Help Topics]") ;

					ShellExecute(NULL, "open", THE_BUGBOARD_HELP_TOPIC_URL, NULL, "", SW_SHOW) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_ASK_QUESTION :
					if(Timeseal.bSocketIsOpen)
					{
						TOOLBOX_DisplayMenuPath("[Help]->[Ask for Help on Chess Server...]") ;

						LpProc = MakeProcInstance((FARPROC) QuestionBoxWndProc, hInst) ;
						DialogBox(hInst, "QuestionBox", hwnd, (DLGPROC) LpProc) ;
						FreeProcInstance(LpProc) ;
						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					else
					{
						TOOLBOX_Beep() ;
						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					return 0 ;

				case IDM_ABOUT_THIEF :
					TOOLBOX_DisplayMenuPath("[Help]->[About Thief...]") ;

					LpProc = MakeProcInstance((FARPROC) AboutBoxWndProc, hInst) ;
					DialogBox(hInst, "AboutBox", hwnd, (DLGPROC) LpProc) ;
					FreeProcInstance(LpProc) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TOGGLE_TELNET :
					if(IsZoomed(hwndWindow [HWND_TELNET]))
					{
						PostMessage(hwndWindow [HWND_FRAME], WM_COMMAND, IDM_RESTORE_TELNET, (LPARAM) 0) ;
					}
					else
					{
						PostMessage(hwndWindow [HWND_FRAME], WM_COMMAND, IDM_MAXIMIZE_TELNET, (LPARAM) 0) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RESET_BOARD :
					if(IsWindow(System.hwndLastGame))
					{
						PostMessage(System.hwndLastGame, WM_USER_RESET_BOARD, (WPARAM) 0, (LPARAM) 0) ;
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_UNEXAM_GAME :
					if(Game [INDEX_PLAY].nGameNumber > 0 && ! Game [INDEX_PLAY].bPlaying)
					{
						TOOLBOX_WriteICS(ICS_UNEXAM_COMMAND) ;
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_CLOSE_BOARD :
					if(IsWindow(System.hwndLastGame))
					{
						PostMessage(System.hwndLastGame, WM_CLOSE, (WPARAM) 0, (LPARAM) 0) ;
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_AUTO_QUEEN :
					User.bAutoQueen = ! User.bAutoQueen ;
					TOOLBOX_WriteInfoOnOff(User.bAutoQueen, "Auto Queen/King is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_AUTO_OBSERVE :
					User.bAutoObserve = ! User.bAutoObserve ;
					TOOLBOX_WriteInfoOnOff(User.bAutoObserve, "Apply Auto Observe is") ;
					TOOLBOX_CheckAllMenu() ;
					Fics.bPobserve = 1 ;
					Fics.nPobserve = 0 ;
					strcpy(Fics.cPobserve, "") ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_AUTO_REMATCH :
					User.bAutoRematch = ! User.bAutoRematch ;
					TOOLBOX_WriteInfoOnOff(User.bAutoRematch, "Apply Auto Rematch is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_COORDINATES_SHOW :
					User.bShowCoord = ! User.bShowCoord ;
					TOOLBOX_WriteInfoOnOff(User.bShowCoord, "Show Coordinates is") ;
					TOOLBOX_CheckAllMenu() ;
					TOOLBOX_InvalidateBoardWindow(0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SMART_MOVE :
					User.bSmartMove = ! User.bSmartMove ;
					SMARTMOVE_Adjust() ;
					TOOLBOX_WriteInfoOnOff(User.bSmartMove, "Smart Move is") ;
					TOOLBOX_CheckAllMenu() ;
					TOOLBOX_IssueISet(0, 0, 0, 1) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TRUE_PREMOVE_THIEF :
					switch(User.nTruePremove)
					{
						case PREMOVE_NONE :
							User.nTruePremove = PREMOVE_THIEF ;
							break ;

						case PREMOVE_THIEF :
							User.nTruePremove = PREMOVE_NONE ;
							break ;

						case PREMOVE_LR :
							User.nTruePremove = PREMOVE_THIEF ;
							break ;

						default :
							User.nTruePremove = PREMOVE_THIEF ;
							break ;
					}
					TRUEPREMOVE_Adjust() ;

					switch(User.nTruePremove)
					{
						case PREMOVE_NONE :
							TOOLBOX_WriteSystem("True Premove is Off\n") ;
							break ;

						case PREMOVE_THIEF :
							TOOLBOX_WriteSystem("True Premove is Thief Style\n") ;
							break ;

						case PREMOVE_LR :
							TOOLBOX_WriteSystem("True Premove is Lightning Rulez Style\n") ;
							break ;
					}
					TOOLBOX_CheckAllMenu() ;
					TOOLBOX_IssueISet(0, 0, 1, 0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TRUE_PREMOVE_LR :
					switch(User.nTruePremove)
					{
						case PREMOVE_NONE :
							User.nTruePremove = PREMOVE_LR ;
							break ;

						case PREMOVE_THIEF :
							User.nTruePremove = PREMOVE_LR ;
							break ;

						case PREMOVE_LR :
							User.nTruePremove = PREMOVE_NONE ;
							break ;

						default :
							User.nTruePremove = PREMOVE_LR ;
							break ;
					}
					TRUEPREMOVE_Adjust() ;

					switch(User.nTruePremove)
					{
						case PREMOVE_NONE :
							TOOLBOX_WriteSystem("True Premove is Off\n") ;
							break ;

						case PREMOVE_THIEF :
							TOOLBOX_WriteSystem("True Premove is Thief Style\n") ;
							break ;

						case PREMOVE_LR :
							TOOLBOX_WriteSystem("True Premove is Lightning Rulez Style\n") ;
							break ;
					}
					TOOLBOX_CheckAllMenu() ;
					TOOLBOX_IssueISet(0, 0, 1, 0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_RIGHT_MOUSE_PARTNER :
					User.bRightMousePartner = ! User.bRightMousePartner ;
					TOOLBOX_WriteInfoOnOff(User.bRightMousePartner, "Right Mouse Click on Partner's Board to Drop is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TEST_LEGAL_PLAY :
					User.bTestLegalPlay = ! User.bTestLegalPlay ;
					TOOLBOX_WriteInfoOnOff(User.bTestLegalPlay, "Test Legality on Playing Board is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TEST_LEGAL_PARTNER :
					User.bTestLegalPartner = ! User.bTestLegalPartner ;
					TOOLBOX_WriteInfoOnOff(User.bTestLegalPartner, "Test Legality on Partner's Board is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_ANIMATE_OBSERVE :
					User.bAnimateObserve = ! User.bAnimateObserve ;
					TOOLBOX_WriteInfoOnOff(User.bAnimateObserve, "Animate Observe Game Move (While Not Playing) is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_ANIMATE_EXAM :
					User.bAnimateExam = ! User.bAnimateExam ;
					TOOLBOX_WriteInfoOnOff(User.bAnimateExam, "Animate Exam Game Move is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_APPLY_TELNET_SOUND :
					User.bTelnetSound = ! User.bTelnetSound ;
					TOOLBOX_WriteInfoOnOff(User.bTelnetSound, "Apply Telnet Console Sounds is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_APPLY_GAME_SOUND :
					User.bGameSound = ! User.bGameSound ;
					TOOLBOX_WriteInfoOnOff(User.bGameSound, "Apply Game Sounds is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_APPLY_EAR_SOUND :
					User.bEarSound = ! User.bEarSound ;
					TOOLBOX_WriteInfoOnOff(User.bEarSound, "Apply Partner Communicator Sounds is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_FINAL_COUNTDOWN :
					User.bFinalCountDown = ! User.bFinalCountDown ;
					TOOLBOX_WriteInfoOnOff(User.bFinalCountDown, "Apply Final Countdown is") ;
					TOOLBOX_CheckAllMenu() ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TOGGLE_TIMER_ONOFF :
					if(TimerCmd.bTimerCommand)
					{
						TOOLBOX_WriteInfoOnOff(! TimerCmd.bTimerCommand, "Timer Command is") ;
						TIMERCMD_Stop() ;
					}
					else
					{
						TOOLBOX_WriteInfoOnOff(! TimerCmd.bTimerCommand, "Timer Command is") ;
						TIMERCMD_Start() ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_BPGN :
					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "bpgn",
												BPGN_FILT,
												"Load BPGN Games",
												NULL, NULL, cTmp, Browse.cSaveBPGNBrowse) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSaveBPGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_LOAD_PGN :
					Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "pgn",
												PGN_FILT,
												"Load PGN Games",
												NULL, NULL, cTmp, Browse.cSavePGNBrowse) ;
					if(Fv != NULL)
					{
						fclose(Fv) ;
						TOOLBOX_GetPath(cTmp, Browse.cSavePGNBrowse) ;
						INI_WriteSystem(INI_GetSysFilename()) ;
						TOOLBOX_OpenGame(cTmp) ;
					}
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SET_TF_TRUE_PREMOVE :
					User.nTruePremove = PREMOVE_THIEF ;

					TRUEPREMOVE_Adjust() ;

					TOOLBOX_WriteSystem("True Premove is Thief Style\n") ;

					TOOLBOX_CheckAllMenu() ;
					TOOLBOX_IssueISet(0, 0, 1, 0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SET_LR_TRUE_PREMOVE :
					User.nTruePremove = PREMOVE_LR ;

					TRUEPREMOVE_Adjust() ;

					TOOLBOX_WriteSystem("True Premove is Lightning Rulez Style\n") ;

					TOOLBOX_CheckAllMenu() ;
					TOOLBOX_IssueISet(0, 0, 1, 0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SET_AUTO_KNIGHT_ON :
					System.bPromoteKnight = 1 ;
					TOOLBOX_WriteSystem("Auto Knight Promotion is On\n") ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_SET_AUTO_KNIGHT_OFF :
					System.bPromoteKnight = 0 ;
					TOOLBOX_WriteSystem("Auto Knight Promotion is Off\n") ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				case IDM_TEXT_SELECTALL :
				case IDM_TEXT_COPY :
				case IDM_TEXT_QUICKPASTE :
				case IDM_TEXT_SAVETOFILE :
				case IDM_EDIT_SELECTALL :
				case IDM_EDIT_COPY :
				case IDM_EDIT_CUT :
				case IDM_EDIT_PASTE :
					SendMessage(hwndWindow [HWND_TELNET_TEXT], WM_COMMAND, wParam, 0) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;

				default :
					hwndChild = (HWND) SendMessage(hwndWindow [HWND_CLIENT], WM_MDIGETACTIVE, 0, 0) ;
					if(IsWindow(hwndChild))
					{
						SendMessage(hwndChild, WM_COMMAND, wParam, lParam) ;

						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					if(wParam == IDM_CHILD_TELNET)
					{
						TOOLBOX_DisplayMenuPath("[Window]->[Telnet Console]") ;

						if(wCoord [COORD_TELNET].s == WIN_HIDE)
						{
							wCoord [COORD_TELNET].s = WIN_SHOW ;
							SendMessage(hwndWindow [HWND_CLIENT],
										WM_MDIACTIVATE,
										(WPARAM)(HWND)
										hwndWindow [HWND_TELNET], 0) ;

							ShowWindow(hwndWindow [HWND_TELNET], SW_SHOW) ;
							SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
						}
					}
					else if(wParam == IDM_CHILD_BUTTON)
					{
						TOOLBOX_DisplayMenuPath("[Window]->[Partner Communicator Buttons]") ;

						if(wCoord [COORD_BUTTON].s == WIN_HIDE)
						{
							wCoord [COORD_BUTTON].s = WIN_SHOW ;
							SendMessage(hwndWindow [HWND_CLIENT],
										WM_MDIACTIVATE,
										(WPARAM)(HWND)
										hwndWindow [HWND_BUTTON], 0) ;

							ShowWindow(hwndWindow [HWND_BUTTON], SW_SHOW) ;
							SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
						}
					}
					else if(wParam == IDM_CHILD_PLAY)
					{
						TOOLBOX_DisplayMenuPath("[Window]->[Playing Board]") ;

						if(wCoord [COORD_PLAY].s == WIN_HIDE)
						{
							wCoord [COORD_PLAY].s = WIN_SHOW ;
							SendMessage(hwndWindow [HWND_CLIENT],
										WM_MDIACTIVATE,
										(WPARAM)(HWND)
										hwndWindow [HWND_PLAY], 0) ;

							ShowWindow(hwndWindow [HWND_PLAY], SW_SHOW) ;
							SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
						}
					}
					break ;
			}
			break ;

		case WM_CLOSE :
			if(! User.bSuppressExitPrompt)
			{
				if(Timeseal.bSocketIsOpen)
				{
					if(IDOK != MessageBox(hwnd,
										  "OK to end Thief?",
										  "Thief",
										  MB_ICONQUESTION | MB_OKCANCEL))
					{
						return 0 ;
					}
				}
			}

			TOOLBOX_SaveAllWindowCoord() ;

			if(Timeseal.bSocketIsOpen)
			{
				TOOLBOX_WriteICS(ICS_QUIT_COMMAND) ;
				TOOLBOX_WriteSystem(ICS_CONN_LOST_DISPLAY) ;
				TOOLBOX_CloseSocket() ;
			}

			TOOLBOX_CloseObserveWindow() ;
			GAME_InitAllGame(0) ;
			GAME_InitialSetup(INDEX_PLAY) ;

			TOOLBOX_InvalidateBoardWindow(0) ;
			TOOLBOX_RedrawBoard(INDEX_PLAY) ;
			SetWindowText(hwndWindow [HWND_PLAY], PLAY_BOARD_TITLE) ;
			break ;

		case WM_DESTROY :
			TOOLBOX_ShowTaskIcon(hwnd, FALSE);
			DestroyIcon(hIcon);
			WSACleanup() ;
			PostQuitMessage(0) ;
			return 0 ;

		default :
			// Has Task Tray been restarted in the meantime?
			if(iMsg == suTaskbarRestart)
			{
				TOOLBOX_ShowTaskIcon(hwnd, FALSE);
			}
			if(User.bMouseWheel)
			{
				if(iMsg == WM_MOUSEWHEEL)
				{
					PostMessage(hwndWindow [HWND_TELNET_TEXT], iMsg, wParam, lParam) ;
					SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					return 0 ;
				}
			}
			break ;
	}
	return DefFrameProc(hwnd, hwndWindow [HWND_CLIENT], iMsg, wParam, lParam) ;
}
