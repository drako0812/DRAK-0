cd "C:\Tools\vcpkg"
git pull
call .\bootstrap-vcpkg.bat
if "%proj_platform%"=="x86" ( goto BIT86 ) else ( if "%proj_platform%"=="x64" ( goto BIT64 ) )

:BIT86
vcpkg install sfml:x86-windows
%comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
goto END

:BIT64
vcpkg install sfml:x64-windows
%comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
goto END

:END
cd "C:\projects\drak-0\deps\luajit-2.0\src"
.\msvcbuild.bat
cd "C:\projects\drak-0"
