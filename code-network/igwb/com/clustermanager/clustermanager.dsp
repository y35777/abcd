# Microsoft Developer Studio Project File - Name="clustermanager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=clustermanager - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "clustermanager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "clustermanager.mak" CFG="clustermanager - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "clustermanager - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "clustermanager - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "clustermanager - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "_PLATFORM_WIN32" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\Release\clustermanager.lib"

!ELSEIF  "$(CFG)" == "clustermanager - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_AFXDLL" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\debug\clustermanager.lib"

!ENDIF 

# Begin Target

# Name "clustermanager - Win32 Release"
# Name "clustermanager - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\asyn_res_kernel.cpp
# End Source File
# Begin Source File

SOURCE=.\asyn_res_object.cpp
# End Source File
# Begin Source File

SOURCE=.\back_save_info_file.cpp
# End Source File
# Begin Source File

SOURCE=.\BtwIpHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\BtwService.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster_alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster_commtask.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster_module.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster_mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster_network.cpp
# End Source File
# Begin Source File

SOURCE=.\cluster_toolbox.cpp
# End Source File
# Begin Source File

SOURCE=.\com_cluster.cpp
# End Source File
# Begin Source File

SOURCE=.\comm_link.cpp
# End Source File
# Begin Source File

SOURCE=.\comm_port.cpp
# End Source File
# Begin Source File

SOURCE=.\double_info_file.cpp
# End Source File
# Begin Source File

SOURCE=.\heartbeat_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\heartbeat_link.cpp
# End Source File
# Begin Source File

SOURCE=.\igwb_alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\logical_cluster.cpp
# End Source File
# Begin Source File

SOURCE=.\msc_sync_data.cpp
# End Source File
# Begin Source File

SOURCE=.\net_info_file.cpp
# End Source File
# Begin Source File

SOURCE=.\ping.cpp
# End Source File
# Begin Source File

SOURCE=.\RaidHDCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\RaidHDChkCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\res_ibm_volume.cpp
# End Source File
# Begin Source File

SOURCE=.\res_ip.cpp
# End Source File
# Begin Source File

SOURCE=.\res_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\res_object.cpp
# End Source File
# Begin Source File

SOURCE=.\res_object_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\res_program.cpp
# End Source File
# Begin Source File

SOURCE=.\res_service.cpp
# End Source File
# Begin Source File

SOURCE=.\res_user_defined.cpp
# End Source File
# Begin Source File

SOURCE=.\res_volume.cpp
# End Source File
# Begin Source File

SOURCE=.\save_info_file.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellShow.cpp
# End Source File
# Begin Source File

SOURCE=.\sync_data.cpp
# End Source File
# Begin Source File

SOURCE=.\sync_data_creator.cpp
# End Source File
# Begin Source File

SOURCE=.\sync_info_file.cpp
# End Source File
# Begin Source File

SOURCE=.\sync_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\udp_link.cpp
# End Source File
# Begin Source File

SOURCE=.\udp_socket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\asyn_res_kernel.h
# End Source File
# Begin Source File

SOURCE=.\asyn_res_object.h
# End Source File
# Begin Source File

SOURCE=.\back_save_info_file.h
# End Source File
# Begin Source File

SOURCE=.\BtwIpHelper.h
# End Source File
# Begin Source File

SOURCE=.\BtwService.h
# End Source File
# Begin Source File

SOURCE=.\cluster_alarm.h
# End Source File
# Begin Source File

SOURCE=.\cluster_commtask.h
# End Source File
# Begin Source File

SOURCE=.\cluster_creator.h
# End Source File
# Begin Source File

SOURCE=.\cluster_module.h
# End Source File
# Begin Source File

SOURCE=.\cluster_mutex.h
# End Source File
# Begin Source File

SOURCE=.\cluster_network.h
# End Source File
# Begin Source File

SOURCE=.\cluster_struct.h
# End Source File
# Begin Source File

SOURCE=.\cluster_toolbox.h
# End Source File
# Begin Source File

SOURCE=.\com_cluster.h
# End Source File
# Begin Source File

SOURCE=.\comm_link.h
# End Source File
# Begin Source File

SOURCE=.\comm_port.h
# End Source File
# Begin Source File

SOURCE=.\double_info_file.h
# End Source File
# Begin Source File

SOURCE=.\heartbeat_creator.h
# End Source File
# Begin Source File

SOURCE=.\heartbeat_link.h
# End Source File
# Begin Source File

SOURCE=.\igwb_alarm.h
# End Source File
# Begin Source File

SOURCE=.\logical_cluster.h
# End Source File
# Begin Source File

SOURCE=.\msc_sync_data.h
# End Source File
# Begin Source File

SOURCE=.\net_info_file.h
# End Source File
# Begin Source File

SOURCE=.\ping.h
# End Source File
# Begin Source File

SOURCE=.\RaidHDCheck.h
# End Source File
# Begin Source File

SOURCE=.\RaidHDChkCreator.h
# End Source File
# Begin Source File

SOURCE=.\RAIDHLP.h
# End Source File
# Begin Source File

SOURCE=.\res_ibm_volume.h
# End Source File
# Begin Source File

SOURCE=.\res_ip.h
# End Source File
# Begin Source File

SOURCE=.\res_manager.h
# End Source File
# Begin Source File

SOURCE=.\res_object.h
# End Source File
# Begin Source File

SOURCE=.\res_object_creator.h
# End Source File
# Begin Source File

SOURCE=.\res_program.h
# End Source File
# Begin Source File

SOURCE=.\res_service.h
# End Source File
# Begin Source File

SOURCE=.\res_user_defined.h
# End Source File
# Begin Source File

SOURCE=.\res_volume.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\save_info_file.h
# End Source File
# Begin Source File

SOURCE=.\ShellShow.h
# End Source File
# Begin Source File

SOURCE=.\sync_data.h
# End Source File
# Begin Source File

SOURCE=.\sync_data_creator.h
# End Source File
# Begin Source File

SOURCE=.\sync_info_file.h
# End Source File
# Begin Source File

SOURCE=.\sync_manager.h
# End Source File
# Begin Source File

SOURCE=.\sync_struct.h
# End Source File
# Begin Source File

SOURCE=.\udp_link.h
# End Source File
# Begin Source File

SOURCE=.\udp_socket.h
# End Source File
# End Group
# End Target
# End Project
