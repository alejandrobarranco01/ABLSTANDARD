#ifndef TIME_UTIL_H
#define TIME_UTIL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t tickstart;
    uint32_t duration;
} timer_t;

typedef struct
{
    uint32_t tickstart;
} stopwatch_t;

int timer_start(timer_t* timer, uint32_t duration);
int timer_restart(timer_t* timer);
bool timer_is_done(timer_t* timer);

int stopwatch_start(stopwatch_t* stopwatch);
uint32_t stopwatch_get_elapsed(stopwatch_t* stopwatch);

#endif // TIME_UTIL_H
