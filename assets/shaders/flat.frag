#version 330 core

in vec2 texcoord;

layout (location = 0) out vec4 out_fragcolor;

void main() {
  out_fragcolor = vec4(0.31, 0.68, 0.9, 1.0);
}