#include "thief.h"

int nCurrentPromote = IDD_QUEEN;
int nPromoteX = 0;
int nPromoteY = 0;

BOOL CALLBACK PromoteBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int nPromote;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			if((Game [INDEX_PLAY].nGameType == GAMETYPE_SUICIDE) ||
					(Game [INDEX_PLAY].nGameType == GAMETYPE_GIVEAWAY))
			{
				ShowWindow(GetDlgItem(hwnd, IDD_KING), SW_SHOW);
				nPromote = IDD_KING;
			}
			else
			{
				ShowWindow(GetDlgItem(hwnd, IDD_KING), SW_HIDE);
				nPromote = IDD_QUEEN;
			}

			SetWindowPos(hwnd, NULL, nPromoteX, nPromoteY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

			SetFocus(GetDlgItem(hwnd, nPromote));

			System.bInPromote  = 1;
			System.hwndPromote = hwnd;

			return TRUE;

		case WM_COMMAND :
			switch(LOWORD(wParam))
			{
				case IDCANCEL :
					EndDialog(hwnd, FALSE);

					System.bInPromote  = 0;
					System.hwndPromote = NULL;

					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_QUEEN :
				case IDD_ROOK :
				case IDD_BISHOP :
				case IDD_KNIGHT :
				case IDD_KING :
					nPromote = LOWORD(wParam);
					nCurrentPromote = nPromote;
					EndDialog(hwnd, TRUE);

					System.bInPromote  = 0;
					System.hwndPromote = NULL;

					STATE_LeaveDialogBox();
					return TRUE;
			}
			break;
	}
	return FALSE;
}

int PROMOTE_Pawn(HWND hwnd, HINSTANCE hInstance, int nX, int nY)
{
	nPromoteX = nX;
	nPromoteY = nY;

	if(DialogBox(hInstance, "PromoteBox", hwnd, PromoteBoxWndProc))
	{
		return nCurrentPromote;
	}
	return 0;
}
