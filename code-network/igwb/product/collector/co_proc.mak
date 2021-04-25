# Microsoft Developer Studio Generated NMAKE File, Based on co_proc.dsp
!IF "$(CFG)" == ""
CFG=co_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to co_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "co_proc - Win32 Release" && "$(CFG)" != "co_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "co_proc.mak" CFG="co_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "co_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "co_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "co_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\co_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\co_manager.obj"
	-@erase "$(INTDIR)\co_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\co_proc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\co_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\co_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib advapi32.lib user32.lib collector.lib collectconvertor.lib format.lib frame.lib comm.lib toolbox.lib toolbase.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\co_proc.pdb" /machine:I386 /out:"$(OUTDIR)\co_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\co_manager.obj" \
	"$(INTDIR)\co_proc.obj"

"$(OUTDIR)\co_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "co_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\co_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\co_manager.obj"
	-@erase "$(INTDIR)\co_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\co_proc.exe"
	-@erase "$(OUTDIR)\co_proc.ilk"
	-@erase "$(OUTDIR)\co_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\co_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\co_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib advapi32.lib user32.lib collector.lib collectconvertor.lib format.lib frame.lib comm.lib toolbox.lib toolbase.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\co_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\co_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\co_manager.obj" \
	"$(INTDIR)\co_proc.obj"

"$(OUTDIR)\co_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("co_proc.dep")
!INCLUDE "co_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "co_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "co_proc - Win32 Release" || "$(CFG)" == "co_proc - Win32 Debug"
SOURCE=.\co_manager.cpp

"$(INTDIR)\co_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\co_proc.cpp

"$(INTDIR)\co_proc.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

