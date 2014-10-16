#!/bin/sh

mkdir build; cd build
for i in "-DCOMPILE_WITH_LOG=1" "-DCOMPILE_WITH_LOG=1 -DLOG_WITH_TIME=1"
do
    for j in "-DCMAKE_BUILD_TYPE=RELEASE" "-DCMAKE_BUILD_TYPE=DEBUG" \
	"-DCMAKE_BUILD_TYPE=RELWITHDEBINFO" "-DCMAKE_BUILD_TYPE=MINSIZEREL"
    do
	    cmake $i $j -G"Unix Makefiles" ..
	    make
    done
done

mkdir ../tests/build
cd ../tests/build/
cmake -DCOMPILE_WITH_LOG=1 -DLOG_WITH_TIME=1 -DCMAKE_BUILD_TYPE=RELEASE \
            -G"Unix Makefiles" ../;
make;
cd ../..; rm -rf build tests/build
