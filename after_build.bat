@echo off
echo +++ Running after_build.bat +++
if "%proj_platform%"=="x86" goto copy
goto quit

:copy
mkdir "C:/projects/DRAK-0/%prof_platform%/%proj_configuration%"
xcopy "C:/projects/DRAK-0/%proj_configuration%" "C:/projects/DRAK-0/%proj_platform%/%proj_configuration%" /s /e

:exit
