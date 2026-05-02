#include <framework/framework.h>
#include <toolkit/toolkit.h>
#include <kernel/kernel.h>

int main() {
  app_create();
  app_set_state(bs_get_vtable());
  app_run();
  return EXIT_SUCCESS;
}