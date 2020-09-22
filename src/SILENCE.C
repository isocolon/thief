#include "thief.h"

// new in Thief 1.2: Silence Mode (Ludens)

int CS_nItem, CS_nMode;
char CS_cHandle  [MAX_CENSOR_ITEM] [ICS_HANDLE_LENGTH];
char CS_cCensor  [MAX_CENSOR_ITEM] [MAX_CENSOR_BIT];
char CS_oHandle  [MAX_CENSOR_ITEM] [ICS_HANDLE_LENGTH];
char CS_cTell    [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
char CS_cPtell   [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
char CS_cSay     [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
char CS_cPartner [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];

BOOL CALLBACK SilenceBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)

{
	char cTmp [2048];

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			SendDlgItemMessage(hwnd, IDD_SILENCE_APPLY,     BM_SETCHECK, Silence.bSilenceApply          ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_SHOUT,     BM_SETCHECK, Silence.bSilenceShout          ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_CSHOUT,    BM_SETCHECK, Silence.bSilenceCShout         ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_ANNOUNCE,  BM_SETCHECK, Silence.bSilenceAnnouncement   ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_CHTELL,    BM_SETCHECK, Silence.bSilenceChTell         ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PETELL,    BM_SETCHECK, Silence.bSilencePeTell         ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_SAY,	     BM_SETCHECK, Silence.bSilenceSay            ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_KIBITZ,    BM_SETCHECK, Silence.bSilenceKibitz         ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_WHISPER,   BM_SETCHECK, Silence.bSilenceWhisper        ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLSHOUT,   BM_SETCHECK, Silence.bSilencePlayShout      ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLCSHOUT,  BM_SETCHECK, Silence.bSilencePlayCShout     ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLANNOUNCE, BM_SETCHECK, Silence.bSilencePlayAnnouncement ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLCHTELL,  BM_SETCHECK, Silence.bSilencePlayChTell     ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLPETELL,  BM_SETCHECK, Silence.bSilencePlayPeTell     ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLSAY,	 BM_SETCHECK, Silence.bSilencePlaySay        ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLKIBITZ,  BM_SETCHECK, Silence.bSilencePlayKibitz     ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_PLWHISPER, BM_SETCHECK, Silence.bSilencePlayWhisper    ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_SILENCE_TELL,		 WM_SETTEXT, 0, (LPARAM) Silence.cTell);

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					Silence.bSilenceApply				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_APPLY,       BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilenceShout				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_SHOUT,       BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilenceCShout				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_CSHOUT,      BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilenceAnnouncement		= ((SendDlgItemMessage(hwnd, IDD_SILENCE_ANNOUNCE,    BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilenceChTell				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_CHTELL,      BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePeTell				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PETELL,      BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilenceSay					= ((SendDlgItemMessage(hwnd, IDD_SILENCE_SAY,         BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilenceKibitz				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_KIBITZ,      BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilenceWhisper				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_WHISPER,     BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlayShout			= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLSHOUT,     BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlayCShout			= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLCSHOUT,    BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlayAnnouncement	= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLANNOUNCE,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlayChTell			= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLCHTELL,    BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlayPeTell			= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLPETELL,    BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlaySay				= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLSAY,       BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlayKibitz			= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLKIBITZ,    BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					Silence.bSilencePlayWhisper			= ((SendDlgItemMessage(hwnd, IDD_SILENCE_PLWHISPER,   BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_TELL, WM_GETTEXT, (WPARAM) SILENCE_COMMAND_SIZE - 1, (LPARAM) cTmp);
					strcpy(Silence.cTell, cTmp);
					SILENCE_Adjust();
					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_SILENCE_DEFAULT :
					SendDlgItemMessage(hwnd, IDD_SILENCE_APPLY,     BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_SHOUT,     BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_CSHOUT,    BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_ANNOUNCE,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_CHTELL,    BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PETELL,    BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_SAY,	     BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_KIBITZ,    BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_WHISPER,   BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLSHOUT,   BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLCSHOUT,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLANNOUNCE, BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLCHTELL,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLPETELL,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLSAY,	 BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLKIBITZ,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_PLWHISPER, BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_SILENCE_TELL,		 WM_SETTEXT, 0, (LPARAM) THIEF_SILENCE_TELL_MESSAGE);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

void SILENCE_Init(void)
{
	Silence.bSilenceApply				= DEFAULT_SILENCE;
	Silence.bSilenceShout				= DEFAULT_SILENCE;
	Silence.bSilenceCShout				= DEFAULT_SILENCE;
	Silence.bSilenceAnnouncement		= DEFAULT_SILENCE;
	Silence.bSilenceChTell				= DEFAULT_SILENCE;
	Silence.bSilencePeTell				= DEFAULT_SILENCE;
	Silence.bSilenceSay					= DEFAULT_SILENCE;
	Silence.bSilenceKibitz				= DEFAULT_SILENCE;
	Silence.bSilenceWhisper				= DEFAULT_SILENCE;
	Silence.bSilencePlayShout			= DEFAULT_SILENCE;
	Silence.bSilencePlayCShout			= DEFAULT_SILENCE;
	Silence.bSilencePlayAnnouncement	= DEFAULT_SILENCE;
	Silence.bSilencePlayChTell			= DEFAULT_SILENCE;
	Silence.bSilencePlayPeTell			= DEFAULT_SILENCE;
	Silence.bSilencePlaySay				= DEFAULT_SILENCE;
	Silence.bSilencePlayKibitz			= DEFAULT_SILENCE;
	Silence.bSilencePlayWhisper			= DEFAULT_SILENCE;
	strcpy(Silence.cTell, THIEF_SILENCE_TELL_MESSAGE);
}

void SILENCE_Adjust(void)
{
	int nJ;
	char cTmp [2048];

	strcpy(cTmp, Silence.cTell);
	TOOLBOX_AllTrim(cTmp);
	cTmp [THIEF_COMMAND_SIZE] = NULL_CHAR;

	for(nJ = 0 ; nJ < ((int) strlen(cTmp)) ; nJ++)
	{
		if((cTmp [nJ] < ' ') || (cTmp [nJ] > '~'))
		{
			cTmp [nJ] = NULL_CHAR;
			break;
		}
	}

	if(strlen(cTmp) == 0)
	{
		strcpy(Silence.cTell, "");
	}
	else
	{
		strcpy(Silence.cTell, cTmp);
	}
}


int SILENCE_Tell(char *cHandle, char *cS)
{
	char *cP, cTmp [512];

	cP = strchr(cS, ':');
	if(cP != NULL)
	{
		if(strlen(Silence.cTell) > 0)
		{
			if(strncmp(cP, THIEF_SILENCE_PREFIX1, THIEF_SILENCE_PREFIX1_SIZE))
			{
				// no it's not an auto message
				sprintf(cTmp,
						"%s %s %s%s\n",
						ICS_SILENCE_TELL_COMMAND,
						cHandle,
						THIEF_SILENCE_PREFIX,
						Silence.cTell);

				TOOLBOX_WriteICS(cTmp);

				Telnet.bTellTold = 1;
				sprintf(Telnet.cTellTold, "(told %s", cHandle);
				Telnet.nTellTold = strlen(Telnet.cTellTold);
			}
			else
			{
				// yes it's an auto message
				Telnet.bTellTold = 0;
			}
		}
		else
		{
			Telnet.bTellTold = 0;
		}
	}
	return 1;
}
