# Microsoft Developer Studio Generated NMAKE File, Based on cdrsender.dsp
!IF "$(CFG)" == ""
CFG=cdrsender - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cdrsender - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cdrsender - Win32 Release" && "$(CFG)" != "cdrsender - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cdrsender.mak" CFG="cdrsender - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cdrsender - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cdrsender - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "cdrsender - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\cdrsender.lib"


CLEAN :
	-@erase "$(INTDIR)\cdr_buffer.obj"
	-@erase "$(INTDIR)\cdr_producer.obj"
	-@erase "$(INTDIR)\cdrsnd_svr.obj"
	-@erase "$(INTDIR)\gtp_client.obj"
	-@erase "$(INTDIR)\gtp_status.obj"
	-@erase "$(INTDIR)\sender_status.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\lib\Release\cdrsender.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\cdrsender.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdrsender.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\cdrsender.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cdr_buffer.obj" \
	"$(INTDIR)\cdr_producer.obj" \
	"$(INTDIR)\cdrsnd_svr.obj" \
	"$(INTDIR)\gtp_client.obj" \
	"$(INTDIR)\gtp_status.obj" \
	"$(INTDIR)\sender_status.obj"

"..\lib\Release\cdrsender.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cdrsender - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\lib\debug\cdrsender.lib" "$(OUTDIR)\cdrsender.bsc"


CLEAN :
	-@erase "$(INTDIR)\cdr_buffer.obj"
	-@erase "$(INTDIR)\cdr_buffer.sbr"
	-@erase "$(INTDIR)\cdr_producer.obj"
	-@erase "$(INTDIR)\cdr_producer.sbr"
	-@erase "$(INTDIR)\cdrsnd_svr.obj"
	-@erase "$(INTDIR)\cdrsnd_svr.sbr"
	-@erase "$(INTDIR)\gtp_client.obj"
	-@erase "$(INTDIR)\gtp_client.sbr"
	-@erase "$(INTDIR)\gtp_status.obj"
	-@erase "$(INTDIR)\gtp_status.sbr"
	-@erase "$(INTDIR)\sender_status.obj"
	-@erase "$(INTDIR)\sender_status.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\cdrsender.bsc"
	-@erase "..\lib\debug\cdrsender.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\cdrsender.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cdrsender.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\cdr_buffer.sbr" \
	"$(INTDIR)\cdr_producer.sbr" \
	"$(INTDIR)\cdrsnd_svr.sbr" \
	"$(INTDIR)\gtp_client.sbr" \
	"$(INTDIR)\gtp_status.sbr" \
	"$(INTDIR)\sender_status.sbr"

"$(OUTDIR)\cdrsender.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\debug\cdrsender.lib" 
LIB32_OBJS= \
	"$(INTDIR)\cdr_buffer.obj" \
	"$(INTDIR)\cdr_producer.obj" \
	"$(INTDIR)\cdrsnd_svr.obj" \
	"$(INTDIR)\gtp_client.obj" \
	"$(INTDIR)\gtp_status.obj" \
	"$(INTDIR)\sender_status.obj"

"..\lib\debug\cdrsender.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("cdrsender.dep")
!INCLUDE "cdrsender.dep"
!ELSE 
!MESSAGE Warning: cannot find "cdrsender.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cdrsender - Win32 Release" || "$(CFG)" == "cdrsender - Win32 Debug"
SOURCE=.\cdr_buffer.cpp

!IF  "$(CFG)" == "cdrsender - Win32 Release"


"$(INTDIR)\cdr_buffer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cdrsender - Win32 Debug"


"$(INTDIR)\cdr_buffer.obj"	"$(INTDIR)\cdr_buffer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\cdr_producer.cpp

!IF  "$(CFG)" == "cdrsender - Win32 Release"


"$(INTDIR)\cdr_producer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cdrsender - Win32 Debug"


"$(INTDIR)\cdr_producer.obj"	"$(INTDIR)\cdr_producer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\cdrsnd_svr.cpp

!IF  "$(CFG)" == "cdrsender - Win32 Release"


"$(INTDIR)\cdrsnd_svr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cdrsender - Win32 Debug"


"$(INTDIR)\cdrsnd_svr.obj"	"$(INTDIR)\cdrsnd_svr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gtp_client.cpp

!IF  "$(CFG)" == "cdrsender - Win32 Release"


"$(INTDIR)\gtp_client.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cdrsender - Win32 Debug"


"$(INTDIR)\gtp_client.obj"	"$(INTDIR)\gtp_client.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gtp_status.cpp

!IF  "$(CFG)" == "cdrsender - Win32 Release"


"$(INTDIR)\gtp_status.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cdrsender - Win32 Debug"


"$(INTDIR)\gtp_status.obj"	"$(INTDIR)\gtp_status.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\sender_status.cpp

!IF  "$(CFG)" == "cdrsender - Win32 Release"


"$(INTDIR)\sender_status.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cdrsender - Win32 Debug"


"$(INTDIR)\sender_status.obj"	"$(INTDIR)\sender_status.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

