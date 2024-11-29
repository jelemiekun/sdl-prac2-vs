#include "ParticleSubject.h"
#include "ParticleObserver.h"
#include <iostream>

void ParticleSubject::addObserver(std::shared_ptr<ParticleObserver> newObserver) {
	particleObservers.push_back(newObserver);
}

void ParticleSubject::removeObserver(std::shared_ptr<ParticleObserver> observer) {
	auto it = std::find(particleObservers.begin(), particleObservers.end(), observer);
	if (it != particleObservers.end()) {
		particleObservers.erase(it);
	}
}

void ParticleSubject::notifyObservers(Particle& particle) {
	for (auto& observer : particleObservers) {
		std::cout << "dito" << '\n';
		observer->updateParticleState(particle);
	}
}