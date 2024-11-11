#pragma once
#include "SDL.h"
#include "AppInfo.h"
#include <memory>

class Dot {
protected:
	std::unique_ptr<SDL_Rect> mCollider;
	std::unique_ptr<SDL_Point> mCenter;

protected:
	void calculateCenter();

protected:
	Dot();
	~Dot();

public:
	virtual void init() = 0;
	virtual void move(SDL_Renderer* gRenderer) = 0;
	bool isBeyondScreen();
	SDL_Point* getCenter() const;
	SDL_Rect* getCollider() const;
};

