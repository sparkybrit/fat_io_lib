#include <stdio.h>
#include <time.h>
#include "timer.h"

extern volatile clock_t milliseconds;

//--------------------------------------------------------------------------
// timer_init:
//--------------------------------------------------------------------------
void timer_init(void)
{
    printf("timer:timer_init()\n");
}
//--------------------------------------------------------------------------
// timer_sleep:
//--------------------------------------------------------------------------
void timer_sleep(int timeMs)
{
    clock_t timer_start = timer_now();

    while (timer_diff(timer_now(), timer_start) < timeMs)
    {    
        printf("timer.c:timer_diff(%d, %d) = %d\n", timer_now(), timer_start, timer_diff(timer_now(), timer_start));
    }
}

//--------------------------------------------------------------------------
// timer_now: Return time since boot in ms
//--------------------------------------------------------------------------
volatile clock_t timer_now(void)
{
    return milliseconds;
}
