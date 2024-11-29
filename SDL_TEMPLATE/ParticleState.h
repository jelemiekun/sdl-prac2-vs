#pragma once
#include <SDL.h>

class Particle;

class ParticleState {
public:
	virtual void render(SDL_Renderer*& gRenderer, Particle& particle) = 0;
};

class FadingState : public ParticleState {
public:
	void render(SDL_Renderer*& gRenderer, Particle& particle) override;
};

class FallingState : public ParticleState {
public:
	void render(SDL_Renderer*& gRenderer, Particle& particle) override;
};
