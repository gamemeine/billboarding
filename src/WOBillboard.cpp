#include "WOBillboard.h"

#include <array>

Program WOBillboard::s_program;
Mesh WOBillboard::s_mesh;

void WOBillboard::Init(Texture texture)
{
	if (s_program.GetID() == 0)
	{
		s_program.Load("world_oriented_billboard.vs.glsl", "world_oriented_billboard.fs.glsl");

		
		std::array vertices =
		{
			Vertex{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f) },
			Vertex{ glm::vec3(+0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f) },
			Vertex{ glm::vec3(+0.5f, +0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f) },
			Vertex{ glm::vec3(-0.5f, +0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f) },
		};

		std::array<uint32_t, 6> indices =
		{
			0, 1, 2,
			2, 3, 0
		};

		s_mesh.Init(vertices, indices);
	}

	_texture = texture;
}

void WOBillboard::Draw(const Camera& camera, glm::vec3 position, glm::vec2 size)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s_program.Activate();

	glUniformMatrix4fv(s_program.GetUniformID("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(s_program.GetUniformID("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

	glUniform3fv(s_program.GetUniformID("uPosition"), 1, glm::value_ptr(position));
	glUniform2fv(s_program.GetUniformID("uSize"), 1, glm::value_ptr(size));


	s_mesh.SetDiffuseTexture(_texture);
	s_mesh.Draw(s_program);

	s_program.Deactivate();

	glDisable(GL_BLEND);
}
