#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(const char* fileName) {
	SDL_Surface* surface = IMG_Load(fileName);
	SDL_Texture* texture = nullptr;

	if (surface == nullptr) {
		std::cout << "Failed to load image: " << IMG_GetError() << '\n';
	} else {
		texture = SDL_CreateTextureFromSurface(Game::gRenderer, surface);

		if (texture == nullptr)
			std::cout << "Failed to create texture from surface: " << SDL_GetError() << '\n';
	}

	SDL_FreeSurface(surface);
	return texture;
}

void TextureManager::draw(SDL_Texture*& texture, SDL_Rect*& srcRect, SDL_Rect*& dstRect) {
	SDL_RenderCopy(Game::gRenderer, texture, srcRect, dstRect);
}