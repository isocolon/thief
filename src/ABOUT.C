#include "thief.h"

BOOL CALLBACK AboutBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hAbout ;
	char cTmp  [255] ;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE) ;
				return FALSE ;
			}

			hAbout = GetDlgItem(hwnd, IDD_ABOUT_VERSION) ;

#ifndef KICS
			sprintf(cTmp, "%s %s", THIEF_VER, THIEF_NICK) ;
#endif

#ifdef KICS
			sprintf(cTmp, "%s %s", THIEF_VER, THIEF_KICS_NICK) ;
#endif
			if(LUDENS)
			{
				strncat(cTmp, THIEF_NICK_LUDENS, strlen(THIEF_NICK_LUDENS));
			}

			SetWindowText(hAbout, cTmp) ;

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER)) ;
			return TRUE ;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
				case IDCANCEL :
					EndDialog(hwnd, TRUE) ;
					STATE_LeaveDialogBox() ;
					return TRUE ;
				case IDD_CREDITS :
					strcpy(cTmp, TOOLBOX_GetInstDirPath());
					strcat(cTmp, "\\credits.txt");
					TOOLBOX_OpenNotePad(cTmp);
					break ;
				case IDD_THANKS :
					strcpy(cTmp, TOOLBOX_GetInstDirPath());
					strcat(cTmp, "\\thanks.txt");
					TOOLBOX_OpenNotePad(cTmp);
					break ;
				case IDD_CHANGES :
					strcpy(cTmp, TOOLBOX_GetInstDirPath());
					strcat(cTmp, "\\changes.txt");
					TOOLBOX_OpenNotePad(cTmp);
					break ;
				case IDD_LGPL :
					strcpy(cTmp, TOOLBOX_GetInstDirPath());
					strcat(cTmp, "\\LGPL.txt");
					TOOLBOX_OpenNotePad(cTmp);
					break ;
			}
			break ;
	}
	return FALSE ;
}
