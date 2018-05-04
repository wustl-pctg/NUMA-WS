#include "stats.h"
#include "bug.h"
#include "os.h"
#include "local_state.h"
#include "sched_stats.h"

#include <stdio.h>


#ifdef SCHED_STATS

#define EXTEND_INSTRUMENT

#define calculateCycle(high,low) (((uint64_t)high<<32) | low)

#define cycleToSecond(cycle) ((double)cycle/(double)2200000000.0)
#define cycleToMicroSecond(cycle) (cycle/2200)
#define MicroSecondToSecond(MicroSecond) ((double)MicroSecond/(double)1000000)

uint64_t beginCycleCount(){
        unsigned cycles_low0, cycles_high0;
        __asm__ volatile("cpuid\n\t"
                  "rdtsc\n\t"
                  "mov %%edx, %0\n\t"
                  "mov %%eax, %1\n\t"
                  : "=r" (cycles_high0), "=r" (cycles_low0)
                  :: "%rax", "%rbx", "%rcx", "%rdx");
        return calculateCycle(cycles_high0, cycles_low0);
}
uint64_t endCycleCount(){
         unsigned cycles_low1, cycles_high1;
         __asm__ volatile ("rdtscp\n\t"
                  "mov %%edx, %0\n\t"
                  "mov %%eax, %1\n\t"
                  "cpuid\n\t"
                  : "=r" (cycles_high1), "=r" (cycles_low1)
                  :: "%rax", "%rbx", "%rcx", "%rdx");
    return calculateCycle(cycles_high1, cycles_low1);
}

void __cilkrts_init_sched_stats(stats *s)
{
    int i;
    for (i = 0; i < NUMBER_OF_STATS; ++i) {
        s->begin[i] = 0;
        s->end[i] = 0;
        s->time[i] = 0;
    }
}

void __cilkrts_init_total_stats(total *s)
{
    int i;
    for (i = 0; i < NUMBER_OF_STATS; ++i) {
        s->begin[i] = 0;
        s->end[i] = 0;
        s->time[i] = 0;
    }
}

void __cilkrts_start_timing(__cilkrts_worker *w, enum timing i)
{
    if (w) {
        stats *s = w->l->sched_stats;
        CILK_ASSERT(s->begin[i] == 0);
        s->end[i] = 0;
        s->begin[i] = beginCycleCount();
    }
}

void __cilkrts_stop_timing(__cilkrts_worker *w, enum timing i)
{
    if (w) {
        stats *s = w->l->sched_stats;
        CILK_ASSERT(s->end[i] == 0);
        s->end[i] = endCycleCount();
        if (s->end > s->begin) {
            s->time[i] += (s->end[i] - s->begin[i]);
        }
        s->begin[i] = 0; 
    }
}

void __cilkrts_drop_timing(__cilkrts_worker *w, enum timing i)
{
    if (w) {
        stats *s = w->l->sched_stats;
        CILK_ASSERT(s->begin[i] != 0);
        s->begin[i] = 0;
    }
}

void __cilkrts_accum_timing()
{
    global_state_t *g = cilkg_get_global_state();

#ifndef EXTEND_INSTRUMENT
    for(int i = 0; i < NUMBER_OF_STATS; ++i) {
        for(int j = 0; j < g->total_workers; ++j) {
            uint64_t increment = 
                cycleToMicroSecond(g->workers[j]->l->sched_stats->time[i]);
            g->sched_stats->time[i] += (double) increment;
            g->workers[j]->l->sched_stats->time[i] = 0;
        }
    }
#else
    for(int j = 0; j < g->total_workers; ++j) {
        for(int i = 0; i < NUMBER_OF_STATS; ++i) {
            uint64_t increment = 
                cycleToMicroSecond(g->workers[j]->l->sched_stats->time[i]);
            g->sched_stats->time[i] += (double) increment;
        }
        printf("Worker %2d=> Scheduling: %2.3f, Working: %2.3f, Idle %2.3f\n", j, 
                MicroSecondToSecond((double)cycleToMicroSecond(g->workers[j]->l->sched_stats->time[INTERVAL_SCHED])),
                MicroSecondToSecond((double)cycleToMicroSecond(g->workers[j]->l->sched_stats->time[INTERVAL_WORKING])),
                MicroSecondToSecond((double)cycleToMicroSecond(g->workers[j]->l->sched_stats->time[INTERVAL_IDLE])));
    }
#endif

    fprintf(stdout, "Total Scheduling Time: %f\n", 
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_SCHED]));
    fprintf(stdout, "Total Working Time: %f\n", 
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_WORKING]));
    fprintf(stdout, "Total Idle Time: %f\n", 
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_IDLE]));
}

void __cilkrts_reset_timing() 
{
    global_state_t *g = cilkg_get_global_state();
    for(int i = 0; i < NUMBER_OF_STATS; ++i) {
        //resent global counter
        g->sched_stats->time[i] = 0.0;

        //reset worker local counter
        for(int j = 0; j < g->total_workers; ++j) {
            g->workers[j]->l->sched_stats->time[i] = 0;
        }

    }

    LIKWID_MARKER_CLOSE;
    LIKWID_MARKER_INIT;
}
#endif
