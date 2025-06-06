#include "Billboard.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <span>
#include <vector>

Billboard::Billboard() : _position(0.0f), _width(2.0f), _height(3.0f) {
    createQuad();
}

Billboard::Billboard(Texture* sharedTexture, const glm::vec3& pos, float width, float height)
    : _texture(sharedTexture), _position(pos), _width(width), _height(height) {
    createQuad();
    if (_texture) _mesh.SetDiffuseTexture(*_texture);
}

void Billboard::SetPosition(const glm::vec3& pos) {
    _position = pos;
}

void Billboard::SetSize(float width, float height) {
    _width = width;
    _height = height;
}

void Billboard::SetTexture(Texture* sharedTexture) {
    _texture = sharedTexture;
    if (_texture) _mesh.SetDiffuseTexture(*_texture);
}

void Billboard::Draw(const glm::vec3& cameraPos, Program& shader) {
    // Axial billboard: rotate only around Y axis
    glm::vec3 toCam = cameraPos - _position;
    float angle = atan2(toCam.x, toCam.z);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), _position);
    model = glm::rotate(model, angle, glm::vec3(0,1,0));
    model = glm::scale(model, glm::vec3(_width, _height, 1.0f));
    glUniformMatrix4fv(shader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    _mesh.Draw(shader);
}

void Billboard::createQuad() {
    std::vector<Vertex> vertices = {
        { { -0.5f, 0.0f, 0.0f }, {0,0,1}, {0,0} },
        { {  0.5f, 0.0f, 0.0f }, {0,0,1}, {1,0} },
        { {  0.5f, 1.0f, 0.0f }, {0,0,1}, {1,1} },
        { { -0.5f, 1.0f, 0.0f }, {0,0,1}, {0,1} },
    };
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
    _mesh.Init(vertices, indices);
}
