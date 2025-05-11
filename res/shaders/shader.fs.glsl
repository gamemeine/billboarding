#version 330 core

in vec3 vNormal;
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
    vec4 texColor = texture(uTexture, vTexCoord);

    FragColor = vec4(texColor.xyz, 1.0f);
    //FragColor = vec4(vTexCoord.xy, 0.0, 1.0f);
    //FragColor = vec4(vNormal.xyz, 1.0f);
} 