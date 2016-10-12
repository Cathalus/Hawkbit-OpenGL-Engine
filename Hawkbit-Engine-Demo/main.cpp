#include <iostream>
#include <vector>
#include <memory>

#include <Display.h>
#include <SDL2/SDL.h>
#include "GameplayScene.h"

int main()
{
	hawk::graphics::Display display("Test", 800, 600, NULL);

	GameplayScene gameplay(display.getWidth(), display.getHeight());
        glm::vec3 tmp = glm::vec3(2, 0, -3.5f);
        hawk::graphics::Camera cam = hawk::graphics::Camera(tmp, 70.0f, (float)display.getWidth() / (float)display.getHeight(), 0.1f, 1000.0f);
        gameplay.addCamera(cam);

	SDL_ShowCursor(SDL_FALSE);

	// FPS
	int fps = 60;
	int fpsMill = 1000 / fps;
	int currentFrameSpeed = 0;
	// Delta Timing
	float deltaTime;
	int lastFrameTime = 0, currentFrameTime = 0;
	currentFrameTime = SDL_GetTicks();

	while (!display.isClosed())
	{
		// Delta Calculation
		lastFrameTime = currentFrameTime;
		currentFrameTime = SDL_GetTicks();
		deltaTime = (float)(currentFrameTime - lastFrameTime) / 1000;

		display.clear(0, 0, 0, 1);

		gameplay.update(deltaTime);
		gameplay.render();

		display.update();

		// Delay when too fast
		currentFrameSpeed = SDL_GetTicks() - currentFrameTime;
		if (fpsMill > currentFrameSpeed)
                    SDL_Delay(fpsMill - currentFrameSpeed);
	}

	return 0;
}