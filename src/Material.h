#pragma once
#include <glm/glm.hpp>

struct Material {
    glm::vec3 diffuseColor  = glm::vec3(1.0f);
    glm::vec3 specularColor = glm::vec3(1.0f);
    float     shininess     = 32.0f;
};
