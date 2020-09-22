#include "thief.h"

void STATE_PlayNewGame(void)
{
	F8KEY_Init() ;
	GAMEEVENT_Command(GAME_EVENT_PLAY_START1) ;
	GAMEEVENT_Command(GAME_EVENT_PLAY_START2) ;
}

void STATE_PlayEndGame(void)
{
	F8KEY_Init() ;

	TOOLBOX_ResetPromotKnight() ;

	if(System.bIssuedAbort)
	{
		System.bIssuedAbort = 0 ;

		GAMEEVENT_Command(GAME_EVENT_PLAY_OVER1) ;
		GAMEEVENT_Command(GAME_EVENT_PLAY_OVER2) ;
	}
	else
	{
		if(User.bLogGame)
		{
			if(User.bSavePlayGame)
			{
				if(! Game [INDEX_PLAY].bSavedGame)
				{
					SAVEGAME_SaveGame(INDEX_PLAY, 0, 0) ;
				}
			}
		}

		GAMEEVENT_Command(GAME_EVENT_PLAY_OVER1) ;
		GAMEEVENT_Command(GAME_EVENT_PLAY_OVER2) ;

		if(User.bAutoRematch)
		{
			PostMessage(hwndWindow [HWND_FRAME], WM_COMMAND, IDM_REMATCH, (LPARAM) 0) ;
		}
	}
}

void STATE_ObserveNewGame(int nI, int bPartner)
{
	GAMEEVENT_Command(GAME_EVENT_OBSERVE_START) ;
}

void STATE_ObserveEndGame(int nI, int bPartner, int bUnobserve)
{
	if(User.bLogGame)
	{
		if(! bPartner)
		{
			if(bUnobserve)
			{
				if(User.bSaveUnobserveGame)
				{
					if(! Game [nI].bSavedGame)
					{
						SAVEGAME_SaveGame(nI, 0, 0) ;
					}
				}
			}
			else
			{
				if(User.bSaveObserveGame)
				{
					if(! Game [nI].bSavedGame)
					{
						SAVEGAME_SaveGame(nI, 0, 0) ;
					}
				}
			}
		}
	}

	GAMEEVENT_Command(GAME_EVENT_OBSERVE_OVER) ;
}

void STATE_PlayPieceBufferUpdate(void)
{
	F8KEY_Issue() ;
}

void STATE_Login(int bHasTimeseal)
{
	F8KEY_Init() ;
}

void STATE_Logout(void)
{
	F8KEY_Init() ;
}

int STATE_EnterDialogBox(void)
{
	if(System.bInDialogBox)
	{
		TOOLBOX_Beep() ;
		return 0 ;
	}
	else
	{
		System.bInDialogBox = 1 ;
		return 1 ;
	}
}

void STATE_LeaveDialogBox(void)
{
	System.bInDialogBox = 0 ;
	SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
}
