# Microsoft Developer Studio Generated NMAKE File, Based on showicon.dsp
!IF "$(CFG)" == ""
CFG=showicon - Win32 Debug
!MESSAGE No configuration specified. Defaulting to showicon - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "showicon - Win32 Release" && "$(CFG)" != "showicon - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "showicon.mak" CFG="showicon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "showicon - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "showicon - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "showicon - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\showicon.exe"


CLEAN :
	-@erase "$(INTDIR)\showicon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\showicon.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\showicon.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\showicon.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib user32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\showicon.pdb" /machine:I386 /out:"$(OUTDIR)\showicon.exe" 
LINK32_OBJS= \
	"$(INTDIR)\showicon.obj"

"$(OUTDIR)\showicon.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "showicon - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\showicon.exe"


CLEAN :
	-@erase "$(INTDIR)\showicon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\showicon.exe"
	-@erase "$(OUTDIR)\showicon.ilk"
	-@erase "$(OUTDIR)\showicon.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\showicon.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\showicon.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib user32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\showicon.pdb" /debug /machine:I386 /out:"$(OUTDIR)\showicon.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\showicon.obj"

"$(OUTDIR)\showicon.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
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
!IF EXISTS("showicon.dep")
!INCLUDE "showicon.dep"
!ELSE 
!MESSAGE Warning: cannot find "showicon.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "showicon - Win32 Release" || "$(CFG)" == "showicon - Win32 Debug"
SOURCE=.\showicon.cpp

"$(INTDIR)\showicon.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

