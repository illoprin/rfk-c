#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>

// WARN shit code
void goImguiAssertHandler(const char* expr, const char* file, int line) {
  printf("ImGui Assert: %s at %s:%d\n", expr, file, line);
}

void Plt_Init() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
#endif
}