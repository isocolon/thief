#include "thief.h"

BOOL CALLBACK PasswordBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static HWND hText, hVar ;
    char cTmp [2048] ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            hText = GetDlgItem (hwnd, IDD_PWD_TEXT) ;
            hVar  = GetDlgItem (hwnd, IDD_PWD_DIALOG_TEXT) ;

			strcpy(cTmp,"\"");
			strncat(cTmp,Login.cLoginHandle,80);
			strncat(cTmp,"\"",1);
			strncat(cTmp,FICS_REGISTERED_LINE1,80);
			strncat(cTmp,"\n",1);
			strncat(cTmp,FICS_REGISTERED_LINE2,80);
			SetWindowText (hText, "") ;

			SendDlgItemMessage (hwnd, IDD_PWD_DIALOG_TEXT, WM_SETTEXT, (WPARAM) strlen (cTmp), (LPARAM) cTmp) ;
            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDOK :
                    SendDlgItemMessage (hwnd, IDD_PWD_TEXT, WM_GETTEXT, (WPARAM) 255, (LPARAM) cTmp) ;
                    strcpy(Login.cLoginPassword,cTmp) ;
                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDCANCEL :
					strcpy(Login.cLoginPassword,"");
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return FALSE;
                }
            break ;
        }
    return FALSE ;
    }
