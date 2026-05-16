#include "time_util.h"

#include "bsp_time.h"

int timer_start(timer_t* timer, uint32_t duration)
{
    timer->tickstart = bsp_time_get_ms();
    timer->duration = duration;
    return 0;
}

int timer_restart(timer_t* timer)
{
    if (!timer->duration)
    {
        return -1;
    }
    timer->tickstart = bsp_time_get_ms();
    return 0;
}

bool timer_is_done(timer_t* timer) { return (bsp_time_get_ms() - timer->tickstart >= timer->duration); }

int stopwatch_start(stopwatch_t* stopwatch)
{
    stopwatch->tickstart = bsp_time_get_ms();
    return 0;
}

uint32_t stopwatch_get_elapsed(stopwatch_t* stopwatch) { return (bsp_time_get_ms() - stopwatch->tickstart); }
