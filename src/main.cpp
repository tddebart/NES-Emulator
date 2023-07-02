#include <iostream>
#include "SDL/SetupSDL.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    InitSDL();

    while (!done) {
        handleInput();

        SDL_RenderPresent(renderer);
    }
    
    return 0;
}
