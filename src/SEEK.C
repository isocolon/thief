#include "thief.h"

#define ICC_SEEK_TYPE   29
#define FICS_SEEK_TYPE  13

char *ICC_SkType [ICC_SEEK_TYPE]  = { "Chess",
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
									  "Wild 25",
									  "Giveaway",
									  "Atomic",
									  "Wild 28",
									  "Wild 29"
									};

char *_ICC_SkType [ICC_SEEK_TYPE] = { "w0",
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
									  "w25",
									  "w26",
									  "w27",
									  "w28",
									  "w29"
									};

char *FICS_SkType [FICS_SEEK_TYPE]  = { "Crazyhouse",
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
									  };

char *_FICS_SkType [FICS_SEEK_TYPE] = { "zh",
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
									  };

char *SK_Color [3] = { "Auto",
					   "Black",
					   "White"
					 };

char *_SK_Color [3] = { "",
						"b",
						"w"
					  };

BOOL CALLBACK SeekBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hInitial, hIncrement, hRating1, hRating2, hType, hColor, hManual, hFormula, hRated;
	char cTmp [MSG_SIZE], cTmp1 [MSG_SIZE], cTime [MSG_SIZE], cInc [MSG_SIZE], cRate1 [MSG_SIZE], cRate2 [MSG_SIZE];
	int nCount, nI, nT, nC, nM, nF, nR, nJ, nK, nL;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			hInitial   = GetDlgItem(hwnd, IDD_INITIAL);
			hIncrement = GetDlgItem(hwnd, IDD_INCREMENT);
			hRating1   = GetDlgItem(hwnd, IDD_RATING1);
			hRating2   = GetDlgItem(hwnd, IDD_RATING2);
			hType      = GetDlgItem(hwnd, IDD_TYPE);
			hColor     = GetDlgItem(hwnd, IDD_COLOR);
			hManual    = GetDlgItem(hwnd, IDD_MANUAL);
			hFormula   = GetDlgItem(hwnd, IDD_FORMULA);
			hRated     = GetDlgItem(hwnd, IDD_RATED);

			sprintf(cTmp, "%d", Seek.nSeekTime);
			SendDlgItemMessage(hwnd, IDD_INITIAL, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp);

			sprintf(cTmp, "%d", Seek.nSeekIncrement);
			SendDlgItemMessage(hwnd, IDD_INCREMENT, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp);

			sprintf(cTmp, "%d", Seek.nSeekRating1);
			SendDlgItemMessage(hwnd, IDD_RATING1, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp);

			sprintf(cTmp, "%d", Seek.nSeekRating2);
			SendDlgItemMessage(hwnd, IDD_RATING2, WM_SETTEXT, (WPARAM) strlen(cTmp), (LPARAM) cTmp);

			SendMessage(hType, CB_RESETCONTENT, 0, 0);
			if(Login.nLoginType == SERVER_ICC)
			{
				nCount = ICC_SEEK_TYPE;
			}
			else
			{
				nCount = FICS_SEEK_TYPE;
			}

			for(nI = 0 ; nI < nCount ; nI++)
			{
				if(Login.nLoginType == SERVER_ICC)
				{
					SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) ICC_SkType [nI]);
				}
				else
				{
					SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) FICS_SkType [nI]);
				}
			}

			if(Login.nLoginType == SERVER_ICC)
			{
				if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) ICC_SkType [Seek.nICCSeekType]) == CB_ERR)
				{
					SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
					SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) ICC_SkType [0]);
				}
			}
			else
			{
				if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) FICS_SkType [Seek.nSeekType]) == CB_ERR)
				{
					SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
					SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) FICS_SkType [0]);
				}
			}

			SendMessage(hColor, CB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < 3 ; nI++)
			{
				SendMessage(hColor, CB_ADDSTRING, 0, (LPARAM) SK_Color [nI]);
			}

			if(SendMessage(hColor, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) SK_Color [Seek.nSeekColor]) == CB_ERR)
			{
				SendMessage(hColor, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hColor, WM_SETTEXT, (WPARAM) 0, (LPARAM) SK_Color [0]);
			}

			if(Seek.bSeekManual)
			{
				SendDlgItemMessage(hwnd, IDD_MANUAL, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_MANUAL, BM_SETCHECK, BST_UNCHECKED, 0);
			}

			if(Seek.bSeekFormula)
			{
				SendDlgItemMessage(hwnd, IDD_FORMULA, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_FORMULA, BM_SETCHECK, BST_UNCHECKED, 0);
			}

			if(Seek.bSeekRated)
			{
				SendDlgItemMessage(hwnd, IDD_RATED, BM_SETCHECK, BST_CHECKED, 0);
			}
			else
			{
				SendDlgItemMessage(hwnd, IDD_RATED, BM_SETCHECK, BST_UNCHECKED, 0);
			}

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					SendDlgItemMessage(hwnd, IDD_INITIAL,   WM_GETTEXT, (WPARAM) 255, (LPARAM) cTime);
					SendDlgItemMessage(hwnd, IDD_INCREMENT, WM_GETTEXT, (WPARAM) 255, (LPARAM) cInc);
					SendDlgItemMessage(hwnd, IDD_RATING1,   WM_GETTEXT, (WPARAM) 255, (LPARAM) cRate1);
					SendDlgItemMessage(hwnd, IDD_RATING2,   WM_GETTEXT, (WPARAM) 255, (LPARAM) cRate2);

					nT = SendMessage(hType, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					nC = SendMessage(hColor, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					nM = SendDlgItemMessage(hwnd, IDD_MANUAL, BM_GETSTATE, 0, 0);
					nF = SendDlgItemMessage(hwnd, IDD_FORMULA, BM_GETSTATE, 0, 0);
					nR = SendDlgItemMessage(hwnd, IDD_RATED, BM_GETSTATE, 0, 0);

					TOOLBOX_AllTrim(cTime);
					TOOLBOX_AllTrim(cInc);
					TOOLBOX_AllTrim(cRate1);
					TOOLBOX_AllTrim(cRate2);

					cTime  [4] = NULL_CHAR;
					cInc   [3] = NULL_CHAR;
					cRate1 [4] = NULL_CHAR;
					cRate2 [4] = NULL_CHAR;

					if(strlen(cTime) > 0)
					{
						nI = atoi(cTime);
						if(nI < 0 || nI > 9999)
						{
							TOOLBOX_Error("Initial Time is Out of Range (0 to 9999).");
							SetFocus(hInitial);
							return FALSE;
						}
					}
					else
					{
						nI = 0;
					}

					if(strlen(cInc) > 0)
					{
						nJ = atoi(cInc);
						if(nJ < 0 || nJ > 999)
						{
							TOOLBOX_Error("Increment is Out of Range (0 to 999).");
							SetFocus(hIncrement);
							return FALSE;
						}
					}
					else
					{
						nJ = 0;
					}

					if(strlen(cRate1) > 0)
					{
						nK = atoi(cRate1);
						if(nK < 0 || nK > 9999)
						{
							TOOLBOX_Error("Minimum Rating is Out of Range (0 to 9999).");
							SetFocus(hRating1);
							return FALSE;
						}
					}
					else
					{
						nK = 0;
					}

					if(strlen(cRate2) > 0)
					{
						nL = atoi(cRate2);
						if(nL < 0 || nL > 9999)
						{
							TOOLBOX_Error("Maximum Rating is Out of Range (0 to 9999).");
							SetFocus(hRating2);
							return FALSE;
						}
					}
					else
					{
						nL = 0;
					}

					Seek.nSeekTime      = nI;
					Seek.nSeekIncrement = nJ;
					Seek.nSeekRating1   = nK;
					Seek.nSeekRating2   = nL;

					if(Seek.nSeekRating1 > Seek.nSeekRating2)
					{
						nI                = Seek.nSeekRating2;
						Seek.nSeekRating2 = Seek.nSeekRating1;
						Seek.nSeekRating1 = nI;
					}

					if(Login.nLoginType == SERVER_ICC)
					{
						Seek.nICCSeekType = nT;
					}
					else
					{
						Seek.nSeekType = nT;
					}

					Seek.nSeekColor = nC;

					if(nM == BST_CHECKED)
					{
						Seek.bSeekManual = 1;
					}
					else
					{
						Seek.bSeekManual = 0;
					}

					if(nF == BST_CHECKED)
					{
						Seek.bSeekFormula = 1;
					}
					else
					{
						Seek.bSeekFormula = 0;
					}

					if(nR == BST_CHECKED)
					{
						Seek.bSeekRated = 1;
					}
					else
					{
						Seek.bSeekRated = 0;
					}

					if(Login.nLoginType == SERVER_ICC)
					{
						strcpy(cTmp1, _ICC_SkType [Seek.nICCSeekType]);
					}
					else
					{
						strcpy(cTmp1, _FICS_SkType [Seek.nSeekType]);
					}

					sprintf(cTmp, "%s%d %d %c",
							ICS_SEEK_COMMAND,
							Seek.nSeekTime,
							Seek.nSeekIncrement,
							Seek.bSeekRated ? 'r' : 'u');

					if(strlen(_SK_Color [Seek.nSeekColor]) > 0)
					{
						strcat(cTmp, " ");
						strcat(cTmp, _SK_Color [Seek.nSeekColor]);
					}

					strcat(cTmp, " ");
					strcat(cTmp, cTmp1);

					if(Seek.bSeekManual)
					{
						strcat(cTmp, " m");
					}

					if(Seek.bSeekFormula)
					{
						strcat(cTmp, " f");
					}

					if(Seek.nSeekRating1 == DEFAULT_SEEK_RATING1 &&
							Seek.nSeekRating2 == DEFAULT_SEEK_RATING2)
					{
					}
					else
					{
						sprintf(cTmp1, " %d-%d", Seek.nSeekRating1, Seek.nSeekRating2);
						strcat(cTmp, cTmp1);
					}

					strcat(cTmp, "\n");

					if(Timeseal.bSocketIsOpen)
					{
						TOOLBOX_WriteICS(cTmp);
					}

					TOOLBOX_WriteUser(cTmp);

					EndDialog(hwnd, TRUE);

					if(User.bSeekCommandAddHist)
					{
						nI = strlen(cTmp);
						if(cTmp [nI - 1] == '\n')
						{
							cTmp [nI - 1] = NULL_CHAR;
						}
						HISTORY_Add(cTmp);
					}

					INI_WriteSystem(INI_GetSysFilename());
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void SEEK_Adjust(void)
{
	int nI;

	if(Seek.nSeekTime < 0 || Seek.nSeekTime > 9999)
	{
		Seek.nSeekTime = DEFAULT_SEEK_TIME;
	}

	if(Seek.nSeekIncrement < 0 || Seek.nSeekIncrement > 999)
	{
		Seek.nSeekIncrement = DEFAULT_SEEK_INCREMENT;
	}

	if(Seek.nSeekRating1 < 0 || Seek.nSeekRating1 > 9999)
	{
		Seek.nSeekRating1 = DEFAULT_SEEK_RATING1;
	}

	if(Seek.nSeekRating2 < 0 || Seek.nSeekRating2 > 9999)
	{
		Seek.nSeekRating2 = DEFAULT_SEEK_RATING2;
	}

	if(Seek.nSeekRating1 > Seek.nSeekRating2)
	{
		nI                = Seek.nSeekRating2;
		Seek.nSeekRating2 = Seek.nSeekRating1;
		Seek.nSeekRating1 = nI;
	}

	if(Seek.nSeekType < 0 || Seek.nSeekType > (FICS_SEEK_TYPE - 1))
	{
		Seek.nSeekType = DEFAULT_SEEK_TYPE;
	}

	if(Seek.nICCSeekType < 0 || Seek.nICCSeekType > (ICC_SEEK_TYPE - 1))
	{
		Seek.nICCSeekType = DEFAULT_ICC_SEEK_TYPE;
	}

	if(Seek.nSeekColor < 0 || Seek.nSeekColor > 2)
	{
		Seek.nSeekColor = DEFAULT_SEEK_COLOR;
	}

	if(Seek.bSeekManual)
	{
		Seek.bSeekManual = 1;
	}
	else
	{
		Seek.bSeekManual = 0;
	}

	if(Seek.bSeekFormula)
	{
		Seek.bSeekFormula = 1;
	}
	else
	{
		Seek.bSeekFormula = 0;
	}

	if(Seek.bSeekRated)
	{
		Seek.bSeekRated = 1;
	}
	else
	{
		Seek.bSeekRated = 0;
	}
}
