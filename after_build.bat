@echo off
echo +++ Running after_build.bat +++
if "%proj_platform%"=="x86" (
mkdir "/%prof_platform%/%proj_configuration%"
xcopy "/%proj_configuration%" "/%proj_platform%/%proj_configuration%" /s /e
)
