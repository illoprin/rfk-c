#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>

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