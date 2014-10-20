pushd %~dp0

::call "%VS120 ...."
set CMAKE_BIN_PATH=%programfiles%\CMake\bin
call cmake_location.bat
set PATH=%PATH%; %CMAKE_BIN_PATH%

mkdir build
mkdir build/bat_build_lib
cd build/bat_build_lib

for /f "tokens=*" %a in (.memlib_build_flags.txt) do echo. Version%~nxa.txt
::cmake -G"Visual Studio 12 2013 Win64" ../..

::msbuild "nq_memlib" /p:Configuration=Release /p:Patform=x64 /m

cd ../..

popd
