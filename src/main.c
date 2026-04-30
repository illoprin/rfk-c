#include <kernel/kernel.h>
#include <framework/framework.h>
#include <toolkit/toolkit.h>

int main() {
  Vector files;
  Vec_Init(&files, char[32]);

  Vec_Push(&files, "corrugate.png");
  Vec_Push(&files, "tiny_tiles.png");
  Vec_Push(&files, "dark_brick.png");
  Vec_Push(&files, "gray_rock.png");
  Vec_Push(&files, "hexagons.png");
  Vec_Push(&files, "round_bricks.png");
  Vec_Push(&files, "123.png");

  struct TextureAtlas atlas;
  RFK_ASSERT(!TexAtlas_InitFromFiles(&atlas, files, "null.png"), "failed to load atlas");

  Vec_Destroy(&files); // free files vector

  RFK_ASSERT(!TexAtlas_WriteToFile(&atlas, "atlas.png"), "failed to write atlas");

  TexAtlas_Free(atlas); // free atlas object



  // App_Create();
  // App_SetState(BlueState_GetVTable());
  // App_Run();
  return EXIT_SUCCESS;
}