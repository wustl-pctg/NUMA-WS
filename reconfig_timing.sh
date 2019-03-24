#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/../locality_runtime \
CC=/project/adams/home/j.deters/sandbox/tapir/build/bin/clang \
CXX=/project/adams/home/j.deters/sandbox/tapir/build/bin/clang++ \
CFLAGS='-O3 -DSCHED_STATS' \
CXXFLAGS='-O3 -DSCHED_STATS' \
LIBS='-lpthread -lrt -lm -ldl'
