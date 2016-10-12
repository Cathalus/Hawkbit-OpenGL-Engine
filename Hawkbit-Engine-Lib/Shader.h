#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>

namespace hawk
{
namespace graphics
{
class Shader
{
public:
	/**
	* Creates a new shader with the given parameters
	* @param shaderName Name of the Shader
	* @param fileList List of all associated shader files
	*/
	void createShader(std::string shaderName, std::vector<std::string> fileList);
	void draw();
	void use();

	inline GLuint getProgramID() { return _programID; }
private:
	friend class Mesh;

	GLuint createShader(GLenum type, const std::string& fileName);
	std::string loadShader(const std::string& fileName);
	bool checkShader(GLuint shaderName, const std::string& file);
	bool checkError(const char* title);
	bool checkProgram(GLuint programID);
	bool validateProgram(GLuint programID);

	std::string _defaultShaderLocation = "data/shaders/";
	GLuint _programID = 0;
};
}
}
