#version 330 core

out vec4 out_fragcolor;

in vec2 uv;

uniform sampler2D u_diffuse;

void main() {
  out_fragcolor = texture(u_diffuse, uv);
}