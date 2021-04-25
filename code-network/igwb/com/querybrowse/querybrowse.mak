# Microsoft Developer Studio Generated NMAKE File, Based on querybrowse.dsp
!IF "$(CFG)" == ""
CFG=querybrowse - Win32 Debug
!MESSAGE No configuration specified. Defaulting to querybrowse - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "querybrowse - Win32 Release" && "$(CFG)" != "querybrowse - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "querybrowse.mak" CFG="querybrowse - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "querybrowse - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "querybrowse - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "querybrowse - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\querybrowse.lib"


CLEAN :
	-@erase "$(INTDIR)\bill_file.obj"
	-@erase "$(INTDIR)\bill_qb.obj"
	-@erase "$(INTDIR)\bill_rec.obj"
	-@erase "$(INTDIR)\file_group.obj"
	-@erase "$(INTDIR)\qb_result.obj"
	-@erase "$(INTDIR)\qb_task.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\querybrowse.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\querybrowse.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\querybrowse.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\querybrowse.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bill_file.obj" \
	"$(INTDIR)\bill_qb.obj" \
	"$(INTDIR)\bill_rec.obj" \
	"$(INTDIR)\file_group.obj" \
	"$(INTDIR)\qb_result.obj" \
	"$(INTDIR)\qb_task.obj"

"..\lib\Release\querybrowse.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "querybrowse - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\Debug\querybrowse.lib"


CLEAN :
	-@erase "$(INTDIR)\bill_file.obj"
	-@erase "$(INTDIR)\bill_qb.obj"
	-@erase "$(INTDIR)\bill_rec.obj"
	-@erase "$(INTDIR)\file_group.obj"
	-@erase "$(INTDIR)\qb_result.obj"
	-@erase "$(INTDIR)\qb_task.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\Debug\querybrowse.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\querybrowse.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\querybrowse.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Debug\querybrowse.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bill_file.obj" \
	"$(INTDIR)\bill_qb.obj" \
	"$(INTDIR)\bill_rec.obj" \
	"$(INTDIR)\file_group.obj" \
	"$(INTDIR)\qb_result.obj" \
	"$(INTDIR)\qb_task.obj"

"..\lib\Debug\querybrowse.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("querybrowse.dep")
!INCLUDE "querybrowse.dep"
!ELSE 
!MESSAGE Warning: cannot find "querybrowse.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "querybrowse - Win32 Release" || "$(CFG)" == "querybrowse - Win32 Debug"
SOURCE=.\bill_file.cpp

"$(INTDIR)\bill_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bill_qb.cpp

"$(INTDIR)\bill_qb.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bill_rec.cpp

"$(INTDIR)\bill_rec.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\file_group.cpp

"$(INTDIR)\file_group.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\qb_result.cpp

"$(INTDIR)\qb_result.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\qb_task.cpp

"$(INTDIR)\qb_task.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

