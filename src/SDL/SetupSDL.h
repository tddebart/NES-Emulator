﻿#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern bool done;

void InitSDL();

void handleInput();
