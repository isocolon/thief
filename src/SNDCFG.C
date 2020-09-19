#include "thief.h"

LPSOUNDS LpSounds ;

BOOL CALLBACK SoundBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
    {
    static int nSel ;
    int nI, nC ;
    FILE *Fv ;
    char cTmp [2048] ;

    switch (iMsg)
        {
        case WM_INITDIALOG :
            if (! STATE_EnterDialogBox ())
                {
                EndDialog (hwnd, FALSE) ;
                return FALSE ;
                }

            LpSounds = (LPSOUNDS) HeapAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY, sizeof (SOUNDS)) ;
            for (nI = 0 ; nI < MAX_SOUND ; nI++)
                {
                LpSounds->bBeep [nI] = Sounds.bBeep [nI] ;
                strcpy (LpSounds->cName [nI], Sounds.cName [nI]) ;
                }

            if (User.bTelnetSound)
                {
                SendDlgItemMessage (hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_CHECKED, 0) ;
                }
            else
                {
                SendDlgItemMessage (hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_UNCHECKED, 0) ;
                }
                
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_RESETCONTENT, 0, 0) ;

            sprintf (cTmp, "%02d. Tell", SOUND_TELL) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Ptell", SOUND_PTELL) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Say", SOUND_SAY) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Kibitz", SOUND_KIBITZ) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Whisper", SOUND_WHISPER) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Shout", SOUND_SHOUT) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Cshout/Sshout", SOUND_CSHOUT) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Challenge", SOUND_CHALLENGE) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Abort Request", SOUND_ABORT_REQUEST) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Partnership Request", SOUND_PARTNER) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Notification", SOUND_NOTIFICATION) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Arrival", SOUND_ARRIVAL) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Departure", SOUND_DEPARTURE) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Game Notification", SOUND_GAME_NOTIFY) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Draw Offer", SOUND_DRAW_REQUEST) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Takeback Request", SOUND_TAKEBACK_REQUEST) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            sprintf (cTmp, "%02d. Adjourn Request", SOUND_ADJOURN_REQUEST) ;
            SendDlgItemMessage (hwnd, IDD_SOUND, LB_ADDSTRING, 0, (LPARAM) cTmp) ;

            SendDlgItemMessage (hwnd, IDD_SOUND, LB_SETCURSEL, 0, 0) ;

            if (LpSounds->bBeep [SOUND_TELL])
                {
                SendDlgItemMessage (hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
                }
            else
                {
                SendDlgItemMessage (hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
                }

            SendDlgItemMessage (hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpSounds->cName [SOUND_TELL]) ;
            nSel = SOUND_TELL - 1 ;

            TOOLBOX_CenterWindow (hwnd, GetWindow (hwnd, GW_OWNER)) ;
            return TRUE ;

        case WM_COMMAND:
            switch (LOWORD (wParam))
                {
                case IDD_SOUND :
                    if (HIWORD (wParam) == LBN_SELCHANGE)
                        {
                        nI = SendDlgItemMessage (hwnd, IDD_SOUND, LB_GETCURSEL, 0, 0) ;
                        nC = SendDlgItemMessage (hwnd, IDD_BEEP,  BM_GETSTATE,  0, 0) ;
                        
                        LpSounds->bBeep [nSel + 1] = (nC == BST_CHECKED) ;

                        SendDlgItemMessage (hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_SOUND_NAME, (LPARAM) cTmp) ;
                        strcpy (LpSounds->cName [nSel + 1], cTmp) ;

                        if (LpSounds->bBeep [nI + 1])
                            {
                            SendDlgItemMessage (hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
                            }
                        else
                            {
                            SendDlgItemMessage (hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
                            }

                        SendDlgItemMessage (hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpSounds->cName [nI + 1]) ;

                        nSel = nI ;
                        }
                    break ;

                case IDD_BROWSE :
                    Fv = TOOLBOX_OpenFileDialog (hwnd, FALSE, NULL, "wav", SOUND_FILT, "Wave File", NULL, NULL, cTmp, TOOLBOX_GetWavePath ()) ;
                    if (Fv != NULL)
                        {
                        fclose (Fv) ;
                        strcpy (cTmp, TOOLBOX_GetWaveFilename (cTmp)) ;
                        SendDlgItemMessage (hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) cTmp) ;
                        }
                    break ;

                case IDD_PLAY :
                    SendDlgItemMessage (hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_SOUND_NAME, (LPARAM) cTmp) ;
                    PlaySound (TOOLBOX_GetFullWaveFilename (cTmp), NULL, SND_FILENAME | SND_ASYNC) ;
                    break ;

                case IDOK :
                    nC = SendDlgItemMessage (hwnd, IDD_BEEP, BM_GETSTATE, 0, 0) ;
                        
                    LpSounds->bBeep [nSel + 1] = (nC == BST_CHECKED) ;

                    SendDlgItemMessage (hwnd, IDD_WAVE, WM_GETTEXT, (WPARAM) MAX_SOUND_NAME, (LPARAM) cTmp) ;
                    strcpy (LpSounds->cName [nSel + 1], cTmp) ;

                    nI = SendDlgItemMessage (hwnd, IDD_SOUND_APPLY, BM_GETSTATE, 0, 0) ;
                    if (nI == BST_CHECKED)
                        {
                        User.bTelnetSound = 1 ;
                        }
                    else
                        {
                        User.bTelnetSound = 0 ;
                        }
                    
                    for (nI = 1 ; nI < MAX_SOUND ; nI++)
                        {
                        Sounds.bBeep [nI] = LpSounds->bBeep [nI] ;
                        strcpy (Sounds.cName [nI], LpSounds->cName [nI]) ;
                        strcpy (Sounds.cName [nI], TOOLBOX_GetWaveFilename (Sounds.cName [nI])) ;
                        TOOLBOX_AllTrim (Sounds.cName [nI]) ;
                        }

                    HeapFree (GetProcessHeap (), 0, LpSounds) ;
                    EndDialog (hwnd, TRUE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDCANCEL :
                    HeapFree (GetProcessHeap (), 0, LpSounds) ;
                    EndDialog (hwnd, FALSE) ;
                    STATE_LeaveDialogBox () ;
                    return TRUE ;

                case IDD_SND_DEFAULT :
                    SendDlgItemMessage (hwnd, IDD_SOUND_APPLY, BM_SETCHECK, BST_CHECKED, 0) ;

                    for (nI = 0 ; nI < MAX_SOUND ; nI++)
                        {
                        LpSounds->bPlay [nI] = 0 ;
                        LpSounds->bBeep [nI] = 0 ;
                        strcpy (LpSounds->cName [nI], "") ;
                        LpSounds->pData [nI] = NULL ;
                        }

                    strcpy (LpSounds->cName [SOUND_TELL],             "WHISTLE.WAV") ;
                    strcpy (LpSounds->cName [SOUND_PTELL],            "DRIP.WAV") ;
                    strcpy (LpSounds->cName [SOUND_SAY],              "WHISTLE.WAV") ;
                    strcpy (LpSounds->cName [SOUND_CHALLENGE],        "ALERT.WAV") ;
					strcpy (LpSounds->cName [SOUND_DRAW_REQUEST],	  "COWBELL.WAV") ;
					strcpy (LpSounds->cName [SOUND_TAKEBACK_REQUEST], "COWBELL.WAV") ;
					strcpy (LpSounds->cName [SOUND_ADJOURN_REQUEST],  "COWBELL.WAV") ;
                    strcpy (LpSounds->cName [SOUND_ABORT_REQUEST],    "ABORTREQUEST.WAV") ;
                    strcpy (LpSounds->cName [SOUND_PARTNER],          "PARTNERSHIPREQUEST.WAV") ;

                    nI = SendDlgItemMessage (hwnd, IDD_SOUND, LB_GETCURSEL, 0, 0) ;
                    if (LpSounds->bBeep [nI + 1])
                        {
                        SendDlgItemMessage (hwnd, IDD_BEEP, BM_SETCHECK, BST_CHECKED, 0) ;
                        }
                    else
                        {
                        SendDlgItemMessage (hwnd, IDD_BEEP, BM_SETCHECK, BST_UNCHECKED, 0) ;
                        }

                    SendDlgItemMessage (hwnd, IDD_WAVE, WM_SETTEXT, 0, (LPARAM) LpSounds->cName [nI + 1]) ;
                    return TRUE ;
                }
            break ;
        }
    return FALSE ;
    }
