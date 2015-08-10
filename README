This version of runtime should be used in combination with the WUSTL version
of the LLVM Cilk Plus comiler in llvm-cilk / clang-cilk.

The main difference is that, this runtime implements __cilkrts_detach and 
has a set of empty functions for the instrumentations used by cilktool.

Support for using __cilkrts_detach implemeneted in the runtime
==============================================================

When compile the user application with -fno-inline-detach, the compiled code
invokes __cilkrts_detach instead of inling it.  Thus, the version implemented 
in the runtime will be invoked.

In addition, by using LTO (linked-time optimization), one can get the compiler 
to also inline the detach implemented by the runtime.

To allow such inlining, one needs to enable LTO, which requires a few additional steps:

- get GNU gold linker (either via rpm package or install from the new binutils source)
- configure and comile llvm-cilk with gold linker support 
  (see README.md for llvm-cilk)
- if llvm-cilk is configured with gold linker support, you will find 
  LLVMgold.so in the lib/ directory where llvm-cilk s installed.
- compile the runtime into LLVM bitcode in the form of a static library, 
  and link the static library with the user application comiled with 
  -fno-inline-detach.

To allow such a static library to be compiled, one should configurate the
runtime as follows:

% libtoolize
% aclocal
% automake --add-missing
% autoconf
% ./configure \
%   --prefix=<where you want to install> \
%   CC=clang CXX=clang++ \
%   CFLAGS="-flto -fno-inline-detach" \
%   CXXFLAGS="-flto -fno-inline-detach" \
%   AR_FLAGS="cru --plugin=<the installation of LLVMgold.so>
% make
% make install

Note that the -flto and -fno-inline-detach flags are necessary even when
compiling the runtime, because there are spawn / sync statements used in the
runtime to implement cilk_for.
                     
The, compile the user app similarly and link with static library libcilkrts.a.
(Note that dynamic linking won't inline detach.)


Support for Cilktools:
======================

When compile the user application with -fcilktool, a set of instrumentations 
will be inserted (see cilkplus_rts/runtime/cilktool.h).  By default, the
runtime implements these instrumentations as empty calls, so as to allow
linking with user applications compiled with -fcilktool.  

If you are implementing your own version of tool, you would also want to 
compile the runtime itself with -fcilktool (since the runtime implements
cilk_for using cilk_spawn / cilk_sync, and those statements need to be
compiled with instrumentation in order for the tool to work correctly on
cilk_for.

To add the -fcilktool flag to compilation, simply follow the same 
instruction above, with addtional CFLAGS in the configure step:

% ./configure \
%   --prefix=<where you want to install> \
%   CC=clang CXX=clang++ \
%   CFLAGS="-flto -fcilktool" \
%   CXXFLAGS="-flto -fcilktool" \
%   AR_FLAGS="cru --plugin=<the installation of LLVMgold.so>

(The -flto and AR_FLAGS are necessary to support LTO.)

Then, link the created static library (libcilkrts.a) with the user app.


If you have configured the runtime previously, do

% make maintainer-clean 

to remove the Makefile and configuration files from previous install.
