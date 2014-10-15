!#/bin/sh

mkdir build;
cd build;
cmake -DCOMPILE_WITH_LOG=1 -DLOG_WITH_TIME=1 -DCMAKE_BUILD_TYPE=RELEASE \
            -G"Unix Makefiles" ../;
#cmake -DCMAKE_BUILD_TYPE=DEBUG -G"Unix Makefiles" ../;
make
mkdir ../tests/build
cd ../tests/build/
cmake -DCOMPILE_WITH_LOG=1 -DLOG_WITH_TIME=1 -DCMAKE_BUILD_TYPE=RELEASE \
            -G"Unix Makefiles" ../;
#cmake -DCMAKE_BUILD_TYPE=DEBUG -G"Unix Makefiles" ../;
make;
cd ../..;
rm -rf build tests/build
