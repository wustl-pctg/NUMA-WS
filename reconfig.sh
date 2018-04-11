#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/.. \
CC=/usr/local/tapir/build/bin/clang \
CXX=/usr/local/tapir/build/bin/clang++ \
CFLAGS='-g -O0' CXXFLAGS='-g -O0'
LIBS="-lcilkrts -lpthread -lrt -lm -ldl"
