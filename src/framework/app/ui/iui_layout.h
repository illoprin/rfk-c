#pragma once

typedef enum {
  STATS_MODE_HIDDEN,
  STATS_MODE_COMPACT,   // only fps
  STATS_MODE_DETAILED,  // drawcalls, memory, frametime
  STATS_MODE_COUNT      // helper element for cycle switch
} iui_StatsOverlayMode;

#define UI_PADDING 24

void iui_draw_stats_overlay();

void iui_switch_stats_mode();