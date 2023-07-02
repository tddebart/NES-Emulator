#pragma once

#include <SDL.h>
#include <vector>

static bool isKeyDown(SDL_Scancode key) {
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    return state[key];
}

static bool isKeyPressed(SDL_Scancode key) {
    static std::vector<bool> isPressed(SDL_NUM_SCANCODES, false);
    if (isKeyDown(key)) {
        if (!isPressed[key]) {
            isPressed[key] = true;
            return true;
        }
    } else {
        isPressed[key] = false;
    }
    return false;
}

static bool isMouseDown(int button) {
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button);
}

static bool isMousePressed(int button) {
    static std::vector<bool> isPressed(3, false);
    if (isMouseDown(button)) {
        if (!isPressed[button-1]) {
            isPressed[button-1] = true;
            return true;
        }
    } else {
        isPressed[button-1] = false;
    }
    return false;
}

static SDL_Point getMousePosition() {
    SDL_Point mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
    return mousePosition;
}
