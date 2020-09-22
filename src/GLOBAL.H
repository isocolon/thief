#include <windows.h>
#include <winsock.h>
#include <wingdi.h>
#include <zmouse.h>
#include <richedit.h>
#include <commdlg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/timeb.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <process.h>
#include <direct.h>
#include "menu.h"
#include "shlobj.h"

#define THIEF_VER                   "Thief 1.25"
#define THIEF_VER_NR                "1.25"
#define THIEF_REL                   "2011.05.10"
#define LUDENS						0

#define THIEF_DOCUMENT_PATH			CSIDL_PERSONAL
//#define THIEF_DOCUMENT_PATH			CSIDL_APPDATA

#ifdef  DEBUG
#define THIEF_NICK                  "(The Beta, Debugging)"
#else
#define THIEF_NICK                  "(The Release)"
#endif

#ifdef KICS
#define THIEF_KICS_NICK             "(The Release) KICS Version"
#endif

#define THIEF_NICK_LUDENS           " Admin Version"

#define THIEF_EXE                   "THIEF.EXE"
#define THIEF_ICO                   "THIEF"

#ifndef KICS
#define THIEF_NAME                  "THIEF"
#endif

#ifdef KICS
#define THIEF_NAME                  "THIEFforKICS"
#endif


// Task_Tray
#define ID_ICON1					1
#define WM_ICONCLICK				(WM_APP+0)


#define LOG_EXT                     ".LOG"
#define INI_EXT                     ".INI"
#define SYS_EXT                     ".SYS"
#define EXE_EXT                     ".EXE"
#define LOGIN_EXT                   ".CON"
#define LAYOUT_EXT                  ".WL"
#define TMP_EXT                     ".TMP"
#define WAV_EXT                     ".WAV"
#define LOG_EXT                     ".LOG"
#define PGN_EXT                     ".PGN"
#define BPGN_EXT                    ".BPGN"
#define OUT_EXT                     ".OUT"
#define BMP_EXT                     ".BMP"
#define INI_EXT                     ".INI"
#define TXT_EXT                     ".TXT"

#define FICS_REGISTRATION_URL       "http://www.freechess.org/Register/index.html"
#define THE_BUGBOARD_URL            "http://www.thebugboard.net/"
#define THE_BUGBOARD_HELP_TOPIC_URL "http://www.thief-interface.com"
#define THE_FICS_FORUM_URL          "http://ficsforum.net"

#define NO_FLAGS_SET                0
#define INIT_MENU_POS               5

#define EXPORT                      __declspec (dllexport)
#define bzero(a)                    memset(a,0,sizeof(a))

#define PLAY_BOARD_TITLE            "Playing Board"
#define BUTTON_TITLE                "Partner Communicator Buttons"
#define TELNET_TITLE                "Telnet Console"

#define SERVER_FICS                 0
#define SERVER_ICC                  1
#define SERVER_NONFICS              2
#define SERVER_ILLEGALCHAR_MSG		"Illegal Character(s) in: "

#ifndef KICS
#define ICS_SERVER_ADDRESS1         "timeseal.exe freechess.org 5000"
#endif

#ifdef KICS
#define ICS_SERVER_ADDRESS1         "timeseal.exe kics.freechess.org 6000"
#endif

#define ICS_SERVER_ADDRESS2         "timeseal.exe freechess.org 23"
#define ICS_SERVER_ADDRESS3         "timeseal.exe europe.freechess.org 5500"
#define ICS_SERVER_ADDRESS4         "timeseal.exe chess-server.spidernet.com.cy 5000"
#define ICS_SERVER_ADDRESS5         "timeseal.exe chess.unix-ag.uni-kl.de 5000"
#define ICS_SERVER_ADDRESS6         "timeseal.exe chess.mds.mdh.se 5555"
#define ICS_SERVER_ADDRESS7         "timeseal.exe global.chessparlor.com 6000"
#define ICS_SERVER_ADDRESS8         "timestamp.exe chessclub.com 5000"
#define ICS_SERVER_ADDRESS9         "timestamp.exe queen.chessclub.com 5000"
#define ICS_SERVER_ADDRESS10        "timestamp.exe chessclub.com 23"

#define WSA_READ                    (WM_USER + 0)
#define WSA_READ1                   (WM_USER + 1)
#define WM_USER_RESET_BOARD         (WM_USER + 2)
#define WM_USER_FLIP_BOARD          (WM_USER + 3)
#define WM_USER_START_TIMER         (WM_USER + 4)
#define WM_USER_STOP_TIMER          (WM_USER + 5)
#define WM_USER_RIGHT_MOUSE0        (WM_USER + 6)
#define WM_USER_RIGHT_MOUSE1        (WM_USER + 7)
#define WM_USER_RIGHT_MOUSE2        (WM_USER + 8)
#define WM_USER_RIGHT_MOUSE3        (WM_USER + 9)
#define WM_USER_RIGHT_MOUSE4        (WM_USER + 10)
#define WM_USER_RIGHT_MOUSE5        (WM_USER + 11)
#define WM_USER_RIGHT_MOUSE6        (WM_USER + 12)
#define WM_USER_RIGHT_MOUSE7        (WM_USER + 13)
#define WM_USER_RIGHT_MOUSE8        (WM_USER + 14)
#define WM_USER_RIGHT_MOUSE9        (WM_USER + 15)
#define WM_USER_RIGHT_MOUSE10       (WM_USER + 16)
#define WM_USER_RIGHT_MOUSE11       (WM_USER + 17)
#define WM_USER_RIGHT_MOUSE12       (WM_USER + 18)
#define WM_USER_RIGHT_MOUSE13       (WM_USER + 19)
#define WM_USER_RIGHT_MOUSE14       (WM_USER + 20)
#define WM_USER_RIGHT_MOUSE15       (WM_USER + 21)
#define WM_USER_RIGHT_MOUSE16       (WM_USER + 22)
#define WM_USER_RIGHT_MOUSE17       (WM_USER + 23)
#define WM_USER_RIGHT_MOUSE18       (WM_USER + 24)
#define WM_USER_RIGHT_MOUSE19       (WM_USER + 25)
#define WM_USER_RIGHT_MOUSE20       (WM_USER + 26)
#define WM_USER_RIGHT_MOUSE21       (WM_USER + 27)
#define WM_USER_RIGHT_MOUSE22       (WM_USER + 28)
#define WM_USER_RIGHT_MOUSE23       (WM_USER + 29)
#define WM_USER_RIGHT_MOUSE24       (WM_USER + 30)
#define WM_USER_RIGHT_MOUSE25       (WM_USER + 31)
#define WM_USER_RIGHT_MOUSE26       (WM_USER + 32)
#define WM_USER_RIGHT_MOUSE27       (WM_USER + 33)
#define WM_USER_RIGHT_MOUSE28       (WM_USER + 34)
#define WM_USER_RIGHT_MOUSE29       (WM_USER + 35)
#define WM_USER_RIGHT_MOUSE30       (WM_USER + 36)
#define WM_USER_RIGHT_MOUSE31       (WM_USER + 37)
#define WM_USER_RIGHT_MOUSE32       (WM_USER + 38)
#define WM_USER_RIGHT_MOUSE33       (WM_USER + 39)
#define WM_USER_RIGHT_MOUSE34       (WM_USER + 40)
#define WM_USER_RIGHT_MOUSE35       (WM_USER + 41)
#define WM_USER_RIGHT_MOUSE36       (WM_USER + 42)
#define WM_USER_RIGHT_MOUSE37       (WM_USER + 43)
#define WM_USER_RIGHT_MOUSE38       (WM_USER + 44)
#define WM_USER_RIGHT_MOUSE39       (WM_USER + 45)
#define WM_USER_RIGHT_MOUSE40       (WM_USER + 46)
#define WM_USER_RIGHT_MOUSE41       (WM_USER + 47)
#define WM_USER_RIGHT_MOUSE42       (WM_USER + 48)
#define WM_USER_RIGHT_MOUSE43       (WM_USER + 49)
#define WM_USER_RIGHT_MOUSE44       (WM_USER + 50)
#define WM_USER_RIGHT_MOUSE45       (WM_USER + 51)
#define WM_USER_RIGHT_MOUSE46       (WM_USER + 52)
#define WM_USER_RIGHT_MOUSE47       (WM_USER + 53)
#define WM_USER_RIGHT_MOUSE48       (WM_USER + 54)
#define WM_USER_RIGHT_MOUSE49       (WM_USER + 55)
#define WM_USER_RIGHT_MOUSE50       (WM_USER + 56)
#define WM_USER_MOUSELEAVE          (WM_USER + 57)

#define FUNCTION_NONE               0
#define FUNCTION_COMMAND            2
#define FUNCTION_TELNET_LAST_TELL   3
#define FUNCTION_SEEK_BOX           4
#define FUNCTION_MATCH_BOX          5
#define FUNCTION_MESSAGE_BOX        6
#define FUNCTION_TELNET_TEXT_ALL    8
#define FUNCTION_TELNET_TEXT_COPY   9
#define FUNCTION_TELNET_TEXT_CP     10
#define FUNCTION_TELNET_TEXT_SF     11
#define FUNCTION_TELNET_EDIT_ALL    12
#define FUNCTION_TELNET_EDIT_COPY   13
#define FUNCTION_TELNET_EDIT_CUT    14
#define FUNCTION_TELNET_EDIT_PASTE  15
#define FUNCTION_TELNET_EDIT_CMD    16
#define FUNCTION_TOGGLE_TELNET      18
#define FUNCTION_MAXIMIZE_TELNET    19
#define FUNCTION_RESTORE_TELNET     20
#define FUNCTION_LOAD_PGN           25
#define FUNCTION_RESET_BOARD        26
#define FUNCTION_FLIP_BOARD         27
#define FUNCTION_CLOSE_BOARD        28
#define FUNCTION_UNEXAM_GAME        40
#define FUNCTION_TOGGLE_TP          45
#define FUNCTION_DROP_PAWN          47
#define FUNCTION_DROP_KNIGHT        48
#define FUNCTION_DROP_BISHOP        49
#define FUNCTION_DROP_ROOK          50
#define FUNCTION_DROP_QUEEN         51
#define FUNCTION_SITPIECE_DROP_MOVE 52
#define FUNCTION_RESTORE_LAYOUT0    54
#define FUNCTION_RESTORE_LAYOUT1    55
#define FUNCTION_RESTORE_LAYOUT2    56
#define FUNCTION_RESTORE_LAYOUT3    57
#define FUNCTION_RESTORE_LAYOUT4    58
#define FUNCTION_RESTORE_LAYOUT5    59
#define FUNCTION_RESTORE_LAYOUT6    60
#define FUNCTION_RESTORE_LAYOUT7    61
#define FUNCTION_RESTORE_LAYOUT8    62
#define FUNCTION_TOGGLE_TIMER_ONOFF 75
#define FUNCTION_EDIT_PARTNER_BOX   93
#define FUNCTION_COPY_VARIABLE      96
#define FUNCTION_QUESTION_BOX       98
#define FUNCTION_INTERNET_BROWSER   99
#define FUNCTION_MINIMIZE_THIEF     101
#define FUNCTION_RUN_SCRIPT         102
#define FUNCTION_LOAD_PROFILE       103
#define FUNCTION_TELNET_TELL_HANDLE 108

#define DEFAULT_TIMER_MINUTE        20
#define DEFAULT_TIMER_FUNCTION      FUNCTION_COMMAND
#define DEFAULT_TIMER_STRING        "set busy is away from keyboard"
#define DEFAULT_ICC_TIMER_STRING    "away away from keyboard"

#define ICS_LAG_TOLERANCE           30
#define ICS_LAG_CHECK_THRESHOLD     300
#define ICS_LAG_FUNCTION            FUNCTION_COMMAND
#define ICS_LAG_COMMAND             "kib Lag >= 30 seconds"
#define ICS_LAG_DISPLAY             "Lag tolerance exceeded.  Issuing threshold command."

#define THIEF_CENSOR_PREFIX         "Auto Message: "
#define THIEF_CENSOR_PREFIX1        ": Auto Message: "
#define THIEF_CENSOR_PREFIX1_SIZE   16
#define THIEF_CENSOR_TELL_MESSAGE   "Sorry, tell not sent.  Censor filter in effect."
#define THIEF_CENSOR_PTELL_MESSAGE  "Sorry, ptell not sent.  Censor filter in effect."
#define THIEF_CENSOR_SAY_MESSAGE    "Sorry, say not sent.  Censor filter in effect."
#define THIEF_CENSOR_PARTNER_MESSAGE "Sorry, no partner filter in effect."
#define THIEF_CENSOR_MESSAGE_MESSAGE "Sorry, message not sent.  Censor filter in effect."
#define THIEF_CENSOR_BIT            "11111111111"

#define THIEF_SILENCE_PREFIX        "Auto Message: "
#define THIEF_SILENCE_PREFIX1       ": Auto Message: "
#define THIEF_SILENCE_PREFIX1_SIZE  16
#define THIEF_SILENCE_TELL_MESSAGE  "Sorry, not sent. User is in silence mode."

#define VAR_WHOAMI                  "%whoami%"
#define VAR_PARTNER                 "%partner%"
#define VAR_OPPONENT                "%opponent%"
#define VAR_PARTNEROPPONENT         "%partneropp%"
#define VAR_LASTTELL                "%lasttell%"
#define VAR_LASTKIB                 "%lastkib%"
#define VAR_LASTSAY                 "%lastsay%"
#define VAR_LASTWHISPER             "%lastwhisper%"
#define VAR_TELNETHANDLE            "%telnethandle%"
#define VAR_BOARDHANDLE             "%boardhandle%"
#define VAR_USERHANDLE1             "%userhandle1%"
#define VAR_USERHANDLE2             "%userhandle2%"

#define ICS_CONN_LOST_DISPLAY       "Connection Lost\n"
#define ICS_CONN_LOST_DISPLAY1      "Connection Closed by ICS\n"

#define ICS_QUIT_COMMAND            "Quit\n"
#define ICS_FLAG_COMMAND            "Flag\n"
#define ICS_UNEXAM_COMMAND          "Unexam\n"
#define ICS_EXAM_1_COMMAND          "Backward 9999\n"
#define ICS_EXAM_2_COMMAND          "Backward\n"
#define ICS_EXAM_3_COMMAND          "Forward\n"
#define ICS_EXAM_4_COMMAND          "Forward 9999\n"
#define ICS_REFRESH_COMMAND         "Refresh\n"
#define ICS_MATCH_COMMAND           "Match "
#define ICS_SEEK_COMMAND            "Seek "
#define ICS_FINGER_COMMAND          "Finger"
#define ICS_VARIABLE_COMMAND        "Var"
#define ICS_PING_COMMAND            "Ping"
#define ICS_OBSERVE_COMMAND         "Observe"
#define ICS_FOLLOW_COMMAND          "Follow"
#define ICS_PARTNER_COMMAND         "Partner"
#define ICS_HISTORY_COMMAND         "History"
#define ICS_ALL_COMMAND             "All"
#define ICS_UNOBSERVE_COMMAND       "Unobserve"
#define ICS_TELL_COMMAND            "Tell"
#define ICS_XTELL_COMMAND           "Xtell"
#define ICS_MESSAGE_COMMAND         "Message"
#define ICS_ACCEPT_COMMAND          "Accept"
#define ICS_DECLINE_COMMAND         "Decline"
#define ICS_FLAG1_COMMAND           "Flag"
#define ICS_ABORT_COMMAND           "Abort"
#define ICS_REMATCH_COMMAND         "Rematch"
#define ICS_CENSOR_TELL_COMMAND     "Xtell"
#define ICS_CLEARMESSAGE_COMMAND	"clearmessages"
#define ICS_SILENCE_TELL_COMMAND     "Xtell"

#define GAMETYPE_CHESS              0
#define GAMETYPE_ICC_WILD1          1
#define GAMETYPE_ICC_WILD2          2
#define GAMETYPE_ICC_WILD3          3
#define GAMETYPE_ICC_WILD4          4
#define GAMETYPE_WILD5              5
#define GAMETYPE_ICC_WILD6          6
#define GAMETYPE_ICC_WILD7          7
#define GAMETYPE_ICC_WILD8          8
#define GAMETYPE_ICC_WILD9          9
#define GAMETYPE_ICC_WILD10         10
#define GAMETYPE_ICC_WILD11         11
#define GAMETYPE_ICC_WILD12         12
#define GAMETYPE_ICC_WILD13         13
#define GAMETYPE_ICC_WILD14         14
#define GAMETYPE_ICC_WILD15         15
#define GAMETYPE_ICC_WILD16         16
#define GAMETYPE_LOSER              17
#define GAMETYPE_ICC_WILD18         18
#define GAMETYPE_ICC_WILD19         19
#define GAMETYPE_ICC_WILD20         20
#define GAMETYPE_ICC_WILD21         21
#define GAMETYPE_ICC_WILD22         22
#define GAMETYPE_CRAZYHOUSE         23
#define GAMETYPE_BUGHOUSE           24
#define GAMETYPE_ICC_WILD25         25
#define GAMETYPE_SUICIDE            26
#define GAMETYPE_GIVEAWAY           26
#define GAMETYPE_ICC_WILD27         27
#define GAMETYPE_ICC_WILD28         28

#define GAMETYPE_FICS_LIGHTNING     97
#define GAMETYPE_FICS_BLITZ         98
#define GAMETYPE_FICS_STANDARD      99
#define GAMETYPE_FICS_WILD0         100
#define GAMETYPE_FICS_WILD1         101
#define GAMETYPE_FICS_WILD2         102
#define GAMETYPE_FICS_WILD3         103
#define GAMETYPE_FICS_WILD4         104
#define GAMETYPE_FICS_WILD8         108
#define GAMETYPE_FICS_WILD8A        109
#define GAMETYPE_FICS_WILDFR        110
#define GAMETYPE_FICS_ATOMIC        111

#define GAMETYPE_ICC_BULLET         196
#define GAMETYPE_ICC_BLITZ          197
#define GAMETYPE_ICC_STANDARD       198
#define GAMETYPE_ICC_5_MINUTE       199
#define GAMETYPE_ICC_1_MINUTE       200

#define ICS_INITIAL_GAMETYPE        GAMETYPE_BUGHOUSE
#define ICS_INITIAL_MOVE_NAME       "none"
#define ICS_INITIAL_TIME_MOVE       "(0:00)"
#define ICS_INITIAL_WHITE_HANDLE    "WHITE"
#define ICS_INITIAL_BLACK_HANDLE    "BLACK"
#define ICS_INITIAL_WHITE_RATING    ""
#define ICS_INITIAL_BLACK_RATING    ""
#define ICS_INITIAL_RATED           -1
#define ICS_INITIAL_GAMETYPE_STRING ""
#define ICS_CASTLE_KING_SIDE_MOVE   "o-o"
#define ICS_CASTLE_QUEEN_SIDE_MOVE  "o-o-o"

#define ICS_KING_IN_CHECK_SYMBOL    '+'
#define ICS_PIECE_CAPTURE_SYMBOL    'x'
#define ICS_CASTLE_SYMBOL           'O'
#define ICS_WHITE_PAWN              'P'
#define ICS_WHITE_ROOK              'R'
#define ICS_WHITE_KNIGHT            'N'
#define ICS_WHITE_BISHOP            'B'
#define ICS_WHITE_QUEEN             'Q'
#define ICS_BLACK_PAWN              'p'
#define ICS_BLACK_ROOK              'r'
#define ICS_BLACK_KNIGHT            'n'
#define ICS_BLACK_BISHOP            'b'
#define ICS_BLACK_QUEEN             'q'
#define ICS_WHITE_KING              'K'
#define ICS_BLACK_KING              'k'
#define ICS_EMPTY_SQUARE            '-'
#define ICS_WHITE_PAWN1             "P"
#define ICS_WHITE_ROOK1             "R"
#define ICS_WHITE_KNIGHT1           "N"
#define ICS_WHITE_BISHOP1           "B"
#define ICS_WHITE_QUEEN1            "Q"
#define ICS_BLACK_PAWN1             "p"
#define ICS_BLACK_ROOK1             "r"
#define ICS_BLACK_KNIGHT1           "n"
#define ICS_BLACK_BISHOP1           "b"
#define ICS_BLACK_QUEEN1            "q"
#define ICS_WHITE_KING1             "K"
#define ICS_BLACK_KING1             "k"
#define ICS_EMPTY_SQUARE1           "-"

#define ICS_HANDLE_LENGTH           17

#define DRAG_MOVE                   0
#define CLICK_MOVE                  1
#define INVISIBLE_MOVE              2

#define DRAW_STATE_NONE             0
#define DRAW_STATE_BOARD            1
#define DRAW_STATE_BUFFER           2
#define DRAW_STATE_MOUSE_DOWN       3
#define DRAW_STATE_MOUSE_MOVE       4
#define DRAW_STATE_MOUSE_UP         5
#define DRAW_STATE_RESTORE          6
#define DRAW_STATE_NEW_GAME         7
#define DRAW_STATE_ERASE_ALL_TP     8
#define DRAW_STATE_DRAW_ILLEGAL_TP  9

#define VECTOR_TOUPS                0
#define VECTOR_NICOK                1
#define VECTOR_KBIRK                2
#define VECTOR_JHURT                3
#define VECTOR_ECHUR                4
#define VECTOR_DFONG                5
#define VECTOR_USER                 6

#define PREMOVE_NONE                0
#define PREMOVE_THIEF               1
#define PREMOVE_LR                  2

#define TENTH_NEVER                 0
#define TENTH_ALWAYS                1
#define TENTH_ONE_MINUTE            2

#define DEFAULT_LOG_TELNET          0
#define DEFAULT_LOG_GAME            0
#define DEFAULT_SAVE_PLAY_GAME      0
#define DEFAULT_SAVE_OBS_GAME       0
#define DEFAULT_SAVE_UNOBS_GAME     0
#define DEFAULT_SAVE_ABORT_GAME     0
#define DEFAULT_SAVE_NO_BFEN_GAME   1
#define DEFAULT_SAVE_DAILY          1
#define DEFAULT_AUTO_FLAG           1
#define DEFAULT_AUTO_REMATCH        0
#define DEFAULT_AUTO_OBSERVE        1
#define DEFAULT_AUTO_OBSERVE_GNOTIFY        0
#define DEFAULT_ACTUAL_SIZE         0
#define DEFAULT_STOP_PLAY_RESIZE    0
#define DEFAULT_ESC_CANCEL_DRAG_MOVE        1
#define DEFAULT_ESC_CANCEL_TRUE_PREMOVE     1
#define DEFAULT_ESC_CANCEL_SIT_DROP_MOVE    1
#define DEFAULT_ESC_CANCEL_PROMOTE_KNIGHT   1
#define DEFAULT_NO_PLAY_ABUSER      0
#define DEFAULT_NO_PLAY_COMPUTER    0
#define DEFAULT_WILD5_PLAY_TOP      0
#define DEFAULT_WILD5_PLAY_BOTTOM   1
#define DEFAULT_WILD5_PLAY_BBOTTOM  2
#define DEFAULT_WILD5_PLAY_WBOTTOM  3
#define DEFAULT_WILD5_PLAY_DEFAULT  DEFAULT_WILD5_PLAY_TOP
#define DEFAULT_SHOW_RESULT         1
#define DEFAULT_SHOW_RATING         1
#define DEFAULT_SHOW_LAGSTAT        1
#define DEFAULT_SHOW_PTELL          1
#define DEFAULT_SHOW_COORDINATES    0
#define DEFAULT_SHOW_GAMETYPE       1
#define DEFAULT_SHOW_GAMETYPE_TITLE 0
#define DEFAULT_SHOW_VALUE_TITLE    0
#define DEFAULT_REDCLOCK_SECOND     10
#define DEFAULT_SHOW_TENTHSECOND    TENTH_NEVER
#define DEFAULT_SHOW_ONESECOND      1
#define DEFAULT_SHOW_LASTMOVE       1
#define DEFAULT_SHOW_LASTMOVE_TENTH 0
#define DEFAULT_SHOW_HIGHLIGHT      1
#define DEFAULT_TRUEMOVE_HIGHLIGHT  1
#define DEFAULT_SHOW_CLOCK_ON_TOP   0
#define DEFAULT_KING_HIGHLIGHT_PM   0
#define DEFAULT_KING_HIGHLIGHT_PO   0
#define DEFAULT_KING_HIGHLIGHT_E    0
#define DEFAULT_KING_HIGHLIGHT_O    0
#define DEFAULT_SHOW_CAPTURED_CHESS 0
#define DEFAULT_BUFFER_LEFT         0
#define DEFAULT_BUFFER_RIGHT        1
#define DEFAULT_BUFFER_TOPBOTTOML   2
#define DEFAULT_BUFFER_TOPBOTTOMR   3
#define DEFAULT_BUFFER_ORIENTATION  DEFAULT_BUFFER_RIGHT
#define DEFAULT_VECTOR_PIECE_SET    VECTOR_TOUPS
#define DEFAULT_AUTO_QUEEN          0
#define DEFAULT_MOVE_TYPE           DRAG_MOVE
#define DEFAULT_AUTO_CENTER_DRAG_PC 1
#define DEFAULT_SMART_MOVE          0
#define DEFAULT_SMART_MOVE1         0
#define DEFAULT_SMART_MOVE2         0
#define DEFAULT_SMART_MOVE_ONBUTTONUP 0
#define DEFAULT_TRUE_PREMOVE        0
#define DEFAULT_PREMOVE_TYPE        PREMOVE_NONE
#define DEFAULT_ILLEGAL_TP_BEEP     1
#define DEFAULT_RIGHT_MOUSE_PLAY    1
#define DEFAULT_RIGHT_MOUSE_PARTNER 1
#define DEFAULT_TEST_LEGAL_PLAY     1
#define DEFAULT_TEST_LEGAL_PARTNER  1
#define DEFAULT_ANIMATE_OBSERVE     1
#define DEFAULT_ANIMATE_EXAM        1
#define DEFAULT_ANIMATE_PLAY        0
#define DEFAULT_ANIMATE_SPEED       0
#define DEFAULT_LONG_PARTNER        0
#define DEFAULT_PARTNER_MOVE1       ""
#define DEFAULT_PARTNER_MOVE2       "Wait for"
#define DEFAULT_PARTNER_MOVE3       "Watch out for"
#define DEFAULT_PARTNER_MOVE4       "Watch out for"
#define DEFAULT_SHOW_MOVE_BUTTON    1
#define DEFAULT_ARROW_MOVE_BUTTON   0

#define DEFAULT_MOUSE_WHEEL         0
#define DEFAULT_SHOW_UNDERLINE	    1
#define DEFAULT_TELNET_HIGHLIGHT    0
#define DEFAULT_TELNET_GO_BOTTOM    1
#define DEFAULT_TELNET_NO_ANNOUNCE  0
#define DEFAULT_TELNET_GO_CKIBITZ   0
#define DEFAULT_TELNET_NO_CWHISPER  0
#define DEFAULT_TELNET_NO_IGNOREFOR 0
#define DEFAULT_TELNET_NO_ROBOWELCOME 0
#define DEFAULT_TELNET_NO_COMP_CHTELL 0
#define DEFAULT_TELNET_NO_COMP_TELL 0
#define DEFAULT_TELNET_NO_COMP_SHOUT 0
#define DEFAULT_TELNET_CONSOLE_TIMESTAMP 0
#define DEFAULT_TELNET_LOG_TIMESTAMP 1

#define DEFAULT_LOGIN_MAX_TELNET    0

#define DEFAULT_SOUND               1
#define DEFAULT_TELNET_SOUND        1
#define DEFAULT_GAME_SOUND          1
#define DEFAULT_EAR_SOUND           1
#define DEFAULT_FINAL_COUNTDOWN     1

#define DEFAULT_LAG_COMMAND         1
#define DEFAULT_CENSOR              0
#define DEFAULT_SAVE_ON_EXIT        1
#define DEFAULT_MINIMIZE_TO_TRAY    0
#define DEFAULT_SHOW_BITMAP_SIZE    0
#define DEFAULT_SHOW_MENU_PATH      0

#define DEFAULT_SILENCE				0

#define DEFAULT_EXTENDED_BUTTON     0

#define DEFAULT_MATCH_TIME          3
#define DEFAULT_MATCH_INCREMENT     0
#define DEFAULT_MATCH_RATED         1
#define DEFAULT_MATCH_TYPE          0
#define DEFAULT_ICC_MATCH_TYPE      0
#define DEFAULT_MATCH_COLOR         0
#define DEFAULT_SEEK_TIME           3
#define DEFAULT_SEEK_INCREMENT      0
#define DEFAULT_SEEK_RATING1        0
#define DEFAULT_SEEK_RATING2        9999
#define DEFAULT_SEEK_TYPE           0
#define DEFAULT_ICC_SEEK_TYPE       0
#define DEFAULT_SEEK_COLOR          0
#define DEFAULT_SEEK_MANUAL         0
#define DEFAULT_SEEK_FORMULA        0
#define DEFAULT_SEEK_RATED          1

#define THIEF_COMMAND_SIZE          1024
#define THIEF_FILE_SIZE             _MAX_PATH

#define MAX_SERVER                  50
#define MAX_SERVER_SIZE             255
#define MAX_SERVER_PORT_SIZE        5
#define MAX_SERVER_PROMPT_SIZE      21

#define MAX_APP_SPAWN               2048

#define MAX_GAME                    20
#define TEMP_GAME_INDEX             MAX_GAME
#define MAX_COORD                   MAX_GAME + 3

#define MAX_LAYOUT                  9

#define MAX_TRUE_PREMOVE            1

#define MAX_BUTTON                  54
#define HALF_BUTTON                 27
#define HALF_REGULAR_BUTTON         15
#define ONE_THIRD_BUTTON            18
#define EXTEND_BUTTON               30

#define MAX_BUTTON_TEXT             255
#define MAX_BUTTON_ACTION           THIEF_COMMAND_SIZE

// When changing DEFAULT_CHESS_BUTTONx_ACTION make sure to adjust nType[x] (no default definitions for that)

#define DEFAULT_CHESS_BUTTONS_USED         29

#define DEFAULT_CHESS_BUTTON0_TEXT         "Draw"
#define DEFAULT_CHESS_BUTTON0_ACTION       "Draw"
#define DEFAULT_CHESS_BUTTON1_TEXT         "Resign"
#define DEFAULT_CHESS_BUTTON1_ACTION       "Resign"
#define DEFAULT_CHESS_BUTTON2_TEXT         "Abort"
#define DEFAULT_CHESS_BUTTON2_ACTION       "Abort"
#define DEFAULT_CHESS_BUTTON3_TEXT         "Adjourn"
#define DEFAULT_CHESS_BUTTON3_ACTION       "Adjourn"
#define DEFAULT_CHESS_BUTTON4_TEXT         "Match"
#define DEFAULT_CHESS_BUTTON4_ACTION       "Match"
#define DEFAULT_CHESS_BUTTON5_TEXT         "Rematch"
#define DEFAULT_CHESS_BUTTON5_ACTION       "Rematch"
#define DEFAULT_CHESS_BUTTON6_TEXT         "Accept"
#define DEFAULT_CHESS_BUTTON6_ACTION       "Accept"
#define DEFAULT_CHESS_BUTTON7_TEXT         "Decline"
#define DEFAULT_CHESS_BUTTON7_ACTION       "Decline t all"
#define DEFAULT_CHESS_BUTTON8_TEXT         "Seek"
#define DEFAULT_CHESS_BUTTON8_ACTION       "Seek"
#define DEFAULT_CHESS_BUTTON9_TEXT         "Sought"
#define DEFAULT_CHESS_BUTTON9_ACTION       "Sought"
#define DEFAULT_CHESS_BUTTON10_TEXT         "Unseek"
#define DEFAULT_CHESS_BUTTON10_ACTION       "Unseek"
#define DEFAULT_CHESS_BUTTON11_TEXT        "Withdraw"
#define DEFAULT_CHESS_BUTTON11_ACTION      "Withdraw t all"
#define DEFAULT_CHESS_BUTTON12_TEXT        "Pending"
#define DEFAULT_CHESS_BUTTON12_ACTION      "pending"
#define DEFAULT_CHESS_BUTTON13_TEXT        "Open"
#define DEFAULT_CHESS_BUTTON13_ACTION      "set open"
#define DEFAULT_CHESS_BUTTON14_TEXT        "0-0"
#define DEFAULT_CHESS_BUTTON14_ACTION      "0-0"
#define DEFAULT_CHESS_BUTTON15_TEXT        "0-0-0"
#define DEFAULT_CHESS_BUTTON15_ACTION      "0-0-0"
#define DEFAULT_CHESS_BUTTON16_TEXT        "Examine"
#define DEFAULT_CHESS_BUTTON16_ACTION      "Examine"
#define DEFAULT_CHESS_BUTTON17_TEXT        "Unexamine"
#define DEFAULT_CHESS_BUTTON17_ACTION      "Unexamine"
#define DEFAULT_CHESS_BUTTON18_TEXT        "Allob me"
#define DEFAULT_CHESS_BUTTON18_ACTION      "ame"
#define DEFAULT_CHESS_BUTTON19_TEXT        "Assess"
#define DEFAULT_CHESS_BUTTON19_ACTION      "Assess"
#define DEFAULT_CHESS_BUTTON20_TEXT        "Tournaments"
#define DEFAULT_CHESS_BUTTON20_ACTION      "mam lt"
#define DEFAULT_CHESS_BUTTON21_TEXT        "T. Standings"
#define DEFAULT_CHESS_BUTTON21_ACTION      "mam standings"
#define DEFAULT_CHESS_BUTTON22_TEXT        "T. Games"
#define DEFAULT_CHESS_BUTTON22_ACTION      "mam games"
#define DEFAULT_CHESS_BUTTON23_TEXT        "T. Players"
#define DEFAULT_CHESS_BUTTON23_ACTION      "mam players"
#define DEFAULT_CHESS_BUTTON24_TEXT        "T. Info"
#define DEFAULT_CHESS_BUTTON24_ACTION      "mam info"
#define DEFAULT_CHESS_BUTTON25_TEXT        "Pr. Queen"
#define DEFAULT_CHESS_BUTTON25_ACTION      "Promote queen"
#define DEFAULT_CHESS_BUTTON26_TEXT        "Pr. Rook"
#define DEFAULT_CHESS_BUTTON26_ACTION      "Promote rook"
#define DEFAULT_CHESS_BUTTON27_TEXT        "Pr. Bishop"
#define DEFAULT_CHESS_BUTTON27_ACTION      "Promote bishop"
#define DEFAULT_CHESS_BUTTON28_TEXT        "Pr. knight"
#define DEFAULT_CHESS_BUTTON28_ACTION      "promote knight"
#define DEFAULT_CHESS_BUTTON29_TEXT        "Pr. King"
#define DEFAULT_CHESS_BUTTON29_ACTION      "promote King"
#define DEFAULT_CHESS_BUTTON30_TEXT        ""
#define DEFAULT_CHESS_BUTTON30_ACTION      ""
#define DEFAULT_CHESS_BUTTON31_TEXT        ""
#define DEFAULT_CHESS_BUTTON31_ACTION      ""
#define DEFAULT_CHESS_BUTTON32_TEXT        ""
#define DEFAULT_CHESS_BUTTON32_ACTION      ""
#define DEFAULT_CHESS_BUTTON33_TEXT        ""
#define DEFAULT_CHESS_BUTTON33_ACTION      ""
#define DEFAULT_CHESS_BUTTON34_TEXT        ""
#define DEFAULT_CHESS_BUTTON34_ACTION      ""
#define DEFAULT_CHESS_BUTTON35_TEXT        ""
#define DEFAULT_CHESS_BUTTON35_ACTION      ""
#define DEFAULT_CHESS_BUTTON36_TEXT        ""
#define DEFAULT_CHESS_BUTTON36_ACTION      ""
#define DEFAULT_CHESS_BUTTON37_TEXT        ""
#define DEFAULT_CHESS_BUTTON37_ACTION      ""
#define DEFAULT_CHESS_BUTTON38_TEXT        ""
#define DEFAULT_CHESS_BUTTON38_ACTION      ""
#define DEFAULT_CHESS_BUTTON39_TEXT        ""
#define DEFAULT_CHESS_BUTTON39_ACTION      ""
#define DEFAULT_CHESS_BUTTON40_TEXT        ""
#define DEFAULT_CHESS_BUTTON40_ACTION      ""
#define DEFAULT_CHESS_BUTTON41_TEXT        ""
#define DEFAULT_CHESS_BUTTON41_ACTION      ""
#define DEFAULT_CHESS_BUTTON42_TEXT        ""
#define DEFAULT_CHESS_BUTTON42_ACTION      ""
#define DEFAULT_CHESS_BUTTON43_TEXT        ""
#define DEFAULT_CHESS_BUTTON43_ACTION      ""
#define DEFAULT_CHESS_BUTTON44_TEXT        ""
#define DEFAULT_CHESS_BUTTON44_ACTION      ""
#define DEFAULT_CHESS_BUTTON45_TEXT        ""
#define DEFAULT_CHESS_BUTTON45_ACTION      ""
#define DEFAULT_CHESS_BUTTON46_TEXT        ""
#define DEFAULT_CHESS_BUTTON46_ACTION      ""
#define DEFAULT_CHESS_BUTTON47_TEXT        ""
#define DEFAULT_CHESS_BUTTON47_ACTION      ""
#define DEFAULT_CHESS_BUTTON48_TEXT        ""
#define DEFAULT_CHESS_BUTTON48_ACTION      ""
#define DEFAULT_CHESS_BUTTON49_TEXT        ""
#define DEFAULT_CHESS_BUTTON49_ACTION      ""
#define DEFAULT_CHESS_BUTTON50_TEXT        ""
#define DEFAULT_CHESS_BUTTON50_ACTION      ""
#define DEFAULT_CHESS_BUTTON51_TEXT        ""
#define DEFAULT_CHESS_BUTTON51_ACTION      ""
#define DEFAULT_CHESS_BUTTON52_TEXT        ""
#define DEFAULT_CHESS_BUTTON52_ACTION      ""
#define DEFAULT_CHESS_BUTTON53_TEXT        ""
#define DEFAULT_CHESS_BUTTON53_ACTION      ""

#define MAX_MENU                    2
#define MAX_WINDOW                  7
#define MAX_COLOR                   61
#define MAX_CHANNEL_COLOR           401
#define MAX_PEN                     13
#define MAX_BRUSH                   7
#define MAX_FONT                    9
#define MAX_FONT_NAME               255
#define MAX_SOUND                   18
#define MAX_SOUND_NAME              THIEF_FILE_SIZE
#define MAX_EAR                     70
#define MAX_EAR_TELL                255
#define MAX_EAR_NAME                THIEF_FILE_SIZE
#define MAX_GAME_SOUND              35
#define MAX_GAME_SOUND_NAME         THIEF_FILE_SIZE
#define MAX_TIME_SOUND              22
#define MAX_TIME_SOUND_NAME         THIEF_FILE_SIZE
#define PARTNER_TIME_SOUND_START    11

#define MAX_LOGIN_EVENT              8
#define MAX_LOGIN_EVENT_COMMAND_SIZE THIEF_COMMAND_SIZE

#define MAX_GAME_EVENT              10
#define MAX_GAME_EVENT_COMMAND_SIZE THIEF_COMMAND_SIZE

#define MAX_RM                      6
#define MAX_RM_ITEM                 40
#define MAX_RM_MENU_SIZE            50
#define MAX_RM_COMMAND_SIZE         THIEF_COMMAND_SIZE

#define MAX_MOVE                    2000
#define MAX_MOVE_STRING_SIZE        20

#define MAX_POSITION                1000
#define MAX_BUGHOUSE                1000
#define MAX_NO_DROP_MOVE            3

#define MOVE_NORMAL                 0
#define MOVE_DROP                   1
#define MOVE_OO                     2
#define MOVE_OOO                    3
#define MOVE_DBLPUSH                4
#define MOVE_ENPASANT               5
#define MOVE_PROMOTE                6

#define MAX_HISTORY                 500

#define MAX_LOGIN                   10
#define MAX_LOGIN_SIZE              255
#define MAX_LOGIN_NAME_SIZE         255
#define MAX_LOGIN_ADDRESS_SIZE      255
#define MAX_LOGIN_PORT_SIZE         MAX_SERVER_PORT_SIZE
#define MAX_LOGIN_PROMPT_SIZE       MAX_SERVER_PROMPT_SIZE
#define MAX_LOGIN_HANDLE_SIZE       18
#define MAX_LOGIN_PASSWORD_SIZE     17
#define MAX_LOGIN_SERVER_NAME_SIZE  20

#define MAX_FMENU                   109
#define MAX_FUNCTION_KEY            11
#define MAX_FUNCTION_KEY_SIZE       THIEF_COMMAND_SIZE
#define MAX_CONTROL_KEY             36
#define MAX_CONTROL_KEY_SIZE        THIEF_COMMAND_SIZE

#define MAX_LAG_COMMAND_SIZE        THIEF_COMMAND_SIZE
#define MAX_LAG_DISPLAY_SIZE        70

#define MAX_CENSOR                  3
#define MAX_CENSOR_ITEM             100
#define MAX_CENSOR_BIT              12
#define CENSOR_COMMAND_SIZE         THIEF_COMMAND_SIZE

#define SILENCE_COMMAND_SIZE		THIEF_COMMAND_SIZE

#define MAX_TELL_HANDLE             30

#define MAX_PARTNER_ASSIST_SIZE     255

#define MAX_ANIMATE_SPEED           10

#define MENU_INIT                   0
#define MENU_INIT_WINDOW            1

#define HWND_FRAME                  0
#define HWND_CLIENT                 1
#define HWND_TELNET                 2
#define HWND_TELNET_TEXT            3
#define HWND_TELNET_EDIT            4
#define HWND_BUTTON                 5
#define HWND_PLAY                   6

#define CLR_WINDOW_COLOR            0
#define CLR_BOARD_OUTLINE           1
#define CLR_BUFFER_OUTLINE          2
#define CLR_BUFFER_FILL             3
#define CLR_WHITE_SQUARE_OUTLINE    4
#define CLR_WHITE_SQUARE_FILL       5
#define CLR_BLACK_SQUARE_OUTLINE    6
#define CLR_BLACK_SQUARE_FILL       7
#define CLR_PIECE_NUMBER            8
#define CLR_WHITE_PIECE_OUTLINE     9
#define CLR_WHITE_PIECE_FILL        10
#define CLR_BLACK_PIECE_OUTLINE     11
#define CLR_BLACK_PIECE_FILL        12

#define CLR_HANDLE                  13

#define CLR_CLOCK_ON                14
#define CLR_CLOCK_ON_BACKGROUND     15
#define CLR_CLOCK_OFF               16
#define CLR_LAGSTAT                 17
#define CLR_CLOCK_TIMEDOWN          18
#define CLR_CLOCK_TIMEUP            19

#define CLR_GAMETYPE                20
#define CLR_LASTMOVE                21
#define CLR_HIGHLIGHT               22
#define CLR_TRUE_PREMOVE_HIGHLIGHT  23
#define CLR_ILLEGAL_TRUE_PREMOVE    24
#define CLR_KING_HIGHLIGHT          25

#define CLR_RESULT_FOREGROUND       26
#define CLR_RESULT_BACKGROUND       27
#define CLR_PTELL_FOREGROUND        28
#define CLR_PTELL_BACKGROUND        29
#define CLR_GINFO_FOREGROUND        30
#define CLR_GINFO_BACKGROUND        31

#define CLR_COORDINATES_FOREGROUND  32

#define CLR_TELNET_NORMAL           33
#define CLR_TELNET_TELL             34
#define CLR_TELNET_PTELL            35
#define CLR_TELNET_SAY              36
#define CLR_TELNET_KIBITZ           37
#define CLR_TELNET_WHISPER          38
#define CLR_TELNET_SHOUT            39
#define CLR_TELNET_CSHOUT           40
#define CLR_TELNET_CHALLENGE        41
#define CLR_TELNET_USERTEXT         42
#define CLR_TELNET_BACKGROUND       43
#define CLR_TELNET_NOTIFICATION     44
#define CLR_TELNET_COLON_NOTIFY     45
#define CLR_TELNET_ARRIVAL          46
#define CLR_TELNET_DEPARTURE        47
#define CLR_TELNET_GAME_NOTIFY      48
#define CLR_TELNET_ANNOUNCEMENT     49
#define CLR_TELNET_COLON            50
#define CLR_TELNET_COLON_256        51
#define CLR_TELNET_COLON_C2         52
#define CLR_TELNET_COLON_T3         53
#define CLR_TELNET_COLON_TSHOUTS    54
#define CLR_TELNET_COLON_MAMER      55
#define CLR_TELNET_CHANNEL          56
#define CLR_TELNET_CHANNELA         57
#define CLR_TELNET_CHANNEL1         58
#define CLR_TELNET_CHANNEL24        59
#define CLR_TELNET_CHANNEL97        60

#define PEN_BOARD_OUTLINE           0
#define PEN_BUFFER_OUTLINE          1
#define PEN_PIECE_NUMBER_OUTLINE    2
#define PEN_WHITE_SQUARE_OUTLINE    3
#define PEN_BLACK_SQUARE_OUTLINE    4
#define PEN_WHITE_PIECE_OUTLINE     5
#define PEN_BLACK_PIECE_OUTLINE     6
#define PEN_HIGHLIGHT               7
#define PEN_TRUEPREMOVE             8
#define PEN_ILLEGAL_TRUEPREMOVE     9
#define PEN_KING_HIGHLIGHT          10
#define PEN_WHITE                   11
#define PEN_BLACK                   12

#define BRUSH_BUFFER_FILL           0
#define BRUSH_WHITE_SQUARE_FILL     1
#define BRUSH_BLACK_SQUARE_FILL     2
#define BRUSH_WHITE_PIECE_FILL      3
#define BRUSH_BLACK_PIECE_FILL      4
#define BRUSH_WHITE                 5
#define BRUSH_BLACK                 6

#define FONT_HANDLE                 0
#define FONT_CLOCK                  1
#define FONT_GAMETYPE               2
#define FONT_LASTMOVE               3
#define FONT_LAGSTAT                4
#define FONT_RESULT                 5
#define FONT_TELNET                 6
#define FONT_BUTTON                 7
#define FONT_COORDINATES            8

#define WRAP_INDENT                 200

#define INDEX_PLAY                  0
#define COORD_PLAY                  INDEX_PLAY
#define COORD_MAIN                  MAX_GAME
#define COORD_TELNET                MAX_GAME + 1
#define COORD_BUTTON                MAX_GAME + 2

#define SOUND_NONE                  0
#define SOUND_TELL                  1
#define SOUND_PTELL                 2
#define SOUND_SAY                   3
#define SOUND_KIBITZ                4
#define SOUND_WHISPER               5
#define SOUND_SHOUT                 6
#define SOUND_CSHOUT                7
#define SOUND_CHALLENGE             8
#define SOUND_ABORT_REQUEST         9
#define SOUND_PARTNER               10
#define SOUND_NOTIFICATION          11
#define SOUND_ARRIVAL               12
#define SOUND_DEPARTURE             13
#define SOUND_GAME_NOTIFY           14
#define SOUND_TAKEBACK_REQUEST		15
#define SOUND_DRAW_REQUEST			16
#define SOUND_ADJOURN_REQUEST		17

#define GAME_SOUND_PLAY_START       0
#define GAME_SOUND_PLAY_MOVE        1
#define GAME_SOUND_PLAY_MOVE_SMART  2
#define GAME_SOUND_PLAY_MOVE_TP     3
#define GAME_SOUND_PLAY_CAPTURE     4
#define GAME_SOUND_PLAY_CASTLE      5
#define GAME_SOUND_PLAY_CHECK       6
#define GAME_SOUND_PLAY_OVER        7
#define GAME_SOUND_PARTNER_MOVE     8
#define GAME_SOUND_PARTNER_CAPTURE  9
#define GAME_SOUND_PARTNER_CASTLE   10
#define GAME_SOUND_PARTNER_CHECK    11
#define GAME_SOUND_OBS_MOVE1        12
#define GAME_SOUND_OBS_CAPTURE1     13
#define GAME_SOUND_OBS_CASTLE1      14
#define GAME_SOUND_OBS_CHECK1       15
#define GAME_SOUND_OBS_OVER1        16
#define GAME_SOUND_OBS_MOVE2        17
#define GAME_SOUND_OBS_CAPTURE2     18
#define GAME_SOUND_OBS_CASTLE2      19
#define GAME_SOUND_OBS_CHECK2       20
#define GAME_SOUND_OBS_OVER2        21
#define GAME_SOUND_I_WON            22
#define GAME_SOUND_I_LOST           23
#define GAME_SOUND_I_DREW           24
#define GAME_SOUND_I_ABORT          25
#define GAME_SOUND_PARTNER_WON      26
#define GAME_SOUND_PARTNER_LOST     27
#define GAME_SOUND_PARTNER_DREW     28
#define GAME_SOUND_PARTNER_ABORT    29
#define GAME_SOUND_EXAM_MOVE        30
#define GAME_SOUND_EXAM_CAPTURE     31
#define GAME_SOUND_EXAM_CASTLE      32
#define GAME_SOUND_EXAM_CHECK       33
#define GAME_SOUND_GAME_ABORT       34

#define TIMER_COMMAND_ID            53
#define TIMER_CLOCK_ID              54
#define TIMER_CLOCK_ID1             55
#define TIMER_CLOCK_ID2             56
#define TIMER_CLOCK_ID3             57
#define TIMER_CLOCK_ID4             58
#define TIMER_CLOCK_ID5             59
#define TIMER_CLOCK_ID6             60
#define TIMER_CLOCK_ID7             61
#define TIMER_CLOCK_ID8             62
#define TIMER_CLOCK_ID9             63

#define GAME_EVENT_PLAY_START1      0
#define GAME_EVENT_PLAY_START2      1
#define GAME_EVENT_PLAY_BUG_START   2
#define GAME_EVENT_PLAY_ONE_START   3
#define GAME_EVENT_PLAY_OVER1       4
#define GAME_EVENT_PLAY_OVER2       5
#define GAME_EVENT_OBSERVE_START    6
#define GAME_EVENT_OBSERVE_BUG_ST   7
#define GAME_EVENT_OBSERVE_ONE_ST   8
#define GAME_EVENT_OBSERVE_OVER     9

#define RIGHT_MOUSE_BUTTON          0
#define RIGHT_MOUSE_PLAY            1
#define RIGHT_MOUSE_OBSERVE         2
#define RIGHT_MOUSE_TELNET_TEXT     3
#define RIGHT_MOUSE_TELNET_EDIT     4
#define RIGHT_MOUSE_BOARD_HANDLE    5

#define WIN_SHOW                    0
#define WIN_HIDE                    1

#define WIN_NORMAL                  0
#define WIN_MINIMIZE                1
#define WIN_MAXIMIZE                2

#define CO_MAX                      100000
#define CO_TRIM                     50000
#define EDIT_LINE_SIZE              2048
#define MSG_SIZE                    512

#define MAX_TIMER_LINE_SIZE         THIEF_COMMAND_SIZE

#define MAX_SOCKET_BUFFER_SIZE      20000

#define SOUND_FILT                  "Wave Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0"
#define CONNECTION_FILT             "Connection Files (*.con)\0*.con\0All Files (*.*)\0*.*\0"
#define SCRIPT_FILT                 "Script Files (*.scr)\0*.scr\0All Files (*.*)\0*.*\0"
#define HELPER_FILT                 "Helper Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0"
#define PGN_VIEWER_FILT             "PGN Viewer Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0"
#define BPGN_VIEWER_FILT            "BPGN Viewer Files (*.exe)\0*.exe\0All Files (*.*)\0*.*\0"
#define LOG_FILT                    "Log Files (*.log)\0*.log\0All Files (*.*)\0*.*\0"
#define GAME_FILT                   "All Files (*.*)\0*.*\0PGN Files (*.pgn)\0*.pgn\0BPGN Files (*.bpgn)\0*.bpgn\0"
#define PGN_FILT                    "PGN Files (*.pgn)\0*.pgn\0All Files (*.*)\0*.*\0"
#define BPGN_FILT                   "BPGN Files (*.bpgn)\0*.bpgn\0All Files (*.*)\0*.*\0"
#define INI_FILT                    "Ini Files (*.ini)\0*.ini\0All Files (*.*)\0*.*\0"
#define BMP_FILT                    "Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0"

#define BELL_CHAR                   '\007'
#define NULL_CHAR                   '\000'

#define INIT_BOARD                  "rnbqkbnr pppppppp -------- -------- -------- -------- PPPPPPPP RNBQKBNR"
#define INIT_WILD5_BOARD            "RNBKQBNR PPPPPPPP -------- -------- -------- -------- pppppppp rnbkqbnr"
#define INIT_FICS_WILD0_BOARD       "rnbkqbnr pppppppp -------- -------- -------- -------- PPPPPPPP RNBQKBNR"
#define INIT_FICS_WILD8_BOARD       "rnbqkbnr -------- -------- pppppppp PPPPPPPP -------- -------- RNBQKBNR"
#define INIT_FICS_WILD8A_BOARD      "rnbkqbnr -------- -------- PPPPPPPP pppppppp -------- -------- RNBKQBNR"
#define INIT_ICC_WILD7_BOARD        "----k--- -----ppp -------- -------- -------- -------- PPP----- ---K----"
#define INIT_ICC_WILD8_BOARD        "rnbqkbnr -------- -------- pppppppp PPPPPPPP -------- -------- RNBQKBNR"
#define INIT_ICC_WILD9_BOARD        "rnbqkknr pppppppp -------- -------- -------- -------- PPPPPPPP RNBQKKNR"
#define INIT_ICC_WILD10_BOARD       "rnbqkbnr ppppp-pp -------- -------- -------- -------- PPPPPPPP RNBQKBNR"
#define INIT_ICC_WILD11_BOARD       "rnbqkbnr pppppppp -------- -------- -------- -------- PPPPPPPP R-BQKBNR"
#define INIT_ICC_WILD12_BOARD       "rnbqkbnr pppppppp -------- -------- -------- -------- PPPPPPPP -NBQKBNR"
#define INIT_ICC_WILD13_BOARD       "rnbqkbnr pppppppp -------- -------- -------- -------- PPPPPPPP RNB-KBNR"
#define INIT_ICC_WILD14_BOARD       "rnbqkbnr pppppppp -------- -------- -------- P------- -PPPPPPP -NBQKBNR"
#define INIT_ICC_WILD15_BOARD       "BK------ -N------ -k------ -------- -------- -------- -------- --------"
#define INIT_ICC_WILD18_BOARD       "qqqqkqqq pppppppp -------- -------- -------- -------- PPPPPPPP QQQQKQQQ"
#define INIT_ICC_WILD19_BOARD       "-------- ------k- ----p--- ----N--- -------- ------K- -------N --------"
#define INIT_ICC_WILD28_BOARD       "rnbkqbnr pppppppp -------- -------- -------- -------- PPPPPPPP RNBKQBNR"
#define INIT_BUFFER                 ""
#define INIT_FEN                    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#define RESULT_WHITE_WINS           "1-0"
#define RESULT_BLACK_WINS           "0-1"
#define RESULT_DRAW                 "1/2-1/2"
#define RESULT_UNKNOWN              "*"

#define ORG_SIZE                    48

#define MAX_CLIPS                   300
#define MAX_PIECE                   12
#define MAX_SQUARE                  3
#define MAX_BUFFER                  10
#define DEFAULT_TRANSPARENT_COLOR0  RGB (255, 255, 255)
#define DEFAULT_TRANSPARENT_COLOR1  RGB (255, 255,   0)
#define DEFAULT_TRANSPARENT_COLOR2  RGB (255,   0,   0)
#define DEFAULT_TRANSPARENT_COLOR3  RGB (  0,   0, 255)
#define DEFAULT_TRANSPARENT_COLOR4  RGB (  0, 255, 255)

#define WHITE_PAWN                  0
#define WHITE_ROOK                  1
#define WHITE_KNIGHT                2
#define WHITE_BISHOP                3
#define WHITE_QUEEN                 4
#define BLACK_PAWN                  5
#define BLACK_ROOK                  6
#define BLACK_KNIGHT                7
#define BLACK_BISHOP                8
#define BLACK_QUEEN                 9
#define WHITE_KING                  10
#define BLACK_KING                  11
#define EMPTY_SQUARE                12

#define INDEX_WHITE                 0
#define INDEX_BLACK                 1
#define INDEX_BUFFER                2

#define MAX_UNDERLINE_STRING        23
#define MAX_BRACKET_STRING          1
#define MAX_MAMER_STRING            196

#define MAX_GNOTIFY_HANDLE          30

typedef struct
{
	long nSec ; // assuming this is >= 32 bits
	int nMs ;   // assuming this is >= 16 bits
}
TIMEMARK;

typedef struct
{
	HFONT hfFont;
	LOGFONT lf;
	char Name [MAX_FONT_NAME + 10];
	int nPointSize;
	int bBold;
	int bItalic;
	int bUnderLine;
	int bStrikeOut;
}
FONTRECORD;

typedef struct
{
	int nBoard [2] [8] [8];
	int nBuffer [2] [MAX_BUFFER];
	char cLastMove [2] [50];
	char cTime4Move [2] [50];
	long nTime [2];
}
POSITION;

typedef struct
{
	HINSTANCE hGame ;                   // instanace
	HWND      hwnd ;                    // game window handle
	HWND      hwnd1 ;                   // move button 1 window handle
	HWND      hwnd2 ;                   // move button 2 window handle
	HWND      hwnd3 ;                   // move button 3 window handle
	HWND      hwnd4 ;                   // move button 4 window handle
	HBRUSH    hbBackground ;            // window background brush
	int       nState ;                  // window state (normal, miminized, maxmized)
	int       bFirstResize ;            // first time it issued WM_SIZE command?
	int       bFirstGame ;              // first time for this new game?
	int       bValid ;                  // valid window?
	int       bFlip ;                   // flip board?

	int       nGameNumber ;             // game number
	int       nGamePartner ;            // partner's game number
	int       nRelation ;               // my relation to this game
	int       nGameType ;               // game type
	int       bChessGame ;              // chess game?
	int       bWhitesMove ;             // white's move?
	int       bLastWhitesMove ;         // last white's move
	int       bPlaying ;                // playing?
	int       bIPlayWhite ;             // playing white side of the board?
	int       bInitialMove ;            // initial move?
	int       nMoveNumber ;             // move number

	char      cLastMove [50] ;          // last move
	char      cVerboseMove [50] ;       // verbose last move
	char      cTime4Move [50] ;         // time it takes to make the last move
	char      cHandle [2] [50] ;        // handle 0 = white, 1 = black
	char      cRating [2] [50] ;        // handle 0 = white, 1 = black
	int       nRated ;                  // 0 = unrated, 1 = rated, -1 = ?
	char      cOrgGameType [255] ;      // original game type string
	char      cGameType [255] ;         // game type
	char      cOrgResult [500] ;        // original game result string
	char      cResult [500] ;           // game result
	int       bSavedGame ;              // game has been saved?

	int       nBoard [8] [8] ;          // board
	int       nLastBoard [8] [8] ;      // last board
	int       nPromoteBoard [8] [8] ;   // promoted piece board
	int       nBuffer [MAX_BUFFER] ;    // buffer piece count (does not include kings)
	int       nLastBuffer [MAX_BUFFER] ;// last buffer piece count
	int       nLastDoublePushFile ;     // last pawn double push file number
	int       bCanCastleKingSide [2] ;  // can castle king side?
	int       bCanCastleQueenSide [2] ; // can castle queen side?
	int       nHalfMoves ;              // number of half moves
	int       nKingX [2] ;              // king x coordinate
	int       nKingY [2] ;              // king x coordinate

	POINT     ptHighlight [2] ;         // current highlight (board coordinates)
	POINT     ptLastHighlight [2] ;     // last highlight (board coordinates)

	POINT     ptKing [2] [8] ;          // current king highlight (0 = white, 1 = black, board coordinates)
	POINT     ptLastKing [2] [8] ;      // last king highlight (0 = white, 1 = black, board coordinates)

	int       nInitialClock ;           // initial time
	int       nIncrementClock ;         // increment time
	int       nClockTimerEvent ;        // clock timer event
	int       bTickClock  ;             // tick clock?
	int       bFlagged [2] ;            // flagged?
	long      nTimeRemaining [2] ;      // time remaining (in milliseconds)
	TIMEMARK  tTickStartTM ;            // last time mark
	long      nIntendedTickLength ;     // intended tick length
	int       bOnLagClock ;             // are we lagging currently?
	long      nNextLagCheck ;           // how long till we check for bug lag?
	long      nLag [2] ;                // lag for this board

	char      cStyle12 [512] ;          // last style 12 string (ICC playing only)

	HBITMAP   hWindow ;                 // window
	HBITMAP   hSave ;                   // save background (drag info)
	HBITMAP   hTemp ;                   // temporary
	HBITMAP   hSquare [MAX_SQUARE] ;    // 0 = white, 1 = black, 2 = buffer
	HBITMAP   hPiece [MAX_PIECE] ;      // piece
	int       nss ;                     // square size
	int       nhss ;                    // half square size
	int       ndss ;                    // double square size
	RECT      rLag [2] ;                // lag
	RECT      rHandle [2] ;             // handle
	RECT      rClock [2] ;              // clock
	RECT      rGameType ;               // game type
	RECT      rLastMove ;               // last move
	RECT      rResult ;                 // result
	RECT      rBoard ;                  // board
	RECT      rBuffer ;                 // buffer
	RECT      rBuffer1 ;                // buffer (bottom)
	RECT      rActual ;                 // actual
	RECT      rVCoord [8] ;             // vertical coordinates
	RECT      rHCoord [8] ;             // horizontal coordinates
	POINT     ptBuffer [10] ;           // buffer piece starting x and y coordinates
	POINT     ptNumber [3] ;            // buffer piece number coordinates

	int       bShowMoveButtons ;        // showed move buttons?
	int       bErrorMoveList ;          // errored move list?
	int       bRelation4 ;              // found style 12 string with relation -4?
	int       bClickedButton ;          // clicked move button?
	int       nInitDoublePushFile ;     // initial last pawn double push file number
	int       bInitKingSide [2] ;       // initial can castle king side?
	int       bInitQueenSide [2] ;      // initial can castle queen side?
	int       nInitHalfMoves ;          // initial number of half moves
	int       nInitKingX [2] ;          // initial king x coordinate
	int       nInitKingY [2] ;          // initial king x coordinate
	int       nInitBoard [8] [8] ;      // initial board
	int       nInitBuffer [MAX_BUFFER] ;// initial buffer piece count (does not include kings)
	int       bInitInitialMove ;        // initial initial move?
	int       bInitWhitesMove ;         // initial white's move?
	int       nInitMoveNumber ;         // initial move number
	char      cInitLastMove [50] ;      // initial last move
	char      cInitTime4Move [50] ;     // initial time it takes to make the last move
	int       nCurrentIndex ;           // current position index
	int       nCurrentColor ;           // current color
	int       nMinIndex ;               // minimum position index
	int       nMinColor ;               // minimum color
	int       nMaxIndex ;               // maximum position index
	int       nMaxColor ;               // maximum color
	int       nMoveListMaxIndex ;       // maximum move list index
	int       nMoveListMaxColor ;       // maximum move list color
	POSITION  Position [MAX_POSITION] ; // position

	int       bShowCapturedPiece ;      // show captured piece?
	int       bIssuedEventCommand ;     // issued game event command?
	int		  nAbortStatus ;            // running = 0, aborted by player = 1, aborted by opp = 2

	int ChessValue [13] ;               // chess piece value
	int BugValue   [13] ;               // bughouse/crazyhouse piece value
}
GAME;

typedef struct
{
	HBITMAP  hPiece   [MAX_PIECE];
	POINT    ptPiece  [MAX_PIECE];
	COLORREF clrPiece [MAX_PIECE];
	HBITMAP  hSquare  [MAX_PIECE];
	POINT    ptSquare [MAX_PIECE];
}
CSET;

typedef struct
{
	int   nss ;             // square size
	int   nhss ;            // half square size
	int   ndss ;            // double square size
	RECT  rLag [2] ;        // lag
	RECT  rHandle [2] ;     // handle
	RECT  rClock [2] ;      // clock
	RECT  rGameType ;       // game type
	RECT  rLastMove ;       // last move
	RECT  rResult ;         // result
	RECT  rPtell ;          // ptell
	RECT  rGinfo ;          // game info
	RECT  rBoard ;          // board
	RECT  rBuffer ;         // buffer
	RECT  rActual ;         // actual
	RECT  rVCoord [8] ;     // vertical coordinates
	RECT  rHCoord [8] ;     // horizontal coordinates
	POINT ptBuffer [10] ;   // buffer piece starting x and y coordinates
	POINT ptNumber [3] ;    // buffer piece number coordinates

	int   bVectorPiece;
	int   bVectorSquare;
	char  cBitmapPiece [_MAX_PATH + 10];
	char  cBitmapSquare [_MAX_PATH + 10];
	int   nPieceSet;

	HBITMAP htTemp;
}
BOARDCFG;

typedef struct
{
	int nMove;
	int nLastMatch;
	int nFromX    [MAX_MOVE];
	int nFromY    [MAX_MOVE];
	int nToX      [MAX_MOVE];
	int nToY      [MAX_MOVE];
	int nMoveType [MAX_MOVE];
	int nCapture  [MAX_MOVE];
	int nDrop     [MAX_MOVE];
	int nPromote  [MAX_MOVE];
}
MOVES;

typedef struct
{
	int  nMainIndex ;                   // main index
	int  nMainBoard [8] [8] ;           // main board
	int  nMainPromote [8] [8] ;         // main promote board
	int  nMainBuffer [MAX_BUFFER] ;     // main buffer piece count (does not include kings)
	int  nMainLastDoublePushFile ;      // main last pawn double push file number
	int  bMainCastleKingSide [2] ;      // main can castle king side?
	int  bMainCastleQueenSide [2] ;     // main can castle queen side?
	long nMainTimeRemaining [2] ;       // main time remaining (in milliseconds)
	int  bMainWhitesMove ;              // main whites move
	int  nMainMove ;                    // main move
	int  nMainColor ;                   // main color
	//
	int  nPartnerIndex ;                // partner index
	int  nPartnerBoard [8] [8] ;        // partner board
	int  nPartnerPromote [8] [8] ;      // partner promote board
	int  nPartnerBuffer [MAX_BUFFER] ;  // partner buffer piece count (does not include kings)
	int  nPartnerLastDoublePushFile ;   // main last pawn double push file number
	int  bPartnerCastleKingSide [2] ;   // main can castle king side?
	int  bPartnerCastleQueenSide [2] ;  // main can castle queen side?
	long nPartnerTimeRemaining [2] ;    // partner time remaining (in milliseconds)
	int  bPartnerWhitesMove ;           // main whites move
	int  nPartnerMove ;                 // partner move
	int  nPartnerColor ;                // partner color
	//
	int  bFromInitialPosition ;         // save game from initial position?
	//
	int  nMove ;                        // number of moves
	int  nIndex [MAX_BUGHOUSE] ;        // index (main or partner)
	int  nNumber [MAX_BUGHOUSE] ;       // move number
	int  nColor [MAX_BUGHOUSE] ;        // color
}
BUGHOUSE;

typedef struct
{
	int x ; // window x coordinate
	int y ; // window y coordinate
	int w ; // window width
	int h ; // window height
	int s ; // window state (0=normal,1=maximized,
}       //               2=minimized,3=closed)
WCOORD;

typedef struct
{
	int nType [MAX_BUTTON];
	char cText [MAX_BUTTON] [MAX_BUTTON_TEXT + 10];
	char cAction [MAX_BUTTON] [MAX_BUTTON_ACTION + 10];
}
BUTTONS, *LPBUTTONS;

typedef struct
{
	int bPlay [MAX_SOUND];
	int bBeep [MAX_SOUND];
	char cName [MAX_SOUND] [MAX_SOUND_NAME + 10];
	void *pData [MAX_SOUND];
}
SOUNDS, *LPSOUNDS;

typedef struct
{
	int bPlay [MAX_EAR];
	int bBeep [MAX_EAR];
	char cTell [MAX_EAR] [MAX_EAR_TELL + 10];
	char cName [MAX_EAR] [MAX_EAR_NAME + 10];
	void *pData [MAX_EAR];
}
EARS, *LPEARS;

typedef struct
{
	int bPlay [MAX_GAME_SOUND];
	int bBeep [MAX_GAME_SOUND];
	char cName [MAX_GAME_SOUND] [MAX_GAME_SOUND_NAME + 10];
	void *pData [MAX_GAME_SOUND];
}
GAMESOUNDS, *LPGAMESOUNDS;

typedef struct
{
	int bPlay [MAX_TIME_SOUND];
	int bPlayed [MAX_TIME_SOUND];
	int nLastSecond;
	char cName [MAX_TIME_SOUND] [MAX_TIME_SOUND_NAME + 10];
	void *pData [MAX_TIME_SOUND];
}
TIMESOUNDS, *LPTIMESOUNDS;

typedef struct
{
	int nType [MAX_LOGIN_EVENT];
	char cCommand [MAX_LOGIN_EVENT] [MAX_LOGIN_EVENT_COMMAND_SIZE + 10];
}
LOGINEVENT;

typedef struct
{
	int nType [MAX_GAME_EVENT];
	char cCommand [MAX_GAME_EVENT] [MAX_GAME_EVENT_COMMAND_SIZE + 10];
}
GAMEEVENT;

typedef struct
{
	char cMenu [MAX_RM_ITEM] [MAX_RM_MENU_SIZE + 10];
	int nType [MAX_RM_ITEM];
	char cCommand [MAX_RM_ITEM] [MAX_RM_COMMAND_SIZE + 10];
}
RIGHTMOUSE;

typedef struct
{
	int nI;
	int nX;
	int nY;
}
LEFTMOUSEDROP;

typedef struct
{
	int nI;
	int nX;
	int nY;
}
RIGHTMOUSEDROP;

typedef struct
{
	int nIndex ;        // board index
	int bFromBoard ;    // dragged piece from board?
	POINT ptFrom ;      // board  coordinates of the piece's original position
	POINT ptCurrent ;   // window coordinates of current position
	POINT ptLast ;      // window coordinates of last position
	POINT ptDist ;      // mouse and center of the original piece distance
	int nPc ;           // dragged piece
	int nClicked ;      // click move counter
}
TFDRAGINFO;

typedef struct
{
	int nIndex ;        // board index
	int bFromBoard ;    // dragged piece from board?
	POINT ptFrom ;      // board  coordinates of the piece's original position
	POINT ptCurrent ;   // window coordinates of current position
	POINT ptLast ;      // window coordinates of last position
	int nPc ;           // dragged piece
}
TFANIMATEINFO;

typedef struct
{
	char cConnectionBrowse [_MAX_PATH + 10];
	char cLogBrowse [_MAX_PATH + 10];
	char cSaveBPGNBrowse [_MAX_PATH + 10];
	char cSavePGNBrowse [_MAX_PATH + 10];
	char cProfileBrowse [_MAX_PATH + 10];
	char cPieceBrowse [_MAX_PATH + 10];
	char cSquareBrowse [_MAX_PATH + 10];
	char cPGNBrowse [_MAX_PATH + 10];
	char cBPGNBrowse [_MAX_PATH + 10];
}
BROWSE;

typedef struct
{
	CHARFORMAT cfTelnet;
	CHARFORMAT cfItalic;
	CHARFORMAT cfUnderline;
	char cTelnetBuffer [CO_MAX + 1 + 10];
	char cMarkBuffer [CO_MAX + 1 + 10];
	char cEditBuffer [EDIT_LINE_SIZE + 10];
	int bHasESC;
	int bFoundHandle;
	int bTelnetSkipLine;
	int bLastTelnetIsCR;
	int bLastIsTelnetEdit;
	int bLastChannel;
	int nLastChannel;
	int nTelnetState;
	COLORREF clrCurrent;
	COLORREF clrLastLine;
	int nCurrentSound;
	int bDisplayLine;
	int bForceVisible;
	int bMissingQuote;
	int bMissingBracket;
	int bMissinghttpwww;
	int bDisplayContinueLine;
	int bSkipDisplayLine;
	int bLastIsChallenge;
	int bTellTold;
	int bPtellTold;
	int bSayTold;
	int bPartnerTold;
	int nTellTold;
	int nPtellTold;
	int nSayTold;
	int nPartnerTold;
	int bReceivedMessage;
	int nAutoClearedMessage;
	char cTellTold [128];
	char cPtellTold [128];
	char cSayTold [128];
	char cPartnerTold [128];
	char cLastGameInfo [512];
}
TELNET;

typedef struct
{
	char cBuffer [MAX_SOCKET_BUFFER_SIZE + 10] ;    // input buffer
	char cAppSpawn [MAX_APP_SPAWN + 10] ;           // child process executable filename
	HANDLE hFile;
	HANDLE hThread;
	HANDLE hProcess;
	HANDLE hTo;
	HANDLE hFrom;
	DWORD nID;
	DWORD nCount;
	char *cNext;
	int nError;
	int bLoadOK;
}
INPUT;

typedef struct
{
	INPUT TSInput;
	SOCKET skSocket;
	char cSocketBuffer [MAX_SOCKET_BUFFER_SIZE + 10];
	char cSocketLine [MAX_SOCKET_BUFFER_SIZE + 10];
	int bSocketLine;
	char cServerAddress [2048];
	int bSocketIsOpen;
	int nSocketLogin;
	int bBeforeLogin;
}
TIMESEAL;

typedef struct
{
	int nTimer;
	int nTimerCount;
	int bTimerCommand;
	int nTimerMinute;
	int nType;
	char cTimerCommand [MAX_TIMER_LINE_SIZE + 10];
	char cICCTimerCommand [MAX_TIMER_LINE_SIZE + 10];
}
TIMERCMD;

typedef struct
{
	TIMEMARK tLastLagCommand;
	int nLagTolerance;
	int nLagThreshold;
	long nLagLThreshold;
	int nType;
	char cLagCommand [MAX_LAG_COMMAND_SIZE + 10];
	char cLagDisplay [MAX_LAG_DISPLAY_SIZE + 10];
}
LAGCMD;

typedef struct
{
	int nRedClock;
	long nLRedClock;
	int nShowTenthSecond;
	int bShowOneSecond;
}
REDCLOCK;

typedef struct
{
	int bCensor;
	int nItem;
	char cHandle [MAX_CENSOR_ITEM] [ICS_HANDLE_LENGTH];
	char cCensor [MAX_CENSOR_ITEM] [MAX_CENSOR_BIT];
	char cTell [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
	char cPtell [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
	char cSay [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
	char cPartner [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
	char cMessage [MAX_CENSOR_ITEM] [CENSOR_COMMAND_SIZE + 10];
	int nStart [26];
	int nEnd [26];
}
CENSOR;

typedef struct
{
	CHARRANGE TmpSel;
	int nGameIndex;
	int bMaximizeMain;
	int bIsMyTurn;
	int nIssuedMovesCommand;
	int nMoveListIndexNumber;
	int bIncludeSpace;
	int bInDialogBox;
	int nRightMouseIndex;
	int bFirstTime;
	int bOkToResizePlay;
	char cDocumentDir [_MAX_PATH + 10];
	HWND hwndLastGame;
	int bUnobserve;
	int nUnobGameNumber;
	char cUnobWname [50];
	char cUnobBname [50];
	TIMEMARK tUnobserve;
	char cSaveBuffer [CO_MAX + CO_MAX + 1 + 10];
	char cLoginFile [_MAX_PATH + 10];
	int nSysPlayGameType;
	int nSysExamGameType;
	int nSysObserveGameType;
	int nLastPlayGameType;
	int nLastExamGameType;
	int nLastObserveGameType;
	//int bMoveOnLButtonDown;
	int nCensorIndex;
	int bIssuedAbort;
	int bICCLastGameWasWild16;
	int bPromoteCommand;
	int nPromoteWPiece;
	int nPromoteBPiece;
	int bPromoteKnight;
	int nTryList;
	int nTryScreen;
	int nTryTaskbar;
	int nTryPlay;
	int bInPromote;
	HWND hwndPromote;
	int bQuickConnection;
	int bCDROMConnection;
}
SYSTEM;

typedef struct
{
	char cFn [_MAX_PATH + 10];
	FILE *Fv;
}
LOG;

typedef struct
{
	int bSilenceApply;
	int bSilenceShout;
	int bSilenceCShout;
	int bSilenceAnnouncement;
	int bSilenceChTell;
	int bSilencePeTell;
	int bSilenceSay;
	int bSilenceKibitz;
	int bSilenceWhisper;
	int bSilencePlayShout;
	int bSilencePlayCShout;
	int bSilencePlayAnnouncement;
	int bSilencePlayChTell;
	int bSilencePlayPeTell;
	int bSilencePlaySay;
	int bSilencePlayKibitz;
	int bSilencePlayWhisper;
	char cTell [SILENCE_COMMAND_SIZE + 10];
}
SILENCE;

typedef struct
{
	int bLogTelnet;
	int bTimeStampTelnet;
	int bTimeStampLog;
	int bLogGame;
	int bSavePlayGame;
	int bSaveObserveGame;
	int bSaveUnobserveGame;
	int bSaveAbortGame;
	int bSaveNoBFENBugGame;
	int bSaveDaily;
	char cPGNViewer [_MAX_PATH + 10];
	char cBPGNViewer [_MAX_PATH + 10];
	int bAutoFlag;
	int bAutoRematch;
	int bAutoObserve;
	int bAutoObserveGNotify;
	int bActualSize;
	int bStopResizePlay;
	int bEscCancelDragMove;
	int bEscCancelTruePremove;
	int bEscCancelSitDropMove;
	int bEscCancelPromoteKnight;
	int bNoPlayAbuser;
	int bNoPlayComputer;
	int nPlayWild5Flip;
	int bShowResult;
	int bShowRating;
	int bShowLagStat;
	int bShowPtell;
	int bShowCoord;
	int bShowGameType;
	int bShowGameTypeOnTitleBar;
	int bShowValueOnTitleBar;
	int bShowLastMove;
	int bShowLastMoveTenth;
	int bShowHighlight;
	int bShowTPHighlight;
	int bShowClockOnTop;
	int bShowKingHighlight;
	int nShowKingHighlightPM;
	int nShowKingHighlightPO;
	int nShowKingHighlightE;
	int nShowKingHighlightO;
	int nShowCapturedChessPiece;
	int nBufferOrientation;
	int bVectorPiece;
	int bVectorSquare;
	char cBitmapPiece [_MAX_PATH + 10];
	char cBitmapSquare [_MAX_PATH + 10];
	int nPieceSet;
	int bAutoQueen;
	int nMoveType;
	int bAutoCenterDragPiece;
	int bSmartMove;
	int bSmartMove1;
	int bSmartMove2;
	int bTruePremove;
	int nTruePremove;
	int bIllegalTPBeep;
	int bRightMousePlay;
	int bRightMousePartner;
	int bTestLegalPlay;
	int bTestLegalPartner;
	int bAnimateObserve;
	int bAnimateExam;
	int bAnimatePlay;
	int nAnimateSpeedObserve;
	int nAnimateSpeedExam;
	int nAnimateSpeedPlay;
	int bLongPartner;
	char cPartnerMove1 [MAX_PARTNER_ASSIST_SIZE + 10];
	char cPartnerMove2 [MAX_PARTNER_ASSIST_SIZE + 10];
	char cPartnerMove3 [MAX_PARTNER_ASSIST_SIZE + 10];
	char cPartnerMove4 [MAX_PARTNER_ASSIST_SIZE + 10];
	int bShowMoveButton;
	int bUseArrowKeyMoveButton;
	int bMouseWheel;
	int bShowUnderline;
	int bEnterHighlight;
	int bTelnetResizeGoBottom;
	int bSuppressAnnouncement;
	int bSuppressCompKibitz;
	int bSuppressCompWhisper;
	int bSuppressIgnoreFormula;
	int bSuppressROBOWelcome;
	int bSuppressCompChTell;
	int bSuppressCompTell;
	int bSuppressCompShout;
	int bMaximizeTelnetUponLogin;
	int bSound;
	int bTelnetSound;
	int bGameSound;
	int bEarSound;
	int bFinalCountDown;
	int bLagCommand;
	int bExtendedButton;
	int bSaveOnExit;
	int bMinimizetoTray;
	int bShowBitmapSize;
	int bShowMenuPath;
	int bButtonCommandAddHist;
	int bPlayCommandAddHist;
	int bObserveCommandAddHist;
	int bTelnetTextCommandAddHist;
	int bTelnetEditCommandAddHist;
	int bLEventCommandAddHist;
	int bGEventCommandAddHist;
	int bFKeyCommandAddHist;
	int bCKeyCommandAddHist;
	int bSeekCommandAddHist;
	int bMatchCommandAddHist;
	int bTellCommandAddHist;
	int bMessageCommandAddHist;
	int bTimerCommandAddHist;
	int bLagCommandAddHist;
	int bQuestionCommandAddHist;
	int bEarPartner;
	int bFICSPlayCloseAll;
	int bFICSPlayEndPartner;
	int bICCPlayCloseAll;
	int bICCPlayEndPartner;
	int bNONFICSPlayCloseAll;
	int bNONFICSPlayEndPartner;
	int bEat12GameNotFoundLine;
	int bEatb1GameNotFoundLine;
	int bRMNoHLSetTelnetHandle;
	int bSuppressExitPrompt;
	int bUseXTellForLastTell;
	char cNotepad [_MAX_PATH + 10];
	int bMessageMouse;
	int bIamAGuest;
	int bColoredClocks;
}
USER;

typedef struct
{
	char cServer [MAX_SERVER] [MAX_SERVER_SIZE + 10];
	char cPort [MAX_SERVER] [MAX_SERVER_PORT_SIZE + 10];
	char cPrompt [MAX_SERVER] [MAX_SERVER_PROMPT_SIZE + 10];
	int nType [MAX_SERVER];
}
SERVER;

typedef struct
{
	int nLoginString;
	char cLoginString [MAX_LOGIN] [MAX_LOGIN_SIZE + 10];
	char cLoginFile [_MAX_PATH + 10];
	char cLoginName [MAX_LOGIN_NAME_SIZE + 10];
	char cLoginAddress [MAX_LOGIN_ADDRESS_SIZE + 10];
	char cLoginPort [MAX_LOGIN_PORT_SIZE + 10];
	char cLoginPrompt [MAX_LOGIN_PROMPT_SIZE + 10];
	int nLoginPrompt;
	char cLoginHelper [_MAX_PATH + 10];
	int bLoginHelper;
	int nLoginType;
	char cLoginHandle [MAX_LOGIN_HANDLE_SIZE + 10];
	char cLoginPassword [MAX_LOGIN_PASSWORD_SIZE + 10];
	char cLoginScript [_MAX_PATH + 10];
	char cServerName [MAX_LOGIN_SERVER_NAME_SIZE + 10];
}
LOGIN;

typedef struct
{
	char cWhoAmI [50];
	char cPartner [50];
	char cOpponent [50];
	char cPartnerOpponent [50];
	char cLastTell [50];
	char cLastKib [50];
	char cLastWhisper [50];
	char cLastSay [50];
	char cTelnetHandle [50];
	char cBoardHandle [50];
	char cUserHandle1 [50];
	char cUserHandle2 [50];
	char cPartnerTell [4096];
	char cPartnerTell1 [50];
	char cPartnerTell2 [50];
	char cFollow [50];
	char cPfollow [50];
	char cFollowGone [255];
	char cFollowGone1 [255];
	char cFollowOn [255];
	int nFollowGone;
	int nFollowGone1;
	int nFollowOn;
	char cObserve [50];
	char cExam [50];
	int nEditVar;
}
VARS;

typedef struct
{
	int nHistoryI;
	int nHistoryP;
	char cHistoryT [MAX_HISTORY] [EDIT_LINE_SIZE + 10];
}
HISTORY;

typedef struct
{
	int  nType [MAX_FUNCTION_KEY];
	char cFKey [MAX_FUNCTION_KEY] [MAX_FUNCTION_KEY_SIZE + 10];
}
FKEY;

typedef struct
{
	char cSitPieceDropMove [MAX_MOVE_STRING_SIZE + 10];
}
F8KEY;

typedef struct
{
	int nLoopTellHandle;
	char cLastTellHandle [MAX_TELL_HANDLE] [50];
}
F9KEY;

typedef struct
{
	int  nType [MAX_CONTROL_KEY];
	char cFKey [MAX_CONTROL_KEY] [MAX_CONTROL_KEY_SIZE + 10];
}
CONTROLKEY;

typedef struct
{
	int bPobserve;
	int nPobserve;
	char cPobserve [50];
}
FICS;

typedef struct
{
	int bIssuedBugCommand;
	int nIssuedBugCommand;
	int bStartBugGameList;
	int nFirstBugGame;
	int nSecondBugGame;
	int bSecondBugFlip;
	int bFindICCPartner;
	char cICCPartner [20];
}
ICC;

typedef struct
{
	int nPremoveCount;
	int nPremoveHead;
	int nPremoveTail;
	char cPremoveBuffer [MAX_TRUE_PREMOVE] [32];
	int nPremoveLegalBuffer [MAX_TRUE_PREMOVE] [9];
	int bLastTP;
	POINT ptLastTP [2];
	int bIllegalTP;
	POINT ptIllegalTP [2];
}
PREMOVE;

typedef struct
{
	int bMatchMouse;
	char cMatchHandle [50];
	int nMatchTime;
	int nMatchIncrement;
	int bMatchRated;
	int nMatchType;
	int nICCMatchType;
	int nMatchColor;
}
MATCH;

typedef struct
{
	int nSeekTime;
	int nSeekIncrement;
	int nSeekRating1;
	int nSeekRating2;
	int nSeekType;
	int nICCSeekType;
	int nSeekColor;
	int bSeekManual;
	int bSeekFormula;
	int bSeekRated;
}
SEEK;

#ifdef __MAIN__
HINSTANCE hInst;

WNDPROC WndProcText;
WNDPROC WndProcEdit;
WNDPROC WndProcButton;

HMENU hMenu [MAX_MENU];
HWND hwndWindow [MAX_WINDOW];

HICON hIcon;

COLORREF clrColor [MAX_COLOR];
COLORREF clrRed;

int bChannel [MAX_CHANNEL_COLOR];
COLORREF clrChannel [MAX_CHANNEL_COLOR];

HPEN hpPen [MAX_PEN];
HBRUSH hbBrush [MAX_BRUSH];

FONTRECORD hfFont [MAX_FONT];

GAME Game [MAX_GAME + 1];
BOARDCFG BoardCfg;
CSET CSet;

MOVES Moves;
MOVES TempMoves;

BUGHOUSE Bughouse [MAX_GAME];

WCOORD wCoord [MAX_COORD];
WCOORD wPartner;
WCOORD wLayout [MAX_LAYOUT] [MAX_COORD];

HWND hwndButton [MAX_BUTTON];
BUTTONS Buttons;

SOUNDS Sounds;
EARS Ears;
GAMESOUNDS GameSounds;
TIMESOUNDS TimeSounds;
LOGINEVENT LEvent;
GAMEEVENT GEvent;
RIGHTMOUSE RightMouse [MAX_RM];
LEFTMOUSEDROP LeftMouseDrop;
RIGHTMOUSEDROP RightMouseDrop;

TFDRAGINFO DragInfo;
TFANIMATEINFO AnimateInfo;

BROWSE Browse;

TELNET Telnet;
TIMESEAL Timeseal;
TIMERCMD TimerCmd;
LAGCMD LagCmd;

REDCLOCK RedClock;
CENSOR Censor [MAX_CENSOR];
SYSTEM System;
LOG Log;
USER User;

SILENCE Silence;

SERVER Server;
LOGIN Login;
VARS Vars;
HISTORY History;

FKEY FKey;
F8KEY F8Key;
F9KEY F9Key;
CONTROLKEY ControlKey;

FICS Fics;
ICC Icc;

PREMOVE Premove;

MATCH Match;
SEEK Seek;

int ColorBoard [8] [8] = { {
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE,
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE
	},
	{
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK,
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK
	},
	{
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE,
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE
	},
	{
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK,
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK
	},
	{
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE,
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE
	},
	{
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK,
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK
	},
	{
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE,
		INDEX_BLACK, INDEX_WHITE, INDEX_BLACK, INDEX_WHITE
	},
	{
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK,
		INDEX_WHITE, INDEX_BLACK, INDEX_WHITE, INDEX_BLACK
	}
};
int ReverseCoord [8] = { 7, 6, 5, 4, 3, 2, 1 };
char ICSPiece [MAX_PIECE] = { ICS_WHITE_PAWN, ICS_WHITE_ROOK, ICS_WHITE_KNIGHT, ICS_WHITE_BISHOP, ICS_WHITE_QUEEN,
							  ICS_BLACK_PAWN, ICS_BLACK_ROOK, ICS_BLACK_KNIGHT, ICS_BLACK_BISHOP, ICS_BLACK_QUEEN,
							  ICS_WHITE_KING, ICS_BLACK_KING
							};
char ICSWPiece [MAX_PIECE] = { ICS_WHITE_PAWN, ICS_WHITE_ROOK, ICS_WHITE_KNIGHT, ICS_WHITE_BISHOP, ICS_WHITE_QUEEN,
							   ICS_WHITE_PAWN, ICS_WHITE_ROOK, ICS_WHITE_KNIGHT, ICS_WHITE_BISHOP, ICS_WHITE_QUEEN,
							   ICS_WHITE_KING, ICS_WHITE_KING
							 };

int ChessPiece        [MAX_BUFFER] = { 8, 2, 2, 2, 1, 8, 2, 2, 2, 1 };
int ReverseChessPiece [MAX_BUFFER] = { 5, 6, 7, 8, 9, 0, 1, 2, 3, 4 };

int AnimateSpeed [MAX_ANIMATE_SPEED] = {  5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };

UINT FMenu [MAX_FMENU] = { IDM_NONE,
						   IDM_TITLE,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_SEEK_GAME,
						   IDM_MATCH_PLAYER,
						   IDM_MESSAGE_USER,
						   IDM_TITLE,
						   IDM_TEXT_SELECTALL,
						   IDM_TEXT_COPY,
						   IDM_TEXT_QUICKPASTE,
						   IDM_TEXT_SAVETOFILE,
						   IDM_EDIT_SELECTALL,
						   IDM_EDIT_COPY,
						   IDM_EDIT_CUT,
						   IDM_EDIT_PASTE,
						   IDM_NONE,
						   IDM_LOG_TELNET,
						   IDM_TOGGLE_TELNET,
						   IDM_MAXIMIZE_TELNET,
						   IDM_RESTORE_TELNET,
						   IDM_TITLE,
						   IDM_LOG_GAMES,
						   IDM_LOG_GAME_SETTINGS,
						   IDM_LOAD_BPGN,
						   IDM_LOAD_PGN,
						   IDM_RESET_BOARD,
						   IDM_FLIP_BOARD,
						   IDM_CLOSE_BOARD,
						   IDM_TITLE,
						   IDM_AUTO_QUEEN,
						   IDM_AUTO_REMATCH,
						   IDM_RIGHT_MOUSE_PARTNER,
						   IDM_COORDINATES_SHOW,
						   IDM_TEST_LEGAL_PLAY,
						   IDM_TEST_LEGAL_PARTNER,
						   IDM_TITLE,
						   IDM_AUTO_OBSERVE,
						   IDM_ANIMATE_OBSERVE,
						   IDM_ANIMATE_EXAM,
						   IDM_UNEXAM_GAME,
						   IDM_TITLE,
						   IDM_SMART_MOVE,
						   IDM_TRUE_PREMOVE_THIEF,
						   IDM_TRUE_PREMOVE_LR,
						   IDM_NONE,
						   IDM_TITLE,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_TITLE,
						   IDM_WINDOW_RESTORE0,
						   IDM_WINDOW_RESTORE1,
						   IDM_WINDOW_RESTORE2,
						   IDM_WINDOW_RESTORE3,
						   IDM_WINDOW_RESTORE4,
						   IDM_WINDOW_RESTORE5,
						   IDM_WINDOW_RESTORE6,
						   IDM_WINDOW_RESTORE7,
						   IDM_WINDOW_RESTORE8,
						   IDM_WINDOW_SAVE0,
						   IDM_WINDOW_SAVE1,
						   IDM_WINDOW_SAVE2,
						   IDM_WINDOW_SAVE3,
						   IDM_WINDOW_SAVE4,
						   IDM_WINDOW_SAVE5,
						   IDM_WINDOW_SAVE6,
						   IDM_WINDOW_SAVE7,
						   IDM_WINDOW_SAVE8,
						   IDM_TITLE,
						   IDM_START_TIMER,
						   IDM_STOP_TIMER,
						   IDM_TOGGLE_TIMER_ONOFF,
						   IDM_TITLE,
						   IDM_APPLY_SOUND,
						   IDM_APPLY_TELNET_SOUND,
						   IDM_APPLY_GAME_SOUND,
						   IDM_APPLY_EAR_SOUND,
						   IDM_FINAL_COUNTDOWN,
						   IDM_TITLE,
						   IDM_TIMER,
						   IDM_BUTTON_CONFIG,
						   IDM_FUNCTION,
						   IDM_CONTROL_KEY,
						   IDM_TELNET_CONFIGURE,
						   IDM_LAG_COMMAND,
						   IDM_FICS_CENSOR,
						   IDM_ICC_CENSOR,
						   IDM_NONFICS_CENSOR,
						   IDM_TITLE,
						   IDM_EDIT_PARTNER,
						   IDM_EDIT_USERHANDLE1,
						   IDM_EDIT_USERHANDLE2,
						   IDM_COPY_VARIABLE,
						   IDM_TITLE,
						   IDM_ASK_QUESTION,
						   IDM_NONE,
						   IDM_READ_THE_BUGBOARD,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_NONE,
						   IDM_SET_TF_TRUE_PREMOVE,
						   IDM_SET_LR_TRUE_PREMOVE,
						   IDM_SET_AUTO_KNIGHT_ON,
						   IDM_SET_AUTO_KNIGHT_OFF,
						   IDM_TEXT_TELL
						 };

char FString [MAX_FMENU] [99] = { "None",
								  "--- Telnet Commands ---",
								  "Configure Personal ICS Command",
								  "Reply to Last Tell",
								  "Seek a Game",
								  "Match Player",
								  "Message a User",
								  "--- Telnet Console Options ---",
								  "Main Telnet Console: Select All Text",
								  "Main Telnet Console: Copy",
								  "Main Telnet Console: Copy and Paste",
								  "Save Text to File",
								  "Command Line: Select All",
								  "Command Line: Copy",
								  "Command Line: Cut",
								  "Command Line: Paste",
								  "Command Line: Edit",
								  "Toggle Log Telnet Console",
								  "Toggle Maximize/Restore Telnet Console",
								  "Maximize Telnet Console",
								  "Restore Telnet Console",
								  "--- Game: General Options ---",
								  "Toggle Start/Stop Logging Games",
								  "Log Game Settings",
								  "Load BPGN Games",
								  "Load PGN Games",
								  "Reset Board",
								  "Flip Board",
								  "Close Board",
								  "--- Game: Playing Options ---",
								  "Toggle Auto Queen/King",
								  "Toggle Apply Auto Rematch",
								  "Toggle Right Mouse Click on Partner's Board to Drop",
								  "Toggle Show Coordinates",
								  "Toggle Test Legality on Playing Board",
								  "Toggle Test Legality on Partner's Board",
								  "--- Game: Observing Options ---",
								  "Toggle Apply Auto Observe",
								  "Toggle Animate Observe Game Move (While Not Playing)",
								  "Toggle Animate Examine Game Move",
								  "Unexamine Game",
								  "--- Move Style ---",
								  "Toggle Smart Move",
								  "Toggle True Premove (Thief Style)",
								  "Toggle True Premove (Lightning Rulez Style)",
								  "Toggle Thief/Lightning Rulez True Premove Style",
								  "--- Piece Drop ---",
								  "Pawn Drop Move",
								  "Knight Drop Move",
								  "Bishop Drop Move",
								  "Rook Drop Move",
								  "Queen Drop Move",
								  "Sit For Piece Drop Move",
								  "--- Board Layout Options ---",
								  "Restore Default Layout",
								  "Restore Layout #1",
								  "Restore Layout #2",
								  "Restore Layout #3",
								  "Restore Layout #4",
								  "Restore Layout #5",
								  "Restore Layout #6",
								  "Restore Layout #7",
								  "Restore Layout #8",
								  "Save as Default Layout",
								  "Save as Layout #1",
								  "Save as Layout #2",
								  "Save as Layout #3",
								  "Save as Layout #4",
								  "Save as Layout #5",
								  "Save as Layout #6",
								  "Save as Layout #7",
								  "Save as Layout #8",
								  "--- Timer Option ---",
								  "Start Timer Command",
								  "Stop Timer Command",
								  "Toggle Timer Command On/Off",
								  "--- Sounds ---",
								  "Toggle Apply Sound",
								  "Toggle Apply Telnet Console Sounds",
								  "Toggle Apply Game Sounds",
								  "Toggle Apply Partner Communicator Sounds",
								  "Toggle Apply Final Countdown",
								  "--- Configuration Options ---",
								  "Customize Timer Command",
								  "Customize Partner Communicator Buttons",
								  "Customize Function Key",
								  "Customize Control Key",
								  "Configure Telnet Console Colors",
								  "Customize Lag Command",
								  "Configure FICS Censor List",
								  "Configure ICC Censor List",
								  "Configure Censor List for Other ICS",
								  "-- Variables ---",
								  "Edit %partner%",
								  "Edit %userhandle1%",
								  "Edit %userhandle2%",
								  "Copy Variable",
								  "--- Miscellaneous ---",
								  "Ask for Help on Chess Server",
								  "Open Internet Browser",
								  "Read The Bug Board",
								  "Minimize Thief",
								  "Run Script",
								  "Load Personal Configuration Profile",
								  "Set Thief True Premove Style",
								  "Set Lightning Rulez True Premove Style",
								  "Set Auto Knight Promotion On",
								  "Set Auto Knight Promotion Off",
								  "Main Telnet Console: Tell Handle"
								};

int FCheck [MAX_FMENU];

char TelnetUnderLine [MAX_UNDERLINE_STRING] [32] = { "play ",
													 "tell ",
													 "finger",
													 "partner ",
													 "help",
													 "more",
													 "accept",
													 "decline",
													 "abort",
													 "adjourn",
													 "draw",
													 "match",
													 "seek",
													 "set ",
													 "who",
													 "bugwho",
													 "follow ",
													 "flag",
													 "adjourn",
													 "messages",
													 "observe ",
													 "examine ",
													 "mam "
												   };
int TelnetUnderSize [MAX_UNDERLINE_STRING];

char TelnetBracket [MAX_BRACKET_STRING] [32] = { "next" };
int TelnetBracketSize [MAX_BRACKET_STRING];

char MamerString [MAX_MAMER_STRING] [99] = { ":-",
											 ":-: Inactive;",
											 ":#: Match request issued.",
											 ":%: Requested half-point bye;",
											 ":(*) Board #",
											 ":(*) Players disconnected: ",
											 ":(*) Players idle: ",
											 ":(manager of tourneys: #",
											 ":(managing event: #",
											 ":(managing tourney: #",
											 ":(observing tourney: #",
											 ":(participating in tourney: #",
											 ":(playing a regular server game)",
											 ":*: Kept.",
											 ":*: Managing.",
											 ":*: Paired.",
											 ":.: Idle;",
											 ":[",
											 ":|-",
											 ":| ",
											 ":+-",
											 ":+: Joinable;",
											 ":+: Online;",
											 ":<: Running.",
											 ":>: Joined;",
											 ":Active:",
											 ":Add Level:",
											 ":AllowedList:",
											 ":Announce:",
											 ":AutoAnnounce:",
											 ":AutoPair:",
											 ":AutoStart:",
											 ":Average rating: ",
											 ":Bell:",
											 ":Board:",
											 ":Byes:",
											 ":Censor:",
											 ":Censoring count:",
											 ":Command:",
											 ":Comment added for ",
											 ":Comment:",
											 ":Comments for ",
											 ":Comments:",
											 ":Computers:",
											 ":Contact ",
											 ":Current Acting-COO:",
											 ":Current COO:",
											 ":Current Programmer:",
											 ":Currently empty.",
											 ":Currently includes ",
											 ":DefaultKOTH:",
											 ":DefaultTourney:",
											 ":Echo:",
											 ":EntryMessage:",
											 ":Event:",
											 ":Examples:",
											 ":ExtraInfo:",
											 ":Female:",
											 ":Force:",
											 ":Game cleared.",
											 ":Game number set.",
											 ":Game set.",
											 ":Getting KOTH Infos count:",
											 ":Getting Tourney Infos count:",
											 ":Getting Tourney Shouts count:",
											 ":HalfJoins:",
											 ":Height:",
											 ":Help file ",
											 ":Idle:",
											 ":In Load:",
											 ":In Size:",
											 ":Inc:",
											 ":Interface variable settings of ",
											 ":Invitation:",
											 ":Keep:",
											 ":KOTHInfo:",
											 ":Language:",
											 ":Last Managed:",
											 ":Last Played:",
											 ":LateJoins:",
											 ":Level:",
											 ":Listed: ",
											 ":Loaded count:",
											 ":Lock:",
											 ":MailingAddress:",
											 ":MailingAddress2:",
											 ":mamer's command list:",
											 ":mamer's Event list:",
											 ":mamer's KOTH list:",
											 ":mamer's manager list:",
											 ":mamer's tourney list:",
											 ":mamer KOTH INFO: ",
											 ":mamer TOURNEY #",
											 ":mamer TOURNEY INFO: ",
											 ":Manager help file ",
											 ":Manager Level:",
											 ":MaxByes:",
											 ":MaxRating:",
											 ":Message sent to ",
											 ":MinRating:",
											 ":Mode:",
											 ":Name:",
											 ":No one is observing tourney #",
											 ":No potential problems detected.",
											 ":NoEscape:",
											 ":Note that this is not your tourney.",
											 ":NotifyAdmin:",
											 ":NotifyCA:",
											 ":NotifyCOO:",
											 ":NotifyFM:",
											 ":NotifyGM:",
											 ":NotifyIM:",
											 ":NotifySR:",
											 ":NotifyTM:",
											 ":NotifyWFM:",
											 ":NotifyWGM:",
											 ":NotifyWIM:",
											 ":Online count:",
											 ":Out Load:",
											 ":Out Size:",
											 ":PlayerLimit:",
											 ":Please leave a comment to explain why ",
											 ":Prize:",
											 ":Purpose:",
											 ":Read Level:",
											 ":RefusedList:",
											 ":RefusedList2:",
											 ":Registered count:",
											 ":Results of tourney #",
											 ":Rounds:",
											 ":RoundUpdates:",
											 ":See Level:",
											 ":SetDefault:",
											 ":ShoutInterface:",
											 ":Silence:",
											 ":Statistics for ",
											 ":Style:",
											 ":Sub Level:",
											 ":The tourney has been forced to an end.",
											 ":The variable ",
											 ":There is no login information.",
											 ":This is a ",
											 ":This is a tourney in progress.",
											 ":This is an aborted tourney.",
											 ":This is an open ",
											 ":TieBreaker:",
											 ":Time:",
											 ":Total Bandwidth:",
											 ":Total: ",
											 ":Tourney #",
											 ":TourneyInfo:",
											 ":TourneyTell:",
											 ":TourneyUpdates:",
											 ":TShout:",
											 ":Type ",
											 ":Unable to comply.",
											 ":Usage count:",
											 ":Usage:",
											 ":Use \"tell mamer Next\" to see the next page.",
											 ":Use Level:",
											 ":Use the \"ManageTourney\" command if you need to take over.",
											 ":Variable settings of ",
											 ":Variant:",
											 ":Warning: There is an odd number of players (",
											 ":Wild:",
											 ":You are managing this tourney.",
											 ":You are already following KOTH #",
											 ":You are no longer observing any tourney.",
											 ":You are no longer observing the games in tourney #",
											 ":You are no longer observing tourney #",
											 ":You are no longer the king of KOTH #",
											 ":You are no longer the queen of KOTH #",
											 ":You are now observing the games in tourney #",
											 ":You are now observing tourney #",
											 ":You are now the king of KOTH #",
											 ":You are now the queen of KOTH #",
											 ":You can join this tourney.",
											 ":You cannot late-join this tourney.",
											 ":You challenge the king of KOTH #",
											 ":You challenge the queen of KOTH #",
											 ":You challenge your opponent in tourney #",
											 ":You currently have ",
											 ":You have a record of ",
											 ":You have been added to the Problematic list by ",
											 ":You have been removed from KOTH #",
											 ":You have been removed from the Problematic list by ",
											 ":You have joined tourney #",
											 ":You have taken over as the manager of tourney #",
											 ":You leave KOTH #",
											 ":You observe the game of KOTH #",
											 ":You resign your game in tourney #",
											 ":You will not follow any KOTH.",
											 ":You will now be following KOTH #",
											 ":You will receive a bye in the next round of tourney #",
											 ":You withdrew from tourney #",
											 ":Your "
										   };
int MamerStringSize [MAX_MAMER_STRING];

int nGNotifyHandle;
char cGNotifyHandle [MAX_GNOTIFY_HANDLE] [ICS_HANDLE_LENGTH];

int ChessValue [13] = {1, 5, 3, 3, 9, 1, 5, 3, 3, 9, 0, 0, 0};
int BugValue   [13] = {1, 2, 2, 2, 4, 1, 2, 2, 2, 4, 0, 0, 0};

#else

extern HINSTANCE hInst;

extern WNDPROC WndProcText;
extern WNDPROC WndProcEdit;
extern WNDPROC WndProcButton;

extern HMENU hMenu [MAX_MENU];
extern HWND hwndWindow [MAX_WINDOW];

extern HICON hIcon;

extern COLORREF clrColor [MAX_COLOR];
extern COLORREF clrRed;

extern int bChannel [MAX_CHANNEL_COLOR];
extern COLORREF clrChannel [MAX_CHANNEL_COLOR];

extern HPEN hpPen [MAX_PEN];
extern HBRUSH hbBrush [MAX_BRUSH];

extern FONTRECORD hfFont [MAX_FONT];

extern GAME Game [MAX_GAME + 1];
extern BOARDCFG BoardCfg;
extern CSET CSet;

extern MOVES Moves;
extern MOVES TempMoves;

extern BUGHOUSE Bughouse [MAX_GAME];

extern WCOORD wCoord [MAX_COORD];
extern WCOORD wPartner;
extern WCOORD wLayout [MAX_LAYOUT] [MAX_COORD];

extern HWND hwndButton [MAX_BUTTON];
extern BUTTONS Buttons;

extern SOUNDS Sounds;
extern EARS Ears;
extern GAMESOUNDS GameSounds;
extern TIMESOUNDS TimeSounds;
extern LOGINEVENT LEvent;
extern GAMEEVENT GEvent;
extern RIGHTMOUSE RightMouse [MAX_RM];
extern LEFTMOUSEDROP LeftMouseDrop;
extern RIGHTMOUSEDROP RightMouseDrop;

extern TFDRAGINFO DragInfo;
extern TFANIMATEINFO AnimateInfo;

extern BROWSE Browse;

extern TELNET Telnet;
extern TIMESEAL Timeseal;
extern TIMERCMD TimerCmd;
extern LAGCMD LagCmd;

extern REDCLOCK RedClock;
extern CENSOR Censor [MAX_CENSOR];
extern SYSTEM System;
extern LOG Log;
extern USER User;
extern SILENCE Silence;

extern SERVER Server;
extern LOGIN Login;
extern VARS Vars;
extern HISTORY History;

extern FKEY FKey;
extern F8KEY F8Key;
extern F9KEY F9Key;
extern CONTROLKEY ControlKey;

extern FICS Fics;
extern ICC Icc;

extern PREMOVE Premove;

extern MATCH Match;
extern SEEK Seek;

extern int ColorBoard [8] [8];
extern int ReverseCoord [8];
extern char ICSPiece [MAX_PIECE];
extern char ICSWPiece [MAX_PIECE];
extern int ChessPiece [MAX_BUFFER];
extern int ReverseChessPiece [MAX_BUFFER];
extern int AnimateSpeed [MAX_ANIMATE_SPEED];

extern UINT FMenu [MAX_FMENU];
extern char FString [MAX_FMENU] [99];
extern int FCheck [MAX_FMENU];
extern char TelnetUnderLine [MAX_UNDERLINE_STRING] [32];
extern int TelnetUnderSize [MAX_UNDERLINE_STRING];
extern char TelnetBracket [MAX_BRACKET_STRING] [32];
extern int TelnetBracketSize [MAX_BRACKET_STRING];
extern char MamerString [MAX_MAMER_STRING] [99];
extern int MamerStringSize [MAX_MAMER_STRING];
extern int nGNotifyHandle;
extern char cGNotifyHandle [MAX_GNOTIFY_HANDLE] [ICS_HANDLE_LENGTH];

extern int ChessValue [13];
extern int BugValue   [13];
#endif
