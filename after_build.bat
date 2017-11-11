@echo +++ Running after_build.bat +++
if "%proj_platform%"=="x86" goto copy else goto x64del

:copy
mkdir "C:\projects\DRAK-0\x86\%proj_configuration%"
xcopy "C:\projects\DRAK-0\%proj_configuration%" "C:\projects\DRAK-0\x86\%proj_configuration%" /s /e /y
del /S "C:\projects\DRAK-0\x86\%proj_configuration%\*.iobj"
del /S "C:\projects\DRAK-0\x86\%proj_configuration%\*.ipdb"
del /S "C:\projects\DRAK-0\x86\%proj_configuration%\*.ilk"
del /S "C:\projects\DRAK-0\x86\%proj_configuration%\*.pdb"
goto quit

:x64del
del /S "C:\projects\DRAK-0\x64\%proj_configuration%\*.iobj"
del /S "C:\projects\DRAK-0\x64\%proj_configuration%\*.ipdb"
del /S "C:\projects\DRAK-0\x64\%proj_configuration%\*.ilk"
del /S "C:\projects\DRAK-0\x64\%proj_configuration%\*.pdb"

:quit
