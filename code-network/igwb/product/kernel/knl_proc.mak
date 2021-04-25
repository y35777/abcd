# Microsoft Developer Studio Generated NMAKE File, Based on knl_proc.dsp
!IF "$(CFG)" == ""
CFG=knl_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to knl_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "knl_proc - Win32 Release" && "$(CFG)" != "knl_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "knl_proc.mak" CFG="knl_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "knl_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "knl_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "knl_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\knl_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\knl_manager.obj"
	-@erase "$(INTDIR)\knl_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\knl_proc.exe"
	-@erase "$(OUTDIR)\knl_proc.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\knl_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\knl_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib comm.lib frame.lib clusterinterface.lib monitor.lib licensemanager.lib toolbox.lib toolbase.lib user32.lib commode.obj /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\knl_proc.pdb" /map:"$(INTDIR)\knl_proc.map" /machine:I386 /out:"$(OUTDIR)\knl_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\knl_manager.obj" \
	"$(INTDIR)\knl_proc.obj"

"$(OUTDIR)\knl_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "knl_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\knl_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\knl_manager.obj"
	-@erase "$(INTDIR)\knl_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\knl_proc.exe"
	-@erase "$(OUTDIR)\knl_proc.ilk"
	-@erase "$(OUTDIR)\knl_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_PLATFORM_WIN32" /D "_CONSOLE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\knl_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib comm.lib frame.lib clusterinterface.lib monitor.lib licensemanager.lib toolbox.lib toolbase.lib user32.lib commode.obj /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\knl_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\knl_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\knl_manager.obj" \
	"$(INTDIR)\knl_proc.obj"

"$(OUTDIR)\knl_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("knl_proc.dep")
!INCLUDE "knl_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "knl_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "knl_proc - Win32 Release" || "$(CFG)" == "knl_proc - Win32 Debug"
SOURCE=.\knl_manager.cpp

"$(INTDIR)\knl_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\knl_proc.cpp

"$(INTDIR)\knl_proc.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

