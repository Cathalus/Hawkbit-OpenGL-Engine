#pragma once

#include "Model.h"
#include "Shader.h"
#include "UniformManager.h"
#include <memory>

namespace hawk{
namespace core{
class GameObject
{
public:
	GameObject(std::shared_ptr<graphics::Model> model) :_model(model){};
	glm::mat4 getMatrix() { return _matrix; }
	void setMatrix(glm::mat4 &matrix) { _matrix = matrix; }
	inline void draw(graphics::Shader &shader, graphics::UniformManager &uniformManager)
	{
		_model->draw(shader, uniformManager, _matrix);
	}
private:
	std::shared_ptr<graphics::Model> _model;
	glm::mat4 _matrix = glm::mat4(1.0f);
};
}
}

