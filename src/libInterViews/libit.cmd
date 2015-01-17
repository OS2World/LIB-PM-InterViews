@echo off
setlocal
rem Asif Malik
rem Computer Systems Lab
rem Texas Instruments
rem Dallas, Tx
rem
rem Lib utility

rem Get LIBRARY name
shift
if %0. == . goto end
set LIBNAME=%0

rem Get objects
set OBJTS=
:next
shift
if %0. == . goto finish
rem Append each object with a -+ for library operation.
set OBJTS=%OBJTS% -+%0
goto next

:finish
rem Put all objects in the LIBRARY.
echo LIB %LIBNAME% /PAGESIZE:64 %OBJTS%;
lib %LIBNAME% /PAGESIZE:64 %OBJTS%;


:end
rem Release evironment space
set LIBNAME=
set OBJTS=
endlocal
