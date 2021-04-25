# Microsoft Developer Studio Project File - Name="netbase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=netbase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netbase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netbase.mak" CFG="netbase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netbase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "netbase - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netbase - Win32 Release"

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
# ADD LIB32 /nologo /out:"../lib/Release\netbase.lib"

!ELSEIF  "$(CFG)" == "netbase - Win32 Debug"

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
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /FD /GZ /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/debug/netbase.lib"

!ENDIF 

# Begin Target

# Name "netbase - Win32 Release"
# Name "netbase - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\data_buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\gtp_related.cpp
# End Source File
# Begin Source File

SOURCE=.\net_app.cpp
# End Source File
# Begin Source File

SOURCE=.\net_status.cpp
# End Source File
# Begin Source File

SOURCE=.\network.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol_stack.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol_track.cpp
# End Source File
# Begin Source File

SOURCE=.\scan_app.cpp
# End Source File
# Begin Source File

SOURCE=.\scan_stack.cpp
# End Source File
# Begin Source File

SOURCE=.\slide_wnd_related.cpp
# End Source File
# Begin Source File

SOURCE=.\wlan_as_app.cpp
# End Source File
# Begin Source File

SOURCE=.\wlan_as_encap.cpp
# End Source File
# Begin Source File

SOURCE=.\wlan_as_stack.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\data_buffer.h
# End Source File
# Begin Source File

SOURCE=.\gtp_related.h
# End Source File
# Begin Source File

SOURCE=.\i_wlan_as_oci.h
# End Source File
# Begin Source File

SOURCE=.\net_app.h
# End Source File
# Begin Source File

SOURCE=.\net_status.h
# End Source File
# Begin Source File

SOURCE=.\network.h
# End Source File
# Begin Source File

SOURCE=.\protocol_stack.h
# End Source File
# Begin Source File

SOURCE=.\protocol_track.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\scan_app.h
# End Source File
# Begin Source File

SOURCE=.\scan_stack.h
# End Source File
# Begin Source File

SOURCE=.\slide_wnd_related.h
# End Source File
# Begin Source File

SOURCE=.\wlan_as_app.h
# End Source File
# Begin Source File

SOURCE=.\wlan_as_encap.h
# End Source File
# Begin Source File

SOURCE=.\wlan_as_stack.h
# End Source File
# End Group
# End Target
# End Project
