# Microsoft Developer Studio Generated NMAKE File, Based on convert.dsp
!IF "$(CFG)" == ""
CFG=convert - Win32 Debug
!MESSAGE No configuration specified. Defaulting to convert - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "convert - Win32 Release" && "$(CFG)" != "convert - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "convert.mak" CFG="convert - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "convert - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "convert - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "convert - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\convert.lib"


CLEAN :
	-@erase "$(INTDIR)\128_proc.obj"
	-@erase "$(INTDIR)\bill_proc.obj"
	-@erase "$(INTDIR)\convert.obj"
	-@erase "$(INTDIR)\gsm_proc.obj"
	-@erase "$(INTDIR)\proc_chanl.obj"
	-@erase "$(INTDIR)\ps_proc.obj"
	-@erase "$(INTDIR)\stp_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Wlan_proc.obj"
	-@erase "..\lib\Release\convert.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\convert.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\convert.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\convert.lib" 
LIB32_OBJS= \
	"$(INTDIR)\128_proc.obj" \
	"$(INTDIR)\bill_proc.obj" \
	"$(INTDIR)\convert.obj" \
	"$(INTDIR)\gsm_proc.obj" \
	"$(INTDIR)\proc_chanl.obj" \
	"$(INTDIR)\ps_proc.obj" \
	"$(INTDIR)\stp_proc.obj" \
	"$(INTDIR)\Wlan_proc.obj"

"..\lib\Release\convert.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "convert - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\convert.lib"


CLEAN :
	-@erase "$(INTDIR)\128_proc.obj"
	-@erase "$(INTDIR)\bill_proc.obj"
	-@erase "$(INTDIR)\convert.obj"
	-@erase "$(INTDIR)\gsm_proc.obj"
	-@erase "$(INTDIR)\proc_chanl.obj"
	-@erase "$(INTDIR)\ps_proc.obj"
	-@erase "$(INTDIR)\stp_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Wlan_proc.obj"
	-@erase "..\lib\debug\convert.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\convert.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\convert.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\convert.lib" 
LIB32_OBJS= \
	"$(INTDIR)\128_proc.obj" \
	"$(INTDIR)\bill_proc.obj" \
	"$(INTDIR)\convert.obj" \
	"$(INTDIR)\gsm_proc.obj" \
	"$(INTDIR)\proc_chanl.obj" \
	"$(INTDIR)\ps_proc.obj" \
	"$(INTDIR)\stp_proc.obj" \
	"$(INTDIR)\Wlan_proc.obj"

"..\lib\debug\convert.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("convert.dep")
!INCLUDE "convert.dep"
!ELSE 
!MESSAGE Warning: cannot find "convert.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "convert - Win32 Release" || "$(CFG)" == "convert - Win32 Debug"
SOURCE=.\128_proc.cpp

"$(INTDIR)\128_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bill_proc.cpp

"$(INTDIR)\bill_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\convert.cpp

"$(INTDIR)\convert.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gsm_proc.cpp

"$(INTDIR)\gsm_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\proc_chanl.cpp

"$(INTDIR)\proc_chanl.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ps_proc.cpp

"$(INTDIR)\ps_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stp_proc.cpp

"$(INTDIR)\stp_proc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Wlan_proc.cpp

"$(INTDIR)\Wlan_proc.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

