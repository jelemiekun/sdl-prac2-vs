#include "ParticleState.h"
#include "Particle.h"
#include "AppInfo.h"
#include "ParticleManager.h"
#include <iostream>

void FadingState::render(SDL_Renderer*& gRenderer, Particle& particle) {
	particle.counter = particle.counter -= ParticleManager::PARTICLE_COUNTER_DECREASER > 0 ? 
		particle.counter -= ParticleManager::PARTICLE_COUNTER_DECREASER : 0;
	
	SDL_RenderCopy(gRenderer, particle.texture->texture, nullptr, particle.dstRect.get());

	if (particle.counter == 0) {
		particle.notifyObservers(particle);
	}
}

void FallingState::render(SDL_Renderer*& gRenderer, Particle& particle) {
	++particle.vel;

	particle.dstRect->y += particle.vel;

	if (particle.dstRect->y + particle.dstRect->h > S_HEIGHT) {
		particle.dstRect->y = S_HEIGHT - particle.dstRect->h;
		particle.setState(std::make_unique<FadingState>());
	}

	SDL_RenderCopy(gRenderer, particle.texture->texture, nullptr, particle.dstRect.get());
}