# Microsoft Developer Studio Generated NMAKE File, Based on log.dsp
!IF "$(CFG)" == ""
CFG=log - Win32 Debug
!MESSAGE No configuration specified. Defaulting to log - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "log - Win32 Release" && "$(CFG)" != "log - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "log.mak" CFG="log - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "log - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "log - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "log - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\log.lib"


CLEAN :
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log_bin_util.obj"
	-@erase "$(INTDIR)\log_encap.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\log.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\log.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\log.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\log.lib" 
LIB32_OBJS= \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\log_encap.obj" \
	"$(INTDIR)\log_bin_util.obj"

"..\lib\Release\log.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "log - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\lib\debug\log.lib" "$(OUTDIR)\log.bsc"


CLEAN :
	-@erase "$(INTDIR)\log.obj"
	-@erase "$(INTDIR)\log.sbr"
	-@erase "$(INTDIR)\log_bin_util.obj"
	-@erase "$(INTDIR)\log_bin_util.sbr"
	-@erase "$(INTDIR)\log_encap.obj"
	-@erase "$(INTDIR)\log_encap.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\log.bsc"
	-@erase "..\lib\debug\log.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\log.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\log.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\log.sbr" \
	"$(INTDIR)\log_encap.sbr" \
	"$(INTDIR)\log_bin_util.sbr"

"$(OUTDIR)\log.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\log.lib" 
LIB32_OBJS= \
	"$(INTDIR)\log.obj" \
	"$(INTDIR)\log_encap.obj" \
	"$(INTDIR)\log_bin_util.obj"

"..\lib\debug\log.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("log.dep")
!INCLUDE "log.dep"
!ELSE 
!MESSAGE Warning: cannot find "log.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "log - Win32 Release" || "$(CFG)" == "log - Win32 Debug"
SOURCE=.\log.cpp

!IF  "$(CFG)" == "log - Win32 Release"


"$(INTDIR)\log.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "log - Win32 Debug"


"$(INTDIR)\log.obj"	"$(INTDIR)\log.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\log_bin_util.cpp

!IF  "$(CFG)" == "log - Win32 Release"


"$(INTDIR)\log_bin_util.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "log - Win32 Debug"


"$(INTDIR)\log_bin_util.obj"	"$(INTDIR)\log_bin_util.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\log_encap.cpp

!IF  "$(CFG)" == "log - Win32 Release"


"$(INTDIR)\log_encap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "log - Win32 Debug"


"$(INTDIR)\log_encap.obj"	"$(INTDIR)\log_encap.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

