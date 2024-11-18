#include "FPS.h"
#include <iostream>

void FPS::calculateAverageFPS(int& countFrame, Uint32& startTime) {
    ++countFrame;

    constexpr Uint16 ONE_SECOND_IN_MS = 1000;
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - startTime > ONE_SECOND_IN_MS) {
        float fps = countFrame / ((currentTime - startTime) / static_cast<float>(ONE_SECOND_IN_MS));
        std::cout << fps << '\n';

        countFrame = 0;
        startTime = currentTime;
    }
}

void FPS::capFPS(const Uint32& frameStart) {
    constexpr Uint8 FPS = 150;
    constexpr float FRAME_DURATION = 1000.0F / FPS;

    Uint32 frameDuration = SDL_GetTicks() - frameStart;

    if (frameDuration < FRAME_DURATION) {
        SDL_Delay(FRAME_DURATION - frameDuration);
    }
}