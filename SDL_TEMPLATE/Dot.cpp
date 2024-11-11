#include "Dot.h"

Dot::Dot() : mCollider(std::make_unique<SDL_Rect>()), mCenter(std::make_unique<SDL_Point>()) {}


Dot::~Dot() {}

void Dot::calculateCenter() {
    int midX = (mCollider->x + (mCollider->x + mCollider->w)) / 2;
    int midY = (mCollider->y + (mCollider->y + mCollider->h)) / 2;

    mCenter->x = midX;
    mCenter->y = midY;
}

bool Dot::isBeyondScreen() {
    if (mCollider->x + mCollider->w < 0) return true;
    if (mCollider->x + mCollider->w > S_WIDTH) return true;
    if (mCollider->y + mCollider->h < 0) return true;
    if (mCollider->y + mCollider->h > S_HEIGHT) return true;

    return false;
}

SDL_Point* Dot::getCenter() const {
    return mCenter.get();
}

SDL_Rect* Dot::getCollider() const {
    return mCollider.get();
}