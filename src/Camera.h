#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:

	Camera();
	//~Camera();
	void Init(float fov, glm::vec3 pos, glm::vec3 direction, float camSpeed);

	void Update(GLFWwindow* window);
	void UpdateDirection(float xoffset, float yoffset);
	void UpdateZoom(float xoffset, float yoffset);
	void Activate();
	void Deactivate();

	glm::mat4& GetViewMatrix();
	glm::mat4& GetProjectionMatrix();
	glm::vec3& GetCameraPos();

private:
	float fieldOfView;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;
	float cameraSpeed;
	float aspectRatio;

	float yaw;
	float pitch;
	float sensitivity;
	bool movable;
	bool lshiftpressed;
};