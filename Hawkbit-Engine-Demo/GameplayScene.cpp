#include "GameplayScene.h"

void GameplayScene::init()
{
	_activeShader.createShader("test", { "test.vert", "test.frag" });
	_lampShader.createShader("lamp", { "lamp.vert", "lamp.frag" });

	_lastX = _width / 2;
	_lastY = _height / 2;


	hawk::core::GameObject temp = hawk::core::GameObject(_modelManager.getModel("plane"));
        glm::mat4 tmp = glm::translate(temp.getMatrix(), glm::vec3(0, -1.5, 0));
	temp.setMatrix(tmp);
	_objects.push_back(temp);

	temp = hawk::core::GameObject(_modelManager.getModel("plane"));
        tmp = glm::translate(temp.getMatrix(), glm::vec3(0, 8.5f, -10));
	temp.setMatrix(tmp);
        tmp = glm::rotate(temp.getMatrix(), glm::radians(90.0f),glm::vec3(1,0,0));
	temp.setMatrix(tmp);
	_objects.push_back(temp);

	temp = hawk::core::GameObject(_modelManager.getModel("plane"));
        tmp = glm::translate(temp.getMatrix(), glm::vec3(0, 8.5f, 10));
	temp.setMatrix(tmp);
        tmp = glm::rotate(temp.getMatrix(), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	temp.setMatrix(tmp);
	_objects.push_back(temp);

	temp = hawk::core::GameObject(_modelManager.getModel("plane"));
        tmp = glm::translate(temp.getMatrix(), glm::vec3(-10, 8.5f, 0));
	temp.setMatrix(tmp);
        tmp = glm::rotate(temp.getMatrix(), glm::radians(-90.0f), glm::vec3(0, 0, 1));
	temp.setMatrix(tmp);
	_objects.push_back(temp);

	temp = hawk::core::GameObject(_modelManager.getModel("plane"));
        tmp = glm::translate(temp.getMatrix(), glm::vec3(10, 8.5f, 0));
	temp.setMatrix(tmp);
        tmp = glm::rotate(temp.getMatrix(), glm::radians(90.0f), glm::vec3(0, 0, 1));
	temp.setMatrix(tmp);
	_objects.push_back(temp);

	_directionalLight = hawk::graphics::DirectionalLight(glm::vec3((float)50 / 255, (float)50 / 255, (float)120 / 255), glm::vec3(0, 60, 80), glm::vec3(1, 0, 0), 0.1f);
	_lights.push_back(std::shared_ptr<hawk::graphics::DirectionalLight>(new hawk::graphics::DirectionalLight(_directionalLight)));
	_lights.push_back(std::shared_ptr<hawk::graphics::PointLight>(new hawk::graphics::PointLight(glm::vec3((float)255 / 255, (float)0 / 255, (float)0 / 255), glm::vec3(0, 15, 0), 1, 0.14f, 0.07f)));
	_lights.push_back(std::shared_ptr<hawk::graphics::PointLight>(new hawk::graphics::PointLight(glm::vec3((float)0 / 255, (float)255 / 255, (float)0 / 255), glm::vec3(0, 0, 0), 1, 0.7f, 1.8f)));
	_lights.push_back(std::shared_ptr<hawk::graphics::PointLight>(new hawk::graphics::PointLight(glm::vec3((float)255 / 255, (float)255 / 255, (float)0 / 255), glm::vec3(3, 0, 3), 1, 0.22f, 0.20f)));

	for (int i = 0; i < _lights.size(); i++)
	{
		_lampColors.push_back( _lights[i]->getColor());
		temp = hawk::core::GameObject(_modelManager.getModel("cube"));
                tmp = glm::translate(temp.getMatrix(), _lights[i]->getPosition());
		temp.setMatrix(tmp);
                tmp = glm::scale(temp.getMatrix(), glm::vec3(0.5f,0.5f,0.5f));
		temp.setMatrix(tmp);
		_lamps.push_back(temp);
	}
}

void GameplayScene::loadAssets()
{
	_modelManager.loadModel("plane", "plane/plane.obj");
	_modelManager.loadModel("cube", "cube/cube.obj");
}

void GameplayScene::unloadAssets()
{
	
}

void GameplayScene::update(float delta)
{
        _acc += delta;
	_mainCamera = _cameras[0];

	handleInput(delta);

        glm::mat4 tmp = _activeCamera->getView();
	_uniformManager.updateUniformData("view", tmp);
        tmp = _activeCamera->getProjection();
	_uniformManager.updateUniformData("projection", tmp);

	// Update Model
	//_objects[0].setMatrix(glm::rotate(_objects[0].getMatrix(), glm::radians(1.0f), glm::vec3(0, 1, 0)));

        for (int j = 0; j < _lights.size(); j++)
        {
            
            glm::vec3 newColor = glm::vec3(fabs(cos(_acc*(j*2+1))),fabs(sin(_acc*(j*3+1))),fabs(cos(_acc*(j*5+1))));
            std::cout << newColor.x << " " << newColor.y << " " << newColor.z << std::endl;
            _lampColors[j] = newColor;
            _lights[j].get()->setColor(newColor);
            
        }
        
        
	uploadLightSources();
}

void GameplayScene::uploadLightSources()
{
    glm::vec3 tmp_v3 = _activeCamera->getPosition();
    _uniformManager.updateUniformData("viewPosition", tmp_v3);
    _uniformManager.updateUniformData("ambientStrength", 0.1f);

    _directionalLight.setDirection(glm::normalize(glm::vec3(0, 0, 0) - _directionalLight.getPosition()));

    int pointCNT = 0;
    for (std::shared_ptr<hawk::graphics::LightSource> lightSource : _lights)
    {
            if (lightSource->getType() == hawk::graphics::LightType::DIRECTIONAL)
            {
                tmp_v3 = _directionalLight.getColor();
                _uniformManager.updateUniformData("directionalLight.color", tmp_v3);
                tmp_v3 = _directionalLight.getDirection();
                _uniformManager.updateUniformData("directionalLight.direction", tmp_v3);
                float tmp_fl = _directionalLight.getIntensity();
                _uniformManager.updateUniformData("directionalLight.intensity", tmp_fl);
                tmp_v3 = _directionalLight.getColor();
                _uniformManager.updateUniformData("lampColor", tmp_v3);
            }
            else if (lightSource->getType() == hawk::graphics::LightType::SPOT)
            {
                tmp_v3 = _flashLight.getColor();
                _uniformManager.updateUniformData("spotLight.color", tmp_v3);
                tmp_v3 = _flashLight.getPosition();
                _uniformManager.updateUniformData("spotLight.position", tmp_v3);
                tmp_v3 = _flashLight.getDirection();
                _uniformManager.updateUniformData("spotLight.direction", tmp_v3);
                _uniformManager.updateUniformData("spotLight.cutOff", _flashLight.getCutOff());
                _uniformManager.updateUniformData("spotLight.cutOuterOff", _flashLight.getOuterCutOff());
                _uniformManager.updateUniformData("spotLight.constant", _flashLight.getConstant());
                _uniformManager.updateUniformData("spotLight.linear", _flashLight.getLinear());
                _uniformManager.updateUniformData("spotLight.quadratic", _flashLight.getQuadratic());
                //_uniformManager.updateUniformData("spotLight.activated", _flashLightActive);
            }
            else if (lightSource->getType() == hawk::graphics::LightType::POINT)
            {
                std::shared_ptr<hawk::graphics::PointLight> pL = std::static_pointer_cast<hawk::graphics::PointLight>(lightSource);

                tmp_v3 = lightSource->getColor(); 
                _uniformManager.updateUniformData("pointLights[" + std::to_string(pointCNT) + "].color",
                        tmp_v3);
                tmp_v3 = lightSource->getPosition();
                _uniformManager.updateUniformData("pointLights[" + std::to_string(pointCNT) + "].position",
                        tmp_v3);
                _uniformManager.updateUniformData("pointLights[" + std::to_string(pointCNT) + "].constant",
                        pL->getConstant());
                _uniformManager.updateUniformData("pointLights[" + std::to_string(pointCNT) + "].linear",
                        pL->getLinear());
                _uniformManager.updateUniformData("pointLights[" + std::to_string(pointCNT) + "].quadratic",
                        pL->getQuadratic());
                _uniformManager.updateUniformData("pointLights[" + std::to_string(pointCNT) + "].activated",
                        true);
                pointCNT++;
            }
    }
}

void GameplayScene::handleInput(float delta)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	glm::vec3 forward = _mainCamera->getForward();

	/* Mouse */
	int xpos, ypos;
	SDL_GetMouseState(&xpos, &ypos);
	
	GLfloat xoffset = xpos - _lastX;
	GLfloat sensitivity = 0.1f;
	GLfloat yoffset = _lastY - ypos; // Reversed since y-coordinates range from bottom to top

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw += xoffset;
	_pitch -= -yoffset;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	if (currentKeyStates[SDL_SCANCODE_W])
	{
		_mainCamera->setPosition(_mainCamera->getPosition() + _mainCamera->getForward()*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		_mainCamera->setPosition(_mainCamera->getPosition() - _mainCamera->getForward()*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		_mainCamera->setPosition(_mainCamera->getPosition() - (glm::normalize(glm::cross(_mainCamera->getForward(), _mainCamera->getUp())))*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		_mainCamera->setPosition(_mainCamera->getPosition() + (glm::normalize(glm::cross(_mainCamera->getForward(), _mainCamera->getUp())))*delta*_movementSpeed);
	}
	if (currentKeyStates[SDL_SCANCODE_SPACE])
	{
		_mainCamera->setPosition(_mainCamera->getPosition() + glm::vec3(0, delta*_movementSpeed, 0));
	}
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
	{
		_mainCamera->setPosition(_mainCamera->getPosition() + glm::vec3(0, -delta*_movementSpeed, 0));
	}

	glm::vec3 fwd;
	fwd.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	fwd.y = sin(glm::radians(_pitch));
	fwd.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	forward = glm::normalize(fwd);
	_mainCamera->setForward(forward);
}

void GameplayScene::render()
{
	for (hawk::core::GameObject obj : _objects)
	{
		obj.draw(_activeShader, _uniformManager);
	}
	for (int i = 0; i < _lamps.size(); i++)
	{
		_uniformManager.updateUniformData("lampColor", _lampColors[i]);
		_lamps[i].draw(_lampShader, _uniformManager);
	}
}