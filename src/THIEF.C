#define  __MAIN__
#include "thief.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG msg ;
	WNDCLASSEX wndclass ;
	HMODULE hLibModule ;
	int bI, bS ;
	hInst = hInstance ;

	// New in versions >= 1.20: Path for all writeables files is %My Document Folder%\Thief
	if(SHGetSpecialFolderPath(HWND_DESKTOP, System.cDocumentDir, THIEF_DOCUMENT_PATH, FALSE))
	{
		CreateDirectory(TOOLBOX_GetMyDocumentPath(), NULL);
		// needed for compatibility: old System.cInstallDir pointed at thief.exe with full path
#ifndef   KICS
		strcat(System.cDocumentDir, "\\Thief\\thief.txt");
#endif
#ifdef    KICS
		strcat(System.cDocumentDir, "\\THIEFforKICS\\thief.txt");
#endif
	}
	else
	{
		TOOLBOX_Error("Could not find My Documents Folder.") ;
		return 0 ;
	}
	if(_chdir(TOOLBOX_GetMyDocumentPath()))
	{
		char cTmp [2048] ;

		sprintf(cTmp, "Changing to Directory Failed (%s).", TOOLBOX_GetMyDocumentPath()) ;
		TOOLBOX_Error(cTmp) ;
		return 0 ;
	}

	if(! hPrevInstance)
	{
		// register the frame window class
		wndclass.cbSize        = sizeof(wndclass) ;
		wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc   = FrameWndProc ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = 0 ;
		wndclass.hInstance     = hInstance ;
		wndclass.hIcon         = LoadIcon(hInstance, THIEF_ICO) ;
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
		wndclass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1) ;
		wndclass.lpszMenuName  = NULL ;
		wndclass.lpszClassName = "MdiFrame" ;
		wndclass.hIconSm       = LoadIcon(hInstance, THIEF_ICO) ;

		RegisterClassEx(&wndclass) ;

		// register the telnet child window class
		wndclass.cbSize        = sizeof(wndclass) ;
		wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc   = TelnetWndProc ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = sizeof(HANDLE) ;
		wndclass.hInstance     = hInstance ;
		wndclass.hIcon         = LoadIcon(hInstance, THIEF_ICO) ;
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
		wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH) ;
		wndclass.lpszMenuName  = NULL ;
		wndclass.lpszClassName = "MdiTelnetChild" ;
		wndclass.hIconSm       = LoadIcon(hInstance, THIEF_ICO) ;

		RegisterClassEx(&wndclass) ;

		// register the button child window class
		wndclass.cbSize        = sizeof(wndclass) ;
		wndclass.style         = CS_HREDRAW | CS_VREDRAW  ;
		wndclass.lpfnWndProc   = ButtonWndProc ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = sizeof(HANDLE) ;
		wndclass.hInstance     = hInstance ;
		wndclass.hIcon         = LoadIcon(hInstance, THIEF_ICO) ;
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
		wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH) ;
		wndclass.lpszMenuName  = NULL ;
		wndclass.lpszClassName = "MdiButtonChild" ;
		wndclass.hIconSm       = LoadIcon(hInstance, THIEF_ICO) ;

		RegisterClassEx(&wndclass) ;

		// register the play child window class
		wndclass.cbSize        = sizeof(wndclass) ;
		wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc   = PlayWndProc ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = sizeof(HANDLE) ;
		wndclass.hInstance     = hInstance ;
		wndclass.hIcon         = LoadIcon(hInstance, THIEF_ICO) ;
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
		wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH) ;
		wndclass.lpszMenuName  = NULL ;
		wndclass.lpszClassName = "MdiPlayChild" ;
		wndclass.hIconSm       = LoadIcon(hInstance, THIEF_ICO) ;

		RegisterClassEx(&wndclass) ;

		// register the observe child window class
		wndclass.cbSize        = sizeof(wndclass) ;
		wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc   = ObserveWndProc ;
		wndclass.cbClsExtra    = 0 ;
		wndclass.cbWndExtra    = sizeof(HANDLE) ;
		wndclass.hInstance     = hInstance ;
		wndclass.hIcon         = LoadIcon(hInstance, THIEF_ICO) ;
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW) ;
		wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH) ;
		wndclass.lpszMenuName  = NULL ;
		wndclass.lpszClassName = "MdiObserveChild" ;
		wndclass.hIconSm       = LoadIcon(hInstance, THIEF_ICO) ;

		RegisterClassEx(&wndclass) ;
	}

	// obtain handles to three possible menus & submenus
	hMenu [MENU_INIT]        = LoadMenu(hInst, "MAINMENU") ;
	hMenu [MENU_INIT_WINDOW] = GetSubMenu(hMenu [MENU_INIT], INIT_MENU_POS) ;

	// load RICHED32.DLL
	hLibModule = LoadLibrary("RICHED32.DLL") ;

	// initialize
	SYS_InitVal() ;

	bI = INI_ReadSetup(INI_GetIniFilename()) ;
	bS = INI_ReadSystem(INI_GetSysFilename()) ;

	if(bI || bS)
	{
		INI_ReadSetup(INI_GetIniFilename()) ;
		INI_ReadSystem(INI_GetSysFilename()) ;
		SYS_CheckToups() ;
		SYS_CheckNicok() ;
		SYS_CheckKbirk() ;
		SYS_CheckJhurt() ;
		SYS_CheckEchur() ;
		SYS_CheckDfong() ;
		SYS_CheckUser() ;
	}

	GAME_InitAllGame(1) ;

	SYS_CreateVal() ;
	SOUND_Load() ;
	EAR_Load() ;
	GAMESOUND_Load() ;
	TIMESOUND_Load() ;
	BUTTON_Refresh() ;
	DRAW_LoadPieces() ;
	RIGHTMOUSE_LoadCheck() ;
	GAME_Init() ;

	// setup FICS menu
	TOOLBOX_SetupFICSMenu() ;

	// create the frame window
#ifndef KICS
	hwndWindow [HWND_FRAME] = CreateWindow("MdiFrame", "Thief",
										   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
										   wCoord [COORD_MAIN].x,
										   wCoord [COORD_MAIN].y,
										   wCoord [COORD_MAIN].w,
										   wCoord [COORD_MAIN].h,
										   NULL, hMenu [MENU_INIT], hInstance, NULL) ;
#endif

#ifdef KICS
	hwndWindow [HWND_FRAME] = CreateWindow("MdiFrame", "Thief for KICS",
										   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
										   wCoord [COORD_MAIN].x,
										   wCoord [COORD_MAIN].y,
										   wCoord [COORD_MAIN].w,
										   wCoord [COORD_MAIN].h,
										   NULL, hMenu [MENU_INIT], hInstance, NULL) ;
#endif


	hwndWindow [HWND_CLIENT] = GetWindow(hwndWindow [HWND_FRAME], GW_CHILD) ;

	LAYOUT_Load(0) ;
	LAYOUT_LoadVirtual() ;

	if(System.bMaximizeMain)
	{
		ShowWindow(hwndWindow [HWND_FRAME], SW_SHOWMAXIMIZED) ;
	}
	else
	{
		ShowWindow(hwndWindow [HWND_FRAME], iCmdShow) ;
	}

	UpdateWindow(hwndWindow [HWND_FRAME]) ;

	// process command line (connection file)
	TOOLBOX_ProcessCmdLine(szCmdLine) ;

	// message loop
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg) ;
		DispatchMessage(&msg) ;
	}

//
//  while (1)
//      {
//      if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
//          {
//          if (msg.message == WM_QUIT)
//              {
//              break ;
//              }
//
//          TranslateMessage (&msg) ;
//          DispatchMessage  (&msg) ;
//          }
//      else
//          {
//          IDLE_Proc () ;
//          }
//      }
//

	// uninitialize
	if(User.bSaveOnExit)
	{
		INI_WriteSetup(INI_GetIniFilename()) ;
		INI_WriteSystem(INI_GetSysFilename()) ;
	}
	else
	{
		if((System.nLastPlayGameType == GAMETYPE_BUGHOUSE) ||
				(System.nLastPlayGameType == GAMETYPE_CRAZYHOUSE))
		{
			System.nLastPlayGameType = GAMETYPE_BUGHOUSE ;
		}
		else
		{
			System.nLastPlayGameType = GAMETYPE_CHESS ;
		}

		if((System.nLastExamGameType == GAMETYPE_BUGHOUSE) ||
				(System.nLastExamGameType == GAMETYPE_CRAZYHOUSE))
		{
			System.nLastExamGameType = GAMETYPE_BUGHOUSE ;
		}
		else
		{
			System.nLastExamGameType = GAMETYPE_CHESS ;
		}

		if((System.nLastObserveGameType == GAMETYPE_BUGHOUSE) ||
				(System.nLastObserveGameType == GAMETYPE_CRAZYHOUSE))
		{
			System.nLastObserveGameType = GAMETYPE_BUGHOUSE ;
		}
		else
		{
			System.nLastObserveGameType = GAMETYPE_CHESS ;
		}

		if(System.nSysPlayGameType    != System.nLastPlayGameType ||
				System.nSysExamGameType    != System.nLastExamGameType ||
				System.nSysObserveGameType != System.nLastObserveGameType)
		{
			INI_WriteSystem(INI_GetSysFilename()) ;
		}
	}

	TIMESOUND_Destroy() ;
	GAMESOUND_Destroy() ;
	EAR_Destroy() ;
	SOUND_Destroy() ;
	SYS_DestroyVal() ;
	CSET_Destroy() ;
	GAME_Destroy() ;
	LOG_End() ;

	// unload RICHED32.DLL
	FreeLibrary(hLibModule) ;

	return msg.wParam ;
}
