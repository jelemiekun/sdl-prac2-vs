#pragma once
#include <SDL.h>
#include <memory>
#include <vector>

class MouseObserver;

class MouseSubject {
private:
	std::vector<std::shared_ptr<MouseObserver>> mouseObservers;

public:
	void addObserver(std::shared_ptr<MouseObserver> newObserver);
	void removeObserver(std::shared_ptr<MouseObserver> observer);
	void notifyObservers();
};

