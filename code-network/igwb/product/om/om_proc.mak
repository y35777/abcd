# Microsoft Developer Studio Generated NMAKE File, Based on om_proc.dsp
!IF "$(CFG)" == ""
CFG=om_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to om_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "om_proc - Win32 Release" && "$(CFG)" != "om_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "om_proc.mak" CFG="om_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "om_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "om_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "om_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\om_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\om_proc.obj"
	-@erase "$(INTDIR)\omm_manager.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\om_proc.exe"
	-@erase "$(OUTDIR)\om_proc.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\om_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\om_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib coagent.lib billtask.lib backup.lib perf.lib alarm.lib alarm_util.lib frame.lib comm.lib mml.lib toolbox.lib toolbase.lib querybrowse.lib log.lib format.lib cdrstat.lib commode.obj user32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\om_proc.pdb" /map:"$(INTDIR)\om_proc.map" /machine:I386 /out:"$(OUTDIR)\om_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\om_proc.obj" \
	"$(INTDIR)\omm_manager.obj"

"$(OUTDIR)\om_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "om_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\om_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\om_proc.obj"
	-@erase "$(INTDIR)\omm_manager.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\om_proc.exe"
	-@erase "$(OUTDIR)\om_proc.ilk"
	-@erase "$(OUTDIR)\om_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\om_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\om_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib coagent.lib billtask.lib backup.lib perf.lib alarm.lib alarm_util.lib frame.lib comm.lib mml.lib toolbox.lib toolbase.lib querybrowse.lib log.lib format.lib cdrstat.lib commode.obj user32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\om_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\om_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\om_proc.obj" \
	"$(INTDIR)\omm_manager.obj"

"$(OUTDIR)\om_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("om_proc.dep")
!INCLUDE "om_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "om_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "om_proc - Win32 Release" || "$(CFG)" == "om_proc - Win32 Debug"
SOURCE=.\om_proc.cpp

"$(INTDIR)\om_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\omm_manager.cpp

"$(INTDIR)\omm_manager.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

