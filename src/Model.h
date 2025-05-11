#pragma once
#include "Mesh.h"

#include <unordered_map>
#include <optional>

class Model
{
public:
    Model();
    ~Model();

    bool Load(const std::string& filepath, bool flipUVs = false);

    void Draw(Program& program);

private:
    std::vector<Mesh> _meshes;
    std::unordered_map<std::string, Texture> _textures;
};