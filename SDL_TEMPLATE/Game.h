#pragma once
#include <iostream>
#include "Bot.h"
#include <SDL.h>

class Game {
public:
	Game();
	~Game();

	void init(const char* title, const int& rXPos, const int& rYPos, const int& rWidth, const int& rHeight, const bool& rFullscreen);
	void initBots();
	void input();
	void update();
	void render();
	void clean();

	bool running();

private:
	bool isRunning;
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_Event gEvent;

	Bot* bots[30];
};

