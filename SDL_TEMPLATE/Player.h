#pragma once
#include "Dot.h"
#include "Bot.h"
#include "AppInfo.h"

class Player : public Dot {

private:
	uint8_t nearestBotIndex;
	bool dirX;
	bool dirY;

public:
	Player();
	~Player();

private:
	void determineDirection(Bot* (&bots)[BOT_COUNT]);
	void deleteNearestBot(Bot* (&bots)[BOT_COUNT]);

public:
	void init() override;
	void calculateDistances(SDL_Renderer* gRenderer, Bot* (&bots)[BOT_COUNT]);
	void move(SDL_Renderer* gRenderer) override;
	void checkCollission(Bot* (&bots)[BOT_COUNT]);
};

