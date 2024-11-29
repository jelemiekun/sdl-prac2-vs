#include "MouseSubject.h"
#include "MouseObserver.h"

void MouseSubject::addObserver(std::shared_ptr<MouseObserver> newObserver) {
	mouseObservers.push_back(newObserver);
}

void MouseSubject::removeObserver(std::shared_ptr<MouseObserver> observer) {
	auto it = std::find(mouseObservers.begin(), mouseObservers.end(), observer);
	if (it != mouseObservers.end()) {
		mouseObservers.erase(it);
	}
}

void MouseSubject::notifyObservers() {
	int x = 0;
	int y = 0;
	SDL_GetMouseState(&x, &y);

	for (auto& observer : mouseObservers) {
		observer->updateXYPosition(x, y);
	}
}