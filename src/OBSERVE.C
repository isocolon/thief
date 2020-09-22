#include "thief.h"

LRESULT CALLBACK ObserveWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc ;
	PAINTSTRUCT ps ;
	HMENU       hMenu, hSubMenu ;
	POINT       pt ;
	MINMAXINFO  *mmi ;
	int         nI, nJ, nK, nMx, nMy, nX, nY ;
	char        cTmp [THIEF_COMMAND_SIZE + 10] ;

	switch(iMsg)
	{
		case WM_TIMER :
			switch(wParam)
			{
				case TIMER_CLOCK_ID1 :
				case TIMER_CLOCK_ID2 :
				case TIMER_CLOCK_ID3 :
				case TIMER_CLOCK_ID4 :
				case TIMER_CLOCK_ID5 :
				case TIMER_CLOCK_ID6 :
				case TIMER_CLOCK_ID7 :
				case TIMER_CLOCK_ID8 :
				case TIMER_CLOCK_ID9 :
					if(Timeseal.bSocketIsOpen)
					{
						for(nI = 1 ; nI < MAX_GAME ; nI++)
						{
							if(hwnd == Game [nI].hwnd)
							{
								if(Game [nI].bValid && Game [nI].nGameNumber > 0)
								{
									if(Login.nLoginType == SERVER_FICS)
									{
										KillTimer(hwnd, Game [nI].nClockTimerEvent) ;
										Game [nI].nClockTimerEvent = 0 ;
										CLOCK_DecrementClocks(nI, hwnd) ;
									}
									else
									{
										if(Game [nI].bPlaying)
										{
											KillTimer(hwnd, Game [nI].nClockTimerEvent) ;
											Game [nI].nClockTimerEvent = 0 ;
											CLOCK_DecrementClocks(nI, hwnd) ;
										}
									}
								}
								break ;
							}
						}
					}
					break ;
			}
			break ;

		case WM_KEYDOWN :
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
			return 0;

		case WM_CHAR :
			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
			return 0 ;

		case WM_PAINT :
			hdc = BeginPaint(hwnd, &ps) ;
			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					BOARD_CheckFlip(nI) ;
					BOARD_DrawRepaint(nI, hwnd, hdc) ;
					break ;
				}
			}
			EndPaint(hwnd, &ps) ;

			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			return 0 ;

		case WM_LBUTTONDOWN :
			// reset left mouse drop
			LeftMouseDrop.nI = -1 ;
			LeftMouseDrop.nX = -1 ;
			LeftMouseDrop.nY = -1 ;

			System.hwndLastGame = hwnd ;
			ReleaseCapture() ;

			if(User.nMoveType == CLICK_MOVE)
			{
				if(DragInfo.nClicked != 0)
				{
					DragInfo.nClicked = 2 ;
					return 0 ;
				}
			}

			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					pt.x = LOWORD(lParam) ;
					pt.y = HIWORD(lParam) ;

					if(PtInRect(&Game [nI].rBoard,   pt) ||
							PtInRect(&Game [nI].rBuffer,  pt) ||
							PtInRect(&Game [nI].rBuffer1, pt))
					{
						if(! TOOLBOX_DisplayActualBoard(nI))
						{
							hdc = GetDC(hwnd) ;
							BOARD_RestoreDragMove(nI, hdc) ;
							ReleaseDC(hwnd, hdc) ;
						}
					}

					if(Game [nI].nGameNumber != 0 && Game [nI].nGameNumber == Game [INDEX_PLAY].nGamePartner)
					{
						if(User.bAutoCenterDragPiece)
						{
							nMx = LOWORD(lParam) ;
							nMy = HIWORD(lParam) ;
						}
						else
						{
							nMx = (LOWORD(lParam) - DragInfo.ptDist.x) + Game [nI].nhss ;
							nMy = (HIWORD(lParam) - DragInfo.ptDist.y) + Game [nI].nhss ;
						}

						if(nMx >= Game [nI].rBoard.left  &&
								nMx <= Game [nI].rBoard.right &&
								nMy >= Game [nI].rBoard.top   &&
								nMy <= Game [nI].rBoard.bottom)
						{
							nX = (nMx - Game [nI].rBoard.left) / Game [nI].nss ;
							nY = (nMy - Game [nI].rBoard.top)  / Game [nI].nss ;

							if(nX < 0 || nX > 7 || nY < 0 || nY > 7)
							{
								return 0 ;
							}

							hdc = GetDC(hwnd) ;
							SetCapture(hwnd) ;
							if(BOARD_OnMouseDown1(nI, hdc, 1, nMx, nMy, nX, nY))
							{
								ReleaseCapture() ;
							}
							ReleaseDC(hwnd, hdc) ;
						}
						else if((nMx >= Game [nI].rBuffer.left   &&
								 nMx <= Game [nI].rBuffer.right  &&
								 nMy >= Game [nI].rBuffer.top    &&
								 nMy <= Game [nI].rBuffer.bottom) ||
								(nMx >= Game [nI].rBuffer1.left  &&
								 nMx <= Game [nI].rBuffer1.right &&
								 nMy >= Game [nI].rBuffer1.top   &&
								 nMy <= Game [nI].rBuffer1.bottom))
						{
							if((Game [nI].nGameType == GAMETYPE_BUGHOUSE) ||
									(Game [nI].nGameType == GAMETYPE_CRAZYHOUSE))
							{
								hdc = GetDC(hwnd) ;
								SetCapture(hwnd) ;
								if(BOARD_OnMouseDown1(nI, hdc, 0, nMx, nMy, 0, 0))
								{
									ReleaseCapture() ;
								}
								ReleaseDC(hwnd, hdc) ;
							}
							else
							{
								DragInfo.nIndex   = -1 ;
								DragInfo.nPc      = EMPTY_SQUARE ;
								DragInfo.ptFrom.x = -1 ;
								DragInfo.ptFrom.y = -1 ;
							}
						}
						else
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
						}
						return 0 ;
					}
					break ;
				}
			}

			DragInfo.nIndex   = -1 ;
			DragInfo.nPc      = EMPTY_SQUARE ;
			DragInfo.ptFrom.x = -1 ;
			DragInfo.ptFrom.y = -1 ;
			return 0 ;

		case WM_MOUSEMOVE :
			if((User.nMoveType != DRAG_MOVE) || DragInfo.nPc == EMPTY_SQUARE)
			{
				ReleaseCapture() ;
			}
			else
			{
				for(nI = 1 ; nI < MAX_GAME ; nI++)
				{
					if(hwnd == Game [nI].hwnd)
					{
						if(Game [nI].nGameNumber != 0 && Game [nI].nGameNumber == Game [INDEX_PLAY].nGamePartner)
						{
							hdc = GetDC(hwnd) ;
							BOARD_OnMouseMove1(nI, hdc, LOWORD(lParam), HIWORD(lParam)) ;
							ReleaseDC(hwnd, hdc) ;
						}
						break ;
					}
				}
			}
			return 0 ;

		case WM_USER_MOUSELEAVE :
			ReleaseCapture() ;

			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					if(Game [nI].nGameNumber != 0 && Game [nI].nGameNumber == Game [INDEX_PLAY].nGamePartner)
					{
						hdc = GetDC(hwnd) ;
						BOARD_RestoreDragMove(nI, hdc) ;
						ReleaseDC(hwnd, hdc) ;
					}
					break ;
				}
			}
			return 0 ;

		case WM_LBUTTONUP :
			// reset left mouse drop
			LeftMouseDrop.nI = -1 ;
			LeftMouseDrop.nX = -1 ;
			LeftMouseDrop.nY = -1 ;

			ReleaseCapture() ;

			if(DragInfo.nPc == EMPTY_SQUARE)
			{
				return 0 ;
			}

			if(User.nMoveType == CLICK_MOVE)
			{
				if(DragInfo.nClicked != 2)
				{
					return 0 ;
				}
			}

			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					if(Game [nI].nGameNumber != 0 && Game [nI].nGameNumber == Game [INDEX_PLAY].nGamePartner)
					{
						hdc = GetDC(hwnd) ;

						nMx = LOWORD(lParam) ;
						nMy = HIWORD(lParam) ;

						if(nMx >= Game [nI].rBoard.left  &&
								nMx <= Game [nI].rBoard.right &&
								nMy >= Game [nI].rBoard.top   &&
								nMy <= Game [nI].rBoard.bottom)
						{
							if(! BOARD_OnMouseUp1(nI, hwnd, hdc,
												  (nMx - Game [nI].rBoard.left) / Game [nI].nss,
												  (nMy - Game [nI].rBoard.top)  / Game [nI].nss))
							{
								BOARD_RestoreDragMove(nI, hdc) ;
							}
						}
						else
						{
							BOARD_RestoreDragMove(nI, hdc) ;
						}
						ReleaseDC(hwnd, hdc) ;
					}
					break ;
				}
			}
			return 0 ;

		case WM_RBUTTONDOWN :
			// reset left mouse drop
			LeftMouseDrop.nI = -1 ;
			LeftMouseDrop.nX = -1 ;
			LeftMouseDrop.nY = -1 ;

			System.hwndLastGame = hwnd ;
			ReleaseCapture() ;

			nMx = LOWORD(lParam) ;
			nMy = HIWORD(lParam) ;

			pt.x = nMx ;
			pt.y = nMy ;

			nJ = 0 ;
			nK = 0 ;
			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					if(PtInRect(&Game [nI].rHandle [INDEX_WHITE], pt))
					{
						strcpy(Vars.cBoardHandle, Game [nI].cHandle [INDEX_WHITE]) ;
						nJ = 1 ;
					}
					else if(PtInRect(&Game [nI].rHandle [INDEX_BLACK], pt))
					{
						strcpy(Vars.cBoardHandle, Game [nI].cHandle [INDEX_BLACK]) ;
						nJ = 1 ;
					}
					else if(Game [nI].nGameNumber != 0 && Game [nI].nGameNumber == Game [INDEX_PLAY].nGamePartner)
					{
						nK = nI ;
						if(DragInfo.nPc != EMPTY_SQUARE)
						{
							hdc = GetDC(hwnd) ;
							BOARD_RestoreDragMove(nI, hdc) ;
							ReleaseDC(hwnd, hdc) ;
						}
					}
					else if(PtInRect(&Game [nI].rBoard,   pt) ||
							PtInRect(&Game [nI].rBuffer,  pt) ||
							PtInRect(&Game [nI].rBuffer1, pt))
					{
						if(! TOOLBOX_DisplayActualBoard(nI))
						{
							hdc = GetDC(hwnd) ;
							BOARD_RestoreDragMove(nI, hdc) ;
							ReleaseDC(hwnd, hdc) ;
						}
					}
					break ;
				}
			}

			if(nJ)
			{
				nJ = 0 ;
				for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
				{
					if(strlen(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI]) == 0)
					{
						break ;
					}

					if(TOOLBOX_IsSeparator(RIGHT_MOUSE_BOARD_HANDLE, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_BOARD_HANDLE ;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU") ;
							hSubMenu = GetSubMenu(hMenu, 0) ;

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION) ;
						}

						AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0) ;

						nJ = nJ + 1 ;
					}
					else if(TOOLBOX_OKRightMouse(RIGHT_MOUSE_BOARD_HANDLE, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_BOARD_HANDLE ;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU") ;
							hSubMenu = GetSubMenu(hMenu, 0) ;

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION) ;
						}

						if(strchr(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI], '%'))
						{
							TOOLBOX_ExpandAllMacro(cTmp, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI], 0) ;
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), cTmp) ;
						}
						else
						{
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI]) ;
						}

						if(FCheck [RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [nI]])
						{
							CheckMenuItem(hSubMenu, (WM_USER_RIGHT_MOUSE0 + nI), MF_CHECKED) ;
						}

						nJ = nJ + 1 ;
					}
				}
				if(nJ > 0)
				{
					pt.x = nMx ;
					pt.y = nMy ;
					TOOLBOX_MenuPopup(hwnd, pt, hMenu, -1) ;
				}
				return 0 ;
			}

			if(nK)
			{
				if(Game [nK].bPlaying)
				{
					// playing
					if(User.bRightMousePartner)
					{
						if(nMx >= Game [nK].rBoard.left  &&
								nMx <= Game [nK].rBoard.right &&
								nMy >= Game [nK].rBoard.top   &&
								nMy <= Game [nK].rBoard.bottom)
						{
							nX = (nMx - Game [nK].rBoard.left) / Game [nK].nss ;
							nY = (nMy - Game [nK].rBoard.top)  / Game [nK].nss ;

							if(nX < 0 || nX > 7 || nY < 0 || nY > 7)
							{
								nK = 0 ;
							}
							else
							{
								if(Game [nK].bFlip)
								{
									nX = ReverseCoord [nX] ;
								}
								else
								{
									nY = ReverseCoord [nY] ;
								}

								RightMouseDrop.nI = nK ;
								RightMouseDrop.nX = nX ;
								RightMouseDrop.nY = nY ;

								if((GetKeyState(VK_CONTROL) & ~1))
								{
									//
									// control right mouse click (drop piece menu for partner's opponent)
									//
									hMenu = LoadMenu(hInst, "OPPONENTDROPPIECEMENU") ;

									if(Game [INDEX_PLAY].bIPlayWhite)
									{
										// opponent pawn
										sprintf(cTmp, "Pawn  %d", Game [nK].nBuffer [WHITE_PAWN]) ;
										ModifyMenu(hMenu, IDM_POPP_PAWN, MF_BYCOMMAND | MF_STRING |   MF_ENABLED, IDM_POPP_PAWN,   cTmp) ;

										// opponent knight
										sprintf(cTmp, "Knight  %d", Game [nK].nBuffer [WHITE_KNIGHT]) ;
										ModifyMenu(hMenu, IDM_POPP_KNIGHT, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_POPP_KNIGHT, cTmp) ;

										// opponent bishop
										sprintf(cTmp, "Bishop  %d", Game [nK].nBuffer [WHITE_BISHOP]) ;
										ModifyMenu(hMenu, IDM_POPP_BISHOP, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_POPP_BISHOP, cTmp) ;

										// opponent rook
										sprintf(cTmp, "Rook  %d", Game [nK].nBuffer [WHITE_ROOK]) ;
										ModifyMenu(hMenu, IDM_POPP_ROOK, MF_BYCOMMAND | MF_STRING | MF_ENABLED,   IDM_POPP_ROOK,  cTmp) ;

										// opponent queen
										sprintf(cTmp, "Queen  %d", Game [nK].nBuffer [WHITE_QUEEN]) ;
										ModifyMenu(hMenu, IDM_POPP_QUEEN, MF_BYCOMMAND | MF_STRING | MF_ENABLED,  IDM_POPP_QUEEN, cTmp) ;
									}
									else
									{
										// opponent pawn
										sprintf(cTmp, "Pawn  %d", Game [nK].nBuffer [BLACK_PAWN]) ;
										ModifyMenu(hMenu, IDM_POPP_PAWN, MF_BYCOMMAND | MF_STRING | MF_ENABLED,   IDM_POPP_PAWN,   cTmp) ;

										// opponent knight
										sprintf(cTmp, "Knight  %d", Game [nK].nBuffer [BLACK_KNIGHT]) ;
										ModifyMenu(hMenu, IDM_POPP_KNIGHT, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_POPP_KNIGHT, cTmp) ;

										// opponent bishop
										sprintf(cTmp, "Bishop  %d", Game [nK].nBuffer [BLACK_BISHOP]) ;
										ModifyMenu(hMenu, IDM_POPP_BISHOP, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_POPP_BISHOP, cTmp) ;

										// opponent rook
										sprintf(cTmp, "Rook  %d", Game [nK].nBuffer [BLACK_ROOK]) ;
										ModifyMenu(hMenu, IDM_POPP_ROOK, MF_BYCOMMAND | MF_STRING | MF_ENABLED,   IDM_POPP_ROOK,   cTmp) ;

										// opponent queen
										sprintf(cTmp, "Queen  %d", Game [nK].nBuffer [BLACK_QUEEN]) ;
										ModifyMenu(hMenu, IDM_POPP_QUEEN, MF_BYCOMMAND | MF_STRING | MF_ENABLED,  IDM_POPP_QUEEN,  cTmp) ;
									}
								}
								else
								{
									//
									// drop piece for partner
									//
									hMenu = LoadMenu(hInst, "PARTNERDROPPIECEMENU") ;

									if(Game [INDEX_PLAY].bIPlayWhite)
									{
										// pawn
										sprintf(cTmp, "Pawn  %d", Game [nK].nBuffer [BLACK_PAWN]) ;
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_PAWN, cTmp) ;

										// knight
										sprintf(cTmp, "Knight  %d", Game [nK].nBuffer [BLACK_KNIGHT]) ;
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_KNIGHT, cTmp) ;

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [nK].nBuffer [BLACK_BISHOP]) ;
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_BISHOP, cTmp) ;

										// rook
										sprintf(cTmp, "Rook  %d", Game [nK].nBuffer [BLACK_ROOK]) ;
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_ROOK, cTmp) ;

										// queen
										sprintf(cTmp, "Queen  %d", Game [nK].nBuffer [BLACK_QUEEN]) ;
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_QUEEN, cTmp) ;
									}
									else
									{
										// pawn
										sprintf(cTmp, "Pawn  %d", Game [nK].nBuffer [WHITE_PAWN]) ;
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_PAWN, cTmp) ;

										// knight
										sprintf(cTmp, "Knight  %d", Game [nK].nBuffer [WHITE_KNIGHT]) ;
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_KNIGHT, cTmp) ;

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [nK].nBuffer [WHITE_BISHOP]) ;
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_BISHOP, cTmp) ;

										// rook
										sprintf(cTmp, "Rook  %d", Game [nK].nBuffer [WHITE_ROOK]) ;
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_ROOK, cTmp) ;

										// queen
										sprintf(cTmp, "Queen  %d", Game [nK].nBuffer [WHITE_QUEEN]) ;
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_QUEEN, cTmp) ;
									}
								}

								nK = 1 ;
							}
						}
						else
						{
							nK = 0 ;
						}
					}
					else
					{
						nK = 0 ;
					}
				}
				else
				{
					nK = 0 ;
				}
			}

			if(nK)
			{
				pt.x = nMx ;
				pt.y = nMy ;
				TOOLBOX_MenuPopup(hwnd, pt, hMenu, -1) ;
			}
			else
			{
				nJ = 0 ;
				for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
				{
					if(strlen(RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [nI]) == 0)
					{
						break ;
					}

					if(TOOLBOX_IsSeparator(RIGHT_MOUSE_OBSERVE, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_OBSERVE ;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU") ;
							hSubMenu = GetSubMenu(hMenu, 0) ;

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION) ;
						}

						AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0) ;

						nJ = nJ + 1 ;
					}
					else if(TOOLBOX_OKRightMouse(RIGHT_MOUSE_OBSERVE, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_OBSERVE ;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU") ;
							hSubMenu = GetSubMenu(hMenu, 0) ;

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION) ;
						}

						if(strchr(RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [nI], '%'))
						{
							TOOLBOX_ExpandAllMacro(cTmp, RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [nI], 0) ;
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), cTmp) ;
						}
						else
						{
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [nI]) ;
						}

						if(FCheck [RightMouse [RIGHT_MOUSE_OBSERVE].nType [nI]])
						{
							CheckMenuItem(hSubMenu, (WM_USER_RIGHT_MOUSE0 + nI), MF_CHECKED) ;
						}

						nJ = nJ + 1 ;
					}
				}
				if(nJ > 0)
				{
					pt.x = nMx ;
					pt.y = nMy ;
					TOOLBOX_MenuPopup(hwnd, pt, hMenu, -1) ;
				}
			}
			return 0 ;

		case WM_CREATE :
			Game [System.nGameIndex].hbBackground = CreateSolidBrush(clrColor [CLR_WINDOW_COLOR]) ;

			DeleteObject((HBRUSH) SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG) Game [System.nGameIndex].hbBackground)) ;

			Game [System.nGameIndex].hGame = ((LPCREATESTRUCT) lParam)->hInstance ;

			Game [System.nGameIndex].hwnd  = hwnd ;

			Game [System.nGameIndex].hwnd1 = CreateWindow("BUTTON", "<<",
											 WS_CHILD | BS_PUSHBUTTON,
											 0, 0, 20, 20, hwnd,
											 (HMENU) 0, Game [System.nGameIndex].hGame, NULL) ;

			Game [System.nGameIndex].hwnd2 = CreateWindow("BUTTON", "<",
											 WS_CHILD | BS_PUSHBUTTON,
											 0, 0, 20, 20, hwnd,
											 (HMENU) 1, Game [System.nGameIndex].hGame, NULL) ;

			Game [System.nGameIndex].hwnd3 = CreateWindow("BUTTON", ">",
											 WS_CHILD | BS_PUSHBUTTON,
											 0, 0, 20, 20, hwnd,
											 (HMENU) 2, Game [System.nGameIndex].hGame, NULL) ;

			Game [System.nGameIndex].hwnd4 = CreateWindow("BUTTON", ">>",
											 WS_CHILD | BS_PUSHBUTTON,
											 0, 0, 20, 20, hwnd,
											 (HMENU) 3, Game [System.nGameIndex].hGame, NULL) ;

			TOOLBOX_ShowMoveButtons(System.nGameIndex, 0) ;

			POSITION_Init(System.nGameIndex) ;

			CLOCK_StartClocks(System.nGameIndex, hwnd) ;
			return 0 ;

		case WM_MDIACTIVATE :
			System.hwndLastGame = hwnd ;
			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			break ;

		case WM_USER_RESET_BOARD :
			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					if(Game [nI].bValid && Game [nI].nGameNumber > 0)
					{
						TOOLBOX_Beep() ;
					}
					else
					{
						if(Telnet.nTelnetState == WIN_MAXIMIZE)
						{
							ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE) ;
						}

						if(Game [nI].nState == WIN_MINIMIZE)
						{
							ShowWindow(Game [nI].hwnd, SW_RESTORE) ;
						}

						GAME_InitialSetup(nI) ;

						hdc = GetDC(hwnd) ;
						BOARD_CheckFlip(nI) ;
						BOARD_DrawRepaint(nI, hwnd, hdc) ;
						ReleaseDC(hwnd, hdc) ;

						SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
					}
					break ;
				}
			}
			return 0 ;

		case WM_USER_FLIP_BOARD :
			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					Game [nI].bFlip = ! Game [nI].bFlip ;

					if(Telnet.nTelnetState == WIN_MAXIMIZE)
					{
						ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE) ;
					}

					if(Game [nI].nState == WIN_MINIMIZE)
					{
						ShowWindow(Game [nI].hwnd, SW_RESTORE) ;
					}

					hdc = GetDC(hwnd) ;
					BOARD_CheckFlip(nI) ;
					BOARD_DrawRepaint(nI, hwnd, hdc) ;
					ReleaseDC(hwnd, hdc) ;

					if(Game [nI].nGamePartner != 0)
					{
						if(Game [nI].nGamePartner != Game [INDEX_PLAY].nGameNumber)
						{
							for(nJ = 1 ; nJ < MAX_GAME ; nJ++)
							{
								if(Game [nJ].nGameNumber == Game [nI].nGamePartner)
								{
									if(IsWindow(Game [nJ].hwnd))
									{
										Game [nJ].bFlip = ! Game [nJ].bFlip ;

										if(Telnet.nTelnetState == WIN_MAXIMIZE)
										{
											ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE) ;
										}

										if(Game [nJ].nState == WIN_MINIMIZE)
										{
											ShowWindow(Game [nJ].hwnd, SW_RESTORE) ;
										}

										hdc = GetDC(Game [nJ].hwnd) ;
										BOARD_CheckFlip(nJ) ;
										BOARD_DrawRepaint(nJ, Game [nJ].hwnd, hdc) ;
										ReleaseDC(Game [nJ].hwnd, hdc) ;
									}
									break ;
								}
							}
						}
					}
					break ;
				}
			}
			return 0 ;

		case WM_SIZE :
			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					if(IsIconic(hwnd))
					{
						Game [nI].nState = WIN_MINIMIZE ;
					}
					else
					{
						if(IsZoomed(hwnd))
						{
							Game [nI].nState = WIN_MAXIMIZE ;
						}
						else
						{
							Game [nI].nState = WIN_NORMAL ;
						}

						hdc = GetDC(hwnd) ;

						switch(User.nBufferOrientation)
						{
							case DEFAULT_BUFFER_LEFT :
								BOARD_ResizeLeft(nI, hdc, LOWORD(lParam), HIWORD(lParam)) ;
								break ;

							case DEFAULT_BUFFER_RIGHT :
								BOARD_ResizeRight(nI, hdc, LOWORD(lParam), HIWORD(lParam)) ;
								break ;

							case DEFAULT_BUFFER_TOPBOTTOML :
								BOARD_ResizeTopBottomL(nI, hdc, LOWORD(lParam), HIWORD(lParam)) ;
								break ;

							case DEFAULT_BUFFER_TOPBOTTOMR :
								BOARD_ResizeTopBottomR(nI, hdc, LOWORD(lParam), HIWORD(lParam)) ;
								break ;

							default :
								BOARD_ResizeRight(nI, hdc, LOWORD(lParam), HIWORD(lParam)) ;
								break ;
						}

						BOARD_LoadBitmaps(nI, hdc) ;
						ReleaseDC(hwnd, hdc) ;

						if(User.bActualSize)
						{
							if(Game [nI].nState == WIN_NORMAL)
							{
								if(Game [nI].bFirstResize)
								{
									Game [nI].bFirstResize = 0 ;

									TOOLBOX_SaveWindowCoord(hwnd, nI) ;

									MoveWindow(hwnd,
											   wCoord  [nI].x,
											   wCoord  [nI].y,
											   (wCoord [nI].w - LOWORD(lParam)) + Game [nI].rActual.right,
											   (wCoord [nI].h - HIWORD(lParam)) + Game [nI].rActual.bottom,
											   TRUE) ;

									Game [nI].bFirstResize = 1 ;
								}
							}
						}
					}
					TOOLBOX_DisplayBitmapSize(nI) ;
					break ;
				}
			}
			break ;

		case WM_GETMINMAXINFO :
			mmi = (MINMAXINFO *) lParam ;
			mmi->ptMinTrackSize.x = 200 ;
			mmi->ptMinTrackSize.y = 200 ;
			break ;

		case WM_DESTROY :
			nJ = -1 ;
			nK = 0 ;
			for(nI = 1 ; nI < MAX_GAME ; nI++)
			{
				if(hwnd == Game [nI].hwnd)
				{
					nJ = nI ;

					(void) CLOCK_StopClockTimer(nI) ;

					if(User.bLogGame)
					{
						if(User.bSaveUnobserveGame)
						{
							if(Game [nI].nGameNumber > 0)
							{
								if(! Game [nI].bSavedGame)
								{
									SAVEGAME_SaveGame(nI, 0, 1) ;
								}
							}
						}
					}

					Game [nI].bValid = 0 ;

					TOOLBOX_SaveWindowCoord(hwnd, nI) ;

					if(Game [nI].hWindow)
					{
						DeleteObject(Game [nI].hWindow) ;
						Game [nI].hWindow = NULL ;
					}

					if(Game [nI].nGameNumber > 0)
					{
						System.bUnobserve      = 1 ;
						System.nUnobGameNumber = Game [nI].nGameNumber ;
						strcpy(System.cUnobWname, Game [nI].cHandle [INDEX_WHITE]) ;
						strcpy(System.cUnobBname, Game [nI].cHandle [INDEX_BLACK]) ;
						CLOCK_GetTimeMark(&System.tUnobserve) ;

						if(Game [nI].nRelation != -3)
						{
							sprintf(cTmp, "%s %d\n", ICS_UNOBSERVE_COMMAND, Game [nI].nGameNumber) ;
							TOOLBOX_WriteICS(cTmp) ;
						}
					}

					nK = Game [nI].nGamePartner ;

					Game [nI].bFirstGame   = 0 ;
					Game [nI].nGameNumber  = 0 ;
					Game [nI].nGamePartner = 0 ;
					Game [nI].bPlaying     = 0 ;
					Game [nI].bTickClock   = 1 ;
					break ;
				}
			}

			BOARD_Destroy(nJ) ;

			DeleteObject((HBRUSH) SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush(WHITE_BRUSH))) ;

			if(nJ != -1)
			{
				DeleteObject(Game [nJ].hbBackground) ;

				if(nK != 0)
				{
					if(nK != Game [INDEX_PLAY].nGameNumber)
					{
						for(nI = 1 ; nI < MAX_GAME ; nI++)
						{
							if(Game [nI].nGameNumber == nK)
							{
								if(IsWindow(Game [nI].hwnd))
								{
									PostMessage(Game [nI].hwnd, WM_CLOSE, (WPARAM) 0, (LPARAM) 0) ;
									break ;
								}
							}
						}
					}
				}
			}
			return 0 ;

		case WM_COMMAND :
			switch(wParam)
			{
				case WM_USER_RIGHT_MOUSE0 :
				case WM_USER_RIGHT_MOUSE1 :
				case WM_USER_RIGHT_MOUSE2 :
				case WM_USER_RIGHT_MOUSE3 :
				case WM_USER_RIGHT_MOUSE4 :
				case WM_USER_RIGHT_MOUSE5 :
				case WM_USER_RIGHT_MOUSE6 :
				case WM_USER_RIGHT_MOUSE7 :
				case WM_USER_RIGHT_MOUSE8 :
				case WM_USER_RIGHT_MOUSE9 :
				case WM_USER_RIGHT_MOUSE10 :
				case WM_USER_RIGHT_MOUSE11 :
				case WM_USER_RIGHT_MOUSE12 :
				case WM_USER_RIGHT_MOUSE13 :
				case WM_USER_RIGHT_MOUSE14 :
				case WM_USER_RIGHT_MOUSE15 :
				case WM_USER_RIGHT_MOUSE16 :
				case WM_USER_RIGHT_MOUSE17 :
				case WM_USER_RIGHT_MOUSE18 :
				case WM_USER_RIGHT_MOUSE19 :
				case WM_USER_RIGHT_MOUSE20 :
				case WM_USER_RIGHT_MOUSE21 :
				case WM_USER_RIGHT_MOUSE22 :
				case WM_USER_RIGHT_MOUSE23 :
				case WM_USER_RIGHT_MOUSE24 :
				case WM_USER_RIGHT_MOUSE25 :
				case WM_USER_RIGHT_MOUSE26 :
				case WM_USER_RIGHT_MOUSE27 :
				case WM_USER_RIGHT_MOUSE28 :
				case WM_USER_RIGHT_MOUSE29 :
					RIGHTMOUSE_Command(System.nRightMouseIndex, (LOWORD(wParam) - WM_USER_RIGHT_MOUSE0), User.bObserveCommandAddHist) ;
					break ;

				case IDM_PAWN :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? BLACK_PAWN : WHITE_PAWN))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_KNIGHT :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? BLACK_KNIGHT : WHITE_KNIGHT))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_BISHOP :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? BLACK_BISHOP : WHITE_BISHOP))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_ROOK :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? BLACK_ROOK : WHITE_ROOK))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_QUEEN :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? BLACK_QUEEN : WHITE_QUEEN))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_POPP_PAWN :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_PAWN : BLACK_PAWN))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_POPP_KNIGHT :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_KNIGHT : BLACK_KNIGHT))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_POPP_BISHOP :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_BISHOP : BLACK_BISHOP))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_POPP_ROOK :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_ROOK : BLACK_ROOK))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				case IDM_POPP_QUEEN :
					if((RightMouseDrop.nI >= 1) && (RightMouseDrop.nI < MAX_GAME))
					{
						if((Game [INDEX_PLAY].nGameNumber > 0)        &&
								(Game [INDEX_PLAY].bPlaying)               &&
								(Game [RightMouseDrop.nI].nGameNumber > 0) &&
								(Game [RightMouseDrop.nI].bPlaying)        &&
								(Game [INDEX_PLAY].nGamePartner == Game [RightMouseDrop.nI].nGameNumber))
						{
							if(! BOARD_RightMouseDropPiece1(RightMouseDrop.nI, RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_QUEEN : BLACK_QUEEN))
							{
								TOOLBOX_Beep() ;
							}
						}
						else
						{
							TOOLBOX_Beep() ;
						}
					}
					else
					{
						TOOLBOX_Beep() ;
					}
					return 0 ;

				default :
					if(HIWORD(wParam) == BN_CLICKED)
					{
						OBSERVE_Command(hwnd, (int) LOWORD(wParam)) ;
					}
					break ;
			}
			return 0 ;

		default :
			if(User.bMouseWheel)
			{
				if(iMsg == WM_MOUSEWHEEL)
				{
					PostMessage(hwndWindow [HWND_TELNET_TEXT], iMsg, wParam, lParam) ;
					return 0 ;
				}
			}
			break ;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam) ;
}

void OBSERVE_Command(HWND hwnd, int nB)
{
	HDC hdc ;
	int nI ;

	if((nB < 0) || (nB > 3))
	{
		SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
		System.hwndLastGame = hwnd ;
		return ;
	}

	if(Timeseal.bSocketIsOpen)
	{
		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			if(hwnd == Game [nI].hwnd)
			{
				if(Game [nI].bValid)
				{
					// make sure it's ok to press move buttons for this game
					if(TOOLBOX_OKMoveButton(nI))
					{
						if(Game [nI].nMaxIndex == -2)
						{
							Game [nI].bClickedButton = 0 ;
						}
						else
						{
							Game [nI].bClickedButton = 1 ;

							switch(nB)
							{
								case 0 :
									Game [nI].nCurrentIndex = -1 ;
									Game [nI].nCurrentColor = -1 ;
									break ;

								case 1 :
									if((Game [nI].nMinIndex     == -1) ||
											(Game [nI].nMaxIndex     == -1) ||
											(Game [nI].nCurrentIndex == -1) ||
											(Game [nI].nCurrentIndex <  Game [nI].nMinIndex))
									{
										Game [nI].nCurrentIndex = -1 ;
										Game [nI].nCurrentColor = -1 ;
									}
									else if(Game [nI].nCurrentIndex == Game [nI].nMinIndex)
									{
										if(Game [nI].nCurrentColor <= Game [nI].nMinColor)
										{
											Game [nI].nCurrentIndex = -1 ;
											Game [nI].nCurrentColor = -1 ;
										}
										else
										{
											Game [nI].nCurrentColor = 0 ;
										}
									}
									else if(Game [nI].nCurrentColor == 0)
									{
										Game [nI].nCurrentIndex = Game [nI].nCurrentIndex - 1 ;
										Game [nI].nCurrentColor = 1 ;
									}
									else
									{
										Game [nI].nCurrentColor = 0 ;
									}
									break ;

								case 2 :
									if((Game [nI].nMaxIndex     == -1) ||
											(Game [nI].nCurrentIndex >  Game [nI].nMaxIndex))
									{
										Game [nI].nCurrentIndex = Game [nI].nMaxIndex ;
										Game [nI].nCurrentColor = Game [nI].nMaxColor ;
									}
									else if(Game [nI].nCurrentIndex == Game [nI].nMaxIndex)
									{
										if(Game [nI].nCurrentColor >= Game [nI].nMaxColor)
										{
											Game [nI].nCurrentIndex = Game [nI].nMaxIndex ;
											Game [nI].nCurrentColor = Game [nI].nMaxColor ;
										}
										else
										{
											Game [nI].nCurrentColor = 1 ;
										}
									}
									else if(Game [nI].nCurrentIndex == -1)
									{
										Game [nI].nCurrentIndex = Game [nI].nMinIndex ;
										Game [nI].nCurrentColor = Game [nI].nMinColor ;
									}
									else if(Game [nI].nCurrentColor == 0)
									{
										Game [nI].nCurrentColor = 1 ;
									}
									else
									{
										Game [nI].nCurrentIndex = Game [nI].nCurrentIndex + 1 ;
										Game [nI].nCurrentColor = 0 ;
									}
									break ;

								case 3 :
									Game [nI].nCurrentIndex = Game [nI].nMaxIndex ;
									Game [nI].nCurrentColor = Game [nI].nMaxColor ;
									break ;
							}

							if((Game [nI].nCurrentIndex == Game [nI].nMaxIndex) &&
									(Game [nI].nCurrentColor == Game [nI].nMaxColor))
							{
								Game [nI].bClickedButton = 0 ;

								hdc = GetDC(Game [nI].hwnd) ;
								BOARD_DrawLastMove(nI, hdc) ;
								BOARD_DrawBoard1(nI, Game [nI].hwnd, hdc, DRAW_STATE_BOARD) ;
								ReleaseDC(Game [nI].hwnd, hdc) ;
							}
							else if(Game [nI].nCurrentIndex < MAX_POSITION)
							{
								hdc = GetDC(Game [nI].hwnd) ;
								BOARD_DrawLastMove1(nI, hdc) ;
								BOARD_DrawMoveButtonBoard1(nI, Game [nI].nCurrentIndex, Game [nI].nCurrentColor, Game [nI].hwnd, hdc) ;
								ReleaseDC(Game [nI].hwnd, hdc) ;
							}
						}
					}
				}
				break ;
			}
		}
	}

	SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
	System.hwndLastGame = hwnd ;
}
