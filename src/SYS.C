#include "thief.h"

void SYS_InitVal(void)
{
	APPBARDATA abd;
	int nL, nI, nW, nH, nW1, nH1;

	char cTmp  [_MAX_DIR];
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Res   [_MAX_PATH];
	char Path  [20] [99] = { "WAVE",
							 "LAYOUT",
							 "LOG",
							 "GAME_LOGS",
							 "GAME_LOGS\\MY_GAMES",
							 "GAME_LOGS\\OBSERVED_GAMES",
							 "GAME_LOGS\\OTHER_GAMES",
							 "PROFILE",
							 "BUGSTUFF",
							 "BUGSTUFF\\SQUARE",
							 "BUGSTUFF\\PIECE",
							 "BUGSTUFF\\PIECE\\BITMAP",
							 "BUGSTUFF\\PIECE\\VECTOR",
							 "BUGSTUFF\\PIECE\\VECTOR\\TOUPS",
							 "BUGSTUFF\\PIECE\\VECTOR\\NICOK",
							 "BUGSTUFF\\PIECE\\VECTOR\\KBIRK",
							 "BUGSTUFF\\PIECE\\VECTOR\\JHURT",
							 "BUGSTUFF\\PIECE\\VECTOR\\ECHUR",
							 "BUGSTUFF\\PIECE\\VECTOR\\DFONG",
							 "BUGSTUFF\\PIECE\\VECTOR\\USER"
						   };

	_tzset();

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	for(nI = 0 ; nI < 20 ; nI++)
	{
		strcpy(cTmp, Dir);

		nL = strlen(cTmp);
		if(nL == 0)
		{
			strcat(cTmp, "\\");
			strcat(cTmp, Path [nI]);
			strcat(cTmp, "\\");
		}
		else
		{
			if(cTmp [nL - 1] == '\\')
			{
				strcat(cTmp, Path [nI]);
				strcat(cTmp, "\\");
			}
			else
			{
				strcat(cTmp, "\\");
				strcat(cTmp, Path [nI]);
				strcat(cTmp, "\\");
			}
		}
		_makepath(Res, Drive, cTmp, "", "");
		_mkdir(Res);

		if(nI == 2)
		{
			strcpy(Browse.cLogBrowse, Res);
		}
		else if(nI == 4)
		{
			strcpy(Browse.cSaveBPGNBrowse, Res);
			strcpy(Browse.cSavePGNBrowse,  Res);
		}
		else if(nI == 7)
		{
			strcpy(Browse.cProfileBrowse, Res);
		}
		else if(nI == 9)
		{
			strcpy(Browse.cSquareBrowse, Res);
		}
		else if(nI == 11)
		{
			strcpy(Browse.cPieceBrowse, Res);
		}
	}

	_makepath(Res, Drive, Dir, "", "");
	strcpy(Browse.cConnectionBrowse, Res);
	strcpy(Browse.cPGNBrowse,        "");
	strcpy(Browse.cBPGNBrowse,       "");

	Telnet.cfTelnet.cbSize    = sizeof(CHARFORMAT);
	Telnet.cfItalic.cbSize    = sizeof(CHARFORMAT);
	Telnet.cfUnderline.cbSize = sizeof(CHARFORMAT);

	COLOR_Init();
	FONT_Init();

	abd.cbSize = sizeof(APPBARDATA);
	SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

	nW  = GetSystemMetrics(SM_CXSCREEN);
	nH  = GetSystemMetrics(SM_CYSCREEN);
	nW1 = nW >> 1;
	nH1 = nH >> 1;

	if(abd.rc.left > nW1)
	{
		wCoord [COORD_MAIN].x = 0;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].w = abd.rc.left;
		wCoord [COORD_MAIN].h = nH;
	}
	else if(abd.rc.top > nH1)
	{
		wCoord [COORD_MAIN].x = 0;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].w = nW;
		wCoord [COORD_MAIN].h = abd.rc.top;
	}
	else if(abd.rc.right > nW1)
	{
		wCoord [COORD_MAIN].x = 0;
		wCoord [COORD_MAIN].y = abd.rc.bottom;
		wCoord [COORD_MAIN].w = nW;
		wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
	}
	else
	{
		wCoord [COORD_MAIN].x = abd.rc.right;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
		wCoord [COORD_MAIN].h = nH;
	}
	wCoord [COORD_MAIN].s = WIN_SHOW;

	LOG_Init();
	SERVER_Init();
	CHESS_Init();
	BUTTON_Init();
	SOUND_Init();
	EAR_Init();
	GAMESOUND_Init();
	TIMESOUND_Init();
	LOGINEVENT_Init();
	GAMEEVENT_Init();
	RIGHTMOUSE_Init();
	HISTORY_Init();
	FKEY_Init();

	strcpy(F8Key.cSitPieceDropMove, "");
	F8KEY_Init();

	F9KEY_Init();
	CONTROLKEY_Init();
	REDCLOCK_Init();
	TIMERCMD_Init();
	VARS_Init();
	CSET_Init();
	GAME_InitAllGame(1);

	System.nGameIndex            = 0;
	System.bMaximizeMain         = 0;
	System.bIsMyTurn             = 0;
	System.nIssuedMovesCommand   = 0;
	System.nMoveListIndexNumber  = -1;
	System.bIncludeSpace         = 0;
	System.bInDialogBox          = 0;
	System.nRightMouseIndex      = RIGHT_MOUSE_BUTTON;
	System.bFirstTime            = 1;
	System.bOkToResizePlay       = 0;
	System.hwndLastGame          = NULL;
	System.bUnobserve            = 0;
	System.nUnobGameNumber       = 0;
	strcpy(System.cUnobWname,   "");
	strcpy(System.cUnobBname,   "");
	strcpy(System.cSaveBuffer,  "");
	strcpy(System.cLoginFile,   "");

	System.nSysPlayGameType      = ICS_INITIAL_GAMETYPE;
	System.nSysExamGameType      = ICS_INITIAL_GAMETYPE;
	System.nSysObserveGameType   = ICS_INITIAL_GAMETYPE;

	System.nLastPlayGameType     = System.nSysPlayGameType;
	System.nLastExamGameType     = System.nSysExamGameType;
	System.nLastObserveGameType  = System.nSysObserveGameType;

	System.nCensorIndex          = 0;
	System.bIssuedAbort          = 0;
	System.bICCLastGameWasWild16 = 0;
	System.bPromoteCommand       = 0;
	System.nPromoteWPiece        = 0;
	System.nPromoteBPiece        = 0;
	System.bPromoteKnight        = 0;
	System.nTryList              = 0;
	System.nTryScreen            = 0;
	System.nTryTaskbar           = 0;
	System.nTryPlay              = 0;
	System.bInPromote            = 0;
	System.hwndPromote           = NULL;
	System.bQuickConnection      = 0;
	System.bCDROMConnection      = 0;

	LagCmd.nLagTolerance         = ICS_LAG_TOLERANCE;
	LagCmd.nLagThreshold         = ICS_LAG_CHECK_THRESHOLD;
	LagCmd.nLagLThreshold        = LagCmd.nLagThreshold * 1000;
	LagCmd.nType                 = ICS_LAG_FUNCTION;
	strcpy(LagCmd.cLagCommand, ICS_LAG_COMMAND);
	strcpy(LagCmd.cLagDisplay, ICS_LAG_DISPLAY);

	User.bLogTelnet              = DEFAULT_LOG_TELNET;

	User.bLogGame                = DEFAULT_LOG_GAME;
	User.bSavePlayGame           = DEFAULT_SAVE_PLAY_GAME;
	User.bSaveObserveGame        = DEFAULT_SAVE_OBS_GAME;
	User.bSaveUnobserveGame      = DEFAULT_SAVE_UNOBS_GAME;
	User.bSaveAbortGame          = DEFAULT_SAVE_ABORT_GAME;
	User.bSaveNoBFENBugGame      = DEFAULT_SAVE_NO_BFEN_GAME;
	User.bSaveDaily              = DEFAULT_SAVE_DAILY;

	strcpy(User.cPGNViewer,  "");
	strcpy(User.cBPGNViewer, "");

	User.bAutoFlag               = DEFAULT_AUTO_FLAG;
	User.bAutoRematch            = DEFAULT_AUTO_REMATCH;
	User.bAutoObserve            = DEFAULT_AUTO_OBSERVE;
	User.bAutoObserveGNotify     = DEFAULT_AUTO_OBSERVE_GNOTIFY;
	User.bActualSize             = DEFAULT_ACTUAL_SIZE;
	User.bStopResizePlay         = DEFAULT_STOP_PLAY_RESIZE;
	User.bEscCancelDragMove      = DEFAULT_ESC_CANCEL_DRAG_MOVE;
	User.bEscCancelTruePremove   = DEFAULT_ESC_CANCEL_TRUE_PREMOVE;
	User.bEscCancelSitDropMove   = DEFAULT_ESC_CANCEL_SIT_DROP_MOVE;
	User.bEscCancelPromoteKnight = DEFAULT_ESC_CANCEL_PROMOTE_KNIGHT;
	User.bNoPlayAbuser           = DEFAULT_NO_PLAY_ABUSER;
	User.bNoPlayComputer         = DEFAULT_NO_PLAY_COMPUTER;
	User.nPlayWild5Flip          = DEFAULT_WILD5_PLAY_DEFAULT;
	User.bShowResult             = DEFAULT_SHOW_RESULT;
	User.bShowRating             = DEFAULT_SHOW_RATING;
	User.bShowLagStat            = DEFAULT_SHOW_LAGSTAT;
	User.bShowPtell              = DEFAULT_SHOW_PTELL;
	User.bShowCoord              = DEFAULT_SHOW_COORDINATES;
	User.bShowGameType           = DEFAULT_SHOW_GAMETYPE;
	User.bShowGameTypeOnTitleBar = DEFAULT_SHOW_GAMETYPE_TITLE;
	User.bShowValueOnTitleBar    = DEFAULT_SHOW_VALUE_TITLE;
	User.bShowLastMove           = DEFAULT_SHOW_LASTMOVE;
	User.bShowLastMoveTenth      = DEFAULT_SHOW_LASTMOVE_TENTH;
	User.bShowHighlight          = DEFAULT_SHOW_HIGHLIGHT;
	User.bShowTPHighlight        = DEFAULT_TRUEMOVE_HIGHLIGHT;
	User.bShowClockOnTop         = DEFAULT_SHOW_CLOCK_ON_TOP;

	User.nShowKingHighlightPM    = DEFAULT_KING_HIGHLIGHT_PM;
	User.nShowKingHighlightPO    = DEFAULT_KING_HIGHLIGHT_PM;
	User.nShowKingHighlightE     = DEFAULT_KING_HIGHLIGHT_E;
	User.nShowKingHighlightO     = DEFAULT_KING_HIGHLIGHT_O;

	User.bShowKingHighlight = (User.nShowKingHighlightPM ||
							   User.nShowKingHighlightPO ||
							   User.nShowKingHighlightE  ||
							   User.nShowKingHighlightO);

	User.nShowCapturedChessPiece = DEFAULT_SHOW_CAPTURED_CHESS;
	User.nBufferOrientation      = DEFAULT_BUFFER_ORIENTATION;

	User.bVectorPiece            = 0;
	User.bVectorSquare           = 0;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

//  strcpy (Ext,  BMP_EXT);
//  strcpy (File, "WPAWN");
//  strcpy (cTmp, Dir);
//  strcat (cTmp, "..\\BUGSTUFF\\PIECE\\BITMAP\\WCN\\");
//  _makepath (User.cBitmapPiece, Drive, cTmp, File, Ext);

	sprintf(User.cBitmapPiece, "BUGSTUFF\\PIECE\\BITMAP\\WCN\\WPAWN%s", BMP_EXT);

//  strcpy (Ext,  BMP_EXT);
//  strcpy (File, "WSQUARE");
//  strcpy (cTmp, Dir);
//  strcat (cTmp, "..\\BUGSTUFF\\SQUARE\\STONE2\\");
//  _makepath (User.cBitmapSquare, Drive, cTmp, File, Ext);

	if(strlen(User.cBitmapSquare) > 0)
	{
		sprintf(User.cBitmapSquare, "BUGSTUFF\\SQUARE\\STONE2\\WSQUARE%s", BMP_EXT);
	}

	User.nPieceSet                 = DEFAULT_VECTOR_PIECE_SET;
	User.bAutoQueen                = DEFAULT_AUTO_QUEEN;
	User.nMoveType                 = DEFAULT_MOVE_TYPE;
	User.bAutoCenterDragPiece      = DEFAULT_AUTO_CENTER_DRAG_PC;
	User.bSmartMove                = DEFAULT_SMART_MOVE;
	User.bSmartMove1               = DEFAULT_SMART_MOVE1;
	User.bSmartMove1               = DEFAULT_SMART_MOVE2;
	User.bTruePremove              = DEFAULT_TRUE_PREMOVE;
	User.nTruePremove              = DEFAULT_PREMOVE_TYPE;
	User.bIllegalTPBeep            = DEFAULT_ILLEGAL_TP_BEEP;
	User.bRightMousePlay           = DEFAULT_RIGHT_MOUSE_PLAY;
	User.bRightMousePartner        = DEFAULT_RIGHT_MOUSE_PARTNER;
	User.bTestLegalPlay            = DEFAULT_TEST_LEGAL_PLAY;
	User.bTestLegalPartner         = DEFAULT_TEST_LEGAL_PARTNER;
	User.bAnimateObserve           = DEFAULT_ANIMATE_OBSERVE;
	User.bAnimateExam              = DEFAULT_ANIMATE_EXAM;
	User.bAnimatePlay              = DEFAULT_ANIMATE_PLAY;
	User.nAnimateSpeedObserve      = DEFAULT_ANIMATE_SPEED;
	User.nAnimateSpeedExam         = DEFAULT_ANIMATE_SPEED;
	User.nAnimateSpeedPlay         = DEFAULT_ANIMATE_SPEED;
	User.bLongPartner              = DEFAULT_LONG_PARTNER;
	User.bShowMoveButton           = DEFAULT_SHOW_MOVE_BUTTON;
	User.bUseArrowKeyMoveButton    = DEFAULT_ARROW_MOVE_BUTTON;
	User.bMouseWheel               = DEFAULT_MOUSE_WHEEL;
	User.bShowUnderline            = DEFAULT_SHOW_UNDERLINE;
	User.bEnterHighlight           = DEFAULT_TELNET_HIGHLIGHT;
	User.bTelnetResizeGoBottom     = DEFAULT_TELNET_GO_BOTTOM;
	User.bSuppressAnnouncement     = DEFAULT_TELNET_NO_ANNOUNCE;
	User.bSuppressCompKibitz       = DEFAULT_TELNET_GO_CKIBITZ;
	User.bSuppressCompWhisper      = DEFAULT_TELNET_NO_CWHISPER;
	User.bSuppressIgnoreFormula    = DEFAULT_TELNET_NO_IGNOREFOR;
	User.bSuppressROBOWelcome      = DEFAULT_TELNET_NO_ROBOWELCOME;
	User.bSuppressCompChTell       = DEFAULT_TELNET_NO_COMP_CHTELL;
	User.bSuppressCompTell         = DEFAULT_TELNET_NO_COMP_TELL;
	User.bSuppressCompShout        = DEFAULT_TELNET_NO_COMP_SHOUT;
	User.bTimeStampTelnet		   = DEFAULT_TELNET_CONSOLE_TIMESTAMP;
	User.bTimeStampLog			   = DEFAULT_TELNET_LOG_TIMESTAMP;
	User.bSound                    = DEFAULT_SOUND;
	User.bTelnetSound              = DEFAULT_TELNET_SOUND;
	User.bGameSound                = DEFAULT_GAME_SOUND;
	User.bEarSound                 = DEFAULT_EAR_SOUND;
	User.bFinalCountDown           = DEFAULT_FINAL_COUNTDOWN;
	User.bLagCommand               = DEFAULT_LAG_COMMAND;
	User.bExtendedButton           = DEFAULT_EXTENDED_BUTTON;
	User.bSaveOnExit               = DEFAULT_SAVE_ON_EXIT;
	User.bMinimizetoTray           = DEFAULT_MINIMIZE_TO_TRAY;
	User.bShowBitmapSize           = DEFAULT_SHOW_BITMAP_SIZE;
	User.bShowMenuPath             = DEFAULT_SHOW_MENU_PATH;

	User.bButtonCommandAddHist     = 1;
	User.bPlayCommandAddHist       = 1;
	User.bObserveCommandAddHist    = 1;
	User.bTelnetTextCommandAddHist = 1;
	User.bTelnetEditCommandAddHist = 1;
	User.bLEventCommandAddHist     = 1;
	User.bGEventCommandAddHist     = 1;
	User.bFKeyCommandAddHist       = 1;
	User.bCKeyCommandAddHist       = 1;
	User.bSeekCommandAddHist       = 1;
	User.bMatchCommandAddHist      = 1;
	User.bTellCommandAddHist       = 1;
	User.bMessageCommandAddHist    = 1;
	User.bTimerCommandAddHist      = 1;
	User.bLagCommandAddHist        = 1;
	User.bQuestionCommandAddHist   = 1;
	User.bEarPartner               = 1;
	User.bFICSPlayCloseAll         = 0;
	User.bFICSPlayEndPartner       = 0;
	User.bICCPlayCloseAll          = 1;
	User.bICCPlayEndPartner        = 0;
	User.bNONFICSPlayCloseAll      = 0;
	User.bNONFICSPlayEndPartner    = 0;
	User.bEat12GameNotFoundLine    = 1;
	User.bEatb1GameNotFoundLine    = 1;
	User.bRMNoHLSetTelnetHandle    = 1;
	User.bShowGameTypeOnTitleBar   = 0;
	User.bMessageMouse             = 0;
	User.bIamAGuest                = 0;
	User.bColoredClocks			   = 1;

	User.bSuppressExitPrompt  = 0;
	User.bUseXTellForLastTell = 0;

	strcpy(User.cNotepad, "notepad.exe");

	Login.nLoginString = 0;
	for(nI = 0 ; nI < MAX_LOGIN ; nI++)
	{
		strcpy(Login.cLoginString [nI], "");
	}

	strcpy(Login.cLoginFile,     "");
	strcpy(Login.cLoginName,     "");
	strcpy(Login.cLoginHandle,   "");
	strcpy(Login.cLoginPassword, "");
	strcpy(Login.cLoginScript,   "");
	strcpy(Login.cServerName,    "FICS");

	Telnet.bHasESC              = 0;
	Telnet.bFoundHandle         = 0;
	Telnet.bTelnetSkipLine      = 0;
	Telnet.bLastTelnetIsCR      = 0;
	Telnet.bLastIsTelnetEdit    = 0;
	Telnet.bLastChannel         = 1;
	Telnet.nLastChannel         = 999;
	Telnet.nCurrentSound        = SOUND_NONE;
	Telnet.bDisplayLine         = 1;
	Telnet.bLastIsChallenge     = 0;
	Telnet.bForceVisible        = 0;
	Telnet.bMissingQuote        = 0;
	Telnet.bMissingBracket      = 0;
	Telnet.bMissinghttpwww      = 0;
	Telnet.bDisplayContinueLine = 1;
	Telnet.bSkipDisplayLine     = 0;
	Telnet.bTellTold            = 0;
	Telnet.bPtellTold           = 0;
	Telnet.bSayTold             = 0;
	Telnet.bPartnerTold         = 0;
	Telnet.nTellTold            = 0;
	Telnet.nPtellTold           = 0;
	Telnet.nSayTold             = 0;
	Telnet.nPartnerTold         = 0;

	strcpy(Telnet.cTellTold,     "");
	strcpy(Telnet.cPtellTold,    "");
	strcpy(Telnet.cSayTold,      "");
	strcpy(Telnet.cPartnerTold,  "");
	strcpy(Telnet.cLastGameInfo, "");

	Timeseal.bSocketIsOpen = 0;
	Timeseal.nSocketLogin  = 0;
	Timeseal.bBeforeLogin  = 1;
	Timeseal.bSocketLine   = 0;
	strcpy(Timeseal.cSocketLine, "");

	Fics.bPobserve = 1;
	Fics.nPobserve = 0;
	strcpy(Fics.cPobserve, "");

	Icc.bIssuedBugCommand = 0;
	Icc.nIssuedBugCommand = 0;
	Icc.bStartBugGameList = 0;
	Icc.nFirstBugGame     = 0;
	Icc.nSecondBugGame    = 0;
	Icc.bSecondBugFlip    = 0;
	Icc.bFindICCPartner   = 0;
	strcpy(Icc.cICCPartner, "");

	DragInfo.bFromBoard  = 0;
	DragInfo.ptFrom.x    = -1;
	DragInfo.ptFrom.y    = -1;
	DragInfo.ptCurrent.x = -1;
	DragInfo.ptCurrent.y = -1;
	DragInfo.ptLast.x    = -1;
	DragInfo.ptLast.y    = -1;
	DragInfo.nIndex      = -1;
	DragInfo.nPc         = EMPTY_SQUARE;
	DragInfo.nClicked    = 0;

	LeftMouseDrop.nI = -1;
	LeftMouseDrop.nX = -1;
	LeftMouseDrop.nY = -1;

	ANIMATE_Init();
	ASSIST_Init();

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

	strcpy(Match.cMatchHandle, "");
	Match.bMatchMouse     = 0;
	Match.nMatchTime      = DEFAULT_MATCH_TIME;
	Match.nMatchIncrement = DEFAULT_MATCH_INCREMENT;
	Match.bMatchRated     = DEFAULT_MATCH_RATED;
	Match.nMatchType      = DEFAULT_MATCH_TYPE;
	Match.nICCMatchType   = DEFAULT_ICC_MATCH_TYPE;
	Match.nMatchColor     = DEFAULT_MATCH_COLOR;

	Seek.nSeekTime      = DEFAULT_SEEK_TIME;
	Seek.nSeekIncrement = DEFAULT_SEEK_INCREMENT;
	Seek.nSeekRating1   = DEFAULT_SEEK_RATING1;
	Seek.nSeekRating2   = DEFAULT_SEEK_RATING2;
	Seek.nSeekType      = DEFAULT_SEEK_TYPE;
	Seek.nICCSeekType   = DEFAULT_ICC_SEEK_TYPE;
	Seek.nSeekColor     = DEFAULT_SEEK_COLOR;
	Seek.bSeekManual    = DEFAULT_SEEK_MANUAL;
	Seek.bSeekFormula   = DEFAULT_SEEK_FORMULA;
	Seek.bSeekRated     = DEFAULT_SEEK_RATED;

	for(nI = 0 ; nI < MAX_UNDERLINE_STRING ; nI++)
	{
		TelnetUnderSize [nI] = strlen(TelnetUnderLine [nI]);
	}

	for(nI = 0 ; nI < MAX_BRACKET_STRING ; nI++)
	{
		TelnetBracketSize [nI] = strlen(TelnetBracket [nI]);
	}

	for(nI = 0 ; nI < MAX_MAMER_STRING ; nI++)
	{
		MamerStringSize [nI] = strlen(MamerString [nI]);
	}

	BUGHOUSE_Init();
	CENSOR_Init();
	SILENCE_Init();
	GNOTIFY_Init();
}

void SYS_InitCoord(HWND hwnd)
{
	RECT rc;
	WINDOWPLACEMENT wp;
	int nW, nH, nW1, nH1, nOW, nOH, nI;

	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Exs   [_MAX_EXT];
	char Ini   [_MAX_PATH];
	char Sys   [_MAX_PATH];

	GetClientRect(hwnd, &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	nOW = 1024;
	nOH = 740;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);
	strcpy(Ext, INI_EXT);
	strcpy(Exs, SYS_EXT);

	_makepath(Ini, Drive, Dir, File, Ext);
	if(! TOOLBOX_CanOpenFile(Ini))
	{
		INI_WriteSetup(Ini);
	}

	_makepath(Sys, Drive, Dir, File, Exs);
	if(! TOOLBOX_CanOpenFile(Sys))
	{
		INI_WriteSystem(Sys);
	}

	for(nI = 0 ; nI < MAX_LAYOUT ; nI++)
	{
		if(! TOOLBOX_CanOpenFile(LAYOUT_File(nI)))
		{
			LAYOUT_CreateDefaultLayout(nW, nH, nW1, nH1, nOW, nOH, nI);
			LAYOUT_Save(nI);
		}
	}
}

void SYS_CreateVal(void)
{
	int nI;

	hpPen [PEN_BOARD_OUTLINE]        = CreatePen(PS_SOLID, 0, clrColor [CLR_BOARD_OUTLINE]);
	hpPen [PEN_BUFFER_OUTLINE]       = CreatePen(PS_SOLID, 0, clrColor [CLR_BUFFER_OUTLINE]);
	hpPen [PEN_PIECE_NUMBER_OUTLINE] = CreatePen(PS_SOLID, 0, clrColor [CLR_PIECE_NUMBER]);
	hpPen [PEN_WHITE_SQUARE_OUTLINE] = CreatePen(PS_SOLID, 0, clrColor [CLR_WHITE_SQUARE_OUTLINE]);
	hpPen [PEN_BLACK_SQUARE_OUTLINE] = CreatePen(PS_SOLID, 0, clrColor [CLR_BLACK_SQUARE_OUTLINE]);
	hpPen [PEN_WHITE_PIECE_OUTLINE]  = CreatePen(PS_SOLID, 0, clrColor [CLR_WHITE_PIECE_OUTLINE]);
	hpPen [PEN_BLACK_PIECE_OUTLINE]  = CreatePen(PS_SOLID, 0, clrColor [CLR_BLACK_PIECE_OUTLINE]);
	hpPen [PEN_HIGHLIGHT]            = CreatePen(PS_SOLID, 0, clrColor [CLR_HIGHLIGHT]);
	hpPen [PEN_TRUEPREMOVE]          = CreatePen(PS_SOLID, 0, clrColor [CLR_TRUE_PREMOVE_HIGHLIGHT]);
	hpPen [PEN_ILLEGAL_TRUEPREMOVE]  = CreatePen(PS_SOLID, 0, clrColor [CLR_ILLEGAL_TRUE_PREMOVE]);
	hpPen [PEN_KING_HIGHLIGHT]       = CreatePen(PS_SOLID, 0, clrColor [CLR_KING_HIGHLIGHT]);
	hpPen [PEN_WHITE]                = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	hpPen [PEN_BLACK]                = CreatePen(PS_SOLID, 0, RGB(0,   0,   0));

	hbBrush [BRUSH_BUFFER_FILL]       = CreateSolidBrush(clrColor [CLR_BUFFER_FILL]);
	hbBrush [BRUSH_WHITE_SQUARE_FILL] = CreateSolidBrush(clrColor [CLR_WHITE_SQUARE_FILL]);
	hbBrush [BRUSH_BLACK_SQUARE_FILL] = CreateSolidBrush(clrColor [CLR_BLACK_SQUARE_FILL]);
	hbBrush [BRUSH_WHITE_PIECE_FILL]  = CreateSolidBrush(clrColor [CLR_WHITE_PIECE_FILL]);
	hbBrush [BRUSH_BLACK_PIECE_FILL]  = CreateSolidBrush(clrColor [CLR_BLACK_PIECE_FILL]);
	hbBrush [BRUSH_WHITE]             = CreateSolidBrush(RGB(255, 255, 255));
	hbBrush [BRUSH_BLACK]             = CreateSolidBrush(RGB(0,   0,   0));

	for(nI = 0 ; nI < MAX_FONT ; nI++)
	{
		TOOLBOX_MakeFont(nI);
	}
}

void SYS_DestroyVal(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_PEN ; nI++)
	{
		DeleteObject(hpPen [nI]);
	}

	for(nI = 0 ; nI < MAX_BRUSH ; nI++)
	{
		DeleteObject(hbBrush [nI]);
	}

	for(nI = 0 ; nI < MAX_FONT ; nI++)
	{
		DeleteObject(hfFont [nI].hfFont);
	}
}

void SYS_ChangeColor(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_PEN ; nI++)
	{
		DeleteObject(hpPen [nI]);
	}

	for(nI = 0 ; nI < MAX_BRUSH ; nI++)
	{
		DeleteObject(hbBrush [nI]);
	}

	hpPen [PEN_BOARD_OUTLINE]        = CreatePen(PS_SOLID, 0, clrColor [CLR_BOARD_OUTLINE]);
	hpPen [PEN_BUFFER_OUTLINE]       = CreatePen(PS_SOLID, 0, clrColor [CLR_BUFFER_OUTLINE]);
	hpPen [PEN_PIECE_NUMBER_OUTLINE] = CreatePen(PS_SOLID, 0, clrColor [CLR_PIECE_NUMBER]);
	hpPen [PEN_WHITE_SQUARE_OUTLINE] = CreatePen(PS_SOLID, 0, clrColor [CLR_WHITE_SQUARE_OUTLINE]);
	hpPen [PEN_BLACK_SQUARE_OUTLINE] = CreatePen(PS_SOLID, 0, clrColor [CLR_BLACK_SQUARE_OUTLINE]);
	hpPen [PEN_WHITE_PIECE_OUTLINE]  = CreatePen(PS_SOLID, 0, clrColor [CLR_WHITE_PIECE_OUTLINE]);
	hpPen [PEN_BLACK_PIECE_OUTLINE]  = CreatePen(PS_SOLID, 0, clrColor [CLR_BLACK_PIECE_OUTLINE]);
	hpPen [PEN_HIGHLIGHT]            = CreatePen(PS_SOLID, 0, clrColor [CLR_HIGHLIGHT]);
	hpPen [PEN_TRUEPREMOVE]          = CreatePen(PS_SOLID, 0, clrColor [CLR_TRUE_PREMOVE_HIGHLIGHT]);
	hpPen [PEN_ILLEGAL_TRUEPREMOVE]  = CreatePen(PS_SOLID, 0, clrColor [CLR_ILLEGAL_TRUE_PREMOVE]);
	hpPen [PEN_KING_HIGHLIGHT]       = CreatePen(PS_SOLID, 0, clrColor [CLR_KING_HIGHLIGHT]);
	hpPen [PEN_WHITE]                = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	hpPen [PEN_BLACK]                = CreatePen(PS_SOLID, 0, RGB(0,   0,   0));

	hbBrush [BRUSH_BUFFER_FILL]       = CreateSolidBrush(clrColor [CLR_BUFFER_FILL]);
	hbBrush [BRUSH_WHITE_SQUARE_FILL] = CreateSolidBrush(clrColor [CLR_WHITE_SQUARE_FILL]);
	hbBrush [BRUSH_BLACK_SQUARE_FILL] = CreateSolidBrush(clrColor [CLR_BLACK_SQUARE_FILL]);
	hbBrush [BRUSH_WHITE_PIECE_FILL]  = CreateSolidBrush(clrColor [CLR_WHITE_PIECE_FILL]);
	hbBrush [BRUSH_BLACK_PIECE_FILL]  = CreateSolidBrush(clrColor [CLR_BLACK_PIECE_FILL]);
	hbBrush [BRUSH_WHITE]             = CreateSolidBrush(RGB(255, 255, 255));
	hbBrush [BRUSH_BLACK]             = CreateSolidBrush(RGB(0,   0,   0));
}

void SYS_WritePieceFile(char *cFn, char *cS1, char *cS2)
{
	FILE *fv;

	if(System.bCDROMConnection)
	{
		return;
	}

	fv = fopen(cFn, "w");
	if(fv != NULL)
	{
		fprintf(fv, "%s\n%s\n", cS1, cS2);
		fclose(fv);
	}
}

void SYS_CreateToups(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\TOUPS\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	SYS_WritePieceFile(Fn, "47", "10,-41 0,5 1,1 0,2 1,1 0,1 1,1 0,1 4,4 0,1 -2,2 1,1 -1,1 0,3 1,1 0,1 3,3 1,0 -2,2 0,3 3,3 3,0 3,-3 0,-3 -2,-2 1,0 3,-3 0,-1 1,-1 0,-3 -1,-1 1,-1 -2,-2 0,-1 4,-4 0,-1 1,-1 0,-1 1,-1 0,-2 1,-1 0,-5");

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	SYS_WritePieceFile(Fn, "47", "7,-43 0,5 30,0 -29,0 0,4 4,4 20,0 -19,0 0,13 19,0 -20,0 -4,4 0,8 6,0 0,-5 5,0 0,5 7,0 0,-5 5,0 0,5 6,0 0,-8 -4,-4 -1,0 0,-13 1,0 4,-4 0,-4 1,0 0,-5");

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	SYS_WritePieceFile(Fn, "47", "13,-43 0,3 7,7 0,1 1,1 0,1 1,1 0,7 1,1 0,1 1,1 0,2 0,-2 -1,-1 0,-1 -1,-1 -1,0 -1,-1 0,-1 -1,0 -1,-1 -1,0 -4,-4 0,-1 -3,-3 -2,0 0,3 1,2 -1,-2 0,-2 -3,0 -3,3 0,2 1,1 0,2 4,8 1,1 0,1 4,0 -2,-2 -1,0 2,2 -3,0 3,3 5,3 -5,-3 -1,5 0,3 1,-1 1,0 1,-1 1,0 4,-4 0,-2 0,2 -2,2 2,0 1,4 3,-4 2,-4 -2,4 6,0 2,-1 7,-7 1,-2 3,-9 0,-6 1,0 0,-10");

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	SYS_WritePieceFile(Fn, "47", "5,-43 -1,1 0,2 2,2 3,1 8,0 1,1 0,2 10,0 -15,0 4,4 12,0 -12,0 0,2 -1,1 2,2 2,1 3,0 0,10 0,-4 -4,0 8,0 -4,0 0,-6 3,0 1,-1 1,0 2,-2 -2,2 -1,0 -1,1 -6,0 -2,-1 -2,-2 -4,4 0,5 1,1 0,1 4,4 1,0 2,2 1,0 1,1 1,0 -1,0 -2,2 0,2 2,2 2,0 2,-2 0,-2 -2,-2 -1,0 1,0 1,-1 1,0 2,-2 1,0 4,-4 0,-1 1,-1 0,-5 -5,-5 0,-2 4,-4 -5,0 0,-2 1,-1 8,0 3,-1 2,-2 0,-2 -1,-1 -13,0 -5,5 -5,-5 -13,0");

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	SYS_WritePieceFile(Fn, "47", "14,-44 -2,2 0,3 12,2 12,-2 -12,2 -12,-2 0,2 -3,3  8,0 7,3 7,-3 8,0 -8,0 -7,3 -7,-3 -8,0 -1,1 0,3 10,4 12,0 10,-4 -10,4 -12,0 -10,-4 0,4 -3,6 0,6 -2,2 2,2 2,-2 -2,-2 3,-3 1,-3 4,-4 0,-4 0,18 -2,2 2,2 2,-2 -2,-2 6,-12 0,-4 0,4 3,7 2,7 -2,2 2,2 2,-2 -2,-2 3,-7 2,-7 0,-4 0,4 6,12 -2,2 2,2 2,-2 -2,-2 0,-18 0,4 4,4 1,3 3,3 -2,2 2,2 2,-2 -2,-2 0,-6 -3,-6 0,-4 0,-3 -1,-1 -3,-3 0,-2 0,-3 -2,-2");

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	SYS_WritePieceFile(Fn, "47", "16,-44 -1,1 -2,0 -1,1 -2,0 -1,1 0,2 7,0 1,1 12,0 1,-1 7,0 -7,0 -1,1 -12,0 -1,-1 -7,0 0,4 4,0 1,1 18,0 1,-1 4,0 -4,0 -1,1 -18,0 -1,-1 -4,0 0,3 -1,1 0,1 -5,5 0,5 1,1 0,1 2,2 1,0 1,1 5,0 1,-1 2,0 3,-3 4,-7 0,-8 0,8 4,7 -4,-7 -4,7 0,4 3,3 0,3 -3,0 0,2 3,0 0,4 3,0 0,-4 3,0 0,-2 -3,0 0,-3 3,-3 0,-4 3,3 2,0 1,1 5,0 1,-1 1,0 2,-2 0,-1 1,-1 0,-5 -5,-5 0,-1 -1,-1 0,-9 -1,-1 -2,0 -1,-1 -2,0 -1,-1");
}

void SYS_CreateNicok(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\NICOK\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	SYS_WritePieceFile(Fn, "48", "10,-42 12,5 -6,0 6,3 0,6 -6,0 6,3 3,0 -3,0 -3,3 0,6 3,3 5,0 3,-3 0,-6 -3,-3 -3,0 3,0 6,-3 -6,0 0,-6 6,-3 -6,0 12,-5");

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-42 0,2 8,0 0,14  2,0 0,-14 4,0 -14,0 4,4 0,9 -4,3 0,6 5,0 0,-4 5,0 0,4 6,0 0,-4 5,0 0,4 5,0 0,-6 -4,-3 0,-9 4,-4 -8,0 0,14 -2,0 0,-14 -4,0 14,0 0,-2");

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-42 8,4 0,1 -2,4 -1,0 -4,-4 -2,0 -1,1 2,2 -1,0 -2,-2 -3,3 4,9 3,0 0,-1 1,0 0,2 -1,0 0,-1 -3,0 3,5  -3,2 13,0 4,-2 -7,-7 7,7 2,-2 -7,-6 7,6 2,-1 -7,-6 7,6 1,-3 -6,-4 6,4 1,-3 -5,-3 5,3 1,-3  -3,-8 3,-3");

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-42 8,5 9,0 -9,0 -4,2 4,2 -5,5 0,6 4,4 5,-5 2,2 -5,5  6,0  6,-6 0,-6 -5,-5 4,-2 -4,-2 8,-5");

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-42 0,1 4,2 10,3 10,-3 -20,0  -3,3  -4,17 -1,0 0,2 2,0 0,-2 -1,0 9,-12 0,17 -1,0 0,2 2,0 0,-2 -1,0 8,-16 8,16 -1,0 0,2 2,0 0,-2 -1,0 0,-17 9,12 -1,0 0,2 2,0 0,-2 -1,0 -4,-17 -3,-3 4,-2 0,-1");

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-42 0,1 2,2 11,3 0,-3 -11,0 0,3 -5,8 0,8 4,4 4,0 5,-5 1,0  0,4 -2,0 0,2 2,0 0,3 2,0 0,-11 -2,0 0,-4 -4,0 0,-4 4,0 0,-4 4,0 0,4 4,0 0,4 -4,0 0,4 -2,0 0,11 2,0 0,-3 2,0 0,-2 -2,0 0,-4 1,0 5,5 4,0 4,-4 0,-8 -5,-8 0,-3  -11,3  0,-3 11,0 2,-2 0,-1");
}

void SYS_CreateKbirk(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\KBIRK\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	SYS_WritePieceFile(Fn, "48", "36,-42 0,1 -1,3 -1,1 -2,1 -1,4 -1,2 -2,3 -2,2 0,2 2,2 0,3 -2,2 -4,0 -2,-2 0,-3 2,-2 0,-2 -2,-2 -2,-3 -1,-2 -1,-4 -2,-1 -1,-1 -1,-3 0,-1");

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-36 -2,-2 -1,-2 0,-1 26,0 0,1 -1,2 -2,2 -4,1 -12,0 -4,-1 4,1 12,0 4,-1 0,1 -1,7 -1,12 2,2 1,2 0,5 -4,0 0,-2 -2,-1 -1,0 -2,1 0,2 -4,0 0,-2 -2,-1 -1,0 -2,1 0,2 -4,0 0,-5 1,-2 2,-2 4,1 8,0 4,-1 -4,1 -8,0 -4,-1 -1,-12 -1,-7 0,-1");

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-36 -2,-2 -1,-2 0,-1 26,0 0,1 -1,2 -2,2 -4,1 -12,0 -4,-1 4,1 12,0 4,-1 1,2 1,3 0,8 -1,3 -3,5 -3,3 -3,2 -4,1 0,-2 0,2 -1,2 -2,0 0,-4 0,2 -2,-1 -2,-2 2,-1 1,0 0,-2 -1,-1 -1,0 0,1 1,2 -2,1 -1,-4 -3,-4 -1,-3 0,-2 1,-1 2,-1 1,1 1,2 -1,-2 0,-1 2,1 1,2 3,1 2,1 1,0 1,-1 0,-1 -1,-2 -2,-2 -4,-2 -1,-1 -1,-2 -1,-3");

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	SYS_WritePieceFile(Fn, "48", "18,-36 0,-1 -10,-1 -1,-2 0,-1 13,0 3,1 2,1 2,-1 3,-1 13,0 0,1 -1,2 -10,1 0,1 -4,1 -6,0 -4,-1 4,1 6,0 4,-1 -1,4 1,3 2,3 1,3 0,4 -1,3 -3,3 -2,1 -3,1 1,1 0,2 -1,1 -2,0 -1,-1 0,-2 1,-1 2,0 -2,0 -3,-1 -2,-1 -3,-3 -1,-3 0,-4 1,-3 2,-3 1,-3 4,1 4,0 4,-1 -4,1 -2,0 0,4 1,0 0,5 3,0 0,2 -3,0 0,3 -2,0 0,-3 -3,0 0,-2 3,0 0,-5 1,0 0,-4 -2,0 -4,-1");

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-36 -1,-1 -1,-2 0,-2 26,0 0,2 -1,2 -1,1 -4,1 -14,0 -4,-1 4,1 14,0 4,-1 0,2 3,13 2,0 1,1 0,2 -1,1 -2,0 -1,-1 0,-2 -6,-3 -1,12 1,0 1,1 0,2 -1,1 -2,0 -1,-1 0,-2 1,-1 -5,-12 -5,12 1,1 0,2 -1,1 -2,0 -1,-1 0,-2 1,-1 1,0 -1,-12 -6,3 0,2 -1,1 -2,0 -1,-1 0,-2 1,-1 2,0 3,-13 0,-2");

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-36 1,-1 -1,-2 0,-2 26,0 0,2 -1,2 -1,1 -4,1 -14,0 -4,-1 4,1 14,0 4,-1 1,3 4,8 1,4 0,4 -1,2 -1,1 -3,1 -4,0 -4,-1 -3,-1 0,4 2,0 0,2 -2,0 0,2 -2,0 0,-2 -2,0 0,-2 2,0 0,-4 -3,1 -4,1 -4,0 -3,-1 -1,-1 -1,-2 0,-4 1,-4 4,-8 1,-3 4,1 7,0 2,4 6,0 2,4 2,5 0,3 -1,1 -3,0 -3,-1 -3,-2 0,-10 -2,-4 -2,4 -6,0 -2,4 -2,5 0,3 1,1 3,0 3,-1 3,-2 0,-10 2,-4 -7,0 -4,-1");
}

void SYS_CreateJhurt(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\JHURT\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	SYS_WritePieceFile(Fn, "48", "16,-36 2,3 4,3 7,0 -7,0 -1,2 -1,3 0,2 1,3 1,2 7,0 -10,0 5,4 -2,2 0,2 2,2 3,0 2,-2 0,-2 -2,-2 5,-4 -3,0 1,-2 1,-3 0,-2 -1,-3 -1,-2 4,-3 2,-3 1,-5 -21,0");

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	SYS_WritePieceFile(Fn, "48", "9,-42 2,9 24,0 -20,0 0,17 20,0 -24,0 -2,8 0,2 6,0 0,-2 6,0 0,2 8,0 0,-2 6,0 0,2 6,0 0,-2 -2,-8 -4,0 0,-17 4,0 2,-9 0,-2 -32,0");

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	SYS_WritePieceFile(Fn, "48", "10,-43 5,4 10,8 1,2 -1,-2 -2,-1 -4,-1 -4,0 -4,-2 -4,-1 -3,1 -1,3 1,3 10,10 3,4 -1,2 4,0 1,4 7,-4 4,-1 4,-3 5,-8 1,-8 0,-11 -30,0");

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	SYS_WritePieceFile(Fn, "48", "4,-38 3,3 0,2 3,0 0,-3 -3,-3  1,-1 9,0 1,1  0,1 12,0 -14,0 0,2 14,0 -12,0 0,4 12,0 -14,0 -4,2 -3,4 -1,4 1,4 2,4 3,3 4,3 5,2 -1,1 0,2 1,1 2,0 1,-1 0,-2 -1,-1 5,-2 4,-3 3,-3 2,-4 1,-4 -1,-4 -3,-4 -4,-2 -2,0 0,-4 2,0 0,-2 -2,0 0,-1  1,-1 9,0 1,1  -3,3 0,3 3,0 0,-3 3,-3 -4,-5 -11,0 -6,6 -5,-6 -11,0");

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-43 -2,1 2,2 4,1 16,0 4,-1 2,-2 -2,2 -4,1 -16,0 -4,-1 -2,-2 0,8 2,2 4,1 16,0 4,-1 2,-2 -2,2 -4,1 -16,0 -4,-1 -2,-2   -6,18 10,-10 2,20 8,-18 8,18 2,-20 10,10 -6,-18 0,-8 -2,-1 -4,-1 -15,0");

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	SYS_WritePieceFile(Fn, "48", "12,-43 -2,1 2,2 4,1 16,0 4,-1 2,-2 -2,2 -4,1 -16,0 -4,-1 -2,-2 0,8 2,2 4,1 16,0 4,-1 2,-2 -2,2 -4,1 -16,0 -4,-1 -2,-2    -4,3 -2,4 -1,4 1,4 2,4    5,4 9,0   3,-2   1,0 0,-18 0,25 0,-2 -3,0 6,0 -3,0 0,-5  1,0 3,2 9,0 5,-4 2,-4 1,-4 -1,-4 -2,-4 -4,-3 0,-8 -2,-1 -4,-1 -15,0");
}

void SYS_CreateEchur(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\ECHUR\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	SYS_WritePieceFile(Fn, "38", "10,-31 2,2 2,0 0,1 1,1 1,0 0,6 5,0 -5,0 -1,1 2,2 3,0 -3,0 0,1 -2,2 0,2 3,3 2,0 3,-3 0,-2 -2,-2 0,-1 2,-2 -1,-1 0,-6 1,0 1,-1 0,-1 2,0 2,-2 0,-1 -18,0");

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	SYS_WritePieceFile(Fn, "38", "9,-33 2,2 1,0 2,2 0,8 8,0 -8,0 -1,0 -1,1 2,2 8,0 -8,0 0,1 -1,0 -1,1 0,2 -1,1 1,1 13,0 1,-1 -1,-1 0,-2 -1,-1 -1,0 0,-1 2,-2 -1,-1 -1,0 0,-8 2,-2 1,0 2,-2 0,-1 -19,0");

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	SYS_WritePieceFile(Fn, "38", "10,-33 3,3 0,5 1,1 1,0 0,1 1,1 1,0 0,1 2,0 0,1 2,0 -2,0 -1,-1 -1,0 -1,-1 -3,0 -1,-1 -1,0 -2,2 0,2 1,1 0,1 1,1 1,0 0,1 1,1 1,0 0,1 3,3 9,0 1,-1 -1,-1 0,-2 1,-1 0,-1 1,-1 0,-1 1,-1 0,-4 -1,-1 0,-1 -1,-1 0,-5 2,-2 0,-1 -20,0");

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	SYS_WritePieceFile(Fn, "38", "9,-33 3,3 1,0 0,3 12,0 -12,0 0,2 1,1 10,0 -10,0 0,2 -1,1 0,1 -1,1 0,1 1,1 0,1 2,2 0,1 3,3 3,0 3,-3 -2,-2 0,-1 -1,-1 0,-2 1,0 0,1 3,3 1,-1 0,-1 1,-1 0,-1 -1,-1 0,-1 -1,-1 0,-2 1,-1 0,-5 1,0 3,-3 0,-1 -21,0");

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	SYS_WritePieceFile(Fn, "38", "10,-32 2,2 1,0 0,1 1,1 1,0 0,1 1,1 0,4 6,0 -7,0 -1,1 1,1 0,3 7,0 -7,0 -3,3 1,1 3,0 0,1 3,3 3,-3 0,-1 3,0 1,-1 -3,-3 0,-3 1,-1 -2,-2 0,-3 1,-1 0,-1 1,0 1,-1 0,-1 1,0 2,-2 0,-2 -18,0");

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	SYS_WritePieceFile(Fn, "38", "9,-32 2,2 1,0 1,2 0,1 1,1 0,4 10,0 -10,0 -1,0 -1,1 1,1 11,0 -11,0 0,2 -2,2 1,1 1,0 1,1 3,0 0,2 -1,0 -1,1 1,1 1,0 1,1 2,0 1,-1 1,0 1,-1 -1,-1 -1,0 0,-2 3,0 1,-1 1,0 1,-1 -2,-2 0,-2 1,-1 -1,-1 -1,0 0,-4 1,-1 0,-1 1,-1 0,-1 1,0 2,-2 0,-2 -20,0");
}

void SYS_CreateDfong(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\DFONG\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	SYS_WritePieceFile(Fn, "38", "17,-27 0,4 -4,4 0,3 4,4 5,0 4,-4 0,-3 -4,-4 0,-4 5,-4 -16,0");

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	SYS_WritePieceFile(Fn, "38", "6,-32 4,4 0,9 -4,3 0,6 5,0 0,-4 5,0 0,4 6,0 0,-4 5,0 0,4 5,0 0,-6 -4,-3 0,-9 4,-4 0,-2 -26,0");

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	SYS_WritePieceFile(Fn, "38", "17,-30 0,1 -2,4 -1,0 -4,-4 -2,0 -4,4 7,14 -3,2 13,0 8,-5 3,-9 -3,-8 3,-3 -20,0");

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	SYS_WritePieceFile(Fn, "38", "16,-30 -4,2 4,2 -5,5 0,6 4,4 5,-5 1,1 -5,5 1,1  0,1 6,0 0,-1 6,-6 0,-6 -5,-5 4,-2 -4,-2 8,-4 -25,0");

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	SYS_WritePieceFile(Fn, "38", "5,-33 4,2 -3,3 -4,17 9,-12 0,17 8,-16 8,16 0,-17 9,12 -4,-17 -3,-3 4,-2 0,-1 -28,0");

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	SYS_WritePieceFile(Fn, "38", "6,-33 5,5 -8,8 0,8 4,4 4,0 5,-5 1,0 0,4 -2,0 0,2 2,0 0,3 4,0 0,-3 2,0 0,-2 -2,0 0,-4 1,0 5,5 4,0 4,-4 0,-8 -8,-8 5,-5 0,-1 -26,0");
}

void SYS_CreateUser(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\USER\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	SYS_WritePieceFile(Fn, "47", "10,-41 0,5 1,1 0,2 1,1 0,1 1,1 0,1 4,4 0,1 -2,2 1,1 -1,1 0,3 1,1 0,1 3,3 1,0 -2,2 0,3 3,3 3,0 3,-3 0,-3 -2,-2 1,0 3,-3 0,-1 1,-1 0,-3 -1,-1 1,-1 -2,-2 0,-1 4,-4 0,-1 1,-1 0,-1 1,-1 0,-2 1,-1 0,-5");

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	SYS_WritePieceFile(Fn, "47", "7,-43 0,5 30,0 -29,0 0,4 4,4 20,0 -19,0 0,13 19,0 -20,0 -4,4 0,8 6,0 0,-5 5,0 0,5 7,0 0,-5 5,0 0,5 6,0 0,-8 -4,-4 -1,0 0,-13 1,0 4,-4 0,-4 1,0 0,-5");

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	SYS_WritePieceFile(Fn, "47", "13,-43 0,3 7,7 0,1 1,1 0,1 1,1 0,7 1,1 0,1 1,1 0,2 0,-2 -1,-1 0,-1 -1,-1 -1,0 -1,-1 0,-1 -1,0 -1,-1 -1,0 -4,-4 0,-1 -3,-3 -2,0 0,3 1,2 -1,-2 0,-2 -3,0 -3,3 0,2 1,1 0,2 4,8 1,1 0,1 4,0 -2,-2 -1,0 2,2 -3,0 3,3 5,3 -5,-3 -1,5 0,3 1,-1 1,0 1,-1 1,0 4,-4 0,-2 0,2 -2,2 2,0 1,4 3,-4 2,-4 -2,4 6,0 2,-1 7,-7 1,-2 3,-9 0,-6 1,0 0,-10");

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	SYS_WritePieceFile(Fn, "47", "5,-43 -1,1 0,2 2,2 3,1 8,0 1,1 0,2 10,0 -15,0 4,4 12,0 -12,0 0,2 -1,1 2,2 2,1 3,0 0,10 0,-4 -4,0 8,0 -4,0 0,-6 3,0 1,-1 1,0 2,-2 -2,2 -1,0 -1,1 -6,0 -2,-1 -2,-2 -4,4 0,5 1,1 0,1 4,4 1,0 2,2 1,0 1,1 1,0 -1,0 -2,2 0,2 2,2 2,0 2,-2 0,-2 -2,-2 -1,0 1,0 1,-1 1,0 2,-2 1,0 4,-4 0,-1 1,-1 0,-5 -5,-5 0,-2 4,-4 -5,0 0,-2 1,-1 8,0 3,-1 2,-2 0,-2 -1,-1 -13,0 -5,5 -5,-5 -13,0");

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	SYS_WritePieceFile(Fn, "47", "14,-44 -2,2 0,3 12,2 12,-2 -12,2 -12,-2 0,2 -3,3  8,0 7,3 7,-3 8,0 -8,0 -7,3 -7,-3 -8,0 -1,1 0,3 10,4 12,0 10,-4 -10,4 -12,0 -10,-4 0,4 -3,6 0,6 -2,2 2,2 2,-2 -2,-2 3,-3 1,-3 4,-4 0,-4 0,18 -2,2 2,2 2,-2 -2,-2 6,-12 0,-4 0,4 3,7 2,7 -2,2 2,2 2,-2 -2,-2 3,-7 2,-7 0,-4 0,4 6,12 -2,2 2,2 2,-2 -2,-2 0,-18 0,4 4,4 1,3 3,3 -2,2 2,2 2,-2 -2,-2 0,-6 -3,-6 0,-4 0,-3 -1,-1 -3,-3 0,-2 0,-3 -2,-2");

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	SYS_WritePieceFile(Fn, "47", "16,-44 -1,1 -2,0 -1,1 -2,0 -1,1 0,2 7,0 1,1 12,0 1,-1 7,0 -7,0 -1,1 -12,0 -1,-1 -7,0 0,4 4,0 1,1 18,0 1,-1 4,0 -4,0 -1,1 -18,0 -1,-1 -4,0 0,3 -1,1 0,1 -5,5 0,5 1,1 0,1 2,2 1,0 1,1 5,0 1,-1 2,0 3,-3 4,-7 0,-8 0,8 4,7 -4,-7 -4,7 0,4 3,3 0,3 -3,0 0,2 3,0 0,4 3,0 0,-4 3,0 0,-2 -3,0 0,-3 3,-3 0,-4 3,3 2,0 1,1 5,0 1,-1 1,0 2,-2 0,-1 1,-1 0,-5 -5,-5 0,-1 -1,-1 0,-9 -1,-1 -2,0 -1,-1 -2,0 -1,-1");
}

void SYS_CheckToups(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	FILE *fv;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\TOUPS\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateToups();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateToups();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateToups();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateToups();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateToups();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateToups();
		return;
	}
	fclose(fv);
}

void SYS_CheckNicok(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	FILE *fv;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\NICOK\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateNicok();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateNicok();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateNicok();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateNicok();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateNicok();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateNicok();
		return;
	}
	fclose(fv);
}

void SYS_CheckKbirk(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	FILE *fv;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\KBIRK\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateKbirk();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateKbirk();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateKbirk();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateKbirk();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateKbirk();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateKbirk();
		return;
	}
	fclose(fv);
}

void SYS_CheckJhurt(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	FILE *fv;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\JHURT\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateJhurt();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateJhurt();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateJhurt();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateJhurt();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateJhurt();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateJhurt();
		return;
	}
	fclose(fv);
}

void SYS_CheckEchur(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	FILE *fv;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\ECHUR\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateEchur();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateEchur();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateEchur();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateEchur();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateEchur();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateEchur();
		return;
	}
	fclose(fv);
}

void SYS_CheckDfong(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	FILE *fv;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\DFONG\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateDfong();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateDfong();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateDfong();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateDfong();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateDfong();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateDfong();
		return;
	}
	fclose(fv);
}

void SYS_CheckUser(void)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	FILE *fv;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcat(Dir, "BUGSTUFF\\PIECE\\VECTOR\\USER\\");

	_makepath(Fn, Drive, Dir, "PAWN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateUser();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "ROOK", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateUser();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KNIGHT", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateUser();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "BISHOP", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateUser();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "QUEEN", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateUser();
		return;
	}
	fclose(fv);

	_makepath(Fn, Drive, Dir, "KING", ".INI");
	fv = fopen(Fn, "r");
	if(fv == NULL)
	{
		SYS_CreateUser();
		return;
	}
	fclose(fv);
}