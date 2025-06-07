#include "Skybox.h"

Skybox::Skybox() : _vao(0), _vbo(0), _ebo(0), _texture(0) {}

void Skybox::Init(std::string resourcePath, unsigned int variant)
{
	if(shader.GetID() == 0)
	{ 
		shader.Load("skybox_shader.vs.glsl", "skybox_shader.fs.glsl");
		//std::cout << "cube shaders loaded\n";

		float skyboxVertices[] = {
			// positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f
		};

		unsigned int skyboxIndices[] = {
			// Back face
			0, 1, 2,
			2, 3, 0,
			// Left face
			4, 5, 1,
			1, 0, 4,
			// Right face
			3, 2, 6,
			6, 7, 3,
			// Front face
			7, 6, 5,
			5, 4, 7,
			// Top face
			4, 0, 3,
			3, 7, 4,
			// Bottom face
			1, 5, 6,
			6, 2, 1
		};

		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	char prefix = variant + 48;
	std::vector<std::string> faces =
	{
		resourcePath + prefix + "posx.jpg",
		resourcePath + prefix + "negx.jpg",
		resourcePath + prefix + "posy.jpg",
		resourcePath + prefix + "negy.jpg",
		resourcePath + prefix + "posz.jpg",
		resourcePath + prefix + "negz.jpg"
	};

	_texture = loadCubemap(faces);
}

void Skybox::Draw(Camera camera)
{
	glDepthFunc(GL_LEQUAL);
	shader.Activate();

	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

	glUniformMatrix4fv(shader.GetUniformID("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(shader.GetUniformID("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

	glBindVertexArray(_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	glBindVertexArray(0);
	shader.Deactivate();
	glDepthFunc(GL_LESS);
}

GLuint Skybox::loadCubemap(std::vector<std::string> faces)
{
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int width, height, nrChannels;
	for (int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load face: " << faces[i] << std::endl;
		
		stbi_image_free(data);
	}

	//std::cout << "cube textures loaded\n";

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture;
}
