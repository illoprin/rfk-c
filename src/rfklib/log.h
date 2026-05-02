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

#if defined(RFKLIB_IMPL) 

#include <time.h>
#include <stdlib.h>

void __log(
  FILE* f,
  enum LogLevel loglevel,
  const char* src,
  int line,
  const char* fmt, ...
) {
  // output format:
  // %DATE% %TIME% [%LOGLEVEL%] %MSG% %FILE%:%LINE%

  // create valist for formatted string
  va_list arglist;

  // date time
  time_t curr = time(nullptr);
  struct tm* t = localtime(&curr);
  char dtStr[64];
  strftime(dtStr, sizeof(dtStr), "[%d/%m/%Y %H:%M:%S]", t);
  fprintf(f, RFK_CYAN "%s " RFK_RESET, dtStr);

  // print loglevel
  switch (loglevel) {
    case RFK_LOG_INFO:
      fprintf(f, RFK_BOLDGREEN "[INFO] " RFK_RESET);
      break;

    case RFK_LOG_WARN:
      fprintf(f, RFK_BOLDYELLOW "[WARNING] " RFK_RESET);
      break;

    case RFK_LOG_ERR:
      fprintf(f, RFK_BOLDRED "[ERROR] " RFK_RESET);
      break;

    default:
      fprintf(f, RFK_BOLDWHITE "[???] " RFK_RESET);
      break;
  }

  // print formatted string
  fputs(RFK_MAGENTA, f);
  va_start(arglist, fmt);
  vfprintf(f, fmt, arglist);
  va_end(arglist);
  fputs(RFK_RESET, f);

  // print file and line
  fprintf(f, " from " RFK_BOLDWHITE FILE_PATH_STR("%s:%d") RFK_RESET "\n", src, line);
};

void __assert(bool condition, const char* str, const char* file, int line) {
  if (!condition) {
    printf(RFK_BOLDRED "Assertion failed: %s" RFK_RESET "\n", str);
    printf(
      RFK_MAGENTA "File: " RFK_RESET RFK_BOLDWHITE "%s:%d" RFK_RESET "\n",
      file, line
    );
    exit(EXIT_FAILURE);
  }
}

#endif