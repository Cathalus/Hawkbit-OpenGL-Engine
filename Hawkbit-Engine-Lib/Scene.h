#pragma once
#include "ModelManager.h"
#include "Camera.h"

namespace hawk{
namespace graphics{
class Scene
{
public:
	Scene(GLint width, GLint height) :_width(width), _height(height){}
	~Scene(){}
	virtual void update(float delta) = 0;
	virtual void render() = 0;
	virtual void loadAssets() = 0;
	virtual void unloadAssets() = 0;
	inline void addCamera(Camera &camera)
	{
		_cameras.push_back(std::shared_ptr<Camera>(new Camera(camera)));
		_activeCamera = _cameras.front();
	}
protected:
	virtual void init() = 0;
	
	std::shared_ptr<Camera> _activeCamera;
	std::vector<std::shared_ptr<Camera>> _cameras;
		
	GLint _width;
	GLint _height;

	Shader _activeShader;
	UniformManager _uniformManager;
	ModelManager _modelManager;
};
}
}