#pragma once
#include "Program.h"
#include "Mesh.h"
#include "Model.h"

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
    void Render();

private:
	Texture boxTexture;
    Mesh box;
    Program shader;
	Model model;
	Model chair;

	float fieldOfView;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;

	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;
	float cameraSpeed;
};