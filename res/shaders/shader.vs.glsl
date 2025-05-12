#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

void main() {
  vec4 worldPos = uModelMatrix * vec4(aPosition, 1.0);
  FragPos = worldPos.xyz;
  Normal = mat3(transpose(inverse(uModelMatrix))) * aNormal;
  TexCoord = aTexCoord;
  gl_Position = uProjectionMatrix * uViewMatrix * worldPos;
}