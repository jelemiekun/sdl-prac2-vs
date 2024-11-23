#include "Map.h"
#include "TextureManager.h"

int lvl1[10][10] = {
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

Map::Map() : srcRect(nullptr), dstRect(nullptr), red(nullptr), green(nullptr),
			blue(nullptr) {
	srcRect = new SDL_Rect;
	srcRect->x = 0;
	srcRect->y = 0;
	srcRect->w = 20;
	srcRect->h = 20;

	dstRect = new SDL_Rect;
	dstRect->x = 0;
	dstRect->y = 0;
	dstRect->w = 50;
	dstRect->h = 50;

	red = TextureManager::loadTexture("assets/red.png");
	green = TextureManager::loadTexture("assets/green.png");
	blue = TextureManager::loadTexture("assets/blue.png");
}

Map::~Map() {}

void Map::initMap(Levels level) {
	for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			if (level == Levels::level1) {
				map[row][col] = lvl1[row][col];
			}
		}
	}
}

void Map::renderMap() {
	int type = 0;

	for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			type = map[row][col];

			dstRect->x = col * 50;
			dstRect->y = row * 50;

			if (type == 0) TextureManager::draw(red, srcRect, dstRect);
			if (type == 1) TextureManager::draw(green, srcRect, dstRect);
			if (type == 2) TextureManager::draw(blue, srcRect, dstRect);
		}
	}
}