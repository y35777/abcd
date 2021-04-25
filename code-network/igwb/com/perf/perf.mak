# Microsoft Developer Studio Generated NMAKE File, Based on perf.dsp
!IF "$(CFG)" == ""
CFG=perf - Win32 Debug
!MESSAGE No configuration specified. Defaulting to perf - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "perf - Win32 Release" && "$(CFG)" != "perf - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "perf - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\perf.lib"


CLEAN :
	-@erase "$(INTDIR)\collect_statistic.obj"
	-@erase "$(INTDIR)\dda_proxy.obj"
	-@erase "$(INTDIR)\ddaquery.obj"
	-@erase "$(INTDIR)\ddaset.obj"
	-@erase "$(INTDIR)\nt_perf_db.obj"
	-@erase "$(INTDIR)\PdhUtility.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf_bin_util.obj"
	-@erase "$(INTDIR)\perf_encap.obj"
	-@erase "$(INTDIR)\perf_task.obj"
	-@erase "$(INTDIR)\perf_task_global.obj"
	-@erase "$(INTDIR)\query_executive.obj"
	-@erase "$(INTDIR)\task_creator.obj"
	-@erase "$(INTDIR)\task_manager.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\perf.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\perf.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\perf.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\perf.lib" 
LIB32_OBJS= \
	"$(INTDIR)\collect_statistic.obj" \
	"$(INTDIR)\nt_perf_db.obj" \
	"$(INTDIR)\PdhUtility.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\perf_bin_util.obj" \
	"$(INTDIR)\perf_encap.obj" \
	"$(INTDIR)\dda_proxy.obj" \
	"$(INTDIR)\ddaquery.obj" \
	"$(INTDIR)\ddaset.obj" \
	"$(INTDIR)\perf_task.obj" \
	"$(INTDIR)\perf_task_global.obj" \
	"$(INTDIR)\query_executive.obj" \
	"$(INTDIR)\task_creator.obj" \
	"$(INTDIR)\task_manager.obj"

"..\lib\Release\perf.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "perf - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\perf.lib"


CLEAN :
	-@erase "$(INTDIR)\collect_statistic.obj"
	-@erase "$(INTDIR)\dda_proxy.obj"
	-@erase "$(INTDIR)\ddaquery.obj"
	-@erase "$(INTDIR)\ddaset.obj"
	-@erase "$(INTDIR)\nt_perf_db.obj"
	-@erase "$(INTDIR)\PdhUtility.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\perf_bin_util.obj"
	-@erase "$(INTDIR)\perf_encap.obj"
	-@erase "$(INTDIR)\perf_task.obj"
	-@erase "$(INTDIR)\perf_task_global.obj"
	-@erase "$(INTDIR)\query_executive.obj"
	-@erase "$(INTDIR)\task_creator.obj"
	-@erase "$(INTDIR)\task_manager.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\perf.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\perf.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\perf.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\perf.lib" 
LIB32_OBJS= \
	"$(INTDIR)\collect_statistic.obj" \
	"$(INTDIR)\nt_perf_db.obj" \
	"$(INTDIR)\PdhUtility.obj" \
	"$(INTDIR)\perf.obj" \
	"$(INTDIR)\perf_bin_util.obj" \
	"$(INTDIR)\perf_encap.obj" \
	"$(INTDIR)\dda_proxy.obj" \
	"$(INTDIR)\ddaquery.obj" \
	"$(INTDIR)\ddaset.obj" \
	"$(INTDIR)\perf_task.obj" \
	"$(INTDIR)\perf_task_global.obj" \
	"$(INTDIR)\query_executive.obj" \
	"$(INTDIR)\task_creator.obj" \
	"$(INTDIR)\task_manager.obj"

"..\lib\debug\perf.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("perf.dep")
!INCLUDE "perf.dep"
!ELSE 
!MESSAGE Warning: cannot find "perf.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "perf - Win32 Release" || "$(CFG)" == "perf - Win32 Debug"
SOURCE=.\collect_statistic.cpp

"$(INTDIR)\collect_statistic.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dda_proxy.cpp

"$(INTDIR)\dda_proxy.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ddaquery.cpp

"$(INTDIR)\ddaquery.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ddaset.cpp

"$(INTDIR)\ddaset.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perf_nt\nt_perf_db.cpp

"$(INTDIR)\nt_perf_db.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\perf_nt\PdhUtility.cpp

"$(INTDIR)\PdhUtility.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\perf.cpp

"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perf_bin_util.cpp

"$(INTDIR)\perf_bin_util.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perf_encap.cpp

"$(INTDIR)\perf_encap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perf_task.cpp

"$(INTDIR)\perf_task.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perf_task_global.cpp

"$(INTDIR)\perf_task_global.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\query_executive.cpp

"$(INTDIR)\query_executive.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\task_creator.cpp

"$(INTDIR)\task_creator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\task_manager.cpp

"$(INTDIR)\task_manager.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

