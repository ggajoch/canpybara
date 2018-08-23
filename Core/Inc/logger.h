#ifndef _LOGGER_H
#define _LOGGER_H

#include <inttypes.h>

#define __LOG__(format, loglevel, ...) printf("%-10"PRIu32" [%s:%d] " format "\n", HAL_GetTick(), __FILE__, __LINE__, ## __VA_ARGS__)
#define LOG(format, ...) __LOG__(format, "DEBUG", ## __VA_ARGS__)
#define LOG_BOOT() LOG("BOOT, build: " __DATE__ " " __TIME__)

#endif