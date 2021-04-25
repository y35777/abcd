# Microsoft Developer Studio Generated NMAKE File, Based on billtask.dsp
!IF "$(CFG)" == ""
CFG=billtask - Win32 Debug
!MESSAGE No configuration specified. Defaulting to billtask - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "billtask - Win32 Release" && "$(CFG)" != "billtask - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "billtask.mak" CFG="billtask - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "billtask - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "billtask - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "billtask - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\billtask.lib"


CLEAN :
	-@erase "$(INTDIR)\bill_task.obj"
	-@erase "$(INTDIR)\bill_task_server.obj"
	-@erase "$(INTDIR)\scp_bill_file.obj"
	-@erase "$(INTDIR)\scp_file_group.obj"
	-@erase "$(INTDIR)\scp_rvy_task.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\billtask.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\billtask.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\billtask.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release/billtask.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bill_task.obj" \
	"$(INTDIR)\bill_task_server.obj" \
	"$(INTDIR)\scp_bill_file.obj" \
	"$(INTDIR)\scp_file_group.obj" \
	"$(INTDIR)\scp_rvy_task.obj"

"..\lib\Release\billtask.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "billtask - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\billtask.lib"


CLEAN :
	-@erase "$(INTDIR)\bill_task.obj"
	-@erase "$(INTDIR)\bill_task_server.obj"
	-@erase "$(INTDIR)\scp_bill_file.obj"
	-@erase "$(INTDIR)\scp_file_group.obj"
	-@erase "$(INTDIR)\scp_rvy_task.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "..\lib\debug\billtask.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\billtask.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\billtask.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/debug/billtask.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bill_task.obj" \
	"$(INTDIR)\bill_task_server.obj" \
	"$(INTDIR)\scp_bill_file.obj" \
	"$(INTDIR)\scp_file_group.obj" \
	"$(INTDIR)\scp_rvy_task.obj"

"..\lib\debug\billtask.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("billtask.dep")
!INCLUDE "billtask.dep"
!ELSE 
!MESSAGE Warning: cannot find "billtask.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "billtask - Win32 Release" || "$(CFG)" == "billtask - Win32 Debug"
SOURCE=.\bill_task.cpp

"$(INTDIR)\bill_task.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bill_task_server.cpp

"$(INTDIR)\bill_task_server.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\scp_bill_file.cpp

"$(INTDIR)\scp_bill_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\scp_file_group.cpp

"$(INTDIR)\scp_file_group.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\scp_rvy_task.cpp

"$(INTDIR)\scp_rvy_task.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

