#pragma once
#include "Game.h"

class GameObject {
private:
	SDL_Texture* mTexture;
	SDL_Rect* srcRect;
	SDL_Rect* dstRect;

	int xPos;
	int yPos;
	int mTextureW;
	int mTextureH;

public:
	GameObject(const char* fileName);
	~GameObject();

	void update();
	void render();
};

