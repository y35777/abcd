# Microsoft Developer Studio Generated NMAKE File, Based on toolbase.dsp
!IF "$(CFG)" == ""
CFG=toolbase - Win32 Debug
!MESSAGE No configuration specified. Defaulting to toolbase - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "toolbase - Win32 Release" && "$(CFG)" != "toolbase - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "toolbase.mak" CFG="toolbase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "toolbase - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "toolbase - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "toolbase - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\toolbase.dll"


CLEAN :
	-@erase "$(INTDIR)\field_filter.obj"
	-@erase "$(INTDIR)\frame_tool.obj"
	-@erase "$(INTDIR)\ftp_client.obj"
	-@erase "$(INTDIR)\ini_file.obj"
	-@erase "$(INTDIR)\ini_file_ex.obj"
	-@erase "$(INTDIR)\persist_err.obj"
	-@erase "$(INTDIR)\persist_resource.obj"
	-@erase "$(INTDIR)\ps_asn_encap.obj"
	-@erase "$(INTDIR)\toolbox.obj"
	-@erase "$(INTDIR)\trace.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\toolbase.dll"
	-@erase "$(OUTDIR)\toolbase.exp"
	-@erase "$(OUTDIR)\toolbase.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "toolbase_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\toolbase.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\toolbase.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib ws2_32.lib commode.obj user32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\toolbase.pdb" /machine:I386 /out:"$(OUTDIR)\toolbase.dll" /implib:"$(OUTDIR)\toolbase.lib" 
LINK32_OBJS= \
	"$(INTDIR)\field_filter.obj" \
	"$(INTDIR)\frame_tool.obj" \
	"$(INTDIR)\ftp_client.obj" \
	"$(INTDIR)\ini_file.obj" \
	"$(INTDIR)\ini_file_ex.obj" \
	"$(INTDIR)\persist_err.obj" \
	"$(INTDIR)\persist_resource.obj" \
	"$(INTDIR)\ps_asn_encap.obj" \
	"$(INTDIR)\toolbox.obj" \
	"$(INTDIR)\trace.obj"

"$(OUTDIR)\toolbase.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "toolbase - Win32 Debug"

OUTDIR=.\../lib/debug
INTDIR=.\debug
# Begin Custom Macros
OutDir=.\../lib/debug
# End Custom Macros

ALL : "$(OUTDIR)\toolbase.dll"


CLEAN :
	-@erase "$(INTDIR)\field_filter.obj"
	-@erase "$(INTDIR)\frame_tool.obj"
	-@erase "$(INTDIR)\ftp_client.obj"
	-@erase "$(INTDIR)\ini_file.obj"
	-@erase "$(INTDIR)\ini_file_ex.obj"
	-@erase "$(INTDIR)\persist_err.obj"
	-@erase "$(INTDIR)\persist_resource.obj"
	-@erase "$(INTDIR)\ps_asn_encap.obj"
	-@erase "$(INTDIR)\toolbox.obj"
	-@erase "$(INTDIR)\trace.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\toolbase.dll"
	-@erase "$(OUTDIR)\toolbase.exp"
	-@erase "$(OUTDIR)\toolbase.ilk"
	-@erase "$(OUTDIR)\toolbase.lib"
	-@erase "$(OUTDIR)\toolbase.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "toolbase_EXPORTS" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\toolbase.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\toolbase.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib ws2_32.lib commode.obj user32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\toolbase.pdb" /debug /machine:I386 /out:"$(OUTDIR)\toolbase.dll" /implib:"$(OUTDIR)\toolbase.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\field_filter.obj" \
	"$(INTDIR)\frame_tool.obj" \
	"$(INTDIR)\ftp_client.obj" \
	"$(INTDIR)\ini_file.obj" \
	"$(INTDIR)\ini_file_ex.obj" \
	"$(INTDIR)\persist_err.obj" \
	"$(INTDIR)\persist_resource.obj" \
	"$(INTDIR)\ps_asn_encap.obj" \
	"$(INTDIR)\toolbox.obj" \
	"$(INTDIR)\trace.obj"

"$(OUTDIR)\toolbase.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("toolbase.dep")
!INCLUDE "toolbase.dep"
!ELSE 
!MESSAGE Warning: cannot find "toolbase.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "toolbase - Win32 Release" || "$(CFG)" == "toolbase - Win32 Debug"
SOURCE=.\field_filter.cpp

"$(INTDIR)\field_filter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\frame_tool.cpp

"$(INTDIR)\frame_tool.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ftp_client.cpp

"$(INTDIR)\ftp_client.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ini_file.cpp

"$(INTDIR)\ini_file.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ini_file_ex.cpp

"$(INTDIR)\ini_file_ex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\persist_err.cpp

"$(INTDIR)\persist_err.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\persist_resource.cpp

"$(INTDIR)\persist_resource.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ps_asn_encap.cpp

"$(INTDIR)\ps_asn_encap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\toolbox.cpp

"$(INTDIR)\toolbox.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\trace.cpp

"$(INTDIR)\trace.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

