# Microsoft Developer Studio Generated NMAKE File, Based on meteracc.dsp
!IF "$(CFG)" == ""
CFG=meteracc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to meteracc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "meteracc - Win32 Release" && "$(CFG)" != "meteracc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "meteracc.mak" CFG="meteracc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "meteracc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "meteracc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "meteracc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\meteracc.lib"


CLEAN :
	-@erase "$(INTDIR)\meter_acc.obj"
	-@erase "$(INTDIR)\meter_file.obj"
	-@erase "$(INTDIR)\meter_index.obj"
	-@erase "$(INTDIR)\meteracc_task.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\meteracc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\meteracc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /nometeracco /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/o"$(OUTDIR)\meteracc.bsc" /nometeracco 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/out:"../lib/Release\meteracc.lib" /nometeracco 
LIB32_OBJS= \
	"$(INTDIR)\meter_acc.obj" \
	"$(INTDIR)\meter_file.obj" \
	"$(INTDIR)\meter_index.obj" \
	"$(INTDIR)\meteracc_task.obj"

"..\lib\Release\meteracc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "meteracc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\lib\Debug\meteracc.lib" "$(OUTDIR)\meteracc.bsc"


CLEAN :
	-@erase "$(INTDIR)\meter_acc.obj"
	-@erase "$(INTDIR)\meter_acc.sbr"
	-@erase "$(INTDIR)\meter_file.obj"
	-@erase "$(INTDIR)\meter_file.sbr"
	-@erase "$(INTDIR)\meter_index.obj"
	-@erase "$(INTDIR)\meter_index.sbr"
	-@erase "$(INTDIR)\meteracc_task.obj"
	-@erase "$(INTDIR)\meteracc_task.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\meteracc.bsc"
	-@erase "..\lib\Debug\meteracc.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\meteracc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /nometeracco /GZ /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/o"$(OUTDIR)\meteracc.bsc" /nometeracco 
BSC32_SBRS= \
	"$(INTDIR)\meter_acc.sbr" \
	"$(INTDIR)\meter_file.sbr" \
	"$(INTDIR)\meter_index.sbr" \
	"$(INTDIR)\meteracc_task.sbr"

"$(OUTDIR)\meteracc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/out:"../lib/Debug\meteracc.lib" /nometeracco 
LIB32_OBJS= \
	"$(INTDIR)\meter_acc.obj" \
	"$(INTDIR)\meter_file.obj" \
	"$(INTDIR)\meter_index.obj" \
	"$(INTDIR)\meteracc_task.obj"

"..\lib\Debug\meteracc.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("meteracc.dep")
!INCLUDE "meteracc.dep"
!ELSE 
!MESSAGE Warning: cannot find "meteracc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "meteracc - Win32 Release" || "$(CFG)" == "meteracc - Win32 Debug"
SOURCE=.\meter_acc.cpp

!IF  "$(CFG)" == "meteracc - Win32 Release"


"$(INTDIR)\meter_acc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "meteracc - Win32 Debug"


"$(INTDIR)\meter_acc.obj"	"$(INTDIR)\meter_acc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\meter_file.cpp

!IF  "$(CFG)" == "meteracc - Win32 Release"


"$(INTDIR)\meter_file.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "meteracc - Win32 Debug"


"$(INTDIR)\meter_file.obj"	"$(INTDIR)\meter_file.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\meter_index.cpp

!IF  "$(CFG)" == "meteracc - Win32 Release"


"$(INTDIR)\meter_index.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "meteracc - Win32 Debug"


"$(INTDIR)\meter_index.obj"	"$(INTDIR)\meter_index.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\meteracc_task.cpp

!IF  "$(CFG)" == "meteracc - Win32 Release"


"$(INTDIR)\meteracc_task.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "meteracc - Win32 Debug"


"$(INTDIR)\meteracc_task.obj"	"$(INTDIR)\meteracc_task.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

