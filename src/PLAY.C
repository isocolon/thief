#include "thief.h"

LRESULT CALLBACK PlayWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC             hdc;
	PAINTSTRUCT     ps;
	HMENU           hMenu, hSubMenu;
	POINT           pt;
	WINDOWPLACEMENT wp;
	MINMAXINFO      *mmi;
	int             nI, nJ, nK, nMx, nMy, nX, nY, bRefresh;
	char            cTmp [THIEF_COMMAND_SIZE + 10];

	switch(iMsg)
	{
		case WM_TIMER :
			switch(wParam)
			{
				case TIMER_CLOCK_ID :
					if(Timeseal.bSocketIsOpen)
					{
						KillTimer(hwnd, Game [INDEX_PLAY].nClockTimerEvent);
						Game [INDEX_PLAY].nClockTimerEvent = 0;
						CLOCK_DecrementClocks(INDEX_PLAY, hwnd);

						if(User.bShowLagStat && Game [INDEX_PLAY].bOnLagClock)
						{
							hdc = GetDC(hwnd);
							if(Game [INDEX_PLAY].bIPlayWhite)
							{
								BOARD_DrawWhiteLag(INDEX_PLAY, hdc);
							}
							else
							{
								BOARD_DrawBlackLag(INDEX_PLAY, hdc);
							}
							ReleaseDC(hwnd, hdc);
						}
					}
					break;
			}
			break;

		case WM_KEYDOWN :
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam);
			return 0;

		case WM_CHAR :
			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam);
			return 0;

		case WM_PAINT :
			hdc = BeginPaint(hwnd, &ps);
			BOARD_CheckFlip(INDEX_PLAY);
			BOARD_DrawRepaint(INDEX_PLAY, hwnd, hdc);
			EndPaint(hwnd, &ps);
			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			return 0;

		case WM_LBUTTONDOWN :
			// reset left mouse drop
			LeftMouseDrop.nI = -1;
			LeftMouseDrop.nX = -1;
			LeftMouseDrop.nY = -1;

			System.hwndLastGame = hwnd;
			ReleaseCapture();

			if(User.nMoveType == CLICK_MOVE)
			{
				if(DragInfo.nClicked != 0)
				{
					DragInfo.nClicked = 2;
					return 0;
				}
			}

			nMx = LOWORD(lParam);
			nMy = HIWORD(lParam);

			if(nMx >= Game [INDEX_PLAY].rBoard.left  &&
					nMx <= Game [INDEX_PLAY].rBoard.right &&
					nMy >= Game [INDEX_PLAY].rBoard.top   &&
					nMy <= Game [INDEX_PLAY].rBoard.bottom)
			{
				nX = (nMx - Game [INDEX_PLAY].rBoard.left) / Game [INDEX_PLAY].nss;
				nY = (nMy - Game [INDEX_PLAY].rBoard.top)  / Game [INDEX_PLAY].nss;

				if(nX < 0 || nX > 7 || nY < 0 || nY > 7)
				{
					return 0;
				}

				if(! TOOLBOX_DisplayActualBoard(INDEX_PLAY))
				{
					hdc = GetDC(hwnd);
					BOARD_RestoreDragMove(INDEX_PLAY, hdc);
					ReleaseDC(hwnd, hdc);
				}

				hdc = GetDC(hwnd);
				SetCapture(hwnd);
				if(BOARD_OnMouseDown(INDEX_PLAY, hwnd, hdc, 1, nMx, nMy, nX, nY))
				{
					ReleaseCapture();
				}
				ReleaseDC(hwnd, hdc);
			}
			else if((nMx >= Game [INDEX_PLAY].rBuffer.left   &&
					 nMx <= Game [INDEX_PLAY].rBuffer.right  &&
					 nMy >= Game [INDEX_PLAY].rBuffer.top    &&
					 nMy <= Game [INDEX_PLAY].rBuffer.bottom) ||
					(nMx >= Game [INDEX_PLAY].rBuffer1.left  &&
					 nMx <= Game [INDEX_PLAY].rBuffer1.right &&
					 nMy >= Game [INDEX_PLAY].rBuffer1.top   &&
					 nMy <= Game [INDEX_PLAY].rBuffer1.bottom))
			{
				if(! TOOLBOX_DisplayActualBoard(INDEX_PLAY))
				{
					hdc = GetDC(hwnd);
					BOARD_RestoreDragMove(INDEX_PLAY, hdc);
					ReleaseDC(hwnd, hdc);
				}

				if((Game [INDEX_PLAY].nGameType == GAMETYPE_BUGHOUSE) ||
						(Game [INDEX_PLAY].nGameType == GAMETYPE_CRAZYHOUSE))
				{
					hdc = GetDC(hwnd);
					SetCapture(hwnd);
					if(BOARD_OnMouseDown(INDEX_PLAY, hwnd, hdc, 0, nMx, nMy, 0, 0))
					{
						ReleaseCapture();
					}
					ReleaseDC(hwnd, hdc);
				}
				else
				{
					DragInfo.nIndex   = -1;
					DragInfo.nPc      = EMPTY_SQUARE;
					DragInfo.ptFrom.x = -1;
					DragInfo.ptFrom.y = -1;
					DragInfo.nClicked = 0;
				}
			}
			else
			{
				DragInfo.nIndex   = -1;
				DragInfo.nPc      = EMPTY_SQUARE;
				DragInfo.ptFrom.x = -1;
				DragInfo.ptFrom.y = -1;
				DragInfo.nClicked = 0;
			}
			return 0;

		case WM_MOUSEMOVE :
			if((User.nMoveType != DRAG_MOVE) || DragInfo.nPc == EMPTY_SQUARE)
			{
				ReleaseCapture();
			}
			else
			{
				hdc = GetDC(hwnd);
				BOARD_OnMouseMove(INDEX_PLAY, hwnd, hdc, LOWORD(lParam), HIWORD(lParam));
				ReleaseDC(hwnd, hdc);
			}
			return 0;

		case WM_USER_MOUSELEAVE :
			ReleaseCapture();

			hdc = GetDC(hwnd);
			BOARD_RestoreDragMove(INDEX_PLAY, hdc);
			ReleaseDC(hwnd, hdc);
			return 0;

		case WM_LBUTTONUP :
			// reset left mouse drop
			ReleaseCapture();

			LeftMouseDrop.nI = -1;
			LeftMouseDrop.nX = -1;
			LeftMouseDrop.nY = -1;

			if(DragInfo.nPc == EMPTY_SQUARE)
			{
				// if no smart move on LButton Up, return on empty square
				return 0;
			}

			if(User.nMoveType == CLICK_MOVE)
			{
				if(DragInfo.nClicked != 2)
				{
					return 0;
				}
			}

			hdc = GetDC(hwnd);

			if(User.bAutoCenterDragPiece)
			{
				nMx = LOWORD(lParam);
				nMy = HIWORD(lParam);
			}
			else
			{
				nMx = (LOWORD(lParam) - DragInfo.ptDist.x) + Game [INDEX_PLAY].nhss;
				nMy = (HIWORD(lParam) - DragInfo.ptDist.y) + Game [INDEX_PLAY].nhss;
			}

			if(nMx >= Game [INDEX_PLAY].rBoard.left  &&
					nMx <= Game [INDEX_PLAY].rBoard.right &&
					nMy >= Game [INDEX_PLAY].rBoard.top   &&
					nMy <= Game [INDEX_PLAY].rBoard.bottom)
			{
				if(! BOARD_OnMouseUp(INDEX_PLAY, hwnd, hdc,
									 (nMx - Game [INDEX_PLAY].rBoard.left) / Game [INDEX_PLAY].nss,
									 (nMy - Game [INDEX_PLAY].rBoard.top)  / Game [INDEX_PLAY].nss,
									 &bRefresh))
				{
					BOARD_RestoreDragMove(INDEX_PLAY, hdc);
					if(bRefresh)
					{
						BOARD_ResetTruePremove();
						F8KEY_Init();
						TOOLBOX_ResetPromotKnight();
					}
				}
			}
			else
			{
				BOARD_RestoreDragMove(INDEX_PLAY, hdc);
				BOARD_ResetTruePremove();
				F8KEY_Init();
				TOOLBOX_ResetPromotKnight();
			}
			ReleaseDC(hwnd, hdc);
			return 0;

		case WM_RBUTTONDOWN :
			// reset left mouse drop
			LeftMouseDrop.nI = -1;
			LeftMouseDrop.nX = -1;
			LeftMouseDrop.nY = -1;

			System.hwndLastGame = hwnd;
			ReleaseCapture();

			if(DragInfo.nPc != EMPTY_SQUARE)
			{
				hdc = GetDC(hwnd);
				BOARD_RestoreDragMove(INDEX_PLAY, hdc);
				ReleaseDC(hwnd, hdc);
			}

			nMx = LOWORD(lParam);
			nMy = HIWORD(lParam);

			pt.x = nMx;
			pt.y = nMy;

			if(PtInRect(&Game [INDEX_PLAY].rBoard,   pt) ||
					PtInRect(&Game [INDEX_PLAY].rBuffer,  pt) ||
					PtInRect(&Game [INDEX_PLAY].rBuffer1, pt))
			{
				if(! TOOLBOX_DisplayActualBoard(INDEX_PLAY))
				{
					hdc = GetDC(hwnd);
					BOARD_RestoreDragMove(INDEX_PLAY, hdc);
					ReleaseDC(hwnd, hdc);
				}
			}

			if(PtInRect(&Game [INDEX_PLAY].rHandle [INDEX_WHITE], pt))
			{
				nJ = INDEX_WHITE;
			}
			else if(PtInRect(&Game [INDEX_PLAY].rHandle [INDEX_BLACK], pt))
			{
				nJ = INDEX_BLACK;
			}
			else
			{
				nJ = -1;
			}

			if((nJ == INDEX_WHITE) || (nJ == INDEX_BLACK))
			{
				strcpy(Vars.cBoardHandle, Game [INDEX_PLAY].cHandle [nJ]);

				nJ = 0;
				for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
				{
					if(strlen(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI]) == 0)
					{
						break;
					}

					if(TOOLBOX_IsSeparator(RIGHT_MOUSE_BOARD_HANDLE, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_BOARD_HANDLE;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU");
							hSubMenu = GetSubMenu(hMenu, 0);

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION);
						}

						AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);

						nJ = nJ + 1;
					}
					else if(TOOLBOX_OKRightMouse(RIGHT_MOUSE_BOARD_HANDLE, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_BOARD_HANDLE;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU");
							hSubMenu = GetSubMenu(hMenu, 0);

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION);
						}

						if(strchr(RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI], '%'))
						{
							TOOLBOX_ExpandAllMacro(cTmp, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI], 0);
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), cTmp);
						}
						else
						{
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI]);
						}

						if(FCheck [RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [nI]])
						{
							CheckMenuItem(hSubMenu, (WM_USER_RIGHT_MOUSE0 + nI), MF_CHECKED);
						}

						nJ = nJ + 1;
					}
				}
				if(nJ > 0)
				{
					pt.x = nMx;
					pt.y = nMy;
					TOOLBOX_MenuPopup(hwnd, pt, hMenu, -1);
				}
				return 0;
			}

			if(Game [INDEX_PLAY].nGameNumber > 0)
			{
				if((Game [INDEX_PLAY].nGameType == GAMETYPE_BUGHOUSE) ||
						(Game [INDEX_PLAY].nGameType == GAMETYPE_CRAZYHOUSE))
				{
					//
					// only activate drop piece menu when it's crazyhouse or bughouse
					//
					if(Game [INDEX_PLAY].bPlaying)
					{
						// playing
						if(User.bRightMousePlay && (Game [INDEX_PLAY].nGameType == GAMETYPE_BUGHOUSE))
						{
							//
							// right mouse click to drop (with sit for piece drop move)
							//
							if(nMx >= Game [INDEX_PLAY].rBoard.left  &&
									nMx <= Game [INDEX_PLAY].rBoard.right &&
									nMy >= Game [INDEX_PLAY].rBoard.top   &&
									nMy <= Game [INDEX_PLAY].rBoard.bottom)
							{
								nX = (nMx - Game [INDEX_PLAY].rBoard.left) / Game [INDEX_PLAY].nss;
								nY = (nMy - Game [INDEX_PLAY].rBoard.top)  / Game [INDEX_PLAY].nss;

								if(nX < 0 || nX > 7 || nY < 0 || nY > 7)
								{
									nK = 0;
								}
								else
								{
									nK = 1;

									if(Game [INDEX_PLAY].bFlip)
									{
										nX = ReverseCoord [nX];
									}
									else
									{
										nY = ReverseCoord [nY];
									}

									RightMouseDrop.nI = INDEX_PLAY;
									RightMouseDrop.nX = nX;
									RightMouseDrop.nY = nY;

									hMenu = LoadMenu(hInst, "DROPPIECEMENU");

									if(Game [INDEX_PLAY].bIPlayWhite)
									{
										char cTmp [30];

										// pawn
										sprintf(cTmp, "Pawn  %d", Game [INDEX_PLAY].nBuffer [WHITE_PAWN]);
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | MF_ENABLED,   IDM_PAWN,   cTmp);

										// knight
										sprintf(cTmp, "Knight  %d", Game [INDEX_PLAY].nBuffer [WHITE_KNIGHT]);
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_KNIGHT, cTmp);

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [INDEX_PLAY].nBuffer [WHITE_BISHOP]);
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_BISHOP, cTmp);

										// rook
										sprintf(cTmp, "Rook  %d", Game [INDEX_PLAY].nBuffer [WHITE_ROOK]);
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | MF_ENABLED,   IDM_ROOK,   cTmp);

										// queen
										sprintf(cTmp, "Queen  %d", Game [INDEX_PLAY].nBuffer [WHITE_QUEEN]);
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | MF_ENABLED,  IDM_QUEEN,  cTmp);
									}
									else
									{
										char cTmp [30];

										// pawn
										sprintf(cTmp, "Pawn  %d", Game [INDEX_PLAY].nBuffer [BLACK_PAWN]);
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | MF_ENABLED,   IDM_PAWN,   cTmp);

										// knight
										sprintf(cTmp, "Knight  %d", Game [INDEX_PLAY].nBuffer [BLACK_KNIGHT]);
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_KNIGHT, cTmp);

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [INDEX_PLAY].nBuffer [BLACK_BISHOP]);
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDM_BISHOP, cTmp);

										// rook
										sprintf(cTmp, "Rook  %d", Game [INDEX_PLAY].nBuffer [BLACK_ROOK]);
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | MF_ENABLED,   IDM_ROOK,   cTmp);

										// queen
										sprintf(cTmp, "Queen  %d", Game [INDEX_PLAY].nBuffer [BLACK_QUEEN]);
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | MF_ENABLED,  IDM_QUEEN,  cTmp);
									}
								}
							}
							else
							{
								nK = 0;
							}
						}
						else if(User.bRightMousePlay && (Game [INDEX_PLAY].nGameType == GAMETYPE_CRAZYHOUSE))
						{
							//
							// right mouse click to drop (no sit for piece drop move)
							//
							if(nMx >= Game [INDEX_PLAY].rBoard.left  &&
									nMx <= Game [INDEX_PLAY].rBoard.right &&
									nMy >= Game [INDEX_PLAY].rBoard.top   &&
									nMy <= Game [INDEX_PLAY].rBoard.bottom)
							{
								nX = (nMx - Game [INDEX_PLAY].rBoard.left) / Game [INDEX_PLAY].nss;
								nY = (nMy - Game [INDEX_PLAY].rBoard.top)  / Game [INDEX_PLAY].nss;

								if(nX < 0 || nX > 7 || nY < 0 || nY > 7)
								{
									nK = 0;
								}
								else
								{
									nK = 1;

									if(Game [INDEX_PLAY].bFlip)
									{
										nX = ReverseCoord [nX];
									}
									else
									{
										nY = ReverseCoord [nY];
									}

									RightMouseDrop.nI = INDEX_PLAY;
									RightMouseDrop.nX = nX;
									RightMouseDrop.nY = nY;

									hMenu = LoadMenu(hInst, "DROPPIECEMENU");

									if(Game [INDEX_PLAY].bIPlayWhite)
									{
										char cTmp [30];

										// pawn
										sprintf(cTmp, "Pawn  %d", Game [INDEX_PLAY].nBuffer [WHITE_PAWN]);
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_PAWN] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_PAWN,   cTmp);

										// knight
										sprintf(cTmp, "Knight  %d", Game [INDEX_PLAY].nBuffer [WHITE_KNIGHT]);
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_KNIGHT] > 0 ? MF_ENABLED : MF_GRAYED), IDM_KNIGHT, cTmp);

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [INDEX_PLAY].nBuffer [WHITE_BISHOP]);
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_BISHOP] > 0 ? MF_ENABLED : MF_GRAYED), IDM_BISHOP, cTmp);

										// rook
										sprintf(cTmp, "Rook  %d", Game [INDEX_PLAY].nBuffer [WHITE_ROOK]);
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_ROOK] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_ROOK,   cTmp);

										// queen
										sprintf(cTmp, "Queen  %d", Game [INDEX_PLAY].nBuffer [WHITE_QUEEN]);
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_QUEEN] > 0 ? MF_ENABLED : MF_GRAYED),   IDM_QUEEN,  cTmp);
									}
									else
									{
										char cTmp [30];

										// pawn
										sprintf(cTmp, "Pawn  %d", Game [INDEX_PLAY].nBuffer [BLACK_PAWN]);
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_PAWN] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_PAWN,   cTmp);

										// knight
										sprintf(cTmp, "Knight  %d", Game [INDEX_PLAY].nBuffer [BLACK_KNIGHT]);
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_KNIGHT] > 0 ? MF_ENABLED : MF_GRAYED), IDM_KNIGHT, cTmp);

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [INDEX_PLAY].nBuffer [BLACK_BISHOP]);
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_BISHOP] > 0 ? MF_ENABLED : MF_GRAYED), IDM_BISHOP, cTmp);

										// rook
										sprintf(cTmp, "Rook  %d", Game [INDEX_PLAY].nBuffer [BLACK_ROOK]);
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_ROOK] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_ROOK,   cTmp);

										// queen
										sprintf(cTmp, "Queen  %d", Game [INDEX_PLAY].nBuffer [BLACK_QUEEN]);
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_QUEEN] > 0 ? MF_ENABLED : MF_GRAYED),   IDM_QUEEN,  cTmp);
									}
								}
							}
							else
							{
								nK = 0;
							}
						}
						else
						{
							nK = 0;
						}
					}
					else
					{
						// examing
						if(nMx >= Game [INDEX_PLAY].rBoard.left  &&
								nMx <= Game [INDEX_PLAY].rBoard.right &&
								nMy >= Game [INDEX_PLAY].rBoard.top   &&
								nMy <= Game [INDEX_PLAY].rBoard.bottom)
						{
							nX = (nMx - Game [INDEX_PLAY].rBoard.left) / Game [INDEX_PLAY].nss;
							nY = (nMy - Game [INDEX_PLAY].rBoard.top)  / Game [INDEX_PLAY].nss;

							if(nX < 0 || nX > 7 || nY < 0 || nY > 7)
							{
								nK = 0;
							}
							else
							{
								nK = 1;

								if(Game [INDEX_PLAY].bFlip)
								{
									nX = ReverseCoord [nX];
								}
								else
								{
									nY = ReverseCoord [nY];
								}

								RightMouseDrop.nI = INDEX_PLAY;
								RightMouseDrop.nX = nX;
								RightMouseDrop.nY = nY;

								hMenu = LoadMenu(hInst, "DROPPIECEMENU");

								if(Game [INDEX_PLAY].nGameType == GAMETYPE_CRAZYHOUSE)
								{
									//
									// only crazyhouse needs to have exact pieces to drop
									//
									if(Game [INDEX_PLAY].bWhitesMove)
									{
										char cTmp [30];

										// pawn
										sprintf(cTmp, "Pawn  %d", Game [INDEX_PLAY].nBuffer [WHITE_PAWN]);
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_PAWN] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_PAWN,   cTmp);

										// knight
										sprintf(cTmp, "Knight  %d", Game [INDEX_PLAY].nBuffer [WHITE_KNIGHT]);
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_KNIGHT] > 0 ? MF_ENABLED : MF_GRAYED), IDM_KNIGHT, cTmp);

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [INDEX_PLAY].nBuffer [WHITE_BISHOP]);
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_BISHOP] > 0 ? MF_ENABLED : MF_GRAYED), IDM_BISHOP, cTmp);

										// rook
										sprintf(cTmp, "Rook  %d", Game [INDEX_PLAY].nBuffer [WHITE_ROOK]);
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_ROOK] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_ROOK,   cTmp);

										// queen
										sprintf(cTmp, "Queen  %d", Game [INDEX_PLAY].nBuffer [WHITE_QUEEN]);
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [WHITE_QUEEN] > 0 ? MF_ENABLED : MF_GRAYED),   IDM_QUEEN,  cTmp);
									}
									else
									{
										char cTmp [30];

										// pawn
										sprintf(cTmp, "Pawn  %d", Game [INDEX_PLAY].nBuffer [BLACK_PAWN]);
										ModifyMenu(hMenu, IDM_PAWN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_PAWN] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_PAWN,   cTmp);

										// knight
										sprintf(cTmp, "Knight  %d", Game [INDEX_PLAY].nBuffer [BLACK_KNIGHT]);
										ModifyMenu(hMenu, IDM_KNIGHT, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_KNIGHT] > 0 ? MF_ENABLED : MF_GRAYED), IDM_KNIGHT, cTmp);

										// bishop
										sprintf(cTmp, "Bishop  %d", Game [INDEX_PLAY].nBuffer [BLACK_BISHOP]);
										ModifyMenu(hMenu, IDM_BISHOP, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_BISHOP] > 0 ? MF_ENABLED : MF_GRAYED), IDM_BISHOP, cTmp);

										// rook
										sprintf(cTmp, "Rook  %d", Game [INDEX_PLAY].nBuffer [BLACK_ROOK]);
										ModifyMenu(hMenu, IDM_ROOK, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_ROOK] > 0 ? MF_ENABLED : MF_GRAYED),     IDM_ROOK,   cTmp);

										// queen
										sprintf(cTmp, "Queen  %d", Game [INDEX_PLAY].nBuffer [BLACK_QUEEN]);
										ModifyMenu(hMenu, IDM_QUEEN, MF_BYCOMMAND | MF_STRING | (Game [INDEX_PLAY].nBuffer [BLACK_QUEEN] > 0 ? MF_ENABLED : MF_GRAYED),   IDM_QUEEN,  cTmp);
									}
								}
							}
						}
						else
						{
							nK = 0;
						}
					}
				}
				else
				{
					nK = 0;
				}
			}
			else
			{
				// not playing nor examing
				nK = 0;
			}

			if(nK)
			{
				pt.x = nMx;
				pt.y = nMy;
				TOOLBOX_MenuPopup(hwnd, pt, hMenu, -1);
			}
			else
			{
				nJ = 0;
				for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
				{
					if(strlen(RightMouse [RIGHT_MOUSE_PLAY].cMenu [nI]) == 0)
					{
						break;
					}

					if(TOOLBOX_IsSeparator(RIGHT_MOUSE_PLAY, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_PLAY;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU");
							hSubMenu = GetSubMenu(hMenu, 0);

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION);
						}

						AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);

						nJ = nJ + 1;
					}
					else if(TOOLBOX_OKRightMouse(RIGHT_MOUSE_PLAY, nI))
					{
						if(nJ == 0)
						{
							System.nRightMouseIndex = RIGHT_MOUSE_PLAY;

							hMenu    = LoadMenu(hInst, "ONEITEMMENU");
							hSubMenu = GetSubMenu(hMenu, 0);

							DeleteMenu(hSubMenu, 0, MF_BYPOSITION);
						}

						if(strchr(RightMouse [RIGHT_MOUSE_PLAY].cMenu [nI], '%'))
						{
							TOOLBOX_ExpandAllMacro(cTmp, RightMouse [RIGHT_MOUSE_PLAY].cMenu [nI], 0);
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), cTmp);
						}
						else
						{
							AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), RightMouse [RIGHT_MOUSE_PLAY].cMenu [nI]);
						}

						if(FCheck [RightMouse [RIGHT_MOUSE_PLAY].nType [nI]])
						{
							CheckMenuItem(hSubMenu, (WM_USER_RIGHT_MOUSE0 + nI), MF_CHECKED);
						}

						nJ = nJ + 1;
					}
				}
				if(nJ > 0)
				{
					pt.x = nMx;
					pt.y = nMy;
					TOOLBOX_MenuPopup(hwnd, pt, hMenu, -1);
				}
			}
			return 0;

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
				case WM_USER_RIGHT_MOUSE30 :
				case WM_USER_RIGHT_MOUSE31 :
				case WM_USER_RIGHT_MOUSE32 :
				case WM_USER_RIGHT_MOUSE33 :
				case WM_USER_RIGHT_MOUSE34 :
				case WM_USER_RIGHT_MOUSE35 :
				case WM_USER_RIGHT_MOUSE36 :
				case WM_USER_RIGHT_MOUSE37 :
				case WM_USER_RIGHT_MOUSE38 :
				case WM_USER_RIGHT_MOUSE39 :
				case WM_USER_RIGHT_MOUSE40 :
				case WM_USER_RIGHT_MOUSE41 :
				case WM_USER_RIGHT_MOUSE42 :
				case WM_USER_RIGHT_MOUSE43 :
				case WM_USER_RIGHT_MOUSE44 :
				case WM_USER_RIGHT_MOUSE45 :
				case WM_USER_RIGHT_MOUSE46 :
				case WM_USER_RIGHT_MOUSE47 :
				case WM_USER_RIGHT_MOUSE48 :
				case WM_USER_RIGHT_MOUSE49 :
				case WM_USER_RIGHT_MOUSE50 :
					RIGHTMOUSE_Command(System.nRightMouseIndex, (LOWORD(wParam) - WM_USER_RIGHT_MOUSE0), User.bPlayCommandAddHist);
					break;

				case IDM_PAWN :
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_PAWN : BLACK_PAWN))
							{
								TOOLBOX_Beep();
							}
						}
						else
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bWhitesMove ? WHITE_PAWN : BLACK_PAWN))
							{
								TOOLBOX_Beep();
							}
						}
					}
					else
					{
						TOOLBOX_Beep();
					}
					return 0;

				case IDM_KNIGHT :
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_KNIGHT : BLACK_KNIGHT))
							{
								TOOLBOX_Beep();
							}
						}
						else
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bWhitesMove ? WHITE_KNIGHT : BLACK_KNIGHT))
							{
								TOOLBOX_Beep();
							}
						}
					}
					else
					{
						TOOLBOX_Beep();
					}
					return 0;

				case IDM_BISHOP :
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_BISHOP : BLACK_BISHOP))
							{
								TOOLBOX_Beep();
							}
						}
						else
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bWhitesMove ? WHITE_BISHOP : BLACK_BISHOP))
							{
								TOOLBOX_Beep();
							}
						}
					}
					else
					{
						TOOLBOX_Beep();
					}
					return 0;

				case IDM_ROOK :
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_ROOK : BLACK_ROOK))
							{
								TOOLBOX_Beep();
							}
						}
						else
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bWhitesMove ? WHITE_ROOK : BLACK_ROOK))
							{
								TOOLBOX_Beep();
							}
						}
					}
					else
					{
						TOOLBOX_Beep();
					}
					return 0;

				case IDM_QUEEN :
					if(Game [INDEX_PLAY].nGameNumber > 0)
					{
						if(Game [INDEX_PLAY].bPlaying)
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bIPlayWhite ? WHITE_QUEEN : BLACK_QUEEN))
							{
								TOOLBOX_Beep();
							}
						}
						else
						{
							if(! BOARD_RightMouseDropPiece(RightMouseDrop.nX, RightMouseDrop.nY, Game [INDEX_PLAY].bWhitesMove ? WHITE_QUEEN : BLACK_QUEEN))
							{
								TOOLBOX_Beep();
							}
						}
					}
					else
					{
						TOOLBOX_Beep();
					}
					return 0;

				default :
					if(HIWORD(wParam) == BN_CLICKED)
					{
						PLAY_Command((int) LOWORD(wParam));
					}
					break;
			}
			return 0;

		case WM_MDIACTIVATE :
			System.hwndLastGame = hwnd;
			if(lParam == (LPARAM) hwnd)
			{
				if(wCoord [COORD_PLAY].s == WIN_SHOW)
				{
					ShowWindow(hwnd, SW_SHOW);
				}
				else
				{
					//ShowWindow (hwnd, SW_HIDE);
					ShowWindow(hwnd, SW_MINIMIZE);
				}
			}
			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			return 0;

		case WM_USER_RESET_BOARD :
			if(Game [INDEX_PLAY].nGameNumber > 0)
			{
				TOOLBOX_Beep();
			}
			else
			{
				if(Telnet.nTelnetState == WIN_MAXIMIZE)
				{
					ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
				}

				if(wCoord [COORD_PLAY].s    == WIN_HIDE ||
						Game [INDEX_PLAY].nState == WIN_MINIMIZE)
				{
					ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);
				}

				wCoord [COORD_PLAY].s = WIN_SHOW;
				SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);

				GAME_InitialSetup(INDEX_PLAY);

				Premove.nPremoveCount     = 0;
				Premove.nPremoveHead      = 0;
				Premove.nPremoveTail      = 0;
				Premove.bLastTP           = 0;
				Premove.ptLastTP [0].x    = -1;
				Premove.ptLastTP [0].y    = -1;
				Premove.ptLastTP [1].x    = -1;
				Premove.ptLastTP [1].y    = -1;
				Premove.bIllegalTP        = 0;
				Premove.ptIllegalTP [0].x = -1;
				Premove.ptIllegalTP [0].y = -1;
				Premove.ptIllegalTP [1].x = -1;
				Premove.ptIllegalTP [1].y = -1;

				hdc = GetDC(hwnd);
				BOARD_CheckFlip(INDEX_PLAY);
				BOARD_DrawRepaint(INDEX_PLAY, hwnd, hdc);
				ReleaseDC(hwnd, hdc);
				SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			}
			return 0;

		case WM_USER_FLIP_BOARD :
			if(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
			{
				TOOLBOX_Beep();
				return 0;
			}

			Game [INDEX_PLAY].bFlip = ! Game [INDEX_PLAY].bFlip;

			if(Telnet.nTelnetState == WIN_MAXIMIZE)
			{
				ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
			}

			if(wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
			{
				ShowWindow(Game [INDEX_PLAY].hwnd, SW_RESTORE);
			}

			wCoord [COORD_PLAY].s = WIN_SHOW;
			SendMessage(hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM)(HWND) hwndWindow [HWND_PLAY], 0);

			hdc = GetDC(hwnd);
			BOARD_CheckFlip(INDEX_PLAY);
			BOARD_DrawRepaint(INDEX_PLAY, hwnd, hdc);
			ReleaseDC(hwnd, hdc);
			return 0;

		case WM_SIZE :
			if(IsIconic(hwnd))
			{
				wCoord [COORD_PLAY].s    = WIN_HIDE;
				Game [INDEX_PLAY].nState = WIN_MINIMIZE;
			}
			else
			{
				wCoord [COORD_PLAY].s = WIN_SHOW;

				if(IsZoomed(hwnd))
				{
					Game [INDEX_PLAY].nState = WIN_MAXIMIZE;
				}
				else
				{
					Game [INDEX_PLAY].nState = WIN_NORMAL;
				}

				hdc = GetDC(hwnd);

				if(System.bFirstTime)
				{
					System.bFirstTime = 0;
					CSET_Load(hdc);
				}

				switch(User.nBufferOrientation)
				{
					case DEFAULT_BUFFER_LEFT :
						BOARD_ResizeLeft(INDEX_PLAY, hdc, LOWORD(lParam), HIWORD(lParam));
						break;

					case DEFAULT_BUFFER_RIGHT :
						BOARD_ResizeRight(INDEX_PLAY, hdc, LOWORD(lParam), HIWORD(lParam));
						break;

					case DEFAULT_BUFFER_TOPBOTTOML :
						BOARD_ResizeTopBottomL(INDEX_PLAY, hdc, LOWORD(lParam), HIWORD(lParam));
						break;

					case DEFAULT_BUFFER_TOPBOTTOMR :
						BOARD_ResizeTopBottomR(INDEX_PLAY, hdc, LOWORD(lParam), HIWORD(lParam));
						break;

					default :
						BOARD_ResizeRight(INDEX_PLAY, hdc, LOWORD(lParam), HIWORD(lParam));
						break;
				}

				BOARD_LoadBitmaps(INDEX_PLAY, hdc);
				ReleaseDC(hwnd, hdc);

				if(User.bActualSize)
				{
					if(Game [INDEX_PLAY].nState == WIN_NORMAL)
					{
						if(Game [INDEX_PLAY].bFirstResize)
						{
							Game [INDEX_PLAY].bFirstResize = 0;

							TOOLBOX_SaveWindowCoord(hwnd, COORD_PLAY);

							System.bOkToResizePlay = 1;

							MoveWindow(hwnd,
									   wCoord  [COORD_PLAY].x,
									   wCoord  [COORD_PLAY].y,
									   (wCoord [COORD_PLAY].w - LOWORD(lParam)) + Game [INDEX_PLAY].rActual.right,
									   (wCoord [COORD_PLAY].h - HIWORD(lParam)) + Game [INDEX_PLAY].rActual.bottom,
									   TRUE);

							System.bOkToResizePlay = 0;

							Game [INDEX_PLAY].bFirstResize = 1;
						}
					}
				}
			}
			TOOLBOX_DisplayBitmapSize(INDEX_PLAY);
			break;

		case WM_GETMINMAXINFO :
			if(User.bStopResizePlay)
			{
				if(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
				{
					if((Game [INDEX_PLAY].nState == WIN_MINIMIZE) || (IsIconic(hwnd)))
					{
						mmi = (MINMAXINFO *) lParam;
						mmi->ptMinTrackSize.x  = 200;
						mmi->ptMinTrackSize.y  = 200;
						System.bOkToResizePlay = 0;
					}
					else if((Game [INDEX_PLAY].nState == WIN_MAXIMIZE) || (IsZoomed(hwnd)))
					{
						mmi = (MINMAXINFO *) lParam;
						mmi->ptMinTrackSize.x  = 200;
						mmi->ptMinTrackSize.y  = 200;
						System.bOkToResizePlay = 0;
					}
					else
					{
						if(System.bOkToResizePlay)
						{
							mmi = (MINMAXINFO *) lParam;
							mmi->ptMinTrackSize.x  = 200;
							mmi->ptMinTrackSize.y  = 200;
							System.bOkToResizePlay = 0;
						}
						else
						{
							wp.length = sizeof(WINDOWPLACEMENT);
							GetWindowPlacement(hwnd, &wp);

							mmi = (MINMAXINFO *) lParam;
							mmi->ptMinTrackSize.x = wp.rcNormalPosition.right  - wp.rcNormalPosition.left;
							mmi->ptMinTrackSize.y = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
							mmi->ptMaxTrackSize.x = wp.rcNormalPosition.right  - wp.rcNormalPosition.left;
							mmi->ptMaxTrackSize.y = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
						}
					}
				}
				else
				{
					mmi = (MINMAXINFO *) lParam;
					mmi->ptMinTrackSize.x  = 200;
					mmi->ptMinTrackSize.y  = 200;
					System.bOkToResizePlay = 0;
				}
			}
			else
			{
				mmi = (MINMAXINFO *) lParam;
				mmi->ptMinTrackSize.x  = 200;
				mmi->ptMinTrackSize.y  = 200;
				System.bOkToResizePlay = 0;
			}
			break;

		case WM_CREATE :
			Game [INDEX_PLAY].hbBackground = CreateSolidBrush(clrColor [CLR_WINDOW_COLOR]);

			DeleteObject((HBRUSH) SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG) Game [INDEX_PLAY].hbBackground));

			Game [INDEX_PLAY].hGame = ((LPCREATESTRUCT) lParam)->hInstance;

			Game [INDEX_PLAY].hwnd1 = CreateWindow("BUTTON", "<<",
												   WS_CHILD | BS_PUSHBUTTON,
												   0, 0, 20, 20, hwnd,
												   (HMENU) 0, Game [INDEX_PLAY].hGame, NULL);

			Game [INDEX_PLAY].hwnd2 = CreateWindow("BUTTON", "<",
												   WS_CHILD | BS_PUSHBUTTON,
												   0, 0, 20, 20, hwnd,
												   (HMENU) 1, Game [INDEX_PLAY].hGame, NULL);

			Game [INDEX_PLAY].hwnd3 = CreateWindow("BUTTON", ">",
												   WS_CHILD | BS_PUSHBUTTON,
												   0, 0, 20, 20, hwnd,
												   (HMENU) 2, Game [INDEX_PLAY].hGame, NULL);

			Game [INDEX_PLAY].hwnd4 = CreateWindow("BUTTON", ">>",
												   WS_CHILD | BS_PUSHBUTTON,
												   0, 0, 20, 20, hwnd,
												   (HMENU) 3, Game [INDEX_PLAY].hGame, NULL);

			TOOLBOX_ShowMoveButtons(INDEX_PLAY, 0);

			POSITION_Init(INDEX_PLAY);

			DragInfo.nClicked = 0;
			return 0;

		case WM_CLOSE :
			if(Game [INDEX_PLAY].nGameNumber > 0)
			{
				if(Game [INDEX_PLAY].bPlaying)
				{
					TOOLBOX_Beep();
					return 0;
				}
				else
				{
					TOOLBOX_WriteICS(ICS_UNEXAM_COMMAND);
				}
			}

			wCoord [COORD_PLAY].s = WIN_HIDE;
			//ShowWindow (hwnd, SW_HIDE);
			ShowWindow(hwnd, SW_MINIMIZE);
			SendMessage(hwndWindow [HWND_CLIENT], WM_MDINEXT, (LPARAM) NULL, 0);
			return 0;

		case WM_DESTROY :
			(void) CLOCK_StopClockTimer(INDEX_PLAY);

			BOARD_Destroy(INDEX_PLAY);

			DeleteObject((HBRUSH) SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush(WHITE_BRUSH)));
			DeleteObject(Game [INDEX_PLAY].hbBackground);

			if(Game [INDEX_PLAY].hWindow)
			{
				DeleteObject(Game [INDEX_PLAY].hWindow);
				Game [INDEX_PLAY].hWindow = NULL;
			}
			return 0;

		default :
			if(User.bMouseWheel)
			{
				if(iMsg == WM_MOUSEWHEEL)
				{
					PostMessage(hwndWindow [HWND_TELNET_TEXT], iMsg, wParam, lParam);
					return 0;
				}
			}
			break;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}

void PLAY_Command(int nB)
{
	HDC hdc;
	char cTmp [255];

	if((nB < 0) || (nB > 3))
	{
		SetFocus(hwndWindow [HWND_TELNET_EDIT]);
		System.hwndLastGame = Game [INDEX_PLAY].hwnd;
		return;
	}

	if(Timeseal.bSocketIsOpen)
	{
		// examing a game
		if((Game [INDEX_PLAY].nGameNumber > 0) && (! Game [INDEX_PLAY].bPlaying))
		{
			switch(nB)
			{
				case 0 :
					strcpy(cTmp, ICS_EXAM_1_COMMAND);
					break;

				case 1 :
					strcpy(cTmp, ICS_EXAM_2_COMMAND);
					break;

				case 2 :
					strcpy(cTmp, ICS_EXAM_3_COMMAND);
					break;

				case 3 :
					strcpy(cTmp, ICS_EXAM_4_COMMAND);
					break;

				default :
					strcpy(cTmp, "");
					break;
			}

			if(strlen(cTmp) > 0)
			{
				TOOLBOX_WriteICS(cTmp);

				if(!(User.bShowResult || User.bShowPtell))
				{
					TOOLBOX_WriteUser(cTmp);
				}
			}

			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			System.hwndLastGame = Game [INDEX_PLAY].hwnd;
			return;
		}

		// make sure it's ok to press move buttons for this game
		if(! TOOLBOX_OKMoveButton(INDEX_PLAY))
		{
			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			System.hwndLastGame = Game [INDEX_PLAY].hwnd;
			return;
		}

		// playing or the game is done
		if(Game [INDEX_PLAY].nMaxIndex == -2)
		{
			Game [INDEX_PLAY].bClickedButton = 0;
		}
		else
		{
			Game [INDEX_PLAY].bClickedButton = 1;

			switch(nB)
			{
				case 0 :
					Game [INDEX_PLAY].nCurrentIndex = -1;
					Game [INDEX_PLAY].nCurrentColor = -1;
					break;

				case 1 :
					if((Game [INDEX_PLAY].nMinIndex     == -1) ||
							(Game [INDEX_PLAY].nMaxIndex     == -1) ||
							(Game [INDEX_PLAY].nCurrentIndex == -1) ||
							(Game [INDEX_PLAY].nCurrentIndex <  Game [INDEX_PLAY].nMinIndex))
					{
						Game [INDEX_PLAY].nCurrentIndex = -1;
						Game [INDEX_PLAY].nCurrentColor = -1;
					}
					else if(Game [INDEX_PLAY].nCurrentIndex == Game [INDEX_PLAY].nMinIndex)
					{
						if(Game [INDEX_PLAY].nCurrentColor <= Game [INDEX_PLAY].nMinColor)
						{
							Game [INDEX_PLAY].nCurrentIndex = -1;
							Game [INDEX_PLAY].nCurrentColor = -1;
						}
						else
						{
							Game [INDEX_PLAY].nCurrentColor = 0;
						}
					}
					else if(Game [INDEX_PLAY].nCurrentColor == 0)
					{
						Game [INDEX_PLAY].nCurrentIndex = Game [INDEX_PLAY].nCurrentIndex - 1;
						Game [INDEX_PLAY].nCurrentColor = 1;
					}
					else
					{
						Game [INDEX_PLAY].nCurrentColor = 0;
					}
					break;

				case 2 :
					if((Game [INDEX_PLAY].nMaxIndex     == -1) ||
							(Game [INDEX_PLAY].nCurrentIndex >  Game [INDEX_PLAY].nMaxIndex))
					{
						Game [INDEX_PLAY].nCurrentIndex = Game [INDEX_PLAY].nMaxIndex;
						Game [INDEX_PLAY].nCurrentColor = Game [INDEX_PLAY].nMaxColor;
					}
					else if(Game [INDEX_PLAY].nCurrentIndex == Game [INDEX_PLAY].nMaxIndex)
					{
						if(Game [INDEX_PLAY].nCurrentColor >= Game [INDEX_PLAY].nMaxColor)
						{
							Game [INDEX_PLAY].nCurrentIndex = Game [INDEX_PLAY].nMaxIndex;
							Game [INDEX_PLAY].nCurrentColor = Game [INDEX_PLAY].nMaxColor;
						}
						else
						{
							Game [INDEX_PLAY].nCurrentColor = 1;
						}
					}
					else if(Game [INDEX_PLAY].nCurrentIndex == -1)
					{
						Game [INDEX_PLAY].nCurrentIndex = Game [INDEX_PLAY].nMinIndex;
						Game [INDEX_PLAY].nCurrentColor = Game [INDEX_PLAY].nMinColor;
					}
					else if(Game [INDEX_PLAY].nCurrentColor == 0)
					{
						Game [INDEX_PLAY].nCurrentColor = 1;
					}
					else
					{
						Game [INDEX_PLAY].nCurrentIndex = Game [INDEX_PLAY].nCurrentIndex + 1;
						Game [INDEX_PLAY].nCurrentColor = 0;
					}
					break;

				case 3 :
					Game [INDEX_PLAY].nCurrentIndex = Game [INDEX_PLAY].nMaxIndex;
					Game [INDEX_PLAY].nCurrentColor = Game [INDEX_PLAY].nMaxColor;
					break;
			}

			if((Game [INDEX_PLAY].nCurrentIndex == Game [INDEX_PLAY].nMaxIndex) &&
					(Game [INDEX_PLAY].nCurrentColor == Game [INDEX_PLAY].nMaxColor))
			{
				Game [INDEX_PLAY].bClickedButton = 0;

				hdc = GetDC(Game [INDEX_PLAY].hwnd);
				BOARD_DrawLastMove(INDEX_PLAY, hdc);
				BOARD_DrawBoard(INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_BOARD);
				ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);
			}
			else if(Game [INDEX_PLAY].nCurrentIndex < MAX_POSITION)
			{
				hdc = GetDC(Game [INDEX_PLAY].hwnd);
				BOARD_DrawLastMove1(INDEX_PLAY, hdc);
				BOARD_DrawMoveButtonBoard(INDEX_PLAY, Game [INDEX_PLAY].nCurrentIndex, Game [INDEX_PLAY].nCurrentColor, Game [INDEX_PLAY].hwnd, hdc);
				ReleaseDC(Game [INDEX_PLAY].hwnd, hdc);
			}
		}
	}

	SetFocus(hwndWindow [HWND_TELNET_EDIT]);
	System.hwndLastGame = Game [INDEX_PLAY].hwnd;
}
