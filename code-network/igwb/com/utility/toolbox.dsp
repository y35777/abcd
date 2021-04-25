# Microsoft Developer Studio Project File - Name="toolbox" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=toolbox - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "toolbox.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "toolbox.mak" CFG="toolbox - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "toolbox - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "toolbox - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "toolbox - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\lib\Release\toolbox.lib"

!ELSEIF  "$(CFG)" == "toolbox - Win32 Debug"

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
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\debug\toolbox.lib"

!ENDIF 

# Begin Target

# Name "toolbox - Win32 Release"
# Name "toolbox - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\5ess_file.cpp
# End Source File
# Begin Source File

SOURCE=.\cc08_file.cpp
# End Source File
# Begin Source File

SOURCE=.\encrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\ewsd_file.cpp
# End Source File
# Begin Source File

SOURCE=.\frame_adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\igwb_file.cpp
# End Source File
# Begin Source File

SOURCE=.\igwb_file_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\mml_para.cpp
# End Source File
# Begin Source File

SOURCE=.\mml_report.cpp
# End Source File
# Begin Source File

SOURCE=.\perf_item.cpp
# End Source File
# Begin Source File

SOURCE=.\ps_file.cpp
# End Source File
# Begin Source File

SOURCE=.\qb_report.cpp
# End Source File
# Begin Source File

SOURCE=.\s1240_file.cpp
# End Source File
# Begin Source File

SOURCE=.\signal_tab.cpp
# End Source File
# Begin Source File

SOURCE=.\system1.cpp
# End Source File
# Begin Source File

SOURCE=.\text_file.cpp
# End Source File
# Begin Source File

SOURCE=.\zxj_file.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\5ess_file.h
# End Source File
# Begin Source File

SOURCE=.\cc08_file.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\encrypt.h
# End Source File
# Begin Source File

SOURCE=.\ewsd_file.h
# End Source File
# Begin Source File

SOURCE=.\frame_adapter.h
# End Source File
# Begin Source File

SOURCE=.\igwb_file.h
# End Source File
# Begin Source File

SOURCE=.\igwb_file_creator.h
# End Source File
# Begin Source File

SOURCE=.\mml_para.h
# End Source File
# Begin Source File

SOURCE=.\mml_report.h
# End Source File
# Begin Source File

SOURCE=.\ps_file.h
# End Source File
# Begin Source File

SOURCE=.\qb_report.h
# End Source File
# Begin Source File

SOURCE=.\s1240_file.h
# End Source File
# Begin Source File

SOURCE=..\include\signal_tab.h
# End Source File
# Begin Source File

SOURCE=.\text_file.h
# End Source File
# Begin Source File

SOURCE=.\zxj_file.h
# End Source File
# End Group
# End Target
# End Project
