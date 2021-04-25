# Microsoft Developer Studio Generated NMAKE File, Based on xml_util.dsp
!IF "$(CFG)" == ""
CFG=xml_util - Win32 Debug
!MESSAGE No configuration specified. Defaulting to xml_util - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "xml_util - Win32 Release" && "$(CFG)" != "xml_util - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xml_util.mak" CFG="xml_util - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xml_util - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xml_util - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "xml_util - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\xml_util.dll"


CLEAN :
	-@erase "$(INTDIR)\alarm_xml_util.obj"
	-@erase "$(INTDIR)\log_xml_util.obj"
	-@erase "$(INTDIR)\m2000_log_xml.obj"
	-@erase "$(INTDIR)\perf_xml_util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\xml_cdr_stat_cfg.obj"
	-@erase "$(INTDIR)\xml_cdr_stat_rst.obj"
	-@erase "$(INTDIR)\xml_file_opr.obj"
	-@erase "$(OUTDIR)\xml_util.dll"
	-@erase "$(OUTDIR)\xml_util.exp"
	-@erase "$(OUTDIR)\xml_util.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "xml_util_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\xml_util.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\xml_util.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=toolbase.lib alarm_util.lib ace.lib xerces-c_2.lib commode.obj /nologo /dll /incremental:no /pdb:"$(OUTDIR)\xml_util.pdb" /machine:I386 /out:"$(OUTDIR)\xml_util.dll" /implib:"$(OUTDIR)\xml_util.lib" 
LINK32_OBJS= \
	"$(INTDIR)\alarm_xml_util.obj" \
	"$(INTDIR)\log_xml_util.obj" \
	"$(INTDIR)\m2000_log_xml.obj" \
	"$(INTDIR)\perf_xml_util.obj" \
	"$(INTDIR)\xml_cdr_stat_cfg.obj" \
	"$(INTDIR)\xml_cdr_stat_rst.obj" \
	"$(INTDIR)\xml_file_opr.obj"

"$(OUTDIR)\xml_util.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "xml_util - Win32 Debug"

OUTDIR=.\../lib/debug
INTDIR=.\debug
# Begin Custom Macros
OutDir=.\../lib/debug
# End Custom Macros

ALL : "$(OUTDIR)\xml_util.dll"


CLEAN :
	-@erase "$(INTDIR)\alarm_xml_util.obj"
	-@erase "$(INTDIR)\log_xml_util.obj"
	-@erase "$(INTDIR)\m2000_log_xml.obj"
	-@erase "$(INTDIR)\perf_xml_util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\xml_cdr_stat_cfg.obj"
	-@erase "$(INTDIR)\xml_cdr_stat_rst.obj"
	-@erase "$(INTDIR)\xml_file_opr.obj"
	-@erase "$(OUTDIR)\xml_util.dll"
	-@erase "$(OUTDIR)\xml_util.exp"
	-@erase "$(OUTDIR)\xml_util.ilk"
	-@erase "$(OUTDIR)\xml_util.lib"
	-@erase "$(OUTDIR)\xml_util.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "xml_util_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\xml_util.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\xml_util.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=toolbase.lib alarm_util.lib aced.lib xerces-c_2d.lib commode.obj /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\xml_util.pdb" /debug /machine:I386 /out:"$(OUTDIR)\xml_util.dll" /implib:"$(OUTDIR)\xml_util.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\alarm_xml_util.obj" \
	"$(INTDIR)\log_xml_util.obj" \
	"$(INTDIR)\m2000_log_xml.obj" \
	"$(INTDIR)\perf_xml_util.obj" \
	"$(INTDIR)\xml_cdr_stat_cfg.obj" \
	"$(INTDIR)\xml_cdr_stat_rst.obj" \
	"$(INTDIR)\xml_file_opr.obj"

"$(OUTDIR)\xml_util.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("xml_util.dep")
!INCLUDE "xml_util.dep"
!ELSE 
!MESSAGE Warning: cannot find "xml_util.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "xml_util - Win32 Release" || "$(CFG)" == "xml_util - Win32 Debug"
SOURCE=.\alarm_xml_util.cpp

"$(INTDIR)\alarm_xml_util.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\log_xml_util.cpp

"$(INTDIR)\log_xml_util.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\m2000_log_xml.cpp

"$(INTDIR)\m2000_log_xml.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perf_xml_util.cpp

"$(INTDIR)\perf_xml_util.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xml_cdr_stat_cfg.cpp

"$(INTDIR)\xml_cdr_stat_cfg.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xml_cdr_stat_rst.cpp

"$(INTDIR)\xml_cdr_stat_rst.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xml_file_opr.cpp

"$(INTDIR)\xml_file_opr.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

