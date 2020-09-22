#include "thief.h"

void BUTTON_Init(void)
{
	BUTTON_Chess() ;
}

void BUTTON_Chess(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_BUTTON ; nI++)
	{
		if(nI <= DEFAULT_CHESS_BUTTONS_USED)
		{
			Buttons.nType [nI] = FUNCTION_COMMAND ;
		}
		else
		{
			Buttons.nType [nI] = FUNCTION_NONE ;
		}
		strcpy(Buttons.cText   [nI], "") ;
		strcpy(Buttons.cAction [nI], "") ;
	}

	Buttons.nType [4] = FUNCTION_MATCH_BOX ;
	Buttons.nType [8] = FUNCTION_SEEK_BOX ;

	strcpy(Buttons.cText   [ 0], DEFAULT_CHESS_BUTTON0_TEXT) ;
	strcpy(Buttons.cAction [ 0], DEFAULT_CHESS_BUTTON0_ACTION) ;

	strcpy(Buttons.cText   [ 1], DEFAULT_CHESS_BUTTON1_TEXT) ;
	strcpy(Buttons.cAction [ 1], DEFAULT_CHESS_BUTTON1_ACTION) ;

	strcpy(Buttons.cText   [ 2], DEFAULT_CHESS_BUTTON2_TEXT) ;
	strcpy(Buttons.cAction [ 2], DEFAULT_CHESS_BUTTON2_ACTION) ;

	strcpy(Buttons.cText   [ 3], DEFAULT_CHESS_BUTTON3_TEXT) ;
	strcpy(Buttons.cAction [ 3], DEFAULT_CHESS_BUTTON3_ACTION) ;

	strcpy(Buttons.cText   [ 4], DEFAULT_CHESS_BUTTON4_TEXT) ;
//        strcpy (Buttons.cAction [ 4], DEFAULT_CHESS_BUTTON4_ACTION) ;

	strcpy(Buttons.cText   [ 5], DEFAULT_CHESS_BUTTON5_TEXT) ;
	strcpy(Buttons.cAction [ 5], DEFAULT_CHESS_BUTTON5_ACTION) ;

	strcpy(Buttons.cText   [ 6], DEFAULT_CHESS_BUTTON6_TEXT) ;
	strcpy(Buttons.cText   [ 6], DEFAULT_CHESS_BUTTON6_ACTION) ;

	strcpy(Buttons.cText   [ 7], DEFAULT_CHESS_BUTTON7_TEXT) ;
	strcpy(Buttons.cAction [ 7], DEFAULT_CHESS_BUTTON7_ACTION) ;

	strcpy(Buttons.cText   [ 8], DEFAULT_CHESS_BUTTON8_TEXT) ;
//        strcpy (Buttons.cAction [ 8], DEFAULT_CHESS_BUTTON8_ACTION) ;

	strcpy(Buttons.cText   [ 9], DEFAULT_CHESS_BUTTON9_TEXT) ;
	strcpy(Buttons.cAction [ 9], DEFAULT_CHESS_BUTTON9_ACTION) ;

	strcpy(Buttons.cText   [10], DEFAULT_CHESS_BUTTON10_TEXT) ;
	strcpy(Buttons.cAction [10], DEFAULT_CHESS_BUTTON10_ACTION) ;

	strcpy(Buttons.cText   [11], DEFAULT_CHESS_BUTTON11_TEXT) ;
	strcpy(Buttons.cAction [11], DEFAULT_CHESS_BUTTON11_ACTION) ;

	strcpy(Buttons.cText   [12], DEFAULT_CHESS_BUTTON12_TEXT) ;
	strcpy(Buttons.cAction [12], DEFAULT_CHESS_BUTTON12_ACTION) ;

	strcpy(Buttons.cText   [13], DEFAULT_CHESS_BUTTON13_TEXT) ;
	strcpy(Buttons.cAction [13], DEFAULT_CHESS_BUTTON13_ACTION) ;

	strcpy(Buttons.cText   [14], DEFAULT_CHESS_BUTTON14_TEXT) ;
	strcpy(Buttons.cAction [14], DEFAULT_CHESS_BUTTON14_ACTION) ;

	strcpy(Buttons.cText   [15], DEFAULT_CHESS_BUTTON15_TEXT) ;
	strcpy(Buttons.cAction [15], DEFAULT_CHESS_BUTTON15_ACTION) ;

	strcpy(Buttons.cText   [16], DEFAULT_CHESS_BUTTON16_TEXT) ;
	strcpy(Buttons.cAction [16], DEFAULT_CHESS_BUTTON16_ACTION) ;

	strcpy(Buttons.cText   [17], DEFAULT_CHESS_BUTTON17_TEXT) ;
	strcpy(Buttons.cAction [17], DEFAULT_CHESS_BUTTON17_ACTION) ;

	strcpy(Buttons.cText   [18], DEFAULT_CHESS_BUTTON18_TEXT) ;
	strcpy(Buttons.cAction [18], DEFAULT_CHESS_BUTTON18_ACTION) ;

	strcpy(Buttons.cText   [19], DEFAULT_CHESS_BUTTON19_TEXT) ;
	strcpy(Buttons.cAction [19], DEFAULT_CHESS_BUTTON19_ACTION) ;

	strcpy(Buttons.cText   [20], DEFAULT_CHESS_BUTTON20_TEXT) ;
	strcpy(Buttons.cAction [20], DEFAULT_CHESS_BUTTON20_ACTION) ;

	strcpy(Buttons.cText   [21], DEFAULT_CHESS_BUTTON21_TEXT) ;
	strcpy(Buttons.cAction [21], DEFAULT_CHESS_BUTTON21_ACTION) ;

	strcpy(Buttons.cText   [22], DEFAULT_CHESS_BUTTON22_TEXT) ;
	strcpy(Buttons.cAction [22], DEFAULT_CHESS_BUTTON22_ACTION) ;

	strcpy(Buttons.cText   [23], DEFAULT_CHESS_BUTTON23_TEXT) ;
	strcpy(Buttons.cAction [23], DEFAULT_CHESS_BUTTON23_ACTION) ;

	strcpy(Buttons.cText   [24], DEFAULT_CHESS_BUTTON24_TEXT) ;
	strcpy(Buttons.cAction [24], DEFAULT_CHESS_BUTTON24_ACTION) ;

	strcpy(Buttons.cText   [25], DEFAULT_CHESS_BUTTON25_TEXT) ;
	strcpy(Buttons.cAction [25], DEFAULT_CHESS_BUTTON25_ACTION) ;

	strcpy(Buttons.cText   [26], DEFAULT_CHESS_BUTTON26_TEXT) ;
	strcpy(Buttons.cAction [26], DEFAULT_CHESS_BUTTON26_ACTION) ;

	strcpy(Buttons.cText   [27], DEFAULT_CHESS_BUTTON27_TEXT) ;
	strcpy(Buttons.cAction [27], DEFAULT_CHESS_BUTTON27_ACTION) ;

	strcpy(Buttons.cText   [28], DEFAULT_CHESS_BUTTON28_TEXT) ;
	strcpy(Buttons.cAction [28], DEFAULT_CHESS_BUTTON28_ACTION) ;

	strcpy(Buttons.cText   [29], DEFAULT_CHESS_BUTTON29_TEXT) ;
	strcpy(Buttons.cAction [29], DEFAULT_CHESS_BUTTON29_ACTION) ;

	if(User.bExtendedButton)
	{
		strcpy(Buttons.cText   [30], DEFAULT_CHESS_BUTTON30_TEXT) ;
		strcpy(Buttons.cAction [30], DEFAULT_CHESS_BUTTON30_ACTION) ;

		strcpy(Buttons.cText   [31], DEFAULT_CHESS_BUTTON31_TEXT) ;
		strcpy(Buttons.cAction [31], DEFAULT_CHESS_BUTTON31_ACTION) ;

		strcpy(Buttons.cText   [32], DEFAULT_CHESS_BUTTON32_TEXT) ;
		strcpy(Buttons.cAction [32], DEFAULT_CHESS_BUTTON32_ACTION) ;

		strcpy(Buttons.cText   [33], DEFAULT_CHESS_BUTTON33_TEXT) ;
		strcpy(Buttons.cAction [33], DEFAULT_CHESS_BUTTON33_ACTION) ;

		strcpy(Buttons.cText   [34], DEFAULT_CHESS_BUTTON34_TEXT) ;
		strcpy(Buttons.cAction [34], DEFAULT_CHESS_BUTTON34_ACTION) ;

		strcpy(Buttons.cText   [35], DEFAULT_CHESS_BUTTON35_TEXT) ;
		strcpy(Buttons.cAction [35], DEFAULT_CHESS_BUTTON35_ACTION) ;
		for(nI = 36 ; nI < MAX_BUTTON ; nI++)
		{
			Buttons.nType [nI] = FUNCTION_NONE ;
			strcpy(Buttons.cText   [nI], "") ;
			strcpy(Buttons.cAction [nI], "") ;
		}
	}
	else
	{
		for(nI = 30 ; nI < MAX_BUTTON ; nI++)
		{
			Buttons.nType [nI] = FUNCTION_NONE ;
			strcpy(Buttons.cText   [nI], "") ;
			strcpy(Buttons.cAction [nI], "") ;
		}
	}
}

void BUTTON_OldFICSEAR(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_BUTTON ; nI++)
	{
		Buttons.nType [nI] = FUNCTION_COMMAND ;
		strcpy(Buttons.cText   [nI], "") ;
		strcpy(Buttons.cAction [nI], "") ;
	}

	strcpy(Buttons.cText   [0], "Sit") ;
	strcpy(Buttons.cAction [0], "ptell Sit") ;

	strcpy(Buttons.cText   [1], "Go") ;
	strcpy(Buttons.cAction [1], "ptell Go") ;

	strcpy(Buttons.cText   [2], "Fast") ;
	strcpy(Buttons.cAction [2], "ptell Fast") ;

	strcpy(Buttons.cText   [3], "Time") ;
	strcpy(Buttons.cAction [3], "ptell Time") ;

	strcpy(Buttons.cText   [4], "++") ;
	strcpy(Buttons.cAction [4], "ptell ++") ;

	strcpy(Buttons.cText   [5], "----") ;
	strcpy(Buttons.cAction [5], "ptell ----") ;

	strcpy(Buttons.cText   [6], "+Q") ;
	strcpy(Buttons.cAction [6], "ptell +Q") ;

	strcpy(Buttons.cText   [7], "-Q") ;
	strcpy(Buttons.cAction [7], "ptell -Q") ;

	strcpy(Buttons.cText   [8], "+R") ;
	strcpy(Buttons.cAction [8], "ptell +R") ;

	strcpy(Buttons.cText   [9], "-R") ;
	strcpy(Buttons.cAction [9], "ptell -R") ;

	strcpy(Buttons.cText   [10], "+N") ;
	strcpy(Buttons.cAction [10], "ptell +N") ;

	strcpy(Buttons.cText   [11], "-N") ;
	strcpy(Buttons.cAction [11], "ptell -N") ;

	strcpy(Buttons.cText   [12], "+B") ;
	strcpy(Buttons.cAction [12], "ptell +B") ;

	strcpy(Buttons.cText   [13], "-B") ;
	strcpy(Buttons.cAction [13], "ptell -B") ;

	strcpy(Buttons.cText   [14], "+P") ;
	strcpy(Buttons.cAction [14], "ptell +P") ;

	strcpy(Buttons.cText   [15], "-P") ;
	strcpy(Buttons.cAction [15], "ptell -P") ;

	strcpy(Buttons.cText   [16], "OK") ;
	strcpy(Buttons.cAction [16], "ptell OK") ;

	strcpy(Buttons.cText   [17], "OK now") ;
	strcpy(Buttons.cAction [17], "ptell OK now") ;

	strcpy(Buttons.cText   [18], "Hard") ;
	strcpy(Buttons.cAction [18], "ptell Hard") ;

	strcpy(Buttons.cText   [19], "Coming") ;
	strcpy(Buttons.cAction [19], "ptell Coming") ;

	strcpy(Buttons.cText   [20], "Maybe") ;
	strcpy(Buttons.cAction [20], "ptell Maybe") ;

	strcpy(Buttons.cText   [21], "I sit") ;
	strcpy(Buttons.cAction [21], "ptell I sit") ;

	strcpy(Buttons.cText   [22], "Mates me") ;
	strcpy(Buttons.cAction [22], "ptell Mates me") ;

	strcpy(Buttons.cText   [23], "Mates him") ;
	strcpy(Buttons.cAction [23], "ptell Mates him") ;

	strcpy(Buttons.cText   [24], "I dead") ;
	strcpy(Buttons.cAction [24], "ptell I dead") ;

	strcpy(Buttons.cText   [25], "Opp dead") ;
	strcpy(Buttons.cAction [25], "ptell Opp dead") ;

	strcpy(Buttons.cText   [26], "Yes") ;
	strcpy(Buttons.cAction [26], "ptell Yes") ;

	strcpy(Buttons.cText   [27], "No") ;
	strcpy(Buttons.cAction [27], "ptell No") ;

	strcpy(Buttons.cText   [28], "Tell Me Go") ;
	strcpy(Buttons.cAction [28], "ptell Tell Me Go") ;

	strcpy(Buttons.cText   [29], "Tell U Go") ;
	strcpy(Buttons.cAction [29], "ptell Sit Until I Tell U Go") ;

	strcpy(Buttons.cText   [30], "Keep Check") ;
	strcpy(Buttons.cAction [30], "ptell Keep Checking") ;

	strcpy(Buttons.cText   [31], "Nevermind") ;
	strcpy(Buttons.cAction [31], "ptell Nevermind") ;

	strcpy(Buttons.cText   [32], "We up") ;
	strcpy(Buttons.cAction [32], "ptell We up") ;

	strcpy(Buttons.cText   [33], "We down") ;
	strcpy(Buttons.cAction [33], "ptell We down") ;

	strcpy(Buttons.cText   [34], "U get") ;
	strcpy(Buttons.cAction [34], "ptell U get") ;

	strcpy(Buttons.cText   [35], "He gets") ;
	strcpy(Buttons.cAction [35], "ptell He gets") ;

	strcpy(Buttons.cText   [36], "Watchout") ;
	strcpy(Buttons.cAction [36], "ptell Watchout") ;

	strcpy(Buttons.cText   [37], "Feed me") ;
	strcpy(Buttons.cAction [37], "ptell Feed me") ;

	strcpy(Buttons.cText   [38], "Lag") ;
	strcpy(Buttons.cAction [38], "ptell Lag") ;

#ifndef  KICS
	strcpy(Buttons.cText   [39], "Lag cheat") ;
	strcpy(Buttons.cAction [39], "ptell Lag cheating fuckers") ;

	strcpy(Buttons.cText   [40], "Mother lag") ;
	strcpy(Buttons.cAction [40], "ptell Mother fucking lag") ;

	strcpy(Buttons.cText   [41], "Bitch dead") ;
	strcpy(Buttons.cAction [41], "ptell Bitch is dead") ;

	strcpy(Buttons.cText   [42], "Bite me") ;
	strcpy(Buttons.cAction [42], "ptell Bite me") ;

	strcpy(Buttons.cText   [43], "Cake") ;
	strcpy(Buttons.cAction [43], "ptell Cake") ;

	strcpy(Buttons.cText   [44], "Eat shit") ;
	strcpy(Buttons.cAction [44], "ptell Eat shit") ;

	strcpy(Buttons.cText   [45], "Fuck u") ;
	strcpy(Buttons.cAction [45], "ptell Fuck u") ;

	strcpy(Buttons.cText   [46], "I focked") ;
	strcpy(Buttons.cAction [46], "ptell I finger focked") ;

	strcpy(Buttons.cText   [47], "I focking") ;
	strcpy(Buttons.cAction [47], "ptell I finger focking") ;

	strcpy(Buttons.cText   [48], "Fast bitch") ;
	strcpy(Buttons.cAction [48], "ptell Move faster bitch") ;

	strcpy(Buttons.cText   [49], "Son bitch") ;
	strcpy(Buttons.cAction [49], "ptell Son of a bitch") ;

	strcpy(Buttons.cText   [50], "So easy") ;
	strcpy(Buttons.cAction [50], "ptell So easy") ;

	strcpy(Buttons.cText   [51], "She dead") ;
	strcpy(Buttons.cAction [51], "ptell She dead") ;

	strcpy(Buttons.cText   [52], "They suck") ;
	strcpy(Buttons.cAction [52], "ptell They suck") ;

	Buttons.nType [53] = FUNCTION_NONE ;
	strcpy(Buttons.cText   [53], "") ;
	strcpy(Buttons.cAction [53], "") ;
#endif
}

void BUTTON_NewFICSEAR(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_BUTTON ; nI++)
	{
		Buttons.nType [nI] = FUNCTION_COMMAND ;
		strcpy(Buttons.cText   [nI], "") ;
		strcpy(Buttons.cAction [nI], "") ;
	}

	strcpy(Buttons.cText   [0], "Sit") ;
	strcpy(Buttons.cAction [0], "ptell Sit") ;

	strcpy(Buttons.cText   [1], "Go") ;
	strcpy(Buttons.cAction [1], "ptell Go") ;

	strcpy(Buttons.cText   [2], "Play Fast!") ;
	strcpy(Buttons.cAction [2], "ptell Fast") ;

	strcpy(Buttons.cText   [3], "Watch Time") ;
	strcpy(Buttons.cAction [3], "ptell Time") ;

	strcpy(Buttons.cText   [4], "+") ;
	strcpy(Buttons.cAction [4], "ptell +") ;

	strcpy(Buttons.cText   [5], "++") ;
	strcpy(Buttons.cAction [5], "ptell ++") ;

	strcpy(Buttons.cText   [6], "+++") ;
	strcpy(Buttons.cAction [6], "ptell +++") ;

	strcpy(Buttons.cText   [7], "-") ;
	strcpy(Buttons.cAction [7], "ptell -") ;

	strcpy(Buttons.cText   [8], "- -") ;
	strcpy(Buttons.cAction [8], "ptell - -") ;

	strcpy(Buttons.cText   [9], "- - -") ;
	strcpy(Buttons.cAction [9], "ptell - - -") ;

	strcpy(Buttons.cText   [10], "Pawn") ;
	strcpy(Buttons.cAction [10], "ptell Pawn") ;

	strcpy(Buttons.cText   [11], "Bishop") ;
	strcpy(Buttons.cAction [11], "ptell Bishop") ;

	strcpy(Buttons.cText   [12], "Knight") ;
	strcpy(Buttons.cAction [12], "ptell Knight") ;

	strcpy(Buttons.cText   [13], "Rook") ;
	strcpy(Buttons.cAction [13], "ptell Rook") ;

	strcpy(Buttons.cText   [14], "Queen") ;
	strcpy(Buttons.cAction [14], "ptell Queen") ;

	strcpy(Buttons.cText   [15], "I lose...") ;
	strcpy(Buttons.cAction [15], "ptell I lose...") ;

	strcpy(Buttons.cText   [16], "OK") ;
	strcpy(Buttons.cAction [16], "ptell OK") ;

	strcpy(Buttons.cText   [17], "OK Now") ;
	strcpy(Buttons.cAction [17], "ptell OK Now") ;

	strcpy(Buttons.cText   [18], "Hard") ;
	strcpy(Buttons.cAction [18], "ptell Hard") ;

	strcpy(Buttons.cText   [19], "Coming") ;
	strcpy(Buttons.cAction [19], "ptell Coming") ;

	strcpy(Buttons.cText   [20], "Maybe") ;
	strcpy(Buttons.cAction [20], "ptell Maybe") ;

	strcpy(Buttons.cText   [21], "I sit") ;
	strcpy(Buttons.cAction [21], "ptell I sit") ;

	strcpy(Buttons.cText   [22], "Can I go?") ;
	strcpy(Buttons.cAction [22], "ptell Can I go?") ;

	strcpy(Buttons.cText   [23], "Nevermind") ;
	strcpy(Buttons.cAction [23], "ptell Nevermind") ;

	strcpy(Buttons.cText   [24], "Take") ;
	strcpy(Buttons.cAction [24], "ptell Take") ;

	strcpy(Buttons.cText   [25], "What u want?") ;
	strcpy(Buttons.cAction [25], "ptell What u want?") ;

	strcpy(Buttons.cText   [26], "No!") ;
	strcpy(Buttons.cAction [26], "ptell No!") ;

	strcpy(Buttons.cText   [27], "Keep Check") ;
	strcpy(Buttons.cAction [27], "ptell Keep Checking") ;

	strcpy(Buttons.cText   [28], "Tell Me Go") ;
	strcpy(Buttons.cAction [28], "ptell Tell Me Go") ;

	strcpy(Buttons.cText   [29], "Tell U Go") ;
	strcpy(Buttons.cAction [29], "ptell Sit Until I Tell U Go") ;

	strcpy(Buttons.cText   [30], "I Mated") ;
	strcpy(Buttons.cAction [30], "ptell I Mated") ;

	strcpy(Buttons.cText   [31], "Opp Mated") ;
	strcpy(Buttons.cAction [31], "ptell Opp Mated") ;

	strcpy(Buttons.cText   [32], "We up") ;
	strcpy(Buttons.cAction [32], "ptell We up") ;

	strcpy(Buttons.cText   [33], "We down") ;
	strcpy(Buttons.cAction [33], "ptell We down") ;

	strcpy(Buttons.cText   [34], "U get") ;
	strcpy(Buttons.cAction [34], "ptell U get") ;

	strcpy(Buttons.cText   [35], "He gets") ;
	strcpy(Buttons.cAction [35], "ptell He gets") ;

	strcpy(Buttons.cText   [36], "Watchout") ;
	strcpy(Buttons.cAction [36], "ptell Watchout") ;

	strcpy(Buttons.cText   [37], "Feed me") ;
	strcpy(Buttons.cAction [37], "ptell Feed me") ;

	strcpy(Buttons.cText   [38], "Lag") ;
	strcpy(Buttons.cAction [38], "ptell Lag") ;

#ifndef  KICS
	strcpy(Buttons.cText   [39], "Lag cheat") ;
	strcpy(Buttons.cAction [39], "ptell Lag cheating fuckers") ;

	strcpy(Buttons.cText   [40], "Mother lag") ;
	strcpy(Buttons.cAction [40], "ptell Mother fucking lag") ;

	strcpy(Buttons.cText   [41], "Bitch dead") ;
	strcpy(Buttons.cAction [41], "ptell Bitch is dead") ;

	strcpy(Buttons.cText   [42], "Bite me") ;
	strcpy(Buttons.cAction [42], "ptell Bite me") ;

	strcpy(Buttons.cText   [43], "Cake") ;
	strcpy(Buttons.cAction [43], "ptell Cake") ;

	strcpy(Buttons.cText   [44], "Eat shit") ;
	strcpy(Buttons.cAction [44], "ptell Eat shit") ;

	strcpy(Buttons.cText   [45], "Fuck u") ;
	strcpy(Buttons.cAction [45], "ptell Fuck u") ;

	strcpy(Buttons.cText   [46], "I focked") ;
	strcpy(Buttons.cAction [46], "ptell I finger focked") ;

	strcpy(Buttons.cText   [47], "I focking") ;
	strcpy(Buttons.cAction [47], "ptell I finger focking") ;

	strcpy(Buttons.cText   [48], "Fast bitch") ;
	strcpy(Buttons.cAction [48], "ptell Move faster bitch") ;

	strcpy(Buttons.cText   [49], "Son bitch") ;
	strcpy(Buttons.cAction [49], "ptell Son of a bitch") ;

	strcpy(Buttons.cText   [50], "So easy") ;
	strcpy(Buttons.cAction [50], "ptell So easy") ;

	strcpy(Buttons.cText   [51], "She dead") ;
	strcpy(Buttons.cAction [51], "ptell She dead") ;

	strcpy(Buttons.cText   [52], "They suck") ;
	strcpy(Buttons.cAction [52], "ptell They suck") ;
#endif

	Buttons.nType [53] = FUNCTION_NONE ;
	strcpy(Buttons.cText   [53], "") ;
	strcpy(Buttons.cAction [53], "") ;
}

void BUTTON_RefreshLabel(void)
{
	int nI ;

	for(nI = 0 ; nI < MAX_BUTTON ; nI++)
	{
		SetWindowText(hwndButton [nI], Buttons.cText [nI]) ;
	}
}

LRESULT CALLBACK ButtonWndProc(HWND hwnd, UINT iMsg, WPARAM wParam,
							   LPARAM lParam)
{
	PAINTSTRUCT ps ;
	int         nW, nH, nI, nJ, nX, nY ;

	switch(iMsg)
	{
		case WM_COMMAND :
			if(HIWORD(wParam) == BN_CLICKED)
			{
				if((LOWORD(wParam) >= 0) && (LOWORD(wParam) < MAX_BUTTON))
				{
					BUTTON_Command(LOWORD(wParam)) ;
				}
			}
			break ;

		case WM_KEYDOWN :
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
			return 0;

		case WM_CHAR :
			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
			return 0;

		case WM_MDIACTIVATE :
			if(lParam == (LPARAM) hwnd)
			{
				if(wCoord [COORD_BUTTON].s == WIN_SHOW)
				{
					ShowWindow(hwnd, SW_SHOW) ;
				}
				else
				{
					//ShowWindow (hwnd, SW_HIDE) ;
					ShowWindow(hwnd, SW_MINIMIZE) ;
				}
			}
			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			return 0 ;

		case WM_CREATE :
			for(nI = 0 ; nI < MAX_BUTTON ; nI++)
			{
				hwndButton [nI] = CreateWindow("BUTTON", Buttons.cText [nI],
											   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
											   0, 0, 20, 20, hwnd, (HMENU) nI,
											   ((LPCREATESTRUCT) lParam) -> hInstance, NULL) ;

				SendMessage(hwndButton [nI], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM(TRUE, 0)) ;

				WndProcButton = (WNDPROC) SetWindowLong(hwndButton [nI], GWL_WNDPROC, (LONG) ButtonSubclass) ;
			}
			return 0 ;

		case WM_SETFONT :
			for(nI = 0 ; nI < MAX_BUTTON ; nI++)
			{
				SendMessage(hwndButton [nI], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM(TRUE, 0)) ;
			}
			return 0 ;

		case WM_PAINT :
			BeginPaint(hwnd, &ps) ;
			EndPaint(hwnd, &ps) ;
			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			break ;

		case WM_SIZE :
			if(IsIconic(hwnd))
			{
				wCoord [COORD_BUTTON].s = WIN_HIDE ;
			}
			else
			{
				wCoord [COORD_BUTTON].s = WIN_SHOW ;

				if(HIWORD(lParam) > LOWORD(lParam))
				{
					nW = LOWORD(lParam) >> 1 ;

					if(User.bExtendedButton)
					{
						nH = HIWORD(lParam) / HALF_BUTTON ;

						nI = 0 ;
						for(nJ = 0 ; nJ < HALF_BUTTON ; nJ++)
						{
							nY = nJ * nH ;

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++],  0, nY, nW, nH, TRUE) ;
							}

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++], nW, nY, nW, nH, TRUE) ;
							}
						}
					}
					else
					{
						nH = HIWORD(lParam) / HALF_REGULAR_BUTTON ;

						nI = 0 ;
						for(nJ = 0 ; nJ < HALF_REGULAR_BUTTON ; nJ++)
						{
							if(nI >= EXTEND_BUTTON)
							{
								nY = (MAX_BUTTON + 10) * nH ;
							}
							else
							{
								nY = nJ * nH ;
							}

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++], 0, nY, nW, nH, TRUE) ;
							}

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++], nW, nY, nW, nH, TRUE) ;
							}
						}

						for(; nI < MAX_BUTTON ; nI++)
						{
							nY = (MAX_BUTTON + 10) * nH ;
							MoveWindow(hwndButton [nI], 0, nY, nW, nH, TRUE) ;
						}
					}
				}
				else
				{
					if(User.bExtendedButton)
					{
						nW = LOWORD(lParam) / ONE_THIRD_BUTTON ;
						nH = HIWORD(lParam) / 3 ;

						nI = 0 ;
						for(nJ = 0 ; nJ < ONE_THIRD_BUTTON ; nJ++)
						{
							nX = nJ * nW ;

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++], nX,  0, nW, nH, TRUE) ;
							}

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++], nX, nH, nW, nH, TRUE) ;
							}
						}

						nJ = 0 ;
						for(; nI < MAX_BUTTON ; nI++)
						{
							nX = nJ * nW ;
							nJ = nJ + 1 ;

							MoveWindow(hwndButton [nI], nX, nH + nH, nW, nH, TRUE) ;
						}
					}
					else
					{
						nW = LOWORD(lParam) / HALF_REGULAR_BUTTON ;
						nH = HIWORD(lParam) >> 1 ;

						nI = 0 ;
						for(nJ = 0 ; nJ < HALF_REGULAR_BUTTON ; nJ++)
						{
							if(nI >= EXTEND_BUTTON)
							{
								nX = (MAX_BUTTON + 10) * nW ;
							}
							else
							{
								nX = nJ * nW ;
							}

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++], nX,  0, nW, nH, TRUE) ;
							}

							if(nI < MAX_BUTTON)
							{
								MoveWindow(hwndButton [nI++], nX, nH, nW, nH, TRUE) ;
							}
						}

						for(; nI < MAX_BUTTON ; nI++)
						{
							nX = (MAX_BUTTON + 10) * nW ;
							MoveWindow(hwndButton [nI], nX, nH + nH, nW, nH, TRUE) ;
						}
					}
				}
			}
			break ;

		case WM_CLOSE :
			wCoord [COORD_BUTTON].s = WIN_HIDE ;
			//ShowWindow (hwnd, SW_HIDE) ;
			ShowWindow(hwnd, SW_MINIMIZE) ;
			SendMessage(hwndWindow [HWND_CLIENT], WM_MDINEXT, (LPARAM) NULL, 0) ;
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

LRESULT CALLBACK ButtonSubclass(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu, hSubMenu ;
	POINT pt ;
	int nI, nJ ;
	char cTmp [THIEF_COMMAND_SIZE + 10] ;

	switch(iMsg)
	{
		case WM_CHAR :
			SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
			PostMessage(hwndWindow [HWND_TELNET_EDIT], iMsg, wParam, lParam) ;
			return 0 ;

		case WM_RBUTTONDOWN :
			nJ = 0 ;
			for(nI = 0 ; nI < MAX_RM_ITEM ; nI++)
			{
				if(strlen(RightMouse [RIGHT_MOUSE_BUTTON].cMenu [nI]) == 0)
				{
					break ;
				}

				if(TOOLBOX_IsSeparator(RIGHT_MOUSE_BUTTON, nI))
				{
					if(nJ == 0)
					{
						System.nRightMouseIndex = RIGHT_MOUSE_BUTTON ;

						hMenu    = LoadMenu(hInst, "ONEITEMMENU") ;
						hSubMenu = GetSubMenu(hMenu, 0) ;

						DeleteMenu(hSubMenu, 0, MF_BYPOSITION) ;
					}

					AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0) ;

					nJ = nJ + 1 ;
				}
				else if(TOOLBOX_OKRightMouse(RIGHT_MOUSE_BUTTON, nI))
				{
					if(nJ == 0)
					{
						System.nRightMouseIndex = RIGHT_MOUSE_BUTTON ;

						hMenu    = LoadMenu(hInst, "ONEITEMMENU") ;
						hSubMenu = GetSubMenu(hMenu, 0) ;

						DeleteMenu(hSubMenu, 0, MF_BYPOSITION) ;
					}

					if(strchr(RightMouse [RIGHT_MOUSE_BUTTON].cMenu [nI], '%'))
					{
						TOOLBOX_ExpandAllMacro(cTmp, RightMouse [RIGHT_MOUSE_BUTTON].cMenu [nI], 0) ;
						AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), cTmp) ;
					}
					else
					{
						AppendMenu(hSubMenu, MF_STRING, (WM_USER_RIGHT_MOUSE0 + nI), RightMouse [RIGHT_MOUSE_BUTTON].cMenu [nI]) ;
					}

					if(FCheck [RightMouse [RIGHT_MOUSE_BUTTON].nType [nI]])
					{
						CheckMenuItem(hSubMenu, (WM_USER_RIGHT_MOUSE0 + nI), MF_CHECKED) ;
					}

					nJ = nJ + 1 ;
				}
			}
			if(nJ > 0)
			{
				pt.x = LOWORD(lParam) ;
				pt.y = HIWORD(lParam) ;
				TOOLBOX_MenuPopup(hwnd, pt, hMenu, -1) ;
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
					RIGHTMOUSE_Command(System.nRightMouseIndex, (LOWORD(wParam) - WM_USER_RIGHT_MOUSE0), User.bButtonCommandAddHist) ;
					break ;
			}
			return 0 ;
	}
	return (*WndProcButton)(hwnd, iMsg, wParam, lParam) ;
}

void BUTTON_Refresh(void)
{
	strcpy(Vars.cPartnerTell1, Vars.cPartner) ;
	strcat(Vars.cPartnerTell1, " tells you:") ;

	strcpy(Vars.cPartnerTell2, Vars.cPartner) ;
	strcat(Vars.cPartnerTell2, "(U) tells you:") ;

	if(Login.nLoginType != SERVER_FICS)
	{
		strupr(Vars.cPartnerTell1) ;
		strupr(Vars.cPartnerTell2) ;
	}
}

void BUTTON_Command(int nI)
{
	char cTmp [MAX_BUTTON_ACTION + 10] ;
	char *token;

//    if (Buttons.nType [nI] != FUNCTION_NONE)
//        {
//        TOOLBOX_Command (Buttons.nType [nI], Buttons.cAction [nI], User.bButtonCommandAddHist) ;
//        }
//    }


	// New Routine for Thief 1.2 (Ludens): Interpret "#" as linebreak
	if(Buttons.nType [nI] != FUNCTION_NONE)
	{
		strcpy(cTmp, Buttons.cAction [nI]) ;
		token = strtok(cTmp, "#");
		if(token != NULL)
		{
			while(token != NULL)
			{
				/* While there are tokens in "cTmp" */
				TOOLBOX_Command(Buttons.nType [nI], token, User.bButtonCommandAddHist) ;
				token = strtok(NULL, "#");
			}
		}
		else
		{
			TOOLBOX_Command(Buttons.nType [nI], Buttons.cAction [nI], User.bButtonCommandAddHist) ;
		}
	}
}