#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* fileName) : mTexture(nullptr), srcRect(nullptr), 
	dstRect(nullptr), xPos(0), yPos(0), mTextureW(0), mTextureH(0) {
	mTexture = TextureManager::loadTexture(fileName);

	SDL_QueryTexture(mTexture, nullptr, nullptr, &mTextureW, &mTextureH);

	srcRect = new SDL_Rect;
	srcRect->x = 0;
	srcRect->y = 0;
	srcRect->w = mTextureW;
	srcRect->h = mTextureH;
	

	dstRect = new SDL_Rect;
	dstRect->x = 0;
	dstRect->y = 0;
	dstRect->w = 50;
	dstRect->h = 50;
}

GameObject::~GameObject() {}

void GameObject::update() {

}

void GameObject::render() {
	TextureManager::draw(mTexture, srcRect, dstRect);
}