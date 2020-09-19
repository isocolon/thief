#include "thief.h"

void LOG_Init (void)
    {
    strcpy (Log.cFn, "") ;
    Log.Fv = NULL ;
    }

int LOG_Start (void)
    {
    char Drive [_MAX_DRIVE] ;
    char Dir   [_MAX_DIR] ;
    char File  [_MAX_FNAME] ;
    char Ext   [_MAX_EXT] ;

    char cDate [20], cTime [20], cTmp [20] ;
    int nI ;

    if (Log.Fv != NULL)
        {
        fclose (Log.Fv) ;
        }

    _splitpath (System.cDocumentDir, Drive, Dir, File, Ext) ;

    nI = strlen (Dir) ;
    if (nI == 0)
        {
        strcat (Dir, "\\LOG\\") ;
        }
    else
        {
        if (Dir [nI - 1] == '\\')
            {
            strcat (Dir, "LOG\\") ;
            }
        else
            {
            strcat (Dir, "\\LOG\\") ;
            }
        }

    _strdate (cDate) ;  // MM/DD/YY
    _strtime (cTime) ;  // HH:MM:SS

    strcpy (File, Login.cServerName) ;

//    cTmp [0] = cDate [0] ;  // M
//    cTmp [1] = cDate [1] ;  // M
//    cTmp [2] = cDate [3] ;  // D
//    cTmp [3] = cDate [4] ;  // D
//    cTmp [4] = cDate [6] ;  // Y
//    cTmp [5] = cDate [7] ;  // Y

	cTmp [0] = cDate [6] ;  // Y
    cTmp [1] = cDate [7] ;  // Y
    cTmp [2] = cDate [0] ;  // M
    cTmp [3] = cDate [1] ;  // M
    cTmp [4] = cDate [3] ;  // D
    cTmp [5] = cDate [4] ;  // D


    cTmp [6] = NULL_CHAR ;

    strcat (File, cTmp) ;

    strcpy (Ext, LOG_EXT) ;

    _makepath (Log.cFn, Drive, Dir, File, Ext) ;

    if (System.bCDROMConnection)
        {
        return 0 ;
        }

    Log.Fv = fopen (Log.cFn, "a") ;
    if (Log.Fv == NULL)
        {
        return 0 ;
        }

    fprintf (Log.Fv, "\n--[%s  %s]--\n", cDate, cTime) ;
    return 1 ;
    }

void LOG_Write (char *cS)
    {
	char cTime [20] ;
	int nLength;

    if (System.bCDROMConnection)
        {
        return ;
        }
    if (Timeseal.bSocketIsOpen)
        {
        if (Log.Fv != NULL)
            {
			fprintf (Log.Fv, "%s", cS) ;
			if (User.bTimeStampLog && !User.bTimeStampTelnet)
				{
				nLength = strlen (cS) ;
				if (cS [nLength-1] == '\n')
					{		
					fprintf (Log.Fv, "[%5.5s] ", _strtime (cTime)) ;
					}
				}
			}
		}
    }

int LOG_End (void)
    {
    if (System.bCDROMConnection)
        {
        return 0 ;
        }

    if (Log.Fv == NULL)
        {
        return 0 ;
        }
    else
        {
        fclose (Log.Fv) ;
        return 1 ;
        }
    }
