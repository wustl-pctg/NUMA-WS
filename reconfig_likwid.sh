#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/../locality_runtime \
CC=/usr/local/tapir/build/bin/clang \
CXX=/usr/local/tapir/build/bin/clang++ \
CFLAGS='-O3 -DLIKWID_PERFMON' \
CXXFLAGS='-O3 -DLIKWID_PERFMON' \
LDFLAGS='-L/usr/local/lib -Wl,-rpath -Wl,/usr/local/lib' \
LIBS='-lpthread -lrt -lm -ldl -llikwid'
