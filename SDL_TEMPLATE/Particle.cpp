#include "Particle.h"
#include "ParticleManager.h"
#include "ParticleState.h"

Particle::Particle(int x, int y, std::shared_ptr<ParticleTexture> texture) {
	state = std::make_unique<FallingState>();

	this->texture = texture;

	dstRect = std::make_unique<SDL_Rect>();
	dstRect->x = x;
	dstRect->y = y;
	dstRect->w = ParticleManager::PARTICLE_WIDTH_HEIGHT;
	dstRect->h = ParticleManager::PARTICLE_WIDTH_HEIGHT;

	vel = 1;

	counter = 255;
}

void Particle::render(SDL_Renderer*& gRenderer) {
	state->render(gRenderer, *this);
}

void Particle::setState(std::unique_ptr<ParticleState> state) {
	this->state = std::move(state);
}