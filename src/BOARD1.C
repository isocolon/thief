#include "thief.h"

HRGN Clips [MAX_CLIPS] ;
int nClips ;

void BOARD_DrawBoard(int nG, HWND hwnd, HDC hdc, int nState)
{
	int bPlayer = 0, bDragPiece = 0, bNeverDrew = 0, bDrag = 0 ;

	int nY, nX, nI, nOT, nPx, nPy, nPc ;

	HDC hdcWindow, hdcSave ;
	HBITMAP htOldWindow, htOldSave ;
	RECT rc ;

	int nW, nH, nW1 ;

	if(nG != INDEX_PLAY)
	{
		BOARD_DrawBoard1(nG, hwnd, hdc, nState) ;
		return ;
	}

	// make sure the window is not minimized
	if(IsIconic(hwnd))
	{
		return ;
	}

	// determine game relation
	if(nG == INDEX_PLAY)
	{
		bPlayer = 1 ;

		if(DragInfo.nPc != EMPTY_SQUARE)
		{
			if(DragInfo.nIndex == nG)
			{
				bDragPiece = 1 ;
			}
		}

	}
	else
	{
		if(Game [nG].nGameNumber == 0)
		{
		}
		else
		{
			if(Game [nG].nGameNumber == Game [INDEX_PLAY].nGamePartner)
			{
				if(DragInfo.nPc != EMPTY_SQUARE)
				{
					if(DragInfo.nIndex == nG)
					{
						bDragPiece = 1 ;
					}
				}
			}
		}
	}

	// get client rect
	GetClientRect(hwnd, &rc) ;

	// assign board and buffer sizes
	nW  = (Game [nG].rBoard.right  - Game [nG].rBoard.left) + 1 ;
	nH  = (Game [nG].rBoard.bottom - Game [nG].rBoard.top) + 1 ;
	nW1 = (Game [nG].rBuffer.right - Game [nG].rBuffer.left) + 1 ;

	// process player
	if(bPlayer)
	{
		if(User.bTruePremove)
		{
			if(User.bShowTPHighlight)
			{
				// erase true premove sent highlight
				if(Premove.bLastTP)
				{
					if((Premove.ptLastTP [0].x >= 0) && (Premove.ptLastTP [0].y >= 0))
					{
						Game [nG].nLastBoard [Premove.ptLastTP [0].x] [Premove.ptLastTP [0].y] = -1 ;
					}
					if((Premove.ptLastTP [1].x >= 0) && (Premove.ptLastTP [1].y >= 0))
					{
						Game [nG].nLastBoard [Premove.ptLastTP [1].x] [Premove.ptLastTP [1].y] = -1 ;
					}

					// reset last true premove highlight
					Premove.bLastTP        = 0 ;
					Premove.ptLastTP [0].x = -1 ;
					Premove.ptLastTP [0].y = -1 ;
					Premove.ptLastTP [1].x = -1 ;
					Premove.ptLastTP [1].y = -1 ;
				}

				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BUFFER :
					case DRAW_STATE_MOUSE_DOWN :
					case DRAW_STATE_MOUSE_MOVE :
					case DRAW_STATE_RESTORE :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;

					case DRAW_STATE_BOARD :
					case DRAW_STATE_MOUSE_UP :
						// erase illegal true premove highlight
						if(Premove.bIllegalTP)
						{
							if((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [0].x] [Premove.ptIllegalTP [0].y] = -1 ;
							}
							if((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [1].x] [Premove.ptIllegalTP [1].y] = -1 ;
							}

							// reset illegal true premove highlight
							Premove.bIllegalTP        = 0 ;
							Premove.ptIllegalTP [0].x = -1 ;
							Premove.ptIllegalTP [0].y = -1 ;
							Premove.ptIllegalTP [1].x = -1 ;
							Premove.ptIllegalTP [1].y = -1 ;
						}
						break ;

					case DRAW_STATE_NEW_GAME :
					case DRAW_STATE_ERASE_ALL_TP :
						// erase illegal true premove highlight
						if(Premove.bIllegalTP)
						{
							if((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [0].x] [Premove.ptIllegalTP [0].y] = -1 ;
							}
							if((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [1].x] [Premove.ptIllegalTP [1].y] = -1 ;
							}

							// reset illegal true premove highlight
							Premove.bIllegalTP        = 0 ;
							Premove.ptIllegalTP [0].x = -1 ;
							Premove.ptIllegalTP [0].y = -1 ;
							Premove.ptIllegalTP [1].x = -1 ;
							Premove.ptIllegalTP [1].y = -1 ;
						}

						// erase all true move highlight
						nOT = Premove.nPremoveTail ;
						for(nI = 0 ; nI < Premove.nPremoveCount ; nI++)
						{
							if(Premove.nPremoveLegalBuffer [nOT] [3] >= 0)
							{
								Game [nG].nLastBoard [Premove.nPremoveLegalBuffer [nOT] [3]] [Premove.nPremoveLegalBuffer [nOT] [4]] = -1 ;
							}
							if(Premove.nPremoveLegalBuffer [nOT] [5] >= 0)
							{
								Game [nG].nLastBoard [Premove.nPremoveLegalBuffer [nOT] [5]] [Premove.nPremoveLegalBuffer [nOT] [6]] = -1 ;
							}

							nOT = nOT + 1 ;
							if(nOT >= MAX_TRUE_PREMOVE)
							{
								nOT = 0 ;
							}
						}

						// reset true premove count
						Premove.nPremoveCount = 0 ;
						Premove.nPremoveHead  = 0 ;
						Premove.nPremoveTail  = 0 ;

						// force last move highlights to be drawn
						Game [nG].ptLastHighlight [0].x = -1 ;
						Game [nG].ptLastHighlight [0].y = -1 ;
						Game [nG].ptLastHighlight [1].x = -1 ;
						Game [nG].ptLastHighlight [1].y = -1 ;
						break ;
				}
			}
			else
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BUFFER :
					case DRAW_STATE_MOUSE_DOWN :
					case DRAW_STATE_MOUSE_MOVE :
					case DRAW_STATE_RESTORE :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;

					case DRAW_STATE_BOARD :
					case DRAW_STATE_MOUSE_UP :
						// erase illegal true premove highlight
						if(Premove.bIllegalTP)
						{
							if((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [0].x] [Premove.ptIllegalTP [0].y] = -1 ;
							}
							if((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [1].x] [Premove.ptIllegalTP [1].y] = -1 ;
							}

							// reset illegal true premove highlight
							Premove.bIllegalTP        = 0 ;
							Premove.ptIllegalTP [0].x = -1 ;
							Premove.ptIllegalTP [0].y = -1 ;
							Premove.ptIllegalTP [1].x = -1 ;
							Premove.ptIllegalTP [1].y = -1 ;
						}
						break ;

					case DRAW_STATE_NEW_GAME :
					case DRAW_STATE_ERASE_ALL_TP :
						// erase illegal true premove highlight
						if(Premove.bIllegalTP)
						{
							if((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [0].x] [Premove.ptIllegalTP [0].y] = -1 ;
							}
							if((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
							{
								Game [nG].nLastBoard [Premove.ptIllegalTP [1].x] [Premove.ptIllegalTP [1].y] = -1 ;
							}

							// reset illegal true premove highlight
							Premove.bIllegalTP        = 0 ;
							Premove.ptIllegalTP [0].x = -1 ;
							Premove.ptIllegalTP [0].y = -1 ;
							Premove.ptIllegalTP [1].x = -1 ;
							Premove.ptIllegalTP [1].y = -1 ;
						}

						// force last move highlights to be drawn
						Game [nG].ptLastHighlight [0].x = -1 ;
						Game [nG].ptLastHighlight [0].y = -1 ;
						Game [nG].ptLastHighlight [1].x = -1 ;
						Game [nG].ptLastHighlight [1].y = -1 ;
						break ;
				}
			}
		}
	}

	// initialize clipping
	nClips = 0 ;

	// drag piece clipping
	if(bDragPiece)
	{
		if(User.nMoveType == DRAG_MOVE)
		{
			if(DragInfo.bFromBoard)
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == DragInfo.nPc)
						{
							// original square from board
							BOARD_SquareToPosition(nG, DragInfo.ptFrom.x, DragInfo.ptFrom.y, &nPx, &nPy) ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary remove the dragging board piece from original position
							Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == DragInfo.nPc)
						{
							// original square from board
							BOARD_SquareToPosition(nG, DragInfo.ptFrom.x, DragInfo.ptFrom.y, &nPx, &nPy) ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary remove the dragging board piece from original position
							Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;

							// force last move highlights to be drawn
							Game [nG].ptLastHighlight [0].x = -1 ;
							Game [nG].ptLastHighlight [0].y = -1 ;
							Game [nG].ptLastHighlight [1].x = -1 ;
							Game [nG].ptLastHighlight [1].y = -1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == DragInfo.nPc)
						{
							// original square from board
							BOARD_SquareToPosition(nG, DragInfo.ptFrom.x, DragInfo.ptFrom.y, &nPx, &nPy) ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary remove the dragging board piece from original position
							Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y])
						{
							if(Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == EMPTY_SQUARE)
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = WHITE_PAWN ;
							}
							else
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
							}
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;

						// force last move highlights to be drawn
						Game [nG].ptLastHighlight [0].x = -1 ;
						Game [nG].ptLastHighlight [0].y = -1 ;
						Game [nG].ptLastHighlight [1].x = -1 ;
						Game [nG].ptLastHighlight [1].y = -1 ;

						// force last legal king square hightlight to be drawn
						if(User.bShowKingHighlight)
						{
							for(nI = 0 ; nI < 8 ; nI++)
							{
								Game [nG].ptLastKing [INDEX_WHITE] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_WHITE] [nI].y = -1 ;

								Game [nG].ptLastKing [INDEX_BLACK] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_BLACK] [nI].y = -1 ;
							}
						}
						break ;

					case DRAW_STATE_NEW_GAME :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y])
						{
							if(Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == EMPTY_SQUARE)
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = WHITE_PAWN ;
							}
							else
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
							}
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
			else
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBuffer [DragInfo.nPc] > 0)
						{
							// original square from buffer
							nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [DragInfo.nPc].x ;
							nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [DragInfo.nPc].y ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary reduce the dragging buffer piece count by one
							Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] - 1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBuffer [DragInfo.nPc] > 0)
						{
							// original square from buffer
							nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [DragInfo.nPc].x ;
							nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [DragInfo.nPc].y ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary reduce the dragging buffer piece count by one
							Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] - 1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBuffer [DragInfo.nPc] > 0)
						{
							// original square from buffer
							nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [DragInfo.nPc].x ;
							nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [DragInfo.nPc].y ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary reduce the dragging buffer piece count by one
							Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] - 1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBuffer [DragInfo.nPc] == Game [nG].nLastBuffer [DragInfo.nPc])
						{
							Game [nG].nLastBuffer [DragInfo.nPc] = Game [nG].nLastBuffer [DragInfo.nPc] + 1 ;
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;

						// force last move highlights to be drawn
						Game [nG].ptLastHighlight [0].x = -1 ;
						Game [nG].ptLastHighlight [0].y = -1 ;
						Game [nG].ptLastHighlight [1].x = -1 ;
						Game [nG].ptLastHighlight [1].y = -1 ;

						// force last legal king square hightlight to be drawn
						if(User.bShowKingHighlight)
						{
							for(nI = 0 ; nI < 8 ; nI++)
							{
								Game [nG].ptLastKing [INDEX_WHITE] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_WHITE] [nI].y = -1 ;

								Game [nG].ptLastKing [INDEX_BLACK] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_BLACK] [nI].y = -1 ;
							}
						}
						break ;

					case DRAW_STATE_NEW_GAME :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBuffer [DragInfo.nPc] == Game [nG].nLastBuffer [DragInfo.nPc])
						{
							Game [nG].nLastBuffer [DragInfo.nPc] = Game [nG].nLastBuffer [DragInfo.nPc] + 1 ;
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
		}
		else
		{
			if(DragInfo.bFromBoard)
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] != DragInfo.nPc)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] != DragInfo.nPc)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] != DragInfo.nPc)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_NEW_GAME :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
			else
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBuffer [DragInfo.nPc] <= 0)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBuffer [DragInfo.nPc] <= 0)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBuffer [DragInfo.nPc] <= 0)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_NEW_GAME :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
		}
	}

	// board clipping
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			if(Game [nG].nBoard [nX] [nY] != Game [nG].nLastBoard [nX] [nY])
			{
				BOARD_SquareToPosition(nG, nX, nY, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
		}
	}

	// buffer clipping
	for(nI = 0 ; nI < MAX_BUFFER ; nI++)
	{
		if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
		{
			if(Game [nG].bChessGame)
			{
				if(User.nShowCapturedChessPiece == 2)
				{
					if((Game [nG].nRelation ==  0) ||   //  0 i am observing a game being played
							(Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
							(Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
					{
						nPc = ReverseChessPiece [nI] ;
						nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nPc].x ;
						nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nPc].y ;
					}
					else
					{
						nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
						nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;
					}
				}
				else
				{
					nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
					nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;
				}
			}
			else
			{
				nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
				nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;
			}
			Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
		}
	}

	// last move clipping
	if(User.bShowHighlight)
	{
		if((Game [nG].ptHighlight [0].x != Game [nG].ptLastHighlight [0].x) ||
				(Game [nG].ptHighlight [0].y != Game [nG].ptLastHighlight [0].y) ||
				(Game [nG].ptHighlight [1].x != Game [nG].ptLastHighlight [1].x) ||
				(Game [nG].ptHighlight [1].y != Game [nG].ptLastHighlight [1].y))
		{
			// erase last highlight
			if((Game [nG].ptLastHighlight [0].x >= 0) && (Game [nG].ptLastHighlight [0].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptLastHighlight [0].x, Game [nG].ptLastHighlight [0].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
			if((Game [nG].ptLastHighlight [1].x >= 0) && (Game [nG].ptLastHighlight [1].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptLastHighlight [1].x, Game [nG].ptLastHighlight [1].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}

			// draw new highlight
			if((Game [nG].ptHighlight [0].x >= 0) && (Game [nG].ptHighlight [0].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptHighlight [0].x, Game [nG].ptHighlight [0].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
			if((Game [nG].ptHighlight [1].x >= 0) && (Game [nG].ptHighlight [1].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptHighlight [1].x, Game [nG].ptHighlight [1].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
		}
	}

	// king legal square clipping
	if(User.bShowKingHighlight)
	{
		for(nI = 0 ; nI < 8 ; nI++)
		{
			if((Game [nG].ptKing [INDEX_WHITE] [nI].x != Game [nG].ptLastKing [INDEX_WHITE] [nI].x) ||
					(Game [nG].ptKing [INDEX_WHITE] [nI].y != Game [nG].ptLastKing [INDEX_WHITE] [nI].y))
			{
				// erase last highlight
				if((Game [nG].ptLastKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_WHITE] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptLastKing [INDEX_WHITE] [nI].x, Game [nG].ptLastKing [INDEX_WHITE] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}

				// draw new highlight
				if((Game [nG].ptKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptKing [INDEX_WHITE] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptKing [INDEX_WHITE] [nI].x, Game [nG].ptKing [INDEX_WHITE] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}
			}

			if((Game [nG].ptKing [INDEX_BLACK] [nI].x != Game [nG].ptLastKing [INDEX_BLACK] [nI].x) ||
					(Game [nG].ptKing [INDEX_BLACK] [nI].y != Game [nG].ptLastKing [INDEX_BLACK] [nI].y))
			{
				// erase last highlight
				if((Game [nG].ptLastKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_BLACK] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptLastKing [INDEX_BLACK] [nI].x, Game [nG].ptLastKing [INDEX_BLACK] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}

				// draw new highlight
				if((Game [nG].ptKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptKing [INDEX_BLACK] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptKing [INDEX_BLACK] [nI].x, Game [nG].ptKing [INDEX_BLACK] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}
			}
		}
	}

	// illegal true move and true premove highlight clipping
	if(bPlayer)
	{
		if(User.bTruePremove)
		{
			// illegal true premove
			if(Premove.bIllegalTP)
			{
				if((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
				{
					BOARD_SquareToPosition(nG,
										   Premove.ptIllegalTP [0].x,
										   Premove.ptIllegalTP [0].y,
										   &nPx,
										   &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}
				if((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
				{
					BOARD_SquareToPosition(nG,
										   Premove.ptIllegalTP [1].x,
										   Premove.ptIllegalTP [1].y,
										   &nPx,
										   &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}
			}

			if(User.bShowTPHighlight)
			{
				// true premove
				nOT = Premove.nPremoveTail ;
				for(nI = 0 ; nI < Premove.nPremoveCount ; nI++)
				{
					if(Premove.nPremoveLegalBuffer [nOT] [3] >= 0)
					{
						BOARD_SquareToPosition(nG,
											   Premove.nPremoveLegalBuffer [nOT] [3],
											   Premove.nPremoveLegalBuffer [nOT] [4],
											   &nPx,
											   &nPy) ;
						Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
					}
					if(Premove.nPremoveLegalBuffer [nOT] [5] >= 0)
					{
						BOARD_SquareToPosition(nG,
											   Premove.nPremoveLegalBuffer [nOT] [5],
											   Premove.nPremoveLegalBuffer [nOT] [6],
											   &nPx,
											   &nPy) ;
						Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
					}

					nOT = nOT + 1 ;
					if(nOT >= MAX_TRUE_PREMOVE)
					{
						nOT = 0 ;
					}
				}
			}
		}
	}

	// if never created Game [nG].hWindow then do it now
	if(Game [nG].hWindow == NULL)
	{
		// create window bitmap
		Game [nG].hWindow = CreateCompatibleBitmap(hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
		bNeverDrew        = 1 ;
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// if never saved once before then do it now
	if(bNeverDrew)
	{
		// copy window into Game [nG].hWindow
		BitBlt(hdcWindow, 0, 0, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1, hdc, 0, 0, SRCCOPY) ;

		// draw full position
		BOARD_DrawFullPosition(nG, hwnd, hdcWindow) ;

		// remove all clippings and set it to none
		if(nClips > 0)
		{
			for(nI = 0 ; nI < nClips ; nI++)
			{
				DeleteObject(Clips [nI]) ;
			}
			nClips = 0 ;
		}
	}

	// set clipping on memory DC
	if(nClips > 0)
	{
		SelectClipRgn(hdcWindow, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdcWindow, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// draw board
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			if(Game [nG].nBoard [nX] [nY] != Game [nG].nLastBoard [nX] [nY])
			{
				// draw board piece
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;

				// save last board
				Game [nG].nLastBoard [nX] [nY] = Game [nG].nBoard [nX] [nY] ;
			}
		}
	}

	// draw buffer
	if(Game [nG].bChessGame)
	{
		if(User.nShowCapturedChessPiece == 2)
		{
			if((Game [nG].nRelation ==  0) ||   //  0 i am observing a game being played
					(Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
					(Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawChessBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;

						// save last buffer
						Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
					}
				}
			}
			else
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;

						// save last buffer
						Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
					}
				}
			}
		}
		else
		{
			for(nI = 0 ; nI < MAX_BUFFER ; nI++)
			{
				if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
				{
					// draw buffer piece
					BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;

					// save last buffer
					Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
				}
			}
		}
	}
	else
	{
		for(nI = 0 ; nI < MAX_BUFFER ; nI++)
		{
			if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
			{
				// draw buffer piece
				BOARD_DrawBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;

				// save last buffer
				Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
			}
		}
	}

	// draw last move highlight
	if(User.bShowHighlight)
	{
		if((Game [nG].ptHighlight [0].x != Game [nG].ptLastHighlight [0].x) ||
				(Game [nG].ptHighlight [0].y != Game [nG].ptLastHighlight [0].y) ||
				(Game [nG].ptHighlight [1].x != Game [nG].ptLastHighlight [1].x) ||
				(Game [nG].ptHighlight [1].y != Game [nG].ptLastHighlight [1].y))
		{
			if((Game [nG].ptLastHighlight [0].x >= 0) && (Game [nG].ptLastHighlight [0].y >= 0))
			{
				nX = Game [nG].ptLastHighlight [0].x ;
				nY = Game [nG].ptLastHighlight [0].y ;
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
			}
			if((Game [nG].ptLastHighlight [1].x >= 0) && (Game [nG].ptLastHighlight [1].y >= 0))
			{
				nX = Game [nG].ptLastHighlight [1].x ;
				nY = Game [nG].ptLastHighlight [1].y ;
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
			}

			if((Game [nG].ptHighlight [0].x >= 0) && (Game [nG].ptHighlight [0].y >= 0))
			{
				BOARD_DrawHighlight(nG, Game [nG].ptHighlight [0].x, Game [nG].ptHighlight [0].y, hdcWindow, PEN_HIGHLIGHT) ;
			}
			if((Game [nG].ptHighlight [1].x >= 0) && (Game [nG].ptHighlight [1].y >= 0))
			{
				BOARD_DrawHighlight(nG, Game [nG].ptHighlight [1].x, Game [nG].ptHighlight [1].y, hdcWindow, PEN_HIGHLIGHT) ;
			}

			Game [nG].ptLastHighlight [0] = Game [nG].ptHighlight [0] ;
			Game [nG].ptLastHighlight [1] = Game [nG].ptHighlight [1] ;
		}
	}

	// draw last king legal square highlight
	if(User.bShowKingHighlight)
	{
		for(nI = 0 ; nI < 8 ; nI++)
		{
			if((Game [nG].ptKing [INDEX_WHITE] [nI].x != Game [nG].ptLastKing [INDEX_WHITE] [nI].x) ||
					(Game [nG].ptKing [INDEX_WHITE] [nI].y != Game [nG].ptLastKing [INDEX_WHITE] [nI].y))
			{
				if((Game [nG].ptLastKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_WHITE] [nI].y >= 0))
				{
					nX = Game [nG].ptLastKing [INDEX_WHITE] [nI].x ;
					nY = Game [nG].ptLastKing [INDEX_WHITE] [nI].y ;
					BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
				}

				if((Game [nG].ptKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptKing [INDEX_WHITE] [nI].y >= 0))
				{
					BOARD_DrawHighlight(nG, Game [nG].ptKing [INDEX_WHITE] [nI].x, Game [nG].ptKing [INDEX_WHITE] [nI].y, hdcWindow, PEN_KING_HIGHLIGHT) ;
				}

				Game [nG].ptLastKing [INDEX_WHITE] [nI] = Game [nG].ptKing [INDEX_WHITE] [nI] ;
			}

			if((Game [nG].ptKing [INDEX_BLACK] [nI].x != Game [nG].ptLastKing [INDEX_BLACK] [nI].x) ||
					(Game [nG].ptKing [INDEX_BLACK] [nI].y != Game [nG].ptLastKing [INDEX_BLACK] [nI].y))
			{
				if((Game [nG].ptLastKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_BLACK] [nI].y >= 0))
				{
					nX = Game [nG].ptLastKing [INDEX_BLACK] [nI].x ;
					nY = Game [nG].ptLastKing [INDEX_BLACK] [nI].y ;
					BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
				}

				if((Game [nG].ptKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptKing [INDEX_BLACK] [nI].y >= 0))
				{
					BOARD_DrawHighlight(nG, Game [nG].ptKing [INDEX_BLACK] [nI].x, Game [nG].ptKing [INDEX_BLACK] [nI].y, hdcWindow, PEN_KING_HIGHLIGHT) ;
				}

				Game [nG].ptLastKing [INDEX_BLACK] [nI] = Game [nG].ptKing [INDEX_BLACK] [nI] ;
			}
		}
	}

	// draw illegal true premove and true premove highlight (only if it's the playing board)
	if(bPlayer)
	{
		if(User.bTruePremove)
		{
			// illegal true premove highlight
			if(Premove.bIllegalTP)
			{
				if((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
				{
					BOARD_DrawHighlight(nG, Premove.ptIllegalTP [0].x, Premove.ptIllegalTP [0].y, hdcWindow, PEN_ILLEGAL_TRUEPREMOVE) ;
				}
				if((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
				{
					BOARD_DrawHighlight(nG, Premove.ptIllegalTP [1].x, Premove.ptIllegalTP [1].y, hdcWindow, PEN_ILLEGAL_TRUEPREMOVE) ;
				}
			}

			if(User.bShowTPHighlight)
			{
				// true premove highlight
				nOT = Premove.nPremoveTail ;
				for(nI = 0 ; nI < Premove.nPremoveCount ; nI++)
				{
					if(Premove.nPremoveLegalBuffer [nOT] [3] >= 0)
					{
						BOARD_DrawHighlight(nG, Premove.nPremoveLegalBuffer [nOT] [3], Premove.nPremoveLegalBuffer [nOT] [4], hdcWindow, PEN_TRUEPREMOVE) ;
					}
					if(Premove.nPremoveLegalBuffer [nOT] [5] >= 0)
					{
						BOARD_DrawHighlight(nG, Premove.nPremoveLegalBuffer [nOT] [5], Premove.nPremoveLegalBuffer [nOT] [6], hdcWindow, PEN_TRUEPREMOVE) ;
					}

					nOT = nOT + 1 ;
					if(nOT >= MAX_TRUE_PREMOVE)
					{
						nOT = 0 ;
					}
				}
			}
		}
	}

	// put the dragged piece back into place and draw it
	if(bDragPiece)
	{
		if(User.nMoveType == DRAG_MOVE)
		{
			if(DragInfo.nPc == EMPTY_SQUARE)
			{

				// lost the dragged piece
				// if it's in the pawn promotion window then close the promotion window
				if(System.bInPromote)
				{
					if(IsWindow(System.hwndPromote))
					{
						SendMessage(System.hwndPromote, WM_CLOSE, 0, 0) ;
					}
				}
			}
			else
			{

				// put drag piece back
				if(DragInfo.bFromBoard)
				{
					Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = DragInfo.nPc ;
				}
				else
				{
					Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] + 1 ;
				}

				// set draw drag piece save background to true
				bDrag = 1 ;

				// create memory DC
				hdcSave   = CreateCompatibleDC(hdc) ;
				htOldSave = (HBITMAP) SelectObject(hdcSave, Game [nG].hSave) ;

				// copy background
				BitBlt(hdcSave, 0, 0, Game [nG].nss, Game [nG].nss, hdcWindow, DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, SRCCOPY) ;

				// draw drag piece
				BOARD_DrawDragPiece(nG, hdcWindow) ;
			}
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;

	// set clipping on target DC
	if(nClips > 0)
	{
		SelectClipRgn(hdc, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdc, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// copy the new bitmap onto the screen in one go to avoid any flickering
	if(TOOLBOX_ShowBuffer(nG))
	{
		switch(User.nBufferOrientation)
		{
			case DEFAULT_BUFFER_LEFT :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, nH, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_RIGHT :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_TOPBOTTOML :
			case DEFAULT_BUFFER_TOPBOTTOMR :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, (Game [nG].rBuffer1.bottom - Game [nG].rBuffer.top) + 1, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			default :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;
		}
	}
	else
	{
		BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
	}

	// check to see if drag piece save background is true
	if(bDrag)
	{
		// restore drag piece saved background
		BitBlt(hdcWindow, DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, Game [nG].nss, Game [nG].nss, hdcSave, 0, 0, SRCCOPY) ;

		// clean up
		SelectObject(hdcSave, htOldSave) ;
		DeleteDC(hdcSave) ;
	}

	// massive clean up
	if(nClips > 0)
	{
		for(nI = 0 ; nI < nClips ; nI++)
		{
			DeleteObject(Clips [nI]) ;
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;
}

void BOARD_DrawBoard1(int nG, HWND hwnd, HDC hdc, int nState)
{
	int bDragPiece = 0, bNeverDrew = 0, bDrag = 0 ;

	int nY, nX, nI, nPx, nPy, nPc ;

	HDC hdcWindow, hdcSave ;
	HBITMAP htOldWindow, htOldSave ;
	RECT rc ;

	int nW, nH, nW1 ;

	if(nG == INDEX_PLAY)
	{
		BOARD_DrawBoard(nG, hwnd, hdc, nState) ;
		return ;
	}

	// make sure the window is not minimized
	if(IsIconic(hwnd))
	{
		return ;
	}

	// determine game relation
	if(Game [nG].nGameNumber == 0)
	{
	}
	else
	{
		if(Game [nG].nGameNumber == Game [INDEX_PLAY].nGamePartner)
		{
			if(DragInfo.nPc != EMPTY_SQUARE)
			{
				if(DragInfo.nIndex == nG)
				{
					bDragPiece = 1 ;
				}
			}
		}
	}

	// get client rect
	GetClientRect(hwnd, &rc) ;

	// assign board and buffer sizes
	nW  = (Game [nG].rBoard.right  - Game [nG].rBoard.left) + 1 ;
	nH  = (Game [nG].rBoard.bottom - Game [nG].rBoard.top) + 1 ;
	nW1 = (Game [nG].rBuffer.right - Game [nG].rBuffer.left) + 1 ;

	// initialize clipping
	nClips = 0 ;

	// drag piece clipping
	if(bDragPiece)
	{
		if(User.nMoveType == DRAG_MOVE)
		{
			if(DragInfo.bFromBoard)
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == DragInfo.nPc)
						{
							// original square from board
							BOARD_SquareToPosition(nG, DragInfo.ptFrom.x, DragInfo.ptFrom.y, &nPx, &nPy) ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary remove the dragging board piece from original position
							Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == DragInfo.nPc)
						{
							// original square from board
							BOARD_SquareToPosition(nG, DragInfo.ptFrom.x, DragInfo.ptFrom.y, &nPx, &nPy) ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary remove the dragging board piece from original position
							Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;

							// force last move highlights to be drawn
							Game [nG].ptLastHighlight [0].x = -1 ;
							Game [nG].ptLastHighlight [0].y = -1 ;
							Game [nG].ptLastHighlight [1].x = -1 ;
							Game [nG].ptLastHighlight [1].y = -1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == DragInfo.nPc)
						{
							// original square from board
							BOARD_SquareToPosition(nG, DragInfo.ptFrom.x, DragInfo.ptFrom.y, &nPx, &nPy) ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary remove the dragging board piece from original position
							Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y])
						{
							if(Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == EMPTY_SQUARE)
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = WHITE_PAWN ;
							}
							else
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
							}
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;

						// force last move highlights to be drawn
						Game [nG].ptLastHighlight [0].x = -1 ;
						Game [nG].ptLastHighlight [0].y = -1 ;
						Game [nG].ptLastHighlight [1].x = -1 ;
						Game [nG].ptLastHighlight [1].y = -1 ;

						// force last legal king square hightlight to be drawn
						if(User.bShowKingHighlight)
						{
							for(nI = 0 ; nI < 8 ; nI++)
							{
								Game [nG].ptLastKing [INDEX_WHITE] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_WHITE] [nI].y = -1 ;

								Game [nG].ptLastKing [INDEX_BLACK] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_BLACK] [nI].y = -1 ;
							}
						}
						break ;

					case DRAW_STATE_NEW_GAME :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y])
						{
							if(Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] == EMPTY_SQUARE)
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = WHITE_PAWN ;
							}
							else
							{
								Game [nG].nLastBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = EMPTY_SQUARE ;
							}
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
			else
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBuffer [DragInfo.nPc] > 0)
						{
							// original square from buffer
							nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [DragInfo.nPc].x ;
							nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [DragInfo.nPc].y ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary reduce the dragging buffer piece count by one
							Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] - 1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBuffer [DragInfo.nPc] > 0)
						{
							// original square from buffer
							nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [DragInfo.nPc].x ;
							nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [DragInfo.nPc].y ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary reduce the dragging buffer piece count by one
							Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] - 1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBuffer [DragInfo.nPc] > 0)
						{
							// original square from buffer
							nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [DragInfo.nPc].x ;
							nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [DragInfo.nPc].y ;
							Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							// temporary reduce the dragging buffer piece count by one
							Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] - 1 ;
						}
						else
						{
							// current position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

							// last position
							Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBuffer [DragInfo.nPc] == Game [nG].nLastBuffer [DragInfo.nPc])
						{
							Game [nG].nLastBuffer [DragInfo.nPc] = Game [nG].nLastBuffer [DragInfo.nPc] + 1 ;
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;

						// force last move highlights to be drawn
						Game [nG].ptLastHighlight [0].x = -1 ;
						Game [nG].ptLastHighlight [0].y = -1 ;
						Game [nG].ptLastHighlight [1].x = -1 ;
						Game [nG].ptLastHighlight [1].y = -1 ;

						// force last legal king square hightlight to be drawn
						if(User.bShowKingHighlight)
						{
							for(nI = 0 ; nI < 8 ; nI++)
							{
								Game [nG].ptLastKing [INDEX_WHITE] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_WHITE] [nI].y = -1 ;

								Game [nG].ptLastKing [INDEX_BLACK] [nI].x = -1 ;
								Game [nG].ptLastKing [INDEX_BLACK] [nI].y = -1 ;
							}
						}
						break ;

					case DRAW_STATE_NEW_GAME :
						// current position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, DragInfo.ptCurrent.x + Game [nG].nss, DragInfo.ptCurrent.y + Game [nG].nss) ;

						// last position
						Clips [nClips++] = CreateRectRgn(DragInfo.ptLast.x, DragInfo.ptLast.y, DragInfo.ptLast.x + Game [nG].nss, DragInfo.ptLast.y + Game [nG].nss) ;

						if(Game [nG].nBuffer [DragInfo.nPc] == Game [nG].nLastBuffer [DragInfo.nPc])
						{
							Game [nG].nLastBuffer [DragInfo.nPc] = Game [nG].nLastBuffer [DragInfo.nPc] + 1 ;
						}

						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
		}
		else
		{
			if(DragInfo.bFromBoard)
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] != DragInfo.nPc)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] != DragInfo.nPc)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] != DragInfo.nPc)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_NEW_GAME :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
			else
			{
				switch(nState)
				{
					case DRAW_STATE_NONE :
					case DRAW_STATE_BOARD :
					case DRAW_STATE_BUFFER :
						if(Game [nG].nBuffer [DragInfo.nPc] <= 0)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_DOWN :
						if(Game [nG].nBuffer [DragInfo.nPc] <= 0)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_MOVE :
						if(Game [nG].nBuffer [DragInfo.nPc] <= 0)
						{
							DragInfo.nIndex   = -1 ;
							DragInfo.nPc      = EMPTY_SQUARE ;
							DragInfo.ptFrom.x = -1 ;
							DragInfo.ptFrom.y = -1 ;
							DragInfo.nClicked = 0 ;
						}
						break ;

					case DRAW_STATE_MOUSE_UP :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_RESTORE :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_NEW_GAME :
						DragInfo.nIndex   = -1 ;
						DragInfo.nPc      = EMPTY_SQUARE ;
						DragInfo.ptFrom.x = -1 ;
						DragInfo.ptFrom.y = -1 ;
						DragInfo.nClicked = 0 ;
						break ;

					case DRAW_STATE_ERASE_ALL_TP :
					case DRAW_STATE_DRAW_ILLEGAL_TP :
						break ;
				}
			}
		}
	}

	// board clipping
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			if(Game [nG].nBoard [nX] [nY] != Game [nG].nLastBoard [nX] [nY])
			{
				BOARD_SquareToPosition(nG, nX, nY, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
		}
	}

	// buffer clipping
	for(nI = 0 ; nI < MAX_BUFFER ; nI++)
	{
		if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
		{
			if(Game [nG].bChessGame)
			{
				if(User.nShowCapturedChessPiece == 2)
				{
					if((Game [nG].nRelation ==  0) ||   //  0 i am observing a game being played
							(Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
							(Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
					{
						nPc = ReverseChessPiece [nI] ;
						nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nPc].x ;
						nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nPc].y ;
					}
					else
					{
						nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
						nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;
					}
				}
				else
				{
					nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
					nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;
				}
			}
			else
			{
				nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [nI].x ;
				nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [nI].y ;
			}
			Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
		}
	}

	// last move clipping
	if(User.bShowHighlight)
	{
		if((Game [nG].ptHighlight [0].x != Game [nG].ptLastHighlight [0].x) ||
				(Game [nG].ptHighlight [0].y != Game [nG].ptLastHighlight [0].y) ||
				(Game [nG].ptHighlight [1].x != Game [nG].ptLastHighlight [1].x) ||
				(Game [nG].ptHighlight [1].y != Game [nG].ptLastHighlight [1].y))
		{
			// erase last highlight
			if((Game [nG].ptLastHighlight [0].x >= 0) && (Game [nG].ptLastHighlight [0].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptLastHighlight [0].x, Game [nG].ptLastHighlight [0].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
			if((Game [nG].ptLastHighlight [1].x >= 0) && (Game [nG].ptLastHighlight [1].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptLastHighlight [1].x, Game [nG].ptLastHighlight [1].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}

			// draw new highlight
			if((Game [nG].ptHighlight [0].x >= 0) && (Game [nG].ptHighlight [0].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptHighlight [0].x, Game [nG].ptHighlight [0].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
			if((Game [nG].ptHighlight [1].x >= 0) && (Game [nG].ptHighlight [1].y >= 0))
			{
				BOARD_SquareToPosition(nG, Game [nG].ptHighlight [1].x, Game [nG].ptHighlight [1].y, &nPx, &nPy) ;
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
			}
		}
	}

	// king legal square clipping
	if(User.bShowKingHighlight)
	{
		for(nI = 0 ; nI < 8 ; nI++)
		{
			if((Game [nG].ptKing [INDEX_WHITE] [nI].x != Game [nG].ptLastKing [INDEX_WHITE] [nI].x) ||
					(Game [nG].ptKing [INDEX_WHITE] [nI].y != Game [nG].ptLastKing [INDEX_WHITE] [nI].y))
			{
				// erase last highlight
				if((Game [nG].ptLastKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_WHITE] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptLastKing [INDEX_WHITE] [nI].x, Game [nG].ptLastKing [INDEX_WHITE] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}

				// draw new highlight
				if((Game [nG].ptKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptKing [INDEX_WHITE] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptKing [INDEX_WHITE] [nI].x, Game [nG].ptKing [INDEX_WHITE] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}
			}

			if((Game [nG].ptKing [INDEX_BLACK] [nI].x != Game [nG].ptLastKing [INDEX_BLACK] [nI].x) ||
					(Game [nG].ptKing [INDEX_BLACK] [nI].y != Game [nG].ptLastKing [INDEX_BLACK] [nI].y))
			{
				// erase last highlight
				if((Game [nG].ptLastKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_BLACK] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptLastKing [INDEX_BLACK] [nI].x, Game [nG].ptLastKing [INDEX_BLACK] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}

				// draw new highlight
				if((Game [nG].ptKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptKing [INDEX_BLACK] [nI].y >= 0))
				{
					BOARD_SquareToPosition(nG, Game [nG].ptKing [INDEX_BLACK] [nI].x, Game [nG].ptKing [INDEX_BLACK] [nI].y, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;
				}
			}
		}
	}

	// if never created Game [nG].hWindow then do it now
	if(Game [nG].hWindow == NULL)
	{
		// create window bitmap
		Game [nG].hWindow = CreateCompatibleBitmap(hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
		bNeverDrew        = 1 ;
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// if never saved once before then do it now
	if(bNeverDrew)
	{
		// copy window into Game [nG].hWindow
		BitBlt(hdcWindow, 0, 0, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1, hdc, 0, 0, SRCCOPY) ;

		// draw full position
		BOARD_DrawFullPosition1(nG, hwnd, hdcWindow) ;

		// remove all clippings and set it to none
		if(nClips > 0)
		{
			for(nI = 0 ; nI < nClips ; nI++)
			{
				DeleteObject(Clips [nI]) ;
			}
			nClips = 0 ;
		}
	}

	// set clipping on memory DC
	if(nClips > 0)
	{
		SelectClipRgn(hdcWindow, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdcWindow, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// draw board
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			if(Game [nG].nBoard [nX] [nY] != Game [nG].nLastBoard [nX] [nY])
			{
				// draw board piece
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;

				// save last board
				Game [nG].nLastBoard [nX] [nY] = Game [nG].nBoard [nX] [nY] ;
			}
		}
	}

	// draw buffer
	if(Game [nG].bChessGame)
	{
		if(User.nShowCapturedChessPiece == 2)
		{
			if((Game [nG].nRelation ==  0) ||   //  0 i am observing a game being played
					(Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
					(Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawChessBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;

						// save last buffer
						Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
					}
				}
			}
			else
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;

						// save last buffer
						Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
					}
				}
			}
		}
		else
		{
			for(nI = 0 ; nI < MAX_BUFFER ; nI++)
			{
				if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
				{
					// draw buffer piece
					BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;

					// save last buffer
					Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
				}
			}
		}
	}
	else
	{
		for(nI = 0 ; nI < MAX_BUFFER ; nI++)
		{
			if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
			{
				// draw buffer piece
				BOARD_DrawBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;

				// save last buffer
				Game [nG].nLastBuffer [nI] = Game [nG].nBuffer [nI] ;
			}
		}
	}

	// draw last move highlight
	if(User.bShowHighlight)
	{
		if((Game [nG].ptHighlight [0].x != Game [nG].ptLastHighlight [0].x) ||
				(Game [nG].ptHighlight [0].y != Game [nG].ptLastHighlight [0].y) ||
				(Game [nG].ptHighlight [1].x != Game [nG].ptLastHighlight [1].x) ||
				(Game [nG].ptHighlight [1].y != Game [nG].ptLastHighlight [1].y))
		{
			if((Game [nG].ptLastHighlight [0].x >= 0) && (Game [nG].ptLastHighlight [0].y >= 0))
			{
				nX = Game [nG].ptLastHighlight [0].x ;
				nY = Game [nG].ptLastHighlight [0].y ;
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
			}
			if((Game [nG].ptLastHighlight [1].x >= 0) && (Game [nG].ptLastHighlight [1].y >= 0))
			{
				nX = Game [nG].ptLastHighlight [1].x ;
				nY = Game [nG].ptLastHighlight [1].y ;
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
			}

			if((Game [nG].ptHighlight [0].x >= 0) && (Game [nG].ptHighlight [0].y >= 0))
			{
				BOARD_DrawHighlight(nG, Game [nG].ptHighlight [0].x, Game [nG].ptHighlight [0].y, hdcWindow, PEN_HIGHLIGHT) ;
			}
			if((Game [nG].ptHighlight [1].x >= 0) && (Game [nG].ptHighlight [1].y >= 0))
			{
				BOARD_DrawHighlight(nG, Game [nG].ptHighlight [1].x, Game [nG].ptHighlight [1].y, hdcWindow, PEN_HIGHLIGHT) ;
			}

			Game [nG].ptLastHighlight [0] = Game [nG].ptHighlight [0] ;
			Game [nG].ptLastHighlight [1] = Game [nG].ptHighlight [1] ;
		}
	}

	// draw last king legal square highlight
	if(User.bShowKingHighlight)
	{
		for(nI = 0 ; nI < 8 ; nI++)
		{
			if((Game [nG].ptKing [INDEX_WHITE] [nI].x != Game [nG].ptLastKing [INDEX_WHITE] [nI].x) ||
					(Game [nG].ptKing [INDEX_WHITE] [nI].y != Game [nG].ptLastKing [INDEX_WHITE] [nI].y))
			{
				if((Game [nG].ptLastKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_WHITE] [nI].y >= 0))
				{
					nX = Game [nG].ptLastKing [INDEX_WHITE] [nI].x ;
					nY = Game [nG].ptLastKing [INDEX_WHITE] [nI].y ;
					BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
				}

				if((Game [nG].ptKing [INDEX_WHITE] [nI].x >= 0) && (Game [nG].ptKing [INDEX_WHITE] [nI].y >= 0))
				{
					BOARD_DrawHighlight(nG, Game [nG].ptKing [INDEX_WHITE] [nI].x, Game [nG].ptKing [INDEX_WHITE] [nI].y, hdcWindow, PEN_KING_HIGHLIGHT) ;
				}

				Game [nG].ptLastKing [INDEX_WHITE] [nI] = Game [nG].ptKing [INDEX_WHITE] [nI] ;
			}

			if((Game [nG].ptKing [INDEX_BLACK] [nI].x != Game [nG].ptLastKing [INDEX_BLACK] [nI].x) ||
					(Game [nG].ptKing [INDEX_BLACK] [nI].y != Game [nG].ptLastKing [INDEX_BLACK] [nI].y))
			{
				if((Game [nG].ptLastKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptLastKing [INDEX_BLACK] [nI].y >= 0))
				{
					nX = Game [nG].ptLastKing [INDEX_BLACK] [nI].x ;
					nY = Game [nG].ptLastKing [INDEX_BLACK] [nI].y ;
					BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
				}

				if((Game [nG].ptKing [INDEX_BLACK] [nI].x >= 0) && (Game [nG].ptKing [INDEX_BLACK] [nI].y >= 0))
				{
					BOARD_DrawHighlight(nG, Game [nG].ptKing [INDEX_BLACK] [nI].x, Game [nG].ptKing [INDEX_BLACK] [nI].y, hdcWindow, PEN_KING_HIGHLIGHT) ;
				}

				Game [nG].ptLastKing [INDEX_BLACK] [nI] = Game [nG].ptKing [INDEX_BLACK] [nI] ;
			}
		}
	}

	// put the dragged piece back into place and draw it
	if(bDragPiece)
	{
		if(User.nMoveType == DRAG_MOVE)
		{
			if(DragInfo.nPc == EMPTY_SQUARE)
			{

				// lost the dragged piece
				// if it's in the pawn promotion window then close the promotion window
				if(System.bInPromote)
				{
					if(IsWindow(System.hwndPromote))
					{
						SendMessage(System.hwndPromote, WM_CLOSE, 0, 0) ;
					}
				}
			}
			else
			{

				// put drag piece back
				if(DragInfo.bFromBoard)
				{
					Game [nG].nBoard [DragInfo.ptFrom.x] [DragInfo.ptFrom.y] = DragInfo.nPc ;
				}
				else
				{
					Game [nG].nBuffer [DragInfo.nPc] = Game [nG].nBuffer [DragInfo.nPc] + 1 ;
				}

				// set draw drag piece save background to true
				bDrag = 1 ;

				// create memory DC
				hdcSave   = CreateCompatibleDC(hdc) ;
				htOldSave = (HBITMAP) SelectObject(hdcSave, Game [nG].hSave) ;

				// copy background
				BitBlt(hdcSave, 0, 0, Game [nG].nss, Game [nG].nss, hdcWindow, DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, SRCCOPY) ;

				// draw drag piece
				BOARD_DrawDragPiece(nG, hdcWindow) ;
			}
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;

	// set clipping on target DC
	if(nClips > 0)
	{
		SelectClipRgn(hdc, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdc, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// copy the new bitmap onto the screen in one go to avoid any flickering
	if(TOOLBOX_ShowBuffer(nG))
	{
		switch(User.nBufferOrientation)
		{
			case DEFAULT_BUFFER_LEFT :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, nH, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_RIGHT :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_TOPBOTTOML :
			case DEFAULT_BUFFER_TOPBOTTOMR :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, (Game [nG].rBuffer1.bottom - Game [nG].rBuffer.top) + 1, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			default :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;
		}
	}
	else
	{
		BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
	}

	// check to see if drag piece save background is true
	if(bDrag)
	{
		// restore drag piece saved background
		BitBlt(hdcWindow, DragInfo.ptCurrent.x, DragInfo.ptCurrent.y, Game [nG].nss, Game [nG].nss, hdcSave, 0, 0, SRCCOPY) ;

		// clean up
		SelectObject(hdcSave, htOldSave) ;
		DeleteDC(hdcSave) ;
	}

	// massive clean up
	if(nClips > 0)
	{
		for(nI = 0 ; nI < nClips ; nI++)
		{
			DeleteObject(Clips [nI]) ;
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;
}

void BOARD_DrawAnimateBoard(int nG, HWND hwnd, HDC hdc)
{
	int nY, nX, nI, nPx, nPy ;

	HDC hdcWindow, hdcSave ;
	HBITMAP htOldWindow, htOldSave ;
	RECT rc ;

	int nW, nH, nW1 ;

	if(nG != INDEX_PLAY)
	{
		BOARD_DrawAnimateBoard1(nG, hwnd, hdc) ;
		return ;
	}

	// make sure the window is not minimized
	if(IsIconic(hwnd))
	{
		return ;
	}

	// get client rect
	GetClientRect(hwnd, &rc) ;

	// assign board and buffer sizes
	nW  = (Game [nG].rBoard.right  - Game [nG].rBoard.left) + 1 ;
	nH  = (Game [nG].rBoard.bottom - Game [nG].rBoard.top) + 1 ;
	nW1 = (Game [nG].rBuffer.right - Game [nG].rBuffer.left) + 1 ;

	// initialize clipping
	nClips = 0 ;

	// animate piece clipping
	if(AnimateInfo.bFromBoard)
	{
		// original square from board
		BOARD_SquareToPosition(nG, AnimateInfo.ptFrom.x, AnimateInfo.ptFrom.y, &nPx, &nPy) ;
		Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

		// current position
		Clips [nClips++] = CreateRectRgn(AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, AnimateInfo.ptCurrent.x + Game [nG].nss, AnimateInfo.ptCurrent.y + Game [nG].nss) ;

		// last position
		if((AnimateInfo.ptLast.x >= 0) && (AnimateInfo.ptLast.y >= 0))
		{
			Clips [nClips++] = CreateRectRgn(AnimateInfo.ptLast.x, AnimateInfo.ptLast.y, AnimateInfo.ptLast.x + Game [nG].nss, AnimateInfo.ptLast.y + Game [nG].nss) ;
		}

		// temporary remove the dragging board piece from original position
		Game [nG].nBoard [AnimateInfo.ptFrom.x] [AnimateInfo.ptFrom.y] = EMPTY_SQUARE ;
	}
	else
	{
		// original square from buffer
		nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [AnimateInfo.nPc].x ;
		nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [AnimateInfo.nPc].y ;
		Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

		// current position
		Clips [nClips++] = CreateRectRgn(AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, AnimateInfo.ptCurrent.x + Game [nG].nss, AnimateInfo.ptCurrent.y + Game [nG].nss) ;

		// last position
		if((AnimateInfo.ptLast.x >= 0) && (AnimateInfo.ptLast.y >= 0))
		{
			Clips [nClips++] = CreateRectRgn(AnimateInfo.ptLast.x, AnimateInfo.ptLast.y, AnimateInfo.ptLast.x + Game [nG].nss, AnimateInfo.ptLast.y + Game [nG].nss) ;
		}

		// temporary reduce the dragging buffer piece count by one
		if(Game [nG].nBuffer [AnimateInfo.nPc] > 0)
		{
			Game [nG].nBuffer [AnimateInfo.nPc] = Game [nG].nBuffer [AnimateInfo.nPc] - 1 ;
		}
		else
		{
			Game [nG].nLastBuffer [AnimateInfo.nPc] = -2 ;
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// set clipping on memory DC
	if(nClips > 0)
	{
		SelectClipRgn(hdcWindow, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdcWindow, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// draw board
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			if(Game [nG].nBoard [nX] [nY] != Game [nG].nLastBoard [nX] [nY])
			{
				// draw board piece
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
			}
		}
	}

	// draw buffer
	if(Game [nG].bChessGame)
	{
		if(User.nShowCapturedChessPiece == 2)
		{
			if((Game [nG].nRelation ==  0) ||   //  0 i am observing a game being played
					(Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
					(Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawChessBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;
					}
				}
			}
			else
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;
					}
				}
			}
		}
		else
		{
			for(nI = 0 ; nI < MAX_BUFFER ; nI++)
			{
				if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
				{
					// draw buffer piece
					BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;
				}
			}
		}
	}
	else
	{
		for(nI = 0 ; nI < MAX_BUFFER ; nI++)
		{
			if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
			{
				// draw buffer piece
				BOARD_DrawBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;
			}
		}
	}

	// put the animate piece back into place and draw it
	if(AnimateInfo.bFromBoard)
	{
		Game [nG].nBoard [AnimateInfo.ptFrom.x] [AnimateInfo.ptFrom.y] = AnimateInfo.nPc ;
	}
	else
	{
		if(Game [nG].nBuffer [AnimateInfo.nPc] >= 0)
		{
			Game [nG].nBuffer [AnimateInfo.nPc] = Game [nG].nBuffer [AnimateInfo.nPc] + 1 ;
		}
	}

	// create memory DC
	hdcSave   = CreateCompatibleDC(hdc) ;
	htOldSave = (HBITMAP) SelectObject(hdcSave, Game [nG].hSave) ;

	// copy background
	BitBlt(hdcSave, 0, 0, Game [nG].nss, Game [nG].nss, hdcWindow, AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, SRCCOPY) ;

	// draw animate piece
	BOARD_DrawAnimatePiece(nG, hdcWindow) ;

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;

	// set clipping on target DC
	if(nClips > 0)
	{
		SelectClipRgn(hdc, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdc, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// copy the new bitmap onto the screen in one go to avoid any flickering
	if(TOOLBOX_ShowBuffer(nG))
	{
		switch(User.nBufferOrientation)
		{
			case DEFAULT_BUFFER_LEFT :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, nH, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_RIGHT :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_TOPBOTTOML :
			case DEFAULT_BUFFER_TOPBOTTOMR :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, (Game [nG].rBuffer1.bottom - Game [nG].rBuffer.top) + 1, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			default :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;
		}
	}
	else
	{
		BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
	}

	// restore animate piece saved background
	BitBlt(hdcWindow, AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, Game [nG].nss, Game [nG].nss, hdcSave, 0, 0, SRCCOPY) ;

	// clean up
	SelectObject(hdcSave, htOldSave) ;
	DeleteDC(hdcSave) ;

	// massive clean up
	if(nClips > 0)
	{
		for(nI = 0 ; nI < nClips ; nI++)
		{
			DeleteObject(Clips [nI]) ;
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;
}

void BOARD_DrawAnimateBoard1(int nG, HWND hwnd, HDC hdc)
{
	int nY, nX, nI, nPx, nPy ;

	HDC hdcWindow, hdcSave ;
	HBITMAP htOldWindow, htOldSave ;
	RECT rc ;

	int nW, nH, nW1 ;

	if(nG == INDEX_PLAY)
	{
		BOARD_DrawAnimateBoard(nG, hwnd, hdc) ;
		return ;
	}

	// make sure the window is not minimized
	if(IsIconic(hwnd))
	{
		return ;
	}

	// get client rect
	GetClientRect(hwnd, &rc) ;

	// assign board and buffer sizes
	nW  = (Game [nG].rBoard.right  - Game [nG].rBoard.left) + 1 ;
	nH  = (Game [nG].rBoard.bottom - Game [nG].rBoard.top) + 1 ;
	nW1 = (Game [nG].rBuffer.right - Game [nG].rBuffer.left) + 1 ;

	// initialize clipping
	nClips = 0 ;

	// animate piece clipping
	if(AnimateInfo.bFromBoard)
	{
		// original square from board
		BOARD_SquareToPosition(nG, AnimateInfo.ptFrom.x, AnimateInfo.ptFrom.y, &nPx, &nPy) ;
		Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

		// current position
		Clips [nClips++] = CreateRectRgn(AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, AnimateInfo.ptCurrent.x + Game [nG].nss, AnimateInfo.ptCurrent.y + Game [nG].nss) ;

		// last position
		if((AnimateInfo.ptLast.x >= 0) && (AnimateInfo.ptLast.y >= 0))
		{
			Clips [nClips++] = CreateRectRgn(AnimateInfo.ptLast.x, AnimateInfo.ptLast.y, AnimateInfo.ptLast.x + Game [nG].nss, AnimateInfo.ptLast.y + Game [nG].nss) ;
		}

		// temporary remove the dragging board piece from original position
		Game [nG].nBoard [AnimateInfo.ptFrom.x] [AnimateInfo.ptFrom.y] = EMPTY_SQUARE ;
	}
	else
	{
		// original square from buffer
		nPx = Game [nG].rBuffer.left + Game [nG].ptBuffer [AnimateInfo.nPc].x ;
		nPy = Game [nG].rBuffer.top  + Game [nG].ptBuffer [AnimateInfo.nPc].y ;
		Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nG].nss, nPy + Game [nG].nss) ;

		// current position
		Clips [nClips++] = CreateRectRgn(AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, AnimateInfo.ptCurrent.x + Game [nG].nss, AnimateInfo.ptCurrent.y + Game [nG].nss) ;

		// last position
		if((AnimateInfo.ptLast.x >= 0) && (AnimateInfo.ptLast.y >= 0))
		{
			Clips [nClips++] = CreateRectRgn(AnimateInfo.ptLast.x, AnimateInfo.ptLast.y, AnimateInfo.ptLast.x + Game [nG].nss, AnimateInfo.ptLast.y + Game [nG].nss) ;
		}

		// temporary reduce the dragging buffer piece count by one
		if(Game [nG].nBuffer [AnimateInfo.nPc] > 0)
		{
			Game [nG].nBuffer [AnimateInfo.nPc] = Game [nG].nBuffer [AnimateInfo.nPc] - 1 ;
		}
		else
		{
			Game [nG].nLastBuffer [AnimateInfo.nPc] = -2 ;
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// set clipping on memory DC
	if(nClips > 0)
	{
		SelectClipRgn(hdcWindow, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdcWindow, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// draw board
	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			if(Game [nG].nBoard [nX] [nY] != Game [nG].nLastBoard [nX] [nY])
			{
				// draw board piece
				BOARD_DrawBoardPiece(nG, hdcWindow, nX, nY, Game [nG].nBoard [nX] [nY]) ;
			}
		}
	}

	// draw buffer
	if(Game [nG].bChessGame)
	{
		if(User.nShowCapturedChessPiece == 2)
		{
			if((Game [nG].nRelation ==  0) ||   //  0 i am observing a game being played
					(Game [nG].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
					(Game [nG].nRelation ==  1))    //  1 i am playing and it is my move
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawChessBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;
					}
				}
			}
			else
			{
				for(nI = 0 ; nI < MAX_BUFFER ; nI++)
				{
					if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;
					}
				}
			}
		}
		else
		{
			for(nI = 0 ; nI < MAX_BUFFER ; nI++)
			{
				if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
				{
					// draw buffer piece
					BOARD_DrawEmptyBufferPiece(nG, hdcWindow, nI) ;
				}
			}
		}
	}
	else
	{
		for(nI = 0 ; nI < MAX_BUFFER ; nI++)
		{
			if(Game [nG].nBuffer [nI] != Game [nG].nLastBuffer [nI])
			{
				// draw buffer piece
				BOARD_DrawBufferPiece(nG, hdcWindow, nI, Game [nG].nBuffer [nI]) ;
			}
		}
	}

	// put the animate piece back into place and draw it
	if(AnimateInfo.bFromBoard)
	{
		Game [nG].nBoard [AnimateInfo.ptFrom.x] [AnimateInfo.ptFrom.y] = AnimateInfo.nPc ;
	}
	else
	{
		if(Game [nG].nBuffer [AnimateInfo.nPc] >= 0)
		{
			Game [nG].nBuffer [AnimateInfo.nPc] = Game [nG].nBuffer [AnimateInfo.nPc] + 1 ;
		}
	}

	// create memory DC
	hdcSave   = CreateCompatibleDC(hdc) ;
	htOldSave = (HBITMAP) SelectObject(hdcSave, Game [nG].hSave) ;

	// copy background
	BitBlt(hdcSave, 0, 0, Game [nG].nss, Game [nG].nss, hdcWindow, AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, SRCCOPY) ;

	// draw animate piece
	BOARD_DrawAnimatePiece(nG, hdcWindow) ;

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;

	// set clipping on target DC
	if(nClips > 0)
	{
		SelectClipRgn(hdc, Clips [0]) ;

		for(nI = 1 ; nI < nClips ; nI++)
		{
			if(ExtSelectClipRgn(hdc, Clips [nI], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nG].hWindow) ;

	// copy the new bitmap onto the screen in one go to avoid any flickering
	if(TOOLBOX_ShowBuffer(nG))
	{
		switch(User.nBufferOrientation)
		{
			case DEFAULT_BUFFER_LEFT :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, nH, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_RIGHT :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_TOPBOTTOML :
			case DEFAULT_BUFFER_TOPBOTTOMR :
				BitBlt(hdc, Game [nG].rBuffer.left, Game [nG].rBuffer.top, nW + nW1, (Game [nG].rBuffer1.bottom - Game [nG].rBuffer.top) + 1, hdcWindow, Game [nG].rBuffer.left, Game [nG].rBuffer.top, SRCCOPY) ;
				break ;

			default :
				BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW + nW1, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
				break ;
		}
	}
	else
	{
		BitBlt(hdc, Game [nG].rBoard.left, Game [nG].rBoard.top, nW, nH, hdcWindow, Game [nG].rBoard.left, Game [nG].rBoard.top, SRCCOPY) ;
	}

	// restore animate piece saved background
	BitBlt(hdcWindow, AnimateInfo.ptCurrent.x, AnimateInfo.ptCurrent.y, Game [nG].nss, Game [nG].nss, hdcSave, 0, 0, SRCCOPY) ;

	// clean up
	SelectObject(hdcSave, htOldSave) ;
	DeleteDC(hdcSave) ;

	// massive clean up
	if(nClips > 0)
	{
		for(nI = 0 ; nI < nClips ; nI++)
		{
			DeleteObject(Clips [nI]) ;
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;
}

void BOARD_DrawMoveButtonBoard(int nI, int nJ, int nC, HWND hwnd, HDC hdc)
{
	int nY, nX, nPx, nPy, nPc, nK ;

	HDC hdcWindow ;
	HBITMAP htOldWindow ;
	RECT rc ;

	int nW, nH, nW1, nOT ;

	if(nI != INDEX_PLAY)
	{
		BOARD_DrawMoveButtonBoard1(nI, nJ, nC, hwnd, hdc) ;
		return ;
	}

	// make sure the window is not minimized
	if(IsIconic(hwnd))
	{
		return ;
	}

	// get client rect
	GetClientRect(hwnd, &rc) ;

	// assign board and buffer sizes
	nW  = (Game [nI].rBoard.right  - Game [nI].rBoard.left) + 1 ;
	nH  = (Game [nI].rBoard.bottom - Game [nI].rBoard.top) + 1 ;
	nW1 = (Game [nI].rBuffer.right - Game [nI].rBuffer.left) + 1 ;

	// process player
	if(nI == INDEX_PLAY)
	{
		if(User.bTruePremove)
		{
			if(User.bShowTPHighlight)
			{
				// erase true premove sent highlight
				if(Premove.bLastTP)
				{
					if((Premove.ptLastTP [0].x >= 0) && (Premove.ptLastTP [0].y >= 0))
					{
						Game [nI].nLastBoard [Premove.ptLastTP [0].x] [Premove.ptLastTP [0].y] = -1 ;
					}
					if((Premove.ptLastTP [1].x >= 0) && (Premove.ptLastTP [1].y >= 0))
					{
						Game [nI].nLastBoard [Premove.ptLastTP [1].x] [Premove.ptLastTP [1].y] = -1 ;
					}
				}

				// erase true premove
				nOT = Premove.nPremoveTail ;
				for(nX = 0 ; nX < Premove.nPremoveCount ; nX++)
				{
					if(Premove.nPremoveLegalBuffer [nOT] [3] >= 0)
					{
						Game [nI].nLastBoard [Premove.nPremoveLegalBuffer [nOT] [3]] [Premove.nPremoveLegalBuffer [nOT] [4]] = -1 ;
					}
					if(Premove.nPremoveLegalBuffer [nOT] [5] >= 0)
					{
						Game [nI].nLastBoard [Premove.nPremoveLegalBuffer [nOT] [5]] [Premove.nPremoveLegalBuffer [nOT] [6]] = -1 ;
					}

					nOT = nOT + 1 ;
					if(nOT >= MAX_TRUE_PREMOVE)
					{
						nOT = 0 ;
					}
				}
			}

			// erase illegal true premove
			if(Premove.bIllegalTP)
			{
				if((Premove.ptIllegalTP [0].x >= 0) && (Premove.ptIllegalTP [0].y >= 0))
				{
					Game [nI].nLastBoard [Premove.ptIllegalTP [0].x] [Premove.ptIllegalTP [0].y] = -1 ;
				}
				if((Premove.ptIllegalTP [1].x >= 0) && (Premove.ptIllegalTP [1].y >= 0))
				{
					Game [nI].nLastBoard [Premove.ptIllegalTP [1].x] [Premove.ptIllegalTP [1].y] = -1 ;
				}
			}
		}
	}

	// erase last move highlight
	if(User.bShowHighlight)
	{
		if((Game [nI].ptLastHighlight [0].x >= 0) && (Game [nI].ptLastHighlight [0].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptLastHighlight [0].x] [Game [nI].ptLastHighlight [0].y] = -1 ;
		}
		if((Game [nI].ptLastHighlight [1].x >= 0) && (Game [nI].ptLastHighlight [1].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptLastHighlight [1].x] [Game [nI].ptLastHighlight [1].y] = -1 ;
		}

		if((Game [nI].ptHighlight [0].x >= 0) && (Game [nI].ptHighlight [0].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptHighlight [0].x] [Game [nI].ptHighlight [0].y] = -1 ;
		}
		if((Game [nI].ptHighlight [1].x >= 0) && (Game [nI].ptHighlight [1].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptHighlight [1].x] [Game [nI].ptHighlight [1].y] = -1 ;
		}

		if(User.bShowKingHighlight)
		{
			for(nK = 0 ; nK < 8 ; nK++)
			{
				if((Game [nI].ptLastKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_WHITE] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_WHITE] [nK].x] [Game [nI].ptLastKing [INDEX_WHITE] [nK].y] = -1 ;
				}
				if((Game [nI].ptLastKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_BLACK] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_BLACK] [nK].x] [Game [nI].ptLastKing [INDEX_BLACK] [nK].y] = -1 ;
				}

				if((Game [nI].ptKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptKing [INDEX_WHITE] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptKing [INDEX_WHITE] [nK].x] [Game [nI].ptKing [INDEX_WHITE] [nK].y] = -1 ;
				}
				if((Game [nI].ptKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptKing [INDEX_BLACK] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptKing [INDEX_BLACK] [nK].x] [Game [nI].ptKing [INDEX_BLACK] [nK].y] = -1 ;
				}
			}
		}

		BOARD_NullLastHighlight(nI) ;
	}
	else if(User.bShowKingHighlight)
	{
		for(nK = 0 ; nK < 8 ; nK++)
		{
			if((Game [nI].ptLastKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_WHITE] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_WHITE] [nK].x] [Game [nI].ptLastKing [INDEX_WHITE] [nK].y] = -1 ;
			}
			if((Game [nI].ptLastKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_BLACK] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_BLACK] [nK].x] [Game [nI].ptLastKing [INDEX_BLACK] [nK].y] = -1 ;
			}

			if((Game [nI].ptKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptKing [INDEX_WHITE] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptKing [INDEX_WHITE] [nK].x] [Game [nI].ptKing [INDEX_WHITE] [nK].y] = -1 ;
			}
			if((Game [nI].ptKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptKing [INDEX_BLACK] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptKing [INDEX_BLACK] [nK].x] [Game [nI].ptKing [INDEX_BLACK] [nK].y] = -1 ;
			}
		}

		BOARD_NullLastHighlight(nI) ;
	}

	// initialize clipping
	nClips = 0 ;

	if(nJ == -1)
	{
		// board clipping
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].nInitBoard [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					BOARD_SquareToPosition(nI, nX, nY, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
				}
			}
		}

		// buffer clipping
		for(nX = 0 ; nX < MAX_BUFFER ; nX++)
		{
			if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
			{
				if(Game [nI].bChessGame)
				{
					if(User.nShowCapturedChessPiece == 2)
					{
						if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
								(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
								(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
						{
							nPc = ReverseChessPiece [nX] ;
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nPc].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nPc].y ;
						}
						else
						{
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
						}
					}
					else
					{
						nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
						nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
					}
				}
				else
				{
					nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
					nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
				}
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
			}
		}
	}
	else
	{
		// board clipping
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].Position [nJ].nBoard [nC] [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					BOARD_SquareToPosition(nI, nX, nY, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
				}
			}
		}

		// buffer clipping
		for(nX = 0 ; nX < MAX_BUFFER ; nX++)
		{
			if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
			{
				if(Game [nI].bChessGame)
				{
					if(User.nShowCapturedChessPiece == 2)
					{
						if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
								(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
								(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
						{
							nPc = ReverseChessPiece [nX] ;
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nPc].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nPc].y ;
						}
						else
						{
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
						}
					}
					else
					{
						nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
						nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
					}
				}
				else
				{
					nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
					nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
				}
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nI].hWindow) ;

	// set clipping on memory DC
	if(nClips > 0)
	{
		SelectClipRgn(hdcWindow, Clips [0]) ;

		for(nX = 1 ; nX < nClips ; nX++)
		{
			if(ExtSelectClipRgn(hdcWindow, Clips [nX], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	if(nJ == -1)
	{
		// draw board
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].nInitBoard [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					// draw board piece
					BOARD_DrawBoardPiece(nI, hdcWindow, nX, nY, Game [nI].nInitBoard [nX] [nY]) ;

					Game [nI].nLastBoard [nX] [nY] = Game [nI].nInitBoard [nX] [nY] ;
				}
			}
		}

		// draw buffer
		if(Game [nI].bChessGame)
		{
			if(User.nShowCapturedChessPiece == 2)
			{
				if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
						(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
						(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawChessBufferPiece(nI, hdcWindow, nX, Game [nI].nInitBuffer [nX]) ;

							Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
						}
					}
				}
				else
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

							Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
						}
					}
				}
			}
			else
			{
				for(nX = 0 ; nX < MAX_BUFFER ; nX++)
				{
					if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

						Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
					}
				}
			}
		}
		else
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
				{
					// draw buffer piece
					BOARD_DrawBufferPiece(nI, hdcWindow, nX, Game [nI].nInitBuffer [nX]) ;

					Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
				}
			}
		}
	}
	else
	{
		// draw board
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].Position [nJ].nBoard [nC] [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					// draw board piece
					BOARD_DrawBoardPiece(nI, hdcWindow, nX, nY, Game [nI].Position [nJ].nBoard [nC] [nX] [nY]) ;

					Game [nI].nLastBoard [nX] [nY] = Game [nI].Position [nJ].nBoard [nC] [nX] [nY] ;
				}
			}
		}

		// draw buffer
		if(Game [nI].bChessGame)
		{
			if(User.nShowCapturedChessPiece == 2)
			{
				if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
						(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
						(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawChessBufferPiece(nI, hdcWindow, nX, Game [nI].Position [nJ].nBuffer [nC] [nX]) ;

							Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
						}
					}
				}
				else
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

							Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
						}
					}
				}
			}
			else
			{
				for(nX = 0 ; nX < MAX_BUFFER ; nX++)
				{
					if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

						Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
					}
				}
			}
		}
		else
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
				{
					// draw buffer piece
					BOARD_DrawBufferPiece(nI, hdcWindow, nX, Game [nI].Position [nJ].nBuffer [nC] [nX]) ;

					Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
				}
			}
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;

	// set clipping on target DC
	if(nClips > 0)
	{
		SelectClipRgn(hdc, Clips [0]) ;

		for(nX = 1 ; nX < nClips ; nX++)
		{
			if(ExtSelectClipRgn(hdc, Clips [nX], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nI].hWindow) ;

	// copy the new bitmap onto the screen in one go to avoid any flickering
	if(TOOLBOX_ShowBuffer(nI))
	{
		switch(User.nBufferOrientation)
		{
			case DEFAULT_BUFFER_LEFT :
				BitBlt(hdc, Game [nI].rBuffer.left, Game [nI].rBuffer.top, nW + nW1, nH, hdcWindow, Game [nI].rBuffer.left, Game [nI].rBuffer.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_RIGHT :
				BitBlt(hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW + nW1, nH, hdcWindow, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_TOPBOTTOML :
			case DEFAULT_BUFFER_TOPBOTTOMR :
				BitBlt(hdc, Game [nI].rBuffer.left, Game [nI].rBuffer.top, nW + nW1, (Game [nI].rBuffer1.bottom - Game [nI].rBuffer.top) + 1, hdcWindow, Game [nI].rBuffer.left, Game [nI].rBuffer.top, SRCCOPY) ;
				break ;

			default :
				BitBlt(hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW + nW1, nH, hdcWindow, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
				break ;
		}
	}
	else
	{
		BitBlt(hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW, nH, hdcWindow, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
	}

	// massive clean up
	if(nClips > 0)
	{
		for(nX = 0 ; nX < nClips ; nX++)
		{
			DeleteObject(Clips [nX]) ;
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;
}

void BOARD_DrawMoveButtonBoard1(int nI, int nJ, int nC, HWND hwnd, HDC hdc)
{
	int nY, nX, nPx, nPy, nPc, nK ;

	HDC hdcWindow ;
	HBITMAP htOldWindow ;
	RECT rc ;

	int nW, nH, nW1 ;

	if(nI == INDEX_PLAY)
	{
		BOARD_DrawMoveButtonBoard(nI, nJ, nC, hwnd, hdc) ;
		return ;
	}

	// make sure the window is not minimized
	if(IsIconic(hwnd))
	{
		return ;
	}

	// get client rect
	GetClientRect(hwnd, &rc) ;

	// assign board and buffer sizes
	nW  = (Game [nI].rBoard.right  - Game [nI].rBoard.left) + 1 ;
	nH  = (Game [nI].rBoard.bottom - Game [nI].rBoard.top) + 1 ;
	nW1 = (Game [nI].rBuffer.right - Game [nI].rBuffer.left) + 1 ;

	// erase last move highlight
	if(User.bShowHighlight)
	{
		if((Game [nI].ptLastHighlight [0].x >= 0) && (Game [nI].ptLastHighlight [0].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptLastHighlight [0].x] [Game [nI].ptLastHighlight [0].y] = -1 ;
		}
		if((Game [nI].ptLastHighlight [1].x >= 0) && (Game [nI].ptLastHighlight [1].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptLastHighlight [1].x] [Game [nI].ptLastHighlight [1].y] = -1 ;
		}

		if((Game [nI].ptHighlight [0].x >= 0) && (Game [nI].ptHighlight [0].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptHighlight [0].x] [Game [nI].ptHighlight [0].y] = -1 ;
		}
		if((Game [nI].ptHighlight [1].x >= 0) && (Game [nI].ptHighlight [1].y >= 0))
		{
			Game [nI].nLastBoard [Game [nI].ptHighlight [1].x] [Game [nI].ptHighlight [1].y] = -1 ;
		}

		if(User.bShowKingHighlight)
		{
			for(nK = 0 ; nK < 8 ; nK++)
			{
				if((Game [nI].ptLastKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_WHITE] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_WHITE] [nK].x] [Game [nI].ptLastKing [INDEX_WHITE] [nK].y] = -1 ;
				}
				if((Game [nI].ptLastKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_BLACK] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_BLACK] [nK].x] [Game [nI].ptLastKing [INDEX_BLACK] [nK].y] = -1 ;
				}

				if((Game [nI].ptKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptKing [INDEX_WHITE] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptKing [INDEX_WHITE] [nK].x] [Game [nI].ptKing [INDEX_WHITE] [nK].y] = -1 ;
				}
				if((Game [nI].ptKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptKing [INDEX_BLACK] [nK].y >= 0))
				{
					Game [nI].nLastBoard [Game [nI].ptKing [INDEX_BLACK] [nK].x] [Game [nI].ptKing [INDEX_BLACK] [nK].y] = -1 ;
				}
			}
		}

		BOARD_NullLastHighlight(nI) ;
	}
	else if(User.bShowKingHighlight)
	{
		for(nK = 0 ; nK < 8 ; nK++)
		{
			if((Game [nI].ptLastKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_WHITE] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_WHITE] [nK].x] [Game [nI].ptLastKing [INDEX_WHITE] [nK].y] = -1 ;
			}
			if((Game [nI].ptLastKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptLastKing [INDEX_BLACK] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptLastKing [INDEX_BLACK] [nK].x] [Game [nI].ptLastKing [INDEX_BLACK] [nK].y] = -1 ;
			}

			if((Game [nI].ptKing [INDEX_WHITE] [nK].x >= 0) && (Game [nI].ptKing [INDEX_WHITE] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptKing [INDEX_WHITE] [nK].x] [Game [nI].ptKing [INDEX_WHITE] [nK].y] = -1 ;
			}
			if((Game [nI].ptKing [INDEX_BLACK] [nK].x >= 0) && (Game [nI].ptKing [INDEX_BLACK] [nK].y >= 0))
			{
				Game [nI].nLastBoard [Game [nI].ptKing [INDEX_BLACK] [nK].x] [Game [nI].ptKing [INDEX_BLACK] [nK].y] = -1 ;
			}
		}

		BOARD_NullLastHighlight(nI) ;
	}

	// initialize clipping
	nClips = 0 ;

	if(nJ == -1)
	{
		// board clipping
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].nInitBoard [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					BOARD_SquareToPosition(nI, nX, nY, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
				}
			}
		}

		// buffer clipping
		for(nX = 0 ; nX < MAX_BUFFER ; nX++)
		{
			if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
			{
				if(Game [nI].bChessGame)
				{
					if(User.nShowCapturedChessPiece == 2)
					{
						if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
								(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
								(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
						{
							nPc = ReverseChessPiece [nX] ;
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nPc].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nPc].y ;
						}
						else
						{
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
						}
					}
					else
					{
						nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
						nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
					}
				}
				else
				{
					nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
					nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
				}
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
			}
		}
	}
	else
	{
		// board clipping
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].Position [nJ].nBoard [nC] [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					BOARD_SquareToPosition(nI, nX, nY, &nPx, &nPy) ;
					Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
				}
			}
		}

		// buffer clipping
		for(nX = 0 ; nX < MAX_BUFFER ; nX++)
		{
			if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
			{
				if(Game [nI].bChessGame)
				{
					if(User.nShowCapturedChessPiece == 2)
					{
						if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
								(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
								(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
						{
							nPc = ReverseChessPiece [nX] ;
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nPc].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nPc].y ;
						}
						else
						{
							nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
							nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
						}
					}
					else
					{
						nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
						nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
					}
				}
				else
				{
					nPx = Game [nI].rBuffer.left + Game [nI].ptBuffer [nX].x ;
					nPy = Game [nI].rBuffer.top  + Game [nI].ptBuffer [nX].y ;
				}
				Clips [nClips++] = CreateRectRgn(nPx, nPy, nPx + Game [nI].nss, nPy + Game [nI].nss) ;
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nI].hWindow) ;

	// set clipping on memory DC
	if(nClips > 0)
	{
		SelectClipRgn(hdcWindow, Clips [0]) ;

		for(nX = 1 ; nX < nClips ; nX++)
		{
			if(ExtSelectClipRgn(hdcWindow, Clips [nX], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	if(nJ == -1)
	{
		// draw board
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].nInitBoard [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					// draw board piece
					BOARD_DrawBoardPiece(nI, hdcWindow, nX, nY, Game [nI].nInitBoard [nX] [nY]) ;

					Game [nI].nLastBoard [nX] [nY] = Game [nI].nInitBoard [nX] [nY] ;
				}
			}
		}

		// draw buffer
		if(Game [nI].bChessGame)
		{
			if(User.nShowCapturedChessPiece == 2)
			{
				if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
						(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
						(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawChessBufferPiece(nI, hdcWindow, nX, Game [nI].nInitBuffer [nX]) ;

							Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
						}
					}
				}
				else
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

							Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
						}
					}
				}
			}
			else
			{
				for(nX = 0 ; nX < MAX_BUFFER ; nX++)
				{
					if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

						Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
					}
				}
			}
		}
		else
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				if(Game [nI].nInitBuffer [nX] != Game [nI].nLastBuffer [nX])
				{
					// draw buffer piece
					BOARD_DrawBufferPiece(nI, hdcWindow, nX, Game [nI].nInitBuffer [nX]) ;

					Game [nI].nLastBuffer [nX] = Game [nI].nInitBuffer [nX] ;
				}
			}
		}
	}
	else
	{
		// draw board
		for(nY = 0 ; nY < 8 ; nY++)
		{
			for(nX = 0 ; nX < 8 ; nX++)
			{
				if(Game [nI].Position [nJ].nBoard [nC] [nX] [nY] != Game [nI].nLastBoard [nX] [nY])
				{
					// draw board piece
					BOARD_DrawBoardPiece(nI, hdcWindow, nX, nY, Game [nI].Position [nJ].nBoard [nC] [nX] [nY]) ;

					Game [nI].nLastBoard [nX] [nY] = Game [nI].Position [nJ].nBoard [nC] [nX] [nY] ;
				}
			}
		}

		// draw buffer
		if(Game [nI].bChessGame)
		{
			if(User.nShowCapturedChessPiece == 2)
			{
				if((Game [nI].nRelation ==  0) ||   //  0 i am observing a game being played
						(Game [nI].nRelation == -1) ||  // -1 i am playing, it is my opponent's move
						(Game [nI].nRelation ==  1))    //  1 i am playing and it is my move
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawChessBufferPiece(nI, hdcWindow, nX, Game [nI].Position [nJ].nBuffer [nC] [nX]) ;

							Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
						}
					}
				}
				else
				{
					for(nX = 0 ; nX < MAX_BUFFER ; nX++)
					{
						if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
						{
							// draw buffer piece
							BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

							Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
						}
					}
				}
			}
			else
			{
				for(nX = 0 ; nX < MAX_BUFFER ; nX++)
				{
					if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
					{
						// draw buffer piece
						BOARD_DrawEmptyBufferPiece(nI, hdcWindow, nX) ;

						Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
					}
				}
			}
		}
		else
		{
			for(nX = 0 ; nX < MAX_BUFFER ; nX++)
			{
				if(Game [nI].Position [nJ].nBuffer [nC] [nX] != Game [nI].nLastBuffer [nX])
				{
					// draw buffer piece
					BOARD_DrawBufferPiece(nI, hdcWindow, nX, Game [nI].Position [nJ].nBuffer [nC] [nX]) ;

					Game [nI].nLastBuffer [nX] = Game [nI].Position [nJ].nBuffer [nC] [nX] ;
				}
			}
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;

	// set clipping on target DC
	if(nClips > 0)
	{
		SelectClipRgn(hdc, Clips [0]) ;

		for(nX = 1 ; nX < nClips ; nX++)
		{
			if(ExtSelectClipRgn(hdc, Clips [nX], RGN_OR) == ERROR)
			{
				abort() ;   // this should never ever happen!
			}
		}
	}

	// create memory DC
	hdcWindow   = CreateCompatibleDC(hdc) ;
	htOldWindow = (HBITMAP) SelectObject(hdcWindow, Game [nI].hWindow) ;

	// copy the new bitmap onto the screen in one go to avoid any flickering
	if(TOOLBOX_ShowBuffer(nI))
	{
		switch(User.nBufferOrientation)
		{
			case DEFAULT_BUFFER_LEFT :
				BitBlt(hdc, Game [nI].rBuffer.left, Game [nI].rBuffer.top, nW + nW1, nH, hdcWindow, Game [nI].rBuffer.left, Game [nI].rBuffer.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_RIGHT :
				BitBlt(hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW + nW1, nH, hdcWindow, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
				break ;

			case DEFAULT_BUFFER_TOPBOTTOML :
			case DEFAULT_BUFFER_TOPBOTTOMR :
				BitBlt(hdc, Game [nI].rBuffer.left, Game [nI].rBuffer.top, nW + nW1, (Game [nI].rBuffer1.bottom - Game [nI].rBuffer.top) + 1, hdcWindow, Game [nI].rBuffer.left, Game [nI].rBuffer.top, SRCCOPY) ;
				break ;

			default :
				BitBlt(hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW + nW1, nH, hdcWindow, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
				break ;
		}
	}
	else
	{
		BitBlt(hdc, Game [nI].rBoard.left, Game [nI].rBoard.top, nW, nH, hdcWindow, Game [nI].rBoard.left, Game [nI].rBoard.top, SRCCOPY) ;
	}

	// massive clean up
	if(nClips > 0)
	{
		for(nX = 0 ; nX < nClips ; nX++)
		{
			DeleteObject(Clips [nX]) ;
		}
	}

	// clean up
	SelectObject(hdcWindow, htOldWindow) ;
	DeleteDC(hdcWindow) ;
}
