#include "thief.h"

char *LAYOUT_File(int nN)
{
	static char cBuffer [_MAX_PATH];

	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	int nI;

	_splitpath(System.cDocumentDir, Drive, Dir, File, Ext);

	nI = strlen(Dir);
	if(nI == 0)
	{
		strcat(Dir, "\\LAYOUT\\");
	}
	else
	{
		if(Dir [nI - 1] == '\\')
		{
			strcat(Dir, "LAYOUT\\");
		}
		else
		{
			strcat(Dir, "\\LAYOUT\\");
		}
	}

	sprintf(File, "%d", nN);
	strcpy(Ext, LAYOUT_EXT);
	_makepath(cBuffer, Drive, Dir, File, Ext);

	return cBuffer;
}

void LAYOUT_Load(int nN)
{
	int nI, nV;
	char Fn [_MAX_PATH], cTmp [_MAX_PATH];

	strcpy(Fn, LAYOUT_File(nN));

	TOOLBOX_ReadINICoord(&wCoord [COORD_MAIN], Fn, "Main", "Rect");

	System.bMaximizeMain = 0;
	nV = GetPrivateProfileInt("Main", "Maximize", System.bMaximizeMain, Fn);
	System.bMaximizeMain = (nV ? 1 : 0);

	TOOLBOX_ReadINICoord(&wCoord [COORD_PLAY], Fn, "Board", "Rect");

	nV = GetPrivateProfileInt("Board", "State",  wCoord [COORD_PLAY].s, Fn);
	wCoord [COORD_PLAY].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	TOOLBOX_ReadINICoord(&wCoord [COORD_TELNET], Fn, "Telnet", "Rect");

	nV = GetPrivateProfileInt("Telnet", "State", wCoord [COORD_TELNET].s, Fn);
	wCoord [COORD_TELNET].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	TOOLBOX_ReadINICoord(&wCoord [COORD_BUTTON], Fn, "Button", "Rect");

	nV = GetPrivateProfileInt("Button", "State", wCoord [COORD_BUTTON].s, Fn);
	wCoord [COORD_BUTTON].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	for(nI = 1 ; nI < MAX_GAME ; nI++)
	{
		sprintf(cTmp, "Rect %d", nI);
		TOOLBOX_ReadINICoord(&wCoord [nI], Fn, "Observe", cTmp);
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;

	TOOLBOX_CheckAllMenu();
	TOOLBOX_LoadAllWindowCoord();
}

void LAYOUT_ActualRestore(char *cFn)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	char cTmp  [_MAX_PATH];

	int nI, nV;

	_splitpath(cFn, Drive, Dir, File, Ext);
	strcpy(Ext, LAYOUT_EXT);
	_makepath(Fn, Drive, Dir, File, Ext);

	TOOLBOX_ReadINICoord(&wCoord [COORD_MAIN], Fn, "Main", "Rect");

	System.bMaximizeMain = 0;
	nV = GetPrivateProfileInt("Main", "Maximize", System.bMaximizeMain, Fn);
	System.bMaximizeMain = (nV ? 1 : 0);

	TOOLBOX_ReadINICoord(&wCoord [COORD_PLAY], Fn, "Board", "Rect");

	nV = GetPrivateProfileInt("Board", "State",  wCoord [COORD_PLAY].s, Fn);
	wCoord [COORD_PLAY].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	TOOLBOX_ReadINICoord(&wCoord [COORD_TELNET], Fn, "Telnet", "Rect");

	nV = GetPrivateProfileInt("Telnet", "State", wCoord [COORD_TELNET].s, Fn);
	wCoord [COORD_TELNET].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	TOOLBOX_ReadINICoord(&wCoord [COORD_BUTTON], Fn, "Button", "Rect");

	nV = GetPrivateProfileInt("Button", "State", wCoord [COORD_BUTTON].s, Fn);
	wCoord [COORD_BUTTON].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	for(nI = 1 ; nI < MAX_GAME ; nI++)
	{
		sprintf(cTmp, "Rect %d", nI);
		TOOLBOX_ReadINICoord(&wCoord [nI], Fn, "Observe", cTmp);
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;

	TOOLBOX_CheckAllMenu();

	System.bOkToResizePlay = 1;
	TOOLBOX_LoadAllWindowCoord();
	System.bOkToResizePlay = 0;
}

void LAYOUT_ActualSave(char *cFn)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	char cTmp  [_MAX_PATH];

	FILE *Fv;
	int nI;

	_splitpath(cFn, Drive, Dir, File, Ext);
	strcpy(Ext, LAYOUT_EXT);
	_makepath(Fn, Drive, Dir, File, Ext);

	TOOLBOX_SaveAllWindowCoord();

	if(System.bCDROMConnection)
	{
		return;
	}

	Fv = fopen(Fn, "w");
	if(Fv == NULL)
	{
		sprintf(cTmp, "Error Writing %s.", Fn);
		TOOLBOX_Error(cTmp);
		return;
	}

	fprintf(Fv, "[Main]\n");
	TOOLBOX_WriteINICoord(Fv, &wCoord [COORD_MAIN], "Rect");

	if(IsWindow(hwndWindow [HWND_FRAME]))
	{
		if(IsZoomed(hwndWindow [HWND_FRAME]))
		{
			fprintf(Fv, "Maximize=1\n");
		}
		else
		{
			fprintf(Fv, "Maximize=0\n");
		}
	}
	else
	{
		fprintf(Fv, "Maximize=0\n");
	}

	fprintf(Fv, "\n[Board]\n");
	TOOLBOX_WriteINICoord(Fv, &wCoord [COORD_PLAY], "Rect");

	sprintf(cTmp, "%d", wCoord [COORD_PLAY].s);
	fprintf(Fv, "State=%s\n", cTmp);

	fprintf(Fv, "\n[Telnet]\n");
	TOOLBOX_WriteINICoord(Fv, &wCoord [COORD_TELNET], "Rect");

	sprintf(cTmp, "%d", wCoord [COORD_TELNET].s);
	fprintf(Fv, "State=%s\n", cTmp);

	fprintf(Fv, "\n[Button]\n");
	TOOLBOX_WriteINICoord(Fv, &wCoord [COORD_BUTTON], "Rect");

	sprintf(cTmp, "%d", wCoord [COORD_BUTTON].s);
	fprintf(Fv, "State=%s\n", cTmp);

	fprintf(Fv, "\n[Observe]\n");
	for(nI = 1 ; nI < MAX_GAME ; nI++)
	{
		sprintf(cTmp, "Rect %d", nI);
		TOOLBOX_WriteINICoord(Fv, &wCoord [nI], cTmp);
	}

	fclose(Fv);
}

void LAYOUT_Restore(int nN)
{
	HWND hwnd;

	if(Game [INDEX_PLAY].nGameNumber > 0)
	{
		hwnd = hwndWindow [HWND_PLAY];
	}
	else
	{
		hwnd = (HWND) SendMessage(hwndWindow [HWND_CLIENT], WM_MDIGETACTIVE, 0, 0);
	}

	LAYOUT_ActualRestore(LAYOUT_File(nN));

	if(IsWindow(hwnd))
	{
		if(hwnd == hwndWindow [HWND_TELNET])
		{
			if(wCoord [COORD_TELNET].s != WIN_HIDE)
			{
				wCoord [COORD_TELNET].s = WIN_SHOW;
				SendMessage(hwndWindow [HWND_CLIENT],
							WM_MDIACTIVATE,
							(WPARAM)(HWND)
							hwndWindow [HWND_TELNET], 0);

				if(IsZoomed(hwndWindow [HWND_TELNET]) ||
						IsIconic(hwndWindow [HWND_TELNET]))
				{
					ShowWindow(hwndWindow [HWND_TELNET], SW_RESTORE);
				}
				else
				{
					ShowWindow(hwndWindow [HWND_TELNET], SW_SHOW);
				}

				SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			}
		}
		else if(hwnd == hwndWindow [HWND_BUTTON])
		{
			if(wCoord [COORD_BUTTON].s != WIN_HIDE)
			{
				wCoord [COORD_BUTTON].s = WIN_SHOW;
				SendMessage(hwndWindow [HWND_CLIENT],
							WM_MDIACTIVATE,
							(WPARAM)(HWND)
							hwndWindow [HWND_BUTTON], 0);

				if(IsZoomed(hwndWindow [HWND_BUTTON]) ||
						IsIconic(hwndWindow [HWND_BUTTON]))
				{
					ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
				}
				else
				{
					ShowWindow(hwndWindow [HWND_BUTTON], SW_SHOW);
				}

				SetFocus(hwndWindow [HWND_TELNET_EDIT]);
			}
		}
		else if(hwnd == hwndWindow [HWND_PLAY])
		{
			wCoord [COORD_PLAY].s = WIN_SHOW;
			SendMessage(hwndWindow [HWND_CLIENT],
						WM_MDIACTIVATE,
						(WPARAM)(HWND)
						hwndWindow [HWND_PLAY], 0);

			if(IsZoomed(hwndWindow [HWND_PLAY]) ||
					IsIconic(hwndWindow [HWND_PLAY]))
			{
				ShowWindow(hwndWindow [HWND_PLAY], SW_RESTORE);
			}
			else
			{
				ShowWindow(hwndWindow [HWND_PLAY], SW_SHOW);
			}

			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
		}
		else
		{
			SendMessage(hwndWindow [HWND_CLIENT],
						WM_MDIACTIVATE,
						(WPARAM)(HWND) hwnd, 0);

			if(IsZoomed(hwnd) ||
					IsIconic(hwnd))
			{
				ShowWindow(hwnd, SW_RESTORE);
			}
			else
			{
				ShowWindow(hwnd, SW_SHOW);
			}

			SetFocus(hwndWindow [HWND_TELNET_EDIT]);
		}
	}
}

void LAYOUT_Save(int nN)
{
	LAYOUT_ActualSave(LAYOUT_File(nN));
}

void LAYOUT_Load1(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	int nI;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = 0;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (151 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = nH - 1;
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	//
	// play
	//
	wCoord [COORD_PLAY].x = wCoord [COORD_BUTTON].w;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = (573 * nW1) / nOW;
	wCoord [COORD_PLAY].h = nH - 1;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	//
	// telnet
	//
	wCoord [COORD_TELNET].x = wCoord [COORD_PLAY].x + wCoord [COORD_PLAY].w;
	wCoord [COORD_TELNET].y = 0;
	wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
	wCoord [COORD_TELNET].h = nH - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	//
	// partner
	//
	wCoord [1].x = wCoord [COORD_PLAY].x;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_Load2(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	int nI, nJ;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = 0;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (183 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = nH - 1;
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	//
	// telnet
	//
	wCoord [COORD_TELNET].x = wCoord [COORD_BUTTON].w;
	wCoord [COORD_TELNET].y = (442 * nH1) / nOH;
	wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	//
	// play and observe #1
	//
	nI = (nW - wCoord [COORD_BUTTON].w - 1) >> 1;

	wCoord [COORD_PLAY].x = wCoord [COORD_BUTTON].w;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = nI;
	wCoord [COORD_PLAY].h = wCoord [COORD_TELNET].y;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	wCoord [1].x = wCoord [COORD_PLAY].x + wCoord [COORD_PLAY].w;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = nW - wCoord [1].x - 1;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	//
	// rest of observe (alternate between play and observe #1)
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		if(nI % 2)
		{
			nJ = 1;
		}
		else
		{
			nJ = INDEX_PLAY;
		}

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_Load3(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	int nI, nJ;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (478 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play, telnet, observe #1, #2, and #3
	//
	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = (nW - 1) / 3;
	wCoord [COORD_PLAY].h = (417 * nH1) / nOH;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	wCoord [COORD_TELNET].x = 0;
	wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
	wCoord [COORD_TELNET].w = nW - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	wCoord [1].x = wCoord [COORD_PLAY].x;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	wCoord [2].x = wCoord [1].w;
	wCoord [2].y = wCoord [1].y;
	wCoord [2].w = wCoord [1].w;
	wCoord [2].h = wCoord [1].h;
	wCoord [2].s = WIN_SHOW;

	wCoord [3].x = wCoord [2].x + wCoord [2].w;
	wCoord [3].y = wCoord [1].y;
	wCoord [3].w = nW - wCoord [3].x - 1;
	wCoord [3].h = wCoord [1].h;
	wCoord [3].s = WIN_SHOW;

	//
	// rest of observe (alternate observe 1, 2, and 3)
	//
	for(nI = 4 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 3);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_Load4(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	int nI, nJ;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (478 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// telnet, play, observe #1, #2, #3, and #4
	//
	wCoord [COORD_TELNET].x = (526 * nW1) / nOW;
	wCoord [COORD_TELNET].y = 0;
	wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = wCoord [COORD_TELNET].x >> 1;
	wCoord [COORD_PLAY].h = nH >> 1;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	wCoord [1].x = wCoord [COORD_PLAY].x;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	wCoord [2].x = wCoord [1].w;
	wCoord [2].y = wCoord [1].y;
	wCoord [2].w = wCoord [1].w;
	wCoord [2].h = wCoord [1].h;
	wCoord [2].s = WIN_SHOW;

	wCoord [3].x = wCoord [1].x;
	wCoord [3].y = wCoord [1].h;
	wCoord [3].w = wCoord [1].w;
	wCoord [3].h = wCoord [1].h;
	wCoord [3].s = WIN_SHOW;

	wCoord [4].x = wCoord [2].x;
	wCoord [4].y = wCoord [3].y;
	wCoord [4].w = wCoord [1].w;
	wCoord [4].h = wCoord [1].h;
	wCoord [4].s = WIN_SHOW;

	//
	// rest of observe (alternate observe 1, 2, 3, and 4)
	//
	for(nI = 5 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 4);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_Load5(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	int nI, nJ;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// telnet, play, observe #1, #2, #3, #4, and #5
	//
	wCoord [COORD_TELNET].x = nW - (nW / 3);
	wCoord [COORD_TELNET].y = nH >> 1;
	wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = nW / 3;
	wCoord [COORD_PLAY].h = nH >> 1;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	wCoord [1].x = wCoord [COORD_PLAY].x;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	wCoord [2].x = wCoord [1].w;
	wCoord [2].y = wCoord [1].y;
	wCoord [2].w = wCoord [1].w;
	wCoord [2].h = wCoord [1].h;
	wCoord [2].s = WIN_SHOW;

	wCoord [3].x = wCoord [2].x + wCoord [2].w;
	wCoord [3].y = wCoord [1].y;
	wCoord [3].w = nW - wCoord [3].x - 1;
	wCoord [3].h = wCoord [1].h;
	wCoord [3].s = WIN_SHOW;

	wCoord [4].x = wCoord [1].x;
	wCoord [4].y = wCoord [1].h;
	wCoord [4].w = wCoord [1].w;
	wCoord [4].h = wCoord [1].h;
	wCoord [4].s = WIN_SHOW;

	wCoord [5].x = wCoord [2].x;
	wCoord [5].y = wCoord [4].y;
	wCoord [5].w = wCoord [2].w;
	wCoord [5].h = wCoord [2].h;
	wCoord [5].s = WIN_SHOW;

	//
	// rest of observe (alternate observe 1, 2, 3, 4, and 5)
	//
	for(nI = 6 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 5);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_Load6(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	int nI, nJ;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (478 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// telnet, play, observe #1, #2, #3, #4, #5, and #6
	//
	wCoord [COORD_TELNET].x = (525 * nW1) / nOW;
	wCoord [COORD_TELNET].y = 0;
	wCoord [COORD_TELNET].w = (229 * nW1) / nOW;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = (262 * nW1) / nOW;
	wCoord [COORD_PLAY].h = nH >> 1;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	wCoord [1].x = wCoord [COORD_PLAY].x;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	wCoord [2].x = wCoord [1].w;
	wCoord [2].y = wCoord [1].y;
	wCoord [2].w = wCoord [1].w;
	wCoord [2].h = wCoord [1].h;
	wCoord [2].s = WIN_SHOW;

	wCoord [3].x = wCoord [COORD_TELNET].x + wCoord [COORD_TELNET].w;
	wCoord [3].y = wCoord [1].y;
	wCoord [3].w = wCoord [1].w;
	wCoord [3].h = wCoord [1].h;
	wCoord [3].s = WIN_SHOW;

	wCoord [4].x = wCoord [1].x;
	wCoord [4].y = wCoord [1].h;
	wCoord [4].w = wCoord [1].w;
	wCoord [4].h = wCoord [1].h;
	wCoord [4].s = WIN_SHOW;

	wCoord [5].x = wCoord [2].x;
	wCoord [5].y = wCoord [4].y;
	wCoord [5].w = wCoord [2].w;
	wCoord [5].h = wCoord [2].h;
	wCoord [5].s = WIN_SHOW;

	wCoord [6].x = wCoord [3].x;
	wCoord [6].y = wCoord [4].y;
	wCoord [6].w = wCoord [3].w;
	wCoord [6].h = wCoord [3].h;
	wCoord [6].s = WIN_SHOW;

	//
	// rest of observe (alternate observe 1, 2, 3, 4, 5, and 6)
	//
	for(nI = 7 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 6);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_Load7(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	APPBARDATA abd;
	int nX, nY, nX1, nY1, nI;

	//
	// main
	//
	nX = GetSystemMetrics(SM_CXSCREEN);
	nY = GetSystemMetrics(SM_CYSCREEN);

	nX1 = nX >> 1;
	nY1 = nY >> 1;

	abd.cbSize = sizeof(APPBARDATA);
	SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

	if(abd.rc.left > nX1)
	{
		wCoord [COORD_MAIN].x = 0;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].w = abd.rc.left >> 1;
		wCoord [COORD_MAIN].h = nY;
	}
	else if(abd.rc.top > nY1)
	{
		wCoord [COORD_MAIN].x = 0;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].w = nX >> 1;
		wCoord [COORD_MAIN].h = abd.rc.top;
	}
	else if(abd.rc.right > nX1)
	{
		wCoord [COORD_MAIN].x = 0;
		wCoord [COORD_MAIN].y = abd.rc.bottom;
		wCoord [COORD_MAIN].w = nX >> 1;
		wCoord [COORD_MAIN].h = nY - wCoord [COORD_MAIN].y;
	}
	else
	{
		wCoord [COORD_MAIN].x = abd.rc.right;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].w = (nX - wCoord [COORD_MAIN].x) >> 1;
		wCoord [COORD_MAIN].h = nY;
	}
	wCoord [COORD_MAIN].s = WIN_SHOW;

	nW  = nW  >> 1;
	nW1 = wCoord [COORD_MAIN].w;
	nOW = nOW >> 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = 0;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (462 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play
	//
	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = nW - 1;
	wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	//
	// telnet
	//
	wCoord [COORD_TELNET].x = 0;
	wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
	wCoord [COORD_TELNET].w = nW - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	//
	// observe #1
	//
	wCoord [1].x = wCoord [COORD_PLAY].w + 1;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_Load8(int nW, int nH, int nW1, int nH1, int nOW, int nOH)
{
	APPBARDATA abd;
	int nX, nY, nX1, nY1, nI;

	//
	// main
	//
	nX = GetSystemMetrics(SM_CXSCREEN);
	nY = GetSystemMetrics(SM_CYSCREEN);

	nX1 = nX >> 1;
	nY1 = nY >> 1;

	abd.cbSize = sizeof(APPBARDATA);
	SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

	if(abd.rc.left > nX1)
	{
		wCoord [COORD_MAIN].w = abd.rc.left >> 1;
		wCoord [COORD_MAIN].x = wCoord [COORD_MAIN].w;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].h = nY;
	}
	else if(abd.rc.top > nY1)
	{
		wCoord [COORD_MAIN].w = nX >> 1;
		wCoord [COORD_MAIN].x = wCoord [COORD_MAIN].w;
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].h = abd.rc.top;
	}
	else if(abd.rc.right > nX1)
	{
		wCoord [COORD_MAIN].w = nX >> 1;
		wCoord [COORD_MAIN].x = wCoord [COORD_MAIN].w;
		wCoord [COORD_MAIN].y = abd.rc.bottom;
		wCoord [COORD_MAIN].h = nY - wCoord [COORD_MAIN].y;
	}
	else
	{
		wCoord [COORD_MAIN].w = (nX - wCoord [COORD_MAIN].x) >> 1;
		wCoord [COORD_MAIN].x = abd.rc.right + (wCoord [COORD_MAIN].w >> 1);
		wCoord [COORD_MAIN].y = 0;
		wCoord [COORD_MAIN].h = nY;
	}
	wCoord [COORD_MAIN].s = WIN_SHOW;

	nW  = nW  >> 1;
	nW1 = wCoord [COORD_MAIN].w;
	nOW = nOW >> 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].x = 0;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (462 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play
	//
	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = nW - 1;
	wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	//
	// telnet
	//
	wCoord [COORD_TELNET].x = 0;
	wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
	wCoord [COORD_TELNET].w = nW - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	//
	// observe #1
	//
	wCoord [1].x = wCoord [COORD_PLAY].w + 1;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}
}

void LAYOUT_LoadVirtual(void)
{
	int nN, nI, nV;
	char Fn [_MAX_PATH], cTmp [_MAX_PATH];
	WCOORD wTemp [MAX_COORD];

	for(nN = 0 ; nN < MAX_LAYOUT ; nN++)
	{
		strcpy(Fn, LAYOUT_File(nN));

		TOOLBOX_ReadINICoord(&wTemp [COORD_MAIN], Fn, "Main",  "Rect");
		TOOLBOX_ReadINICoord(&wTemp [COORD_PLAY], Fn, "Board", "Rect");

		nV = GetPrivateProfileInt("Board", "State",  wTemp [COORD_PLAY].s, Fn);
		wTemp [COORD_PLAY].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

		TOOLBOX_ReadINICoord(&wTemp [COORD_TELNET], Fn, "Telnet", "Rect");

		nV = GetPrivateProfileInt("Telnet", "State", wTemp [COORD_TELNET].s, Fn);
		wTemp [COORD_TELNET].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

		TOOLBOX_ReadINICoord(&wTemp [COORD_BUTTON], Fn, "Button", "Rect");

		nV = GetPrivateProfileInt("Button", "State", wTemp [COORD_BUTTON].s, Fn);
		wTemp [COORD_BUTTON].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

		for(nI = 1 ; nI < MAX_GAME ; nI++)
		{
			sprintf(cTmp, "Rect %d", nI);
			TOOLBOX_ReadINICoord(&wTemp [nI], Fn, "Observe", cTmp);
		}

		for(nI = 0 ; nI < MAX_COORD ; nI++)
		{
			wLayout [nN] [nI].x = wTemp [nI].x;
			wLayout [nN] [nI].y = wTemp [nI].y;
			wLayout [nN] [nI].w = wTemp [nI].w;
			wLayout [nN] [nI].h = wTemp [nI].h;
			wLayout [nN] [nI].s = wTemp [nI].s;
		}
	}
}

void LAYOUT_CreateDefaultLayout(int nW, int nH, int nW1, int nH1, int nOW, int nOH, int nI)
{
	switch(nI)
	{
		case 0 :
			LAYOUT_Load2(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 1 :
			LAYOUT_Load1(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 2 :
			LAYOUT_Load2(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 3 :
			LAYOUT_Load3(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 4 :
			LAYOUT_Load4(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 5 :
			LAYOUT_Load5(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 6 :
			LAYOUT_Load6(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 7 :
			LAYOUT_Load7(nW, nH, nW1, nH1, nOW, nH1);
			break;
		case 8 :
			LAYOUT_Load8(nW, nH, nW1, nH1, nOW, nH1);
			break;
	}
}

void LAYOUT_Try_1(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_BUTTON].x = 0;
		wCoord [COORD_BUTTON].y = 0;
		wCoord [COORD_BUTTON].w = (183 * nW1) / nOW;

		if(User.bExtendedButton)
		{
			wCoord [COORD_BUTTON].h = nH - 1;
		}
		else
		{
			wCoord [COORD_BUTTON].h = (442 * nH1) / nOH;
		}

		wCoord [COORD_BUTTON].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_BUTTON].w = (183 * nW1) / nOW;
		wCoord [COORD_BUTTON].x = nW - wCoord [COORD_BUTTON].w - 1;
		wCoord [COORD_BUTTON].y = 0;

		if(User.bExtendedButton)
		{
			wCoord [COORD_BUTTON].h = nH - 1;
		}
		else
		{
			wCoord [COORD_BUTTON].h = (442 * nH1) / nOH;
		}

		wCoord [COORD_BUTTON].s = WIN_SHOW;
	}

	//
	// telnet
	//
	if(bPlayOnLeft)
	{
		if(User.bExtendedButton)
		{
			wCoord [COORD_TELNET].x = wCoord [COORD_BUTTON].w;
		}
		else
		{
			wCoord [COORD_TELNET].x = 0;
		}

		wCoord [COORD_TELNET].y = (442 * nH1) / nOH;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = (442 * nH1) / nOH;

		if(User.bExtendedButton)
		{
			wCoord [COORD_TELNET].w = wCoord [COORD_BUTTON].x;
		}
		else
		{
			wCoord [COORD_TELNET].w = nW - 1;
		}

		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;
	}

	//
	// play and observe #1
	//
	if(bPlayOnLeft)
	{
		nI = (nW - wCoord [COORD_BUTTON].w - 1) >> 1;

		wCoord [COORD_PLAY].x = wCoord [COORD_BUTTON].w;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = nI;
		wCoord [COORD_PLAY].h = wCoord [COORD_TELNET].y;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x + wCoord [COORD_PLAY].w;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = nW - wCoord [1].x - 1;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		nI = (wCoord [COORD_BUTTON].x - 1) >> 1;

		wCoord [COORD_PLAY].x = wCoord [COORD_BUTTON].x - nI;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = nI;
		wCoord [COORD_PLAY].h = wCoord [COORD_TELNET].y;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = 0;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].x;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate between play and observe #1)
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		if(nI % 2)
		{
			nJ = 1;
		}
		else
		{
			nJ = INDEX_PLAY;
		}

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_2(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].w = (183 * nW1) / nOW;
	wCoord [COORD_BUTTON].x = (nW >> 1) - (wCoord [COORD_BUTTON].w >> 1);
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].h = (442 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	//
	// telnet
	//
	wCoord [COORD_TELNET].x = 0;
	wCoord [COORD_TELNET].y = wCoord [COORD_BUTTON].h;
	wCoord [COORD_TELNET].w = nW - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	//
	// play and observe #1
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = wCoord [COORD_BUTTON].x;
		wCoord [COORD_PLAY].h = wCoord [COORD_BUTTON].h;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_BUTTON].x + wCoord [COORD_BUTTON].w;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = nW - wCoord [1].x - 1;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_PLAY].x = wCoord [COORD_BUTTON].x + wCoord [COORD_BUTTON].w;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = nW - wCoord [COORD_PLAY].x - 1;
		wCoord [COORD_PLAY].h = wCoord [COORD_BUTTON].h;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = 0;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_BUTTON].x;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate between play and observe #1)
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		if(nI % 2)
		{
			nJ = 1;
		}
		else
		{
			nJ = INDEX_PLAY;
		}

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_3(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	GetClientRect(hwndWindow [HWND_BUTTON], &rc);

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_BUTTON], &wp);

	wCoord [COORD_BUTTON].x = 0;
	wCoord [COORD_BUTTON].y = (((rc.bottom - rc.top) + 1) - ((wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1)) + 4;
	wCoord [COORD_BUTTON].w = nW - 1;

	if(User.bExtendedButton)
	{
		wCoord [COORD_BUTTON].h = (105 * nH1) / nOH;
	}
	else
	{
		wCoord [COORD_BUTTON].h = (64 * nH1) / nOH;
	}

	//
	// play
	//
	wCoord [COORD_PLAY].w = (642 * nW1) / nOW;

	if(bPlayOnLeft)
	{
		wCoord [COORD_PLAY].x = 0;
	}
	else
	{
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
	}

	if(wCoord [COORD_BUTTON].y < 0)
	{
		wCoord [COORD_PLAY].y = wCoord [COORD_BUTTON].h + wCoord [COORD_BUTTON].y;
	}
	else
	{
		wCoord [COORD_PLAY].y = wCoord [COORD_BUTTON].h - wCoord [COORD_BUTTON].y;
	}

	wCoord [COORD_PLAY].h = nH - wCoord [COORD_PLAY].y - 1;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	//
	// telnet
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_TELNET].x = (643 * nW1) / nOW;
		wCoord [COORD_TELNET].y = (457 * nH1) / nOH;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = (457 * nH1) / nOH;
		wCoord [COORD_TELNET].w = wCoord [COORD_PLAY].x;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;
	}

	//
	// observe #1
	//
	if(bPlayOnLeft)
	{
		wCoord [1].x = wCoord [COORD_TELNET].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_TELNET].w;
		wCoord [1].h = wCoord [COORD_TELNET].y - wCoord [COORD_PLAY].y - 1;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		wCoord [1].x = wCoord [COORD_TELNET].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_TELNET].w;
		wCoord [1].h = wCoord [COORD_TELNET].y - wCoord [COORD_PLAY].y - 1;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate between play and observe #1)
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		if(nI % 2)
		{
			nJ = 1;
		}
		else
		{
			nJ = INDEX_PLAY;
		}

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_4(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play, telnet, and observe #1
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (nW - 1) >> 1;
		wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].w = nW - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].w;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_PLAY].w = (nW - 1) >> 1;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].w = nW - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = 0;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].x;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate between play and observe #1)
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		if(nI % 2)
		{
			nJ = 1;
		}
		else
		{
			nJ = INDEX_PLAY;
		}

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_5(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (462 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play, telnet, and observe #1
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (722 * nW1) / nOW;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = wCoord [COORD_PLAY].w;
		wCoord [COORD_TELNET].y = (343 * nH1) / nOH;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].w;
		wCoord [1].y = 0;
		wCoord [1].w = nW - wCoord [1].x - 1;
		wCoord [1].h = wCoord [COORD_TELNET].y;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_PLAY].w = (722 * nW1) / nOW;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = (343 * nH1) / nOH;
		wCoord [COORD_TELNET].w = wCoord [COORD_PLAY].x;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = 0;
		wCoord [1].y = 0;
		wCoord [1].w = wCoord [COORD_PLAY].x;
		wCoord [1].h = wCoord [COORD_TELNET].y;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate between play and observe #1)
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		if(nI % 2)
		{
			nJ = 1;
		}
		else
		{
			nJ = INDEX_PLAY;
		}

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_6(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 512;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW >> 1;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left >> 1;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW >> 1;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW >> 1;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = (nW - wCoord [COORD_MAIN].x) >> 1;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = 0;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (462 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play
	//
	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = nW - 1;
	wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	//
	// telnet
	//
	wCoord [COORD_TELNET].x = 0;
	wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
	wCoord [COORD_TELNET].w = nW - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	//
	// observe #1
	//
	wCoord [1].x = wCoord [COORD_PLAY].w + 1;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_7(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 512;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].w = nW >> 1;
			wCoord [COORD_MAIN].x = wCoord [COORD_MAIN].w;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].w = abd.rc.left >> 1;
				wCoord [COORD_MAIN].x = wCoord [COORD_MAIN].w;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].w = nW >> 1;
				wCoord [COORD_MAIN].x = wCoord [COORD_MAIN].w;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].w = nW >> 1;
				wCoord [COORD_MAIN].x = wCoord [COORD_MAIN].w;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].w = (nW - wCoord [COORD_MAIN].x) >> 1;
				wCoord [COORD_MAIN].x = abd.rc.right + (wCoord [COORD_MAIN].w >> 1);
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = 0;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (462 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play
	//
	wCoord [COORD_PLAY].x = 0;
	wCoord [COORD_PLAY].y = 0;
	wCoord [COORD_PLAY].w = nW - 1;
	wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
	wCoord [COORD_PLAY].s = WIN_SHOW;

	//
	// telnet
	//
	wCoord [COORD_TELNET].x = 0;
	wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
	wCoord [COORD_TELNET].w = nW - 1;
	wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
	wCoord [COORD_TELNET].s = WIN_SHOW;

	//
	// observe #1
	//
	wCoord [1].x = wCoord [COORD_PLAY].w + 1;
	wCoord [1].y = wCoord [COORD_PLAY].y;
	wCoord [1].w = wCoord [COORD_PLAY].w;
	wCoord [1].h = wCoord [COORD_PLAY].h;
	wCoord [1].s = WIN_SHOW;

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_8(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play, telnet, and observe #1
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (722 * nW1) / nOW;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = wCoord [COORD_PLAY].w;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].y;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_TELNET].h = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_PLAY].w = (722 * nW1) / nOW;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].y;
		wCoord [COORD_TELNET].w = wCoord [COORD_PLAY].x;
		wCoord [COORD_TELNET].h = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_9(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play, telnet, and observe #1
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = nW >> 1;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = wCoord [COORD_PLAY].w;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].y;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_TELNET].h = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_PLAY].w = nW >> 1;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].y;
		wCoord [COORD_TELNET].w = wCoord [COORD_PLAY].x;
		wCoord [COORD_TELNET].h = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_10(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 768;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (478 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// play, telnet, observe #1, #2, and #3
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (nW - 1) / 3;
		wCoord [COORD_PLAY].h = (418 * nH1) / nOH;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].w = nW - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = wCoord [2].x + wCoord [2].w;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = nW - wCoord [3].x - 1;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_PLAY].w = (nW - 1) / 3;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = (418 * nH1) / nOH;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
		wCoord [COORD_TELNET].w = nW - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].x - wCoord [1].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = 0;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = wCoord [2].x;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate observe 1, 2, and 3)
	//
	for(nI = 4 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 3);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;

//  System.bMaximizeMain = 0;
//
//  //
//  // original screen size
//  //
//  nOW = 1024;
//  nOH = 740;
//
//  //
//  // main
//  //
//  if (bMaxScreen)
//      {
//      nW = GetSystemMetrics (SM_CXSCREEN);
//      nH = GetSystemMetrics (SM_CYSCREEN);
//
//      if (bOverlapTaskbar)
//          {
//          wCoord [COORD_MAIN].x = 0;
//          wCoord [COORD_MAIN].y = 0;
//          wCoord [COORD_MAIN].w = nW;
//          wCoord [COORD_MAIN].h = nH;
//          wCoord [COORD_MAIN].s = WIN_SHOW;
//          }
//      else
//          {
//          nW1 = nW >> 1;
//          nH1 = nH >> 1;
//
//          abd.cbSize = sizeof (APPBARDATA);
//          SHAppBarMessage (ABM_GETTASKBARPOS, &abd);
//
//          if (abd.rc.left > nW1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = abd.rc.left;
//              wCoord [COORD_MAIN].h = nH;
//              }
//          else
//          if (abd.rc.top > nH1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = nW;
//              wCoord [COORD_MAIN].h = abd.rc.top;
//              }
//          else
//          if (abd.rc.right > nW1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = abd.rc.bottom;
//              wCoord [COORD_MAIN].w = nW;
//              wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
//              }
//          else
//              {
//              wCoord [COORD_MAIN].x = abd.rc.right;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
//              wCoord [COORD_MAIN].h = nH;
//              }
//          wCoord [COORD_MAIN].s = WIN_SHOW;
//          }
//      }
//  else
//      {
//      wp.length = sizeof (WINDOWPLACEMENT);
//      GetWindowPlacement (hwndWindow [HWND_FRAME], &wp);
//
//      wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
//      wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
//      wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
//      wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1;
//      wCoord [COORD_MAIN].s = WIN_SHOW;
//      }
//
//  MoveWindow (hwndWindow [HWND_FRAME],
//              wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
//              wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
//              TRUE);
//
//  GetClientRect (hwndWindow [HWND_FRAME], &rc);
//
//  nW = (rc.right  - rc.left) + 1;
//  nH = (rc.bottom - rc.top ) + 1;
//
//  wp.length = sizeof (WINDOWPLACEMENT);
//  GetWindowPlacement (hwndWindow [HWND_FRAME], &wp);
//
//  nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
//  nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1;
//
//  //
//  // button
//  //
//  wCoord [COORD_BUTTON].s = WIN_SHOW;
//
//  if (IsIconic (hwndWindow [HWND_BUTTON]) || IsZoomed (hwndWindow [HWND_BUTTON]))
//      {
//      ShowWindow (hwndWindow [HWND_BUTTON], SW_RESTORE);
//      }
//
//  wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
//  wCoord [COORD_BUTTON].y = 0;
//  wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
//  wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
//  wCoord [COORD_BUTTON].s = WIN_HIDE;
//
//  //
//  // play, telnet, observe #1, #2, and #3
//  //
//  if (bPlayOnLeft)
//      {
//      wCoord [COORD_PLAY].x = 0;
//      wCoord [COORD_PLAY].y = 0;
//      wCoord [COORD_PLAY].w = (nW - 1) / 3;
//      wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
//      wCoord [COORD_PLAY].s = WIN_SHOW;
//
//      wCoord [COORD_TELNET].x = 0;
//      wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
//      wCoord [COORD_TELNET].w = nW - 1;
//      wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
//      wCoord [COORD_TELNET].s = WIN_SHOW;
//
//      wCoord [1].x = wCoord [COORD_PLAY].x;
//      wCoord [1].y = wCoord [COORD_PLAY].y;
//      wCoord [1].w = wCoord [COORD_PLAY].w;
//      wCoord [1].h = wCoord [COORD_PLAY].h;
//      wCoord [1].s = WIN_SHOW;
//
//      wCoord [2].x = wCoord [1].w;
//      wCoord [2].y = wCoord [1].y;
//      wCoord [2].w = wCoord [1].w;
//      wCoord [2].h = wCoord [1].h;
//      wCoord [2].s = WIN_SHOW;
//
//      wCoord [3].x = wCoord [2].x + wCoord [2].w;
//      wCoord [3].y = wCoord [1].y;
//      wCoord [3].w = nW - wCoord [3].x - 1;
//      wCoord [3].h = wCoord [1].h;
//      wCoord [3].s = WIN_SHOW;
//      }
//  else
//      {
//      wCoord [COORD_PLAY].w = (nW - 1) / 3;
//      wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
//      wCoord [COORD_PLAY].y = 0;
//      wCoord [COORD_PLAY].h = (511 * nH1) / nOH;
//      wCoord [COORD_PLAY].s = WIN_SHOW;
//
//      wCoord [COORD_TELNET].x = 0;
//      wCoord [COORD_TELNET].y = wCoord [COORD_PLAY].h;
//      wCoord [COORD_TELNET].w = nW - 1;
//      wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
//      wCoord [COORD_TELNET].s = WIN_SHOW;
//
//      wCoord [1].x = wCoord [COORD_PLAY].x;
//      wCoord [1].y = wCoord [COORD_PLAY].y;
//      wCoord [1].w = wCoord [COORD_PLAY].w;
//      wCoord [1].h = wCoord [COORD_PLAY].h;
//      wCoord [1].s = WIN_SHOW;
//
//      wCoord [2].x = wCoord [1].x - wCoord [1].w;
//      wCoord [2].y = wCoord [1].y;
//      wCoord [2].w = wCoord [1].w;
//      wCoord [2].h = wCoord [1].h;
//      wCoord [2].s = WIN_SHOW;
//
//      wCoord [3].x = 0;
//      wCoord [3].y = wCoord [1].y;
//      wCoord [3].w = wCoord [2].x;
//      wCoord [3].h = wCoord [1].h;
//      wCoord [3].s = WIN_SHOW;
//      }
//
//  //
//  // rest of observe (alternate observe 1, 2, and 3)
//  //
//  for (nI = 4 ; nI < MAX_GAME ; nI++)
//      {
//      nJ = (nI % 3);
//
//      wCoord [nI].x = wCoord [nJ].x;
//      wCoord [nI].y = wCoord [nJ].y;
//      wCoord [nI].w = wCoord [nJ].w;
//      wCoord [nI].h = wCoord [nJ].h;
//      wCoord [nI].s = WIN_SHOW;
//      }
//
//  wPartner.x = wCoord [1].x;
//  wPartner.y = wCoord [1].y;
//  wPartner.w = wCoord [1].w;
//  wPartner.h = wCoord [1].h;
//  wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_11(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 767;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (478 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// telnet, play, observe #1, #2, #3, and #4
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_TELNET].x = (526 * nW1) / nOW;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = wCoord [COORD_TELNET].x >> 1;
		wCoord [COORD_PLAY].h = nH >> 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = wCoord [1].x;
		wCoord [3].y = wCoord [1].h;
		wCoord [3].w = wCoord [1].w;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [2].x;
		wCoord [4].y = wCoord [3].y;
		wCoord [4].w = wCoord [1].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = nW - ((490 * nW1) / nOW);
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].w = (nW - wCoord [COORD_TELNET].w) >> 1;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = nH >> 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [COORD_TELNET].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w - 1;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = wCoord [1].x;
		wCoord [3].y = wCoord [1].h;
		wCoord [3].w = wCoord [1].w;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [2].x;
		wCoord [4].y = wCoord [3].y;
		wCoord [4].w = wCoord [2].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate observe 1, 2, 3, and 4)
	//
	for(nI = 5 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 4);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;

//  System.bMaximizeMain = 0;
//
//  //
//  // original screen size
//  //
//  nOW = 1024;
//  nOH = 740;
//
//  //
//  // main
//  //
//  if (bMaxScreen)
//      {
//      nW = GetSystemMetrics (SM_CXSCREEN);
//      nH = GetSystemMetrics (SM_CYSCREEN);
//
//      if (bOverlapTaskbar)
//          {
//          wCoord [COORD_MAIN].x = 0;
//          wCoord [COORD_MAIN].y = 0;
//          wCoord [COORD_MAIN].w = nW;
//          wCoord [COORD_MAIN].h = nH;
//          wCoord [COORD_MAIN].s = WIN_SHOW;
//          }
//      else
//          {
//          nW1 = nW >> 1;
//          nH1 = nH >> 1;
//
//          abd.cbSize = sizeof (APPBARDATA);
//          SHAppBarMessage (ABM_GETTASKBARPOS, &abd);
//
//          if (abd.rc.left > nW1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = abd.rc.left;
//              wCoord [COORD_MAIN].h = nH;
//              }
//          else
//          if (abd.rc.top > nH1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = nW;
//              wCoord [COORD_MAIN].h = abd.rc.top;
//              }
//          else
//          if (abd.rc.right > nW1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = abd.rc.bottom;
//              wCoord [COORD_MAIN].w = nW;
//              wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
//              }
//          else
//              {
//              wCoord [COORD_MAIN].x = abd.rc.right;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
//              wCoord [COORD_MAIN].h = nH;
//              }
//          wCoord [COORD_MAIN].s = WIN_SHOW;
//          }
//      }
//  else
//      {
//      wp.length = sizeof (WINDOWPLACEMENT);
//      GetWindowPlacement (hwndWindow [HWND_FRAME], &wp);
//
//      wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
//      wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
//      wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
//      wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1;
//      wCoord [COORD_MAIN].s = WIN_SHOW;
//      }
//
//  MoveWindow (hwndWindow [HWND_FRAME],
//              wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
//              wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
//              TRUE);
//
//  GetClientRect (hwndWindow [HWND_FRAME], &rc);
//
//  nW = (rc.right  - rc.left) + 1;
//  nH = (rc.bottom - rc.top ) + 1;
//
//  wp.length = sizeof (WINDOWPLACEMENT);
//  GetWindowPlacement (hwndWindow [HWND_FRAME], &wp);
//
//  nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
//  nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1;
//
//  //
//  // button
//  //
//  wCoord [COORD_BUTTON].s = WIN_SHOW;
//
//  if (IsIconic (hwndWindow [HWND_BUTTON]) || IsZoomed (hwndWindow [HWND_BUTTON]))
//      {
//      ShowWindow (hwndWindow [HWND_BUTTON], SW_RESTORE);
//      }
//
//  wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
//  wCoord [COORD_BUTTON].y = 0;
//  wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
//  wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
//  wCoord [COORD_BUTTON].s = WIN_HIDE;
//
//  //
//  // telnet, play, observe #1, #2, #3, and #4
//  //
//  if (bPlayOnLeft)
//      {
//      wCoord [COORD_TELNET].x = (678 * nW1) / nOW;
//      wCoord [COORD_TELNET].y = 0;
//      wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
//      wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
//      wCoord [COORD_TELNET].s = WIN_SHOW;
//
//      wCoord [COORD_PLAY].x = 0;
//      wCoord [COORD_PLAY].y = 0;
//      wCoord [COORD_PLAY].w = wCoord [COORD_TELNET].x >> 1;
//      wCoord [COORD_PLAY].h = nH >> 1;
//      wCoord [COORD_PLAY].s = WIN_SHOW;
//
//      wCoord [1].x = wCoord [COORD_PLAY].x;
//      wCoord [1].y = wCoord [COORD_PLAY].y;
//      wCoord [1].w = wCoord [COORD_PLAY].w;
//      wCoord [1].h = wCoord [COORD_PLAY].h;
//      wCoord [1].s = WIN_SHOW;
//
//      wCoord [2].x = wCoord [1].w;
//      wCoord [2].y = wCoord [1].y;
//      wCoord [2].w = wCoord [1].w;
//      wCoord [2].h = wCoord [1].h;
//      wCoord [2].s = WIN_SHOW;
//
//      wCoord [3].x = wCoord [1].x;
//      wCoord [3].y = wCoord [1].h;
//      wCoord [3].w = wCoord [1].w;
//      wCoord [3].h = wCoord [1].h;
//      wCoord [3].s = WIN_SHOW;
//
//      wCoord [4].x = wCoord [2].x;
//      wCoord [4].y = wCoord [3].y;
//      wCoord [4].w = wCoord [1].w;
//      wCoord [4].h = wCoord [1].h;
//      wCoord [4].s = WIN_SHOW;
//      }
//  else
//      {
//      wCoord [COORD_TELNET].x = 0;
//      wCoord [COORD_TELNET].y = 0;
//      wCoord [COORD_TELNET].w = nW - ((678 * nW1) / nOW);
//      wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
//      wCoord [COORD_TELNET].s = WIN_SHOW;
//
//      wCoord [COORD_PLAY].w = (nW - wCoord [COORD_TELNET].w) >> 1;
//      wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
//      wCoord [COORD_PLAY].y = 0;
//      wCoord [COORD_PLAY].h = nH >> 1;
//      wCoord [COORD_PLAY].s = WIN_SHOW;
//
//      wCoord [1].x = wCoord [COORD_PLAY].x;
//      wCoord [1].y = wCoord [COORD_PLAY].y;
//      wCoord [1].w = wCoord [COORD_PLAY].w;
//      wCoord [1].h = wCoord [COORD_PLAY].h;
//      wCoord [1].s = WIN_SHOW;
//
//      wCoord [2].x = wCoord [COORD_TELNET].w;
//      wCoord [2].y = wCoord [1].y;
//      wCoord [2].w = wCoord [1].w - 1;
//      wCoord [2].h = wCoord [1].h;
//      wCoord [2].s = WIN_SHOW;
//
//      wCoord [3].x = wCoord [1].x;
//      wCoord [3].y = wCoord [1].h;
//      wCoord [3].w = wCoord [1].w;
//      wCoord [3].h = wCoord [1].h;
//      wCoord [3].s = WIN_SHOW;
//
//      wCoord [4].x = wCoord [2].x;
//      wCoord [4].y = wCoord [3].y;
//      wCoord [4].w = wCoord [2].w;
//      wCoord [4].h = wCoord [1].h;
//      wCoord [4].s = WIN_SHOW;
//      }
//
//  //
//  // rest of observe (alternate observe 1, 2, 3, and 4)
//  //
//  for (nI = 5 ; nI < MAX_GAME ; nI++)
//      {
//      nJ = (nI % 4);
//
//      wCoord [nI].x = wCoord [nJ].x;
//      wCoord [nI].y = wCoord [nJ].y;
//      wCoord [nI].w = wCoord [nJ].w;
//      wCoord [nI].h = wCoord [nJ].h;
//      wCoord [nI].s = WIN_SHOW;
//      }
//
//  wPartner.x = wCoord [1].x;
//  wPartner.y = wCoord [1].y;
//  wPartner.w = wCoord [1].w;
//  wPartner.h = wCoord [1].h;
//  wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_12(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// telnet, play, observe #1, #2, #3, #4, and #5
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_TELNET].x = nW - (nW / 3);
		wCoord [COORD_TELNET].y = nH >> 1;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = nW / 3;
		wCoord [COORD_PLAY].h = nH >> 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = wCoord [2].x + wCoord [2].w;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = nW - wCoord [3].x - 1;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [1].x;
		wCoord [4].y = wCoord [1].h;
		wCoord [4].w = wCoord [1].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;

		wCoord [5].x = wCoord [2].x;
		wCoord [5].y = wCoord [4].y;
		wCoord [5].w = wCoord [2].w;
		wCoord [5].h = wCoord [2].h;
		wCoord [5].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = nH >> 1;
		wCoord [COORD_TELNET].w = nW / 3;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].w = nW / 3;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = nH >> 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = (wCoord [1].x - wCoord [1].w) + 1;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w - 1;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = 0;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = wCoord [2].x;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [1].x;
		wCoord [4].y = wCoord [1].h;
		wCoord [4].w = wCoord [1].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;

		wCoord [5].x = wCoord [2].x;
		wCoord [5].y = wCoord [4].y;
		wCoord [5].w = wCoord [2].w;
		wCoord [5].h = wCoord [4].h;
		wCoord [5].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate observe 1, 2, 3, 4, and 5)
	//
	for(nI = 6 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 5);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_13(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1025;
	nOH = 767;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (478 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// telnet, play, observe #1, #2, #3, #4, #5, and #6
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_TELNET].x = (525 * nW1) / nOW;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = (229 * nW1) / nOW;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (262 * nW1) / nOW;
		wCoord [COORD_PLAY].h = nH >> 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = wCoord [COORD_TELNET].x + wCoord [COORD_TELNET].w;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = wCoord [1].w;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [1].x;
		wCoord [4].y = wCoord [1].h;
		wCoord [4].w = wCoord [1].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;

		wCoord [5].x = wCoord [2].x;
		wCoord [5].y = wCoord [4].y;
		wCoord [5].w = wCoord [2].w;
		wCoord [5].h = wCoord [2].h;
		wCoord [5].s = WIN_SHOW;

		wCoord [6].x = wCoord [3].x;
		wCoord [6].y = wCoord [4].y;
		wCoord [6].w = wCoord [3].w;
		wCoord [6].h = wCoord [3].h;
		wCoord [6].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_PLAY].w = (262 * nW1) / nOW;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = nH >> 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = wCoord [COORD_PLAY].w;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = (229 * nW1) / nOW;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].x - wCoord [1].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = 0;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = wCoord [1].w - 1;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [1].x;
		wCoord [4].y = wCoord [1].h;
		wCoord [4].w = wCoord [1].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;

		wCoord [5].x = wCoord [2].x;
		wCoord [5].y = wCoord [4].y;
		wCoord [5].w = wCoord [2].w;
		wCoord [5].h = wCoord [2].h;
		wCoord [5].s = WIN_SHOW;

		wCoord [6].x = wCoord [3].x;
		wCoord [6].y = wCoord [4].y;
		wCoord [6].w = wCoord [3].w;
		wCoord [6].h = wCoord [3].h;
		wCoord [6].s = WIN_SHOW;
	}

	//
	// rest of observe (alternate observe 1, 2, 3, 4, 5, and 6)
	//
	for(nI = 7 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 6);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;

//  System.bMaximizeMain = 0;
//
//  //
//  // original screen size
//  //
//  nOW = 1024;
//  nOH = 740;
//
//  //
//  // main
//  //
//  if (bMaxScreen)
//      {
//      nW = GetSystemMetrics (SM_CXSCREEN);
//      nH = GetSystemMetrics (SM_CYSCREEN);
//
//      if (bOverlapTaskbar)
//          {
//          wCoord [COORD_MAIN].x = 0;
//          wCoord [COORD_MAIN].y = 0;
//          wCoord [COORD_MAIN].w = nW;
//          wCoord [COORD_MAIN].h = nH;
//          wCoord [COORD_MAIN].s = WIN_SHOW;
//          }
//      else
//          {
//          nW1 = nW >> 1;
//          nH1 = nH >> 1;
//
//          abd.cbSize = sizeof (APPBARDATA);
//          SHAppBarMessage (ABM_GETTASKBARPOS, &abd);
//
//          if (abd.rc.left > nW1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = abd.rc.left;
//              wCoord [COORD_MAIN].h = nH;
//              }
//          else
//          if (abd.rc.top > nH1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = nW;
//              wCoord [COORD_MAIN].h = abd.rc.top;
//              }
//          else
//          if (abd.rc.right > nW1)
//              {
//              wCoord [COORD_MAIN].x = 0;
//              wCoord [COORD_MAIN].y = abd.rc.bottom;
//              wCoord [COORD_MAIN].w = nW;
//              wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
//              }
//          else
//              {
//              wCoord [COORD_MAIN].x = abd.rc.right;
//              wCoord [COORD_MAIN].y = 0;
//              wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
//              wCoord [COORD_MAIN].h = nH;
//              }
//          wCoord [COORD_MAIN].s = WIN_SHOW;
//          }
//      }
//  else
//      {
//      wp.length = sizeof (WINDOWPLACEMENT);
//      GetWindowPlacement (hwndWindow [HWND_FRAME], &wp);
//
//      wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
//      wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
//      wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
//      wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1;
//      wCoord [COORD_MAIN].s = WIN_SHOW;
//      }
//
//  MoveWindow (hwndWindow [HWND_FRAME],
//              wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
//              wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
//              TRUE);
//
//  GetClientRect (hwndWindow [HWND_FRAME], &rc);
//
//  nW = (rc.right  - rc.left) + 1;
//  nH = (rc.bottom - rc.top ) + 1;
//
//  wp.length = sizeof (WINDOWPLACEMENT);
//  GetWindowPlacement (hwndWindow [HWND_FRAME], &wp);
//
//  nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
//  nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top ) + 1;
//
//  //
//  // button
//  //
//  wCoord [COORD_BUTTON].s = WIN_SHOW;
//
//  if (IsIconic (hwndWindow [HWND_BUTTON]) || IsZoomed (hwndWindow [HWND_BUTTON]))
//      {
//      ShowWindow (hwndWindow [HWND_BUTTON], SW_RESTORE);
//      }
//
//  wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
//  wCoord [COORD_BUTTON].y = 0;
//  wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
//  wCoord [COORD_BUTTON].h = (460 * nH1) / nOH;
//  wCoord [COORD_BUTTON].s = WIN_HIDE;
//
//  //
//  // telnet, play, observe #1, #2, #3, #4, #5, and #6
//  //
//  if (bPlayOnLeft)
//      {
//      wCoord [COORD_TELNET].x = (678 * nW1) / nOW;
//      wCoord [COORD_TELNET].y = 0;
//      wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
//      wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
//      wCoord [COORD_TELNET].s = WIN_SHOW;
//
//      wCoord [COORD_PLAY].x = 0;
//      wCoord [COORD_PLAY].y = 0;
//      wCoord [COORD_PLAY].w = wCoord [COORD_TELNET].x / 3;
//      wCoord [COORD_PLAY].h = nH >> 1;
//      wCoord [COORD_PLAY].s = WIN_SHOW;
//
//      wCoord [1].x = wCoord [COORD_PLAY].x;
//      wCoord [1].y = wCoord [COORD_PLAY].y;
//      wCoord [1].w = wCoord [COORD_PLAY].w;
//      wCoord [1].h = wCoord [COORD_PLAY].h;
//      wCoord [1].s = WIN_SHOW;
//
//      wCoord [2].x = wCoord [1].w;
//      wCoord [2].y = wCoord [1].y;
//      wCoord [2].w = wCoord [1].w;
//      wCoord [2].h = wCoord [1].h;
//      wCoord [2].s = WIN_SHOW;
//
//      wCoord [3].x = wCoord [2].x + wCoord [2].w;
//      wCoord [3].y = wCoord [1].y;
//      wCoord [3].w = wCoord [1].w;
//      wCoord [3].h = wCoord [1].h;
//      wCoord [3].s = WIN_SHOW;
//
//      wCoord [4].x = wCoord [1].x;
//      wCoord [4].y = wCoord [1].h;
//      wCoord [4].w = wCoord [1].w;
//      wCoord [4].h = wCoord [1].h;
//      wCoord [4].s = WIN_SHOW;
//
//      wCoord [5].x = wCoord [2].x;
//      wCoord [5].y = wCoord [4].y;
//      wCoord [5].w = wCoord [2].w;
//      wCoord [5].h = wCoord [2].h;
//      wCoord [5].s = WIN_SHOW;
//
//      wCoord [6].x = wCoord [3].x;
//      wCoord [6].y = wCoord [4].y;
//      wCoord [6].w = wCoord [3].w;
//      wCoord [6].h = wCoord [3].h;
//      wCoord [6].s = WIN_SHOW;
//      }
//  else
//      {
//      wCoord [COORD_TELNET].x = 0;
//      wCoord [COORD_TELNET].y = 0;
//      wCoord [COORD_TELNET].w = nW - ((678 * nW1) / nOW);
//      wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
//      wCoord [COORD_TELNET].s = WIN_SHOW;
//
//      wCoord [COORD_PLAY].w = (nW - wCoord [COORD_TELNET].w) / 3;
//      wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
//      wCoord [COORD_PLAY].y = 0;
//      wCoord [COORD_PLAY].h = nH >> 1;
//      wCoord [COORD_PLAY].s = WIN_SHOW;
//
//      wCoord [1].x = wCoord [COORD_PLAY].x;
//      wCoord [1].y = wCoord [COORD_PLAY].y;
//      wCoord [1].w = wCoord [COORD_PLAY].w;
//      wCoord [1].h = wCoord [COORD_PLAY].h;
//      wCoord [1].s = WIN_SHOW;
//
//      wCoord [2].x = wCoord [1].x - wCoord [1].w;
//      wCoord [2].y = wCoord [1].y;
//      wCoord [2].w = wCoord [1].w;
//      wCoord [2].h = wCoord [1].h;
//      wCoord [2].s = WIN_SHOW;
//
//      wCoord [3].x = (wCoord [2].x - wCoord [2].w) + 1;
//      wCoord [3].y = wCoord [1].y;
//      wCoord [3].w = wCoord [1].w - 1;
//      wCoord [3].h = wCoord [1].h;
//      wCoord [3].s = WIN_SHOW;
//
//      wCoord [4].x = wCoord [1].x;
//      wCoord [4].y = wCoord [1].h;
//      wCoord [4].w = wCoord [1].w;
//      wCoord [4].h = wCoord [1].h;
//      wCoord [4].s = WIN_SHOW;
//
//      wCoord [5].x = wCoord [2].x;
//      wCoord [5].y = wCoord [4].y;
//      wCoord [5].w = wCoord [2].w;
//      wCoord [5].h = wCoord [2].h;
//      wCoord [5].s = WIN_SHOW;
//
//      wCoord [6].x = wCoord [3].x;
//      wCoord [6].y = wCoord [4].y;
//      wCoord [6].w = wCoord [3].w;
//      wCoord [6].h = wCoord [3].h;
//      wCoord [6].s = WIN_SHOW;
//      }
//
//  //
//  // rest of observe (alternate observe 1, 2, 3, 4, 5, and 6)
//  //
//  for (nI = 7 ; nI < MAX_GAME ; nI++)
//      {
//      nJ = (nI % 6);
//
//      wCoord [nI].x = wCoord [nJ].x;
//      wCoord [nI].y = wCoord [nJ].y;
//      wCoord [nI].w = wCoord [nJ].w;
//      wCoord [nI].h = wCoord [nJ].h;
//      wCoord [nI].s = WIN_SHOW;
//      }
//
//  wPartner.x = wCoord [1].x;
//  wPartner.y = wCoord [1].y;
//  wPartner.w = wCoord [1].w;
//  wPartner.h = wCoord [1].h;
//  wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_14(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 740;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	//
	// play, telnet, and observe #1
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_BUTTON].x = 0;
		wCoord [COORD_BUTTON].y = 0;
		wCoord [COORD_BUTTON].w = (151 * nW1) / nOW;
		wCoord [COORD_BUTTON].h = nH - 1;
		wCoord [COORD_BUTTON].s = WIN_SHOW;

		wCoord [COORD_PLAY].x = wCoord [COORD_BUTTON].w;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (573 * nW1) / nOW;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_TELNET].x = wCoord [COORD_PLAY].x + wCoord [COORD_PLAY].w;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
		wCoord [COORD_TELNET].h = nH - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}
	else
	{
		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = (292 * nW1) / nOW;
		wCoord [COORD_TELNET].h = nH - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].x = wCoord [COORD_TELNET].w;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (573 * nW1) / nOW;
		wCoord [COORD_PLAY].h = nH - 1;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [COORD_BUTTON].x = wCoord [COORD_PLAY].x + wCoord [COORD_PLAY].w;
		wCoord [COORD_BUTTON].y = 0;
		wCoord [COORD_BUTTON].w = nW - wCoord [COORD_BUTTON].x - 1;
		wCoord [COORD_BUTTON].h = nH - 1;
		wCoord [COORD_BUTTON].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;
	}

	//
	// rest of observe
	//
	for(nI = 2 ; nI < MAX_GAME ; nI++)
	{
		wCoord [nI].x = wCoord [1].x;
		wCoord [nI].y = wCoord [1].y;
		wCoord [nI].w = wCoord [1].w;
		wCoord [nI].h = wCoord [1].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_Try_15(int bMaxScreen, int bOverlapTaskbar, int bPlayOnLeft)
{
	RECT rc;
	APPBARDATA abd;
	WINDOWPLACEMENT wp;
	int nOW, nOH, nW, nH, nW1, nH1, nI, nJ;

	System.bMaximizeMain = 0;

	//
	// original screen size
	//
	nOW = 1024;
	nOH = 767;

	//
	// main
	//
	if(bMaxScreen)
	{
		nW = GetSystemMetrics(SM_CXSCREEN);
		nH = GetSystemMetrics(SM_CYSCREEN);

		if(bOverlapTaskbar)
		{
			wCoord [COORD_MAIN].x = 0;
			wCoord [COORD_MAIN].y = 0;
			wCoord [COORD_MAIN].w = nW;
			wCoord [COORD_MAIN].h = nH;
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
		else
		{
			nW1 = nW >> 1;
			nH1 = nH >> 1;

			abd.cbSize = sizeof(APPBARDATA);
			SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

			if(abd.rc.left > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = abd.rc.left;
				wCoord [COORD_MAIN].h = nH;
			}
			else if(abd.rc.top > nH1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = abd.rc.top;
			}
			else if(abd.rc.right > nW1)
			{
				wCoord [COORD_MAIN].x = 0;
				wCoord [COORD_MAIN].y = abd.rc.bottom;
				wCoord [COORD_MAIN].w = nW;
				wCoord [COORD_MAIN].h = nH - wCoord [COORD_MAIN].y;
			}
			else
			{
				wCoord [COORD_MAIN].x = abd.rc.right;
				wCoord [COORD_MAIN].y = 0;
				wCoord [COORD_MAIN].w = nW - wCoord [COORD_MAIN].x;
				wCoord [COORD_MAIN].h = nH;
			}
			wCoord [COORD_MAIN].s = WIN_SHOW;
		}
	}
	else
	{
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

		wCoord [COORD_MAIN].x = wp.rcNormalPosition.left;
		wCoord [COORD_MAIN].y = wp.rcNormalPosition.top;
		wCoord [COORD_MAIN].w = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
		wCoord [COORD_MAIN].h = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;
		wCoord [COORD_MAIN].s = WIN_SHOW;
	}

	MoveWindow(hwndWindow [HWND_FRAME],
			   wCoord [COORD_MAIN].x, wCoord [COORD_MAIN].y,
			   wCoord [COORD_MAIN].w, wCoord [COORD_MAIN].h,
			   TRUE);

	GetClientRect(hwndWindow [HWND_FRAME], &rc);

	nW = (rc.right  - rc.left) + 1;
	nH = (rc.bottom - rc.top) + 1;

	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwndWindow [HWND_FRAME], &wp);

	nW1 = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left) + 1;
	nH1 = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top) + 1;

	//
	// button
	//
	wCoord [COORD_BUTTON].s = WIN_SHOW;

	if(IsIconic(hwndWindow [HWND_BUTTON]) || IsZoomed(hwndWindow [HWND_BUTTON]))
	{
		ShowWindow(hwndWindow [HWND_BUTTON], SW_RESTORE);
	}

	wCoord [COORD_BUTTON].x = (435 * nW1) / nOW;
	wCoord [COORD_BUTTON].y = 0;
	wCoord [COORD_BUTTON].w = (145 * nW1) / nOW;
	wCoord [COORD_BUTTON].h = (478 * nH1) / nOH;
	wCoord [COORD_BUTTON].s = WIN_HIDE;

	//
	// telnet, play, observe #1, #2, #3, and #4
	//
	if(bPlayOnLeft)
	{
		wCoord [COORD_TELNET].x = (700 * nW1) / nOW;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = nW - wCoord [COORD_TELNET].x - 1;
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].x = 0;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].w = (nW - wCoord [COORD_TELNET].w) / 4;
		wCoord [COORD_PLAY].h = (nH - wCoord [COORD_TELNET].y - 1) / 2;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = wCoord [2].x + wCoord [2].w;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = wCoord [1].w;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [3].x + wCoord [3].w;
		wCoord [4].y = wCoord [3].y;
		wCoord [4].w = wCoord [1].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;

		wCoord [5].x = wCoord [COORD_PLAY].x;
		wCoord [5].y = wCoord [COORD_PLAY].h;
		wCoord [5].w = wCoord [COORD_PLAY].w;
		wCoord [5].h = wCoord [COORD_PLAY].h;
		wCoord [5].s = WIN_SHOW;

		wCoord [6].x = wCoord [1].w;
		wCoord [6].y = wCoord [1].h;
		wCoord [6].w = wCoord [1].w;
		wCoord [6].h = wCoord [1].h;
		wCoord [6].s = WIN_SHOW;

		wCoord [7].x = wCoord [6].x + wCoord [6].w;
		wCoord [7].y = wCoord [1].h;
		wCoord [7].w = wCoord [1].w;
		wCoord [7].h = wCoord [1].h;
		wCoord [7].s = WIN_SHOW;

		wCoord [8].x = wCoord [7].x + wCoord [7].w;
		wCoord [8].y = wCoord [1].h;
		wCoord [8].w = wCoord [1].w;
		wCoord [8].h = wCoord [1].h;
		wCoord [8].s = WIN_SHOW;

	}
	else
	{
		wCoord [COORD_TELNET].x = 0;
		wCoord [COORD_TELNET].y = 0;
		wCoord [COORD_TELNET].w = nW - ((700 * nW1) / nOW);
		wCoord [COORD_TELNET].h = nH - wCoord [COORD_TELNET].y - 1;
		wCoord [COORD_TELNET].s = WIN_SHOW;

		wCoord [COORD_PLAY].w = (nW - wCoord [COORD_TELNET].w) / 4;
		wCoord [COORD_PLAY].x = nW - wCoord [COORD_PLAY].w - 1;
		wCoord [COORD_PLAY].y = 0;
		wCoord [COORD_PLAY].h = (nH - wCoord [COORD_TELNET].y - 1) / 2;;
		wCoord [COORD_PLAY].s = WIN_SHOW;

		wCoord [1].x = wCoord [COORD_PLAY].x;
		wCoord [1].y = wCoord [COORD_PLAY].y;
		wCoord [1].w = wCoord [COORD_PLAY].w;
		wCoord [1].h = wCoord [COORD_PLAY].h;
		wCoord [1].s = WIN_SHOW;

		wCoord [2].x = wCoord [1].x - wCoord [COORD_PLAY].w;
		wCoord [2].y = wCoord [1].y;
		wCoord [2].w = wCoord [1].w;
		wCoord [2].h = wCoord [1].h;
		wCoord [2].s = WIN_SHOW;

		wCoord [3].x = wCoord [2].x - wCoord [COORD_PLAY].w;
		wCoord [3].y = wCoord [1].y;
		wCoord [3].w = wCoord [1].w;
		wCoord [3].h = wCoord [1].h;
		wCoord [3].s = WIN_SHOW;

		wCoord [4].x = wCoord [3].x - wCoord [COORD_PLAY].w;
		wCoord [4].y = wCoord [3].y;
		wCoord [4].w = wCoord [2].w;
		wCoord [4].h = wCoord [1].h;
		wCoord [4].s = WIN_SHOW;

		wCoord [5].x = wCoord [COORD_PLAY].x;
		wCoord [5].y = wCoord [COORD_PLAY].h;
		wCoord [5].w = wCoord [COORD_PLAY].w;
		wCoord [5].h = wCoord [COORD_PLAY].h;
		wCoord [5].s = WIN_SHOW;

		wCoord [6].x = wCoord [5].x - wCoord [COORD_PLAY].w;
		wCoord [6].y = wCoord [1].h;
		wCoord [6].w = wCoord [1].w;
		wCoord [6].h = wCoord [1].h;
		wCoord [6].s = WIN_SHOW;

		wCoord [7].x = wCoord [6].x - wCoord [COORD_PLAY].w;
		wCoord [7].y = wCoord [1].h;
		wCoord [7].w = wCoord [1].w;
		wCoord [7].h = wCoord [1].h;
		wCoord [7].s = WIN_SHOW;

		wCoord [8].x = wCoord [7].x - wCoord [COORD_PLAY].w;
		wCoord [8].y = wCoord [1].h;
		wCoord [8].w = wCoord [1].w;
		wCoord [8].h = wCoord [1].h;
		wCoord [8].s = WIN_SHOW;
	}

	//
	// rest of observe
	//
	for(nI = 9 ; nI < MAX_GAME ; nI++)
	{
		nJ = (nI % 4);

		wCoord [nI].x = wCoord [nJ].x;
		wCoord [nI].y = wCoord [nJ].y;
		wCoord [nI].w = wCoord [nJ].w;
		wCoord [nI].h = wCoord [nJ].h;
		wCoord [nI].s = WIN_SHOW;
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;
}

void LAYOUT_ProfileRestore(char *cFn)
{
	char Drive [_MAX_DRIVE];
	char Dir   [_MAX_DIR];
	char File  [_MAX_FNAME];
	char Ext   [_MAX_EXT];
	char Fn    [_MAX_PATH];
	char cTmp  [_MAX_PATH];

	int nI, nV;

	_splitpath(cFn, Drive, Dir, File, Ext);
	strcpy(Ext, LAYOUT_EXT);
	_makepath(Fn, Drive, Dir, File, Ext);

	TOOLBOX_ReadINICoord(&wCoord [COORD_MAIN], Fn, "Main", "Rect");

	System.bMaximizeMain = 0;
	nV = GetPrivateProfileInt("Main", "Maximize", System.bMaximizeMain, Fn);
	System.bMaximizeMain = (nV ? 1 : 0);

	TOOLBOX_ReadINICoord(&wCoord [COORD_PLAY], Fn, "Board", "Rect");

	nV = GetPrivateProfileInt("Board", "State",  wCoord [COORD_PLAY].s, Fn);
	wCoord [COORD_PLAY].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	TOOLBOX_ReadINICoord(&wCoord [COORD_TELNET], Fn, "Telnet", "Rect");

	nV = GetPrivateProfileInt("Telnet", "State", wCoord [COORD_TELNET].s, Fn);
	wCoord [COORD_TELNET].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	TOOLBOX_ReadINICoord(&wCoord [COORD_BUTTON], Fn, "Button", "Rect");

	nV = GetPrivateProfileInt("Button", "State", wCoord [COORD_BUTTON].s, Fn);
	wCoord [COORD_BUTTON].s = (nV == WIN_HIDE ? WIN_HIDE : WIN_SHOW);

	for(nI = 1 ; nI < MAX_GAME ; nI++)
	{
		sprintf(cTmp, "Rect %d", nI);
		TOOLBOX_ReadINICoord(&wCoord [nI], Fn, "Observe", cTmp);
	}

	wPartner.x = wCoord [1].x;
	wPartner.y = wCoord [1].y;
	wPartner.w = wCoord [1].w;
	wPartner.h = wCoord [1].h;
	wPartner.s = wCoord [1].s;

	TOOLBOX_CheckAllMenu();

	System.bOkToResizePlay = 1;
	TOOLBOX_TryAllWindowCoord();
//    TOOLBOX_LoadAllWindowCoord ();
	System.bOkToResizePlay = 0;
}
