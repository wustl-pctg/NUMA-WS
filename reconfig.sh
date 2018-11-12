#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/../locality_runtime \
CC=/usr/local/tapir/build/bin/clang \
CXX=/usr/local/tapir/build/bin/clang++ \
CFLAGS='-O3' \
CXXFLAGS='-O3' \
LDFLAGS='' \
LIBS='-lpthread -lrt -lm -ldl'
