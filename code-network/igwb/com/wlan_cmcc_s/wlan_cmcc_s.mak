# Microsoft Developer Studio Generated NMAKE File, Based on wlan_cmcc_s.dsp
!IF "$(CFG)" == ""
CFG=wlan_cmcc_s - Win32 Debug
!MESSAGE No configuration specified. Defaulting to wlan_cmcc_s - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "wlan_cmcc_s - Win32 Release" && "$(CFG)" != "wlan_cmcc_s - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wlan_cmcc_s.mak" CFG="wlan_cmcc_s - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wlan_cmcc_s - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wlan_cmcc_s - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "wlan_cmcc_s - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\wlan_cmcc_s.dll"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wlan_cmcc_s.obj"
	-@erase "$(OUTDIR)\wlan_cmcc_s.dll"
	-@erase "$(OUTDIR)\wlan_cmcc_s.exp"
	-@erase "$(OUTDIR)\wlan_cmcc_s.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "wlan_cmcc_s_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\wlan_cmcc_s.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wlan_cmcc_s.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib toolbase.lib user32.lib commode.obj /nologo /dll /incremental:no /pdb:"$(OUTDIR)\wlan_cmcc_s.pdb" /machine:I386 /out:"$(OUTDIR)\wlan_cmcc_s.dll" /implib:"$(OUTDIR)\wlan_cmcc_s.lib" 
LINK32_OBJS= \
	"$(INTDIR)\wlan_cmcc_s.obj"

"$(OUTDIR)\wlan_cmcc_s.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "wlan_cmcc_s - Win32 Debug"

OUTDIR=.\../lib/Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../lib/Debug
# End Custom Macros

ALL : "$(OUTDIR)\wlan_cmcc_s.dll"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\wlan_cmcc_s.obj"
	-@erase "$(OUTDIR)\wlan_cmcc_s.dll"
	-@erase "$(OUTDIR)\wlan_cmcc_s.exp"
	-@erase "$(OUTDIR)\wlan_cmcc_s.ilk"
	-@erase "$(OUTDIR)\wlan_cmcc_s.lib"
	-@erase "$(OUTDIR)\wlan_cmcc_s.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "wlan_cmcc_s_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\wlan_cmcc_s.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wlan_cmcc_s.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib toolbase.lib user32.lib commode.obj /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\wlan_cmcc_s.pdb" /debug /machine:I386 /out:"$(OUTDIR)\wlan_cmcc_s.dll" /implib:"$(OUTDIR)\wlan_cmcc_s.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\wlan_cmcc_s.obj"

"$(OUTDIR)\wlan_cmcc_s.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("wlan_cmcc_s.dep")
!INCLUDE "wlan_cmcc_s.dep"
!ELSE 
!MESSAGE Warning: cannot find "wlan_cmcc_s.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "wlan_cmcc_s - Win32 Release" || "$(CFG)" == "wlan_cmcc_s - Win32 Debug"
SOURCE=.\wlan_cmcc_s.cpp

"$(INTDIR)\wlan_cmcc_s.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

