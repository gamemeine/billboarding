#version 330 core

in vec3 vFragPos;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
  vec4 color = texture(uTexture, vTexCoord);
  FragColor = color;
}