#include "thief.h"

#define TEL_NUM_LINES   26
#define TEL_CO_MAX      1000
#define TEL_CO_TRIM     100
#define TEL_BACKGROUND  11

char *TelStr [TEL_NUM_LINES] = { "Font",
								 "Normal Color",
								 "Tell Color",
								 "Ptell Color",
								 "Say Color",
								 "Kibitz Color",
								 "Whisper Color",
								 "Shout Color",
								 "Cshout/Sshout Color",
								 "Challenge Color",
								 "User Text Color",
								 "Background Color",
								 "Notification Color",
								 ":Notification: Color",
								 "Arrival Color",
								 "Departure Color",
								 "Game Notification Color",
								 "Announcement Color",
								 ": Color",
								 ":Handle(###): Color",
								 ":Handle(C###): Color",
								 ":Handle(T###): Color",
								 ":Handle t-shouts: Color",
								 ":Mamer Color",
								 "Channel Color",
								 "Alternate Channel Color"
							   };

char TelBuf [TEL_CO_MAX + 1];
COLORREF TelClr [TEL_NUM_LINES - 1];

int Tel_bChannel [MAX_CHANNEL_COLOR];
COLORREF Tel_clrChannel [MAX_CHANNEL_COLOR];

int TelCount;
int TelIndex [MAX_CHANNEL_COLOR];

FONTRECORD TelFr;
CHARFORMAT cfTelCf;

BOOL CALLBACK TelnetBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hText;
	static HWND hChannel;
	int nI, nJ, nK, nTmp, bDoIt;
	char cTmp [2048];

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			hText    = GetDlgItem(hwnd, IDD_TELNET_TEXT);
			hChannel = GetDlgItem(hwnd, IDD_TELNET_CHANNEL);

			cfTelCf.cbSize = sizeof(CHARFORMAT);

			for(nI = CLR_TELNET_NORMAL ; nI <= CLR_TELNET_CHANNEL97 ; nI++)
			{
				TelClr [nI - CLR_TELNET_NORMAL] = clrColor [nI];
			}

			TelCount = 0;
			for(nI = 0 ; nI < MAX_CHANNEL_COLOR ; nI++)
			{
				Tel_bChannel   [nI] = bChannel   [nI];
				Tel_clrChannel [nI] = clrChannel [nI];
				if(Tel_bChannel [nI])
				{
					TelIndex [TelCount] = nI;
					TelCount = TelCount + 1;
				}
			}

			FONT_Copy_Info(&TelFr, &hfFont [FONT_TELNET]);

			SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_RESETCONTENT, 0, 0);
			for(nI = 0 ; nI < TEL_NUM_LINES ; nI++)
			{
				sprintf(cTmp, "%03d. %s", nI + 1, TelStr [nI]);
				SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);
			}
			nJ = TEL_NUM_LINES;
			for(nI = 0 ; nI < MAX_CHANNEL_COLOR ; nI++)
			{
				if(Tel_bChannel [nI])
				{
					nJ = nJ + 1;
					sprintf(cTmp, "%03d. Channel %d Color", nJ, nI);
					SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);
				}
			}
			SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_SETCURSEL, 0, 0);

			strcpy(cTmp, "");
			SendDlgItemMessage(hwnd, IDD_TELNET_CHANNEL, WM_SETTEXT, 0, (LPARAM) cTmp);

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			TELCFG_Refresh(hText);
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_TELNET_LIST :
				case IDD_TELNET_CONFIGURE :
					if(LOWORD(wParam) == IDD_TELNET_CONFIGURE)
					{
						bDoIt = 1;
					}
					else
					{
						bDoIt = (HIWORD(wParam) == LBN_DBLCLK);
					}

					if(bDoIt)
					{
						nI = SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_GETCURSEL, 0, 0);
						if(nI == 0)                     // font
						{
							if(FONT_ChooseFont1(hwnd, &TelFr))
							{
								DeleteObject(TelFr.hfFont);
								TELCFG_Refresh(hText);
							}
						}
						else if(nI == TEL_BACKGROUND)       // background color
						{
							if(COLOR_ChooseColor1(hwnd, &TelClr [nI - 1]))
							{
								DeleteObject(TelFr.hfFont);
								TELCFG_Refresh(hText);
							}
						}
						else if(nI < TEL_NUM_LINES)         // other colors
						{
							if(COLOR_ChooseColor1(hwnd, &TelClr [nI - 1]))
							{
								DeleteObject(TelFr.hfFont);
								TELCFG_Refresh(hText);
							}
						}
						else                            // telnet channel colors
						{
							nJ = TelIndex [nI - TEL_NUM_LINES];
							if(nJ < MAX_CHANNEL_COLOR)
							{
								if(Tel_bChannel [nJ])
								{
									if(COLOR_ChooseColor1(hwnd, &Tel_clrChannel [nJ]))
									{
										DeleteObject(TelFr.hfFont);
										TELCFG_Refresh(hText);
									}
								}
							}
						}
					}
					break;

				case IDOK :
					EndDialog(hwnd, TRUE);
					DeleteObject(TelFr.hfFont);

					for(nI = CLR_TELNET_NORMAL ; nI <= CLR_TELNET_CHANNEL97 ; nI++)
					{
						clrColor [nI] = TelClr [nI - CLR_TELNET_NORMAL];
					}

					for(nI = 0 ; nI < MAX_CHANNEL_COLOR ; nI++)
					{
						bChannel   [nI] = Tel_bChannel   [nI];
						clrChannel [nI] = Tel_clrChannel [nI];
					}

					FONT_Copy_Info(&hfFont [FONT_TELNET], &TelFr);

					DeleteObject(hfFont [FONT_TELNET].hfFont);
					TOOLBOX_MakeFont(FONT_TELNET);

					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					DeleteObject(TelFr.hfFont);

					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_TELNET_DEFAULT :
				case IDD_TELNET_MUSOT :
					TelClr [CLR_TELNET_NORMAL       - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_TELL         - CLR_TELNET_NORMAL] = RGB(0xd8, 0xbb, 0x87);
					TelClr [CLR_TELNET_PTELL        - CLR_TELNET_NORMAL] = RGB(0xff, 0xff, 0x00);
					TelClr [CLR_TELNET_SAY          - CLR_TELNET_NORMAL] = RGB(0xd8, 0xb6, 0x87);
					TelClr [CLR_TELNET_KIBITZ       - CLR_TELNET_NORMAL] = RGB(0x00, 0x80, 0x40);
					TelClr [CLR_TELNET_WHISPER      - CLR_TELNET_NORMAL] = RGB(0x00, 0x80, 0x40);
					TelClr [CLR_TELNET_SHOUT        - CLR_TELNET_NORMAL] = RGB(0x80, 0x80, 0x00);
					TelClr [CLR_TELNET_CSHOUT       - CLR_TELNET_NORMAL] = RGB(0x80, 0x80, 0x00);
					TelClr [CLR_TELNET_CHALLENGE    - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0xff);
					TelClr [CLR_TELNET_USERTEXT     - CLR_TELNET_NORMAL] = RGB(0x00, 0x80, 0xff);
					TelClr [CLR_TELNET_BACKGROUND   - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_NOTIFICATION - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_COLON_NOTIFY - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_ARRIVAL      - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_DEPARTURE    - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_GAME_NOTIFY  - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_ANNOUNCEMENT - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_COLON        - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_COLON_256    - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_COLON_C2     - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_COLON_T3     - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_COLON_TSHOUTS- CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_COLON_MAMER  - CLR_TELNET_NORMAL] = RGB(0xf3, 0x78, 0x49);
					TelClr [CLR_TELNET_CHANNEL      - CLR_TELNET_NORMAL] = RGB(0x00, 0x80, 0x40);
					TelClr [CLR_TELNET_CHANNELA     - CLR_TELNET_NORMAL] = RGB(0x00, 0x80, 0x87);

					Tel_bChannel [ 1] = 1;
					Tel_bChannel [24] = 1;
					Tel_bChannel [97] = 1;

					Tel_clrChannel [ 1] = RGB(0x00, 0x80, 0x40);
					Tel_clrChannel [24] = RGB(0xc0, 0xc0, 0xc0);
					Tel_clrChannel [97] = RGB(0xc0, 0xc0, 0xc0);

					strcpy(TelFr.Name, "Fixedsys");

					TelFr.nPointSize = 9;
					TelFr.bBold      = 0;
					TelFr.bItalic    = 0;
					TelFr.bUnderLine = 0;
					TelFr.bStrikeOut = 0;

					DeleteObject(TelFr.hfFont);
					TELCFG_Refresh(hText);
					return TRUE;

				case IDD_TELNET_ALTHEAS :
					TelClr [CLR_TELNET_NORMAL       - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_TELL         - CLR_TELNET_NORMAL] = RGB(0xff, 0x00, 0x00);
					TelClr [CLR_TELNET_PTELL        - CLR_TELNET_NORMAL] = RGB(0xff, 0x00, 0x00);
					TelClr [CLR_TELNET_SAY          - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_KIBITZ       - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_WHISPER      - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_SHOUT        - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_CSHOUT       - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_CHALLENGE    - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_USERTEXT     - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_BACKGROUND   - CLR_TELNET_NORMAL] = RGB(0xff, 0xff, 0xff);
					TelClr [CLR_TELNET_NOTIFICATION - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_COLON_NOTIFY - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_ARRIVAL      - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_DEPARTURE    - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_GAME_NOTIFY  - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_ANNOUNCEMENT - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_COLON        - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_COLON_256    - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_COLON_C2     - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_COLON_T3     - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_COLON_TSHOUTS- CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_COLON_MAMER  - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_CHANNEL      - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0xff);
					TelClr [CLR_TELNET_CHANNELA     - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0xff);

					Tel_bChannel [ 1] = 1;
					Tel_bChannel [24] = 1;
					Tel_bChannel [97] = 1;

					Tel_clrChannel [ 1] = RGB(0x00, 0x00, 0xff);
					Tel_clrChannel [24] = RGB(0x00, 0x00, 0xff);
					Tel_clrChannel [97] = RGB(0x00, 0x00, 0xff);

					strcpy(TelFr.Name, "Courier New");

					TelFr.nPointSize = 9;
					TelFr.bBold      = 0;
					TelFr.bItalic    = 0;
					TelFr.bUnderLine = 0;
					TelFr.bStrikeOut = 0;

					DeleteObject(TelFr.hfFont);
					TELCFG_Refresh(hText);
					return TRUE;

				case IDD_TELNET_SLICS :
					TelClr [CLR_TELNET_NORMAL       - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_TELL         - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0xff);
					TelClr [CLR_TELNET_PTELL        - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0xff);
					TelClr [CLR_TELNET_SAY          - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_KIBITZ       - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_WHISPER      - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_SHOUT        - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_CSHOUT       - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_CHALLENGE    - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_USERTEXT     - CLR_TELNET_NORMAL] = RGB(0xff, 0xff, 0x00);
					TelClr [CLR_TELNET_BACKGROUND   - CLR_TELNET_NORMAL] = RGB(0x00, 0x00, 0x00);
					TelClr [CLR_TELNET_NOTIFICATION - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_COLON_NOTIFY - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_ARRIVAL      - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_DEPARTURE    - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_GAME_NOTIFY  - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_ANNOUNCEMENT - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_COLON        - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_COLON_256    - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_COLON_C2     - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_COLON_T3     - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_COLON_TSHOUTS- CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_CHANNEL      - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);
					TelClr [CLR_TELNET_CHANNELA     - CLR_TELNET_NORMAL] = RGB(0x00, 0xff, 0x00);

					Tel_bChannel [ 1] = 1;
					Tel_bChannel [24] = 1;
					Tel_bChannel [97] = 1;

					Tel_clrChannel [ 1] = RGB(0x00, 0xff, 0x00);
					Tel_clrChannel [24] = RGB(0x00, 0xff, 0x00);
					Tel_clrChannel [97] = RGB(0x00, 0xff, 0x00);

					strcpy(TelFr.Name, "Courier New");

					TelFr.nPointSize = 9;
					TelFr.bBold      = 0;
					TelFr.bItalic    = 0;
					TelFr.bUnderLine = 0;
					TelFr.bStrikeOut = 0;

					DeleteObject(TelFr.hfFont);
					TELCFG_Refresh(hText);
					return TRUE;

				case IDD_TELNET_CHANNEL :
					return TRUE;

				case IDD_TELNET_ADD :
					if(TelCount >= MAX_CHANNEL_COLOR)
					{
						sprintf(cTmp, "You Have Reached The Maximum Number of Channel Colors (%d).", MAX_CHANNEL_COLOR - 1);
						TOOLBOX_Error(cTmp);
						return FALSE;
					}

					SendDlgItemMessage(hwnd, IDD_TELNET_CHANNEL, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					if(strlen(cTmp) == 0)
					{
						TOOLBOX_Error("Missing Channel #.");
						SetFocus(hChannel);
						return FALSE;
					}

					nTmp = 0;
					for(nI = 0 ; nI < ((int) strlen(cTmp)) ; nI++)
					{
						if(! isdigit(cTmp [nI]))
						{
							nTmp = 1;
							break;
						}
					}
					if(nTmp)
					{
						TOOLBOX_Error("Invalid Channel #.");
						SetFocus(hChannel);
						return FALSE;
					}

					nTmp = atoi(cTmp);
					if((nTmp < 0) || (nTmp >= MAX_CHANNEL_COLOR))
					{
						sprintf(cTmp, "Channel # is Out of Range (0 to %d).", MAX_CHANNEL_COLOR - 1);
						TOOLBOX_Error(cTmp);
						SetFocus(hChannel);
						return FALSE;
					}

					if((nTmp == 1) || (nTmp == 24) || (nTmp == 97))
					{
						sprintf(cTmp, "System Channel #%d Already Exists.", nTmp);
						TOOLBOX_Error(cTmp);
						SetFocus(hChannel);
						return FALSE;
					}

					nJ = 0;
					for(nI = 0 ; nI < TelCount ; nI++)
					{
						if(TelIndex [nI] == nTmp)
						{
							nJ = 1;
							break;
						}
					}

					if(nJ)
					{
						sprintf(cTmp, "Channel #%d Already Exists.", nTmp);
						TOOLBOX_Error(cTmp);
						SetFocus(hChannel);
						return FALSE;
					}

					// process
					Tel_bChannel   [nTmp] = 1;
					Tel_clrChannel [nTmp] = TelClr [CLR_TELNET_CHANNEL - CLR_TELNET_NORMAL];

					TelIndex [TelCount] = nTmp;
					TelCount = TelCount + 1;

					nJ = TEL_NUM_LINES + TelCount;

					sprintf(cTmp, "%03d. Channel %d Color", nJ, nTmp);
					SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);
					SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_SETCURSEL, nJ - 1, 0);

					DeleteObject(TelFr.hfFont);
					TELCFG_Refresh(hText);

					strcpy(cTmp, "");
					SendDlgItemMessage(hwnd, IDD_TELNET_CHANNEL, WM_SETTEXT, 0, (LPARAM) cTmp);
					return TRUE;

				case IDD_TELNET_DELETE :
					if(TelCount <= 0)
					{
						TOOLBOX_Error("No Custom Channel # to Delete.");
						return FALSE;
					}

					SendDlgItemMessage(hwnd, IDD_TELNET_CHANNEL, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp);
					TOOLBOX_AllTrim(cTmp);
					if(strlen(cTmp) == 0)
					{
						TOOLBOX_Error("Missing Channel #.");
						SetFocus(hChannel);
						return FALSE;
					}

					nTmp = 0;
					for(nI = 0 ; nI < ((int) strlen(cTmp)) ; nI++)
					{
						if(! isdigit(cTmp [nI]))
						{
							nTmp = 1;
							break;
						}
					}
					if(nTmp)
					{
						TOOLBOX_Error("Invalid Channel #.");
						SetFocus(hChannel);
						return FALSE;
					}

					nTmp = atoi(cTmp);
					if((nTmp < 0) || (nTmp >= MAX_CHANNEL_COLOR))
					{
						sprintf(cTmp, "Channel # is Out of Range (0 to %d).", MAX_CHANNEL_COLOR - 1);
						TOOLBOX_Error(cTmp);
						SetFocus(hChannel);
						return FALSE;
					}

					if((nTmp == 1) || (nTmp == 24) || (nTmp == 97))
					{
						sprintf(cTmp, "System Channel #%d Can Not be Deleted.", nTmp);
						TOOLBOX_Error(cTmp);
						SetFocus(hChannel);
						return FALSE;
					}

					nJ = 0;
					for(nI = 0 ; nI < TelCount ; nI++)
					{
						if(TelIndex [nI] == nTmp)
						{
							nJ = 1;
							nK = nI;
							break;
						}
					}

					if(! nJ)
					{
						sprintf(cTmp, "Channel #%d Not Found.", nTmp);
						TOOLBOX_Error(cTmp);
						SetFocus(hChannel);
						return FALSE;
					}

					// process
					Tel_bChannel   [nTmp] = 0;
					Tel_clrChannel [nTmp] = TelClr [CLR_TELNET_CHANNEL];

					for(nI = nK ; nI < (TelCount - 1) ; nI++)
					{
						TelIndex [nI] = TelIndex [nI + 1];
					}

					TelCount = TelCount - 1;
					TelIndex [TelCount] = 0;

					// reload list
					SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_RESETCONTENT, 0, 0);
					for(nI = 0 ; nI < TEL_NUM_LINES ; nI++)
					{
						sprintf(cTmp, "%03d. %s", nI + 1, TelStr [nI]);
						SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);
					}
					nJ = TEL_NUM_LINES;
					for(nI = 0 ; nI < MAX_CHANNEL_COLOR ; nI++)
					{
						if(Tel_bChannel [nI])
						{
							nJ = nJ + 1;
							sprintf(cTmp, "%03d. Channel %d Color", nJ, nI);
							SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_ADDSTRING, 0, (LPARAM) cTmp);
						}
					}
					SendDlgItemMessage(hwnd, IDD_TELNET_LIST, LB_SETCURSEL, 0, 0);

					DeleteObject(TelFr.hfFont);
					TELCFG_Refresh(hText);

					strcpy(cTmp, "");
					SendDlgItemMessage(hwnd, IDD_TELNET_CHANNEL, WM_SETTEXT, 0, (LPARAM) cTmp);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void TELCFG_SetFont(HWND hwnd)
{
	CHARFORMAT cfmt;
	CHARRANGE  tmpsel, sel;
	PARAFORMAT paraf;

	cfmt.cbSize = sizeof(CHARFORMAT);
	cfmt.dwMask = CFM_FACE | CFM_SIZE | CFM_CHARSET;
	strcpy(cfmt.szFaceName, TelFr.Name);

	//
	// the 20.0 below is a magic number that is totally undocumented.
	// i deduced it by looking at the WordPad source code in the
	// MSVC++ samples directory.
	//
	cfmt.yHeight         = (int)(TelFr.nPointSize * 20.0 + 0.5);
	cfmt.bCharSet        = TelFr.lf.lfCharSet;
	cfmt.bPitchAndFamily = TelFr.lf.lfPitchAndFamily;

	SendMessage(hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfmt);

	// why are the following seemingly needed too?
	SendMessage(hwnd, EM_SETCHARFORMAT, SCF_DEFAULT, (LPARAM) &cfmt);
	SendMessage(hwnd, EM_EXGETSEL, 0, (LPARAM) &sel);

	tmpsel.cpMin = 0;
	tmpsel.cpMax = -1 ;     // 999999?

	SendMessage(hwnd, EM_EXSETSEL, 0, (LPARAM) &tmpsel);
	SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cfmt);

	//
	// trying putting this here too.  It still seems to tickle a RichEdit
	// bug: sometimes RichEdit indents the first line of a paragraph too.
	//
	paraf.cbSize        = sizeof(paraf);
	paraf.dwMask        = PFM_OFFSET | PFM_STARTINDENT;
	paraf.dxStartIndent = 0;
	paraf.dxOffset      = WRAP_INDENT;

	SendMessage(hwnd, EM_SETPARAFORMAT, 0, (LPARAM) &paraf);
	SendMessage(hwnd, EM_EXSETSEL,      0, (LPARAM) &sel);

	TELCFG_Colorize(TelClr [0]);
}

void TELCFG_Colorize(COLORREF clr)
{
	cfTelCf.dwMask      = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
	cfTelCf.crTextColor = clr;
	cfTelCf.dwEffects   = 0;

	if(TelFr.bBold)
	{
		cfTelCf.dwEffects |= CFE_BOLD;
	}

	if(TelFr.bItalic)
	{
		cfTelCf.dwEffects |= CFE_ITALIC;
	}

	if(TelFr.bUnderLine)
	{
		cfTelCf.dwEffects |= CFE_UNDERLINE;
	}

	if(TelFr.bStrikeOut)
	{
		cfTelCf.dwEffects |= CFE_STRIKEOUT;
	}
}

void TELCFG_Write(HWND hwnd, COLORREF clr, char *cData)
{
	int       nExlen;
	CHARRANGE sel;

	cfTelCf.crTextColor = clr;

	strcpy(TelBuf, cData);

	nExlen = GetWindowTextLength(hwnd);

	sel.cpMin = nExlen;
	sel.cpMax = nExlen;

	SendMessage(hwnd, EM_EXSETSEL, 0, (LPARAM) &sel);
	SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cfTelCf);
	SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM) TelBuf);
}

void TELCFG_Refresh(HWND hwnd)
{
	int  nI, nJ;
	char cTmp [100];

	FONT_CreateFont1(&TelFr,
					 TelFr.Name,       TelFr.nPointSize,
					 TelFr.bBold,      TelFr.bItalic,
					 TelFr.bUnderLine, TelFr.bStrikeOut);

	SendMessage(hwnd, EM_SETBKGNDCOLOR, FALSE, (LPARAM) TelClr [CLR_TELNET_BACKGROUND-CLR_TELNET_NORMAL]);

	TELCFG_Colorize(TelClr [0]);

	SendMessage(hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cfTelCf);

	TELCFG_SetFont(hwnd);

	SetWindowText(hwnd, "");

	for(nI = 0 ; nI < TEL_NUM_LINES ; nI++)
	{
		if((nI != 0) && (nI != TEL_BACKGROUND))
		{
			sprintf(cTmp, "%s\n", TelStr [nI]);
			TELCFG_Write(hwnd, TelClr [nI - 1], cTmp);
		}
	}

	for(nI = 0 ; nI < TelCount ; nI++)
	{
		nJ = TelIndex [nI];
		if(Tel_bChannel [nJ])
		{
			sprintf(cTmp, "Channel %d Color\n", nJ);
			TELCFG_Write(hwnd, Tel_clrChannel [nJ], cTmp);
		}
	}
}
