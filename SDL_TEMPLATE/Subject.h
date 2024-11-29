#pragma once
#include <vector>
#include <memory>

class Observer;

class Subject {
private:
	std::vector<std::shared_ptr<Observer>> observers;

public:
	void addObserver(std::shared_ptr<Observer> observer);
	void removeObserver(std::shared_ptr<Observer> observer);
	void notifyObservers();
};