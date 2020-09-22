#include "thief.h"

BOOL CALLBACK TelnetOptionBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			SendDlgItemMessage(hwnd, IDD_TOPTION_1,  BM_SETCHECK, User.bMouseWheel            ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_2,  BM_SETCHECK, User.bShowUnderline         ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_3,  BM_SETCHECK, User.bEnterHighlight        ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_4,  BM_SETCHECK, User.bTelnetResizeGoBottom  ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_5,  BM_SETCHECK, User.bSuppressAnnouncement  ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_6,  BM_SETCHECK, User.bSuppressCompKibitz    ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_7,  BM_SETCHECK, User.bSuppressCompWhisper   ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_8,  BM_SETCHECK, User.bSuppressIgnoreFormula ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_9,  BM_SETCHECK, User.bSuppressROBOWelcome   ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_10, BM_SETCHECK, User.bSuppressCompChTell    ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_11, BM_SETCHECK, User.bSuppressCompTell      ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_12, BM_SETCHECK, User.bSuppressCompShout     ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_14, BM_SETCHECK, User.bTimeStampTelnet	   ? BST_CHECKED : BST_UNCHECKED, 0) ;
			SendDlgItemMessage(hwnd, IDD_TOPTION_15, BM_SETCHECK, User.bTimeStampLog		   ? BST_CHECKED : BST_UNCHECKED, 0) ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					User.bMouseWheel            = ((SendDlgItemMessage(hwnd, IDD_TOPTION_1,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bShowUnderline         = ((SendDlgItemMessage(hwnd, IDD_TOPTION_2,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bEnterHighlight        = ((SendDlgItemMessage(hwnd, IDD_TOPTION_3,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bTelnetResizeGoBottom  = ((SendDlgItemMessage(hwnd, IDD_TOPTION_4,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressAnnouncement  = ((SendDlgItemMessage(hwnd, IDD_TOPTION_5,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressCompKibitz    = ((SendDlgItemMessage(hwnd, IDD_TOPTION_6,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressCompWhisper   = ((SendDlgItemMessage(hwnd, IDD_TOPTION_7,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressIgnoreFormula = ((SendDlgItemMessage(hwnd, IDD_TOPTION_8,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressROBOWelcome   = ((SendDlgItemMessage(hwnd, IDD_TOPTION_9,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressCompChTell    = ((SendDlgItemMessage(hwnd, IDD_TOPTION_10, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressCompTell      = ((SendDlgItemMessage(hwnd, IDD_TOPTION_11, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bSuppressCompShout     = ((SendDlgItemMessage(hwnd, IDD_TOPTION_12, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bTimeStampTelnet		= ((SendDlgItemMessage(hwnd, IDD_TOPTION_14, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;
					User.bTimeStampLog			= ((SendDlgItemMessage(hwnd, IDD_TOPTION_15, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0) ;

					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDCANCEL :
					EndDialog(hwnd, FALSE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;

				case IDD_TOPTION_13 :
					SendDlgItemMessage(hwnd, IDD_TOPTION_1,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_2,  BM_SETCHECK, BST_CHECKED,   0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_3,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_4,  BM_SETCHECK, BST_CHECKED,   0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_5,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_6,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_7,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_8,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_9,  BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_10, BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_11, BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_12, BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_14, BM_SETCHECK, BST_UNCHECKED, 0) ;
					SendDlgItemMessage(hwnd, IDD_TOPTION_15, BM_SETCHECK, BST_UNCHECKED, 0) ;
					return TRUE ;
			}
			break ;
	}
	return FALSE ;
}
