#pragma once
#include "SDL.h"

constexpr const char* S_TITLE = "Title";

constexpr auto S_XPOS = SDL_WINDOWPOS_CENTERED;
constexpr auto S_YPOS = SDL_WINDOWPOS_CENTERED;
constexpr auto S_WIDTH = 640;
constexpr auto S_HEIGHT = 640;
constexpr auto S_FLAGS = 0;

constexpr static SDL_Color BOT_COLOR = { 0, 0, 0, 255 };
constexpr static uint8_t BOT_WIDTH = 10;
constexpr static uint8_t BOT_HEIGHT = 10;
constexpr static uint8_t BOT_VELOCITY = 2;