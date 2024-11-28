#include "Game.h"
#include "AppInfo.h"
#include "Invoker.h"
#include "TrafficLight.h"
#include "TrafficLightTextureFactory.h"

Invoker invoker;

TrafficLightTextureFactory factory;
TrafficLightStateTexture* red;
TrafficLightStateTexture* green;
TrafficLightStateTexture* yellow;

SDL_Renderer* Game::gRenderer = nullptr;

Game::Game() : isRunning(false), gWindow(nullptr), gFont(nullptr) {}

Game::~Game() {}

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
		/*SDL_Surface* iconSurface = IMG_Load("assets/icon.ico");
		SDL_SetWindowIcon(gWindow, iconSurface);
		SDL_FreeSurface(iconSurface);*/
	}

	{
		TrafficLight::factory = &factory;

		red = factory.getTexture(Game::gRenderer, TEXTURE_PATHS::RED);
		green = factory.getTexture(Game::gRenderer, TEXTURE_PATHS::GREEN);
		yellow = factory.getTexture(Game::gRenderer, TEXTURE_PATHS::YELLOW);
	}

	{
		int distance = 150;
		SDL_KeyCode numpadKeys[3][3] = {
			{SDLK_KP_7, SDLK_KP_8, SDLK_KP_9},
			{SDLK_KP_4, SDLK_KP_5, SDLK_KP_6},
			{SDLK_KP_1, SDLK_KP_2, SDLK_KP_3}
		};

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				int x = distance * j;
				int y = distance * i;

				std::shared_ptr<TrafficLight> trafficLight = std::make_shared<TrafficLight>(x, y, red);
				TrafficLight::trafficLights.push_back(trafficLight);

				invoker.assignKeyToTrafficLight(numpadKeys[i][j], trafficLight);
			}
		}
	}

	isRunning = true;
}


void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			isRunning = false;
		} else if (gEvent.type == SDL_KEYDOWN) {
			switch (gEvent.key.keysym.sym) {
			case SDLK_KP_1: invoker.pressButton(SDLK_KP_1); break;
			case SDLK_KP_2: invoker.pressButton(SDLK_KP_2); break;
			case SDLK_KP_3: invoker.pressButton(SDLK_KP_3); break;
			case SDLK_KP_4: invoker.pressButton(SDLK_KP_4); break;
			case SDLK_KP_5: invoker.pressButton(SDLK_KP_5); break;
			case SDLK_KP_6: invoker.pressButton(SDLK_KP_6); break;
			case SDLK_KP_7: invoker.pressButton(SDLK_KP_7); break;
			case SDLK_KP_8: invoker.pressButton(SDLK_KP_8); break;
			case SDLK_KP_9: invoker.pressButton(SDLK_KP_9); break;
			default:
				break;
			}
		}
	}
}

void Game::update() {

}


void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);

	for (const auto& trafficLight : TrafficLight::trafficLights) {
		trafficLight->render();
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