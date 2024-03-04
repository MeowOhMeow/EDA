#ifndef DEBUG_H
#define DEBUG_H

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define Assert(cond, format, ...)                                                        \
    do                                                                                   \
    {                                                                                    \
        if (!(cond))                                                                     \
        {                                                                                \
            fprintf(stderr, ANSI_COLOR_RED format ANSI_COLOR_RESET "\n", ##__VA_ARGS__); \
            assert(cond);                                                                \
        }                                                                                \
    } while (0)
#define Perror(cond, format, ...) Assert(cond, format ": %s", ##__VA_ARGS__, strerror(errno))
#define Panic(...) Assert(0, ##__VA_ARGS__)

#endif // DEBUG_H