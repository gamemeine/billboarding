#include "Camera.h"
Camera::Camera() {}

void Camera::Update(GLFWwindow* window, float dt)
{
    if (!movable)
        return;

    viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += cameraDirection * cameraSpeed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= cameraDirection * cameraSpeed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cameraPosition.y -= cameraSpeed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraPosition.y += cameraSpeed * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !lshiftpressed)
    {
        lshiftpressed = true;
        cameraSpeed *= 4;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && lshiftpressed)
    {
        lshiftpressed = false;
        cameraSpeed *= 0.25;
    }
}

void Camera::UpdateDirection(float xoffset, float yoffset)
{
    if (!movable)
        return;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;


    glm::vec3 view;
    view.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    view.y = sin(glm::radians(pitch));
    view.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection = glm::normalize(view);

}

void Camera::UpdateZoom(float xoffset, float yoffset)
{
    yoffset *= 2;
    fieldOfView -= yoffset;
    if (fieldOfView < 1.0f)
        fieldOfView = 1.0f;
    if (fieldOfView > 45.0f)
        fieldOfView = 45.0f;

    //std::cerr << "FOV: " << fieldOfView << std::endl;
    projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 1000.0f);
}

void Camera::Activate()
{
    movable = true;
}

void Camera::Deactivate()
{
    movable = false;
}

void Camera::Init(float fov, glm::vec3 pos, glm::vec3 direction, float camSpeed)
{
    movable = true;
    lshiftpressed = false;

    fieldOfView = fov;
    cameraPosition = pos;
    cameraDirection = direction;
    cameraUp = glm::vec3(0, 1, 0);
    cameraSpeed = camSpeed;
    aspectRatio = 16.0f / 9.0f;

    projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 1000.0f);
    viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);

    yaw = -90.0f;
    pitch = 0.0f;
    sensitivity = 0.2f;
}

const glm::mat4& Camera::GetViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

const glm::vec3& Camera::GetCameraPos() const
{
    return cameraPosition;
}