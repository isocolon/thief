#include "thief.h"

BOOL CALLBACK GameOptionBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hWild5 ;
	char cTmp [2048] ;
	int  nI ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			hWild5 = GetDlgItem(hwnd, IDD_GOPTION_14) ;

			SendDlgItemMessage(hwnd, IDD_GOPTION_1,  BM_SETCHECK, User.bAutoQueen              ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_2,  BM_SETCHECK, User.bAutoFlag               ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_3,  BM_SETCHECK, User.bAutoRematch            ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_4,  BM_SETCHECK, User.bAutoObserve            ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_5,  BM_SETCHECK, User.bAutoObserveGNotify     ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_6,  BM_SETCHECK, User.bNoPlayAbuser           ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_7,  BM_SETCHECK, User.bNoPlayComputer         ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_8,  BM_SETCHECK, User.bActualSize             ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_9,  BM_SETCHECK, User.bStopResizePlay         ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_10, BM_SETCHECK, User.bEscCancelDragMove      ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_11, BM_SETCHECK, User.bEscCancelTruePremove   ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_12, BM_SETCHECK, User.bEscCancelSitDropMove   ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_GOPTION_13, BM_SETCHECK, User.bEscCancelPromoteKnight ? BST_CHECKED : BST_UNCHECKED, 0) ;

			SendMessage(hWild5, CB_RESETCONTENT, 0, 0) ;

			strcpy(cTmp, "Put My Pieces on Top") ;
			SendMessage(hWild5, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

			strcpy(cTmp, "Put My Pieces on Bottom") ;
			SendMessage(hWild5, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

			strcpy(cTmp, "Put Black Pieces on Bottom") ;
			SendMessage(hWild5, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

			strcpy(cTmp, "Put White Pieces on Bottom") ;
			SendMessage(hWild5, CB_ADDSTRING, 0, (LPARAM) cTmp) ;

			switch(User.nPlayWild5Flip)
			{
				case DEFAULT_WILD5_PLAY_TOP :
					strcpy(cTmp, "Put My Pieces on Top") ;
					break ;

				case DEFAULT_WILD5_PLAY_BOTTOM :
					strcpy(cTmp, "Put My Pieces on Bottom") ;
					break ;

				case DEFAULT_WILD5_PLAY_BBOTTOM :
					strcpy(cTmp, "Put Black Pieces on Bottom") ;
					break ;

				case DEFAULT_WILD5_PLAY_WBOTTOM :
					strcpy(cTmp, "Put White Pieces on Bottom") ;
					break ;

				default :
					User.nPlayWild5Flip = DEFAULT_WILD5_PLAY_TOP ;
					strcpy(cTmp, "Put My Pieces on Top") ;
					break ;
			}

			if(SendMessage(hWild5, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hWild5, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
				SendMessage(hWild5, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
			}

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					User.bAutoQueen              = ((SendDlgItemMessage(hwnd, IDD_GOPTION_1,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bAutoFlag               = ((SendDlgItemMessage(hwnd, IDD_GOPTION_2,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bAutoRematch            = ((SendDlgItemMessage(hwnd, IDD_GOPTION_3,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bAutoObserve            = ((SendDlgItemMessage(hwnd, IDD_GOPTION_4,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bAutoObserveGNotify     = ((SendDlgItemMessage(hwnd, IDD_GOPTION_5,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bNoPlayAbuser           = ((SendDlgItemMessage(hwnd, IDD_GOPTION_6,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bNoPlayComputer         = ((SendDlgItemMessage(hwnd, IDD_GOPTION_7,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bActualSize             = ((SendDlgItemMessage(hwnd, IDD_GOPTION_8,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bStopResizePlay         = ((SendDlgItemMessage(hwnd, IDD_GOPTION_9,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bEscCancelDragMove      = ((SendDlgItemMessage(hwnd, IDD_GOPTION_10, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bEscCancelTruePremove   = ((SendDlgItemMessage(hwnd, IDD_GOPTION_11, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bEscCancelSitDropMove   = ((SendDlgItemMessage(hwnd, IDD_GOPTION_12, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bEscCancelPromoteKnight = ((SendDlgItemMessage(hwnd, IDD_GOPTION_13, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

					nI = SendMessage(hWild5, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0) ;
					if(nI == CB_ERR)
					{
						User.nPlayWild5Flip = DEFAULT_WILD5_PLAY_TOP ;
					}
					else
					{
						switch(nI)
						{
							case 0 :
								User.nPlayWild5Flip = DEFAULT_WILD5_PLAY_TOP ;
								break ;

							case 1 :
								User.nPlayWild5Flip = DEFAULT_WILD5_PLAY_BOTTOM ;
								break ;

							case 2 :
								User.nPlayWild5Flip = DEFAULT_WILD5_PLAY_BBOTTOM ;
								break ;

							case 3 :
								User.nPlayWild5Flip = DEFAULT_WILD5_PLAY_WBOTTOM ;
								break ;

							default :
								User.nPlayWild5Flip = DEFAULT_WILD5_PLAY_TOP ;
								break ;
						}
					}

					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_GOPTION_15 :
					SendDlgItemMessage(hwnd, IDD_GOPTION_1,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_2,  BM_SETCHECK, BST_CHECKED,   0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_3,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_4,  BM_SETCHECK, BST_CHECKED,   0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_5,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_6,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_7,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_8,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_9,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_10, BM_SETCHECK, BST_CHECKED,   0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_11, BM_SETCHECK, BST_CHECKED,   0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_12, BM_SETCHECK, BST_CHECKED,   0) ;
					SendDlgItemMessage(hwnd, IDD_GOPTION_13, BM_SETCHECK, BST_CHECKED,   0) ;

					strcpy(cTmp, "Put My Pieces on Top") ;
					if(SendMessage(hWild5, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hWild5, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0) ;
						SendMessage(hWild5, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp) ;
					}
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}
