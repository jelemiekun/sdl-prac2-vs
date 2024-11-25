#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <unordered_map>

class Game {
private:
	struct Entity {
		uint16_t id;
		SDL_Rect dstRect;
		int velX;
		int velY;
		SDL_Color color;
	};

	class Command {
	public:
		virtual void execute(Entity& entity) = 0;
	};

	class JumpCommand : public Command {
	public:
		void execute(Entity& entity) override {
			entity.velY = -20;
		}
	};

	class MoveCommand : public Command {
	public:
		void execute(Entity& entity) override {
			entity.dstRect.x += entity.velX;
			entity.dstRect.y += entity.velY;
		}
	};

	class Invoker {
	public:
		enum class KEYS {
			BTN_JUMP,
			NONE
		};

	private:
		std::unordered_map<KEYS, std::shared_ptr<Command>> keyCommands;
		std::unordered_map<int, std::shared_ptr<Entity>> entities;

	public:
		void addEntity(std::shared_ptr<Entity> entity) {
			entities[entity->id] = entity;
		}

		void assignKeyToComponents(KEYS key, std::shared_ptr<Command> command) {
			keyCommands[key] = command;
		}

		void pressButton(KEYS key) {
			auto it = keyCommands.find(key);
			if (it != keyCommands.end()) {
				for (auto& entity : entities) {
					it->second->execute(*entity.second);
				}
			} else {
				std::cout << "No command assigned for this key!" << std::endl;
			}
		}

	};

public:
	Game();
	~Game();

	void init(const char* title, const int& rXPos, const int& rYPos, const int& rWidth, const int& rHeight, const bool& rFullscreen);
	void input();
	void update();
	void render();
	void clean();

	bool running();

	void reset();
	void loadScore();

	constexpr static int PIPE_GAP = 230;
	static SDL_Renderer* gRenderer;

private:
	bool isRunning;
	SDL_Window* gWindow;
	SDL_Event gEvent;

	TTF_Font* gFont;

	std::shared_ptr<JumpCommand> jumpCommand = std::make_shared<JumpCommand>();
	std::shared_ptr<MoveCommand> moveCommand = std::make_shared<MoveCommand>();

	Invoker birdController;
	std::shared_ptr<Entity> bird;

	Invoker pipesController;
	std::shared_ptr<Entity> pipe1;
	std::shared_ptr<Entity> pipe2;

	SDL_Texture* scoreTexture;
	int score;
};

