#pragma once

#include <rfklib/types.h>

#define SMOOTHED_DELTA_TIME_ALPHA 0.1
#define FPS_UPDATE_PERIOD .5 // freq fps updating (seconds)

typedef struct {
  double Time;
  float DeltaTime;
  float SmoothedDeltaTime;
  int FPS;
  int LastDrawCalls;
  int LastVertices;
  int LastTriangles;
} SystemStats;

void prof_update();
float prof_dt();
float prof_smoothed_dt();
SystemStats prof_stats();