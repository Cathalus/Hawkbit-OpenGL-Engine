#pragma once
#define GLEW_STATIC
#include <string>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "Utils.h"
#undef main

namespace hawk
{
namespace graphics
{
class Display
{
public:
	Display(std::string title, GLuint width, GLuint height, GLuint flags = 0);
	/**
	* Updates the Display and swaps buffers. This must be called on each Frame
	*/
	void update();
	/**
	* Clears all Buffers and sets the color buffer to the specified color
	* @param r	Red
	* @param g	Green
	* @param b	Blue
	* @param a	Alpha
	*/
	void clear(float r, float g, float b, float a);
	/**
	* Binds the OpenGL Context as the current Render Target
	*/
	void bindAsRenderTarget();
	/**
	* Checks if the Window is closed
	* @return isClosed
	*/
	inline bool isClosed() { return _isClosed; }
	/**
	* Returns the width of the Window
	* @return width
	*/
	inline int getWidth() { return _width; }
	/**
	* Returns the height of the Window
	* @return height
	*/
	inline int getHeight() { return _height;  }



	~Display();
private:
	bool _isClosed = false;
	SDL_GLContext _context;
	std::unique_ptr<SDL_Window, SDL_Deleter> _window;
	GLint _width;
	GLint _height;
};
}
}

