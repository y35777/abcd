# Microsoft Developer Studio Generated NMAKE File, Based on KillCluster.dsp
!IF "$(CFG)" == ""
CFG=KillCluster - Win32 Debug
!MESSAGE No configuration specified. Defaulting to KillCluster - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "KillCluster - Win32 Release" && "$(CFG)" != "KillCluster - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KillCluster.mak" CFG="KillCluster - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KillCluster - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KillCluster - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "KillCluster - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\KillCluster.exe"


CLEAN :
	-@erase "$(INTDIR)\KillCluster.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\KillCluster.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\KillCluster.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KillCluster.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\KillCluster.pdb" /machine:I386 /out:"$(OUTDIR)\KillCluster.exe" 
LINK32_OBJS= \
	"$(INTDIR)\KillCluster.obj"

"$(OUTDIR)\KillCluster.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "KillCluster - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\KillCluster.exe"


CLEAN :
	-@erase "$(INTDIR)\KillCluster.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\KillCluster.exe"
	-@erase "$(OUTDIR)\KillCluster.ilk"
	-@erase "$(OUTDIR)\KillCluster.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\KillCluster.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\KillCluster.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\KillCluster.pdb" /debug /machine:I386 /out:"$(OUTDIR)\KillCluster.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\KillCluster.obj"

"$(OUTDIR)\KillCluster.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("KillCluster.dep")
!INCLUDE "KillCluster.dep"
!ELSE 
!MESSAGE Warning: cannot find "KillCluster.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "KillCluster - Win32 Release" || "$(CFG)" == "KillCluster - Win32 Debug"
SOURCE=.\KillCluster.cpp

"$(INTDIR)\KillCluster.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

