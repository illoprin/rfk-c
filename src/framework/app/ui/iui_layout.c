#include "iui_layout.h"

#include "imgui.h"
#include <kernel/core/profiler.h>
#include <kernel/gfx/rhi/render_device.h>

static iui_StatsOverlayMode g_stats_mode = STATS_MODE_COMPACT;
static const float          PAD          = (float)UI_PADDING;

void iui_draw_stats_overlay() {
  if (g_stats_mode == STATS_MODE_HIDDEN) { return; }

  ImGuiWindowFlags window_flags =
    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
    | ImGuiWindowFlags_NoSavedSettings
    | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
    | ImGuiWindowFlags_NoMove;

  ImGuiIO* io               = igGetIO();
  ImVec2   window_pos       = {PAD, PAD};
  ImVec2   window_pos_pivot = {0.0f, 0.0f};  // left top

  igSetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
  igSetNextWindowBgAlpha(0.5f);

  SystemStats st = prof_stats();

  if (igBegin("##StatsOverlay", NULL, window_flags)) {
    igText("FPS: %d", st.FPS);
    igText("Frametime: %.3f ms", st.DeltaTime * 1000.f);

    if (g_stats_mode == STATS_MODE_DETAILED) {
      igSeparator();
      igText("Draw calls: %d", st.LastDrawCalls);
      igText(rhi_device_str_renderer());
      igText(rhi_device_str_version());
    }
  }
  igEnd();
}

void iui_switch_stats_mode() {
  g_stats_mode =
    (iui_StatsOverlayMode)((g_stats_mode + 1) % STATS_MODE_COUNT);
}
