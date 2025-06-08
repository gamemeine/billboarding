#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uLightColor;

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material uMaterial;

void main() {
  vec4 tex = texture(uTexture, TexCoord);
  if (tex.a < 0.1) discard;  // Alpha test

  vec3 color = tex.rgb;
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(uLightPos - FragPos);

  // ambient
  vec3 ambient = 0.1 * uMaterial.diffuse * uLightColor;

  // diffuse
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * uMaterial.diffuse * uLightColor;

  // specular
  vec3 viewDir = normalize(uViewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
  vec3 specular = spec * uMaterial.specular * uLightColor;

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result * color, tex.a);
}
