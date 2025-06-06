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
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glViewport(0, 0, width, height);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(_window, OnWindowResize);
    glfwSetCursorPosCallback(_window, Window::UpdateMouse);
    glfwSetScrollCallback(_window, Window::UpdateScroll);
    glfwSetWindowUserPointer(_window, this);

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

    double timePrev = glfwGetTime();

    while (!glfwWindowShouldClose(_window))
    {
        double timeNow = glfwGetTime();
        double deltaTime = timeNow - timePrev;
        timePrev = timeNow;

        _scene.Update(_window, float(deltaTime));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _scene.Render();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void Window::UpdateMouse(GLFWwindow* window, double xpos, double ypos)
{
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance)
    {
        instance->HandleMouse(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

void Window::UpdateScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance)
    {
        instance->HandleScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
}

void Window::HandleMouse(float xpos, float ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    _scene.UpdateCameraDirection(xoffset, yoffset);
}

void Window::HandleScroll(float xoffset, float yoffset)
{
    //std::cerr << "scroll moved \n" << xoffset << " " << yoffset;
    _scene.UpdateCameraZoom(xoffset, yoffset);
}

static void OnWindowResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}