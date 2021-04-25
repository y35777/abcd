# Microsoft Developer Studio Generated NMAKE File, Based on collector.dsp
!IF "$(CFG)" == ""
CFG=collector - Win32 Debug
!MESSAGE No configuration specified. Defaulting to collector - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "collector - Win32 Release" && "$(CFG)" != "collector - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "collector - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\release\collector.lib"


CLEAN :
	-@erase "$(INTDIR)\5ess_collect.obj"
	-@erase "$(INTDIR)\bk_link_mgr.obj"
	-@erase "$(INTDIR)\collect_base.obj"
	-@erase "$(INTDIR)\collect_factory.obj"
	-@erase "$(INTDIR)\collect_task.obj"
	-@erase "$(INTDIR)\collect_task_creator.obj"
	-@erase "$(INTDIR)\collect_task_obj.obj"
	-@erase "$(INTDIR)\collect_task_obj_factory.obj"
	-@erase "$(INTDIR)\collector.obj"
	-@erase "$(INTDIR)\collector_adapter.obj"
	-@erase "$(INTDIR)\collector_file_pool.obj"
	-@erase "$(INTDIR)\com_collect.obj"
	-@erase "$(INTDIR)\com_dir_collect.obj"
	-@erase "$(INTDIR)\ewsd_ama_collect.obj"
	-@erase "$(INTDIR)\ewsd_amas_collect.obj"
	-@erase "$(INTDIR)\ewsd_mm_collect.obj"
	-@erase "$(INTDIR)\file_protocol.obj"
	-@erase "$(INTDIR)\file_protocol_creator.obj"
	-@erase "$(INTDIR)\file_scan_protocol.obj"
	-@erase "$(INTDIR)\file_sn_manager.obj"
	-@erase "$(INTDIR)\ftp_protocol.obj"
	-@erase "$(INTDIR)\global_fun.obj"
	-@erase "$(INTDIR)\s1240_cmise_mgr.obj"
	-@erase "$(INTDIR)\s1240_collect.obj"
	-@erase "$(INTDIR)\s1240_tp_collect.obj"
	-@erase "$(INTDIR)\s1240_vertel_cmise.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vertel_ftam.obj"
	-@erase "$(INTDIR)\x25_link_check.obj"
	-@erase "..\lib\release\collector.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\collector.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\collector.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/release/collector.lib" 
LIB32_OBJS= \
	"$(INTDIR)\5ess_collect.obj" \
	"$(INTDIR)\bk_link_mgr.obj" \
	"$(INTDIR)\collect_base.obj" \
	"$(INTDIR)\collect_factory.obj" \
	"$(INTDIR)\collect_task.obj" \
	"$(INTDIR)\collect_task_creator.obj" \
	"$(INTDIR)\collect_task_obj.obj" \
	"$(INTDIR)\collect_task_obj_factory.obj" \
	"$(INTDIR)\collector.obj" \
	"$(INTDIR)\collector_adapter.obj" \
	"$(INTDIR)\collector_file_pool.obj" \
	"$(INTDIR)\com_collect.obj" \
	"$(INTDIR)\com_dir_collect.obj" \
	"$(INTDIR)\ewsd_ama_collect.obj" \
	"$(INTDIR)\ewsd_amas_collect.obj" \
	"$(INTDIR)\ewsd_mm_collect.obj" \
	"$(INTDIR)\file_protocol.obj" \
	"$(INTDIR)\file_protocol_creator.obj" \
	"$(INTDIR)\file_sn_manager.obj" \
	"$(INTDIR)\ftp_protocol.obj" \
	"$(INTDIR)\global_fun.obj" \
	"$(INTDIR)\s1240_cmise_mgr.obj" \
	"$(INTDIR)\s1240_collect.obj" \
	"$(INTDIR)\s1240_tp_collect.obj" \
	"$(INTDIR)\s1240_vertel_cmise.obj" \
	"$(INTDIR)\vertel_ftam.obj" \
	"$(INTDIR)\x25_link_check.obj" \
	"$(INTDIR)\file_scan_protocol.obj"

"..\lib\release\collector.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "collector - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\collector.lib"


CLEAN :
	-@erase "$(INTDIR)\5ess_collect.obj"
	-@erase "$(INTDIR)\bk_link_mgr.obj"
	-@erase "$(INTDIR)\collect_base.obj"
	-@erase "$(INTDIR)\collect_factory.obj"
	-@erase "$(INTDIR)\collect_task.obj"
	-@erase "$(INTDIR)\collect_task_creator.obj"
	-@erase "$(INTDIR)\collect_task_obj.obj"
	-@erase "$(INTDIR)\collect_task_obj_factory.obj"
	-@erase "$(INTDIR)\collector.obj"
	-@erase "$(INTDIR)\collector_adapter.obj"
	-@erase "$(INTDIR)\collector_file_pool.obj"
	-@erase "$(INTDIR)\com_collect.obj"
	-@erase "$(INTDIR)\com_dir_collect.obj"
	-@erase "$(INTDIR)\ewsd_ama_collect.obj"
	-@erase "$(INTDIR)\ewsd_amas_collect.obj"
	-@erase "$(INTDIR)\ewsd_mm_collect.obj"
	-@erase "$(INTDIR)\file_protocol.obj"
	-@erase "$(INTDIR)\file_protocol_creator.obj"
	-@erase "$(INTDIR)\file_scan_protocol.obj"
	-@erase "$(INTDIR)\file_sn_manager.obj"
	-@erase "$(INTDIR)\ftp_protocol.obj"
	-@erase "$(INTDIR)\global_fun.obj"
	-@erase "$(INTDIR)\s1240_cmise_mgr.obj"
	-@erase "$(INTDIR)\s1240_collect.obj"
	-@erase "$(INTDIR)\s1240_tp_collect.obj"
	-@erase "$(INTDIR)\s1240_vertel_cmise.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vertel_ftam.obj"
	-@erase "$(INTDIR)\x25_link_check.obj"
	-@erase "..\lib\debug\collector.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_LIB_DEBUG_" /Fp"$(INTDIR)\collector.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\collector.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/debug/collector.lib" 
LIB32_OBJS= \
	"$(INTDIR)\5ess_collect.obj" \
	"$(INTDIR)\bk_link_mgr.obj" \
	"$(INTDIR)\collect_base.obj" \
	"$(INTDIR)\collect_factory.obj" \
	"$(INTDIR)\collect_task.obj" \
	"$(INTDIR)\collect_task_creator.obj" \
	"$(INTDIR)\collect_task_obj.obj" \
	"$(INTDIR)\collect_task_obj_factory.obj" \
	"$(INTDIR)\collector.obj" \
	"$(INTDIR)\collector_adapter.obj" \
	"$(INTDIR)\collector_file_pool.obj" \
	"$(INTDIR)\com_collect.obj" \
	"$(INTDIR)\com_dir_collect.obj" \
	"$(INTDIR)\ewsd_ama_collect.obj" \
	"$(INTDIR)\ewsd_amas_collect.obj" \
	"$(INTDIR)\ewsd_mm_collect.obj" \
	"$(INTDIR)\file_protocol.obj" \
	"$(INTDIR)\file_protocol_creator.obj" \
	"$(INTDIR)\file_sn_manager.obj" \
	"$(INTDIR)\ftp_protocol.obj" \
	"$(INTDIR)\global_fun.obj" \
	"$(INTDIR)\s1240_cmise_mgr.obj" \
	"$(INTDIR)\s1240_collect.obj" \
	"$(INTDIR)\s1240_tp_collect.obj" \
	"$(INTDIR)\s1240_vertel_cmise.obj" \
	"$(INTDIR)\vertel_ftam.obj" \
	"$(INTDIR)\x25_link_check.obj" \
	"$(INTDIR)\file_scan_protocol.obj"

"..\lib\debug\collector.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("collector.dep")
!INCLUDE "collector.dep"
!ELSE 
!MESSAGE Warning: cannot find "collector.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "collector - Win32 Release" || "$(CFG)" == "collector - Win32 Debug"
SOURCE=.\collect_task\5ess\5ess_collect.cpp

"$(INTDIR)\5ess_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\bk_link_mgr.cpp

"$(INTDIR)\bk_link_mgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\collect_base.cpp

"$(INTDIR)\collect_base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\collect_factory.cpp

"$(INTDIR)\collect_factory.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\collect_task.cpp

"$(INTDIR)\collect_task.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\collect_task_creator.cpp

"$(INTDIR)\collect_task_creator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task_obj.cpp

"$(INTDIR)\collect_task_obj.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\collect_task_obj_factory.cpp

"$(INTDIR)\collect_task_obj_factory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\collector.cpp

"$(INTDIR)\collector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\collector_adapter.cpp

"$(INTDIR)\collector_adapter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\collector_file_pool.cpp

"$(INTDIR)\collector_file_pool.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\collect_task\com\com_collect.cpp

"$(INTDIR)\com_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\com\com_dir_collect.cpp

"$(INTDIR)\com_dir_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\ewsd\ewsd_ama_collect.cpp

"$(INTDIR)\ewsd_ama_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\ewsd\ewsd_amas_collect.cpp

"$(INTDIR)\ewsd_amas_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\ewsd\ewsd_mm_collect.cpp

"$(INTDIR)\ewsd_mm_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\file_protocol.cpp

"$(INTDIR)\file_protocol.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\file_protocol_creator.cpp

"$(INTDIR)\file_protocol_creator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\file_scan_protocol.cpp

"$(INTDIR)\file_scan_protocol.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\file_sn_manager.cpp

"$(INTDIR)\file_sn_manager.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\ftp_protocol.cpp

"$(INTDIR)\ftp_protocol.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\global_fun.cpp

"$(INTDIR)\global_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\s1240\s1240_cmise_mgr.cpp

"$(INTDIR)\s1240_cmise_mgr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\s1240\s1240_collect.cpp

"$(INTDIR)\s1240_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\s1240\s1240_tp_collect.cpp

"$(INTDIR)\s1240_tp_collect.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\s1240\s1240_vertel_cmise.cpp

"$(INTDIR)\s1240_vertel_cmise.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\vertel_ftam.cpp

"$(INTDIR)\vertel_ftam.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_task\x25_link_check.cpp

"$(INTDIR)\x25_link_check.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

