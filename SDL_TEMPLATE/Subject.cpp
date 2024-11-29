#include "Subject.h"
#include "Observer.h"

void Subject::addObserver(std::shared_ptr<Observer> observer) {
	observers.push_back(observer);
}

void Subject::removeObserver(std::shared_ptr<Observer> observer) {
	auto it = std::find(observers.begin(), observers.end(), observer);
	if (it != observers.end()) {
		observers.erase(it);
	}
}

void Subject::notifyObservers() {
	for (const auto& it : observers) {
		it->update(*this);
	}
}