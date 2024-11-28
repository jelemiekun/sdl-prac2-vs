#pragma once
#include <unordered_map>
#include <SDL.h>
#include <memory>
#include "TrafficLight.h"

class Invoker {
private:
	std::unordered_map<SDL_KeyCode, std::shared_ptr<TrafficLight>> keyToTrafficLights;

public:
	Invoker();

	void assignKeyToTrafficLight(SDL_KeyCode key, std::shared_ptr<TrafficLight> trafficLight);

	void pressButton(SDL_KeyCode key);
};