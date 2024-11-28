#include "TrafficLightTextureFactory.h"
#include <iostream>

TrafficLightTextureFactory::TrafficLightTextureFactory() {}

TrafficLightTextureFactory::~TrafficLightTextureFactory() {
	for (auto& entry : trafficLightsTextures) {
		delete entry.second;
	}
}

TrafficLightStateTexture* TrafficLightTextureFactory::getTexture(SDL_Renderer*& gRenderer, TEXTURE_PATHS path) {
	std::string texturePath = TrafficLightStateTexture::getPathTextture(path);

	if (trafficLightsTextures.find(texturePath) != trafficLightsTextures.end()) {
		return trafficLightsTextures[texturePath];
	}

	TrafficLightStateTexture* newTexture = new TrafficLightStateTexture(gRenderer, path);
	trafficLightsTextures[texturePath] = newTexture;
	return newTexture;
}