#pragma once
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <map>

namespace hawk{
namespace graphics{
class UniformManager
{
private:
	friend class Shader;
public:
	void updateUniformData(std::string uniformName, glm::mat4& mat4);
	void updateUniformData(std::string uniformName, glm::mat3& mat3);
	void updateUniformData(std::string uniformName, glm::vec4& vec4);
	void updateUniformData(std::string uniformName, glm::vec3& vec3);
	void updateUniformData(std::string uniformName, glm::vec2& vec2);
	void updateUniformData(std::string uniformName, float f);
	void updateUniformData(std::string uniformName, unsigned int ui);
	void updateUniformData(std::string uniformName, int i);
	void uploadUniformData(Shader shader);
private:
	std::map<std::string, glm::mat4> _mat4;
	std::map<std::string, glm::mat3> _mat3;
	std::map<std::string, glm::vec4> _vec4;
	std::map<std::string, glm::vec3> _vec3;
	std::map<std::string, glm::vec2> _vec2;
	std::map<std::string, float> _float;
	std::map<std::string, unsigned int> _uint;
	std::map<std::string, int> _int;
};
}
}