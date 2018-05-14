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
#define cycleToMicroSecond(cycle) ((double)cycle/(double)2200.0)
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
        s->last_user_code_op = INTERVAL_WORK_UNPINNED; // default 
        s->total_steals = 0;
        s->successful_steals = 0;
        s->successful_steals_pushed = 0;
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
        uint64_t elapsed = s->end[i] - s->begin[i];
        if (elapsed > 0) {
            s->time[i] += elapsed;
            if(i == INTERVAL_WORK_INFLATION) {
                s->time[s->last_user_code_op] += elapsed;
            }
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
            double increment = 
                cycleToMicroSecond(g->workers[j]->l->sched_stats->time[i]);
            g->sched_stats->time[i] += increment;
            g->workers[j]->l->sched_stats->time[i] = 0;
        }
    }
#else
    for(int j = 0; j < g->total_workers; ++j) {
        struct sched_stats *wstats = g->workers[j]->l->sched_stats;
        for(int i = 0; i < NUMBER_OF_STATS; ++i) {
            double increment = cycleToMicroSecond(wstats->time[i]);
            g->sched_stats->time[i] += increment;
        }
        printf("Worker %2d=> Scheduling: %2.3f, Working: %2.3f (%2.3f(l)/%2.3f(r)/%2.3f(u)), Idle %2.3f, steals: %u(p)/%u(s)/%u\n",
               j, 
               cycleToSecond(wstats->time[INTERVAL_SCHED]),
               cycleToSecond(wstats->time[INTERVAL_WORK_INFLATION]),
               cycleToSecond(wstats->time[INTERVAL_WORK_LOCAL]),
               cycleToSecond(wstats->time[INTERVAL_WORK_REMOTE]),
               cycleToSecond(wstats->time[INTERVAL_WORK_UNPINNED]),
               cycleToSecond(wstats->time[INTERVAL_IDLE]), 
               wstats->successful_steals_pushed, 
               wstats->successful_steals, 
               wstats->total_steals);
    }
#endif

    fprintf(stdout, "Total Scheduling Time: %f\n", 
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_SCHED]));
    fprintf(stdout, "Total Working Time: %f (%f(l)/%f(r)/%f(u))\n", 
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_WORK_INFLATION]),
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_WORK_LOCAL]),
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_WORK_REMOTE]),
            MicroSecondToSecond(g->sched_stats->time[INTERVAL_WORK_UNPINNED]));
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

#else
void __cilkrts_accum_timing(){}
void __cilkrts_reset_timing(){}
#endif
