#include <framework/framework.h>
#include <toolkit/toolkit.h>
#include <kernel/kernel.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define RFKLIB_IMPL
#include <rfklib/rfklib.h>

int main() {
  app_create();
  app_set_state(bs_get_vtable());
  app_run();
  return EXIT_SUCCESS;
}