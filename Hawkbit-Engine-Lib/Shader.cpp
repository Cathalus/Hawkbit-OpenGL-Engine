#include "Shader.h"


namespace hawk
{
namespace graphics
{

void Shader::createShader(std::string shaderName, std::vector<std::string> fileList)
{
	bool isValidated = true;

	GLuint programID;
	GLuint shaderID;

	// Create Shader Program
	programID = glCreateProgram();

	// Load shader programs
	std::size_t found;
	for (auto a : fileList)
	{
		/* Vertex Shader */
		found = a.find(".vert");
		if (found != std::string::npos)
		{
			shaderID = createShader(GL_VERTEX_SHADER, (_defaultShaderLocation + a).c_str());
			isValidated = isValidated && checkShader(shaderID, (_defaultShaderLocation + a).c_str());

			glAttachShader(programID, shaderID);
			glDeleteShader(shaderID);
		}
		/* Fragment Shader */
		found = a.find(".frag");
		if (found != std::string::npos)
		{
			shaderID = createShader(GL_FRAGMENT_SHADER, (_defaultShaderLocation + a).c_str());
			isValidated = isValidated && checkShader(shaderID, (_defaultShaderLocation + a).c_str());

			glAttachShader(programID, shaderID);
			glDeleteShader(shaderID);
		}
	}

	// Link the program
	glLinkProgram(programID);
	isValidated = isValidated && checkProgram(programID);

	if (!isValidated)
	{
		glDeleteProgram(programID);
		assert(false);
	}
	_programID = programID;
}

GLuint Shader::createShader(GLenum type, const std::string& fileName)
{
	GLuint shaderID = 0;

	if (!fileName.empty())
	{
		// Load Shader Source
		std::string content = loadShader(fileName);
		const char* contentPtr = content.c_str();
		shaderID = glCreateShader(type);
		glShaderSource(shaderID, 1, &contentPtr, NULL);
		glCompileShader(shaderID);
	}

	return shaderID;
}

std::string Shader::loadShader(const std::string& fileName)
{
	std::string result;

	std::ifstream stream(fileName.c_str());
	if (!stream.is_open())           // if stream isn't opened -> return null string
		return result;
	result.reserve(stream.tellg()); // reserve enough space for the program
	stream.seekg(0, std::ios::beg); // seek to beginning

	// fill result
	result.assign((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());

	return result;
}

bool Shader::validateProgram(GLuint programID)
{
	if (!programID)
		return false;

	glValidateProgram(programID);
	GLint result = GL_FALSE;
	// Get validation status
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);

	// Error has occured
	if (result == GL_FALSE)
	{
		std::cerr << "Error: Program is invalid! " << std::endl;
		int infoLogLength;
		// Get info log length
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> buffer(infoLogLength);
			// Get info log | http://docs.gl/gl4/glGetProgramInfoLog
			glGetProgramInfoLog(programID, infoLogLength, NULL, &buffer[0]);
			fprintf(stdout, "%s\n", &buffer[0]);
		}
	}

	return result == GL_TRUE;
}

bool Shader::checkProgram(GLuint programID)
{
	if (!programID)
		return false;

	GLint result = GL_FALSE;
	// Get link status
	glGetProgramiv(programID, GL_LINK_STATUS, &result);

	fprintf(stdout, "Linking program...\n");
	int infoLogLength;
	// Get info log length
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{

		std::vector<char> buffer(infoLogLength < 1 ? int(1) : infoLogLength);
		// Get info log
		glGetProgramInfoLog(programID, infoLogLength, NULL, &buffer[0]);
		fprintf(stdout, "%s\n", &buffer[0]);
	}

	return result == GL_TRUE;
}

bool Shader::checkShader(GLuint shaderName, const std::string &fileName)
{
	if (!shaderName)
		return false;

	GLint result = GL_FALSE;
	// Get compile status
	glGetShaderiv(shaderName, GL_COMPILE_STATUS, &result);

	fprintf(stdout, "Compiling Shader '%s' ...", fileName.c_str());
	int infoLogLength;
	// Get info log length
	glGetShaderiv(shaderName, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> buffer(infoLogLength);
		// Get info log
		glGetShaderInfoLog(shaderName, infoLogLength, NULL, &buffer[0]);
		fprintf(stdout, "%s\n", &buffer[0]);
	}
	return result == GL_TRUE;
}

void Shader::use()
{
	glUseProgram(_programID);
}

}
}