#include "initial_ui.h"

#include <kernel/core/files.h>

bool demoOpen = true;

static ImFont* s_inter_font = NULL;
static ImGuiIO* io = NULL;

static void ui_configure() {
  igStyleColorsDark(NULL);
  io = igGetIO();
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io->IniFilename = "assets/imgui.ini";
} 

static void ui_init_font() {
  ImFontAtlas* atlas = io->Fonts;
  ImFontConfig* config = NULL;
  const ImWchar* range = ImFontAtlas_GetGlyphRangesDefault(atlas);
  
  s_inter_font = ImFontAtlas_AddFontFromFileTTF(
    atlas,
    FLS_FONT_PATH("inter.ttf"),
    18.0,
    config,
    range
  );
  igPushFont(s_inter_font, 18.f);
};

static void ui_init_style() {
  ImGuiStyle* style = igGetStyle();

  // font scaling
  style->FontScaleMain = 1.12;

  // roundings
  style->WindowRounding = 8.0f;     // windows
  style->FrameRounding = 4.0f;      // buttons, inputs
  style->GrabRounding = 12.0f;      // sliders
  style->PopupRounding = 6.0f;      // drop-down menus

  // padding  
  style->WindowPadding = (ImVec2){10, 10};   // internal (window) padding
  style->CellPadding = (ImVec2){8, 8};       // tables
  style->FramePadding = (ImVec2){10, 5};     // button padding
  style->ItemSpacing = (ImVec2){10, 8};      // margin between elements
  style->WindowBorderSize = 0.f;

  ImVec4* colors = style->Colors;
  colors[ImGuiCol_FrameBg] = (ImVec4){0.48f, 0.26f, 0.16f, 0.54f};
  colors[ImGuiCol_FrameBgHovered] = (ImVec4){0.98f, 0.50f, 0.26f, 0.40f};
  colors[ImGuiCol_FrameBgActive] = (ImVec4){0.98f, 0.63f, 0.26f, 0.67f};
  colors[ImGuiCol_TitleBgActive] = (ImVec4){0.48f, 0.28f, 0.16f, 1.00f};
  colors[ImGuiCol_CheckMark] = (ImVec4){0.98f, 0.58f, 0.26f, 1.00f};
  colors[ImGuiCol_SliderGrab] = (ImVec4){0.88f, 0.57f, 0.24f, 1.00f};
  colors[ImGuiCol_SliderGrabActive] = (ImVec4){1.00f, 0.85f, 0.59f, 1.00f};
  colors[ImGuiCol_Button] = (ImVec4){0.98f, 0.68f, 0.26f, 0.40f};
  colors[ImGuiCol_ButtonHovered] = (ImVec4){0.98f, 0.56f, 0.26f, 1.00f};
  colors[ImGuiCol_ButtonActive] = (ImVec4){0.98f, 0.43f, 0.06f, 1.00f};
  colors[ImGuiCol_Header] = (ImVec4){0.98f, 0.66f, 0.26f, 0.31f};
  colors[ImGuiCol_HeaderHovered] = (ImVec4){0.98f, 0.56f, 0.26f, 0.80f};
  colors[ImGuiCol_HeaderActive] = (ImVec4){0.98f, 0.68f, 0.26f, 1.00f};
  colors[ImGuiCol_TabHovered] = (ImVec4){0.98f, 0.68f, 0.26f, 0.80f};
  colors[ImGuiCol_Tab] = (ImVec4){0.58f, 0.36f, 0.18f, 0.86f};
  colors[ImGuiCol_TabSelected] = (ImVec4){0.68f, 0.46f, 0.20f, 1.00f};
  colors[ImGuiCol_TabSelectedOverline] = (ImVec4){0.98f, 0.63f, 0.26f, 1.00f};
}

void ui_init(GLFWwindow* window) {
  igCreateContext(NULL);

  ui_configure();

  ui_init_font();

  ui_init_style();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ui_set_input(bool enabled) {
  if (enabled) {
    io->ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    io->ConfigFlags &= ~ImGuiConfigFlags_NoKeyboard;
  } else {
    io->ConfigFlags |= ImGuiConfigFlags_NoMouse;
    io->ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
  }
}

void ui_begin_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();
}

void ui_draw_demo() {
  igBegin("Debug Tools", NULL, 0);
  igText("Application average %.3f ms/frame (%.1f FPS)",
    1000.0f / igGetIO()->Framerate, igGetIO()->Framerate
  );
  if (igButton("Click Me", (ImVec2) { })) {
    printf("Button clicked!\n");
  }
  igEnd();
  igShowDemoWindow(&demoOpen);
}

void ui_end_frame() {
  igRender();
  ImDrawData* data = igGetDrawData();
  ImGui_ImplOpenGL3_RenderDrawData(data);
}

void ui_destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);
}
 