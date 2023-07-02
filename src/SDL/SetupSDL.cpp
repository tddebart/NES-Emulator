#include <Windows.h>
#include "../ScreenInfo.h"
#include "SetupSDL.h"

SDL_Window *window;
SDL_Renderer *renderer;
bool done = false;

void InitSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to start SDL: %s", SDL_GetError());
        exit(-1);
    }
    
    SetProcessDPIAware();

    window = SDL_CreateWindow("My SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr) {
        printf("Failed to create window: %s", SDL_GetError());
        exit(-1);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("Failed to create renderer: %s", SDL_GetError());
        exit(-1);
    }
}

void handleInput() {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                done = true;
                break;

            default:
                break;
        }
    }
}