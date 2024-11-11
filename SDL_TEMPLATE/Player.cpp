#include "Player.h"
#include <cmath>
#include <iostream>

Player::Player() : dirX(false), dirY(false), nearestBotIndex(-1) {}

Player::~Player() {}

void Player::init() {
    mCollider->w = PLAYER_WIDTH;
    mCollider->h = PLAYER_HEIGHT;
    mCollider->x = (S_WIDTH / 2) - (PLAYER_WIDTH / 2);
    mCollider->y = (S_HEIGHT / 2) - (PLAYER_HEIGHT / 2);
    calculateCenter();
}

void Player::determineDirection(Bot* (&bots)[BOT_COUNT]) {
    if (bots[nearestBotIndex] != nullptr) {
        SDL_Point temp = { bots[nearestBotIndex]->getCenter()->x - mCenter->x, bots[nearestBotIndex]->getCenter()->y - mCenter->y };

        dirX = temp.x > 0;
        dirY = temp.y > 0;
    }
}

void Player::calculateDistances(SDL_Renderer* gRenderer, Bot* (&bots)[BOT_COUNT]) {
    Bot nearestBot;
    nearestBotIndex = -1;
    float shortestDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < BOT_COUNT; i++) {
        if (bots[i] != nullptr) {
            float dx = mCenter->x - bots[i]->getCenter()->x;
            float dy = mCenter->y - bots[i]->getCenter()->y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < shortestDistance) {
                nearestBotIndex = i;
                shortestDistance = distance;
            }
        }
    }

    for (int i = 0; i < BOT_COUNT; i++) {
        if (bots[i] != nullptr) {
            SDL_Color lineColor = { 0, 0, 0, 255 };

            if (i == nearestBotIndex) {
                lineColor = { 0, 255, 0, 255 };
            }

            SDL_SetRenderDrawColor(gRenderer, lineColor.r, lineColor.g, lineColor.b, 255);
            if (showLine) SDL_RenderDrawLine(gRenderer, mCenter->x, mCenter->y, bots[i]->getCenter()->x, bots[i]->getCenter()->y);
        }
    }

    determineDirection(bots);
}

void Player::move(SDL_Renderer* gRenderer) {
    if (dirX) mCollider->x += PLAYER_VELOCITY;
    else mCollider->x -= PLAYER_VELOCITY;

    if (dirY) mCollider->y += PLAYER_VELOCITY;
    else mCollider->y -= PLAYER_VELOCITY;


    SDL_SetRenderDrawColor(gRenderer, PLAYER_COLOR.r, PLAYER_COLOR.g, PLAYER_COLOR.b, PLAYER_COLOR.a);
    SDL_RenderFillRect(gRenderer, mCollider.get());

    calculateCenter();
}

void Player::checkCollission(Bot* (&bots)[BOT_COUNT]) {
    if (bots[nearestBotIndex] != nullptr) {
        int leftA = mCollider->x;
        int rightA = mCollider->x + mCollider->w;
        int topA = mCollider->y;
        int bottomA = mCollider->y + mCollider->h;

        int leftB = bots[nearestBotIndex]->getCollider()->x;
        int rightB = bots[nearestBotIndex]->getCollider()->x + bots[nearestBotIndex]->getCollider()->w;
        int topB = bots[nearestBotIndex]->getCollider()->y;
        int bottomB = bots[nearestBotIndex]->getCollider()->y + bots[nearestBotIndex]->getCollider()->h;

        if (bottomA <= topB) return;
        if (topA >= bottomB) return;
        if (rightA <= leftB) return;
        if (leftA >= rightB) return;

        deleteNearestBot(bots);
    }
}

void Player::deleteNearestBot(Bot* (&bots)[BOT_COUNT]) {
    if (bots[nearestBotIndex] != nullptr) {
        delete bots[nearestBotIndex];
        bots[nearestBotIndex] = nullptr;
    }
}