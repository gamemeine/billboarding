#pragma once
#include "Scene.h"

#include <string>


struct GLFWwindow;

class Window
{
public:
	Window();
	~Window();

	bool Init(int width, int height, const std::string& title);
	void RunMainLoop();

private:

private:
    GLFWwindow *_window;
    Scene _scene;
};