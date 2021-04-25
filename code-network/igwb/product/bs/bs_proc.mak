# Microsoft Developer Studio Generated NMAKE File, Based on bs_proc.dsp
!IF "$(CFG)" == ""
CFG=bs_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to bs_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "bs_proc - Win32 Release" && "$(CFG)" != "bs_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bs_proc.mak" CFG="bs_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bs_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "bs_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "bs_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\bs_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\bs_manager.obj"
	-@erase "$(INTDIR)\bs_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\bs_proc.exe"
	-@erase "$(OUTDIR)\bs_proc.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\bs_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib frame.lib comm.lib cdrsender.lib toolbox.lib toolbase.lib commode.obj user32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\bs_proc.pdb" /map:"$(INTDIR)\bs_proc.map" /machine:I386 /out:"$(OUTDIR)\bs_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bs_manager.obj" \
	"$(INTDIR)\bs_proc.obj"

"$(OUTDIR)\bs_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "bs_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\bs_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\bs_manager.obj"
	-@erase "$(INTDIR)\bs_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\bs_proc.exe"
	-@erase "$(OUTDIR)\bs_proc.ilk"
	-@erase "$(OUTDIR)\bs_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\bs_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib frame.lib comm.lib cdrsender.lib toolbox.lib toolbase.lib commode.obj user32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\bs_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\bs_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\bs_manager.obj" \
	"$(INTDIR)\bs_proc.obj"

"$(OUTDIR)\bs_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("bs_proc.dep")
!INCLUDE "bs_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "bs_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "bs_proc - Win32 Release" || "$(CFG)" == "bs_proc - Win32 Debug"
SOURCE=.\bs_manager.cpp

"$(INTDIR)\bs_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bs_proc.cpp

"$(INTDIR)\bs_proc.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

