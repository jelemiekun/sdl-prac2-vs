#include "Game.h"
#include "AppInfo.h"
#include <string>
#include <random>



SDL_Rect grid[3][3];
int mouseX = 0;
int mouseY = 0;
bool mouseClickFinished = false;
bool mouseClickInProgress = false;
bool playerMove = true;
bool gameFinish = false;
bool insideUndo = false;
bool insideReset = false;
bool draw = false;
bool playerScored = false;

constexpr uint32_t FINISH_DURATION = 750;
uint32_t TICK_STARTED = 0;


SDL_Texture* loadPNGAsTexture(SDL_Renderer* renderer, const std::string& filePath) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	if (!surface) {
		std::cerr << "Failed to load PNG: " << IMG_GetError() << '\n';
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << '\n';
	}
	SDL_FreeSurface(surface);

	return texture;
}

unsigned int getRandomNumber() {
	std::random_device dev; 
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 8);
	return dist6(rng); 
}

void loadFromRenderedText(SDL_Renderer*& gRenderer, SDL_Texture*& texture, TTF_Font*& gFont, std::string textureText, SDL_Color color) {
	SDL_DestroyTexture(texture);
	texture = nullptr;

	
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), color);
	if (textSurface == NULL) {
		std::cout << "Unable to render text surface: " << TTF_GetError() << '\n';
	}

	texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	if (texture == NULL) {
		std::cout << "Failed to create texture from rendered text: " << SDL_GetError() << '\n';
		SDL_FreeSurface(textSurface);
	}

	SDL_FreeSurface(textSurface);
}






SDL_Renderer* Game::gRenderer = nullptr;

Game::Game() : isRunning(false), gWindow(nullptr), gFont(nullptr), ticTacToe(nullptr),
				invoker(nullptr), image(nullptr), srcRectImgX(nullptr), srcRectImgY(nullptr),
				srcRectImgUndo(nullptr), srcRectImgWhiteX(nullptr), textResetBoard(nullptr),
				textXScore(nullptr), textOScore(nullptr), rectUndo(nullptr), textCondition(nullptr) {

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

	ticTacToe = new Entity;
	ticTacToe->id = 0;
	ticTacToe->playerScore = 0;
	ticTacToe->botScore = 0;

	invoker = new Invoker(*ticTacToe);

	{
		image = loadPNGAsTexture(gRenderer, "assets/TTT.png");

		int imgW = 0;
		int imgH = 0;

		SDL_QueryTexture(image, NULL, NULL, &imgW, &imgH);

		srcRectImgX = new SDL_Rect;
		srcRectImgY = new SDL_Rect;
		srcRectImgUndo = new SDL_Rect;
		srcRectImgWhiteX = new SDL_Rect;

		srcRectImgX->x = (imgW / 2) * 0;
		srcRectImgX->y = (imgH / 2) * 0;
		srcRectImgX->w = (imgW / 2);
		srcRectImgX->h = (imgW / 2);

		srcRectImgY->x = (imgW / 2) * 1;
		srcRectImgY->y = (imgH / 2) * 0;
		srcRectImgY->w = (imgW / 2);
		srcRectImgY->h = (imgW / 2);

		srcRectImgUndo->x = (imgW / 2) * 0;
		srcRectImgUndo->y = (imgH / 2) * 1;
		srcRectImgUndo->w = (imgW / 2);
		srcRectImgUndo->h = (imgW / 2);

		srcRectImgWhiteX->x = (imgW / 2) * 1;
		srcRectImgWhiteX->y = (imgH / 2) * 1;
		srcRectImgWhiteX->w = (imgW / 2);
		srcRectImgWhiteX->h = (imgW / 2);
	}

	{
		loadFromRenderedText(gRenderer, textResetBoard, gFont, "Reset Board", {255, 255 ,255 ,255});
		loadFromRenderedText(gRenderer, textXScore, gFont, "-" + std::to_string(ticTacToe->playerScore), { 255, 255 ,255 ,255 });
		loadFromRenderedText(gRenderer, textOScore, gFont, "-" + std::to_string(ticTacToe->botScore), { 255, 255 ,255 ,255 });
	}

	{
		rectUndo = new SDL_Rect;
		rectUndo->w = 30;
		rectUndo->h = 30;
		rectUndo->y = 14;
		rectUndo->x = S_WIDTH - rectUndo->w - 10;
	}


	isRunning = true;
}

void Game::resetBoard() {
	loadFromRenderedText(gRenderer, textXScore, gFont, "-" + std::to_string(ticTacToe->playerScore), { 255, 255 ,255 ,255 });
	loadFromRenderedText(gRenderer, textOScore, gFont, "-" + std::to_string(ticTacToe->botScore), { 255, 255 ,255 ,255 });
	for (int i = 0; i < sizeof(ticTacToe->moves); i++) {
		ticTacToe->moves[i] = 0;
	}
}

void Game::checkWinCondition() {
	if (!gameFinish) {
		if ((ticTacToe->moves[0] == 1) && (ticTacToe->moves[1] == 1) && (ticTacToe->moves[2] == 1) ||
			(ticTacToe->moves[3] == 1) && (ticTacToe->moves[4] == 1) && (ticTacToe->moves[5] == 1) || 
			(ticTacToe->moves[6] == 1) && (ticTacToe->moves[7] == 1) && (ticTacToe->moves[8] == 1) || 
			(ticTacToe->moves[0] == 1) && (ticTacToe->moves[3] == 1) && (ticTacToe->moves[6] == 1) || 
			(ticTacToe->moves[1] == 1) && (ticTacToe->moves[4] == 1) && (ticTacToe->moves[7] == 1) || 
			(ticTacToe->moves[2] == 1) && (ticTacToe->moves[5] == 1) && (ticTacToe->moves[8] == 1) || 
			(ticTacToe->moves[0] == 1) && (ticTacToe->moves[4] == 1) && (ticTacToe->moves[8] == 1) || 
			(ticTacToe->moves[6] == 1) && (ticTacToe->moves[4] == 1) && (ticTacToe->moves[2] == 1)) {
			gameFinish = true;
			playerScored = true;
			++ticTacToe->playerScore;
		}

		if ((ticTacToe->moves[0] == 2) && (ticTacToe->moves[1] == 2) && (ticTacToe->moves[2] == 2) ||
			(ticTacToe->moves[3] == 2) && (ticTacToe->moves[4] == 2) && (ticTacToe->moves[5] == 2) ||
			(ticTacToe->moves[6] == 2) && (ticTacToe->moves[7] == 2) && (ticTacToe->moves[8] == 2) ||
			(ticTacToe->moves[0] == 2) && (ticTacToe->moves[3] == 2) && (ticTacToe->moves[6] == 2) ||
			(ticTacToe->moves[1] == 2) && (ticTacToe->moves[4] == 2) && (ticTacToe->moves[7] == 2) ||
			(ticTacToe->moves[2] == 2) && (ticTacToe->moves[5] == 2) && (ticTacToe->moves[8] == 2) ||
			(ticTacToe->moves[0] == 2) && (ticTacToe->moves[4] == 2) && (ticTacToe->moves[8] == 2) ||
			(ticTacToe->moves[6] == 2) && (ticTacToe->moves[4] == 2) && (ticTacToe->moves[2] == 2)) {
			gameFinish = true;
			playerScored = false;
			++ticTacToe->botScore;
		}

		if (!gameFinish) {
			bool full = true;
			for (int i = 0; i < sizeof(ticTacToe->moves); i++) {
				if (ticTacToe->moves[i] == 0) full = false;
			}

			draw = full;
			gameFinish = full;
		}

		if (gameFinish) {
			{
				std::string message = draw ? "DRAW" : playerScored ? "YOU SCORED" : "BOT SCORED";
				SDL_Color color = { 0, 0, 0, 255 };
				if (draw) {
					color = { 102, 102, 102, 255 };
				} else {
					if (playerScored) color = { 48, 255, 48, 255 };
					else color = { 255, 71, 71, 255 };
				}

				loadFromRenderedText(gRenderer, textCondition, gFont, message, color);
			}

			{
				TICK_STARTED = SDL_GetTicks();
				loadFromRenderedText(gRenderer, textXScore, gFont, "-" + std::to_string(ticTacToe->playerScore), { 255, 255 ,255 ,255 });
				loadFromRenderedText(gRenderer, textOScore, gFont, "-" + std::to_string(ticTacToe->botScore), { 255, 255 ,255 ,255 });
			}
		}
		
		
	}
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			isRunning = false;
		} else if (gEvent.type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&mouseX, &mouseY);
			if (mouseX > rectResetBoard.x && mouseX < rectResetBoard.x + rectResetBoard.w &&
				mouseY > rectResetBoard.y && mouseY < rectResetBoard.y + rectResetBoard.h) {
				insideReset = true;
				insideUndo = false;
				SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
			} else if (mouseX > rectUndo->x && mouseX < rectUndo->x + rectUndo->w &&
				mouseY > rectUndo->y && mouseY < rectUndo->y + rectUndo->h) {
				insideReset = false;
				insideUndo = true;
				SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
			} else {
				insideReset = false;
				insideUndo = false;
				SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
			}

		} else if (gEvent.type == SDL_MOUSEBUTTONDOWN) {
			if (gEvent.button.button == SDL_BUTTON_LEFT) {
				SDL_GetMouseState(&mouseX, &mouseY);
				if (!mouseClickInProgress) {
					mouseClickFinished = true;
					mouseClickInProgress = true;
				}
			}
		} else if (gEvent.type == SDL_MOUSEBUTTONUP) {
			if (gEvent.button.button == SDL_BUTTON_LEFT) {
				mouseClickFinished = true;
				mouseClickInProgress = false;
			}
		}
	}
}

void Game::update() {
	if (mouseClickFinished && mouseClickInProgress && insideReset) { 
		invoker->pressButton();
		resetBoard(); 
	} 
	if (mouseClickFinished && mouseClickInProgress && insideUndo) {
		invoker->undo();
		invoker->undo();
		loadFromRenderedText(gRenderer, textXScore, gFont, "-" + std::to_string(ticTacToe->playerScore), { 255, 255 ,255 ,255 });
		loadFromRenderedText(gRenderer, textOScore, gFont, "-" + std::to_string(ticTacToe->botScore), { 255, 255 ,255 ,255 });
	}

	

	if (mouseClickFinished && mouseClickInProgress && !gameFinish) {
		mouseClickFinished = false; 
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				if (mouseX > grid[row][col].x && mouseX < grid[row][col].x + grid[row][col].w &&
					mouseY > grid[row][col].y && mouseY < grid[row][col].y + grid[row][col].h) {

					ticTacToe->movePos = row * 3 + col;
					ticTacToe->moveState = 1;
					invoker->pressButton();

					if (ticTacToe->successMove)
						playerMove = false;
				}
			}
		}
	}

	if (ticTacToe->successMove) checkWinCondition();

	if (!playerMove && !gameFinish) {
		bool moveMade = false;
		while (!moveMade) {
			int randomPos = getRandomNumber();
			ticTacToe->movePos = randomPos;
			ticTacToe->moveState = 2;
			invoker->pressButton();

			if (ticTacToe->successMove) {
				moveMade = true;
				playerMove = true;
			}
		}
	}

	if (ticTacToe->successMove) checkWinCondition();

	if (gameFinish) {
		uint32_t time_elapsed = SDL_GetTicks() - TICK_STARTED;

		if (time_elapsed > FINISH_DURATION) {
			resetBoard();
			gameFinish = false;
			playerMove = true;
			draw = false;
		}
	}
}


void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);

	// Render grid
	{
		constexpr static uint8_t HEIGHT_PADDING = 60;
		int gridHeight = S_HEIGHT - 2 * HEIGHT_PADDING;

		int cellWidth = S_WIDTH / 3;
		int cellHeight = gridHeight / 3;

		int startY = HEIGHT_PADDING;
		int startX = 0;

		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				grid[row][col] = { startX + cellWidth * col, startY + cellHeight * row, cellWidth, cellHeight };

				SDL_SetRenderDrawColor(gRenderer, 20, 189, 172, 255);
				SDL_RenderFillRect(gRenderer, &grid[row][col]);

				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderDrawRect(gRenderer, &grid[row][col]); 

				auto index = ticTacToe->moves[row * 3 + col];
				if (index == 1) {
					SDL_RenderCopy(gRenderer, image, srcRectImgX, &grid[row][col]);
				}
				if (index == 2) {
					SDL_RenderCopy(gRenderer, image, srcRectImgY, &grid[row][col]);
				}
			}
		}
	}

	// Render text reset board
	{
		rectResetBoard.x = (S_WIDTH / 2) - (rectResetBoard.w / 2);
		rectResetBoard.y = S_HEIGHT - rectResetBoard.h - 20;

		SDL_RenderCopy(gRenderer, textResetBoard, nullptr, &rectResetBoard);
	}

	// Render scores
	{
		SDL_Rect playerX = { 10, 15, 30, 30 };
		SDL_RenderCopy(gRenderer, image, srcRectImgWhiteX, &playerX);

		SDL_Rect playerXScore = { 40, 17, 60, 25 };
		SDL_RenderCopy(gRenderer, textXScore, nullptr, &playerXScore);

		SDL_Rect playerY = { 110, 12, 32, 32 };
		SDL_RenderCopy(gRenderer, image, srcRectImgY, &playerY);

		SDL_Rect playerYScore = { 140, 17, 60, 25 };
		SDL_RenderCopy(gRenderer, textOScore, nullptr, &playerYScore);
	}

	// Render undo
	{
		SDL_RenderCopy(gRenderer, image, srcRectImgUndo, rectUndo);
	}

	// Render winning result
	if (gameFinish) {
		SDL_Rect dstTextCondition = { 210, 15, 130, 30 };
		SDL_RenderCopy(gRenderer, textCondition, nullptr, &dstTextCondition);
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