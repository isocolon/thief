#include "thief.h"

#define ICC_MATCH_TYPE  30
#define FICS_MATCH_TYPE 14

char *ICC_MtType [ICC_MATCH_TYPE]  = { "Chess",
									   "Wild 1",
									   "Wild 2",
									   "Wild 3",
									   "Wild 4",
									   "Wild 5",
									   "Wild 6",
									   "Wild 7",
									   "Wild 8",
									   "Wild 9",
									   "Wild 10",
									   "Wild 11",
									   "Wild 12",
									   "Wild 13",
									   "Wild 14",
									   "Wild 15",
									   "Kriegspiel",
									   "Loser's chess",
									   "Power chess",
									   "Wild 19",
									   "Wild 20",
									   "Wild 21",
									   "Fischer random",
									   "Crazyhouse",
									   "Bughouse",
									   "Wild 25",
									   "Giveaway",
									   "Atomic",
									   "Wild 28",
									   "Wild 29"
									 } ;

char *_ICC_MtType [ICC_MATCH_TYPE] = { "w0",
									   "w1",
									   "w2",
									   "w3",
									   "w4",
									   "w5",
									   "w6",
									   "w7",
									   "w8",
									   "w9",
									   "w10",
									   "w11",
									   "w12",
									   "w13",
									   "w14",
									   "w15",
									   "w16",
									   "w17",
									   "w18",
									   "w19",
									   "w20",
									   "w21",
									   "w22",
									   "w23",
									   "w24",
									   "w25",
									   "w26",
									   "w27",
									   "w28",
									   "w29"
									 } ;

char *_ICC_MtType1 [ICC_MATCH_TYPE] = { " b",
										" 1",
										" 2",
										" 3",
										" 4",
										" 5",
										" 6",
										" 7",
										" 8",
										" 9",
										"10",
										"11",
										"12",
										"13",
										"14",
										"15",
										"16",
										"17",
										"18",
										"19",
										"20",
										"21",
										"22",
										"23",
										"24",
										"25",
										"26",
										"27",
										"28",
										"29"
									  } ;

char *FICS_MtType [FICS_MATCH_TYPE] =  { "Bughouse",
										 "Crazyhouse",
										 "Chess",
										 "Suicide",
										 "Wild 0",
										 "Wild 1",
										 "Wild 2",
										 "Wild 3",
										 "Wild 4",
										 "Wild 5",
										 "Wild 8",
										 "Wild 8a",
										 "Fischer random",
										 "Atomic"
									   } ;

char *_FICS_MtType [FICS_MATCH_TYPE] = { "bug",
										 "zh",
										 "chess",
										 "suicide",
										 "w0",
										 "w1",
										 "w2",
										 "w3",
										 "w4",
										 "w5",
										 "w8",
										 "w8a",
										 "wild fr",
										 "atomic"
									   } ;

char *MT_Color [3] = { "Auto",
					   "Black",
					   "White"
					 } ;

char *_MT_Color [3] = { "",
						"b",
						"w"
					  } ;

BOOL CALLBACK MatchBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hOpponent, hInitial, hIncrement, hRated, hType, hColor ;
	char cTmp [MSG_SIZE], cOpp [MSG_SIZE], cTime [MSG_SIZE], cInc [MSG_SIZE] ;
	int nCount, nI, nR, nT, nC, nJ ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			if(Match.bMatchMouse)
			{
				Match.bMatchMouse = 0 ;
				TOOLBOX_GetTelnetHandle(FALSE) ;
				strcpy(cTmp, Vars.cTelnetHandle) ;
			}
			else
			{
				strcpy(cTmp, Match.cMatchHandle) ;
			}

			cTmp [ICS_HANDLE_LENGTH] = NULL_CHAR ;

			if(strlen(cTmp) == 0)
			{
				strcpy(cTmp, Vars.cOpponent) ;
			}

			strcpy(Match.cMatchHandle, cTmp) ;

			hOpponent  = GetDlgItem(hwnd, IDD_OPPONENT) ;
			hInitial   = GetDlgItem(hwnd, IDD_INITIAL) ;
			hIncrement = GetDlgItem(hwnd, IDD_INCREMENT) ;
			hRated     = GetDlgItem(hwnd, IDD_RATED) ;
			hType      = GetDlgItem(hwnd, IDD_TYPE) ;
			hColor     = GetDlgItem(hwnd, IDD_COLOR) ;

			SendDlgItemMessage(hwnd, IDD_OPPONENT, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp) ;

			sprintf(cTmp, "%d", Match.nMatchTime) ;
			SendDlgItemMessage(hwnd, IDD_INITIAL, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp) ;

			sprintf(cTmp, "%d", Match.nMatchIncrement) ;
			SendDlgItemMessage(hwnd, IDD_INCREMENT, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp) ;

			if(Match.bMatchRated)
			{
				SendDlgItemMessage(hwnd, IDD_RATED, BM_SETCHECK, BST_CHECKED, 0) ;
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_RATED, BM_SETCHECK, BST_UNCHECKED, 0) ;
			}

			SendMessage(hType, CB_RESETCONTENT, 0, 0) ;
			if(Login.nLoginType == SERVER_ICC)
			{
				nCount = ICC_MATCH_TYPE ;
			}
			else
			{
				nCount = FICS_MATCH_TYPE ;
			}

			for(nI = 0 ; nI < nCount ; nI++)
			{
				if(Login.nLoginType == SERVER_ICC)
				{
					SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) ICC_MtType [nI]) ;
				}
				else
				{
					SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) FICS_MtType [nI]) ;
				}
			}

			if(Login.nLoginType == SERVER_ICC)
			{
				if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) ICC_MtType [Match.nICCMatchType]) == CB_ERR)
				{
					SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
					SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) ICC_MtType [0]) ;
				}
			}
			else
			{
				if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) FICS_MtType [Match.nMatchType]) == CB_ERR)
				{
					SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
					SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) FICS_MtType [0]) ;
				}
			}

			SendMessage(hColor, CB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < 3 ; nI++)
			{
				SendMessage(hColor, CB_ADDSTRING, 0, (LPARAM) MT_Color [nI]) ;
			}

			if(SendMessage(hColor, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) MT_Color [Match.nMatchColor]) == CB_ERR)
			{
				SendMessage(hColor, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
				SendMessage(hColor, WM_SETTEXT, (WPARAM) 0, (LPARAM) MT_Color [0]) ;
			}

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					SendDlgItemMessage(hwnd, IDD_OPPONENT,  WM_GETTEXT, (WPARAM) 255, (LPARAM) cOpp) ;
					SendDlgItemMessage(hwnd, IDD_INITIAL,   WM_GETTEXT, (WPARAM) 255, (LPARAM) cTime) ;
					SendDlgItemMessage(hwnd, IDD_INCREMENT, WM_GETTEXT, (WPARAM) 255, (LPARAM) cInc) ;

					nR = SendDlgItemMessage(hwnd, IDD_RATED, BM_GETSTATE, 0, 0) ;
					nT = SendMessage(hType, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
					nC = SendMessage(hColor, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;

					TOOLBOX_AllTrim(cOpp) ;
					TOOLBOX_AllTrim(cTime) ;
					TOOLBOX_AllTrim(cInc) ;

					cOpp  [ICS_HANDLE_LENGTH] = NULL_CHAR ;
					cTime [4]                 = NULL_CHAR ;
					cInc  [3]                 = NULL_CHAR ;

					if(strlen(cOpp) == 0)
					{
						TOOLBOX_Error("Missing Opponent.") ;
						SetFocus(hOpponent) ;
						return FALSE ;
					}

					if(strlen(cTime) > 0)
					{
						nI = atoi(cTime) ;
						if(nI < 0 || nI > 9999)
						{
							TOOLBOX_Error("Initial Time is Out of Range (0 to 9999).") ;
							SetFocus(hInitial) ;
							return FALSE ;
						}
					}
					else
					{
						nI = 0 ;
					}

					if(strlen(cInc) > 0)
					{
						nJ = atoi(cInc) ;
						if(nJ < 0 || nJ > 999)
						{
							TOOLBOX_Error("Increment is Out of Range (0 to 999).") ;
							SetFocus(hIncrement) ;
							return FALSE ;
						}
					}
					else
					{
						nJ = 0 ;
					}

					strcpy(Match.cMatchHandle, cOpp) ;

					Match.nMatchTime      = nI ;
					Match.nMatchIncrement = nJ ;

					if(nR == BST_CHECKED)
					{
						Match.bMatchRated = 1 ;
					}
					else
					{
						Match.bMatchRated = 0 ;
					}

					if(Login.nLoginType == SERVER_ICC)
					{
						Match.nICCMatchType = nT ;
					}
					else
					{
						Match.nMatchType = nT ;
					}

					Match.nMatchColor = nC ;

					if(Login.nLoginType == SERVER_ICC)
					{
						sprintf(cTmp, "%s%s %d %d %c %s",
								ICS_MATCH_COMMAND,
								cOpp,
								Match.nMatchTime,
								Match.nMatchIncrement,
								Match.bMatchRated ? 'r' : 'u',
								_ICC_MtType [Match.nICCMatchType]) ;
					}
					else
					{
						sprintf(cTmp, "%s%s %d %d %c %s",
								ICS_MATCH_COMMAND,
								cOpp,
								Match.nMatchTime,
								Match.nMatchIncrement,
								Match.bMatchRated ? 'r' : 'u',
								_FICS_MtType [Match.nMatchType]) ;
					}

					if(strlen(_MT_Color [Match.nMatchColor]) > 0)
					{
						strcat(cTmp, " ") ;
						strcat(cTmp, _MT_Color [Match.nMatchColor]) ;
					}

					strcat(cTmp, "\n") ;

					if(Timeseal.bSocketIsOpen)
					{
						TOOLBOX_WriteICS(cTmp) ;
					}

					TOOLBOX_WriteUser(cTmp) ;

					EndDialog(hwnd, TRUE) ;

					if(User.bMatchCommandAddHist)
					{
						nI = strlen(cTmp) ;
						if(cTmp [nI - 1] == '\n')
						{
							cTmp [nI - 1] = NULL_CHAR ;
						}
						HISTORY_Add(cTmp) ;
					}

					INI_WriteSystem(INI_GetSysFilename()) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}

void MATCH_AdjustMatch(int nG, char *cO, int nT, int nI, int bR)
{
	strcpy(Match.cMatchHandle, cO) ;

	Match.nMatchTime      = nT ;
	Match.nMatchIncrement = nI ;
	Match.bMatchRated     = bR ;
	Match.nMatchType      = DEFAULT_MATCH_TYPE ;
	Match.nICCMatchType   = DEFAULT_ICC_MATCH_TYPE ;

	if(Login.nLoginType == SERVER_ICC)
	{
		switch(Game [nG].nGameType)
		{
			case GAMETYPE_ICC_BULLET :
				Match.nICCMatchType = 0 ;
				break ;

			case GAMETYPE_ICC_BLITZ :
				Match.nICCMatchType = 0 ;
				break ;

			case GAMETYPE_ICC_STANDARD :
				Match.nICCMatchType = 0 ;
				break ;

			case GAMETYPE_ICC_5_MINUTE :
				Match.nICCMatchType = 0 ;
				break ;

			case GAMETYPE_ICC_1_MINUTE :
				Match.nICCMatchType = 0 ;
				break ;

			default :
				if((Game [nG].nGameType >= 0) && (Game [nG].nGameType < (ICC_MATCH_TYPE - 1)))
				{
					Match.nICCMatchType = Game [nG].nGameType ;
				}
				else
				{
					Match.nICCMatchType = 0 ;
				}
				break ;
		}
	}
	else
	{
		switch(Game [nG].nGameType)
		{
			case GAMETYPE_BUGHOUSE :
				Match.nMatchType = 0 ;
				break ;

			case GAMETYPE_CRAZYHOUSE :
				Match.nMatchType = 1 ;
				break ;

			case GAMETYPE_FICS_LIGHTNING :
				Match.nMatchType = 2 ;
				break ;

			case GAMETYPE_FICS_BLITZ :
				Match.nMatchType = 2 ;
				break ;

			case GAMETYPE_FICS_STANDARD :
				Match.nMatchType = 2 ;
				break ;

			case GAMETYPE_SUICIDE :
				Match.nMatchType = 3 ;
				break ;

			case GAMETYPE_FICS_WILD0 :
				Match.nMatchType = 4 ;
				break ;

			case GAMETYPE_FICS_WILD1 :
				Match.nMatchType = 5 ;
				break ;

			case GAMETYPE_FICS_WILD2 :
				Match.nMatchType = 6 ;
				break ;

			case GAMETYPE_FICS_WILD3 :
				Match.nMatchType = 7 ;
				break ;

			case GAMETYPE_FICS_WILD4 :
				Match.nMatchType = 8 ;
				break ;

			case GAMETYPE_WILD5 :
				Match.nMatchType = 9 ;
				break ;

			case GAMETYPE_FICS_WILD8 :
				Match.nMatchType = 10 ;
				break ;

			case GAMETYPE_FICS_WILD8A :
				Match.nMatchType = 11 ;
				break ;

			case GAMETYPE_FICS_WILDFR :
				Match.nMatchType = 12 ;
				break ;

			case GAMETYPE_FICS_ATOMIC :
				Match.nMatchType = 13 ;
				break ;

			default :
				Match.nMatchType = 2 ;
				break ;
		}
	}
	Match.nMatchColor = 0 ;
}

void MATCH_AdjustValues(void)
{
	if(Match.nMatchTime < 0 || Match.nMatchTime > 9999)
	{
		Match.nMatchTime = DEFAULT_MATCH_TIME ;
	}

	if(Match.nMatchIncrement < 0 || Match.nMatchIncrement > 999)
	{
		Match.nMatchIncrement = DEFAULT_MATCH_INCREMENT ;
	}

	if(Match.bMatchRated)
	{
		Match.bMatchRated = 1 ;
	}
	else
	{
		Match.bMatchRated = 0 ;
	}

	if(Match.nMatchType < 0 || Match.nMatchType > (FICS_MATCH_TYPE - 1))
	{
		Match.nMatchType = DEFAULT_MATCH_TYPE ;
	}

	if(Match.nICCMatchType < 0 || Match.nICCMatchType > (ICC_MATCH_TYPE - 1))
	{
		Match.nICCMatchType = DEFAULT_ICC_MATCH_TYPE ;
	}

	if(Match.nMatchColor < 0 || Match.nMatchColor > 2)
	{
		Match.nMatchColor = DEFAULT_MATCH_COLOR ;
	}
}
