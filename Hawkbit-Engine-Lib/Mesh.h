#pragma once
#include "Texture.h"
#include "UniformManager.h"
#include "Shader.h"
#include <vector>
#include <memory>

namespace hawk
{
namespace graphics
{
struct Material {
	glm::vec3 Ambient = glm::vec3(1, 1, 1);
	glm::vec3 Diffuse = glm::vec3(1, 1, 1);
	glm::vec3 Specular = glm::vec3(1, 1, 1);
	float Shininess = 32;
	float ShininessStrength = 0.5;
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec3 Tangent;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::shared_ptr<Texture> texture, bool instanced = false, GLuint vao = 0);
	void draw(Shader &shader,UniformManager &uniformManager);
	~Mesh();

	hawk::graphics::Material Material;
private:
	friend class Shader;
	void createMesh();

	/* Mesh Data */
	std::vector<Vertex> _vertices;
	std::vector<GLuint> _indices;

	GLuint _indicesCount;
	bool _hasNormalTexture = false;

	/* RenderData */
	GLuint _VAO, _VBO, _EBO;

	bool _instanced = false;
	std::shared_ptr<Texture> _texture;
};
}
}
