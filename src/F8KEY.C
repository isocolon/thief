#include "thief.h"

void F8KEY_Init(void)
{
	if(strlen(F8Key.cSitPieceDropMove) != 0)
	{
		TOOLBOX_WriteSystem("Cleared Sit For Piece Drop Move\n");
		strcpy(F8Key.cSitPieceDropMove, "");
	}
}

void F8KEY_Set(void)
{
	FARPROC LpProc;

	if(Game [INDEX_PLAY].nGameNumber > 0 && Game [INDEX_PLAY].bPlaying)
	{
		LpProc = MakeProcInstance((FARPROC) SitPieceBoxWndProc, hInst);
		DialogBox(hInst, "SitPieceBox", hwndWindow [HWND_FRAME], (DLGPROC) LpProc);
		FreeProcInstance(LpProc);
		SetFocus(hwndWindow [HWND_TELNET_EDIT]);
	}
	else
	{
		TOOLBOX_Beep();
	}
}

void F8KEY_Issue(void)
{
	char cTmp [255];
	int bOK;

	if(strlen(F8Key.cSitPieceDropMove) > 0)
	{
		if(Game [INDEX_PLAY].bIPlayWhite)
		{
			switch(F8Key.cSitPieceDropMove [0])
			{
				case ICS_WHITE_QUEEN :
				case ICS_BLACK_QUEEN :
					bOK = (Game [INDEX_PLAY].nBuffer [WHITE_QUEEN] > 0);
					break;

				case ICS_WHITE_ROOK :
				case ICS_BLACK_ROOK :
					bOK = (Game [INDEX_PLAY].nBuffer [WHITE_ROOK] > 0);
					break;

				case ICS_WHITE_BISHOP :
				case ICS_BLACK_BISHOP :
					bOK = (Game [INDEX_PLAY].nBuffer [WHITE_BISHOP] > 0);
					break;

				case ICS_WHITE_KNIGHT :
				case ICS_BLACK_KNIGHT :
					bOK = (Game [INDEX_PLAY].nBuffer [WHITE_KNIGHT] > 0);
					break;

				case ICS_WHITE_PAWN :
				case ICS_BLACK_PAWN :
					bOK = (Game [INDEX_PLAY].nBuffer [WHITE_PAWN] > 0);
					break;

				default :
					bOK = 0;
					break;
			}
		}
		else
		{
			switch(F8Key.cSitPieceDropMove [0])
			{
				case ICS_WHITE_QUEEN :
				case ICS_BLACK_QUEEN :
					bOK = (Game [INDEX_PLAY].nBuffer [BLACK_QUEEN] > 0);
					break;

				case ICS_WHITE_ROOK :
				case ICS_BLACK_ROOK :
					bOK = (Game [INDEX_PLAY].nBuffer [BLACK_ROOK] > 0);
					break;

				case ICS_WHITE_BISHOP :
				case ICS_BLACK_BISHOP :
					bOK = (Game [INDEX_PLAY].nBuffer [BLACK_BISHOP] > 0);
					break;

				case ICS_WHITE_KNIGHT :
				case ICS_BLACK_KNIGHT :
					bOK = (Game [INDEX_PLAY].nBuffer [BLACK_KNIGHT] > 0);
					break;

				case ICS_WHITE_PAWN :
				case ICS_BLACK_PAWN :
					bOK = (Game [INDEX_PLAY].nBuffer [BLACK_PAWN] > 0);
					break;

				default :
					bOK = 0;
					break;
			}
		}

		if(bOK)
		{
			sprintf(cTmp, "%s\n", F8Key.cSitPieceDropMove);
			TOOLBOX_WriteICS(cTmp);

			sprintf(cTmp, "Issued Sit For Piece Drop Move %s\n", F8Key.cSitPieceDropMove);
			TOOLBOX_WriteSystem(cTmp);

			F8KEY_Init();
		}
	}
}
