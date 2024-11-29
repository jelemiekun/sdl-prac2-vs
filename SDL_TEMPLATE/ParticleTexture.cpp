#include "ParticleTexture.h"
#include <SDL_image.h>
#include <iostream>

ParticleTexture::ParticleTexture(SDL_Renderer*& gRenderer, const std::string& path) {
	texture = IMG_LoadTexture(gRenderer, path.c_str());

	if (texture == nullptr) std::cout << "Failed to load " << path << " : " << IMG_GetError();
	else std::cout << "Loaded " << path << ". " << '\n';
}

ParticleTexture::~ParticleTexture() {
	SDL_DestroyTexture(texture);
}

std::string ParticleTexture::getStringPath(const TYPES& type) {
	std::string path = " ";

	switch (type) {
	case TYPES::BLUE: path = "assets/blue.png"; break;
	case TYPES::BROWN: path = "assets/brown.png"; break;
	case TYPES::GREEN: path = "assets/green.png"; break;
	case TYPES::RED: path = "assets/red.png"; break;
	case TYPES::YELLOW: path = "assets/yellow.png"; break;
	default: break;
	}

	return path;
}