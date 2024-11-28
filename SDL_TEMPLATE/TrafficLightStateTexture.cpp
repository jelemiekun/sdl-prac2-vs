#include "TrafficLightStateTexture.h"
#include <SDL_image.h>
#include <iostream>

TrafficLightStateTexture::TrafficLightStateTexture(SDL_Renderer*& gRenderer, TEXTURE_PATHS path) {
	texture = IMG_LoadTexture(gRenderer, getPathTextture(path).c_str());
}

TrafficLightStateTexture::~TrafficLightStateTexture() {
	SDL_DestroyTexture(texture);
}

std::string TrafficLightStateTexture::getPathTextture(TEXTURE_PATHS path) {
	std::string texturePath = " ";

	switch (path) {
	case TEXTURE_PATHS::RED: texturePath = "assets/red.png"; break;
	case TEXTURE_PATHS::GREEN: texturePath = "assets/green.png"; break;
	case TEXTURE_PATHS::YELLOW: texturePath = "assets/yellow.png"; break;
	default: break;
	}

	return texturePath;
}