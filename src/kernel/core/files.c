#include "files.h"

#include <limits.h>
#include <rfklib/log.h>
#include <stb/stb_image.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char* fls_read_file(const char* fpath, size_t* out_size) {
  FILE* file = fopen(fpath, "rb");
  if (!file) {
    LogErr("could not open file \"%s\"", fpath);
    perror(fpath);
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    LogErr("fseek failed \"%s\"", fpath);
    fclose(file);
    return NULL;
  }

  long size = ftell(file);
  if (size < 0) {
    LogErr("ftell failed \"%s\"", fpath);
    fclose(file);
    return NULL;
  }

  if (fseek(file, 0, SEEK_SET) != 0) {
    LogErr("fseek rewind failed \"%s\"", fpath);
    fclose(file);
    return NULL;
  }

  // защита от переполнения
  if ((uint64_t)size >= SIZE_MAX - 1) {
    LogErr("file too large \"%s\"", fpath);
    fclose(file);
    return NULL;
  }

  size_t byte_size = (size_t)size;

  char* buffer = (char*)malloc(byte_size + 1);
  if (!buffer) {
    LogErr("out of memory \"%s\"", fpath);
    fclose(file);
    return NULL;
  }

  size_t readed = fread(buffer, 1, byte_size, file);
  if (readed != byte_size) {
    LogErr("failed to fully read file \"%s\"", fpath);
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[byte_size] = '\0';

  fclose(file);

  if (out_size) *out_size = byte_size;

  LogInfo("file \"%s\" read", fpath);

  return buffer;
}
