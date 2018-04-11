#include <cilk/common.h>
#include "rts-common.h"
#include "internal/abi.h"

#define SCHED_STATS 1 

#ifdef SCHED_STATS
#define NUMBER_OF_STATS 3

__CILKRTS_BEGIN_EXTERN_C

enum timing
{
        INTERVAL_SCHED, //Scheduler Overhead
            INTERVAL_WORK_INFLATION, //Work inflation measurement
                INTERVAL_IDLE, // Idle time
};

typedef struct sched_stats
{
        volatile uint64_t time[NUMBER_OF_STATS]; // Total time measured for all stats

            volatile uint64_t begin[NUMBER_OF_STATS]; // Begin time of current measurement

                volatile uint64_t end[NUMBER_OF_STATS];  // End time of current measurement
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
void __cilkrts_accum_timings(__cilkrts_worker *w);

COMMON_PORTABLE
void __cilkrts_drop_timing(__cilkrts_worker *w, enum timing i);

COMMON_PORTABLE
void __cilkrts_stop_timing(__cilkrts_worker *w, enum timing i);

# define START_TIMING(w, i) __cilkrts_start_timing(w, i);
# define STOP_TIMING(w, i) __cilkrts_stop_timing(w, i);
# define DROP_TIMING(w, i) __cilkrts_drop_timing(w, i);

#else
# define START_TIMING(w, i)
# define STOP_TIMING(w, i)
# define DROP_TIMING(w, i)

#endif
__CILKRTS_END_EXTERN_C
