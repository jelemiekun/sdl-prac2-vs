#include "TrafficLightState.h"
#include "TrafficLight.h"

void RedState::changeState(TrafficLight& trafficLight) {
	trafficLight.setState(std::make_unique<GreenState>());
	trafficLight.setTexture(TEXTURE_PATHS::GREEN);
}

void GreenState::changeState(TrafficLight& trafficLight) {
	trafficLight.setState(std::make_unique<YellowState>());
	trafficLight.setTexture(TEXTURE_PATHS::YELLOW);
}

void YellowState::changeState(TrafficLight& trafficLight) {
	trafficLight.setState(std::make_unique<RedState>());
	trafficLight.setTexture(TEXTURE_PATHS::RED);
}