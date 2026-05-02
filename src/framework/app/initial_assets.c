#include "initial_assets.h"

#include <framework/gfx/resource/mesh.h>
#include <framework/gfx/resource/program_loader.h>
#include <kernel/core/files.h>

static rhi_Program s_prog_grid;
static Mesh s_mesh_quad;

void ia_init() {
  // ------------------------------
  //          Programs
  // ------------------------------

  RFK_ASSERT(rhi_prog_quick_load(&s_prog_grid,
    FLS_SHADER_PATH("grid.vert"),
    FLS_SHADER_PATH("grid.frag")
  ), "failed to load grid program");

  // ------------------------------
  //          Meshes
  // ------------------------------

  mesh_init_basic_quad(&s_mesh_quad);
}

const Mesh* ia_mesh_quad() {
  return &s_mesh_quad;
}

void ia_render_inf_grid(Camera* cam, float y, float sc_size, float bc_size) {
  rhi_device_use_program(s_prog_grid);

  // projection view matrix
  mat4 pv;
  glm_mat4_mul(cam->Proj, cam->View, pv[0]);
  rhi_prog_uniform_mat4(s_prog_grid, "u_pv", pv[0]);

  // grid positioning
  vec3 pos = { cam->Position[0], y, cam->Position[2] };
  rhi_prog_uniform_3f(s_prog_grid, "u_pos", pos);

  // grid params
  rhi_prog_uniform_1f(s_prog_grid, "u_sc_size", sc_size);
  rhi_prog_uniform_1f(s_prog_grid, "u_bc_size", bc_size);

  glDisable(GL_CULL_FACE);
  rhi_device_draw(s_mesh_quad.VAO, s_mesh_quad.indexNum);
  glEnable(GL_CULL_FACE);
}

void ia_render_sphere(Camera* cam, ia_SphereType t, float* mat_model, float* color) {

}

void ia_render_cube(Camera* cam, float* mat_model, float* color) {

}

void ia_destroy() {
  rhi_prog_invalidate(&s_prog_grid);
  mesh_invalidate(&s_mesh_quad);
}
