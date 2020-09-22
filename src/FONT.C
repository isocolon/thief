#include "thief.h"

void FONT_Init(void)
{
	FONT_Init_Handle() ;
	FONT_Init_Clock() ;
	FONT_Init_GameType() ;
	FONT_Init_LastMove() ;
	FONT_Init_LagStat() ;
	FONT_Init_Result() ;
	FONT_Init_Telnet() ;
	FONT_Init_Button() ;
	FONT_Init_Coordinates() ;
}

void FONT_Init_Handle(void)
{
	strcpy(hfFont [FONT_HANDLE].Name, "Times New Roman") ;
	hfFont [FONT_HANDLE].nPointSize = 12 ;
	hfFont [FONT_HANDLE].bBold      = 1  ;
	hfFont [FONT_HANDLE].bItalic    = 0  ;
	hfFont [FONT_HANDLE].bUnderLine = 0  ;
	hfFont [FONT_HANDLE].bStrikeOut = 0  ;
}

void FONT_Init_Clock(void)
{
	strcpy(hfFont [FONT_CLOCK].Name, "Times New Roman") ;
	hfFont [FONT_CLOCK].nPointSize = 16 ;
	hfFont [FONT_CLOCK].bBold      = 0  ;
	hfFont [FONT_CLOCK].bItalic    = 0  ;
	hfFont [FONT_CLOCK].bUnderLine = 0  ;
	hfFont [FONT_CLOCK].bStrikeOut = 0  ;
}

void FONT_Init_GameType(void)
{
	strcpy(hfFont [FONT_GAMETYPE].Name, "Times New Roman") ;
	hfFont [FONT_GAMETYPE].nPointSize = 12 ;
	hfFont [FONT_GAMETYPE].bBold      = 1  ;
	hfFont [FONT_GAMETYPE].bItalic    = 0  ;
	hfFont [FONT_GAMETYPE].bUnderLine = 0  ;
	hfFont [FONT_GAMETYPE].bStrikeOut = 0  ;
}

void FONT_Init_LastMove(void)
{
	strcpy(hfFont [FONT_LASTMOVE].Name, "Times New Roman") ;
	hfFont [FONT_LASTMOVE].nPointSize = 11 ;
	hfFont [FONT_LASTMOVE].bBold      = 0  ;
	hfFont [FONT_LASTMOVE].bItalic    = 0  ;
	hfFont [FONT_LASTMOVE].bUnderLine = 0  ;
	hfFont [FONT_LASTMOVE].bStrikeOut = 0  ;
}

void FONT_Init_LagStat(void)
{
	strcpy(hfFont [FONT_LAGSTAT].Name, "Times New Roman") ;
	hfFont [FONT_LAGSTAT].nPointSize = 11 ;
	hfFont [FONT_LAGSTAT].bBold      = 0  ;
	hfFont [FONT_LAGSTAT].bItalic    = 0  ;
	hfFont [FONT_LAGSTAT].bUnderLine = 0  ;
	hfFont [FONT_LAGSTAT].bStrikeOut = 0  ;
}

void FONT_Init_Result(void)
{
	strcpy(hfFont [FONT_RESULT].Name, "Times New Roman") ;
	hfFont [FONT_RESULT].nPointSize = 11 ;
	hfFont [FONT_RESULT].bBold      = 0  ;
	hfFont [FONT_RESULT].bItalic    = 0  ;
	hfFont [FONT_RESULT].bUnderLine = 0  ;
	hfFont [FONT_RESULT].bStrikeOut = 0  ;
}

void FONT_Init_Coordinates(void)
{
	strcpy(hfFont [FONT_COORDINATES].Name, "Times New Roman") ;
	hfFont [FONT_COORDINATES].nPointSize = 11 ;
	hfFont [FONT_COORDINATES].bBold      = 0  ;
	hfFont [FONT_COORDINATES].bItalic    = 0  ;
	hfFont [FONT_COORDINATES].bUnderLine = 0  ;
	hfFont [FONT_COORDINATES].bStrikeOut = 0  ;
}

void FONT_Init_Telnet(void)
{
	strcpy(hfFont [FONT_TELNET].Name, "Fixedsys") ;
	hfFont [FONT_TELNET].nPointSize = 9 ;
	hfFont [FONT_TELNET].bBold      = 0 ;
	hfFont [FONT_TELNET].bItalic    = 0 ;
	hfFont [FONT_TELNET].bUnderLine = 0 ;
	hfFont [FONT_TELNET].bStrikeOut = 0 ;
}

void FONT_Init_Telnet1(void)
{
	strcpy(hfFont [FONT_TELNET].Name, "Courier New") ;
	hfFont [FONT_TELNET].nPointSize = 9 ;
	hfFont [FONT_TELNET].bBold      = 0 ;
	hfFont [FONT_TELNET].bItalic    = 0 ;
	hfFont [FONT_TELNET].bUnderLine = 0 ;
	hfFont [FONT_TELNET].bStrikeOut = 0 ;
}

void FONT_Init_Telnet2(void)
{
	strcpy(hfFont [FONT_TELNET].Name, "Courier New") ;
	hfFont [FONT_TELNET].nPointSize = 9 ;
	hfFont [FONT_TELNET].bBold      = 0 ;
	hfFont [FONT_TELNET].bItalic    = 0 ;
	hfFont [FONT_TELNET].bUnderLine = 0 ;
	hfFont [FONT_TELNET].bStrikeOut = 0 ;
}

void FONT_Init_Telnet3(void)
{
	strcpy(hfFont [FONT_TELNET].Name, "Arial") ;
	hfFont [FONT_TELNET].nPointSize = 14 ;
	hfFont [FONT_TELNET].bBold      = 0 ;
	hfFont [FONT_TELNET].bItalic    = 0 ;
	hfFont [FONT_TELNET].bUnderLine = 0 ;
	hfFont [FONT_TELNET].bStrikeOut = 0 ;
}

void FONT_Init_Button(void)
{
	strcpy(hfFont [FONT_BUTTON].Name, "Times New Roman") ;
	hfFont [FONT_BUTTON].nPointSize = 11 ;
	hfFont [FONT_BUTTON].bBold      = 0  ;
	hfFont [FONT_BUTTON].bItalic    = 0  ;
	hfFont [FONT_BUTTON].bUnderLine = 0  ;
	hfFont [FONT_BUTTON].bStrikeOut = 0  ;
}

void FONT_CreateFont(int nI, char *cName, int nPointSize, int bBold,
					 int bItalic, int bUnderLine, int bStrikeOut)
{
	HDC hdc ;

	strcpy(hfFont [nI].lf.lfFaceName, cName) ;

	hdc = CreateDC("DISPLAY", NULL, NULL, NULL) ;
	hfFont [nI].lf.lfHeight = - (int)(nPointSize * GetDeviceCaps(hdc, LOGPIXELSY) / 72.0 + 0.5) ;
	DeleteDC(hdc) ;

	hfFont [nI].lf.lfWidth          = 0 ;
	hfFont [nI].lf.lfEscapement     = 0 ;
	hfFont [nI].lf.lfOrientation    = 0 ;
	hfFont [nI].lf.lfWeight         = bBold ? 700 : 0 ;
	hfFont [nI].lf.lfItalic         = bItalic ;
	hfFont [nI].lf.lfUnderline      = bUnderLine ;
	hfFont [nI].lf.lfStrikeOut      = bStrikeOut ;
	hfFont [nI].lf.lfCharSet        = DEFAULT_CHARSET ;
	hfFont [nI].lf.lfOutPrecision   = OUT_DEFAULT_PRECIS ;
	hfFont [nI].lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
	hfFont [nI].lf.lfQuality        = DEFAULT_QUALITY ;
	hfFont [nI].lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE ;

	hfFont [nI].hfFont = CreateFontIndirect(&hfFont [nI].lf) ;
}

BOOL FONT_ChooseFont(HWND hwnd, int nI)
{
	HDC        hdc ;
	LOGFONT    lf ;
	CHOOSEFONT cf ;

	strcpy(lf.lfFaceName, hfFont [nI].Name) ;

	hdc = CreateDC("DISPLAY", NULL, NULL, NULL) ;
	lf.lfHeight = - (int)(hfFont [nI].nPointSize * GetDeviceCaps(hdc, LOGPIXELSY) / 72.0 + 0.5) ;
	DeleteDC(hdc) ;

	lf.lfWidth          = 0 ;
	lf.lfEscapement     = 0 ;
	lf.lfOrientation    = 0 ;
	lf.lfWeight         = hfFont [nI].bBold ? 700 : 0 ;
	lf.lfItalic         = hfFont [nI].bItalic ;
	lf.lfUnderline      = hfFont [nI].bUnderLine ;
	lf.lfStrikeOut      = hfFont [nI].bStrikeOut ;
	lf.lfCharSet        = DEFAULT_CHARSET ;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS ;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
	lf.lfQuality        = DEFAULT_QUALITY ;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE ;

	cf.lStructSize      = sizeof(CHOOSEFONT) ;
	cf.hwndOwner        = hwnd ;
	cf.hDC              = NULL ;
	cf.lpLogFont        = &lf ;
	cf.iPointSize       = 0 ;
	cf.Flags            = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS
						  | CF_EFFECTS ;
	cf.rgbColors        = 0L ;
	cf.lCustData        = 0L ;
	cf.lpfnHook         = NULL ;
	cf.lpTemplateName   = NULL ;
	cf.hInstance        = NULL ;
	cf.lpszStyle        = NULL ;
	cf.nFontType        = 0 ;               // Returned from ChooseFont
	cf.nSizeMin         = 0 ;
	cf.nSizeMax         = 0 ;

	if(! ChooseFont(&cf))
	{
		SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
		return FALSE ;
	}

	strcpy(hfFont [nI].lf.lfFaceName, lf.lfFaceName) ;

	hfFont[nI].lf.lfHeight         = lf.lfHeight ;
	hfFont[nI].lf.lfWidth          = lf.lfWidth ;
	hfFont[nI].lf.lfEscapement     = lf.lfEscapement ;
	hfFont[nI].lf.lfOrientation    = lf.lfOrientation ;
	hfFont[nI].lf.lfWeight         = lf.lfWeight ;
	hfFont[nI].lf.lfItalic         = lf.lfItalic ;
	hfFont[nI].lf.lfUnderline      = lf.lfUnderline ;
	hfFont[nI].lf.lfStrikeOut      = lf.lfStrikeOut ;
	hfFont[nI].lf.lfCharSet        = lf.lfCharSet ;
	hfFont[nI].lf.lfOutPrecision   = lf.lfOutPrecision ;
	hfFont[nI].lf.lfClipPrecision  = lf.lfClipPrecision ;
	hfFont[nI].lf.lfQuality        = lf.lfQuality ;
	hfFont[nI].lf.lfPitchAndFamily = lf.lfPitchAndFamily ;

	strcpy(hfFont [nI].Name, hfFont [nI].lf.lfFaceName) ;

	hdc = CreateDC("DISPLAY", NULL, NULL, NULL) ;
	hfFont [nI].nPointSize = (int)(((- hfFont [nI].lf.lfHeight) + 0.5) * 72 / GetDeviceCaps(hdc, LOGPIXELSY)) ;
	DeleteDC(hdc) ;

	hfFont [nI].bBold      = (hfFont [nI].lf.lfWeight == 700) ? 1 : 0 ;
	hfFont [nI].bItalic    = hfFont [nI].lf.lfItalic ? 1 : 0 ;
	hfFont [nI].bUnderLine = hfFont [nI].lf.lfUnderline ? 1 : 0 ;
	hfFont [nI].bStrikeOut = hfFont [nI].lf.lfStrikeOut ? 1 : 0 ;

	SetFocus(hwndWindow [HWND_TELNET_EDIT]) ;
	return TRUE ;
}

void FONT_CreateFont1(FONTRECORD *fr, char *cName, int nPointSize, int bBold, int bItalic, int bUnderLine, int bStrikeOut)
{
	HDC hdc ;

	strcpy(fr->lf.lfFaceName, cName) ;

	hdc = CreateDC("DISPLAY", NULL, NULL, NULL) ;
	fr->lf.lfHeight = - (int)(nPointSize * GetDeviceCaps(hdc, LOGPIXELSY) / 72.0 + 0.5) ;
	DeleteDC(hdc) ;

	fr->lf.lfWidth          = 0 ;
	fr->lf.lfEscapement     = 0 ;
	fr->lf.lfOrientation    = 0 ;
	fr->lf.lfWeight         = bBold ? 700 : 0 ;
	fr->lf.lfItalic         = bItalic ;
	fr->lf.lfUnderline      = bUnderLine ;
	fr->lf.lfStrikeOut      = bStrikeOut ;
	fr->lf.lfCharSet        = DEFAULT_CHARSET ;
	fr->lf.lfOutPrecision   = OUT_DEFAULT_PRECIS ;
	fr->lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
	fr->lf.lfQuality        = DEFAULT_QUALITY ;
	fr->lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE ;

	fr->hfFont = CreateFontIndirect(&fr->lf) ;
}

BOOL FONT_ChooseFont1(HWND hwnd, FONTRECORD *fr)
{
	HDC        hdc ;
	LOGFONT    lf ;
	CHOOSEFONT cf ;

	strcpy(lf.lfFaceName, fr->Name) ;

	hdc = CreateDC("DISPLAY", NULL, NULL, NULL) ;
	lf.lfHeight = - (int)(fr->nPointSize * GetDeviceCaps(hdc, LOGPIXELSY) / 72.0 + 0.5) ;
	DeleteDC(hdc) ;

	lf.lfWidth          = 0 ;
	lf.lfEscapement     = 0 ;
	lf.lfOrientation    = 0 ;
	lf.lfWeight         = fr->bBold ? 700 : 0 ;
	lf.lfItalic         = fr->bItalic ;
	lf.lfUnderline      = fr->bUnderLine ;
	lf.lfStrikeOut      = fr->bStrikeOut ;
	lf.lfCharSet        = DEFAULT_CHARSET ;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS ;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
	lf.lfQuality        = DEFAULT_QUALITY ;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE ;

	cf.lStructSize      = sizeof(CHOOSEFONT) ;
	cf.hwndOwner        = hwnd ;
	cf.hDC              = NULL ;
	cf.lpLogFont        = &lf ;
	cf.iPointSize       = 0 ;
	cf.Flags            = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS
						  | CF_EFFECTS ;
	cf.rgbColors        = 0L ;
	cf.lCustData        = 0L ;
	cf.lpfnHook         = NULL ;
	cf.lpTemplateName   = NULL ;
	cf.hInstance        = NULL ;
	cf.lpszStyle        = NULL ;
	cf.nFontType        = 0 ;               // Returned from ChooseFont
	cf.nSizeMin         = 0 ;
	cf.nSizeMax         = 0 ;

	if(! ChooseFont(&cf))
	{
		return FALSE ;
	}

	strcpy(fr->lf.lfFaceName, lf.lfFaceName) ;

	fr->lf.lfHeight         = lf.lfHeight ;
	fr->lf.lfWidth          = lf.lfWidth ;
	fr->lf.lfEscapement     = lf.lfEscapement ;
	fr->lf.lfOrientation    = lf.lfOrientation ;
	fr->lf.lfWeight         = lf.lfWeight ;
	fr->lf.lfItalic         = lf.lfItalic ;
	fr->lf.lfUnderline      = lf.lfUnderline ;
	fr->lf.lfStrikeOut      = lf.lfStrikeOut ;
	fr->lf.lfCharSet        = lf.lfCharSet ;
	fr->lf.lfOutPrecision   = lf.lfOutPrecision ;
	fr->lf.lfClipPrecision  = lf.lfClipPrecision ;
	fr->lf.lfQuality        = lf.lfQuality ;
	fr->lf.lfPitchAndFamily = lf.lfPitchAndFamily ;

	strcpy(fr->Name, fr->lf.lfFaceName) ;

	hdc = CreateDC("DISPLAY", NULL, NULL, NULL) ;
	fr->nPointSize = (int)(((- fr->lf.lfHeight) + 0.5) * 72 / GetDeviceCaps(hdc, LOGPIXELSY)) ;
	DeleteDC(hdc) ;

	fr->bBold      = (fr->lf.lfWeight == 700) ? 1 : 0 ;
	fr->bItalic    = fr->lf.lfItalic ? 1 : 0 ;
	fr->bUnderLine = fr->lf.lfUnderline ? 1 : 0 ;
	fr->bStrikeOut = fr->lf.lfStrikeOut ? 1 : 0 ;

	return TRUE ;
}

void FONT_Copy_Info(FONTRECORD *frd, FONTRECORD *frs)
{
	strcpy(frd->Name, frs->Name) ;
	frd->nPointSize = frs->nPointSize ;
	frd->bBold      = frs->bBold      ;
	frd->bItalic    = frs->bItalic    ;
	frd->bUnderLine = frs->bUnderLine ;
	frd->bStrikeOut = frs->bStrikeOut ;
}
