#include "data_manager.h"

#include "main.h"

#include <stdbool.h>

static bool initialized = false;
static float internal_data = 0.0f;

int data_manager_init(void)
{
    if (initialized)
    {
        return -1;
    }

    initialized = true;

    LOG_INFO("Initialized data manager");

    return 0;
}

int data_manager_task(void)
{
    if (!initialized)
    {
        return -1;
    }

    internal_data += 0.01;

    return 0;
}

int data_manager_get_data(float* data)
{
    if (!initialized || data == NULL)
    {
        return -1;
    }

    *data = internal_data;

    return 0;
}

int data_manager_deinit(void)
{
    if (!initialized)
    {
        return -1;
    }

    initialized = false;

    LOG_INFO("Deinitialized data manager");

    return 0;
}
