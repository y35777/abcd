# Microsoft Developer Studio Generated NMAKE File, Based on merge.dsp
!IF "$(CFG)" == ""
CFG=merge - Win32 Debug
!MESSAGE No configuration specified. Defaulting to merge - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "merge - Win32 Release" && "$(CFG)" != "merge - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "merge.mak" CFG="merge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "merge - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "merge - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "merge - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\merge.dll"


CLEAN :
	-@erase "$(INTDIR)\bill.obj"
	-@erase "$(INTDIR)\merge.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\merge.dll"
	-@erase "$(OUTDIR)\merge.exp"
	-@erase "$(OUTDIR)\merge.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MERGE_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\merge.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\merge.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib toolbase.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\merge.pdb" /machine:I386 /out:"$(OUTDIR)\merge.dll" /implib:"$(OUTDIR)\merge.lib" 
LINK32_OBJS= \
	"$(INTDIR)\bill.obj" \
	"$(INTDIR)\merge.obj"

"$(OUTDIR)\merge.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "merge - Win32 Debug"

OUTDIR=.\../lib/Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../lib/Debug
# End Custom Macros

ALL : "$(OUTDIR)\merge.dll"


CLEAN :
	-@erase "$(INTDIR)\bill.obj"
	-@erase "$(INTDIR)\merge.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\merge.dll"
	-@erase "$(OUTDIR)\merge.exp"
	-@erase "$(OUTDIR)\merge.ilk"
	-@erase "$(OUTDIR)\merge.lib"
	-@erase "$(OUTDIR)\merge.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MERGE_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\merge.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\merge.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib toolbase.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\merge.pdb" /debug /machine:I386 /out:"$(OUTDIR)\merge.dll" /implib:"$(OUTDIR)\merge.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\bill.obj" \
	"$(INTDIR)\merge.obj"

"$(OUTDIR)\merge.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("merge.dep")
!INCLUDE "merge.dep"
!ELSE 
!MESSAGE Warning: cannot find "merge.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "merge - Win32 Release" || "$(CFG)" == "merge - Win32 Debug"
SOURCE=.\bill.cpp

"$(INTDIR)\bill.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\merge.cpp

"$(INTDIR)\merge.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

