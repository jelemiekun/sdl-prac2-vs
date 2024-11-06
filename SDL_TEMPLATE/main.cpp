#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[]) {

    int cFrames = 0;
    Uint32 startTime = SDL_GetTicks();
    

    game = new Game();

    game->init("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, 0);

    while (game->running()) {
        game->input();
        game->update();
        game->render();

        ++cFrames;
        Uint32 currentTime = SDL_GetTicks();

        if (currentTime - startTime > 1000) {
            float avrgFPS = cFrames / ((currentTime - startTime) / 1000.0f);
            std::cout << avrgFPS << '\n';

            cFrames = 0;
            startTime = currentTime;
        }

    }

    game->clean();

    return 0;
}