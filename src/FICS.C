#include "thief.h"

void FICS_ProcessBeforeLoginLine (char *cS)
    {
    int      nI, nJ ;
    char     cTmp [255], cTmp1 [255] ;
    COLORREF nC ;
	FARPROC  LpProc ;

    if (cS [0] == '\\')
        {
        if (Telnet.bDisplayContinueLine)
            {
            Telnet.clrCurrent    = Telnet.clrLastLine ;
            Telnet.nCurrentSound = SOUND_NONE ;
            TELNET_Write (cS) ;
            }
        return ;
        }
    Telnet.bDisplayContinueLine = 1 ;

    if (cS [1] == 'i' && cS [2] == 'c' && cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
        {
        cS += FICS_PROMPT_LENGTH ;
        }

    while (*cS & 0x80)
        {
        cS++ ;
        }
	
    if (TELNET_MatchExp (cS, FICS_LOGIN))
        {
        if (Timeseal.nSocketLogin == 0)
            {
            nC = Telnet.clrCurrent ;
            Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
            TELNET_Write (cS) ;

            strcpy (cTmp, Login.cLoginHandle) ;
            strcat (cTmp, "\n") ;
            Telnet.clrCurrent = clrColor [CLR_TELNET_USERTEXT] ;
            TELNET_ItalicWrite (cTmp) ;

            Telnet.clrCurrent = nC ;

            TOOLBOX_WriteICS (Login.cLoginHandle) ;
            TOOLBOX_WriteICS ("\n") ;

            Timeseal.nSocketLogin++ ;
            strcpy (Vars.cWhoAmI, Login.cLoginHandle) ;

            TOOLBOX_SetTelnetCaption () ;

            User.bIamAGuest = 0 ;
            TOOLBOX_CheckGuestMenu () ;
            }
        else
            {
            TOOLBOX_WriteSystem (ICS_CONN_LOST_DISPLAY) ;
            TOOLBOX_CloseSocket () ;
            }
        return ;
        }

    if (TELNET_MatchExp (cS, FICS_PASSWORD))
        {
        strcpy (cTmp, cS) ;

        nJ = strlen (Login.cLoginPassword) ;

        Telnet.bFoundHandle = (nJ > 0) ;

        cTmp1 [0] = NULL_CHAR ;

        for (nI = 0 ; nI < nJ ; nI++)
            {
            cTmp1 [nI]     = '*' ;
            cTmp1 [nI + 1] = NULL_CHAR ;
            }

        if (nJ == 0)
			{
			// If connection profile contains no password for registered account, ask for it
			LpProc = MakeProcInstance ((FARPROC) PasswordBoxWndProc, hInst) ;
			DialogBox (hInst, "PasswordBox", hwndWindow [HWND_CLIENT], (DLGPROC) LpProc) ;
			FreeProcInstance (LpProc) ;
			SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
			nJ = strlen (Login.cLoginPassword) ;
			}
        
        strcat (cTmp, cTmp1) ;
        strcat (cTmp, "\n") ;

        nC = Telnet.clrCurrent ;
        Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
        TELNET_Write (cTmp) ;
        Telnet.clrCurrent = nC ;

        if (nJ > 0)
            {
            TOOLBOX_WriteICS (Login.cLoginPassword) ;
            TOOLBOX_WriteICS ("\n") ;
            FICS_SendBeforeTwelve();
            }
        else
            {
            TOOLBOX_WriteSystem ("\nMissing Password\n") ;
            TOOLBOX_WriteSystem (ICS_CONN_LOST_DISPLAY) ;
            TOOLBOX_CloseSocket () ;
            }
        return ;
        }

    if (TELNET_MatchExp (cS, FICS_ENTER))
        {
        Telnet.bFoundHandle = 1 ;

        nJ = 0 ;
        for (nI = 37 ; nI < ((int) strlen (cS)) ; nI++)
            {
            if (cS [nI] == '"')
                {
                break ;
                }
            Vars.cWhoAmI [nJ++] = cS [nI] ;
            }
        Vars.cWhoAmI [nJ] = NULL_CHAR ;

        TOOLBOX_SetTelnetCaption () ;

        strcpy (cTmp, cS) ;
        strcat (cTmp, "\n") ;

        nC = Telnet.clrCurrent ;
        Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
        TELNET_Write (cTmp) ;
        Telnet.clrCurrent = nC ;
        FICS_SendBeforeTwelve();

        User.bIamAGuest = 1 ;
        TOOLBOX_CheckGuestMenu () ;
        return ;
        }

    if (Telnet.bFoundHandle)
        {
        if (TELNET_MatchExp (cS, FICS_LOGIN_GUEST))
            {
            strcpy (cTmp, cS) ;
            strcat (cTmp, "\n") ;

            nC = Telnet.clrCurrent ;
            Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
            TELNET_Write (cTmp) ;
            Telnet.clrCurrent = nC ;
            return ;
            }
        }
    else    
        {
        if (TELNET_MatchExp (cS, FICS_LOGIN_GUEST))
            {
            Telnet.bFoundHandle = 1 ;

            nJ = 0 ;
            for (nI = 35 ; nI < ((int) strlen (cS)) ; nI++)
                {
                if (cS [nI] == '"')
                    {
                    break ;
                    }
                Vars.cWhoAmI [nJ++] = cS [nI] ;
                }
            Vars.cWhoAmI [nJ] = NULL_CHAR ;

            TOOLBOX_SetTelnetCaption () ;

            strcpy (cTmp, cS) ;
            strcat (cTmp, "\n") ;

            nC = Telnet.clrCurrent ;
            Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
            TELNET_Write (cTmp) ;
            Telnet.clrCurrent = nC ;
            FICS_SendBeforeTwelve();

            User.bIamAGuest = 1 ;
            TOOLBOX_CheckGuestMenu () ;
            return ;
            }
        }

    //if (TELNET_MatchExp (cS, FICS_STYLE_12_SET ) ||

    if (strstr (cS, FICS_STYLE_12_SET))
        {
        nC = Telnet.clrCurrent ;
        Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
        TELNET_Write (cS) ;
        Telnet.clrCurrent = nC ;

        Timeseal.bBeforeLogin = 0 ;
        return ;
        }
	
    if (FICS_ColorIndex (cS))
        {
        TELNET_Write (cS) ;

        if (! EAR_Play (cS))
            {
            SOUND_Play () ;
            }
        Telnet.clrLastLine = Telnet.clrCurrent ;
        }
    else
        {
        if (Telnet.bDisplayLine)
            {
            TELNET_Write (cS) ;
            SOUND_Play () ;

            Telnet.clrLastLine = Telnet.clrCurrent ;
            }
        }
    }

void FICS_SendBeforeTwelve (void)
    {
    TOOLBOX_WriteICS ("\n") ;

    TOOLBOX_WriteICS (FICS_SET_INTERFACE_COMMAND) ;
	TOOLBOX_WriteICS (THIEF_VER) ;
    TOOLBOX_WriteICS ("\n") ;

    TOOLBOX_WriteICS (FICS_SET_BELLOFF_COMMAND) ;
    TOOLBOX_WriteICS (FICS_SET_PROMPT_COMMAND) ;
    TOOLBOX_WriteICS (FICS_SET_BELL_0_COMMAND) ;
    TOOLBOX_WriteICS (FICS_SET_PTIME_0_COMMAND) ;
    TOOLBOX_WriteICS (FICS_SET_STYLE12_COMMAND) ;
    TOOLBOX_WriteICS (FICS_SET_ISET_GI_1_COMMAND) ;
	TOOLBOX_WriteICS (FICS_SET_ISET_TP_1_COMMAND);
    TOOLBOX_IssueISet (1, 1, 1, 1) ;
	TOOLBOX_WriteICS (FICS_SET_ISET_LOCK_COMMAND);
    }

void FICS_ProcessLine (char *cS)
    {
    HDC  hdc ;
    int  nG, nI, nJ, nPartner, bPlaying, bPartner, bFound, bBug ;
    char *cP, *cQ, cTmp [2048], cNumber [30], cMove1 [500], cMove2 [500], cWRating [500], cWHandle [500], cBRating [500], cBHandle [500] ;

    if (cS [0] == BELL_CHAR)
        {        
        cS++ ;
        }

    if (cS [1] == 'i' && cS [2] == 'c' && cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
        {        
//        cS += FICS_PROMPT_LENGTH ;
//
//		if (cS [1] == 'i' && cS [2] == 'c' && cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
//            {
//            cS += FICS_PROMPT_LENGTH ;
//            }

// we want to filter out all prompts, even consecutive ones
		while (cS [1] == 'i' && cS [2] == 'c' && cS [3] == 's' && cS [4] == '%' && cS [5] == ' ')
           {
           cS += FICS_PROMPT_LENGTH ;
           }

        if ((cS [0] == NULL_CHAR) || (cS[0] == '\n' && cS [1] == NULL_CHAR))
            {
            Telnet.bTelnetSkipLine = 1 ;
            return ;
            }
        }
    else
        {
        if ((cS [0] == NULL_CHAR) || (cS [0] == '\n' && cS [1] == NULL_CHAR))
            {
            if (! Telnet.bTelnetSkipLine)
                {
                TELNET_Write (cS) ;
                }
            return ;
            }
        }

    if (Telnet.bHasESC)
        {
        nI = 0 ;
        nJ = 0 ;
        nG = strlen (cS) ;

        while (nI < nG)
            {
            if (cS [nI] == 27)
                {
                nI = nI + 4 ;
                }
            else
                {
                cTmp [nJ] = cS [nI] ;
                nI++ ;
                nJ++ ;
                }
            }

        cTmp [nJ] = NULL_CHAR ;

        strcpy (cS, cTmp) ;

        Telnet.bHasESC = 0 ;
        }

    if (! strncmp (cS, FICS_G1_INIT, 5))
        {
        strcpy (Telnet.cLastGameInfo, cS) ;
        Telnet.bTelnetSkipLine = 1 ;
        return ;
        }

    if (! strncmp (cS, FICS_12_INIT, 5))
        {
        if (FICS_ParseBoard (cS))
            {
            Telnet.bTelnetSkipLine = 1 ;
            return ;
            }		
        if (User.bEat12GameNotFoundLine)
            {
            Telnet.bTelnetSkipLine = 1 ;
            return ;
            }
        }

    if (! strncmp (cS, FICS_B1_INIT, 5))   
        {
        if (FICS_ParsePiece (cS))
            {
            Telnet.bTelnetSkipLine = 1 ;
            return ;
            }
        if (User.bEatb1GameNotFoundLine)
            {
            Telnet.bTelnetSkipLine = 1 ;
            return ;
            }
        }

    Telnet.bTelnetSkipLine = 0 ;

    if (! strncmp (cS, FICS_ILLEGAL_MOVE_1,  13) ||
        ! strncmp (cS, FICS_ILLEGAL_MOVE_2,  16) ||
        ! strncmp (cS, FICS_ILLEGAL_MOVE_3,  21) ||
        ! strncmp (cS, FICS_NOT_YOUR_TURN_1, 20) ||
        ! strncmp (cS, FICS_NOT_YOUR_TURN_2, 18))
        {
        if (Game [INDEX_PLAY].bPlaying)
            {
            TOOLBOX_WriteICS (FICS_REFRESH_COMMAND) ;
            TOOLBOX_WriteICS (Game [INDEX_PLAY].cHandle [INDEX_WHITE]) ;
            TOOLBOX_WriteICS ("\n") ;
            }
        else
            {
	    TOOLBOX_WriteICS (FICS_REFRESH_COMMAND1) ;
            }
        TELNET_NormalPrint (cS) ;
        return ;
        }

    if (! strncmp (cS, FICS_PART_PLAYING_GAME, 29))
        {
        sscanf (cS + 29, "%d", &nG) ;
        Game [INDEX_PLAY].nGamePartner = nG ;
        TELNET_NormalPrint (cS) ;
        return ;
        }

    if (! strncmp (cS, FICS_STOP_OBS_GAME, 14))
        {
        TELNET_NormalPrint (cS) ;

        sscanf (cS + 14, "%d", &nG) ;

        for (nI = 1 ; nI < MAX_GAME ; nI++)
            {
            if (Game [nI].nGameNumber == nG)
                {
                Game [nI].bFirstGame  = 0 ;
                Game [nI].nGameNumber = 0 ;
                Game [nI].bPlaying    = 0 ;
                Game [nI].bTickClock  = 1 ;

                if (IsWindow (Game [nI].hwnd))
                    {
                    SendMessage (Game [nI].hwnd, WM_CLOSE, 0, 0) ;
                    }

                System.bUnobserve           = 0 ;
                System.nUnobGameNumber      = 0 ;
                strcpy (System.cUnobWname, "") ;
                strcpy (System.cUnobBname, "") ;

                if (Game [INDEX_PLAY].nGamePartner == nG)
                    {
                    Game [INDEX_PLAY].nGamePartner = 0 ;
                    Game [nI].nGamePartner = 0 ;

                    STATE_ObserveEndGame (nI, 1, 1) ;
                    }
                else
                    {
                    Game [nI].nGamePartner = 0 ;

                    STATE_ObserveEndGame (nI, 0, 1) ;
                    }
                break ;
                }
            }
        return ;
        }
    if (strstr (cS, FICS_I_ABORTED))
		{
        if (Game [INDEX_PLAY].bPlaying)
			{
			Game [INDEX_PLAY].nAbortStatus = 2;
			}
        TELNET_NormalPrint (cS) ;
        return ;
		}
    if (strstr (cS, FICS_OPPONENT_ABORTED))
		{
        if (Game [INDEX_PLAY].bPlaying)
			{
			Game [INDEX_PLAY].nAbortStatus = 4;
			}
        TELNET_NormalPrint (cS) ;
        return ;
		}

    if (! strncmp (cS, FICS_GAME_NUMBER_INIT, 6))
        {
        TELNET_NormalPrint (cS) ;

        sscanf (cS + 6, "%d", &nG) ;

        cP = strchr (cS, ')') ;
        cQ = strchr (cS, '}') ;

        for (nI = 0 ; nI < MAX_GAME ; nI++)
            {
            if (Game [nI].nGameNumber == nG)
                {
                bPlaying = Game [nI].bPlaying ;
                nPartner = Game [nI].nGamePartner ;

                Game [nI].bFirstGame  = 0 ;
                Game [nI].nGameNumber = 0 ;
                Game [nI].bPlaying    = 0 ;
                Game [nI].bTickClock  = 1 ;
        
                strcpy (Game [nI].cOrgResult, cS) ;

                if (nI == INDEX_PLAY)
                    {
                    System.bIsMyTurn = 0 ;

                    hdc = GetDC (Game [nI].hwnd) ;
                    BOARD_DrawBoard (nI, Game [nI].hwnd, hdc, DRAW_STATE_ERASE_ALL_TP) ;
                    ReleaseDC (Game [nI].hwnd, hdc) ;

                    Premove.nPremoveCount     = 0 ;
                    Premove.nPremoveHead      = 0 ;
                    Premove.nPremoveTail      = 0 ;
                    Premove.bLastTP           = 0 ;
                    Premove.ptLastTP [0].x    = -1 ;
                    Premove.ptLastTP [0].y    = -1 ;
                    Premove.ptLastTP [1].x    = -1 ;
                    Premove.ptLastTP [1].y    = -1 ;
                    Premove.bIllegalTP        = 0 ;
                    Premove.ptIllegalTP [0].x = -1 ;
                    Premove.ptIllegalTP [0].y = -1 ;
                    Premove.ptIllegalTP [1].x = -1 ;
                    Premove.ptIllegalTP [1].y = -1 ;

                    Game [INDEX_PLAY].nGamePartner = 0 ;

                    PostMessage (Game [nI].hwnd, WM_USER_MOUSELEAVE, 0, 0) ;
                    }
                else
                    {
                    if (Game [INDEX_PLAY].nGamePartner == nG)
                        {
                        Game [INDEX_PLAY].nGamePartner = 0 ;
                        Game [nI].nGamePartner = 0 ;

                        bPartner = 1 ;
                        PostMessage (Game [nI].hwnd, WM_USER_MOUSELEAVE, 0, 0) ;
                        }
                    else
                        {
                        Game [nI].nGamePartner = 0 ;

                        bPartner = 0 ;
                        }
                    }

                if (cP && cQ)
                    {
                    (void) CLOCK_StopClockTimer (nI) ;

                    strncpy (Game [nI].cResult, cP + 1, cQ - cP + 1) ;

                    cQ = strchr (Game [nI].cResult, '}') ;
                    if (cQ != NULL)
                        {
                        *cQ = NULL_CHAR ;
                        }

                    strcpy (Vars.cPartnerTell, "") ;

                    TOOLBOX_AllTrim (Game [nI].cResult) ;

                    hdc = GetDC (Game [nI].hwnd) ;
                    BOARD_DrawResult (nI, hdc) ;
                    ReleaseDC (Game [nI].hwnd, hdc) ;
                    if (bPlaying)
                        {
                        if (nI == INDEX_PLAY)
                            {
                            if (! GAMESOUND_PlayBoard (cS))
                                {
                                GAMESOUND_Play (GAME_SOUND_PLAY_OVER) ;
                                }

                            for (nJ = 0 ; nJ < MAX_TIME_SOUND ; nJ++)
                                {
                                TimeSounds.bPlayed [nJ] = 0 ;
                                }
                            TimeSounds.nLastSecond = -500 ;

                            Premove.nPremoveCount  = 0 ;
                            Premove.nPremoveHead   = 0 ;
                            Premove.nPremoveTail   = 0 ;
                            Premove.bLastTP        = 0 ;
                            Premove.ptLastTP [0].x = -1 ;
                            Premove.ptLastTP [0].y = -1 ;
                            Premove.ptLastTP [1].x = -1 ;
                            Premove.ptLastTP [1].y = -1 ;
                            }
                        else
                            {
                            if (Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
                                {
                                if ((stricmp (Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
                                    (stricmp (Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
                                    {
                                    if (! GAMESOUND_PartnerBoard (cS))
                                        {
                                        GAMESOUND_Play (GAME_SOUND_OBS_OVER1) ;
                                        }
                                    }
                                else
                                    {
                                    GAMESOUND_Play (GAME_SOUND_OBS_OVER1) ;
                                    }
                                }
                            else
                                {
                                if ((stricmp (Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
                                    (stricmp (Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
                                    {
                                    if (! GAMESOUND_PartnerBoard (cS))
                                        {
                                        GAMESOUND_Play (GAME_SOUND_OBS_OVER2) ;
                                        }
                                    }
                                else
                                    {
                                    GAMESOUND_Play (GAME_SOUND_OBS_OVER2) ;
                                    }
                                }
                            }
                        }

                    if (! Fics.bPobserve)
                        {
                        if (Fics.nPobserve == nG)
                            {
                            Fics.bPobserve = 1 ;
                            Fics.nPobserve = 0 ;
                            strcpy (Fics.cPobserve, "") ;
                            }
                        }
                    }

                if (nI == INDEX_PLAY)
                    {
                    bFound = 0 ;

                    if (User.bFICSPlayEndPartner)
                        {
                        if (bPlaying)
                            {
                            if (strlen (Vars.cPartner) > 0)
                                {
                                for (nI = 1 ; nI < MAX_GAME ; nI++)
                                    {
                                    if (Game [nI].bValid)
                                        {
                                        if (Game [nI].nGameNumber > 0)
                                            {
                                            if (Game [nI].nGameNumber == nPartner)
                                                {
                                                if (Game [nI].bPlaying)
                                                    {
                                                    if ((stricmp (Game [nI].cHandle [INDEX_WHITE], Vars.cPartner) == 0) ||
                                                        (stricmp (Game [nI].cHandle [INDEX_BLACK], Vars.cPartner) == 0))
                                                        {
                                                        Game [nI].bFirstGame   = 0 ;
                                                        Game [nI].bPlaying     = 0 ;
                                                        Game [nI].nGameNumber  = 0 ;
                                                        Game [nI].nGamePartner = 0 ;
                                                        Game [nI].bTickClock   = 1 ;

                                                        (void) CLOCK_StopClockTimer (nI) ;

                                                        strcpy (Game [nI].cResult, Game [INDEX_PLAY].cResult) ;

                                                        hdc = GetDC (Game [nI].hwnd) ;
                                                        BOARD_DrawResult (nI, hdc) ;
                                                        ReleaseDC (Game [nI].hwnd, hdc) ;

                                                        bFound = nI ;
                                                        break ;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        
                    STATE_PlayEndGame () ;

                    if (bFound)
                        {
                        STATE_ObserveEndGame (bFound, 1, 0) ;
                        }
                    }
                else
                    {
                    STATE_ObserveEndGame (nI, bPartner, 0) ;
                    }
                break ;
                }
            }
        return ;
        }

    if (! strncmp (cS, FICS_STOP_EXA_GAME, 33))
        {
        sscanf (cS + 33, "%d", &nG) ;

        for (nI = 0 ; nI < MAX_GAME ; nI++)
            {
            if (Game [nI].nGameNumber == nG)
                {
                Game [nI].bFirstGame   = 0 ;
                Game [nI].nGameNumber  = 0 ;
                Game [nI].nGamePartner = 0 ;
                Game [nI].bPlaying     = 0 ;
                Game [nI].bTickClock   = 1 ;

                System.bUnobserve          = 0 ;
                System.nUnobGameNumber     = 0 ;
                strcpy (System.cUnobWname, "") ;
                strcpy (System.cUnobBname, "") ;

                if (nI == INDEX_PLAY)
                    {
                    (void) CLOCK_StopClockTimer (INDEX_PLAY) ;

                    GAME_InitialSetup (INDEX_PLAY) ;

                    TOOLBOX_InvalidateBoardWindow (0) ;

                    if (! User.bShowMoveButton)
                        {
                        TOOLBOX_ShowMoveButtons (INDEX_PLAY, 0) ;
                        }

                    TOOLBOX_RedrawBoard (INDEX_PLAY) ;
                    SetWindowText (Game [INDEX_PLAY].hwnd, TOOLBOX_GetGameWindowTitle (INDEX_PLAY)) ;
                    }
                break ;
                }
            }
        TELNET_NormalPrint (cS) ;
        return ;
        }

    if (cS [0] == '\\')
        {
        if (Telnet.bDisplayContinueLine)
            {
            Telnet.clrCurrent    = Telnet.clrLastLine ;
            Telnet.nCurrentSound = SOUND_NONE ;
            TELNET_Write (cS) ;
            }
        return ;
        }
    Telnet.bDisplayContinueLine = 1 ;
        
//
// You will now be following AcCoNcIo's partner's games.
// You will now be following AcCoNcIo's games.
//
    if (! strncmp (cS, FICS_HEARED_FOLLOW, 26))
        {
        sscanf (cS, "%s %s %s %s %s %s %s", &cTmp, &cTmp, &cTmp, &cTmp, &cTmp, &cWHandle, &cBHandle) ;

        nI = strlen (cWHandle) ;
        if ((cWHandle [nI - 1] == 's' ) && (cWHandle [nI - 2] == '\''))
            {
            cWHandle [nI - 2] = NULL_CHAR ;
            }
        cWHandle [ICS_HANDLE_LENGTH] = NULL_CHAR ;

        if (stricmp (cBHandle, "partner's") == 0)
            {

            // pfollow
            strcpy (Vars.cFollow,  "") ;
            strcpy (Vars.cPfollow, cWHandle) ;
            }
        else
            {

            // follow
            strcpy (Vars.cFollow,  cWHandle) ;
            strcpy (Vars.cPfollow, "") ;
            }

        sprintf (Vars.cFollowGone, FICS_FOLLOW_GONE_STRING, cWHandle) ;
        Vars.nFollowGone = strlen (Vars.cFollowGone) ;

        sprintf (Vars.cFollowGone1, "") ;
        Vars.nFollowGone1 = 0 ;

        strcpy (Vars.cFollowOn, "") ;
        Vars.nFollowOn = 0 ;

        TELNET_NormalPrint (cS) ;
        TOOLBOX_SetTelnetCaption () ;
        return ;
        }
        
//
// You will not follow any player's games.
//
    if (! strncmp (cS, FICS_HEARED_NO_FOLLOW, 39))
        {
        strcpy (Vars.cFollow,      "") ;
        strcpy (Vars.cPfollow,     "") ;
        strcpy (Vars.cFollowGone,  "") ;
        strcpy (Vars.cFollowGone1, "") ;
        strcpy (Vars.cFollowOn,    "") ;

        Vars.nFollowGone  = 0 ;
        Vars.nFollowGone1 = 0 ;
        Vars.nFollowOn    = 0 ;

        TELNET_NormalPrint (cS) ;
        TOOLBOX_SetTelnetCaption () ;
        return ;
        }

//
// GuestNVMF, whose games you were following, has logged out.
//
    if (Vars.nFollowGone > 0)
        {
        if (! strncmp (cS, Vars.cFollowGone, Vars.nFollowGone))
            {
            strcpy (Vars.cFollow,      "") ;
            strcpy (Vars.cPfollow,     "") ;
            strcpy (Vars.cFollowGone,  "") ;
            strcpy (Vars.cFollowGone1, "") ;
            strcpy (Vars.cFollowOn,    "") ;

            Vars.nFollowGone  = 0 ;
            Vars.nFollowGone1 = 0 ;
            Vars.nFollowOn    = 0 ;

            TELNET_NormalPrint (cS) ;
            TOOLBOX_SetTelnetCaption () ;
            return ;
            }
        }

//
// Game 38: WhoAmI goes forward 1 move.
// Game 16: GuestHHCK sets white's rating to 100.
// Game 16: GuestHHCK sets black's rating to 200.
// Game notification: LSS (2171) vs. killabug (2102) rated bughouse 2 0: Game 116
//
    if (! strncmp (cS, FICS_GAME_INFORMATION, 5))
        {
        if (strncmp (cS, FICS_HEARED_GAME_NOTIFY, 19))
            {
            sscanf (cS + 5, "%d", &nG) ;

            for (nI = 0 ; nI < MAX_GAME ; nI++)
                {
                if (Game [nI].nGameNumber == nG)
                    {
                    if (strstr (cS, FICS_SET_RATING_WHITE1))
                        {
                        sscanf (cS, FICS_SET_RATING_WHITE2, &nG, &cTmp, &cWRating) ;

                        nJ = strlen (cWRating) ;
                        cWRating [nJ - 1] = ')' ;

                        strcpy (Game [nI].cRating [INDEX_WHITE], "(") ;
                        strcat (Game [nI].cRating [INDEX_WHITE], cWRating) ;

                        hdc = GetDC (Game [nI].hwnd) ;
                        BOARD_DrawHandle     (nI, hdc) ;
                        BOARD_DrawGameType   (nI, hdc) ;
                        BOARD_DrawLastMove   (nI, hdc) ;
                        BOARD_DrawWhiteClock (nI, hdc) ;
                        BOARD_DrawBlackClock (nI, hdc) ;
                        ReleaseDC (Game [nI].hwnd, hdc) ;

                        TELNET_NormalPrint (cS) ;
                        }
                    else
                    if (strstr (cS, FICS_SET_RATING_BLACK1))
                        {
                        sscanf (cS, FICS_SET_RATING_BLACK2, &nG, &cTmp, &cBRating) ;

                        nJ = strlen (cBRating) ;
                        cBRating [nJ - 1] = ')' ;

                        strcpy (Game [nI].cRating [INDEX_BLACK], "(") ;
                        strcat (Game [nI].cRating [INDEX_BLACK], cBRating) ;

                        hdc = GetDC (Game [nI].hwnd) ;
                        BOARD_DrawHandle     (nI, hdc) ;
                        BOARD_DrawGameType   (nI, hdc) ;
                        BOARD_DrawLastMove   (nI, hdc) ;
                        BOARD_DrawWhiteClock (nI, hdc) ;
                        BOARD_DrawBlackClock (nI, hdc) ;
                        ReleaseDC (Game [nI].hwnd, hdc) ;

                        TELNET_NormalPrint (cS) ;
                        }
                    else
                    if (User.bShowResult || User.bShowPtell)
                        {
                        hdc = GetDC (Game [nI].hwnd) ;
                        BOARD_DrawGinfo (nI, hdc, cS) ;
                        ReleaseDC (Game [nI].hwnd, hdc) ;

                        Telnet.bTelnetSkipLine = 1 ;
                        }
                    return ;
                    }
                }
            }
        }

//
// Promotion piece set to QUEEN.
// Promotion piece set to KNIGHT.
// Promotion piece set to BISHOP.
// Promotion piece set to ROOK.
// Promotion piece set to KING.
//
    if (! strncmp (cS, FICS_PROMOTE_PIECE, 23))
        {
        TELNET_NormalPrint (cS) ;

        System.bPromoteCommand = 1 ;
        System.bPromoteKnight  = 0 ;

        sscanf (cS + 23, "%s", &cTmp) ;

        if (! strncmp (cTmp, "QUEEN.", 6))
            {
            System.nPromoteWPiece = WHITE_QUEEN ;
            System.nPromoteBPiece = BLACK_QUEEN ;
            }
        else
        if (! strncmp (cTmp, "KNIGHT.", 7))
            {
            System.nPromoteWPiece = WHITE_KNIGHT ;
            System.nPromoteBPiece = BLACK_KNIGHT ;
            }
        else
        if (! strncmp (cTmp, "BISHOP.", 7))
            {
            System.nPromoteWPiece = WHITE_BISHOP ;
            System.nPromoteBPiece = BLACK_BISHOP ;
            }
        else
        if (! strncmp (cTmp, "ROOK.", 5))
            {
            System.nPromoteWPiece = WHITE_ROOK ;
            System.nPromoteBPiece = BLACK_ROOK ;
            }
        else
        if (! strncmp (cTmp, "KING.", 5))
            {
            System.nPromoteWPiece = WHITE_KING ;
            System.nPromoteBPiece = BLACK_KING ;
            }
        else
            {
            System.nPromoteWPiece = WHITE_QUEEN ;
            System.nPromoteBPiece = BLACK_QUEEN ;
            }
        return ;
        }

    if (System.nIssuedMovesCommand > 0)
        {
        if (! strncmp (cS, FICS_MOVELIST_GAME, 18))
            {
            sscanf (cS + 18, "%d", &nG) ;

            System.nMoveListIndexNumber = -1 ;

            for (nI = 0 ; nI < MAX_GAME ; nI++)
                {
                if (Game [nI].nGameNumber == nG)
                    {
                    System.nMoveListIndexNumber = nI ;

                    Game [nI].bFirstGame = 0 ;
                    break ;
                    }
                }

            if (System.nMoveListIndexNumber == -1)
                {
                System.nIssuedMovesCommand  = System.nIssuedMovesCommand - 1 ;
                System.nMoveListIndexNumber = -1 ;
                }

            Telnet.bTelnetSkipLine = 1 ;
            return ;
            }

        if (System.nMoveListIndexNumber >= 0)
            {
            nI = System.nMoveListIndexNumber ;

            Game [nI].bFirstGame = 0 ;

            if (strlen (cS) > 20)
                {
                //
                //OBIT (1694) vs. boffo (1833) --- Thu Feb  7, 21:11 PST 2002
                //Rated bughouse match, initial time: 3 minutes, increment: 0 seconds.
                //
                //Ifrikkinownyou (UNR) vs. Murr (1837) --- Thu Feb  7, 22:03 PST 2002
                //Unrated crazyhouse match, initial time: 1 minutes, increment: 0 seconds.
                //
                //Unrated bughouse match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated crazyhouse match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated suicide match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated lightning match, initial time: 1 minutes, increment: 0 seconds.
                //Unrated blitz match, initial time: 5 minutes, increment: 0 seconds.
                //Unrated standard match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/0 match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/1 match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/2 match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/3 match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/4 match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/5 match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/8 match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/8a match, initial time: 60 minutes, increment: 0 seconds.
                //Unrated wild/fr match, initial time: 60 minutes, increment: 0 seconds.
                //

                sscanf (cS, "%s %s %s %s %s", &cWHandle, &cWRating, &cTmp, &cBHandle, &cBRating) ;

                if (stricmp (cTmp, "vs.") == 0)
                    {
                    if ((stricmp (Game [nI].cHandle [INDEX_WHITE], cWHandle) == 0) &&
                        (stricmp (Game [nI].cHandle [INDEX_BLACK], cBHandle) == 0))
                        {
                        strcpy (Game [nI].cRating [INDEX_WHITE], cWRating) ;
                        strcpy (Game [nI].cRating [INDEX_BLACK], cBRating) ;

                        Telnet.bTelnetSkipLine = 1 ;
                        //TELNET_NormalPrint (cS) ;
                        return ;
                        }
                    }
                else
                    {
                    if (stricmp (cWHandle, "Rated") == 0)
                        {
                        Game [nI].nRated = 1 ;
                        bFound           = 1 ;
                        }
                    else
                    if (stricmp (cWHandle, "Unrated") == 0)
                        {
                        Game [nI].nRated = 0 ;
                        bFound           = 1 ;
                        }
                    else
                        {
                        bFound = 0 ;
                        }

                    if (bFound)
                        {
                        strcpy (Game [nI].cOrgGameType, cWRating) ;

                        if (stricmp (cWRating, "bughouse") == 0)
                            {
                            strcpy (Game [nI].cGameType, "bug") ;
                            Game [nI].nGameType  = GAMETYPE_BUGHOUSE ;
                            Game [nI].bChessGame = 0 ;
                            bBug = 1 ;
                            }
                        else
                            {
                            if (stricmp (cWRating, "crazyhouse") == 0)
                                {
                                strcpy (Game [nI].cGameType, "zh") ;
                                Game [nI].nGameType  = GAMETYPE_CRAZYHOUSE ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "suicide") == 0)
                                {
                                strcpy (Game [nI].cGameType, cWRating) ;
                                Game [nI].nGameType  = GAMETYPE_SUICIDE ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "lightning") == 0)
                                {
                                strcpy (Game [nI].cGameType, cWRating) ;
                                Game [nI].nGameType  = GAMETYPE_FICS_LIGHTNING ;
                                Game [nI].bChessGame = 1 ;
                                BOARD_ResetPromoteBoard (nI) ;
                                }
                            else
                            if (stricmp (cWRating, "blitz") == 0)
                                {
                                strcpy (Game [nI].cGameType, cWRating) ;
                                Game [nI].nGameType  = GAMETYPE_FICS_BLITZ ;
                                Game [nI].bChessGame = 1 ;
                                BOARD_ResetPromoteBoard (nI) ;
                                }
                            else
                            if (stricmp (cWRating, "standard") == 0)
                                {
                                strcpy (Game [nI].cGameType, cWRating) ;
                                Game [nI].nGameType  = GAMETYPE_FICS_STANDARD ;
                                Game [nI].bChessGame = 1 ;
                                BOARD_ResetPromoteBoard (nI) ;
                                }
                            else
                            if (stricmp (cWRating, "wild/0") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w0") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILD0 ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/1") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w1") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILD1 ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/2") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w2") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILD2 ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/3") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w3") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILD3 ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/4") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w4") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILD4 ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/5") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w5") ;
                                Game [nI].nGameType  = GAMETYPE_WILD5 ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/8") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w8") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILD8 ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/8a") == 0)
                                {
                                strcpy (Game [nI].cGameType, "w8a") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILD8A ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "wild/fr") == 0)
                                {
                                strcpy (Game [nI].cGameType, "wfr") ;
                                Game [nI].nGameType  = GAMETYPE_FICS_WILDFR ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                            if (stricmp (cWRating, "atomic") == 0)
                                {
                                strcpy (Game [nI].cGameType, cWRating) ;
                                Game [nI].nGameType  = GAMETYPE_FICS_ATOMIC ;
                                Game [nI].bChessGame = 0 ;
                                }
                            else
                                {
                                strcpy (Game [nI].cGameType, cWRating) ;
                                Game [nI].nGameType  = GAMETYPE_CHESS ;
                                Game [nI].bChessGame = 1 ;
                                BOARD_ResetPromoteBoard (nI) ;
                                }
                            bBug = 0 ;
                            }

                        hdc = GetDC (Game [nI].hwnd) ;
                        BOARD_DrawHandle     (nI, hdc) ;
                        BOARD_DrawGameType   (nI, hdc) ;
                        BOARD_DrawLastMove   (nI, hdc) ;
                        BOARD_DrawWhiteClock (nI, hdc) ;
                        BOARD_DrawBlackClock (nI, hdc) ;
                        ReleaseDC (Game [nI].hwnd, hdc) ;

                        if (User.bShowGameTypeOnTitleBar)
                            {
                            SetWindowText (Game [nI].hwnd, TOOLBOX_GetGameWindowTitle (nI)) ;
                            }
                        else
                            {
                            if (User.bShowValueOnTitleBar)
                                {
                                SetWindowText (Game [nI].hwnd, TOOLBOX_GetGameWindowTitle (nI)) ;
                                }
                            }

                        TOOLBOX_AdjustShowMoveButtons (nI) ;
                                
                        if (nI == INDEX_PLAY)
                            {
                            if (Game [nI].bPlaying)
                                {
                                if (bBug)
                                    {
                                    GAMEEVENT_Command (GAME_EVENT_PLAY_BUG_START) ;
                                    }
                                else
                                    {
                                    GAMEEVENT_Command (GAME_EVENT_PLAY_ONE_START) ;
                                    }

                                if (Game [nI].bIPlayWhite)
                                    {
                                    MATCH_AdjustMatch (nI,
                                                       Game [nI].cHandle [INDEX_BLACK],
                                                       Game [nI].nInitialClock,
                                                       Game [nI].nIncrementClock,
                                                       Game [nI].nRated) ;
                                    }
                                else
                                    {
                                    MATCH_AdjustMatch (nI,
                                                       Game [nI].cHandle [INDEX_WHITE],
                                                       Game [nI].nInitialClock,
                                                       Game [nI].nIncrementClock,
                                                       Game [nI].nRated) ;
                                    }

                                System.nLastPlayGameType = Game [nI].nGameType ;
                                }
                            else
                                {
                                if (! User.nShowCapturedChessPiece)
                                    {
                                    RECT rc ;

                                    GetClientRect (Game [INDEX_PLAY].hwnd, &rc) ;

                                    hdc = GetDC (Game [INDEX_PLAY].hwnd) ;

                                    switch (User.nBufferOrientation)
                                        {
                                        case DEFAULT_BUFFER_LEFT :
                                            BOARD_ResizeLeft (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                            break ;

                                        case DEFAULT_BUFFER_RIGHT :
                                            BOARD_ResizeRight (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                            break ;

                                        case DEFAULT_BUFFER_TOPBOTTOML :
                                            BOARD_ResizeTopBottomL (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                            break ;

                                        case DEFAULT_BUFFER_TOPBOTTOMR :
                                            BOARD_ResizeTopBottomR (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                            break ;

                                        default :
                                            BOARD_ResizeRight (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                            break ;
                                        }

                                    BOARD_LoadBitmaps (INDEX_PLAY, hdc) ;
                                    BOARD_CheckFlip (INDEX_PLAY) ;
                                    BOARD_DrawRepaint (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc) ;

                                    ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
                                    }

                                GAMEEVENT_Command (GAME_EVENT_PLAY_ONE_START) ;

                                System.nLastExamGameType = Game [nI].nGameType ;
                                }

                            Game [nI].bIssuedEventCommand = 1 ;
                            }
                        else
                            {
                            if ((bBug) && (Game [nI].nRelation != -2))
                                {
                                for (nJ = 0 ; nJ < MAX_GAME ; nJ++)
                                    {
                                    if (nJ != nI)
                                        {
                                        if (Game [nJ].nGamePartner == Game [nI].nGameNumber)
                                            {
                                            if (Game [nJ].bIssuedEventCommand)
                                                {
                                                Telnet.bTelnetSkipLine = 1 ;
                                                //TELNET_NormalPrint (cS) ;
                                                return ;
                                                }
                                            }
                                        }
                                    }

                                GAMEEVENT_Command (GAME_EVENT_OBSERVE_BUG_ST) ;
                                }
                            else
                                {
                                GAMEEVENT_Command (GAME_EVENT_OBSERVE_ONE_ST) ;
                                }

                            Game [nI].bIssuedEventCommand = 1 ;

                            System.nLastObserveGameType = Game [nI].nGameType ;
                            }

                        Telnet.bTelnetSkipLine = 1 ;
                        //TELNET_NormalPrint (cS) ;
                        return ;
                        }
                    }
                }

            if (! strncmp (cS, FICS_MOVELIST_END, 7))
                {
                nJ = System.nMoveListIndexNumber ;

                strcpy (Game [nJ].cOrgResult, cS) ;
                TOOLBOX_AllTrim (Game [nJ].cOrgResult) ;

                if (Game [nJ].bChessGame)
                    {
                    if (Game [nJ].nMaxIndex >= 0)
                        {
                        BOARD_CopyPromoteBoard (nJ) ;
                        }

                    BOARD_FindCapturedPieces (nJ) ;
                    BUGHOUSE_EndMoveList     (nJ) ;

                    System.nIssuedMovesCommand  = System.nIssuedMovesCommand - 1 ;
                    System.nMoveListIndexNumber = -1 ;
                    Telnet.bTelnetSkipLine      = 1 ;

                    if ((nJ == INDEX_PLAY) || (TOOLBOX_DisplayActualBoard (nJ)))
                        {
                        hdc = GetDC (Game [nJ].hwnd) ;
                        if (nJ == INDEX_PLAY)
                            {
                            BOARD_DrawBoard (nJ, Game [nJ].hwnd, hdc, DRAW_STATE_BUFFER) ;
                            }
                        else
                            {
                            BOARD_DrawBoard1 (nJ, Game [nJ].hwnd, hdc, DRAW_STATE_BUFFER) ;
                            }
                        ReleaseDC (Game [nJ].hwnd, hdc) ;
                        }
                    }
                else
                    {
                    BUGHOUSE_EndMoveList (nJ) ;

                    System.nIssuedMovesCommand  = System.nIssuedMovesCommand - 1 ;
                    System.nMoveListIndexNumber = -1 ;
                    Telnet.bTelnetSkipLine      = 1 ;
                    }
                return ;
                }

            if (! strncmp (cS, FICS_MOVELIST_NO_GAME, 22))
                {
                System.nIssuedMovesCommand  = System.nIssuedMovesCommand - 1 ;
                System.nMoveListIndexNumber = -1 ;
                Telnet.bTelnetSkipLine      = 1 ;
                //TELNET_NormalPrint (cS) ;
                return ;
                }

            if (! strncmp (cS, FICS_MOVELIST_BEGIN, 6))
                {
                Telnet.bTelnetSkipLine = 1 ;
                return ;
                }

            if (! strncmp (cS, FICS_MOVELIST_BEGIN1, 6))
                {
                Telnet.bTelnetSkipLine = 1 ;
                return ;
                }

            if (cS [3] == '.')
                {
                //
                //  1.  e4      (0:01.430)      e6      (0:00.330)
                //  2.  d4      (0:02.750)      Nc6     (0:00.930)
                //  3.  Nc3     (0:02.030)      Nf6     (0:00.490)
                //  4.  e5      (0:00.710)      Nd5     (0:01.210)
                //  5.  Nxd5    (0:00.390)      exd5    (0:00.100)
                //  6.  Nf3     (0:00.980)      d6      (0:00.100)
                //  7.  Be2     (0:00.830)
                //
                strcpy (cNumber,  "") ;
                strcpy (cMove1,   "") ;
                strcpy (cWRating, "") ;
                strcpy (cMove2,   "") ;
                strcpy (cBRating, "") ;
                sscanf (cS, "%s %s %s %s %s", &cNumber, &cMove1, &cWRating, &cMove2, &cBRating) ;

                cNumber [3] = NULL_CHAR ;
                sscanf (cNumber, "%d", &nJ) ;
                nJ = nJ - 1 ;

                if (nJ < MAX_POSITION)
                    {
                    if (strlen (cMove1) > 0)
                        {
                        POSITION_AddMoveList (System.nMoveListIndexNumber, nJ, 0, cMove1, cWRating) ;
                        }

                    if (strlen (cMove2) > 0)
                        {
                        POSITION_AddMoveList (System.nMoveListIndexNumber, nJ, 1, cMove2, cBRating) ;
                        }
                    }

                Telnet.bTelnetSkipLine = 1 ;
                return ;
                }
            }
        }

    if (FICS_ColorIndex (cS))
        {
        TELNET_Write (cS) ;

        if (! EAR_Play (cS))
            {
            SOUND_Play () ;
            }
        Telnet.clrLastLine = Telnet.clrCurrent ;
        }
    else
        {
        if (Telnet.bDisplayLine)
            {
            TELNET_Write (cS) ;
            SOUND_Play () ;

            Telnet.clrLastLine = Telnet.clrCurrent ;
            }
        }
    }

int FICS_ParseBoard (char *cS)
    {
    HDC hdc ;
    MDICREATESTRUCT mdicreate ;
    char cGarbage [100], cGame [100], cTurn, cwName [20], cbName [20], cVerboseMove [10], cTime4Move [10], cLastMove [10], *cP ;
    int nGarbage, nN, nLP, bWCK, bWCQ, bBCK, bBCQ, nHalf, nRl, nIc, nIi, nMn, nFlip, nClock ;
    int bOrgFlip, bNewGame, bPartner, bNotPartner, nI, nJ, nK ;
    int nA, nB, nC, nD, nX, nY, nG, bB, bC, bT, nOA ;
    long nWc, nBc, nLag ;

    sscanf (cS + 77,
            "%c %d %d %d %d %d %d %d %s %s %d %d %d %d %d %d %d %d %s %s %s %d %d %ld",
            &cTurn,
            &nLP, &bWCK, &bWCQ, &bBCK, &bBCQ,
            &nHalf, &nN,
            &cwName, &cbName,
            &nRl, &nIc, &nIi,
            &cGarbage, &cGarbage,
            &nWc, &nBc, &nMn,
            &cVerboseMove,
            &cTime4Move,
            &cLastMove,
            &nFlip,
            &nClock,
            &nLag) ;

    // playing game
    if (nRl == -1 ||    // -1 i am playing, it is my opponent's move
        nRl ==  1)      //  1 i am playing and it is my move
        {
        System.bIsMyTurn = (nRl == 1) ;

        bOrgFlip = Game [INDEX_PLAY].bFlip ;

        if (Game [INDEX_PLAY].nGameNumber == nN)
            {
            bNewGame = 0 ;
            }
        else
            {
            bNewGame = 1 ;

            //
            //<g1> 14 p=0 t=crazyhouse r=1 u=0,0 it=180,0 i=180,0 pt=0 rt=1778,1785 ts=1,1 m=2 n=0
            //<g1> 62 p=0 t=lightning r=1 u=0,0 it=60,0 i=60,0 pt=0 rt=1231,1599 ts=1,1 m=2 n=0
            //<g1> 48 p=0 t=standard r=0 u=1,1 it=900,0 i=900,0 pt=0 rt=0P,0P ts=0,1 m=2 n=0
            //<g1> 73 p=0 t=blitz r=1 u=0,0 it=120,12 i=120,12 pt=0 rt=1645,1743 ts=1,0 m=2 n=0
            //<g1> 77 p=0 t=wild/8 r=0 u=0,1 it=300,0 i=300,0 pt=0 rt=0P,0P ts=1,0 m=2 n=0
            //<g1> 64 p=0 t=wild/fr r=1 u=0,0 it=180,10 i=180,10 pt=0 rt=1720P,1720P ts=1,1 m=2 n=0
            //<g1> 39 p=0 t=bughouse r=1 u=0,0 it=180,0 i=180,0 pt=26 rt=1931,2108 ts=1,1 m=2 n=0
            //
            //<g1> game_number p=private(1/0) t=type r=rated(1/0)
            //u=white_registered(1/0),black_registered(1/0)
            //it=initial_white_time,initial_black_time
            //i=initial_white_inc,initial_black_inc
            //pt=partner's_game_number(or 0 if none)
            //rt=white_rating(+ provshow character),black_rating(+ provshow character)
            //ts=white_uses_timeseal(0/1),black_uses_timeseal(0/1)
            //

            sscanf (Telnet.cLastGameInfo + 5, "%d p=%d t=%s", &nG, &nGarbage, &cGame) ;

            if (stricmp (cGame, "bughouse") == 0)
                {
                bB = 1 ;
                bC = 0 ;
                bT = GAMETYPE_BUGHOUSE ;
                strcpy (cGame, "bug") ;
                }
            else
            if (stricmp (cGame, "crazyhouse") == 0)
                {
                bB = 0 ;
                bC = 1 ;
                bT = GAMETYPE_CRAZYHOUSE ;
                strcpy (cGame, "zh") ;
                }
            else
            if (stricmp (cGame, "suicide") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_SUICIDE ;
                }
            else
            if (stricmp (cGame, "lightning") == 0)
                {	
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_LIGHTNING ;
                }
            else
            if (stricmp (cGame, "blitz") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_BLITZ ;
                }
            else
            if (stricmp (cGame, "standard") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_STANDARD ;
                }
            else
            if (stricmp (cGame, "wild/0") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD0 ;
                strcpy (cGame, "w0") ;
                }
            else
            if (stricmp (cGame, "wild/1") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD1 ;
                strcpy (cGame, "w1") ;
                }
            else
            if (stricmp (cGame, "wild/2") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD2 ;
                strcpy (cGame, "w2") ;
                }
            else
            if (stricmp (cGame, "wild/3") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD3 ;
                strcpy (cGame, "w3") ;
                }
            else
            if (stricmp (cGame, "wild/4") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD4 ;
                strcpy (cGame, "w4") ;
                }
            else
            if (stricmp (cGame, "wild/5") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_WILD5 ;
                strcpy (cGame, "w5") ;
                }
            else
            if (stricmp (cGame, "wild/8") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD8 ;
                strcpy (cGame, "w8") ;
                }
            else
            if (stricmp (cGame, "wild/8a") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD8A ;
                strcpy (cGame, "w8a") ;
                }
            else
            if (stricmp (cGame, "wild/fr") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILDFR ;
                strcpy (cGame, "wfr") ;
                }
            else
            if (stricmp (cGame, "atomic") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_ATOMIC ;
                }
            else
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_CHESS ;
                }
            }

        if (bNewGame)
            {
            if (nN == nG)
                {
                if (bT == GAMETYPE_WILD5)	
                    {
                    RECT rc ;

                    switch (User.nPlayWild5Flip)
                        {
                        case DEFAULT_WILD5_PLAY_TOP :
                            if (stricmp (cwName, Vars.cWhoAmI) == 0)
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 1 ;
                                Game [INDEX_PLAY].bFlip       = 0 ;
                                }
                            else
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 0 ;
                                Game [INDEX_PLAY].bFlip       = 1 ;
                                }
                            break ;

                        case DEFAULT_WILD5_PLAY_BOTTOM :
                            if (stricmp (cwName, Vars.cWhoAmI) == 0)
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 1 ;
                                Game [INDEX_PLAY].bFlip       = 1 ;
                                }
                            else
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 0 ;
                                Game [INDEX_PLAY].bFlip       = 0 ;
                                }
                            break ;

                        case DEFAULT_WILD5_PLAY_BBOTTOM :
                            if (stricmp (cwName, Vars.cWhoAmI) == 0)
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 1 ;
                                }
                            else
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 0 ;
                                }
                            Game [INDEX_PLAY].bFlip = 0 ;
                            break ;

                        case DEFAULT_WILD5_PLAY_WBOTTOM :
                            if (stricmp (cwName, Vars.cWhoAmI) == 0)
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 1 ;
                                }
                            else
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 0 ;
                                }
                            Game [INDEX_PLAY].bFlip = 1 ;
                            break ;

                        default :
                            if (stricmp (cwName, Vars.cWhoAmI) == 0)
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 1 ;
                                Game [INDEX_PLAY].bFlip       = 0 ;
                                }
                            else
                                {
                                Game [INDEX_PLAY].bIPlayWhite = 0 ;
                                Game [INDEX_PLAY].bFlip       = 1 ;
                                }
                            break ;
                        }

                    Game [INDEX_PLAY].nGameType = bT ;

                    GetClientRect (Game [INDEX_PLAY].hwnd, &rc) ;

                    hdc = GetDC (Game [INDEX_PLAY].hwnd) ;

                    switch (User.nBufferOrientation)
                        {
                        case DEFAULT_BUFFER_LEFT :
                            BOARD_ResizeLeft (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                            break ;

                        case DEFAULT_BUFFER_RIGHT :
                            BOARD_ResizeRight (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                            break ;

                        case DEFAULT_BUFFER_TOPBOTTOML :
                            BOARD_ResizeTopBottomL (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                            break ;

                        case DEFAULT_BUFFER_TOPBOTTOMR :
                            BOARD_ResizeTopBottomR (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                            break ;

                        default :
                            BOARD_ResizeRight (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                            break ;
                        }

                    BOARD_LoadBitmaps (INDEX_PLAY, hdc) ;
                    BOARD_CheckFlip (INDEX_PLAY) ;
                    BOARD_DrawRepaint (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc) ;

                    ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
                    }
                else
                    {
                    if (stricmp (cwName, Vars.cWhoAmI) == 0)
                        {
                        Game [INDEX_PLAY].bIPlayWhite = 1 ;
                        Game [INDEX_PLAY].bFlip       = 0 ;
                        }
                    else
                        {
                        Game [INDEX_PLAY].bIPlayWhite = 0 ;
                        Game [INDEX_PLAY].bFlip       = 1 ;
                        }

                    if (TOOLBOX_ShowBuffer1 (Game [INDEX_PLAY].nGameType))
                        {
                        Game [INDEX_PLAY].nGameType = bT ;

                        if (! TOOLBOX_ShowBuffer1 (bT))
                            {
                            RECT rc ;

                            GetClientRect (Game [INDEX_PLAY].hwnd, &rc) ;

                            hdc = GetDC (Game [INDEX_PLAY].hwnd) ;

                            BOARD_ResizeNoBuffer (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                            BOARD_LoadBitmaps (INDEX_PLAY, hdc) ;
                            BOARD_CheckFlip (INDEX_PLAY) ;
                            BOARD_DrawRepaint (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc) ;

                            ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
                            }
                        }
                    else
                        {
                        Game [INDEX_PLAY].nGameType = bT ;

                        if (TOOLBOX_ShowBuffer1 (bT))
                            {
                            RECT rc ;

                            GetClientRect (Game [INDEX_PLAY].hwnd, &rc) ;

                            hdc = GetDC (Game [INDEX_PLAY].hwnd) ;

                            switch (User.nBufferOrientation)
                                {
                                case DEFAULT_BUFFER_LEFT :
                                    BOARD_ResizeLeft (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                    break ;

                                case DEFAULT_BUFFER_RIGHT :
                                    BOARD_ResizeRight (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                    break ;

                                case DEFAULT_BUFFER_TOPBOTTOML :
                                    BOARD_ResizeTopBottomL (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                    break ;

                                case DEFAULT_BUFFER_TOPBOTTOMR :
                                    BOARD_ResizeTopBottomR (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                    break ;

                                default :
                                    BOARD_ResizeRight (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                    break ;
                                }

                            BOARD_LoadBitmaps (INDEX_PLAY, hdc) ;
                            BOARD_CheckFlip (INDEX_PLAY) ;
                            BOARD_DrawRepaint (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc) ;

                            ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
                            }
						else
							{
							RECT rc ;

                            GetClientRect (Game [INDEX_PLAY].hwnd, &rc) ;

                            hdc = GetDC (Game [INDEX_PLAY].hwnd) ;

                            BOARD_ResizeNoBuffer (INDEX_PLAY, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                            BOARD_LoadBitmaps (INDEX_PLAY, hdc) ;
                            BOARD_CheckFlip (INDEX_PLAY) ;
                            BOARD_DrawRepaint (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc) ;

                            ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
							}
                        }
                    }
                }
            else
                {
                if (stricmp (cwName, Vars.cWhoAmI) == 0)
                    {
                    Game [INDEX_PLAY].bIPlayWhite = 1 ;
                    Game [INDEX_PLAY].bFlip       = 0 ;
                    }
                else
                    {
                    Game [INDEX_PLAY].bIPlayWhite = 0 ;
                    Game [INDEX_PLAY].bFlip       = 1 ;
                    }
                }
            }
        else
            {
            if (Game [INDEX_PLAY].nGameType == GAMETYPE_WILD5)
                {
                switch (User.nPlayWild5Flip)
                    {
                    case DEFAULT_WILD5_PLAY_TOP :
                        if (stricmp (cwName, Vars.cWhoAmI) == 0)
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 1 ;
                            Game [INDEX_PLAY].bFlip       = 0 ;
                            }
                        else
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 0 ;
                            Game [INDEX_PLAY].bFlip       = 1 ;
                            }
                        break ;

                    case DEFAULT_WILD5_PLAY_BOTTOM :
                        if (stricmp (cwName, Vars.cWhoAmI) == 0)
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 1 ;
                            Game [INDEX_PLAY].bFlip       = 1 ;
                            }
                        else
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 0 ;
                            Game [INDEX_PLAY].bFlip       = 0 ;
                            }
                        break ;

                    case DEFAULT_WILD5_PLAY_BBOTTOM :
                        if (stricmp (cwName, Vars.cWhoAmI) == 0)
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 1 ;
                            }
                        else
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 0 ;
                            }
                        Game [INDEX_PLAY].bFlip = 0 ;
                        break ;

                    case DEFAULT_WILD5_PLAY_WBOTTOM :
                        if (stricmp (cwName, Vars.cWhoAmI) == 0)
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 1 ;
                            }
                        else
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 0 ;
                            }
                        Game [INDEX_PLAY].bFlip = 1 ;
                        break ;

                    default :
                        if (stricmp (cwName, Vars.cWhoAmI) == 0)
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 1 ;
                            Game [INDEX_PLAY].bFlip       = 0 ;
                            }
                        else
                            {
                            Game [INDEX_PLAY].bIPlayWhite = 0 ;
                            Game [INDEX_PLAY].bFlip       = 1 ;
                            }
                        break ;
                    }
                }
            else
                {
                if (stricmp (cwName, Vars.cWhoAmI) == 0)
                    {
                    Game [INDEX_PLAY].bIPlayWhite = 1 ;
                    Game [INDEX_PLAY].bFlip       = 0 ;
                    }
                else
                    {
                    Game [INDEX_PLAY].bIPlayWhite = 0 ;
                    Game [INDEX_PLAY].bFlip       = 1 ;
                    }
                }
            }

        Game [INDEX_PLAY].bValid       = 1 ;
        Game [INDEX_PLAY].nGameNumber  = nN ;
        Game [INDEX_PLAY].nRelation    = nRl ;
        Game [INDEX_PLAY].bWhitesMove  = (cTurn == 'W') ;
        Game [INDEX_PLAY].bPlaying     = 1 ;
		Game [INDEX_PLAY].nAbortStatus = 0 ;
        Game [INDEX_PLAY].bInitialMove = (stricmp (cLastMove, ICS_INITIAL_MOVE_NAME) == 0) ;
        Game [INDEX_PLAY].nMoveNumber  = nMn ;

        strcpy (Game [INDEX_PLAY].cHandle [INDEX_WHITE], cwName) ;
        strcpy (Game [INDEX_PLAY].cHandle [INDEX_BLACK], cbName) ;
        strcpy (Game [INDEX_PLAY].cResult,               "") ;

        Game [INDEX_PLAY].nLastDoublePushFile               = nLP ;
        Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_WHITE] = bWCK ;
        Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_WHITE] = bWCQ ;
        Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_BLACK] = bBCK ;
        Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_BLACK] = bBCQ ;
        Game [INDEX_PLAY].nHalfMoves                        = nHalf ;

        Game [INDEX_PLAY].nInitialClock   = nIc ;
        Game [INDEX_PLAY].nIncrementClock = nIi ;
        Game [INDEX_PLAY].bOnLagClock     = 0 ;
        Game [INDEX_PLAY].bTickClock      = nClock ;

        if (bNewGame)
            {
            Game [INDEX_PLAY].bChessGame = 0 ;

            BOARD_LoadBoard (INDEX_PLAY, cS + 5, cVerboseMove) ;
            }
        else
            {
            if (User.bAnimatePlay)
                {
                if (! System.bIsMyTurn)
                    {
                    BOARD_LoadBoard (INDEX_PLAY, cS + 5, cVerboseMove) ;
                    }
                }
            else
                {
                BOARD_LoadBoard (INDEX_PLAY, cS + 5, cVerboseMove) ;
                }
            }

        BOARD_GetHighlightFromLastMove (INDEX_PLAY, cVerboseMove, cLastMove) ;

        hdc = GetDC (Game [INDEX_PLAY].hwnd) ;

        if (bOrgFlip != Game [INDEX_PLAY].bFlip)
            {
            BOARD_CheckFlip (INDEX_PLAY) ;
            }

        if (Telnet.nTelnetState == WIN_MAXIMIZE)
            {
            wCoord [COORD_TELNET].s = WIN_SHOW ;
            ShowWindow (hwndWindow [HWND_TELNET], SW_RESTORE) ;
            }

        if (bNewGame)
            {
            System.bPromoteCommand = 0 ;
            System.nPromoteWPiece  = 0 ;
            System.nPromoteBPiece  = 0 ;
            System.bPromoteKnight  = 0 ;

            Game [INDEX_PLAY].bFirstResize    = 1 ;
            Game [INDEX_PLAY].bFirstGame      = 1 ;
            Game [INDEX_PLAY].bLastWhitesMove = Game [INDEX_PLAY].bWhitesMove ;

            Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc ;
            Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc ;

            strcpy (Game [INDEX_PLAY].cLastMove,             ICS_INITIAL_MOVE_NAME) ;
            strcpy (Game [INDEX_PLAY].cVerboseMove,          ICS_INITIAL_MOVE_NAME) ;
            strcpy (Game [INDEX_PLAY].cTime4Move,            ICS_INITIAL_TIME_MOVE) ;
            strcpy (Game [INDEX_PLAY].cOrgResult,            "") ;
            strcpy (Game [INDEX_PLAY].cRating [INDEX_WHITE], "") ;
            strcpy (Game [INDEX_PLAY].cRating [INDEX_BLACK], "") ;

            Game [INDEX_PLAY].bSavedGame = 0 ;
            Game [INDEX_PLAY].nRated     = ICS_INITIAL_RATED ;

            if (nN == nG)
                {
                Game [INDEX_PLAY].nGameType = bT ;
                strcpy (Game [INDEX_PLAY].cGameType, cGame) ;
                }
            else
                {
                Game [INDEX_PLAY].nGameType = System.nLastPlayGameType ;
                strcpy (Game [INDEX_PLAY].cGameType, ICS_INITIAL_GAMETYPE_STRING) ;
                }

            LagCmd.tLastLagCommand.nSec = 0 ;
            LagCmd.tLastLagCommand.nMs  = 0 ;

            Game [INDEX_PLAY].nClockTimerEvent       = 0 ;
            Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 0 ;
            Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 0 ;
            Game [INDEX_PLAY].tTickStartTM.nSec      = 0 ;
            Game [INDEX_PLAY].tTickStartTM.nMs       = 0 ;
            Game [INDEX_PLAY].nIntendedTickLength    = 0 ;
            Game [INDEX_PLAY].nLag [INDEX_WHITE]     = 0 ;
            Game [INDEX_PLAY].nLag [INDEX_BLACK]     = 0 ;

            Game [INDEX_PLAY].bIssuedEventCommand = 0 ;

            CLOCK_StartClocks (INDEX_PLAY, Game [INDEX_PLAY].hwnd) ;

            BOARD_LoadBuffer (INDEX_PLAY, INIT_BUFFER, INIT_BUFFER) ;

            BOARD_NullLastHighlight   (INDEX_PLAY) ;
            BOARD_NullLastBoardBuffer (INDEX_PLAY) ;

            Premove.nPremoveCount     = 0 ;
            Premove.nPremoveHead      = 0 ;
            Premove.nPremoveTail      = 0 ;
            Premove.bLastTP           = 0 ;
            Premove.ptLastTP [0].x    = -1 ;
            Premove.ptLastTP [0].y    = -1 ;
            Premove.ptLastTP [1].x    = -1 ;
            Premove.ptLastTP [1].y    = -1 ;
            Premove.bIllegalTP        = 0 ;
            Premove.ptIllegalTP [0].x = -1 ;
            Premove.ptIllegalTP [0].y = -1 ;
            Premove.ptIllegalTP [1].x = -1 ;
            Premove.ptIllegalTP [1].y = -1 ;

            BOARD_DrawHandle      (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteLag    (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackLag    (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteClock  (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackClock  (INDEX_PLAY, hdc) ;
            BOARD_DrawGameType    (INDEX_PLAY, hdc) ;
            BOARD_DrawLastMove    (INDEX_PLAY, hdc) ;
            BOARD_DrawResult      (INDEX_PLAY, hdc) ;
            BOARD_DrawCoordinates (INDEX_PLAY, hdc) ;
            BOARD_DrawBoard       (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NEW_GAME) ;

            SetWindowText (Game [INDEX_PLAY].hwnd, TOOLBOX_GetGameWindowTitle (INDEX_PLAY)) ;

            if (wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
                {
                wCoord [COORD_PLAY].s = WIN_SHOW ;
                ShowWindow (Game [INDEX_PLAY].hwnd, SW_RESTORE) ;
                }

            wCoord [COORD_PLAY].s = WIN_SHOW ;
            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) hwndWindow [HWND_PLAY], 0) ;

            // start of own game
            GAMESOUND_Play (GAME_SOUND_PLAY_START) ;

            POSITION_FirstTime (INDEX_PLAY) ;

            sprintf (cGarbage, "%s %d\n", FICS_MOVELIST_COMMAND, nN) ;
            TOOLBOX_WriteICS (cGarbage) ;

            System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1 ;

            BUGHOUSE_FirstTime (INDEX_PLAY) ;

            DragInfo.nClicked = 0 ;

            if (strlen (Vars.cPartner) > 0)
                {
                if (strlen (Vars.cPfollow) > 0)
                    {
                    if (stricmp (Vars.cWhoAmI, Vars.cPfollow) != 0)
                        {
                        TOOLBOX_WriteICS (FICS_PFOLLOW_COMMAND) ;
                        TOOLBOX_WriteICS (Vars.cWhoAmI);
                        TOOLBOX_WriteICS ("\n");
                        }
                    }
                else
                if (strlen (Vars.cFollow) > 0)
                    {
                    if (stricmp (Vars.cPartner, Vars.cFollow) != 0)
                        {
                        TOOLBOX_WriteICS (FICS_PFOLLOW_COMMAND) ;
                        TOOLBOX_WriteICS (Vars.cWhoAmI);
                        TOOLBOX_WriteICS ("\n");
                        }
                    }
                else
                    {
                    TOOLBOX_WriteICS (FICS_PFOLLOW_COMMAND) ;
                    TOOLBOX_WriteICS (Vars.cWhoAmI);
                    TOOLBOX_WriteICS ("\n");
                    }
                }
            else
                {
                if (strlen (Vars.cPfollow) > 0)
                    {
                    if (stricmp (Vars.cWhoAmI, Vars.cPfollow) != 0)
                        {
                        TOOLBOX_WriteICS (FICS_NO_FOLLOW_COMMAND) ;
                        }
                    }
                else
                if (strlen (Vars.cFollow) > 0)
                    {
                    TOOLBOX_WriteICS (FICS_NO_FOLLOW_COMMAND) ;
                    }
                }

            if (! User.bShowMoveButton)
                {
                if (Game [INDEX_PLAY].bShowMoveButtons)
                    {
                    TOOLBOX_ShowMoveButtons (INDEX_PLAY, 0) ;
                    }
                }

            if (User.bFICSPlayCloseAll)
                {
                for (nI = 1 ; nI < MAX_GAME ; nI++)
                    {
                    if (IsWindow (Game [nI].hwnd))
                        {
                        Game [nI].bFirstGame  = 0 ;
                        Game [nI].nGameNumber = 0 ;
                        Game [nI].bPlaying    = 0 ;
                        Game [nI].bTickClock  = 1 ;

                        (void) CLOCK_StopClockTimer (nI) ;

                        SendMessage (Game [nI].hwnd, WM_CLOSE, 0, 0) ;
                        }
                    }
                }

            if (Game [INDEX_PLAY].bIPlayWhite)
                {
                strcpy (Vars.cOpponent, cbName) ;
                }
            else
                {
                strcpy (Vars.cOpponent, cwName) ;
                }

            if (Censor [Login.nLoginType].bCensor)
                {
                if (CENSOR_Other (7, Vars.cOpponent))
                    {
                    sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                    TOOLBOX_WriteICS (cGarbage) ;

                    System.bIssuedAbort = 1 ;
                    }
                else
                    {
                    System.bIssuedAbort = 0 ;
                    }

                if (strlen (Vars.cPartner) > 0)
                    {
                    if (CENSOR_Partner (Vars.cPartner))
                        {
                        if (! System.bIssuedAbort)
                            {
                            sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                            TOOLBOX_WriteICS (cGarbage) ;

                            System.bIssuedAbort = 1 ;
                            }

                        TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                        }
                    }
                }

            STATE_PlayNewGame () ;

            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
            }
        else
            {
            strcpy (Game [INDEX_PLAY].cLastMove,    cLastMove) ;
            strcpy (Game [INDEX_PLAY].cVerboseMove, cVerboseMove) ;
            strcpy (Game [INDEX_PLAY].cTime4Move,   cTime4Move) ;

            if (User.bShowLagStat || User.bLagCommand)
                {
                if (Game [INDEX_PLAY].bWhitesMove != Game [INDEX_PLAY].bLastWhitesMove)
                    {
                    if (Game [INDEX_PLAY].bWhitesMove)
                        {
                        Game [INDEX_PLAY].nLag [INDEX_BLACK] += nLag ;
                        }
                    else
                        {
                        Game [INDEX_PLAY].nLag [INDEX_WHITE] += nLag ;
                        }
                    }
                }

            Game [INDEX_PLAY].bLastWhitesMove = Game [INDEX_PLAY].bWhitesMove ;

            Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc ;
            Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc ;

            CLOCK_StartClocks (INDEX_PLAY, Game [INDEX_PLAY].hwnd) ;

            BOARD_DrawHandle     (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteLag   (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackLag   (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteClock (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackClock (INDEX_PLAY, hdc) ;
            BOARD_DrawGameType   (INDEX_PLAY, hdc) ;
            BOARD_DrawLastMove   (INDEX_PLAY, hdc) ;

            if (User.bAnimatePlay)
                {
                if (System.bIsMyTurn)
                    {
                    ANIMATE_Move (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, User.nAnimateSpeedPlay) ;
                    BOARD_LoadBoard (INDEX_PLAY, cS + 5, cVerboseMove) ;
                    }
                }

            BOARD_DrawBoard (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_BOARD) ;

            if (wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
                {
                wCoord [COORD_PLAY].s = WIN_SHOW ;
                ShowWindow (Game [INDEX_PLAY].hwnd, SW_RESTORE) ;

                SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) hwndWindow [HWND_PLAY], 0) ;
                SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                }

            if (! System.bIsMyTurn)
                {
                if (strlen (F8Key.cSitPieceDropMove) > 0)
                    {
                    F8KEY_Init () ;
                    }
                }

            POSITION_SaveGameMove (INDEX_PLAY) ;
            }

        if (! Game [INDEX_PLAY].bInitialMove)
            {
            if (Game [INDEX_PLAY].bIPlayWhite)
                {
                if (Game [INDEX_PLAY].bWhitesMove)
                    {
                    if (strchr (cLastMove, ICS_KING_IN_CHECK_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CHECK) ;
                        }
                    else
                    if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CAPTURE) ;
                        }
                    else
                    if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CASTLE) ;
                        }
                    else
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_MOVE) ;
                        }
                    }
                else
                    {
                    if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CAPTURE) ;
                        }
                    else
                    if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CASTLE) ;
                        }
                    else
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_MOVE) ;
                        }
                    }
                }
            else
                {
                if (Game [INDEX_PLAY].bWhitesMove)
                    {
                    if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CAPTURE) ;
                        }
                    else
                    if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CASTLE) ;
                        }
                    else
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_MOVE) ;
                        }
                    }
                else
                    {
                    if (strchr (cLastMove, ICS_KING_IN_CHECK_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CHECK) ;
                        }
                    else
                    if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CAPTURE) ;
                        }
                    else
                    if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_CASTLE) ;
                        }
                    else
                        {
                        GAMESOUND_Play (GAME_SOUND_PLAY_MOVE) ;
                        }
                    }
                }
            }

        // has true premove turned on
        if (User.bTruePremove)
            {
            // it's my move
            if (System.bIsMyTurn)
                {
                // has alreay a true premove in the buffer
                if (Premove.nPremoveCount > 0)
                    {
                    nJ = 0 ;
                    while (Premove.nPremoveCount > 0)
                        {
                        if (User.nTruePremove == PREMOVE_LR)
                            {
                            if (Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [8] == EMPTY_SQUARE)
                                {
                                nK = 1 ;
                                }
                            else
                                {
                                if (Game [INDEX_PLAY].nBoard [Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5]]
                                                             [Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6]] == EMPTY_SQUARE)
                                    {
                                    if (Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [2] == (Game [INDEX_PLAY].bIPlayWhite ? WHITE_PAWN : BLACK_PAWN))
                                        {
                                        if (Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3] ==
                                            Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5])
                                            {
                                            nK = 1 ;
                                            }
                                        else
                                            {
                                            nK = 0 ;
                                            }
                                        }
                                    else
                                        {
                                        nK = 0 ;
                                        }
                                    }
                                else
                                    {
                                    nK = 1 ;
                                    }
                                }
                            }
                        else
                            {
                            nK = 1 ;
                            }

                        if (nK)
                            {
                            nOA = User.bAutoQueen ;
                            User.bAutoQueen = 0 ;
							
							// Bugfix for 1.22 (Ludens): Check if premoved piece was captured. 
							// That renders the premove illegal.
							if ((Game [INDEX_PLAY].nBoard 
								           [Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5]]
                                           [Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6]] == EMPTY_SQUARE) || 
							    (Game [INDEX_PLAY].nBoard 
								           [Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3]]
                                           [Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4]] ==
								Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [2]))

								{
								if (CHESS_IsLegalMove (Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [1],
                                                Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [2],
	                                            Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3],
                                                Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4],
                                                Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5],
                                                Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6],
                                                Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [7]))

									{
		                            // it's a legal move lets send it
			                        TOOLBOX_WriteICS (Premove.cPremoveBuffer [Premove.nPremoveTail]) ;
	
		                            User.bAutoQueen = nOA ;
			                        
				                    // erase this true premove highlight
					                Premove.bLastTP        = 1 ;
						            Premove.ptLastTP [0].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3] ;
							        Premove.ptLastTP [0].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4] ;
								    Premove.ptLastTP [1].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5] ;
									Premove.ptLastTP [1].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6] ;

									// update true premove
	                                Premove.nPremoveCount = Premove.nPremoveCount - 1 ;
		                            Premove.nPremoveTail  = Premove.nPremoveTail  + 1 ;

			                        if (Premove.nPremoveTail >= MAX_TRUE_PREMOVE)
				                        {
					                    Premove.nPremoveTail = 0 ;
						                }
	
		                            Game [INDEX_PLAY].bOnLagClock = 1 ;
			                        Game [INDEX_PLAY].bWhitesMove = (! Game [INDEX_PLAY].bWhitesMove) ;
	
		                            CLOCK_SwitchClocks (INDEX_PLAY) ;
			                        BOARD_DrawWhiteClock (INDEX_PLAY, hdc) ;
				                    BOARD_DrawBlackClock (INDEX_PLAY, hdc) ;
					                BOARD_DrawBoard      (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NONE) ;
	
	                                ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;

		                            System.bIsMyTurn = 0 ;

			                        if (strlen (F8Key.cSitPieceDropMove) > 0)
				                        {
					                    F8KEY_Init () ;
						                }
							        return 1 ;
								    }
								}
							User.bAutoQueen = nOA ;
							}

                        // erase this true premove highlight
                        Premove.bLastTP        = 1 ;
                        Premove.ptLastTP [0].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3] ;
                        Premove.ptLastTP [0].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4] ;
                        Premove.ptLastTP [1].x = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5] ;
                        Premove.ptLastTP [1].y = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6] ;

                        // remember this illegal true premove
                        if (nJ == 0)
                            {
                            nJ = 1 ;
                            nA = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [3] ;
                            nB = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [4] ;
                            nC = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [5] ;
                            nD = Premove.nPremoveLegalBuffer [Premove.nPremoveTail] [6] ;
                            }

                        // update true premove
                        Premove.nPremoveCount = Premove.nPremoveCount - 1 ;
                        Premove.nPremoveTail  = Premove.nPremoveTail  + 1 ;

                        if (Premove.nPremoveTail >= MAX_TRUE_PREMOVE)
                            {
                            Premove.nPremoveTail = 0 ;
                            }

                        // draw board
                        BOARD_DrawBoard (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NONE) ;
                        }

                    Premove.nPremoveCount  = 0 ;
                    Premove.nPremoveHead   = 0 ;
                    Premove.nPremoveTail   = 0 ;
                    Premove.bLastTP        = 0 ;
                    Premove.ptLastTP [0].x = -1 ;
                    Premove.ptLastTP [0].y = -1 ;
                    Premove.ptLastTP [1].x = -1 ;
                    Premove.ptLastTP [1].y = -1 ;

                    if (nJ)
                        {
                        Premove.bIllegalTP        = 1 ;
                        Premove.ptIllegalTP [0].x = nA ;
                        Premove.ptIllegalTP [0].y = nB ;
                        Premove.ptIllegalTP [1].x = nC ;
                        Premove.ptIllegalTP [1].y = nD ;

                        BOARD_DrawBoard (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_DRAW_ILLEGAL_TP) ;
                        }

                    if (User.bIllegalTPBeep)
                        {
                        TOOLBOX_Beep () ;
                        }
                    }
                }
            }

        ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
        return 1 ;
        }
            
    // examing game
    if (nRl == 2 ||    // i am examing a game
        nRl == 4)      // i am examing a game issued pause/unpause command
        {
        System.bIsMyTurn = 0 ;

        bOrgFlip = Game [INDEX_PLAY].bFlip ;

        if (Game [INDEX_PLAY].nGameNumber == nN)
            {
            bNewGame = 0 ;
            }
        else
            {
            bNewGame = 1 ;

            if (strlen (Vars.cExam) > 0)
                {
                if (TOOLBOX_MatchHandle (cwName, Vars.cExam))
                    {
                    strcpy (Vars.cExam, "") ;
                    Game [INDEX_PLAY].bFlip = 0 ;
                    }
                else
                if (TOOLBOX_MatchHandle (cbName, Vars.cExam))
                    {
                    strcpy (Vars.cExam, "") ;
                    Game [INDEX_PLAY].bFlip = 1 ;
                    }
                else
                    {
                    Game [INDEX_PLAY].bFlip = 0 ;
                    }
                }
            else
                {
                Game [INDEX_PLAY].bFlip = 0 ;
                }
            }
                
        Game [INDEX_PLAY].bValid          = 1 ;
        Game [INDEX_PLAY].nGameNumber     = nN ;
        Game [INDEX_PLAY].nGamePartner    = 0 ;
        Game [INDEX_PLAY].nRelation       = nRl ;
        Game [INDEX_PLAY].bWhitesMove     = (cTurn == 'W') ;
        Game [INDEX_PLAY].bLastWhitesMove = Game [INDEX_PLAY].bWhitesMove ;
        Game [INDEX_PLAY].bPlaying        = 0 ;
        Game [INDEX_PLAY].bIPlayWhite     = (stricmp (cwName, Vars.cWhoAmI) == 0) ;
        Game [INDEX_PLAY].bInitialMove    = (stricmp (cLastMove, ICS_INITIAL_MOVE_NAME) == 0) ;
        Game [INDEX_PLAY].nMoveNumber     = nMn ;

        strcpy (Game [INDEX_PLAY].cHandle [INDEX_WHITE], cwName) ;
        strcpy (Game [INDEX_PLAY].cHandle [INDEX_BLACK], cbName) ;
        strcpy (Game [INDEX_PLAY].cResult,               "") ;

        Game [INDEX_PLAY].nLastDoublePushFile               = nLP ;
        Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_WHITE] = bWCK ;
        Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_WHITE] = bWCQ ;
        Game [INDEX_PLAY].bCanCastleKingSide  [INDEX_BLACK] = bBCK ;
        Game [INDEX_PLAY].bCanCastleQueenSide [INDEX_BLACK] = bBCQ ;
        Game [INDEX_PLAY].nHalfMoves                        = nHalf ;

        Game [INDEX_PLAY].nInitialClock                = nIc ;
        Game [INDEX_PLAY].nIncrementClock              = nIi ;
        Game [INDEX_PLAY].bTickClock                   = nClock ;
        Game [INDEX_PLAY].nTimeRemaining [INDEX_WHITE] = nWc ;
        Game [INDEX_PLAY].nTimeRemaining [INDEX_BLACK] = nBc ;
        Game [INDEX_PLAY].bOnLagClock                  = 0 ;
        Game [INDEX_PLAY].nNextLagCheck                = 1000 ;
        
        LagCmd.tLastLagCommand.nSec = 0 ;
        LagCmd.tLastLagCommand.nMs  = 0 ;

        hdc = GetDC (Game [INDEX_PLAY].hwnd) ;

        if (bOrgFlip != Game [INDEX_PLAY].bFlip)
            {
            BOARD_CheckFlip (INDEX_PLAY) ;
            }

        if (Telnet.nTelnetState == WIN_MAXIMIZE)
            {
            wCoord [COORD_TELNET].s = WIN_SHOW ;
            ShowWindow (hwndWindow [HWND_TELNET], SW_RESTORE) ;
            }

        if (bNewGame)
            {
            System.bPromoteCommand = 0 ;
            System.nPromoteWPiece  = 0 ;
            System.nPromoteBPiece  = 0 ;
            System.bPromoteKnight  = 0 ;

            Game [INDEX_PLAY].bFirstResize = 1 ;
            Game [INDEX_PLAY].bFirstGame   = 1 ;
            Game [INDEX_PLAY].bChessGame   = 0 ;

            TOOLBOX_InvalidateBoardWindow (0) ;

            BOARD_LoadBoard (INDEX_PLAY, cS + 5, ICS_INITIAL_MOVE_NAME) ;

            BOARD_GetHighlightFromLastMove (INDEX_PLAY, cVerboseMove, cLastMove) ;

            strcpy (Game [INDEX_PLAY].cLastMove,             ICS_INITIAL_MOVE_NAME) ;
            strcpy (Game [INDEX_PLAY].cVerboseMove,          ICS_INITIAL_MOVE_NAME) ;
            strcpy (Game [INDEX_PLAY].cTime4Move,            ICS_INITIAL_TIME_MOVE) ;
            strcpy (Game [INDEX_PLAY].cOrgResult,            "") ;
            strcpy (Game [INDEX_PLAY].cRating [INDEX_WHITE], "") ;
            strcpy (Game [INDEX_PLAY].cRating [INDEX_BLACK], "") ;

            Game [INDEX_PLAY].bSavedGame = 0 ;
            Game [INDEX_PLAY].nRated     = ICS_INITIAL_RATED ;
            Game [INDEX_PLAY].nGameType  = System.nLastExamGameType ;

            strcpy (Game [INDEX_PLAY].cGameType, ICS_INITIAL_GAMETYPE_STRING) ;

            Game [INDEX_PLAY].bIssuedEventCommand = 0 ;

            Game [INDEX_PLAY].nClockTimerEvent       = 0 ;
            Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 0 ;
            Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 0 ;
            Game [INDEX_PLAY].tTickStartTM.nSec      = 0 ;
            Game [INDEX_PLAY].tTickStartTM.nMs       = 0 ;
            Game [INDEX_PLAY].nIntendedTickLength    = 0 ;
            Game [INDEX_PLAY].nLag [INDEX_WHITE]     = 0 ;
            Game [INDEX_PLAY].nLag [INDEX_BLACK]     = 0 ;

            CLOCK_StartClocks (INDEX_PLAY, Game [INDEX_PLAY].hwnd) ;

            BOARD_LoadBuffer (INDEX_PLAY, INIT_BUFFER, INIT_BUFFER) ;

            BOARD_NullLastHighlight   (INDEX_PLAY) ;
            BOARD_NullLastBoardBuffer (INDEX_PLAY) ;

            Premove.nPremoveCount  = 0 ;
            Premove.nPremoveHead   = 0 ;
            Premove.nPremoveTail   = 0 ;
            Premove.bLastTP        = 0 ;
            Premove.ptLastTP [0].x = -1 ;
            Premove.ptLastTP [0].y = -1 ;
            Premove.ptLastTP [1].x = -1 ;
            Premove.ptLastTP [1].y = -1 ;

            BOARD_DrawHandle      (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteLag    (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackLag    (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteClock  (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackClock  (INDEX_PLAY, hdc) ;
            BOARD_DrawGameType    (INDEX_PLAY, hdc) ;
            BOARD_DrawLastMove    (INDEX_PLAY, hdc) ;
            BOARD_DrawResult      (INDEX_PLAY, hdc) ;
            BOARD_DrawCoordinates (INDEX_PLAY, hdc) ;
            BOARD_DrawBoard       (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_NEW_GAME) ;

            POSITION_FirstTime (INDEX_PLAY) ;

            sprintf (cGarbage, "%s %d\n", FICS_MOVELIST_COMMAND, nN) ;
            TOOLBOX_WriteICS (cGarbage) ;

            System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1 ;

            BUGHOUSE_FirstTime (INDEX_PLAY) ;

            SetWindowText (Game [INDEX_PLAY].hwnd, TOOLBOX_GetGameWindowTitle (INDEX_PLAY)) ;

            if (wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
                {
                wCoord [COORD_PLAY].s = WIN_SHOW ;
                ShowWindow (Game [INDEX_PLAY].hwnd, SW_RESTORE) ;
                }

            wCoord [COORD_PLAY].s = WIN_SHOW ;
            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) hwndWindow [HWND_PLAY], 0) ;

            TOOLBOX_ShowMoveButtons (INDEX_PLAY, 1) ;

            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
            }
        else
            {
            strcpy (Game [INDEX_PLAY].cLastMove,    cLastMove) ;
            strcpy (Game [INDEX_PLAY].cVerboseMove, cVerboseMove) ;
            strcpy (Game [INDEX_PLAY].cTime4Move,   cTime4Move) ;

            CLOCK_StartClocks (INDEX_PLAY, Game [INDEX_PLAY].hwnd) ;

            if (wCoord [COORD_PLAY].s == WIN_HIDE || Game [INDEX_PLAY].nState == WIN_MINIMIZE)
                {
                wCoord [COORD_PLAY].s = WIN_SHOW ;
                ShowWindow (Game [INDEX_PLAY].hwnd, SW_RESTORE) ;

                SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) hwndWindow [HWND_PLAY], 0) ;
                SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                }

            BOARD_DrawHandle     (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteLag   (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackLag   (INDEX_PLAY, hdc) ;
            BOARD_DrawWhiteClock (INDEX_PLAY, hdc) ;
            BOARD_DrawBlackClock (INDEX_PLAY, hdc) ;
            BOARD_DrawGameType   (INDEX_PLAY, hdc) ;
            BOARD_DrawLastMove   (INDEX_PLAY, hdc) ;

            if (User.bAnimateExam)
                {
                ANIMATE_Move (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, User.nAnimateSpeedExam) ;
                }
            BOARD_LoadBoard (INDEX_PLAY, cS + 5, cVerboseMove) ;

            BOARD_GetHighlightFromLastMove (INDEX_PLAY, cVerboseMove, cLastMove) ;

            BOARD_DrawBoard (INDEX_PLAY, Game [INDEX_PLAY].hwnd, hdc, DRAW_STATE_BOARD) ;
            }

        ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;

        if (! Game [INDEX_PLAY].bInitialMove)
            {
            if (strchr (cLastMove, ICS_KING_IN_CHECK_SYMBOL))
                {
                GAMESOUND_Play (GAME_SOUND_EXAM_CHECK) ;
                }
            else
            if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                {
                GAMESOUND_Play (GAME_SOUND_EXAM_CAPTURE) ;
                }
            else
            if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                {
                GAMESOUND_Play (GAME_SOUND_EXAM_CASTLE) ;
                }
            else
                {
                GAMESOUND_Play (GAME_SOUND_EXAM_MOVE) ;
                }
            }
        return 1 ;
        }

    if ((nRl ==  0) ||  //  0 i am observing a game being played
        (nRl == -2) ||  // -2 i am observing game being examined
        (nRl == -3))    // -3 isolated position, such as for "ref 3" or the "sposition" command
        {
        // observe game
        for (nI = 1 ; nI < MAX_GAME ; nI++)
            {
            if (Game [nI].bValid)
                {
                if (Game [nI].nGameNumber == nN)
                    {
                    Game [nI].nRelation    = nRl ;
                    Game [nI].bWhitesMove  = (cTurn == 'W') ;
                    Game [nI].bPlaying     = (nRl == 0) ;
                    Game [nI].bInitialMove = (strcmp (cLastMove, ICS_INITIAL_MOVE_NAME) == 0) ;
                    Game [nI].nMoveNumber  = nMn ;

                    strcpy (Game [nI].cLastMove,             cLastMove) ;
                    strcpy (Game [nI].cVerboseMove,          cVerboseMove) ;
                    strcpy (Game [nI].cTime4Move,            cTime4Move) ;
                    strcpy (Game [nI].cHandle [INDEX_WHITE], cwName) ;
                    strcpy (Game [nI].cHandle [INDEX_BLACK], cbName) ;
                    strcpy (Game [nI].cResult,               "") ;

                    Game [nI].nLastDoublePushFile               = nLP ;
                    Game [nI].bCanCastleKingSide  [INDEX_WHITE] = bWCK ;
                    Game [nI].bCanCastleQueenSide [INDEX_WHITE] = bWCQ ;
                    Game [nI].bCanCastleKingSide  [INDEX_BLACK] = bBCK ;
                    Game [nI].bCanCastleQueenSide [INDEX_BLACK] = bBCQ ;
                    Game [nI].nHalfMoves                        = nHalf ;

                    Game [nI].nInitialClock   = nIc ;
                    Game [nI].nIncrementClock = nIi ;

                    if (Game [nI].bPlaying)
                        {
                        if (User.bShowLagStat || User.bLagCommand)
                            {
                            if (Game [nI].bWhitesMove != Game [nI].bLastWhitesMove)
                                {
                                if (Game [nI].bWhitesMove)
                                    {
                                    Game [nI].nLag [INDEX_BLACK] += nLag ;
                                    }
                                else
                                    {
                                    Game [nI].nLag [INDEX_WHITE] += nLag ;
                                    }
                                }
                            }
                        }

                    Game [nI].bLastWhitesMove = Game [nI].bWhitesMove ;

                    Game [nI].nTimeRemaining [INDEX_WHITE] = nWc ;
                    Game [nI].nTimeRemaining [INDEX_BLACK] = nBc ;
                    Game [nI].bTickClock                   = nClock ;

                    CLOCK_StartClocks (nI, Game [nI].hwnd) ;

                    if (Game [nI].bValid)
                        {
                        hdc = GetDC (Game [nI].hwnd) ;

                        BOARD_DrawHandle     (nI, hdc) ;
                        BOARD_DrawWhiteLag   (nI, hdc) ;
                        BOARD_DrawBlackLag   (nI, hdc) ;
                        BOARD_DrawWhiteClock (nI, hdc) ;
                        BOARD_DrawBlackClock (nI, hdc) ;
                        BOARD_DrawGameType   (nI, hdc) ;

                        if (TOOLBOX_DisplayActualBoard (nI))
                            {
                            BOARD_DrawLastMove (nI, hdc) ;

                            if (User.bAnimateObserve)
                                {
                                ANIMATE_Move (nI, Game [nI].hwnd, hdc, User.nAnimateSpeedObserve) ;
                                }
                            BOARD_LoadBoard (nI, cS + 5, cVerboseMove) ;

                            BOARD_GetHighlightFromLastMove (nI, cVerboseMove, cLastMove) ;

                            BOARD_DrawBoard1 (nI, Game [nI].hwnd, hdc, DRAW_STATE_BOARD) ;
                            }
                        else
                            {
                            BOARD_DrawLastMove1 (nI, hdc) ;

                            BOARD_LoadBoard (nI, cS + 5, cVerboseMove) ;

                            BOARD_GetHighlightFromLastMove (nI, cVerboseMove, cLastMove) ;
                            }

                        ReleaseDC (Game [nI].hwnd, hdc) ;

                        if (nRl != -3)
                            {
                            if (! Game [nI].bInitialMove)
                                {
                                if (Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
                                    {
                                    if (nN == Game [INDEX_PLAY].nGamePartner)
                                        {
                                        if (Game [INDEX_PLAY].bIPlayWhite)
                                            {
                                            if (Game [nI].bWhitesMove)
                                                {
                                                if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CAPTURE) ;
                                                    }
                                                else
                                                if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CASTLE) ;
                                                    }
                                                else
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_MOVE) ;
                                                    }
                                                }
                                            else
                                                {
                                                if (strchr (cLastMove, ICS_KING_IN_CHECK_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CHECK) ;
                                                    }
                                                else
                                                if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CAPTURE) ;
                                                    }
                                                else
                                                if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CASTLE) ;
                                                    }
                                                else
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_MOVE) ;
                                                    }
                                                }
                                            }
                                        else
                                            {
                                            if (Game [nI].bWhitesMove)
                                                {
                                                if (strchr (cLastMove, ICS_KING_IN_CHECK_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CHECK) ;
                                                    }
                                                else
                                                if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CAPTURE) ;
                                                    }
                                                else
                                                if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CASTLE) ;
                                                    }
                                                else
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_MOVE) ;
                                                    }
                                                }
                                            else
                                                {
                                                if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CAPTURE) ;
                                                    }
                                                else
                                                if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_CASTLE) ;
                                                    }
                                                else
                                                    {
                                                    GAMESOUND_Play (GAME_SOUND_PARTNER_MOVE) ;
                                                    }
                                                }
                                            }
                                        }
                                    else
                                        {
                                        if (strchr (cLastMove, ICS_KING_IN_CHECK_SYMBOL))
                                            {
                                            GAMESOUND_Play (GAME_SOUND_OBS_CHECK1) ;
                                            }
                                        else
                                        if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                                            {
                                            GAMESOUND_Play (GAME_SOUND_OBS_CAPTURE1) ;
                                            }
                                        else
                                        if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                                            {
                                            GAMESOUND_Play (GAME_SOUND_OBS_CASTLE1) ;
                                            }
                                        else
                                            {
                                            GAMESOUND_Play (GAME_SOUND_OBS_MOVE1) ;
                                            }
                                        }
                                    }
                                else
                                    {
                                    if (strchr (cLastMove, ICS_KING_IN_CHECK_SYMBOL))
                                        {
                                        GAMESOUND_Play (GAME_SOUND_OBS_CHECK2) ;
                                        }
                                    else
                                    if (strchr (cLastMove, ICS_PIECE_CAPTURE_SYMBOL))
                                        {
                                        GAMESOUND_Play (GAME_SOUND_OBS_CAPTURE2) ;
                                        }
                                    else
                                    if (strchr (cLastMove, ICS_CASTLE_SYMBOL))
                                        {
                                        GAMESOUND_Play (GAME_SOUND_OBS_CASTLE2) ;
                                        }
                                    else
                                        {
                                        GAMESOUND_Play (GAME_SOUND_OBS_MOVE2) ;
                                        }
                                    }
                                }
                            }
                        }

                    POSITION_SaveGameMove (nI) ;
                    return 1 ;
                    }
                }
            }

        //
        // if it's a game that we unobserved then eat this style 12 string
        //
        if (System.bUnobserve)
            {
            long nTmp ;
            TIMEMARK now ;

            CLOCK_GetTimeMark (&now) ;

            nTmp = CLOCK_SubtractTimeMarks (&now, &System.tUnobserve) ;
            if (nTmp <= 3)
                {
                if ((System.nUnobGameNumber              == nN) &&
                    (stricmp (System.cUnobWname, cwName) == 0 ) &&
                    (stricmp (System.cUnobBname, cbName) == 0 ))
                    {
                    if (strlen (Vars.cObserve) == 0)
                        {
                        //
                        // skip this second style 12 string
                        //
                        System.bUnobserve           = 0 ;
                        System.nUnobGameNumber      = 0 ;
                        strcpy (System.cUnobWname, "") ;
                        strcpy (System.cUnobBname, "") ;

                        sprintf (cGarbage, "%s %d\n", ICS_UNOBSERVE_COMMAND, nN) ;
                        TOOLBOX_WriteICS (cGarbage) ;
                        return 0 ;
                        }
                    else
                        {
                        if ((! TOOLBOX_MatchHandle (System.cUnobWname, Vars.cObserve)) &&
                            (! TOOLBOX_MatchHandle (System.cUnobBname, Vars.cObserve)))
                            {
                            //
                            // skip this second style 12 string
                            //
                            System.bUnobserve           = 0 ;
                            System.nUnobGameNumber      = 0 ;
                            strcpy (System.cUnobWname, "") ;
                            strcpy (System.cUnobBname, "") ;

                            sprintf (cGarbage, "%s %d\n", ICS_UNOBSERVE_COMMAND, nN) ;
                            TOOLBOX_WriteICS (cGarbage) ;
                            return 0 ;
                            }
                        }
                    }
                }

            System.bUnobserve           = 0 ;
            System.nUnobGameNumber      = 0 ;
            strcpy (System.cUnobWname, "") ;
            strcpy (System.cUnobBname, "") ;
            }

        if (nRl == -3)  // -3 isolated position, such as for "ref 3" or the "sposition" command
            {
            nG = nN + 1 ;
            }
        else
            {
            //
            //<g1> 14 p=0 t=crazyhouse r=1 u=0,0 it=180,0 i=180,0 pt=0 rt=1778,1785 ts=1,1 m=2 n=0
            //<g1> 62 p=0 t=lightning r=1 u=0,0 it=60,0 i=60,0 pt=0 rt=1231,1599 ts=1,1 m=2 n=0
            //<g1> 48 p=0 t=standard r=0 u=1,1 it=900,0 i=900,0 pt=0 rt=0P,0P ts=0,1 m=2 n=0
            //<g1> 73 p=0 t=blitz r=1 u=0,0 it=120,12 i=120,12 pt=0 rt=1645,1743 ts=1,0 m=2 n=0
            //<g1> 77 p=0 t=wild/8 r=0 u=0,1 it=300,0 i=300,0 pt=0 rt=0P,0P ts=1,0 m=2 n=0
            //<g1> 64 p=0 t=wild/fr r=1 u=0,0 it=180,10 i=180,10 pt=0 rt=1720P,1720P ts=1,1 m=2 n=0
            //<g1> 39 p=0 t=bughouse r=1 u=0,0 it=180,0 i=180,0 pt=26 rt=1931,2108 ts=1,1 m=2 n=0
            //
            //<g1> game_number p=private(1/0) t=type r=rated(1/0)
            //u=white_registered(1/0),black_registered(1/0)
            //it=initial_white_time,initial_black_time
            //i=initial_white_inc,initial_black_inc
            //pt=partner's_game_number(or 0 if none)
            //rt=white_rating(+ provshow character),black_rating(+ provshow character)
            //ts=white_uses_timeseal(0/1),black_uses_timeseal(0/1)
            //

            sscanf (Telnet.cLastGameInfo + 5, "%d p=%d t=%s", &nG, &nGarbage, &cGame) ;

            if (stricmp (cGame, "bughouse") == 0)
                {
                bB = 1 ;
                bC = 0 ;
                bT = GAMETYPE_BUGHOUSE ;
                strcpy (cGame, "bug") ;
                }
            else
            if (stricmp (cGame, "crazyhouse") == 0)
                {
                bB = 0 ;
                bC = 1 ;
                bT = GAMETYPE_CRAZYHOUSE ;
                strcpy (cGame, "zh") ;
                }
            else
            if (stricmp (cGame, "suicide") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_SUICIDE ;
                }
            else
            if (stricmp (cGame, "lightning") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_LIGHTNING ;
                }
            else
            if (stricmp (cGame, "blitz") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_BLITZ ;
                }
            else
            if (stricmp (cGame, "standard") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_STANDARD ;
                }
            else
            if (stricmp (cGame, "wild/0") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD0 ;
                strcpy (cGame, "w0") ;
                }
            else
            if (stricmp (cGame, "wild/1") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD1 ;
                strcpy (cGame, "w1") ;
                }
            else
            if (stricmp (cGame, "wild/2") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD2 ;
                strcpy (cGame, "w2") ;
                }
            else
            if (stricmp (cGame, "wild/3") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD3 ;
                strcpy (cGame, "w3") ;
                }
            else
            if (stricmp (cGame, "wild/4") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD4 ;
                strcpy (cGame, "w4") ;
                }
            else
            if (stricmp (cGame, "wild/5") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_WILD5 ;
                strcpy (cGame, "w5") ;
                }
            else
            if (stricmp (cGame, "wild/8") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD8 ;
                strcpy (cGame, "w8") ;
                }
            else
            if (stricmp (cGame, "wild/8a") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILD8A ;
                strcpy (cGame, "w8a") ;
                }
            else
            if (stricmp (cGame, "wild/fr") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_WILDFR ;
                strcpy (cGame, "wfr") ;
                }
            else
            if (stricmp (cGame, "atomic") == 0)
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_FICS_ATOMIC ;
                }
            else
                {
                bB = 0 ;
                bC = 0 ;
                bT = GAMETYPE_CHESS ;
                }
            }

        // recycle observe window
        for (nI = 1 ; nI < MAX_GAME ; nI++)
            {
            if (Game [nI].bValid)
                {
                if (Game [nI].nGameNumber == 0)
                    {
                    Game [nI].bFirstResize = 1 ;
                    Game [nI].bFirstGame   = 1 ;
                    Game [nI].bValid       = 1 ;
                    Game [nI].nGamePartner = 0 ;
                    Game [nI].nRelation    = nRl ;

                    bOrgFlip = Game [nI].bFlip ;

                    bNotPartner = (stricmp (Vars.cPartner, cwName) != 0 && stricmp (Vars.cPartner, cbName) != 0) ;

                    if (nN == nG)
                        {
                        if (bB)     // bughouse game
                            {
                            if (bNotPartner)
                                {
                                if (User.bAutoObserve)
                                    {
                                    if (Fics.bPobserve)
                                        {
                                        Game [nI].bFlip = 0 ;

                                        if (strlen (Vars.cPfollow) > 0)
                                            {
                                            if (stricmp (cbName, Vars.cPfollow) == 0)
                                                {
                                                Game [nI].bFlip = 1 ;
                                                }
                                            }

                                        if (strlen (Vars.cFollow) > 0)
                                            {
                                            if (stricmp (cbName, Vars.cFollow) == 0)
                                                {
                                                Game [nI].bFlip = 1 ;
                                                }
                                            }

                                        if (strlen (Vars.cObserve) > 0)
                                            {
                                            if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                                {
                                                Game [nI].bFlip = 1 ;
                                                strcpy (Vars.cObserve, "") ;
                                                }
                                            }
                                        }
                                    else
                                        {
                                        Game [nI].bFlip = 1 ;

                                        if (strlen (Vars.cPfollow) > 0)
                                            {
                                            if (stricmp (cbName, Vars.cPfollow) == 0)
                                                {
                                                Game [nI].bFlip = 0 ;
                                                }
                                            }

                                        if (strlen (Vars.cFollow) > 0)
                                            {
                                            if (stricmp (cbName, Vars.cFollow) == 0)
                                                {
                                                Game [nI].bFlip = 0 ;
                                                }
                                            }

                                        if (strlen (Vars.cObserve) > 0)
                                            {
                                            if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                                {
                                                Game [nI].bFlip = 0 ;
                                                strcpy (Vars.cObserve, "") ;
                                                }
                                            }

                                        if (Fics.nPobserve != 0)
                                            {
                                            for (nJ = 1 ; nJ < MAX_GAME ; nJ++)
                                                {
                                                if (Game [nJ].bValid)
                                                    {
                                                    if (Game [nJ].nGameNumber == Fics.nPobserve)
                                                        {
                                                        Game [nJ].nGamePartner = nN ;
                                                        Game [nI].nGamePartner = Game [nJ].nGameNumber ;
                                                        Game [nI].bFlip        = (! Game [nJ].bFlip) ;
                                                        break ;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                else
                                    {
                                    Game [nI].bFlip = 0 ;

                                    if (strlen (Vars.cPfollow) > 0)
                                        {
                                        if (stricmp (cbName, Vars.cPfollow) == 0)
                                            {
                                            Game [nI].bFlip = 1 ;
                                            }
                                        }

                                    if (strlen (Vars.cFollow) > 0)
                                        {
                                        if (stricmp (cbName, Vars.cFollow) == 0)
                                            {
                                            Game [nI].bFlip = 1 ;
                                            }
                                        }

                                    if (strlen (Vars.cObserve) > 0)
                                        {
                                        if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                            {
                                            Game [nI].bFlip = 1 ;
                                            strcpy (Vars.cObserve, "") ;
                                            }
                                        }
                                    }
                                }
                            else
                                {
                                if (Game [INDEX_PLAY].nGameNumber > 0)
                                    {
                                    Game [nI].bFlip        = ! Game [INDEX_PLAY].bFlip ;
                                    Game [nI].nGamePartner = Game [INDEX_PLAY].nGameNumber ;
                                    }
                                else
                                    {
                                    Game [nI].bFlip = 0 ;
                                    }
                                }
                            }
                        else    // single board game
                            {
                            Game [nI].bFlip = 0 ;

                            if (strlen (Vars.cPfollow) > 0)
                                {
                                if (stricmp (cbName, Vars.cPfollow) == 0)
                                    {
                                    Game [nI].bFlip = 1 ;
                                    }
                                }

                            if (strlen (Vars.cFollow) > 0)
                                {
                                if (stricmp (cbName, Vars.cFollow) == 0)
                                    {
                                    Game [nI].bFlip = 1 ;
                                    }
                                }

                            if (strlen (Vars.cObserve) > 0)
                                {
                                if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                    {
                                    Game [nI].bFlip = 1 ;
                                    strcpy (Vars.cObserve, "") ;
                                    }
                                }
                            }


                        if (TOOLBOX_ShowBuffer1 (Game [nI].nGameType))
                            {
                            Game [nI].nGameType = bT ;

                            if (! TOOLBOX_ShowBuffer1 (bT))
                                {
                                RECT rc ;

                                GetClientRect (Game [nI].hwnd, &rc) ;

                                hdc = GetDC (Game [nI].hwnd) ;

                                BOARD_ResizeNoBuffer (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                BOARD_LoadBitmaps (nI, hdc) ;
                                BOARD_CheckFlip (nI) ;
                                BOARD_DrawRepaint (nI, Game [nI].hwnd, hdc) ;

                                ReleaseDC (Game [nI].hwnd, hdc) ;
                                }
                            }
                        else
                            {
                            Game [nI].nGameType = bT ;

                            if (TOOLBOX_ShowBuffer1 (bT))
                                {
                                RECT rc ;

                                GetClientRect (Game [nI].hwnd, &rc) ;

                                hdc = GetDC (Game [nI].hwnd) ;

                                switch (User.nBufferOrientation)
                                    {
                                    case DEFAULT_BUFFER_LEFT :
                                        BOARD_ResizeLeft (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                        break ;

                                    case DEFAULT_BUFFER_RIGHT :
                                        BOARD_ResizeRight (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                        break ;

                                    case DEFAULT_BUFFER_TOPBOTTOML :
                                        BOARD_ResizeTopBottomL (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                        break ;

                                    case DEFAULT_BUFFER_TOPBOTTOMR :
                                        BOARD_ResizeTopBottomR (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                        break ;

                                    default :
                                        BOARD_ResizeRight (nI, hdc, (rc.right - rc.left) + 1, (rc.bottom - rc.top) + 1) ;
                                        break ;
                                    }

                                BOARD_LoadBitmaps (nI, hdc) ;
                                BOARD_CheckFlip (nI) ;
                                BOARD_DrawRepaint (nI, Game [nI].hwnd, hdc) ;

                                ReleaseDC (Game [nI].hwnd, hdc) ;
                                }
                            }
                        }
                    else
                        {
                        if (bNotPartner)
                            {
                            if (User.bAutoObserve)
                                {
                                if (Fics.bPobserve)
                                    {
                                    Game [nI].bFlip = 0 ;
                                    }
                                else
                                    {
                                    Game [nI].bFlip = 1 ;

                                    if (Fics.nPobserve != 0)
                                        {
                                        for (nJ = 1 ; nJ < MAX_GAME ; nJ++)
                                            {
                                            if (Game [nJ].bValid)
                                                {
                                                if (Game [nJ].nGameNumber == Fics.nPobserve)
                                                    {
                                                    Game [nJ].nGamePartner = nN ;
                                                    Game [nI].nGamePartner = Game [nJ].nGameNumber ;
                                                    break ;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            else
                                {
                                Game [nI].bFlip = 0 ;
                                }
                            }
                        else
                            {
                            if (Game [INDEX_PLAY].nGameNumber > 0)
                                {
                                Game [nI].bFlip        = ! Game [INDEX_PLAY].bFlip ;
                                Game [nI].nGamePartner = Game [INDEX_PLAY].nGameNumber ;
                                }
                            else
                                {
                                Game [nI].bFlip = 0 ;
                                }
                            }
                        }

                    Game [nI].nGameNumber     = nN ;
                    Game [nI].bChessGame      = 0 ;
                    Game [nI].bWhitesMove     = (cTurn == 'W') ;
                    Game [nI].bLastWhitesMove = (! Game [nI].bWhitesMove) ;
                    Game [nI].bPlaying        = (nRl == 0) ;
                    Game [nI].bInitialMove    = (strcmp (cLastMove, ICS_INITIAL_MOVE_NAME) == 0) ;
                    Game [nI].nMoveNumber     = nMn ;

                    if (! Game [nI].bPlaying)
                        {
                        bNotPartner = 0 ;
                        }

                    strcpy (Game [nI].cLastMove,             cLastMove) ;
                    strcpy (Game [nI].cVerboseMove,          cVerboseMove) ;
                    strcpy (Game [nI].cTime4Move,            cTime4Move) ;
                    strcpy (Game [nI].cHandle [INDEX_WHITE], cwName) ;
                    strcpy (Game [nI].cHandle [INDEX_BLACK], cbName) ;
                    strcpy (Game [nI].cRating [INDEX_WHITE], "") ;
                    strcpy (Game [nI].cRating [INDEX_BLACK], "") ;

                    Game [nI].nRated = ICS_INITIAL_RATED ;

                    if (nN == nG)
                        {
                        Game [nI].nGameType = bT ;
                        strcpy (Game [nI].cGameType, cGame) ;
                        }
                    else
                        {
                        Game [nI].nGameType = System.nLastObserveGameType ;
                        strcpy (Game [nI].cGameType, ICS_INITIAL_GAMETYPE_STRING) ;
                        }

                    strcpy (Game [nI].cOrgResult, "") ;
                    strcpy (Game [nI].cResult,    "") ;

                    Game [nI].bSavedGame                        = 0 ;
                    Game [nI].nLastDoublePushFile               = nLP ;
                    Game [nI].bCanCastleKingSide  [INDEX_WHITE] = bWCK ;
                    Game [nI].bCanCastleQueenSide [INDEX_WHITE] = bWCQ ;
                    Game [nI].bCanCastleKingSide  [INDEX_BLACK] = bBCK ;
                    Game [nI].bCanCastleQueenSide [INDEX_BLACK] = bBCQ ;
                    Game [nI].nHalfMoves                        = nHalf ;

                    Game [nI].nInitialClock                = nIc ;
                    Game [nI].nIncrementClock              = nIi ;
                    Game [nI].nTimeRemaining [INDEX_WHITE] = nWc ;
                    Game [nI].nTimeRemaining [INDEX_BLACK] = nBc ;
                    Game [nI].nClockTimerEvent             = 0 ;
                    Game [nI].bTickClock                   = nClock ;
                    Game [nI].bFlagged [INDEX_WHITE]       = 0 ;
                    Game [nI].bFlagged [INDEX_BLACK]       = 0 ;

                    Game [nI].tTickStartTM.nSec   = 0 ;
                    Game [nI].tTickStartTM.nMs    = 0 ;
                    Game [nI].nIntendedTickLength = 0 ;
                    Game [nI].nNextLagCheck       = 1000 ;
                    Game [nI].nLag [INDEX_WHITE]  = 0 ;
                    Game [nI].nLag [INDEX_BLACK]  = 0 ;

                    if (Game [nI].bPlaying)
                        {
                        if (User.bShowLagStat || User.bLagCommand)
                            {
                            if (Game [nI].bWhitesMove != Game [nI].bLastWhitesMove)
                                {
                                if (Game [nI].bWhitesMove)
                                    {
                                    Game [nI].nLag [INDEX_BLACK] += nLag ;
                                    }
                                else
                                    {
                                    Game [nI].nLag [INDEX_WHITE] += nLag ;
                                    }
                                }
                            }
                        }

                    Game [nI].bLastWhitesMove = Game [nI].bWhitesMove ;

                    Game [nI].bIssuedEventCommand = 0 ;

                    CLOCK_StartClocks (nI, Game [nI].hwnd) ;

                    BOARD_LoadBoard  (nI, cS + 5, ICS_INITIAL_MOVE_NAME) ;
                    BOARD_LoadBuffer (nI, INIT_BUFFER, INIT_BUFFER) ;

                    BOARD_GetHighlightFromLastMove (nI, cVerboseMove, cLastMove) ;

                    BOARD_NullLastHighlight   (nI) ;
                    BOARD_NullLastBoardBuffer (nI) ;

                    if (Game [nI].bValid)
                        {
                        hdc = GetDC (Game [nI].hwnd) ;

                        if (bOrgFlip != Game [nI].bFlip)
                            {
                            BOARD_CheckFlip (nI) ;
                            }

                        BOARD_DrawHandle      (nI, hdc) ;
                        BOARD_DrawWhiteLag    (nI, hdc) ;
                        BOARD_DrawBlackLag    (nI, hdc) ;
                        BOARD_DrawWhiteClock  (nI, hdc) ;
                        BOARD_DrawBlackClock  (nI, hdc) ;
                        BOARD_DrawGameType    (nI, hdc) ;
                        BOARD_DrawLastMove    (nI, hdc) ;
                        BOARD_DrawResult      (nI, hdc) ;
                        BOARD_DrawCoordinates (nI, hdc) ;
                        BOARD_DrawBoard1      (nI, Game [nI].hwnd, hdc, DRAW_STATE_BOARD) ;

                        ReleaseDC (Game [nI].hwnd, hdc) ;

                        SetWindowText (Game [nI].hwnd, TOOLBOX_GetGameWindowTitle (nI)) ;

                        if (Telnet.nTelnetState == WIN_MAXIMIZE)
                            {
                            wCoord [COORD_TELNET].s = WIN_SHOW ;
                            ShowWindow (hwndWindow [HWND_TELNET], SW_RESTORE) ;
                            }

                        if (Game [nI].nState == WIN_MINIMIZE)
                            {
                            ShowWindow (Game [nI].hwnd, SW_RESTORE) ;
                            }
                        else
                            {
                            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) Game [nI].hwnd, 0) ;
                            SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
                            }

                        POSITION_FirstTime (nI) ;

                        if (Game [nI].nRelation != -3)
                            {
                            sprintf (cGarbage, "%s %d\n", FICS_MOVELIST_COMMAND, nN) ;
                            TOOLBOX_WriteICS (cGarbage) ;

                            System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1 ;
                            }

                        BUGHOUSE_FirstTime (nI) ;

                        // auto observe
                        if (User.bAutoObserve)
                            {
                            if (nN == nG)
                                {
                                if (bNotPartner && Fics.bPobserve && bB)
                                    {
                                    Fics.bPobserve = 0 ;
                                    Fics.nPobserve = nN ;
                                    strcpy (Fics.cPobserve, cwName) ;
                                    strcat (Fics.cPobserve, FICS_POBSERVE_FALSE_STRING) ;

                                    TOOLBOX_WriteICS (FICS_POBSERVE_COMMAND) ;
                                    TOOLBOX_WriteICS (cwName) ;
                                    TOOLBOX_WriteICS ("\n") ;
                                    }
                                else
                                    {
                                    Fics.bPobserve = 1 ;
                                    Fics.nPobserve = 0 ;
                                    strcpy (Fics.cPobserve, "") ;
                                    }
                                }
                            else
                                {
                                if (bNotPartner && Fics.bPobserve)
                                    {
                                    Fics.bPobserve = 0 ;
                                    Fics.nPobserve = nN ;
                                    strcpy (Fics.cPobserve, cwName) ;
                                    strcat (Fics.cPobserve, FICS_POBSERVE_FALSE_STRING) ;

                                    TOOLBOX_WriteICS (FICS_POBSERVE_COMMAND) ;
                                    TOOLBOX_WriteICS (cwName) ;
                                    TOOLBOX_WriteICS ("\n") ;
                                    }
                                else
                                    {
                                    Fics.bPobserve = 1 ;
                                    Fics.nPobserve = 0 ;
                                    strcpy (Fics.cPobserve, "") ;

                                    if (Game [INDEX_PLAY].nGamePartner != nN)
                                        {
                                        nA = 0 ;

                                        if (strlen (Vars.cPfollow) > 0)
                                            {
                                            if (stricmp (cwName, Vars.cPfollow) == 0)
                                                {
                                                if (Game [nI].bFlip)
                                                    {
                                                    nA = 1 ;
                                                    }
                                                }
                                            }

                                        if (strlen (Vars.cFollow) > 0)
                                            {
                                            if (stricmp (cwName, Vars.cFollow) == 0)
                                                {
                                                if (Game [nI].bFlip)
                                                    {
                                                    nA = 1 ;
                                                    }
                                                }
                                            }

                                        if (! nA)
                                            {
                                            for (nJ = 1 ; nJ < MAX_GAME ; nJ++)
                                                {
                                                if (Game [nJ].bValid)
                                                    {
                                                    if (Game [nJ].nGamePartner == nN)
                                                        {
                                                        if (strlen (Vars.cPfollow) > 0)
                                                            {
                                                            if (stricmp (Game [nJ].cHandle [INDEX_BLACK], Vars.cPfollow) == 0)
                                                                {
                                                                if (! Game [nJ].bFlip)
                                                                    {
                                                                    nA = 1 ;
                                                                    }
                                                                }
                                                            }

                                                        if (strlen (Vars.cFollow) > 0)
                                                            {
                                                            if (stricmp (Game [nJ].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
                                                                {
                                                                if (! Game [nJ].bFlip)
                                                                    {
                                                                    nA = 1 ;
                                                                    }
                                                                }
                                                            }

                                                        if (! nA)
                                                            {
                                                            if (strlen (Vars.cObserve) > 0)
                                                                {
                                                                if (TOOLBOX_MatchHandle (Game [nJ].cHandle [INDEX_WHITE], Vars.cObserve))
                                                                    {
                                                                    strcpy (Vars.cObserve, "") ;

                                                                    if (Game [nJ].bFlip)
                                                                        {
                                                                        nA = 1 ;
                                                                        }
                                                                    }
                                                                else
                                                                if (TOOLBOX_MatchHandle (Game [nJ].cHandle [INDEX_BLACK], Vars.cObserve))
                                                                    {
                                                                    strcpy (Vars.cObserve, "") ;

                                                                    if (! Game [nJ].bFlip)
                                                                        {
                                                                        nA = 1 ;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        break ;
                                                        }
                                                    }
                                                }
                                            }

                                        if (nA)
                                            {
                                            PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                            }
                                        else
                                            {
                                            if (strlen (Vars.cObserve) > 0)
                                                {
                                                if (TOOLBOX_MatchHandle (cwName, Vars.cObserve))
                                                    {
                                                    strcpy (Vars.cObserve, "") ;

                                                    if (Game [nI].bFlip)
                                                        {
                                                        PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                                        }
                                                    }
                                                else
                                                if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                                    {
                                                    strcpy (Vars.cObserve, "") ;

                                                    if (! Game [nI].bFlip)
                                                        {
                                                        PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                    if (stricmp (Vars.cPartner, cwName) == 0)
                        {
                        if (IsWindow (Game [nI].hwnd))
                            {
                            MoveWindow (Game [nI].hwnd, wPartner.x, wPartner.y, wPartner.w, wPartner.h, TRUE) ;
                            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) Game [nI].hwnd, 0) ;
                            }

                        strcpy (Vars.cPartnerOpponent, cbName) ;

                        if (Censor [Login.nLoginType].bCensor)
                            {
                            if (CENSOR_Other (7, Vars.cPartnerOpponent))
                                {
                                sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                TOOLBOX_WriteICS (cGarbage) ;

                                System.bIssuedAbort = 1 ;
                                }
                            else
                                {
                                System.bIssuedAbort = 0 ;
                                }

                            if (strlen (Vars.cPartner) > 0)
                                {
                                if (CENSOR_Partner (Vars.cPartner))
                                    {
                                    if (! System.bIssuedAbort)
                                        {
                                        sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                        TOOLBOX_WriteICS (cGarbage) ;

                                        System.bIssuedAbort = 1 ;
                                        }

                                    TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                                    }
                                }
                            }

                        STATE_ObserveNewGame (nI, 1) ;
                        }
                    else
                    if (stricmp (Vars.cPartner, cbName) == 0)
                        {
                        if (IsWindow (Game [nI].hwnd))
                            {
                            MoveWindow (Game [nI].hwnd, wPartner.x, wPartner.y, wPartner.w, wPartner.h, TRUE) ;
                            SendMessage (hwndWindow [HWND_CLIENT], WM_MDIACTIVATE, (WPARAM) (HWND) Game [nI].hwnd, 0) ;
                            }

                        strcpy (Vars.cPartnerOpponent, cwName) ;

                        if (Censor [Login.nLoginType].bCensor)
                            {
                            if (CENSOR_Other (7, Vars.cPartnerOpponent))
                                {
                                sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                TOOLBOX_WriteICS (cGarbage) ;

                                System.bIssuedAbort = 1 ;
                                }
                            else
                                {
                                System.bIssuedAbort = 0 ;
                                }

                            if (strlen (Vars.cPartner) > 0)
                                {
                                if (CENSOR_Partner (Vars.cPartner))
                                    {
                                    if (! System.bIssuedAbort)
                                        {
                                        sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                        TOOLBOX_WriteICS (cGarbage) ;

                                        System.bIssuedAbort = 1 ;
                                        }

                                    TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                                    }
                                }
                            }

                        STATE_ObserveNewGame (nI, 1) ;
                        }
                    else
                        {
                        STATE_ObserveNewGame (nI, 0) ;
                        }
                    return 1 ;
                    }
                }
            }

        // new window
        for (nI = 1 ; nI < MAX_GAME ; nI++)
            {
            if (! Game [nI].bValid)
                {
                Game [nI].bFirstResize = 1 ;
                Game [nI].bFirstGame   = 1 ;
                Game [nI].bValid       = 1 ;
                Game [nI].nGamePartner = 0 ;
                Game [nI].nRelation    = nRl ;

                bNotPartner = (stricmp (Vars.cPartner, cwName) != 0 && stricmp (Vars.cPartner, cbName) != 0) ;

                if (nN == nG)
                    {
                    if (bB)     // bughouse game
                        {
                        if (bNotPartner)
                            {
                            if (User.bAutoObserve)
                                {
                                if (Fics.bPobserve)
                                    {
                                    Game [nI].bFlip = 0 ;

                                    if (strlen (Vars.cPfollow) > 0)
                                        {
                                        if (stricmp (cbName, Vars.cPfollow) == 0)
                                            {
                                            Game [nI].bFlip = 1 ;
                                            }
                                        }

                                    if (strlen (Vars.cFollow) > 0)
                                        {
                                        if (stricmp (cbName, Vars.cFollow) == 0)
                                            {
                                            Game [nI].bFlip = 1 ;
                                            }
                                        }

                                    if (strlen (Vars.cObserve) > 0)
                                        {
                                        if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                            {
                                            Game [nI].bFlip = 1 ;
                                            strcpy (Vars.cObserve, "") ;
                                            }
                                        }
                                    }
                                else
                                    {
                                    Game [nI].bFlip = 1 ;

                                    if (strlen (Vars.cPfollow) > 0)
                                        {
                                        if (stricmp (cbName, Vars.cPfollow) == 0)
                                            {
                                            Game [nI].bFlip = 0 ;
                                            }
                                        }

                                    if (strlen (Vars.cFollow) > 0)
                                        {
                                        if (stricmp (cbName, Vars.cFollow) == 0)
                                            {
                                            Game [nI].bFlip = 0 ;
                                            }
                                        }

                                    if (strlen (Vars.cObserve) > 0)
                                        {
                                        if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                            {
                                            Game [nI].bFlip = 0 ;
                                            strcpy (Vars.cObserve, "") ;
                                            }
                                        }

                                    if (Fics.nPobserve != 0)
                                        {
                                        for (nJ = 1 ; nJ < MAX_GAME ; nJ++)
                                            {
                                            if (Game [nJ].bValid)
                                                {
                                                if (Game [nJ].nGameNumber == Fics.nPobserve)
                                                    {
                                                    Game [nJ].nGamePartner = nN ;
                                                    Game [nI].nGamePartner = Game [nJ].nGameNumber ;
                                                    Game [nI].bFlip        = (! Game [nJ].bFlip) ;
                                                    break ;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            else
                                {
                                Game [nI].bFlip = 0 ;

                                if (strlen (Vars.cPfollow) > 0)
                                    {
                                    if (stricmp (cbName, Vars.cPfollow) == 0)
                                        {
                                        Game [nI].bFlip = 1 ;
                                        }
                                    }

                                if (strlen (Vars.cFollow) > 0)
                                    {
                                    if (stricmp (cbName, Vars.cFollow) == 0)
                                        {
                                        Game [nI].bFlip = 1 ;
                                        }
                                    }

                                if (strlen (Vars.cObserve) > 0)
                                    {
                                    if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                        {
                                        Game [nI].bFlip = 1 ;
                                        strcpy (Vars.cObserve, "") ;
                                        }
                                    }
                                }
                            }
                        else
                            {
                            if (Game [INDEX_PLAY].nGameNumber > 0)
                                {
                                Game [nI].bFlip        = ! Game [INDEX_PLAY].bFlip ;
                                Game [nI].nGamePartner = Game [INDEX_PLAY].nGameNumber ;
                                }
                            else
                                {
                                Game [nI].bFlip = 0 ;
                                }
                            }
                        }
                    else    // single board game
                        {
                        Game [nI].bFlip = 0 ;

                        if (strlen (Vars.cPfollow) > 0)
                            {
                            if (stricmp (cbName, Vars.cPfollow) == 0)
                                {
                                Game [nI].bFlip = 1 ;
                                }
                            }

                        if (strlen (Vars.cFollow) > 0)
                            {
                            if (stricmp (cbName, Vars.cFollow) == 0)
                                {
                                Game [nI].bFlip = 1 ;
                                }
                            }

                        if (strlen (Vars.cObserve) > 0)
                            {
                            if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                {
                                Game [nI].bFlip = 1 ;
                                strcpy (Vars.cObserve, "") ;
                                }
                            }
                        }
                    }
                else
                    {
                    if (bNotPartner)
                        {
                        if (User.bAutoObserve)
                            {
                            if (Fics.bPobserve)
                                {
                                Game [nI].bFlip = 0 ;
                                }
                            else
                                {
                                Game [nI].bFlip = 1 ;

                                if (Fics.nPobserve != 0)
                                    {
                                    for (nJ = 1 ; nJ < MAX_GAME ; nJ++)
                                        {
                                        if (Game [nJ].bValid)
                                            {
                                            if (Game [nJ].nGameNumber == Fics.nPobserve)
                                                {
                                                Game [nJ].nGamePartner = nN ;
                                                Game [nI].nGamePartner = Game [nJ].nGameNumber ;
                                                break ;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        else
                            {
                            Game [nI].bFlip = 0 ;
                            }
                        }
                    else
                        {
                        if (Game [INDEX_PLAY].nGameNumber > 0)
                            {
                            Game [nI].bFlip        = ! Game [INDEX_PLAY].bFlip ;
                            Game [nI].nGamePartner = Game [INDEX_PLAY].nGameNumber ;
                            }
                        else
                            {
                            Game [nI].bFlip = 0 ;
                            }
                        }
                    }

                Game [nI].nGameNumber     = nN ;
                Game [nI].bChessGame      = 0 ;
                Game [nI].bWhitesMove     = (cTurn == 'W') ;
                Game [nI].bLastWhitesMove = (! Game [nI].bWhitesMove) ;
                Game [nI].bPlaying        = (nRl == 0) ;
                Game [nI].bInitialMove    = (strcmp (cLastMove, ICS_INITIAL_MOVE_NAME) == 0) ;
                Game [nI].nMoveNumber     = nMn ;

                if (! Game [nI].bPlaying)
                    {
                    bNotPartner = 0 ;
                    }

                strcpy (Game [nI].cLastMove,             cLastMove) ;
                strcpy (Game [nI].cVerboseMove,          cVerboseMove) ;
                strcpy (Game [nI].cTime4Move,            cTime4Move) ;
                strcpy (Game [nI].cHandle [INDEX_WHITE], cwName) ;
                strcpy (Game [nI].cHandle [INDEX_BLACK], cbName) ;
                strcpy (Game [nI].cRating [INDEX_WHITE], "") ;
                strcpy (Game [nI].cRating [INDEX_BLACK], "") ;

                Game [nI].nRated = ICS_INITIAL_RATED ;

                if (nN == nG)
                    {
                    Game [nI].nGameType = bT ;
                    strcpy (Game [nI].cGameType, cGame) ;
                    }
                else
                    {
                    Game [nI].nGameType = System.nLastObserveGameType ;
                    strcpy (Game [nI].cGameType, ICS_INITIAL_GAMETYPE_STRING) ;
                    }

                strcpy (Game [nI].cOrgResult, "") ;
                strcpy (Game [nI].cResult,    "") ;

                Game [nI].bSavedGame                        = 0 ;
                Game [nI].nLastDoublePushFile               = nLP ;
                Game [nI].bCanCastleKingSide  [INDEX_WHITE] = bWCK ;
                Game [nI].bCanCastleQueenSide [INDEX_WHITE] = bWCQ ;
                Game [nI].bCanCastleKingSide  [INDEX_BLACK] = bBCK ;
                Game [nI].bCanCastleQueenSide [INDEX_BLACK] = bBCQ ;
                Game [nI].nHalfMoves                        = nHalf ;

                Game [nI].nInitialClock                = nIc ;
                Game [nI].nIncrementClock              = nIi ;
                Game [nI].nTimeRemaining [INDEX_WHITE] = nWc ;
                Game [nI].nTimeRemaining [INDEX_BLACK] = nBc ;
                Game [nI].nClockTimerEvent             = 0 ;
                Game [nI].bTickClock                   = nClock ;
                Game [nI].bFlagged [INDEX_WHITE]       = 0 ;
                Game [nI].bFlagged [INDEX_BLACK]       = 0 ;

                Game [nI].tTickStartTM.nSec   = 0 ;
                Game [nI].tTickStartTM.nMs    = 0 ;
                Game [nI].nIntendedTickLength = 0 ;
                Game [nI].nNextLagCheck       = 1000 ;
                Game [nI].nLag [INDEX_WHITE]  = 0 ;
                Game [nI].nLag [INDEX_BLACK]  = 0 ;

                if (Game [nI].bPlaying)
                    {
                    if (User.bShowLagStat || User.bLagCommand)
                        {
                        if (Game [nI].bWhitesMove != Game [nI].bLastWhitesMove)
                            {
                            if (Game [nI].bWhitesMove)
                                {
                                Game [nI].nLag [INDEX_BLACK] += nLag ;
                                }
                            else
                                {
                                Game [nI].nLag [INDEX_WHITE] += nLag ;
                                }
                            }
                        }
                    }

                Game [nI].bLastWhitesMove = Game [nI].bWhitesMove ;

                Game [nI].bIssuedEventCommand = 0 ;

                BOARD_LoadBoard  (nI, cS + 5, ICS_INITIAL_MOVE_NAME) ;
                BOARD_LoadBuffer (nI, INIT_BUFFER, INIT_BUFFER) ;

                BOARD_GetHighlightFromLastMove (nI, cVerboseMove, cLastMove) ;

                BOARD_NullLastHighlight   (nI) ;
                BOARD_NullLastBoardBuffer (nI) ;

                mdicreate.szClass = "MdiObserveChild" ;
                mdicreate.szTitle = "Observe" ;
                mdicreate.hOwner  = hInst ;

                if (nN == nG)
                    {
                    if (bB)
                        {
                        if (stricmp (Vars.cPartner, cwName) == 0)
                            {
                            bPartner = 1 ;

                            if ((GEvent.nType [GAME_EVENT_PLAY_BUG_START] >= FUNCTION_RESTORE_LAYOUT0) &&
                                (GEvent.nType [GAME_EVENT_PLAY_BUG_START] <= FUNCTION_RESTORE_LAYOUT8))
                                {
                                nJ = (GEvent.nType [GAME_EVENT_PLAY_BUG_START] - FUNCTION_RESTORE_LAYOUT0) ;

                                mdicreate.x  = wLayout [nJ] [1].x ;
                                mdicreate.y  = wLayout [nJ] [1].y ;
                                mdicreate.cx = wLayout [nJ] [1].w ;
                                mdicreate.cy = wLayout [nJ] [1].h ;
                                }
                            else
                                {
                                mdicreate.x  = wPartner.x ;
                                mdicreate.y  = wPartner.y ;
                                mdicreate.cx = wPartner.w ;
                                mdicreate.cy = wPartner.h ;
                                }

                            strcpy (Vars.cPartnerOpponent, cbName) ;

                            if (Censor [Login.nLoginType].bCensor)
                                {
                                if (CENSOR_Other (7, Vars.cPartnerOpponent))
                                    {
                                    sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                    TOOLBOX_WriteICS (cGarbage) ;

                                    System.bIssuedAbort = 1 ;
                                    }
                                else
                                    {
                                    System.bIssuedAbort = 0 ;
                                    }

                                if (strlen (Vars.cPartner) > 0)
                                    {
                                    if (CENSOR_Partner (Vars.cPartner))
                                        {
                                        if (! System.bIssuedAbort)
                                            {
                                            sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                            TOOLBOX_WriteICS (cGarbage) ;

                                            System.bIssuedAbort = 1 ;
                                            }

                                        TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                                        }
                                    }
                                }
                            }
                        else
                        if (stricmp (Vars.cPartner, cbName) == 0)
                            {
                            bPartner = 1 ;

                            if ((GEvent.nType [GAME_EVENT_PLAY_BUG_START] >= FUNCTION_RESTORE_LAYOUT0) &&
                                (GEvent.nType [GAME_EVENT_PLAY_BUG_START] <= FUNCTION_RESTORE_LAYOUT8))
                                {
                                nJ = (GEvent.nType [GAME_EVENT_PLAY_BUG_START] - FUNCTION_RESTORE_LAYOUT0) ;

                                mdicreate.x  = wLayout [nJ] [1].x ;
                                mdicreate.y  = wLayout [nJ] [1].y ;
                                mdicreate.cx = wLayout [nJ] [1].w ;
                                mdicreate.cy = wLayout [nJ] [1].h ;
                                }
                            else
                                {
                                mdicreate.x  = wPartner.x ;
                                mdicreate.y  = wPartner.y ;
                                mdicreate.cx = wPartner.w ;
                                mdicreate.cy = wPartner.h ;
                                }

                            strcpy (Vars.cPartnerOpponent, cwName) ;

                            if (Censor [Login.nLoginType].bCensor)
                                {
                                if (CENSOR_Other (7, Vars.cPartnerOpponent))
                                    {
                                    sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                    TOOLBOX_WriteICS (cGarbage) ;

                                    System.bIssuedAbort = 1 ;
                                    }
                                else
                                    {
                                    System.bIssuedAbort = 0 ;
                                    }

                                if (strlen (Vars.cPartner) > 0)
                                    {
                                    if (CENSOR_Partner (Vars.cPartner))
                                        {
                                        if (! System.bIssuedAbort)
                                            {
                                            sprintf (cGarbage, "%s\n", ICS_ABORT_COMMAND) ;
                                            TOOLBOX_WriteICS (cGarbage) ;

                                            System.bIssuedAbort = 1 ;
                                            }

                                        TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                                        }
                                    }
                                }
                            }
                        else
                            {
                            bPartner = 0 ;

                            if ((GEvent.nType [GAME_EVENT_OBSERVE_BUG_ST] >= FUNCTION_RESTORE_LAYOUT0) &&
                                (GEvent.nType [GAME_EVENT_OBSERVE_BUG_ST] <= FUNCTION_RESTORE_LAYOUT8))
                                {
                                nJ = (GEvent.nType [GAME_EVENT_OBSERVE_BUG_ST] - FUNCTION_RESTORE_LAYOUT0) ;

                                mdicreate.x  = wLayout [nJ] [nI].x ;
                                mdicreate.y  = wLayout [nJ] [nI].y ;
                                mdicreate.cx = wLayout [nJ] [nI].w ;
                                mdicreate.cy = wLayout [nJ] [nI].h ;
                                }
                            else
                                {
                                mdicreate.x  = wCoord [nI].x ;
                                mdicreate.y  = wCoord [nI].y ;
                                mdicreate.cx = wCoord [nI].w ;
                                mdicreate.cy = wCoord [nI].h ;
                                }
                            }
                        }
                    else
                        {
                        bPartner = 0 ;

                        if ((GEvent.nType [GAME_EVENT_OBSERVE_ONE_ST] >= FUNCTION_RESTORE_LAYOUT0) &&
                            (GEvent.nType [GAME_EVENT_OBSERVE_ONE_ST] <= FUNCTION_RESTORE_LAYOUT8))
                            {
                            nJ = (GEvent.nType [GAME_EVENT_OBSERVE_ONE_ST] - FUNCTION_RESTORE_LAYOUT0) ;

                            mdicreate.x  = wLayout [nJ] [nI].x ;
                            mdicreate.y  = wLayout [nJ] [nI].y ;
                            mdicreate.cx = wLayout [nJ] [nI].w ;
                            mdicreate.cy = wLayout [nJ] [nI].h ;
                            }
                        else
                            {
                            mdicreate.x  = wCoord [nI].x ;
                            mdicreate.y  = wCoord [nI].y ;
                            mdicreate.cx = wCoord [nI].w ;
                            mdicreate.cy = wCoord [nI].h ;
                            }
                        }
                    }
                else
                    {
                    bPartner = 0 ;

                    mdicreate.x  = wCoord [nI].x ;
                    mdicreate.y  = wCoord [nI].y ;
                    mdicreate.cx = wCoord [nI].w ;
                    mdicreate.cy = wCoord [nI].h ;
                    }


                mdicreate.style  = WS_OVERLAPPEDWINDOW | WS_VISIBLE ;
                mdicreate.lParam = 0 ;

                System.nGameIndex = nI ;

                Game [nI].hwnd = (HWND) SendMessage (hwndWindow [HWND_CLIENT], WM_MDICREATE, 0, (LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;

                if (Game [nI].bValid)
                    {
                    SetWindowText (Game [nI].hwnd, TOOLBOX_GetGameWindowTitle (nI)) ;

                    if (Telnet.nTelnetState == WIN_MAXIMIZE)
                        {
                        wCoord [COORD_TELNET].s = WIN_SHOW ;
                        ShowWindow (hwndWindow [HWND_TELNET], SW_RESTORE) ;
                        }

                    POSITION_FirstTime (nI) ;

                    if (Game [nI].nRelation != -3)
                        {
                        sprintf (cGarbage, "%s %d\n", FICS_MOVELIST_COMMAND, nN) ;
                        TOOLBOX_WriteICS (cGarbage) ;

                        System.nIssuedMovesCommand = System.nIssuedMovesCommand + 1 ;
                        }

                    BUGHOUSE_FirstTime (nI) ;

                    // auto observe
                    if (User.bAutoObserve)
                        {
                        if (nN == nG)
                            {
                            if (bNotPartner && Fics.bPobserve && bB)
                                {
                                Fics.bPobserve = 0 ;
                                Fics.nPobserve = nN ;
                                strcpy (Fics.cPobserve, cwName) ;
                                strcat (Fics.cPobserve, FICS_POBSERVE_FALSE_STRING) ;

                                TOOLBOX_WriteICS (FICS_POBSERVE_COMMAND) ;
                                TOOLBOX_WriteICS (cwName) ;
                                TOOLBOX_WriteICS ("\n") ;
                                }
                            else
                                {
                                Fics.bPobserve = 1 ;
                                Fics.nPobserve = 0 ;
                                strcpy (Fics.cPobserve, "") ;
                                }
                            }
                        else
                            {
                            if (bNotPartner && Fics.bPobserve)
                                {
                                Fics.bPobserve = 0 ;
                                Fics.nPobserve = nN ;
                                strcpy (Fics.cPobserve, cwName) ;
                                strcat (Fics.cPobserve, FICS_POBSERVE_FALSE_STRING) ;

                                TOOLBOX_WriteICS (FICS_POBSERVE_COMMAND) ;
                                TOOLBOX_WriteICS (cwName) ;
                                TOOLBOX_WriteICS ("\n") ;
                                }
                            else
                                {
                                Fics.bPobserve = 1 ;
                                Fics.nPobserve = 0 ;
                                strcpy (Fics.cPobserve, "") ;

                                if (Game [INDEX_PLAY].nGamePartner != nN)
                                    {
                                    nA = 0 ;

                                    if (strlen (Vars.cPfollow) > 0)
                                        {
                                        if (stricmp (cwName, Vars.cPfollow) == 0)
                                            {
                                            if (Game [nI].bFlip)
                                                {
                                                nA = 1 ;
                                                }
                                            }
                                        }

                                    if (strlen (Vars.cFollow) > 0)
                                        {
                                        if (stricmp (cwName, Vars.cFollow) == 0)
                                            {
                                            if (Game [nI].bFlip)
                                                {
                                                nA = 1 ;
                                                }
                                            }
                                        }

                                    if (! nA)
                                        {
                                        for (nJ = 1 ; nJ < MAX_GAME ; nJ++)
                                            {
                                            if (Game [nJ].bValid)
                                                {
                                                if (Game [nJ].nGamePartner == nN)
                                                    {
                                                    if (strlen (Vars.cPfollow) > 0)
                                                        {
                                                        if (stricmp (Game [nJ].cHandle [INDEX_BLACK], Vars.cPfollow) == 0)
                                                            {
                                                            if (! Game [nJ].bFlip)
                                                                {
                                                                nA = 1 ;
                                                                }
                                                            }
                                                        }

                                                    if (strlen (Vars.cFollow) > 0)
                                                        {
                                                        if (stricmp (Game [nJ].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
                                                            {
                                                            if (! Game [nJ].bFlip)
                                                                {
                                                                nA = 1 ;
                                                                }
                                                            }
                                                        }

                                                    if (! nA)
                                                        {
                                                        if (strlen (Vars.cObserve) > 0)
                                                            {
                                                            if (TOOLBOX_MatchHandle (Game [nJ].cHandle [INDEX_WHITE], Vars.cObserve))
                                                                {
                                                                strcpy (Vars.cObserve, "") ;

                                                                if (Game [nJ].bFlip)
                                                                    {
                                                                    nA = 1 ;
                                                                    }
                                                                }
                                                            else
                                                            if (TOOLBOX_MatchHandle (Game [nJ].cHandle [INDEX_BLACK], Vars.cObserve))
                                                                {
                                                                strcpy (Vars.cObserve, "") ;

                                                                if (! Game [nJ].bFlip)
                                                                    {
                                                                    nA = 1 ;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    break ;
                                                    }
                                                }
                                            }
                                        }

                                    if (nA)
                                        {
                                        PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                        }
                                    else
                                        {
                                        if (strlen (Vars.cObserve) > 0)
                                            {
                                            if (TOOLBOX_MatchHandle (cwName, Vars.cObserve))
                                                {
                                                strcpy (Vars.cObserve, "") ;

                                                if (Game [nI].bFlip)
                                                    {
                                                    PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                                    }
                                                }
                                            else
                                            if (TOOLBOX_MatchHandle (cbName, Vars.cObserve))
                                                {
                                                strcpy (Vars.cObserve, "") ;

                                                if (! Game [nI].bFlip)
                                                    {
                                                    PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                STATE_ObserveNewGame (nI, bPartner) ;
                return 1 ;
                }
            }
        }

    if (nRl == -4)  // move list non-standard chess starting position
        {
        if (System.nIssuedMovesCommand > 0)
            {
            if (System.nMoveListIndexNumber >= 0)
                {
                nI = System.nMoveListIndexNumber ;

                Game [nI].bErrorMoveList               = 0 ;
                Game [nI].bRelation4                   = 1 ;
                Game [nI].bClickedButton               = 0 ;
                Game [nI].nInitDoublePushFile          = nLP ;
                Game [nI].bInitKingSide  [INDEX_WHITE] = bWCK ;
                Game [nI].bInitQueenSide [INDEX_WHITE] = bWCQ ;
                Game [nI].bInitKingSide  [INDEX_BLACK] = bBCK ;
                Game [nI].bInitQueenSide [INDEX_BLACK] = bBCQ ;
                Game [nI].nInitHalfMoves               = nHalf ;
                Game [nI].nInitKingX     [INDEX_WHITE] = -1 ;
                Game [nI].nInitKingY     [INDEX_WHITE] = -1 ;
                Game [nI].nInitKingX     [INDEX_BLACK] = -1 ;
                Game [nI].nInitKingY     [INDEX_BLACK] = -1 ;
                Game [nI].nCurrentIndex                = -1 ;
                Game [nI].nCurrentColor                = -1 ;
                Game [nI].nMinIndex                    = MAX_POSITION ;
                Game [nI].nMinColor                    = -1 ;
                Game [nI].nMaxIndex                    = -1 ;
                Game [nI].nMaxColor                    = -1 ;

                cP = cS + 5 ;
                for (nY = 7 ; nY >= 0 ; nY--)
                    {
                    for (nX = 0 ; nX < 8 ; nX++)
                        {
                        Game [nI].nInitBoard [nX] [nY] = BOARD_FICS_To_Int (*cP++) ;

                        if (Game [nI].nInitBoard [nX] [nY] == WHITE_KING)
                            {
                            Game [nI].nInitKingX [INDEX_WHITE] = nX ;
                            Game [nI].nInitKingY [INDEX_WHITE] = nY ;
                            }
                        else
                        if (Game [nI].nInitBoard [nX] [nY] == BLACK_KING)
                            {
                            Game [nI].nInitKingX [INDEX_BLACK] = nX ;
                            Game [nI].nInitKingY [INDEX_BLACK] = nY ;
                            }
                        }
                    *cP++ ;
                    }

                for (nX = 0 ; nX < MAX_BUFFER ; nX++)
                    {
                    Game [nI].nInitBuffer [nX] = 0 ;
                    }
                }
            }
        return 1 ;
        }

    return 0 ;
    }

int FICS_ParsePiece (char *cS)
    {
    HDC hdc ;
    char *cP1, *cP2 ;
    char cBugs1 [40], cBugs2 [40], cArrow [40] ;
    int nN, nI ;

    strcpy (cArrow, "") ;
    sscanf (cS, "<b1> game %d white %s black %s %s", &nN, &cBugs1, &cBugs2, &cArrow) ;

    for (nI = 0 ; nI < MAX_GAME ; nI++)
        {
        if (Game [nI].bValid)
            {
            if (Game [nI].nGameNumber == nN)
                {
                cP1 = strchr (cBugs1, ']') ;
                cP2 = strchr (cBugs2, ']') ;

                if ((cP1 != NULL) && (cP2 != NULL))
                    {
                    *cP1 = NULL_CHAR ;
                    *cP2 = NULL_CHAR ;

                    BOARD_LoadBuffer (nI, cBugs1 + 1, cBugs2 + 1) ;

                    if (Game [nI].bValid)
                        {
                        if ((nI == INDEX_PLAY) || (TOOLBOX_DisplayActualBoard (nI)))
                            {
                            hdc = GetDC (Game [nI].hwnd) ;
                            if (nI == INDEX_PLAY)
                                {
                                BOARD_DrawBoard (nI, Game [nI].hwnd, hdc, DRAW_STATE_BUFFER) ;
                                }
                            else
                                {
                                BOARD_DrawBoard1 (nI, Game [nI].hwnd, hdc, DRAW_STATE_BUFFER) ;
                                }
                            ReleaseDC (Game [nI].hwnd, hdc) ;
                            }

                        //
                        // update move list position buffer
                        //
                        // <b1> game 71 white [] black []           (PREVIOUS)
                        //
                        // <b1> game 71 white [N] black [] <- WN    (NEXT)
                        //
                        if (strcmp (cArrow, "<-") == 0)
                            {
                            POSITION_FICS_SaveGameBuffer (nI, 1) ;
                            }
                        else
                            {
                            POSITION_FICS_SaveGameBuffer (nI, 0) ;
                            }
                        }
                    }

                if (nI == INDEX_PLAY)
                    {
                    STATE_PlayPieceBufferUpdate () ;
                    }
                return 1 ;
                }
            }
        }
    return 0 ;
    }

int FICS_ColorIndex (char *cS)
    {
    HDC hdc ;
    int nI, nJ, nK ;
    char cTmp [50], cTmp1 [50], cTmp2 [50] ;

    if (Telnet.bSkipDisplayLine)
        {
        //--** indiantachyon is an abuser **--
        //--** indiantachyon is a computer **--
        if (! strncmp (cS, "--** ", 5))
            {
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            Telnet.bSkipDisplayLine     = 1 ;
            return 0 ;
            }
        else
            {
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            Telnet.bSkipDisplayLine     = 0 ;
            return 0 ;
            }
        }

    Telnet.bDisplayLine = 1 ;

    if (Telnet.bLastIsChallenge)
        {
        if (User.bNoPlayAbuser)
            {
            //--** indiantachyon is an abuser **--
            if (! strncmp (cS, "--** ", 5))
                {
                if (strstr (cS, " is an abuser"))
                    {
                    Telnet.clrCurrent       = clrColor [CLR_TELNET_NORMAL] ;
                    Telnet.nCurrentSound    = SOUND_NONE ;
                    Telnet.bLastIsChallenge = 0 ;
                    TOOLBOX_WriteICS (FICS_DECLINE_COMMAND) ;
                    return 0 ;
                    }

                if (User.bNoPlayComputer)
                    {
                    //--** indiantachyon is a computer **--
                    if (strstr (cS, " is a computer"))
                        {
                        Telnet.clrCurrent       = clrColor [CLR_TELNET_NORMAL] ;
                        Telnet.nCurrentSound    = SOUND_NONE ;
                        Telnet.bLastIsChallenge = 0 ;
                        TOOLBOX_WriteICS (FICS_DECLINE_COMMAND) ;
                        return 0 ;
                        }
                    }
                }
            }
        else
        if (User.bNoPlayComputer)
            {
            //--** indiantachyon is a computer **--
            if (! strncmp (cS, "--** ", 5))
                {
                if (strstr (cS, " is a computer"))
                    {
                    Telnet.clrCurrent       = clrColor [CLR_TELNET_NORMAL] ;
                    Telnet.nCurrentSound    = SOUND_NONE ;
                    Telnet.bLastIsChallenge = 0 ;
                    TOOLBOX_WriteICS (FICS_DECLINE_COMMAND) ;
                    return 0 ;
                    }
                }
            }

        Telnet.bLastIsChallenge = 0 ;
        }

    if (cS [0] == '\\')
        {
        Telnet.clrCurrent    = Telnet.clrLastLine ;
        Telnet.nCurrentSound = SOUND_NONE ;
        return 0 ;
        }

    if (cS [0] == ' ')
        {
        //
        //    **ANNOUNCEMENT** from mamer: The 5 0 qualifier will start in 5 minutes.
        //\   Read "news 326" for info, and "mam join 21" to join!
        //
        if (! strncmp (cS, FICS_ANNOUNCEMENT, 21))
            {
            if ((User.bSuppressAnnouncement) || (Silence.bSilenceApply && Silence.bSilenceAnnouncement) ||
				(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayAnnouncement))
                {
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            Telnet.clrCurrent = clrColor [CLR_TELNET_ANNOUNCEMENT] ;
            }
        else
            {
            Telnet.clrCurrent = clrColor [CLR_TELNET_NORMAL] ;
            }
        Telnet.nCurrentSound = SOUND_NONE ;
        return 0 ;
        }

    if (cS [0] == ':')
        {
        Telnet.clrCurrent    = FICS_ColonColor (cS) ;
        Telnet.nCurrentSound = SOUND_NONE ;
        return 0 ;
        }

    if (cS [0] == '[')
        {
        if (strstr (cS, FICS_ADDED_GAME_NOTIFY))
            {
            strcpy (cTmp, TOOLBOX_GetICSHandle (cS + 1, FALSE)) ;
            GNOTIFY_Add (cTmp) ;
            }
        else
        if (strstr (cS, FICS_REMOVED_GAME_NOTIFY))
            {
            strcpy (cTmp, TOOLBOX_GetICSHandle (cS + 1, FALSE)) ;
            GNOTIFY_Delete (cTmp) ;
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_HEARED_SAY))
        {

		strcpy (cTmp, TOOLBOX_GetICSHandle (cS, FALSE)) ;

        if ((Silence.bSilenceApply && Silence.bSilenceSay) ||
			(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlaySay))
			{
			if (SILENCE_Tell (cTmp, cS))
				{
				LOG_Write(cS);
		        Telnet.bDisplayLine         = 0 ;
			    Telnet.bDisplayContinueLine = 0 ;
				return 0 ;
				}
			}

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Say (cTmp, cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_SAY] ;
        Telnet.nCurrentSound = SOUND_SAY ;

        strcpy (Vars.cLastSay, cTmp) ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_HEARED_SHOUT))
        {

		if ((Silence.bSilenceApply && Silence.bSilenceShout) ||
			(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayShout))
			{
            LOG_Write(cS);
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            return 0 ;
			}

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Other (5, TOOLBOX_GetICSHandle (cS, FALSE)))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        if (User.bSuppressCompShout)
            {
            if (TOOLBOX_IsComputerHandle (cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_SHOUT] ;
        Telnet.nCurrentSound = SOUND_SHOUT ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_HEARED_KIBITZ))
        {
        if (User.bSuppressCompKibitz) 
            {
            if (TOOLBOX_IsComputerHandle (cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

		if ((Silence.bSilenceApply && Silence.bSilenceKibitz) ||
			(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayKibitz))
			{
			LOG_Write(cS);
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            return 0 ;
			}

        strcpy (cTmp, TOOLBOX_GetICSHandle (cS, FALSE)) ;

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Other (3, cTmp))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }
		
        Telnet.clrCurrent    = clrColor [CLR_TELNET_KIBITZ] ;
        Telnet.nCurrentSound = SOUND_KIBITZ ;

        strcpy (Vars.cLastKib, cTmp) ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_HEARED_WHISPER))
        {
        if (User.bSuppressCompWhisper)
            {
            if (TOOLBOX_IsComputerHandle (cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }
             
		if ((Silence.bSilenceApply && Silence.bSilenceWhisper) ||
			(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayWhisper))
			{
			LOG_Write(cS);
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            return 0 ;
			}

        strcpy (cTmp, TOOLBOX_GetICSHandle (cS, FALSE)) ;

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Other (4, cTmp))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_WHISPER] ;
        Telnet.nCurrentSound = SOUND_WHISPER ;

        strcpy (Vars.cLastWhisper, cTmp) ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_HEARED_CSHOUT))
        {

		if ((Silence.bSilenceApply && Silence.bSilenceCShout) ||
			(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayCShout))
			{
			LOG_Write(cS);
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            return 0 ;
			}

		if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Other (5, TOOLBOX_GetICSHandle (cS, FALSE)))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        if (User.bSuppressCompShout)
            {
            if (TOOLBOX_IsComputerHandle (cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_CSHOUT] ;
        Telnet.nCurrentSound = SOUND_CSHOUT ;
        return 0 ;
        }

    if (TELNET_IsChannel (cS, &nI))
        {

		if ((Silence.bSilenceApply && Silence.bSilenceChTell) ||
			(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayChTell))
			{
			LOG_Write(cS);
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            return 0 ;
			}

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Other (6, TOOLBOX_GetICSHandle (cS, FALSE)))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        if (nI)
            {
            if (User.bSuppressCompChTell)
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_HEARED_TELL))
        {

        strcpy (cTmp, TOOLBOX_GetICSHandle (cS, FALSE)) ;

		if ((Silence.bSilenceApply && Silence.bSilencePeTell) ||
			(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayPeTell))
			{
			if (SILENCE_Tell (cTmp, cS))
				{
				LOG_Write(cS);
				Telnet.bDisplayLine         = 0 ;
				Telnet.bDisplayContinueLine = 0 ;
				return 0 ;
				}
			}

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Tell (cTmp, cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        if (User.bIamAGuest)
            {
            if (User.bSuppressROBOWelcome)
                {
                if (! strncmp (cS, FICS_HEARED_ROBO_WELCOME, 39))
                    {
					LOG_Write(cS);
                    Telnet.bDisplayLine         = 0 ;
                    Telnet.bDisplayContinueLine = 0 ;
                    return 0 ;
                    }
                }
            }

        if (User.bSuppressCompTell)
            {
            if (TOOLBOX_IsComputerHandle (cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_TELL] ;
        Telnet.nCurrentSound = SOUND_TELL ;

        strcpy (Vars.cLastTell, cTmp) ;
        F9KEY_Add (Vars.cLastTell) ;

        if (User.bEarPartner || User.bShowPtell)
            {
            if (strlen (Vars.cPartnerTell1) > 0)
                {
                if (TELNET_MatchExp (cS, Vars.cPartnerTell1) ||
                    TELNET_MatchExp (cS, Vars.cPartnerTell2))
                    {
                    if (User.bShowPtell)
                        {
                        hdc = GetDC (Game [INDEX_PLAY].hwnd) ;
                        BOARD_DrawPtell (INDEX_PLAY, hdc, cS) ;
                        ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
                        }
                    return 1 ;
                    }
                else
                    {
                    return 0 ;
                    }
                }
            else
                {
                return 0 ;
                }
            }
        else
            {
            return 1 ;
            }
        }

    if (TELNET_MatchExp (cS, FICS_HEARED_PTELL))
        {
        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Ptell (Vars.cPartner, cS))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }

            if (strlen (Vars.cPartner) > 0)
                {
                if (CENSOR_Partner (Vars.cPartner))
                    {
                    TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                    }
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_PTELL] ;
        Telnet.nCurrentSound = SOUND_PTELL ;

        if (strlen (Vars.cPartner) > 0)
            {
            F9KEY_Add (Vars.cPartner) ;
            strcpy (Vars.cLastTell, Vars.cPartner) ;
            }
		
        if (User.bShowPtell)
            {
            hdc = GetDC (Game [INDEX_PLAY].hwnd) ;
            BOARD_DrawPtell (INDEX_PLAY, hdc, cS) ;
            ReleaseDC (Game [INDEX_PLAY].hwnd, hdc) ;
            }
        return 1 ;
        }

    Telnet.bLastIsChallenge = 0 ;

    // new in 1.24: FICS censor for messages 
    
    if (TELNET_MatchExp (cS, FICS_RECEIVED_MESSAGE))
	//The following message was received:
		{
		Telnet.bReceivedMessage     = 1 ;
		Telnet.bDisplayLine         = 0 ;
		Telnet.bDisplayContinueLine = 0 ;			
		}
	
	if (TELNET_MatchExp (cS, FICS_RECEIVED_MESSAGEBODY))
		//Thieftest at Fri Feb 11, 15:16 PST 2011: bla
		{
		if (Telnet.bReceivedMessage)
			{
			Telnet.bReceivedMessage = 0;
			if (Censor [Login.nLoginType].bCensor)
				{
				strcpy (cTmp, TOOLBOX_GetICSHandleFromMessage (cS)) ;
			    if (CENSOR_Message (cTmp))
				        {
						LOG_Write(cS);
						Telnet.bDisplayLine         = 0 ;
						Telnet.bDisplayContinueLine = 0 ;
						return 0 ;
			            }
				}
			TELNET_NormalPrint (FICS_RECEIVED_MESSAGE) ;
			TELNET_NormalPrint ("\n") ;
			} 
		}

	if (TELNET_MatchExp (cS, FICS_MESSAGES_CLEARED))
		//Messages from Thieftest cleared.
		{
		if (Censor [Login.nLoginType].bCensor)
			{
			if (Telnet.nAutoClearedMessage > 0){
				strcpy (cTmp, TOOLBOX_GetICSHandleFromMessageDeleted (cS)) ;
			    if (CENSOR_Other (10,cTmp))
			        {
					LOG_Write(cS);
					Telnet.bDisplayLine         = 0 ;
					Telnet.bDisplayContinueLine = 0 ;
					Telnet.nAutoClearedMessage--;
					return 0 ;
				    }
				}
			}
		}

	if (TELNET_MatchExp (cS, FICS_MESSAGES_NONE))
		//You have no messages from @.
		{
		if (Censor [Login.nLoginType].bCensor)
			{
			if (Telnet.nAutoClearedMessage > 0){
				strcpy (cTmp, TOOLBOX_GetICSHandleFromNoMessage (cS)) ;
			    if (CENSOR_Other (10,cTmp))
			        {
					LOG_Write(cS);
					Telnet.bDisplayLine         = 0 ;
					Telnet.bDisplayContinueLine = 0 ;
					Telnet.nAutoClearedMessage--;
					return 0 ;
				    }
				} 
			}
		}

	if (TELNET_MatchExp (cS, FICS_MESSAGES_LOGIN))
		//You have x messages (y unread).
		//You have x message (y unread).
		{
		if (Censor [Login.nLoginType].bCensor)
			{
            CENSOR_ClearMessages(cS) ;
			}
		}
 
    if (TELNET_MatchExp (cS, FICS_GOT_CHALLENGED))
        {
        //Challenge: heheman (----) cocoman (----) unrated crazyhouse 10 0.
        if (Censor [Login.nLoginType].bCensor)
            {
            char cWhite [50], cBlack [50] ;

            sscanf (cS, "%s %s %s %s", &cTmp, &cWhite, &cTmp, &cBlack) ;

            if (stricmp (cWhite, Vars.cWhoAmI) == 0)
                {
                if (CENSOR_Other (7, cBlack))
                    {
					TOOLBOX_WriteICS (FICS_DECLINE_COMMAND) ;
					LOG_Write(cS);
                    Telnet.bDisplayLine         = 0 ;
                    Telnet.bDisplayContinueLine = 0 ;
                    Telnet.bSkipDisplayLine     = 1 ;
                    return 0 ;
                    }
                }
            else
                {
                if (CENSOR_Other (7, cWhite))
                    {
					TOOLBOX_WriteICS (FICS_DECLINE_COMMAND) ;
					LOG_Write(cS);
                    Telnet.bDisplayLine         = 0 ;
                    Telnet.bDisplayContinueLine = 0 ;
                    Telnet.bSkipDisplayLine     = 1 ;
                    return 0 ;
                    }
                }
            }

        Telnet.bSkipDisplayLine = 0 ;
        Telnet.clrCurrent       = clrColor [CLR_TELNET_CHALLENGE] ;
        Telnet.nCurrentSound    = SOUND_CHALLENGE ;
        Telnet.bLastIsChallenge = 1 ;
        return 0 ;
        }
        
    if (TELNET_MatchExp (cS, FICS_HEARED_IT))
        {
		if ((Silence.bSilenceApply && Silence.bSilenceShout) ||
			(Silence.bSilenceApply && Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying && Silence.bSilencePlayShout))
			{
			LOG_Write(cS);
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            return 0 ;
			}

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Other (5, TOOLBOX_GetICSHandle (cS + 4, FALSE)))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_SHOUT] ;
        Telnet.nCurrentSound = SOUND_SHOUT ;
        return 0 ;
        }


    if (TELNET_MatchExp (cS, FICS_HEARED_NOTIFICATION))
        {
        if (TELNET_MatchExp (cS, FICS_HEARED_ARRIVAL))
            {
            if (Censor [Login.nLoginType].bCensor)
                {
                if (CENSOR_Other (8, TOOLBOX_GetICSHandle (cS + 14, FALSE)))
                    {
					LOG_Write(cS);
                    Telnet.bDisplayLine         = 0 ;
                    Telnet.bDisplayContinueLine = 0 ;
                    return 0 ;
                    }
                }

            Telnet.clrCurrent    = clrColor [CLR_TELNET_ARRIVAL] ;
            Telnet.nCurrentSound = SOUND_ARRIVAL ;
            return 0 ;
            }

        if (TELNET_MatchExp (cS, FICS_HEARED_ARRIVAL1))
            {
            if (Censor [Login.nLoginType].bCensor)
                {
                if (CENSOR_Other (8, TOOLBOX_GetICSHandle (cS + 14, FALSE)))
                    {
					LOG_Write(cS);
                    Telnet.bDisplayLine         = 0 ;
                    Telnet.bDisplayContinueLine = 0 ;
                    return 0 ;
                    }
                }

            Telnet.clrCurrent    = clrColor [CLR_TELNET_ARRIVAL] ;
            Telnet.nCurrentSound = SOUND_ARRIVAL ;
            return 0 ;
            }

        if (TELNET_MatchExp (cS, FICS_HEARED_DEPARTURE))
            {
            if (Censor [Login.nLoginType].bCensor)
                {
                if (CENSOR_Other (8, TOOLBOX_GetICSHandle (cS + 14, FALSE)))
                    {
					LOG_Write(cS);
                    Telnet.bDisplayLine         = 0 ;
                    Telnet.bDisplayContinueLine = 0 ;
                    return 0 ;
                    }
                }

            Telnet.clrCurrent    = clrColor [CLR_TELNET_DEPARTURE] ;
            Telnet.nCurrentSound = SOUND_DEPARTURE ;
            return 0 ;
            }

        if (TELNET_MatchExp (cS, FICS_HEARED_DEPARTURE1))
            {
            if (Censor [Login.nLoginType].bCensor)
                {
                if (CENSOR_Other (8, TOOLBOX_GetICSHandle (cS + 14, FALSE)))
                    {
					LOG_Write(cS);
                    Telnet.bDisplayLine         = 0 ;
                    Telnet.bDisplayContinueLine = 0 ;
                    return 0 ;
                    }
                }

            Telnet.clrCurrent    = clrColor [CLR_TELNET_DEPARTURE] ;
            Telnet.nCurrentSound = SOUND_DEPARTURE ;
            return 0 ;
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_NOTIFICATION] ;
        Telnet.nCurrentSound = SOUND_NOTIFICATION ;
        return 0 ;
        }

    if (! strncmp (cS, FICS_HEARED_GAME_NOTIFY, 19))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_GAME_NOTIFY] ;
        Telnet.nCurrentSound = SOUND_GAME_NOTIFY ;

        if (User.bAutoObserveGNotify)
            {

            //Game notification: VABORIS (2422) vs. venomous (2401) rated crazyhouse 1 0: Game 4
            sscanf (cS, "%s %s %s %s %s %s", &cTmp, &cTmp, &cTmp1, &cTmp, &cTmp, &cTmp2) ;

            if ((stricmp (cTmp1, Vars.cPartner) == 0) ||
                (stricmp (cTmp1, Vars.cFollow ) == 0) ||
                (stricmp (cTmp1, Vars.cPfollow) == 0) ||
                (stricmp (cTmp2, Vars.cPartner) == 0) ||
                (stricmp (cTmp2, Vars.cFollow ) == 0) ||
                (stricmp (cTmp2, Vars.cPfollow) == 0))
                {

                // do nothing
                }
            else
                {

                // observe but check to make sure neither of the handle is already being observed
                nJ = 0 ;
                for (nI = 0 ; nI < MAX_GAME ; nI++)
                    {
                    if (Game [nI].bValid)
                        {
                        if (Game [nI].nGameNumber > 0)
                            {
                            if (Game [nI].bPlaying)
                                {
                                if ((stricmp (Game [nI].cHandle [INDEX_WHITE], cTmp1) == 0) ||
                                    (stricmp (Game [nI].cHandle [INDEX_BLACK], cTmp1) == 0) ||
                                    (stricmp (Game [nI].cHandle [INDEX_WHITE], cTmp2) == 0) ||
                                    (stricmp (Game [nI].cHandle [INDEX_BLACK], cTmp2) == 0))
                                    {
                                    nJ = 1 ;
                                    break ;
                                    }
                                }
                            }
                        }
                    }

                if (nJ == 0)
                    {
                    if (Game [INDEX_PLAY].nGameNumber > 0)
                        {
                        if (Game [INDEX_PLAY].bPlaying)
                            {

                            // playing
                            TOOLBOX_WriteSystem ("No Auto Game Notification Observe Due to Playing\n") ;
                            }
                        else
                            {

                            // examing
                            TOOLBOX_WriteSystem ("No Auto Game Notification Observe Due to Examing\n") ;
                            }
                        }
                    else
                        {
                        if (GNOTIFY_Find (cTmp1))
                            {
                            strcpy (Vars.cObserve, cTmp1) ;
                            }
                        else
                        if (GNOTIFY_Find (cTmp2))
                            {
                            strcpy (Vars.cObserve, cTmp2) ;
                            }

                        // issue the observe command
                        TOOLBOX_WriteICS (FICS_OBSERVE_COMMAND) ;
                        TOOLBOX_WriteICS (cTmp1) ;
                        TOOLBOX_WriteICS ("\n") ;
                        }
                    }
                }
            }
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_ABORT_REQUEST))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_ABORT_REQUEST ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_DRAW_REQUEST))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_DRAW_REQUEST ;
        return 0 ;
        }

	if (TELNET_MatchExp (cS, FICS_ADJOURN_REQUEST))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_ADJOURN_REQUEST ;
        return 0 ;
        }

	if (TELNET_MatchExp (cS, FICS_TAKEBACK_REQUEST))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_TAKEBACK_REQUEST ;
        return 0 ;
        }

    if (User.bSuppressIgnoreFormula)
        {
        if (! strncmp (cS, FICS_HEARED_IGNORE_FORMULA, 20))
            {
            Telnet.bDisplayLine         = 0 ;
            Telnet.bDisplayContinueLine = 0 ;
            return 0 ;
            }
        }

    if (TELNET_MatchExp (cS, FICS_PARTNER_REQUEST))
        {
        strcpy (cTmp, TOOLBOX_GetICSHandle (cS, FALSE)) ;

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Partner (cTmp))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                TOOLBOX_WriteICS (FICS_DECLINE_COMMAND) ;
                return 0 ;
                }
            }
            
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_PARTNER ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_PARTNER_AGREE_1))
        {
        nJ = 0 ;
        for (nI = 16 ; nI < ((int) strlen (cS)) ; nI++)
            {
            if (cS [nI] == 39)
                {
                break ;
                }
            cTmp [nJ++] = cS [nI] ;
            }
        cTmp [nJ] = NULL_CHAR ;

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Partner (cTmp))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;

        strcpy (Vars.cPartner, cTmp) ;

        strcpy (Vars.cPartnerTell, "") ;
        Game [INDEX_PLAY].nGamePartner = 0 ;

        TOOLBOX_SetTelnetCaption () ;
        BUTTON_Refresh () ;

        if (strlen (Vars.cPfollow) == 0)
            {
            TOOLBOX_WriteICS (FICS_PFOLLOW_COMMAND) ;
            TOOLBOX_WriteICS (Vars.cWhoAmI) ;
            TOOLBOX_WriteICS ("\n") ;
            }
        else
        if (stricmp (Vars.cPfollow, Vars.cWhoAmI) != 0)
            {
            TOOLBOX_WriteICS (FICS_PFOLLOW_COMMAND) ;
            TOOLBOX_WriteICS (Vars.cWhoAmI) ;
            TOOLBOX_WriteICS ("\n") ;
            }
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_PARTNER_AGREE_2))
        {
        nJ = 0 ;
        for (nI = 0 ; nI < ((int) strlen (cS)) ; nI++)
            {
            if (cS [nI] == ' ')
                {
                break ;
                }
            cTmp [nJ++] = cS [nI] ;
            }
        cTmp [nJ] = NULL_CHAR ;

        if (Censor [Login.nLoginType].bCensor)
            {
            if (CENSOR_Partner (cTmp))
                {
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                TOOLBOX_WriteICS (FICS_NO_PARTNER_COMMAND) ;
                return 0 ;
                }
            }

        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;

        strcpy (Vars.cPartner, cTmp) ;

        strcpy (Vars.cPartnerTell, "") ;
        Game [INDEX_PLAY].nGamePartner = 0 ;

        TOOLBOX_SetTelnetCaption () ;
        BUTTON_Refresh () ;

        if (strlen (Vars.cPfollow) == 0)
            {
            TOOLBOX_WriteICS (FICS_PFOLLOW_COMMAND) ;
            TOOLBOX_WriteICS (Vars.cWhoAmI) ;
            TOOLBOX_WriteICS ("\n") ;
            }
        else
        if (stricmp (Vars.cPfollow, Vars.cWhoAmI) != 0)
            {
            TOOLBOX_WriteICS (FICS_PFOLLOW_COMMAND) ;
            TOOLBOX_WriteICS (Vars.cWhoAmI) ;
            TOOLBOX_WriteICS ("\n") ;
            }
        return 0 ;
        }

//
//  "You no longer have a bughouse partner." // ok new
//  "Your partner has disconnected." // ok
//  "Your partner has just chosen a new partner." // no more
//  "Your partner has unset his/her partner variable." // no more
//  "Your partner has ended partnership." // ok
//  "Your partner has accepted a partnership with" //ok
//
    if (TELNET_MatchExp (cS, "You no longer have a bughouse partner")        ||
        TELNET_MatchExp (cS, "Your partner has ended partnership")           ||
        TELNET_MatchExp (cS, "Your partner has accepted a partnership with") ||
        TELNET_MatchExp (cS, "Your partner has disconnected"))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;

        strcpy (Vars.cPartner, "") ;
        strcpy (Vars.cPartnerTell, "") ;
        Game [INDEX_PLAY].nGamePartner = 0 ;

        TOOLBOX_SetTelnetCaption () ;
        BUTTON_Refresh () ;
        return 0 ;
        }

    if (TELNET_MatchExp (cS, FICS_GET_MEXED))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;

		TOOLBOX_WriteICS (FICS_REFRESH_COMMAND1) ;
        return 0 ;
        }

//
// Your opponent has added 300 seconds to your clock.
// 200 seconds were added to your opponents clock
//
    if (TELNET_MatchExp (cS, FICS_GOT_MORETIME_1) ||
        TELNET_MatchExp (cS, FICS_GOT_MORETIME_2))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;

		TOOLBOX_WriteICS (FICS_REFRESH_COMMAND1) ;
        return 0 ;
        }

//
// Opponent has timeseal; checking if (s)he's lagging.
// Opponent is not out of time, wait for server autoflag.
// Flag still pending from previous request; opponent is lagging.
//
//  if (TELNET_MatchExp (cS, FICS_OPP_LAGGING_1) ||
//      TELNET_MatchExp (cS, FICS_OPP_LAGGING_2) ||
//      TELNET_MatchExp (cS, FICS_OPP_LAGGING_3) ||
//      TELNET_MatchExp (cS, FICS_OPP_LAGGING_4))
//      {
//      Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
//      Telnet.nCurrentSound = SOUND_NONE ;
//
//      Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 0 ;
//      Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 0 ;
//      return 0 ;
//      }

    if (! strncmp (cS, FICS_TIME_LEFT_0, 54))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;
        return 0 ;
        }

    if (strstr (cS, FICS_TIME_LEFT_1) ||
        strstr (cS, FICS_TIME_LEFT_2) ||
        strstr (cS, FICS_TIME_LEFT_3) ||
        strstr (cS, FICS_TIME_LEFT_4))
        {
        Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
        Telnet.nCurrentSound = SOUND_NONE ;

        Game [INDEX_PLAY].bFlagged [INDEX_WHITE] = 0 ;
        Game [INDEX_PLAY].bFlagged [INDEX_BLACK] = 0 ;
        return 0 ;
        }

    if (Censor [Login.nLoginType].bCensor)
        {
        if (Telnet.bTellTold)
            {
            if (! strncmp (cS, Telnet.cTellTold, Telnet.nTellTold))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                Telnet.bTellTold            = 0 ;
                return 0 ;
                }
            }

        if (Telnet.bPtellTold)
            {
            if (! strncmp (cS, Telnet.cPtellTold, Telnet.nPtellTold))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                Telnet.bPtellTold           = 0 ;
                return 0 ;
                }
            }

        if (Telnet.bSayTold)
            {
            if (! strncmp (cS, Telnet.cSayTold, Telnet.nSayTold))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                Telnet.bSayTold             = 0 ;
                return 0 ;
                }
            }

        if (Telnet.bPartnerTold)
            {
            if (! strncmp (cS, Telnet.cPartnerTold, Telnet.nPartnerTold))
                {
				LOG_Write(cS);
                Telnet.bDisplayLine         = 0 ;
                Telnet.bDisplayContinueLine = 0 ;
                Telnet.bPartnerTold         = 0 ;
                return 0 ;
                }
            }
        }

    if (User.bAutoObserve)
        {
        if (! Fics.bPobserve)
            {
            if (TELNET_MatchExp (cS, Fics.cPobserve))
                {
                Telnet.bDisplayLine = 0 ;

                nJ = Fics.nPobserve ;

                Fics.bPobserve = 1 ;
                Fics.nPobserve = 0 ;
                strcpy (Fics.cPobserve, "") ;

                if (nJ != 0)
                    {
                    for (nI = 1 ; nI < MAX_GAME ; nI++)
                        {
                        if (Game [nI].bValid)
                            {
                            if (Game [nI].nGameNumber == nJ)
                                {
                                nK = 0 ;

                                if (strlen (Vars.cFollow) > 0)
                                    {
                                    if (stricmp (Game [nI].cHandle [INDEX_BLACK], Vars.cFollow) == 0)
                                        {
                                        if (! Game [nI].bFlip)
                                            {
                                            PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                            nK = 1 ;
                                            }
                                        }
                                    }

                                if (! nK)
                                    {
                                    if (strlen (Vars.cObserve) > 0)
                                        {
                                        if (TOOLBOX_MatchHandle (Game [nI].cHandle [INDEX_WHITE], Vars.cObserve))
                                            {
                                            strcpy (Vars.cObserve, "") ;

                                            if (Game [nI].bFlip)
                                                {
                                                PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                                }
                                            }
                                        else
                                        if (TOOLBOX_MatchHandle (Game [nI].cHandle [INDEX_BLACK], Vars.cObserve))
                                            {
                                            strcpy (Vars.cObserve, "") ;

                                            if (! Game [nI].bFlip)
                                                {
                                                PostMessage (hwndWindow [HWND_FRAME], WM_COMMAND, IDM_FLIP_BOARD, (LPARAM) 0) ;
                                                }
                                            }
                                        }
                                    }
                                break ;
                                }
                            }
                        }
                    }
                }
            }
        }

    Telnet.clrCurrent    = clrColor [CLR_TELNET_NORMAL] ;
    Telnet.nCurrentSound = SOUND_NONE ;
    return 0 ;
    }

COLORREF FICS_ColonColor (char *cS)
    {
    COLORREF clr ;
    int nL, nI ;
    char cTmp [10] ;

    clr = clrColor [CLR_TELNET_COLON] ;

    if (! strncmp (cS, ":Notification: ", 15))
        {
        clr = clrColor [CLR_TELNET_COLON_NOTIFY] ;
        }
    else
    if ((cS [0] == ':') && (cS [1] == NULL_CHAR))
        {
        clr = clrColor [CLR_TELNET_COLON_MAMER] ;
        }
    else
        {
        nL = 0 ;
        for (nI = 0 ; nI < MAX_MAMER_STRING ; nI++)
            {
            if (! strncmp (cS, MamerString [nI], MamerStringSize [nI]))
                {
                nL  = 1 ;
                clr = clrColor [CLR_TELNET_COLON_MAMER] ;
                break ;
                }
            }

        if (! nL)
            {
            if (strstr (cS, " is observing tourney #")                 ||
                strstr (cS, " has been added to tourney #")            ||
                strstr (cS, " are observing tourney #")                ||
                strstr (cS, " has been invited to tourney #")          ||
                strstr (cS, " the king of KOTH #")                     ||
                strstr (cS, " the queen of KOTH #")                    ||
                strstr (cS, " managed the tourney.")                   ||
                strstr (cS, " is managing the tourney.")               ||
                strstr (cS, " variable has been set to")               ||
                strstr (cS, " has been made the manager of tourney #") ||
                strstr (cS, " has been forfeited from tourney #"))
                {
                clr = clrColor [CLR_TELNET_COLON_MAMER] ;
                }
            else
            if ((cS [0] == ':') &&
                (cS [1] >= '0') &&
                (cS [1] <= '9') &&
                (cS [2] == '.'))
                {
                clr = clrColor [CLR_TELNET_COLON_MAMER] ;
                }
            else
            if ((cS [0] == ':') &&
                (cS [1] >= '0') &&
                (cS [1] <= '9') &&
                (cS [2] >= '0') &&
                (cS [2] <= '9') &&
                (cS [3] == '.'))
                {
                clr = clrColor [CLR_TELNET_COLON_MAMER] ;
                }
            else
            if (TOOLBOX_IsQChannelTell (cS, cTmp))
                {
                clr = clrColor [CLR_TELNET_COLON_256] ;

                nL = strlen (cTmp) ;
                if (nL == 2)
                    {
                    //:knighttour(*)(C1): xxx
                    if (cTmp [0] == 'C' && isdigit (cTmp [1]))
                        {
                        clr = clrColor [CLR_TELNET_COLON_C2] ;
                        }
                    else
                    //:knighttour(*)(T1): xxx
                    if (cTmp [0] == 'T' && isdigit (cTmp [1]))
                        {
                        clr = clrColor [CLR_TELNET_COLON_T3] ;
                        }
                    }
                else
                if (nL == 3)
                    {
                    //:knighttour(*)(C12): xxx
                    if (cTmp [0] == 'C' && isdigit (cTmp [1]) && isdigit (cTmp [2]))
                        {
                        clr = clrColor [CLR_TELNET_COLON_C2] ;
                        }
                    else
                    //:knighttour(*)(T12): xxx
                    if (cTmp [0] == 'T' && isdigit (cTmp [1]) && isdigit (cTmp [2]))
                        {
                        clr = clrColor [CLR_TELNET_COLON_T3] ;
                        }
                    }
                else
                if (nL == 4)
                    {
                    //:knighttour(*)(C123): xxx
                    if (cTmp [0] == 'C' && isdigit (cTmp [1]) && isdigit (cTmp [2]) && isdigit (cTmp [3]))
                        {
                        clr = clrColor [CLR_TELNET_COLON_C2] ;
                        }
                    else
                    //:knighttour(*)(T123): xxx
                    if (cTmp [0] == 'T' && isdigit (cTmp [1]) && isdigit (cTmp [2]) && isdigit (cTmp [3]))
                        {
                        clr = clrColor [CLR_TELNET_COLON_T3] ;
                        }
                    }
                }
            else
                {
                nL = strlen (cS) ;
                for (nI = 0 ; nI < nL ; nI++)
                    {
                    //:knighttour(TM) t-shouts: xxx
                    if (cS [nI     ] == ' ' &&
                        cS [nI +  1] == 't' &&
                        cS [nI +  2] == '-' &&
                        cS [nI +  3] == 's' &&
                        cS [nI +  4] == 'h' &&
                        cS [nI +  5] == 'o' &&
                        cS [nI +  6] == 'u' &&
                        cS [nI +  7] == 't' &&
                        cS [nI +  8] == 's' &&
                        cS [nI +  9] == ':' &&
                        cS [nI + 10] == ' ')
                        {
                        clr = clrColor [CLR_TELNET_COLON_TSHOUTS] ;
                        break ;
                        }
                    }
                }
            }
        }
    return clr ;
    }
