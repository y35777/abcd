@echo off

setlocal

rem /****** ���ڴ�ָ��ACE��XML��OSS��·�� ******/
set ACE_ROOT=c:\ACE\
set XML_ROOT=D:\xerces-c_2_3_0-win32
set OSSINFO =%OSSINFO%
rem /*******************************************/

if not exist %ACE_ROOT%\VERSION (
    echo �Ҳ����ļ� %ACE_ROOT%\VERSION
    echo ���޸��������ļ��е�ACE_ROOT������ʹָ֮����ȷ��ACEĿ¼
    GOTO ALLEND
)

if not exist %XML_ROOT%\version.incl (
    echo �Ҳ����ļ� %XML_ROOT%\version.incl
    echo ���޸��������ļ��е�XML_ROOT������ʹָ֮����ȷ��XMLĿ¼
    GOTO ALLEND
)

if not exist %OSSINFO%\ossinfo (
    echo �Ҳ����ļ� %OSSINFO%\ossinfo
    echo ���޸��������ļ��е�OSSINFO������ʹָ֮����ȷ��OSSĿ¼
    GOTO ALLEND
)

if not exist %~dp0com\include\base_type.h (
    echo �Ҳ���Ŀ¼ %~dp0com\include\base_type.h
    echo �뽫���������ķŵ�igwb�ĸ�Ŀ¼������
    GOTO ALLEND
)

rem ��������еĵ�һ������
IF /i "%1" == "ccs" (
    cd %~dp0com\include
    del /F /Q product.h
    copy product_ccs.h product.h
    IF ERRORLEVEL 1 GOTO COMERROR
    GOTO CHECK_PARA_2
)
IF /i "%1" == "ngn" (
    cd %~dp0com\include
    del /F /Q product.h
    copy product_ngn.h product.h
    IF ERRORLEVEL 1 GOTO COMERROR
    GOTO CHECK_PARA_2
)
IF /i "%1" == "cg" (
    cd %~dp0com\include
    del /F /Q product.h
    copy product_cg.h product.h
    IF ERRORLEVEL 1 GOTO COMERROR
    GOTO CHECK_PARA_2
)
IF /i "%1" == "moto" (
    cd %~dp0com\include
    del /F /Q product.h
    copy product_moto.h product.h
    IF ERRORLEVEL 1 GOTO COMERROR
    GOTO CHECK_PARA_2
)
GOTO PARA_ERR

rem ��������еĵڶ�������
:CHECK_PARA_2
IF /i "%2" == "Debug" (
    GOTO BEGIN
)
IF /i "%2" == "Release" (
    GOTO BEGIN
)
IF /i "%2" == "Clean" (
    GOTO BEGIN
)

rem �����в�������
:PARA_ERR

echo.
echo igwb�Զ������������ļ�
echo.
echo Error:����ı��￪�ز���ȷ
echo.
echo Usage:
echo       vmake [ccs^|ngn^|cg^|moto] [debug^|release^|clean]
echo product:
echo       ccs          ����ccs�汾
echo       ngn          ����ngn�汾
echo       cg           ����cg�汾
echo       moto         ����moto�汾
echo Options:
echo       debug        ����debug�汾
echo       release      ����release�汾
echo       clean        ���debug�汾��release�汾�����б�������
GOTO ALLEND

rem ��ʼ����
:BEGIN

set BakInc=%INCLUDE%
set BakLib=%LIB%
set INCLUDE=%~dp0;%~dp0com\include;%ACE_ROOT%;%OSSINFO%\include;%XML_ROOT%\include;%BakInc%
set LIB_TMP=%ACE_ROOT%\ACE;%XML_ROOT%\lib;%BakLib%

if not exist %~dp0com\lib\debug mkdir %~dp0com\lib\debug
if not exist %~dp0com\lib\release mkdir %~dp0com\lib\release
if not exist %~dp0output mkdir %~dp0output
if not exist %~dp0output\debug mkdir %~dp0output\debug
if not exist %~dp0output\release mkdir %~dp0output\release

IF /i "%2" == "Debug" (
    set LIB=%~dp0com\lib\;%~dp0com\lib\debug;%LIB_TMP%;
)
IF /i "%2" == "Release" (
    set LIB=%~dp0com\lib\;%~dp0com\lib\release;%LIB_TMP%;
)

rem �ȱ������̬��
cd %~dp0com
for /F %%d in (%~dp0vc-dll.txt) do (
    cd %~dp0com\%%d
    for %%f in (*.mak) do (
        IF /i "%2" == "Debug" (
            echo ����%%f
            nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug"
            IF ERRORLEVEL 1 GOTO COMERROR
            if exist %~dp0com\%%d\debug\*.lib move %~dp0com\%%d\debug\*.lib %~dp0com\lib\debug
        )
        IF /i "%2" == "Release" (
            echo ����%%f
            nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release"
            IF ERRORLEVEL 1 GOTO COMERROR
            IF EXIST %~dp0com\%%d\release\*.lib move %~dp0com\%%d\release\*.lib %~dp0com\lib\release
        )
        IF /i "%2" == "Clean" (
            echo ���%%f
            if exist %~dp0com\%%d\debug (
               nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug" /e CLEAN
               del /S /F /Q %~dp0com\%%d\debug
               rmdir %~dp0com\%%d\debug
            )
            if exist %~dp0com\%%d\release (
               nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release" /e CLEAN
               del /S /F /Q %~dp0com\%%d\release
               rmdir %~dp0com\%%d\release
            )
        )
    )
)

cd %~dp0com
for /D %%d in (*.*) do (
    set FILTER=disable
    cd %~dp0com\%%d
    for %%f in (*.mak) do (
        IF /i "%1" == "ccs" (
            for /F %%e in (%~dp0filter-ccs.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF /i "%1" == "ngn" (
            for /F %%e in (%~dp0filter-ngn.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF /i "%1" == "cg" (
            for /F %%e in (%~dp0filter-cg.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF /i "%1" == "moto" (
            for /F %%e in (%~dp0filter-moto.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF not exist .\vmake.bat (
            IF /i "%2" == "Debug" (
                echo ����%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug"
                IF ERRORLEVEL 1 GOTO COMERROR
                if exist %~dp0com\%%d\debug\*.lib move %~dp0com\%%d\debug\*.lib %~dp0com\lib\debug
            )
            IF /i "%2" == "Release" (
                echo ����%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release"
                IF ERRORLEVEL 1 GOTO COMERROR
                IF EXIST %~dp0com\%%d\release\*.lib move %~dp0com\%%d\release\*.lib %~dp0com\lib\release
            )
            IF /i "%2" == "Clean" (
                echo ���%%f
                if exist %~dp0com\%%d\debug (
                   nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug" /e CLEAN
                   del /S /F /Q %~dp0com\%%d\debug
                   rmdir %~dp0com\%%d\debug
                )
                if exist %~dp0com\%%d\release (
                   nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release" /e CLEAN
                   del /S /F /Q %~dp0com\%%d\release
                   rmdir %~dp0com\%%d\release
                )
            )
        )
    )
)
cd %~dp0com

echo end
IF /i "%2" == "Debug" (
    move %~dp0com\lib\debug\*.dll %~dp0output\debug
)
IF /i "%2" == "Release" (
    move %~dp0com\lib\release\*.dll %~dp0output\release
)

cd %~dp0product
for /D %%d in (*.*) do (
    cd %~dp0product\%%d
    for %%f in (*.mak) do (
        IF /i "%1" == "ccs" (
            for /F %%e in (%~dp0filter-ccs.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF /i "%1" == "ngn" (
            for /F %%e in (%~dp0filter-ngn.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF /i "%1" == "cg" (
            for /F %%e in (%~dp0filter-cg.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF /i "%1" == "moto" (
            for /F %%e in (%~dp0filter-moto.txt) do (
                IF /i %%d == %%e cd %~dp0
            )
        )
        IF not exist .\vmake.bat (
            IF /i "%2" == "Debug" (
                echo ����%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug"
                IF ERRORLEVEL 1 GOTO COMERROR
                move debug\*.exe %~dp0output\debug
            )
            IF /i "%2" == "Release" (
                echo ����%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release"
                IF ERRORLEVEL 1 GOTO COMERROR
                move release\*.exe %~dp0output\release
            )
            IF /i "%2" == "Clean" (
                echo ���%%f
                if exist %~dp0product\%%d\debug (
                   nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug" /e Clean
                   del /S /F /Q %~dp0product\%%d\debug 
                   rmdir %~dp0product\%%d\debug 
                )
                if exist %~dp0product\%%d\release (
                   nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release" /e Clean
                   del /S /F /Q %~dp0product\%%d\release 
                   rmdir %~dp0product\%%d\release 
                )
            )
        )
    )
)
set INCLUDE=%BakInc%;
set LIB=%BakLib%;
cd %~dp0
echo ����ɹ�
echo.
IF /i "%2" == "Debug" (
    echo ע�⣺������������%~dp0output\debugĿ¼��
)
IF /i "%2" == "Release" (
    echo ע�⣺������������%~dp0output\releaseĿ¼��
)
echo.
GOTO ALLEND

rem �������
:COMERROR
set INCLUDE=%BakInc%;
set LIB=%BakLib%;
cd %~dp0
echo �������
GOTO ALLEND

rem ���
:ALLEND

