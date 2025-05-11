#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


static void OnWindowResize(GLFWwindow *window, int width, int height);


Window::Window() : _window(nullptr)
{

}


Window::~Window()
{
    glfwTerminate();
}


bool Window::Init(int width, int height, const std::string& title)
{
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // Enable forward‐compat on macOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!_window)
    {
        std::cerr << "GLFW window creation failed!" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(_window, OnWindowResize);

    return true;
}


void Window::RunMainLoop()
{
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    _scene.Init();

    while (!glfwWindowShouldClose(_window))
    {
        _scene.Update(_window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _scene.Render();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}



static void OnWindowResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}