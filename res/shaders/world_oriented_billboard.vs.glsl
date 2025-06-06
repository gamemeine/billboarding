#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 vFragPos;
out vec2 vTexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uPosition;
uniform vec2 uSize;

void main() 
{
	vec3 cameraRight = vec3(uViewMatrix[0][0], uViewMatrix[1][0], uViewMatrix[2][0]);
    vec3 cameraUp    = vec3(uViewMatrix[0][1], uViewMatrix[1][1], uViewMatrix[2][1]);

    vec3 offset = uSize.x * aPosition.x * cameraRight + uSize.y * aPosition.y * cameraUp;
    vec3 worldPos = uPosition + offset;

	vFragPos = worldPos;
	vTexCoord = aTexCoord;
	gl_Position = uProjectionMatrix * uViewMatrix * vec4(worldPos, 1.0);
}
