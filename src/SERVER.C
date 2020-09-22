#include "thief.h"

char New_cName [MAX_SERVER] [2048];
char New_Server [MAX_SERVER] [MAX_SERVER_SIZE];
char New_Port [MAX_SERVER] [MAX_SERVER_PORT_SIZE];
char New_Prompt [MAX_SERVER] [MAX_SERVER_PROMPT_SIZE];
int New_Type [MAX_SERVER];

BOOL CALLBACK ServerBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hAddress, hPort, hPrompt, hType;
	static int nSel, nCount, nMode;
	int nI, nType;
	char cTmp [2048], cTmp1 [2048], cAddress [2048], cPort [2048], cPrompt [2048];

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			// get window
			hAddress = GetDlgItem(hwnd, IDD_SERVER_ADDRESS);
			hPort    = GetDlgItem(hwnd, IDD_SERVER_PORT);
			hPrompt  = GetDlgItem(hwnd, IDD_SERVER_PROMPT);
			hType    = GetDlgItem(hwnd, IDD_SERVER_TYPE);

			// save into temporary arrays
			for(nI = 0 ; nI < MAX_SERVER ; nI++)
			{
				strcpy(New_Server [nI], Server.cServer [nI]);
				strcpy(New_Port [nI], Server.cPort [nI]);
				strcpy(New_Prompt [nI], Server.cPrompt [nI]);
				New_Type [nI] = Server.nType [nI];
			}

			// show list
			nCount = 0;
			SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < MAX_SERVER ; nI++)
			{
				if(strlen(New_Server [nI]) == 0)
				{
					break;
				}

				nCount = nCount + 1;

				switch(New_Type [nI])
				{
					case SERVER_FICS :
						strcpy(cTmp1, "fics");
						break;

					case SERVER_ICC :
						strcpy(cTmp1, "icc");
						break;

					case SERVER_NONFICS :
						strcpy(cTmp1, "other");
						break;

					default :
						New_Type [nI] = SERVER_FICS;
						strcpy(cTmp1, "fics");
						break;
				}

				sprintf(cTmp, "%02d. %s %s [%s] (%s)", nI + 1, New_Server [nI], New_Port [nI], New_Prompt [nI], cTmp1);
				SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_ADDSTRING, 0, (LPARAM) cTmp);

				strcpy(New_cName [nI], cTmp);
			}
			SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);

			// show item zero
			SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) New_Server [0]);
			SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) New_Port   [0]);
			SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) New_Prompt [0]);

			SendMessage(hType, CB_RESETCONTENT, 0, 0);
			strcpy(cTmp, "fics");
			SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "icc");
			SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "other");
			SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

			switch(New_Type [0])
			{
				case SERVER_FICS :
					strcpy(cTmp, "fics");
					break;
				case SERVER_ICC  :
					strcpy(cTmp, "icc");
					break;
				case SERVER_NONFICS :
					strcpy(cTmp, "other");
					break;
				default :
					strcpy(cTmp, "fics");
					break;
			}
			if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			nSel = 0;

			// disable all input items
			EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
			EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
			EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
			EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

			// disable "new" button if there are already to the maximum items
			if(nCount >= MAX_SERVER)
			{
				EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
			}

			// disable "edit" and "delete" buttons if there are no items
			if(nCount <= 0)
			{
				EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
				EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
			}

			// assign input mode
			nMode = 0;

			// centralize window
			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_SERVER_DEFINE :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nSel = SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_GETCURSEL, 0, 0);

						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) New_Server [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) New_Port   [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) New_Prompt [nSel]);

						switch(New_Type [nSel])
						{
							case SERVER_FICS :
								strcpy(cTmp, "fics");
								break;
							case SERVER_ICC  :
								strcpy(cTmp, "icc");
								break;
							case SERVER_NONFICS :
								strcpy(cTmp, "other");
								break;
							default :
								strcpy(cTmp, "fics");
								break;
						}
						if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, (WPARAM) nSel, 0);
					}
					break;

				case IDD_SERVER_NEW :
					// disable list box
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DEFINE), 0);

					// hide "new", "edit", "delete", "default", "up", and "down" buttons
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_NEW),     SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),    SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE),  SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DEFAULT), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_UP),      SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DOWN),    SW_HIDE);

					// enable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 1);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    1);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  1);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    1);

					// assign input mode
					nMode = 1;
					break;

				case IDD_SERVER_EDIT :
					nCount = 0;
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						if(strlen(New_Server [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Server to Edit.");
						return FALSE;
					}

					// disable list box
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DEFINE), 0);

					// hide "new", "edit", "delete", "default", "up", and "down" buttons
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_NEW),     SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),    SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE),  SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DEFAULT), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_UP),      SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DOWN),    SW_HIDE);

					// enable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 1);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    1);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  1);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    1);

					// assign input mode
					nMode = 2;
					break;

				case IDD_SERVER_DELETE :
					nCount = 0;
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						if(strlen(New_Server [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Server to Delete.");
						return FALSE;
					}

					if(nSel >= nCount)
					{
						TOOLBOX_Error("No Server Selected.");
						return FALSE;
					}

					for(nI = nSel ; nI < (MAX_SERVER - 1) ; nI++)
					{
						strcpy(New_Server [nI], New_Server [nI + 1]);
						strcpy(New_Port   [nI], New_Port   [nI + 1]);
						strcpy(New_Prompt [nI], New_Prompt [nI + 1]);
						New_Type [nI] = New_Type [nI + 1];
					}

					// show list again
					nCount = 0;
					SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_RESETCONTENT, 0, 0);
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						if(strlen(New_Server [nI]) == 0)
						{
							break;
						}

						nCount = nCount + 1;

						switch(New_Type [nI])
						{
							case SERVER_FICS :
								strcpy(cTmp1, "fics");
								break;

							case SERVER_ICC :
								strcpy(cTmp1, "icc");
								break;

							case SERVER_NONFICS :
								strcpy(cTmp1, "other");
								break;

							default :
								New_Type [nI] = SERVER_FICS;
								strcpy(cTmp1, "fics");
								break;
						}

						sprintf(cTmp, "%02d. %s %s [%s] (%s)", nI + 1, New_Server [nI], New_Port [nI], New_Prompt [nI], cTmp1);
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(New_cName [nI], cTmp);
					}

					if(nCount > 0)
					{
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);

						// reset rest
						for(nI = nCount ; nI < MAX_SERVER ; nI++)
						{
							strcpy(New_Server [nI], "");
							strcpy(New_Port [nI], "");
							strcpy(New_Prompt [nI], "");
							New_Type [nI] = SERVER_FICS;
						}

						if(nSel >= nCount)
						{
							nSel = (nCount - 1);
						}

						// select item
						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) New_Server [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) New_Port   [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) New_Prompt [nSel]);

						SendMessage(hType, CB_RESETCONTENT, 0, 0);
						strcpy(cTmp, "fics");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(cTmp, "icc");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(cTmp, "other");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						switch(New_Type [nSel])
						{
							case SERVER_FICS :
								strcpy(cTmp, "fics");
								break;
							case SERVER_ICC  :
								strcpy(cTmp, "icc");
								break;
							case SERVER_NONFICS :
								strcpy(cTmp, "other");
								break;
							default :
								strcpy(cTmp, "fics");
								break;
						}
						if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						// select item
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, (WPARAM) nSel, 0);
					}
					else
					{
						nSel = 0;

						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);
#ifndef KICS
						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) "freechess.org");
						SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) "5000");
						SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) FICS_PROMPT);
#endif

#ifdef KICS
						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) "kics.freechess.org");
						SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) "6000");
						SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) FICS_PROMPT);
#endif


						strcpy(cTmp, "fics");
						if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}
					}

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_SERVER)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDD_SERVER_DEFAULT :
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						strcpy(New_Server [nI], "");
						strcpy(New_Port [nI], "");
						strcpy(New_Prompt [nI], "");
						New_Type [0] = SERVER_FICS;
					}

#ifndef KICS
					strcpy(New_Server [ 0], "freechess.org");
					strcpy(New_Port   [ 0], "5000");
					strcpy(New_Prompt [ 0], FICS_PROMPT);
#endif

#ifdef KICS
					strcpy(New_Server [ 0], "kics.freechess.org");
					strcpy(New_Port   [ 0], "6000");
					strcpy(New_Prompt [ 0], FICS_PROMPT);
#endif

					New_Type [ 0] = SERVER_FICS;

					strcpy(New_Server [ 1], "freechess.org");
					strcpy(New_Port   [ 1], "23");
					strcpy(New_Prompt [ 1], FICS_PROMPT);
					New_Type [ 1] = SERVER_FICS;

					strcpy(New_Server [ 2], "global.chessparlor.com");
					strcpy(New_Port   [ 2], "6000");
					strcpy(New_Prompt [ 2], FICS_PROMPT);
					New_Type [ 2] = SERVER_FICS;

					strcpy(New_Server [ 3], "chessclub.com");
					strcpy(New_Port   [ 3], "5000");
					strcpy(New_Prompt [ 3], ICC_PROMPT);
					New_Type [ 3] = SERVER_ICC;

					strcpy(New_Server [ 4], "chessclub.com");
					strcpy(New_Port   [ 4], "23");
					strcpy(New_Prompt [ 4], ICC_PROMPT);
					New_Type [ 4] = SERVER_ICC;

					strcpy(New_Server [ 5], "queen.chessclub.com");
					strcpy(New_Port   [ 5], "5000");
					strcpy(New_Prompt [ 5], ICC_PROMPT);
					New_Type [ 5] = SERVER_ICC;

					strcpy(New_Server [ 6], "chess-square.com");
					strcpy(New_Port   [ 6], "5000");
					strcpy(New_Prompt [ 6], "fics% ");
					New_Type [ 6] = SERVER_NONFICS;

					strcpy(New_Server [ 7], "zics.org");
					strcpy(New_Port   [ 7], "5000");
					strcpy(New_Prompt [ 7], "zics% ");
					New_Type [ 7] = SERVER_NONFICS;

					strcpy(New_Server [ 8], "chess.unix-ag.uni-kl.de");
					strcpy(New_Port   [ 8], "5000");
					strcpy(New_Prompt [ 8], "gics% ");
					New_Type [ 8] = SERVER_NONFICS;

					strcpy(New_Server [ 9], "chess.tridgell.net");
					strcpy(New_Port   [ 9], "5000");
					strcpy(New_Prompt [ 9], "fics% ");
					New_Type [ 9] = SERVER_NONFICS;

					strcpy(New_Server [10], "ajedrez.cec.uchile.cl");
					strcpy(New_Port   [10], "5000");
					strcpy(New_Prompt [10], "fics% ");
					New_Type [10] = SERVER_NONFICS;

					strcpy(New_Server [11], "chess.mds.mdh.se");
					strcpy(New_Port   [11], "5000");
					strcpy(New_Prompt [11], "sics% ");
					New_Type [11] = SERVER_NONFICS;

					strcpy(New_Server [12], "chess.mds.mdh.se");
					strcpy(New_Port   [12], "5555");
					strcpy(New_Prompt [12], "fics% ");
					New_Type [12] = SERVER_NONFICS;

//                  strcpy (New_Server [13], "europe.freechess.org");
//                  strcpy (New_Port   [13], "5000");
//                  strcpy (New_Prompt [13], FICS_PROMPT);
//                  New_Type [13] = SERVER_FICS;
//
//                  strcpy (New_Server [14], "www.freechess.ru");
//                  strcpy (New_Port   [14], "5000");
//                  strcpy (New_Prompt [14], "fics% ");
//                  New_Type [14] = SERVER_NONFICS;
//
//                  strcpy (New_Server [15], "crocus.warwick.ac.uk");
//                  strcpy (New_Port   [15], "5000");
//                  strcpy (New_Prompt [15], "fics% ");
//                  New_Type [15] = SERVER_NONFICS;
//
//                  strcpy (New_Server [16], "fly.cc.fer.hr");
//                  strcpy (New_Port   [16], "7890");
//                  strcpy (New_Prompt [16], "fics% ");
//                  New_Type [16] = SERVER_NONFICS;
//
//                  strcpy (New_Server [17], "dics.dds.nl");
//                  strcpy (New_Port   [17], "5000");
//                  strcpy (New_Prompt [17], "fics% ");
//                  New_Type [17] = SERVER_NONFICS;
//
//                  strcpy (New_Server [18], "chess.eerie.fr");
//                  strcpy (New_Port   [18], "5000");
//                  strcpy (New_Prompt [18], "fics% ");
//                  New_Type [18] = SERVER_NONFICS;
//
//                  strcpy (New_Server [19], "callisto.si.usherb.ca");
//                  strcpy (New_Port   [19], "5000");
//                  strcpy (New_Prompt [19], "fics% ");
//                  New_Type [19] = SERVER_NONFICS;
//
//                  strcpy (New_Server [20], "wisdom.weizmann.ac.il");
//                  strcpy (New_Port   [20], "5000");
//                  strcpy (New_Prompt [20], "fics% ");
//                  New_Type [20] = SERVER_NONFICS;
//
//                  strcpy (New_Server [21], "chess.aaum.pt");
//                  strcpy (New_Port   [21], "5000");
//                  strcpy (New_Prompt [21], "fics% ");
//                  New_Type [21] = SERVER_NONFICS;
//
//                  strcpy (New_Server [22], "vics.ver.ucc.mx");
//                  strcpy (New_Port   [22], "5000");
//                  strcpy (New_Prompt [22], "fics% ");
//                  New_Type [22] = SERVER_NONFICS;
//
//                  strcpy (New_Server [23], "ics.interdomain.net.au");
//                  strcpy (New_Port   [23], "5000");
//                  strcpy (New_Prompt [23], "fics% ");
//                  New_Type [23] = SERVER_NONFICS;
//
//                  strcpy (New_Server [24], "chess.anu.edu.au");
//                  strcpy (New_Port   [24], "5000");
//                  strcpy (New_Prompt [24], "fics% ");
//                  New_Type [24] = SERVER_NONFICS;
//
//                  strcpy (New_Server [25], "fics.infcom.it");
//                  strcpy (New_Port   [25], "5000");
//                  strcpy (New_Prompt [25], "fics% ");
//                  New_Type [25] = SERVER_NONFICS;
//
//                  strcpy (New_Server [26], "cygnus.csa.iisc.ernet.in");
//                  strcpy (New_Port   [26], "5000");
//                  strcpy (New_Prompt [26], "fics% ");
//                  New_Type [26] = SERVER_NONFICS;
//
//                  strcpy (New_Server [27], "mezquite.iico.uaslp.mx");
//                  strcpy (New_Port   [27], "5000");
//                  strcpy (New_Prompt [27], "fics% ");
//                  New_Type [27] = SERVER_NONFICS;
//
//                  strcpy (New_Server [28], "gsi.gsini.net");
//                  strcpy (New_Port   [28], "5000");
//                  strcpy (New_Prompt [28], "fics% ");
//                  New_Type [28] = SERVER_NONFICS;
//
//                  strcpy (New_Server [29], "rogue.coe.ohio-state.edu");
//                  strcpy (New_Port   [29], "5000");
//                  strcpy (New_Prompt [29], "fics% ");
//                  New_Type [29] = SERVER_NONFICS;
//
//                  strcpy (New_Server [30], "yakko.cs.wmich.edu");
//                  strcpy (New_Port   [30], "5000");
//                  strcpy (New_Prompt [30], "fics% ");
//                  New_Type [30] = SERVER_NONFICS;
//
//                  strcpy (New_Server [31], "nebin.eurecom.fr");
//                  strcpy (New_Port   [31], "5000");
//                  strcpy (New_Prompt [31], "fics% ");
//                  New_Type [31] = SERVER_NONFICS;

					nCount = 0;
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						if(strlen(New_Server [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					// show list again
					nCount = 0;
					SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_RESETCONTENT, 0, 0);
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						if(strlen(New_Server [nI]) == 0)
						{
							break;
						}

						nCount = nCount + 1;

						switch(New_Type [nI])
						{
							case SERVER_FICS :
								strcpy(cTmp1, "fics");
								break;

							case SERVER_ICC :
								strcpy(cTmp1, "icc");
								break;

							case SERVER_NONFICS :
								strcpy(cTmp1, "other");
								break;

							default :
								New_Type [nI] = SERVER_FICS;
								strcpy(cTmp1, "fics");
								break;
						}

						sprintf(cTmp, "%02d. %s %s [%s] (%s)", nI + 1, New_Server [nI], New_Port [nI], New_Prompt [nI], cTmp1);
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(New_cName [nI], cTmp);
					}
					SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);

					// reset rest
					for(nI = nCount ; nI < MAX_SERVER ; nI++)
					{
						strcpy(New_Server [nI], "");
						strcpy(New_Port [nI], "");
						strcpy(New_Prompt [nI], "");
						New_Type [nI] = SERVER_FICS;
					}

					// select item zero
					SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) New_Server [0]);
					SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) New_Port   [0]);
					SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) New_Prompt [0]);

					SendMessage(hType, CB_RESETCONTENT, 0, 0);
					strcpy(cTmp, "fics");
					SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

					strcpy(cTmp, "icc");
					SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

					strcpy(cTmp, "other");
					SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

					switch(New_Type [0])
					{
						case SERVER_FICS :
							strcpy(cTmp, "fics");
							break;
						case SERVER_ICC  :
							strcpy(cTmp, "icc");
							break;
						case SERVER_NONFICS :
							strcpy(cTmp, "other");
							break;
						default :
							strcpy(cTmp, "fics");
							break;
					}
					if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					nSel = 0;

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_SERVER)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDD_SERVER_UP :
					nCount = 0;
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						if(strlen(New_Server [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Server to Move.");
						return FALSE;
					}

					if(nSel > 0)
					{
						strcpy(cAddress, New_Server [nSel]);
						strcpy(cPort,    New_Port   [nSel]);
						strcpy(cPrompt,  New_Prompt [nSel]);
						nType = New_Type [nSel];

						strcpy(New_Server [nSel], New_Server [nSel - 1]);
						strcpy(New_Port   [nSel], New_Port   [nSel - 1]);
						strcpy(New_Prompt [nSel], New_Prompt [nSel - 1]);
						New_Type [nSel] = New_Type [nSel - 1];

						strcpy(New_Server [nSel - 1], cAddress);
						strcpy(New_Port   [nSel - 1], cPort);
						strcpy(New_Prompt [nSel - 1], cPrompt);
						New_Type [nSel - 1] = nType;

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							if(strlen(New_Server [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							switch(New_Type [nI])
							{
								case SERVER_FICS :
									strcpy(cTmp1, "fics");
									break;

								case SERVER_ICC :
									strcpy(cTmp1, "icc");
									break;

								case SERVER_NONFICS :
									strcpy(cTmp1, "other");
									break;

								default :
									New_Type [nI] = SERVER_FICS;
									strcpy(cTmp1, "fics");
									break;
							}

							sprintf(cTmp, "%02d. %s %s [%s] (%s)", nI + 1, New_Server [nI], New_Port [nI], New_Prompt [nI], cTmp1);
							SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(New_cName [nI], cTmp);
						}

						nSel = nSel - 1;
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, (WPARAM) nSel, 0);
					}

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_SERVER)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDD_SERVER_DOWN :
					nCount = 0;
					for(nI = 0 ; nI < MAX_SERVER ; nI++)
					{
						if(strlen(New_Server [nI]) == 0)
						{
							break;
						}
						nCount = nCount + 1;
					}

					if(nCount <= 0)
					{
						TOOLBOX_Error("No Server to Move.");
						return FALSE;
					}

					if((nSel + 1) < nCount)
					{
						strcpy(cAddress, New_Server [nSel + 1]);
						strcpy(cPort,    New_Port   [nSel + 1]);
						strcpy(cPrompt,  New_Prompt [nSel + 1]);
						nType = New_Type [nSel + 1];

						strcpy(New_Server [nSel + 1], New_Server [nSel]);
						strcpy(New_Port   [nSel + 1], New_Port   [nSel]);
						strcpy(New_Prompt [nSel + 1], New_Prompt [nSel]);
						New_Type [nSel + 1] = New_Type [nSel];

						strcpy(New_Server [nSel], cAddress);
						strcpy(New_Port   [nSel], cPort);
						strcpy(New_Prompt [nSel], cPrompt);
						New_Type [nSel] = nType;

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							if(strlen(New_Server [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							switch(New_Type [nI])
							{
								case SERVER_FICS :
									strcpy(cTmp1, "fics");
									break;

								case SERVER_ICC :
									strcpy(cTmp1, "icc");
									break;

								case SERVER_NONFICS :
									strcpy(cTmp1, "other");
									break;

								default :
									New_Type [nI] = SERVER_FICS;
									strcpy(cTmp1, "fics");
									break;
							}

							sprintf(cTmp, "%02d. %s %s [%s] (%s)", nI + 1, New_Server [nI], New_Port [nI], New_Prompt [nI], cTmp1);
							SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(New_cName [nI], cTmp);
						}

						nSel = nSel + 1;
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, (WPARAM) nSel, 0);
					}

					// disable all input items
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
					EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

					// disable "new" button if there are already to the maximum items
					if(nCount >= MAX_SERVER)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 1);
					}

					// disable "edit" and "delete" buttons if there are no items
					if(nCount <= 0)
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   1);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 1);
					}

					// assign input mode
					nMode = 0;
					break;

				case IDOK :
					if(nMode == 1)
					{
						nCount = 0;
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							if(strlen(New_Server [nI]) == 0)
							{
								break;
							}
							nCount = nCount + 1;
						}

						if(nCount >= MAX_SERVER)
						{
							sprintf(cTmp, "You Have Reached The Maximum Number of Servers (%d).", MAX_SERVER);
							TOOLBOX_Error(cTmp);
							return FALSE;
						}

						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_GETTEXT, (WPARAM) MAX_SERVER_SIZE - 1, (LPARAM) cTmp);
						TOOLBOX_AllTrim(cTmp);
						if(strlen(cTmp) == 0)
						{
							TOOLBOX_Error("Missing Address.");
							SetFocus(hAddress);
							return FALSE;
						}
						strcpy(cAddress, cTmp);

						SendDlgItemMessage(hwnd, IDD_SERVER_PORT, WM_GETTEXT, (WPARAM) MAX_SERVER_SIZE - 1, (LPARAM) cTmp);
						TOOLBOX_AllTrim(cTmp);
						cTmp [MAX_SERVER_PORT_SIZE - 1] = NULL_CHAR;
						if(strlen(cTmp) == 0)
						{
							TOOLBOX_Error("Missing Port.");
							SetFocus(hPort);
							return FALSE;
						}
						strcpy(cPort, cTmp);

						nType = SendMessage(hType, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nType == CB_ERR)
						{
							TOOLBOX_Error("Invalid Type.");
							SetFocus(hType);
							return FALSE;
						}

						switch(nType)
						{
							case 0 :
								strcpy(cPrompt, FICS_PROMPT);
								break;

							case 1 :
								strcpy(cPrompt, ICC_PROMPT);
								break;

							default :
								SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT, WM_GETTEXT, (WPARAM) MAX_SERVER_PROMPT_SIZE - 1, (LPARAM) cTmp);
								if(strlen(cTmp) == 0)
								{
									TOOLBOX_Error("Missing Prompt.");
									SetFocus(hPrompt);
									return FALSE;
								}
								cTmp [MAX_SERVER_PROMPT_SIZE - 1] = NULL_CHAR;
								strcpy(cPrompt, cTmp);
								break;
						}

						// add it into the array
						strcpy(New_Server [nCount], cAddress);
						strcpy(New_Port [nCount], cPort);
						strcpy(New_Prompt [nCount], cPrompt);
						New_Type [nCount] = nType;

						nCount = nCount + 1;

						// reset rest
						for(nI = nCount ; nI < MAX_SERVER ; nI++)
						{
							strcpy(New_Server [nI], "");
							strcpy(New_Port [nI], "");
							strcpy(New_Prompt [nI], "");
							New_Type [nI] = SERVER_FICS;
						}

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							if(strlen(New_Server [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							switch(New_Type [nI])
							{
								case SERVER_FICS :
									strcpy(cTmp1, "fics");
									break;

								case SERVER_ICC :
									strcpy(cTmp1, "icc");
									break;

								case SERVER_NONFICS :
									strcpy(cTmp1, "other");
									break;

								default :
									New_Type [nI] = SERVER_FICS;
									strcpy(cTmp1, "fics");
									break;
							}

							sprintf(cTmp, "%02d. %s %s [%s] (%s)", nI + 1, New_Server [nI], New_Port [nI], New_Prompt [nI], cTmp1);
							SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(New_cName [nI], cTmp);
						}
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);

						nSel = nCount - 1;

						// select just added
						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) New_Server [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) New_Port   [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) New_Prompt [nSel]);

						SendMessage(hType, CB_RESETCONTENT, 0, 0);
						strcpy(cTmp, "fics");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(cTmp, "icc");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(cTmp, "other");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						switch(New_Type [nSel])
						{
							case SERVER_FICS :
								strcpy(cTmp, "fics");
								break;
							case SERVER_ICC  :
								strcpy(cTmp, "icc");
								break;
							case SERVER_NONFICS :
								strcpy(cTmp, "other");
								break;
							default :
								strcpy(cTmp, "fics");
								break;
						}
						if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						// select item
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, (WPARAM) nSel, 0);

						// disable all input items
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

						// enable list box
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DEFINE), 1);

						// show "new", "edit", "delete", "default", "up", and "down" buttons
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_NEW),     SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),    SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE),  SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DEFAULT), SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_UP),      SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DOWN),    SW_SHOW);

						// disable "new" button if there are already to the maximum items
						if(nCount >= MAX_SERVER)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 1);
						}

						// disable "edit" and "delete" buttons if there are no items
						if(nCount <= 0)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   1);
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 1);
						}

						// assign input mode
						nMode = 0;
					}
					else if(nMode == 2)
					{
						nSel = SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_GETCURSEL, 0, 0);

						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_GETTEXT, (WPARAM) MAX_SERVER_SIZE - 1, (LPARAM) cTmp);
						TOOLBOX_AllTrim(cTmp);
						if(strlen(cTmp) == 0)
						{
#ifndef KICS
							strcpy(cTmp, "freechess.org");
#endif

#ifdef KICS
							strcpy(cTmp, "kics.freechess.org");
#endif
						}
						strcpy(cAddress, cTmp);

						SendDlgItemMessage(hwnd, IDD_SERVER_PORT, WM_GETTEXT, (WPARAM) MAX_SERVER_SIZE - 1, (LPARAM) cTmp);
						TOOLBOX_AllTrim(cTmp);
						if(strlen(cTmp) == 0)
						{
#ifndef KICS
							strcpy(cTmp, "5000");
#endif

#ifdef KICS
							strcpy(cTmp, "6000");
#endif
						}
						cTmp [MAX_SERVER_PORT_SIZE - 1] = NULL_CHAR;
						strcpy(cPort, cTmp);

						nType = SendMessage(hType, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nType == CB_ERR)
						{
#ifndef KICS
							strcpy(cPrompt, "%fics ");
#endif

#ifndef FICS
							strcpy(cPrompt, "kics% ");
#endif
						}
						else
						{
							switch(nType)
							{
								case 0 :
									strcpy(cPrompt, FICS_PROMPT);
									break;

								case 1 :
									strcpy(cPrompt, ICC_PROMPT);
									break;

								default :
									SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT, WM_GETTEXT, (WPARAM) MAX_SERVER_PROMPT_SIZE - 1, (LPARAM) cTmp);
									if(strlen(cTmp) == 0)
									{
										strcpy(cTmp, FICS_PROMPT);
									}
									cTmp [MAX_SERVER_PROMPT_SIZE - 1] = NULL_CHAR;
									strcpy(cPrompt, cTmp);
									break;
							}
						}

						// update array
						strcpy(New_Server [nSel], cAddress);
						strcpy(New_Port [nSel],   cPort);
						strcpy(New_Prompt [nSel], cPrompt);
						New_Type [nSel] = nType;

						switch(New_Type [nSel])
						{
							case SERVER_FICS :
								strcpy(cTmp1, "fics");
								break;

							case SERVER_ICC :
								strcpy(cTmp1, "icc");
								break;

							case SERVER_NONFICS :
								strcpy(cTmp1, "other");
								break;

							default :
								New_Type [nSel] = SERVER_FICS;
								strcpy(cTmp1, "fics");
								break;
						}

						// show list again
						nCount = 0;
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_RESETCONTENT, 0, 0);
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							if(strlen(New_Server [nI]) == 0)
							{
								break;
							}

							nCount = nCount + 1;

							switch(New_Type [nI])
							{
								case SERVER_FICS :
									strcpy(cTmp1, "fics");
									break;

								case SERVER_ICC :
									strcpy(cTmp1, "icc");
									break;

								case SERVER_NONFICS :
									strcpy(cTmp1, "other");
									break;

								default :
									New_Type [nI] = SERVER_FICS;
									strcpy(cTmp1, "fics");
									break;
							}

							sprintf(cTmp, "%02d. %s %s [%s] (%s)", nI + 1, New_Server [nI], New_Port [nI], New_Prompt [nI], cTmp1);
							SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(New_cName [nI], cTmp);
						}
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);

						// select just added
						SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) New_Server [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) New_Port   [nSel]);
						SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) New_Prompt [nSel]);

						SendMessage(hType, CB_RESETCONTENT, 0, 0);
						strcpy(cTmp, "fics");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(cTmp, "icc");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						strcpy(cTmp, "other");
						SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

						switch(New_Type [nSel])
						{
							case SERVER_FICS :
								strcpy(cTmp, "fics");
								break;
							case SERVER_ICC  :
								strcpy(cTmp, "icc");
								break;
							case SERVER_NONFICS :
								strcpy(cTmp, "other");
								break;
							default :
								strcpy(cTmp, "fics");
								break;
						}
						if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
							SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
						}

						// select item
						SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, (WPARAM) nSel, 0);

						// disable all input items
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

						// enable list box
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DEFINE), 1);

						// show "new", "edit", "delete", "default", "up", and "down" buttons
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_NEW),     SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),    SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE),  SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DEFAULT), SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_UP),      SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DOWN),    SW_SHOW);

						// disable "new" button if there are already to the maximum items
						if(nCount >= MAX_SERVER)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 1);
						}

						// disable "edit" and "delete" buttons if there are no items
						if(nCount <= 0)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   1);
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 1);
						}

						// assign input mode
						nMode = 0;
					}
					else
					{

						// adjust array
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							strcpy(Server.cServer [nI], New_Server [nI]);
							TOOLBOX_AllTrim(Server.cServer [nI]);

							strcpy(Server.cPort [nI], New_Port [nI]);
							TOOLBOX_AllTrim(Server.cPort [nI]);

							strcpy(Server.cPrompt [nI], New_Prompt [nI]);

							Server.nType [nI] = New_Type [nI];
						}

						SERVER_Adjust();
						INI_WriteSystem(INI_GetSysFilename());

						EndDialog(hwnd, TRUE);
						STATE_LeaveDialogBox();
						return TRUE;
					}
					break;

				case IDCANCEL :
					if(nMode == 0)
					{
						EndDialog(hwnd, FALSE);
						STATE_LeaveDialogBox();
						return TRUE;
					}
					else
					{
						nCount = 0;
						for(nI = 0 ; nI < MAX_SERVER ; nI++)
						{
							if(strlen(New_Server [nI]) == 0)
							{
								break;
							}
							nCount = nCount + 1;
						}

						if(nCount > 0)
						{
							SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);

							// reset rest
							for(nI = nCount ; nI < MAX_SERVER ; nI++)
							{
								strcpy(New_Server [nI], "");
								strcpy(New_Port [nI], "");
								strcpy(New_Prompt [nI], "");
								New_Type [nI] = SERVER_FICS;
							}

							if(nSel >= nCount)
							{
								nSel = (nCount - 1);
							}

							// select item
							SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) New_Server [nSel]);
							SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) New_Port   [nSel]);
							SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) New_Prompt [nSel]);

							SendMessage(hType, CB_RESETCONTENT, 0, 0);
							strcpy(cTmp, "fics");
							SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(cTmp, "icc");
							SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

							strcpy(cTmp, "other");
							SendMessage(hType, CB_ADDSTRING, 0, (LPARAM) cTmp);

							switch(New_Type [nSel])
							{
								case SERVER_FICS :
									strcpy(cTmp, "fics");
									break;
								case SERVER_ICC  :
									strcpy(cTmp, "icc");
									break;
								case SERVER_NONFICS :
									strcpy(cTmp, "other");
									break;
								default :
									strcpy(cTmp, "fics");
									break;
							}
							if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
							{
								SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
								SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
							}

							// select item
							SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, (WPARAM) nSel, 0);
						}
						else
						{
							nSel = 0;

							SendDlgItemMessage(hwnd, IDD_SERVER_DEFINE, LB_SETCURSEL, 0, 0);
#ifndef KICS
							SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) "freechess.org");
							SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) "5000");
							SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) "fics% ");
#endif

#ifdef KICS
							SendDlgItemMessage(hwnd, IDD_SERVER_ADDRESS, WM_SETTEXT, 0, (LPARAM) "kics.freechess.org");
							SendDlgItemMessage(hwnd, IDD_SERVER_PORT,    WM_SETTEXT, 0, (LPARAM) "6000");
							SendDlgItemMessage(hwnd, IDD_SERVER_PROMPT,  WM_SETTEXT, 0, (LPARAM) "kics% ");
#endif

							strcpy(cTmp, "fics");
							if(SendMessage(hType, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
							{
								SendMessage(hType, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
								SendMessage(hType, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
							}
						}

						// disable all input items
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_ADDRESS), 0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PORT),    0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_PROMPT),  0);
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_TYPE),    0);

						// enable list box
						EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DEFINE), 1);

						// show "new", "edit", "delete", "default", "up", and "down" buttons
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_NEW),     SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),    SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE),  SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DEFAULT), SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_UP),      SW_SHOW);
						ShowWindow(GetDlgItem(hwnd, IDD_SERVER_DOWN),    SW_SHOW);

						// disable "new" button if there are already to the maximum items
						if(nCount >= MAX_SERVER)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_NEW), 1);
						}

						// disable "edit" and "delete" buttons if there are no items
						if(nCount <= 0)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   0);
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 0);
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_EDIT),   1);
							EnableWindow(GetDlgItem(hwnd, IDD_SERVER_DELETE), 1);
						}

						// assign input mode
						nMode = 0;
					}
					break;
			}
			break;
	}
	return FALSE;
}

void SERVER_Init(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_SERVER ; nI++)
	{
		strcpy(Server.cServer [nI], "");
		strcpy(Server.cPort [nI], "");
		strcpy(Server.cPrompt [nI], "");
		Server.nType [0] = SERVER_FICS;
	}

#ifndef KICS
	strcpy(Server.cServer [ 0], "freechess.org");
	strcpy(Server.cPort   [ 0], "5000");
	strcpy(Server.cPrompt [ 0], FICS_PROMPT);
	Server.nType [ 0] = SERVER_FICS;
#endif

#ifdef KICS
	strcpy(Server.cServer [ 0], "kics.freechess.org");
	strcpy(Server.cPort   [ 0], "6000");
	strcpy(Server.cPrompt [ 0], FICS_PROMPT);
	Server.nType [ 0] = SERVER_FICS;
#endif

	strcpy(Server.cServer [ 1], "freechess.org");
	strcpy(Server.cPort   [ 1], "23");
	strcpy(Server.cPrompt [ 1], FICS_PROMPT);
	Server.nType [ 1] = SERVER_FICS;

	strcpy(Server.cServer [ 2], "chessclub.com");
	strcpy(Server.cPort   [ 2], "5000");
	strcpy(Server.cPrompt [ 2], ICC_PROMPT);
	Server.nType [ 2] = SERVER_ICC;

	strcpy(Server.cServer [ 3], "chessclub.com");
	strcpy(Server.cPort   [ 3], "23");
	strcpy(Server.cPrompt [ 3], ICC_PROMPT);
	Server.nType [ 3] = SERVER_ICC;

	strcpy(Server.cServer [ 4], "queen.chessclub.com");
	strcpy(Server.cPort   [ 4], "5000");
	strcpy(Server.cPrompt [ 4], ICC_PROMPT);
	Server.nType [ 4] = SERVER_ICC;

	strcpy(Server.cServer [ 5], "chess-square.com");
	strcpy(Server.cPort   [ 5], "5000");
	strcpy(Server.cPrompt [ 5], "fics% ");
	Server.nType [ 5] = SERVER_NONFICS;

	strcpy(Server.cServer [ 6], "zics.org");
	strcpy(Server.cPort   [ 6], "5000");
	strcpy(Server.cPrompt [ 6], "zics% ");
	Server.nType [ 6] = SERVER_NONFICS;

	strcpy(Server.cServer [ 7], "chess.unix-ag.uni-kl.de");
	strcpy(Server.cPort   [ 7], "5000");
	strcpy(Server.cPrompt [ 7], "gics% ");
	Server.nType [ 7] = SERVER_NONFICS;

	strcpy(Server.cServer [ 8], "chess.tridgell.net");
	strcpy(Server.cPort   [ 8], "5000");
	strcpy(Server.cPrompt [ 8], "fics% ");
	Server.nType [ 8] = SERVER_NONFICS;

	strcpy(Server.cServer [9], "ajedrez.cec.uchile.cl");
	strcpy(Server.cPort   [9], "5000");
	strcpy(Server.cPrompt [9], "fics% ");
	Server.nType [9] = SERVER_NONFICS;

//	  strcpy (Server.cServer [ 2], "global.chessparlor.com");
//    strcpy (Server.cPort   [ 2], "6000");
//    strcpy (Server.cPrompt [ 2], FICS_PROMPT);
//    Server.nType [ 2] = SERVER_FICS;

//    strcpy (Server.cServer [11], "chess.mds.mdh.se");
//    strcpy (Server.cPort   [11], "5000");
//    strcpy (Server.cPrompt [11], "sics% ");
//    Server.nType [11] = SERVER_NONFICS;

//    strcpy (Server.cServer [12], "chess.mds.mdh.se");
//    strcpy (Server.cPort   [12], "5555");
//    strcpy (Server.cPrompt [12], "fics% ");
//    Server.nType [12] = SERVER_NONFICS;

//  strcpy (Server.cServer [13], "europe.freechess.org");
//  strcpy (Server.cPort   [13], "5000");
//  strcpy (Server.cPrompt [13], FICS_PROMPT);
//  Server.nType [13] = SERVER_FICS;
//
//  strcpy (Server.cServer [14], "www.freechess.ru");
//  strcpy (Server.cPort   [14], "5000");
//  strcpy (Server.cPrompt [14], "fics% ");
//  Server.nType [14] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [15], "crocus.warwick.ac.uk");
//  strcpy (Server.cPort   [15], "5000");
//  strcpy (Server.cPrompt [15], "fics% ");
//  Server.nType [15] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [16], "fly.cc.fer.hr");
//  strcpy (Server.cPort   [16], "7890");
//  strcpy (Server.cPrompt [16], "fics% ");
//  Server.nType [16] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [17], "dics.dds.nl");
//  strcpy (Server.cPort   [17], "5000");
//  strcpy (Server.cPrompt [17], "fics% ");
//  Server.nType [17] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [18], "chess.eerie.fr");
//  strcpy (Server.cPort   [18], "5000");
//  strcpy (Server.cPrompt [18], "fics% ");
//  Server.nType [18] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [19], "callisto.si.usherb.ca");
//  strcpy (Server.cPort   [19], "5000");
//  strcpy (Server.cPrompt [19], "fics% ");
//  Server.nType [19] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [20], "wisdom.weizmann.ac.il");
//  strcpy (Server.cPort   [20], "5000");
//  strcpy (Server.cPrompt [20], "fics% ");
//  Server.nType [20] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [21], "chess.aaum.pt");
//  strcpy (Server.cPort   [21], "5000");
//  strcpy (Server.cPrompt [21], "fics% ");
//  Server.nType [21] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [22], "vics.ver.ucc.mx");
//  strcpy (Server.cPort   [22], "5000");
//  strcpy (Server.cPrompt [22], "fics% ");
//  Server.nType [22] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [23], "ics.interdomain.net.au");
//  strcpy (Server.cPort   [23], "5000");
//  strcpy (Server.cPrompt [23], "fics% ");
//  Server.nType [23] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [24], "chess.anu.edu.au");
//  strcpy (Server.cPort   [24], "5000");
//  strcpy (Server.cPrompt [24], "fics% ");
//  Server.nType [24] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [25], "fics.infcom.it");
//  strcpy (Server.cPort   [25], "5000");
//  strcpy (Server.cPrompt [25], "fics% ");
//  Server.nType [25] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [26], "cygnus.csa.iisc.ernet.in");
//  strcpy (Server.cPort   [26], "5000");
//  strcpy (Server.cPrompt [26], "fics% ");
//  Server.nType [26] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [27], "mezquite.iico.uaslp.mx");
//  strcpy (Server.cPort   [27], "5000");
//  strcpy (Server.cPrompt [27], "fics% ");
//  Server.nType [27] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [28], "gsi.gsini.net");
//  strcpy (Server.cPort   [28], "5000");
//  strcpy (Server.cPrompt [28], "fics% ");
//  Server.nType [28] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [29], "rogue.coe.ohio-state.edu");
//  strcpy (Server.cPort   [29], "5000");
//  strcpy (Server.cPrompt [29], "fics% ");
//  Server.nType [29] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [30], "yakko.cs.wmich.edu");
//  strcpy (Server.cPort   [30], "5000");
//  strcpy (Server.cPrompt [30], "fics% ");
//  Server.nType [30] = SERVER_NONFICS;
//
//  strcpy (Server.cServer [31], "nebin.eurecom.fr");
//  strcpy (Server.cPort   [31], "5000");
//  strcpy (Server.cPrompt [31], "fics% ");
//  Server.nType [31] = SERVER_NONFICS;
}

void SERVER_Adjust(void)
{
	int nI, nJ;

	for(nI = 0 ; nI < MAX_SERVER ; nI++)
	{
		if(strlen(Server.cServer [nI]) == 0)
		{
			for(nJ = nI ; nJ < (MAX_SERVER - 1) ; nJ++)
			{
				strcpy(Server.cServer [nJ], Server.cServer [nJ + 1]);
				strcpy(Server.cPort   [nJ], Server.cPort   [nJ + 1]);
				strcpy(Server.cPrompt [nJ], Server.cPrompt [nJ + 1]);
				Server.nType [nJ] = Server.nType [nJ + 1];
			}
		}
	}

	for(nI = 0 ; nI < MAX_SERVER ; nI++)
	{
		if(strlen(Server.cServer [nI]) > 0)
		{
			if(strlen(Server.cPort [nI]) == 0)
			{
#ifndef KICS
				strcpy(Server.cPort [nI], "5000");
#endif

#ifdef KICS
				strcpy(Server.cPort [nI], "6000");
#endif
			}
			else
			{
				Server.cPort [nI] [MAX_SERVER_PORT_SIZE - 1] = NULL_CHAR;
			}

			switch(Server.nType [nI])
			{
				case SERVER_FICS :
#ifndef KICS
					strcpy(Server.cPrompt [nI], FICS_PROMPT);
#endif
#ifdef KICS
					strcpy(Server.cPrompt [nI], FICS_PROMPT);
#endif

					break;

				case SERVER_ICC :
					strcpy(Server.cPrompt [nI], ICC_PROMPT);
					break;

				case SERVER_NONFICS :
					if(strlen(Server.cPrompt [nI]) == 0)
					{
						strcpy(Server.cPrompt [nI], NONFICS_PROMPT);
					}
					break;

				default :
					Server.nType [nI] = SERVER_FICS;
					strcpy(Server.cPrompt [nI], FICS_PROMPT);
					break;
			}
		}
	}
}
