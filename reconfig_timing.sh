#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/../locality_runtime \
CC=/usr/local/tapir/build/bin/clang \
CXX=/usr/local/tapir/build/bin/clang++ \
CFLAGS='-g -O3 -DSCHED_STATS' CXXFLAGS='-g -O3 -DSCHED_STATS' \
LIBS="-lpthread -lrt -lm -ldl"
