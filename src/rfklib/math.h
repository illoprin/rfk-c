#pragma once

#include "types.h"
#include <stdlib.h>
#include "limits.h"

float randf();
uint randu(uint max);

#ifdef RFKLIB_IMPL

float randf() {
  return (float)rand() / (float)INT_MAX;
}

uint randu(uint max) {
  return rand() % max;
}

#endif