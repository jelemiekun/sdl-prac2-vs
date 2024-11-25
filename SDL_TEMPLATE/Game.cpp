#include "Game.h"
#include "AppInfo.h"
#include <random>
#include <string>


static int getRandomYOffset() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, S_HEIGHT - Game::PIPE_GAP);

	return dist6(rng);
}


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

		gFont = TTF_OpenFont("assets/PressStart2P-vaV7.ttf", 32);

		if (gFont == nullptr) {
			std::cout << "Failed to load font: " << TTF_GetError() << '\n';
		} else {
			std::cout << "Font loaded." << '\n';
		}
	}

	bird = std::make_shared<Entity>();
	bird->id = 0;
	bird->dstRect = { 100, 300, 30, 30 };
	bird->color = { 255, 255, 0, 255 };
	bird->velX = 0;
	bird->velY = 5;

	birdController.addEntity(bird);
	birdController.assignKeyToComponents(Invoker::KEYS::BTN_JUMP, jumpCommand);

	pipe1 = std::make_shared<Entity>();
	pipe1->id = 1;
	pipe1->dstRect = { S_WIDTH, -300, 50, 600};
	pipe1->color = { 0, 220, 0, 255 };
	pipe1->velX = -5;
	pipe1->velY = 0;

	pipe2 = std::make_shared<Entity>();
	pipe2->id = 2;
	pipe2->dstRect = { S_WIDTH, pipe1->dstRect.y + pipe1->dstRect.h + PIPE_GAP, 50, 600 };
	pipe2->color = { 0, 220, 0, 255 };
	pipe2->velX = -5;
	pipe2->velY = 0;

	pipesController.addEntity(pipe1);
	pipesController.addEntity(pipe2);

	pipesController.assignKeyToComponents(Invoker::KEYS::NONE, moveCommand);

	score = 0;

	isRunning = true;
}

void Game::reset() {
	bird->dstRect = { 100, 300, 30, 30 };

	pipe1->dstRect = { S_WIDTH, -300, 50, 600 };

	pipe2->dstRect = { S_WIDTH, pipe1->dstRect.y + pipe1->dstRect.h + PIPE_GAP, 50, 600 };

	score = 0;
}

void Game::loadScore() {
	SDL_DestroyTexture(scoreTexture);
	scoreTexture = nullptr;

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, (std::to_string(score)).c_str(), white);


	if (textSurface == NULL) {
		std::cout << "Unable to render text surface: " << TTF_GetError() << '\n';
	}

	scoreTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	if (scoreTexture == NULL) {
		std::cout << "Failed to create texture from rendered text: " << SDL_GetError() << '\n';
	}

	SDL_Rect dstRect{ 0, 20, 45, 70 };
	dstRect.x = (S_WIDTH / 2) - (dstRect.w / 2);

	SDL_RenderCopy(gRenderer, scoreTexture, nullptr, &dstRect);

	SDL_FreeSurface(textSurface);
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			isRunning = false;
		} else if (gEvent.type == SDL_KEYDOWN) {
			switch (gEvent.key.keysym.sym) {
			case SDLK_SPACE:
				birdController.pressButton(Invoker::KEYS::BTN_JUMP);
				break;
			default:
				break;
			}
		}
	}
}

void Game::update() {
	{
		bird->velY += 1;
		bird->dstRect.y += bird->velY;

		if (bird->dstRect.y < 0) {
			bird->dstRect.y = 0;
			bird->velY = 0;
		}

		if (bird->dstRect.y + bird->dstRect.h > S_HEIGHT) {
			bird->dstRect.y = S_HEIGHT - bird->dstRect.h;
			bird->velY = 0;
		}

		auto checkCollision = [](SDL_Rect& bird, SDL_Rect& pipe) -> bool {
			int leftA = bird.x;
			int rightA = bird.x + bird.w;
			int topA = bird.y;
			int bottomA = bird.y + bird.h;

			int leftB = pipe.x;
			int rightB = pipe.x + pipe.w;
			int topB = pipe.y;
			int bottomB = pipe.y + pipe.h;

			if (bottomA <= topB) return false;
			if (topA >= bottomB) return false;
			if (rightA <= leftB) return false;
			if (leftA >= rightB) return false;

			return true;
			};

		if (checkCollision(bird->dstRect, pipe1->dstRect) ||
			checkCollision(bird->dstRect, pipe2->dstRect)) {
			reset();
		}

		static bool scoredFinished = true;
		static bool scoredInProgress = false;

		if (bird->dstRect.x + bird->dstRect.w >= pipe1->dstRect.x + pipe1->dstRect.w &&
			scoredFinished && !scoredInProgress) {
			scoredInProgress = true;
			++score;
			scoredFinished = false;
		}

		if (bird->dstRect.x + bird->dstRect.w < pipe1->dstRect.x && !scoredFinished) {
			scoredInProgress = false;
			scoredFinished = true;
		}

	}

	{
		pipesController.pressButton(Invoker::KEYS::NONE);

		if (pipe1->dstRect.x + pipe1->dstRect.w < 0) {
			pipe1->dstRect.y = -pipe1->dstRect.h + getRandomYOffset();
			pipe2->dstRect.y = pipe1->dstRect.y + pipe1->dstRect.h + PIPE_GAP;

			pipe1->dstRect.x = S_WIDTH;
			pipe2->dstRect.x = S_WIDTH;
		}
	}
}

void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);

	SDL_SetRenderDrawColor(gRenderer, bird->color.r, bird->color.g, bird->color.b, bird->color.a);
	SDL_RenderFillRect(gRenderer, &bird->dstRect);

	SDL_SetRenderDrawColor(gRenderer, pipe1->color.r, pipe1->color.g, pipe1->color.b, pipe1->color.a);
	SDL_RenderFillRect(gRenderer, &pipe1->dstRect);

	SDL_SetRenderDrawColor(gRenderer, pipe2->color.r, pipe2->color.g, pipe2->color.b, pipe2->color.a);
	SDL_RenderFillRect(gRenderer, &pipe2->dstRect);

	loadScore();

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