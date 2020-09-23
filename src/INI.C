#include "thief.h"

char *INI_GetIniFilename(void)
{
	static char cBuffer [_MAX_PATH];

	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);
	strcpy(Ext, INI_EXT);
	_makepath(cBuffer, Drive, Dir, File, Ext);

	return cBuffer;
}

char *INI_GetSysFilename(void)
{
	static char cBuffer [_MAX_PATH];

	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);
	strcpy(Ext, SYS_EXT);
	_makepath(cBuffer, Drive, Dir, File, Ext);

	return cBuffer;
}

int INI_ReadSetup(char *cIni)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Ini   [_MAX_PATH];

	char cLS [MAX_LOGIN] [MAX_LOGIN_SIZE + 10];

	int  nC, nI, nJ, nK, nS;
	char cTmp [_MAX_PATH], cTmp1 [_MAX_PATH], cTmp2 [_MAX_PATH];

	int btoups, bnicok, bkbirk, bjhurt, bechur, bdfong, buser;
	int nDragMove, bClickMove;

	int bNeedSave = 0;

	_splitpath(cIni, Drive, Dir, File, Ext);
	strcpy(Ext, INI_EXT);
	_makepath(Ini, Drive, Dir, File, Ext);

	//
	// [Main]
	//

	// save games?
	nJ = -1;
	nJ = GetPrivateProfileInt("Main", "Save Games", nJ, Ini);
	if(nJ == -1)
	{
		User.bLogGame      = GetPrivateProfileInt("Main", "Log games", User.bLogGame, Ini);
		User.bSavePlayGame = GetPrivateProfileInt("Main", "Save played game", User.bSavePlayGame, Ini);
	}
	else
	{
		User.bLogGame      = nJ;
		User.bSavePlayGame = nJ;
	}

	// save observed game?
	User.bSaveObserveGame = GetPrivateProfileInt("Main", "Save observed game", User.bSaveObserveGame, Ini);

	// save unobserved game?
	User.bSaveUnobserveGame = GetPrivateProfileInt("Main", "Save unobserved game", User.bSaveUnobserveGame, Ini);

	// save aborted or disconnected game?
	User.bSaveAbortGame = GetPrivateProfileInt("Main", "Save aborted or disconnected game", User.bSaveAbortGame, Ini);

	// save no BFEN bughouse game
	User.bSaveNoBFENBugGame = GetPrivateProfileInt("Main", "Save no BFEN bughousen game", User.bSaveNoBFENBugGame, Ini);

	// save daily
	User.bSaveDaily = GetPrivateProfileInt("Main", "Save daily", User.bSaveDaily, Ini);

	// pgn viewer
	GetPrivateProfileString("Main", "PGN viewer", User.cPGNViewer, User.cPGNViewer, _MAX_PATH, Ini);

	// bpgn viewer
	GetPrivateProfileString("Main", "BPGN viewer", User.cBPGNViewer, User.cBPGNViewer, _MAX_PATH, Ini);

	// save settings on exit?
	User.bSaveOnExit = GetPrivateProfileInt("Main", "Save settings on exit", User.bSaveOnExit, Ini);

	// save settings on exit?
	User.bMinimizetoTray = GetPrivateProfileInt("Main", "Minimize to Tray", User.bMinimizetoTray, Ini);

	// Colored bughouse clocks
	User.bColoredClocks = GetPrivateProfileInt("Main", "Colored Bughouse clocks", User.bColoredClocks, Ini);

	// suppress exit prompt
	User.bSuppressExitPrompt = GetPrivateProfileInt("Main", "Suppress exit prompt", User.bSuppressExitPrompt, Ini);

	// show bitmap size during game window resize
	User.bShowBitmapSize = GetPrivateProfileInt("Main", "Show bitmap size during game window resize", User.bShowBitmapSize, Ini);

	// show menu path in telnet console window
	User.bShowMenuPath = GetPrivateProfileInt("Main", "Show menu path in telnet console window", User.bShowMenuPath, Ini);

	// browses
	GetPrivateProfileString("Main", "Browse connection", Browse.cConnectionBrowse, Browse.cConnectionBrowse, _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse log",        Browse.cLogBrowse,        Browse.cLogBrowse,        _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse save BPGN",  Browse.cSaveBPGNBrowse,   Browse.cSaveBPGNBrowse,   _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse save PGN",   Browse.cSavePGNBrowse,    Browse.cSavePGNBrowse,    _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse profile",    Browse.cProfileBrowse,    Browse.cProfileBrowse,    _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse piece",      Browse.cPieceBrowse,      Browse.cPieceBrowse,      _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse square",     Browse.cSquareBrowse,     Browse.cSquareBrowse,     _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse PGN",        Browse.cPGNBrowse,        Browse.cPGNBrowse,        _MAX_PATH, Ini);
	GetPrivateProfileString("Main", "Browse BPGN",       Browse.cBPGNBrowse,       Browse.cBPGNBrowse,       _MAX_PATH, Ini);

	//
	// [Hidden]
	//

	// add button command into history?
	User.bButtonCommandAddHist = GetPrivateProfileInt("Hidden", "Add button command into history", User.bButtonCommandAddHist, Ini);

	// add play command into history?
	User.bPlayCommandAddHist = GetPrivateProfileInt("Hidden", "Add play command into history", User.bPlayCommandAddHist, Ini);

	// add observe command into history?
	User.bObserveCommandAddHist = GetPrivateProfileInt("Hidden", "Add observe command into history", User.bObserveCommandAddHist, Ini);

	// add telnet text command into history?
	User.bTelnetTextCommandAddHist = GetPrivateProfileInt("Hidden", "Add telnet text command into history", User.bTelnetTextCommandAddHist, Ini);

	// add telnet edit command into history?
	User.bTelnetEditCommandAddHist = GetPrivateProfileInt("Hidden", "Add telnet edit command into history", User.bTelnetEditCommandAddHist, Ini);

	// add login event command into history?
	User.bLEventCommandAddHist = GetPrivateProfileInt("Hidden", "Add login event command into history", User.bLEventCommandAddHist, Ini);

	// add game event command into history?
	User.bGEventCommandAddHist = GetPrivateProfileInt("Hidden", "Add game event command into history", User.bGEventCommandAddHist, Ini);

	// add function key command into history?
	User.bFKeyCommandAddHist = GetPrivateProfileInt("Hidden", "Add function key command into history", User.bFKeyCommandAddHist, Ini);

	// add control key command into history?
	User.bCKeyCommandAddHist = GetPrivateProfileInt("Hidden", "Add control key command into history", User.bCKeyCommandAddHist, Ini);

	// add seek command into history?
	User.bSeekCommandAddHist = GetPrivateProfileInt("Hidden", "Add seek command into history", User.bSeekCommandAddHist, Ini);

	// add match command into history?
	User.bMatchCommandAddHist = GetPrivateProfileInt("Hidden", "Add match command into history", User.bMatchCommandAddHist, Ini);

	// add tell command into history?
	User.bTellCommandAddHist = GetPrivateProfileInt("Hidden", "Add tell command into history", User.bTellCommandAddHist, Ini);

	// add message command into history?
	User.bMessageCommandAddHist = GetPrivateProfileInt("Hidden", "Add message command into history", User.bMessageCommandAddHist, Ini);

	// add timer command into history?
	User.bTimerCommandAddHist = GetPrivateProfileInt("Hidden", "Add timer command into history", User.bTimerCommandAddHist, Ini);

	// add lag command into history?
	User.bLagCommandAddHist = GetPrivateProfileInt("Hidden", "Add lag command into history", User.bLagCommandAddHist, Ini);

	// add question command into history?
	User.bQuestionCommandAddHist = GetPrivateProfileInt("Hidden", "Add question command into history", User.bQuestionCommandAddHist, Ini);

	nJ = -1;
	nJ = GetPrivateProfileInt("Partner sound", "Apply only to partner's tell", nJ, Ini);
	if(nJ == -1)
	{
		// apply only to partner's tell (partner sound)?
		User.bEarPartner = GetPrivateProfileInt("Hidden", "Apply only to partner's tell", User.bEarPartner, Ini);
	}
	else
	{
		bNeedSave = 1;

		// apply only to partner's tell (partner sound)?
		User.bEarPartner = GetPrivateProfileInt("Partner sound", "Apply only to partner's tell", User.bEarPartner, Ini);
	}

	// FICS start playing close all game
	User.bFICSPlayCloseAll = GetPrivateProfileInt("Hidden", "FICS start playing close all games", User.bFICSPlayCloseAll, Ini);

	// FICS play end stop partner game
	User.bFICSPlayEndPartner = GetPrivateProfileInt("Hidden", "FICS play end stop partner game", User.bFICSPlayEndPartner, Ini);

	// ICC start playing close all game
	User.bICCPlayCloseAll = GetPrivateProfileInt("Hidden", "ICC start playing close all games", User.bICCPlayCloseAll, Ini);

	// ICC play end stop partner game
	User.bICCPlayEndPartner = GetPrivateProfileInt("Hidden", "ICC play end stop partner game", User.bICCPlayEndPartner, Ini);

	// NONFICS start playing close all game
	User.bNONFICSPlayCloseAll = GetPrivateProfileInt("Hidden", "NONFICS start playing close all games", User.bNONFICSPlayCloseAll, Ini);

	// NONFICS play end stop partner game
	User.bNONFICSPlayEndPartner = GetPrivateProfileInt("Hidden", "NONFICS play end stop partner game", User.bNONFICSPlayEndPartner, Ini);

	// eat <12> game not found line
	User.bEat12GameNotFoundLine = GetPrivateProfileInt("Hidden", "Eat <12> game not found line", User.bEat12GameNotFoundLine, Ini);

	// eat <b1> game not found line
	User.bEatb1GameNotFoundLine = GetPrivateProfileInt("Hidden", "Eat <b1> game not found line", User.bEatb1GameNotFoundLine, Ini);

	// use right mouse click no highlight set telnet handle
	User.bRMNoHLSetTelnetHandle = GetPrivateProfileInt("Hidden", "Use right mouse click no highlight set telnet handle", User.bRMNoHLSetTelnetHandle, Ini);

	// use xtell for last tell
	User.bUseXTellForLastTell = GetPrivateProfileInt("Hidden", "Use xtell for last tell", User.bUseXTellForLastTell, Ini);

	// notepad executable file
	GetPrivateProfileString("Hidden", "Notepad executable file", User.cNotepad, User.cNotepad, _MAX_PATH, Ini);

	if(strlen(User.cNotepad) == 0)
	{
		strcpy(User.cNotepad, "notepad.exe");
	}

	TOOLBOX_AllTrim(User.cNotepad);

	//
	// [Board]
	//

	// bitmap piece file
	GetPrivateProfileString("Board", "Bitmap piece file", User.cBitmapPiece, User.cBitmapPiece, _MAX_PATH, Ini);

	// bitmap square file
	GetPrivateProfileString("Board", "Bitmap square file", User.cBitmapSquare, User.cBitmapSquare, _MAX_PATH, Ini);

	if(strlen(User.cBitmapPiece) == 0)
	{
		User.bVectorPiece = 1;
	}
	else
	{
		User.bVectorPiece = 0;
	}

//    if (strlen (User.cBitmapSquare) == 0)
//        {
//        User.bVectorSquare = 1;
//        }
//    else
//        {
	User.bVectorSquare = 0;
//        }

	// piece sets
	btoups = 0;
	bnicok = 0;
	bkbirk = 0;
	bjhurt = 0;
	bechur = 0;
	bdfong = 0;
	buser  = 0;

	btoups = GetPrivateProfileInt("Board", "Toups set", btoups, Ini);
	bnicok = GetPrivateProfileInt("Board", "Nicok set", bnicok, Ini);
	bkbirk = GetPrivateProfileInt("Board", "Kbirk set", bkbirk, Ini);
	bjhurt = GetPrivateProfileInt("Board", "Jhurt set", bjhurt, Ini);
	bechur = GetPrivateProfileInt("Board", "Echur set", bechur, Ini);
	bdfong = GetPrivateProfileInt("Board", "Dfong set", bdfong, Ini);
	buser  = GetPrivateProfileInt("Board", "User set",  buser,  Ini);

	if(btoups)
	{
		User.nPieceSet = VECTOR_TOUPS;
	}
	else if(bnicok)
	{
		User.nPieceSet = VECTOR_NICOK;
	}
	else if(bkbirk)
	{
		User.nPieceSet = VECTOR_KBIRK;
	}
	else if(bjhurt)
	{
		User.nPieceSet = VECTOR_JHURT;
	}
	else if(bechur)
	{
		User.nPieceSet = VECTOR_ECHUR;
	}
	else if(bdfong)
	{
		User.nPieceSet = VECTOR_DFONG;
	}
	else if(buser)
	{
		User.nPieceSet = VECTOR_USER;
	}
	else
	{
		User.nPieceSet = GetPrivateProfileInt("Board", "Piece set", User.nPieceSet, Ini);
	}

	// move type
	nDragMove  = -1;
	bClickMove = -1;

	nDragMove  = GetPrivateProfileInt("Board", "Drag move",  nDragMove,  Ini);
	bClickMove = GetPrivateProfileInt("Board", "Click move", bClickMove, Ini);

	if((nDragMove == -1) && (bClickMove == -1))
	{
		//
		// new THIEF.INI ("Move type")
		//
		User.nMoveType = GetPrivateProfileInt("Board", "Move type", User.nMoveType, Ini);
	}
	else
	{
		//
		// old THIEF.INI ("Drag move" and "Click move")
		//
		bNeedSave = 1;
		if(nDragMove == 0)
		{
			User.nMoveType = INVISIBLE_MOVE;
		}
		else if(nDragMove == 1)
		{
			User.nMoveType = DRAG_MOVE;
		}
		else if(nDragMove == 2)
		{
			User.nMoveType = DRAG_MOVE;
		}
		else if(nDragMove == 3)
		{
			User.nMoveType = DRAG_MOVE;
		}
		else
		{
			User.nMoveType = DRAG_MOVE;
		}

		if(bClickMove)
		{
			User.nMoveType = CLICK_MOVE;
		}
	}

	if((User.nMoveType != DRAG_MOVE) &&
			(User.nMoveType != CLICK_MOVE) &&
			(User.nMoveType != INVISIBLE_MOVE))
	{
		User.nMoveType = DRAG_MOVE;
	}

	// Auto center drag piece
	nJ = -1;
	nJ = GetPrivateProfileInt("Board", "Auto center drag piece", nJ, Ini);
	if(nJ == -1)
	{
		User.bAutoCenterDragPiece = GetPrivateProfileInt("Hidden", "Auto center drag piece", User.bAutoCenterDragPiece, Ini);
	}
	else
	{
		User.bAutoCenterDragPiece = nJ;
	}

	// smart move ?
	nI = -1;
	nI = GetPrivateProfileInt("Board", "Smart move", nI, Ini);
	if(nI == -1)
	{
		bNeedSave = 1;
	}
	else
	{

		// smart move (source square) ?
		nJ = -1;
		nJ = GetPrivateProfileInt("Board", "Smart move (source square)", nJ, Ini);
		if(nJ == -1)
		{
			bNeedSave = 1;
		}

		// smart move (destination square) ?
		nK = -1;
		nK = GetPrivateProfileInt("Board", "Smart move (destination square)", nK, Ini);
		if(nK == -1)
		{
			bNeedSave = 1;
		}

		if((nJ == -1) && (nK == -1))
		{
			if(nI == 0)
			{
				User.bSmartMove  = 0;
				User.bSmartMove1 = 0;
				User.bSmartMove2 = 0;
			}
			else
			{
				User.bSmartMove  = 1;
				User.bSmartMove1 = 1;
				User.bSmartMove2 = 1;
			}
		}
		else
		{
			if(nI == 0)
			{
				User.bSmartMove  = 0;
				User.bSmartMove1 = (nJ ? 1 : 0);
				User.bSmartMove2 = (nK ? 1 : 0);
			}
			else
			{
				User.bSmartMove  = 1;
				User.bSmartMove1 = (nJ ? 1 : 0);
				User.bSmartMove2 = (nK ? 1 : 0);
			}
		}
	}

	// true premove ?
	User.nTruePremove = GetPrivateProfileInt("Board", "True Premove", User.nTruePremove, Ini);
	if(! User.nTruePremove)
	{
		nI = 0;
		nI = GetPrivateProfileInt("Board", "True Premove Old Style", nI, Ini);
		if(nI)
		{
			bNeedSave = 1;
			User.nTruePremove = 1;
		}
	}

	// illegal true premove beep?
	nJ = -1;
	nJ = GetPrivateProfileInt("Board", "Illegal true premove beep", nJ, Ini);
	if(nJ == -1)
	{
		User.bIllegalTPBeep = GetPrivateProfileInt("Hidden", "Illegal true premove beep", User.bIllegalTPBeep, Ini);
	}
	else
	{
		User.bIllegalTPBeep = nJ;
	}

	// right mouse click to drop?
	nJ = -1;
	nJ = GetPrivateProfileInt("Board", "Right mouse click to drop", nJ, Ini);
	if(nJ == -1)
	{
		User.bRightMousePlay    = GetPrivateProfileInt("Board", "Right mouse click on playing board to drop", User.bRightMousePlay,    Ini);
		User.bRightMousePartner = GetPrivateProfileInt("Board", "Right mouse click on partner board to drop", User.bRightMousePartner, Ini);
	}
	else
	{
		bNeedSave = 1;
		User.bRightMousePlay    = (nJ ? 1 : 0);
		User.bRightMousePartner = User.bRightMousePlay;
	}

	// test legality on playing board?
	User.bTestLegalPlay = GetPrivateProfileInt("Board", "Test legality on playing board", User.bTestLegalPlay, Ini);

	// test legality on partner board?
	User.bTestLegalPartner = GetPrivateProfileInt("Board", "Test legality on partner board", User.bTestLegalPartner, Ini);

	// Animate observe game move (while not playing)?
	User.bAnimateObserve = GetPrivateProfileInt("Board", "Animate observe game move (while not playing)", User.bAnimateObserve, Ini);

	// Animate exam game move?
	User.bAnimateExam = GetPrivateProfileInt("Board", "Animate exam game move", User.bAnimateExam, Ini);

	// Animate play game move?
	User.bAnimatePlay = GetPrivateProfileInt("Board", "Animate play game move", User.bAnimatePlay, Ini);

	// Animate speed observe
	nJ = -1;
	nJ = GetPrivateProfileInt("Board", "Animate speed", nJ, Ini);

	if(nJ == -1)
	{
		User.nAnimateSpeedObserve = GetPrivateProfileInt("Board", "Animate speed", User.nAnimateSpeedObserve, Ini);
		User.nAnimateSpeedExam    = GetPrivateProfileInt("Board", "Animate speed", User.nAnimateSpeedExam,    Ini);
		User.nAnimateSpeedPlay    = GetPrivateProfileInt("Board", "Animate speed", User.nAnimateSpeedPlay,    Ini);
	}
	else
	{
		// Animate speed observe
		User.nAnimateSpeedObserve = GetPrivateProfileInt("Board", "Animate speed observe", User.nAnimateSpeedObserve, Ini);

		// Animate speed exam
		User.nAnimateSpeedExam = GetPrivateProfileInt("Board", "Animate speed exam", User.nAnimateSpeedExam, Ini);

		// Animate speed play
		User.nAnimateSpeedPlay = GetPrivateProfileInt("Board", "Animate speed play", User.nAnimateSpeedPlay, Ini);
	}

	if(User.nAnimateSpeedObserve < 0)
	{
		User.nAnimateSpeedObserve = 0;
	}
	else if(User.nAnimateSpeedObserve >= MAX_ANIMATE_SPEED)
	{
		User.nAnimateSpeedObserve = MAX_ANIMATE_SPEED - 1;
	}

	if(User.nAnimateSpeedExam < 0)
	{
		User.nAnimateSpeedExam = 0;
	}
	else if(User.nAnimateSpeedExam >= MAX_ANIMATE_SPEED)
	{
		User.nAnimateSpeedExam = MAX_ANIMATE_SPEED - 1;
	}

	if(User.nAnimateSpeedPlay < 0)
	{
		User.nAnimateSpeedPlay = 0;
	}
	else if(User.nAnimateSpeedPlay >= MAX_ANIMATE_SPEED)
	{
		User.nAnimateSpeedPlay = MAX_ANIMATE_SPEED - 1;
	}

	// use long alegrbraic notation for partner assist ?
	User.bLongPartner = GetPrivateProfileInt("Board", "Use long algebraic notation for partner assist", User.bLongPartner, Ini);

	// partner assist move prefix
	GetPrivateProfileString("Board", "Partner assist move prefix", User.cPartnerMove1, User.cPartnerMove1, MAX_PARTNER_ASSIST_SIZE, Ini);

	// partner assist sit for piece move prefix
	GetPrivateProfileString("Board", "Partner assist sit for piece move prefix", User.cPartnerMove2, User.cPartnerMove2, MAX_PARTNER_ASSIST_SIZE, Ini);

	// partner assist opponent move prefix
	GetPrivateProfileString("Board", "Partner assist opponent move prefix", User.cPartnerMove3, User.cPartnerMove3, MAX_PARTNER_ASSIST_SIZE, Ini);

	// partner assist opponent sit for piece move prefix
	GetPrivateProfileString("Board", "Partner assist opponent sit for piece move prefix", User.cPartnerMove4, User.cPartnerMove4, MAX_PARTNER_ASSIST_SIZE, Ini);

	// show move buttons
	User.bShowMoveButton = GetPrivateProfileInt("Board", "Show move buttons", User.bShowMoveButton, Ini);

	// use arrow keys for move buttons
	nJ = -1;
	nJ = GetPrivateProfileInt("Board", "User arrow keys for move buttons", nJ, Ini);
	if(nJ == -1)
	{
		User.bUseArrowKeyMoveButton = GetPrivateProfileInt("Hidden", "User arrow keys for move buttons", User.bUseArrowKeyMoveButton, Ini);
	}
	else
	{
		User.bUseArrowKeyMoveButton = nJ;
	}

	// show actual board size?
	User.bActualSize = GetPrivateProfileInt("Board", "Show actual board size", User.bActualSize, Ini);

	// stop resize playing board while playing
	User.bStopResizePlay = GetPrivateProfileInt("Board", "Stop resize playing board while playing", User.bStopResizePlay, Ini);

	// escape key cancels drag move
	User.bEscCancelDragMove = GetPrivateProfileInt("Board", "Escape key cancels drag move", User.bEscCancelDragMove, Ini);

	// escape key cancels true premove
	User.bEscCancelTruePremove = GetPrivateProfileInt("Board", "Escape key cancels true premove", User.bEscCancelTruePremove, Ini);

	// escape key cancels sit drop move
	User.bEscCancelSitDropMove = GetPrivateProfileInt("Board", "Escape key cancels sit drop move", User.bEscCancelSitDropMove, Ini);

	// escape key cancels auto knight promotion
	User.bEscCancelPromoteKnight = GetPrivateProfileInt("Board", "Escape key cancels auto knight promotion", User.bEscCancelPromoteKnight, Ini);

	// no play abuser
	User.bNoPlayAbuser = GetPrivateProfileInt("Board", "No play abuser", User.bNoPlayAbuser, Ini);

	// no play computer
	User.bNoPlayComputer = GetPrivateProfileInt("Board", "No play computer", User.bNoPlayComputer, Ini);

	// play wild 5 auto flip
	User.nPlayWild5Flip = GetPrivateProfileInt("Board", "Play wild 5 auto flip", User.nPlayWild5Flip, Ini);

	// show result?
	User.bShowResult = GetPrivateProfileInt("Board", "Show result", User.bShowResult, Ini);

	// show rating?
	User.bShowRating = GetPrivateProfileInt("Board", "Show rating", User.bShowRating, Ini);

	// show lag stat?
	User.bShowLagStat = GetPrivateProfileInt("Board", "Show lag stat", User.bShowLagStat, Ini);

	// show partner tell?
	User.bShowPtell = GetPrivateProfileInt("Board", "Show partner tell", User.bShowPtell, Ini);

	// show coordinates?
	User.bShowCoord = GetPrivateProfileInt("Board", "Show coordinates", User.bShowCoord, Ini);

	// show game type?
	User.bShowGameType = GetPrivateProfileInt("Board", "Show game type", User.bShowGameType, Ini);

	// show game type on window title bar
	nJ = -1;
	nJ = GetPrivateProfileInt("Board", "Show game type on window title bar", nJ, Ini);
	if(nJ == -1)
	{
		User.bShowGameTypeOnTitleBar = GetPrivateProfileInt("Hidden", "Show game type on window title bar", User.bShowGameTypeOnTitleBar, Ini);
	}
	else
	{
		User.bShowGameTypeOnTitleBar = nJ;
	}

	// show piece value on window title bar
	User.bShowValueOnTitleBar = GetPrivateProfileInt("Board", "Show piece value on window title bar", User.bShowValueOnTitleBar, Ini);

	// show last move?
	User.bShowLastMove = GetPrivateProfileInt("Board", "Show last move", User.bShowLastMove, Ini);

	// show last move time in tenth of a second (FICS)?
	User.bShowLastMoveTenth = GetPrivateProfileInt("Board", "Show last move time in tenth of a second (FICS)", User.bShowLastMoveTenth, Ini);

	// show last move square highlight?
	User.bShowHighlight = GetPrivateProfileInt("Board", "Show last move square highlight", User.bShowHighlight, Ini);

	if(! User.bShowHighlight)
	{
		nI = 0;
		nI = GetPrivateProfileInt("Board", "Show last move square highlight (thick line)", nI, Ini);
		if(nI)
		{
			bNeedSave = 1;
			User.bShowHighlight = 1;
		}
	}

	// show true premove square highlight?
	User.bShowTPHighlight = GetPrivateProfileInt("Board", "Show true premove square highlight", User.bShowTPHighlight, Ini);
	if(! User.bShowTPHighlight)
	{
		nI = 0;
		nI = GetPrivateProfileInt("Board", "Show true premove square highlight (thick line)", nI, Ini);
		if(nI)
		{
			bNeedSave = 1;
			User.bShowTPHighlight = 1;
		}
	}

	// show clock on top?
	User.bShowClockOnTop = GetPrivateProfileInt("Board", "Show clock on top", User.bShowClockOnTop, Ini);

	// show king highlight (play my piece) ?
	User.nShowKingHighlightPM = GetPrivateProfileInt("Board", "Show king highlight (play my piece)", User.nShowKingHighlightPM, Ini);

	// show king highlight (play opponent piece) ?
	User.nShowKingHighlightPO = GetPrivateProfileInt("Board", "Show king highlight (play opponent piece)", User.nShowKingHighlightPO, Ini);

	// show king highlight (exam) ?
	User.nShowKingHighlightE = GetPrivateProfileInt("Board", "Show king highlight (exam)", User.nShowKingHighlightE, Ini);

	// show king highlight (observe) ?
	User.nShowKingHighlightO = GetPrivateProfileInt("Board", "Show king highlight (observe)", User.nShowKingHighlightO, Ini);

	// adjust king highlight
	User.bShowKingHighlight = (User.nShowKingHighlightPM ||
							   User.nShowKingHighlightPO ||
							   User.nShowKingHighlightE  ||
							   User.nShowKingHighlightO);

	// show captured piece for chess game?
	User.nShowCapturedChessPiece = GetPrivateProfileInt("Board", "Show captured piece for chess game", User.nShowCapturedChessPiece, Ini);

	// piece buffer orientation
	User.nBufferOrientation = GetPrivateProfileInt("Board", "Piece buffer orientation", User.nBufferOrientation, Ini);

	switch(User.nBufferOrientation)
	{
		case DEFAULT_BUFFER_LEFT :
		case DEFAULT_BUFFER_RIGHT :
		case DEFAULT_BUFFER_TOPBOTTOML :
		case DEFAULT_BUFFER_TOPBOTTOMR :
			break;

		default :
			User.nBufferOrientation = DEFAULT_BUFFER_RIGHT;
			break;
	}

	// auto queen?
	User.bAutoQueen = GetPrivateProfileInt("Board", "Auto queen", User.bAutoQueen, Ini);

	// auto flag?
	User.bAutoFlag = GetPrivateProfileInt("Board", "Auto flag", User.bAutoFlag, Ini);

	// auto rematch?
	User.bAutoRematch = GetPrivateProfileInt("Board", "Auto rematch", User.bAutoRematch, Ini);

	// auto observe?
	User.bAutoObserve = GetPrivateProfileInt("Board", "Auto observe", User.bAutoObserve, Ini);

	// auto observe game notification?
	User.bAutoObserveGNotify = GetPrivateProfileInt("Board", "Auto observe game notification", User.bAutoObserveGNotify, Ini);

	// apply final count down?
	User.bFinalCountDown = GetPrivateProfileInt("Board", "Apply final countdown", User.bFinalCountDown, Ini);

	// game window color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_WINDOW_COLOR], "Board", "Game window color");

	// board outline color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_BOARD_OUTLINE], "Board", "Board outline color");

	// buffer outline color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_BUFFER_OUTLINE], "Board", "Buffer outline color");

	// buffer fill color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_BUFFER_FILL], "Board", "Buffer fill color");

	// white square outline color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_WHITE_SQUARE_OUTLINE], "Board", "White square outline color");

	// white square fill color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_WHITE_SQUARE_FILL], "Board", "White square fill color");

	// black square outline color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_BLACK_SQUARE_OUTLINE], "Board", "Black square outline color");

	// black square fill color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_BLACK_SQUARE_FILL], "Board", "Black square fill color");

	// piece number color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_PIECE_NUMBER], "Board", "Piece number color");

	// white piece outline color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_WHITE_PIECE_OUTLINE], "Board", "White piece outline color");

	// white piece fill color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_WHITE_PIECE_FILL], "Board", "White piece fill color");

	// black piece outline color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_BLACK_PIECE_OUTLINE], "Board", "Black piece outline color");

	// black piece fill color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_BLACK_PIECE_FILL], "Board", "Black piece fill color");

	// handle color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_HANDLE], "Board", "Handle color");

	// clock on color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_CLOCK_ON], "Board", "Clock on color");

	// clock on background color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_CLOCK_ON_BACKGROUND], "Board", "Clock on background color");

	// clock off color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_CLOCK_OFF], "Board", "Clock off color");

	// Time Down (Bughouse) color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_CLOCK_TIMEDOWN], "Board", "Team Down color");

	// Time Up (Bughouse) color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_CLOCK_TIMEUP], "Board", "Team Up color");

	// lag stat color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_LAGSTAT], "Board", "Lag stat color");

	// red clock
	RedClock.nRedClock = GetPrivateProfileInt("Board", "Red clock second", RedClock.nRedClock, Ini);

	// show tenth of second?
	RedClock.nShowTenthSecond = GetPrivateProfileInt("Board", "Show tenth of second", RedClock.nShowTenthSecond, Ini);

	// show less than one second?
	RedClock.bShowOneSecond = GetPrivateProfileInt("Board", "Show less than one second", RedClock.bShowOneSecond, Ini);

	// game type color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_GAMETYPE], "Board", "Game type color");

	// last move color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_LASTMOVE], "Board", "Last move color");

	// last move square highlight color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_HIGHLIGHT], "Board", "Last move square highlight color");

	// true premove square highlight color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TRUE_PREMOVE_HIGHLIGHT], "Board", "True premove square highlight");

	// illegal true premove square highlight color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_ILLEGAL_TRUE_PREMOVE], "Board", "Illegal true premove square highlight");

	// king legal square highlight color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_KING_HIGHLIGHT], "Board", "King legal square highlight");

	// result text color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_RESULT_FOREGROUND], "Board", "Result text color");

	// result background color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_RESULT_BACKGROUND], "Board", "Result background color");

	// ptell text color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_PTELL_FOREGROUND], "Board", "Ptell text color");

	// ptell background color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_PTELL_BACKGROUND], "Board", "Ptell background color");

	// game info text color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_GINFO_FOREGROUND], "Board", "Game info text color");

	// game info background color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_GINFO_BACKGROUND], "Board", "Game info background color");

	// coordinates text color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_COORDINATES_FOREGROUND], "Board", "Coordinates text color");

	// handle font
	TOOLBOX_ReadINIFont(Ini, FONT_HANDLE, "Board", "Handle font");

	// clock font
	TOOLBOX_ReadINIFont(Ini, FONT_CLOCK, "Board", "Clock font");

	// game type font
	TOOLBOX_ReadINIFont(Ini, FONT_GAMETYPE, "Board", "Game type font");

	// last move font
	TOOLBOX_ReadINIFont(Ini, FONT_LASTMOVE, "Board", "Last move font");

	// lag stat font
	TOOLBOX_ReadINIFont(Ini, FONT_LAGSTAT, "Board", "Lag stat font");

	// result font
	TOOLBOX_ReadINIFont(Ini, FONT_RESULT, "Board", "Result font");

	// coordindates font
	TOOLBOX_ReadINIFont(Ini, FONT_COORDINATES, "Board", "Coordinates font");

	// Apply Silence
	Silence.bSilenceApply = GetPrivateProfileInt("Silence", "Apply Silence", Silence.bSilenceApply, Ini);

	// Silence Message
	GetPrivateProfileString("Silence", "Silence Tell Message", Silence.cTell, Silence.cTell, THIEF_COMMAND_SIZE, Ini);

	// Silence Always Shout
	Silence.bSilenceShout = GetPrivateProfileInt("Silence", "Silence Always Shout", Silence.bSilenceShout, Ini);

	// Silence Always c-Shout
	Silence.bSilenceCShout = GetPrivateProfileInt("Silence", "Silence Always CShout", Silence.bSilenceCShout, Ini);

	// Silence Always Announcement
	Silence.bSilenceAnnouncement = GetPrivateProfileInt("Silence", "Silence Always Announcement", Silence.bSilenceAnnouncement, Ini);

	// Silence Always Channel Tell
	Silence.bSilenceChTell = GetPrivateProfileInt("Silence", "Silence Always Channel Tell", Silence.bSilenceChTell, Ini);

	// Silence Always Personal Tell
	Silence.bSilencePeTell = GetPrivateProfileInt("Silence", "Silence Always Personal Tell", Silence.bSilencePeTell, Ini);

	// Silence Always Say
	Silence.bSilenceSay = GetPrivateProfileInt("Silence", "Silence Always Say", Silence.bSilenceSay, Ini);

	// Silence Always Kibitz
	Silence.bSilenceKibitz = GetPrivateProfileInt("Silence", "Silence Always Kibitz", Silence.bSilenceKibitz, Ini);

	// Silence Always Whisper
	Silence.bSilenceWhisper = GetPrivateProfileInt("Silence", "Silence Always Whisper", Silence.bSilenceWhisper, Ini);

	// Silence Playing Shout
	Silence.bSilencePlayShout = GetPrivateProfileInt("Silence", "Silence Playing Shout", Silence.bSilencePlayShout, Ini);

	// Silence Playing c-Shout
	Silence.bSilencePlayCShout = GetPrivateProfileInt("Silence", "Silence Playing CShout", Silence.bSilencePlayCShout, Ini);

	// Silence Playing Announcement
	Silence.bSilencePlayAnnouncement = GetPrivateProfileInt("Silence", "Silence Playing Announcement", Silence.bSilencePlayAnnouncement, Ini);

	// Silence Playing Channel Tell
	Silence.bSilencePlayChTell = GetPrivateProfileInt("Silence", "Silence Playing Channel Tell", Silence.bSilencePlayChTell, Ini);

	// Silence Playing Personal Tell
	Silence.bSilencePlayPeTell = GetPrivateProfileInt("Silence", "Silence Playing Personal Tell", Silence.bSilencePlayPeTell, Ini);

	// Silence Playing Say
	Silence.bSilencePlaySay = GetPrivateProfileInt("Silence", "Silence Playing Say", Silence.bSilencePlaySay, Ini);

	// Silence Playing Kibitz
	Silence.bSilencePlayKibitz = GetPrivateProfileInt("Silence", "Silence Playing Kibitz", Silence.bSilencePlayKibitz, Ini);

	// Silence Playing Whisper
	Silence.bSilencePlayWhisper = GetPrivateProfileInt("Silence", "Silence Playing Whisper", Silence.bSilencePlayWhisper, Ini);

	//
	// [Telnet]
	//

	// log telnet?
	User.bLogTelnet = GetPrivateProfileInt("Telnet", "Log telnet", User.bLogTelnet, Ini);

	// apply mouse wheel?
	User.bMouseWheel = GetPrivateProfileInt("Telnet", "Apply mouse wheel", User.bMouseWheel, Ini);

	// show underline?
	User.bShowUnderline = GetPrivateProfileInt("Telnet", "Show underline", User.bShowUnderline, Ini);

	// enter key highlights input text?
	User.bEnterHighlight = GetPrivateProfileInt("Telnet", "Enter key highlights input text", User.bEnterHighlight, Ini);

	// go bottom after telnet resize
	User.bTelnetResizeGoBottom = GetPrivateProfileInt("Telnet", "Go bottom after telnet resize", User.bTelnetResizeGoBottom, Ini);

	// suppress announcement
	User.bSuppressAnnouncement = GetPrivateProfileInt("Telnet", "Suppress announcement", User.bSuppressAnnouncement, Ini);

	// suppress computer kibitz
	User.bSuppressCompKibitz = GetPrivateProfileInt("Telnet", "Suppress computer kibitz", User.bSuppressCompKibitz, Ini);

	// suppress computer whisper
	User.bSuppressCompWhisper = GetPrivateProfileInt("Telnet", "Suppress computer whisper", User.bSuppressCompWhisper, Ini);

	// suppress ignore formula
	User.bSuppressIgnoreFormula = GetPrivateProfileInt("Telnet", "Suppress ignore formula", User.bSuppressIgnoreFormula, Ini);

	// suppress ROBO welcome message
	User.bSuppressROBOWelcome = GetPrivateProfileInt("Telnet", "Suppress ROBO welcome message", User.bSuppressROBOWelcome, Ini);

	// suppress computer channel tell
	User.bSuppressCompChTell = GetPrivateProfileInt("Telnet", "Suppress computer channel tell", User.bSuppressCompChTell, Ini);

	// suppress computer tell
	User.bSuppressCompTell = GetPrivateProfileInt("Telnet", "Suppress computer tell", User.bSuppressCompTell, Ini);

	// suppress computer shout
	User.bSuppressCompShout = GetPrivateProfileInt("Telnet", "Suppress computer shout", User.bSuppressCompShout, Ini);

	// Console Time Stamp
	User.bTimeStampTelnet = GetPrivateProfileInt("Telnet", "Console Time Stamp", User.bTimeStampTelnet, Ini);

	// Log Time Stamp
	User.bTimeStampLog = GetPrivateProfileInt("Telnet", "Log Time Stamp", User.bTimeStampLog, Ini);

	// telnet font
	TOOLBOX_ReadINIFont(Ini, FONT_TELNET, "Telnet", "Telnet font");

	// telnet normal color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_NORMAL], "Telnet", "Normal color");

	// telnet tell color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_TELL], "Telnet", "Tell color");

	// telnet ptell color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_PTELL], "Telnet", "Ptell color");

	// telnet say color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_SAY], "Telnet", "Say color");

	// telnet kibitz color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_KIBITZ], "Telnet", "Kibitz color");

	// telnet whisper color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_WHISPER], "Telnet", "Whisper color");

	// telnet shout color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_SHOUT], "Telnet", "Shout color");

	// telnet cshout color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_CSHOUT], "Telnet", "Cshout color");

	// telnet challenge color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_CHALLENGE], "Telnet", "Challenge color");

	// user text color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_USERTEXT], "Telnet", "User text color");

	// telnet background color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_BACKGROUND], "Telnet", "Background color");

	// telnet notification color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_NOTIFICATION], "Telnet", "Notification color");

	// telnet :notification: color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_COLON_NOTIFY], "Telnet", "Colon notification color");

	// telnet arrival color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_ARRIVAL], "Telnet", "Arrival color");

	// telnet departure color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_DEPARTURE], "Telnet", "Departure color");

	// telnet game notification color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_GAME_NOTIFY], "Telnet", "Game notification color");

	// telnet announcement color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_ANNOUNCEMENT], "Telnet", "Announcement color");

	// telnet roombot (:) color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_COLON], "Telnet", "Roombot (:) color");

	// telnet :knighttour(256): xxx color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_COLON_256], "Telnet", "Colon 256 color");

	// telnet :knighttour(*)(C2): xxx color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_COLON_C2], "Telnet", "Colon c2 color");

	// telnet :knighttour(TM)(T3): xxx color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_COLON_T3], "Telnet", "Colon t3 color");

	// telnet :knighttour(TM) t-shouts: xxx color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_COLON_TSHOUTS], "Telnet", "Colon t-shouts color");

	// telnet :mamer color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_COLON_MAMER], "Telnet", "Colon mamer color");

	// telnet channel color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_CHANNEL], "Telnet", "Channel color");

	// telnet alternate channel color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_CHANNELA], "Telnet", "Alt Channel color");

	// telnet channel 1 color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_CHANNEL1], "Telnet", "Channel 1 color");

	// telnet channel 24 color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_CHANNEL24], "Telnet", "Channel 24 color");

	// telnet channel 97 color
	TOOLBOX_ReadINIColor(Ini, &clrColor [CLR_TELNET_CHANNEL97], "Telnet", "Channel 97 color");

	//
	// [Button]
	//

	// apply extended button?
	User.bExtendedButton = GetPrivateProfileInt("Button", "Apply extended button", User.bExtendedButton, Ini);

	// button font
	TOOLBOX_ReadINIFont(Ini, FONT_BUTTON, "Button", "Button font");

	// button label and action
	nJ = -1;
	nJ = GetPrivateProfileInt("Button", "Type 1", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
		for(nI = 0 ; nI < MAX_BUTTON ; nI++)
		{
			sprintf(cTmp, "Button %d", nI + 1);
			GetPrivateProfileString("Button", cTmp, Buttons.cText [nI], Buttons.cText [nI], MAX_BUTTON_TEXT, Ini);

			Buttons.nType [nI] = FUNCTION_COMMAND;

			sprintf(cTmp, "Action %d", nI + 1);
			GetPrivateProfileString("Button", cTmp, Buttons.cAction [nI], Buttons.cAction [nI], MAX_BUTTON_ACTION, Ini);

			TOOLBOX_AllTrim(Buttons.cText   [nI]);
			TOOLBOX_AllTrim(Buttons.cAction [nI]);
		}
		Buttons.nType [4] = FUNCTION_MATCH_BOX;
		Buttons.nType [8] = FUNCTION_SEEK_BOX;
	}
	else
	{
		for(nI = 0 ; nI < MAX_BUTTON ; nI++)
		{
			sprintf(cTmp, "Button %d", nI + 1);
			GetPrivateProfileString("Button", cTmp, Buttons.cText [nI], Buttons.cText [nI], MAX_BUTTON_TEXT, Ini);

			sprintf(cTmp, "Type %d", nI + 1);
			Buttons.nType [nI] = GetPrivateProfileInt("Button", cTmp, Buttons.nType [nI], Ini);

			sprintf(cTmp, "Action %d", nI + 1);
			GetPrivateProfileString("Button", cTmp, Buttons.cAction [nI], Buttons.cAction [nI], MAX_BUTTON_ACTION, Ini);

			TOOLBOX_AllTrim(Buttons.cText [nI]);
			TOOLBOX_AllTrim(Buttons.cAction [nI]);

			if((Buttons.nType [nI] >= 0) && (Buttons.nType [nI] < MAX_FMENU))
			{
				if(! TOOLBOX_NeedCommandValue(Buttons.nType [nI]))
				{
					strcpy(Buttons.cAction [nI], "");
				}
			}
			else
			{
				Buttons.nType [nI] = FUNCTION_COMMAND;
			}
		}
	}

	//
	// [Sound]
	//
	User.bSound = GetPrivateProfileInt("Sound", "Sound", User.bSound, Ini);

	//
	// [Telnet sound]
	//

	// telnet sound
	User.bTelnetSound = GetPrivateProfileInt("Telnet sound", "Telnet sound", User.bTelnetSound, Ini);

	Sounds.bBeep [SOUND_TELL] = GetPrivateProfileInt("Telnet sound", "Tell beep", Sounds.bBeep [SOUND_TELL], Ini);
	GetPrivateProfileString("Telnet sound", "Tell", Sounds.cName [SOUND_TELL], Sounds.cName [SOUND_TELL], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_TELL], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_TELL]));

	Sounds.bBeep [SOUND_PTELL] = GetPrivateProfileInt("Telnet sound", "Ptell beep", Sounds.bBeep [SOUND_PTELL], Ini);
	GetPrivateProfileString("Telnet sound", "Ptell", Sounds.cName [SOUND_PTELL], Sounds.cName [SOUND_PTELL], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_PTELL], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_PTELL]));

	Sounds.bBeep [SOUND_SAY] = GetPrivateProfileInt("Telnet sound", "Say beep", Sounds.bBeep [SOUND_SAY], Ini);
	GetPrivateProfileString("Telnet sound", "Say", Sounds.cName [SOUND_SAY], Sounds.cName [SOUND_SAY], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_SAY], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_SAY]));

	Sounds.bBeep [SOUND_KIBITZ] = GetPrivateProfileInt("Telnet sound", "Kibitz beep", Sounds.bBeep [SOUND_KIBITZ], Ini);
	GetPrivateProfileString("Telnet sound", "Kibitz", Sounds.cName [SOUND_KIBITZ], Sounds.cName [SOUND_KIBITZ], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_KIBITZ], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_KIBITZ]));

	Sounds.bBeep [SOUND_WHISPER] = GetPrivateProfileInt("Telnet sound", "Whisper beep", Sounds.bBeep [SOUND_WHISPER], Ini);
	GetPrivateProfileString("Telnet sound", "Whisper", Sounds.cName [SOUND_WHISPER], Sounds.cName [SOUND_WHISPER], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_WHISPER], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_WHISPER]));

	Sounds.bBeep [SOUND_SHOUT] = GetPrivateProfileInt("Telnet sound", "Shout beep", Sounds.bBeep [SOUND_SHOUT], Ini);
	GetPrivateProfileString("Telnet sound", "Shout", Sounds.cName [SOUND_SHOUT], Sounds.cName [SOUND_SHOUT], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_SHOUT], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_SHOUT]));

	Sounds.bBeep [SOUND_CSHOUT] = GetPrivateProfileInt("Telnet sound", "Cshout beep", Sounds.bBeep [SOUND_CSHOUT], Ini);
	GetPrivateProfileString("Telnet sound", "Cshout", Sounds.cName [SOUND_CSHOUT], Sounds.cName [SOUND_CSHOUT], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_CSHOUT], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_CSHOUT]));

	Sounds.bBeep [SOUND_CHALLENGE] = GetPrivateProfileInt("Telnet sound", "Challenge beep", Sounds.bBeep [SOUND_CHALLENGE], Ini);
	GetPrivateProfileString("Telnet sound", "Challenge", Sounds.cName [SOUND_CHALLENGE], Sounds.cName [SOUND_CHALLENGE], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_CHALLENGE], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_CHALLENGE]));

	Sounds.bBeep [SOUND_ABORT_REQUEST] = GetPrivateProfileInt("Telnet sound", "ABort beep", Sounds.bBeep [SOUND_ABORT_REQUEST], Ini);
	GetPrivateProfileString("Telnet sound", "Abort", Sounds.cName [SOUND_ABORT_REQUEST], Sounds.cName [SOUND_ABORT_REQUEST], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_ABORT_REQUEST], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_ABORT_REQUEST]));

	Sounds.bBeep [SOUND_PARTNER] = GetPrivateProfileInt("Telnet sound", "Partner beep", Sounds.bBeep [SOUND_PARTNER], Ini);
	GetPrivateProfileString("Telnet sound", "Partner", Sounds.cName [SOUND_PARTNER], Sounds.cName [SOUND_PARTNER], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_PARTNER], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_PARTNER]));

	Sounds.bBeep [SOUND_NOTIFICATION] = GetPrivateProfileInt("Telnet sound", "Notification beep", Sounds.bBeep [SOUND_NOTIFICATION], Ini);
	GetPrivateProfileString("Telnet sound", "Notification", Sounds.cName [SOUND_NOTIFICATION], Sounds.cName [SOUND_NOTIFICATION], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_NOTIFICATION], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_NOTIFICATION]));

	Sounds.bBeep [SOUND_ARRIVAL] = GetPrivateProfileInt("Telnet sound", "Arrival beep", Sounds.bBeep [SOUND_ARRIVAL], Ini);
	GetPrivateProfileString("Telnet sound", "Arrival", Sounds.cName [SOUND_ARRIVAL], Sounds.cName [SOUND_ARRIVAL], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_ARRIVAL], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_ARRIVAL]));

	Sounds.bBeep [SOUND_DEPARTURE] = GetPrivateProfileInt("Telnet sound", "Departure beep", Sounds.bBeep [SOUND_DEPARTURE], Ini);
	GetPrivateProfileString("Telnet sound", "Departure", Sounds.cName [SOUND_DEPARTURE], Sounds.cName [SOUND_DEPARTURE], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_DEPARTURE], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_DEPARTURE]));

	Sounds.bBeep [SOUND_GAME_NOTIFY] = GetPrivateProfileInt("Telnet sound", "Game notification beep", Sounds.bBeep [SOUND_GAME_NOTIFY], Ini);
	GetPrivateProfileString("Telnet sound", "Game notification", Sounds.cName [SOUND_GAME_NOTIFY], Sounds.cName [SOUND_GAME_NOTIFY], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_GAME_NOTIFY], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_GAME_NOTIFY]));

	// Takeback/adjourn request & draw offer sounds introduced with Thief 1.23, set defaults before reading possibly old Thief.ini

	Sounds.bBeep [SOUND_TAKEBACK_REQUEST] = 0;
	strcpy(Sounds.cName [SOUND_TAKEBACK_REQUEST], "COWBELL.WAV");
	strcpy(Sounds.cName [SOUND_TAKEBACK_REQUEST], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_TAKEBACK_REQUEST]));
	TOOLBOX_AllTrim(Sounds.cName [SOUND_TAKEBACK_REQUEST]);

	Sounds.bBeep [SOUND_DRAW_REQUEST] = 0;
	strcpy(Sounds.cName [SOUND_DRAW_REQUEST], "COWBELL.WAV");
	strcpy(Sounds.cName [SOUND_DRAW_REQUEST], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_DRAW_REQUEST]));
	TOOLBOX_AllTrim(Sounds.cName [SOUND_DRAW_REQUEST]);

	Sounds.bBeep [SOUND_ADJOURN_REQUEST] = 0;
	strcpy(Sounds.cName [SOUND_ADJOURN_REQUEST], "COWBELL.WAV");
	strcpy(Sounds.cName [SOUND_ADJOURN_REQUEST], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_ADJOURN_REQUEST]));
	TOOLBOX_AllTrim(Sounds.cName [SOUND_ADJOURN_REQUEST]);

	Sounds.bBeep [SOUND_TAKEBACK_REQUEST] = GetPrivateProfileInt("Telnet sound", "Takeback request beep", Sounds.bBeep [SOUND_TAKEBACK_REQUEST], Ini);
	GetPrivateProfileString("Telnet sound", "Takeback request", Sounds.cName [SOUND_TAKEBACK_REQUEST], Sounds.cName [SOUND_TAKEBACK_REQUEST], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_TAKEBACK_REQUEST], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_TAKEBACK_REQUEST]));

	Sounds.bBeep [SOUND_DRAW_REQUEST] = GetPrivateProfileInt("Telnet sound", "Draw offer beep", Sounds.bBeep [SOUND_DRAW_REQUEST], Ini);
	GetPrivateProfileString("Telnet sound", "Draw offer", Sounds.cName [SOUND_DRAW_REQUEST], Sounds.cName [SOUND_DRAW_REQUEST], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_DRAW_REQUEST], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_DRAW_REQUEST]));

	Sounds.bBeep [SOUND_ADJOURN_REQUEST] = GetPrivateProfileInt("Telnet sound", "Adjourn request beep", Sounds.bBeep [SOUND_ADJOURN_REQUEST], Ini);
	GetPrivateProfileString("Telnet sound", "Adjourn request", Sounds.cName [SOUND_ADJOURN_REQUEST], Sounds.cName [SOUND_ADJOURN_REQUEST], MAX_SOUND_NAME, Ini);
	strcpy(Sounds.cName [SOUND_ADJOURN_REQUEST], TOOLBOX_GetWaveFilename(Sounds.cName [SOUND_ADJOURN_REQUEST]));

	//
	// [Event sound]
	//

	// event sound
	User.bGameSound = GetPrivateProfileInt("Event sound", "Event sound", User.bGameSound, Ini);

	GameSounds.bBeep [GAME_SOUND_PLAY_START] = GetPrivateProfileInt("Event sound", "Playing board game start beep", GameSounds.bBeep [GAME_SOUND_PLAY_START], Ini);
	GetPrivateProfileString("Event sound", "Playing board game start", GameSounds.cName [GAME_SOUND_PLAY_START], GameSounds.cName [GAME_SOUND_PLAY_START], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_START], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_START]));

	GameSounds.bBeep [GAME_SOUND_PLAY_MOVE] = GetPrivateProfileInt("Event sound", "Playing board move beep", GameSounds.bBeep [GAME_SOUND_PLAY_MOVE], Ini);
	GetPrivateProfileString("Event sound", "Playing board move", GameSounds.cName [GAME_SOUND_PLAY_MOVE], GameSounds.cName [GAME_SOUND_PLAY_MOVE], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_MOVE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_MOVE]));

	GameSounds.bBeep [GAME_SOUND_PLAY_MOVE_SMART] = GetPrivateProfileInt("Event sound", "Playing board smart move beep", GameSounds.bBeep [GAME_SOUND_PLAY_MOVE_SMART], Ini);
	GetPrivateProfileString("Event sound", "Playing board smart move", GameSounds.cName [GAME_SOUND_PLAY_MOVE_SMART], GameSounds.cName [GAME_SOUND_PLAY_MOVE_SMART], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_MOVE_SMART], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_MOVE_SMART]));

	GameSounds.bBeep [GAME_SOUND_PLAY_MOVE_TP] = GetPrivateProfileInt("Event sound", "Playing board true premove beep", GameSounds.bBeep [GAME_SOUND_PLAY_MOVE_TP], Ini);
	GetPrivateProfileString("Event sound", "Playing board true premove", GameSounds.cName [GAME_SOUND_PLAY_MOVE_TP], GameSounds.cName [GAME_SOUND_PLAY_MOVE_TP], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_MOVE_TP], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_MOVE_TP]));

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Playing board capture beep", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_PLAY_CAPTURE] = GameSounds.bBeep [GAME_SOUND_PLAY_MOVE];
		strcpy(GameSounds.cName [GAME_SOUND_PLAY_CAPTURE], GameSounds.cName [GAME_SOUND_PLAY_MOVE]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_PLAY_CAPTURE] = GetPrivateProfileInt("Event sound", "Playing board capture beep", GameSounds.bBeep [GAME_SOUND_PLAY_CAPTURE], Ini);
		GetPrivateProfileString("Event sound", "Playing board capture", GameSounds.cName [GAME_SOUND_PLAY_CAPTURE], GameSounds.cName [GAME_SOUND_PLAY_CAPTURE], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_PLAY_CAPTURE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_CAPTURE]));
	}

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Playing board castle beep", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_PLAY_CASTLE] = GameSounds.bBeep [GAME_SOUND_PLAY_MOVE];
		strcpy(GameSounds.cName [GAME_SOUND_PLAY_CASTLE], GameSounds.cName [GAME_SOUND_PLAY_MOVE]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_PLAY_CASTLE] = GetPrivateProfileInt("Event sound", "Playing board castle beep", GameSounds.bBeep [GAME_SOUND_PLAY_CASTLE], Ini);
		GetPrivateProfileString("Event sound", "Playing board castle", GameSounds.cName [GAME_SOUND_PLAY_CASTLE], GameSounds.cName [GAME_SOUND_PLAY_CASTLE], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_PLAY_CASTLE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_CASTLE]));
	}

	GameSounds.bBeep [GAME_SOUND_PLAY_CHECK] = GetPrivateProfileInt("Event sound", "Playing board my king in check beep", GameSounds.bBeep [GAME_SOUND_PLAY_CHECK], Ini);
	GetPrivateProfileString("Event sound", "Playing board my king in check", GameSounds.cName [GAME_SOUND_PLAY_CHECK], GameSounds.cName [GAME_SOUND_PLAY_CHECK], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_CHECK], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_CHECK]));

	GameSounds.bBeep [GAME_SOUND_PLAY_OVER] = GetPrivateProfileInt("Event sound", "Playing board game over beep", GameSounds.bBeep [GAME_SOUND_PLAY_OVER], Ini);
	GetPrivateProfileString("Event sound", "Playing board game over", GameSounds.cName [GAME_SOUND_PLAY_OVER], GameSounds.cName [GAME_SOUND_PLAY_OVER], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PLAY_OVER], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PLAY_OVER]));

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Partner's board move beep", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_OBS_MOVE1] = GetPrivateProfileInt("Event sound", "Observe board move beep (playing)", GameSounds.bBeep [GAME_SOUND_OBS_MOVE1], Ini);
		GetPrivateProfileString("Event sound", "Observe board move (playing)", GameSounds.cName [GAME_SOUND_OBS_MOVE1], GameSounds.cName [GAME_SOUND_OBS_MOVE1], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_MOVE1], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_MOVE1]));

		GameSounds.bBeep [GAME_SOUND_PARTNER_MOVE] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE1];
		strcpy(GameSounds.cName [GAME_SOUND_PARTNER_MOVE], GameSounds.cName [GAME_SOUND_OBS_MOVE1]);

		GameSounds.bBeep [GAME_SOUND_PARTNER_CAPTURE] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE1];
		strcpy(GameSounds.cName [GAME_SOUND_PARTNER_CAPTURE], GameSounds.cName [GAME_SOUND_OBS_MOVE1]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_PARTNER_MOVE] = GetPrivateProfileInt("Event sound", "Partner's board move beep", GameSounds.bBeep [GAME_SOUND_PARTNER_MOVE], Ini);
		GetPrivateProfileString("Event sound", "Partner's board move", GameSounds.cName [GAME_SOUND_PARTNER_MOVE], GameSounds.cName [GAME_SOUND_PARTNER_MOVE], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_PARTNER_MOVE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_MOVE]));

		GameSounds.bBeep [GAME_SOUND_PARTNER_CAPTURE] = GetPrivateProfileInt("Event sound", "Partner's board capture beep", GameSounds.bBeep [GAME_SOUND_PARTNER_CAPTURE], Ini);
		GetPrivateProfileString("Event sound", "Partner's board capture", GameSounds.cName [GAME_SOUND_PARTNER_CAPTURE], GameSounds.cName [GAME_SOUND_PARTNER_CAPTURE], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_PARTNER_CAPTURE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_CAPTURE]));
	}

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Partner's board castle beep", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_OBS_MOVE1] = GetPrivateProfileInt("Event sound", "Observe board move beep (playing)", GameSounds.bBeep [GAME_SOUND_OBS_MOVE1], Ini);
		GetPrivateProfileString("Event sound", "Observe board move (playing)", GameSounds.cName [GAME_SOUND_OBS_MOVE1], GameSounds.cName [GAME_SOUND_OBS_MOVE1], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_MOVE1], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_MOVE1]));

		GameSounds.bBeep [GAME_SOUND_PARTNER_CASTLE] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE1];
		strcpy(GameSounds.cName [GAME_SOUND_PARTNER_CASTLE], GameSounds.cName [GAME_SOUND_OBS_MOVE1]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_PARTNER_CASTLE] = GetPrivateProfileInt("Event sound", "Partner's board castle beep", GameSounds.bBeep [GAME_SOUND_PARTNER_CASTLE], Ini);
		GetPrivateProfileString("Event sound", "Partner's board castle", GameSounds.cName [GAME_SOUND_PARTNER_CASTLE], GameSounds.cName [GAME_SOUND_PARTNER_CASTLE], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_PARTNER_CASTLE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_CASTLE]));
	}

	GameSounds.bBeep [GAME_SOUND_PARTNER_CHECK] = GetPrivateProfileInt("Event sound", "Partner's king is in check beep", GameSounds.bBeep [GAME_SOUND_PARTNER_CHECK], Ini);
	GetPrivateProfileString("Event sound", "Partner's king is in check", GameSounds.cName [GAME_SOUND_PARTNER_CHECK], GameSounds.cName [GAME_SOUND_PARTNER_CHECK], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_CHECK], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_CHECK]));

	GameSounds.bBeep [GAME_SOUND_OBS_MOVE1] = GetPrivateProfileInt("Event sound", "Observe board move beep (playing)", GameSounds.bBeep [GAME_SOUND_OBS_MOVE1], Ini);
	GetPrivateProfileString("Event sound", "Observe board move (playing)", GameSounds.cName [GAME_SOUND_OBS_MOVE1], GameSounds.cName [GAME_SOUND_OBS_MOVE1], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_OBS_MOVE1], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_MOVE1]));

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Observe board capture beep (playing)", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE1] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE1];
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CAPTURE1], GameSounds.cName [GAME_SOUND_OBS_MOVE1]);

		GameSounds.bBeep [GAME_SOUND_OBS_CHECK1] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE1];
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CHECK1], GameSounds.cName [GAME_SOUND_OBS_MOVE1]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE1] = GetPrivateProfileInt("Event sound", "Observe board capture beep (playing)", GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE1], Ini);
		GetPrivateProfileString("Event sound", "Observe board capture (playing)", GameSounds.cName [GAME_SOUND_OBS_CAPTURE1], GameSounds.cName [GAME_SOUND_OBS_CAPTURE1], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CAPTURE1], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_CAPTURE1]));

		GameSounds.bBeep [GAME_SOUND_OBS_CHECK1] = GetPrivateProfileInt("Event sound", "Observe board check beep (playing)", GameSounds.bBeep [GAME_SOUND_OBS_CHECK1], Ini);
		GetPrivateProfileString("Event sound", "Observe board check (playing)", GameSounds.cName [GAME_SOUND_OBS_CHECK1], GameSounds.cName [GAME_SOUND_OBS_CHECK1], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CHECK1], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_CHECK1]));
	}

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Observe board castle beep (playing)", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CASTLE1] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE1];
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CASTLE1], GameSounds.cName [GAME_SOUND_OBS_MOVE1]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CASTLE1] = GetPrivateProfileInt("Event sound", "Observe board castle beep (playing)", GameSounds.bBeep [GAME_SOUND_OBS_CASTLE1], Ini);
		GetPrivateProfileString("Event sound", "Observe board castle (playing)", GameSounds.cName [GAME_SOUND_OBS_CASTLE1], GameSounds.cName [GAME_SOUND_OBS_CASTLE1], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CASTLE1], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_CASTLE1]));
	}

	GameSounds.bBeep [GAME_SOUND_OBS_OVER1] = GetPrivateProfileInt("Event sound", "Observe board game over beep (playing)", GameSounds.bBeep [GAME_SOUND_OBS_OVER1], Ini);
	GetPrivateProfileString("Event sound", "Observe board game over (playing)", GameSounds.cName [GAME_SOUND_OBS_OVER1], GameSounds.cName [GAME_SOUND_OBS_OVER1], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_OBS_OVER1], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_OVER1]));

	GameSounds.bBeep [GAME_SOUND_OBS_MOVE2] = GetPrivateProfileInt("Event sound", "Observe board move beep (not playing)", GameSounds.bBeep [GAME_SOUND_OBS_MOVE2], Ini);
	GetPrivateProfileString("Event sound", "Observe board move (not playing)", GameSounds.cName [GAME_SOUND_OBS_MOVE2], GameSounds.cName [GAME_SOUND_OBS_MOVE2], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_OBS_MOVE2], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_MOVE2]));

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Observe board capture beep (not playing)", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE2] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE2];
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CAPTURE2], GameSounds.cName [GAME_SOUND_OBS_MOVE2]);

		GameSounds.bBeep [GAME_SOUND_OBS_CHECK2] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE2];
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CHECK2], GameSounds.cName [GAME_SOUND_OBS_MOVE2]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE2] = GetPrivateProfileInt("Event sound", "Observe board capture beep (not playing)", GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE2], Ini);
		GetPrivateProfileString("Event sound", "Observe board capture (not playing)", GameSounds.cName [GAME_SOUND_OBS_CAPTURE2], GameSounds.cName [GAME_SOUND_OBS_CAPTURE2], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CAPTURE2], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_CAPTURE2]));

		GameSounds.bBeep [GAME_SOUND_OBS_CHECK2] = GetPrivateProfileInt("Event sound", "Observe board check beep (not playing)", GameSounds.bBeep [GAME_SOUND_OBS_CHECK2], Ini);
		GetPrivateProfileString("Event sound", "Observe board check (not playing)", GameSounds.cName [GAME_SOUND_OBS_CHECK2], GameSounds.cName [GAME_SOUND_OBS_CHECK2], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CHECK2], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_CHECK2]));
	}

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Observe board castle beep (not playing)", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CASTLE2] = GameSounds.bBeep [GAME_SOUND_OBS_MOVE2];
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CASTLE2], GameSounds.cName [GAME_SOUND_OBS_MOVE2]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_OBS_CASTLE2] = GetPrivateProfileInt("Event sound", "Observe board castle beep (not playing)", GameSounds.bBeep [GAME_SOUND_OBS_CASTLE2], Ini);
		GetPrivateProfileString("Event sound", "Observe board castle (not playing)", GameSounds.cName [GAME_SOUND_OBS_CASTLE2], GameSounds.cName [GAME_SOUND_OBS_CASTLE2], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_OBS_CASTLE2], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_CASTLE2]));
	}

	GameSounds.bBeep [GAME_SOUND_OBS_OVER2] = GetPrivateProfileInt("Event sound", "Observe board game over beep (not playing)", GameSounds.bBeep [GAME_SOUND_OBS_OVER2], Ini);
	GetPrivateProfileString("Event sound", "Observe board game over (not playing)", GameSounds.cName [GAME_SOUND_OBS_OVER2], GameSounds.cName [GAME_SOUND_OBS_OVER2], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_OBS_OVER2], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_OBS_OVER2]));

	GameSounds.bBeep [GAME_SOUND_I_WON] = GetPrivateProfileInt("Event sound", "I won beep", GameSounds.bBeep [GAME_SOUND_I_WON], Ini);
	GetPrivateProfileString("Event sound", "I won", GameSounds.cName [GAME_SOUND_I_WON], GameSounds.cName [GAME_SOUND_I_WON], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_I_WON], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_I_WON]));

	GameSounds.bBeep [GAME_SOUND_I_LOST] = GetPrivateProfileInt("Event sound", "I lost beep", GameSounds.bBeep [GAME_SOUND_I_LOST], Ini);
	GetPrivateProfileString("Event sound", "I lost", GameSounds.cName [GAME_SOUND_I_LOST], GameSounds.cName [GAME_SOUND_I_LOST], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_I_LOST], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_I_LOST]));

	GameSounds.bBeep [GAME_SOUND_I_DREW] = GetPrivateProfileInt("Event sound", "I drew beep", GameSounds.bBeep [GAME_SOUND_I_DREW], Ini);
	GetPrivateProfileString("Event sound", "I drew", GameSounds.cName [GAME_SOUND_I_DREW], GameSounds.cName [GAME_SOUND_I_DREW], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_I_DREW], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_I_DREW]));

	GameSounds.bBeep [GAME_SOUND_I_ABORT] = GetPrivateProfileInt("Event sound", "I aborted beep", GameSounds.bBeep [GAME_SOUND_I_ABORT], Ini);
	GetPrivateProfileString("Event sound", "I aborted", GameSounds.cName [GAME_SOUND_I_ABORT], GameSounds.cName [GAME_SOUND_I_ABORT], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_I_ABORT], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_I_ABORT]));

	GameSounds.bBeep [GAME_SOUND_PARTNER_WON] = GetPrivateProfileInt("Event sound", "Partner won beep", GameSounds.bBeep [GAME_SOUND_PARTNER_WON], Ini);
	GetPrivateProfileString("Event sound", "Partner won", GameSounds.cName [GAME_SOUND_PARTNER_WON], GameSounds.cName [GAME_SOUND_PARTNER_WON], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_WON], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_WON]));

	GameSounds.bBeep [GAME_SOUND_PARTNER_LOST] = GetPrivateProfileInt("Event sound", "Partner lost beep", GameSounds.bBeep [GAME_SOUND_PARTNER_LOST], Ini);
	GetPrivateProfileString("Event sound", "Partner lost", GameSounds.cName [GAME_SOUND_PARTNER_LOST], GameSounds.cName [GAME_SOUND_PARTNER_LOST], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_LOST], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_LOST]));

//    GameSounds.bBeep [GAME_SOUND_PARTNER_DREW] = GetPrivateProfileInt ("Event sound", "Partner drew beep", GameSounds.bBeep [GAME_SOUND_PARTNER_DREW], Ini);
//    GetPrivateProfileString ("Event sound", "Partner drew", GameSounds.cName [GAME_SOUND_PARTNER_DREW], GameSounds.cName [GAME_SOUND_PARTNER_DREW], MAX_GAME_SOUND_NAME, Ini);
//    strcpy (GameSounds.cName [GAME_SOUND_PARTNER_DREW], TOOLBOX_GetWaveFilename (GameSounds.cName [GAME_SOUND_PARTNER_DREW]));
// 1.24

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Partner drew beep", nJ, Ini);
	if(nJ != -1)
	{
		GameSounds.bBeep [GAME_SOUND_PARTNER_DREW] = GetPrivateProfileInt("Event sound", "Partner drew beep", GameSounds.bBeep [GAME_SOUND_PARTNER_DREW], Ini);
		GetPrivateProfileString("Event sound", "Partner drew", GameSounds.cName [GAME_SOUND_PARTNER_DREW], GameSounds.cName [GAME_SOUND_PARTNER_DREW], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_PARTNER_DREW], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_DREW]));
	}

	GameSounds.bBeep [GAME_SOUND_PARTNER_ABORT] = GetPrivateProfileInt("Event sound", "Partner aborted beep", GameSounds.bBeep [GAME_SOUND_PARTNER_ABORT], Ini);
	GetPrivateProfileString("Event sound", "Partner aborted", GameSounds.cName [GAME_SOUND_PARTNER_ABORT], GameSounds.cName [GAME_SOUND_PARTNER_ABORT], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_PARTNER_ABORT], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_PARTNER_ABORT]));

	GameSounds.bBeep [GAME_SOUND_EXAM_MOVE] = GetPrivateProfileInt("Event sound", "Exam move beep", GameSounds.bBeep [GAME_SOUND_EXAM_MOVE], Ini);
	GetPrivateProfileString("Event sound", "Exam move", GameSounds.cName [GAME_SOUND_EXAM_MOVE], GameSounds.cName [GAME_SOUND_EXAM_MOVE], MAX_GAME_SOUND_NAME, Ini);
	strcpy(GameSounds.cName [GAME_SOUND_EXAM_MOVE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_EXAM_MOVE]));

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Exam capture beep", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_EXAM_CAPTURE] = GameSounds.bBeep [GAME_SOUND_EXAM_MOVE];
		strcpy(GameSounds.cName [GAME_SOUND_EXAM_CAPTURE], GameSounds.cName [GAME_SOUND_EXAM_MOVE]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_EXAM_CAPTURE] = GetPrivateProfileInt("Event sound", "Exam capture beep", GameSounds.bBeep [GAME_SOUND_EXAM_CAPTURE], Ini);
		GetPrivateProfileString("Event sound", "Exam capture", GameSounds.cName [GAME_SOUND_EXAM_CAPTURE], GameSounds.cName [GAME_SOUND_EXAM_CAPTURE], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_EXAM_CAPTURE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_EXAM_CAPTURE]));
	}

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Exam castle beep", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_EXAM_CASTLE] = GameSounds.bBeep [GAME_SOUND_EXAM_MOVE];
		strcpy(GameSounds.cName [GAME_SOUND_EXAM_CASTLE], GameSounds.cName [GAME_SOUND_EXAM_MOVE]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_EXAM_CASTLE] = GetPrivateProfileInt("Event sound", "Exam castle beep", GameSounds.bBeep [GAME_SOUND_EXAM_CASTLE], Ini);
		GetPrivateProfileString("Event sound", "Exam castle", GameSounds.cName [GAME_SOUND_EXAM_CASTLE], GameSounds.cName [GAME_SOUND_EXAM_CASTLE], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_EXAM_CASTLE], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_EXAM_CASTLE]));
	}

	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Exam check beep", nJ, Ini);
	if(nJ == -1)
	{
		GameSounds.bBeep [GAME_SOUND_EXAM_CHECK] = GameSounds.bBeep [GAME_SOUND_EXAM_MOVE];
		strcpy(GameSounds.cName [GAME_SOUND_EXAM_CHECK], GameSounds.cName [GAME_SOUND_EXAM_MOVE]);
	}
	else
	{
		GameSounds.bBeep [GAME_SOUND_EXAM_CHECK] = GetPrivateProfileInt("Event sound", "Exam check beep", GameSounds.bBeep [GAME_SOUND_EXAM_CHECK], Ini);
		GetPrivateProfileString("Event sound", "Exam check", GameSounds.cName [GAME_SOUND_EXAM_CHECK], GameSounds.cName [GAME_SOUND_EXAM_CHECK], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_EXAM_CHECK], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_EXAM_CHECK]));
	}

	// 1.24
	nJ = -1;
	nJ = GetPrivateProfileInt("Event sound", "Game Abort beep", nJ, Ini);
	if(nJ != -1)
	{
		GameSounds.bBeep [GAME_SOUND_GAME_ABORT] = GetPrivateProfileInt("Event sound", "Game Abort beep", GameSounds.bBeep [GAME_SOUND_GAME_ABORT], Ini);
		GetPrivateProfileString("Event sound", "Abort Game", GameSounds.cName [GAME_SOUND_GAME_ABORT], GameSounds.cName [GAME_SOUND_GAME_ABORT], MAX_GAME_SOUND_NAME, Ini);
		strcpy(GameSounds.cName [GAME_SOUND_GAME_ABORT], TOOLBOX_GetWaveFilename(GameSounds.cName [GAME_SOUND_GAME_ABORT]));
	}

	//
	// [Partner sound]
	//

	// partner sound
	User.bEarSound = GetPrivateProfileInt("Partner sound", "Partner sound", User.bEarSound, Ini);

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		sprintf(cTmp, "Tell %d", nI + 1);
		GetPrivateProfileString("Partner sound", cTmp, Ears.cTell [nI], Ears.cTell [nI], MAX_EAR_TELL, Ini);

		sprintf(cTmp, "Beep %d", nI + 1);
		Ears.bBeep [nI] = GetPrivateProfileInt("Partner sound", cTmp, Ears.bBeep [nI], Ini);

		sprintf(cTmp, "Wave %d", nI + 1);
		GetPrivateProfileString("Partner sound", cTmp, Ears.cName [nI], Ears.cName [nI], MAX_EAR_NAME, Ini);
		strcpy(Ears.cName [nI], TOOLBOX_GetWaveFilename(Ears.cName [nI]));

		TOOLBOX_AllTrim(Ears.cTell [nI]);
		TOOLBOX_AllTrim(Ears.cName [nI]);
	}

	//
	// [Time sound]
	//
	for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
	{
		sprintf(cTmp, "Wave %d", nI);
		GetPrivateProfileString("Time sound", cTmp, TimeSounds.cName [nI], TimeSounds.cName [nI], MAX_TIME_SOUND_NAME, Ini);
		strcpy(TimeSounds.cName [nI], TOOLBOX_GetWaveFilename(TimeSounds.cName [nI]));
		TOOLBOX_AllTrim(TimeSounds.cName [nI]);
	}

	//
	// [Login event]
	//
	for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
	{
		sprintf(cTmp, "Login event type %d", nI + 1);
		LEvent.nType [nI] = GetPrivateProfileInt("Login event", cTmp, LEvent.nType [nI], Ini);

		sprintf(cTmp, "Login event command %d", nI + 1);
		GetPrivateProfileString("Login event", cTmp, LEvent.cCommand [nI], LEvent.cCommand [nI], MAX_LOGIN_EVENT_COMMAND_SIZE, Ini);
	}

	//
	// [Game event]
	//
	GEvent.nType [GAME_EVENT_PLAY_START1] = GetPrivateProfileInt("Game event", "Playing board game start type 1", GEvent.nType [GAME_EVENT_PLAY_START1], Ini);
	GetPrivateProfileString("Game event", "Playing board game start command 1", GEvent.cCommand [GAME_EVENT_PLAY_START1], GEvent.cCommand [GAME_EVENT_PLAY_START1], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_PLAY_START2] = GetPrivateProfileInt("Game event", "Playing board game start type 2", GEvent.nType [GAME_EVENT_PLAY_START2], Ini);
	GetPrivateProfileString("Game event", "Playing board game start command 2", GEvent.cCommand [GAME_EVENT_PLAY_START2], GEvent.cCommand [GAME_EVENT_PLAY_START2], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_PLAY_BUG_START] = GetPrivateProfileInt("Game event", "Playing board bughouse game start type", GEvent.nType [GAME_EVENT_PLAY_BUG_START], Ini);
	GetPrivateProfileString("Game event", "Playing board bughouse game start command", GEvent.cCommand [GAME_EVENT_PLAY_START2], GEvent.cCommand [GAME_EVENT_PLAY_BUG_START], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_PLAY_ONE_START] = GetPrivateProfileInt("Game event", "Playing board one game start type", GEvent.nType [GAME_EVENT_PLAY_ONE_START], Ini);
	GetPrivateProfileString("Game event", "Playing board one game start command", GEvent.cCommand [GAME_EVENT_PLAY_START2], GEvent.cCommand [GAME_EVENT_PLAY_ONE_START], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_PLAY_OVER1] = GetPrivateProfileInt("Game event", "Playing board game over type 1", GEvent.nType [GAME_EVENT_PLAY_OVER1], Ini);
	GetPrivateProfileString("Game event", "Playing board game over command 1", GEvent.cCommand [GAME_EVENT_PLAY_OVER1], GEvent.cCommand [GAME_EVENT_PLAY_OVER1], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_PLAY_OVER2] = GetPrivateProfileInt("Game event", "Playing board game over type 2", GEvent.nType [GAME_EVENT_PLAY_OVER2], Ini);
	GetPrivateProfileString("Game event", "Playing board game over command 2", GEvent.cCommand [GAME_EVENT_PLAY_OVER2], GEvent.cCommand [GAME_EVENT_PLAY_OVER2], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_OBSERVE_START] = GetPrivateProfileInt("Game event", "Observe board game start type", GEvent.nType [GAME_EVENT_OBSERVE_START], Ini);
	GetPrivateProfileString("Game event", "Observe board game start command", GEvent.cCommand [GAME_EVENT_OBSERVE_START], GEvent.cCommand [GAME_EVENT_OBSERVE_START], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_OBSERVE_BUG_ST] = GetPrivateProfileInt("Game event", "Observe board bughouse game start type", GEvent.nType [GAME_EVENT_OBSERVE_BUG_ST], Ini);
	GetPrivateProfileString("Game event", "Observe board bughouse game start command", GEvent.cCommand [GAME_EVENT_OBSERVE_BUG_ST], GEvent.cCommand [GAME_EVENT_OBSERVE_BUG_ST], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_OBSERVE_ONE_ST] = GetPrivateProfileInt("Game event", "Observe board one game start type", GEvent.nType [GAME_EVENT_OBSERVE_ONE_ST], Ini);
	GetPrivateProfileString("Game event", "Observe board one game start command", GEvent.cCommand [GAME_EVENT_OBSERVE_ONE_ST], GEvent.cCommand [GAME_EVENT_OBSERVE_ONE_ST], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	GEvent.nType [GAME_EVENT_OBSERVE_OVER] = GetPrivateProfileInt("Game event", "Observe board game over type", GEvent.nType [GAME_EVENT_OBSERVE_OVER], Ini);
	GetPrivateProfileString("Game event", "Observe board game over command", GEvent.cCommand [GAME_EVENT_OBSERVE_OVER], GEvent.cCommand [GAME_EVENT_OBSERVE_OVER], MAX_GAME_EVENT_COMMAND_SIZE, Ini);

	//
	// [Button right mouse click event]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Button right mouse click event", "Event 1 type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
	}
	else
	{
		for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
		{
			sprintf(cTmp, "Event %d menu", nI + 1);
			GetPrivateProfileString("Button right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_BUTTON].cMenu [nI], RightMouse [RIGHT_MOUSE_BUTTON].cMenu [nI], MAX_RM_MENU_SIZE, Ini);

			sprintf(cTmp, "Event %d type", nI + 1);
			RightMouse [RIGHT_MOUSE_BUTTON].nType [nI] = GetPrivateProfileInt("Button right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_BUTTON].nType [nI], Ini);

			sprintf(cTmp, "Event %d command", nI + 1);
			GetPrivateProfileString("Button right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_BUTTON].cCommand [nI], RightMouse [RIGHT_MOUSE_BUTTON].cCommand [nI], MAX_RM_COMMAND_SIZE, Ini);
		}
	}

	//
	// [Play right mouse click event]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Play right mouse click event", "Event 1 type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
	}
	else
	{
		for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
		{
			sprintf(cTmp, "Event %d menu", nI + 1);
			GetPrivateProfileString("Play right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_PLAY].cMenu [nI], RightMouse [RIGHT_MOUSE_PLAY].cMenu [nI], MAX_RM_MENU_SIZE, Ini);

			sprintf(cTmp, "Event %d type", nI + 1);
			RightMouse [RIGHT_MOUSE_PLAY].nType [nI] = GetPrivateProfileInt("Play right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_PLAY].nType [nI], Ini);

			sprintf(cTmp, "Event %d command", nI + 1);
			GetPrivateProfileString("Play right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_PLAY].cCommand [nI], RightMouse [RIGHT_MOUSE_PLAY].cCommand [nI], MAX_RM_COMMAND_SIZE, Ini);
		}
	}

	//
	// [Observe right mouse click event]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Observe right mouse click event", "Event 1 type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
	}
	else
	{
		for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
		{
			sprintf(cTmp, "Event %d menu", nI + 1);
			GetPrivateProfileString("Observe right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [nI], RightMouse [RIGHT_MOUSE_OBSERVE].cMenu [nI], MAX_RM_MENU_SIZE, Ini);

			sprintf(cTmp, "Event %d type", nI + 1);
			RightMouse [RIGHT_MOUSE_OBSERVE].nType [nI] = GetPrivateProfileInt("Observe right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_OBSERVE].nType [nI], Ini);

			sprintf(cTmp, "Event %d command", nI + 1);
			GetPrivateProfileString("Observe right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_OBSERVE].cCommand [nI], RightMouse [RIGHT_MOUSE_OBSERVE].cCommand [nI], MAX_RM_COMMAND_SIZE, Ini);
		}
	}

	//
	// [Telnet text right mouse click event]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Telnet text right mouse click event", "Event 1 type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
	}
	else
	{
		for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
		{
			sprintf(cTmp, "Event %d menu", nI + 1);
			GetPrivateProfileString("Telnet text right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [nI], RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu [nI], MAX_RM_MENU_SIZE, Ini);

			sprintf(cTmp, "Event %d type", nI + 1);
			RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [nI] = GetPrivateProfileInt("Telnet text right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType [nI], Ini);

			sprintf(cTmp, "Event %d command", nI + 1);
			GetPrivateProfileString("Telnet text right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [nI], RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [nI], MAX_RM_COMMAND_SIZE, Ini);
		}
	}

	//
	// [Telnet edit right mouse click event]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Telnet edit right mouse click event", "Event 1 type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
	}
	else
	{
		for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
		{
			sprintf(cTmp, "Event %d menu", nI + 1);
			GetPrivateProfileString("Telnet edit right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [nI], RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu [nI], MAX_RM_MENU_SIZE, Ini);

			sprintf(cTmp, "Event %d type", nI + 1);
			RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType [nI] = GetPrivateProfileInt("Telnet edit right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType [nI], Ini);

			sprintf(cTmp, "Event %d command", nI + 1);
			GetPrivateProfileString("Telnet edit right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_TELNET_EDIT].cCommand [nI], RightMouse [RIGHT_MOUSE_TELNET_EDIT].cCommand [nI], MAX_RM_COMMAND_SIZE, Ini);
		}
	}

	//
	// [Board handle right mouse click event]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Board handle right mouse click event", "Event 1 type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
	}
	else
	{
		for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
		{
			sprintf(cTmp, "Event %d menu", nI + 1);
			GetPrivateProfileString("Board handle right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI], RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu [nI], MAX_RM_MENU_SIZE, Ini);

			sprintf(cTmp, "Event %d type", nI + 1);
			RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [nI] = GetPrivateProfileInt("Board handle right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType [nI], Ini);

			sprintf(cTmp, "Event %d command", nI + 1);
			GetPrivateProfileString("Board handle right mouse click event", cTmp, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [nI], RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [nI], MAX_RM_COMMAND_SIZE, Ini);
		}
	}

	//
	// [User]
	//
	GetPrivateProfileString("User", "User handle 1", Vars.cUserHandle1, Vars.cUserHandle1, ICS_HANDLE_LENGTH + 1, Ini);
	GetPrivateProfileString("User", "User handle 2", Vars.cUserHandle2, Vars.cUserHandle2, ICS_HANDLE_LENGTH + 1, Ini);

	//
	// [Timer]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Timer", "Type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
		TimerCmd.nType = FUNCTION_COMMAND;
		TimerCmd.nTimerMinute = GetPrivateProfileInt("Timer", "Minute", TimerCmd.nTimerMinute, Ini);
		GetPrivateProfileString("Timer", "Command", TimerCmd.cTimerCommand, TimerCmd.cTimerCommand, MAX_TIMER_LINE_SIZE, Ini);
		GetPrivateProfileString("Timer", "ICC Command", TimerCmd.cICCTimerCommand, TimerCmd.cICCTimerCommand, MAX_TIMER_LINE_SIZE, Ini);
	}
	else
	{
		TimerCmd.nTimerMinute = GetPrivateProfileInt("Timer", "Minute", TimerCmd.nTimerMinute, Ini);
		TimerCmd.nType = GetPrivateProfileInt("Timer", "Type", TimerCmd.nType, Ini);
		GetPrivateProfileString("Timer", "Command", TimerCmd.cTimerCommand, TimerCmd.cTimerCommand, MAX_TIMER_LINE_SIZE, Ini);
		GetPrivateProfileString("Timer", "ICC Command", TimerCmd.cICCTimerCommand, TimerCmd.cICCTimerCommand, MAX_TIMER_LINE_SIZE, Ini);
	}

	//
	// [Function key]
	//
	for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
	{
		if((nI + 1) >= 10)
		{
			sprintf(cTmp,  "Function key %d type",    nI + 2);
			sprintf(cTmp1, "Function key %d command", nI + 2);
			sprintf(cTmp2, "Function key %d",         nI + 2);
		}
		else
		{
			sprintf(cTmp,  "Function key %d type",    nI + 1);
			sprintf(cTmp1, "Function key %d command", nI + 1);
			sprintf(cTmp2, "Function key %d",         nI + 1);
		}

		nJ = -1;
		nJ = GetPrivateProfileInt("Function key", cTmp, nJ, Ini);
		if(nJ == -1)
		{
			bNeedSave = 1;
			GetPrivateProfileString("Function key", cTmp2, FKey.cFKey [nI], FKey.cFKey [nI], MAX_FUNCTION_KEY_SIZE, Ini);

			switch(nI)
			{
				case 0 :
					// set F1 as Ask for Help on Chess Server
					FKey.nType [0] = FUNCTION_QUESTION_BOX;
					break;

				case 1 :
					// set F2 as Flip Board
					FKey.nType [1] = FUNCTION_FLIP_BOARD;
					break;

				case 2 :
					// set F3 as Accept command
					FKey.nType [2] = FUNCTION_COMMAND;
					strcpy(FKey.cFKey [2], ICS_ACCEPT_COMMAND);
					break;

				case 3 :
					// set F4 as Decline command
					FKey.nType [3] = FUNCTION_COMMAND;
					strcpy(FKey.cFKey [3], ICS_DECLINE_COMMAND);
					break;

				case 4 :
					// set F5 as Flag command
					FKey.nType [4] = FUNCTION_COMMAND;
					strcpy(FKey.cFKey [4], ICS_FLAG1_COMMAND);
					break;

				case 5 :
					// set F6 as Abort command
					FKey.nType [5] = FUNCTION_COMMAND;
					strcpy(FKey.cFKey [5], ICS_ABORT_COMMAND);
					break;

				case 6 :
					// set F7 as Rematch command
					FKey.nType [6] = FUNCTION_COMMAND;
					strcpy(FKey.cFKey [6], ICS_REMATCH_COMMAND);
					break;

				case 7 :
					// set F8 as sit for piece drop move
					FKey.nType [7] = FUNCTION_SITPIECE_DROP_MOVE;
					break;

				case 8 :
					// set F9 as Telnet last tell
					FKey.nType [8] = FUNCTION_TELNET_LAST_TELL;
					break;

				case 9 :
					// set F11 as Toggle Maximize/Restore Telnet Console
					FKey.nType [9] = FUNCTION_TOGGLE_TELNET;
					break;

				case 10 :
					// set F12 as Toggle Timer Command On/Off
					FKey.nType [10] = FUNCTION_TOGGLE_TIMER_ONOFF;
					break;
			}
		}
		else
		{
			if((nJ < 0) || (nJ >= MAX_FMENU))
			{
				FKey.nType [nI] = FUNCTION_NONE;
			}
			else
			{
				FKey.nType [nI] = nJ;
			}

			if(TOOLBOX_NeedCommandValue(FKey.nType [nI]))
			{
				GetPrivateProfileString("Function key", cTmp1, FKey.cFKey [nI], FKey.cFKey [nI], MAX_FUNCTION_KEY_SIZE, Ini);
			}
			else
			{
				strcpy(FKey.cFKey [nI], "");
			}
		}
	}

	//
	// [Control key]
	//
	for(nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
	{
		if(nI < 26)
		{
			sprintf(cTmp,  "Control key %c type", (char)(nI + 'A'));
			sprintf(cTmp1, "Control key %c command", (char)(nI + 'A'));
		}
		else
		{
			sprintf(cTmp,  "Control key %c type", (char)((nI - 26) + '0'));
			sprintf(cTmp1, "Control key %c command", (char)((nI - 26) + '0'));
		}

		ControlKey.nType [nI] = GetPrivateProfileInt("Control key", cTmp, ControlKey.nType [nI], Ini);

		if((ControlKey.nType [nI] < 0) || (ControlKey.nType [nI] >= MAX_FMENU))
		{
			ControlKey.nType [nI] = FUNCTION_NONE;
		}

		if(TOOLBOX_NeedCommandValue(ControlKey.nType [nI]))
		{
			GetPrivateProfileString("Control key", cTmp1, ControlKey.cFKey [nI], ControlKey.cFKey [nI], MAX_CONTROL_KEY_SIZE, Ini);
		}
		else
		{
			strcpy(ControlKey.cFKey [nI], "");
		}
	}

	//
	// [Lag command]
	//
	nJ = -1;
	nJ = GetPrivateProfileInt("Lag command", "Lag type", nJ, Ini);
	if(nJ == -1)
	{
		bNeedSave = 1;
		LagCmd.nType = FUNCTION_COMMAND;
		User.bLagCommand     = GetPrivateProfileInt("Lag command", "Lag command", User.bLagCommand, Ini);
		LagCmd.nLagTolerance = GetPrivateProfileInt("Lag command", "Lag tolerance", LagCmd.nLagTolerance, Ini);
		LagCmd.nLagThreshold = GetPrivateProfileInt("Lag command", "Lag threshold", LagCmd.nLagThreshold, Ini);

		GetPrivateProfileString("Lag command", "Lag command to issue", LagCmd.cLagCommand, LagCmd.cLagCommand, MAX_LAG_COMMAND_SIZE, Ini);
		GetPrivateProfileString("Lag command", "Lag display", LagCmd.cLagDisplay, LagCmd.cLagDisplay, MAX_LAG_DISPLAY_SIZE, Ini);
	}
	else
	{
		User.bLagCommand     = GetPrivateProfileInt("Lag command", "Lag command", User.bLagCommand, Ini);
		LagCmd.nLagTolerance = GetPrivateProfileInt("Lag command", "Lag tolerance", LagCmd.nLagTolerance, Ini);
		LagCmd.nLagThreshold = GetPrivateProfileInt("Lag command", "Lag threshold", LagCmd.nLagThreshold, Ini);

		LagCmd.nType = GetPrivateProfileInt("Lag command", "Lag type", LagCmd.nType, Ini);
		GetPrivateProfileString("Lag command", "Lag command to issue", LagCmd.cLagCommand, LagCmd.cLagCommand, MAX_LAG_COMMAND_SIZE, Ini);
		GetPrivateProfileString("Lag command", "Lag display", LagCmd.cLagDisplay, LagCmd.cLagDisplay, MAX_LAG_DISPLAY_SIZE, Ini);
	}

	//
	// [Censor]
	//
	for(nC = 0 ; nC < MAX_CENSOR ; nC++)
	{
		if(nC == 0)
		{
			Censor [nC].bCensor = GetPrivateProfileInt("Censor", "FICS censor", Censor [nC].bCensor, Ini);
		}
		else if(nC == 1)
		{
			Censor [nC].bCensor = GetPrivateProfileInt("Censor", "ICC censor", Censor [nC].bCensor, Ini);
		}
		else
		{
			Censor [nC].bCensor = GetPrivateProfileInt("Censor", "NONFICS censor", Censor [nC].bCensor, Ini);
		}

		for(nI = 0 ; nI < MAX_CENSOR_ITEM ; nI++)
		{
			if(nC == 0)
			{
				sprintf(cTmp, "FICS %d", nI + 1);
			}
			else if(nC == 1)
			{
				sprintf(cTmp, "ICC %d", nI + 1);
			}
			else
			{
				sprintf(cTmp, "NONFICS %d", nI + 1);
			}

			strcpy(cTmp2, "");
			GetPrivateProfileString("Censor", cTmp, cTmp2, cTmp2, _MAX_PATH, Ini);

			if(strlen(cTmp2) == 0)
			{
				break;
			}

			strcpy(cTmp,  "");
			strcpy(cTmp1, "");

			sscanf(cTmp2, "%s %s", &cTmp, &cTmp1);

			if((strlen(cTmp) == 0) || (strlen(cTmp1) == 0))
			{
				break;
			}

			TOOLBOX_AllTrim(cTmp);
			cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR;

			cTmp1 [(MAX_CENSOR_BIT - 1)] = NULL_CHAR;

			nS = strlen(cTmp1);

			if(nS < (MAX_CENSOR_BIT - 1))
			{
				for(nJ = (nS - 1) ; nJ < (MAX_CENSOR_BIT - 1) ; nJ++)
				{
					cTmp1 [nJ] = '1';
				}
			}

			strcpy(Censor [nC].cHandle [nI], cTmp);
			strcpy(Censor [nC].cCensor [nI], cTmp1);

			if(nC == 0)
			{
				sprintf(cTmp, "FICS %d", nI + 1);
			}
			else if(nC == 1)
			{
				sprintf(cTmp, "ICC %d", nI + 1);
			}
			else
			{
				sprintf(cTmp, "NONFICS %d", nI + 1);
			}

			sprintf(cTmp1, "%s tell", cTmp);
			GetPrivateProfileString("Censor", cTmp1, Censor [nC].cTell [nI], Censor [nC].cTell [nI], CENSOR_COMMAND_SIZE, Ini);

			sprintf(cTmp1, "%s ptell", cTmp);
			GetPrivateProfileString("Censor", cTmp1, Censor [nC].cPtell [nI], Censor [nC].cPtell [nI], CENSOR_COMMAND_SIZE, Ini);

			sprintf(cTmp1, "%s say", cTmp);
			GetPrivateProfileString("Censor", cTmp1, Censor [nC].cSay [nI], Censor [nC].cSay [nI], CENSOR_COMMAND_SIZE, Ini);

			sprintf(cTmp1, "%s partner", cTmp);
			GetPrivateProfileString("Censor", cTmp1, Censor [nC].cPartner [nI], Censor [nC].cPartner [nI], CENSOR_COMMAND_SIZE, Ini);

			sprintf(cTmp1, "%s message", cTmp);
			GetPrivateProfileString("Censor", cTmp1, Censor [nC].cMessage [nI], Censor [nC].cMessage [nI], CENSOR_COMMAND_SIZE, Ini);
		}
	}

	//
	// [History login]
	//
	nJ = 0;
	for(nI = 0 ; nI < MAX_LOGIN ; nI++)
	{
		sprintf(cTmp, "History login string %d", nI + 1);
		strcpy(cTmp1, "");

		GetPrivateProfileString("History login", cTmp, cTmp1, cTmp1, MAX_LOGIN_SIZE, Ini);

		strcpy(cTmp1, TOOLBOX_GetConFilename(cTmp1));

		TOOLBOX_AllTrim(cTmp1);

		if(strlen(cTmp1) > 0)
		{
			if(TOOLBOX_CanOpenFile(TOOLBOX_GetFullConFilename(cTmp1)))
			{
				strcpy(cLS [nJ], cTmp1);
				++nJ;
			}
		}
	}

	Login.nLoginString = 0;
	for(nI = 0 ; nI < nJ ; nI++)
	{
		nS = 0;
		for(nC = 0 ; nC < Login.nLoginString ; nC++)
		{
			if(stricmp(Login.cLoginString [nC], cLS [nI]) == 0)
			{
				nS = 1;
				break;
			}
		}
		if(nS == 0)
		{
			strcpy(Login.cLoginString [Login.nLoginString], cLS [nI]);
			++Login.nLoginString;
		}
	}

	//
	// [Server]
	//
	strcpy(cTmp, "Server 1 address");
	strcpy(cTmp1, "");
	GetPrivateProfileString("Server", cTmp, cTmp1, cTmp1, MAX_SERVER_SIZE, Ini);
	if(strlen(cTmp1) >= 0)
	{
		for(nI = 0 ; nI < MAX_SERVER ; nI++)
		{
			sprintf(cTmp, "Server %d address", nI + 1);
			GetPrivateProfileString("Server", cTmp, Server.cServer [nI], Server.cServer [nI], MAX_SERVER_SIZE, Ini);

			sprintf(cTmp, "Server %d port", nI + 1);
			GetPrivateProfileString("Server", cTmp, Server.cPort [nI], Server.cPort [nI], MAX_SERVER_PORT_SIZE, Ini);
			Server.cPort [nI] [MAX_SERVER_PORT_SIZE - 1] = NULL_CHAR;

			sprintf(cTmp, "Server %d prompt", nI + 1);
			strcpy(cTmp1, "");
			GetPrivateProfileString("Server", cTmp, cTmp1, cTmp1, MAX_SERVER_PROMPT_SIZE + 2, Ini);
			nS = strlen(cTmp1);
			if(nS == 0)
			{
				strcpy(Server.cPrompt [nI], "");
			}
			else
			{
				if((cTmp1 [0] == '[') && (cTmp1 [nS - 1] == ']'))
				{
					cTmp1 [nS - 1] = NULL_CHAR;

					for(nJ = 1 ; nJ < nS ; nJ++)
					{
						Server.cPrompt [nI] [nJ - 1] = cTmp1 [nJ];
					}
				}
				else
				{
					strcpy(Server.cPrompt [nI], "");
				}
			}
			Server.cPrompt [nI] [MAX_SERVER_PROMPT_SIZE - 1] = NULL_CHAR;

			sprintf(cTmp, "Server %d type", nI + 1);
			Server.nType [nI] = GetPrivateProfileInt("Server", cTmp, Server.nType [nI], Ini);
		}
	}

	//
	// [Match]
	//
	Match.nMatchTime      = GetPrivateProfileInt("Match", "Time",      Match.nMatchTime,      Ini);
	Match.nMatchIncrement = GetPrivateProfileInt("Match", "Increment", Match.nMatchIncrement, Ini);
	Match.bMatchRated     = GetPrivateProfileInt("Match", "Rated",     Match.bMatchRated,     Ini);
	Match.nMatchType      = GetPrivateProfileInt("Match", "Type",      Match.nMatchType,      Ini);
	Match.nICCMatchType   = GetPrivateProfileInt("Match", "ICCType",   Match.nICCMatchType,   Ini);
	Match.nMatchColor     = GetPrivateProfileInt("Match", "Color",     Match.nMatchColor,     Ini);

	//
	// [Seek]
	//
	Seek.nSeekTime      = GetPrivateProfileInt("Seek", "Time",      Seek.nSeekTime,      Ini);
	Seek.nSeekIncrement = GetPrivateProfileInt("Seek", "Increment", Seek.nSeekIncrement, Ini);
	Seek.nSeekRating1   = GetPrivateProfileInt("Seek", "Rating1",   Seek.nSeekRating1,   Ini);
	Seek.nSeekRating2   = GetPrivateProfileInt("Seek", "Rating2",   Seek.nSeekRating2,   Ini);
	Seek.nSeekType      = GetPrivateProfileInt("Seek", "Type",      Seek.nSeekType,      Ini);
	Seek.nICCSeekType   = GetPrivateProfileInt("Seek", "ICCType",   Seek.nICCSeekType,   Ini);
	Seek.nSeekColor     = GetPrivateProfileInt("Seek", "Color",     Seek.nSeekColor,     Ini);
	Seek.bSeekManual    = GetPrivateProfileInt("Seek", "Manual",    Seek.bSeekManual,    Ini);
	Seek.bSeekFormula   = GetPrivateProfileInt("Seek", "Formula",   Seek.bSeekFormula,   Ini);
	Seek.bSeekRated     = GetPrivateProfileInt("Seek", "Rated",     Seek.bSeekRated,     Ini);

	//
	// [Channel color]
	//
	for(nI = 0 ; nI < MAX_CHANNEL_COLOR ; nI++)
	{
		sprintf(cTmp, "%d", nI);
		bChannel [nI] = TOOLBOX_ReadINIColor1(Ini, &clrChannel [nI], "Channel color", cTmp);
	}
	if(! bChannel [1])
	{
		bChannel   [1] = 1;
		clrChannel [1] = clrColor [CLR_TELNET_CHANNEL1];
	}
	if(! bChannel [24])
	{
		bChannel   [24] = 1;
		clrChannel [24] = clrColor [CLR_TELNET_CHANNEL24];
	}
	if(! bChannel [97])
	{
		bChannel   [97] = 1;
		clrChannel [97] = clrColor [CLR_TELNET_CHANNEL97];
	}

	//
	// adjustments
	//
	switch(User.nPieceSet)
	{
		case VECTOR_TOUPS :
			SYS_CheckToups();
			break;

		case VECTOR_NICOK :
			SYS_CheckNicok();
			break;

		case VECTOR_KBIRK :
			SYS_CheckKbirk();
			break;

		case VECTOR_JHURT :
			SYS_CheckJhurt();
			break;

		case VECTOR_ECHUR :
			SYS_CheckEchur();
			break;

		case VECTOR_DFONG :
			SYS_CheckDfong();
			break;

		case VECTOR_USER :
			SYS_CheckUser();
			break;

		default :
			User.nPieceSet = VECTOR_TOUPS;
			SYS_CheckToups();
			break;
	}

	SMARTMOVE_Adjust();
	TRUEPREMOVE_Adjust();
	REDCLOCK_Adjust();

	for(nI = SOUND_TELL ; nI <= SOUND_PARTNER ; nI++)
	{
		TOOLBOX_AllTrim(Sounds.cName [nI]);
	}

	for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
	{
		TOOLBOX_AllTrim(GameSounds.cName [nI]);
	}

	for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
	{
		TOOLBOX_AllTrim(TimeSounds.cName [nI]);
	}

	EAR_Append();
	TIMERCMD_Adjust();
	FKEY_Adjust();
	CONTROLKEY_Adjust();
	LAGCMD_Adjust();
	MATCH_AdjustValues();
	SEEK_Adjust();
	SERVER_Adjust();
	ASSIST_Adjust();
	CENSOR_Adjust();
	SILENCE_Adjust();
	TOOLBOX_SortLoginString();

	//
	// need to save into THIEF.INI one time
	//
	if(bNeedSave)
	{
		INI_WriteSetup(Ini);
		return 1;
	}
	else
	{
		return 0;
	}
}

void INI_WriteSetup(char *cIni)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Ini   [_MAX_PATH];

	FILE *Fv;
	int  nC, nI;
	char cTmp [_MAX_PATH], cTmp1 [_MAX_PATH], cError [1024];

	_splitpath(cIni, Drive, Dir, File, Ext);
	strcpy(Ext, INI_EXT);
	_makepath(Ini, Drive, Dir, File, Ext);

	//
	// adjustments
	//
	SMARTMOVE_Adjust();
	TRUEPREMOVE_Adjust();
	REDCLOCK_Adjust();
	TIMERCMD_Adjust();
	TOOLBOX_AllTrim(TimerCmd.cTimerCommand);
	TOOLBOX_AllTrim(TimerCmd.cICCTimerCommand);
	FKEY_Adjust();
	CONTROLKEY_Adjust();
	LAGCMD_Adjust();
	ASSIST_Adjust();
	CENSOR_Adjust();
	SILENCE_Adjust();

	if(User.nAnimateSpeedObserve < 0)
	{
		User.nAnimateSpeedObserve = 0;
	}
	else if(User.nAnimateSpeedObserve >= MAX_ANIMATE_SPEED)
	{
		User.nAnimateSpeedObserve = MAX_ANIMATE_SPEED - 1;
	}

	if(User.nAnimateSpeedExam < 0)
	{
		User.nAnimateSpeedExam = 0;
	}
	else if(User.nAnimateSpeedExam >= MAX_ANIMATE_SPEED)
	{
		User.nAnimateSpeedExam = MAX_ANIMATE_SPEED - 1;
	}

	if(User.nAnimateSpeedPlay < 0)
	{
		User.nAnimateSpeedPlay = 0;
	}
	else if(User.nAnimateSpeedPlay >= MAX_ANIMATE_SPEED)
	{
		User.nAnimateSpeedPlay = MAX_ANIMATE_SPEED - 1;
	}

	switch(User.nBufferOrientation)
	{
		case DEFAULT_BUFFER_LEFT :
		case DEFAULT_BUFFER_RIGHT :
		case DEFAULT_BUFFER_TOPBOTTOML :
		case DEFAULT_BUFFER_TOPBOTTOMR :
			break;

		default :
			User.nBufferOrientation = DEFAULT_BUFFER_RIGHT;
			break;
	}

	if(System.bCDROMConnection)
	{
		return;
	}

	// open
	Fv = fopen(Ini, "w");
	if(Fv == NULL)
	{
		sprintf(cError, "Error writing %s%s.", THIEF_NAME, INI_EXT);
		TOOLBOX_Error(cError);
		return;
	}

	//
	// [Main]
	//

	// save played game?
	fprintf(Fv, "[Main]\n");
	fprintf(Fv, "Thief version=%s\n", THIEF_VER_NR);
	fprintf(Fv, "Log games=%d\n", User.bLogGame ? 1 : 0);
	fprintf(Fv, "Save played game=%d\n", User.bSavePlayGame ? 1 : 0);

	// save observed game?
	fprintf(Fv, "Save observed game=%d\n", User.bSaveObserveGame ? 1 : 0);

	// save unobserved game?
	fprintf(Fv, "Save unobserved game=%d\n", User.bSaveUnobserveGame ? 1 : 0);

	// save aborted or disconnected game?
	fprintf(Fv, "Save aborted or disconnected game=%d\n", User.bSaveAbortGame ? 1 : 0);

	// save no BFEN bughouse game
	fprintf(Fv, "Save no BFEN bughousen game=%d\n", User.bSaveNoBFENBugGame ? 1 : 0);

	// save daily
	fprintf(Fv, "Save daily=%d\n", User.bSaveDaily ? 1 : 0);

	// pgn viewer
	fprintf(Fv, "PGN viewer=%s\n", User.cPGNViewer);

	// bpgn viewer
	fprintf(Fv, "BPGN viewer=%s\n", User.cBPGNViewer);

	// save settings on exit?
	fprintf(Fv, "Save settings on exit=%d\n", User.bSaveOnExit ? 1 : 0);

	// Minimize to Tray
	fprintf(Fv, "Minimize to Tray=%d\n", User.bMinimizetoTray ? 1 : 0);

	// Colored bughouse clocks
	fprintf(Fv, "Colored Bughouse clocks=%d\n", User.bColoredClocks  ? 1 : 0);

	// suppress exit prompt
	fprintf(Fv, "Suppress exit prompt=%d\n", User.bSuppressExitPrompt ? 1 : 0);

	// show bitmap size during game window resize
	fprintf(Fv, "Show bitmap size during game window resize=%d\n", User.bShowBitmapSize ? 1 : 0);

	// show menu path in telnet console window
	fprintf(Fv, "Show menu path in telnet console window=%d\n", User.bShowMenuPath ? 1 : 0);

	//
	// [Hidden]
	//

	// add button command into history?
	fprintf(Fv, "\n[Hidden]\n");
	fprintf(Fv, "Add button command into history=%d\n", User.bButtonCommandAddHist ? 1 : 0);

	// add play command into history?
	fprintf(Fv, "Add play command into history=%d\n", User.bPlayCommandAddHist ? 1 : 0);

	// add observe command into history?
	fprintf(Fv, "Add observe command into history=%d\n", User.bObserveCommandAddHist ? 1 : 0);

	// add telnet text command into history?
	fprintf(Fv, "Add telnet text command into history=%d\n", User.bTelnetTextCommandAddHist ? 1 : 0);

	// add telnet edit command into history?
	fprintf(Fv, "Add telnet edit command into history=%d\n", User.bTelnetEditCommandAddHist ? 1 : 0);

	// add login event command into history?
	fprintf(Fv, "Add login event command into history=%d\n", User.bLEventCommandAddHist ? 1 : 0);

	// add game event command into history?
	fprintf(Fv, "Add game event command into history=%d\n", User.bGEventCommandAddHist ? 1 : 0);

	// add function key command into history?
	fprintf(Fv, "Add function key command into history=%d\n", User.bFKeyCommandAddHist ? 1 : 0);

	// add control key command into history?
	fprintf(Fv, "Add control key command into history=%d\n", User.bCKeyCommandAddHist ? 1 : 0);

	// add seek command into history?
	fprintf(Fv, "Add seek command into history=%d\n", User.bSeekCommandAddHist ? 1 : 0);

	// add match command into history?
	fprintf(Fv, "Add match command into history=%d\n", User.bMatchCommandAddHist ? 1 : 0);

	// add tell command into history?
	fprintf(Fv, "Add tell command into history=%d\n", User.bTellCommandAddHist ? 1 : 0);

	// add message command into history?
	fprintf(Fv, "Add message command into history=%d\n", User.bMessageCommandAddHist ? 1 : 0);

	// add timer command into history?
	fprintf(Fv, "Add timer command into history=%d\n", User.bTimerCommandAddHist ? 1 : 0);

	// add lag command into history?
	fprintf(Fv, "Add lag command into history=%d\n", User.bLagCommandAddHist ? 1 : 0);

	// add question command into history?
	fprintf(Fv, "Add question command into history=%d\n", User.bQuestionCommandAddHist ? 1 : 0);

	// apply only to partner's tell (partner sound)?
	fprintf(Fv, "Apply only to partner's tell=%d\n", User.bEarPartner ? 1 : 0);

	// FICS start playing close all game
	fprintf(Fv, "FICS start playing close all games=%d\n", User.bFICSPlayCloseAll ? 1 : 0);

	// FICS play end stop partner game
	fprintf(Fv, "FICS play end stop partner game=%d\n", User.bFICSPlayEndPartner ? 1 : 0);

	// ICC start playing close all game
	fprintf(Fv, "ICC start playing close all games=%d\n", User.bICCPlayCloseAll ? 1 : 0);

	// ICC play end stop partner game
	fprintf(Fv, "ICC play end stop partner game=%d\n", User.bICCPlayEndPartner ? 1 : 0);

	// NONFICS start playing close all game
	fprintf(Fv, "NONFICS start playing close all games=%d\n", User.bNONFICSPlayCloseAll ? 1 : 0);

	// NONFICS play end stop partner game
	fprintf(Fv, "NONFICS play end stop partner game=%d\n", User.bNONFICSPlayEndPartner ? 1 : 0);

	// eat <12> game not found line
	fprintf(Fv, "Eat <12> game not found line=%d\n", User.bEat12GameNotFoundLine ? 1 : 0);

	// eat <b1> game not found line
	fprintf(Fv, "Eat <b1> game not found line=%d\n", User.bEatb1GameNotFoundLine ? 1 : 0);

	// use right mouse click no highlight set telnet handle
	fprintf(Fv, "Use right mouse click no highlight set telnet handle=%d\n", User.bRMNoHLSetTelnetHandle ? 1 : 0);

	// use xtell for last tell
	fprintf(Fv, "Use xtell for last tell=%d\n", User.bUseXTellForLastTell ? 1 : 0);

	// notepad executable file
	if(strlen(User.cNotepad) == 0)
	{
		strcpy(User.cNotepad, "notepad.exe");
	}
	fprintf(Fv, "Notepad executable file=%s\n", User.cNotepad, Ini);

	//
	// [Board]
	//

	// bitmap piece file?
	fprintf(Fv, "\n[Board]\n");
	fprintf(Fv, "Bitmap piece file=%s\n", User.cBitmapPiece);

	// bitmap square file?
	fprintf(Fv, "Bitmap square file=%s\n", User.cBitmapSquare);

	// piece set
	fprintf(Fv, "Piece set=%d\n", User.nPieceSet);

	// move type
	fprintf(Fv, "Move type=%d\n", User.nMoveType);

	// Auto center drag piece
	fprintf(Fv, "Auto center drag piece=%d\n", User.bAutoCenterDragPiece ? 1 : 0);

	// smart move?
	fprintf(Fv, "Smart move=%d\n", User.bSmartMove ? 1 : 0);

	// smart move (source square) ?
	fprintf(Fv, "Smart move (source square)=%d\n", User.bSmartMove1 ? 1 : 0);

	// smart move (destination square) ?
	fprintf(Fv, "Smart move (destination square)=%d\n", User.bSmartMove2 ? 1 : 0);

	// true premove?
	fprintf(Fv, "True Premove=%d\n", User.nTruePremove);

	// illegal true premove beep?
	fprintf(Fv, "Illegal true premove beep=%d\n", User.bIllegalTPBeep ? 1 : 0);

	// right mouse click on playing board to drop?
	fprintf(Fv, "Right mouse click on playing board to drop=%d\n", User.bRightMousePlay ? 1 : 0);

	// right mouse click on partner board to drop?
	fprintf(Fv, "Right mouse click on partner board to drop=%d\n", User.bRightMousePartner ? 1 : 0);

	// test legality on playing board?
	fprintf(Fv, "Test legality on playing board=%d\n", User.bTestLegalPlay ? 1 : 0);

	// Animate observe game move (while not playing)?
	fprintf(Fv, "Animate observe game move (while not playing)=%d\n", User.bAnimateObserve ? 1 : 0);

	// Animate exam game move?
	fprintf(Fv, "Animate exam game move=%d\n", User.bAnimateExam ? 1 : 0);

	// Animate play game move?
	fprintf(Fv, "Animate play game move=%d\n", User.bAnimatePlay ? 1 : 0);

	// Animate speed observe
	fprintf(Fv, "Animate speed observe=%d\n", User.nAnimateSpeedObserve);

	// Animate speed exam
	fprintf(Fv, "Animate speed exam=%d\n", User.nAnimateSpeedExam);

	// Animate speed play
	fprintf(Fv, "Animate speed play=%d\n", User.nAnimateSpeedPlay);

	// test legality on partner board?
	fprintf(Fv, "Test legality on partner board=%d\n", User.bTestLegalPartner ? 1 : 0);

	// use long alegrbraic notation for partner assist ?
	fprintf(Fv, "Use long algebraic notation for partner assist=%d\n", User.bLongPartner ? 1 : 0);

	// partner assist move prefix
	fprintf(Fv, "Partner assist move prefix=%s\n", User.cPartnerMove1);

	// partner assist sit for piece move prefix
	fprintf(Fv, "Partner assist sit for piece move prefix=%s\n", User.cPartnerMove2);

	// partner assist opponent move prefix
	fprintf(Fv, "Partner assist opponent move prefix=%s\n", User.cPartnerMove3);

	// partner assist opponent sit for piece move prefix
	fprintf(Fv, "Partner assist opponent sit for piece move prefix=%s\n", User.cPartnerMove4);

	// show move buttons
	fprintf(Fv, "Show move buttons=%d\n", User.bShowMoveButton ? 1 : 0);

	// use arrow keys for move buttons
	fprintf(Fv, "User arrow keys for move buttons=%d\n", User.bUseArrowKeyMoveButton ? 1 : 0);

	// show actual board size?
	fprintf(Fv, "Show actual board size=%d\n", User.bActualSize ? 1 : 0);

	// stop resize playing board while playing
	fprintf(Fv, "Stop resize playing board while playing=%d\n", User.bStopResizePlay ? 1 : 0);

	// escape key cancels drag move
	fprintf(Fv, "Escape key cancels drag move=%d\n", User.bEscCancelDragMove ? 1 : 0);

	// escape key cancels true premove
	fprintf(Fv, "Escape key cancels true premove=%d\n", User.bEscCancelTruePremove ? 1 : 0);

	// escape key cancels sit drop move
	fprintf(Fv, "Escape key cancels sit drop move=%d\n", User.bEscCancelSitDropMove ? 1 : 0);

	// escape key cancels auto knight promotion
	fprintf(Fv, "Escape key cancels auto knight promotion=%d\n", User.bEscCancelPromoteKnight ? 1 : 0);

	// no play abuser
	fprintf(Fv, "No play abuser=%d\n", User.bNoPlayAbuser ? 1 : 0);

	// no play computer
	fprintf(Fv, "No play computer=%d\n", User.bNoPlayComputer ? 1 : 0);

	// play wild 5 auto flip
	fprintf(Fv, "Play wild 5 auto flip=%d\n", User.nPlayWild5Flip);

	// show result?
	fprintf(Fv, "Show result=%d\n", User.bShowResult ? 1 : 0);

	// show rating?
	fprintf(Fv, "Show rating=%d\n", User.bShowRating ? 1 : 0);

	// show lag stat?
	fprintf(Fv, "Show lag stat=%d\n", User.bShowLagStat ? 1 : 0);

	// show partner tell?
	fprintf(Fv, "Show partner tell=%d\n", User.bShowPtell ? 1 : 0);

	// show coordinates?
	fprintf(Fv, "Show coordinates=%d\n", User.bShowCoord ? 1 : 0);

	// show game type?
	fprintf(Fv, "Show game type=%d\n", User.bShowGameType ? 1 : 0);

	// show game type on window title bar
	fprintf(Fv, "Show game type on window title bar=%d\n", User.bShowGameTypeOnTitleBar ? 1 : 0);

	// show piece value on window title bar
	fprintf(Fv, "Show piece value on window title bar=%d\n", User.bShowValueOnTitleBar ? 1 : 0);

	// show last move?
	fprintf(Fv, "Show last move=%d\n", User.bShowLastMove ? 1 : 0);

	// show last move time in tenth of a second (FICS)?
	fprintf(Fv, "Show last move time in tenth of a second (FICS)=%d\n", User.bShowLastMoveTenth ? 1 : 0);

	// show last move square highlight?
	fprintf(Fv, "Show last move square highlight=%d\n", User.bShowHighlight ? 1 : 0);

	// show true premove square highlight?
	fprintf(Fv, "Show true premove square highlight=%d\n", User.bShowTPHighlight ? 1 : 0);

	// show clock on top?
	fprintf(Fv, "Show clock on top=%d\n", User.bShowClockOnTop ? 1 : 0);

	// show king highlight (play my piece) ?
	fprintf(Fv, "Show king highlight (play my piece)=%d\n", User.nShowKingHighlightPM);

	// show king highlight (play opponent piece) ?
	fprintf(Fv, "Show king highlight (play opponent piece)=%d\n", User.nShowKingHighlightPO);

	// show king highlight (exam) ?
	fprintf(Fv, "Show king highlight (exam)=%d\n", User.nShowKingHighlightE);

	// show king highlight (observe) ?
	fprintf(Fv, "Show king highlight (observe)=%d\n", User.nShowKingHighlightO);

	// show captured piece for chess game?
	fprintf(Fv, "Show captured piece for chess game=%d\n", User.nShowCapturedChessPiece);

	// piece buffer orientation
	fprintf(Fv, "Piece buffer orientation=%d\n", User.nBufferOrientation);

	// auto queen?
	fprintf(Fv, "Auto queen=%d\n", User.bAutoQueen ? 1 : 0);

	// auto flag?
	fprintf(Fv, "Auto flag=%d\n", User.bAutoFlag ? 1 : 0);

	// auto rematch?
	fprintf(Fv, "Auto rematch=%d\n", User.bAutoRematch ? 1 : 0);

	// auto observe?
	fprintf(Fv, "Auto observe=%d\n", User.bAutoObserve ? 1 : 0);

	// auto observe game notification?
	fprintf(Fv, "Auto observe game notification=%d\n", User.bAutoObserveGNotify ? 1 : 0);

	// apply final countdown?
	fprintf(Fv, "Apply final countdown=%d\n", User.bFinalCountDown ? 1 : 0);

	// game window color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_WINDOW_COLOR], "Game window color");

	// board outline color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_BOARD_OUTLINE], "Board outline color");

	// buffer outline color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_BUFFER_OUTLINE], "Buffer outline color");

	// buffer fill color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_BUFFER_FILL], "Buffer fill color");

	// white square outline color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_WHITE_SQUARE_OUTLINE], "White square outline color");

	// white square fill color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_WHITE_SQUARE_FILL], "White square fill color");

	// black square outline color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_BLACK_SQUARE_OUTLINE], "Black square outline color");

	// black square fill color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_BLACK_SQUARE_FILL], "Black square fill color");

	// piece number color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_PIECE_NUMBER], "Piece number color");

	// white piece outline color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_WHITE_PIECE_OUTLINE], "White piece outline color");

	// white piece fill color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_WHITE_PIECE_FILL], "White piece fill color");

	// black piece outline color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_BLACK_PIECE_OUTLINE], "Black piece outline color");

	// black piece fill color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_BLACK_PIECE_FILL], "Black piece fill color");

	// handle color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_HANDLE], "Handle color");

	// clock on color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_CLOCK_ON], "Clock on color");

	// clock on background color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_CLOCK_ON_BACKGROUND], "Clock on background color");

	// clock off color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_CLOCK_OFF], "Clock off color");

	// Time Down (Bughouse) color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_CLOCK_TIMEDOWN], "Team Down color");

	// Time Up (Bughouse) color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_CLOCK_TIMEUP], "Team Up color");

	// lag stat color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_LAGSTAT], "Lag stat color");

	// red clock
	fprintf(Fv, "Red clock second=%d\n", RedClock.nRedClock);

	// show tenth of second?
	fprintf(Fv, "Show tenth of second=%d\n", RedClock.nShowTenthSecond);

	// show less than one second?
	fprintf(Fv, "Show less than one second=%d\n", RedClock.bShowOneSecond);

	// game type color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_GAMETYPE], "Game type color");

	// last move color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_LASTMOVE], "Last move color");

	// last move square highlight color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_HIGHLIGHT], "Last move square highlight color");

	// true premove square highlight color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TRUE_PREMOVE_HIGHLIGHT], "True premove square highlight");

	// illegal true premove square highlight color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_ILLEGAL_TRUE_PREMOVE], "Illegal true premove square highlight");

	// king legal square highlight color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_KING_HIGHLIGHT], "King legal square highlight");

	// result text color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_RESULT_FOREGROUND], "Result text color");

	// result background color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_RESULT_BACKGROUND], "Result background color");

	// ptell text color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_PTELL_FOREGROUND], "Ptell text color");

	// ptell background color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_PTELL_BACKGROUND], "Ptell background color");

	// game info text color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_GINFO_FOREGROUND], "Game info text color");

	// game info background color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_GINFO_BACKGROUND], "Game info background color");

	// coordinates text color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_COORDINATES_FOREGROUND], "Coordinates text color");

	// handle font
	TOOLBOX_WriteINIFont(Fv, FONT_HANDLE, "Handle font");

	// clock font
	TOOLBOX_WriteINIFont(Fv, FONT_CLOCK, "Clock font");

	// game type font
	TOOLBOX_WriteINIFont(Fv, FONT_GAMETYPE, "Game type font");

	// last move font
	TOOLBOX_WriteINIFont(Fv, FONT_LASTMOVE, "Last move font");

	// lag stat font
	TOOLBOX_WriteINIFont(Fv, FONT_LAGSTAT, "Lag stat font");

	// result font
	TOOLBOX_WriteINIFont(Fv, FONT_RESULT, "Result font");

	// coordinates font
	TOOLBOX_WriteINIFont(Fv, FONT_COORDINATES, "Coordinates font");

	//
	// [Silence]
	//

	// Apply Silence
	fprintf(Fv, "\n[Silence]\n");
	fprintf(Fv, "Apply Silence=%d\n", Silence.bSilenceApply ? 1 : 0);
	fprintf(Fv, "Silence Tell Message=%s\n", Silence.cTell);

	// Silence Always Shout
	fprintf(Fv, "Silence Always Shout=%d\n", Silence.bSilenceShout ? 1 : 0);

	// Silence Always c-Shout
	fprintf(Fv, "Silence Always CShout=%d\n", Silence.bSilenceCShout ? 1 : 0);

	// Silence Always Announcement
	fprintf(Fv, "Silence Always Announcement=%d\n", Silence.bSilenceAnnouncement ? 1 : 0);

	// Silence Always Channel Tell
	fprintf(Fv, "Silence Always Channel Tell=%d\n", Silence.bSilenceChTell ? 1 : 0);

	// Silence Always Personal Tell
	fprintf(Fv, "Silence Always Personal Tell=%d\n", Silence.bSilencePeTell ? 1 : 0);

	// Silence Always Say
	fprintf(Fv, "Silence Always Say=%d\n", Silence.bSilenceSay ? 1 : 0);

	// Silence Always Kibitz
	fprintf(Fv, "Silence Always Kibitz=%d\n", Silence.bSilenceKibitz ? 1 : 0);

	// Silence Always Whisper
	fprintf(Fv, "Silence Always Whisper=%d\n", Silence.bSilenceWhisper ? 1 : 0);

	// Silence Playing Shout
	fprintf(Fv, "Silence Playing Shout=%d\n", Silence.bSilencePlayShout ? 1 : 0);

	// Silence Playing c-Shout
	fprintf(Fv, "Silence Playing CShout=%d\n", Silence.bSilencePlayCShout ? 1 : 0);

	// Silence Playing Announcement
	fprintf(Fv, "Silence Playing Announcement=%d\n", Silence.bSilencePlayAnnouncement ? 1 : 0);

	// Silence Playing Channel Tell
	fprintf(Fv, "Silence Playing Channel Tell=%d\n", Silence.bSilencePlayChTell ? 1 : 0);

	// Silence Playing Personal Tell
	fprintf(Fv, "Silence Playing Personal Tell=%d\n", Silence.bSilencePlayPeTell ? 1 : 0);

	// Silence Playing Say
	fprintf(Fv, "Silence Playing Say=%d\n", Silence.bSilencePlaySay ? 1 : 0);

	// Silence Playing Kibitz
	fprintf(Fv, "Silence Playing Kibitz=%d\n", Silence.bSilencePlayKibitz ? 1 : 0);

	// Silence Playing Whisper
	fprintf(Fv, "Silence Playing Whisper=%d\n", Silence.bSilencePlayWhisper ? 1 : 0);

	//
	// [Telnet]
	//

	// log telnet?
	fprintf(Fv, "\n[Telnet]\n");
	fprintf(Fv, "Log telnet=%d\n", User.bLogTelnet ? 1 : 0);

	// apply mouse wheel?
	fprintf(Fv, "Apply mouse wheel=%d\n", User.bMouseWheel ? 1 : 0);

	// show underline?
	fprintf(Fv, "Show underline=%d\n", User.bShowUnderline ? 1 : 0);

	// enter key highlights input text?
	fprintf(Fv, "Enter key highlights input text=%d\n", User.bEnterHighlight ? 1 : 0);

	// go bottom after telnet resize
	fprintf(Fv, "Go bottom after telnet resize=%d\n", User.bTelnetResizeGoBottom ? 1 : 0);

	// suppress announcement
	fprintf(Fv, "Suppress announcement=%d\n", User.bSuppressAnnouncement ? 1 : 0);

	// suppress computer kibitz
	fprintf(Fv, "Suppress computer kibitz=%d\n", User.bSuppressCompKibitz ? 1 : 0);

	// suppress computer whisper
	fprintf(Fv, "Suppress computer whisper=%d\n", User.bSuppressCompWhisper ? 1 : 0);

	// suppress ignore formula
	fprintf(Fv, "Suppress ignore formula=%d\n", User.bSuppressIgnoreFormula ? 1 : 0);

	// suppress ROBO welcome message
	fprintf(Fv, "Suppress ROBO welcome message=%d\n", User.bSuppressROBOWelcome ? 1 : 0);

	// suppress computer channel tell
	fprintf(Fv, "Suppress computer channel tell=%d\n", User.bSuppressCompChTell ? 1 : 0);

	// suppress computer tell
	fprintf(Fv, "Suppress computer tell=%d\n", User.bSuppressCompTell ? 1 : 0);

	// suppress computer shout
	fprintf(Fv, "Suppress computer shout=%d\n", User.bSuppressCompShout ? 1 : 0);

	// Console Time Stamp
	fprintf(Fv, "Console Time Stamp=%d\n", User.bTimeStampTelnet ? 1 : 0);

	// Log Time Stamp
	fprintf(Fv, "Log Time Stamp=%d\n", User.bTimeStampLog ? 1 : 0);

	// telnet font
	TOOLBOX_WriteINIFont(Fv, FONT_TELNET, "Telnet font");

	// telnet normal color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_NORMAL], "Normal color");

	// telnet tell color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_TELL], "Tell color");

	// telnet ptell color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_PTELL], "Ptell color");

	// telnet say color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_SAY], "Say color");

	// telnet kibitz color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_KIBITZ], "Kibitz color");

	// telnet whisper color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_WHISPER], "Whisper color");

	// telnet shout color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_SHOUT], "Shout color");

	// telnet cshout color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_CSHOUT], "Cshout color");

	// telnet challenge color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_CHALLENGE], "Challenge color");

	// telnet user text color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_USERTEXT], "User text color");

	// telnet background color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_BACKGROUND], "Background color");

	// telnet notification color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_NOTIFICATION], "Notification color");

	// telnet :notification: color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_COLON_NOTIFY], "Colon notification color");

	// telnet arrival color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_ARRIVAL], "Arrival color");

	// telnet departure color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_DEPARTURE], "Departure color");

	// telnet game notification color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_GAME_NOTIFY], "Game notification color");

	// telnet announcement color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_ANNOUNCEMENT], "Announcement color");

	// telnet roombot (:) color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_COLON], "Roombot (:) color");

	// telnet :knighttour(256): xxx color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_COLON_256], "Colon 256 color");

	// telnet :knighttour(*)(C2): xxx color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_COLON_C2], "Colon c2 color");

	// telnet :knighttour(TM)(T3): xxx color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_COLON_T3], "Colon t3 color");

	// telnet :knighttour(TM) t-shouts: xxx color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_COLON_TSHOUTS], "Colon t-shouts color");

	// telnet :mamer color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_COLON_MAMER], "Colon mamer color");

	// telnet channel color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_CHANNEL], "Channel color");

	// alternate telnet channel color
	TOOLBOX_WriteINIColor(Fv, clrColor [CLR_TELNET_CHANNELA], "Alt Channel color");

	//
	// [Button]
	//

	// apply extended button?
	fprintf(Fv, "\n[Button]\n");
	fprintf(Fv, "Apply extended button=%d\n", User.bExtendedButton ? 1 : 0);

	// button font
	TOOLBOX_WriteINIFont(Fv, FONT_BUTTON, "Button font");

	// button label and action
	for(nI = 0 ; nI < MAX_BUTTON ; nI++)
	{
		TOOLBOX_AllTrim(Buttons.cText   [nI]);
		TOOLBOX_AllTrim(Buttons.cAction [nI]);

		if((Buttons.nType [nI] >= 0) && (Buttons.nType [nI] < MAX_FMENU))
		{
			if(! TOOLBOX_NeedCommandValue(Buttons.nType [nI]))
			{
				strcpy(Buttons.cAction [nI], "");
			}
		}
		else
		{
			Buttons.nType [nI] = FUNCTION_COMMAND;
		}

		sprintf(cTmp, "Button %d", nI + 1);
		fprintf(Fv, "%s=%s\n", cTmp, Buttons.cText [nI]);

		sprintf(cTmp, "Type %d", nI + 1);
		fprintf(Fv, "%s=%d\n", cTmp, Buttons.nType [nI]);

		sprintf(cTmp, "Action %d", nI + 1);
		fprintf(Fv, "%s=%s\n", cTmp, Buttons.cAction [nI]);
	}

	//
	// [Sound]
	//
	fprintf(Fv, "\n[Sound]\n");
	fprintf(Fv, "Sound=%d\n", User.bSound ? 1 : 0);

	//
	// [Telnet sound]
	//
	fprintf(Fv, "\n[Telnet sound]\n");
	fprintf(Fv, "Telnet sound=%d\n", User.bTelnetSound ? 1 : 0);

	for(nI = SOUND_TELL ; nI <= SOUND_PARTNER ; nI++)
	{
		TOOLBOX_AllTrim(Sounds.cName [nI]);
	}

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_TELL] ? 1 : 0);
	fprintf(Fv, "Tell beep=%s\n", cTmp);

	fprintf(Fv, "Tell=%s\n", Sounds.cName [SOUND_TELL]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_PTELL] ? 1 : 0);
	fprintf(Fv, "Ptell beep=%s\n", cTmp);

	fprintf(Fv, "Ptell=%s\n", Sounds.cName [SOUND_PTELL]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_SAY] ? 1 : 0);
	fprintf(Fv, "Say beep=%s\n", cTmp);

	fprintf(Fv, "Say=%s\n", Sounds.cName [SOUND_SAY]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_KIBITZ] ? 1 : 0);
	fprintf(Fv, "Kibitz beep=%s\n", cTmp);

	fprintf(Fv, "Kibitz=%s\n", Sounds.cName [SOUND_KIBITZ]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_WHISPER] ? 1 : 0);
	fprintf(Fv, "Whisper beep=%s\n", cTmp);

	fprintf(Fv, "Whisper=%s\n", Sounds.cName [SOUND_WHISPER]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_SHOUT] ? 1 : 0);
	fprintf(Fv, "Shout beep=%s\n", cTmp);

	fprintf(Fv, "Shout=%s\n", Sounds.cName [SOUND_SHOUT]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_CSHOUT] ? 1 : 0);
	fprintf(Fv, "Cshout beep=%s\n", cTmp);

	fprintf(Fv, "Cshout=%s\n", Sounds.cName [SOUND_CSHOUT]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_CHALLENGE] ? 1 : 0);
	fprintf(Fv, "Challenge beep=%s\n", cTmp);

	fprintf(Fv, "Challenge=%s\n", Sounds.cName [SOUND_CHALLENGE]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_ABORT_REQUEST] ? 1 : 0);
	fprintf(Fv, "Abort beep=%s\n", cTmp);

	fprintf(Fv, "Abort=%s\n", Sounds.cName [SOUND_ABORT_REQUEST]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_PARTNER] ? 1 : 0);
	fprintf(Fv, "Partner beep=%s\n", cTmp);

	fprintf(Fv, "Partner=%s\n", Sounds.cName [SOUND_PARTNER]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_NOTIFICATION] ? 1 : 0);
	fprintf(Fv, "Notification beep=%s\n", cTmp);

	fprintf(Fv, "Notification=%s\n", Sounds.cName [SOUND_NOTIFICATION]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_ARRIVAL] ? 1 : 0);
	fprintf(Fv, "Arrival beep=%s\n", cTmp);

	fprintf(Fv, "Arrival=%s\n", Sounds.cName [SOUND_ARRIVAL]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_DEPARTURE] ? 1 : 0);
	fprintf(Fv, "Departure beep=%s\n", cTmp);

	fprintf(Fv, "Departure=%s\n", Sounds.cName [SOUND_DEPARTURE]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_GAME_NOTIFY] ? 1 : 0);
	fprintf(Fv, "Game notification beep=%s\n", cTmp);

	fprintf(Fv, "Game notification=%s\n", Sounds.cName [SOUND_GAME_NOTIFY]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_TAKEBACK_REQUEST] ? 1 : 0);
	fprintf(Fv, "Takeback request beep=%s\n", cTmp);

	fprintf(Fv, "Takeback request=%s\n", Sounds.cName [SOUND_TAKEBACK_REQUEST]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_DRAW_REQUEST] ? 1 : 0);
	fprintf(Fv, "Draw offer beep=%s\n", cTmp);

	fprintf(Fv, "Draw Offer=%s\n", Sounds.cName [SOUND_DRAW_REQUEST]);

	sprintf(cTmp, "%d", Sounds.bBeep [SOUND_ADJOURN_REQUEST] ? 1 : 0);
	fprintf(Fv, "Adjourn request beep=%s\n", cTmp);

	fprintf(Fv, "Adjourn request=%s\n", Sounds.cName [SOUND_ADJOURN_REQUEST]);

	//
	// [Event sound]
	//
	fprintf(Fv, "\n[Event sound]\n");
	sprintf(cTmp, "%d", User.bGameSound ? 1 : 0);
	fprintf(Fv, "Event sound=%s\n", cTmp);

	for(nI = 0 ; nI < MAX_GAME_SOUND ; nI++)
	{
		TOOLBOX_AllTrim(GameSounds.cName [nI]);
	}

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_START] ? 1 : 0);
	fprintf(Fv, "Playing board game start beep=%s\n", cTmp);
	fprintf(Fv, "Playing board game start=%s\n", GameSounds.cName [GAME_SOUND_PLAY_START]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_MOVE] ? 1 : 0);
	fprintf(Fv, "Playing board move beep=%s\n", cTmp);
	fprintf(Fv, "Playing board move=%s\n", GameSounds.cName [GAME_SOUND_PLAY_MOVE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_MOVE_SMART] ? 1 : 0);
	fprintf(Fv, "Playing board smart move beep=%s\n", cTmp);
	fprintf(Fv, "Playing board smart move=%s\n", GameSounds.cName [GAME_SOUND_PLAY_MOVE_SMART]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_MOVE_TP] ? 1 : 0);
	fprintf(Fv, "Playing board true premove beep=%s\n", cTmp);
	fprintf(Fv, "Playing board true premove=%s\n", GameSounds.cName [GAME_SOUND_PLAY_MOVE_TP]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_CAPTURE] ? 1 : 0);
	fprintf(Fv, "Playing board capture beep=%s\n", cTmp);
	fprintf(Fv, "Playing board capture=%s\n", GameSounds.cName [GAME_SOUND_PLAY_CAPTURE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_CASTLE] ? 1 : 0);
	fprintf(Fv, "Playing board castle beep=%s\n", cTmp);
	fprintf(Fv, "Playing board castle=%s\n", GameSounds.cName [GAME_SOUND_PLAY_CASTLE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_CHECK] ? 1 : 0);
	fprintf(Fv, "Playing board my king in check beep=%s\n", cTmp);
	fprintf(Fv, "Playing board my king in check=%s\n", GameSounds.cName [GAME_SOUND_PLAY_CHECK]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PLAY_OVER] ? 1 : 0);
	fprintf(Fv, "Playing board game over beep=%s\n", cTmp);
	fprintf(Fv, "Playing board game over=%s\n", GameSounds.cName [GAME_SOUND_PLAY_OVER]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_MOVE] ? 1 : 0);
	fprintf(Fv, "Partner's board move beep=%s\n", cTmp);
	fprintf(Fv, "Partner's board move=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_MOVE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_CAPTURE] ? 1 : 0);
	fprintf(Fv, "Partner's board capture beep=%s\n", cTmp);
	fprintf(Fv, "Partner's board capture=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_CAPTURE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_CASTLE] ? 1 : 0);
	fprintf(Fv, "Partner's board castle beep=%s\n", cTmp);
	fprintf(Fv, "Partner's board castle=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_CASTLE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_CHECK] ? 1 : 0);
	fprintf(Fv, "Partner's king is in check beep=%s\n", cTmp);
	fprintf(Fv, "Partner's king is in check=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_CHECK]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_MOVE1] ? 1 : 0);
	fprintf(Fv, "Observe board move beep (playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board move (playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_MOVE1]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE1] ? 1 : 0);
	fprintf(Fv, "Observe board capture beep (playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board capture (playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_CAPTURE1]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_CASTLE1] ? 1 : 0);
	fprintf(Fv, "Observe board castle beep (playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board castle (playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_CASTLE1]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_CHECK1] ? 1 : 0);
	fprintf(Fv, "Observe board check beep (playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board check (playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_CHECK1]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_OVER1] ? 1 : 0);
	fprintf(Fv, "Observe board game over beep (playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board game over (playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_OVER1]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_MOVE2] ? 1 : 0);
	fprintf(Fv, "Observe board move beep (not playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board move (not playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_MOVE2]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_CAPTURE2] ? 1 : 0);
	fprintf(Fv, "Observe board capture beep (not playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board capture (not playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_CAPTURE2]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_CASTLE2] ? 1 : 0);
	fprintf(Fv, "Observe board castle beep (not playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board castle (not playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_CASTLE2]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_CHECK2] ? 1 : 0);
	fprintf(Fv, "Observe board check beep (not playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board check (not playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_CHECK2]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_OBS_OVER2] ? 1 : 0);
	fprintf(Fv, "Observe board game over beep (not playing)=%s\n", cTmp);
	fprintf(Fv, "Observe board game over (not playing)=%s\n", GameSounds.cName [GAME_SOUND_OBS_OVER2]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_I_WON] ? 1 : 0);
	fprintf(Fv, "I won beep=%s\n", cTmp);
	fprintf(Fv, "I won=%s\n", GameSounds.cName [GAME_SOUND_I_WON]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_I_LOST] ? 1 : 0);
	fprintf(Fv, "I lost beep=%s\n", cTmp);
	fprintf(Fv, "I lost=%s\n", GameSounds.cName [GAME_SOUND_I_LOST]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_I_DREW] ? 1 : 0);
	fprintf(Fv, "I drew beep=%s\n", cTmp);
	fprintf(Fv, "I drew=%s\n", GameSounds.cName [GAME_SOUND_I_DREW]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_I_ABORT] ? 1 : 0);
	fprintf(Fv, "I aborted beep=%s\n", cTmp);
	fprintf(Fv, "I aborted=%s\n", GameSounds.cName [GAME_SOUND_I_ABORT]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_WON] ? 1 : 0);
	fprintf(Fv, "Partner won beep=%s\n", cTmp);
	fprintf(Fv, "Partner won=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_WON]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_LOST] ? 1 : 0);
	fprintf(Fv, "Partner lost beep=%s\n", cTmp);
	fprintf(Fv, "Partner lost=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_LOST]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_DREW] ? 1 : 0);
	fprintf(Fv, "Partner drew beep=%s\n", cTmp);
	fprintf(Fv, "Partner drew=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_DREW]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_PARTNER_ABORT] ? 1 : 0);
	fprintf(Fv, "Partner aborted beep=%s\n", cTmp);
	fprintf(Fv, "Partner aborted=%s\n", GameSounds.cName [GAME_SOUND_PARTNER_ABORT]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_EXAM_MOVE] ? 1 : 0);
	fprintf(Fv, "Exam move beep=%s\n", cTmp);
	fprintf(Fv, "Exam move=%s\n", GameSounds.cName [GAME_SOUND_EXAM_MOVE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_EXAM_CAPTURE] ? 1 : 0);
	fprintf(Fv, "Exam capture beep=%s\n", cTmp);
	fprintf(Fv, "Exam capture=%s\n", GameSounds.cName [GAME_SOUND_EXAM_CAPTURE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_EXAM_CASTLE] ? 1 : 0);
	fprintf(Fv, "Exam castle beep=%s\n", cTmp);
	fprintf(Fv, "Exam castle=%s\n", GameSounds.cName [GAME_SOUND_EXAM_CASTLE]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_EXAM_CHECK] ? 1 : 0);
	fprintf(Fv, "Exam check beep=%s\n", cTmp);
	fprintf(Fv, "Exam check=%s\n", GameSounds.cName [GAME_SOUND_EXAM_CHECK]);

	sprintf(cTmp, "%d", GameSounds.bBeep [GAME_SOUND_GAME_ABORT] ? 1 : 0);
	fprintf(Fv, "Game Abort beep=%s\n", cTmp);
	fprintf(Fv, "Game Abort=%s\n", GameSounds.cName [GAME_SOUND_GAME_ABORT]);

	//
	// [Partner sound]
	//
	fprintf(Fv, "\n[Partner sound]\n");
	sprintf(cTmp, "%d", User.bEarSound ? 1 : 0);
	fprintf(Fv, "Partner sound=%s\n", cTmp);

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		TOOLBOX_AllTrim(Ears.cTell [nI]);
		TOOLBOX_AllTrim(Ears.cName [nI]);

		sprintf(cTmp, "Tell %d", nI + 1);
		fprintf(Fv, "%s=%s\n", cTmp, Ears.cTell [nI]);

		sprintf(cTmp, "Beep %d", nI + 1);
		sprintf(cTmp1, "%d", Ears.bBeep [nI] ? 1 : 0);
		fprintf(Fv, "%s=%s\n", cTmp, cTmp1);

		sprintf(cTmp, "Wave %d", nI + 1);
		fprintf(Fv, "%s=%s\n", cTmp, Ears.cName [nI]);
	}

	//
	// [Time sound]
	//
	fprintf(Fv, "\n[Time sound]\n");
	for(nI = 0 ; nI < MAX_TIME_SOUND ; nI++)
	{
		TOOLBOX_AllTrim(TimeSounds.cName [nI]);
		sprintf(cTmp, "Wave %d", nI);
		fprintf(Fv, "%s=%s\n", cTmp, TimeSounds.cName [nI]);
	}

	//
	// [Login event]
	//
	fprintf(Fv, "\n[Login event]\n");
	for(nI = 0 ; nI < MAX_LOGIN_EVENT ; nI++)
	{
		fprintf(Fv, "Login event type %d=%d\n",    nI + 1, LEvent.nType    [nI]);
		fprintf(Fv, "Login event command %d=%s\n", nI + 1, LEvent.cCommand [nI]);
	}

	//
	// [Game event]
	//
	fprintf(Fv, "\n[Game event]\n");
	fprintf(Fv, "Playing board game start type 1=%d\n",           GEvent.nType    [GAME_EVENT_PLAY_START1]);
	fprintf(Fv, "Playing board game start command 1=%s\n",        GEvent.cCommand [GAME_EVENT_PLAY_START1]);
	fprintf(Fv, "Playing board game start type 2=%d\n",           GEvent.nType    [GAME_EVENT_PLAY_START2]);
	fprintf(Fv, "Playing board game start command 2=%s\n",        GEvent.cCommand [GAME_EVENT_PLAY_START2]);
	fprintf(Fv, "Playing board bughouse game start type=%d\n",    GEvent.nType    [GAME_EVENT_PLAY_BUG_START]);
	fprintf(Fv, "Playing board bughouse game start command=%s\n", GEvent.cCommand [GAME_EVENT_PLAY_BUG_START]);
	fprintf(Fv, "Playing board one game start type=%d\n",         GEvent.nType    [GAME_EVENT_PLAY_ONE_START]);
	fprintf(Fv, "Playing board one game start command=%s\n",      GEvent.cCommand [GAME_EVENT_PLAY_ONE_START]);
	fprintf(Fv, "Playing board game over type 1=%d\n",            GEvent.nType    [GAME_EVENT_PLAY_OVER1]);
	fprintf(Fv, "Playing board game over command 1=%s\n",         GEvent.cCommand [GAME_EVENT_PLAY_OVER1]);
	fprintf(Fv, "Playing board game over type 2=%d\n",            GEvent.nType    [GAME_EVENT_PLAY_OVER2]);
	fprintf(Fv, "Playing board game over command 2=%s\n",         GEvent.cCommand [GAME_EVENT_PLAY_OVER2]);
	fprintf(Fv, "Observe board game start type=%d\n",             GEvent.nType    [GAME_EVENT_OBSERVE_START]);
	fprintf(Fv, "Observe board game start command=%s\n",          GEvent.cCommand [GAME_EVENT_OBSERVE_START]);
	fprintf(Fv, "Observe board bughouse game start type=%d\n",    GEvent.nType    [GAME_EVENT_OBSERVE_BUG_ST]);
	fprintf(Fv, "Observe board bughouse game start command=%s\n", GEvent.cCommand [GAME_EVENT_OBSERVE_BUG_ST]);
	fprintf(Fv, "Observe board one game start type=%d\n",         GEvent.nType    [GAME_EVENT_OBSERVE_ONE_ST]);
	fprintf(Fv, "Observe board one game start command=%s\n",      GEvent.cCommand [GAME_EVENT_OBSERVE_ONE_ST]);
	fprintf(Fv, "Observe board game over type=%d\n",              GEvent.nType    [GAME_EVENT_OBSERVE_OVER]);
	fprintf(Fv, "Observe board game over command=%s\n",           GEvent.cCommand [GAME_EVENT_OBSERVE_OVER]);

	//
	// [Button right mouse click event]
	//
	fprintf(Fv, "\n[Button right mouse click event]\n");
	for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
	{
		fprintf(Fv, "Event %d menu=%s\n",    nI + 1, RightMouse [RIGHT_MOUSE_BUTTON].cMenu    [nI]);
		fprintf(Fv, "Event %d type=%d\n",    nI + 1, RightMouse [RIGHT_MOUSE_BUTTON].nType    [nI]);
		fprintf(Fv, "Event %d command=%s\n", nI + 1, RightMouse [RIGHT_MOUSE_BUTTON].cCommand [nI]);
	}

	//
	// [Play right mouse click event]
	//
	fprintf(Fv, "\n[Play right mouse click event]\n");
	for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
	{
		fprintf(Fv, "Event %d menu=%s\n",    nI + 1, RightMouse [RIGHT_MOUSE_PLAY].cMenu    [nI]);
		fprintf(Fv, "Event %d type=%d\n",    nI + 1, RightMouse [RIGHT_MOUSE_PLAY].nType    [nI]);
		fprintf(Fv, "Event %d command=%s\n", nI + 1, RightMouse [RIGHT_MOUSE_PLAY].cCommand [nI]);
	}

	//
	// [Observe right mouse click event]
	//
	fprintf(Fv, "\n[Observe right mouse click event]\n");
	for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
	{
		fprintf(Fv, "Event %d menu=%s\n",    nI + 1, RightMouse [RIGHT_MOUSE_OBSERVE].cMenu    [nI]);
		fprintf(Fv, "Event %d type=%d\n",    nI + 1, RightMouse [RIGHT_MOUSE_OBSERVE].nType    [nI]);
		fprintf(Fv, "Event %d command=%s\n", nI + 1, RightMouse [RIGHT_MOUSE_OBSERVE].cCommand [nI]);
	}

	//
	// [Telnet text right mouse click event]
	//
	fprintf(Fv, "\n[Telnet text right mouse click event]\n");
	for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
	{
		fprintf(Fv, "Event %d menu=%s\n",    nI + 1, RightMouse [RIGHT_MOUSE_TELNET_TEXT].cMenu    [nI]);
		fprintf(Fv, "Event %d type=%d\n",    nI + 1, RightMouse [RIGHT_MOUSE_TELNET_TEXT].nType    [nI]);
		fprintf(Fv, "Event %d command=%s\n", nI + 1, RightMouse [RIGHT_MOUSE_TELNET_TEXT].cCommand [nI]);
	}

	//
	// [Telnet edit right mouse click event]
	//
	fprintf(Fv, "\n[Telnet edit right mouse click event]\n");
	for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
	{
		fprintf(Fv, "Event %d menu=%s\n",    nI + 1, RightMouse [RIGHT_MOUSE_TELNET_EDIT].cMenu    [nI]);
		fprintf(Fv, "Event %d type=%d\n",    nI + 1, RightMouse [RIGHT_MOUSE_TELNET_EDIT].nType    [nI]);
		fprintf(Fv, "Event %d command=%s\n", nI + 1, RightMouse [RIGHT_MOUSE_TELNET_EDIT].cCommand [nI]);
	}

	//
	// [Board handle right mouse click event]
	//
	fprintf(Fv, "\n[Board handle right mouse click event]\n");
	for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
	{
		fprintf(Fv, "Event %d menu=%s\n",    nI + 1, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cMenu    [nI]);
		fprintf(Fv, "Event %d type=%d\n",    nI + 1, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].nType    [nI]);
		fprintf(Fv, "Event %d command=%s\n", nI + 1, RightMouse [RIGHT_MOUSE_BOARD_HANDLE].cCommand [nI]);
	}

	//
	// [User]
	//
	fprintf(Fv, "\n[User]\n");
	fprintf(Fv, "User handle 1=%s\n", Vars.cUserHandle1);
	fprintf(Fv, "User handle 2=%s\n", Vars.cUserHandle2);

	//
	// [Timer]
	//
	fprintf(Fv, "\n[Timer]\n");
	fprintf(Fv, "Minute=%d\n",      TimerCmd.nTimerMinute);
	fprintf(Fv, "Type=%d\n",        TimerCmd.nType);
	fprintf(Fv, "Command=%s\n",     TimerCmd.cTimerCommand);
	fprintf(Fv, "ICC Command=%s\n", TimerCmd.cICCTimerCommand);

	//
	// [Function key]
	//
	fprintf(Fv, "\n[Function key]\n");
	for(nI = 0 ; nI < MAX_FUNCTION_KEY ; nI++)
	{
		if((nI + 1) >= 10)
		{
			sprintf(cTmp,  "Function key %d type",    nI + 2);
			sprintf(cTmp1, "Function key %d command", nI + 2);
		}
		else
		{
			sprintf(cTmp,  "Function key %d type",    nI + 1);
			sprintf(cTmp1, "Function key %d command", nI + 1);
		}
		fprintf(Fv, "%s=%d\n", cTmp,  FKey.nType [nI]);
		fprintf(Fv, "%s=%s\n", cTmp1, FKey.cFKey [nI]);
	}

	//
	// [Control key]
	//
	fprintf(Fv, "\n[Control key]\n");
	for(nI = 0 ; nI < MAX_CONTROL_KEY ; nI++)
	{
		if(nI < 26)
		{
			sprintf(cTmp,  "Control key %c type", (char)(nI + 'A'));
			sprintf(cTmp1, "Control key %c command", (char)(nI + 'A'));
		}
		else
		{
			sprintf(cTmp,  "Control key %c type", (char)((nI - 26) + '0'));
			sprintf(cTmp1, "Control key %c command", (char)((nI - 26) + '0'));
		}

		fprintf(Fv, "%s=%d\n", cTmp,  ControlKey.nType [nI]);
		fprintf(Fv, "%s=%s\n", cTmp1, ControlKey.cFKey [nI]);
	}

	//
	// [Lag command]
	//
	fprintf(Fv, "\n[Lag command]\n");
	sprintf(cTmp, "%d", User.bLagCommand ? 1 : 0);
	fprintf(Fv, "Lag command=%s\n", cTmp);

	sprintf(cTmp, "%d", LagCmd.nLagTolerance);
	fprintf(Fv, "Lag tolerance=%s\n", cTmp);

	sprintf(cTmp, "%d", LagCmd.nLagThreshold);
	fprintf(Fv, "Lag threshold=%s\n", cTmp);

	fprintf(Fv, "Lag type=%d\n", LagCmd.nType);
	fprintf(Fv, "Lag command to issue=%s\n", LagCmd.cLagCommand);
	fprintf(Fv, "Lag display=%s\n", LagCmd.cLagDisplay);

	//
	// [Censor]
	//
	fprintf(Fv, "\n[Censor]\n");
	for(nC = 0 ; nC < MAX_CENSOR ; nC++)
	{
		if(nC == 0)
		{
			strcpy(cTmp, "FICS");
		}
		else if(nC == 1)
		{
			strcpy(cTmp, "ICC");
		}
		else
		{
			strcpy(cTmp, "NONFICS");
		}

		fprintf(Fv, "%s censor=%d\n", cTmp, Censor [nC].bCensor ? 1 : 0);

		for(nI = 0 ; nI < Censor [nC].nItem ; nI++)
		{
			fprintf(Fv, "%s %d=%s %s\n",      cTmp, nI + 1, Censor [nC].cHandle  [nI], Censor [nC].cCensor [nI]);
			fprintf(Fv, "%s %d tell=%s\n",    cTmp, nI + 1, Censor [nC].cTell    [nI]);
			fprintf(Fv, "%s %d ptell=%s\n",   cTmp, nI + 1, Censor [nC].cPtell   [nI]);
			fprintf(Fv, "%s %d say=%s\n",     cTmp, nI + 1, Censor [nC].cSay     [nI]);
			fprintf(Fv, "%s %d partner=%s\n", cTmp, nI + 1, Censor [nC].cPartner [nI]);
			fprintf(Fv, "%s %d message=%s\n", cTmp, nI + 1, Censor [nC].cMessage [nI]);
		}
	}

	//
	// [Channel color]
	//
	fprintf(Fv, "\n[Channel color]\n");
	for(nI = 0 ; nI < MAX_CHANNEL_COLOR ; nI++)
	{
		if(bChannel [nI])
		{
			sprintf(cTmp, "%d", nI);
			TOOLBOX_WriteINIColor(Fv, clrChannel [nI], cTmp);
		}
	}

	fclose(Fv);
}

int INI_ReadSystem(char *cSys)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Sys   [_MAX_PATH];

	char cLS [MAX_LOGIN] [MAX_LOGIN_SIZE + 10];

	int  nC, nI, nJ, nS;
	char cTmp [_MAX_PATH], cTmp1 [_MAX_PATH];

	int bNeedSave = 0;

	_splitpath(cSys, Drive, Dir, File, Ext);
	strcpy(Ext, SYS_EXT);
	_makepath(Sys, Drive, Dir, File, Ext);

	//
	// [Main]
	//

	// browses
	GetPrivateProfileString("Main", "Browse connection", Browse.cConnectionBrowse, Browse.cConnectionBrowse, _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse log",        Browse.cLogBrowse,        Browse.cLogBrowse,        _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse save BPGN",  Browse.cSaveBPGNBrowse,   Browse.cSaveBPGNBrowse,   _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse save PGN",   Browse.cSavePGNBrowse,    Browse.cSavePGNBrowse,    _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse profile",    Browse.cProfileBrowse,    Browse.cProfileBrowse,    _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse piece",      Browse.cPieceBrowse,      Browse.cPieceBrowse,      _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse square",     Browse.cSquareBrowse,     Browse.cSquareBrowse,     _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse PGN",        Browse.cPGNBrowse,        Browse.cPGNBrowse,        _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Browse BPGN",       Browse.cBPGNBrowse,       Browse.cBPGNBrowse,       _MAX_PATH, Sys);
	GetPrivateProfileString("Main", "Last login file",   System.cLoginFile,        System.cLoginFile,        _MAX_PATH, Sys);

	System.nSysPlayGameType    = GetPrivateProfileInt("Main", "Play game type",    System.nSysPlayGameType,    Sys);
	System.nSysExamGameType    = GetPrivateProfileInt("Main", "Exam game type",    System.nSysExamGameType,    Sys);
	System.nSysObserveGameType = GetPrivateProfileInt("Main", "Observe game type", System.nSysObserveGameType, Sys);

	System.nLastPlayGameType    = System.nSysPlayGameType;
	System.nLastExamGameType    = System.nSysExamGameType;
	System.nLastObserveGameType = System.nSysObserveGameType;

	//
	// [Try]
	//
	System.nTryList    = GetPrivateProfileInt("Try", "List",    System.nTryList,    Sys);
	System.nTryScreen  = GetPrivateProfileInt("Try", "Screen",  System.nTryScreen,  Sys);
	System.nTryTaskbar = GetPrivateProfileInt("Try", "Taskbar", System.nTryTaskbar, Sys);
	System.nTryPlay    = GetPrivateProfileInt("Try", "Play",    System.nTryPlay,    Sys);

	//
	// [Match]
	//
	Match.nMatchTime      = GetPrivateProfileInt("Match", "Time",      Match.nMatchTime,      Sys);
	Match.nMatchIncrement = GetPrivateProfileInt("Match", "Increment", Match.nMatchIncrement, Sys);
	Match.bMatchRated     = GetPrivateProfileInt("Match", "Rated",     Match.bMatchRated,     Sys);
	Match.nMatchType      = GetPrivateProfileInt("Match", "Type",      Match.nMatchType,      Sys);
	Match.nICCMatchType   = GetPrivateProfileInt("Match", "ICCType",   Match.nICCMatchType,   Sys);
	Match.nMatchColor     = GetPrivateProfileInt("Match", "Color",     Match.nMatchColor,     Sys);

	//
	// [Seek]
	//
	Seek.nSeekTime      = GetPrivateProfileInt("Seek", "Time",      Seek.nSeekTime,      Sys);
	Seek.nSeekIncrement = GetPrivateProfileInt("Seek", "Increment", Seek.nSeekIncrement, Sys);
	Seek.nSeekRating1   = GetPrivateProfileInt("Seek", "Rating1",   Seek.nSeekRating1,   Sys);
	Seek.nSeekRating2   = GetPrivateProfileInt("Seek", "Rating2",   Seek.nSeekRating2,   Sys);
	Seek.nSeekType      = GetPrivateProfileInt("Seek", "Type",      Seek.nSeekType,      Sys);
	Seek.nICCSeekType   = GetPrivateProfileInt("Seek", "ICCType",   Seek.nICCSeekType,   Sys);
	Seek.nSeekColor     = GetPrivateProfileInt("Seek", "Color",     Seek.nSeekColor,     Sys);
	Seek.bSeekManual    = GetPrivateProfileInt("Seek", "Manual",    Seek.bSeekManual,    Sys);
	Seek.bSeekFormula   = GetPrivateProfileInt("Seek", "Formula",   Seek.bSeekFormula,   Sys);
	Seek.bSeekRated     = GetPrivateProfileInt("Seek", "Rated",     Seek.bSeekRated,     Sys);

	//
	// [History login]
	//
	nJ = 0;
	for(nI = 0 ; nI < MAX_LOGIN ; nI++)
	{
		sprintf(cTmp, "History login string %d", nI + 1);
		strcpy(cTmp1, "");

		GetPrivateProfileString("History login", cTmp, cTmp1, cTmp1, MAX_LOGIN_SIZE, Sys);

		strcpy(cTmp1, TOOLBOX_GetConFilename(cTmp1));

		TOOLBOX_AllTrim(cTmp1);

		if(strlen(cTmp1) > 0)
		{
			if(TOOLBOX_CanOpenFile(TOOLBOX_GetFullConFilename(cTmp1)))
			{
				strcpy(cLS [nJ], cTmp1);
				++nJ;
			}
		}
	}

	Login.nLoginString = 0;
	for(nI = 0 ; nI < nJ ; nI++)
	{
		nS = 0;
		for(nC = 0 ; nC < Login.nLoginString ; nC++)
		{
			if(stricmp(Login.cLoginString [nC], cLS [nI]) == 0)
			{
				nS = 1;
				break;
			}
		}
		if(nS == 0)
		{
			strcpy(Login.cLoginString [Login.nLoginString], cLS [nI]);
			++Login.nLoginString;
		}
	}

	//
	// [Server]
	//
	strcpy(cTmp, "Server 1 address");
	strcpy(cTmp1, "");
	GetPrivateProfileString("Server", cTmp, cTmp1, cTmp1, MAX_SERVER_SIZE, Sys);
	if(strlen(cTmp1) == 0)
	{
		bNeedSave = 1;
	}
	else
	{
		for(nI = 0 ; nI < MAX_SERVER ; nI++)
		{
			sprintf(cTmp, "Server %d address", nI + 1);
			GetPrivateProfileString("Server", cTmp, Server.cServer [nI], Server.cServer [nI], MAX_SERVER_SIZE, Sys);

			sprintf(cTmp, "Server %d port", nI + 1);
			GetPrivateProfileString("Server", cTmp, Server.cPort [nI], Server.cPort [nI], MAX_SERVER_PORT_SIZE, Sys);
			Server.cPort [nI] [MAX_SERVER_PORT_SIZE - 1] = NULL_CHAR;

			sprintf(cTmp, "Server %d prompt", nI + 1);
			strcpy(cTmp1, "");
			GetPrivateProfileString("Server", cTmp, cTmp1, cTmp1, MAX_SERVER_PROMPT_SIZE + 2, Sys);
			nS = strlen(cTmp1);
			if(nS == 0)
			{
				strcpy(Server.cPrompt [nI], "");
			}
			else
			{
				if((cTmp1 [0] == '[') && (cTmp1 [nS - 1] == ']'))
				{
					cTmp1 [nS - 1] = NULL_CHAR;

					for(nJ = 1 ; nJ < nS ; nJ++)
					{
						Server.cPrompt [nI] [nJ - 1] = cTmp1 [nJ];
					}
				}
				else
				{
					strcpy(Server.cPrompt [nI], "");
				}
			}
			Server.cPrompt [nI] [MAX_SERVER_PROMPT_SIZE - 1] = NULL_CHAR;

			sprintf(cTmp, "Server %d type", nI + 1);
			Server.nType [nI] = GetPrivateProfileInt("Server", cTmp, Server.nType [nI], Sys);
		}
	}

	//
	// adjustments
	//
	MATCH_AdjustValues();
	SEEK_Adjust();
	SERVER_Adjust();
	TOOLBOX_SortLoginString();

	//
	// need to save into THIEF.SYS one time
	//
	if(bNeedSave)
	{
		INI_WriteSystem(Sys);
		return 1;
	}
	else
	{
		return 0;
	}
}

void INI_WriteSystem(char *cSys)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Sys   [_MAX_PATH];

	FILE *Fv;
	int  nI;
	char cTmp [_MAX_PATH], cError [1024];

	_splitpath(cSys, Drive, Dir, File, Ext);
	strcpy(Ext, SYS_EXT);
	_makepath(Sys, Drive, Dir, File, Ext);

	//
	// adjustments
	//
	MATCH_AdjustValues();
	SEEK_Adjust();
	SERVER_Adjust();

	if(System.bCDROMConnection)
	{
		return;
	}

	// open
	Fv = fopen(Sys, "w");
	if(Fv == NULL)
	{
		sprintf(cError, "Error writing %s%s.", THIEF_NAME, SYS_EXT);
		TOOLBOX_Error(cError);
		return;
	}

	//
	// [Main]
	//

	// browses
	fprintf(Fv, "[Main]\n");
	fprintf(Fv, "Browse connection=%s\n", Browse.cConnectionBrowse);
	fprintf(Fv, "Browse log=%s\n",        Browse.cLogBrowse);
	fprintf(Fv, "Browse save BPGN=%s\n",  Browse.cSaveBPGNBrowse);
	fprintf(Fv, "Browse save PGN=%s\n",   Browse.cSavePGNBrowse);
	fprintf(Fv, "Browse profile=%s\n",    Browse.cProfileBrowse);
	fprintf(Fv, "Browse piece=%s\n",      Browse.cPieceBrowse);
	fprintf(Fv, "Browse square=%s\n",     Browse.cSquareBrowse);
	fprintf(Fv, "Browse PGN=%s\n",        Browse.cPGNBrowse);
	fprintf(Fv, "Browse BPGN=%s\n",       Browse.cBPGNBrowse);
	fprintf(Fv, "Last login file=%s\n",   System.cLoginFile);

	if((System.nLastPlayGameType == GAMETYPE_BUGHOUSE) ||
			(System.nLastPlayGameType == GAMETYPE_CRAZYHOUSE))
	{
		System.nSysPlayGameType = GAMETYPE_BUGHOUSE;
	}
	else
	{
		System.nSysPlayGameType = GAMETYPE_CHESS;
	}

	if((System.nLastExamGameType == GAMETYPE_BUGHOUSE) ||
			(System.nLastExamGameType == GAMETYPE_CRAZYHOUSE))
	{
		System.nSysExamGameType = GAMETYPE_BUGHOUSE;
	}
	else
	{
		System.nSysExamGameType = GAMETYPE_CHESS;
	}

	if((System.nLastObserveGameType == GAMETYPE_BUGHOUSE) ||
			(System.nLastObserveGameType == GAMETYPE_CRAZYHOUSE))
	{
		System.nSysObserveGameType = GAMETYPE_BUGHOUSE;
	}
	else
	{
		System.nSysObserveGameType = GAMETYPE_CHESS;
	}

	System.nLastPlayGameType    = System.nSysPlayGameType;
	System.nLastExamGameType    = System.nSysExamGameType;
	System.nLastObserveGameType = System.nSysObserveGameType;

	fprintf(Fv, "Play game type=%d\n",    System.nSysPlayGameType);
	fprintf(Fv, "Exam game type=%d\n",    System.nSysExamGameType);
	fprintf(Fv, "Observe game type=%d\n", System.nSysObserveGameType);

	//
	// [Try]
	//
	fprintf(Fv, "\n[Try]\n");
	fprintf(Fv, "List=%d\n",    System.nTryList);
	fprintf(Fv, "Screen=%d\n",  System.nTryScreen);
	fprintf(Fv, "Taskbar=%d\n", System.nTryTaskbar);
	fprintf(Fv, "Play=%d\n",    System.nTryPlay);

	//
	// [Match]
	//
	fprintf(Fv, "\n[Match]\n");
	fprintf(Fv, "Time=%d\n", Match.nMatchTime);
	fprintf(Fv, "Increment=%d\n", Match.nMatchIncrement);
	fprintf(Fv, "Rated=%d\n", Match.bMatchRated ? 1 : 0);
	fprintf(Fv, "Type=%d\n", Match.nMatchType);
	fprintf(Fv, "ICCType=%d\n", Match.nICCMatchType);
	fprintf(Fv, "Color=%d\n", Match.nMatchColor);

	//
	// [Seek]
	//
	fprintf(Fv, "\n[Seek]\n");
	fprintf(Fv, "Time=%d\n", Seek.nSeekTime);
	fprintf(Fv, "Increment=%d\n", Seek.nSeekIncrement);
	fprintf(Fv, "Rating1=%d\n", Seek.nSeekRating1);
	fprintf(Fv, "Rating2=%d\n", Seek.nSeekRating2);
	fprintf(Fv, "Type=%d\n", Seek.nSeekType);
	fprintf(Fv, "ICCType=%d\n", Seek.nICCSeekType);
	fprintf(Fv, "Color=%d\n", Seek.nSeekColor);
	fprintf(Fv, "Manual=%d\n", Seek.bSeekManual ? 1 : 0);
	fprintf(Fv, "Formula=%d\n", Seek.bSeekFormula ? 1 : 0);
	fprintf(Fv, "Rated=%d\n", Seek.bSeekRated ? 1 : 0);

	//
	// [History login]
	//
	fprintf(Fv, "\n[History login]\n");
	for(nI = 0 ; nI < MAX_LOGIN ; nI++)
	{
		sprintf(cTmp, "History login string %d", nI + 1);
		fprintf(Fv, "%s=%s\n", cTmp, Login.cLoginString [nI]);
	}

	//
	// [Server]
	//
	fprintf(Fv, "\n[Server]\n");
	for(nI = 0 ; nI < MAX_SERVER ; nI++)
	{
		sprintf(cTmp, "Server %d address", nI + 1);
		fprintf(Fv, "%s=%s\n", cTmp, Server.cServer [nI]);

		sprintf(cTmp, "Server %d port", nI + 1);
		fprintf(Fv, "%s=%s\n", cTmp, Server.cPort [nI]);

		sprintf(cTmp, "Server %d prompt", nI + 1);
		fprintf(Fv, "%s=[%s]\n", cTmp, Server.cPrompt [nI]);

		sprintf(cTmp, "Server %d type", nI + 1);
		fprintf(Fv, "%s=%d\n", cTmp, Server.nType [nI]);
	}

	fclose(Fv);
}
