# Microsoft Developer Studio Generated NMAKE File, Based on mml.dsp
!IF "$(CFG)" == ""
CFG=mml - Win32 Debug
!MESSAGE No configuration specified. Defaulting to mml - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "mml - Win32 Release" && "$(CFG)" != "mml - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mml.mak" CFG="mml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mml - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mml - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "mml - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\mml.lib"


CLEAN :
	-@erase "$(INTDIR)\FastDes.obj"
	-@erase "$(INTDIR)\MD5.obj"
	-@erase "$(INTDIR)\mml_ini_file.obj"
	-@erase "$(INTDIR)\mml_persist_cmd.obj"
	-@erase "$(INTDIR)\mml_persist_para.obj"
	-@erase "$(INTDIR)\mml_persist_usr.obj"
	-@erase "$(INTDIR)\mml_right_man.obj"
	-@erase "$(INTDIR)\mml_svr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\y.tab.obj"
	-@erase "..\lib\Release\mml.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\mml.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mml.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\mml.lib" 
LIB32_OBJS= \
	"$(INTDIR)\mml_ini_file.obj" \
	"$(INTDIR)\mml_persist_cmd.obj" \
	"$(INTDIR)\mml_persist_para.obj" \
	"$(INTDIR)\mml_persist_usr.obj" \
	"$(INTDIR)\mml_right_man.obj" \
	"$(INTDIR)\mml_svr.obj" \
	"$(INTDIR)\y.tab.obj" \
	"$(INTDIR)\FastDes.obj" \
	"$(INTDIR)\MD5.obj"

"..\lib\Release\mml.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mml - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\mml.lib"


CLEAN :
	-@erase "$(INTDIR)\FastDes.obj"
	-@erase "$(INTDIR)\MD5.obj"
	-@erase "$(INTDIR)\mml_ini_file.obj"
	-@erase "$(INTDIR)\mml_persist_cmd.obj"
	-@erase "$(INTDIR)\mml_persist_para.obj"
	-@erase "$(INTDIR)\mml_persist_usr.obj"
	-@erase "$(INTDIR)\mml_right_man.obj"
	-@erase "$(INTDIR)\mml_svr.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\y.tab.obj"
	-@erase "..\lib\debug\mml.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\mml.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mml.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\mml.lib" 
LIB32_OBJS= \
	"$(INTDIR)\mml_ini_file.obj" \
	"$(INTDIR)\mml_persist_cmd.obj" \
	"$(INTDIR)\mml_persist_para.obj" \
	"$(INTDIR)\mml_persist_usr.obj" \
	"$(INTDIR)\mml_right_man.obj" \
	"$(INTDIR)\mml_svr.obj" \
	"$(INTDIR)\y.tab.obj" \
	"$(INTDIR)\FastDes.obj" \
	"$(INTDIR)\MD5.obj"

"..\lib\debug\mml.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("mml.dep")
!INCLUDE "mml.dep"
!ELSE 
!MESSAGE Warning: cannot find "mml.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "mml - Win32 Release" || "$(CFG)" == "mml - Win32 Debug"
SOURCE=.\FastDes.cpp

"$(INTDIR)\FastDes.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MD5.cpp

"$(INTDIR)\MD5.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_ini_file.cpp

"$(INTDIR)\mml_ini_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_persist_cmd.cpp

"$(INTDIR)\mml_persist_cmd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_persist_para.cpp

"$(INTDIR)\mml_persist_para.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_persist_usr.cpp

"$(INTDIR)\mml_persist_usr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_right_man.cpp

"$(INTDIR)\mml_right_man.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mml_svr.cpp

"$(INTDIR)\mml_svr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\y.tab.cpp

"$(INTDIR)\y.tab.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

