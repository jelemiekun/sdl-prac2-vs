#include "Invoker.h"

Invoker::Invoker() {}

void Invoker::assignKeyToTrafficLight(SDL_KeyCode key, std::shared_ptr<TrafficLight> trafficLight) {
	keyToTrafficLights[key] = trafficLight;
}

void Invoker::pressButton(SDL_KeyCode key) {
	auto it = keyToTrafficLights.find(key);
	if (it != keyToTrafficLights.end()) {
		keyToTrafficLights[key]->changeState();
	}
}