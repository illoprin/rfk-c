#include <kernel/kernel.h>
#include <toolkit/toolkit.h>

int main() {
  Game_Create();
  Game_SetState(BlueState_GetVTable());
  Game_Run();
  return EXIT_SUCCESS;
}