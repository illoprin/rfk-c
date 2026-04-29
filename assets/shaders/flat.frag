#version 330 core

in vec2 texcoord;

layout (location = 0) out vec4 out_fragcolor;

void main() {
  out_fragcolor = vec4(0.929, 0.384, 0.827, 1.0);
}