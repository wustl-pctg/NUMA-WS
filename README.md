# NUMA-WS

These are a series of benchmarks to test and demonstrate the capability of the
NUMA-WS runtime system. Read more [here](https://ieeexplore.ieee.org/document/8573486). You
can find the companion benchmarks [here](https://github.com/wustl-pctg/NUMA-WS-Benchmarks).

##  Using the runtime
Currently, the NUMA aware runtime calls need to be added to programs *by hand*.

## Getting Started
We recommend using [Tapir](http://cilk.mit.edu/download/) to compile your programs
to work with this runtime.

1. Clone this repository to the machine you wish to install it on.
2. Run one of the reconfigure scrips depending on your use case.
    1. reconfigure.sh: completely vanilla instalation
    2. reconfigure_timing.sh: provides runtime statistics
    3. reconfigure_likwid.sh: provides the ability to use the LIKWID profiler.
4. make
5. make install

**Note:** NUMA-WS assumes an 8 core machine. Change CORES_PER_SOCKET in /include/cilk/common.h to adapt it to your machine.

## Locality Paramaters added to this runtime
- CILK_NUM_SOCKETS is the number of sockets the computation will run on.
- CILK_WORKERS_PER_SOCKET is the number of workers on each socket.
- CILK_LOCAL_PERCENT is the percent chance that the steal will be on the local socket.
- CILK_NEIGHBOR_PERCENT is the percent chance that the steal will be on an immediate neighbor.
- CILK_REMOTE_PERCENT is the percent chance that the steal will be on the remote socket.
- CILK_MAX_NONLOCAL_STEAL_ATTEMPTS is the number of times a worker can try to steal a frame
before is stops being pushed back.

ex: CILK_NUM_SOCKETS=1 CILK_WORKERS_PER_SOCKET=8 CILK_NWORKERS=8 ./foo

## New runtime calls for locality
- __cilkrts_set_pinning_info(int) sets the continuation with a socket to be pushed to.
- __cilkrts_unset_pinning_info() unsets all pinning info for the continuation.
- __cilkrts_pin_top_level_frame_at_socket() allows for the top level frame to have pinning information.

## Cititation
Please use the following citiation when using this runtime in your work:

J. Deters, J. Wu, Y. Xu and I. Lee, "A NUMA-Aware Provably-Efficient Task-Parallel Platform Based on the Work-First Principle", 
2018 IEEE International Symposium on Workload Characterization (IISWC), 2018. 
Available: https://ieeexplore.ieee.org/document/8573486.

## Acknowledgment
This research was supported in part by National Science Foundation under grant number CCF-1527692 and CCF-1733873.

Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) 
and do not necessarily reflect the views of the National Science Foundation.
