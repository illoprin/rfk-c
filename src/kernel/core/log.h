#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "types.h"

/* Colors */
#define RFK_RESET       "\033[0m"
#define RFK_BLACK       "\033[30m"             /* Black */
#define RFK_RED         "\033[31m"             /* Red */
#define RFK_GREEN       "\033[32m"             /* Green */
#define RFK_YELLOW      "\033[33m"             /* Yellow */
#define RFK_BLUE        "\033[34m"             /* Blue */
#define RFK_MAGENTA     "\033[35m"             /* Magenta */
#define RFK_CYAN        "\033[36m"             /* Cyan */
#define RFK_WHITE       "\033[37m"             /* White */
#define RFK_BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define RFK_BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define RFK_BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define RFK_BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define RFK_BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define RFK_BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define RFK_BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define RFK_BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

enum LogLevel : uch {
  RFK_LOG_INFO = 1,
  RFK_LOG_WARN,
  RFK_LOG_ERR,
};

void __log(FILE*, enum LogLevel, const char* file, int line, const char* fmt, ...);
void __assert(bool condition, const char* condStr, const char* file, int line);

#define FILE_PATH_STR(file) "\"" file "\""

#define LogInfo(...) \
  __log(stdout, RFK_LOG_INFO, __BASE_FILE__, __LINE__, __VA_ARGS__)

#define LogWarn(...) \
  __log(stdout, RFK_LOG_WARN, __BASE_FILE__, __LINE__, __VA_ARGS__)

#define LogErr(...) \
  __log(stdout, RFK_LOG_ERR, __BASE_FILE__, __LINE__, __VA_ARGS__)

#define RFK_ASSERT(condition, msg) \
  __assert((condition), (msg), __FILE__, __LINE__);
