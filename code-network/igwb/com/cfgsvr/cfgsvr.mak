# Microsoft Developer Studio Generated NMAKE File, Based on cfgsvr.dsp
!IF "$(CFG)" == ""
CFG=cfgsvr - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cfgsvr - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cfgsvr - Win32 Release" && "$(CFG)" != "cfgsvr - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cfgsvr.mak" CFG="cfgsvr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cfgsvr - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cfgsvr - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "cfgsvr - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\cfgsvr.lib"


CLEAN :
	-@erase "$(INTDIR)\cfg_file.obj"
	-@erase "$(INTDIR)\cfg_svr.obj"
	-@erase "$(INTDIR)\file_grp.obj"
	-@erase "$(INTDIR)\file_mgr.obj"
	-@erase "$(INTDIR)\para_file.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\cfgsvr.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\cfgsvr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cfgsvr.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\cfgsvr.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cfg_file.obj" \
	"$(INTDIR)\cfg_svr.obj" \
	"$(INTDIR)\file_grp.obj" \
	"$(INTDIR)\file_mgr.obj" \
	"$(INTDIR)\para_file.obj"

"..\lib\Release\cfgsvr.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cfgsvr - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\Debug\cfgsvr.lib"


CLEAN :
	-@erase "$(INTDIR)\cfg_file.obj"
	-@erase "$(INTDIR)\cfg_svr.obj"
	-@erase "$(INTDIR)\file_grp.obj"
	-@erase "$(INTDIR)\file_mgr.obj"
	-@erase "$(INTDIR)\para_file.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\Debug\cfgsvr.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /I "E:\ACE_Wrappers" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\cfgsvr.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cfgsvr.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Debug\cfgsvr.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cfg_file.obj" \
	"$(INTDIR)\cfg_svr.obj" \
	"$(INTDIR)\file_grp.obj" \
	"$(INTDIR)\file_mgr.obj" \
	"$(INTDIR)\para_file.obj"

"..\lib\Debug\cfgsvr.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("cfgsvr.dep")
!INCLUDE "cfgsvr.dep"
!ELSE 
!MESSAGE Warning: cannot find "cfgsvr.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cfgsvr - Win32 Release" || "$(CFG)" == "cfgsvr - Win32 Debug"
SOURCE=.\cfg_file.cpp

"$(INTDIR)\cfg_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cfg_svr.cpp

"$(INTDIR)\cfg_svr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\file_grp.cpp

"$(INTDIR)\file_grp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\file_mgr.cpp

"$(INTDIR)\file_mgr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\para_file.cpp

"$(INTDIR)\para_file.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

