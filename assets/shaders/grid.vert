#version 330 core

layout (location = 0) in vec2 in_position;

uniform mat4 u_pv = mat4(1.0); // projection * view
uniform vec3 u_pos = vec3(0.0);
uniform float u_size = 100.0;

out vec3 position;

void main() {
  vec2 xz = (in_position * u_size) + u_pos.xz;

  position = vec3(xz.x, u_pos.y, xz.y);
  gl_Position = u_pv * vec4(position, 1.0);
}