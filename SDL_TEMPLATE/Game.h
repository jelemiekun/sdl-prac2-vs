#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <memory>
#include <unordered_map>

class Memento {
public:
	uint8_t mId;
	uint8_t mMoves[9];
	int playerScore;
	int botScore;

	Memento(uint8_t id, const uint8_t* moves, int p, int b) : mId(id), 
		playerScore(p), botScore(b) {
		std::memcpy(mMoves, moves, sizeof(mMoves));
	}
};

struct Entity{
	uint8_t id;
	uint8_t moves[9] = { 0 };
	int playerScore;
	int botScore;

	int movePos;
	int moveState;
	bool successMove;


	std::shared_ptr<Memento> saveState() {
		std::cout << "SAVED!!!!" << '\n';
		return std::make_shared<Memento>(id, moves, playerScore, botScore);
	}

	void restoreState(const Memento& memento) {
		id = memento.mId;
		std::memcpy(moves, memento.mMoves, sizeof(moves));
		playerScore = memento.playerScore;
		botScore = memento.botScore;
	}
};

class Command {
public:
	virtual void execute(Entity& entity) = 0;
};

class ClickCommand : public Command {
public:
	void execute(Entity& entity) override {
		if (entity.movePos < 0 || entity.movePos > 8) {
			std::cout << "Invalid move." << '\n';
			entity.successMove = false;
			return;
		}

		if (entity.moveState != 0 && entity.moveState != 1 && entity.moveState != 2) {
			std::cout << "Invalid move state." << '\n';
			entity.successMove = false;
			return;
		}

		if (entity.moves[entity.movePos] != 0) {
			std::cout << "Move position already occupied." << '\n';
			entity.successMove = false;
			return;
		}

		entity.moves[entity.movePos] = entity.moveState;
		entity.successMove = true;
	}
};

class Invoker {
private:
	ClickCommand click;
	std::vector<std::shared_ptr<Memento>> mementos;

public:
	Entity& mEntity;

	Invoker(Entity& entity) : mEntity(entity) {}

	void pressButton() {
		mementos.push_back(mEntity.saveState());
		click.execute(mEntity);

		if (!mEntity.successMove) mementos.pop_back();
	}

	void undo() {
		if (!mementos.empty()) {
			std::cout << "MEOW!!!!" << '\n';
			mEntity.restoreState(*mementos.back());
			mementos.pop_back();
		}
	}
};



class Game {
public:
	Game();
	~Game();

	void init(const char* title, const int& rXPos, const int& rYPos, const int& rWidth, const int& rHeight, const bool& rFullscreen);
	void input();
	void update();
	void render();
	void clean();

	bool running();

	void resetBoard();
	void checkWinCondition();

	constexpr static int PIPE_GAP = 230;
	static SDL_Renderer* gRenderer;

private:
	bool isRunning;
	SDL_Window* gWindow;
	SDL_Event gEvent;

	TTF_Font* gFont;

	Entity* ticTacToe;
	Invoker* invoker;

	SDL_Texture* image;
	SDL_Rect* srcRectImgX;
	SDL_Rect* srcRectImgY;
	SDL_Rect* srcRectImgUndo;
	SDL_Rect* srcRectImgWhiteX;

	SDL_Texture* textResetBoard;
	SDL_Rect rectResetBoard = { 0, 0, 130, 20 };

	SDL_Texture* textXScore;
	SDL_Texture* textOScore;

	SDL_Rect* rectUndo;

	SDL_Texture* textCondition;
};

