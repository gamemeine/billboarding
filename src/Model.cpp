#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>


static glm::vec3 aiVecToGLM(aiVector3D v) { return glm::vec3(v.x, v.y, v.z); }


Model::Model()
{

}

Model::~Model()
{
    
}

bool Model::Load(const std::string& filepath, bool flipUVs)
{
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    unsigned int flags = 0;
    flags |= aiProcess_Triangulate;
    flags |= aiProcess_GenSmoothNormals;
    flags |= aiProcess_CalcTangentSpace;

    if (flipUVs)
        flags |= aiProcess_FlipUVs;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath.c_str(), flags);
    if (!scene)
        return false;

    auto modelFile = std::filesystem::path(filepath);
    auto modelDir = modelFile.parent_path();
    
    _meshes.reserve(scene->mNumMeshes);

    // @TODO: Traverse nodes and transform the meshes (aiNode::mTransformation)

    for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
    {
        aiMesh *sceneMesh = scene->mMeshes[meshIndex];

        vertices.clear();
        indices.clear();

        for (int i = 0; i < sceneMesh->mNumVertices; i++)
        {
            glm::vec3 position = aiVecToGLM(sceneMesh->mVertices[i]);
            glm::vec3 normal = aiVecToGLM(sceneMesh->mNormals[i]);
            glm::vec2 texCoord = glm::vec2(0);

            if (sceneMesh->mTextureCoords[0])
                texCoord = aiVecToGLM(sceneMesh->mTextureCoords[0][i]);

            vertices.push_back({ position, normal, texCoord });
        }
        for (int i = 0; i < sceneMesh->mNumFaces; i++)
        {
            aiFace face = sceneMesh->mFaces[i];
        
            for (int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }

        Mesh mesh;
        mesh.Init(vertices, indices);

        aiMaterial* material = scene->mMaterials[sceneMesh->mMaterialIndex];
        if (material)
        {
            aiString aipath;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aipath) == aiReturn_FAILURE)
            {
                material->GetTexture(aiTextureType_BASE_COLOR, 0, &aipath);
            }

            std::string path = std::string(aipath.C_Str());
            std::replace(path.begin(), path.end(), '\\', '/');  // Convert to POSIX path

            auto it = _textures.find(path);
            if (it == _textures.end())
            {
                Texture diffuse;

                if (path[0] == '*')
                {
                    // @TODO: Load embedded texture
                    char *endptr;
                    long index = std::strtoul(path.c_str() + 1, &endptr, 10);

                    if (endptr > path.c_str() + 1)
                    {
                        //scene->mTextures[index]->pcData;
                    }
                }
                else
                {
                    std::string fullPath = modelDir.string() + "/" + path;
                    diffuse.Load(fullPath, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
                }

                if (diffuse.GetID())
                {
                    mesh.SetDiffuseTexture(diffuse);
                    _textures[path] = diffuse;
                }
            }
            else
            {
                mesh.SetDiffuseTexture(it->second);
            }

            // @TODO: Other material properties
            //material->Get(AI_MATKEY_COLOR_SPECULAR, 
        }
        _meshes.push_back(mesh);
    }

    return true;
}

void Model::SetMaterial(const Material& mat) {
    _material = mat;
}

void Model::Draw(Program& program)
{
    for (Mesh& mesh : _meshes)
    {
        glUniform3fv(program.GetUniformID("uMaterial.diffuse"), 1, glm::value_ptr(_material.diffuseColor));
        glUniform3fv(program.GetUniformID("uMaterial.specular"), 1, glm::value_ptr(_material.specularColor));
        glUniform1f (program.GetUniformID("uMaterial.shininess"), _material.shininess);

        mesh.Draw(program);
    }
}
