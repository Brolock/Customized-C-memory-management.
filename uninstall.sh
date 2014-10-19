#!/bin/sh

#check if the user is root/sudo
if [ $(whoami) != "root" ]; then
    echo "Sorry this uninstall needs you to be root"; exit 1
fi

rm -rf /usr/local/lib/nq_memlib
rm -rf /usr/local/include/nq_memlib

rm -rf lib;
rm -rf tests/bin;
