#pragma once

// Initial Assets (prefix 'ia') - is module, that represents build-in
// assets supplied with the engine and can be used immediately

#include <kernel/core/camera.h>
#include <framework/gfx/resource/mesh.h>

typedef enum {
  IA_SPHERE_8,
  IA_SPHERE_16,
  IA_SPHERE_32,
} ia_SphereType;

void ia_init();

/// @brief basic quad vao (by default uses for screen or simple planes)
/// @return quad mesh
/// single attr - vec2 position xy - [-1..1]
const Mesh* ia_mesh_quad();

/// @brief draws infinite grid that follows camera
///        single export - out_frag_color
/// @param cam camera handle
/// @param y grid vertical height
/// @param sc_size small cell size (units)
/// @param bc_size big cell size (units)
/// @param sc_lwidth small cell line width
/// @param bc_lwidth big cell line width
void ia_render_inf_grid(Camera* cam, float y, float sc_size, float bc_size);

void ia_render_sphere(Camera* cam, ia_SphereType t, float* mat_model, float* color);

void ia_render_cube(Camera* cam, float* mat_model, float* color);

void ia_destroy();