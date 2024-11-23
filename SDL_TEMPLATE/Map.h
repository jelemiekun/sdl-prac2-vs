#pragma once
#include <SDL.h>

enum class Levels {
	level1
};

class Map {
private:
	SDL_Rect* srcRect;
	SDL_Rect* dstRect;

	SDL_Texture* red;
	SDL_Texture* green;
	SDL_Texture* blue;

	int map[10][10];

public:
	Map();
	~Map();

	void initMap(Levels level);
	void renderMap();
};

