#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <stb_image.h>

#include "Program.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox();
	void Init(std::string resourcePath, unsigned int variant);
	void Draw(Camera camera);

private:
	GLuint _vao, _vbo, _ebo;
	GLuint _texture;
	Program shader;

	GLuint loadCubemap(std::vector<std::string> faces);
};