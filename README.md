# NUMA-WS

These are a series of benchmarks to test and demonstrate the capability of the
NUMA-WS runtime system. Read more [here](https://arxiv.org/abs/1806.11128). You
can find the companion benchmarks [here](https://gitlab.com/wustl-pctg/numa-ws-benchmarks).

## Future Changes
Currently, the NUMA aware runtime calls need to be added to programs *by hand*.
In the future these runtime calls will be inserted at compile time and new
syntax will be introduced.

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
- __cilkrts_disable_nonlocal_steal() only allows for steals from the local socket.
- __cilkrts_enable_nonlocal_steal() allows for steals from outside the local socket.
- __cilkrts_pin_top_level_frame_at_socket() allows for the top level frame to have pinning information.

