#! /bin/bash

cd $(dirname $0);

#Tests build
mkdir build
cd build/

cat ../../.memlib_compile_flags.txt | while read log_flags
do
    cat ../../.memlib_build_flags.txt | while read build_flags
    do
	    cmake $log_flags $build_flags -G"Unix Makefiles" ..
	    make
    done
done
cd ..; rm -rf build


#Tests script
nb_file=0
nb_passed=0
for file in bin/*; do
    echo -n "Testing $file ..."
    
    if ./$file > /dev/null; then
        echo -e "\e[0;32m Succesfuly passed! \e[0m"
        nb_passed=$(($nb_passed + 1))
    else
        echo -e "\e[0;31m FAILED! \e[0m"
    fi
    nb_file=$(($nb_file + 1))
done

if [ $nb_file -eq $nb_passed ]; then
    echo -e "\e[0;32m ALL TESTS SUCCESFULY PASSED! (^-^)v=v~ \e[0m"
else
    echo -e "\e[0;31m TEST FAILED!\
        Only $nb_passed / $nb_file passed! (-.-)v=v- \e[0m"
fi
