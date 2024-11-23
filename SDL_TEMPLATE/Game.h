#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Game {

public:
	Game();
	~Game();

	void init(const char* title, const int& rXPos, const int& rYPos, const int& rWidth, const int& rHeight, const bool& rFullscreen);
	void input();
	void update();
	void render();
	void clean();

	bool running();

	static SDL_Renderer* gRenderer;

private:
	bool isRunning;
	SDL_Window* gWindow;
	SDL_Event gEvent;

	TTF_Font* gFont;
};

