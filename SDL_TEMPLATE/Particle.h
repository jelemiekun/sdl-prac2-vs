#pragma once
#include <SDL.h>
#include <memory>
#include "ParticleSubject.h"

class ParticleTexture;
class ParticleState;

class Particle : public ParticleSubject {
private:
	std::unique_ptr<ParticleState> state;

public:
	std::shared_ptr<ParticleTexture> texture;
	std::unique_ptr<SDL_Rect> dstRect;
	int counter;
	int vel;

public:
	Particle(int x, int y, std::shared_ptr<ParticleTexture> texture);

	void render(SDL_Renderer*& gRenderer);
	void setState(std::unique_ptr<ParticleState> state);
};