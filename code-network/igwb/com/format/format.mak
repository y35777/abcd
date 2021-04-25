# Microsoft Developer Studio Generated NMAKE File, Based on format.dsp
!IF "$(CFG)" == ""
CFG=format - Win32 Debug
!MESSAGE No configuration specified. Defaulting to format - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "format - Win32 Release" && "$(CFG)" != "format - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "format.mak" CFG="format - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "format - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "format - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "format - Win32 Release"

OUTDIR=.\..\lib\release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\lib\release
# End Custom Macros

ALL : "$(OUTDIR)\format.dll"


CLEAN :
	-@erase "$(INTDIR)\access_point.obj"
	-@erase "$(INTDIR)\channel.obj"
	-@erase "$(INTDIR)\condition.obj"
	-@erase "$(INTDIR)\convert.obj"
	-@erase "$(INTDIR)\convert_fun.obj"
	-@erase "$(INTDIR)\dis_condition.obj"
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\format_global.obj"
	-@erase "$(INTDIR)\format_lib.obj"
	-@erase "$(INTDIR)\ibill_processor_imp.obj"
	-@erase "$(INTDIR)\iformat_cfg_imp.obj"
	-@erase "$(INTDIR)\igwb.obj"
	-@erase "$(INTDIR)\iquery_browse_imp.obj"
	-@erase "$(INTDIR)\postfix_exp.obj"
	-@erase "$(INTDIR)\utility_ex.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\format.dll"
	-@erase "$(OUTDIR)\format.exp"
	-@erase "$(OUTDIR)\format.lib"
	-@erase ".\Release\format.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FORMAT_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\format.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\format.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib toolbase.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\format.pdb" /map:"$(INTDIR)\format.map" /machine:I386 /out:"$(OUTDIR)\format.dll" /implib:"$(OUTDIR)\format.lib" 
LINK32_OBJS= \
	"$(INTDIR)\access_point.obj" \
	"$(INTDIR)\channel.obj" \
	"$(INTDIR)\condition.obj" \
	"$(INTDIR)\convert.obj" \
	"$(INTDIR)\convert_fun.obj" \
	"$(INTDIR)\dis_condition.obj" \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\format_global.obj" \
	"$(INTDIR)\format_lib.obj" \
	"$(INTDIR)\ibill_processor_imp.obj" \
	"$(INTDIR)\iformat_cfg_imp.obj" \
	"$(INTDIR)\igwb.obj" \
	"$(INTDIR)\iquery_browse_imp.obj" \
	"$(INTDIR)\postfix_exp.obj" \
	"$(INTDIR)\utility_ex.obj"

"$(OUTDIR)\format.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "format - Win32 Debug"

OUTDIR=.\..\lib\debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\lib\debug
# End Custom Macros

ALL : "$(OUTDIR)\format.dll"


CLEAN :
	-@erase "$(INTDIR)\access_point.obj"
	-@erase "$(INTDIR)\channel.obj"
	-@erase "$(INTDIR)\condition.obj"
	-@erase "$(INTDIR)\convert.obj"
	-@erase "$(INTDIR)\convert_fun.obj"
	-@erase "$(INTDIR)\dis_condition.obj"
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\format_global.obj"
	-@erase "$(INTDIR)\format_lib.obj"
	-@erase "$(INTDIR)\ibill_processor_imp.obj"
	-@erase "$(INTDIR)\iformat_cfg_imp.obj"
	-@erase "$(INTDIR)\igwb.obj"
	-@erase "$(INTDIR)\iquery_browse_imp.obj"
	-@erase "$(INTDIR)\postfix_exp.obj"
	-@erase "$(INTDIR)\utility_ex.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\format.dll"
	-@erase "$(OUTDIR)\format.exp"
	-@erase "$(OUTDIR)\format.ilk"
	-@erase "$(OUTDIR)\format.lib"
	-@erase "$(OUTDIR)\format.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FORMAT_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\format.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\format.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib toolbase.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\format.pdb" /debug /machine:I386 /out:"$(OUTDIR)\format.dll" /implib:"$(OUTDIR)\format.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\access_point.obj" \
	"$(INTDIR)\channel.obj" \
	"$(INTDIR)\condition.obj" \
	"$(INTDIR)\convert.obj" \
	"$(INTDIR)\convert_fun.obj" \
	"$(INTDIR)\dis_condition.obj" \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\format_global.obj" \
	"$(INTDIR)\format_lib.obj" \
	"$(INTDIR)\ibill_processor_imp.obj" \
	"$(INTDIR)\iformat_cfg_imp.obj" \
	"$(INTDIR)\igwb.obj" \
	"$(INTDIR)\iquery_browse_imp.obj" \
	"$(INTDIR)\postfix_exp.obj" \
	"$(INTDIR)\utility_ex.obj"

"$(OUTDIR)\format.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("format.dep")
!INCLUDE "format.dep"
!ELSE 
!MESSAGE Warning: cannot find "format.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "format - Win32 Release" || "$(CFG)" == "format - Win32 Debug"
SOURCE=.\access_point.cpp

"$(INTDIR)\access_point.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\channel.cpp

"$(INTDIR)\channel.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\condition.cpp

"$(INTDIR)\condition.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\convert.cpp

"$(INTDIR)\convert.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\convert_fun.cpp

"$(INTDIR)\convert_fun.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dis_condition.cpp

"$(INTDIR)\dis_condition.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\format.cpp

"$(INTDIR)\format.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\format_global.cpp

"$(INTDIR)\format_global.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\format_lib.cpp

"$(INTDIR)\format_lib.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ibill_processor_imp.cpp

"$(INTDIR)\ibill_processor_imp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\iformat_cfg_imp.cpp

"$(INTDIR)\iformat_cfg_imp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\igwb.cpp

"$(INTDIR)\igwb.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\iquery_browse_imp.cpp

"$(INTDIR)\iquery_browse_imp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\postfix_exp.cpp

"$(INTDIR)\postfix_exp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\utility_ex.cpp

"$(INTDIR)\utility_ex.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

