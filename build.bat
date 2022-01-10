@echo off

echo Building using GCC...

REM Try to use GCC compiler, if available.

gcc -m64 -Wall "%~dp0src\cphookloader64.c" "%~dp0src\memtools.c" -o "%~dp0cphookloader64.exe"

echo;

if %errorlevel%==9009 echo GCC is not installed or PATH not properly set! & echo; & goto msvc

if %errorlevel% NEQ 0 ( echo Building failed... & goto end ) else ( echo; & echo Build OK... & goto end )


REM If GCC is not installed, try MSVC compiler

:msvc

echo Building using MSVC compiler... & echo;

cl "%~dp0src\cphookloader64.c" "%~dp0src\memtools.c" /Fo:"%~dp0\" /link /out:"%~dp0cphookloader64.exe"

echo;

if %errorlevel%==9009 echo MSVC compiler is not installed or PATH not properly set! & goto end

echo;

if %errorlevel% NEQ 0 (echo; & echo Building failed... & echo;) else (echo; & echo Build OK... & echo;)

REM Clean object files

if exist "%~dp0*.obj" (
echo Deleting object files... 
del "%~dp0\*.obj" 
)

:end

echo;
pause
exit