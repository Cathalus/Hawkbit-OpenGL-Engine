#include "Texture.h"

namespace hawk {
namespace graphics {
TextureData::TextureData(GLenum textureTarget, int width, int height, unsigned char* data, GLfloat filter, GLenum format, GLenum internalFormat, bool clamp, TextureType type)
{
	_width = width;
	_height = height;
	_textureTarget = textureTarget;
	_type = type;

	/* Generate Texture */
	glGenTextures(1, &_textureID);
	glBindTexture(_textureTarget, _textureID);

	/* Set Texture Parameters */
	if (clamp)
	{
		glTexParameterf(_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else{
		glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexParameterf(_textureTarget, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameterf(_textureTarget, GL_TEXTURE_MAG_FILTER, filter);

	glTexImage2D(_textureTarget, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	if (filter == GL_NEAREST_MIPMAP_LINEAR ||
		filter == GL_NEAREST_MIPMAP_NEAREST ||
		filter == GL_LINEAR_MIPMAP_LINEAR ||
		filter == GL_LINEAR_MIPMAP_NEAREST)
	{
		// Generates MipMap and sets anisotropic filtering to a maximum of 8
		glGenerateMipmap(_textureTarget);
		// Check if anisotropic filtering is supported
		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	}
	else{
		glTexParameteri(_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
	}
}

std::shared_ptr<TextureData> Texture::generateTexture(const std::string &fileName, TextureType type, GLfloat filters, bool clamp)
{
	switch (type)
	{
	case DIFFUSE:
		return generateDiffuseTexture(fileName, filters, clamp);
	case NORMAL:
		return generateNormalTexture(fileName, filters, clamp);
	case DEPTH:
		break;
	}
}

std::shared_ptr<TextureData> Texture::generateDiffuseTexture(const std::string &fileName, GLfloat filter, bool clamp)
{
	std::shared_ptr<TextureData> result;
	int width = 0, height = 0;

	std::cout << "Loading Texture " << fileName.c_str() << " ..." << std::endl;
	unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	std::cout << SOIL_last_result() << std::endl;

	TextureData texture(GL_TEXTURE_2D, width, height, image, filter, GL_RGBA, GL_RGBA, false, DIFFUSE);
	texture.setTextureName(fileName);
	
	result = std::shared_ptr<TextureData>(new TextureData(texture));
	_data.push_back(result);

	delete image;
	return result;
}

std::shared_ptr<TextureData> Texture::generateNormalTexture(const std::string &fileName, GLfloat filter, bool clamp)
{
	std::shared_ptr<TextureData> result;
	int width = 0, height = 0;

	std::cout << "Loading Normal Texture " << fileName.c_str() << " ..." << std::endl;
	unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	std::cout << SOIL_last_result() << std::endl;

	TextureData texture(GL_TEXTURE_2D, width, height, image, filter, GL_RGB, GL_RGB, false, NORMAL);
	texture.setTextureName(fileName);

	result = std::shared_ptr<TextureData>(new TextureData(texture));
	_data.push_back(result);

	delete image;
	return result;
}

bool Texture::hasTexture(std::string texName)
{
	if (_data.size() != 0)
	{
		for (int i = 0; i < _data.size(); i++)
		{
			if (_data[i]->getTextureName() == texName)
				return true;
		}
	}
	return false;
}

void TextureData::bind(GLuint textureUnit)
{
	assert(textureUnit >= 0 && textureUnit <= 31);
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(_textureTarget, _textureID);
}


}
}