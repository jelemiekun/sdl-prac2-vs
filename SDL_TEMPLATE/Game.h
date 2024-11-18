#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Percolation.h"
#include "PercolationStats.h"

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

private:
	constexpr static int numberOfRowColumn = 50;

	bool isRunning;
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_Event gEvent;

	SDL_Texture* texturePercolation;
	Percolation* percolation;
	PercolationStats* percStats;
	
	TTF_Font* gFont;
};

