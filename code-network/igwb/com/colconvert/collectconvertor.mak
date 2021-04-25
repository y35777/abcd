# Microsoft Developer Studio Generated NMAKE File, Based on collectconvertor.dsp
!IF "$(CFG)" == ""
CFG=collectconvertor - Win32 Debug
!MESSAGE No configuration specified. Defaulting to collectconvertor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "collectconvertor - Win32 Release" && "$(CFG)" != "collectconvertor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "collectconvertor.mak" CFG="collectconvertor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "collectconvertor - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "collectconvertor - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "collectconvertor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\lib\Release\collectconvertor.lib"


CLEAN :
	-@erase "$(INTDIR)\5ess_collect_convert.obj"
	-@erase "$(INTDIR)\5ess_convert_fun.obj"
	-@erase "$(INTDIR)\5ess_prefix_collect_convert.obj"
	-@erase "$(INTDIR)\cc08_convert_fun.obj"
	-@erase "$(INTDIR)\collect_convert_base.obj"
	-@erase "$(INTDIR)\collect_convert_creator.obj"
	-@erase "$(INTDIR)\collect_convert_obj.obj"
	-@erase "$(INTDIR)\collect_convert_obj_factory.obj"
	-@erase "$(INTDIR)\collect_convert_subobj.obj"
	-@erase "$(INTDIR)\collect_convertor.obj"
	-@erase "$(INTDIR)\com_collect_convert.obj"
	-@erase "$(INTDIR)\com_dir_collect_convert.obj"
	-@erase "$(INTDIR)\comm_convert_fun.obj"
	-@erase "$(INTDIR)\ewsd_ama_collect_convert.obj"
	-@erase "$(INTDIR)\ewsd_amas_collect_convert.obj"
	-@erase "$(INTDIR)\ewsd_convert_fun.obj"
	-@erase "$(INTDIR)\ewsd_mm_collect_convert.obj"
	-@erase "$(INTDIR)\s1240_collect_convert.obj"
	-@erase "$(INTDIR)\s1240_convert_fun.obj"
	-@erase "$(INTDIR)\s1240_gb_convert_fun.obj"
	-@erase "$(INTDIR)\s1240_tp_collect_convert.obj"
	-@erase "$(INTDIR)\tellin_convert_fun.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\zte_convert_fun.obj"
	-@erase "..\lib\Release\collectconvertor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /Fp"$(INTDIR)\collectconvertor.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\collectconvertor.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\lib\Release\collectconvertor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\5ess_collect_convert.obj" \
	"$(INTDIR)\5ess_convert_fun.obj" \
	"$(INTDIR)\5ess_prefix_collect_convert.obj" \
	"$(INTDIR)\cc08_convert_fun.obj" \
	"$(INTDIR)\collect_convert_base.obj" \
	"$(INTDIR)\collect_convert_creator.obj" \
	"$(INTDIR)\collect_convert_obj.obj" \
	"$(INTDIR)\collect_convert_obj_factory.obj" \
	"$(INTDIR)\collect_convert_subobj.obj" \
	"$(INTDIR)\collect_convertor.obj" \
	"$(INTDIR)\com_collect_convert.obj" \
	"$(INTDIR)\com_dir_collect_convert.obj" \
	"$(INTDIR)\comm_convert_fun.obj" \
	"$(INTDIR)\ewsd_ama_collect_convert.obj" \
	"$(INTDIR)\ewsd_amas_collect_convert.obj" \
	"$(INTDIR)\ewsd_convert_fun.obj" \
	"$(INTDIR)\ewsd_mm_collect_convert.obj" \
	"$(INTDIR)\s1240_collect_convert.obj" \
	"$(INTDIR)\s1240_convert_fun.obj" \
	"$(INTDIR)\s1240_gb_convert_fun.obj" \
	"$(INTDIR)\s1240_tp_collect_convert.obj" \
	"$(INTDIR)\tellin_convert_fun.obj" \
	"$(INTDIR)\zte_convert_fun.obj"

"..\lib\Release\collectconvertor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "collectconvertor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\lib\Debug\collectconvertor.lib"


CLEAN :
	-@erase "$(INTDIR)\5ess_collect_convert.obj"
	-@erase "$(INTDIR)\5ess_convert_fun.obj"
	-@erase "$(INTDIR)\5ess_prefix_collect_convert.obj"
	-@erase "$(INTDIR)\cc08_convert_fun.obj"
	-@erase "$(INTDIR)\collect_convert_base.obj"
	-@erase "$(INTDIR)\collect_convert_creator.obj"
	-@erase "$(INTDIR)\collect_convert_obj.obj"
	-@erase "$(INTDIR)\collect_convert_obj_factory.obj"
	-@erase "$(INTDIR)\collect_convert_subobj.obj"
	-@erase "$(INTDIR)\collect_convertor.obj"
	-@erase "$(INTDIR)\com_collect_convert.obj"
	-@erase "$(INTDIR)\com_dir_collect_convert.obj"
	-@erase "$(INTDIR)\comm_convert_fun.obj"
	-@erase "$(INTDIR)\ewsd_ama_collect_convert.obj"
	-@erase "$(INTDIR)\ewsd_amas_collect_convert.obj"
	-@erase "$(INTDIR)\ewsd_convert_fun.obj"
	-@erase "$(INTDIR)\ewsd_mm_collect_convert.obj"
	-@erase "$(INTDIR)\s1240_collect_convert.obj"
	-@erase "$(INTDIR)\s1240_convert_fun.obj"
	-@erase "$(INTDIR)\s1240_gb_convert_fun.obj"
	-@erase "$(INTDIR)\s1240_tp_collect_convert.obj"
	-@erase "$(INTDIR)\tellin_convert_fun.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\zte_convert_fun.obj"
	-@erase "..\lib\Debug\collectconvertor.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_PLATFORM_WIN32" /D "_LIB_DEBUG_" /Fp"$(INTDIR)\collectconvertor.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\collectconvertor.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../lib/Debug\collectconvertor.lib" 
LIB32_OBJS= \
	"$(INTDIR)\5ess_collect_convert.obj" \
	"$(INTDIR)\5ess_convert_fun.obj" \
	"$(INTDIR)\5ess_prefix_collect_convert.obj" \
	"$(INTDIR)\cc08_convert_fun.obj" \
	"$(INTDIR)\collect_convert_base.obj" \
	"$(INTDIR)\collect_convert_creator.obj" \
	"$(INTDIR)\collect_convert_obj.obj" \
	"$(INTDIR)\collect_convert_obj_factory.obj" \
	"$(INTDIR)\collect_convert_subobj.obj" \
	"$(INTDIR)\collect_convertor.obj" \
	"$(INTDIR)\com_collect_convert.obj" \
	"$(INTDIR)\com_dir_collect_convert.obj" \
	"$(INTDIR)\comm_convert_fun.obj" \
	"$(INTDIR)\ewsd_ama_collect_convert.obj" \
	"$(INTDIR)\ewsd_amas_collect_convert.obj" \
	"$(INTDIR)\ewsd_convert_fun.obj" \
	"$(INTDIR)\ewsd_mm_collect_convert.obj" \
	"$(INTDIR)\s1240_collect_convert.obj" \
	"$(INTDIR)\s1240_convert_fun.obj" \
	"$(INTDIR)\s1240_gb_convert_fun.obj" \
	"$(INTDIR)\s1240_tp_collect_convert.obj" \
	"$(INTDIR)\tellin_convert_fun.obj" \
	"$(INTDIR)\zte_convert_fun.obj"

"..\lib\Debug\collectconvertor.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("collectconvertor.dep")
!INCLUDE "collectconvertor.dep"
!ELSE 
!MESSAGE Warning: cannot find "collectconvertor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "collectconvertor - Win32 Release" || "$(CFG)" == "collectconvertor - Win32 Debug"
SOURCE=.\convert\5ess\5ess_collect_convert.cpp

"$(INTDIR)\5ess_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\5ess\5ess_convert_fun.cpp

"$(INTDIR)\5ess_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\5ess\5ess_prefix_collect_convert.cpp

"$(INTDIR)\5ess_prefix_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\com\cc08_convert_fun.cpp

"$(INTDIR)\cc08_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\collect_convert_base.cpp

"$(INTDIR)\collect_convert_base.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\collect_convert_creator.cpp

"$(INTDIR)\collect_convert_creator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_convert_obj.cpp

"$(INTDIR)\collect_convert_obj.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\collect_convert_obj_factory.cpp

"$(INTDIR)\collect_convert_obj_factory.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\convert\collect_convert_subobj.cpp

"$(INTDIR)\collect_convert_subobj.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\collect_convertor.cpp

"$(INTDIR)\collect_convertor.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\convert\com\com_collect_convert.cpp

"$(INTDIR)\com_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\com\com_dir_collect_convert.cpp

"$(INTDIR)\com_dir_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\comm_convert_fun.cpp

"$(INTDIR)\comm_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\ewsd\ewsd_ama_collect_convert.cpp

"$(INTDIR)\ewsd_ama_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\ewsd\ewsd_amas_collect_convert.cpp

"$(INTDIR)\ewsd_amas_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\ewsd\ewsd_convert_fun.cpp

"$(INTDIR)\ewsd_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\ewsd\ewsd_mm_collect_convert.cpp

"$(INTDIR)\ewsd_mm_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\s1240\s1240_collect_convert.cpp

"$(INTDIR)\s1240_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\s1240\s1240_convert_fun.cpp

"$(INTDIR)\s1240_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\s1240\s1240_gb_convert_fun.cpp

"$(INTDIR)\s1240_gb_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\s1240\s1240_tp_collect_convert.cpp

"$(INTDIR)\s1240_tp_collect_convert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\com\tellin_convert_fun.cpp

"$(INTDIR)\tellin_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\convert\com\zte_convert_fun.cpp

"$(INTDIR)\zte_convert_fun.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

