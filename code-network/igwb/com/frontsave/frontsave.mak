# Microsoft Developer Studio Generated NMAKE File, Based on frontsave.dsp
!IF "$(CFG)" == ""
CFG=frontsave - Win32 Debug
!MESSAGE No configuration specified. Defaulting to frontsave - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "frontsave - Win32 Release" && "$(CFG)" != "frontsave - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "frontsave.mak" CFG="frontsave - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "frontsave - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "frontsave - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "frontsave - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\frontsave.lib"


CLEAN :
	-@erase "$(INTDIR)\front_save.obj"
	-@erase "$(INTDIR)\front_stor.obj"
	-@erase "$(INTDIR)\info_file.obj"
	-@erase "$(INTDIR)\save.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\frontsave.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\frontsave.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\frontsave.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\frontsave.lib" 
LIB32_OBJS= \
	"$(INTDIR)\front_save.obj" \
	"$(INTDIR)\front_stor.obj" \
	"$(INTDIR)\info_file.obj" \
	"$(INTDIR)\save.obj"

"..\lib\Release\frontsave.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "frontsave - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\frontsave.lib"


CLEAN :
	-@erase "$(INTDIR)\front_save.obj"
	-@erase "$(INTDIR)\front_stor.obj"
	-@erase "$(INTDIR)\info_file.obj"
	-@erase "$(INTDIR)\save.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\frontsave.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\frontsave.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\frontsave.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/debug/frontsave.lib" 
LIB32_OBJS= \
	"$(INTDIR)\front_save.obj" \
	"$(INTDIR)\front_stor.obj" \
	"$(INTDIR)\info_file.obj" \
	"$(INTDIR)\save.obj"

"..\lib\debug\frontsave.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("frontsave.dep")
!INCLUDE "frontsave.dep"
!ELSE 
!MESSAGE Warning: cannot find "frontsave.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "frontsave - Win32 Release" || "$(CFG)" == "frontsave - Win32 Debug"
SOURCE=.\front_save.cpp

"$(INTDIR)\front_save.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\front_stor.cpp

"$(INTDIR)\front_stor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\info_file.cpp

"$(INTDIR)\info_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\save.cpp

"$(INTDIR)\save.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

