#include "monitor.h"

#include <kernel/gfx/gfx.h>
#include <kernel/core/defs.h>

struct MonitorStats stats = { 0 };

float elapsedTime = .0;
float lastDeltaTime = .0;

void Mon_Update() {
  // calculate delta time
  double time = glfwGetTime();
  lastDeltaTime = stats.DeltaTime;
  stats.DeltaTime = time - stats.Time;
  stats.Time = time;

  // calculate smoothed delta time
  stats.SmoothedDeltaTime = lastDeltaTime + (stats.DeltaTime - lastDeltaTime) * SMOOTHED_DELTA_TIME_ALPHA;

  // calculate FPS
  elapsedTime += stats.DeltaTime;
  if (elapsedTime >= FPS_UPDATE_PERIOD) {
    stats.FPS = stats.DeltaTime > EPSILON ? 1.0 / stats.DeltaTime : 9999u;
    elapsedTime = .0;
  }

  // calculate render stats
  struct rhi_RenderStats r = rhi_GetStats();
  stats.LastDrawCalls = r.DrawCalls;
  stats.LastTriangles = r.Triangles;
  stats.LastVertices = r.Vertices;
}

float GetDeltaTime() {
  return stats.DeltaTime;
};

float GetSmoothedDeltaTime() {
  return stats.SmoothedDeltaTime;
};

struct MonitorStats Mon_GetStats() {
  return stats;
}
