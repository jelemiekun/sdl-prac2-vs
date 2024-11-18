#include "Game.h"
#include "FPS.h"
#include <cmath>
#include <string>

Game::Game() : isRunning(false), gWindow(nullptr), gRenderer(nullptr), texturePercolation(nullptr),
		percolation(nullptr), percStats(nullptr), gFont(nullptr) {

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

		gFont = TTF_OpenFont("assets/OpenSans-Bold.ttf", 32);

		if (gFont == nullptr) {
			std::cout << "Failed to load font: " << TTF_GetError() << '\n';
		} else {
			std::cout << "Font loaded." << '\n';
		}
	}

	int widthHeightTexture = 700;
	texturePercolation = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 700, 700);


	percStats = new PercolationStats;

	isRunning = true;
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			isRunning = false;
		} else if (gEvent.type == SDL_KEYDOWN) {
			switch (gEvent.key.keysym.sym) {
			case SDLK_SPACE:
				percolation->openRandomSite();
				break;
			default:
				break;
			}
		}
	}
}

void Game::update() {
	if (percolation == nullptr) {
		percolation = new Percolation(numberOfRowColumn, 700);
	} else {
		for (int i = 0; i < 100; i++) {
			percolation->openRandomSite();
		}
		
		if (percolation->percolates()) {
			percStats->addOpenSite(percolation->getNumberOfOpenSites());
			percStats->compute();

			delete percolation;
			percolation = nullptr;
		}
	}
}

void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);

	if (percolation != nullptr) {
		int wText;
		int hText;

		SDL_QueryTexture(texturePercolation, NULL, NULL, &wText, &hText);
		SDL_Rect textureRect = { 50, 100, wText, hText };
		SDL_RenderCopy(gRenderer, texturePercolation, nullptr, &textureRect);

		SDL_SetRenderTarget(gRenderer, texturePercolation);
		percolation->render(gRenderer);

		SDL_SetRenderTarget(gRenderer, nullptr);
	}

	{

		auto render = [](SDL_Renderer*& gRenderer, TTF_Font* font, std::string message, SDL_Rect dstRect) -> void {
			SDL_Texture* textDisplay = nullptr;
			// Free the existing texture if it exists
			if (textDisplay != nullptr) {
				SDL_DestroyTexture(textDisplay);
				textDisplay = nullptr;
			}

			SDL_Color white = { 255, 255, 255, 255 };

			// Create the text surface
			SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), white);
			if (textSurface == nullptr) {
				std::cout << "Unable to render text surface: " << TTF_GetError() << '\n';
				return; // Return early if the surface couldn't be created
			}

			// Create a texture from the surface
			textDisplay = SDL_CreateTextureFromSurface(gRenderer, textSurface);
			if (textDisplay == nullptr) {
				std::cout << "Failed to create texture from rendered text: " << SDL_GetError() << '\n';
			}

			// Free the surface after creating the texture
			SDL_FreeSurface(textSurface);

			// Only render if texture creation was successful
			if (textDisplay != nullptr) {
				SDL_RenderCopy(gRenderer, textDisplay, nullptr, &dstRect);
			}

			SDL_DestroyTexture(textDisplay);
			textDisplay = nullptr;
			};

		// Call render for each line of text
		SDL_Rect rectTimes = { 50, 10, 220, 40 };
		render(gRenderer, gFont, "Mean: " + std::to_string(percStats->getMean()), rectTimes);

		rectTimes = { 50, 50, 270, 40 };
		render(gRenderer, gFont, "Standard Deviation: " + std::to_string(percStats->getStdDev()), rectTimes);

		rectTimes = { 350, 10, 270, 40 };
		render(gRenderer, gFont, "Confidence Low: " + std::to_string(percStats->getConfidenceLo()), rectTimes);

		rectTimes = { 350, 50, 270, 40 };
		render(gRenderer, gFont, "Confidence High: " + std::to_string(percStats->getConfidenceHi()), rectTimes);

		rectTimes = { 660, 10, 90, 40 };
		render(gRenderer, gFont, "Count: " + std::to_string(percStats->getTimes()), rectTimes);

		rectTimes = { 650, 50, 100, 40 };
		render(gRenderer, gFont, "# of Squares: " + std::to_string(numberOfRowColumn * numberOfRowColumn), rectTimes);
	}

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