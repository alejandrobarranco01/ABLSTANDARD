#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint8_t* buffer;
    size_t size;
    size_t mask;
    size_t element_size;
    volatile size_t head;
    volatile size_t tail;
} circular_buffer_t;

bool circular_buffer_init(circular_buffer_t* cb, void* storage, size_t capacity, size_t element_size);
bool circular_buffer_push(circular_buffer_t* cb, const void* element);
bool circular_buffer_pop(circular_buffer_t* cb, void* element_out);
bool circular_buffer_is_empty(const circular_buffer_t* cb);
bool circular_buffer_is_full(const circular_buffer_t* cb);

#endif // CIRCULAR_BUFFER_H
