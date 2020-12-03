#include "Window.h"

Window::Window(int width, int height, SDL_Color color) :
	WIDTH(width), HEIGHT(height) {
	window = SDL_CreateWindow("RaspWindow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		throw SDLException("Couldn't create the window. %s", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		throw SDLException("Couldn't create the renderer. %s", SDL_GetError());
	}

	//screenSurf = SDL_GetWindowSurface(window);

	if (!screenSurf)
	{
		//throw SDLException("Couldn't retrieve the window surface");
	}

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
	//SDL_FillRect(screenSurf, NULL, SDL_MapRGB(screenSurf->format, color.r, color.g, color.b));

	SDL_RenderPresent(renderer);
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

SDL_Window* Window::getWindowObject() const
{
	return window;
}

SDL_Surface* Window::getSurface() const
{
	return screenSurf;
}

SDL_Renderer* Window::getRenderer() const
{
	return renderer;
}

void Window::update()
{
	SDL_UpdateWindowSurface(window);
}
