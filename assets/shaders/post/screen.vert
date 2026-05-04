#version 330 core

layout (location = 0) in vec2 in_position;

out vec2 texcoord;

void main() {
  texcoord = ((in_position * 2.0) + 1.0) / 2.0;
  gl_Position = vec4(in_position, 0.0, 1.0);
} 