#pragma once
#include <string>
#include <SDL.h>

class ParticleTexture {
public:
	enum class TYPES {
		BLUE,
		BROWN,
		GREEN,
		RED,
		YELLOW
	};

public:
	SDL_Texture* texture;

	ParticleTexture(SDL_Renderer*& gRenderer, const std::string& path);
	~ParticleTexture();
	static std::string getStringPath(const TYPES& type);
};

