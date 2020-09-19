#include "thief.h"

void CSET_Init (void)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_PIECE ; nI++)
        {
        CSet.hPiece   [nI]   = NULL ;
        CSet.ptPiece  [nI].x = 0 ;
        CSet.ptPiece  [nI].y = 0 ;
        CSet.clrPiece [nI]   = RGB (0, 0, 0) ;
        }

    for (nI = 0 ; nI < MAX_SQUARE ; nI++)
        {
        CSet.hSquare  [nI]   = NULL ;
        CSet.ptSquare [nI].x = 0 ;
        CSet.ptSquare [nI].y = 0 ;
        }
    }

void CSET_Load (HDC hdc)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;
    char Res   [_MAX_PATH] ;

    char Square [MAX_SQUARE] [10] = { "WSQUARE",
                                      "BSQUARE",
                                      "BUFFER" } ;

    char Piece [MAX_PIECE] [10] = { "WPAWN",
                                    "WROOK",
                                    "WKNIGHT",
                                    "WBISHOP",
                                    "WQUEEN",
                                    "BPAWN",
                                    "BROOK",
                                    "BKNIGHT",
                                    "BBISHOP",
                                    "BQUEEN",
                                    "WKING",
                                    "BKING" } ;

    HDC hdcMem ;
    HBITMAP htOld ;
    BITMAP bitmap ;
    COLORREF clrDef ;

    int nI, bError ;

    CSET_Destroy () ;

    if (User.bVectorSquare || User.bVectorPiece)
        {
        DRAW_LoadPieces () ;
        }

    if (User.bVectorSquare)
        {
        for (nI = 0 ; nI < MAX_SQUARE ; nI++)
            {
            CSet.hSquare  [nI]   = NULL ;
            CSet.ptSquare [nI].x = CSet.ptPiece [WHITE_PAWN].x ;
            CSet.ptSquare [nI].y = CSet.ptPiece [WHITE_PAWN].x ;
            }
        }
    else
        {
        _splitpath (User.cBitmapSquare, Drive, Dir, File, Ext) ;

        strcpy (Ext, BMP_EXT) ;

        bError = 0 ;
        for (nI = 0 ; nI < MAX_SQUARE ; nI++)
            {
            strcpy (File, Square [nI]) ;
            _makepath (Res, Drive, Dir, File, Ext) ;

            _chdir (TOOLBOX_GetMyDocumentPath ()) ;

            CSet.hSquare [nI] = LoadImage (NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
            if (CSet.hSquare [nI] == NULL)
                {
                bError = 1 ;
                break ;
                }
            else
                {
                GetObject (CSet.hSquare [nI], sizeof (BITMAP), (LPVOID) &bitmap) ;

                CSet.ptSquare [nI].x = bitmap.bmWidth ;
                CSet.ptSquare [nI].y = bitmap.bmHeight ;
                }
            }

        if (bError)
            {
            User.bVectorSquare = 1 ;

            for (nI = 0 ; nI < MAX_SQUARE ; nI++)
                {
                if (CSet.hSquare [nI])
                    {
                    DeleteObject (CSet.hSquare [nI]) ;
                    }

                CSet.hSquare  [nI]   = NULL ;
                CSet.ptSquare [nI].x = CSet.ptPiece [WHITE_PAWN].x ;
                CSet.ptSquare [nI].y = CSet.ptPiece [WHITE_PAWN].x ;
                }
            }
        }

    if (User.bVectorPiece)
        {
        if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR0) &&
            (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR0) &&
            (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR0) &&
            (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR0))
            {
            clrDef = DEFAULT_TRANSPARENT_COLOR0 ;
            }
        else
        if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR1) &&
            (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR1) &&
            (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR1) &&
            (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR1))
            {
            clrDef = DEFAULT_TRANSPARENT_COLOR1 ;
            }
        else
        if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR2) &&
            (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR2) &&
            (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR2) &&
            (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR2))
            {
            clrDef = DEFAULT_TRANSPARENT_COLOR2 ;
            }
        else
        if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR3) &&
            (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR3) &&
            (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR3) &&
            (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR3))
            {
            clrDef = DEFAULT_TRANSPARENT_COLOR3 ;
            }
        else
            {
            clrDef = DEFAULT_TRANSPARENT_COLOR4 ;
            }

        for (nI = 0 ; nI < MAX_PIECE ; nI++)
            {
            CSet.hPiece   [nI] = NULL ;
            CSet.clrPiece [nI] = clrDef ;
            }
        }
    else
        {
        _splitpath (User.cBitmapPiece, Drive, Dir, File, Ext) ;

        strcpy (Ext, BMP_EXT) ;

        bError = 0 ;
        for (nI = 0 ; nI < MAX_PIECE ; nI++)
            {
            strcpy (File, Piece [nI]) ;
            _makepath (Res, Drive, Dir, File, Ext) ;

            _chdir (TOOLBOX_GetMyDocumentPath ()) ;

            CSet.hPiece [nI] = LoadImage (NULL, Res, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE) ;
            if (CSet.hPiece [nI] == NULL)
                {
                bError = 1 ;
                break ;
                }
            else
                {
                GetObject (CSet.hPiece [nI], sizeof (BITMAP), (LPVOID) &bitmap) ;

                CSet.ptPiece [nI].x = bitmap.bmWidth ;
                CSet.ptPiece [nI].y = bitmap.bmHeight ;

                hdcMem = CreateCompatibleDC (hdc) ;
                htOld  = (HBITMAP) SelectObject (hdcMem, CSet.hPiece [nI]) ;

                CSet.clrPiece [nI] = GetPixel (hdcMem, 0, 0) ;

                SelectObject (hdcMem, htOld) ;
                DeleteDC (hdcMem) ;
                }
            }

        if (bError)
            {
            if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR0) &&
                (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR0) &&
                (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR0) &&
                (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR0))
                {
                clrDef = DEFAULT_TRANSPARENT_COLOR0 ;
                }
            else
            if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR1) &&
                (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR1) &&
                (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR1) &&
                (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR1))
                {
                clrDef = DEFAULT_TRANSPARENT_COLOR1 ;
                }
            else
            if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR2) &&
                (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR2) &&
                (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR2) &&
                (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR2))
                {
                clrDef = DEFAULT_TRANSPARENT_COLOR2 ;
                }
            else
            if ((clrColor [CLR_WHITE_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR3) &&
                (clrColor [CLR_WHITE_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR3) &&
                (clrColor [CLR_BLACK_PIECE_OUTLINE] != DEFAULT_TRANSPARENT_COLOR3) &&
                (clrColor [CLR_BLACK_PIECE_FILL   ] != DEFAULT_TRANSPARENT_COLOR3))
                {
                clrDef = DEFAULT_TRANSPARENT_COLOR3 ;
                }
            else
                {
                clrDef = DEFAULT_TRANSPARENT_COLOR4 ;
                }

            User.bVectorPiece = 1 ;

            for (nI = 0 ; nI < MAX_PIECE ; nI++)
                {
                if (CSet.hPiece [nI])
                    {
                    DeleteObject (CSet.hPiece [nI]) ;
                    }

                CSet.hPiece   [nI] = NULL ;
                CSet.clrPiece [nI] = clrDef ;
                }
            }
        }
    }

void CSET_Destroy (void)
    {
    int nI ;

    for (nI = 0 ; nI < MAX_PIECE ; nI++)
        {
        if (CSet.hPiece [nI])
            {
            DeleteObject (CSet.hPiece [nI]) ;
            }
        }

    for (nI = 0 ; nI < MAX_SQUARE ; nI++)
        {
        if (CSet.hSquare [nI])
            {
            DeleteObject (CSet.hSquare [nI]) ;
            }
        }
    }
