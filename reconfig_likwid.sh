#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/.. \
CC=/usr/local/tapir/build/bin/clang \
CXX=/usr/local/tapir/build/bin/clang++ \
CFLAGS="-g -O3 -DLIKWID_PERFMON"
CXXFLAGS="-g O3 -DLIKWID_PERFMON"
LDFLAGS="-L/usr/local/lib -Wl,-rpath -Wl,/usr/local/lib"
LIBS="-lcilkrts -lrt -lm -ldl -llikwid"
