#pragma once
#include "Program.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <span>


struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};


class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    void Init(std::span<Vertex> vertices, std::span<uint32_t> indices);

    void SetDiffuseTexture(Texture diffuse);

    void Draw(Program& program);

private:
    Texture _diffuse;
    GLuint _vao;
    GLuint _vbo;
    GLuint _ibo;
    GLsizei _indexCount;
};