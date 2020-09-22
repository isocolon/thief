#include "thief.h"

LPBUTTONS  LpButtons ;
FONTRECORD ButFont ;

BOOL CALLBACK ButtonBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int nSel ;
	static HWND hFunction ;
	int nI, nF ;
	char cTmp [2048] ;
	RECT rc ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			hFunction = GetDlgItem(hwnd, IDD_BUTTON_FUNCTION) ;
			LpButtons = (LPBUTTONS) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BUTTONS)) ;

			for(nI = 0 ; nI < MAX_BUTTON ; nI++)
			{
				LpButtons->nType [nI] = Buttons.nType [nI] ;
				strcpy(LpButtons->cText   [nI], Buttons.cText   [nI]) ;
				strcpy(LpButtons->cAction [nI], Buttons.cAction [nI]) ;
			}

			SendDlgItemMessage(hwnd, IDD_BUTTON_EXTENDED, BM_SETCHECK, User.bExtendedButton ? BST_CHECKED : BST_UNCHECKED, 0) ;

			SendDlgItemMessage(hwnd, IDD_BUTTON, LB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < MAX_BUTTON ; nI++)
			{
				sprintf(cTmp, "Button %02d", nI + 1) ;
				SendDlgItemMessage(hwnd, IDD_BUTTON, LB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}

			SendDlgItemMessage(hwnd, IDD_BUTTON, LB_SETCURSEL, 0, 0) ;
			SendDlgItemMessage(hwnd, IDD_BUTTON_LABEL,   WM_SETTEXT, 0, (LPARAM) LpButtons->cText   [0]) ;

			SendMessage(hFunction, CB_RESETCONTENT, 0, 0) ;
			for(nI = 0 ; nI < MAX_FMENU ; nI++)
			{
				strcpy(cTmp, FString [nI]) ;
				SendMessage(hFunction, CB_ADDSTRING, 0, (LPARAM) cTmp) ;
			}

			strcpy(cTmp, FString [LpButtons->nType [0]]) ;
			if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
				SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
			}

			SendDlgItemMessage(hwnd, IDD_BUTTON_COMMAND, WM_SETTEXT, 0, (LPARAM) LpButtons->cAction [0]) ;

			FONT_Copy_Info(&ButFont, &hfFont [FONT_BUTTON]) ;

			nSel = 0 ;
			EnableWindow(GetDlgItem(hwnd, IDD_BUTTON_COMMAND), TOOLBOX_NeedCommandValue(LpButtons->nType [nSel])) ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDD_BUTTON :
					if(HIWORD(wParam) == LBN_SELCHANGE)
					{
						nI = SendDlgItemMessage(hwnd, IDD_BUTTON, LB_GETCURSEL, 0, 0) ;

						SendDlgItemMessage(hwnd, IDD_BUTTON_LABEL, WM_GETTEXT, (WPARAM) MAX_BUTTON_TEXT, (LPARAM) cTmp) ;
						strcpy(LpButtons->cText [nSel], cTmp) ;

						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
						if(nF == CB_ERR)
						{
							LpButtons->nType [nSel] = 0 ;
						}
						else
						{
							LpButtons->nType [nSel] = nF ;
						}

						if(TOOLBOX_NeedCommandValue(LpButtons->nType [nSel]))
						{
							SendDlgItemMessage(hwnd, IDD_BUTTON_COMMAND, WM_GETTEXT, (WPARAM) MAX_BUTTON_ACTION - 1, (LPARAM) cTmp) ;
							strcpy(LpButtons->cAction [nSel], cTmp) ;
						}
						else
						{
							strcpy(LpButtons->cAction [nSel], "") ;
						}

						strcpy(cTmp, FString [LpButtons->nType [nI]]) ;
						if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
						{
							SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
							SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
						}

						SendDlgItemMessage(hwnd, IDD_BUTTON_LABEL,   WM_SETTEXT, 0, (LPARAM) LpButtons->cText   [nI]) ;
						SendDlgItemMessage(hwnd, IDD_BUTTON_COMMAND, WM_SETTEXT, 0, (LPARAM) LpButtons->cAction [nI]) ;

						nSel = nI ;
						EnableWindow(GetDlgItem(hwnd, IDD_BUTTON_COMMAND), TOOLBOX_NeedCommandValue(LpButtons->nType [nSel])) ;
					}
					break ;

				case IDD_BUTTON_FUNCTION :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
						if(nF == CB_ERR)
						{
							EnableWindow(GetDlgItem(hwnd, IDD_BUTTON_COMMAND), 0) ;
						}
						else
						{
							EnableWindow(GetDlgItem(hwnd, IDD_BUTTON_COMMAND), TOOLBOX_NeedCommandValue(nF)) ;
						}
					}
					break ;

				case IDOK :
					User.bExtendedButton = ((SendDlgItemMessage(hwnd, IDD_BUTTON_EXTENDED, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

					SendDlgItemMessage(hwnd, IDD_BUTTON_LABEL, WM_GETTEXT, (WPARAM) MAX_BUTTON_TEXT, (LPARAM) cTmp) ;
					strcpy(LpButtons->cText [nSel], cTmp) ;

					nF = SendMessage(hFunction, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
					if(nF == CB_ERR)
					{
						LpButtons->nType [nSel] = 0 ;
					}
					else
					{
						LpButtons->nType [nSel] = nF ;
					}

					if(TOOLBOX_NeedCommandValue(LpButtons->nType [nSel]))
					{
						SendDlgItemMessage(hwnd, IDD_BUTTON_COMMAND, WM_GETTEXT, (WPARAM) MAX_BUTTON_ACTION - 1, (LPARAM) cTmp) ;
						strcpy(LpButtons->cAction [nSel], cTmp) ;
					}
					else
					{
						strcpy(LpButtons->cAction [nSel], "") ;
					}

					for(nI = 0 ; nI < MAX_BUTTON ; nI++)
					{
						Buttons.nType [nI] = LpButtons->nType [nI] ;
						strcpy(Buttons.cText   [nI], LpButtons->cText   [nI]) ;
						strcpy(Buttons.cAction [nI], LpButtons->cAction [nI]) ;

						TOOLBOX_AllTrim(Buttons.cText [nI]) ;

						if(TOOLBOX_NeedCommandValue(Buttons.nType [nI]))
						{
							TOOLBOX_AllTrim(Buttons.cAction [nI]) ;
						}
						else
						{
							strcpy(Buttons.cAction [nI], "") ;
						}
					}

					HeapFree(GetProcessHeap(), 0, LpButtons) ;

					DeleteObject(ButFont.hfFont) ;

					TOOLBOX_MakeFont(FONT_BUTTON) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM(TRUE, 0)) ;

					BUTTON_Refresh() ;
					BUTTON_RefreshLabel() ;

					GetClientRect(hwndWindow [HWND_BUTTON], &rc) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SIZE, SIZE_RESTORED, (LPARAM)((rc.bottom << 16) | rc.right)) ;

					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					HeapFree(GetProcessHeap(), 0, LpButtons) ;

					FONT_Copy_Info(&hfFont [FONT_BUTTON], &ButFont) ;

					DeleteObject(hfFont [FONT_BUTTON].hfFont) ;

					TOOLBOX_MakeFont(FONT_BUTTON) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM(TRUE, 0)) ;

					GetClientRect(hwndWindow [HWND_BUTTON], &rc) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SIZE, SIZE_RESTORED, (LPARAM)((rc.bottom << 16) | rc.right)) ;

					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_BUTTON_FONT :
					if(FONT_ChooseFont(hwndWindow [HWND_BUTTON], FONT_BUTTON))
					{
					}
					return TRUE ;

				case IDD_BUTTON_DEFAULT :
					for(nI = 0 ; nI < MAX_BUTTON ; nI++)
					{
						if(nI <= DEFAULT_CHESS_BUTTONS_USED)
						{
							LpButtons->nType [nI] = FUNCTION_COMMAND ;
						}
						else
						{
							LpButtons->nType [nI] = FUNCTION_NONE ;
						}
						strcpy(LpButtons->cText   [nI], "") ;
						strcpy(LpButtons->cAction [nI], "") ;
					}

					LpButtons->nType [4] = FUNCTION_MATCH_BOX ;
					LpButtons->nType [8] = FUNCTION_SEEK_BOX ;

					strcpy(LpButtons->cText   [0], DEFAULT_CHESS_BUTTON0_TEXT) ;
					strcpy(LpButtons->cAction [0], DEFAULT_CHESS_BUTTON0_ACTION) ;

					strcpy(LpButtons->cText   [1], DEFAULT_CHESS_BUTTON1_TEXT) ;
					strcpy(LpButtons->cAction [1], DEFAULT_CHESS_BUTTON1_ACTION) ;

					strcpy(LpButtons->cText   [2], DEFAULT_CHESS_BUTTON2_TEXT) ;
					strcpy(LpButtons->cAction [2], DEFAULT_CHESS_BUTTON2_ACTION) ;

					strcpy(LpButtons->cText   [3], DEFAULT_CHESS_BUTTON3_TEXT) ;
					strcpy(LpButtons->cAction [3], DEFAULT_CHESS_BUTTON3_ACTION) ;

					strcpy(LpButtons->cText   [4], DEFAULT_CHESS_BUTTON4_TEXT) ;
//                    strcpy (LpButtons->cAction [4], DEFAULT_CHESS_BUTTON4_ACTION) ;

					strcpy(LpButtons->cText   [5], DEFAULT_CHESS_BUTTON5_TEXT) ;
					strcpy(LpButtons->cAction [5], DEFAULT_CHESS_BUTTON5_ACTION) ;

					strcpy(LpButtons->cText   [6], DEFAULT_CHESS_BUTTON6_TEXT) ;
					strcpy(LpButtons->cAction [6], DEFAULT_CHESS_BUTTON6_ACTION) ;

					strcpy(LpButtons->cText   [7], DEFAULT_CHESS_BUTTON7_TEXT) ;
					strcpy(LpButtons->cAction [7], DEFAULT_CHESS_BUTTON7_ACTION) ;

					strcpy(LpButtons->cText   [8], DEFAULT_CHESS_BUTTON8_TEXT) ;
//                    strcpy (LpButtons->cAction [8], DEFAULT_CHESS_BUTTON8_ACTION) ;

					strcpy(LpButtons->cText   [9], DEFAULT_CHESS_BUTTON9_TEXT) ;
					strcpy(LpButtons->cAction [9], DEFAULT_CHESS_BUTTON9_ACTION) ;

					strcpy(LpButtons->cText   [10], DEFAULT_CHESS_BUTTON10_TEXT) ;
					strcpy(LpButtons->cAction [10], DEFAULT_CHESS_BUTTON10_ACTION) ;

					strcpy(LpButtons->cText   [11], DEFAULT_CHESS_BUTTON11_TEXT) ;
					strcpy(LpButtons->cAction [11], DEFAULT_CHESS_BUTTON11_ACTION) ;

					strcpy(LpButtons->cText   [12], DEFAULT_CHESS_BUTTON12_TEXT) ;
					strcpy(LpButtons->cAction [12], DEFAULT_CHESS_BUTTON12_ACTION) ;

					strcpy(LpButtons->cText   [13], DEFAULT_CHESS_BUTTON13_TEXT) ;
					strcpy(LpButtons->cAction [13], DEFAULT_CHESS_BUTTON13_ACTION) ;

					strcpy(LpButtons->cText   [14], DEFAULT_CHESS_BUTTON14_TEXT) ;
					strcpy(LpButtons->cAction [14], DEFAULT_CHESS_BUTTON14_ACTION) ;

					strcpy(LpButtons->cText   [15], DEFAULT_CHESS_BUTTON15_TEXT) ;
					strcpy(LpButtons->cAction [15], DEFAULT_CHESS_BUTTON15_ACTION) ;

					strcpy(LpButtons->cText   [16], DEFAULT_CHESS_BUTTON16_TEXT) ;
					strcpy(LpButtons->cAction [16], DEFAULT_CHESS_BUTTON16_ACTION) ;

					strcpy(LpButtons->cText   [17], DEFAULT_CHESS_BUTTON17_TEXT) ;
					strcpy(LpButtons->cAction [17], DEFAULT_CHESS_BUTTON17_ACTION) ;

					strcpy(LpButtons->cText   [18], DEFAULT_CHESS_BUTTON18_TEXT) ;
					strcpy(LpButtons->cAction [18], DEFAULT_CHESS_BUTTON18_ACTION) ;

					strcpy(LpButtons->cText   [19], DEFAULT_CHESS_BUTTON19_TEXT) ;
					strcpy(LpButtons->cAction [19], DEFAULT_CHESS_BUTTON19_ACTION) ;

					strcpy(LpButtons->cText   [20], DEFAULT_CHESS_BUTTON20_TEXT) ;
					strcpy(LpButtons->cAction [20], DEFAULT_CHESS_BUTTON20_ACTION) ;

					strcpy(LpButtons->cText   [21], DEFAULT_CHESS_BUTTON21_TEXT) ;
					strcpy(LpButtons->cAction [21], DEFAULT_CHESS_BUTTON21_ACTION) ;

					strcpy(LpButtons->cText   [22], DEFAULT_CHESS_BUTTON22_TEXT) ;
					strcpy(LpButtons->cAction [22], DEFAULT_CHESS_BUTTON22_ACTION) ;

					strcpy(LpButtons->cText   [23], DEFAULT_CHESS_BUTTON23_TEXT) ;
					strcpy(LpButtons->cAction [23], DEFAULT_CHESS_BUTTON23_ACTION) ;

					strcpy(LpButtons->cText   [24], DEFAULT_CHESS_BUTTON24_TEXT) ;
					strcpy(LpButtons->cAction [24], DEFAULT_CHESS_BUTTON24_ACTION) ;

					strcpy(LpButtons->cText   [25], DEFAULT_CHESS_BUTTON25_TEXT) ;
					strcpy(LpButtons->cAction [25], DEFAULT_CHESS_BUTTON25_ACTION) ;

					strcpy(LpButtons->cText   [26], DEFAULT_CHESS_BUTTON26_TEXT) ;
					strcpy(LpButtons->cAction [26], DEFAULT_CHESS_BUTTON26_ACTION) ;

					strcpy(LpButtons->cText   [27], DEFAULT_CHESS_BUTTON27_TEXT) ;
					strcpy(LpButtons->cAction [27], DEFAULT_CHESS_BUTTON27_ACTION) ;

					strcpy(LpButtons->cText   [28], DEFAULT_CHESS_BUTTON28_TEXT) ;
					strcpy(LpButtons->cAction [28], DEFAULT_CHESS_BUTTON28_ACTION) ;

					strcpy(LpButtons->cText   [29], DEFAULT_CHESS_BUTTON29_TEXT) ;
					strcpy(LpButtons->cAction [29], DEFAULT_CHESS_BUTTON29_ACTION) ;

					strcpy(LpButtons->cText   [30], DEFAULT_CHESS_BUTTON30_TEXT) ;
					strcpy(LpButtons->cAction [30], DEFAULT_CHESS_BUTTON30_ACTION) ;

					strcpy(LpButtons->cText   [31], DEFAULT_CHESS_BUTTON31_TEXT) ;
					strcpy(LpButtons->cAction [31], DEFAULT_CHESS_BUTTON31_ACTION) ;

					strcpy(LpButtons->cText   [32], DEFAULT_CHESS_BUTTON32_TEXT) ;
					strcpy(LpButtons->cAction [32], DEFAULT_CHESS_BUTTON32_ACTION) ;

					strcpy(LpButtons->cText   [33], DEFAULT_CHESS_BUTTON33_TEXT) ;
					strcpy(LpButtons->cAction [33], DEFAULT_CHESS_BUTTON33_ACTION) ;

					strcpy(LpButtons->cText   [34], DEFAULT_CHESS_BUTTON34_TEXT) ;
					strcpy(LpButtons->cAction [34], DEFAULT_CHESS_BUTTON34_ACTION) ;

					strcpy(LpButtons->cText   [35], DEFAULT_CHESS_BUTTON35_TEXT) ;
					strcpy(LpButtons->cAction [35], DEFAULT_CHESS_BUTTON35_ACTION) ;

					strcpy(LpButtons->cText   [36], DEFAULT_CHESS_BUTTON36_TEXT) ;
					strcpy(LpButtons->cAction [36], DEFAULT_CHESS_BUTTON36_ACTION) ;

					strcpy(LpButtons->cText   [37], DEFAULT_CHESS_BUTTON37_TEXT) ;
					strcpy(LpButtons->cAction [37], DEFAULT_CHESS_BUTTON37_ACTION) ;

					strcpy(LpButtons->cText   [38], DEFAULT_CHESS_BUTTON38_TEXT) ;
					strcpy(LpButtons->cAction [38], DEFAULT_CHESS_BUTTON38_ACTION) ;

					strcpy(LpButtons->cText   [39], DEFAULT_CHESS_BUTTON39_TEXT) ;
					strcpy(LpButtons->cAction [39], DEFAULT_CHESS_BUTTON39_ACTION) ;

					strcpy(LpButtons->cText   [40], DEFAULT_CHESS_BUTTON40_TEXT) ;
					strcpy(LpButtons->cAction [40], DEFAULT_CHESS_BUTTON40_ACTION) ;

					strcpy(LpButtons->cText   [41], DEFAULT_CHESS_BUTTON41_TEXT) ;
					strcpy(LpButtons->cAction [41], DEFAULT_CHESS_BUTTON41_ACTION) ;

					strcpy(LpButtons->cText   [42], DEFAULT_CHESS_BUTTON42_TEXT) ;
					strcpy(LpButtons->cAction [42], DEFAULT_CHESS_BUTTON42_ACTION) ;

					strcpy(LpButtons->cText   [43], DEFAULT_CHESS_BUTTON43_TEXT) ;
					strcpy(LpButtons->cAction [43], DEFAULT_CHESS_BUTTON43_ACTION) ;

					strcpy(LpButtons->cText   [44], DEFAULT_CHESS_BUTTON44_TEXT) ;
					strcpy(LpButtons->cAction [44], DEFAULT_CHESS_BUTTON44_ACTION) ;

					strcpy(LpButtons->cText   [45], DEFAULT_CHESS_BUTTON45_TEXT) ;
					strcpy(LpButtons->cAction [45], DEFAULT_CHESS_BUTTON45_ACTION) ;

					strcpy(LpButtons->cText   [46], DEFAULT_CHESS_BUTTON46_TEXT) ;
					strcpy(LpButtons->cAction [46], DEFAULT_CHESS_BUTTON46_ACTION) ;

					strcpy(LpButtons->cText   [47], DEFAULT_CHESS_BUTTON47_TEXT) ;
					strcpy(LpButtons->cAction [47], DEFAULT_CHESS_BUTTON47_ACTION) ;

					strcpy(LpButtons->cText   [48], DEFAULT_CHESS_BUTTON48_TEXT) ;
					strcpy(LpButtons->cAction [48], DEFAULT_CHESS_BUTTON48_ACTION) ;

					strcpy(LpButtons->cText   [49], DEFAULT_CHESS_BUTTON49_TEXT) ;
					strcpy(LpButtons->cAction [49], DEFAULT_CHESS_BUTTON49_ACTION) ;

					strcpy(LpButtons->cText   [50], DEFAULT_CHESS_BUTTON50_TEXT) ;
					strcpy(LpButtons->cAction [50], DEFAULT_CHESS_BUTTON50_ACTION) ;

					strcpy(LpButtons->cText   [51], DEFAULT_CHESS_BUTTON51_TEXT) ;
					strcpy(LpButtons->cAction [51], DEFAULT_CHESS_BUTTON51_ACTION) ;

					strcpy(LpButtons->cText   [52], DEFAULT_CHESS_BUTTON52_TEXT) ;
					strcpy(LpButtons->cAction [52], DEFAULT_CHESS_BUTTON52_ACTION) ;

					strcpy(LpButtons->cText   [53], DEFAULT_CHESS_BUTTON53_TEXT) ;
					strcpy(LpButtons->cAction [53], DEFAULT_CHESS_BUTTON53_ACTION) ;

					nI = SendDlgItemMessage(hwnd, IDD_BUTTON, LB_GETCURSEL, 0, 0) ;

					SendDlgItemMessage(hwnd, IDD_BUTTON_LABEL, WM_SETTEXT, 0, (LPARAM) LpButtons->cText [nI]) ;

					if(! TOOLBOX_NeedCommandValue(LpButtons->nType [nI]))
					{
						strcpy(LpButtons->cAction [nI], "") ;
					}

					strcpy(cTmp, FString [LpButtons->nType [nI]]) ;
					if(SendMessage(hFunction, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hFunction, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
						SendMessage(hFunction, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
					}

					SendDlgItemMessage(hwnd, IDD_BUTTON_COMMAND, WM_SETTEXT, 0, (LPARAM) LpButtons->cAction [nI]) ;

					nSel = nI ;
					EnableWindow(GetDlgItem(hwnd, IDD_BUTTON_COMMAND), TOOLBOX_NeedCommandValue(LpButtons->nType [nSel])) ;

					FONT_Init_Button() ;
					return TRUE ;

				case IDD_BUTTON_CHESS :
					User.bExtendedButton = ((SendDlgItemMessage(hwnd, IDD_BUTTON_EXTENDED, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

					HeapFree(GetProcessHeap(), 0, LpButtons) ;

					FONT_Copy_Info(&hfFont [FONT_BUTTON], &ButFont) ;

					DeleteObject(hfFont [FONT_BUTTON].hfFont) ;

					TOOLBOX_MakeFont(FONT_BUTTON) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM(TRUE, 0)) ;

					GetClientRect(hwndWindow [HWND_BUTTON], &rc) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SIZE, SIZE_RESTORED, (LPARAM)((rc.bottom << 16) | rc.right)) ;

					BUTTON_Chess() ;
					BUTTON_Refresh() ;
					BUTTON_RefreshLabel() ;

					EAR_Destroy() ;
					EAR_OldFICSEAR() ;
					EAR_Load() ;

					GetClientRect(hwndWindow [HWND_BUTTON], &rc) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SIZE, SIZE_RESTORED, (LPARAM)((rc.bottom << 16) | rc.right)) ;

					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_BUTTON_OLDFICS :
					User.bExtendedButton = ((SendDlgItemMessage(hwnd, IDD_BUTTON_EXTENDED, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

					HeapFree(GetProcessHeap(), 0, LpButtons) ;

					FONT_Copy_Info(&hfFont [FONT_BUTTON], &ButFont) ;

					DeleteObject(hfFont [FONT_BUTTON].hfFont) ;

					TOOLBOX_MakeFont(FONT_BUTTON) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM(TRUE, 0)) ;

					GetClientRect(hwndWindow [HWND_BUTTON], &rc) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SIZE, SIZE_RESTORED, (LPARAM)((rc.bottom << 16) | rc.right)) ;

					BUTTON_OldFICSEAR() ;
					BUTTON_Refresh() ;
					BUTTON_RefreshLabel() ;

					EAR_Destroy() ;
					EAR_OldFICSEAR() ;
					EAR_Load() ;

					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_BUTTON_NEWFICS :
					User.bExtendedButton = ((SendDlgItemMessage(hwnd, IDD_BUTTON_EXTENDED, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

					HeapFree(GetProcessHeap(), 0, LpButtons) ;

					FONT_Copy_Info(&hfFont [FONT_BUTTON], &ButFont) ;

					DeleteObject(hfFont [FONT_BUTTON].hfFont) ;

					TOOLBOX_MakeFont(FONT_BUTTON) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SETFONT, (WPARAM) hfFont [FONT_BUTTON].hfFont, MAKELPARAM(TRUE, 0)) ;

					GetClientRect(hwndWindow [HWND_BUTTON], &rc) ;

					SendMessage(hwndWindow [HWND_BUTTON], WM_SIZE, SIZE_RESTORED, (LPARAM)((rc.bottom << 16) | rc.right)) ;

					BUTTON_NewFICSEAR() ;
					BUTTON_Refresh() ;
					BUTTON_RefreshLabel() ;

					EAR_Destroy() ;
					EAR_NewFICSEAR() ;
					EAR_Load() ;

					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}
