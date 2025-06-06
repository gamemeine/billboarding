#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Program.h"
#include <glm/glm.hpp>

class Billboard {
public:
    Billboard();
    Billboard(Texture* sharedTexture, const glm::vec3& pos, float width = 2.0f, float height = 3.0f);
    void SetPosition(const glm::vec3& pos);
    void SetSize(float width, float height);
    void SetTexture(Texture* sharedTexture);
    void Draw(const glm::vec3& cameraPos, Program& shader);
private:
    Mesh _mesh;
    Texture* _texture = nullptr;
    glm::vec3 _position;
    float _width, _height;
    void createQuad();
};
