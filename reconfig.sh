#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/.. \
CC=/usr/local/tapir/build/bin/clang \
CXX=/usr/local/tapir/build/bin/clang++ \
CFLAGS='-g' CXXFLAGS='-g'
