#!/bin/sh

#check if the user is root/sudo
if [ $(whoami) != "root" ];then
    echo "Sorry this install needs you to be root"; exit 1
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


mkdir ../tests/build
cd ../tests/build/
cmake -DCOMPILE_WITH_LOG=1 -DLOG_WITH_TIME=1 -DCMAKE_BUILD_TYPE=RELEASE \
            -G"Unix Makefiles" ../;
make;
cd ../..; rm -rf build tests/build
