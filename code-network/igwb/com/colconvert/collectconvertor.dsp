# Microsoft Developer Studio Project File - Name="collectconvertor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=collectconvertor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "collectconvertor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "collectconvertor.mak" CFG="collectconvertor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "collectconvertor - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "collectconvertor - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/iGWB V200配置库/综合采集/代码库/CCS V200R002/服务器端/com/colconvert", NISEAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "collectconvertor - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\lib\Release\collectconvertor.lib"

!ELSEIF  "$(CFG)" == "collectconvertor - Win32 Debug"

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
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_LIB_DEBUG_" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/Debug\collectconvertor.lib"

!ENDIF 

# Begin Target

# Name "collectconvertor - Win32 Release"
# Name "collectconvertor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\convert\5ess\5ess_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\5ess\5ess_convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\5ess\5ess_prefix_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\com\cc08_convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\collect_convert_base.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\collect_convert_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_convert_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_convert_obj_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\collect_convert_subobj.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_convertor.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\com\com_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\com\com_dir_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\comm_convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_ama_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_amas_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_mm_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_gb_convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_tp_collect_convert.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\com\tellin_convert_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\convert\com\zte_convert_fun.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\convert\5ess\5ess_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\5ess\5ess_convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\convert\5ess\5ess_prefix_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\com\cc08_convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\convert\collect_convert_base.h
# End Source File
# Begin Source File

SOURCE=.\include\collect_convert_creator.h
# End Source File
# Begin Source File

SOURCE=.\convert\include\collect_convert_define.h
# End Source File
# Begin Source File

SOURCE=.\include\collect_convert_include.h
# End Source File
# Begin Source File

SOURCE=.\collect_convert_obj.h
# End Source File
# Begin Source File

SOURCE=.\collect_convert_obj_factory.h
# End Source File
# Begin Source File

SOURCE=.\convert\include\collect_convert_struct.h
# End Source File
# Begin Source File

SOURCE=.\convert\collect_convert_subobj.h
# End Source File
# Begin Source File

SOURCE=.\collect_convertor.h
# End Source File
# Begin Source File

SOURCE=.\convert\com\com_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\com\com_dir_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\comm_convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_ama_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_amas_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\convert\ewsd\ewsd_mm_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\include\i_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_gb_convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\convert\s1240\s1240_tp_collect_convert.h
# End Source File
# Begin Source File

SOURCE=.\convert\com\tellin_convert_fun.h
# End Source File
# Begin Source File

SOURCE=.\convert\com\zte_convert_fun.h
# End Source File
# End Group
# End Target
# End Project
