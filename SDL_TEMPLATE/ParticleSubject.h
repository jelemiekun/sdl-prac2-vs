#pragma once
#include <memory>
#include <vector>

class ParticleObserver;
class Particle;

class ParticleSubject {
private:
	std::vector<std::shared_ptr<ParticleObserver>> particleObservers;

public:
	void addObserver(std::shared_ptr<ParticleObserver> newObserver);
	void removeObserver(std::shared_ptr<ParticleObserver> observer);
	void notifyObservers(Particle& particle);
};
