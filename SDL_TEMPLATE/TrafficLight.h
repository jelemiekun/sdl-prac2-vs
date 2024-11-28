#pragma once
#include <vector>
#include <memory>
#include <string>
#include "TrafficLightStateTexture.h"
#include "TrafficLightState.h"

class TrafficLightTextureFactory;


class TrafficLight {
public:
	static std::vector<std::shared_ptr<TrafficLight>> trafficLights;
	static TrafficLightTextureFactory* factory;
private:
	TrafficLightStateTexture* texture;
	int x;
	int y;
	std::unique_ptr<TrafficLightState> state;

public:
	TrafficLight(int vX, int vY, TrafficLightStateTexture* texture);


	void changeState();
	void setState(std::unique_ptr<TrafficLightState> newState);
	void setTexture(TEXTURE_PATHS path);
	void render();
	
	
	// Explicitly delete copy constructor and copy assignment operator
	TrafficLight(const TrafficLight&) = delete;
	TrafficLight& operator=(const TrafficLight&) = delete;
};