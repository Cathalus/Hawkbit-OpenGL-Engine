#include "Mesh.h"

namespace hawk{
namespace graphics{
	Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::shared_ptr<Texture> texture, bool instanced, GLuint vao)
{
	_vertices = vertices;
	_indices = indices;
	_instanced = instanced;
	_VAO = vao;
	_texture = texture;
	_indicesCount = _indices.size();

	createMesh();

	_vertices.clear();
	_indices.clear();
}

void Mesh::createMesh()
{
	if (_VAO == 0)
	{
		glGenVertexArrays(1, &_VAO);
	}
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	// Bind VAO
	glBindVertexArray(_VAO);

	// Bind VBO and Buffer Data
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(_vertices[0]), &_vertices[0], GL_STATIC_DRAW);

	// Bind EBO and Buffer Data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(_indices[0]), &_indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
	
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	// Vertex TexCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));

	// Vertex Tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
	glBindVertexArray(0);
}

void Mesh::draw(Shader &shader, UniformManager &uniformManager)
{
	/* Upload Material Data */
	uniformManager.updateUniformData("material.ambient", Material.Ambient);
	uniformManager.updateUniformData("material.diffuse", Material.Diffuse);
	uniformManager.updateUniformData("material.specular", Material.Specular);
	uniformManager.updateUniformData("material.shininess", Material.Shininess);
	uniformManager.updateUniformData("material.shininessStrength", Material.ShininessStrength);

	/* Upload Texture Data */
	
	for (std::shared_ptr<TextureData> t : _texture->getTextureData())
	{
		_hasNormalTexture = false;
		switch (t->getTextureType())
		{
		case DIFFUSE:
			t->bind(0);
			uniformManager.updateUniformData("texture_diffuse", 0);
			break;
		case NORMAL:
			t->bind(1);
			uniformManager.updateUniformData("texture_normal", 1);
			_hasNormalTexture = true;
			break;
		case DEPTH:
			t->bind(2);
			uniformManager.updateUniformData("texture_depth", 2);
			break;
		}
	}
	uniformManager.updateUniformData("hasNormalTexture", _hasNormalTexture);
	uniformManager.uploadUniformData(shader);
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


Mesh::~Mesh()
{

}

}
}