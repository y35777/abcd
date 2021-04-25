# Microsoft Developer Studio Generated NMAKE File, Based on ap_proc.dsp
!IF "$(CFG)" == ""
CFG=ap_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ap_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ap_proc - Win32 Release" && "$(CFG)" != "ap_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ap_proc.mak" CFG="ap_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ap_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ap_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "ap_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ap_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\ap_manager.obj"
	-@erase "$(INTDIR)\ap_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ap_proc.exe"
	-@erase "$(OUTDIR)\ap_proc.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\ap_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ap_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib comm.lib frame.lib toolbox.lib frontsave.lib backsave.lib netbase.lib format.lib merge.lib convert.lib merge.lib toolbase.lib user32.lib commode.obj /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ap_proc.pdb" /map:"$(INTDIR)\ap_proc.map" /machine:I386 /out:"$(OUTDIR)\ap_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ap_manager.obj" \
	"$(INTDIR)\ap_proc.obj"

"$(OUTDIR)\ap_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ap_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ap_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\ap_manager.obj"
	-@erase "$(INTDIR)\ap_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ap_proc.exe"
	-@erase "$(OUTDIR)\ap_proc.ilk"
	-@erase "$(OUTDIR)\ap_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\ap_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ap_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib comm.lib frame.lib toolbox.lib frontsave.lib backsave.lib netbase.lib format.lib merge.lib convert.lib merge.lib toolbase.lib user32.lib commode.obj /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ap_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ap_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ap_manager.obj" \
	"$(INTDIR)\ap_proc.obj"

"$(OUTDIR)\ap_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("ap_proc.dep")
!INCLUDE "ap_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "ap_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ap_proc - Win32 Release" || "$(CFG)" == "ap_proc - Win32 Debug"
SOURCE=.\ap_manager.cpp

"$(INTDIR)\ap_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ap_proc.cpp

"$(INTDIR)\ap_proc.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

