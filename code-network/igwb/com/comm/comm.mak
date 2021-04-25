# Microsoft Developer Studio Generated NMAKE File, Based on comm.dsp
!IF "$(CFG)" == ""
CFG=comm - Win32 Debug
!MESSAGE No configuration specified. Defaulting to comm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "comm - Win32 Release" && "$(CFG)" != "comm - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "comm.mak" CFG="comm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "comm - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "comm - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "comm - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\comm.lib"


CLEAN :
	-@erase "$(INTDIR)\comm_layer.obj"
	-@erase "$(INTDIR)\comm_router.obj"
	-@erase "$(INTDIR)\sockets.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\comm.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\comm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\comm.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\comm.lib" 
LIB32_OBJS= \
	"$(INTDIR)\comm_layer.obj" \
	"$(INTDIR)\comm_router.obj" \
	"$(INTDIR)\sockets.obj"

"..\lib\Release\comm.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "comm - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\comm.lib"


CLEAN :
	-@erase "$(INTDIR)\comm_layer.obj"
	-@erase "$(INTDIR)\comm_router.obj"
	-@erase "$(INTDIR)\sockets.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\comm.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\comm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\comm.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\comm.lib" 
LIB32_OBJS= \
	"$(INTDIR)\comm_layer.obj" \
	"$(INTDIR)\comm_router.obj" \
	"$(INTDIR)\sockets.obj"

"..\lib\debug\comm.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("comm.dep")
!INCLUDE "comm.dep"
!ELSE 
!MESSAGE Warning: cannot find "comm.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "comm - Win32 Release" || "$(CFG)" == "comm - Win32 Debug"
SOURCE=.\comm_layer.cpp

"$(INTDIR)\comm_layer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\comm_router.cpp

"$(INTDIR)\comm_router.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sockets.cpp

"$(INTDIR)\sockets.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

