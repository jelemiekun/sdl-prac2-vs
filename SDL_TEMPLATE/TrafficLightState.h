#pragma once

class TrafficLight;

class TrafficLightState {
public:
	virtual void changeState(TrafficLight& trafficLight) = 0;
};

class RedState : public TrafficLightState {
public:
	void changeState(TrafficLight& trafficLight) override;
};

class GreenState : public TrafficLightState {
public:
	void changeState(TrafficLight& trafficLight) override;
};

class YellowState : public TrafficLightState {
public:
	void changeState(TrafficLight& trafficLight) override;
};