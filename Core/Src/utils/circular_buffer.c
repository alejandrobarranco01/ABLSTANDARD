#include "circular_buffer.h"

#include <string.h>

static inline bool is_power_of_two(size_t x) { return (x != 0U) && ((x & (x - 1U)) == 0U); }

bool circular_buffer_init(circular_buffer_t* cb, void* storage, size_t capacity, size_t element_size)
{
    if ((cb == NULL) || (storage == NULL) || (element_size == 0U) || !is_power_of_two(capacity))
    {
        return false;
    }

    cb->buffer = (uint8_t*) storage;
    cb->size = capacity;
    cb->mask = capacity - 1U;
    cb->element_size = element_size;
    cb->head = 0U;
    cb->tail = 0U;
    return true;
}

bool circular_buffer_is_empty(const circular_buffer_t* cb) { return cb->head == cb->tail; }

bool circular_buffer_is_full(const circular_buffer_t* cb) { return (((cb->head + 1U) & cb->mask) == cb->tail); }

bool circular_buffer_push(circular_buffer_t* cb, const void* element)
{
    if (circular_buffer_is_full(cb))
    {
        return false;
    }

    uint8_t* dest = cb->buffer + (cb->head * cb->element_size);
    memcpy(dest, element, cb->element_size);
    cb->head = (cb->head + 1U) & cb->mask;
    return true;
}

bool circular_buffer_pop(circular_buffer_t* cb, void* element_out)
{
    if (circular_buffer_is_empty(cb))
    {
        return false;
    }

    const uint8_t* src = cb->buffer + (cb->tail * cb->element_size);
    memcpy(element_out, src, cb->element_size);
    cb->tail = (cb->tail + 1U) & cb->mask;
    return true;
}
