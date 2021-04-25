# Microsoft Developer Studio Generated NMAKE File, Based on monitor.dsp
!IF "$(CFG)" == ""
CFG=monitor - Win32 Debug
!MESSAGE No configuration specified. Defaulting to monitor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "monitor - Win32 Release" && "$(CFG)" != "monitor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "monitor.mak" CFG="monitor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "monitor - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "monitor - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "monitor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\monitor.lib"


CLEAN :
	-@erase "$(INTDIR)\monitor.obj"
	-@erase "$(INTDIR)\proc_child.obj"
	-@erase "$(INTDIR)\proc_mgr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\monitor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\monitor.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\monitor.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\monitor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\monitor.obj" \
	"$(INTDIR)\proc_child.obj" \
	"$(INTDIR)\proc_mgr.obj"

"..\lib\Release\monitor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "monitor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\monitor.lib"


CLEAN :
	-@erase "$(INTDIR)\monitor.obj"
	-@erase "$(INTDIR)\proc_child.obj"
	-@erase "$(INTDIR)\proc_mgr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\monitor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\monitor.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\monitor.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\monitor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\monitor.obj" \
	"$(INTDIR)\proc_child.obj" \
	"$(INTDIR)\proc_mgr.obj"

"..\lib\debug\monitor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("monitor.dep")
!INCLUDE "monitor.dep"
!ELSE 
!MESSAGE Warning: cannot find "monitor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "monitor - Win32 Release" || "$(CFG)" == "monitor - Win32 Debug"
SOURCE=.\monitor.cpp

"$(INTDIR)\monitor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\proc_child.cpp

"$(INTDIR)\proc_child.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\proc_mgr.cpp

"$(INTDIR)\proc_mgr.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

