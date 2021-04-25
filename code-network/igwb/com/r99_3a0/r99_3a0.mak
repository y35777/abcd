# Microsoft Developer Studio Generated NMAKE File, Based on r99_3a0.dsp
!IF "$(CFG)" == ""
CFG=r99_3a0 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to r99_3a0 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "r99_3a0 - Win32 Release" && "$(CFG)" != "r99_3a0 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "r99_3a0.mak" CFG="r99_3a0 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "r99_3a0 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "r99_3a0 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "r99_3a0 - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\r99_3a0.dll"


CLEAN :
	-@erase "$(INTDIR)\asn1_r99_final.obj"
	-@erase "$(INTDIR)\asn1_r99_ini.obj"
	-@erase "$(INTDIR)\CDRF_CMCC_3GCDR.OBJ"
	-@erase "$(INTDIR)\CDRF_CMCC_3GCDR_CG.OBJ"
	-@erase "$(INTDIR)\format_r99.obj"
	-@erase "$(INTDIR)\r99_bill.obj"
	-@erase "$(INTDIR)\r99_cdr_browse.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\r99_3a0.dll"
	-@erase "$(OUTDIR)\r99_3a0.exp"
	-@erase "$(OUTDIR)\r99_3a0.lib"
	-@erase ".\Release\r99_3a0.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "r99_3a0_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\r99_3a0.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\r99_3a0.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib format.lib merge.lib toolbase.lib toedcomd.lib user32.lib commode.obj /nologo /dll /incremental:no /pdb:"$(OUTDIR)\r99_3a0.pdb" /map:"$(INTDIR)\r99_3a0.map" /machine:I386 /out:"$(OUTDIR)\r99_3a0.dll" /implib:"$(OUTDIR)\r99_3a0.lib" 
LINK32_OBJS= \
	"$(INTDIR)\asn1_r99_final.obj" \
	"$(INTDIR)\asn1_r99_ini.obj" \
	"$(INTDIR)\CDRF_CMCC_3GCDR.OBJ" \
	"$(INTDIR)\CDRF_CMCC_3GCDR_CG.OBJ" \
	"$(INTDIR)\format_r99.obj" \
	"$(INTDIR)\r99_bill.obj" \
	"$(INTDIR)\r99_cdr_browse.obj" \
	"$(INTDIR)\utility.obj"

"$(OUTDIR)\r99_3a0.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "r99_3a0 - Win32 Debug"

OUTDIR=.\../lib/Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../lib/Debug
# End Custom Macros

ALL : "$(OUTDIR)\r99_3a0.dll"


CLEAN :
	-@erase "$(INTDIR)\asn1_r99_final.obj"
	-@erase "$(INTDIR)\asn1_r99_ini.obj"
	-@erase "$(INTDIR)\CDRF_CMCC_3GCDR.OBJ"
	-@erase "$(INTDIR)\CDRF_CMCC_3GCDR_CG.OBJ"
	-@erase "$(INTDIR)\format_r99.obj"
	-@erase "$(INTDIR)\r99_bill.obj"
	-@erase "$(INTDIR)\r99_cdr_browse.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\r99_3a0.dll"
	-@erase "$(OUTDIR)\r99_3a0.exp"
	-@erase "$(OUTDIR)\r99_3a0.ilk"
	-@erase "$(OUTDIR)\r99_3a0.lib"
	-@erase "$(OUTDIR)\r99_3a0.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "r99_3a0_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\r99_3a0.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\r99_3a0.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib format.lib merge.lib toolbase.lib toedcomd.lib user32.lib commode.obj /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\r99_3a0.pdb" /debug /machine:I386 /out:"$(OUTDIR)\r99_3a0.dll" /implib:"$(OUTDIR)\r99_3a0.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\asn1_r99_final.obj" \
	"$(INTDIR)\asn1_r99_ini.obj" \
	"$(INTDIR)\CDRF_CMCC_3GCDR.OBJ" \
	"$(INTDIR)\CDRF_CMCC_3GCDR_CG.OBJ" \
	"$(INTDIR)\format_r99.obj" \
	"$(INTDIR)\r99_bill.obj" \
	"$(INTDIR)\r99_cdr_browse.obj" \
	"$(INTDIR)\utility.obj"

"$(OUTDIR)\r99_3a0.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("r99_3a0.dep")
!INCLUDE "r99_3a0.dep"
!ELSE 
!MESSAGE Warning: cannot find "r99_3a0.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "r99_3a0 - Win32 Release" || "$(CFG)" == "r99_3a0 - Win32 Debug"
SOURCE=.\asn1_r99\asn1_r99_final.cpp

"$(INTDIR)\asn1_r99_final.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\asn1_r99\asn1_r99_ini.cpp

"$(INTDIR)\asn1_r99_ini.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\asn1_r99_nt\CDRF_CMCC_3GCDR.C

"$(INTDIR)\CDRF_CMCC_3GCDR.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\asn1_r99_nt\CDRF_CMCC_3GCDR_CG.C

"$(INTDIR)\CDRF_CMCC_3GCDR_CG.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\format_r99.cpp

"$(INTDIR)\format_r99.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\r99\r99_bill.cpp

"$(INTDIR)\r99_bill.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\r99_cdr_browse.cpp

"$(INTDIR)\r99_cdr_browse.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\utility.cpp

"$(INTDIR)\utility.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

