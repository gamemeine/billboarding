#include "Scene.h"
#include "Billboard.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <chrono>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <algorithm>

static void createBox(Mesh& mesh);
static void createQuad(Mesh& mesh);

Scene::Scene()
{

}

Scene::~Scene()
{
    
}

void Scene::Init()
{
    basicShader.Load("shader.vs.glsl", "shader.fs.glsl");
    modelShader.Load("model_shader.vs.glsl", "model_shader.fs.glsl");

    boxTexture.Load("../res/textures/box.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    createBox(box);
    box.SetDiffuseTexture(boxTexture);

    grassTexture.Load("../res/textures/grass.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    createQuad(grass);
    grass.SetDiffuseTexture(grassTexture);

    // Setup tree billboards
    treeTexture.Load("../res/textures/tree.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    trees.clear();

    // Parametry konfiguracji okręgów
    const int numCircles = 5;
    const int treesPerCircle = 80;
    const float startRadius = 75.0f;
    const float radiusStep = 5.0f;
    const float treeHeight = -4.0f;
    const glm::vec2 treeSize = glm::vec2(6.0f, 9.6f);

    for (int circle = 0; circle < numCircles; ++circle) {
        float radius = startRadius + circle * radiusStep;
        float angleOffset = glm::pi<float>() / treesPerCircle * circle;

        for (int i = 0; i < treesPerCircle; ++i) {
            float angle = glm::two_pi<float>() * (float(i) / treesPerCircle) + angleOffset;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;

            trees.emplace_back(&treeTexture, glm::vec3(x, treeHeight, z), treeSize.x, treeSize.y);
        }
    }

    model.Load("../res/models/marble_bust/marble_bust_01_1k.fbx");

    model2.Load("../res/models/marble_bust/marble_bust_01_1k.fbx");

    chair.Load("../res/models/Chair.glb", true);

    Material bustMat;
    bustMat.diffuseColor = glm::vec3(0.9f, 0.85f, 0.8f);
    bustMat.specularColor = glm::vec3(0.5f);
    bustMat.shininess = 64.0f;
    model.SetMaterial(bustMat);

    Material shinyMarble;
    shinyMarble.diffuseColor  = glm::vec3(0.5f, 0.7f, 1.0f);
    shinyMarble.specularColor = glm::vec3(1.0f);
    shinyMarble.shininess     = 128.0f;
    model2.SetMaterial(shinyMarble);

    Material chairMat;
    chairMat.diffuseColor = glm::vec3(0.9f, 0.8f, 0.8f);
    chairMat.specularColor = glm::vec3(0.2f);
    chairMat.shininess = 16.0f;
    chair.SetMaterial(chairMat);

    camera.Init(45, glm::vec3(0, 0, 20), glm::vec3(0, 0, -1), 4.0f);

    cloudTexture1.Load("../res/textures/cloud1.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    cloudTexture2.Load("../res/textures/cloud2.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    cloudTexture3.Load("../res/textures/cloud3.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    clouds.emplace_back(cloudTexture1, glm::vec3(0, 0, 5.0f), glm::vec2(3.0f, 3.0f));
    clouds.emplace_back(cloudTexture2, glm::vec3(10, 10, -10), glm::vec2(5.574f, 4.0f));
    clouds.emplace_back(cloudTexture3, glm::vec3(-15, 20, -30), glm::vec2(10.04f, 4.0f));
    clouds.emplace_back(cloudTexture1, glm::vec3(20, 30, -50), glm::vec2(6.0f));

    skybox.Init("../res/skyboxtx/", 4);
}

void Scene::Update(GLFWwindow *window, float dt)
{
    // UPDATE CAMERA
    camera.Update(window, dt);

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
    modelShader.Activate();

    glm::vec3 lightPos(0.0f, 50.0f, 20.0f);
    glUniform3fv(modelShader.GetUniformID("uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(modelShader.GetUniformID("uLightColor"), 1, glm::value_ptr(glm::vec3(1.0f)));

    glUniform3fv(modelShader.GetUniformID("uViewPos"), 1, glm::value_ptr(camera.GetCameraPos()));
    glUniformMatrix4fv(modelShader.GetUniformID("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    glUniformMatrix4fv(modelShader.GetUniformID("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

    modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
    glUniformMatrix4fv(modelShader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    chair.Draw(modelShader);

    modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(4.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f));
    modelMatrix = glm::rotate(modelMatrix, -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -0.5f));
    glUniformMatrix4fv(modelShader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    model.Draw(modelShader);

    modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-4.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f));
    modelMatrix = glm::rotate(modelMatrix, -glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -0.5f));
    glUniformMatrix4fv(modelShader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    model2.Draw(modelShader);

    basicShader.Activate();

    glUniform3fv(basicShader.GetUniformID("uLightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(basicShader.GetUniformID("uLightColor"), 1, glm::value_ptr(glm::vec3(1.0f)));

    glUniform1f(basicShader.GetUniformID("uAmbientStrength"), 0.8f);
    glUniform1f(basicShader.GetUniformID("uSpecularStrength"), 0.5f);
    glUniform1i(basicShader.GetUniformID("uShininess"), 32);

    glUniform3fv(basicShader.GetUniformID("uViewPos"), 1, glm::value_ptr(camera.GetCameraPos()));
    glUniformMatrix4fv(basicShader.GetUniformID("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    glUniformMatrix4fv(basicShader.GetUniformID("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
    
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(i * 3.0f, j * 3.0f, -8.0f));
            glUniformMatrix4fv(basicShader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

            box.Draw(basicShader);
        }
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -3.5f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(200.0f, 1.0f, 200.0f));
    glUniformMatrix4fv(basicShader.GetUniformID("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    grass.Draw(basicShader);

    // Draw trees as axial billboards
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto& tree : trees) {
        tree.Draw(camera.GetCameraPos(), basicShader);
    }
    glDisable(GL_BLEND);

    // Skybox must be drawn before other billboards/seethrough-objects
    skybox.Draw(camera);

    // Draw clouds from back to front:
    std::sort(clouds.begin(), clouds.end(), [this](WOBillboard& a, WOBillboard& b)
        {
            float distanceToA = glm::distance(a.GetPosition(), camera.GetCameraPos());
            float distanceToB = glm::distance(b.GetPosition(), camera.GetCameraPos());

            return distanceToA > distanceToB;
        });

    for (auto& cloud : clouds) {
        cloud.Draw(camera);
    }
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

void createQuad(Mesh& mesh)
{
    std::vector<Vertex> vertices = {
        {{-0.5f, 0.0f, -0.5f}, {0, 1, 0}, {0.0f, 0.0f}},
        {{ 0.5f, 0.0f, -0.5f}, {0, 1, 0}, {50.0f, 0.0f}},
        {{ 0.5f, 0.0f,  0.5f}, {0, 1, 0}, {50.0f, 50.0f}},
        {{-0.5f, 0.0f,  0.5f}, {0, 1, 0}, {0.0f, 50.0f}},
    };
    std::vector<uint32_t> indices = {
        0, 2, 1,
        0, 3, 2
    };
    mesh.Init(vertices, indices);
}
