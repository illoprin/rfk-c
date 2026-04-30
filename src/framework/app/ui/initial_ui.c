#include "initial_ui.h"

bool demoOpen = true;

void UI_Init(GLFWwindow* window) {
  igCreateContext(NULL);

  igStyleColorsDark(NULL);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460 core");
}

void UI_NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();
}

void UI_DrawDemoWindow() {
  igBegin("Debug Tools", NULL, 0);
  igText("Application average %.3f ms/frame (%.1f FPS)",
    1000.0f / igGetIO()->Framerate, igGetIO()->Framerate
  );
  if (igButton("Click Me", (ImVec2) { 100, 20 })) {
    printf("Button clicked!\n");
  }
  igEnd();
  igShowDemoWindow(&demoOpen);
}

void UI_EndFrame() {
  igRender();
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void UI_Destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);
}
 