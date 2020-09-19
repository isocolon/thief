// file
#define IDM_NEW_CONNECT         100
#define IDM_CONNECT             101
#define IDM_RECONNECT           102
#define IDM_DISCONNECT          103
#define IDM_QUICK_CONNECT       104
#define IDM_CDROM_CONNECT       105
#define IDM_SAVE_PROFILE        106
#define IDM_LOAD_PROFILE        107
#define IDM_LOG_TELNET          108
#define IDM_OPEN_LOG            109
#define IDM_OPEN_THIEF_INI      110
#define IDM_OPEN_THIEF_SYS      111
#define IDM_OPEN_DEFAULT_LAYOUT 112
#define IDM_OPEN_LAYOUT1        113
#define IDM_OPEN_LAYOUT2        114
#define IDM_OPEN_LAYOUT3        115
#define IDM_OPEN_LAYOUT4        116
#define IDM_OPEN_LAYOUT5        117
#define IDM_OPEN_LAYOUT6        118
#define IDM_OPEN_LAYOUT7        119
#define IDM_OPEN_LAYOUT8        120
#define IDM_OPEN_SCRIPT         121
#define IDM_RUN_SCRIPT          122
#define IDM_EXIT                123
#define IDM_EXTRA               124
#define IDM_EXTRA1              125
#define IDM_EXTRA2              126
#define IDM_EXTRA3              127
#define IDM_EXTRA4              128
#define IDM_EXTRA5              129
#define IDM_EXTRA6              130
#define IDM_EXTRA7              131
#define IDM_EXTRA8              132
#define IDM_EXTRA9              133

// edit
#define IDM_COPY_PASTE          200
#define IDM_COPY                201
#define IDM_PASTE               202
#define IDM_SELECT_ALL          203

// game
#define IDM_SEEK_GAME           300
#define IDM_MATCH_PLAYER        301
#define IDM_REMATCH             302
#define IDM_RESUME_GAME         303
#define IDM_FLIP_BOARD          304
#define IDM_LOG_GAMES           305
#define IDM_LOG_GAME_SETTINGS   306
#define IDM_LOAD_MY_BPGN        307
#define IDM_LOAD_OBSERVED_BPGN  308
#define IDM_LOAD_OTHER_BPGN     309
#define IDM_LOAD_MY_PGN         310
#define IDM_LOAD_OBSERVED_PGN   311
#define IDM_LOAD_OTHER_PGN      312

// actions
#define IDM_TELL_USER           400
#define IDM_MESSAGE_USER        401
#define IDM_MY_RATING_NOTES     402
#define IDM_MY_VARS             403
#define IDM_MY_PING             404
#define IDM_MY_HISTORY          405
#define IDM_MY_STORED           406
#define IDM_MY_JOURNAL          407
#define IDM_LIST_BUG_PLAY       408
#define IDM_LIST_BUG_PARTNERED  409
#define IDM_LIST_BUG_OPEN       410
#define IDM_LIST_PLAYERS        411
#define IDM_LIST_ALL_PLAYERS    412
#define IDM_START_TIMER         413
#define IDM_STOP_TIMER          414
#define IDM_FICS_REGISTER       415
#define IDM_READ_THE_BUGBOARD   416
#define IDM_READ_THE_FICS_FORUM 417

// preferences
#define IDM_GAME_OPTION         500
#define IDM_GAME_DISPLAY        501
#define IDM_BOARD_CONFIGURE     502
#define IDM_CLOCK_DISPLAY       503
#define IDM_MOVE_INPUT          504
#define IDM_MOVE_BUTTON         505
#define IDM_PARTNER_ASSIST      506
#define IDM_TELNET_OPTION       507
#define IDM_TELNET_CONFIGURE    508
#define IDM_APPLY_SOUND         509
#define IDM_SOUND_TELNET        510
#define IDM_SOUND_GAME          511
#define IDM_SOUND_EAR           512
#define IDM_SOUND_FINAL         513
#define IDM_LOGEVENT_CONFIGURE  514
#define IDM_GAMEEVENT_CONFIGURE 515
#define IDM_RM_BUTTON           516
#define IDM_RM_PLAY             517
#define IDM_RM_OBSERVE          518
#define IDM_RM_TELNET_TEXT      519
#define IDM_RM_TELNET_EDIT      520
#define IDM_RM_BOARD_HANDLE     521
#define IDM_EDIT_WHOAMI         522
#define IDM_EDIT_PARTNER        523
#define IDM_EDIT_OPPONENT       524
#define IDM_EDIT_PARTNEROPP     525
#define IDM_EDIT_LASTTELL       526
#define IDM_EDIT_LASTKIB        527
#define IDM_EDIT_LASTWHISPER    528
#define IDM_EDIT_LASTSAY        529
#define IDM_EDIT_TELNETHANDLE   530
#define IDM_EDIT_BOARDHANDLE    531
#define IDM_EDIT_USERHANDLE1    532
#define IDM_EDIT_USERHANDLE2    533
#define IDM_TIMER               534
#define IDM_BUTTON_CONFIG       535
#define IDM_FUNCTION            536
#define IDM_CONTROL_KEY         537
#define IDM_LAG_COMMAND         538
#define IDM_FICS_CENSOR         539
#define IDM_ICC_CENSOR          540
#define IDM_NONFICS_CENSOR      541
#define IDM_EXIT_PROMPT			542
#define IDM_SAVE_SET_NOW        543
#define IDM_SAVE_SET_EXIT       544
#define IDM_SILENCE				545

// window
#define IDM_MAXIMIZE_TELNET     600
#define IDM_RESTORE_TELNET      601
#define IDM_SHOW_BITMAP_SIZE    602
#define IDM_SHOW_MENU_PATH      603
#define IDM_TRY_LAYOUT          604
#define IDM_MINIMIZE_TRAY       610
#define IDM_WINDOW_RESTORE0     650
#define IDM_WINDOW_RESTORE1     651
#define IDM_WINDOW_RESTORE2     652
#define IDM_WINDOW_RESTORE3     653
#define IDM_WINDOW_RESTORE4     654
#define IDM_WINDOW_RESTORE5     655
#define IDM_WINDOW_RESTORE6     656
#define IDM_WINDOW_RESTORE7     657
#define IDM_WINDOW_RESTORE8     658
#define IDM_WINDOW_SAVE0        659
#define IDM_WINDOW_SAVE1        660
#define IDM_WINDOW_SAVE2        661
#define IDM_WINDOW_SAVE3        662
#define IDM_WINDOW_SAVE4        663
#define IDM_WINDOW_SAVE5        664
#define IDM_WINDOW_SAVE6        665
#define IDM_WINDOW_SAVE7        666
#define IDM_WINDOW_SAVE8        667

// Help
#define IDM_HELP_TOPIC          700
#define IDM_ASK_QUESTION        701
#define IDM_ABOUT_THIEF         702

// Traymenu
#define IDM_RESTORE_THIEF       800
#define IDM_EXIT_THIEF			801



// ============================================================================
// rest of the menu items
// ============================================================================
#define IDM_NONE                0
#define IDM_TITLE               1

#define IDM_TOGGLE_TELNET       3000    // toggle maximize/restore telnet console
#define IDM_RESET_BOARD         3001    // reset board
#define IDM_UNEXAM_GAME         3002    // unexamine game
#define IDM_CLOSE_BOARD         3003    // close board
#define IDM_AUTO_QUEEN          3004    // apply auto queen/king
#define IDM_AUTO_OBSERVE        3005    // apply auto obsereve
#define IDM_AUTO_REMATCH        3006    // apply auto rematch
#define IDM_COORDINATES_SHOW    3007    // show coordinates
#define IDM_SMART_MOVE          3008    // smart move
#define IDM_TRUE_PREMOVE_THIEF  3009    // true premove (thief style)
#define IDM_TRUE_PREMOVE_LR     3010    // true premove (lightning rulez style)
#define IDM_RIGHT_MOUSE_PARTNER 3011    // right mouse click on partner board to drop
#define IDM_TEST_LEGAL_PLAY     3012    // test legality on playing board
#define IDM_TEST_LEGAL_PARTNER  3013    // test legality on partner board
#define IDM_ANIMATE_OBSERVE     3014    // animate observe game move (while not playing)
#define IDM_ANIMATE_EXAM        3015    // animate exam game move
#define IDM_ANIMATE_PLAY        3016    // animate play game move
#define IDM_APPLY_TELNET_SOUND  3017    // apply telnet sound
#define IDM_APPLY_GAME_SOUND    3018    // apply event sound
#define IDM_APPLY_EAR_SOUND     3019    // apply partner sound
#define IDM_FINAL_COUNTDOWN     3020    // apply final countdown
#define IDM_TOGGLE_TIMER_ONOFF  3021    // toggle timer command on/off
#define IDM_LOAD_BPGN           3022    // load BPGN
#define IDM_LOAD_PGN            3023    // load PGN
#define IDM_SET_TF_TRUE_PREMOVE 3024    // set to Thief true premove style
#define IDM_SET_LR_TRUE_PREMOVE 3025    // set to Lightning Rulez true premove style
#define IDM_SET_AUTO_KNIGHT_ON  3026    // set auto knight promotion on
#define IDM_SET_AUTO_KNIGHT_OFF 3027    // set auto knight promotion off

#define IDM_TEXT_SELECTALL      8000    // telnet text select all
#define IDM_TEXT_COPY           8001    // telnet text copy
#define IDM_TEXT_QUICKPASTE     8002    // telnet text copy and paste
#define IDM_TEXT_SAVETOFILE     8003    // telnet text save to file
#define IDM_EDIT_SELECTALL      8100    // telnet edit select all
#define IDM_EDIT_COPY           8101    // telnet edit copy
#define IDM_EDIT_CUT            8102    // telnet edit cut
#define IDM_EDIT_PASTE          8103    // telnet edit paste
#define IDM_TEXT_TELL			8104    // telnet text tell handle
#define IDM_COPY_VARIABLE       8200    // copy variable

#define IDM_PAWN                8500    // pawn drop
#define IDM_KNIGHT              8501    // knight drop
#define IDM_BISHOP              8502    // bishop drop
#define IDM_ROOK                8503    // rook drop
#define IDM_QUEEN               8504    // queen drop

#define IDM_POPP_PAWN           8550    // pawn drop   (partner's opponent)
#define IDM_POPP_KNIGHT         8551    // knight drop (partner's opponent)
#define IDM_POPP_BISHOP         8552    // bishop drop (partner's opponent)
#define IDM_POPP_ROOK           8553    // rook drop   (partner's opponent)
#define IDM_POPP_QUEEN          8554    // queen drop  (partner's opponent)

#define IDM_CHILD               9000
#define IDM_CHILD_PLAY          IDM_CHILD
#define IDM_CHILD_BUTTON        9001
#define IDM_CHILD_TELNET        9002
