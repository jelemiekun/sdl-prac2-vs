#include "Game.h"
#include "FPS.h"

Game* game = nullptr;

int main(int argc, char* argv[]) {

    game = new Game();

    int countFrame = 0;
    Uint32 startTime = SDL_GetTicks();

    game->init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, 0);

    while (game->running()) {
        Uint32 frameStart = SDL_GetTicks();

        game->input();
        game->update();
        game->render();

        FPS::calculateAverageFPS(countFrame, startTime);
        FPS::capFPS(frameStart);
    }

    game->clean();

    return 0;
}