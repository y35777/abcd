# Microsoft Developer Studio Generated NMAKE File, Based on frame.dsp
!IF "$(CFG)" == ""
CFG=frame - Win32 Debug
!MESSAGE No configuration specified. Defaulting to frame - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "frame - Win32 Release" && "$(CFG)" != "frame - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "frame.mak" CFG="frame - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "frame - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "frame - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "frame - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\frame.lib"


CLEAN :
	-@erase "$(INTDIR)\msg_proc.obj"
	-@erase "$(INTDIR)\msg_router.obj"
	-@erase "$(INTDIR)\pck_cache.obj"
	-@erase "$(INTDIR)\tm_manager.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wrk_manager.obj"
	-@erase "$(INTDIR)\wrk_module.obj"
	-@erase "..\lib\Release\frame.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\frame.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\frame.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\frame.lib" 
LIB32_OBJS= \
	"$(INTDIR)\msg_proc.obj" \
	"$(INTDIR)\msg_router.obj" \
	"$(INTDIR)\pck_cache.obj" \
	"$(INTDIR)\tm_manager.obj" \
	"$(INTDIR)\wrk_manager.obj" \
	"$(INTDIR)\wrk_module.obj"

"..\lib\Release\frame.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "frame - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\frame.lib"


CLEAN :
	-@erase "$(INTDIR)\msg_proc.obj"
	-@erase "$(INTDIR)\msg_router.obj"
	-@erase "$(INTDIR)\pck_cache.obj"
	-@erase "$(INTDIR)\tm_manager.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\wrk_manager.obj"
	-@erase "$(INTDIR)\wrk_module.obj"
	-@erase "..\lib\debug\frame.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\frame.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\frame.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\frame.lib" 
LIB32_OBJS= \
	"$(INTDIR)\msg_proc.obj" \
	"$(INTDIR)\msg_router.obj" \
	"$(INTDIR)\pck_cache.obj" \
	"$(INTDIR)\tm_manager.obj" \
	"$(INTDIR)\wrk_manager.obj" \
	"$(INTDIR)\wrk_module.obj"

"..\lib\debug\frame.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("frame.dep")
!INCLUDE "frame.dep"
!ELSE 
!MESSAGE Warning: cannot find "frame.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "frame - Win32 Release" || "$(CFG)" == "frame - Win32 Debug"
SOURCE=.\msg_proc.cpp

"$(INTDIR)\msg_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\msg_router.cpp

"$(INTDIR)\msg_router.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pck_cache.cpp

"$(INTDIR)\pck_cache.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tm_manager.cpp

"$(INTDIR)\tm_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\wrk_manager.cpp

"$(INTDIR)\wrk_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\wrk_module.cpp

"$(INTDIR)\wrk_module.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

