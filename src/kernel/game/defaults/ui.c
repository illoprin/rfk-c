#include "ui.h"

void UI_Init(GLFWwindow* window) {
  // 1. Создаем контекст
  igCreateContext(NULL);

  // Настройка стиля (по желанию)
  igStyleColorsDark(NULL);

  // 2. Инициализируем бэкенды
  // Второй аргумент true в Glfw_Init говорит ImGui самому ставить колбеки на ввод
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460 core");
}

void UI_NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();
}

void UI_DrawDemoWindow() {
  // Простое окно "Hello World"
  igBegin("Debug Tools", NULL, 0);
  igText("Application average %.3f ms/frame (%.1f FPS)",
    1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);

  if (igButton("Click Me", (ImVec2) { 100, 20 })) {
    printf("Button clicked!\n");
  }
  igEnd();
}

void UI_EndFrame() {
  // Собираем данные для рендера
  igRender();
  // Выполняем отрисовку через OpenGL бэкенд
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void UI_Destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);
}
 