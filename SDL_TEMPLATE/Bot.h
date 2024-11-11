#pragma once
#include "Dot.h"
class Bot : public Dot {

private:
	std::unique_ptr<SDL_Point> generateRandomPoint();

	enum class DIRECTIONS {
		POSITIVE,
		NEGATIVE,
		NONE
	};

private:
	DIRECTIONS dirX;
	DIRECTIONS dirY;

public:
	Bot();
	~Bot();

private:
	void generateRandomDirection();

public:
	void init() override;
	void move(SDL_Renderer* gRenderer) override;
};

