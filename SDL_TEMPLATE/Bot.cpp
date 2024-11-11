#include "Bot.h"
#include <random>
#include <iostream>

Bot::Bot() : Dot() {}

Bot::~Bot() {}

std::unique_ptr<SDL_Point> Bot::generateRandomPoint() {
    static std::random_device dev1;
    static std::mt19937 rng1(dev1());
    std::uniform_int_distribution<std::mt19937::result_type> distX(1, S_WIDTH - BOT_WIDTH - 1);

    static std::random_device dev2;
    static std::mt19937 rng2(dev2());
    std::uniform_int_distribution<std::mt19937::result_type> distY(1, S_HEIGHT - BOT_HEIGHT - 1);

    std::unique_ptr<SDL_Point> point = std::make_unique<SDL_Point>();
    point->x = (distX(rng1));
    point->y = (distY(rng2));

    return point;
}

void Bot::generateRandomDirection() {
    static std::random_device dev3;
    static std::mt19937 rng3(dev3());
    std::uniform_int_distribution<std::mt19937::result_type> distXX(0, 2);
    int generatedRandomXDirection = distXX(rng3);

    int maxYMovement = (generatedRandomXDirection == 2) ? 1 : 2;

    static std::random_device dev4;
    static std::mt19937 rng4(dev4());
    std::uniform_int_distribution<std::mt19937::result_type> distYY(0, maxYMovement);

    switch (generatedRandomXDirection) {
    case 0: dirX = DIRECTIONS::POSITIVE; break;
    case 1: dirX = DIRECTIONS::NEGATIVE; break;
    case 2: dirX = DIRECTIONS::NONE; break;
    default: dirX = DIRECTIONS::POSITIVE; break;
    }

    int generatedRandomYDirection = distYY(rng4);
    switch (generatedRandomYDirection) {
    case 0: dirY = DIRECTIONS::POSITIVE; break;
    case 1: dirY = DIRECTIONS::NEGATIVE; break;
    case 2: dirY = DIRECTIONS::NONE; break;
    default: dirY = DIRECTIONS::POSITIVE; break;
    }
}


void Bot::init() {
    std::unique_ptr<SDL_Point> coordinates = std::move(generateRandomPoint());
    mCollider->x = coordinates->x;
    mCollider->y = coordinates->y;
    mCollider->w = BOT_WIDTH;
    mCollider->h = BOT_HEIGHT;

    calculateCenter();
    generateRandomDirection();
}

void Bot::move(SDL_Renderer* gRenderer) {
    switch (dirX) {
    case Bot::DIRECTIONS::POSITIVE: mCollider->x += BOT_VELOCITY; break;
    case Bot::DIRECTIONS::NEGATIVE: mCollider->x -= BOT_VELOCITY; break;
    case Bot::DIRECTIONS::NONE: break;
    default: mCollider->x += BOT_VELOCITY; break;
    }

    switch (dirY) {
    case Bot::DIRECTIONS::POSITIVE: mCollider->y += BOT_VELOCITY; break;
    case Bot::DIRECTIONS::NEGATIVE: mCollider->y -= BOT_VELOCITY; break;
    case Bot::DIRECTIONS::NONE: break;
    default: mCollider->y += BOT_VELOCITY; break;
    }

    SDL_SetRenderDrawColor(gRenderer, BOT_COLOR.r, BOT_COLOR.g, BOT_COLOR.b, BOT_COLOR.a);
    SDL_RenderFillRect(gRenderer, mCollider.get());

    calculateCenter();
}