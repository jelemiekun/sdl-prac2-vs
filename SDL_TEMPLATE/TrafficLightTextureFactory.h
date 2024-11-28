#pragma once
#include <unordered_map>
#include <string>
#include <SDL.h>
#include "TrafficLightStateTexture.h"

class TrafficLightTextureFactory {
private:
	std::unordered_map<std::string, TrafficLightStateTexture*> trafficLightsTextures;

public:
	TrafficLightTextureFactory();

	~TrafficLightTextureFactory();

	TrafficLightStateTexture* getTexture(SDL_Renderer*& gRenderer, TEXTURE_PATHS path);
};