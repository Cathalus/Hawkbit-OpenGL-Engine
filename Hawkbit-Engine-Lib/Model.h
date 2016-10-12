#pragma once
#include "Shader.h"
#include "Texture.h"
#include "UniformManager.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

namespace hawk{
namespace graphics{
class Model
{
public:
	Model(const std::string& fileName, bool combine = false)
	{
		loadModel(fileName, combine);
	}
	void draw(Shader &shader, UniformManager &uniformManager, glm::mat4 &transformation);
	glm::mat4 getMatrix() { return _matrix; }
	void setMatrix(glm::mat4 matrix) { _matrix = matrix; }
private:
	void loadModel(const std::string& fileName, bool combine);
	void processNode(aiNode *node, const aiScene *scene);
	void loadMaterialTextures(aiMaterial *material, aiTextureType type, std::shared_ptr<Texture> texture);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	
	std::string _defaultModelLocation = "data/models/";
	std::string _directory;
	std::vector<Mesh> _meshes;
	std::map<std::string, std::shared_ptr<TextureData>> _loadedTextures;
	glm::mat4 _matrix;
	GLuint _VAO;
};
}
}
