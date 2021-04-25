# Microsoft Developer Studio Generated NMAKE File, Based on mtr_proc.dsp
!IF "$(CFG)" == ""
CFG=mtr_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to mtr_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "mtr_proc - Win32 Release" && "$(CFG)" != "mtr_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mtr_proc.mak" CFG="mtr_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mtr_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mtr_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "mtr_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\mtr_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\mtr_manager.obj"
	-@erase "$(INTDIR)\mtr_proc.obj"
	-@erase "$(INTDIR)\mtr_service.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\mtr_proc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mtr_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib frame.lib comm.lib format.lib toolbox.lib meteracc.lib toolbase.lib commode.obj user32.lib advapi32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\mtr_proc.pdb" /machine:I386 /out:"$(OUTDIR)\mtr_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\mtr_manager.obj" \
	"$(INTDIR)\mtr_proc.obj" \
	"$(INTDIR)\mtr_service.obj"

"$(OUTDIR)\mtr_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mtr_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\mtr_proc.exe" "$(OUTDIR)\mtr_proc.bsc"


CLEAN :
	-@erase "$(INTDIR)\mtr_manager.obj"
	-@erase "$(INTDIR)\mtr_manager.sbr"
	-@erase "$(INTDIR)\mtr_proc.obj"
	-@erase "$(INTDIR)\mtr_proc.sbr"
	-@erase "$(INTDIR)\mtr_service.obj"
	-@erase "$(INTDIR)\mtr_service.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\mtr_proc.bsc"
	-@erase "$(OUTDIR)\mtr_proc.exe"
	-@erase "$(OUTDIR)\mtr_proc.ilk"
	-@erase "$(OUTDIR)\mtr_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mtr_proc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\mtr_manager.sbr" \
	"$(INTDIR)\mtr_proc.sbr" \
	"$(INTDIR)\mtr_service.sbr"

"$(OUTDIR)\mtr_proc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=aced.lib frame.lib comm.lib format.lib toolbox.lib meteracc.lib toolbase.lib commode.obj user32.lib advapi32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\mtr_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\mtr_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\mtr_manager.obj" \
	"$(INTDIR)\mtr_proc.obj" \
	"$(INTDIR)\mtr_service.obj"

"$(OUTDIR)\mtr_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("mtr_proc.dep")
!INCLUDE "mtr_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "mtr_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "mtr_proc - Win32 Release" || "$(CFG)" == "mtr_proc - Win32 Debug"
SOURCE=.\mtr_manager.cpp

!IF  "$(CFG)" == "mtr_proc - Win32 Release"


"$(INTDIR)\mtr_manager.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mtr_proc - Win32 Debug"


"$(INTDIR)\mtr_manager.obj"	"$(INTDIR)\mtr_manager.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mtr_proc.cpp

!IF  "$(CFG)" == "mtr_proc - Win32 Release"


"$(INTDIR)\mtr_proc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "mtr_proc - Win32 Debug"


"$(INTDIR)\mtr_proc.obj"	"$(INTDIR)\mtr_proc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\service_nt\mtr_service.cpp

!IF  "$(CFG)" == "mtr_proc - Win32 Release"


"$(INTDIR)\mtr_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "mtr_proc - Win32 Debug"


"$(INTDIR)\mtr_service.obj"	"$(INTDIR)\mtr_service.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

