#pragma once
#include "Mesh.h"
#include "Material.h"

#include <unordered_map>
#include <optional>
#include <vector>

class Model
{
public:
    Model();
    ~Model();

    bool Load(const std::string& filepath, bool flipUVs = false);

    void SetMaterial(const Material& mat);

    void Draw(Program& program);

private:
    std::vector<Mesh> _meshes;
    std::unordered_map<std::string, Texture> _textures;
    Material _material;
};