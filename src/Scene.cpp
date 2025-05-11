#include "Scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

static void createBox(Mesh& mesh);


Scene::Scene()
{

}

Scene::~Scene()
{
    
}

void Scene::Init()
{
    shader.Load("shader.vs.glsl", "shader.fs.glsl");

    boxTexture.Load("../res/textures/box.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    createBox(box);
    box.SetDiffuseTexture(boxTexture);

    model.Load("../res/models/marble_bust/marble_bust_01_1k.fbx");

    chair.Load("../res/models/Chair.glb", true);

    fieldOfView = 45;
    cameraPosition = glm::vec3(0, 0, 20);
    cameraDirection = glm::vec3(0, 0, -1);
    cameraUp = glm::vec3(0, 1, 0);
    cameraSpeed = 0.05f;

    float aspectRatio = 16.0f / 9.0f;
    projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 100.0f);
}

void Scene::Update(GLFWwindow *window)
{
    viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition.z -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition.z += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition.x -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition.x += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        cameraPosition.y -= cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        cameraPosition.y += cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        cameraDirection = glm::vec3(0, 0, -1);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        cameraDirection = glm::vec3(0, 0, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        cameraDirection = glm::vec3(-1, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        cameraDirection = glm::vec3(1, 0, 0);
    }
}

void Scene::Render()
{
    shader.Activate();

    glUniformMatrix4fv(shader.GetUniformID("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader.GetUniformID("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(i * 3.0f, j * 3.0f, -8.0f));
            glUniformMatrix4fv(shader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

            box.Draw(shader);
        }
    }

    modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
    glUniformMatrix4fv(shader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    chair.Draw(shader);

    modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(5.0f));
    modelMatrix = glm::rotate(modelMatrix, -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -0.5f));
    glUniformMatrix4fv(shader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    model.Draw(shader);
}



void createBox(Mesh& mesh)
{

   std::vector<glm::vec3> positions;
   // Front face
   positions.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
   // Back face
   positions.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
   positions.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
   positions.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
   // Top face
   positions.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
   positions.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
   positions.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
   // Bottom face
   positions.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
   // Left face
   positions.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3(-1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3(-1.0f,  1.0f,  1.0f));
   // Right face
   positions.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));
   positions.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));
   positions.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3( 1.0f, -1.0f,  1.0f));
   positions.push_back(glm::vec3( 1.0f, -1.0f, -1.0f));
   positions.push_back(glm::vec3( 1.0f,  1.0f, -1.0f));

   std::vector<glm::vec3> normals;
   // Front face
   normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
   // Back face
   normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
   normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
   // Top face
   normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
   // Bottom face
   normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
   normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
   // Left face
   normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
   // Right face
   normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
   normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

   std::vector<glm::vec2> texCoord;
   // Front face
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   // Back face
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   // Top face
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   // Bottom face
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   // Left face
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   // Right face
   texCoord.push_back(glm::vec2(1.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 1.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(0.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 0.0f));
   texCoord.push_back(glm::vec2(1.0f, 1.0f));

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    for (size_t i = 0; i < positions.size(); i++)
    {
        vertices.push_back({ positions[i], normals[i], texCoord[i] });
        indices.push_back(i);
    }

    mesh.Init(vertices, indices);
}