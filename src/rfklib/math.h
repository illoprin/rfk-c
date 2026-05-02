#ifndef __RFKLIB_MATH_H__
#define __RFKLIB_MATH_H__

#include "types.h"

float randf();
uint randu(uint max);

#endif // __RFKLIB_MATH_H__

#ifdef RFKLIB_IMPL

#include <stdlib.h>
#include "limits.h"

float randf() {
  return (float)rand() / (float)INT_MAX;
}

uint randu(uint max) {
  return rand() % max;
}

#endif // RFKLIB_IMPL