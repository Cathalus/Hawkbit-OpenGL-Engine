#pragma once
#include <map>
#include "Model.h"

namespace hawk{
namespace graphics{
class ModelManager{
public:
	void loadModel(std::string name, std::string fileName, bool combine = false)
	{
		// check if model is already loaded
		if (_models.find(name) == _models.end())
		{
			_models[name] = std::shared_ptr<Model>(new Model(fileName,combine));
		}
	}
	void unloadModel(std::string name)
	{
		if (_models.find(name) != _models.end())
			_models.erase(name);
	}
	std::shared_ptr<Model> getModel(std::string name)
	{
		if (_models.find(name) == _models.end())
		{
			return NULL;
		}
		else{
			return _models[name];
		}
	}
private:
	std::map <std::string, std::shared_ptr<Model>> _models;
};
}
}