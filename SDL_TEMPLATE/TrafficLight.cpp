#include "TrafficLight.h"
#include "TrafficLightState.h"
#include "TrafficLightTextureFactory.h"
#include "Game.h"

std::vector<std::shared_ptr<TrafficLight>> TrafficLight::trafficLights = {};
TrafficLightTextureFactory* TrafficLight::factory = nullptr;


TrafficLight::TrafficLight(int vX, int vY, TrafficLightStateTexture* texture)
	: state(std::make_unique<RedState>()), texture(texture), x(vX), y(vY) {
}


void TrafficLight::changeState() {
	state->changeState(*this);
}

void TrafficLight::setState(std::unique_ptr<TrafficLightState> newState) {
	state = std::move(newState);
}

void TrafficLight::setTexture(TEXTURE_PATHS path) {
	texture = factory->getTexture(Game::gRenderer, path);
}

void TrafficLight::render() {
	SDL_Rect dstRect = { x, y, 120, 120 };
	SDL_RenderCopy(Game::gRenderer, texture->texture, nullptr, &dstRect);
}