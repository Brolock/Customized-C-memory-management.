#! /bin/bash

#check if the user is root
if [ $(whoami) != "root" ];then
    echo "Sorry this test needs you to be root"; exit 1
fi

cd $(dirname $0);

#Tests build

cat ../.memlib_compile_flags.txt | while read log_flags
do
    mkdir build; cd build
    cat ../../.memlib_build_flags.txt | while read build_flags
    do
        echo "COMPILING WITH $log_flags $build_flags"
	    cmake $log_flags $build_flags -G"Unix Makefiles" ..
	    make
    done
    cd ..; rm -rf build
done


#Tests script
nb_file=0
nb_passed=0
for file in bin/*; do
    echo -n "Testing $file ..."
    
    if ./$file > /dev/null; then
        echo -e "$file \e[0;32m Succesfuly passed! \e[0m"
        nb_passed=$(($nb_passed + 1))
    else
        echo -e "$file \e[0;31m FAILED! \e[0m"
    fi
    nb_file=$(($nb_file + 1))
done

if [ $nb_file -eq $nb_passed ]; then
    echo -e "\e[0;32m ALL TESTS SUCCESFULY PASSED!\
        $nb_passed / $nb_file passed!(^-^)v=v~ \e[0m"
else
    echo -e "\e[0;31m TESTS FAILED!\
        Only $nb_passed / $nb_file passed! (-.-)v=v-- \e[0m"
fi
