#include "thief.h"

void F9KEY_Init(void)
{
	int nI ;

	F9Key.nLoopTellHandle = 0 ;

	for(nI = 0 ; nI < MAX_TELL_HANDLE ; nI++)
	{
		strcpy(F9Key.cLastTellHandle [nI], "") ;
	}
}

void F9KEY_Clear(void)
{
	F9Key.nLoopTellHandle = 0 ;
}

void F9KEY_Add(char *cHandle)
{
	int nI, nJ ;

	F9Key.nLoopTellHandle = 0 ;

	nI = 0 ;

	while((nI < MAX_TELL_HANDLE) && (strcmp(F9Key.cLastTellHandle [nI], cHandle) != 0))
	{
		nI = nI + 1 ;
	}

	if(nI == MAX_TELL_HANDLE)
	{
		nJ = MAX_TELL_HANDLE - 1 ;  // not found
	}
	else
	{
		nJ = nI ;                   // found
	}

	while(nJ > 0)
	{
		strcpy(F9Key.cLastTellHandle [nJ], F9Key.cLastTellHandle [nJ - 1]) ;
		nJ = nJ - 1 ;
	}

	strcpy(F9Key.cLastTellHandle [0], cHandle) ;
}

void F9KEY_Set(void)
{
	CHARRANGE TSel ;
	int       nI ;
	char      cTmp [255] ;

	nI = F9Key.nLoopTellHandle ;

	F9Key.nLoopTellHandle = F9Key.nLoopTellHandle + 1 ;

	if(F9Key.nLoopTellHandle > (MAX_TELL_HANDLE + 1))
	{
		F9Key.nLoopTellHandle = 0 ;
	}

	if(nI == 0)
	{
		nI = 1 ;
		F9Key.nLoopTellHandle = F9Key.nLoopTellHandle + 1 ;
	}

	if(nI < MAX_TELL_HANDLE)
	{
		if(strlen(F9Key.cLastTellHandle [nI - 1]) == 0)
		{
			if(Login.nLoginType == SERVER_ICC)
			{
				if(User.bUseXTellForLastTell)
				{
					strcpy(cTmp, "/Xtell ") ;
				}
				else
				{
					strcpy(cTmp, "/Tell ") ;
				}
			}
			else
			{
				if(User.bUseXTellForLastTell)
				{
					strcpy(cTmp, "Xtell ") ;
				}
				else
				{
					strcpy(cTmp, "Tell ") ;
				}
			}
			F9Key.nLoopTellHandle = 0 ;
		}
		else
		{
			if(Login.nLoginType == SERVER_ICC)
			{
				if(User.bUseXTellForLastTell)
				{
					sprintf(cTmp, "/Xtell %s! ", F9Key.cLastTellHandle [nI - 1]) ;
				}
				else
				{
					sprintf(cTmp, "/Tell %s! ", F9Key.cLastTellHandle [nI - 1]) ;
				}
			}
			else
			{
				if(User.bUseXTellForLastTell)
				{
					sprintf(cTmp, "Xtell %s ", F9Key.cLastTellHandle [nI - 1]) ;
				}
				else
				{
					sprintf(cTmp, "Tell %s ", F9Key.cLastTellHandle [nI - 1]) ;
				}
			}
		}
	}
	else
	{
		if(Login.nLoginType == SERVER_ICC)
		{
			if(User.bUseXTellForLastTell)
			{
				strcpy(cTmp, "/Tell ") ;
			}
			else
			{
				strcpy(cTmp, "/Xtell ") ;
			}
		}
		else
		{
			if(User.bUseXTellForLastTell)
			{
				strcpy(cTmp, "Xtell ") ;
			}
			else
			{
				strcpy(cTmp, "Tell ") ;
			}
		}
		F9Key.nLoopTellHandle = 0 ;
	}

	TSel.cpMin = 999999 ;
	TSel.cpMax = 999999 ;

	SetWindowText(hwndWindow [HWND_TELNET_EDIT], cTmp) ;
	SendMessage(hwndWindow [HWND_TELNET_EDIT], EM_EXSETSEL, 0, (LPARAM) &TSel) ;
	SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
}
