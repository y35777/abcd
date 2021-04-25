# Microsoft Developer Studio Generated NMAKE File, Based on toolbox.dsp
!IF "$(CFG)" == ""
CFG=toolbox - Win32 Debug
!MESSAGE No configuration specified. Defaulting to toolbox - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "toolbox - Win32 Release" && "$(CFG)" != "toolbox - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "toolbox - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\toolbox.lib"


CLEAN :
	-@erase "$(INTDIR)\5ess_file.obj"
	-@erase "$(INTDIR)\cc08_file.obj"
	-@erase "$(INTDIR)\encrypt.obj"
	-@erase "$(INTDIR)\ewsd_file.obj"
	-@erase "$(INTDIR)\frame_adapter.obj"
	-@erase "$(INTDIR)\igwb_file.obj"
	-@erase "$(INTDIR)\igwb_file_creator.obj"
	-@erase "$(INTDIR)\mml_para.obj"
	-@erase "$(INTDIR)\mml_report.obj"
	-@erase "$(INTDIR)\perf_item.obj"
	-@erase "$(INTDIR)\ps_file.obj"
	-@erase "$(INTDIR)\qb_report.obj"
	-@erase "$(INTDIR)\s1240_file.obj"
	-@erase "$(INTDIR)\signal_tab.obj"
	-@erase "$(INTDIR)\system1.obj"
	-@erase "$(INTDIR)\text_file.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\zxj_file.obj"
	-@erase "..\lib\Release\toolbox.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\toolbox.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\toolbox.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\toolbox.lib" 
LIB32_OBJS= \
	"$(INTDIR)\5ess_file.obj" \
	"$(INTDIR)\cc08_file.obj" \
	"$(INTDIR)\encrypt.obj" \
	"$(INTDIR)\ewsd_file.obj" \
	"$(INTDIR)\frame_adapter.obj" \
	"$(INTDIR)\igwb_file.obj" \
	"$(INTDIR)\igwb_file_creator.obj" \
	"$(INTDIR)\mml_para.obj" \
	"$(INTDIR)\mml_report.obj" \
	"$(INTDIR)\perf_item.obj" \
	"$(INTDIR)\ps_file.obj" \
	"$(INTDIR)\qb_report.obj" \
	"$(INTDIR)\s1240_file.obj" \
	"$(INTDIR)\signal_tab.obj" \
	"$(INTDIR)\system1.obj" \
	"$(INTDIR)\text_file.obj" \
	"$(INTDIR)\zxj_file.obj"

"..\lib\Release\toolbox.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "toolbox - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\toolbox.lib"


CLEAN :
	-@erase "$(INTDIR)\5ess_file.obj"
	-@erase "$(INTDIR)\cc08_file.obj"
	-@erase "$(INTDIR)\encrypt.obj"
	-@erase "$(INTDIR)\ewsd_file.obj"
	-@erase "$(INTDIR)\frame_adapter.obj"
	-@erase "$(INTDIR)\igwb_file.obj"
	-@erase "$(INTDIR)\igwb_file_creator.obj"
	-@erase "$(INTDIR)\mml_para.obj"
	-@erase "$(INTDIR)\mml_report.obj"
	-@erase "$(INTDIR)\perf_item.obj"
	-@erase "$(INTDIR)\ps_file.obj"
	-@erase "$(INTDIR)\qb_report.obj"
	-@erase "$(INTDIR)\s1240_file.obj"
	-@erase "$(INTDIR)\signal_tab.obj"
	-@erase "$(INTDIR)\system1.obj"
	-@erase "$(INTDIR)\text_file.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\zxj_file.obj"
	-@erase "..\lib\debug\toolbox.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\toolbox.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\toolbox.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\toolbox.lib" 
LIB32_OBJS= \
	"$(INTDIR)\5ess_file.obj" \
	"$(INTDIR)\cc08_file.obj" \
	"$(INTDIR)\encrypt.obj" \
	"$(INTDIR)\ewsd_file.obj" \
	"$(INTDIR)\frame_adapter.obj" \
	"$(INTDIR)\igwb_file.obj" \
	"$(INTDIR)\igwb_file_creator.obj" \
	"$(INTDIR)\mml_para.obj" \
	"$(INTDIR)\mml_report.obj" \
	"$(INTDIR)\perf_item.obj" \
	"$(INTDIR)\ps_file.obj" \
	"$(INTDIR)\qb_report.obj" \
	"$(INTDIR)\s1240_file.obj" \
	"$(INTDIR)\signal_tab.obj" \
	"$(INTDIR)\system1.obj" \
	"$(INTDIR)\text_file.obj" \
	"$(INTDIR)\zxj_file.obj"

"..\lib\debug\toolbox.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("toolbox.dep")
!INCLUDE "toolbox.dep"
!ELSE 
!MESSAGE Warning: cannot find "toolbox.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "toolbox - Win32 Release" || "$(CFG)" == "toolbox - Win32 Debug"
SOURCE=.\5ess_file.cpp

"$(INTDIR)\5ess_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cc08_file.cpp

"$(INTDIR)\cc08_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\encrypt.cpp

"$(INTDIR)\encrypt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ewsd_file.cpp

"$(INTDIR)\ewsd_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\frame_adapter.cpp

"$(INTDIR)\frame_adapter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\igwb_file.cpp

"$(INTDIR)\igwb_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\igwb_file_creator.cpp

"$(INTDIR)\igwb_file_creator.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_para.cpp

"$(INTDIR)\mml_para.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_report.cpp

"$(INTDIR)\mml_report.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perf_item.cpp

"$(INTDIR)\perf_item.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ps_file.cpp

"$(INTDIR)\ps_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\qb_report.cpp

"$(INTDIR)\qb_report.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\s1240_file.cpp

"$(INTDIR)\s1240_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\signal_tab.cpp

"$(INTDIR)\signal_tab.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\system1.cpp

"$(INTDIR)\system1.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\text_file.cpp

"$(INTDIR)\text_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\zxj_file.cpp

"$(INTDIR)\zxj_file.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

