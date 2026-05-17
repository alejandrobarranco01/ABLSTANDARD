#ifndef TIME_UTIL_H
#define TIME_UTIL_H

#include <stdbool.h>
#include <stdint.h>

#define HAL_TIME_1_MS (1U)
#define HAL_TIME_5_MS (5U)
#define HAL_TIME_10_MS (10U)
#define HAL_TIME_20_MS (20U)
#define HAL_TIME_50_MS (50U)
#define HAL_TIME_100_MS (100U)
#define HAL_TIME_200_MS (200U)
#define HAL_TIME_250_MS (250U)
#define HAL_TIME_500_MS (500U)

#define HAL_TIME_1_SEC (1000U)
#define HAL_TIME_2_SEC (2000U)
#define HAL_TIME_5_SEC (5000U)
#define HAL_TIME_10_SEC (10000U)
#define HAL_TIME_15_SEC (15000U)
#define HAL_TIME_30_SEC (30000U)

#define HAL_TIME_1_MIN (60000U)
#define HAL_TIME_2_MIN (120000U)
#define HAL_TIME_5_MIN (300000U)
#define HAL_TIME_10_MIN (600000U)
#define HAL_TIME_15_MIN (900000U)
#define HAL_TIME_30_MIN (1800000U)

#define HAL_TIME_1_HOUR (3600000U)

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
