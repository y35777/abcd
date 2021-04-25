# Microsoft Developer Studio Generated NMAKE File, Based on cls_proc.dsp
!IF "$(CFG)" == ""
CFG=cls_proc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cls_proc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cls_proc - Win32 Release" && "$(CFG)" != "cls_proc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cls_proc.mak" CFG="cls_proc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cls_proc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cls_proc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "cls_proc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\cls_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\cls_manager.obj"
	-@erase "$(INTDIR)\cls_proc.obj"
	-@erase "$(INTDIR)\cls_proc.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\cls_proc.exe"
	-@erase "$(OUTDIR)\cls_proc.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\cls_proc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\cls_proc.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cls_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib comm.lib frame.lib RAIDHLP.lib toolbox.lib toolbase.lib format.lib frontsave.lib Iphlpapi.lib clustermanager.lib commode.obj /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\cls_proc.pdb" /map:"$(INTDIR)\cls_proc.map" /machine:I386 /out:"$(OUTDIR)\cls_proc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\cls_manager.obj" \
	"$(INTDIR)\cls_proc.obj" \
	"$(INTDIR)\cls_proc.res"

"$(OUTDIR)\cls_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cls_proc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\cls_proc.exe"


CLEAN :
	-@erase "$(INTDIR)\cls_manager.obj"
	-@erase "$(INTDIR)\cls_proc.obj"
	-@erase "$(INTDIR)\cls_proc.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\cls_proc.exe"
	-@erase "$(OUTDIR)\cls_proc.ilk"
	-@erase "$(OUTDIR)\cls_proc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\cls_proc.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cls_proc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib comm.lib frame.lib RAIDHLP.lib toolbox.lib toolbase.lib format.lib frontsave.lib Iphlpapi.lib clustermanager.lib commode.obj /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\cls_proc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\cls_proc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\cls_manager.obj" \
	"$(INTDIR)\cls_proc.obj" \
	"$(INTDIR)\cls_proc.res"

"$(OUTDIR)\cls_proc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("cls_proc.dep")
!INCLUDE "cls_proc.dep"
!ELSE 
!MESSAGE Warning: cannot find "cls_proc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cls_proc - Win32 Release" || "$(CFG)" == "cls_proc - Win32 Debug"
SOURCE=.\cls_manager.cpp

"$(INTDIR)\cls_manager.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cls_proc.cpp

"$(INTDIR)\cls_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cls_proc.rc

"$(INTDIR)\cls_proc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

