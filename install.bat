pushd %~dp0

::call "%VS120 ...."
set CMAKE_BIN_PATH=%programfiles%\CMake\bin
call cmake_location.bat
set PATH=%PATH%; %CMAKE_BIN_PATH%

mkdir build
mkdir build/bat_build_lib
cd build/bat_build_lib

::echo. Version%%~nxa.txt
for /f "tokens=*" %%J in (../../.memlib_compile_flags.txt) do
(
    for /f "tokens=*" %%I in (../../.memlib_build_flags.txt) do
    (
        cmake %%I %%J -G"Visual Studio 12 2013 Win64" ../..
    )
)


msbuild "nq_memlib" /p:Configuration=Release /p:Patform=x64 /m

cd ../..

popd
