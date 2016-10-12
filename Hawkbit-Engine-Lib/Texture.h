#pragma once

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <memory>

namespace hawk {
namespace graphics {
enum TextureType {
	DIFFUSE, NORMAL, DEPTH
};
class TextureData;
class Texture
{
public:
	std::vector<std::shared_ptr<TextureData>> &getTextureData() { return _data; }
	inline void setDirectory(std::string directory) { _defaultTextureLocation = directory; }
	bool hasTexture(std::string texName);
	std::shared_ptr<TextureData> generateTexture(const std::string &fileName, TextureType type, GLfloat filters, bool clamp);
	void addTexture(std::shared_ptr<TextureData> data) { _data.push_back(data); }
private:
	std::shared_ptr<TextureData> generateDiffuseTexture(const std::string &fileName, GLfloat filters, bool clamp);
	std::shared_ptr<TextureData> generateNormalTexture(const std::string &fileName, GLfloat filters, bool clamp);
	std::vector<std::shared_ptr<TextureData>> _data = std::vector<std::shared_ptr<TextureData>>();
	std::string _defaultTextureLocation = "data/textures/";
};
class TextureData
{
public:
	TextureData(GLenum textureTarget, int width, int height, unsigned char* data, GLfloat filters, GLenum format, GLenum internalFormat, bool clamp, TextureType type = DIFFUSE);
	void bind(GLuint textureUnit);
	void bindAsRenderTarget();
	inline TextureType getTextureType() { return _type; }
	inline GLuint getTextureID() { return _textureID; }
	inline void setTextureName(std::string name) { _texName = name; }
	inline std::string getTextureName() { return _texName; }
private:
	std::string _texName = "";
	TextureType _type = DIFFUSE;
	int _width, _height;
	GLenum _textureTarget;
	GLuint _textureID;
};
}
}