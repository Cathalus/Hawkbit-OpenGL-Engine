#include "Model.h"

namespace hawk{
namespace graphics{
void Model::loadModel(const std::string& fileName, bool combine)
{
	if (combine)
		glGenVertexArrays(1, &_VAO);
	std::string filePath = (_defaultModelLocation + fileName);
	std::cout << "Loading Model " << filePath << std::endl;
	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
	if (combine)
	{
		flags |= aiProcess_PreTransformVertices;
	}
	else{
		flags |= aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes;
	}
	const aiScene* scene = importer.ReadFile(filePath, flags);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	_directory = filePath.substr(0, filePath.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	/* Process all Meshes */
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(processMesh(mesh, scene));
	}
	/* Process all Child Nodes */
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(new Texture());

	/* Vertex Processing */
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 data;
		
		/* Position */
		data.x = mesh->mVertices[i].x;
		data.y = mesh->mVertices[i].y;
		data.z = mesh->mVertices[i].z;
		vertex.Position = data;

		/* Normals */
		if (mesh->HasNormals())
		{
			data.x = mesh->mNormals[i].x;
			data.y = mesh->mNormals[i].y;
			data.z = mesh->mNormals[i].z;
			vertex.Normal = data;
		}

		/* Tangents and Bitangents */
		if (mesh->HasTangentsAndBitangents())
		{
			data.x = mesh->mTangents[i].x;
			data.y = mesh->mTangents[i].y;
			data.z = mesh->mTangents[i].z;
			vertex.Tangent = data;
		}

		/* Texture Coordinates */
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = vec;
		}
		vertices.push_back(vertex);
	}
	
	/* Index Processing */
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	/* Material Processing */
	if (scene->HasMaterials())
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		/* Diffuse Textures */
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			loadMaterialTextures(material, aiTextureType_DIFFUSE, texture);
		}
		/* Normal Textures */
		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			loadMaterialTextures(material, aiTextureType_NORMALS, texture);
		}
	}
	
	

	Mesh result = Mesh(vertices, indices, texture, false, _VAO);

	if (scene->HasMaterials())
	{
		/* Material Properties */
		aiColor3D color(0.f, 0.f, 0.f);
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// Diffuse Color
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
			result.Material.Diffuse = glm::vec3(color.r, color.g, color.b);
		// Ambient Color
		if (material->Get(AI_MATKEY_COLOR_AMBIENT , color) == AI_SUCCESS)
			result.Material.Ambient = glm::vec3(color.r, color.g, color.b);
		// Specular Color
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
			result.Material.Specular = glm::max(glm::vec3(color.r, color.g, color.b), glm::vec3(0.1, 0.1, 0.1));
		// Shininess
		float shininess = 0.0f;
		if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
			result.Material.Shininess = glm::max(shininess, 8.0f);
		// Shininess Strength
		float shininessStrength = 0.0f;
		if (material->Get(AI_MATKEY_SHININESS, shininessStrength) == AI_SUCCESS)
			result.Material.ShininessStrength = shininessStrength;
	}

	return result;
}

void Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::shared_ptr<Texture> texture)
{
	for (GLuint i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		std::string texturePath = _directory + "/" + str.C_Str();

		if (_loadedTextures[texturePath] != nullptr)
		{
			texture->addTexture(_loadedTextures[texturePath]);
		}
		else{
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				_loadedTextures[texturePath] = texture->generateTexture(texturePath, DIFFUSE, GL_LINEAR_MIPMAP_LINEAR, false);
				break;
			case aiTextureType_NORMALS:
				_loadedTextures[texturePath] = texture->generateTexture(texturePath, NORMAL, GL_LINEAR_MIPMAP_LINEAR, false);
				break;
			}
		}
	}
}

void Model::draw(Shader &shader, UniformManager &uniformManager, glm::mat4 &transformation)
{
	shader.use();
        glm::mat4 result = transformation*_matrix;
	uniformManager.updateUniformData("model", result);
	for (Mesh m : _meshes)
	{
		m.draw(shader, uniformManager);
	}
}

}
}