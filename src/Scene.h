#pragma once
#include "Program.h"
#include "Mesh.h"
#include "Model.h"
#include "WOBillboard.h"

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
    void Update(GLFWwindow *window, float dt);
	void UpdateCameraDirection(float xoffset, float yoffset);
	void UpdateCameraZoom(float xoffset, float yoffset);
    void Render();

private:
	Texture boxTexture;
    Mesh box;
    Program shader;
	Model model;
	Model chair;
	WOBillboard cloud1;
	WOBillboard  cloud2;
	WOBillboard  cloud3;
	Texture cloudTexture1;
	Texture cloudTexture2;
	Texture cloudTexture3;
	Camera camera;

	glm::mat4 modelMatrix;
};