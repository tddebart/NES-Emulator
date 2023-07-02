#include <iostream>
#include <sstream>
#include "SDL/SetupSDL.h"
#include "Vector2.h"
#include "CPU-6502/Bus.h"
#include "ScreenInfo.h"
#include "SDL/Input.h"

static TTF_Font* font;
const int FONT_SIZE = 20;

Bus nes;
std::map<uint16_t, std::string> disassembly;

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
            s += hex(nes.read(address), 2) + " ";
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
    // Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
    /*
        *=$8000
        LDX #10
        STX $0000
        LDX #3
        STX $0001
        LDY $0000
        LDA #0
        CLC
        loop
        ADC $0001
        DEY
        BNE loop
        STA $0002
        NOP
        NOP
        NOP
    */

    // Convert hex string into bytes for RAM
    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        nes.ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
    }
    
    // Set Reset Vector
    nes.ram[0xFFFC] = 0x00;
    nes.ram[0xFFFD] = 0x80;
    
    disassembly = nes.cpu.disassemble(0x0000, 0xFFFF);
    
    nes.cpu.reset();
}

void update() {
    // Input
    if (isKeyPressed(SDL_SCANCODE_SPACE)) {
        do {
            nes.cpu.clock();
        } while (!nes.cpu.complete());
    }
    
    if (isKeyPressed(SDL_SCANCODE_R)) {
        nes.cpu.reset();
    }
    
    if (isKeyPressed(SDL_SCANCODE_I)) {
        nes.cpu.irq();
    }
    
    if (isKeyPressed(SDL_SCANCODE_N)) {
        nes.cpu.nmi();
    }
    
    drawRam(Vector2(2, 2), 0x0000, 16, 16);
    drawRam(Vector2(2, 2 + (FONT_SIZE+2) * 18), 0x8000, 16, 16);
    drawCpu(Vector2(SCREEN_WIDTH - (2 + (FONT_SIZE+2) * 17), 2));
    drawCode(Vector2(SCREEN_WIDTH - (2 + (FONT_SIZE+2) * 17), 2 + (FONT_SIZE+2) * 7), 26);

    drawText("SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI", Vector2(2, SCREEN_HEIGHT - (FONT_SIZE - 2) * 3), {255, 255, 255});
}
