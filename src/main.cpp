#include <iostream>
#include <sstream>
#include "SDL/SetupSDL.h"
#include "Vector2.h"
#include "CPU-6502/Bus.h"
#include "ScreenInfo.h"
#include "SDL/Input.h"

static TTF_Font* font;
const int FONT_SIZE = 20;

std::shared_ptr<Cartridge> cartridge;
Bus nes;
std::map<uint16_t, std::string> disassembly;

bool emulationRunning = false;
unsigned long nextFrameTime = 0;

uint8_t selectedPalette = 0x00;

void update();
void init();

std::string hex(uint16_t n, int d) {
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4) {
        s[i] = "0123456789ABCDEF"[n & 0xF];
    }
    return s;
}

void drawText(const std::string& text, Vector2 pos, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {static_cast<int>(pos.x), static_cast<int>(pos.y), surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawRam(Vector2 pos, uint16_t address, int nRows, int nCols) {
    SDL_Color color = {255, 255, 255};
    for (int row = 0; row < nRows; row++) {
        std::string s = "$" + hex(address, 4) + ": ";
        for (int col = 0; col < nCols; col++) {
            s += hex(nes.cpuRead(address), 2) + " ";
            address++;
        }
        drawText(s.c_str(), pos, color);
        pos.y += FONT_SIZE+2;
    }
}

// Draw status's green if true, red if false
void drawCpu(Vector2 pos) {
    SDL_Color white = {255, 255, 255};
    SDL_Color red = {197, 15, 31};
    SDL_Color green = {19, 161, 14};
    
    auto tempPos = pos;
    drawText("STATUS:", tempPos, white);
    tempPos.x += FONT_SIZE*5;
    drawText("N", tempPos, nes.cpu.GetFlag(olc6502::N) ? green : red);
    tempPos.x += FONT_SIZE+2;
    drawText("V", tempPos, nes.cpu.GetFlag(olc6502::V) ? green : red);
    tempPos.x += FONT_SIZE+2;
    drawText("-", tempPos, white);
    tempPos.x += FONT_SIZE+2;
    drawText("B", tempPos, nes.cpu.GetFlag(olc6502::B) ? green : red);
    tempPos.x += FONT_SIZE+2;
    drawText("D", tempPos, nes.cpu.GetFlag(olc6502::D) ? green : red);
    tempPos.x += FONT_SIZE+2;
    drawText("I", tempPos, nes.cpu.GetFlag(olc6502::I) ? green : red);
    tempPos.x += FONT_SIZE+2;
    drawText("Z", tempPos, nes.cpu.GetFlag(olc6502::Z) ? green : red);
    tempPos.x += FONT_SIZE+2;
    drawText("C", tempPos, nes.cpu.GetFlag(olc6502::C) ? green : red);
    
    tempPos = pos;
    tempPos.y += FONT_SIZE+2;
    
    drawText("PC: $" + hex(nes.cpu.pc, 4), tempPos, white);
    tempPos.y += FONT_SIZE+2;
    drawText("A: $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]", tempPos, white);
    tempPos.y += FONT_SIZE+2;
    drawText("X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]", tempPos, white);
    tempPos.y += FONT_SIZE+2;
    drawText("Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]", tempPos, white);
    tempPos.y += FONT_SIZE+2;
    drawText("Stack P: $" + hex(nes.cpu.stkp, 4), tempPos, white);
}

void drawCode(Vector2 pos, int nLines) {
    SDL_Color white = {255, 255, 255};
    SDL_Color cyan = {58, 150, 221};
    auto it_a = disassembly.find(nes.cpu.pc);
    int nLineY = (nLines >> 1) * (FONT_SIZE + 2) + pos.y;
    if (it_a != disassembly.end()) {
        drawText(it_a->second, Vector2(pos.x, nLineY), cyan);
        while (nLineY < pos.y + nLines * (FONT_SIZE + 2)) {
            nLineY += FONT_SIZE + 2;
            it_a++;
            if (it_a != disassembly.end()) {
                drawText(it_a->second, Vector2(pos.x, nLineY), white);
            }
        }
    }

    it_a = disassembly.find(nes.cpu.pc);
    nLineY = (nLines >> 1) * (FONT_SIZE + 2) + pos.y;
    if (it_a != disassembly.end()) {
        while (nLineY > pos.y) {
            nLineY -= FONT_SIZE + 2;
            it_a--;
            if (it_a != disassembly.end()) {
                drawText(it_a->second, Vector2(pos.x, nLineY), white);
            }
        }
    }
}

int main() {
    if (TTF_Init() < 0) {
        printf("Failed to start SDL_ttf: %s", TTF_GetError());
        exit(-1);
    }

    font = TTF_OpenFont("res/RobotoMono-Regular.ttf", FONT_SIZE);
    if (font == nullptr) {
        printf("Failed to load font: %s", TTF_GetError());
        exit(-1);
    }
    InitSDL();
    
    init();

    while (!done) {
        handleInput();
        
        SDL_SetRenderDrawColor(renderer, 1, 36, 86, 255);
        SDL_RenderClear(renderer);

        update();

        SDL_RenderPresent(renderer);
    }
    
    return 0;
}

void init() {
    // Load the cartridge
    cartridge = std::make_shared<Cartridge>("nestest.nes");
    
    // Insert into NES
    nes.insertCartridge(cartridge);
    
    // Extract dissassembly
    disassembly = nes.cpu.disassemble(0x0000, 0xFFFF);
    
    // Reset NES
    nes.reset();
}

void update() {
    if (emulationRunning) {
        
        if (SDL_GetTicks64() > nextFrameTime) {
            do {
                nes.clock();
            } while (!nes.ppu.frame_complete);
            nes.ppu.frame_complete = false;
            
            nextFrameTime = SDL_GetTicks64() + 1000.0f/60.0f;
        }
        
    } else {
        // Emulate code step-by-step
        if (isKeyPressed(SDL_SCANCODE_C)) {
            // Clock enough times to execute a whole instruction
            do {
                nes.clock();
            } while (!nes.cpu.complete());
            // Drain the clock to complete the instruction
            do {
                nes.clock();
            } while (nes.cpu.complete());
        }

        // Emulate one whole frame
        if (isKeyPressed(SDL_SCANCODE_F)) {
            do {
                nes.clock();
            } while (!nes.ppu.frame_complete);

            // Use residual cycles to complete current instruction
            do {
                nes.clock();
            } while (!nes.cpu.complete());

            // Reset frame completion flag
            nes.ppu.frame_complete = false;
        }
    }
    
    // Reset
    if (isKeyPressed(SDL_SCANCODE_R)) {
        nes.reset();
    }
    // Toggle emulation
    if (isKeyPressed(SDL_SCANCODE_SPACE)) {
        emulationRunning = !emulationRunning;
    }
    // Toggle pattern table
    if (isKeyPressed(SDL_SCANCODE_P)) {
        (++selectedPalette) &= 0x07;
    }

    drawCpu( Vector2(nes_width*4 + 10, 2));
    drawCode(Vector2(nes_width*4 + 10, 2 + (FONT_SIZE+2) * 7), 22);
    
    // Copy the ppu screen scaled x3
    SDL_UpdateTexture(nes.ppu.screenTexture, nullptr, &nes.ppu.screenBuffer, nes_width * sizeof(uint32_t));
    auto dstRect = SDL_Rect{0, 0, nes_width*4, nes_height*4};
    SDL_RenderCopy(renderer, nes.ppu.GetScreen(), nullptr, &dstRect);
    
    // Draw palettes
    const int swatchSize = 13;
    for (int p = 0; p < 8; p++) {
        for (int s = 0; s < 4; s++) {
            auto color = nes.ppu.GetColorFromPaletteRam(p, s);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_Rect rect = { 1034 + p * (swatchSize * 5) + s * swatchSize , SCREEN_HEIGHT-pattern_width*2-32, swatchSize, swatchSize };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    
    // Draw the selected palette outline
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = { 1034 + selectedPalette * (swatchSize * 5) - 1, SCREEN_HEIGHT-pattern_width*2-33, swatchSize * 4 + 1, swatchSize + 1 };
    SDL_RenderDrawRect(renderer, &rect);
    
    // Draw the pattern tables
    SDL_Texture* patternTable1 = nes.ppu.GetPatternTable(0, selectedPalette);
    SDL_Texture* patternTable2 = nes.ppu.GetPatternTable(1, selectedPalette);
    SDL_UpdateTexture(patternTable1, nullptr, &nes.ppu.patternTableBuffer[0], pattern_width * sizeof(uint32_t));
    SDL_UpdateTexture(patternTable2, nullptr, &nes.ppu.patternTableBuffer[1], pattern_width * sizeof(uint32_t));

    auto pattern1Rect = SDL_Rect{nes_width*4 + 10, SCREEN_HEIGHT-pattern_width*2-10, pattern_width*2, pattern_width*2};
    SDL_RenderCopy(renderer, patternTable1, nullptr, &pattern1Rect);
    auto pattern2Rect = SDL_Rect{(nes_width*4 + 10)+pattern_width*2+10, SCREEN_HEIGHT-pattern_width*2-10, pattern_width*2, pattern_width*2};
    SDL_RenderCopy(renderer, patternTable2, nullptr, &pattern2Rect);
}
