#pragma once
#include "Program.h"
#include "Mesh.h"
#include "Model.h"
#include "WOBillboard.h"
#include "Billboard.h"
#include "Skybox.h"

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
    Texture treeTexture;
    Mesh box;
	Mesh grass;
    std::vector<Billboard> trees;
    Program basicShader;
	Program modelShader;
	Model model;
	Model model2;
	Model model3;
	Model chair;
	WOBillboard cloud1;
	WOBillboard cloud2;
	WOBillboard cloud3;
	
	std::vector<WOBillboard> clouds;

	Skybox skybox;

	Texture cloudTexture1;
	Texture cloudTexture2;
	Texture cloudTexture3;
	Texture grassTexture;
	Camera camera;

	glm::mat4 modelMatrix;

	glm::mat4 box1Matrix;
    glm::mat4 box2Matrix;
    glm::mat4 box3Matrix;
    glm::mat4 box4Matrix;
    glm::mat4 box5Matrix;
};