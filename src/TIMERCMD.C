#include "thief.h"

void TIMERCMD_Init(void)
{
	TimerCmd.nTimer      = -1;
	TimerCmd.nTimerCount = 0;

	TimerCmd.bTimerCommand = 0;
	TimerCmd.nTimerMinute  = DEFAULT_TIMER_MINUTE;

	TimerCmd.nType = DEFAULT_TIMER_FUNCTION;

	strcpy(TimerCmd.cTimerCommand,    DEFAULT_TIMER_STRING);
	strcpy(TimerCmd.cICCTimerCommand, DEFAULT_ICC_TIMER_STRING);
}

void TIMERCMD_Clear(void)
{
	TimerCmd.bTimerCommand = 0;
}

void TIMERCMD_Adjust(void)
{
	if((TimerCmd.nType >= 0) && (TimerCmd.nType < MAX_FMENU))
	{
		if(! TOOLBOX_NeedCommandValue(TimerCmd.nType))
		{
			strcpy(TimerCmd.cTimerCommand,    "");
			strcpy(TimerCmd.cICCTimerCommand, "");
		}
	}
	else
	{
		TimerCmd.nType = FUNCTION_COMMAND;
	}

	if(TimerCmd.nTimerMinute < 0)
	{
		TimerCmd.nTimerMinute = 1;
	}
	else if(TimerCmd.nTimerMinute > 50)
	{
		TimerCmd.nTimerMinute = 50;
	}
}

void TIMERCMD_Start(void)
{
	TimerCmd.bTimerCommand = 1;

	if(Login.nLoginType == SERVER_ICC)
	{
		if(TimerCmd.nType != FUNCTION_NONE)
		{
			TOOLBOX_Command(TimerCmd.nType, TimerCmd.cICCTimerCommand, User.bTimerCommandAddHist);
		}
	}
	else
	{
		if(TimerCmd.nType != FUNCTION_NONE)
		{
			TOOLBOX_Command(TimerCmd.nType, TimerCmd.cTimerCommand, User.bTimerCommandAddHist);
		}
	}

	TOOLBOX_SetTimerCommandMenu();

	SendMessage(hwndWindow [HWND_TELNET], WM_USER_START_TIMER, 0, 0);

	SetFocus(hwndWindow [HWND_TELNET_EDIT]);
}

void TIMERCMD_Stop(void)
{
	TimerCmd.bTimerCommand = 0;

	TOOLBOX_SetTimerCommandMenu();

	SendMessage(hwndWindow [HWND_TELNET], WM_USER_STOP_TIMER, 0, 0);

	SetFocus(hwndWindow [HWND_TELNET_EDIT]);
}

void TIMERCMD_Timer(void)
{
	if(TimerCmd.bTimerCommand)
	{
		if(++TimerCmd.nTimerCount >= TimerCmd.nTimerMinute)
		{
			if(Login.nLoginType == SERVER_ICC)
			{
				if(TimerCmd.nType != FUNCTION_NONE)
				{
					TOOLBOX_Command(TimerCmd.nType, TimerCmd.cICCTimerCommand, User.bTimerCommandAddHist);
				}
			}
			else
			{
				if(TimerCmd.nType != FUNCTION_NONE)
				{
					TOOLBOX_Command(TimerCmd.nType, TimerCmd.cTimerCommand, User.bTimerCommandAddHist);
				}
			}
			TimerCmd.nTimerCount = 0;
		}
	}
}

void TIMERCMD_Set(HWND hwnd)
{
	TimerCmd.nTimerCount = 0;

	TimerCmd.nTimer = SetTimer(hwnd, TIMER_COMMAND_ID, 60 * 1000, NULL);
}

void TIMERCMD_Reset(HWND hwnd)
{
	if(TimerCmd.nTimer != -1)
	{
		KillTimer(hwnd, TIMER_COMMAND_ID);
		TimerCmd.nTimer = -1;
	}
}
