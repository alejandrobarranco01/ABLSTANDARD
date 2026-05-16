#include "interrupt_handler.h"

#include "main.h"
#include "circular_buffer.h"
#include "ui.h"

#define EVENT_BUFFER_CAPACITY 16U

typedef enum
{
    EVENT_NONE = 0,
    EVENT_LEFT_BUTTON_PRESS,
    EVENT_RIGHT_BUTTON_PRESS,
} EVENT_E;

static EVENT_E event_storage[EVENT_BUFFER_CAPACITY];
static circular_buffer_t event_buffer;

int interrupt_handler_init(void)
{
    if (!circular_buffer_init(&event_buffer, event_storage, EVENT_BUFFER_CAPACITY, sizeof(EVENT_E)))
    {
        LOG_ERROR("Failed to initialize interrupt handler circular buffer");
        return -1;
    }
    return 0;
}

int interrupt_handler_task(void)
{
    EVENT_E event;

    while (circular_buffer_pop(&event_buffer, &event))
    {
        switch (event)
        {
            case EVENT_LEFT_BUTTON_PRESS:
                LOG_INFO("Left button pressed!");
                ui_handle_event(LEFT_BUTTON_PRESS);
                break;
            case EVENT_RIGHT_BUTTON_PRESS:
                LOG_INFO("Right button pressed!");
                ui_handle_event(RIGHT_BUTTON_PRESS);
                break;
            default:
                LOG_WARNING("Unknown event in buffer: %d", event);
                break;
        }
    }
    return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    EVENT_E event;

    switch (GPIO_Pin)
    {
        case BUTTON_1_Pin:
            event = EVENT_LEFT_BUTTON_PRESS;
            break;
        case BUTTON_2_Pin:
            event = EVENT_RIGHT_BUTTON_PRESS;
            break;
        default:
            return;
    }

    circular_buffer_push(&event_buffer, &event);
}
