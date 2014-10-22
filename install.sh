#!/bin/sh

#check if the user is root
if [ $(whoami) != "root" ];then
    echo "Sorry this install needs you to be root"; exit 1
fi

cd $(dirname $0);

#check if user have Cmake installed
if !(which cmake > /dev/null cmake 2> /dev/null); then
    echo "Sorry you need to install cmake"; exit 1
fi

#create the directory in local for the mem install
mkdir /usr/local/lib/nq_memlib
mkdir /usr/local/include/nq_memlib

cat .memlib_compile_flags.txt | while read log_flags
do
    #cmake does not ALWAYS recompile the new flags if they're not changing
    mkdir build; cd build
    cat ../.memlib_build_flags.txt | while read build_flags
    do
        echo "COMPILING WITH $log_flags $build_flags"
	    cmake $log_flags $build_flags -G"Unix Makefiles" ..
	    make
    done
    #cmake hack
    cd ..; rm -rf build
done

#Since I'm removing build on the loop, need to recreate it for install
mkdir build; cd build
cmake -G"Unix Makefiles" ..
make install
cd ..; rm -rf build
