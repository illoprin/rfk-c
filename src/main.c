#include <kernel/kernel.h>
#include <toolkit/toolkit.h>

int main() {
  App_Create();
  App_SetState(BlueState_GetVTable());
  App_Run();
  return EXIT_SUCCESS;
}