# Microsoft Developer Studio Generated NMAKE File, Based on alarm_util.dsp
!IF "$(CFG)" == ""
CFG=alarm_util - Win32 Debug
!MESSAGE No configuration specified. Defaulting to alarm_util - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "alarm_util - Win32 Release" && "$(CFG)" != "alarm_util - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "alarm_util.mak" CFG="alarm_util - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "alarm_util - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "alarm_util - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "alarm_util - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\alarm_util.dll"


CLEAN :
	-@erase "$(INTDIR)\alarm_config.obj"
	-@erase "$(INTDIR)\alarm_struct.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\alarm_util.dll"
	-@erase "$(OUTDIR)\alarm_util.exp"
	-@erase "$(OUTDIR)\alarm_util.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "alarm_util_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\alarm_util.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\alarm_util.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib toolbase.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\alarm_util.pdb" /machine:I386 /out:"$(OUTDIR)\alarm_util.dll" /implib:"$(OUTDIR)\alarm_util.lib" 
LINK32_OBJS= \
	"$(INTDIR)\alarm_config.obj" \
	"$(INTDIR)\alarm_struct.obj"

"$(OUTDIR)\alarm_util.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "alarm_util - Win32 Debug"

OUTDIR=.\../lib/debug
INTDIR=.\debug
# Begin Custom Macros
OutDir=.\../lib/debug
# End Custom Macros

ALL : "$(OUTDIR)\alarm_util.dll"


CLEAN :
	-@erase "$(INTDIR)\alarm_config.obj"
	-@erase "$(INTDIR)\alarm_struct.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\alarm_util.dll"
	-@erase "$(OUTDIR)\alarm_util.exp"
	-@erase "$(OUTDIR)\alarm_util.ilk"
	-@erase "$(OUTDIR)\alarm_util.lib"
	-@erase "$(OUTDIR)\alarm_util.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "alarm_util_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\alarm_util.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\alarm_util.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib toolbase.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\alarm_util.pdb" /debug /machine:I386 /out:"$(OUTDIR)\alarm_util.dll" /implib:"$(OUTDIR)\alarm_util.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\alarm_config.obj" \
	"$(INTDIR)\alarm_struct.obj"

"$(OUTDIR)\alarm_util.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("alarm_util.dep")
!INCLUDE "alarm_util.dep"
!ELSE 
!MESSAGE Warning: cannot find "alarm_util.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "alarm_util - Win32 Release" || "$(CFG)" == "alarm_util - Win32 Debug"
SOURCE=.\alarm_config.cpp

"$(INTDIR)\alarm_config.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alarm_struct.cpp

"$(INTDIR)\alarm_struct.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

