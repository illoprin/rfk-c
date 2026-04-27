#pragma once

// C headers

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <locale.h>
#include <wchar.h>

// Math

#include <math.h>
#include <cglm/cglm.h>

#ifdef RFK_UTIL_IMPL

int Rand_Uniformi(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

#endif