#include "Game.h"
#include "AppInfo.h"
#include <string>
#include <random>
#include <unordered_map>

class TileType { // Intrinsic Data
public:
	SDL_Texture* texture;

	TileType(SDL_Renderer*& gRenderer, const std::string& path) {
		texture = IMG_LoadTexture(gRenderer, path.c_str());
	}

	~TileType() {
		SDL_DestroyTexture(texture);
	}
};


class Tile { // Extrinsic Data
private:
	int posX;
	int posY;
	TileType* tileType; // Shared reference to common data

public:
	Tile(int x, int y, TileType* type) : posX(x), posY(y), tileType(type) {}

	void render(SDL_Renderer*& renderer) {
		SDL_Rect dest = { posX, posY, 50, 50 };
		SDL_RenderCopy(renderer, tileType->texture, nullptr, &dest);
	}
};

class TileFactory {
private:
	std::unordered_map<std::string, TileType*> tileTypes; // Cache of shared tile types

public:
	TileFactory() {}

	TileType* getTileType(SDL_Renderer*& gRenderer, const std::string& texturePath) {
		// If file type already exists, return it
		if (tileTypes.find(texturePath) != tileTypes.end()) {
			return tileTypes[texturePath];
		}

		// Otherwise, create a new TileType and store it;
		TileType* newTileType = new TileType(gRenderer, texturePath);
		tileTypes[texturePath] = newTileType;
		return newTileType;
	}

	~TileFactory() {
		for (auto& entry : tileTypes) {
			delete entry.second;
		}
	}
};



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

	if (TTF_Init() == -1) {
		std::cout << "SDL_TTF could not initialize: " << TTF_GetError() << '\n';
	} else {
		std::cout << "SDL_TTF initialized." << '\n';

		gFont = TTF_OpenFont("assets/PressStart2P-vaV7.ttf", 32);

		if (gFont == nullptr) {
			std::cout << "Failed to load font: " << TTF_GetError() << '\n';
		} else {
			std::cout << "Font loaded." << '\n';
		}
	}

	{
		SDL_Surface* iconSurface = IMG_Load("assets/icon.ico");
		SDL_SetWindowIcon(gWindow, iconSurface);
		SDL_FreeSurface(iconSurface);
	}

	isRunning = true;
}


void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			isRunning = false;
		} else if (gEvent.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}
}

void Game::update() {

}


void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);


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