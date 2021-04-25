# Microsoft Developer Studio Generated NMAKE File, Based on clustermanager.dsp
!IF "$(CFG)" == ""
CFG=clustermanager - Win32 Debug
!MESSAGE No configuration specified. Defaulting to clustermanager - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "clustermanager - Win32 Release" && "$(CFG)" != "clustermanager - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "clustermanager - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\clustermanager.lib"


CLEAN :
	-@erase "$(INTDIR)\asyn_res_kernel.obj"
	-@erase "$(INTDIR)\asyn_res_object.obj"
	-@erase "$(INTDIR)\back_save_info_file.obj"
	-@erase "$(INTDIR)\BtwIpHelper.obj"
	-@erase "$(INTDIR)\BtwService.obj"
	-@erase "$(INTDIR)\cluster_alarm.obj"
	-@erase "$(INTDIR)\cluster_commtask.obj"
	-@erase "$(INTDIR)\cluster_creator.obj"
	-@erase "$(INTDIR)\cluster_module.obj"
	-@erase "$(INTDIR)\cluster_mutex.obj"
	-@erase "$(INTDIR)\cluster_network.obj"
	-@erase "$(INTDIR)\cluster_toolbox.obj"
	-@erase "$(INTDIR)\com_cluster.obj"
	-@erase "$(INTDIR)\comm_link.obj"
	-@erase "$(INTDIR)\comm_port.obj"
	-@erase "$(INTDIR)\double_info_file.obj"
	-@erase "$(INTDIR)\heartbeat_creator.obj"
	-@erase "$(INTDIR)\heartbeat_link.obj"
	-@erase "$(INTDIR)\igwb_alarm.obj"
	-@erase "$(INTDIR)\logical_cluster.obj"
	-@erase "$(INTDIR)\msc_sync_data.obj"
	-@erase "$(INTDIR)\net_info_file.obj"
	-@erase "$(INTDIR)\ping.obj"
	-@erase "$(INTDIR)\RaidHDCheck.obj"
	-@erase "$(INTDIR)\RaidHDChkCreator.obj"
	-@erase "$(INTDIR)\res_ibm_volume.obj"
	-@erase "$(INTDIR)\res_ip.obj"
	-@erase "$(INTDIR)\res_manager.obj"
	-@erase "$(INTDIR)\res_object.obj"
	-@erase "$(INTDIR)\res_object_creator.obj"
	-@erase "$(INTDIR)\res_program.obj"
	-@erase "$(INTDIR)\res_service.obj"
	-@erase "$(INTDIR)\res_user_defined.obj"
	-@erase "$(INTDIR)\res_volume.obj"
	-@erase "$(INTDIR)\save_info_file.obj"
	-@erase "$(INTDIR)\ShellShow.obj"
	-@erase "$(INTDIR)\sync_data.obj"
	-@erase "$(INTDIR)\sync_data_creator.obj"
	-@erase "$(INTDIR)\sync_info_file.obj"
	-@erase "$(INTDIR)\sync_manager.obj"
	-@erase "$(INTDIR)\udp_link.obj"
	-@erase "$(INTDIR)\udp_socket.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\clustermanager.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\clustermanager.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\clustermanager.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\clustermanager.lib" 
LIB32_OBJS= \
	"$(INTDIR)\asyn_res_kernel.obj" \
	"$(INTDIR)\asyn_res_object.obj" \
	"$(INTDIR)\BtwIpHelper.obj" \
	"$(INTDIR)\BtwService.obj" \
	"$(INTDIR)\cluster_alarm.obj" \
	"$(INTDIR)\cluster_commtask.obj" \
	"$(INTDIR)\cluster_creator.obj" \
	"$(INTDIR)\cluster_module.obj" \
	"$(INTDIR)\cluster_mutex.obj" \
	"$(INTDIR)\cluster_network.obj" \
	"$(INTDIR)\cluster_toolbox.obj" \
	"$(INTDIR)\com_cluster.obj" \
	"$(INTDIR)\comm_link.obj" \
	"$(INTDIR)\comm_port.obj" \
	"$(INTDIR)\double_info_file.obj" \
	"$(INTDIR)\heartbeat_creator.obj" \
	"$(INTDIR)\heartbeat_link.obj" \
	"$(INTDIR)\igwb_alarm.obj" \
	"$(INTDIR)\logical_cluster.obj" \
	"$(INTDIR)\msc_sync_data.obj" \
	"$(INTDIR)\net_info_file.obj" \
	"$(INTDIR)\ping.obj" \
	"$(INTDIR)\RaidHDCheck.obj" \
	"$(INTDIR)\RaidHDChkCreator.obj" \
	"$(INTDIR)\res_ibm_volume.obj" \
	"$(INTDIR)\res_ip.obj" \
	"$(INTDIR)\res_manager.obj" \
	"$(INTDIR)\res_object.obj" \
	"$(INTDIR)\res_object_creator.obj" \
	"$(INTDIR)\res_program.obj" \
	"$(INTDIR)\res_service.obj" \
	"$(INTDIR)\res_user_defined.obj" \
	"$(INTDIR)\res_volume.obj" \
	"$(INTDIR)\save_info_file.obj" \
	"$(INTDIR)\ShellShow.obj" \
	"$(INTDIR)\sync_data.obj" \
	"$(INTDIR)\sync_data_creator.obj" \
	"$(INTDIR)\sync_info_file.obj" \
	"$(INTDIR)\sync_manager.obj" \
	"$(INTDIR)\udp_link.obj" \
	"$(INTDIR)\udp_socket.obj" \
	"$(INTDIR)\back_save_info_file.obj"

"..\lib\Release\clustermanager.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "clustermanager - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\clustermanager.lib"


CLEAN :
	-@erase "$(INTDIR)\asyn_res_kernel.obj"
	-@erase "$(INTDIR)\asyn_res_object.obj"
	-@erase "$(INTDIR)\back_save_info_file.obj"
	-@erase "$(INTDIR)\BtwIpHelper.obj"
	-@erase "$(INTDIR)\BtwService.obj"
	-@erase "$(INTDIR)\cluster_alarm.obj"
	-@erase "$(INTDIR)\cluster_commtask.obj"
	-@erase "$(INTDIR)\cluster_creator.obj"
	-@erase "$(INTDIR)\cluster_module.obj"
	-@erase "$(INTDIR)\cluster_mutex.obj"
	-@erase "$(INTDIR)\cluster_network.obj"
	-@erase "$(INTDIR)\cluster_toolbox.obj"
	-@erase "$(INTDIR)\com_cluster.obj"
	-@erase "$(INTDIR)\comm_link.obj"
	-@erase "$(INTDIR)\comm_port.obj"
	-@erase "$(INTDIR)\double_info_file.obj"
	-@erase "$(INTDIR)\heartbeat_creator.obj"
	-@erase "$(INTDIR)\heartbeat_link.obj"
	-@erase "$(INTDIR)\igwb_alarm.obj"
	-@erase "$(INTDIR)\logical_cluster.obj"
	-@erase "$(INTDIR)\msc_sync_data.obj"
	-@erase "$(INTDIR)\net_info_file.obj"
	-@erase "$(INTDIR)\ping.obj"
	-@erase "$(INTDIR)\RaidHDCheck.obj"
	-@erase "$(INTDIR)\RaidHDChkCreator.obj"
	-@erase "$(INTDIR)\res_ibm_volume.obj"
	-@erase "$(INTDIR)\res_ip.obj"
	-@erase "$(INTDIR)\res_manager.obj"
	-@erase "$(INTDIR)\res_object.obj"
	-@erase "$(INTDIR)\res_object_creator.obj"
	-@erase "$(INTDIR)\res_program.obj"
	-@erase "$(INTDIR)\res_service.obj"
	-@erase "$(INTDIR)\res_user_defined.obj"
	-@erase "$(INTDIR)\res_volume.obj"
	-@erase "$(INTDIR)\save_info_file.obj"
	-@erase "$(INTDIR)\ShellShow.obj"
	-@erase "$(INTDIR)\sync_data.obj"
	-@erase "$(INTDIR)\sync_data_creator.obj"
	-@erase "$(INTDIR)\sync_info_file.obj"
	-@erase "$(INTDIR)\sync_manager.obj"
	-@erase "$(INTDIR)\udp_link.obj"
	-@erase "$(INTDIR)\udp_socket.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\clustermanager.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_AFXDLL" /Fp"$(INTDIR)\clustermanager.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\clustermanager.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\clustermanager.lib" 
LIB32_OBJS= \
	"$(INTDIR)\asyn_res_kernel.obj" \
	"$(INTDIR)\asyn_res_object.obj" \
	"$(INTDIR)\BtwIpHelper.obj" \
	"$(INTDIR)\BtwService.obj" \
	"$(INTDIR)\cluster_alarm.obj" \
	"$(INTDIR)\cluster_commtask.obj" \
	"$(INTDIR)\cluster_creator.obj" \
	"$(INTDIR)\cluster_module.obj" \
	"$(INTDIR)\cluster_mutex.obj" \
	"$(INTDIR)\cluster_network.obj" \
	"$(INTDIR)\cluster_toolbox.obj" \
	"$(INTDIR)\com_cluster.obj" \
	"$(INTDIR)\comm_link.obj" \
	"$(INTDIR)\comm_port.obj" \
	"$(INTDIR)\double_info_file.obj" \
	"$(INTDIR)\heartbeat_creator.obj" \
	"$(INTDIR)\heartbeat_link.obj" \
	"$(INTDIR)\igwb_alarm.obj" \
	"$(INTDIR)\logical_cluster.obj" \
	"$(INTDIR)\msc_sync_data.obj" \
	"$(INTDIR)\net_info_file.obj" \
	"$(INTDIR)\ping.obj" \
	"$(INTDIR)\RaidHDCheck.obj" \
	"$(INTDIR)\RaidHDChkCreator.obj" \
	"$(INTDIR)\res_ibm_volume.obj" \
	"$(INTDIR)\res_ip.obj" \
	"$(INTDIR)\res_manager.obj" \
	"$(INTDIR)\res_object.obj" \
	"$(INTDIR)\res_object_creator.obj" \
	"$(INTDIR)\res_program.obj" \
	"$(INTDIR)\res_service.obj" \
	"$(INTDIR)\res_user_defined.obj" \
	"$(INTDIR)\res_volume.obj" \
	"$(INTDIR)\save_info_file.obj" \
	"$(INTDIR)\ShellShow.obj" \
	"$(INTDIR)\sync_data.obj" \
	"$(INTDIR)\sync_data_creator.obj" \
	"$(INTDIR)\sync_info_file.obj" \
	"$(INTDIR)\sync_manager.obj" \
	"$(INTDIR)\udp_link.obj" \
	"$(INTDIR)\udp_socket.obj" \
	"$(INTDIR)\back_save_info_file.obj"

"..\lib\debug\clustermanager.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("clustermanager.dep")
!INCLUDE "clustermanager.dep"
!ELSE 
!MESSAGE Warning: cannot find "clustermanager.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "clustermanager - Win32 Release" || "$(CFG)" == "clustermanager - Win32 Debug"
SOURCE=.\asyn_res_kernel.cpp

"$(INTDIR)\asyn_res_kernel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\asyn_res_object.cpp

"$(INTDIR)\asyn_res_object.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\back_save_info_file.cpp

"$(INTDIR)\back_save_info_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BtwIpHelper.cpp

"$(INTDIR)\BtwIpHelper.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BtwService.cpp

"$(INTDIR)\BtwService.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster_alarm.cpp

"$(INTDIR)\cluster_alarm.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster_commtask.cpp

"$(INTDIR)\cluster_commtask.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster_creator.cpp

"$(INTDIR)\cluster_creator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster_module.cpp

"$(INTDIR)\cluster_module.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster_mutex.cpp

"$(INTDIR)\cluster_mutex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster_network.cpp

"$(INTDIR)\cluster_network.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cluster_toolbox.cpp

"$(INTDIR)\cluster_toolbox.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\com_cluster.cpp

"$(INTDIR)\com_cluster.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\comm_link.cpp

"$(INTDIR)\comm_link.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\comm_port.cpp

"$(INTDIR)\comm_port.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\double_info_file.cpp

"$(INTDIR)\double_info_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\heartbeat_creator.cpp

"$(INTDIR)\heartbeat_creator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\heartbeat_link.cpp

"$(INTDIR)\heartbeat_link.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\igwb_alarm.cpp

"$(INTDIR)\igwb_alarm.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\logical_cluster.cpp

"$(INTDIR)\logical_cluster.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\msc_sync_data.cpp

"$(INTDIR)\msc_sync_data.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\net_info_file.cpp

"$(INTDIR)\net_info_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ping.cpp

"$(INTDIR)\ping.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RaidHDCheck.cpp

"$(INTDIR)\RaidHDCheck.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RaidHDChkCreator.cpp

"$(INTDIR)\RaidHDChkCreator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_ibm_volume.cpp

"$(INTDIR)\res_ibm_volume.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_ip.cpp

"$(INTDIR)\res_ip.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_manager.cpp

"$(INTDIR)\res_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_object.cpp

"$(INTDIR)\res_object.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_object_creator.cpp

"$(INTDIR)\res_object_creator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_program.cpp

"$(INTDIR)\res_program.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_service.cpp

"$(INTDIR)\res_service.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_user_defined.cpp

"$(INTDIR)\res_user_defined.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_volume.cpp

"$(INTDIR)\res_volume.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\save_info_file.cpp

"$(INTDIR)\save_info_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ShellShow.cpp

"$(INTDIR)\ShellShow.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sync_data.cpp

"$(INTDIR)\sync_data.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sync_data_creator.cpp

"$(INTDIR)\sync_data_creator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sync_info_file.cpp

"$(INTDIR)\sync_info_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sync_manager.cpp

"$(INTDIR)\sync_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\udp_link.cpp

"$(INTDIR)\udp_link.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\udp_socket.cpp

"$(INTDIR)\udp_socket.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

