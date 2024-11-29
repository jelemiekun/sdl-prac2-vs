#pragma once

class Particle;

class ParticleObserver {
public:
	virtual void updateParticleState(Particle& particle) = 0;
};

