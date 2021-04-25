# Microsoft Developer Studio Project File - Name="perf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=perf - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "perf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "perf.mak" CFG="perf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "perf - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "perf - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "perf - Win32 Release"

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
# ADD CPP /nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/Release\perf.lib"

!ELSEIF  "$(CFG)" == "perf - Win32 Debug"

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
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\debug\perf.lib"

!ENDIF 

# Begin Target

# Name "perf - Win32 Release"
# Name "perf - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\collect_statistic.cpp
# End Source File
# Begin Source File

SOURCE=.\dda_proxy.cpp
# End Source File
# Begin Source File

SOURCE=.\ddaquery.cpp
# End Source File
# Begin Source File

SOURCE=.\ddaset.cpp
# End Source File
# Begin Source File

SOURCE=.\perf_nt\nt_perf_db.cpp
# End Source File
# Begin Source File

SOURCE=.\perf_nt\PdhUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\perf.cpp
# End Source File
# Begin Source File

SOURCE=.\perf_bin_util.cpp
# End Source File
# Begin Source File

SOURCE=.\perf_encap.cpp
# End Source File
# Begin Source File

SOURCE=.\perf_task.cpp
# End Source File
# Begin Source File

SOURCE=.\perf_task_global.cpp
# End Source File
# Begin Source File

SOURCE=.\query_executive.cpp
# End Source File
# Begin Source File

SOURCE=.\task_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\task_manager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\collect_statistic.h
# End Source File
# Begin Source File

SOURCE=.\dda_proxy.h
# End Source File
# Begin Source File

SOURCE=.\ddaquery.h
# End Source File
# Begin Source File

SOURCE=.\ddaset.h
# End Source File
# Begin Source File

SOURCE=.\perf_nt\nt_perf_db.h
# End Source File
# Begin Source File

SOURCE=.\os_perf_db.h
# End Source File
# Begin Source File

SOURCE=.\perf_nt\PdhUtility.h
# End Source File
# Begin Source File

SOURCE=.\perf.h
# End Source File
# Begin Source File

SOURCE=.\perf_bin_util.h
# End Source File
# Begin Source File

SOURCE=.\perf_encap.h
# End Source File
# Begin Source File

SOURCE=.\perf_hdr.h
# End Source File
# Begin Source File

SOURCE=.\perf_task.h
# End Source File
# Begin Source File

SOURCE=.\perf_task_global.h
# End Source File
# Begin Source File

SOURCE=.\query_executive.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\perf_nt\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\task_creator.h
# End Source File
# Begin Source File

SOURCE=.\task_manager.h
# End Source File
# End Group
# End Target
# End Project
