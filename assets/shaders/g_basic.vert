#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texcoord;
layout (location = 2) in vec3 in_normal;

uniform mat4 u_projection = mat4(1.0);
uniform mat4 u_view = mat4(1.0);

out vec2 uv;

void main() {
  uv = in_texcoord;
  gl_Position = u_projection * u_view * vec4(in_position, 1.0);
}