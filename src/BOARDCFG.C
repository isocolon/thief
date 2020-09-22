#include "thief.h"

//#define BD_LEFT             210
//#define BD_TOP              8
//#define BD_RIGHT            460
//#define BD_BOTTOM           440

#define BD_LEFT             173
#define BD_TOP              8
#define BD_RIGHT            423
#define BD_BOTTOM           440

#define BD_NUM_LINES        40

#define BD_MAX_FONT         7
#define BD_FONT_HANDLE      0
#define BD_FONT_CLOCK       1
#define BD_FONT_GAMETYPE    2
#define BD_FONT_LASTMOVE    3
#define BD_FONT_LAGSTAT     4
#define BD_FONT_RESULT      5
#define BD_FONT_COORDINATES 6

#define BD_GAME_WINDOW          0
#define BD_BOARD_BORDER         1
#define BD_BUFFER_BORDER        2
#define BD_BUFFER               3
#define BD_WHITE_SQUARE_BORDER  4
#define BD_WHITE_SQUARE         5
#define BD_BLACK_SQUARE_BORDER  6
#define BD_BLACK_SQUARE         7
#define BD_PIECE_NUMBER         8
#define BD_WHITE_PIECE_BORDER   9
#define BD_WHITE_PIECE          10
#define BD_BLACK_PIECE_BORDER   11
#define BD_BLACK_PIECE          12
#define BD_HANDLE_FONT          13
#define BD_HANDLE_RATING        14
#define BD_CLOCK_FONT           15
#define BD_CLOCK_TURN           16
#define BD_CLOCK_TURN_BACK      17
#define BD_CLOCK_WAIT           18
#define BD_LAG_STAT_FONT        19
#define BD_LAG_STAT             20
#define BD_GAME_TYPE_FONT       21
#define BD_GAME_TYPE            22
#define BD_LAST_MOVE_FONT       23
#define BD_LAST_MOVE            24
#define BD_LAST_MOVE_SQUARE     25
#define BD_TRUE_PREMOVE_SQUARE  26
#define BD_ILLEGAL_PREMOVE      27
#define BD_KING_HIGHLIGHT       28
#define BD_RESULT_FONT          29
#define BD_RESULT_TEXT          30
#define BD_RESULT_BACK          31
#define BD_PTELL_TEXT           32
#define BD_PTELL_BACK           33
#define BD_GINFO_TEXT           34
#define BD_GINFO_BACK           35
#define BD_COORD_FONT           36
#define BD_COORD_TEXT           37
#define BD_CLOCK_TIMEDOWN		38
#define BD_CLOCK_TIMEUP			39

char *BdStr [BD_NUM_LINES] = { "Game Window",
							   "Board Border",
							   "Buffer Border",
							   "Buffer",
							   "White Square Border",
							   "White Square",
							   "Black Square Border",
							   "Black Square",
							   "Piece Number",
							   "White Piece Border",
							   "White Piece",
							   "Black Piece Border",
							   "Black Piece",
							   "Handle Font",
							   "Handle (Rating)",
							   "Clock Font",
							   "Clock Turn",
							   "Clock Turn Background",
							   "Clock Wait",
							   "Lag Stat Font",
							   "Lag Stat",
							   "Game Type Font",
							   "Game Type",
							   "Last Move Font",
							   "Last Move",
							   "Last Move Square",
							   "True Premove Square",
							   "Illegal Premove Square",
							   "Legal King Square",
							   "Result Font",
							   "Result Text",
							   "Result Background",
							   "Ptell Text",
							   "Ptell Background",
							   "Game Info Text",
							   "Game Info Background",
							   "Coordinates Font",
							   "Coordinates Text",
							   "Team Down (Bughouse)",
							   "Team Up (Bughouse)"
							 } ;

FONTRECORD BdFont [BD_MAX_FONT] ;
COLORREF   BdClr  [BD_NUM_LINES] ;

POINT _BdptNumber [3] = { {2, 1}, {4, 5}, {6, 9} } ;

#define BD_DEFAULT_SQUARE   19
#define BD_DEFAULT_SQUARE1  0
#define BD_NUM_SQUARES      27
#define BD_SQ_KNIGHTS_FROM  3
#define BD_SQ_KNIGHTS_TO    13
char *BdSq [BD_NUM_SQUARES] = { "<Solid Color>",
								"Canvas 1",
								"Canvas 2",
								"Knights Brushed Liquid",
								"Knights Cold Marble",
								"Knights Gray Tiles",
								"Knights Green Marble",
								"Knights Pale Wood",
								"Knights Provence",
								"Knights Red & Tan",
								"Knights Tan & Gray",
								"Knights Winter",
								"Knights XBoard",
								"Knights Yellow & Blue",
								"Marble 1",
								"Marble 2",
								"Paper 1",
								"Paper 2",
								"Stone 1",
								"Stone 2",
								"Wood 1",
								"Wood 2",
								"Tournament",
								"Winboard",
								"Slics",
								"Thief",
								"<Custom>"
							  } ;

char *BdDirSq [BD_NUM_SQUARES] = { "",
								   "Canvas1",
								   "Canvas2",
								   "K_Brushed_Liquid",
								   "K_Cold_Marble",
								   "K_Gray_Tiles",
								   "K_Green_Marble",
								   "K_Pale_Wood",
								   "K_Provence",
								   "K_Red_and_Tan",
								   "K_Tan_and_Gray",
								   "K_Winter",
								   "K_XBoard",
								   "K_Yellow_and_Blue",
								   "Marble1",
								   "Marble2",
								   "Paper1",
								   "Paper2",
								   "Stone1",
								   "Stone2",
								   "Wood1",
								   "Wood2",
								   "Tournament",
								   "Winboard",
								   "Slics",
								   "Thief",
								   ""
								 } ;

#define BD_DEFAULT_PIECE    40
#define BD_DEFAULT_PIECE1   0
#define BD_NUM_PIECES       46
#define BD_PC_KNIGHTS_FROM  19
#define BD_PC_KNIGHTS_TO    29
char *BdPc [BD_NUM_PIECES] = { "H. J. Toups",
							   "Nico Koolsbergen",
							   "Kurt Birkefeldt",
							   "Jeff A Hurt",
							   "Eric Churchill",
							   "Dong Fong",
							   "User",
							   "Alpha",
							   "Book",
							   "Bookup",
							   "Chap",
							   "Cmf",
							   "Decius",
							   "Dyche",
							   "Dyche 1",
							   "Dyche 2",
							   "Fritz",
							   "Habsburg",
							   "Kingdom",
							   "Knights Abstract",
							   "Knights Classic",
							   "Knights Metallic",
							   "Knights Modern",
							   "Knights Nuts & Bolts",
							   "Knights Penguins",
							   "Knights Provence",
							   "Knights Staunton",
							   "Knights Staunton Wood",
							   "Knights XBoard",
							   "Knights XBoard 3D",
							   "Leipzig",
							   "Lucena",
							   "Mac",
							   "Mayan",
							   "Mono",
							   "Oldstyle",
							   "Staunton",
							   "Traveler",
							   "Uscf",
							   "Usual",
							   "Wcn",
							   "Winboard",
							   "Wstaunton",
							   "Wuscf",
							   "Yahoo",
							   "<Custom>"
							 } ;

char *BdDirPc [BD_NUM_PIECES] = { "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "",
								  "Alpha",
								  "Book",
								  "Bookup",
								  "Chap",
								  "Cmf",
								  "Decius",
								  "Dyche",
								  "Dyche1",
								  "Dyche2",
								  "Fritz",
								  "Habsburg",
								  "Kingdom",
								  "K_Abstract",
								  "K_Classic",
								  "K_Metallic",
								  "K_Modern",
								  "K_Nuts_and_Bolts",
								  "K_Penguins",
								  "K_Provence",
								  "K_Staunton",
								  "K_Staunton_Wood",
								  "K_XBoard",
								  "K_XBoard_3D",
								  "Leipzig",
								  "Lucena",
								  "Mac",
								  "Mayan",
								  "Mono",
								  "Oldstyle",
								  "Staunton",
								  "Traveler",
								  "Uscf",
								  "Usual",
								  "Wcn",
								  "Winboard",
								  "Wstaunton",
								  "Wuscf",
								  "Yahoo",
								  ""
								} ;

BOOL CALLBACK BoardBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc ;
	PAINTSTRUCT ps ;

	int nI, nJ, nF, bDoIt, bFound ;
	char cTmp [2048] ;

	char Drive [_MAX_DRIVE] ;
	char Dir   [_MAX_DIR] ;
	char File  [_MAX_FNAME] ;
	char Ext   [_MAX_EXT] ;
	char Res   [_MAX_PATH] ;

	FILE *Fv ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			FONT_Copy_Info(&BdFont [BD_FONT_HANDLE],        &hfFont [FONT_HANDLE]) ;
			FONT_Copy_Info(&BdFont [BD_FONT_CLOCK],         &hfFont [FONT_CLOCK]) ;
			FONT_Copy_Info(&BdFont [BD_FONT_GAMETYPE],      &hfFont [FONT_GAMETYPE]) ;
			FONT_Copy_Info(&BdFont [BD_FONT_LASTMOVE],      &hfFont [FONT_LASTMOVE]) ;
			FONT_Copy_Info(&BdFont [BD_FONT_LAGSTAT],       &hfFont [FONT_LAGSTAT]) ;
			FONT_Copy_Info(&BdFont [BD_FONT_RESULT],        &hfFont [FONT_RESULT]) ;
			FONT_Copy_Info(&BdFont [BD_FONT_COORDINATES],   &hfFont [FONT_COORDINATES]) ;

			for(nI = 0 ; nI < BD_MAX_FONT ; nI++)
			{
				FONT_CreateFont1(&BdFont [nI],
								 BdFont [nI].Name,       BdFont [nI].nPointSize,
								 BdFont [nI].bBold,      BdFont [nI].bItalic,
								 BdFont [nI].bUnderLine, BdFont [nI].bStrikeOut) ;
			}

			BdClr [BD_GAME_WINDOW]         = clrColor [CLR_WINDOW_COLOR] ;
			BdClr [BD_BOARD_BORDER]        = clrColor [CLR_BOARD_OUTLINE] ;
			BdClr [BD_BUFFER_BORDER]       = clrColor [CLR_BUFFER_OUTLINE] ;
			BdClr [BD_BUFFER]              = clrColor [CLR_BUFFER_FILL] ;
			BdClr [BD_WHITE_SQUARE_BORDER] = clrColor [CLR_WHITE_SQUARE_OUTLINE] ;
			BdClr [BD_WHITE_SQUARE]        = clrColor [CLR_WHITE_SQUARE_FILL] ;
			BdClr [BD_BLACK_SQUARE_BORDER] = clrColor [CLR_BLACK_SQUARE_OUTLINE] ;
			BdClr [BD_BLACK_SQUARE]        = clrColor [CLR_BLACK_SQUARE_FILL] ;
			BdClr [BD_PIECE_NUMBER]        = clrColor [CLR_PIECE_NUMBER] ;
			BdClr [BD_WHITE_PIECE_BORDER]  = clrColor [CLR_WHITE_PIECE_OUTLINE] ;
			BdClr [BD_WHITE_PIECE]         = clrColor [CLR_WHITE_PIECE_FILL] ;
			BdClr [BD_BLACK_PIECE_BORDER]  = clrColor [CLR_BLACK_PIECE_OUTLINE] ;
			BdClr [BD_BLACK_PIECE]         = clrColor [CLR_BLACK_PIECE_FILL] ;

			BdClr [BD_HANDLE_RATING]       = clrColor [CLR_HANDLE] ;

			BdClr [BD_CLOCK_TURN]          = clrColor [CLR_CLOCK_ON] ;
			BdClr [BD_CLOCK_TURN_BACK]     = clrColor [CLR_CLOCK_ON_BACKGROUND] ;
			BdClr [BD_CLOCK_WAIT]          = clrColor [CLR_CLOCK_OFF] ;

			BdClr [BD_LAG_STAT]            = clrColor [CLR_LAGSTAT] ;
			BdClr [BD_CLOCK_TIMEDOWN]      = clrColor [CLR_CLOCK_TIMEDOWN] ;
			BdClr [BD_CLOCK_TIMEUP]        = clrColor [CLR_CLOCK_TIMEUP] ;

			BdClr [BD_GAME_TYPE]           = clrColor [CLR_GAMETYPE] ;

			BdClr [BD_LAST_MOVE]           = clrColor [CLR_LASTMOVE] ;
			BdClr [BD_LAST_MOVE_SQUARE]    = clrColor [CLR_HIGHLIGHT] ;

			BdClr [BD_TRUE_PREMOVE_SQUARE] = clrColor [CLR_TRUE_PREMOVE_HIGHLIGHT] ;
			BdClr [BD_ILLEGAL_PREMOVE]     = clrColor [CLR_ILLEGAL_TRUE_PREMOVE] ;

			BdClr [BD_KING_HIGHLIGHT]      = clrColor [CLR_KING_HIGHLIGHT] ;

			BdClr [BD_RESULT_TEXT]         = clrColor [CLR_RESULT_FOREGROUND] ;
			BdClr [BD_RESULT_BACK]         = clrColor [CLR_RESULT_BACKGROUND] ;

			BdClr [BD_PTELL_TEXT]          = clrColor [CLR_PTELL_FOREGROUND] ;
			BdClr [BD_PTELL_BACK]          = clrColor [CLR_PTELL_BACKGROUND] ;

			BdClr [BD_GINFO_TEXT]          = clrColor [CLR_GINFO_FOREGROUND] ;
			BdClr [BD_GINFO_BACK]          = clrColor [CLR_GINFO_BACKGROUND] ;

			BdClr [BD_COORD_TEXT]          = clrColor [CLR_COORDINATES_FOREGROUND] ;

			SendDlgItemMessage(hwnd, IDD_BOARD_LIST, LB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < BD_NUM_LINES ; nI++)
			{
				sprintf(cTmp, "%02d. %s", nI + 1, BdStr [nI]) ;
				SendDlgItemMessage(hwnd, IDD_BOARD_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}
			SendDlgItemMessage(hwnd, IDD_BOARD_LIST, LB_SETCURSEL, 0, 0) ;

			nF = 0 ;

			if(! User.bVectorSquare)
			{
				bFound = 0 ;

				for(nI = 0 ; nI < BD_NUM_SQUARES ; nI++)
				{
					sprintf(Res, "\\BUGSTUFF\\SQUARE\\%s\\", BdDirSq [nI]) ;

					strcpy(cTmp, User.cBitmapSquare);
					cTmp [(int) strlen(Res)] = NULL_CHAR ;

					if(stricmp(Res, cTmp) == 0)
					{
						nF     = nI ;
						bFound = 1 ;
						break ;
					}
				}

				if(! bFound)
				{
					if(strlen(User.cBitmapSquare) == 0)
					{
						nF = 0 ;
					}
					else
					{
						nF = BD_NUM_SQUARES - 1 ;
					}
				}
			}

			SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < BD_NUM_SQUARES ; nI++)
			{
				sprintf(cTmp, "%02d. %s", nI + 1, BdSq [nI]) ;
				SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}
			SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_SETCURSEL, nF, 0) ;

			if(User.bVectorPiece)
			{
				nF = User.nPieceSet ;
			}
			else
			{
				nF     = 0 ;
				bFound = 0 ;

				for(nI = 0 ; nI < BD_NUM_PIECES ; nI++)
				{
					sprintf(Res, "BUGSTUFF\\PIECE\\BITMAP\\%s\\", BdDirPc [nI]) ;

					strcpy(cTmp, User.cBitmapPiece);
					cTmp [(int) strlen(Res)] = NULL_CHAR ;

					if(stricmp(Res, cTmp) == 0)
					{
						nF     = nI ;
						bFound = 1 ;
						break ;
					}
				}

				if(! bFound)
				{
					if(strlen(User.cBitmapPiece) == 0)
					{
						nF = 0 ;
					}
					else
					{
						nF = BD_NUM_PIECES - 1 ;
					}
				}
			}

			SendDlgItemMessage(hwnd, IDD_BOARD_PIECE, LB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < BD_NUM_PIECES ; nI++)
			{
				sprintf(cTmp, "%02d. %s", nI + 1, BdPc [nI]) ;
				SendDlgItemMessage(hwnd, IDD_BOARD_PIECE, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}
			SendDlgItemMessage(hwnd, IDD_BOARD_PIECE, LB_SETCURSEL, nF, 0) ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;

			BoardCfg.bVectorPiece  = User.bVectorPiece ;
			BoardCfg.bVectorSquare = User.bVectorSquare ;

			strcpy(BoardCfg.cBitmapPiece,  User.cBitmapPiece) ;
			strcpy(BoardCfg.cBitmapSquare, User.cBitmapSquare) ;

			BoardCfg.nPieceSet = User.nPieceSet ;

			hdc = GetDC(hwnd) ;
			BOARDCFG_DrawAll(hwnd, hdc) ;
			ReleaseDC(hwnd, hdc) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_BOARD_LIST :
				case IDD_BOARD_CONFIGURE :
					if(LOWORD(wParam) == IDD_BOARD_CONFIGURE)
					{
						bDoIt = 1 ;
					}
					else
					{
						bDoIt = (HIWORD(wParam) == LBN_DBLCLK) ;
					}

					if(bDoIt)
					{
						nI = SendDlgItemMessage(hwnd, IDD_BOARD_LIST, LB_GETCURSEL, 0, 0) ;
						switch(nI)
						{
							case BD_HANDLE_FONT :
								if(FONT_ChooseFont1(hwnd, &BdFont [BD_FONT_HANDLE]))
								{
									DeleteObject(BdFont [BD_FONT_HANDLE].hfFont) ;
									FONT_CreateFont1(&BdFont [BD_FONT_HANDLE],
													 BdFont [BD_FONT_HANDLE].Name,
													 BdFont [BD_FONT_HANDLE].nPointSize,
													 BdFont [BD_FONT_HANDLE].bBold,
													 BdFont [BD_FONT_HANDLE].bItalic,
													 BdFont [BD_FONT_HANDLE].bUnderLine,
													 BdFont [BD_FONT_HANDLE].bStrikeOut) ;
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;

							case BD_CLOCK_FONT :
								if(FONT_ChooseFont1(hwnd, &BdFont [BD_FONT_CLOCK]))
								{
									DeleteObject(BdFont [BD_FONT_CLOCK].hfFont) ;
									FONT_CreateFont1(&BdFont [BD_FONT_CLOCK],
													 BdFont [BD_FONT_CLOCK].Name,
													 BdFont [BD_FONT_CLOCK].nPointSize,
													 BdFont [BD_FONT_CLOCK].bBold,
													 BdFont [BD_FONT_CLOCK].bItalic,
													 BdFont [BD_FONT_CLOCK].bUnderLine,
													 BdFont [BD_FONT_CLOCK].bStrikeOut) ;
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;

							case BD_GAME_TYPE_FONT :
								if(FONT_ChooseFont1(hwnd, &BdFont [BD_FONT_GAMETYPE]))
								{
									DeleteObject(BdFont [BD_FONT_GAMETYPE].hfFont) ;
									FONT_CreateFont1(&BdFont [BD_FONT_GAMETYPE],
													 BdFont [BD_FONT_GAMETYPE].Name,
													 BdFont [BD_FONT_GAMETYPE].nPointSize,
													 BdFont [BD_FONT_GAMETYPE].bBold,
													 BdFont [BD_FONT_GAMETYPE].bItalic,
													 BdFont [BD_FONT_GAMETYPE].bUnderLine,
													 BdFont [BD_FONT_GAMETYPE].bStrikeOut) ;
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;

							case BD_LAST_MOVE_FONT :
								if(FONT_ChooseFont1(hwnd, &BdFont [BD_FONT_LASTMOVE]))
								{
									DeleteObject(BdFont [BD_FONT_LASTMOVE].hfFont) ;
									FONT_CreateFont1(&BdFont [BD_FONT_LASTMOVE],
													 BdFont [BD_FONT_LASTMOVE].Name,
													 BdFont [BD_FONT_LASTMOVE].nPointSize,
													 BdFont [BD_FONT_LASTMOVE].bBold,
													 BdFont [BD_FONT_LASTMOVE].bItalic,
													 BdFont [BD_FONT_LASTMOVE].bUnderLine,
													 BdFont [BD_FONT_LASTMOVE].bStrikeOut) ;
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;

							case BD_LAG_STAT_FONT :
								if(FONT_ChooseFont1(hwnd, &BdFont [BD_FONT_LAGSTAT]))
								{
									DeleteObject(BdFont [BD_FONT_LAGSTAT].hfFont) ;
									FONT_CreateFont1(&BdFont [BD_FONT_LAGSTAT],
													 BdFont [BD_FONT_LAGSTAT].Name,
													 BdFont [BD_FONT_LAGSTAT].nPointSize,
													 BdFont [BD_FONT_LAGSTAT].bBold,
													 BdFont [BD_FONT_LAGSTAT].bItalic,
													 BdFont [BD_FONT_LAGSTAT].bUnderLine,
													 BdFont [BD_FONT_LAGSTAT].bStrikeOut) ;
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;

							case BD_RESULT_FONT :
								if(FONT_ChooseFont1(hwnd, &BdFont [BD_FONT_RESULT]))
								{
									DeleteObject(BdFont [BD_FONT_RESULT].hfFont) ;
									FONT_CreateFont1(&BdFont [BD_FONT_RESULT],
													 BdFont [BD_FONT_RESULT].Name,
													 BdFont [BD_FONT_RESULT].nPointSize,
													 BdFont [BD_FONT_RESULT].bBold,
													 BdFont [BD_FONT_RESULT].bItalic,
													 BdFont [BD_FONT_RESULT].bUnderLine,
													 BdFont [BD_FONT_RESULT].bStrikeOut) ;
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;

							case BD_COORD_FONT :
								if(FONT_ChooseFont1(hwnd, &BdFont [BD_FONT_COORDINATES]))
								{
									DeleteObject(BdFont [BD_FONT_COORDINATES].hfFont) ;
									FONT_CreateFont1(&BdFont [BD_FONT_COORDINATES],
													 BdFont [BD_FONT_COORDINATES].Name,
													 BdFont [BD_FONT_COORDINATES].nPointSize,
													 BdFont [BD_FONT_COORDINATES].bBold,
													 BdFont [BD_FONT_COORDINATES].bItalic,
													 BdFont [BD_FONT_COORDINATES].bUnderLine,
													 BdFont [BD_FONT_COORDINATES].bStrikeOut) ;
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;

							default :
								if(COLOR_ChooseColor1(hwnd, &BdClr [nI]))
								{
									BOARDCFG_Refresh(hwnd) ;
								}
								break ;
						}
					}
					break ;

				case IDD_BOARD_SQUARE :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_GETCURSEL, 0, 0) ;

						if(nI == 0)
						{
							BoardCfg.bVectorSquare = 1 ;
							strcpy(BoardCfg.cBitmapSquare, "") ;
						}
						else
						{
							BoardCfg.bVectorSquare = 0 ;

							if(nI == (BD_NUM_SQUARES - 1))
							{
								Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "bmp", BMP_FILT, "Square File", NULL, NULL, Res, Browse.cSquareBrowse) ;
								if(Fv != NULL)
								{
									fclose(Fv) ;
									TOOLBOX_GetPath(Res, Browse.cSquareBrowse) ;
									INI_WriteSystem(INI_GetSysFilename()) ;

									BoardCfg.bVectorSquare = 0 ;

									SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_SETCURSEL, BD_NUM_SQUARES - 1, 0) ;

									_splitpath(Res, Drive, Dir, File, Ext) ;
									strcpy(Ext,  BMP_EXT) ;
									strcpy(File, "WSQUARE") ;
									_makepath(BoardCfg.cBitmapSquare, Drive, Dir, File, Ext) ;

									BOARDCFG_Refresh(hwnd) ;
								}
							}
							else
							{
								_splitpath(System.cDocumentDir, Drive, Dir, File, Ext) ;

								strcpy(Ext,  BMP_EXT) ;
								strcpy(File, "WSQUARE") ;
								strcat(Dir,  "BUGSTUFF\\SQUARE\\") ;
								strcat(Dir,  BdDirSq [nI]) ;
								_makepath(BoardCfg.cBitmapSquare, Drive, Dir, File, Ext) ;
							}
						}

						BOARDCFG_Refresh(hwnd) ;
					}
					break ;

				case IDD_BOARD_PIECE :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_BOARD_PIECE, LB_GETCURSEL, 0, 0) ;

						if(nI == 0)
						{
							BoardCfg.bVectorPiece = 1 ;
							BoardCfg.nPieceSet    = VECTOR_TOUPS ;
							strcpy(BoardCfg.cBitmapPiece, "") ;
						}
						else if(nI == 1)
						{
							BoardCfg.bVectorPiece = 1 ;
							BoardCfg.nPieceSet    = VECTOR_NICOK ;
							strcpy(BoardCfg.cBitmapPiece, "") ;
						}
						else if(nI == 2)
						{
							BoardCfg.bVectorPiece = 1 ;
							BoardCfg.nPieceSet    = VECTOR_KBIRK ;
							strcpy(BoardCfg.cBitmapPiece, "") ;
						}
						else if(nI == 3)
						{
							BoardCfg.bVectorPiece = 1 ;
							BoardCfg.nPieceSet    = VECTOR_JHURT ;
							strcpy(BoardCfg.cBitmapPiece, "") ;
						}
						else if(nI == 4)
						{
							BoardCfg.bVectorPiece = 1 ;
							BoardCfg.nPieceSet    = VECTOR_ECHUR ;
							strcpy(BoardCfg.cBitmapPiece, "") ;
						}
						else if(nI == 5)
						{
							BoardCfg.bVectorPiece = 1 ;
							BoardCfg.nPieceSet    = VECTOR_DFONG ;
							strcpy(BoardCfg.cBitmapPiece, "") ;
						}
						else if(nI == 6)
						{
							BoardCfg.bVectorPiece = 1 ;
							BoardCfg.nPieceSet    = VECTOR_USER ;
							strcpy(BoardCfg.cBitmapPiece, "") ;
						}
						else
						{
							BoardCfg.bVectorPiece = 0 ;

							if(nI == (BD_NUM_PIECES - 1))
							{
								Fv = TOOLBOX_OpenFileDialog(hwnd, FALSE, NULL, "bmp", BMP_FILT, "Piece File", NULL, NULL, Res, Browse.cPieceBrowse) ;
								if(Fv != NULL)
								{
									fclose(Fv) ;
									TOOLBOX_GetPath(Res, Browse.cPieceBrowse) ;
									INI_WriteSystem(INI_GetSysFilename()) ;

									BoardCfg.bVectorPiece = 0 ;

									SendDlgItemMessage(hwnd, IDD_BOARD_PIECE, LB_SETCURSEL, BD_NUM_PIECES - 1, 0) ;

									_splitpath(Res, Drive, Dir, File, Ext) ;
									strcpy(Ext,  BMP_EXT) ;
									strcpy(File, "WPAWN") ;
									_makepath(BoardCfg.cBitmapPiece, Drive, Dir, File, Ext) ;

									BOARDCFG_Refresh(hwnd) ;
								}
							}
							else
							{
								_splitpath(System.cDocumentDir, Drive, Dir, File, Ext) ;

								strcpy(Ext,  BMP_EXT) ;
								strcpy(File, "WPAWN") ;
								strcat(Dir,  "BUGSTUFF\\PIECE\\BITMAP\\") ;
								strcat(Dir,  BdDirPc [nI]) ;
								_makepath(BoardCfg.cBitmapPiece, Drive, Dir, File, Ext) ;
							}
						}

						BOARDCFG_Refresh(hwnd) ;
					}
					break ;

				case IDOK :
					nI = SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_GETCURSEL, 0, 0) ;
					nJ = SendDlgItemMessage(hwnd, IDD_BOARD_PIECE,  LB_GETCURSEL, 0, 0) ;

					if(((nI >= BD_SQ_KNIGHTS_FROM) && (nI <= BD_SQ_KNIGHTS_TO)) &&
							((nJ >= BD_PC_KNIGHTS_FROM) && (nJ <= BD_PC_KNIGHTS_TO)))
					{
						MessageBox(NULL,
								   "These pieces and squares were imported from Linux's Chess Interface called Knights.\nVisit http://knights.sourceforge.net and http://www.kde.org.",
								   "Thief",
								   MB_ICONINFORMATION) ;
					}
					else if((nI >= BD_SQ_KNIGHTS_FROM) && (nI <= BD_SQ_KNIGHTS_TO))
					{
						MessageBox(NULL,
								   "These squares were imported from Linux's Chess Interface called Knights.\nVisit http://knights.sourceforge.net and http://www.kde.org.",
								   "Thief",
								   MB_ICONINFORMATION) ;
					}
					else if((nJ >= BD_PC_KNIGHTS_FROM) && (nJ <= BD_PC_KNIGHTS_TO))
					{
						MessageBox(NULL,
								   "These pieces were imported from Linux's Chess Interface called Knights.\nVisit http://knights.sourceforge.net and http://www.kde.org.",
								   "Thief",
								   MB_ICONINFORMATION) ;
					}

					EndDialog(hwnd, TRUE) ;

					for(nI = 0 ; nI < BD_MAX_FONT ; nI++)
					{
						DeleteObject(BdFont [nI].hfFont) ;
					}

					FONT_Copy_Info(&hfFont [FONT_HANDLE],      &BdFont [BD_FONT_HANDLE]) ;
					FONT_Copy_Info(&hfFont [FONT_CLOCK],       &BdFont [BD_FONT_CLOCK]) ;
					FONT_Copy_Info(&hfFont [FONT_GAMETYPE],    &BdFont [BD_FONT_GAMETYPE]) ;
					FONT_Copy_Info(&hfFont [FONT_LASTMOVE],    &BdFont [BD_FONT_LASTMOVE]) ;
					FONT_Copy_Info(&hfFont [FONT_LAGSTAT],     &BdFont [BD_FONT_LAGSTAT]) ;
					FONT_Copy_Info(&hfFont [FONT_RESULT],      &BdFont [BD_FONT_RESULT]) ;
					FONT_Copy_Info(&hfFont [FONT_COORDINATES], &BdFont [BD_FONT_COORDINATES]) ;

					DeleteObject(hfFont [FONT_HANDLE].     hfFont) ;
					DeleteObject(hfFont [FONT_CLOCK].      hfFont) ;
					DeleteObject(hfFont [FONT_GAMETYPE].   hfFont) ;
					DeleteObject(hfFont [FONT_LASTMOVE].   hfFont) ;
					DeleteObject(hfFont [FONT_LAGSTAT].    hfFont) ;
					DeleteObject(hfFont [FONT_RESULT].     hfFont) ;
					DeleteObject(hfFont [FONT_COORDINATES].hfFont) ;

					TOOLBOX_MakeFont(FONT_HANDLE) ;
					TOOLBOX_MakeFont(FONT_CLOCK) ;
					TOOLBOX_MakeFont(FONT_GAMETYPE) ;
					TOOLBOX_MakeFont(FONT_LASTMOVE) ;
					TOOLBOX_MakeFont(FONT_LAGSTAT) ;
					TOOLBOX_MakeFont(FONT_RESULT) ;
					TOOLBOX_MakeFont(FONT_COORDINATES) ;

					clrColor [CLR_WINDOW_COLOR]           = BdClr [BD_GAME_WINDOW] ;
					clrColor [CLR_BOARD_OUTLINE]          = BdClr [BD_BOARD_BORDER] ;
					clrColor [CLR_BUFFER_OUTLINE]         = BdClr [BD_BUFFER_BORDER] ;
					clrColor [CLR_BUFFER_FILL]            = BdClr [BD_BUFFER] ;
					clrColor [CLR_WHITE_SQUARE_OUTLINE]   = BdClr [BD_WHITE_SQUARE_BORDER] ;
					clrColor [CLR_WHITE_SQUARE_FILL]      = BdClr [BD_WHITE_SQUARE] ;
					clrColor [CLR_BLACK_SQUARE_OUTLINE]   = BdClr [BD_BLACK_SQUARE_BORDER] ;
					clrColor [CLR_BLACK_SQUARE_FILL]      = BdClr [BD_BLACK_SQUARE] ;
					clrColor [CLR_PIECE_NUMBER]           = BdClr [BD_PIECE_NUMBER] ;
					clrColor [CLR_WHITE_PIECE_OUTLINE]    = BdClr [BD_WHITE_PIECE_BORDER] ;
					clrColor [CLR_WHITE_PIECE_FILL]       = BdClr [BD_WHITE_PIECE] ;
					clrColor [CLR_BLACK_PIECE_OUTLINE]    = BdClr [BD_BLACK_PIECE_BORDER] ;
					clrColor [CLR_BLACK_PIECE_FILL]       = BdClr [BD_BLACK_PIECE] ;

					clrColor [CLR_HANDLE]                 = BdClr [BD_HANDLE_RATING] ;

					clrColor [CLR_CLOCK_ON]               = BdClr [BD_CLOCK_TURN] ;
					clrColor [CLR_CLOCK_ON_BACKGROUND]    = BdClr [BD_CLOCK_TURN_BACK] ;
					clrColor [CLR_CLOCK_OFF]              = BdClr [BD_CLOCK_WAIT] ;
					clrColor [CLR_CLOCK_TIMEDOWN]		  = BdClr [BD_CLOCK_TIMEDOWN];
					clrColor [CLR_CLOCK_TIMEUP]			  = BdClr [BD_CLOCK_TIMEUP];

					clrColor [CLR_LAGSTAT]                = BdClr [BD_LAG_STAT] ;

					clrColor [CLR_GAMETYPE]               = BdClr [BD_GAME_TYPE] ;

					clrColor [CLR_LASTMOVE]               = BdClr [BD_LAST_MOVE] ;
					clrColor [CLR_HIGHLIGHT]              = BdClr [BD_LAST_MOVE_SQUARE] ;

					clrColor [CLR_TRUE_PREMOVE_HIGHLIGHT] = BdClr [BD_TRUE_PREMOVE_SQUARE] ;
					clrColor [CLR_ILLEGAL_TRUE_PREMOVE]   = BdClr [BD_ILLEGAL_PREMOVE] ;

					clrColor [CLR_KING_HIGHLIGHT]         = BdClr [BD_KING_HIGHLIGHT] ;

					clrColor [CLR_RESULT_FOREGROUND]      = BdClr [BD_RESULT_TEXT] ;
					clrColor [CLR_RESULT_BACKGROUND]      = BdClr [BD_RESULT_BACK] ;

					clrColor [CLR_PTELL_FOREGROUND]       = BdClr [BD_PTELL_TEXT] ;
					clrColor [CLR_PTELL_BACKGROUND]       = BdClr [BD_PTELL_BACK] ;

					clrColor [CLR_GINFO_FOREGROUND]       = BdClr [BD_GINFO_TEXT] ;
					clrColor [CLR_GINFO_BACKGROUND]       = BdClr [BD_GINFO_BACK] ;

					clrColor [CLR_COORDINATES_FOREGROUND] = BdClr [BD_COORD_TEXT] ;

					//
					// get square
					//
					nI = SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_GETCURSEL, 0, 0) ;

					if(nI == 0)
					{
						BoardCfg.bVectorSquare = 1 ;
						strcpy(BoardCfg.cBitmapSquare, "") ;
					}
					else
					{
						BoardCfg.bVectorSquare = 0 ;

						if(nI < (BD_NUM_SQUARES - 1))
						{
							_splitpath(System.cDocumentDir, Drive, Dir, File, Ext) ;
							sprintf(BoardCfg.cBitmapSquare, "BUGSTUFF\\SQUARE\\%s\\WSQUARE%s", BdDirSq [nI], BMP_EXT) ;
						}
					}

					//
					// get piece
					//
					nI = SendDlgItemMessage(hwnd, IDD_BOARD_PIECE, LB_GETCURSEL, 0, 0) ;

					if(nI == 0)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_TOUPS ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 1)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_NICOK ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 2)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_KBIRK ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 3)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_JHURT ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 4)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_ECHUR ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 5)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_DFONG ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 6)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_USER ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else
					{
						BoardCfg.bVectorPiece = 0 ;

						if(nI < (BD_NUM_PIECES - 1))
						{
							_splitpath(System.cDocumentDir, Drive, Dir, File, Ext) ;
							sprintf(BoardCfg.cBitmapPiece, "BUGSTUFF\\PIECE\\BITMAP\\%s\\WPAWN%s", BdDirPc [nI], BMP_EXT) ;
						}
					}

					//
					// save back to user structure
					//
					User.bVectorPiece  = BoardCfg.bVectorPiece ;
					User.bVectorSquare = BoardCfg.bVectorSquare ;

					strcpy(User.cBitmapPiece,  BoardCfg.cBitmapPiece) ;
					strcpy(User.cBitmapSquare, BoardCfg.cBitmapSquare) ;

					User.nPieceSet = BoardCfg.nPieceSet ;

					//
					// leave
					//
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					EndDialog(hwnd, FALSE) ;

					for(nI = 0 ; nI < BD_MAX_FONT ; nI++)
					{
						DeleteObject(BdFont [nI].hfFont) ;
					}

					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_BOARD_DEFAULT :
				case IDD_BOARD_DEFAULT_OLD :
					BdClr [BD_GAME_WINDOW]         = RGB(0xc0, 0xc0, 0xc0) ;
					BdClr [BD_BOARD_BORDER]        = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_BUFFER_BORDER]       = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_BUFFER]              = RGB(0xcc, 0xbd, 0xa4) ;
					BdClr [BD_WHITE_SQUARE_BORDER] = RGB(0xcc, 0xbd, 0xa4) ;
					BdClr [BD_WHITE_SQUARE]        = RGB(0xcc, 0xbd, 0xa4) ;
					BdClr [BD_BLACK_SQUARE_BORDER] = RGB(0xa8, 0x89, 0x64) ;
					BdClr [BD_BLACK_SQUARE]        = RGB(0xa8, 0x89, 0x64) ;
					BdClr [BD_PIECE_NUMBER]        = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_WHITE_PIECE_BORDER]  = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_WHITE_PIECE]         = RGB(0xff, 0xff, 0xff) ;
					BdClr [BD_BLACK_PIECE_BORDER]  = RGB(0xd4, 0xd4, 0xd4) ;
					BdClr [BD_BLACK_PIECE]         = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_HANDLE_RATING]       = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_CLOCK_TURN]          = RGB(0xff, 0xff, 0xff) ;
					BdClr [BD_CLOCK_TURN_BACK]     = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_CLOCK_WAIT]          = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_CLOCK_TIMEUP]        = RGB(0x00, 0xc0, 0x00) ;
					BdClr [BD_CLOCK_TIMEDOWN]      = RGB(0xc0, 0x00, 0x00) ;
					BdClr [BD_LAG_STAT]            = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_GAME_TYPE]           = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_LAST_MOVE]           = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_LAST_MOVE_SQUARE]    = RGB(0x00, 0xff, 0x00) ;
					BdClr [BD_TRUE_PREMOVE_SQUARE] = RGB(0xff, 0x00, 0x00) ;
					BdClr [BD_ILLEGAL_PREMOVE]     = RGB(0x00, 0x00, 0xff) ;
					BdClr [BD_KING_HIGHLIGHT]      = RGB(0xff, 0xff, 0x00) ;
					BdClr [BD_RESULT_TEXT]         = RGB(0xff, 0xff, 0xff) ;
					BdClr [BD_RESULT_BACK]         = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_PTELL_TEXT]          = RGB(0xff, 0xff, 0xff) ;
					BdClr [BD_PTELL_BACK]          = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_GINFO_TEXT]          = RGB(0xff, 0xff, 0xff) ;
					BdClr [BD_GINFO_BACK]          = RGB(0x00, 0x00, 0x00) ;
					BdClr [BD_COORD_TEXT]          = RGB(0x00, 0x00, 0x00) ;

					for(nI = 0 ; nI < BD_MAX_FONT ; nI++)
					{
						DeleteObject(BdFont [nI].hfFont) ;
					}

					strcpy(BdFont [BD_FONT_HANDLE].Name, "Times New Roman") ;

					BdFont [BD_FONT_HANDLE].nPointSize = 12 ;
					BdFont [BD_FONT_HANDLE].bBold      = 1  ;
					BdFont [BD_FONT_HANDLE].bItalic    = 0  ;
					BdFont [BD_FONT_HANDLE].bUnderLine = 0  ;
					BdFont [BD_FONT_HANDLE].bStrikeOut = 0  ;

					strcpy(BdFont [BD_FONT_CLOCK].Name, "Times New Roman") ;

					BdFont [BD_FONT_CLOCK].nPointSize = 16 ;
					BdFont [BD_FONT_CLOCK].bBold      = 0  ;
					BdFont [BD_FONT_CLOCK].bItalic    = 0  ;
					BdFont [BD_FONT_CLOCK].bUnderLine = 0  ;
					BdFont [BD_FONT_CLOCK].bStrikeOut = 0  ;

					strcpy(BdFont [BD_FONT_GAMETYPE].Name, "Times New Roman") ;

					BdFont [BD_FONT_GAMETYPE].nPointSize = 12 ;
					BdFont [BD_FONT_GAMETYPE].bBold      = 1  ;
					BdFont [BD_FONT_GAMETYPE].bItalic    = 0  ;
					BdFont [BD_FONT_GAMETYPE].bUnderLine = 0  ;
					BdFont [BD_FONT_GAMETYPE].bStrikeOut = 0  ;

					strcpy(BdFont [BD_FONT_LASTMOVE].Name, "Times New Roman") ;

					BdFont [BD_FONT_LASTMOVE].nPointSize = 11 ;
					BdFont [BD_FONT_LASTMOVE].bBold      = 0  ;
					BdFont [BD_FONT_LASTMOVE].bItalic    = 0  ;
					BdFont [BD_FONT_LASTMOVE].bUnderLine = 0  ;
					BdFont [BD_FONT_LASTMOVE].bStrikeOut = 0  ;

					strcpy(BdFont [BD_FONT_LAGSTAT].Name, "Times New Roman") ;

					BdFont [BD_FONT_LAGSTAT].nPointSize = 11 ;
					BdFont [BD_FONT_LAGSTAT].bBold      = 0  ;
					BdFont [BD_FONT_LAGSTAT].bItalic    = 0  ;
					BdFont [BD_FONT_LAGSTAT].bUnderLine = 0  ;
					BdFont [BD_FONT_LAGSTAT].bStrikeOut = 0  ;

					strcpy(BdFont [BD_FONT_RESULT].Name, "Times New Roman") ;

					BdFont [BD_FONT_RESULT].nPointSize = 11 ;
					BdFont [BD_FONT_RESULT].bBold      = 0  ;
					BdFont [BD_FONT_RESULT].bItalic    = 0  ;
					BdFont [BD_FONT_RESULT].bUnderLine = 0  ;
					BdFont [BD_FONT_RESULT].bStrikeOut = 0  ;

					strcpy(BdFont [BD_FONT_COORDINATES].Name, "Times New Roman") ;

					BdFont [BD_FONT_COORDINATES].nPointSize = 11 ;
					BdFont [BD_FONT_COORDINATES].bBold      = 0  ;
					BdFont [BD_FONT_COORDINATES].bItalic    = 0  ;
					BdFont [BD_FONT_COORDINATES].bUnderLine = 0  ;
					BdFont [BD_FONT_COORDINATES].bStrikeOut = 0  ;

					for(nI = 0 ; nI < BD_MAX_FONT ; nI++)
					{
						FONT_CreateFont1(&BdFont [nI],
										 BdFont [nI].Name,
										 BdFont [nI].nPointSize,
										 BdFont [nI].bBold,
										 BdFont [nI].bItalic,
										 BdFont [nI].bUnderLine,
										 BdFont [nI].bStrikeOut) ;
					}

					//
					// set square and piece to default
					//
					if(LOWORD(wParam) == IDD_BOARD_DEFAULT)
					{
						SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_SETCURSEL, BD_DEFAULT_SQUARE, 0) ;
						SendDlgItemMessage(hwnd, IDD_BOARD_PIECE,  LB_SETCURSEL, BD_DEFAULT_PIECE,  0) ;
					}
					else
					{
						SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_SETCURSEL, BD_DEFAULT_SQUARE1, 0) ;
						SendDlgItemMessage(hwnd, IDD_BOARD_PIECE,  LB_SETCURSEL, BD_DEFAULT_PIECE1,  0) ;
					}

					//
					// get square
					//
					nI = SendDlgItemMessage(hwnd, IDD_BOARD_SQUARE, LB_GETCURSEL, 0, 0) ;

					if(nI == 0)
					{
						BoardCfg.bVectorSquare = 1 ;
						strcpy(BoardCfg.cBitmapSquare, "") ;
					}
					else
					{
						BoardCfg.bVectorSquare = 0 ;

						if(nI < (BD_NUM_SQUARES - 1))
						{
							_splitpath(System.cDocumentDir, Drive, Dir, File, Ext) ;

							strcpy(Ext,  BMP_EXT) ;
							strcpy(File, "WSQUARE") ;
							strcat(Dir,  "BUGSTUFF\\SQUARE\\") ;
							strcat(Dir,  BdDirSq [nI]) ;
							_makepath(BoardCfg.cBitmapSquare, Drive, Dir, File, Ext) ;
						}
					}

					//
					// get piece
					//
					nI = SendDlgItemMessage(hwnd, IDD_BOARD_PIECE, LB_GETCURSEL, 0, 0) ;

					if(nI == 0)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_TOUPS ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 1)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_NICOK ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 2)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_KBIRK ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 3)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_JHURT ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 4)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_ECHUR ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 5)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_DFONG ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else if(nI == 6)
					{
						BoardCfg.bVectorPiece = 1 ;
						BoardCfg.nPieceSet    = VECTOR_USER ;
						strcpy(BoardCfg.cBitmapPiece, "") ;
					}
					else
					{
						BoardCfg.bVectorPiece = 0 ;

						if(nI < (BD_NUM_PIECES - 1))
						{
							_splitpath(System.cDocumentDir, Drive, Dir, File, Ext) ;

							strcpy(Ext,  BMP_EXT) ;
							strcpy(File, "WPAWN") ;
							strcat(Dir,  "BUGSTUFF\\PIECE\\BITMAP\\") ;
							strcat(Dir,  BdDirPc [nI]) ;
							_makepath(BoardCfg.cBitmapPiece, Drive, Dir, File, Ext) ;
						}
					}

					BOARDCFG_Refresh(hwnd) ;
					return TRUE ;
			}
			break ;

		case WM_PAINT :
			hdc = BeginPaint(hwnd, &ps) ;
			BOARDCFG_DrawAll(hwnd, hdc) ;
			EndPaint(hwnd, &ps) ;
			break ;
	}
	return FALSE ;
}

void BOARDCFG_Resize(HDC hdc, int nxClient, int nyClient)
{
	HFONT hfOld ;
	SIZE szLag, szHandle, szClock, szGameType, szLastMove, szLMX, szResult, szPtell, szGinfo, szCoord ;
	int nH, nT, nB, nU, nI, nss, nC, nF, nG ;
	char cTmp [100] ;

	nxClient = nxClient - 4 ;
	nyClient = nyClient - 2 ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_LAGSTAT].hfFont) ;
	GetTextExtentPoint32(hdc, "Lag:00:00", 9, &szLag) ;

	SelectObject(hdc, BdFont [BD_FONT_HANDLE].hfFont) ;
	GetTextExtentPoint32(hdc, "WWWWWWWWWWWWWWWW", 16, &szHandle) ;

	SelectObject(hdc, BdFont [BD_FONT_CLOCK].hfFont) ;

	sprintf(cTmp, "%s", CLOCK_TimeString(23 * 60 * 60 * 1000L)) ;

	GetTextExtentPoint32(hdc, cTmp, strlen(cTmp), &szClock) ;

	SelectObject(hdc, BdFont [BD_FONT_GAMETYPE].hfFont) ;
	GetTextExtentPoint32(hdc, "100 12 W WWWWWWWW", 17, &szGameType) ;

	SelectObject(hdc, BdFont [BD_FONT_LASTMOVE].hfFont) ;
	GetTextExtentPoint32(hdc, "111. WWWWWWWWWW (12:34)", 23, &szLastMove) ;
	GetTextExtentPoint32(hdc, "W", 1, &szLMX) ;

	SelectObject(hdc, BdFont [BD_FONT_RESULT].hfFont) ;
	GetTextExtentPoint32(hdc, "Game Result     ", 16, &szResult) ;

	SelectObject(hdc, BdFont [BD_FONT_RESULT].hfFont) ;
	GetTextExtentPoint32(hdc, "Partner Tell    ", 16, &szPtell) ;

	SelectObject(hdc, BdFont [BD_FONT_RESULT].hfFont) ;
	GetTextExtentPoint32(hdc, "Game Information", 16, &szGinfo) ;

	SelectObject(hdc, BdFont [BD_FONT_COORDINATES].hfFont) ;
	GetTextExtentPoint32(hdc, "8", 1, &szCoord) ;

	SelectObject(hdc, hfOld) ;

	if(szHandle.cy > szClock.cy)
	{
		nH = szHandle.cy ;
	}
	else
	{
		nH = szClock.cy ;
	}

	if(szLag.cy > nH)
	{
		nH = szLag.cy ;
	}

	nT = nH + 2 ;
	nB = 2 + nH ;

	if(szGameType.cy > szLastMove.cy)
	{
		nU = szGameType.cy ;
	}
	else
	{
		nU = szLastMove.cy ;
	}
	nB = nB + (2 + nU) ;

	nB = nB + (2 + szResult.cy) ;
	nB = nB + (1 + szCoord.cy) ;

	nyClient = nyClient - (nT + nB) ;
	nxClient = nxClient - szCoord.cx - 4 ;

	nss = nxClient / 10 ;               // x square size
	if((nss << 3) > nyClient)           // y board size  (shift right 3 = x 8)
	{
		nss = nyClient >> 3 ;           // x square size (shift left  3 = / 8)
	}

	BoardCfg.nss  = nss ;               // assign square size
	BoardCfg.nhss = nss >> 1 ;          // assign half square size
	BoardCfg.ndss = nss << 1 ;          // assign double square size

	BoardCfg.rBoard.left   = BD_LEFT + 2 + szCoord.cx + 2 ;
	BoardCfg.rBoard.top    = BD_TOP  + nT ;
	BoardCfg.rBoard.right  = BoardCfg.rBoard.left + (nss << 3) + 1 ;
	BoardCfg.rBoard.bottom = BoardCfg.rBoard.top  + (nss << 3) + 1 ;

	BoardCfg.rBuffer.left   = BoardCfg.rBoard.right + 1 ;
	BoardCfg.rBuffer.top    = BoardCfg.rBoard.top ;
	BoardCfg.rBuffer.right  = BoardCfg.rBuffer.left + nss + nss + 1 ;
	BoardCfg.rBuffer.bottom = BoardCfg.rBoard.bottom ;

	nT = BD_TOP + 0 ;
	nB = BoardCfg.rBoard.bottom ;

	BoardCfg.rHandle [INDEX_BLACK].top = (nT + nH) - szHandle.cy ;
	BoardCfg.rClock  [INDEX_BLACK].top = (nT + nH) - szClock.cy ;
	BoardCfg.rLag    [INDEX_BLACK].top = (nT + nH) - szLag.cy ;

	nB = nB + 1 ;
	nF = (nss >> 1) ;
	nG = (szCoord.cx >> 1) ;

	for(nI = 0 ; nI < 8 ; nI++)
	{
		nC = ((BoardCfg.rBoard.left + (nss * nI) + nF) - nG) ;
		BoardCfg.rHCoord [nI].top    = nB ;
		BoardCfg.rHCoord [nI].bottom = nB + szCoord.cy ;
		BoardCfg.rHCoord [nI].left   = nC ;
		BoardCfg.rHCoord [nI].right  = nC + szCoord.cx ;
	}

	nG = (szCoord.cy >> 1) ;
	for(nI = 0 ; nI < 8 ; nI++)
	{
		nC = ((BoardCfg.rBoard.top + (nss * nI) + nF) - nG) ;
		BoardCfg.rVCoord [nI].top    = nC ;
		BoardCfg.rVCoord [nI].bottom = nC + szCoord.cy ;
		BoardCfg.rVCoord [nI].left   = BD_LEFT + 2 ;
		BoardCfg.rVCoord [nI].right  = BD_LEFT + 2 + szCoord.cx ;
	}

	nB = nB + 1 + szCoord.cy ;

	BoardCfg.rHandle [INDEX_WHITE].top = nB ;
	BoardCfg.rClock  [INDEX_WHITE].top = nB ;
	BoardCfg.rLag    [INDEX_WHITE].top = nB ;

	nB = nB + nH ;
	nT = nT + (nH + 2) ;

	BoardCfg.rHandle [INDEX_WHITE].left   = BoardCfg.rBoard.left + 1 ;
	BoardCfg.rHandle [INDEX_BLACK].left   = BoardCfg.rBoard.left + 1 ;

	BoardCfg.rHandle [INDEX_WHITE].right  = BoardCfg.rHandle [INDEX_WHITE].left + szHandle.cx ;
	BoardCfg.rHandle [INDEX_BLACK].right  = BoardCfg.rHandle [INDEX_BLACK].left + szHandle.cx ;

	BoardCfg.rHandle [INDEX_WHITE].bottom = BoardCfg.rHandle [INDEX_WHITE].top  + szHandle.cy ;
	BoardCfg.rHandle [INDEX_BLACK].bottom = BoardCfg.rHandle [INDEX_BLACK].top  + szHandle.cy ;

	BoardCfg.rClock [INDEX_WHITE].left   = BoardCfg.rBoard.right - szClock.cx ;
	BoardCfg.rClock [INDEX_BLACK].left   = BoardCfg.rClock [INDEX_WHITE].left ;

	BoardCfg.rClock [INDEX_WHITE].right  = BoardCfg.rClock [INDEX_WHITE].left + szClock.cx ;
	BoardCfg.rClock [INDEX_BLACK].right  = BoardCfg.rClock [INDEX_BLACK].left + szClock.cx ;

	BoardCfg.rClock [INDEX_WHITE].bottom = BoardCfg.rClock [INDEX_WHITE].top  + szClock.cy ;
	BoardCfg.rClock [INDEX_BLACK].bottom = BoardCfg.rClock [INDEX_BLACK].top  + szClock.cy ;

	BoardCfg.rLag [INDEX_WHITE].bottom = BoardCfg.rLag [INDEX_WHITE].top + szLag.cy ;
	BoardCfg.rLag [INDEX_WHITE].left   = BoardCfg.rClock [INDEX_WHITE].right + 1 ;
	BoardCfg.rLag [INDEX_WHITE].right  = BoardCfg.rLag [INDEX_WHITE].left + szLag.cx ;

	BoardCfg.rLag [INDEX_BLACK].bottom = BoardCfg.rLag [INDEX_BLACK].top + szLag.cy ;
	BoardCfg.rLag [INDEX_BLACK].left   = BoardCfg.rLag [INDEX_WHITE].left ;
	BoardCfg.rLag [INDEX_BLACK].right  = BoardCfg.rLag [INDEX_WHITE].right ;

	nB = nB + 2 ;

	BoardCfg.rGameType.top    = nB ;
	BoardCfg.rGameType.bottom = BoardCfg.rGameType.top + szGameType.cy ;
	BoardCfg.rGameType.left   = BoardCfg.rBoard.left ;
	BoardCfg.rGameType.right  = BoardCfg.rGameType.left + szGameType.cx ;

	BoardCfg.rLastMove.top    = nB ;
	BoardCfg.rLastMove.bottom = BoardCfg.rLastMove.top + szLastMove.cy ;

	nI = ((BoardCfg.rBoard.right - BoardCfg.rBoard.left) + 1) >> 1 ;

	BoardCfg.rLastMove.left   = (BoardCfg.rBoard.left + nI) - szLMX.cx - szLMX.cx ;
	BoardCfg.rLastMove.right  = BoardCfg.rLastMove.left + szLastMove.cx ;

	nB = nB + (nU + 2) ;

	BoardCfg.rResult.top    = nB ;
	BoardCfg.rResult.bottom = BoardCfg.rResult.top + szResult.cy ;
	BoardCfg.rResult.left   = BoardCfg.rHandle [INDEX_WHITE].left ;
	BoardCfg.rResult.right  = BoardCfg.rResult.left + szResult.cx ;

	BoardCfg.rPtell.top    = BoardCfg.rResult.top ;
	BoardCfg.rPtell.bottom = BoardCfg.rResult.bottom ;
	BoardCfg.rPtell.left   = BoardCfg.rResult.right ;
	BoardCfg.rPtell.right  = BoardCfg.rPtell.left + szPtell.cx ;

	BoardCfg.rGinfo.top    = BoardCfg.rResult.top ;
	BoardCfg.rGinfo.bottom = BoardCfg.rResult.bottom ;
	BoardCfg.rGinfo.left   = BoardCfg.rPtell.right ;
	BoardCfg.rGinfo.right  = BoardCfg.rGinfo.left + szGinfo.cx ;

	nB = nB + szResult.cy ;

	BoardCfg.rActual.top    = 0 ;
	BoardCfg.rActual.left   = 0 ;
	BoardCfg.rActual.right  = BoardCfg.rBuffer.right + 1 ;
	BoardCfg.rActual.bottom = nB + 1 ;

	BoardCfg.ptBuffer [BLACK_PAWN].x   = 1 ;
	BoardCfg.ptBuffer [BLACK_PAWN].y   = 1 ;

	BoardCfg.ptBuffer [BLACK_KNIGHT].x = 1 ;
	BoardCfg.ptBuffer [BLACK_KNIGHT].y = 1 + nss ;

	BoardCfg.ptBuffer [BLACK_BISHOP].x = 1 ;
	BoardCfg.ptBuffer [BLACK_BISHOP].y = 1 + nss + nss ;

	BoardCfg.ptBuffer [BLACK_ROOK].x   = 1 + nss ;
	BoardCfg.ptBuffer [BLACK_ROOK].y   = 1 ;

	BoardCfg.ptBuffer [BLACK_QUEEN].x  = 1 + nss ;
	BoardCfg.ptBuffer [BLACK_QUEEN].y  = 1 + nss ;

	BoardCfg.ptBuffer [WHITE_PAWN].x   = 1 ;
	BoardCfg.ptBuffer [WHITE_PAWN].y   = 1 + (nss * 5) ;

	BoardCfg.ptBuffer [WHITE_KNIGHT].x = 1 ;
	BoardCfg.ptBuffer [WHITE_KNIGHT].y = 1 + (nss * 6) ;

	BoardCfg.ptBuffer [WHITE_BISHOP].x = 1 ;
	BoardCfg.ptBuffer [WHITE_BISHOP].y = 1 + (nss * 7) ;

	BoardCfg.ptBuffer [WHITE_ROOK].x   = 1 + nss ;
	BoardCfg.ptBuffer [WHITE_ROOK].y   = 1 + (nss * 5) ;

	BoardCfg.ptBuffer [WHITE_QUEEN].x  = 1 + nss ;
	BoardCfg.ptBuffer [WHITE_QUEEN].y  = 1 + (nss * 6) ;

	BDRAW_LoadPieces() ;    // load piece files
	BDRAW_SetSize(nss) ;    // adjust bitmap size

	for(nI = 0 ; nI < 3 ; nI++)
	{
		BoardCfg.ptNumber [nI].x = 1 + ((_BdptNumber [nI].x * nss) / ORG_SIZE) ;
		BoardCfg.ptNumber [nI].y = (_BdptNumber [nI].y * nss) / ORG_SIZE ;
	}
}

void BOARDCFG_DrawHandle(HDC hdc)
{
	HFONT    hfOld ;
	COLORREF clrOT, clrOB ;

	hfOld = (HFONT)    SelectObject(hdc, BdFont [BD_FONT_HANDLE].hfFont) ;
	clrOT = (COLORREF) SetTextColor(hdc, BdClr [BD_HANDLE_RATING]) ;
	clrOB = (COLORREF) SetBkColor(hdc, BdClr [BD_GAME_WINDOW]) ;

	ExtTextOut(hdc,
			   BoardCfg.rHandle [INDEX_WHITE].left,
			   BoardCfg.rHandle [INDEX_WHITE].top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rHandle [INDEX_WHITE],
			   "WHITE (1234)",
			   strlen("WHITE (1234)"),
			   NULL) ;

	ExtTextOut(hdc,
			   BoardCfg.rHandle [INDEX_BLACK].left,
			   BoardCfg.rHandle [INDEX_BLACK].top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rHandle [INDEX_BLACK],
			   "BLACK (1234)",
			   strlen("BLACK (1234)"),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawWhiteLag(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;
	char cTime [15] ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_LAGSTAT].hfFont) ;

	strcpy(cTime, "Lag:00:02") ;

	clrOT = SetTextColor(hdc, BdClr [BD_LAG_STAT]) ;
	clrOB = SetBkColor(hdc, BdClr [BD_GAME_WINDOW]) ;

	ExtTextOut(hdc,
			   BoardCfg.rLag [INDEX_WHITE].left,
			   BoardCfg.rLag [INDEX_WHITE].top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rLag [INDEX_WHITE], cTime, strlen(cTime),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawBlackLag(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;
	char cTime [15] ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_LAGSTAT].hfFont) ;

	strcpy(cTime, "Lag:00:01") ;

	clrOT = SetTextColor(hdc, BdClr [BD_LAG_STAT]) ;
	clrOB = SetBkColor(hdc, BdClr [BD_GAME_WINDOW]) ;

	ExtTextOut(hdc,
			   BoardCfg.rLag [INDEX_BLACK].left,
			   BoardCfg.rLag [INDEX_BLACK].top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rLag [INDEX_BLACK], cTime, strlen(cTime),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawWhiteClock(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;
	char cTime [100] ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_CLOCK].hfFont) ;

	sprintf(cTime, "%s", CLOCK_TimeString(179000L)) ;

	clrOT = SetTextColor(hdc, BdClr [BD_CLOCK_WAIT]) ;
	clrOB = SetBkColor(hdc, BdClr [BD_GAME_WINDOW]) ;

	ExtTextOut(hdc,
			   BoardCfg.rClock [INDEX_WHITE].left,
			   BoardCfg.rClock [INDEX_WHITE].top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rClock [INDEX_WHITE], cTime, strlen(cTime),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawBlackClock(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;
	char cTime [100] ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_CLOCK].hfFont) ;

	sprintf(cTime, "%s", CLOCK_TimeString(180000L)) ;

	clrOT = SetTextColor(hdc, BdClr [BD_CLOCK_TURN]) ;
	clrOB = SetBkColor(hdc, BdClr [BD_CLOCK_TURN_BACK]) ;

	ExtTextOut(hdc,
			   BoardCfg.rClock [INDEX_BLACK].left,
			   BoardCfg.rClock [INDEX_BLACK].top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rClock [INDEX_BLACK], cTime, strlen(cTime),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawGameType(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;
	char cTmp [30] ;

	hfOld = (HFONT)    SelectObject(hdc, BdFont [BD_FONT_GAMETYPE].hfFont) ;
	clrOT = (COLORREF) SetTextColor(hdc, BdClr [BD_GAME_TYPE]) ;
	clrOB = (COLORREF) SetBkColor(hdc, BdClr [BD_GAME_WINDOW]) ;

	strcpy(cTmp, "3 0 r bug") ;

	ExtTextOut(hdc,
			   BoardCfg.rGameType.left,
			   BoardCfg.rGameType.top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rGameType,
			   cTmp,
			   strlen(cTmp),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawLastMove(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;
	char cTmp [30] ;

	hfOld = (HFONT)    SelectObject(hdc, BdFont [BD_FONT_LASTMOVE].hfFont) ;
	clrOT = (COLORREF) SetTextColor(hdc, BdClr [BD_LAST_MOVE]) ;
	clrOB = (COLORREF) SetBkColor(hdc, BdClr [BD_GAME_WINDOW]) ;

	strcpy(cTmp, "1. e4 (00:01)") ;

	ExtTextOut(hdc,
			   BoardCfg.rLastMove.left,
			   BoardCfg.rLastMove.top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rLastMove,
			   cTmp,
			   strlen(cTmp),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawResult(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_RESULT].hfFont) ;

	clrOT = (COLORREF) SetTextColor(hdc, BdClr [BD_RESULT_TEXT]) ;
	clrOB = (COLORREF) SetBkColor(hdc, BdClr [BD_RESULT_BACK]) ;

	ExtTextOut(hdc,
			   BoardCfg.rResult.left,
			   BoardCfg.rResult.top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rResult,
			   "Game Result",
			   strlen("Game Result"),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawPtell(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_RESULT].hfFont) ;

	clrOT = (COLORREF) SetTextColor(hdc, BdClr [BD_PTELL_TEXT]) ;
	clrOB = (COLORREF) SetBkColor(hdc, BdClr [BD_PTELL_BACK]) ;

	ExtTextOut(hdc,
			   BoardCfg.rPtell.left,
			   BoardCfg.rPtell.top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rPtell,
			   "Partner Tell",
			   strlen("Partner Tell"),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawGinfo(HDC hdc)
{
	HFONT hfOld ;
	COLORREF clrOT, clrOB ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_RESULT].hfFont) ;

	clrOT = (COLORREF) SetTextColor(hdc, BdClr [BD_GINFO_TEXT]) ;
	clrOB = (COLORREF) SetBkColor(hdc, BdClr [BD_GINFO_BACK]) ;

	ExtTextOut(hdc,
			   BoardCfg.rGinfo.left,
			   BoardCfg.rGinfo.top,
			   ETO_CLIPPED | ETO_OPAQUE,
			   &BoardCfg.rGinfo,
			   "Game Information",
			   strlen("Game Information"),
			   NULL) ;

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_DrawCoordinates(HDC hdc, int bFlip)
{
	HFONT    hfOld ;
	COLORREF clrOT, clrOB ;

	hfOld = (HFONT) SelectObject(hdc, BdFont [BD_FONT_COORDINATES].hfFont) ;

	clrOT = (COLORREF) SetTextColor(hdc, BdClr [BD_COORD_TEXT]) ;
	clrOB = (COLORREF) SetBkColor(hdc, BdClr [BD_GAME_WINDOW]) ;

	if(bFlip)
	{
		// 87654321
		ExtTextOut(hdc, BoardCfg.rVCoord [0].left, BoardCfg.rVCoord [0].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [0], "1", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [1].left, BoardCfg.rVCoord [1].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [1], "2", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [2].left, BoardCfg.rVCoord [2].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [2], "3", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [3].left, BoardCfg.rVCoord [3].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [3], "4", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [4].left, BoardCfg.rVCoord [4].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [4], "5", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [5].left, BoardCfg.rVCoord [5].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [5], "6", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [6].left, BoardCfg.rVCoord [6].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [6], "7", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [7].left, BoardCfg.rVCoord [7].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [7], "8", 1, NULL) ;

		// abcdefgh
		ExtTextOut(hdc, BoardCfg.rHCoord [0].left, BoardCfg.rHCoord [0].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [0], "h", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [1].left, BoardCfg.rHCoord [1].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [1], "g", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [2].left, BoardCfg.rHCoord [2].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [2], "f", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [3].left, BoardCfg.rHCoord [3].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [3], "e", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [4].left, BoardCfg.rHCoord [4].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [4], "d", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [5].left, BoardCfg.rHCoord [5].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [5], "c", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [6].left, BoardCfg.rHCoord [6].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [6], "b", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [7].left, BoardCfg.rHCoord [7].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [7], "a", 1, NULL) ;
	}
	else
	{
		// 12345678
		ExtTextOut(hdc, BoardCfg.rVCoord [0].left, BoardCfg.rVCoord [0].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [0], "8", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [1].left, BoardCfg.rVCoord [1].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [1], "7", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [2].left, BoardCfg.rVCoord [2].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [2], "6", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [3].left, BoardCfg.rVCoord [3].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [3], "5", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [4].left, BoardCfg.rVCoord [4].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [4], "4", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [5].left, BoardCfg.rVCoord [5].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [5], "3", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [6].left, BoardCfg.rVCoord [6].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [6], "2", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rVCoord [7].left, BoardCfg.rVCoord [7].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rVCoord [7], "1", 1, NULL) ;

		// abcdefgh
		ExtTextOut(hdc, BoardCfg.rHCoord [0].left, BoardCfg.rHCoord [0].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [0], "a", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [1].left, BoardCfg.rHCoord [1].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [1], "b", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [2].left, BoardCfg.rHCoord [2].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [2], "c", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [3].left, BoardCfg.rHCoord [3].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [3], "d", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [4].left, BoardCfg.rHCoord [4].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [4], "e", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [5].left, BoardCfg.rHCoord [5].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [5], "f", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [6].left, BoardCfg.rHCoord [6].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [6], "g", 1, NULL) ;

		ExtTextOut(hdc, BoardCfg.rHCoord [7].left, BoardCfg.rHCoord [7].top,
				   ETO_CLIPPED | ETO_OPAQUE, &BoardCfg.rHCoord [7], "h", 1, NULL) ;
	}

	SetTextColor(hdc, clrOT) ;
	SetBkColor(hdc, clrOB) ;
	SelectObject(hdc, hfOld) ;
}

void BOARDCFG_SquareToPosition(int nBx, int nBy, int *nPx, int *nPy)
{
	*nPx = BoardCfg.rBoard.left + 1 + (nBx * BoardCfg.nss) ;
	*nPy = BoardCfg.rBoard.top  + 1 + (ReverseCoord [nBy] * BoardCfg.nss) ;
}

void BOARDCFG_DrawTruePremove(HDC hdc)
{
	HPEN hpTmp, hpOld ;
	int nPx, nPy ;

	hpTmp = CreatePen(PS_SOLID, 0, BdClr [BD_TRUE_PREMOVE_SQUARE]) ;
	hpOld = (HPEN) SelectObject(hdc, hpTmp) ;

	BOARDCFG_SquareToPosition(1, 0, &nPx, &nPy) ;

	MoveToEx(hdc, nPx + 1, nPy + 1, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + 1) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + 1) ;

	MoveToEx(hdc, nPx + 2, nPy + 2, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + 2) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + 2) ;

	BOARDCFG_SquareToPosition(2, 2, &nPx, &nPy) ;

	MoveToEx(hdc, nPx + 1, nPy + 1, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + 1) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + 1) ;

	MoveToEx(hdc, nPx + 2, nPy + 2, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + 2) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + 2) ;

	SelectObject(hdc, hpOld) ;
	DeleteObject(hpTmp) ;
}

void BOARDCFG_DrawIllegalTruePremove(HDC hdc)
{
	HPEN hpTmp, hpOld ;
	int nPx, nPy ;

	hpTmp = CreatePen(PS_SOLID, 0, BdClr [BD_ILLEGAL_PREMOVE]) ;
	hpOld = (HPEN) SelectObject(hdc, hpTmp) ;

	BOARDCFG_SquareToPosition(7, 0, &nPx, &nPy) ;

	MoveToEx(hdc, nPx + 1, nPy + 1, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + 1) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + 1) ;

	MoveToEx(hdc, nPx + 2, nPy + 2, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + 2) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + 2) ;

	BOARDCFG_SquareToPosition(5, 0, &nPx, &nPy) ;

	MoveToEx(hdc, nPx + 1, nPy + 1, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + 1) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + 1) ;

	MoveToEx(hdc, nPx + 2, nPy + 2, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + 2) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + 2) ;

	SelectObject(hdc, hpOld) ;
	DeleteObject(hpTmp) ;
}

void BOARDCFG_DrawKingHighlight(HDC hdc)
{
	HPEN hpTmp, hpOld ;
	int nPx, nPy ;

	hpTmp = CreatePen(PS_SOLID, 0, BdClr [BD_KING_HIGHLIGHT]) ;
	hpOld = (HPEN) SelectObject(hdc, hpTmp) ;

	BOARDCFG_SquareToPosition(4, 1, &nPx, &nPy) ;

	MoveToEx(hdc, nPx + 1, nPy + 1, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + 1) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + 1) ;

	MoveToEx(hdc, nPx + 2, nPy + 2, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + 2) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + 2) ;

	SelectObject(hdc, hpOld) ;
	DeleteObject(hpTmp) ;
}

void BOARDCFG_DrawFromToSquareHighlight(HDC hdc)
{
	HPEN hpTmp, hpOld ;
	int nPx, nPy ;

	hpTmp = CreatePen(PS_SOLID, 0, BdClr [BD_LAST_MOVE_SQUARE]) ;
	hpOld = (HPEN) SelectObject(hdc, hpTmp) ;

	BOARDCFG_SquareToPosition(4, 1, &nPx, &nPy) ;

	MoveToEx(hdc, nPx + 1, nPy + 1, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + 1) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + 1) ;

	MoveToEx(hdc, nPx + 2, nPy + 2, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + 2) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + 2) ;

	BOARDCFG_SquareToPosition(4, 3, &nPx, &nPy) ;

	MoveToEx(hdc, nPx + 1, nPy + 1, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + 1) ;
	LineTo(hdc, nPx + BoardCfg.nss - 2, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + BoardCfg.nss - 2) ;
	LineTo(hdc, nPx + 1, nPy + 1) ;

	MoveToEx(hdc, nPx + 2, nPy + 2, NULL) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + 2) ;
	LineTo(hdc, nPx + BoardCfg.nss - 3, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + BoardCfg.nss - 3) ;
	LineTo(hdc, nPx + 2, nPy + 2) ;

	SelectObject(hdc, hpOld) ;
	DeleteObject(hpTmp) ;
}

void BOARDCFG_DrawBoardPiece(HDC hdc, int nBx, int nBy, int nP)
{
	HDC      hdcMem, hdcBitmap, hdcMem1 ;
	HBITMAP  htOld, htSquare, htMask, htPiece, htBitmap, htOldBitmap, htOld1 ;
	BITMAP   bitmap ;
	COLORREF clrTran ;
	int      nS, nPx, nPy, bV ;

	char Drive [_MAX_DRIVE] ;
	char Dir   [_MAX_DIR] ;
	char File  [_MAX_FNAME] ;
	char Ext   [_MAX_EXT] ;
	char Res   [_MAX_PATH] ;

	char Piece [MAX_PIECE] [10] = { "WPAWN",
									"WROOK",
									"WKNIGHT",
									"WBISHOP",
									"WQUEEN",
									"BPAWN",
									"BROOK",
									"BKNIGHT",
									"BBISHOP",
									"BQUEEN",
									"WKING",
									"BKING"
								  } ;

	BOARDCFG_SquareToPosition(nBx, nBy, &nPx, &nPy) ;

	hdcMem = CreateCompatibleDC(hdc) ;
	SetPolyFillMode(hdcMem, WINDING) ;

	htSquare = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;
	htOld    = (HBITMAP) SelectObject(hdcMem, htSquare) ;

	if(ColorBoard [nBx] [nBy] == INDEX_WHITE)
	{
		if(BoardCfg.bVectorSquare)
		{
			BDRAW_SquareW1(hdcMem, BdClr [BD_WHITE_SQUARE_BORDER], BdClr [BD_WHITE_SQUARE]) ;
		}
		else
		{
			_splitpath(BoardCfg.cBitmapSquare, Drive, Dir, File, Ext) ;

			strcpy(Ext, BMP_EXT) ;
			strcpy(File, "WSQUARE") ;
			_makepath(Res, Drive, Dir, File, Ext) ;

			_chdir(TOOLBOX_GetMyDocumentPath()) ;

			htBitmap = LoadImage(NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
			if(htBitmap == NULL)
			{
				BoardCfg.bVectorSquare = 1 ;

				BDRAW_SquareW1(hdcMem, BdClr [BD_WHITE_SQUARE_BORDER], BdClr [BD_WHITE_SQUARE]) ;
			}
			else
			{
				GetObject(htBitmap, sizeof(BITMAP), (LPVOID) &bitmap) ;

				nS          = SetStretchBltMode(hdcMem, COLORONCOLOR) ;
				hdcBitmap   = CreateCompatibleDC(hdc) ;
				htOldBitmap = (HBITMAP) SelectObject(hdcBitmap, htBitmap) ;

				SetStretchBltMode(hdcMem, COLORONCOLOR) ;
				StretchBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcBitmap, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY) ;
				SetStretchBltMode(hdcMem, nS) ;

				SelectObject(hdcBitmap, htOldBitmap) ;
				DeleteDC(hdcBitmap) ;
				DeleteObject(htBitmap) ;
			}
		}
	}
	else
	{
		if(BoardCfg.bVectorSquare)
		{
			BDRAW_SquareB1(hdcMem, BdClr [BD_BLACK_SQUARE_BORDER], BdClr [BD_BLACK_SQUARE]) ;
		}
		else
		{
			_splitpath(BoardCfg.cBitmapSquare, Drive, Dir, File, Ext) ;

			strcpy(Ext, BMP_EXT) ;
			strcpy(File, "BSQUARE") ;
			_makepath(Res, Drive, Dir, File, Ext) ;

			_chdir(TOOLBOX_GetMyDocumentPath()) ;

			htBitmap = LoadImage(NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
			if(htBitmap == NULL)
			{
				BoardCfg.bVectorSquare = 1 ;

				BDRAW_SquareB1(hdcMem, BdClr [BD_BLACK_SQUARE_BORDER], BdClr [BD_BLACK_SQUARE]) ;
			}
			else
			{
				GetObject(htBitmap, sizeof(BITMAP), (LPVOID) &bitmap) ;

				nS          = SetStretchBltMode(hdcMem, COLORONCOLOR) ;
				hdcBitmap   = CreateCompatibleDC(hdc) ;
				htOldBitmap = (HBITMAP) SelectObject(hdcBitmap, htBitmap) ;

				SetStretchBltMode(hdcMem, COLORONCOLOR) ;
				StretchBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcBitmap, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY) ;
				SetStretchBltMode(hdcMem, nS) ;

				SelectObject(hdcBitmap, htOldBitmap) ;
				DeleteDC(hdcBitmap) ;
				DeleteObject(htBitmap) ;
			}
		}
	}

	bV = 1 ;

	if(nP == EMPTY_SQUARE)
	{
		BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;
	}
	else
	{
		if(BoardCfg.bVectorPiece)
		{
			BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;

			htMask = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

			SelectObject(hdcMem, htMask) ;
			BDRAW_PieceMask(hdcMem, nP) ;

			BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCAND) ;

			htPiece = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

			SelectObject(hdcMem, htPiece) ;
			BDRAW_Piece1(hdcMem, nP, BdClr [BD_WHITE_PIECE_BORDER], BdClr [BD_WHITE_PIECE], BdClr [BD_BLACK_PIECE_BORDER], BdClr [BD_BLACK_PIECE]) ;

			BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCPAINT) ;
		}
		else
		{
			_splitpath(BoardCfg.cBitmapPiece, Drive, Dir, File, Ext) ;

			strcpy(Ext, BMP_EXT) ;
			strcpy(File, Piece [nP]) ;
			_makepath(Res, Drive, Dir, File, Ext) ;

			_chdir(TOOLBOX_GetMyDocumentPath()) ;

			htBitmap = LoadImage(NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
			if(htBitmap == NULL)
			{
				BoardCfg.bVectorPiece = 1 ;

				BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;

				htMask = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

				SelectObject(hdcMem, htMask) ;
				BDRAW_PieceMask(hdcMem, nP) ;

				BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCAND) ;

				htPiece = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

				SelectObject(hdcMem, htPiece) ;
				BDRAW_Piece1(hdcMem, nP, BdClr [BD_WHITE_PIECE_BORDER], BdClr [BD_WHITE_PIECE], BdClr [BD_BLACK_PIECE_BORDER], BdClr [BD_BLACK_PIECE]) ;

				BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCPAINT) ;
			}
			else
			{
				bV = 0 ;

				GetObject(htBitmap, sizeof(BITMAP), (LPVOID) &bitmap) ;

				hdcBitmap   = CreateCompatibleDC(hdc) ;
				htOldBitmap = (HBITMAP) SelectObject(hdcBitmap, htBitmap) ;
				clrTran     = GetPixel(hdcBitmap, 0, 0) ;

				BoardCfg.htTemp = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

				hdcMem1 = CreateCompatibleDC(hdc) ;
				htOld1  = (HBITMAP) SelectObject(hdcMem1, BoardCfg.htTemp) ;

				nS = SetStretchBltMode(hdcMem1, COLORONCOLOR) ;
				SetStretchBltMode(hdcMem1, COLORONCOLOR) ;
				StretchBlt(hdcMem1, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcBitmap, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY) ;
				SetStretchBltMode(hdcMem1, nS) ;

				SelectObject(hdcMem1, htOld1) ;
				DeleteDC(hdcMem1) ;

				SelectObject(hdcBitmap, htOldBitmap) ;
				DeleteDC(hdcBitmap) ;
				DeleteObject(htBitmap) ;

				BDRAW_TransparentBlt(hdcMem, clrTran) ;
				DeleteObject(BoardCfg.htTemp) ;

				BitBlt(hdc, nPx, nPy, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;
			}
		}
	}

	SelectObject(hdcMem, htOld) ;
	DeleteDC(hdcMem) ;
	DeleteObject(htSquare) ;

	if(nP != EMPTY_SQUARE)
	{
		if(bV)
		{
			DeleteObject(htMask) ;
			DeleteObject(htPiece) ;
		}
	}
}

void BOARDCFG_DrawNumber(HDC hdc, int nN)
{
	HPEN hpTmp, hpOld ;

	if(nN > 1)
	{
		hpTmp = CreatePen(PS_SOLID, 0, BdClr [BD_PIECE_NUMBER]) ;
		hpOld = (HPEN) SelectObject(hdc, hpTmp) ;

		if(nN > 9)
		{
			MoveToEx(hdc, 1, BoardCfg.ptNumber [0].y, NULL) ;
			LineTo(hdc, 1, BoardCfg.ptNumber [2].y) ;

			nN = nN - 10 ;
		}

		switch(nN)
		{
			case 0 :
				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y) ;
				break ;

			case 1 :
				MoveToEx(hdc, BoardCfg.ptNumber [1].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [1].x, BoardCfg.ptNumber [2].y) ;
				break ;

			case 2 :
				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [1].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [1].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				break ;

			case 3 :
				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [2].y) ;

				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [1].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [1].y) ;
				break ;

			case 4 :
				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [1].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [1].y) ;

				MoveToEx(hdc, BoardCfg.ptNumber [1].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [1].x, BoardCfg.ptNumber [2].y) ;
				break ;

			case 5 :
				MoveToEx(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [1].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [1].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [2].y) ;
				break ;

			case 6 :
				MoveToEx(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [1].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [1].y) ;
				break ;

			case 7 :
				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				break ;

			case 8 :
				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [2].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y) ;

				MoveToEx(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [1].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [1].y) ;
				break ;

			case 9 :
				MoveToEx(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [1].y, NULL) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [1].y) ;
				LineTo(hdc, BoardCfg.ptNumber [0].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [0].y) ;
				LineTo(hdc, BoardCfg.ptNumber [2].x, BoardCfg.ptNumber [2].y) ;
				break ;
		}

		SelectObject(hdc, hpOld) ;
		DeleteObject(hpTmp) ;
	}
}

void BOARDCFG_DrawBufferPiece(HDC hdc, int nP, int nN)
{
	HDC      hdcMem, hdcMem1, hdcBitmap, hdcMem2 ;
	HBITMAP  htTmp, htSquare, htMask, htPiece, htOld, htOld1, htBitmap, htOldBitmap, htOld2 ;
	BITMAP   bitmap ;
	COLORREF clrTran ;
	int      nS, bV ;

	char Drive [_MAX_DRIVE] ;
	char Dir   [_MAX_DIR] ;
	char File  [_MAX_FNAME] ;
	char Ext   [_MAX_EXT] ;
	char Res   [_MAX_PATH] ;

	char Piece [MAX_PIECE] [10] = { "WPAWN",
									"WROOK",
									"WKNIGHT",
									"WBISHOP",
									"WQUEEN",
									"BPAWN",
									"BROOK",
									"BKNIGHT",
									"BBISHOP",
									"BQUEEN",
									"WKING",
									"BKING"
								  } ;

	hdcMem  = CreateCompatibleDC(hdc) ;
	hdcMem1 = CreateCompatibleDC(hdc) ;

	SetPolyFillMode(hdcMem,  WINDING) ;
	SetPolyFillMode(hdcMem1, WINDING) ;

	htTmp    = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;
	htSquare = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

	htOld    = (HBITMAP) SelectObject(hdcMem,  htTmp) ;
	htOld1   = (HBITMAP) SelectObject(hdcMem1, htSquare) ;

	if(BoardCfg.bVectorSquare)
	{
		BDRAW_SquareBF1(hdcMem1, BdClr [BD_BUFFER], BdClr [BD_BUFFER]) ;
	}
	else
	{
		_splitpath(BoardCfg.cBitmapSquare, Drive, Dir, File, Ext) ;

		strcpy(Ext, BMP_EXT) ;
		strcpy(File, "BUFFER") ;
		_makepath(Res, Drive, Dir, File, Ext) ;

		_chdir(TOOLBOX_GetMyDocumentPath()) ;

		htBitmap = LoadImage(NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
		if(htBitmap == NULL)
		{
			BoardCfg.bVectorSquare = 1 ;

			BDRAW_SquareBF1(hdcMem1, BdClr [BD_BUFFER], BdClr [BD_BUFFER]) ;
		}
		else
		{
			GetObject(htBitmap, sizeof(BITMAP), (LPVOID) &bitmap) ;

			nS          = SetStretchBltMode(hdcMem1, COLORONCOLOR) ;
			hdcBitmap   = CreateCompatibleDC(hdc) ;
			htOldBitmap = (HBITMAP) SelectObject(hdcBitmap, htBitmap) ;

			SetStretchBltMode(hdcMem1, COLORONCOLOR) ;
			StretchBlt(hdcMem1, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcBitmap, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY) ;
			SetStretchBltMode(hdcMem1, nS) ;

			SelectObject(hdcBitmap, htOldBitmap) ;
			DeleteDC(hdcBitmap) ;
			DeleteObject(htBitmap) ;
		}
	}

	BitBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcMem1, 0, 0, SRCCOPY) ;

	if(nN > 0)
	{
		bV = 1 ;

		if(BoardCfg.bVectorPiece)
		{
			htMask = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

			SelectObject(hdcMem1, htMask) ;
			BDRAW_PieceMask(hdcMem1, nP) ;

			BitBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcMem1, 0, 0, SRCAND) ;

			htPiece = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

			SelectObject(hdcMem1, htPiece) ;
			BDRAW_Piece1(hdcMem1, nP, BdClr [BD_WHITE_PIECE_BORDER], BdClr [BD_WHITE_PIECE], BdClr [BD_BLACK_PIECE_BORDER], BdClr [BD_BLACK_PIECE]) ;

			BitBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcMem1, 0, 0, SRCPAINT) ;

			BOARDCFG_DrawNumber(hdcMem, nN) ;
		}
		else
		{
			_splitpath(BoardCfg.cBitmapPiece, Drive, Dir, File, Ext) ;

			strcpy(Ext, BMP_EXT) ;
			strcpy(File, Piece [nP]) ;
			_makepath(Res, Drive, Dir, File, Ext) ;

			_chdir(TOOLBOX_GetMyDocumentPath()) ;

			htBitmap = LoadImage(NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
			if(htBitmap == NULL)
			{
				BoardCfg.bVectorPiece = 1 ;

				htMask = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

				SelectObject(hdcMem1, htMask) ;
				BDRAW_PieceMask(hdcMem1, nP) ;

				BitBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcMem1, 0, 0, SRCAND) ;

				htPiece = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

				SelectObject(hdcMem1, htPiece) ;
				BDRAW_Piece1(hdcMem1, nP, BdClr [BD_WHITE_PIECE_BORDER], BdClr [BD_WHITE_PIECE], BdClr [BD_BLACK_PIECE_BORDER], BdClr [BD_BLACK_PIECE]) ;

				BitBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcMem1, 0, 0, SRCPAINT) ;

				BOARDCFG_DrawNumber(hdcMem, nN) ;
			}
			else
			{
				bV = 0 ;

				GetObject(htBitmap, sizeof(BITMAP), (LPVOID) &bitmap) ;

				hdcBitmap   = CreateCompatibleDC(hdc) ;
				htOldBitmap = (HBITMAP) SelectObject(hdcBitmap, htBitmap) ;
				clrTran     = GetPixel(hdcBitmap, 0, 0) ;

				BoardCfg.htTemp = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;

				hdcMem2 = CreateCompatibleDC(hdc) ;
				htOld2  = (HBITMAP) SelectObject(hdcMem2, BoardCfg.htTemp) ;

				nS = SetStretchBltMode(hdcMem2, COLORONCOLOR) ;
				SetStretchBltMode(hdcMem2, COLORONCOLOR) ;
				StretchBlt(hdcMem2, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcBitmap, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY) ;
				SetStretchBltMode(hdcMem2, nS) ;

				SelectObject(hdcMem2, htOld2) ;
				DeleteDC(hdcMem2) ;

				SelectObject(hdcBitmap, htOldBitmap) ;
				DeleteDC(hdcBitmap) ;
				DeleteObject(htBitmap) ;

				BDRAW_TransparentBlt(hdcMem, clrTran) ;
				DeleteObject(BoardCfg.htTemp) ;

				BOARDCFG_DrawNumber(hdcMem, nN) ;
			}
		}
	}

	BitBlt(hdc,
		   BoardCfg.rBuffer.left + BoardCfg.ptBuffer [nP].x,
		   BoardCfg.rBuffer.top  + BoardCfg.ptBuffer [nP].y,
		   BoardCfg.nss,
		   BoardCfg.nss,
		   hdcMem,
		   0,
		   0,
		   SRCCOPY) ;

	SelectObject(hdcMem,  htOld) ;
	SelectObject(hdcMem1, htOld1) ;

	DeleteDC(hdcMem) ;
	DeleteDC(hdcMem1) ;

	DeleteObject(htTmp) ;
	DeleteObject(htSquare) ;

	if(nN > 0)
	{
		if(bV)
		{
			DeleteObject(htMask) ;
			DeleteObject(htPiece) ;
		}
	}
}

void BOARDCFG_DrawPosition(HDC hdc)
{
	HDC hdcMem, hdcBitmap ;
	HPEN hpTmp, hpTmp1, hpOld ;
	HBRUSH hbTmp, hbOld ;
	HBITMAP htBuffer, htOld, htBitmap, htOldBitmap ;
	BITMAP bitmap ;

	char Drive [_MAX_DRIVE] ;
	char Dir   [_MAX_DIR] ;
	char File  [_MAX_FNAME] ;
	char Ext   [_MAX_EXT] ;
	char Res   [_MAX_PATH] ;

	int nY, nX, nW, nH, nW1, nC, nS ;
	int Bd [8] [8] ;
	char cTmp [100], *cP ;

	strcpy(cTmp, INIT_BOARD) ;
	cP = cTmp ;
	for(nY = 7 ; nY >= 0 ; nY--)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			Bd [nX] [nY] = BOARD_FICS_To_Int(*cP++) ;
		}
		*cP++ ;
	}

	Bd [4] [1] = EMPTY_SQUARE ;
	Bd [4] [3] = WHITE_PAWN ;

	nW  = (BoardCfg.rBoard.right - BoardCfg.rBoard.left) + 1 ;
	nH  = (BoardCfg.rBoard.bottom - BoardCfg.rBoard.top) + 1 ;

	hpTmp  = CreatePen(PS_SOLID, 0, BdClr [BD_BOARD_BORDER]) ;
	hpTmp1 = CreatePen(PS_SOLID, 0, BdClr [BD_BUFFER_BORDER]) ;
	hbTmp  = CreateSolidBrush(BdClr [BD_BUFFER]) ;

	hpOld = (HPEN) SelectObject(hdc, hpTmp) ;

	Rectangle(hdc,
			  BoardCfg.rBoard.left,
			  BoardCfg.rBoard.top,
			  BoardCfg.rBoard.left + nW,
			  BoardCfg.rBoard.top  + nH) ;

	nW1 = (BoardCfg.rBuffer.right - BoardCfg.rBuffer.left) + 1 ;

	hbOld = (HBRUSH) SelectObject(hdc, hbTmp) ;
	SelectObject(hdc, hpTmp1) ;

	Rectangle(hdc,
			  BoardCfg.rBoard.left + nW,
			  BoardCfg.rBoard.top,
			  BoardCfg.rBoard.left + nW + nW1,
			  BoardCfg.rBoard.top + nH) ;

	SelectObject(hdc, hbOld) ;

	for(nY = 0 ; nY < 8 ; nY++)
	{
		for(nX = 0 ; nX < 8 ; nX++)
		{
			BOARDCFG_DrawBoardPiece(hdc, nX, nY, Bd [nX] [nY]) ;
		}
	}

	for(nX = WHITE_PAWN ; nX <= BLACK_QUEEN ; nX++)
	{
		switch(nX)
		{
			case WHITE_PAWN :
			case BLACK_PAWN :
				nC = 5 ;
				break ;

			case WHITE_KNIGHT :
			case BLACK_KNIGHT :
				nC = 4 ;
				break ;

			case WHITE_BISHOP :
			case BLACK_BISHOP :
				nC = 3 ;
				break ;

			case WHITE_ROOK :
			case BLACK_ROOK :
				nC = 2 ;
				break ;

			case WHITE_QUEEN :
			case BLACK_QUEEN :
				nC = 1 ;
				break ;

			default :
				nC = 0 ;
				break ;
		}

		BOARDCFG_DrawBufferPiece(hdc, nX, nC) ;
	}

	hdcMem = CreateCompatibleDC(hdc) ;
	SetPolyFillMode(hdcMem, WINDING) ;

	htBuffer = CreateCompatibleBitmap(hdc, BoardCfg.nss, BoardCfg.nss) ;
	htOld    = (HBITMAP) SelectObject(hdcMem, htBuffer) ;

	if(BoardCfg.bVectorSquare)
	{
		BDRAW_SquareBF1(hdcMem, BdClr [BD_BUFFER], BdClr [BD_BUFFER]) ;
	}
	else
	{
		_splitpath(BoardCfg.cBitmapSquare, Drive, Dir, File, Ext) ;

		strcpy(Ext, BMP_EXT) ;
		strcpy(File, "BUFFER") ;
		_makepath(Res, Drive, Dir, File, Ext) ;

		_chdir(TOOLBOX_GetMyDocumentPath()) ;

		htBitmap = LoadImage(NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
		if(htBitmap == NULL)
		{
			BDRAW_SquareBF1(hdcMem, BdClr [BD_BUFFER], BdClr [BD_BUFFER]) ;
		}
		else
		{
			GetObject(htBitmap, sizeof(BITMAP), (LPVOID) &bitmap) ;

			nS          = SetStretchBltMode(hdcMem, COLORONCOLOR) ;
			hdcBitmap   = CreateCompatibleDC(hdc) ;
			htOldBitmap = (HBITMAP) SelectObject(hdcBitmap, htBitmap) ;

			SetStretchBltMode(hdcMem, COLORONCOLOR) ;
			StretchBlt(hdcMem, 0, 0, BoardCfg.nss, BoardCfg.nss, hdcBitmap, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY) ;
			SetStretchBltMode(hdcMem, nS) ;

			SelectObject(hdcBitmap, htOldBitmap) ;
			DeleteDC(hdcBitmap) ;
			DeleteObject(htBitmap) ;
		}
	}

	BitBlt(hdc, BoardCfg.rBuffer.left + BoardCfg.ptBuffer [BLACK_QUEEN].x,  BoardCfg.rBuffer.top + BoardCfg.ptBuffer [BLACK_BISHOP].y,                               BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;
	BitBlt(hdc, BoardCfg.rBuffer.left + BoardCfg.ptBuffer [BLACK_BISHOP].x, BoardCfg.rBuffer.top + BoardCfg.ptBuffer [BLACK_BISHOP].y + BoardCfg.nss,                BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;
	BitBlt(hdc, BoardCfg.rBuffer.left + BoardCfg.ptBuffer [BLACK_QUEEN].x,  BoardCfg.rBuffer.top + BoardCfg.ptBuffer [BLACK_BISHOP].y + BoardCfg.nss,                BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;
	BitBlt(hdc, BoardCfg.rBuffer.left + BoardCfg.ptBuffer [BLACK_BISHOP].x, BoardCfg.rBuffer.top + BoardCfg.ptBuffer [BLACK_BISHOP].y + BoardCfg.nss + BoardCfg.nss, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;
	BitBlt(hdc, BoardCfg.rBuffer.left + BoardCfg.ptBuffer [BLACK_QUEEN].x,  BoardCfg.rBuffer.top + BoardCfg.ptBuffer [BLACK_BISHOP].y + BoardCfg.nss + BoardCfg.nss, BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;
	BitBlt(hdc, BoardCfg.rBuffer.left + BoardCfg.ptBuffer [WHITE_QUEEN].x,  BoardCfg.rBuffer.top + BoardCfg.ptBuffer [WHITE_BISHOP].y,                               BoardCfg.nss, BoardCfg.nss, hdcMem, 0, 0, SRCCOPY) ;

	SelectObject(hdcMem, htOld) ;
	DeleteDC(hdcMem) ;
	DeleteObject(htBuffer) ;

	BOARDCFG_DrawFromToSquareHighlight(hdc) ;

	SelectObject(hdc, hpOld) ;

	DeleteObject(hpTmp) ;
	DeleteObject(hpTmp1) ;
	DeleteObject(hbTmp) ;
}

void BOARDCFG_DrawAll(HWND hwnd, HDC hdc)
{
	RECT rc ;
	HDC hdcWindow ;
	HBITMAP htWindow, htOld ;
	HPEN hpTmp, hpOld ;
	HBRUSH hbTmp, hbOld ;

	GetClientRect(hwnd, &rc) ;

	hdcWindow = CreateCompatibleDC(hdc) ;
	htWindow  = CreateCompatibleBitmap(hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
	htOld     = (HBITMAP) SelectObject(hdcWindow, htWindow) ;

	hpTmp = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNFACE)) ;
	hbTmp = CreateSolidBrush(GetSysColor(COLOR_BTNFACE)) ;
	hpOld = (HPEN)   SelectObject(hdcWindow, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdcWindow, hbTmp) ;
	Rectangle(hdcWindow, rc.left, rc.top, rc.right, rc.bottom) ;
	SelectObject(hdcWindow, hpOld) ;
	SelectObject(hdcWindow, hbOld) ;
	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;

	BOARDCFG_Resize(hdcWindow, BD_RIGHT, BD_BOTTOM) ;

	hpTmp = CreatePen(PS_SOLID, 0, BdClr [BD_GAME_WINDOW]) ;
	hbTmp = CreateSolidBrush(BdClr [BD_GAME_WINDOW]) ;

	hpOld = (HPEN)   SelectObject(hdcWindow, hpTmp) ;
	hbOld = (HBRUSH) SelectObject(hdcWindow, hbTmp) ;

	Rectangle(hdcWindow, BD_LEFT, BD_TOP, BD_LEFT + BD_RIGHT, BD_TOP + BD_BOTTOM) ;

	BOARDCFG_DrawPosition(hdcWindow) ;
	BOARDCFG_DrawHandle(hdcWindow) ;
	BOARDCFG_DrawWhiteLag(hdcWindow) ;
	BOARDCFG_DrawBlackLag(hdcWindow) ;
	BOARDCFG_DrawWhiteClock(hdcWindow) ;
	BOARDCFG_DrawBlackClock(hdcWindow) ;
	BOARDCFG_DrawGameType(hdcWindow) ;
	BOARDCFG_DrawLastMove(hdcWindow) ;
	BOARDCFG_DrawResult(hdcWindow) ;
	BOARDCFG_DrawPtell(hdcWindow) ;
	BOARDCFG_DrawGinfo(hdcWindow) ;
	BOARDCFG_DrawCoordinates(hdcWindow, 0) ;
	BOARDCFG_DrawTruePremove(hdcWindow) ;
	BOARDCFG_DrawIllegalTruePremove(hdcWindow) ;
	BOARDCFG_DrawKingHighlight(hdcWindow) ;

	SelectObject(hdcWindow, hpOld) ;
	SelectObject(hdcWindow, hbOld) ;

	DeleteObject(hpTmp) ;
	DeleteObject(hbTmp) ;

	BitBlt(hdc, BD_LEFT, BD_TOP, BD_RIGHT, BD_BOTTOM, hdcWindow, BD_LEFT, BD_TOP, SRCCOPY) ;

	SelectObject(hdcWindow, htOld) ;
	DeleteObject(hdcWindow) ;
	DeleteObject(htWindow) ;
}

void BOARDCFG_Refresh(HWND hwnd)
{
	HDC hdc ;

	hdc = GetDC(hwnd) ;
	BOARDCFG_DrawAll(hwnd, hdc) ;
	ReleaseDC(hwnd, hdc) ;
}
