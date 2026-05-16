#include "utils/logger.h"

#include "uart.h"

#include <stdarg.h>
#include <stdio.h>

#define MESSAGE_BUFFER_SIZE (512UL)

extern UART_HandleTypeDef huart2;

static uint8_t message_buffer[MESSAGE_BUFFER_SIZE];

static void log_write(const char* prefix, const char* fmt, va_list args)
{
    int offset = snprintf((char*) message_buffer, sizeof(message_buffer), "%s", prefix);
    offset += vsnprintf((char*) message_buffer + offset, sizeof(message_buffer) - (size_t) offset, fmt, args);
    snprintf((char*) message_buffer + offset, sizeof(message_buffer) - (size_t) offset, "\r\n");

#if defined UART_ENABLED
    uart_send(&huart2, message_buffer);
#else
    printf("%s", (char*) message_buffer);
#endif // UART_ENABLED
}

void log_info(const char* fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    log_write("[INFO]    ", fmt, a);
    va_end(a);
}
void log_debug(const char* fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    log_write("[DEBUG]   ", fmt, a);
    va_end(a);
}
void log_warning(const char* fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    log_write("[WARNING] ", fmt, a);
    va_end(a);
}
void log_error(const char* fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    log_write("[ERROR]   ", fmt, a);
    va_end(a);
}
void log_fatal(const char* fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    log_write("[FATAL]   ", fmt, a);
    va_end(a);
}

#if !defined(UART_ENABLED)
int _write(int file, char* ptr, int len)
{
    (void) file;
    HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, 100);
    return len;
}
#endif
