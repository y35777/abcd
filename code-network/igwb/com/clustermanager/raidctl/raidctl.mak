# Microsoft Developer Studio Generated NMAKE File, Based on raidctl.dsp
!IF "$(CFG)" == ""
CFG=raidctl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to raidctl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "raidctl - Win32 Release" && "$(CFG)" != "raidctl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "raidctl.mak" CFG="raidctl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "raidctl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "raidctl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "raidctl - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\raidctl.dll"


CLEAN :
	-@erase "$(INTDIR)\raid_dev_ctl.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\raidctl.dll"
	-@erase "$(OUTDIR)\raidctl.exp"
	-@erase "$(OUTDIR)\raidctl.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RAIDCTL_EXPORTS" /D "INITGUID" /Fp"$(INTDIR)\raidctl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\raidctl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=setupapi.lib cfgmgr32.lib kernel32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\raidctl.pdb" /machine:I386 /out:"$(OUTDIR)\raidctl.dll" /implib:"$(OUTDIR)\raidctl.lib" 
LINK32_OBJS= \
	"$(INTDIR)\raid_dev_ctl.obj"

"$(OUTDIR)\raidctl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "raidctl - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\raidctl.dll" "$(OUTDIR)\raidctl.bsc"


CLEAN :
	-@erase "$(INTDIR)\raid_dev_ctl.obj"
	-@erase "$(INTDIR)\raid_dev_ctl.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\raidctl.bsc"
	-@erase "$(OUTDIR)\raidctl.dll"
	-@erase "$(OUTDIR)\raidctl.exp"
	-@erase "$(OUTDIR)\raidctl.ilk"
	-@erase "$(OUTDIR)\raidctl.lib"
	-@erase "$(OUTDIR)\raidctl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RAIDCTL_EXPORTS" /D "INITGUID" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\raidctl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\raidctl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\raid_dev_ctl.sbr"

"$(OUTDIR)\raidctl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=setupapi.lib cfgmgr32.lib kernel32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\raidctl.pdb" /debug /machine:I386 /out:"$(OUTDIR)\raidctl.dll" /implib:"$(OUTDIR)\raidctl.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\raid_dev_ctl.obj"

"$(OUTDIR)\raidctl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("raidctl.dep")
!INCLUDE "raidctl.dep"
!ELSE 
!MESSAGE Warning: cannot find "raidctl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "raidctl - Win32 Release" || "$(CFG)" == "raidctl - Win32 Debug"
SOURCE=.\raid_dev_ctl.cpp

!IF  "$(CFG)" == "raidctl - Win32 Release"


"$(INTDIR)\raid_dev_ctl.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "raidctl - Win32 Debug"


"$(INTDIR)\raid_dev_ctl.obj"	"$(INTDIR)\raid_dev_ctl.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

