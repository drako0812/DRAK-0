@echo +++ Running after_build.bat +++
if "%proj_platform%"=="x86" goto copy
goto quit

:copy
mkdir "C:\projects\DRAK-0\x86\%proj_configuration%"
xcopy "C:\projects\DRAK-0\%proj_configuration%" "C:\projects\DRAK-0\x86\%proj_configuration%" /s /e /y

:quit
