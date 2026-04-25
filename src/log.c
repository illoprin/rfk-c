#include "log.h"

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

void __assert(const char* cond, const char* file, int line) {
  printf(RFK_BOLDRED "Assertion failed: %s" RFK_RESET "\n", cond);
  printf(
    RFK_MAGENTA "File: " RFK_RESET RFK_BOLDWHITE "%s:%d" RFK_RESET "\n",
    file, line
  );
  exit(-1);
}