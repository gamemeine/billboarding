#pragma once
#include "Texture.h"
#include "Program.h"
#include "Mesh.h"
#include "Camera.h"

/* World-oriented billboard */
class WOBillboard
{
public:
	WOBillboard();
	WOBillboard(Texture texture, const glm::vec3& position, const glm::vec2& size);
	void Init(Texture texture);

	void SetPosition(const glm::vec3& position);
	void SetSize(const glm::vec2& size);

	const glm::vec3& GetPosition() const;

	void Draw(const Camera& camera);

private:
	Texture _texture;
	glm::vec3 _position;
	glm::vec2 _size;
	static Program s_program;
	static Mesh s_mesh;
};