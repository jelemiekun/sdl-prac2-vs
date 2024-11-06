#include "Game.h"

Game::Game() : gWindow(nullptr), gRenderer(nullptr), isRunning(false) {

}

Game::~Game() {

}

void Game::init(const char* title, const int& rXPos, const int& rYPos, const int& rWidth, const int& rHeight, const bool& rFullscreen) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL subsystems initialized." << '\n';
	} else {
		std::cout << "Failed to initialize SDL subsystems: " << SDL_GetError() << '\n';
		return;
	}

	int flags = rFullscreen ? SDL_WINDOW_FULLSCREEN : 0;
	gWindow = SDL_CreateWindow(title, rXPos, rYPos, rWidth, rHeight, flags);

	if (gWindow != nullptr) {
		std::cout << "Window created." << '\n';
	} else {
		std::cout << "Failed to create window: " << SDL_GetError() << '\n';
		return;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (gRenderer != nullptr) {
		std::cout << "Renderer created." << '\n';
	} else {
		std::cout << "Failed to create renderer: " << SDL_GetError() << '\n';
		return;
	}

	isRunning = true;
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			isRunning = false;
		}
	}
}

void Game::update() {

}

void Game::render() {

}

void Game::clean() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}

bool Game::running() {
	return isRunning;
}