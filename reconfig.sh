#!/bin/bash
libtoolize
aclocal
automake --add-missing
autoconf
./configure --prefix=`pwd`/.. \
CC=/usr/local/llvm-cilk/bin/clang \
CXX=/usr/local/llvm-cilk/bin/clang++ \
CFLAGS='-g -O3 -DSCHED_STATS' CXXFLAGS='-g -O3 -DSCHED_STATS' \
LIBS="-lpthread -lrt -lm -ldl"
