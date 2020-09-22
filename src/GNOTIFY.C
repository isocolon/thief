#include "thief.h"

void GNOTIFY_Init(void)
{
	int nI;

	nGNotifyHandle = 0;

	for(nI = 0 ; nI < MAX_GNOTIFY_HANDLE ; nI++)
	{
		strcpy(cGNotifyHandle [nI], "");
	}
}

void GNOTIFY_Add(char *cH)
{
	int bF, nI;

	if(strlen(cH) == 0)
	{
		return;
	}

	if(nGNotifyHandle > MAX_GNOTIFY_HANDLE)
	{
		return;
	}

	bF = 0;
	for(nI = 0 ; nI <nGNotifyHandle ; nI++)
	{
		if(strlen(cGNotifyHandle [nI]) == 0)
		{
			bF = 1;
			strcpy(cGNotifyHandle [nGNotifyHandle++], cH);
			break;
		}
	}

	if(! bF)
	{
		if(nGNotifyHandle < MAX_GNOTIFY_HANDLE)
		{
			strcpy(cGNotifyHandle [nGNotifyHandle++], cH);
		}
	}
}

void GNOTIFY_Delete(char *cH)
{
	int nI;

	if(strlen(cH) == 0)
	{
		return;
	}

	if(nGNotifyHandle <= 0)
	{
		return;
	}

	for(nI = 0 ; nI < nGNotifyHandle ; nI++)
	{
		if(strlen(cGNotifyHandle [nI]) > 0)
		{
			if(stricmp(cGNotifyHandle [nI], cH) == 0)
			{
				strcpy(cGNotifyHandle [nGNotifyHandle++], "");
				break;
			}
		}
	}
}

int GNOTIFY_Find(char *cH)
{
	int nI, bF;

	if(strlen(cH) == 0)
	{
		return 0;
	}

	if(nGNotifyHandle <= 0)
	{
		return 0;
	}

	bF = 0;
	for(nI = 0 ; nI < nGNotifyHandle ; nI++)
	{
		if(strlen(cGNotifyHandle [nI]) > 0)
		{
			if(stricmp(cGNotifyHandle [nI], cH) == 0)
			{
				bF = 1;
				break;
			}
		}
	}

	return bF;
}
