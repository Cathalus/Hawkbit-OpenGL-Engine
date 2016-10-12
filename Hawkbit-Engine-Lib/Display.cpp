#include "Display.h"

namespace hawk{ 
namespace graphics{

Display::Display(std::string title, GLuint width, GLuint height, GLuint flags)
{
	/* Initialize SDL2 */
	SDL_Init(SDL_INIT_EVERYTHING);

	/* Setting OpenGL Attributes */
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);                                        // Number of bits for red colors (8)
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);					// Number of bits for blue colors (8)
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);					// Number of bits for green colors (8)
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);					// Number of bits for alpha
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);					// Size of the buffer (3*8 = 32)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);					// Size of the depth buffer (3*8 = 32) http://www.opengl.org/archives/resources/faq/technical/depthbuffer.htm
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);					// Activate Double Buffering
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);				// Set Major Version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);				// Set Minor Version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	// Set Core Profile
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);				// Enable Multisampling (MSAA)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);				// 4xMSAA


	/* Create Window */
	_window = std::unique_ptr<SDL_Window, SDL_Deleter>(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | flags),
		SDL_Deleter());
	_context = SDL_GL_CreateContext(_window.get());

	/* Initialize GLEW */
	glewExperimental = true;
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK)
		std::cerr << "GLEW failed to initialize!" << std::endl;

	_width = width;
	_height = height;

	/* Enable OpenGL Features */ 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	SDL_GL_SetSwapInterval(1);
	SDL_GetWindowSize(_window.get(), &_width,&_height);

	std::cout << "Initialized OpenGL" << std::endl << "Version: " << glGetString(GL_VERSION) << std::endl;
}

void Display::update()
{
	SDL_GL_SwapWindow(_window.get());

	SDL_Event* e = new SDL_Event();
	while (SDL_PollEvent(e))
	{
		if (e->type == SDL_QUIT)
			_isClosed = true;
		if (e->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			_isClosed = true;
	}

	SDL_WarpMouseInWindow(_window.get(), _width / 2, _height / 2);
}

void Display::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::bindAsRenderTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, _width, _height);
}

Display::~Display()
{
	SDL_GL_DeleteContext(_context);
	SDL_Quit();
}

}
}
