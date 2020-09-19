#include "thief.h"

void COLOR_Init (void)
    {
    int nI ;

    COLOR_Init_Red () ;
    COLOR_Init_Board () ;
    COLOR_Init_Handle () ;
    COLOR_Init_Clock () ;
    COLOR_Init_LagStat () ;
    COLOR_Init_GameType () ;
    COLOR_Init_LastMove () ;
    COLOR_Init_Highlight () ;
    COLOR_Init_TruePremove () ;
    COLOR_Init_IllegalTruePremove () ;
    COLOR_Init_KingHighlight () ;
    COLOR_Init_Result () ;
    COLOR_Init_Ptell () ;
    COLOR_Init_GameInfo () ;
    COLOR_Init_Coordinates () ;

    for (nI = 0 ; nI < MAX_CHANNEL_COLOR ; nI++)
        {
        bChannel   [nI] = 0 ;
        clrChannel [nI] = RGB (0x00, 0x00, 0x00) ;
        }

    COLOR_Init_Telnet () ;
    }

void COLOR_Init_Red (void)
    {
    clrRed = RGB (0xff, 0x00, 0x00) ;
    }

void COLOR_Init_Board (void)
    {
    clrColor [CLR_WINDOW_COLOR]         = RGB (0xc0, 0xc0, 0xc0) ;
    clrColor [CLR_BOARD_OUTLINE]        = RGB (0x00, 0x00, 0x00) ;
    clrColor [CLR_BUFFER_OUTLINE]       = RGB (0x00, 0x00, 0x00) ;
    clrColor [CLR_BUFFER_FILL]          = RGB (0xcc, 0xbd, 0xa4) ;
    clrColor [CLR_WHITE_SQUARE_OUTLINE] = RGB (0xcc, 0xbd, 0xa4) ;
    clrColor [CLR_WHITE_SQUARE_FILL]    = RGB (0xcc, 0xbd, 0xa4) ;
    clrColor [CLR_BLACK_SQUARE_OUTLINE] = RGB (0xa8, 0x89, 0x64) ;
    clrColor [CLR_BLACK_SQUARE_FILL]    = RGB (0xa8, 0x89, 0x64) ;
    clrColor [CLR_PIECE_NUMBER]         = RGB (0x00, 0x00, 0x00) ;
    clrColor [CLR_WHITE_PIECE_OUTLINE]  = RGB (0x00, 0x00, 0x00) ;
    clrColor [CLR_WHITE_PIECE_FILL]     = RGB (0xff, 0xff, 0xff) ;
    clrColor [CLR_BLACK_PIECE_OUTLINE]  = RGB (0xd4, 0xd4, 0xd4) ;
    clrColor [CLR_BLACK_PIECE_FILL]     = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_Handle (void)
    {
    clrColor [CLR_HANDLE] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_Clock (void)
    {
    clrColor [CLR_CLOCK_ON]            = RGB (0xff, 0xff, 0xff) ;
    clrColor [CLR_CLOCK_ON_BACKGROUND] = RGB (0x00, 0x00, 0x00) ;
    clrColor [CLR_CLOCK_OFF]           = RGB (0x00, 0x00, 0x00) ;
    clrColor [CLR_CLOCK_OFF]           = RGB (0x00, 0x00, 0x00) ;
	clrColor [CLR_CLOCK_TIMEDOWN]      = RGB (0xc0, 0x00, 0x00) ;
	clrColor [CLR_CLOCK_TIMEUP]        = RGB (0x00, 0xc0, 0x00) ;
    }

void COLOR_Init_LagStat (void)
    {
    clrColor [CLR_LAGSTAT] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_GameType (void)
    {
    clrColor [CLR_GAMETYPE] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_LastMove (void)
    {
    clrColor [CLR_LASTMOVE] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_Highlight (void)
    {
    clrColor [CLR_HIGHLIGHT] = RGB (0x00, 0xff, 0x00) ;
    }

void COLOR_Init_TruePremove (void)
    {
    clrColor [CLR_TRUE_PREMOVE_HIGHLIGHT] = RGB (0xff, 0x00, 0x00) ;
    }

void COLOR_Init_IllegalTruePremove (void)
    {
    clrColor [CLR_ILLEGAL_TRUE_PREMOVE] = RGB (0x00, 0x00, 0xff) ;
    }

void COLOR_Init_KingHighlight (void)
    {
    clrColor [CLR_KING_HIGHLIGHT] = RGB (0xff, 0xff, 0x00) ;
    }

void COLOR_Init_Result (void)
    {
    clrColor [CLR_RESULT_FOREGROUND] = RGB (0xff, 0xff, 0xff) ;
    clrColor [CLR_RESULT_BACKGROUND] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_Ptell (void)
    {
    clrColor [CLR_PTELL_FOREGROUND] = RGB (0xff, 0xff, 0xff) ;
    clrColor [CLR_PTELL_BACKGROUND] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_GameInfo (void)
    {
    clrColor [CLR_GINFO_FOREGROUND] = RGB (0xff, 0xff, 0xff) ;
    clrColor [CLR_GINFO_BACKGROUND] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_Coordinates (void)
    {
    clrColor [CLR_COORDINATES_FOREGROUND] = RGB (0x00, 0x00, 0x00) ;
    }

void COLOR_Init_Telnet (void)
    {
    clrColor [CLR_TELNET_NORMAL]          = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_TELL]            = RGB (0xd8, 0xbb, 0x87) ;
    clrColor [CLR_TELNET_PTELL]           = RGB (0xff, 0xff, 0x00) ;
    clrColor [CLR_TELNET_SAY]             = RGB (0xd8, 0xb6, 0x87) ;
    clrColor [CLR_TELNET_KIBITZ]          = RGB (0x00, 0x80, 0x40) ;
    clrColor [CLR_TELNET_WHISPER]         = RGB (0x00, 0x80, 0x40) ;
    clrColor [CLR_TELNET_SHOUT]           = RGB (0x80, 0x80, 0x00) ;
    clrColor [CLR_TELNET_CSHOUT]          = RGB (0x80, 0x80, 0x00) ;
    clrColor [CLR_TELNET_CHALLENGE]       = RGB (0x00, 0xff, 0xff) ;
    clrColor [CLR_TELNET_USERTEXT]        = RGB (0x00, 0x80, 0xff) ;
    clrColor [CLR_TELNET_BACKGROUND]      = RGB (0x00, 0x00, 0x00) ;
    clrColor [CLR_TELNET_NOTIFICATION]    = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_COLON_NOTIFY]    = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_ARRIVAL]         = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_DEPARTURE]       = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_GAME_NOTIFY]     = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_ANNOUNCEMENT]    = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_COLON]           = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_COLON_256]       = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_COLON_C2]        = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_COLON_T3]        = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_COLON_TSHOUTS]   = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_COLON_MAMER]     = RGB (0xf3, 0x78, 0x49) ;
    clrColor [CLR_TELNET_CHANNEL]         = RGB (0x00, 0x80, 0x40) ;
    clrColor [CLR_TELNET_CHANNELA]        = RGB (0x00, 0x80, 0x87) ;
    clrColor [CLR_TELNET_CHANNEL1]        = RGB (0x00, 0x80, 0x40) ;
    clrColor [CLR_TELNET_CHANNEL24]       = RGB (0xc0, 0xc0, 0xc0) ;
    clrColor [CLR_TELNET_CHANNEL97]       = RGB (0xc0, 0xc0, 0xc0) ;

    bChannel [ 1] = 1 ;
    bChannel [24] = 1 ;
    bChannel [97] = 1 ;

    clrChannel [ 1] = RGB (0x00, 0x80, 0x40) ;
    clrChannel [24] = RGB (0xc0, 0xc0, 0xc0) ;
    clrChannel [97] = RGB (0xc0, 0xc0, 0xc0) ;
    }

BOOL COLOR_ChooseColor (int nI)
    {
    static CHOOSECOLOR cc ;
    static COLORREF    crCustColors[16] ;

    cc.lStructSize    = sizeof (CHOOSECOLOR) ;
    cc.hwndOwner      = hwndWindow [HWND_FRAME] ;
    cc.hInstance      = NULL ;
    cc.rgbResult      = clrColor [nI] ;
    cc.lpCustColors   = crCustColors ;
    cc.Flags          = CC_RGBINIT | CC_FULLOPEN ;
    cc.lCustData      = 0L ;
    cc.lpfnHook       = NULL ;
    cc.lpTemplateName = NULL ;

    if (! ChooseColor (&cc))
        {
        SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
        return FALSE ;
        }

    clrColor [nI] = cc.rgbResult ;

    SetFocus (hwndWindow [HWND_TELNET_EDIT]) ;
    return TRUE ;
    }

BOOL COLOR_ChooseColor1 (HWND hwnd, COLORREF *clr)
    {
    static CHOOSECOLOR cc ;
    static COLORREF    crCustColors[16] ;

    cc.lStructSize    = sizeof (CHOOSECOLOR) ;
    cc.hwndOwner      = hwndWindow [HWND_FRAME] ;
    cc.hInstance      = NULL ;
    cc.rgbResult      = *clr ;
    cc.lpCustColors   = crCustColors ;
    cc.Flags          = CC_RGBINIT | CC_FULLOPEN ;
    cc.lCustData      = 0L ;
    cc.lpfnHook       = NULL ;
    cc.lpTemplateName = NULL ;

    if (! ChooseColor (&cc))
        {
        return FALSE ;
        }

    *clr = cc.rgbResult ;
    return TRUE ;
    }
