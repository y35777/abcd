# Microsoft Developer Studio Generated NMAKE File, Based on wlan.dsp
!IF "$(CFG)" == ""
CFG=wlan - Win32 Debug
!MESSAGE No configuration specified. Defaulting to wlan - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "wlan - Win32 Release" && "$(CFG)" != "wlan - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wlan.mak" CFG="wlan - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wlan - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wlan - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "wlan - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\wlan.dll"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wlan_bill.obj"
	-@erase "$(OUTDIR)\wlan.dll"
	-@erase "$(OUTDIR)\wlan.exp"
	-@erase "$(OUTDIR)\wlan.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "wlan_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\wlan.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wlan.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib format.lib merge.lib toedcomd.lib user32.lib commode.obj /nologo /dll /incremental:no /pdb:"$(OUTDIR)\wlan.pdb" /machine:I386 /out:"$(OUTDIR)\wlan.dll" /implib:"$(OUTDIR)\wlan.lib" 
LINK32_OBJS= \
	"$(INTDIR)\wlan_bill.obj"

"$(OUTDIR)\wlan.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "wlan - Win32 Debug"

OUTDIR=.\../lib/Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../lib/Debug
# End Custom Macros

ALL : "$(OUTDIR)\wlan.dll"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\wlan_bill.obj"
	-@erase "$(OUTDIR)\wlan.dll"
	-@erase "$(OUTDIR)\wlan.exp"
	-@erase "$(OUTDIR)\wlan.ilk"
	-@erase "$(OUTDIR)\wlan.lib"
	-@erase "$(OUTDIR)\wlan.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "wlan_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\wlan.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wlan.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib format.lib merge.lib toedcomd.lib user32.lib commode.obj /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\wlan.pdb" /debug /machine:I386 /out:"$(OUTDIR)\wlan.dll" /implib:"$(OUTDIR)\wlan.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\wlan_bill.obj"

"$(OUTDIR)\wlan.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("wlan.dep")
!INCLUDE "wlan.dep"
!ELSE 
!MESSAGE Warning: cannot find "wlan.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "wlan - Win32 Release" || "$(CFG)" == "wlan - Win32 Debug"
SOURCE=.\wlan_bill.cpp

"$(INTDIR)\wlan_bill.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

