#pragma once
#include <Scene.h>
#include <Camera.h>
#include <GameObject.h>
#include <LightSource.h>
#include <SDL2/SDL.h>
#include <math.h>

class GameplayScene : public hawk::graphics::Scene
{
public:
	GameplayScene(GLint width, GLint height) : hawk::graphics::Scene(width, height){ 
		loadAssets();
		init(); 
	};
	~GameplayScene(){ unloadAssets(); }
	void update(float delta);
	void render();
private:
	void init();
	void loadAssets();
	void unloadAssets();
	void handleInput(float delta);
	void uploadLightSources();

	/* Controls */
	GLfloat _lastX, _lastY;
	GLfloat _yaw = -90.0f, _pitch = 0;
	float _speed = 60.0f;
	float _movementSpeed = 6.0f;
        
        /* Accumulator */
        float _acc = 0;

	std::shared_ptr<hawk::graphics::Camera> _mainCamera;
	
	/* Lights */
	hawk::graphics::DirectionalLight _directionalLight;
	hawk::graphics::SpotLight _flashLight;
	std::vector<std::shared_ptr<hawk::graphics::LightSource>> _lights;

	hawk::graphics::Shader _lampShader;

	/* GameObjects */
	std::vector<hawk::core::GameObject> _objects;
	std::vector<hawk::core::GameObject> _lamps;
	std::vector<glm::vec3> _lampColors;
};

