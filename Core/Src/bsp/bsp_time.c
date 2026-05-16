#include "bsp_time.h"

#include "main.h"

uint32_t bsp_time_get_ms(void) { return HAL_GetTick(); }
