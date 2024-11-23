#include "Game.h"
#include "GameObject.h"
#include "Map.h"

GameObject* player;
Map* map;




SDL_Renderer* Game::gRenderer = nullptr;

Game::Game() : isRunning(false), gWindow(nullptr), gFont(nullptr) {

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

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "Warning: SDL_Image could not initialize: " << IMG_GetError() << '\n';
	} else {
		std::cout << "SDL_image initialized." << '\n';
	}

	if (TTF_Init() == -1) {
		std::cout << "SDL_TTF could not initialize: " << TTF_GetError() << '\n';
	} else {
		std::cout << "SDL_TTF initialized." << '\n';

		gFont = TTF_OpenFont("assets/OpenSans-Bold.ttf", 32);

		if (gFont == nullptr) {
			std::cout << "Failed to load font: " << TTF_GetError() << '\n';
		} else {
			std::cout << "Font loaded." << '\n';
		}
	}

	player = new GameObject("assets/yellow.png");

	map = new Map;
	map->initMap(Levels::level1);

	isRunning = true;
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			isRunning = false;
		} else if (gEvent.type == SDL_KEYDOWN) {
			switch (gEvent.key.keysym.sym) {
			case SDLK_SPACE:
				break;
			default:
				break;
			}
		}
	}
}

void Game::update() {
	player->update();
}

void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);

	player->render();
	map->renderMap();

	SDL_RenderPresent(gRenderer);
}


void Game::clean() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}

bool Game::running() {
	return isRunning;
}