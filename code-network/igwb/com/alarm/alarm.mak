# Microsoft Developer Studio Generated NMAKE File, Based on alarm.dsp
!IF "$(CFG)" == ""
CFG=alarm - Win32 Debug
!MESSAGE No configuration specified. Defaulting to alarm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "alarm - Win32 Release" && "$(CFG)" != "alarm - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "alarm - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\alarm.lib"


CLEAN :
	-@erase "$(INTDIR)\active_alarm_list.obj"
	-@erase "$(INTDIR)\alarm_bin_util.obj"
	-@erase "$(INTDIR)\alarm_box.obj"
	-@erase "$(INTDIR)\alarm_encap.obj"
	-@erase "$(INTDIR)\alarm_handler.obj"
	-@erase "$(INTDIR)\alarm_mml_report.obj"
	-@erase "$(INTDIR)\alarm_server.obj"
	-@erase "$(INTDIR)\bin_alarm_handler.obj"
	-@erase "$(INTDIR)\box_comm.obj"
	-@erase "$(INTDIR)\hd_monitor_proxy.obj"
	-@erase "$(INTDIR)\history_alarm.obj"
	-@erase "$(INTDIR)\mml_alarm_handler.obj"
	-@erase "$(INTDIR)\snmp_agent_obj.obj"
	-@erase "$(INTDIR)\v2_hd_monitor_proxy.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\alarm.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_NO_SNMPv3" /Fp"$(INTDIR)\alarm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\alarm.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\alarm.lib" 
LIB32_OBJS= \
	"$(INTDIR)\active_alarm_list.obj" \
	"$(INTDIR)\alarm_bin_util.obj" \
	"$(INTDIR)\alarm_box.obj" \
	"$(INTDIR)\alarm_encap.obj" \
	"$(INTDIR)\alarm_handler.obj" \
	"$(INTDIR)\alarm_mml_report.obj" \
	"$(INTDIR)\alarm_server.obj" \
	"$(INTDIR)\bin_alarm_handler.obj" \
	"$(INTDIR)\box_comm.obj" \
	"$(INTDIR)\history_alarm.obj" \
	"$(INTDIR)\mml_alarm_handler.obj" \
	"$(INTDIR)\hd_monitor_proxy.obj" \
	"$(INTDIR)\v2_hd_monitor_proxy.obj" \
	"$(INTDIR)\snmp_agent_obj.obj"

"..\lib\Release\alarm.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "alarm - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\Debug\alarm.lib"


CLEAN :
	-@erase "$(INTDIR)\active_alarm_list.obj"
	-@erase "$(INTDIR)\alarm_bin_util.obj"
	-@erase "$(INTDIR)\alarm_box.obj"
	-@erase "$(INTDIR)\alarm_encap.obj"
	-@erase "$(INTDIR)\alarm_handler.obj"
	-@erase "$(INTDIR)\alarm_mml_report.obj"
	-@erase "$(INTDIR)\alarm_server.obj"
	-@erase "$(INTDIR)\bin_alarm_handler.obj"
	-@erase "$(INTDIR)\box_comm.obj"
	-@erase "$(INTDIR)\hd_monitor_proxy.obj"
	-@erase "$(INTDIR)\history_alarm.obj"
	-@erase "$(INTDIR)\mml_alarm_handler.obj"
	-@erase "$(INTDIR)\snmp_agent_obj.obj"
	-@erase "$(INTDIR)\v2_hd_monitor_proxy.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\Debug\alarm.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_NO_SNMPv3" /Fp"$(INTDIR)\alarm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\alarm.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Debug/alarm.lib" 
LIB32_OBJS= \
	"$(INTDIR)\active_alarm_list.obj" \
	"$(INTDIR)\alarm_bin_util.obj" \
	"$(INTDIR)\alarm_box.obj" \
	"$(INTDIR)\alarm_encap.obj" \
	"$(INTDIR)\alarm_handler.obj" \
	"$(INTDIR)\alarm_mml_report.obj" \
	"$(INTDIR)\alarm_server.obj" \
	"$(INTDIR)\bin_alarm_handler.obj" \
	"$(INTDIR)\box_comm.obj" \
	"$(INTDIR)\history_alarm.obj" \
	"$(INTDIR)\mml_alarm_handler.obj" \
	"$(INTDIR)\hd_monitor_proxy.obj" \
	"$(INTDIR)\v2_hd_monitor_proxy.obj" \
	"$(INTDIR)\snmp_agent_obj.obj"

"..\lib\Debug\alarm.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("alarm.dep")
!INCLUDE "alarm.dep"
!ELSE 
!MESSAGE Warning: cannot find "alarm.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "alarm - Win32 Release" || "$(CFG)" == "alarm - Win32 Debug"
SOURCE=.\active_alarm_list.cpp

"$(INTDIR)\active_alarm_list.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alarm_bin_util.cpp

"$(INTDIR)\alarm_bin_util.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alarm_box.cpp

"$(INTDIR)\alarm_box.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alarm_encap.cpp

"$(INTDIR)\alarm_encap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alarm_handler.cpp

"$(INTDIR)\alarm_handler.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alarm_mml_report.cpp

"$(INTDIR)\alarm_mml_report.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alarm_server.cpp

"$(INTDIR)\alarm_server.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bin_alarm_handler.cpp

"$(INTDIR)\bin_alarm_handler.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\box_comm.cpp

"$(INTDIR)\box_comm.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\history_alarm.cpp

"$(INTDIR)\history_alarm.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_alarm_handler.cpp

"$(INTDIR)\mml_alarm_handler.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hd_monitor\hd_monitor_proxy.cpp

"$(INTDIR)\hd_monitor_proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\hd_monitor\v2_hd_monitor_proxy.cpp

"$(INTDIR)\v2_hd_monitor_proxy.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\snmp_agent\snmp_agent_obj.cpp

"$(INTDIR)\snmp_agent_obj.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

