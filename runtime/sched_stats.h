#include <cilk/common.h>

#include "rts-common.h"
#include "internal/abi.h"

#ifndef __SCHED_STATS_HEADER__
#define __SCHED_STATS_HEADER__

__CILKRTS_BEGIN_EXTERN_C

#ifdef SCHED_STATS
enum timing
{
    INTERVAL_SCHED = 0, //Scheduler Overhead
    INTERVAL_WORK_INFLATION, //Work inflation measurement
    INTERVAL_IDLE, // Idle time
    INTERVAL_WORK_LOCAL, // time spent working on local items
    INTERVAL_WORK_REMOTE, // time spent working on remote items
    INTERVAL_WORK_UNPINNED, // time spent working on remote items
    NUMBER_OF_STATS // always keep this as the last
};

typedef struct sched_stats
{
    enum timing last_user_code_op; // type of frame this worker set up last
    uint32_t total_steals; // total number of steals including the following two
    uint32_t successful_steals_pushed; // steals that results work pushed
    uint32_t successful_steals; // steal that results work
    uint64_t time[NUMBER_OF_STATS]; // Total time measured for all stats
    uint64_t begin[NUMBER_OF_STATS]; // Begin time of current measurement
    uint64_t end[NUMBER_OF_STATS];  // End time of current measurement
} stats;

typedef struct total_stats
{
    double time[NUMBER_OF_STATS]; // Total time measured for all stats

    double begin[NUMBER_OF_STATS]; // Begin time of current measurement

    double end[NUMBER_OF_STATS];  // End time of current measurement

} total;

COMMON_PORTABLE 
void __cilkrts_init_sched_stats(stats *s);

COMMON_PORTABLE
void __cilkrts_init_total_stats(total *s);

COMMON_PORTABLE
void __cilkrts_start_timing(__cilkrts_worker *w, enum timing i);

COMMON_PORTABLE
void __cilkrts_accum_timing();

COMMON_PORTABLE
void __cilkrts_drop_timing(__cilkrts_worker *w, enum timing i);

COMMON_PORTABLE
void __cilkrts_stop_timing(__cilkrts_worker *w, enum timing i);

COMMON_PORTABLE
void __cilkrts_reset_timing();

# define START_TIMING(w, i) __cilkrts_start_timing(w, i);
# define STOP_TIMING(w, i) __cilkrts_stop_timing(w, i);
# define DROP_TIMING(w, i) __cilkrts_drop_timing(w, i);

#else
# define START_TIMING(w, i)
# define STOP_TIMING(w, i)
# define DROP_TIMING(w, i)
#endif // SCHED_STATS

__CILKRTS_END_EXTERN_C
#endif // __SCHED_STATS_HEADER__
