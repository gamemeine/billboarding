#pragma once
#include "Texture.h"
#include "Program.h"
#include "Mesh.h"
#include "Camera.h"

/* World-oriented billboard */
class WOBillboard
{
public:
	void Init(Texture texture);

	void Draw(const Camera& camera, glm::vec3 position, glm::vec2 size);

private:
	Texture _texture;
	static Program s_program;
	static Mesh s_mesh;
};