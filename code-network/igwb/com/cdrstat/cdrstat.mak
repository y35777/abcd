# Microsoft Developer Studio Generated NMAKE File, Based on cdrstat.dsp
!IF "$(CFG)" == ""
CFG=cdrstat - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cdrstat - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cdrstat - Win32 Release" && "$(CFG)" != "cdrstat - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cdrstat.mak" CFG="cdrstat - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cdrstat - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cdrstat - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "cdrstat - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\cdrstat.lib"


CLEAN :
	-@erase "$(INTDIR)\cdr_stat_cfg.obj"
	-@erase "$(INTDIR)\cdr_stat_file.obj"
	-@erase "$(INTDIR)\cdr_stat_rst.obj"
	-@erase "$(INTDIR)\cdr_stat_svr.obj"
	-@erase "$(INTDIR)\cdr_stat_tsk.obj"
	-@erase "$(INTDIR)\stat_cfg_encap.obj"
	-@erase "$(INTDIR)\stat_cfg_mgr.obj"
	-@erase "$(INTDIR)\stat_file_group.obj"
	-@erase "$(INTDIR)\stat_rst_encap.obj"
	-@erase "$(INTDIR)\stat_rst_mgr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\cdrstat.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\cdrstat.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdrstat.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release/cdrstat.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cdr_stat_cfg.obj" \
	"$(INTDIR)\cdr_stat_file.obj" \
	"$(INTDIR)\cdr_stat_rst.obj" \
	"$(INTDIR)\cdr_stat_svr.obj" \
	"$(INTDIR)\cdr_stat_tsk.obj" \
	"$(INTDIR)\stat_cfg_mgr.obj" \
	"$(INTDIR)\stat_file_group.obj" \
	"$(INTDIR)\stat_rst_mgr.obj" \
	"$(INTDIR)\stat_cfg_encap.obj" \
	"$(INTDIR)\stat_rst_encap.obj"

"..\lib\Release\cdrstat.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cdrstat - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\cdrstat.lib"


CLEAN :
	-@erase "$(INTDIR)\cdr_stat_cfg.obj"
	-@erase "$(INTDIR)\cdr_stat_file.obj"
	-@erase "$(INTDIR)\cdr_stat_rst.obj"
	-@erase "$(INTDIR)\cdr_stat_svr.obj"
	-@erase "$(INTDIR)\cdr_stat_tsk.obj"
	-@erase "$(INTDIR)\stat_cfg_encap.obj"
	-@erase "$(INTDIR)\stat_cfg_mgr.obj"
	-@erase "$(INTDIR)\stat_file_group.obj"
	-@erase "$(INTDIR)\stat_rst_encap.obj"
	-@erase "$(INTDIR)\stat_rst_mgr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\cdrstat.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\cdrstat.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdrstat.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/debug/cdrstat.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cdr_stat_cfg.obj" \
	"$(INTDIR)\cdr_stat_file.obj" \
	"$(INTDIR)\cdr_stat_rst.obj" \
	"$(INTDIR)\cdr_stat_svr.obj" \
	"$(INTDIR)\cdr_stat_tsk.obj" \
	"$(INTDIR)\stat_cfg_mgr.obj" \
	"$(INTDIR)\stat_file_group.obj" \
	"$(INTDIR)\stat_rst_mgr.obj" \
	"$(INTDIR)\stat_cfg_encap.obj" \
	"$(INTDIR)\stat_rst_encap.obj"

"..\lib\debug\cdrstat.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("cdrstat.dep")
!INCLUDE "cdrstat.dep"
!ELSE 
!MESSAGE Warning: cannot find "cdrstat.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cdrstat - Win32 Release" || "$(CFG)" == "cdrstat - Win32 Debug"
SOURCE=.\cdr_stat_cfg.cpp

"$(INTDIR)\cdr_stat_cfg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cdr_stat_file.cpp

"$(INTDIR)\cdr_stat_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cdr_stat_rst.cpp

"$(INTDIR)\cdr_stat_rst.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cdr_stat_svr.cpp

"$(INTDIR)\cdr_stat_svr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cdr_stat_tsk.cpp

"$(INTDIR)\cdr_stat_tsk.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stat_cfg_encap.cpp

"$(INTDIR)\stat_cfg_encap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stat_cfg_mgr.cpp

"$(INTDIR)\stat_cfg_mgr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stat_file_group.cpp

"$(INTDIR)\stat_file_group.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stat_rst_encap.cpp

"$(INTDIR)\stat_rst_encap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stat_rst_mgr.cpp

"$(INTDIR)\stat_rst_mgr.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

