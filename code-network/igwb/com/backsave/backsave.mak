# Microsoft Developer Studio Generated NMAKE File, Based on backsave.dsp
!IF "$(CFG)" == ""
CFG=backsave - Win32 Debug
!MESSAGE No configuration specified. Defaulting to backsave - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "backsave - Win32 Release" && "$(CFG)" != "backsave - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "backsave.mak" CFG="backsave - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "backsave - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "backsave - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "backsave - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\backsave.lib"


CLEAN :
	-@erase "$(INTDIR)\back_stor.obj"
	-@erase "$(INTDIR)\hot_save_chanl.obj"
	-@erase "$(INTDIR)\patch_encap.obj"
	-@erase "$(INTDIR)\save_chanl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\backsave.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\backsave.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\backsave.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release/backsave.lib" 
LIB32_OBJS= \
	"$(INTDIR)\back_stor.obj" \
	"$(INTDIR)\hot_save_chanl.obj" \
	"$(INTDIR)\patch_encap.obj" \
	"$(INTDIR)\save_chanl.obj"

"..\lib\Release\backsave.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "backsave - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\Debug\backsave.lib"


CLEAN :
	-@erase "$(INTDIR)\back_stor.obj"
	-@erase "$(INTDIR)\hot_save_chanl.obj"
	-@erase "$(INTDIR)\patch_encap.obj"
	-@erase "$(INTDIR)\save_chanl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\Debug\backsave.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\backsave.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\backsave.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Debug/backsave.lib" 
LIB32_OBJS= \
	"$(INTDIR)\back_stor.obj" \
	"$(INTDIR)\hot_save_chanl.obj" \
	"$(INTDIR)\patch_encap.obj" \
	"$(INTDIR)\save_chanl.obj"

"..\lib\Debug\backsave.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("backsave.dep")
!INCLUDE "backsave.dep"
!ELSE 
!MESSAGE Warning: cannot find "backsave.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "backsave - Win32 Release" || "$(CFG)" == "backsave - Win32 Debug"
SOURCE=.\back_stor.cpp

"$(INTDIR)\back_stor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hot_save_chanl.cpp

"$(INTDIR)\hot_save_chanl.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\patch_encap.cpp

"$(INTDIR)\patch_encap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\save_chanl.cpp

"$(INTDIR)\save_chanl.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

