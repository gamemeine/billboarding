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
    GLFWwindow *_window;
    Scene _scene;

	bool firstMouse;
	float lastX;
	float lastY;

	static void UpdateMouse(GLFWwindow* window, double xpos, double ypos);
	static void UpdateScroll(GLFWwindow* window, double xoffset, double yoffset);
	void HandleMouse(float xpos, float ypos);
	void HandleScroll(float xoffset, float yoffset);
};