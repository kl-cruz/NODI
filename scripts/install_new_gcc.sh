#!/bin/bash

LINK=`curl https://launchpad.net/gcc-arm-embedded/ | grep -o -e 'https://launchpad.net/gcc-arm-embedded/.\{1,200\}\linux.tar.bz2'`
mkdir env
cd env
wget $LINK -O gcc.tar.bz2
if [ -d "toolchain" ]; then
  rm -r -f toolchain
fi
tar -xvf gcc.tar.bz2
mv gcc-arm-none-eabi-* toolchain
rm gcc.tar.bz2
