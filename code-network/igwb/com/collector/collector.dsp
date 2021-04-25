# Microsoft Developer Studio Project File - Name="collector" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=collector - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "collector.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "collector.mak" CFG="collector - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "collector - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "collector - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/iGWB V200配置库/综合采集/代码库/CCS V200R002/服务器端/com/collector", SOFEAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "collector - Win32 Release"

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
# ADD LIB32 /nologo /out:"../lib/release/collector.lib"

!ELSEIF  "$(CFG)" == "collector - Win32 Debug"

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
# ADD LIB32 /nologo /out:"../lib/debug/collector.lib"

!ENDIF 

# Begin Target

# Name "collector - Win32 Release"
# Name "collector - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\collect_task\5ess\5ess_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\bk_link_mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_base.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_task.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_task_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task_obj_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\collector.cpp
# End Source File
# Begin Source File

SOURCE=.\collector_adapter.cpp
# End Source File
# Begin Source File

SOURCE=.\collector_file_pool.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\com\com_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\com\com_dir_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\ewsd\ewsd_ama_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\ewsd\ewsd_amas_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\ewsd\ewsd_mm_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_protocol_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_scan_protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_sn_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\ftp_protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\global_fun.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_cmise_mgr.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_tp_collect.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_vertel_cmise.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\vertel_ftam.cpp
# End Source File
# Begin Source File

SOURCE=.\collect_task\x25_link_check.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\collect_task\5ess\5ess_collect.h
# End Source File
# Begin Source File

SOURCE=.\include\adapter_include.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\bk_link_mgr.h
# End Source File
# Begin Source File

SOURCE=.\include\cmise_cmd_code.h
# End Source File
# Begin Source File

SOURCE=.\include\cmise_creator.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_base.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_define.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_factory.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_include.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_struct.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\collect_task.h
# End Source File
# Begin Source File

SOURCE=.\include\collect_task_creator.h
# End Source File
# Begin Source File

SOURCE=.\collect_task_obj.h
# End Source File
# Begin Source File

SOURCE=.\collect_task_obj_factory.h
# End Source File
# Begin Source File

SOURCE=.\collector.h
# End Source File
# Begin Source File

SOURCE=.\collector_adapter.h
# End Source File
# Begin Source File

SOURCE=.\collector_file_pool.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\com\com_collect.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\com\com_dir_collect.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\ewsd\ewsd_ama_collect.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\ewsd\ewsd_amas_collect.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\ewsd\ewsd_mm_collect.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_protocol.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_protocol_creator.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_scan_protocol.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\file_sn_manager.h
# End Source File
# Begin Source File

SOURCE=.\include\ftam_cmd_code.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\ftp_protocol.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\global_fun.h
# End Source File
# Begin Source File

SOURCE=.\include\i_cmise_fun.h
# End Source File
# Begin Source File

SOURCE=.\include\i_collect_task.h
# End Source File
# Begin Source File

SOURCE=.\include\i_ftam.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\link_base.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\link_group.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\link_mgr.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\link_mgr_factory.h
# End Source File
# Begin Source File

SOURCE=.\include\res_config.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_cmise.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_cmise_group.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_cmise_mgr.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_collect.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_tp_collect.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\s1240\s1240_vertel_cmise.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\vertel_ftam.h
# End Source File
# Begin Source File

SOURCE=.\collect_task\x25_link_check.h
# End Source File
# End Group
# End Target
# End Project
