# Microsoft Developer Studio Generated NMAKE File, Based on r98_760.dsp
!IF "$(CFG)" == ""
CFG=r98_760 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to r98_760 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "r98_760 - Win32 Release" && "$(CFG)" != "r98_760 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "r98_760.mak" CFG="r98_760 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "r98_760 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "r98_760 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "r98_760 - Win32 Release"

OUTDIR=.\../lib/Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\../lib/Release
# End Custom Macros

ALL : "$(OUTDIR)\r98_760.dll"


CLEAN :
	-@erase "$(INTDIR)\asn1_r98_final.obj"
	-@erase "$(INTDIR)\asn1_r98_ini.obj"
	-@erase "$(INTDIR)\CDRF_2GCDR_Fin.obj"
	-@erase "$(INTDIR)\CDRF_2GCDR_Org.obj"
	-@erase "$(INTDIR)\format_r98.obj"
	-@erase "$(INTDIR)\r98_bill.obj"
	-@erase "$(INTDIR)\r98_cdr_browse.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\r98_760.dll"
	-@erase "$(OUTDIR)\r98_760.exp"
	-@erase "$(OUTDIR)\r98_760.lib"
	-@erase ".\Release\r98_760.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "r98_760_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\r98_760.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\r98_760.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ace.lib format.lib merge.lib toolbase.lib toedcomd.lib user32.lib commode.obj /nologo /dll /incremental:no /pdb:"$(OUTDIR)\r98_760.pdb" /map:"$(INTDIR)\r98_760.map" /machine:I386 /out:"$(OUTDIR)\r98_760.dll" /implib:"$(OUTDIR)\r98_760.lib" 
LINK32_OBJS= \
	"$(INTDIR)\asn1_r98_final.obj" \
	"$(INTDIR)\asn1_r98_ini.obj" \
	"$(INTDIR)\CDRF_2GCDR_Fin.obj" \
	"$(INTDIR)\CDRF_2GCDR_Org.obj" \
	"$(INTDIR)\format_r98.obj" \
	"$(INTDIR)\r98_bill.obj" \
	"$(INTDIR)\r98_cdr_browse.obj" \
	"$(INTDIR)\utility.obj"

"$(OUTDIR)\r98_760.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "r98_760 - Win32 Debug"

OUTDIR=.\../lib/Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\../lib/Debug
# End Custom Macros

ALL : "$(OUTDIR)\r98_760.dll"


CLEAN :
	-@erase "$(INTDIR)\asn1_r98_final.obj"
	-@erase "$(INTDIR)\asn1_r98_ini.obj"
	-@erase "$(INTDIR)\CDRF_2GCDR_Fin.obj"
	-@erase "$(INTDIR)\CDRF_2GCDR_Org.obj"
	-@erase "$(INTDIR)\format_r98.obj"
	-@erase "$(INTDIR)\r98_bill.obj"
	-@erase "$(INTDIR)\r98_cdr_browse.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\r98_760.dll"
	-@erase "$(OUTDIR)\r98_760.exp"
	-@erase "$(OUTDIR)\r98_760.ilk"
	-@erase "$(OUTDIR)\r98_760.lib"
	-@erase "$(OUTDIR)\r98_760.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "r98_760_EXPORTS" /D "_PLATFORM_WIN32" /D "ONE_DLL" /Fp"$(INTDIR)\r98_760.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\r98_760.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=aced.lib format.lib merge.lib toolbase.lib toedcomd.lib user32.lib commode.obj /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\r98_760.pdb" /debug /machine:I386 /out:"$(OUTDIR)\r98_760.dll" /implib:"$(OUTDIR)\r98_760.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\asn1_r98_final.obj" \
	"$(INTDIR)\asn1_r98_ini.obj" \
	"$(INTDIR)\CDRF_2GCDR_Fin.obj" \
	"$(INTDIR)\CDRF_2GCDR_Org.obj" \
	"$(INTDIR)\format_r98.obj" \
	"$(INTDIR)\r98_bill.obj" \
	"$(INTDIR)\r98_cdr_browse.obj" \
	"$(INTDIR)\utility.obj"

"$(OUTDIR)\r98_760.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("r98_760.dep")
!INCLUDE "r98_760.dep"
!ELSE 
!MESSAGE Warning: cannot find "r98_760.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "r98_760 - Win32 Release" || "$(CFG)" == "r98_760 - Win32 Debug"
SOURCE=.\asn1_r98\asn1_r98_final.cpp

"$(INTDIR)\asn1_r98_final.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\asn1_r98\asn1_r98_ini.cpp

"$(INTDIR)\asn1_r98_ini.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\asn1_r98_nt\CDRF_2GCDR_Fin.c

"$(INTDIR)\CDRF_2GCDR_Fin.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\asn1_r98_nt\CDRF_2GCDR_Org.c

"$(INTDIR)\CDRF_2GCDR_Org.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\format_r98.cpp

"$(INTDIR)\format_r98.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\r98\r98_bill.cpp

"$(INTDIR)\r98_bill.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\r98_cdr_browse.cpp

"$(INTDIR)\r98_cdr_browse.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\utility.cpp

"$(INTDIR)\utility.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

