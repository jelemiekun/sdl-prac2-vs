#include "Game.h"
#include "AppInfo.h"
#include "FPS.h"

Game* game = nullptr;

int main(int argc, char* argv[]) {

    game = new Game();

    int countFrame = 0;
    Uint32 startTime = SDL_GetTicks();

    game->init(S_TITLE, S_XPOS, S_YPOS, S_WIDTH, S_HEIGHT, S_FLAGS);

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