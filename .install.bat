pushd %~dp0

call "%VS120COMNTOOLS%VsDevCmd.bat"

set CMAKE_BIN_PATH=%programfiles(x86)%\CMake\bin
call cmake_location.bat
set PATH=%PATH%; %CMAKE_BIN_PATH%

mkdir build
cd build
mkdir bat_build_lib
cd bat_build_lib

for /f "tokens=*" %%I in (../../.memlib_compile_flags.txt) do (
    cmake %%I -G"Visual Studio 12 2013 Win64" ../..
	msbuild "nq_memlib.sln" /p:Configuration=Release /p:Patform=x64 /m
	msbuild "nq_memlib.sln" /p:Configuration=RelWithDebInfo /p:Patform=x64 /m
	msbuild "nq_memlib.sln" /p:Configuration=Debug /p:Patform=x64 /m
	msbuild "nq_memlib.sln" /p:Configuration=MinSizeRel /p:Patform=x64 /m
	
	cd ..
	rmdir /S /Q bat_build_lib
	mkdir bat_build_lib
	cd bat_build_lib
)

cmake -G"Visual Studio 12 2013 Win64" ../..

cd ../..

popd
