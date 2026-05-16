#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

void log_info(const char* fmt, ...);
void log_debug(const char* fmt, ...);
void log_warning(const char* fmt, ...);
void log_error(const char* fmt, ...);
void log_fatal(const char* fmt, ...);

#define LOG_INFO(...) log_info(__VA_ARGS__)
#define LOG_DEBUG(...) log_debug(__VA_ARGS__)
#define LOG_WARNING(...) log_warning(__VA_ARGS__)
#define LOG_ERROR(...) log_error(__VA_ARGS__)
#define LOG_FATAL(...) log_fatal(__VA_ARGS__)

#endif
