# Microsoft Developer Studio Project File - Name="alarm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=alarm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "alarm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "alarm.mak" CFG="alarm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "alarm - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "alarm - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/iGWB V200ÅäÖÃ¿â/´úÂë¿â/iGWB V200R002/·þÎñÆ÷¶Ë/com/alarm", PXTDAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "alarm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_NO_SNMPv3" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\Release\alarm.lib"

!ELSEIF  "$(CFG)" == "alarm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_NO_SNMPv3" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/Debug/alarm.lib"

!ENDIF 

# Begin Target

# Name "alarm - Win32 Release"
# Name "alarm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\active_alarm_list.cpp
# End Source File
# Begin Source File

SOURCE=.\alarm_bin_util.cpp
# End Source File
# Begin Source File

SOURCE=.\alarm_box.cpp
# End Source File
# Begin Source File

SOURCE=.\alarm_encap.cpp
# End Source File
# Begin Source File

SOURCE=.\alarm_handler.cpp
# End Source File
# Begin Source File

SOURCE=.\alarm_mml_report.cpp
# End Source File
# Begin Source File

SOURCE=.\alarm_server.cpp
# End Source File
# Begin Source File

SOURCE=.\bin_alarm_handler.cpp
# End Source File
# Begin Source File

SOURCE=.\box_comm.cpp
# End Source File
# Begin Source File

SOURCE=.\history_alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\mml_alarm_handler.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\active_alarm_list.h
# End Source File
# Begin Source File

SOURCE=.\alarm_bin_util.h
# End Source File
# Begin Source File

SOURCE=.\alarm_box.h
# End Source File
# Begin Source File

SOURCE=.\alarm_encap.h
# End Source File
# Begin Source File

SOURCE=.\alarm_handler.h
# End Source File
# Begin Source File

SOURCE=.\alarm_mml_report.h
# End Source File
# Begin Source File

SOURCE=.\alarm_server.h
# End Source File
# Begin Source File

SOURCE=.\bin_alarm_handler.h
# End Source File
# Begin Source File

SOURCE=.\box_comm.h
# End Source File
# Begin Source File

SOURCE=.\history_alarm.h
# End Source File
# Begin Source File

SOURCE=.\mml_alarm_handler.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "hd_monitor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hd_monitor\hd_monitor_proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_monitor\hd_monitor_proxy.h
# End Source File
# Begin Source File

SOURCE=.\hd_monitor\HDmonitor.h
# End Source File
# Begin Source File

SOURCE=.\hd_monitor\v2_hd_monitor_proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\hd_monitor\v2_hd_monitor_proxy.h
# End Source File
# End Group
# Begin Group "snmp_agent"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\snmp_agent\snmp_agent_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\snmp_agent\snmp_agent_obj.h
# End Source File
# End Group
# End Target
# End Project
