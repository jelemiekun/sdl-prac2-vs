#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "MouseObserver.h"
#include "ParticleTexture.h"
#include "ParticleObserver.h"

class Particle;

class ParticleManager : public MouseObserver, public ParticleObserver {
private:
	std::vector<std::shared_ptr<Particle>> particles;
	std::unordered_map<ParticleTexture::TYPES, std::shared_ptr<ParticleTexture>> particleTextures;

	int x;
	int y;

public:
	constexpr static int PARTICLE_WIDTH_HEIGHT = 50;
	constexpr static int PARTICLE_COUNTER_DECREASER = 5;

private:
	std::shared_ptr<ParticleTexture> getRandomTexture();
	std::vector<ParticleTexture::TYPES> getAllParticleTextureTypes();

public:
	void initParticleTextures(SDL_Renderer*& gRenderer);
	void updateXYPosition(int vX, int vY) override;
	void updateParticleState(Particle& particle) override;
	void createParticle();
	void renderParticles(SDL_Renderer*& gRenderer);
};

