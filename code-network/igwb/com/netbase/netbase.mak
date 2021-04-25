# Microsoft Developer Studio Generated NMAKE File, Based on netbase.dsp
!IF "$(CFG)" == ""
CFG=netbase - Win32 Debug
!MESSAGE No configuration specified. Defaulting to netbase - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "netbase - Win32 Release" && "$(CFG)" != "netbase - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netbase.mak" CFG="netbase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netbase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "netbase - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "netbase - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\netbase.lib"


CLEAN :
	-@erase "$(INTDIR)\data_buffer.obj"
	-@erase "$(INTDIR)\gtp_related.obj"
	-@erase "$(INTDIR)\net_app.obj"
	-@erase "$(INTDIR)\net_status.obj"
	-@erase "$(INTDIR)\network.obj"
	-@erase "$(INTDIR)\protocol_stack.obj"
	-@erase "$(INTDIR)\protocol_track.obj"
	-@erase "$(INTDIR)\scan_app.obj"
	-@erase "$(INTDIR)\scan_stack.obj"
	-@erase "$(INTDIR)\slide_wnd_related.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wlan_as_app.obj"
	-@erase "$(INTDIR)\wlan_as_encap.obj"
	-@erase "$(INTDIR)\wlan_as_stack.obj"
	-@erase "..\lib\Release\netbase.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\netbase.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\netbase.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Release\netbase.lib" 
LIB32_OBJS= \
	"$(INTDIR)\data_buffer.obj" \
	"$(INTDIR)\gtp_related.obj" \
	"$(INTDIR)\net_app.obj" \
	"$(INTDIR)\net_status.obj" \
	"$(INTDIR)\network.obj" \
	"$(INTDIR)\protocol_stack.obj" \
	"$(INTDIR)\protocol_track.obj" \
	"$(INTDIR)\scan_app.obj" \
	"$(INTDIR)\scan_stack.obj" \
	"$(INTDIR)\slide_wnd_related.obj" \
	"$(INTDIR)\wlan_as_app.obj" \
	"$(INTDIR)\wlan_as_stack.obj" \
	"$(INTDIR)\wlan_as_encap.obj"

"..\lib\Release\netbase.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "netbase - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\debug\netbase.lib"


CLEAN :
	-@erase "$(INTDIR)\data_buffer.obj"
	-@erase "$(INTDIR)\gtp_related.obj"
	-@erase "$(INTDIR)\net_app.obj"
	-@erase "$(INTDIR)\net_status.obj"
	-@erase "$(INTDIR)\network.obj"
	-@erase "$(INTDIR)\protocol_stack.obj"
	-@erase "$(INTDIR)\protocol_track.obj"
	-@erase "$(INTDIR)\scan_app.obj"
	-@erase "$(INTDIR)\scan_stack.obj"
	-@erase "$(INTDIR)\slide_wnd_related.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\wlan_as_app.obj"
	-@erase "$(INTDIR)\wlan_as_encap.obj"
	-@erase "$(INTDIR)\wlan_as_stack.obj"
	-@erase "..\lib\debug\netbase.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\netbase.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/debug/netbase.lib" 
LIB32_OBJS= \
	"$(INTDIR)\data_buffer.obj" \
	"$(INTDIR)\gtp_related.obj" \
	"$(INTDIR)\net_app.obj" \
	"$(INTDIR)\net_status.obj" \
	"$(INTDIR)\network.obj" \
	"$(INTDIR)\protocol_stack.obj" \
	"$(INTDIR)\protocol_track.obj" \
	"$(INTDIR)\scan_app.obj" \
	"$(INTDIR)\scan_stack.obj" \
	"$(INTDIR)\slide_wnd_related.obj" \
	"$(INTDIR)\wlan_as_app.obj" \
	"$(INTDIR)\wlan_as_stack.obj" \
	"$(INTDIR)\wlan_as_encap.obj"

"..\lib\debug\netbase.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("netbase.dep")
!INCLUDE "netbase.dep"
!ELSE 
!MESSAGE Warning: cannot find "netbase.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "netbase - Win32 Release" || "$(CFG)" == "netbase - Win32 Debug"
SOURCE=.\data_buffer.cpp

"$(INTDIR)\data_buffer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gtp_related.cpp

"$(INTDIR)\gtp_related.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\net_app.cpp

"$(INTDIR)\net_app.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\net_status.cpp

"$(INTDIR)\net_status.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\network.cpp

"$(INTDIR)\network.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\protocol_stack.cpp

"$(INTDIR)\protocol_stack.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\protocol_track.cpp

"$(INTDIR)\protocol_track.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\scan_app.cpp

"$(INTDIR)\scan_app.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\scan_stack.cpp

"$(INTDIR)\scan_stack.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\slide_wnd_related.cpp

"$(INTDIR)\slide_wnd_related.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\wlan_as_app.cpp

"$(INTDIR)\wlan_as_app.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\wlan_as_encap.cpp

"$(INTDIR)\wlan_as_encap.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\wlan_as_stack.cpp

"$(INTDIR)\wlan_as_stack.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

