# Microsoft Developer Studio Project File - Name="format" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=format - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "format.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "format.mak" CFG="format - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "format - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "format - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/iGWB V200ÅäÖÃ¿â/´úÂë¿â/iGWB V200R002B02/·þÎñÆ÷¶Ë/com/format", QEWEAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "format - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FORMAT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FORMAT_EXPORTS" /D "_PLATFORM_WIN32" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ace.lib toolbase.lib /nologo /dll /map /machine:I386

!ELSEIF  "$(CFG)" == "format - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib\debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FORMAT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FORMAT_EXPORTS" /D "_PLATFORM_WIN32" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 aced.lib toolbase.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "format - Win32 Release"
# Name "format - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\access_point.cpp
# End Source File
# Begin Source File

SOURCE=.\channel.cpp
# End Source File
# Begin Source File

SOURCE=.\condition.cpp
# End Source File
# Begin Source File

SOURCE=.\convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\dis_condition.cpp
# End Source File
# Begin Source File

SOURCE=.\format.cpp
# End Source File
# Begin Source File

SOURCE=.\format_global.cpp
# End Source File
# Begin Source File

SOURCE=.\format_lib.cpp
# End Source File
# Begin Source File

SOURCE=.\ibill_processor_imp.cpp
# End Source File
# Begin Source File

SOURCE=.\iformat_cfg_imp.cpp
# End Source File
# Begin Source File

SOURCE=.\igwb.cpp
# End Source File
# Begin Source File

SOURCE=.\iquery_browse_imp.cpp
# End Source File
# Begin Source File

SOURCE=.\postfix_exp.cpp
# End Source File
# Begin Source File

SOURCE=.\utility_ex.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\access_point.h
# End Source File
# Begin Source File

SOURCE=.\channel.h
# End Source File
# Begin Source File

SOURCE=.\condition.h
# End Source File
# Begin Source File

SOURCE=.\convert.h
# End Source File
# Begin Source File

SOURCE=.\convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\dis_condition.h
# End Source File
# Begin Source File

SOURCE=.\format.h
# End Source File
# Begin Source File

SOURCE=.\format_comm.h
# End Source File
# Begin Source File

SOURCE=.\format_def.h
# End Source File
# Begin Source File

SOURCE=.\format_global.h
# End Source File
# Begin Source File

SOURCE=.\format_lib.h
# End Source File
# Begin Source File

SOURCE=.\format_out.h
# End Source File
# Begin Source File

SOURCE=.\ibill_processor.h
# End Source File
# Begin Source File

SOURCE=.\ibill_processor_imp.h
# End Source File
# Begin Source File

SOURCE=.\iformat_cfg.h
# End Source File
# Begin Source File

SOURCE=.\iformat_cfg_imp.h
# End Source File
# Begin Source File

SOURCE=.\igwb.h
# End Source File
# Begin Source File

SOURCE=.\iquery_browse.h
# End Source File
# Begin Source File

SOURCE=.\iquery_browse_imp.h
# End Source File
# Begin Source File

SOURCE=.\postfix_exp.h
# End Source File
# Begin Source File

SOURCE=.\utility_ex.h
# End Source File
# End Group
# End Target
# End Project
