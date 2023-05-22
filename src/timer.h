#ifndef __TIMER_H__
#define __TIMER_H__

//-----------------------------------------------------------------
// Prototypes:
//-----------------------------------------------------------------

// General timer
void            timer_init(void);
clock_t         timer_now(void);
static long     timer_diff(clock_t a, clock_t b) { return (long)(a - b); } 
void            timer_sleep(int timeMs);

#endif
