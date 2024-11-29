#include "ParticleManager.h"
#include "ParticleState.h"
#include "AppInfo.h"
#include "Particle.h"
#include <random>
#include <iostream>

std::shared_ptr<ParticleTexture> ParticleManager::getRandomTexture() {
	std::vector<ParticleTexture::TYPES> types = getAllParticleTextureTypes();

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, types.size() - 1);

	int randomIndex = dist6(rng);
	ParticleTexture::TYPES randomType = types[randomIndex];
	return particleTextures[randomType];
}


std::vector<ParticleTexture::TYPES> ParticleManager::getAllParticleTextureTypes() {
	return {
		ParticleTexture::TYPES::BLUE,
		ParticleTexture::TYPES::BROWN,
		ParticleTexture::TYPES::GREEN,
		ParticleTexture::TYPES::RED,
		ParticleTexture::TYPES::YELLOW
	};
}

void ParticleManager::updateXYPosition(int vX, int vY) {
	x = vX;
	y = vY;

	if (y + ParticleManager::PARTICLE_WIDTH_HEIGHT > S_HEIGHT) {
		y = S_HEIGHT - ParticleManager::PARTICLE_WIDTH_HEIGHT;
	}
}

void ParticleManager::createParticle() {
	std::shared_ptr<Particle> particle = std::make_shared<Particle>(x, y, getRandomTexture());
	particles.push_back(particle);
}


void ParticleManager::renderParticles(SDL_Renderer*& gRenderer) {
	std::vector<std::shared_ptr<Particle>> toRemove;

	for (auto& particle : particles) {
		particle->render(gRenderer);

		if (particle->dstRect->y == S_HEIGHT - particle->dstRect->h) {
			toRemove.push_back(particle);
		}
	}

	for (auto& particle : toRemove) {
		auto it = std::find(particles.begin(), particles.end(), particle);
		if (it != particles.end()) {
			particles.erase(it);
		}
	}
}



void ParticleManager::initParticleTextures(SDL_Renderer*& gRenderer) {
	for (const auto& type : getAllParticleTextureTypes()) {
		if (particleTextures.find(type) == particleTextures.end()) {
			std::string path = ParticleTexture::getStringPath(type);
			std::shared_ptr<ParticleTexture> texture = std::make_shared<ParticleTexture>(gRenderer, path);
			particleTextures[type] = texture;
		}
	}
}


void ParticleManager::updateParticleState(Particle& particle) {
	auto it = std::find_if(particles.begin(), particles.end(),
		[&particle](const std::shared_ptr<Particle>& p) {
			return p.get() == &particle; 
		});

	if (it != particles.end()) {
		particles.erase(it);
	}
}
