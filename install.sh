#!/bin/sh

#check if the user is root
if [ $(whoami) != "root" ];then
    echo "Sorry this install needs you to be root"; exit 1
fi

#check if user have Cmake installed
if !(which cmake > /dev/null cmake 2> /dev/null); then
    echo "Sorry you need to install cmake"; exit 1
fi

#create the directory in local for the mem install
mkdir /usr/local/lib/nq_memlib
mkdir /usr/local/include/nq_memlib

mkdir build; cd build

cat ../.memlib_compile_flags.txt | while read log_flags
do
    cat ../.memlib_build_flags.txt | while read build_flags
    do
	    cmake $log_flags $build_flags -G"Unix Makefiles" ..
	    make
    done
done
make install

cd ..; rm -rf build
