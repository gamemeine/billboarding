#pragma once
#include "Program.h"
#include "Mesh.h"
#include "Model.h"
#include "Billboard.h"

#include "Camera.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

struct GLFWwindow;

class Scene
{
public:
    Scene();
    ~Scene();
    void Init();
    void Update(GLFWwindow *window);
    void UpdateCameraDirection(float xoffset, float yoffset);
    void UpdateCameraZoom(float xoffset, float yoffset);
    void Render();

private:
    Texture boxTexture;
    Texture treeTexture;
    Mesh box;
    std::vector<Billboard> trees;
    Program shader;
    Model model;
    Model chair;
    Camera camera;
    glm::mat4 modelMatrix;
};