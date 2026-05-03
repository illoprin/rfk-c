#include "profiler.h"

#include <kernel/core/defs.h>
#include <kernel/gfx/gfx.h>

SystemStats stats = {0};

float elapsedTime   = .0;
float lastDeltaTime = .0;

void prof_update() {
  // calculate delta time
  double time     = glfwGetTime();
  lastDeltaTime   = stats.DeltaTime;
  stats.DeltaTime = time - stats.Time;
  stats.Time      = time;

  // calculate smoothed delta time
  stats.SmoothedDeltaTime =
    lastDeltaTime
    + (stats.DeltaTime - lastDeltaTime) * SMOOTHED_DELTA_TIME_ALPHA;

  // calculate FPS
  elapsedTime += stats.DeltaTime;
  if (elapsedTime >= FPS_UPDATE_PERIOD) {
    stats.FPS =
      stats.DeltaTime > EPSILON ? 1.0 / stats.DeltaTime : 9999u;
    elapsedTime = .0;
  }

  // calculate render stats
  rhi_RenderStats r   = rhi_get_stats();
  stats.LastDrawCalls = r.DrawCalls;
  stats.LastTriangles = r.Triangles;
  stats.LastVertices  = r.Vertices;
}

float prof_dt() {
  return stats.DeltaTime;
}

float prof_smoothed_dt() {
  return stats.SmoothedDeltaTime;
}

SystemStats prof_stats() {
  return stats;
}
