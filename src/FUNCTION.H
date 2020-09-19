// about.c
BOOL CALLBACK AboutBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// assist.c
BOOL CALLBACK PartnerAssistBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void ASSIST_Init (void) ;
void ASSIST_Adjust (void) ;

// animate.c
void ANIMATE_Init (void) ;
void ANIMATE_Tween (POINT *, POINT *, POINT *, int, POINT *, int *) ;
int ANIMATE_AnimateMove (int, HWND, HDC, int, int, int, int, int, int) ;
void ANIMATE_Move (int, HWND, HDC, int) ;

// bdraw.c
void BDRAW_LoadPieces (void) ;
void BDRAW_SetSize (int) ;
void BDRAW_WhiteSquare (HDC hdc) ;
void BDRAW_BlackSquare (HDC hdc) ;
void BDRAW_PawnM (HDC) ;
void BDRAW_RookM (HDC) ;
void BDRAW_KnightM (HDC) ;
void BDRAW_BishopM (HDC) ;
void BDRAW_QueenM (HDC) ;
void BDRAW_KingM (HDC) ;
void BDRAW_PieceMask (HDC, int) ;
void BDRAW_SquareW1 (HDC, COLORREF, COLORREF) ;
void BDRAW_PawnW1 (HDC, COLORREF, COLORREF) ;
void BDRAW_RookW1 (HDC, COLORREF, COLORREF) ;
void BDRAW_KnightW1 (HDC, COLORREF, COLORREF) ;
void BDRAW_BishopW1 (HDC, COLORREF, COLORREF) ;
void BDRAW_QueenW1 (HDC, COLORREF, COLORREF) ;
void BDRAW_KingW1 (HDC, COLORREF, COLORREF) ;
void BDRAW_SquareB1 (HDC, COLORREF, COLORREF) ;
void BDRAW_PawnB1 (HDC, COLORREF, COLORREF) ;
void BDRAW_RookB1 (HDC, COLORREF, COLORREF) ;
void BDRAW_KnightB1 (HDC, COLORREF, COLORREF) ;
void BDRAW_BishopB1 (HDC, COLORREF, COLORREF) ;
void BDRAW_QueenB1 (HDC, COLORREF, COLORREF) ;
void BDRAW_KingB1 (HDC, COLORREF, COLORREF) ;
void BDRAW_Piece1 (HDC, int, COLORREF, COLORREF, COLORREF, COLORREF) ;
void BDRAW_SquareBF1 (HDC, COLORREF, COLORREF) ;
void BDRAW_TransparentBlt (HDC, COLORREF) ;

// board.c
int BOARD_FICS_To_Int (char) ;
int BOARD_FICS_To_White_Int (char) ;
int BOARD_FICS_To_Black_Int (char) ;
int BOARD_IsWhitePiece (int) ;
int BOARD_IsBlackPiece (int) ;
int BOARD_IsSameColor (int, int) ;
int BOARD_DetermineColorUpDown (int, int, long) ;
void BOARD_FindKings (int) ;
void BOARD_FindICCWild9Kings (int) ;
void BOARD_UpdatePromoteBoard (int, int, char *) ;
void BOARD_LoadBoard (int, char *, char *) ;
void BOARD_LoadBuffer (int, char *, char *) ;
void BOARD_FindCapturedPieces (int) ;
void BOARD_SaveLastBoardBuffer (int) ;
void BOARD_NullLastBoardBuffer (int) ;
void BOARD_NullHighlight (int) ;
void BOARD_SaveLastHighlight (int) ;
void BOARD_NullLastHighlight (int) ;
void BOARD_ResetPromoteBoard (int) ;
void BOARD_CopyPromoteBoard (int) ;
void BOARD_ResizeNoBuffer (int, HDC, int, int) ;
void BOARD_ResizeNoBuffer1 (int, HDC, int, int) ;
void BOARD_ResizeLeft (int, HDC, int, int) ;
void BOARD_ResizeLeft1 (int, HDC, int, int) ;
void BOARD_ResizeRight (int, HDC, int, int) ;
void BOARD_ResizeRight1 (int, HDC, int, int) ;
void BOARD_ResizeTopBottomL (int, HDC, int, int) ;
void BOARD_ResizeTopBottomL1 (int, HDC, int, int) ;
void BOARD_ResizeTopBottomR (int, HDC, int, int) ;
void BOARD_ResizeTopBottomR1 (int, HDC, int, int) ;
void BOARD_CheckFlip (int) ;
void BOARD_LoadBitmaps (int, HDC) ;
void BOARD_Destroy (int) ;
void BOARD_DrawRepaint (int, HWND, HDC) ;
void BOARD_DrawWindowBackground (HWND, HDC) ;
void BOARD_Draw0 (int, HDC) ;
void BOARD_Draw1 (int, HDC) ;
void BOARD_Draw2 (int, HDC) ;
void BOARD_Draw3 (int, HDC) ;
void BOARD_Draw4 (int, HDC) ;
void BOARD_Draw5 (int, HDC) ;
void BOARD_Draw6 (int, HDC) ;
void BOARD_Draw7 (int, HDC) ;
void BOARD_Draw8 (int, HDC) ;
void BOARD_Draw9 (int, HDC) ;
void BOARD_Draw11 (int, HDC) ;
void BOARD_DrawNumber (int, HDC, int) ;
void BOARD_DrawHandle (int, HDC) ;
void BOARD_DrawWhiteClock (int, HDC) ;
void BOARD_DrawBlackClock (int, HDC) ;
void BOARD_DisplayWhiteClock (int, HWND, long) ;
void BOARD_DisplayBlackClock (int, HWND, long) ;
void BOARD_DrawWhiteLag (int, HDC) ;
void BOARD_DrawBlackLag (int, HDC) ;
void BOARD_DrawGameType (int, HDC) ;
void BOARD_DrawLastMove (int, HDC) ;
void BOARD_DrawLastMove1 (int, HDC) ;
void BOARD_DrawResult (int, HDC) ;
void BOARD_DrawPtell (int, HDC, char *) ;
void BOARD_DrawGinfo (int, HDC, char *) ;
void BOARD_DrawCoordinates (int, HDC) ;
void BOARD_SquareToPosition (int, int, int, int *, int *) ;
void BOARD_DrawHighlight (int, int, int, HDC, int) ;
void BOARD_DrawBoardPiece (int, HDC, int, int, int) ;
void BOARD_DrawBufferPiece (int, HDC, int, int) ;
void BOARD_DrawChessBufferPiece (int, HDC, int, int) ;
void BOARD_DrawEmptyBufferPiece (int, HDC, int) ;
void BOARD_DrawDragPiece (int, HDC) ;
void BOARD_DrawAnimatePiece (int, HDC) ;
void BOARD_DrawFullPosition (int, HWND, HDC) ;
void BOARD_DrawFullPosition1 (int, HWND, HDC) ;
int  BOARD_OnMouseDown (int, HWND, HDC, int, int, int, int, int) ;
void BOARD_OnMouseMove (int, HWND, HDC, int, int) ;
int  BOARD_OnMouseUp (int, HWND, HDC, int, int, int *) ;
void BOARD_RestoreDragMove (int, HDC) ;
int  BOARD_OnMouseDown1 (int, HDC, int, int, int, int, int) ;
void BOARD_OnMouseMove1 (int, HDC, int, int) ;
int  BOARD_OnMouseUp1 (int, HWND, HDC, int, int) ;
void BOARD_GetHighlightFromLastMove (int, char *, char *) ;
void BOARD_ResetTruePremove (void) ;
void BOARD_MakeSmartMove (HDC, int, int, int, int, int) ;
int BOARD_RightMouseDropPiece (int, int, int) ;
int BOARD_RightMouseDropPiece1 (int, int, int, int) ;
void BOARD_DrawBoard (int, HWND, HDC, int) ;
void BOARD_DrawBoard1 (int, HWND, HDC, int) ;
void BOARD_DrawAnimateBoard (int, HWND, HDC) ;
void BOARD_DrawAnimateBoard1 (int, HWND, HDC) ;
void BOARD_DrawMoveButtonBoard (int, int, int, HWND, HDC) ;
void BOARD_DrawMoveButtonBoard1 (int, int, int, HWND, HDC) ;

// board1.c
void BOARD_DrawBoard (int, HWND, HDC, int) ;
void BOARD_DrawBoard1 (int, HWND, HDC, int) ;
void BOARD_DrawAnimateBoard (int, HWND, HDC) ;
void BOARD_DrawAnimateBoard1 (int, HWND, HDC) ;
void BOARD_DrawMoveButtonBoard (int, int, int, HWND, HDC) ;
void BOARD_DrawMoveButtonBoard1 (int, int, int, HWND, HDC) ;

// boardcfg.c
BOOL CALLBACK BoardBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void BOARDCFG_Resize (HDC, int, int) ;
void BOARDCFG_DrawHandle (HDC) ;
void BOARDCFG_DrawWhiteLag (HDC) ;
void BOARDCFG_DrawBlackLag (HDC) ;
void BOARDCFG_DrawWhiteClock (HDC) ;
void BOARDCFG_DrawBlackClock (HDC) ;
void BOARDCFG_DrawGameType (HDC) ;
void BOARDCFG_DrawLastMove (HDC) ;
void BOARDCFG_DrawResult (HDC) ;
void BOARDCFG_DrawPtell (HDC) ;
void BOARDCFG_DrawGinfo (HDC) ;
void BOARDCFG_DrawCoordinates (HDC, int) ;
void BOARDCFG_DrawTruePremove (HDC) ;
void BOARDCFG_DrawIllegalTruePremove (HDC) ;
void BOARDCFG_DrawKingHighlight (HDC) ;
void BOARDCFG_SquareToPosition (int, int, int *, int *) ;
void BOARDCFG_DrawFromToSquareHighlight (HDC) ;
void BOARDCFG_DrawBoardPiece (HDC, int, int, int) ;
void BOARDCFG_DrawNumber (HDC, int) ;
void BOARDCFG_DrawBufferPiece (HDC, int, int) ;
void BOARDCFG_DrawPosition (HDC) ;
void BOARDCFG_DrawAll (HWND, HDC) ;
void BOARDCFG_Refresh (HWND) ;

// bughouse.c
void BUGHOUSE_Init (void) ;
void BUGHOUSE_FirstTime (int) ;
void BUGHOUSE_EndMoveList (int) ;
void BUGHOUSE_SaveGameMove (int, int, int) ;

// butcfg.c
BOOL CALLBACK ButtonBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// button.c
void BUTTON_Init (void) ;
void BUTTON_Chess (void) ;
void BUTTON_OldFICSEAR (void) ;
void BUTTON_NewFICSEAR (void) ;
void BUTTON_RefreshLabel (void) ;
LRESULT CALLBACK ButtonWndProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK ButtonSubclass (HWND, UINT, WPARAM, LPARAM) ;
void BUTTON_Refresh (void) ;
void BUTTON_Command (int) ;

// censor.c
BOOL CALLBACK CensorBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void CENSOR_Init (void) ;
void CENSOR_Sort (void) ;
void CENSOR_Index (void) ;
void CENSOR_Adjust (void) ;
int CENSOR_Tell (char *, char *) ;
int CENSOR_Ptell (char *, char *) ;
int CENSOR_Say (char *, char *) ;
int CENSOR_Partner (char *) ;
int CENSOR_Message (char *) ;
int CENSOR_Other (int, char *) ;
int CENSOR_ClearMessages (char *) ;

// chess.c
void CHESS_Init (void) ;
int CHESS_IsEmptySquare (int, int, int) ;
int CHESS_IsWhitePiece (int, int, int) ;
int CHESS_IsBlackPiece (int, int, int) ;
int CHESS_IsSameColor (int, int, int, int) ;
int CHESS_ReversePiece (int) ;
void CHESS_CopyTempMove (int) ;
void CHESS_CopyMoveTemp (int, int) ;
void CHESS_AddTempMove (int, int, int, int, int, int, int, int) ;
int CHESS_CheckBQ (int, int, int, int, int, int, int, int *, int *) ;
int CHESS_CheckRQ (int, int, int, int, int, int, int, int *, int *) ;
int CHESS_CheckN (int, int, int, int, int *) ;
int CHESS_CheckWP (int, int, int, int *) ;
int CHESS_CheckBP (int, int, int, int *) ;
int CHESS_CheckK (int, int, int, int, int *) ;
int CHESS_SquareInCheck (int, int, int, int, int *, int *) ;
void CHESS_GenKnight (int, int, int, int) ;
void CHESS_GenKing (int, int, int, int) ;
void CHESS_GenDiagonal (int, int, int, int) ;
void CHESS_GenHorizontal (int, int, int, int) ;
void CHESS_GenWhitePawn (int, int, int) ;
void CHESS_GenWhiteKing (int, int, int) ;
void CHESS_GenBlackPawn (int, int, int) ;
void CHESS_GenBlackKing (int, int, int) ;
void CHESS_BackupPosition (int) ;
void CHESS_RestorePosition (int) ;
void CHESS_MakeMove (int, int, int, int, int, int, int, int, int) ;
void CHESS_GenMove (int, int, int) ;
int CHESS_SmartMoveSource (int, int, int) ;
int CHESS_SmartMoveDestination (int, int, int) ;
char * CHESS_GetNotation (int, int) ;
char * CHESS_GetLongNotation (int, int) ;
int CHESS_IsLegalMoveString (int, char *) ;
int CHESS_IsLegalMove (int, int, int, int, int, int, int) ;
int CHESS_IsLegalTruePremove (int, int, int, int, int, int, int) ;
int CHESS_IsLegalExamDropMove (int, int, int, int) ;
int CHESS_IsLegalPartnerBoardMove (int, int, int, int, int, int, int) ;
int CHESS_GenKingLegalSquare (int, int) ;
void CHESS_AtomicCapture (int, int, int) ;
int CHESS_OpponentKingIsNear (int, int, int, int) ;

// clock.c
int CLOCK_TimerRunning (int) ;
int CLOCK_StopClockTimer (int) ;
void CLOCK_StartClockTimer (int, long) ;
void CLOCK_GetTimeMark (TIMEMARK *) ;
long CLOCK_SubtractTimeMarks (TIMEMARK *, TIMEMARK *) ;
long CLOCK_NextTickLength (long) ;
void CLOCK_ResetClocks (int) ;
void CLOCK_DecrementClocks (int, HWND) ;
void CLOCK_SwitchClocks (int) ;
void CLOCK_StopClocks (int, HWND) ;
void CLOCK_StartClocks (int, HWND) ;
void CLOCK_CheckFlags (void) ;
char * CLOCK_TimeString (long) ;

// color.c
void COLOR_Init (void) ;
void COLOR_Init_Red (void) ;
void COLOR_Init_Board (void) ;
void COLOR_Init_Handle (void) ;
void COLOR_Init_Clock (void) ;
void COLOR_Init_LagStat (void) ;
void COLOR_Init_GameType (void) ;
void COLOR_Init_LastMove (void) ;
void COLOR_Init_Highlight (void) ;
void COLOR_Init_TruePremove (void) ;
void COLOR_Init_IllegalTruePremove (void) ;
void COLOR_Init_KingHighlight (void) ;
void COLOR_Init_Result (void) ;
void COLOR_Init_Ptell (void) ;
void COLOR_Init_GameInfo (void) ;
void COLOR_Init_Coordinates (void) ;
void COLOR_Init_Telnet (void) ;
void COLOR_Init_Telnet1 (void) ;
void COLOR_Init_Telnet2 (void) ;
void COLOR_Init_Telnet3 (void) ;
BOOL COLOR_ChooseColor (int) ;
BOOL COLOR_ChooseColor1 (HWND, COLORREF *) ;

// cset.c
void CSET_Init (void) ;
void CSET_Load (HDC) ;
void CSET_Destroy (void) ;

// ctrlkey.c
BOOL CALLBACK ControlKeyBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void CONTROLKEY_Init (void) ;
void CONTROLKEY_Refresh (void) ;
void CONTROLKEY_Adjust (void) ;
int CONTROLKEY_Command (int) ;

// draw.c
void DRAW_LoadPieces (void) ;
void DRAW_SetSize (int) ;
void DRAW_WhiteSquare (HDC hdc) ;
void DRAW_BlackSquare (HDC hdc) ;
void DRAW_SquareW (HDC) ;
void DRAW_PawnW (HDC) ;
void DRAW_RookW (HDC) ;
void DRAW_KnightW (HDC) ;
void DRAW_BishopW (HDC) ;
void DRAW_QueenW (HDC) ;
void DRAW_KingW (HDC) ;
void DRAW_SquareB (HDC) ;
void DRAW_PawnB (HDC) ;
void DRAW_RookB (HDC) ;
void DRAW_KnightB (HDC) ;
void DRAW_BishopB (HDC) ;
void DRAW_QueenB (HDC) ;
void DRAW_KingB (HDC) ;
void DRAW_Piece (HDC, int) ;
void DRAW_SquareBF (HDC) ;
void DRAW_PawnM (HDC) ;
void DRAW_RookM (HDC) ;
void DRAW_KnightM (HDC) ;
void DRAW_BishopM (HDC) ;
void DRAW_QueenM (HDC) ;
void DRAW_KingM (HDC) ;
void DRAW_PieceMask (HDC, int) ;
void DRAW_PawnW_Outline (HDC, COLORREF) ;
void DRAW_RookW_Outline (HDC, COLORREF) ;
void DRAW_KnightW_Outline (HDC, COLORREF) ;
void DRAW_BishopW_Outline (HDC, COLORREF) ;
void DRAW_QueenW_Outline (HDC, COLORREF) ;
void DRAW_KingW_Outline (HDC, COLORREF) ;
void DRAW_PawnB_Outline (HDC, COLORREF) ;
void DRAW_RookB_Outline (HDC, COLORREF) ;
void DRAW_KnightB_Outline (HDC, COLORREF) ;
void DRAW_BishopB_Outline (HDC, COLORREF) ;
void DRAW_QueenB_Outline (HDC, COLORREF) ;
void DRAW_KingB_Outline (HDC, COLORREF) ;
void DRAW_Piece_Outline (HDC, int, COLORREF) ;
void DRAW_SquareW1 (HDC, COLORREF, COLORREF) ;
void DRAW_PawnW1 (HDC, COLORREF, COLORREF) ;
void DRAW_RookW1 (HDC, COLORREF, COLORREF) ;
void DRAW_KnightW1 (HDC, COLORREF, COLORREF) ;
void DRAW_BishopW1 (HDC, COLORREF, COLORREF) ;
void DRAW_QueenW1 (HDC, COLORREF, COLORREF) ;
void DRAW_KingW1 (HDC, COLORREF, COLORREF) ;
void DRAW_SquareB1 (HDC, COLORREF, COLORREF) ;
void DRAW_PawnB1 (HDC, COLORREF, COLORREF) ;
void DRAW_RookB1 (HDC, COLORREF, COLORREF) ;
void DRAW_KnightB1 (HDC, COLORREF, COLORREF) ;
void DRAW_BishopB1 (HDC, COLORREF, COLORREF) ;
void DRAW_QueenB1 (HDC, COLORREF, COLORREF) ;
void DRAW_KingB1 (HDC, COLORREF, COLORREF) ;
void DRAW_Piece1 (HDC, int, COLORREF, COLORREF, COLORREF, COLORREF) ;
void DRAW_SquareBF1 (HDC, COLORREF, COLORREF) ;
void DRAW_VectorPiece (HDC, int, COLORREF, int) ;
void DRAW_TransparentBlt (int, HDC, int) ;

// ear.c
void EAR_Append (void) ;
void EAR_Init (void) ;
void EAR_OldFICSEAR (void) ;
void EAR_NewFICSEAR (void) ;
void EAR_Load (void) ;
int EAR_Play (char *) ;
void EAR_Pack () ;
void EAR_Destroy (void) ;

// earcfg.c
BOOL CALLBACK EarBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// f8key.c
void F8KEY_Init (void) ;
void F8KEY_Set (void) ;
void F8KEY_Issue (void) ;

// f9key.c
void F9KEY_Init (void) ;
void F9KEY_Clear (void) ;
void F9KEY_Add (char *) ;
void F9KEY_Set (void) ;

// fics.c
void FICS_ProcessBeforeLoginLine (char *) ;
void FICS_SendBeforeTwelve (void) ;
void FICS_ProcessLine (char *) ;
int FICS_ParseBoard (char *) ;
int FICS_ParsePiece (char *) ;
int FICS_ColorIndex (char *) ;
COLORREF FICS_ColonColor (char *) ;

// fkey.c
BOOL CALLBACK FunctionBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void FKEY_Init (void) ;
void FKEY_Refresh (void) ;
void FKEY_Adjust (void) ;
int FKEY_Command (int) ;

// font.c
void FONT_Init (void) ;
void FONT_Init_Handle (void) ;
void FONT_Init_Clock (void) ;
void FONT_Init_GameType (void) ;
void FONT_Init_LastMove (void) ;
void FONT_Init_LagStat (void) ;
void FONT_Init_Result (void) ;
void FONT_Init_Coordinates (void) ;
void FONT_Init_Telnet (void) ;
void FONT_Init_Telnet1 (void) ;
void FONT_Init_Telnet2 (void) ;
void FONT_Init_Telnet3 (void) ;
void FONT_Init_Button (void) ;
void FONT_CreateFont (int, char *, int, int, int, int, int) ;
BOOL FONT_ChooseFont (HWND, int) ;
void FONT_CreateFont1 (FONTRECORD *, char *, int, int, int, int, int) ;
BOOL FONT_ChooseFont1 (HWND, FONTRECORD *) ;
void FONT_Copy_Info (FONTRECORD *, FONTRECORD *) ;

// game.c
void GAME_Init (void) ;
void GAME_Destroy (void) ;
void GAME_InitAllGame (int) ;
void GAME_InitialSetup (int) ;

// gamecfg.c
BOOL CALLBACK GameSoundBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// gameevnt.c
BOOL CALLBACK GameEventBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void GAMEEVENT_Init (void) ;
void GAMEEVENT_Adjust (void) ;
int GAMEEVENT_Command (int) ;

// gdisplay.c
BOOL CALLBACK GameDisplayBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// gnotify.c
void GNOTIFY_Init (void) ;
void GNOTIFY_Add (char *) ;
void GNOTIFY_Delete (char *) ;
int GNOTIFY_Find (char *) ;

// goption.c
BOOL CALLBACK GameOptionBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// gamesnd.c
void GAMESOUND_Init (void) ;
void GAMESOUND_Load (void) ;
void GAMESOUND_Play (int) ;
void GAMESOUND_Destroy (void) ;
int GAMESOUND_PlayBoard (char *) ;
int GAMESOUND_PartnerBoard (char *) ;

// history.c
void HISTORY_Init (void) ;
void HISTORY_Previous (void) ;
void HISTORY_Next (void) ;
void HISTORY_Add (char *) ;

// icc.c
void ICC_ProcessBeforeLoginLine (char *) ;
void ICC_SendBeforeTwelve (void) ;
void ICC_ProcessLine (char *) ;
int ICC_ParseBoard (char *) ;
void ICC_ParseExamBoard (char *) ;
int ICC_ParsePiece (char *) ;
int ICC_ColorIndex (char *) ;
COLORREF ICC_ColonColor (char *) ;

// ini.c
char * INI_GetIniFilename (void) ;
char * INI_GetSysFilename (void) ;
int INI_ReadSetup (char *) ;
void INI_WriteSetup (char *) ;
int INI_ReadSystem (char *) ;
void INI_WriteSystem (char *) ;

// lagcmd.c
BOOL CALLBACK LagCommandBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void LAGCMD_Adjust (void) ;

// layout.c
char * LAYOUT_File (int) ;
void LAYOUT_Load (int) ;
void LAYOUT_ActualRestore (char *) ;
void LAYOUT_ActualSave (char *) ;
void LAYOUT_Restore (int) ;
void LAYOUT_Save (int) ;
void LAYOUT_Load1 (int, int, int, int, int, int) ;
void LAYOUT_Load2 (int, int, int, int, int, int) ;
void LAYOUT_Load3 (int, int, int, int, int, int) ;
void LAYOUT_Load4 (int, int, int, int, int, int) ;
void LAYOUT_Load5 (int, int, int, int, int, int) ;
void LAYOUT_Load6 (int, int, int, int, int, int) ;
void LAYOUT_Load7 (int, int, int, int, int, int) ;
void LAYOUT_Load8 (int, int, int, int, int, int) ;
void LAYOUT_LoadVirtual (void) ;
void LAYOUT_CreateDefaultLayout (int, int, int, int, int, int, int) ;
void LAYOUT_Try_1 (int, int, int) ;
void LAYOUT_Try_2 (int, int, int) ;
void LAYOUT_Try_3 (int, int, int) ;
void LAYOUT_Try_4 (int, int, int) ;
void LAYOUT_Try_5 (int, int, int) ;
void LAYOUT_Try_6 (int, int, int) ;
void LAYOUT_Try_7 (int, int, int) ;
void LAYOUT_Try_8 (int, int, int) ;
void LAYOUT_Try_9 (int, int, int) ;
void LAYOUT_Try_10 (int, int, int) ;
void LAYOUT_Try_11 (int, int, int) ;
void LAYOUT_Try_12 (int, int, int) ;
void LAYOUT_Try_13 (int, int, int) ;
void LAYOUT_Try_14 (int, int, int) ;
void LAYOUT_Try_15 (int, int, int) ;
void LAYOUT_ProfileRestore (char *) ;

// log.c
void LOG_Init (void) ;
int LOG_Start (void) ;
void LOG_Write (char *) ;
int LOG_End (void) ;

// login.c
void LOGIN_File (char *, char *) ;
int LOGIN_Load (char *, char *, char *, char *, char *, int *, char *, char *, char *, char *) ;
void LOGIN_Save (char *, char *, char *, char *, char *, int *, char *, char *, char *, char *) ;
void LOGIN_Prepare (void) ;
void LOGIN_Check (void) ;
int LOGIN_FillAddress (PSOCKADDR_IN, int) ;
void LOGIN_Login (void) ;
void PMI_TS_Load (void) ;
void PMI_TS_Write (char *) ;
DWORD PMI_TS_ReadThread (LPVOID) ;
void PMI_TS_Destroy (void) ;

// logevent.c
BOOL CALLBACK LoginEventBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void LOGINEVENT_Init (void) ;
void LOGINEVENT_Adjust (void) ;
int LOGINEVENT_Command (int) ;

// main.c
LRESULT CALLBACK FrameWndProc (HWND, UINT, WPARAM, LPARAM) ;

// match.c
BOOL CALLBACK MatchBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void MATCH_AdjustMatch (int, char *, int, int, int) ;
void MATCH_AdjustValues (void) ;

// message.c
BOOL CALLBACK MessageBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// minput.c
BOOL CALLBACK MoveInputBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// movebut.c
BOOL CALLBACK MoveButtonBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// newlogin.c
BOOL CALLBACK NewLoginBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// nonfics.c
int NONFICS_MatchFirstItem (char *, char *, int) ;
void NONFICS_ProcessBeforeLoginLine (char *) ;
void NONFICS_SendBeforeTwelve (void) ;
void NONFICS_ProcessLine (char *) ;
int NONFICS_ParseBoard (char *) ;
int NONFICS_ParsePiece (char *) ;
int NONFICS_ColorIndex (char *) ;
COLORREF NONFICS_ColonColor (char *) ;

// observe.c
LRESULT CALLBACK ObserveWndProc (HWND, UINT, WPARAM, LPARAM) ;
void OBSERVE_Command (HWND, int) ;

// oldlogin.c
BOOL CALLBACK OldLoginBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// password.c
BOOL CALLBACK PasswordBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// play.c
LRESULT CALLBACK PlayWndProc (HWND, UINT, WPARAM, LPARAM) ;
void PLAY_Command (int) ;

// position.c
void POSITION_Init (int) ;
void POSITION_FirstTime (int) ;
void POSITION_AddMoveList (int, int, int, char *, char *) ;
void POSITION_SaveGameMove (int) ;
void POSITION_FICS_SaveGameBuffer (int, int) ;
void POSITION_ICC_SaveGameBuffer (int) ;
void POSITION_NONFICS_SaveGameBuffer (int, int) ;

// promote.c
BOOL CALLBACK PromoteBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
int PROMOTE_Pawn (HWND, HINSTANCE, int, int) ;

// question.c
BOOL CALLBACK QuestionBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// redclock.c
BOOL CALLBACK RedClockBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void REDCLOCK_Init (void) ;
void REDCLOCK_Adjust (void) ;

// rightm.c
BOOL CALLBACK RightMouseClickBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void RIGHTMOUSE_Init (void) ;
void RIGHTMOUSE_Init1 (void) ;
void RIGHTMOUSE_Adjust (void) ;
int RIGHTMOUSE_Command (int, int, int) ;
void RIGHTMOUSE_AssignCheck (UINT, int) ;
void RIGHTMOUSE_LoadCheck (void) ;

// savegame.c
BOOL CALLBACK SaveGameBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void SAVEGAME_GetFilename (int, int, int, int, char *) ;
char * SAVEGAME_GetFEN (int) ;
char * SAVEGAME_GetBFEN (int) ;
char * SAVEGAME_GetPGNHeader (int, char *) ;
void SAVEGAME_SavePGN (int, int, int) ;
char * SAVEGAME_GetZHHeader (int, char *) ;
void SAVEGAME_SaveZH (int, int, int) ;
char * SAVEGAME_GetBHHeader (int, int, int, char *) ;
void SAVEGAME_SaveBH (int, int, int, int) ;
void SAVEGAME_SaveGame (int, int, int) ;

// seek.c
BOOL CALLBACK SeekBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void SEEK_Adjust (void) ;

// server.c
BOOL CALLBACK ServerBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void SERVER_Init (void) ;
void SERVER_Adjust (void) ;

// silence.c
BOOL CALLBACK SilenceBoxWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) ;
void SILENCE_Init (void) ;
int SILENCE_Tell (char *, char *) ;
void SILENCE_Adjust (void) ;

// sitpiece.c
BOOL CALLBACK SitPieceBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// smove.c
void SMARTMOVE_Adjust (void) ;

// sndcfg.c
BOOL CALLBACK SoundBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// sound.c
void SOUND_Init (void) ;
void SOUND_Load (void) ;
void SOUND_Play (void) ;
void SOUND_Destroy (void) ;

// state.c
void STATE_PlayNewGame (void) ;
void STATE_PlayEndGame (void) ;
void STATE_ObserveNewGame (int, int) ;
void STATE_ObserveEndGame (int, int, int) ;
void STATE_PlayPieceBufferUpdate (void) ;
void STATE_Login (int) ;
void STATE_Logout (void) ;
int STATE_EnterDialogBox (void) ;
void STATE_LeaveDialogBox (void) ;

// sys.c
void SYS_InitVal (void) ;
void SYS_InitCoord (HWND) ;
void SYS_CreateVal (void) ;
void SYS_DestroyVal (void) ;
void SYS_ChangeColor (void) ;
void SYS_WritePieceFile (char *, char *, char *) ;
void SYS_CreateToups (void) ;
void SYS_CreateNicok (void) ;
void SYS_CreateKbirk (void) ;
void SYS_CreateJhurt (void) ;
void SYS_CreateEchur (void) ;
void SYS_CreateDfong (void) ;
void SYS_CreateUser (void) ;
void SYS_CheckToups (void) ;
void SYS_CheckNicok (void) ;
void SYS_CheckKbirk (void) ;
void SYS_CheckJhurt (void) ;
void SYS_CheckEchur (void) ;
void SYS_CheckDfong (void) ;
void SYS_CheckUser (void) ;

// telcfg.c
BOOL CALLBACK TelnetBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void TELCFG_SetFont (HWND) ;
void TELCFG_Colorize (COLORREF) ;
void TELCFG_Write (HWND, COLORREF, char *) ;
void TELCFG_Refresh (HWND) ;

// tell.c
BOOL CALLBACK TellBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// telnet.c
LRESULT CALLBACK TelnetWndProc (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK TelnetTextSubclass (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK TelnetEditSubclass (HWND, UINT, WPARAM, LPARAM) ;
void TELNET_Colorize (COLORREF) ;
void TELNET_SetEditColor (void) ;
void TELNET_SetFont (void) ;
void TELNET_SetEditFont (void) ;
void TELNET_Clear (void) ;
void TELNET_GoBottom (void) ;
void TELNET_ClearScreen (void);
void TELNET_GoBottomFillText (void) ;
void TELNET_Write (char *) ;
void TELNET_Write1 (char *) ;
void TELNET_NormalWrite (char *) ;
void TELNET_ItalicWrite (char *) ;
void TELNET_UnderlineWrite (char *) ;
void TELNET_NormalPrint (char *) ;
void TELNET_EditEnter (void) ;
int TELNET_MatchExp (char *, char *) ;
int TELNET_IsChannel (char *, int *) ;
void TELNET_CheckPressReturn (char *) ;

// thief.c
int WINAPI WinMain (HINSTANCE, HINSTANCE, PSTR, int) ;

// timercfg.c
BOOL CALLBACK TimerBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// timercmd.c
void TIMERCMD_Init (void) ;
void TIMERCMD_Clear (void) ;
void TIMERCMD_Adjust (void) ;
void TIMERCMD_Start (void) ;
void TIMERCMD_Stop (void) ;
void TIMERCMD_Timer (void) ;
void TIMERCMD_Set (HWND) ;
void TIMERCMD_Reset (HWND) ;

// timesnd.c
BOOL CALLBACK FinalBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void TIMESOUND_Init (void) ;
void TIMESOUND_Load (void) ;
void TIMESOUND_Play (int, long) ;
void TIMESOUND_Destroy (void) ;

// tmove.c
void TRUEPREMOVE_Adjust (void) ;

// toption.c
BOOL CALLBACK TelnetOptionBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// toolbox.c
int TOOLBOX_IsAlpha (char) ;
int TOOLBOX_IsAlpha1 (char, BOOL) ;
void TOOLBOX_Write (char *) ;
void TOOLBOX_Writeln (char *) ;
void TOOLBOX_Beep (void) ;
void TOOLBOX_Error (char *) ;
void TOOLBOX_SaveWindowCoord (HWND, int) ;
void TOOLBOX_GetWindowCoord (HWND, int *, int *, int *, int *) ;
void TOOLBOX_TryAllWindowCoord (void) ;
void TOOLBOX_LoadAllWindowCoord (void) ;
void TOOLBOX_SaveAllWindowCoord (void) ;
void TOOLBOX_ReadINICoord (WCOORD *, char *, char *, char *) ;
void TOOLBOX_WriteINICoord (FILE *, WCOORD *, char *) ;
void TOOLBOX_ReadINIColor (char *, COLORREF *, char *, char *) ;
int TOOLBOX_ReadINIColor1 (char *, COLORREF *, char *, char *) ;
void TOOLBOX_WriteINIColor (FILE *, COLORREF, char *) ;
void TOOLBOX_ReadINIFont (char *, int, char *, char *) ;
void TOOLBOX_WriteINIFont (FILE *, int, char *) ;
void TOOLBOX_MakeFont (int) ;
void TOOLBOX_ConnectedMenu (void) ;
void TOOLBOX_DisconnectedMenu (void) ;
void TOOLBOX_CheckGuestMenu (void) ;
void TOOLBOX_CheckAllMenu (void) ;
void TOOLBOX_SetTimerCommandMenu (void) ;
void TOOLBOX_RefreshWindowColor (void) ;
void TOOLBOX_InvalidateBoardWindow (int) ;
void TOOLBOX_InvalidateBoardWindow1 (int) ;
void TOOLBOX_CloseObserveWindow (void) ;
int TOOLBOX_CenterWindow (HWND, HWND) ;
FILE * TOOLBOX_OpenFileDialog (HWND hwnd, BOOL write, char *defName, char *defExt, char *nameFilt, char *dlgTitle, UINT *number, char fileTitle [MSG_SIZE], char fileName [MSG_SIZE], char Init [_MAX_PATH]) ;
void TOOLBOX_AllTrim (char *) ;
int TOOLBOX_CanOpenFile (char *) ;
void TOOLBOX_SetTelnetCaption (void) ;
void TOOLBOX_WriteICS (char *) ;
void TOOLBOX_WriteUser (char *) ;
void TOOLBOX_WriteSystem (char *) ;
void TOOLBOX_CloseSocket (void) ;
int TOOLBOX_ReadScriptFile (char *) ;
int TOOLBOX_FindGameByNumber (int) ;
int TOOLBOX_ExpandHandleMacro (char *, char *, char *, char *) ;
void TOOLBOX_ExpandAllMacro (char *, char *, int) ;
void TOOLBOX_SetupFICSMenu (void) ;
void TOOLBOX_AddFICSMenu (char *) ;
void TOOLBOX_ShowMoveButtons (int, int) ;
void TOOLBOX_ProcessCmdLine (PSTR) ;
void TOOLBOX_GetTelnetHandle (BOOL) ;
void TOOLBOX_GetCclientHandle (CHARRANGE, char *, BOOL) ;
void APIENTRY TOOLBOX_MenuPopup (HWND, POINT, HMENU, UINT) ;
void TOOLBOX_GetPath (char *, char *) ;
void TOOLBOX_WriteInfoOnOff (int, char *) ;
int TOOLBOX_Command (int, char *, int) ;
char * TOOLBOX_GetICSHandle (char *, BOOL) ;
char * TOOLBOX_GetICSHandleFromMessage (char *) ;
char * TOOLBOX_GetICSHandleFromMessageDeleted (char *) ;
char * TOOLBOX_GetICSHandleFromNoMessage (char *cS) ;
int TOOLBOX_IsComputerHandle (char *) ;
int TOOLBOX_IsSeparator (int, int) ;
int TOOLBOX_OKRightMouse (int, int) ;
int TOOLBOX_SaveTelnetMarkedText (void) ;
int TOOLBOX_TelnetTextDoubleClickExecute (void) ;
char * TOOLBOX_QuotesAroundFilename (char *) ;
void TOOLBOX_OpenNotePad (char *) ;
void TOOLBOX_OpenGame (char *) ;
void TOOLBOX_RedrawBoard (int) ;
void TOOLBOX_IssueISet (int, int, int, int) ;
void TOOLBOX_CheckObserveExam (char *) ;
void TOOLBOX_LowerString (char *) ;
int TOOLBOX_MatchHandle (char *, char *) ;
char * TOOLBOX_GetGameTypeString (int) ;
int TOOLBOX_ShiftArrowKey (int) ;
int TOOLBOX_DisplayActualBoard (int) ;
int TOOLBOX_OKMoveButton (int) ;
void TOOLBOX_IssueLoginEvent (void) ;
void TOOLBOX_AdjustShowMoveButtons (int) ;
int TOOLBOX_NeedCommandValue (int) ;
void TOOLBOX_Issue_Command (char *, char *, char *, int) ;
char * TOOLBOX_GetMyGamePath (void) ;
char * TOOLBOX_GetObservedGamePath (void) ;
char * TOOLBOX_GetOtherGamePath (void) ;
char * TOOLBOX_GetWavePath (void) ;
char * TOOLBOX_GetFullWaveFilename (char *) ;
char * TOOLBOX_GetWaveFilename (char *) ;
char * TOOLBOX_GetHelperPath (void) ;
char * TOOLBOX_GetFullHelperFilename (char *) ;
char * TOOLBOX_GetHelperFilename (char *) ;
char * TOOLBOX_GetScriptPath (void) ;
char * TOOLBOX_GetFullScriptFilename (char *) ;
char * TOOLBOX_GetScriptFilename (char *) ;
char * TOOLBOX_GetFullConFilename (char *) ;
char * TOOLBOX_GetConFilename (char *) ;
char * TOOLBOX_GetFullProfileFilename (char *) ;
char * TOOLBOX_GetMyDocumentPath (void) ;
char * TOOLBOX_GetInstDirPath (void) ;
void TOOLBOX_SortLoginString (void) ;
int TOOLBOX_LoadProfile (char *) ;
int TOOLBOX_ShowBuffer (int) ;
int TOOLBOX_ShowBuffer1 (int) ;
void TOOLBOX_AdjustClipboardData (HWND) ;
int TOOLBOX_IsQChannelTell (char *, char *) ;
void TOOLBOX_DisplayBitmapSize (int) ;
void TOOLBOX_DisplayMenuPath (char *) ;
void TOOLBOX_ResetPromotKnight (void) ;
char * TOOLBOX_GetGameWindowTitle (int) ;
void TOOLBOX_ShowTaskIcon (HWND, BOOL) ;

// try.c
BOOL CALLBACK TryBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;

// vars.c
BOOL CALLBACK VarsBoxWndProc (HWND, UINT, WPARAM, LPARAM) ;
void VARS_Init (void) ;
