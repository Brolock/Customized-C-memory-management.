#!/bin/sh

#create the directory in local for the mem install
sudo mkdir /usr/local/lib/nq_memlib
sudo mkdir /usr/local/include/nq_memlib

sudo mkdir build; cd build
for i in "-DCOMPILE_WITH_LOG=1" "-DCOMPILE_WITH_LOG=1 -DLOG_WITH_TIME=1"\
    "-DCOMPILE_WITH_LOG=0"
do
    for j in "-DCMAKE_BUILD_TYPE=RELEASE" "-DCMAKE_BUILD_TYPE=DEBUG" \
	"-DCMAKE_BUILD_TYPE=RELWITHDEBINFO" "-DCMAKE_BUILD_TYPE=MINSIZEREL"
    do
	    cmake $i $j -G"Unix Makefiles" ..
	    make
    done
done
sudo make install

sudo mkdir ../tests/build
cd ../tests/build/
sudo cmake -DCOMPILE_WITH_LOG=1 -DLOG_WITH_TIME=1 -DCMAKE_BUILD_TYPE=RELEASE \
            -G"Unix Makefiles" ../;
make;
cd ../..; sudo rm -rf build tests/build
