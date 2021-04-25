# Microsoft Developer Studio Generated NMAKE File, Based on ps_patch.dsp
!IF "$(CFG)" == ""
CFG=ps_patch - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ps_patch - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ps_patch - Win32 Release" && "$(CFG)" != "ps_patch - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ps_patch.mak" CFG="ps_patch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ps_patch - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ps_patch - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "ps_patch - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\ps_patch.dll"


CLEAN :
	-@erase "$(INTDIR)\ps_patch.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ps_patch.dll"
	-@erase "$(OUTDIR)\ps_patch.exp"
	-@erase "$(OUTDIR)\ps_patch.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ps_patch_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\ps_patch.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ps_patch.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib toolbase.lib user32.lib commode.obj /nologo /dll /incremental:no /pdb:"$(OUTDIR)\ps_patch.pdb" /machine:I386 /out:"$(OUTDIR)\ps_patch.dll" /implib:"$(OUTDIR)\ps_patch.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ps_patch.obj"

"$(OUTDIR)\ps_patch.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ps_patch - Win32 Debug"

OUTDIR=.\../lib/Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../lib/Debug
# End Custom Macros

ALL : "$(OUTDIR)\ps_patch.dll"


CLEAN :
	-@erase "$(INTDIR)\ps_patch.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ps_patch.dll"
	-@erase "$(OUTDIR)\ps_patch.exp"
	-@erase "$(OUTDIR)\ps_patch.ilk"
	-@erase "$(OUTDIR)\ps_patch.lib"
	-@erase "$(OUTDIR)\ps_patch.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ps_patch_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\ps_patch.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ps_patch.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib toolbase.lib user32.lib commode.obj /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\ps_patch.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ps_patch.dll" /implib:"$(OUTDIR)\ps_patch.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ps_patch.obj"

"$(OUTDIR)\ps_patch.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("ps_patch.dep")
!INCLUDE "ps_patch.dep"
!ELSE 
!MESSAGE Warning: cannot find "ps_patch.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ps_patch - Win32 Release" || "$(CFG)" == "ps_patch - Win32 Debug"
SOURCE=.\ps_patch.cpp

"$(INTDIR)\ps_patch.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

