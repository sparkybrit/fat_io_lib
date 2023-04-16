#include <stdio.h>
#include <time.h>
#include "timer.h"


//--------------------------------------------------------------------------
// timer_init:
//--------------------------------------------------------------------------
void timer_init(void)
{
    printf("timer_init()\n");
}
//--------------------------------------------------------------------------
// timer_sleep:
//--------------------------------------------------------------------------
void timer_sleep(int timeMs)
{
    t_time t = timer_now();

    while (timer_diff(timer_now(), t) < timeMs)
        ;
}
//--------------------------------------------------------------------------
// timer_now: Return time now in ms
//--------------------------------------------------------------------------
t_time timer_now(void)
{
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    return spec.tv_nsec / 1.0e6; // convert nanoseconds to milliseconds    
}
