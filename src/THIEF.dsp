# Microsoft Developer Studio Project File - Name="THIEF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=THIEF - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "THIEF.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "THIEF.mak" CFG="THIEF - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "THIEF - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "THIEF - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "THIEF - Win32 KICS Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "THIEF - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "THIEF___Win32_Release"
# PROP Intermediate_Dir "THIEF___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "THIEF - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "THIEF___Win32_Debug"
# PROP BASE Intermediate_Dir "THIEF___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "THIEF___Win32_Debug"
# PROP Intermediate_Dir "THIEF___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib /nologo /subsystem:windows /map /debug /machine:I386

!ELSEIF  "$(CFG)" == "THIEF - Win32 KICS Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "THIEF___Win32_KICS_Release"
# PROP BASE Intermediate_Dir "THIEF___Win32_KICS_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "THIEF___Win32_KICS_Release"
# PROP Intermediate_Dir "THIEF___Win32_KICS_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "KICS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 wsock32.lib winmm.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msimg32.lib /nologo /subsystem:windows /machine:I386

!ENDIF 

# Begin Target

# Name "THIEF - Win32 Release"
# Name "THIEF - Win32 Debug"
# Name "THIEF - Win32 KICS Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\About.c
# End Source File
# Begin Source File

SOURCE=.\Animate.c
# End Source File
# Begin Source File

SOURCE=.\Assist.c
# End Source File
# Begin Source File

SOURCE=.\Bdraw.c
# End Source File
# Begin Source File

SOURCE=.\Board.c
# End Source File
# Begin Source File

SOURCE=.\BOARD1.C
# End Source File
# Begin Source File

SOURCE=.\Boardcfg.c
# End Source File
# Begin Source File

SOURCE=.\Bughouse.c
# End Source File
# Begin Source File

SOURCE=.\Butcfg.c
# End Source File
# Begin Source File

SOURCE=.\Button.c
# End Source File
# Begin Source File

SOURCE=.\Censor.c
# End Source File
# Begin Source File

SOURCE=.\Chess.c
# End Source File
# Begin Source File

SOURCE=.\Clock.c
# End Source File
# Begin Source File

SOURCE=.\Color.c
# End Source File
# Begin Source File

SOURCE=.\Cset.c
# End Source File
# Begin Source File

SOURCE=.\Ctrlkey.c
# End Source File
# Begin Source File

SOURCE=.\Draw.c
# End Source File
# Begin Source File

SOURCE=.\Ear.c
# End Source File
# Begin Source File

SOURCE=.\Earcfg.c
# End Source File
# Begin Source File

SOURCE=.\F8key.c
# End Source File
# Begin Source File

SOURCE=.\F9key.c
# End Source File
# Begin Source File

SOURCE=.\Fics.c
# End Source File
# Begin Source File

SOURCE=.\Fkey.c
# End Source File
# Begin Source File

SOURCE=.\Font.c
# End Source File
# Begin Source File

SOURCE=.\Game.c
# End Source File
# Begin Source File

SOURCE=.\Gamecfg.c
# End Source File
# Begin Source File

SOURCE=.\Gameevnt.c
# End Source File
# Begin Source File

SOURCE=.\Gamesnd.c
# End Source File
# Begin Source File

SOURCE=.\Gdisplay.c
# End Source File
# Begin Source File

SOURCE=.\GNOTIFY.C
# End Source File
# Begin Source File

SOURCE=.\Goption.c
# End Source File
# Begin Source File

SOURCE=.\History.c
# End Source File
# Begin Source File

SOURCE=.\Icc.c
# End Source File
# Begin Source File

SOURCE=.\Ini.c
# End Source File
# Begin Source File

SOURCE=.\Lagcmd.c
# End Source File
# Begin Source File

SOURCE=.\Layout.c
# End Source File
# Begin Source File

SOURCE=.\Log.c
# End Source File
# Begin Source File

SOURCE=.\Logevent.c
# End Source File
# Begin Source File

SOURCE=.\Login.c
# End Source File
# Begin Source File

SOURCE=.\Main.c
# End Source File
# Begin Source File

SOURCE=.\Match.c
# End Source File
# Begin Source File

SOURCE=.\Message.c
# End Source File
# Begin Source File

SOURCE=.\Minput.c
# End Source File
# Begin Source File

SOURCE=.\Movebut.c
# End Source File
# Begin Source File

SOURCE=.\Newlogin.c
# End Source File
# Begin Source File

SOURCE=.\Nonfics.c
# End Source File
# Begin Source File

SOURCE=.\Observe.c
# End Source File
# Begin Source File

SOURCE=.\Oldlogin.c
# End Source File
# Begin Source File

SOURCE=.\Password.c
# End Source File
# Begin Source File

SOURCE=.\Play.c
# End Source File
# Begin Source File

SOURCE=.\Position.c
# End Source File
# Begin Source File

SOURCE=.\Promote.c
# End Source File
# Begin Source File

SOURCE=.\Question.c
# End Source File
# Begin Source File

SOURCE=.\Redclock.c
# End Source File
# Begin Source File

SOURCE=.\Rightm.c
# End Source File
# Begin Source File

SOURCE=.\Savegame.c
# End Source File
# Begin Source File

SOURCE=.\Seek.c
# End Source File
# Begin Source File

SOURCE=.\Server.c
# End Source File
# Begin Source File

SOURCE=.\Silence.c
# End Source File
# Begin Source File

SOURCE=.\Sitpiece.c
# End Source File
# Begin Source File

SOURCE=.\Smove.c
# End Source File
# Begin Source File

SOURCE=.\Sndcfg.c
# End Source File
# Begin Source File

SOURCE=.\Sound.c
# End Source File
# Begin Source File

SOURCE=.\State.c
# End Source File
# Begin Source File

SOURCE=.\Sys.c
# End Source File
# Begin Source File

SOURCE=.\Telcfg.c
# End Source File
# Begin Source File

SOURCE=.\Tell.c
# End Source File
# Begin Source File

SOURCE=.\Telnet.c
# End Source File
# Begin Source File

SOURCE=.\Thief.c
# End Source File
# Begin Source File

SOURCE=.\Timercfg.c
# End Source File
# Begin Source File

SOURCE=.\Timercmd.c
# End Source File
# Begin Source File

SOURCE=.\Timesnd.c
# End Source File
# Begin Source File

SOURCE=.\Tmove.c
# End Source File
# Begin Source File

SOURCE=.\Toolbox.c
# End Source File
# Begin Source File

SOURCE=.\Toption.c
# End Source File
# Begin Source File

SOURCE=.\TRY.C
# End Source File
# Begin Source File

SOURCE=.\Vars.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Dialog.h
# End Source File
# Begin Source File

SOURCE=.\Fics.h
# End Source File
# Begin Source File

SOURCE=.\Function.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\Icc.h
# End Source File
# Begin Source File

SOURCE=.\Menu.h
# End Source File
# Begin Source File

SOURCE=.\Nonfics.h
# End Source File
# Begin Source File

SOURCE=.\Thief.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\thief.RES
# End Source File
# End Target
# End Project
