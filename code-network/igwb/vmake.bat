@echo off

setlocal

rem /****** 请在此指定ACE、XML、OSS的路径 ******/
set ACE_ROOT=c:\ACE\
set XML_ROOT=D:\xerces-c_2_3_0-win32
set OSSINFO =%OSSINFO%
rem /*******************************************/

if not exist %ACE_ROOT%\VERSION (
    echo 找不到文件 %ACE_ROOT%\VERSION
    echo 请修改批处理文件中的ACE_ROOT变量，使之指向正确的ACE目录
    GOTO ALLEND
)

if not exist %XML_ROOT%\version.incl (
    echo 找不到文件 %XML_ROOT%\version.incl
    echo 请修改批处理文件中的XML_ROOT变量，使之指向正确的XML目录
    GOTO ALLEND
)

if not exist %OSSINFO%\ossinfo (
    echo 找不到文件 %OSSINFO%\ossinfo
    echo 请修改批处理文件中的OSSINFO变量，使之指向正确的OSS目录
    GOTO ALLEND
)

if not exist %~dp0com\include\base_type.h (
    echo 找不到目录 %~dp0com\include\base_type.h
    echo 请将该批处理文放到igwb的根目录中运行
    GOTO ALLEND
)

rem 检查命令行的第一个参数
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

rem 检查命令行的第二个参数
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

rem 命令行参数错误
:PARA_ERR

echo.
echo igwb自动编绎批处理文件
echo.
echo Error:输入的编绎开关不正确
echo.
echo Usage:
echo       vmake [ccs^|ngn^|cg^|moto] [debug^|release^|clean]
echo product:
echo       ccs          编绎ccs版本
echo       ngn          编绎ngn版本
echo       cg           编绎cg版本
echo       moto         编绎moto版本
echo Options:
echo       debug        编绎debug版本
echo       release      编绎release版本
echo       clean        清除debug版本及release版本的所有编绎内容
GOTO ALLEND

rem 开始编绎
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

rem 先编基础动态库
cd %~dp0com
for /F %%d in (%~dp0vc-dll.txt) do (
    cd %~dp0com\%%d
    for %%f in (*.mak) do (
        IF /i "%2" == "Debug" (
            echo 生成%%f
            nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug"
            IF ERRORLEVEL 1 GOTO COMERROR
            if exist %~dp0com\%%d\debug\*.lib move %~dp0com\%%d\debug\*.lib %~dp0com\lib\debug
        )
        IF /i "%2" == "Release" (
            echo 生成%%f
            nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release"
            IF ERRORLEVEL 1 GOTO COMERROR
            IF EXIST %~dp0com\%%d\release\*.lib move %~dp0com\%%d\release\*.lib %~dp0com\lib\release
        )
        IF /i "%2" == "Clean" (
            echo 清除%%f
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
                echo 生成%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug"
                IF ERRORLEVEL 1 GOTO COMERROR
                if exist %~dp0com\%%d\debug\*.lib move %~dp0com\%%d\debug\*.lib %~dp0com\lib\debug
            )
            IF /i "%2" == "Release" (
                echo 生成%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release"
                IF ERRORLEVEL 1 GOTO COMERROR
                IF EXIST %~dp0com\%%d\release\*.lib move %~dp0com\%%d\release\*.lib %~dp0com\lib\release
            )
            IF /i "%2" == "Clean" (
                echo 清除%%f
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
                echo 生成%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Debug"
                IF ERRORLEVEL 1 GOTO COMERROR
                move debug\*.exe %~dp0output\debug
            )
            IF /i "%2" == "Release" (
                echo 生成%%f
                nmake /NOLOGO /f %%f CFG="%%~nf - Win32 Release"
                IF ERRORLEVEL 1 GOTO COMERROR
                move release\*.exe %~dp0output\release
            )
            IF /i "%2" == "Clean" (
                echo 清除%%f
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
echo 编译成功
echo.
IF /i "%2" == "Debug" (
    echo 注意：编译结果放置在%~dp0output\debug目录中
)
IF /i "%2" == "Release" (
    echo 注意：编译结果放置在%~dp0output\release目录中
)
echo.
GOTO ALLEND

rem 编译出错
:COMERROR
set INCLUDE=%BakInc%;
set LIB=%BakLib%;
cd %~dp0
echo 编译错误
GOTO ALLEND

rem 完成
:ALLEND

