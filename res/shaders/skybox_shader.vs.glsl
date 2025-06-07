#version 330 core
layout (location = 0) in vec3 aPosition;

out vec3 vTexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
	vec4 position = uProjectionMatrix * uViewMatrix * vec4(aPosition, 1.0f);
	gl_Position = vec4(position.xyww);
	vTexCoord = vec3(aPosition.xy, -aPosition.z);
}