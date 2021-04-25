# Microsoft Developer Studio Generated NMAKE File, Based on backup.dsp
!IF "$(CFG)" == ""
CFG=backup - Win32 Debug
!MESSAGE No configuration specified. Defaulting to backup - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "backup - Win32 Release" && "$(CFG)" != "backup - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "backup.mak" CFG="backup - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "backup - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "backup - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "backup - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\backup.lib"


CLEAN :
	-@erase "$(INTDIR)\backup_file_pool.obj"
	-@erase "$(INTDIR)\backup_server.obj"
	-@erase "$(INTDIR)\backup_task.obj"
	-@erase "$(INTDIR)\bk_status_file.obj"
	-@erase "$(INTDIR)\manual_task.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\backup.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\backup.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\backup.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\backup.lib" 
LIB32_OBJS= \
	"$(INTDIR)\backup_file_pool.obj" \
	"$(INTDIR)\backup_server.obj" \
	"$(INTDIR)\backup_task.obj" \
	"$(INTDIR)\bk_status_file.obj" \
	"$(INTDIR)\manual_task.obj"

"..\lib\Release\backup.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "backup - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\backup.lib"


CLEAN :
	-@erase "$(INTDIR)\backup_file_pool.obj"
	-@erase "$(INTDIR)\backup_server.obj"
	-@erase "$(INTDIR)\backup_task.obj"
	-@erase "$(INTDIR)\bk_status_file.obj"
	-@erase "$(INTDIR)\manual_task.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\backup.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\backup.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\backup.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\backup.lib" 
LIB32_OBJS= \
	"$(INTDIR)\backup_file_pool.obj" \
	"$(INTDIR)\backup_server.obj" \
	"$(INTDIR)\backup_task.obj" \
	"$(INTDIR)\bk_status_file.obj" \
	"$(INTDIR)\manual_task.obj"

"..\lib\debug\backup.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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
!IF EXISTS("backup.dep")
!INCLUDE "backup.dep"
!ELSE 
!MESSAGE Warning: cannot find "backup.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "backup - Win32 Release" || "$(CFG)" == "backup - Win32 Debug"
SOURCE=.\backup_file_pool.cpp

"$(INTDIR)\backup_file_pool.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\backup_server.cpp

"$(INTDIR)\backup_server.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\backup_task.cpp

"$(INTDIR)\backup_task.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bk_status_file.cpp

"$(INTDIR)\bk_status_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\manual_task.cpp

"$(INTDIR)\manual_task.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

