# Microsoft Developer Studio Generated NMAKE File, Based on cfg_proc.dsp
!IF "$(CFG)" == ""
CFG=cfg_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cfg_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cfg_proc - Win32 Release" && "$(CFG)" != "cfg_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cfg_proc.mak" CFG="cfg_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cfg_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cfg_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "cfg_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\cfg_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\cfg_manager.obj"
	-@erase "$(INTDIR)\cfg_proc.obj"
	-@erase "$(INTDIR)\cfg_service.obj"
	-@erase "$(INTDIR)\service_base.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\cfg_proc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cfg_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib frame.lib comm.lib toolbox.lib toolbase.lib mml.lib cfgsvr.lib commode.obj /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\cfg_proc.pdb" /machine:I386 /out:"$(OUTDIR)\cfg_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cfg_manager.obj" \
	"$(INTDIR)\cfg_proc.obj" \
	"$(INTDIR)\cfg_service.obj" \
	"$(INTDIR)\service_base.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\cfg_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cfg_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\cfg_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\cfg_manager.obj"
	-@erase "$(INTDIR)\cfg_proc.obj"
	-@erase "$(INTDIR)\cfg_service.obj"
	-@erase "$(INTDIR)\service_base.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\cfg_proc.exe"
	-@erase "$(OUTDIR)\cfg_proc.ilk"
	-@erase "$(OUTDIR)\cfg_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cfg_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib frame.lib comm.lib toolbox.lib toolbase.lib mml.lib cfgsvr.lib commode.obj /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\cfg_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\cfg_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cfg_manager.obj" \
	"$(INTDIR)\cfg_proc.obj" \
	"$(INTDIR)\cfg_service.obj" \
	"$(INTDIR)\service_base.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\cfg_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("cfg_proc.dep")
!INCLUDE "cfg_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "cfg_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cfg_proc - Win32 Release" || "$(CFG)" == "cfg_proc - Win32 Debug"
SOURCE=.\cfg_manager.cpp

"$(INTDIR)\cfg_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cfg_proc.cpp

"$(INTDIR)\cfg_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\service_nt\cfg_service.cpp

"$(INTDIR)\cfg_service.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\service_nt\service_base.cpp

"$(INTDIR)\service_base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\service_nt\StdAfx.cpp

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

