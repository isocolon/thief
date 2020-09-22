#include "thief.h"

void LOGIN_File(char *cN, char *Fn)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	strcpy(File, cN);
	TOOLBOX_AllTrim(File);
	strcpy(Ext, LOGIN_EXT);

	_makepath(Fn, Drive, Dir, File, Ext);
}

int LOGIN_Load(char *Fn,        // file name
			   char *cLN,      // name
			   char *cLA,      // address
			   char *cLP,      // port
			   char *cLX,      // timeseal
			   int *nLT,       // type
			   char *cLO,      // prompt
			   char *cLH,      // handle
			   char *cLW,      // password
			   char *cLS)      // script
{
	char cTmpFn [_MAX_PATH], cTmp [_MAX_PATH], cTmp1 [_MAX_PATH], cTmp2 [_MAX_PATH];
	int  nTmp, bWF, nS, nI;

	strcpy(cTmpFn, TOOLBOX_GetConFilename(Fn));
	strcpy(cTmpFn, TOOLBOX_GetFullConFilename(cTmpFn));

	if(! TOOLBOX_CanOpenFile(cTmpFn))
	{
		sprintf(cTmp, "Error Opening %s\n", cTmpFn);
		TOOLBOX_WriteSystem(cTmp);
		return 0;
	}

	GetPrivateProfileString("Login", "Name", cLN, cLN, 255, cTmpFn);
	TOOLBOX_AllTrim(cLN);

	//
	// old login file layout
	//
	//[Login]
	//Name=AAAAAA
	//FICS=1
	//FICS handle=AAAAAA
	//FICS password=xxxx
	//Script file=

	nTmp = -1;
	strcpy(cTmp,  "");
	strcpy(cTmp1, "");

	nTmp = GetPrivateProfileInt("Login", "FICS", nTmp, cTmpFn);

	GetPrivateProfileString("Login", "FICS handle",   cTmp,  cTmp,  MAX_LOGIN_HANDLE_SIZE,   cTmpFn);
	GetPrivateProfileString("Login", "FICS password", cTmp1, cTmp1, MAX_LOGIN_PASSWORD_SIZE, cTmpFn);

	TOOLBOX_AllTrim(cTmp);
	TOOLBOX_AllTrim(cTmp1);

	if((nTmp == -1) && (strlen(cTmp) == 0) && (strlen(cTmp1) == 0))
	{
		bWF = 0;

		GetPrivateProfileString("Login", "Address", cLA, cLA, MAX_LOGIN_ADDRESS_SIZE, cTmpFn);
		GetPrivateProfileString("Login", "Port",    cLP, cLP, MAX_LOGIN_PORT_SIZE,    cTmpFn);
		GetPrivateProfileString("Login", "Helper",  cLX, cLX, _MAX_PATH,              cTmpFn);

		*nLT = GetPrivateProfileInt("Login", "Type", *nLT, cTmpFn);

		strcpy(cTmp2, "");
		GetPrivateProfileString("Login", "Prompt", cTmp2, cTmp2, MAX_LOGIN_PROMPT_SIZE + 2, cTmpFn);

		nS = strlen(cTmp2);
		if(nS == 0)
		{
			strcpy(cLO, "");
		}
		else
		{
			if((cTmp2 [0] == '[') && (cTmp2 [nS - 1] == ']'))
			{
				cTmp2 [nS - 1] = NULL_CHAR;
				for(nI = 1 ; nI < nS ; nI++)
				{
					cLO [nI - 1] = cTmp2 [nI];
				}
			}
			else
			{
				strcpy(cLO, "");
			}
		}

		GetPrivateProfileString("Login", "Handle",   cLH, cLH,  MAX_LOGIN_HANDLE_SIZE,   cTmpFn);
		GetPrivateProfileString("Login", "Password", cLW, cLW,  MAX_LOGIN_PASSWORD_SIZE, cTmpFn);
		GetPrivateProfileString("Login", "Script",   cLS, cLS,  _MAX_PATH,               cTmpFn);
	}
	else
	{
		bWF = 1;

		strcpy(cLH, cTmp);
		strcpy(cLW, cTmp1);

		switch(nTmp)
		{
			case 2 :                // "timeseal.exe freechess.org 23"
				strcpy(cLA, "freechess.org");
				strcpy(cLP, "23");
				strcpy(cLX, "timeseal.exe");
				*nLT = SERVER_FICS;
				strcpy(cLO, FICS_PROMPT);
				break;

			case 3 :                // "timeseal.exe europe.freechess.org 5500"
				strcpy(cLA, "europe.freechess.org");
				strcpy(cLP, "5500");
				strcpy(cLX, "timeseal.exe");
				*nLT = SERVER_FICS;
				strcpy(cLO, FICS_PROMPT);
				break;

			case 4 :                // "timeseal.exe chess-server.spidernet.com.cy 5000"
				strcpy(cLA, "chess-server.spidernet.com.cy");
				strcpy(cLP, "5000");
				strcpy(cLX, "timeseal.exe");
				*nLT = SERVER_NONFICS;
				strcpy(cLO, NONFICS_PROMPT);
				break;

			case 5 :                // "timeseal.exe chess.unix-ag.uni-kl.de 5000"
				strcpy(cLA, "chess.unix-ag.uni-kl.de");
				strcpy(cLP, "5000");
				strcpy(cLX, "timeseal.exe");
				*nLT = SERVER_NONFICS;
				strcpy(cLO, NONFICS_PROMPT);
				break;

			case 6 :                // "timeseal.exe chess.mds.mdh.se 5555"
				strcpy(cLA, "chess.mds.mdh.se");
				strcpy(cLP, "5555");
				strcpy(cLX, "timeseal.exe");
				*nLT = SERVER_NONFICS;
				strcpy(cLO, NONFICS_PROMPT);
				break;

			case 7 :                // "timeseal.exe global.chessparlor.com 6000"
				strcpy(cLA, "global.chessparlor.com");
				strcpy(cLP, "6000");
				strcpy(cLX, "timeseal.exe");
				*nLT = SERVER_FICS;
				strcpy(cLO, FICS_PROMPT);
				break;

			case 8 :                // "timestamp.exe chessclub.com 5000"
				strcpy(cLA, "chessclub.com");
				strcpy(cLP, "5000");
				strcpy(cLX, "timestamp.exe");
				*nLT = SERVER_ICC;
				strcpy(cLO, ICC_PROMPT);
				break;

			case 9 :                // "timestamp.exe queen.chessclub.com 5000"
				strcpy(cLA, "queen.chessclub.com");
				strcpy(cLP, "5000");
				strcpy(cLX, "timestamp.exe");
				*nLT = SERVER_ICC;
				strcpy(cLO, ICC_PROMPT);
				break;

			case 10 :               // "timestamp.exe chessclub.com 23"
				strcpy(cLA, "chessclub.com");
				strcpy(cLP, "23");
				strcpy(cLX, "timestamp.exe");
				*nLT = SERVER_ICC;
				strcpy(cLO, ICC_PROMPT);
				break;

			default :               // "timeseal.exe freechess.org 5000"
#ifndef    KICS
				strcpy(cLA, "freechess.org");
				strcpy(cLP, "5000");
				strcpy(cLO, FICS_PROMPT);
#endif

#ifdef     KICS
				strcpy(cLA, "kics.freechess.org");
				strcpy(cLP, "6000");
				strcpy(cLO, FICS_PROMPT);
#endif

				strcpy(cLX, "timeseal.exe");
				*nLT = SERVER_FICS;
				break;
		}

		GetPrivateProfileString("Login", "Script file", cLS, cLS, 255, cTmpFn);
	}

	TOOLBOX_AllTrim(cLA);
	TOOLBOX_AllTrim(cLP);
	TOOLBOX_AllTrim(cLX);

	if(strlen(cLA) == 0)
	{
#ifndef KICS
		strcpy(cLA, "freechess.org");
#endif

#ifdef KICS
		strcpy(cLA, "kics.freechess.org");
#endif

	}

	if(strlen(cLP) == 0)
	{
#ifndef KICS
		strcpy(cLP, "5000");
#endif

#ifdef KICS
		strcpy(cLP, "6000");
#endif

	}
	cLP [MAX_LOGIN_PORT_SIZE - 1] = NULL_CHAR;

	if(strlen(cLX) > 0)
	{
		strcpy(cLX, TOOLBOX_GetHelperFilename(cLX));
	}

	switch(*nLT)
	{
		case SERVER_FICS :
			strcpy(cLO, FICS_PROMPT);
			break;

		case SERVER_ICC :
			strcpy(cLO, ICC_PROMPT);
			break;

		case SERVER_NONFICS :
			if(strlen(cLO) == 0)
			{
				strcpy(cLO, NONFICS_PROMPT);
			}
			break;

		default :
			*nLT = SERVER_FICS;
			strcpy(cLO, FICS_PROMPT);
			break;
	}
	cLO [MAX_LOGIN_PROMPT_SIZE - 1] = NULL_CHAR;

	TOOLBOX_AllTrim(cLH);

	for(nI = 0 ; nI < ((int) strlen(cLH)) ; nI++)
	{
		if((! isalnum(cLH [nI])) && (cLH [nI] != '-'))
		{
			cLH [nI] = NULL_CHAR;
			break;
		}
	}

	TOOLBOX_AllTrim(cLW);

	if(strlen(cLS) > 0)
	{
		strcpy(cLS, TOOLBOX_GetScriptFilename(cLS));
	}

	TOOLBOX_AllTrim(cLS);

	if(bWF)
	{
		LOGIN_Save(cTmpFn, cLN, cLA, cLP, cLX, nLT, cLO, cLH, cLW, cLS);
	}

	Login.bLoginHelper = (strlen(cLX) > 0);
	Login.nLoginPrompt = strlen(cLO);
	return 1;
}

void LOGIN_Save(char *Fn,        // file name
				char *cLN,      // name
				char *cLA,      // address
				char *cLP,      // port
				char *cLX,      // timeseal
				int *nLT,       // type
				char *cLO,      // prompt
				char *cLH,      // handle
				char *cLW,      // password
				char *cLS)      // script
{
	FILE *Fv;
	char cTmpFn [_MAX_PATH], cTmp [_MAX_PATH];
	int nI;

	if(System.bCDROMConnection)
	{
		return;
	}

	strcpy(cTmpFn, TOOLBOX_GetConFilename(Fn));
	strcpy(cTmpFn, TOOLBOX_GetFullConFilename(cTmpFn));

	Fv = fopen(cTmpFn, "w");
	if(Fv == NULL)
	{
		sprintf(cTmp, "Error Writing %s", cTmpFn);
		TELNET_Write(cTmp);
		return;
	}

	TOOLBOX_AllTrim(cLN);
	TOOLBOX_AllTrim(cLA);
	TOOLBOX_AllTrim(cLP);
	TOOLBOX_AllTrim(cLX);

	if(strlen(cLA) == 0)
	{
#ifndef KICS
		strcpy(cLA, "freechess.org");
#endif
#ifdef KICS
		strcpy(cLA, "kics.freechess.org");
#endif

	}

	if(strlen(cLP) == 0)
	{
#ifndef KICS
		strcpy(cLP, "5000");
#endif
#ifdef KICS
		strcpy(cLP, "6000");
#endif

	}
	cLP [MAX_LOGIN_PORT_SIZE - 1] = NULL_CHAR;

	if(strlen(cLX) > 0)
	{
		strcpy(cLX, TOOLBOX_GetHelperFilename(cLX));
	}

	switch(*nLT)
	{
		case SERVER_FICS :
			strcpy(cLO, FICS_PROMPT);
			break;

		case SERVER_ICC :
			strcpy(cLO, ICC_PROMPT);
			break;

		case SERVER_NONFICS :
			if(strlen(cLO) == 0)
			{
				strcpy(cLO, NONFICS_PROMPT);
			}
			break;

		default :
			*nLT = SERVER_FICS;
			strcpy(cLO, FICS_PROMPT);
			break;
	}
	cLO [MAX_LOGIN_PROMPT_SIZE - 1] = NULL_CHAR;

	TOOLBOX_AllTrim(cLH);

	for(nI = 0 ; nI < ((int) strlen(cLH)) ; nI++)
	{
		if((! isalnum(cLH [nI])) && (cLH [nI] != '-'))
		{
			cLH [nI] = NULL_CHAR;
			break;
		}
	}

	TOOLBOX_AllTrim(cLW);

	if(strlen(cLS) > 0)
	{
		strcpy(cLS, TOOLBOX_GetScriptFilename(cLS));
	}

	TOOLBOX_AllTrim(cLS);

	fprintf(Fv, "[Login]\n");
	fprintf(Fv, "Name=%s\n",     cLN);
	fprintf(Fv, "Address=%s\n",  cLA);
	fprintf(Fv, "Port=%s\n",     cLP);
	fprintf(Fv, "Helper=%s\n",   cLX);
	fprintf(Fv, "Type=%d\n",     *nLT);
	fprintf(Fv, "Prompt=[%s]\n", cLO);
	fprintf(Fv, "Handle=%s\n",   cLH);
	fprintf(Fv, "Password=%s\n", cLW);
	fprintf(Fv, "Script=%s\n",   cLS);

	fclose(Fv);

	Login.bLoginHelper = (strlen(cLX) > 0);
	Login.nLoginPrompt = strlen(cLO);
}

void LOGIN_Prepare(void)
{
	Login.bLoginHelper = (strlen(Login.cLoginHelper) > 0);
	Login.nLoginPrompt = strlen(Login.cLoginPrompt);

	if(Login.bLoginHelper)
	{
		sprintf(Timeseal.cServerAddress,
				"%s %s %s",
				TOOLBOX_GetFullHelperFilename(Login.cLoginHelper),
				Login.cLoginAddress,
				Login.cLoginPort);
	}
	else
	{
		strcpy(Timeseal.cServerAddress, Login.cLoginAddress);
	}
}

void LOGIN_Check(void)
{
	WSADATA WSAData;
	int     nStatus;
	char    cTmp [_MAX_PATH];

	if(!((nStatus = WSAStartup(MAKEWORD(1, 1), &WSAData)) == 0))
	{
		sprintf(cTmp, "%d is The Error\n", nStatus);
		TOOLBOX_WriteSystem(cTmp);
	}
}

int LOGIN_FillAddress(PSOCKADDR_IN psin, int bClient)
{
	DWORD    dwSize;
	PHOSTENT phe;
	char     cTmp [_MAX_PATH];
	u_short  nPortNumber;

	psin->sin_family = AF_INET;

	if(bClient)
	{
		phe = gethostbyname(Timeseal.cSocketBuffer);

		if(phe == NULL)
		{
			TOOLBOX_WriteSystem("gethostbyname() Failed\n");

			sprintf(cTmp, "%d is The Error\n", WSAGetLastError());
			TOOLBOX_WriteSystem(cTmp);

			sprintf(cTmp, "Make Sure '%s' is Listed in The Host File\n", Timeseal.cSocketBuffer);
			TOOLBOX_WriteSystem(cTmp);
			return 0;
		}

		memcpy((char FAR *) &(psin->sin_addr), phe->h_addr, phe->h_length);
	}
	else
	{
		dwSize = sizeof(Timeseal.cSocketBuffer);
		gethostname(Timeseal.cSocketBuffer, dwSize);
		psin->sin_addr.s_addr = INADDR_ANY;
	}

	if(! Login.bLoginHelper)
	{
		nPortNumber = atoi(Login.cLoginPort);
		psin->sin_port = htons(nPortNumber);
	}
	return 1;
}

void LOGIN_Login(void)
{
	SOCKADDR_IN dest_sin;
	char cTmp [_MAX_PATH + 100];
	int nStatus, bOK;

	VARS_Init();
	TELNET_Clear();
	TOOLBOX_CloseSocket();
	TOOLBOX_CloseObserveWindow();
	GAME_InitAllGame(0);
	GAME_InitialSetup(INDEX_PLAY);

	TOOLBOX_InvalidateBoardWindow(0);
	TOOLBOX_RedrawBoard(INDEX_PLAY);
	SetWindowText(hwndWindow [HWND_PLAY], PLAY_BOARD_TITLE);
	SetFocus(hwndWindow [HWND_TELNET_EDIT]);

	switch(Login.nLoginType)
	{
		case SERVER_FICS :
			strcpy(Login.cServerName, "FICS");
			break;

		case SERVER_ICC :
			strcpy(Login.cServerName, "ICC");
			break;

		case SERVER_NONFICS :
			strcpy(Login.cServerName, "NONFICS");
			break;

		default :
			strcpy(Login.cServerName, "FICS");
			break;
	}

	sprintf(cTmp, "Connect Using %s\n", TOOLBOX_GetFullConFilename(Login.cLoginFile));
	TOOLBOX_WriteSystem(cTmp);

	if(User.bLogTelnet)
	{
		if(LOG_Start())
		{
			sprintf(cTmp, "Creating %s Successful\n", Log.cFn);
		}
		else
		{
			sprintf(cTmp, "Creating %s Failed\n", Log.cFn);
		}
		TOOLBOX_WriteSystem(cTmp);
	}

	if(Login.bLoginHelper)
	{
		strcpy(Timeseal.TSInput.cAppSpawn, Timeseal.cServerAddress);
		TOOLBOX_WriteSystem("Loading Helper\n");
		PMI_TS_Load();
	}
	else
	{
		TOOLBOX_WriteSystem("No Helper\n");
		sprintf(cTmp, "Connecting to %s %s\n\n", Timeseal.cServerAddress, Login.cLoginPort);
		TOOLBOX_WriteSystem(cTmp);
	}

	Timeseal.bSocketIsOpen = 1;
	Timeseal.nSocketLogin  = 0;
	Timeseal.bBeforeLogin  = 1;
	Timeseal.bSocketLine   = 0;
	strcpy(Timeseal.cSocketLine, "");

	Telnet.bHasESC         = 0;
	Telnet.bFoundHandle    = 0;
	Telnet.bTelnetSkipLine = 0;

	Fics.bPobserve = 1;
	Fics.nPobserve = 0;
	strcpy(Fics.cPobserve, "");

	System.bIsMyTurn             = 0;
	System.nIssuedMovesCommand   = 0;
	System.nMoveListIndexNumber  = -1;
	System.bIncludeSpace         = 0;
	System.bInDialogBox          = 0;
	System.bUnobserve            = 0;
	System.nUnobGameNumber       = 0;
	strcpy(System.cUnobWname,   "");
	strcpy(System.cUnobBname,   "");
	strcpy(System.cSaveBuffer,  "");
	System.nCensorIndex          = 0;
	System.bIssuedAbort          = 0;
	System.bICCLastGameWasWild16 = 0;
	System.bPromoteCommand       = 0;
	System.nPromoteWPiece        = 0;
	System.nPromoteBPiece        = 0;
	System.bPromoteKnight        = 0;
	System.bInPromote            = 0;
	System.hwndPromote           = NULL;

	TOOLBOX_AddFICSMenu(Login.cLoginFile);

	strcpy(System.cLoginFile, Login.cLoginFile);
	INI_WriteSystem(INI_GetSysFilename());

	TIMERCMD_Clear();
	TOOLBOX_ConnectedMenu();
	TOOLBOX_SetTelnetCaption();

	if(Login.bLoginHelper)
	{
		TOOLBOX_WriteSystem("Connected to Helper\n");
	}

	Telnet.clrCurrent           = clrColor [CLR_TELNET_NORMAL];
	Telnet.clrLastLine          = clrColor [CLR_TELNET_NORMAL];
	Telnet.bLastTelnetIsCR      = 0;
	Telnet.bLastIsTelnetEdit    = 0;
	Telnet.bLastChannel         = 1;
	Telnet.nLastChannel         = 999;
	Telnet.bDisplayLine         = 1;
	Telnet.bLastIsChallenge     = 0;
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
	Telnet.bReceivedMessage     = 0;
	Telnet.nAutoClearedMessage  = 0;

	strcpy(Telnet.cTellTold,     "");
	strcpy(Telnet.cPtellTold,    "");
	strcpy(Telnet.cSayTold,      "");
	strcpy(Telnet.cPartnerTold,  "");
	strcpy(Telnet.cLastGameInfo, "");

	Icc.bIssuedBugCommand  = 0;
	Icc.nIssuedBugCommand  = 0;
	Icc.bStartBugGameList  = 0;
	Icc.nFirstBugGame      = 0;
	Icc.nSecondBugGame     = 0;
	Icc.bSecondBugFlip     = 0;
	Icc.bFindICCPartner    = 0;
	strcpy(Icc.cICCPartner, "");

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

	BUGHOUSE_Init();
	GNOTIFY_Init();

	if(! Login.bLoginHelper)
	{
		strcpy(Timeseal.cSocketBuffer, Timeseal.cServerAddress);

		Timeseal.skSocket = socket(AF_INET, SOCK_STREAM, 0);

		bOK = 1;

		if(Timeseal.skSocket == INVALID_SOCKET)
		{
			TOOLBOX_WriteSystem("socket() Failed\n");
			bOK = 0;
		}

		if(bOK)
		{
			if(! LOGIN_FillAddress(&dest_sin, TRUE))
			{
				TOOLBOX_CloseSocket();
				bOK = 0;
			}
		}

		if(bOK)
		{
			if(connect(Timeseal.skSocket, (PSOCKADDR) &dest_sin, sizeof(dest_sin)) < 0)
			{
				// failed
				bOK = 0;
				TOOLBOX_CloseSocket();
				TOOLBOX_WriteSystem("connect() Failed\n");
			}
			else
			{
				// sucessful
				bOK = 1;
			}
		}

		if(bOK)
		{
			nStatus = WSAAsyncSelect(Timeseal.skSocket, hwndWindow [HWND_FRAME], WSA_READ1, FD_READ | FD_CLOSE);
			if(nStatus > 0)
			{
				TOOLBOX_CloseSocket();
				TOOLBOX_WriteSystem("Error on WSAAsyncSelect()\n");
				bOK = 0;
			}
			else
			{
				bOK = 1;
			}
		}

		if(bOK)
		{
			TELNET_Write("\n");
			TELNET_GoBottom();

			SetWindowText(hwndWindow [HWND_TELNET], Login.cServerName);
			STATE_Login(0);
		}
	}
}

void PMI_TS_Load(void)
{
	HANDLE hChildStdinRd,    hChildStdinWr;
	HANDLE hChildStdoutRd,   hChildStdoutWr;
	HANDLE hChildStdinWrDup, hChildStdoutRdDup;

	SECURITY_ATTRIBUTES saAttr;

	BOOL fSuccess;

	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO         siStartInfo;

	char cTmp [2048];

	if(strlen(Timeseal.TSInput.cAppSpawn) == 0)
	{
		TOOLBOX_WriteSystem("No Helper Found\n");
		return;
	}

	sprintf(cTmp, "Loading %s\n", Timeseal.TSInput.cAppSpawn);
	TOOLBOX_WriteSystem(cTmp);

	// set the bInheritHandle flag so pipe handles are inherited
	saAttr.nLength              = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle       = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	//
	// the steps for redirecting child's STDOUT:
	//     1. Create anonymous pipe to be STDOUT for child.
	//     2. Create a noninheritable duplicate of read handle,
	//         and close the inheritable read handle.
	//

	// create a pipe for the child's STDOUT
	if(! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
	{
		TOOLBOX_WriteSystem("TSInput: CreatePipe () Failed\n");
		return;
	}

	// duplicate the read handle to the pipe, so it is not inherited
	fSuccess = DuplicateHandle(GetCurrentProcess(),
							   hChildStdoutRd,
							   GetCurrentProcess(),
							   &hChildStdoutRdDup,
							   0,
							   FALSE,                      // not inherited
							   DUPLICATE_SAME_ACCESS);
	if(! fSuccess)
	{
		TOOLBOX_WriteSystem("TSInput: DuplicateHandle () Failed\n");
		return;
	}

	CloseHandle(hChildStdoutRd);

	//
	// the steps for redirecting child's STDIN:
	//     1. Create anonymous pipe to be STDIN for child.
	//     2. Create a noninheritable duplicate of write handle,
	//         and close the inheritable write handle.
	//

	// create a pipe for the child's STDIN
	if(! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0))
	{
		TOOLBOX_WriteSystem("TSInput: CreatePipe () Failed\n");
		return;
	}

	// duplicate the write handle to the pipe, so it is not inherited
	fSuccess = DuplicateHandle(GetCurrentProcess(),
							   hChildStdinWr,
							   GetCurrentProcess(),
							   &hChildStdinWrDup,
							   0,
							   FALSE,                      // not inherited
							   DUPLICATE_SAME_ACCESS);
	if(! fSuccess)
	{
		TOOLBOX_WriteSystem("TSInput: DuplicateHandle () Failed\n");
		return;
	}

	CloseHandle(hChildStdinWr);

	// now create the child process
	siStartInfo.cb          = sizeof(STARTUPINFO);
	siStartInfo.lpReserved  = NULL;
	siStartInfo.lpDesktop   = NULL;
	siStartInfo.lpTitle     = NULL;
	siStartInfo.dwFlags     = STARTF_USESTDHANDLES;
	siStartInfo.cbReserved2 = 0;
	siStartInfo.lpReserved2 = NULL;
	siStartInfo.hStdInput   = hChildStdinRd;
	siStartInfo.hStdOutput  = hChildStdoutWr;
	siStartInfo.hStdError   = hChildStdoutWr;

	fSuccess = CreateProcess(NULL,
							 Timeseal.TSInput.cAppSpawn,   // command line
							 NULL,                         // process security attributes
							 NULL,                         // primary thread security attrs
							 TRUE,                         // handles are inherited
							 DETACHED_PROCESS|CREATE_NEW_PROCESS_GROUP,
							 NULL,                         // use parent's environment
							 NULL,
							 &siStartInfo,                 // STARTUPINFO pointer
							 &piProcInfo) ;                // receives PROCESS_INFORMATION

	// close the handles we don't need in the parent
	CloseHandle(hChildStdinRd);
	CloseHandle(hChildStdoutWr);

	Timeseal.TSInput.hProcess = piProcInfo.hProcess;
	Timeseal.TSInput.hFrom    = hChildStdoutRdDup;
	Timeseal.TSInput.hTo      = hChildStdinWrDup;

	bzero(Timeseal.TSInput.cBuffer);

	Timeseal.TSInput.cNext   = Timeseal.TSInput.cBuffer;
	Timeseal.TSInput.bLoadOK = 1;

	sprintf(cTmp, "Loading %s Successful\n", Timeseal.TSInput.cAppSpawn);
	TOOLBOX_WriteSystem(cTmp);

	TOOLBOX_WriteSystem("Creating TSInput Input Thread\n");

	Timeseal.TSInput.hFile   = Timeseal.TSInput.hFrom;
	Timeseal.TSInput.hFrom   = NULL ;          // now owned by PMI_TS_ReadThread
	Timeseal.TSInput.hThread = CreateThread(NULL,
											0,
											(LPTHREAD_START_ROUTINE) PMI_TS_ReadThread,
											(LPVOID) NULL,
											0,
											&Timeseal.TSInput.nID);

	TOOLBOX_WriteSystem("Creating TSInput Input Thread Successful\n");

	SetWindowText(hwndWindow [HWND_TELNET], Login.cServerName);
	STATE_Login(1);
}

void PMI_TS_Write(char *s)
{
	unsigned long nW;

	if(! WriteFile(Timeseal.TSInput.hTo, s, strlen(s), &nW, NULL))       // send it to stdin
	{
		TOOLBOX_Beep();

		TOOLBOX_WriteSystem("Program -> WriteFile (TSInput) Failed\n");
	}
}

DWORD PMI_TS_ReadThread(LPVOID arg)
{
	int  i;
	char *p, *q, prev;

	while((Timeseal.TSInput.hThread != NULL) && Timeseal.TSInput.bLoadOK)
	{
		Timeseal.TSInput.nError = ReadFile(Timeseal.TSInput.hFile,
										   Timeseal.TSInput.cNext,
										   MAX_SOCKET_BUFFER_SIZE - (Timeseal.TSInput.cNext - Timeseal.TSInput.cBuffer),
										   &Timeseal.TSInput.nCount,
										   NULL) ? NO_ERROR : GetLastError();

		if(Timeseal.TSInput.nError == NO_ERROR)
		{
			// change CR LF to CR
			if(Timeseal.TSInput.cNext > Timeseal.TSInput.cBuffer)
			{
				p = Timeseal.TSInput.cNext  - 1;
				i = Timeseal.TSInput.nCount + 1;
			}
			else
			{
				p = Timeseal.TSInput.cNext;
				i = Timeseal.TSInput.nCount;
			}

			q    = p;
			prev = '\0';

			while(i > 0)
			{
				if(prev == '\r' && *p == '\n')
				{
					*(q-1) = '\n';
					Timeseal.TSInput.nCount--;
				}
				else
				{
					*q++ = *p;
				}

				prev = *p++;
				i--;
			}

			*q = '\0';
			Timeseal.TSInput.cNext = q;
		}
		else
		{
			if(Timeseal.TSInput.nError == ERROR_BROKEN_PIPE)
			{
				// correct for MS brain damage.  EOF reading a pipe is not an error
				Timeseal.TSInput.nCount = 0;
			}
			else
			{
				Timeseal.TSInput.nCount = (DWORD) -1;
			}
		}

		SendMessage(hwndWindow [HWND_FRAME], WSA_READ, 0, 0);

		if(Timeseal.TSInput.nCount < 0)
		{
			break ;                 // quit on error
		}
	}

	CloseHandle(Timeseal.TSInput.hFile);
	return 0;
}

void PMI_TS_Destroy(void)
{
	char cTmp [2048];

	if(Timeseal.TSInput.bLoadOK)
	{
		Timeseal.TSInput.bLoadOK = 0;

		sprintf(cTmp, "Releasing %s\n", Timeseal.TSInput.cAppSpawn);
		TOOLBOX_WriteSystem(cTmp);

		CloseHandle(Timeseal.TSInput.hTo) ;             // closing this will give the child an EOF and hopefully kill it

		if(Timeseal.TSInput.hFrom)
		{
			CloseHandle(Timeseal.TSInput.hFrom) ;      // if NULL, PMI_TS_ReadThread will close it
		}

		CloseHandle(Timeseal.TSInput.hProcess);
	}
}
