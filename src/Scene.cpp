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

    camera.Init(45, glm::vec3(0, 0, 20), glm::vec3(0, 0, -1), 0.05f);
}

void Scene::Update(GLFWwindow *window)
{
    // UPDATE CAMERA
    camera.Update(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // INPUT FOR BLOCKING CAMERA MOVEMENT
    static float lastPress = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && (glfwGetTime() - lastPress) > 0.5f)
    {
        switch (glfwGetInputMode(window, GLFW_CURSOR))
        {
        case GLFW_CURSOR_DISABLED:
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            camera.Deactivate();
            break;
        }
        default:
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
            camera.Activate();
            break;
        }
        }
        lastPress = glfwGetTime();
    }
}

void Scene::UpdateCameraDirection(float xoffset, float yoffset)
{
    camera.UpdateDirection(xoffset, yoffset);
}

void Scene::UpdateCameraZoom(float xoffset, float yoffset)
{
    camera.UpdateZoom(xoffset, yoffset);
}

void Scene::Render()
{
    shader.Activate();

    glm::vec3 lightPos(10.0f, 10.0f, 10.0f);
    glUniform3fv(shader.GetUniformID("uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(shader.GetUniformID("uLightColor"), 1, glm::value_ptr(glm::vec3(1.0f)));

    glUniform1f(shader.GetUniformID("uAmbientStrength"), 0.1f);
    glUniform1f(shader.GetUniformID("uSpecularStrength"), 0.5f);
    glUniform1i(shader.GetUniformID("uShininess"), 32);

    glUniform3fv(shader.GetUniformID("uViewPos"), 1, glm::value_ptr(camera.GetCameraPos()));
    glUniformMatrix4fv(shader.GetUniformID("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    glUniformMatrix4fv(shader.GetUniformID("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

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