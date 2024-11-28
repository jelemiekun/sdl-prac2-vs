#pragma once
#include <string>
#include <SDL.h>

enum class TEXTURE_PATHS {
	RED,
	GREEN,
	YELLOW
};


class TrafficLightStateTexture {
public:
	SDL_Texture* texture;

	TrafficLightStateTexture(SDL_Renderer*& gRenderer, TEXTURE_PATHS path);

	~TrafficLightStateTexture();

	static std::string getPathTextture(TEXTURE_PATHS path);
};
