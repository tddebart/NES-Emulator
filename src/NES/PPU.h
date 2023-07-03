#pragma once


#include <cstdint>
#include <memory>
#include <SDL.h>
#include "Cartridge.h"

const int nes_width = 256;
const int nes_height = 240;

class PPU {
private:
    uint8_t     tblName[2][1024];
    uint8_t		tblPalette[32];
    uint8_t     tblPattern[2][4096]; // Unused for now
    
public:
    PPU();

    // Communications with Main Bus
    uint8_t cpuRead(uint16_t addr, bool rdonly = false);
    void    cpuWrite(uint16_t addr, uint8_t  data);

    // Communications with PPU Bus
    uint8_t ppuRead(uint16_t addr, bool rdonly = false);
    void    ppuWrite(uint16_t addr, uint8_t data);

    void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
    
    SDL_Texture* texture;
    uint32_t buffer[nes_height*nes_width];
private:
    // The Cartridge or "GamePak"
    std::shared_ptr<Cartridge> cartridge;
    
    
    SDL_Color palScreen[0x40];
    
    int16_t scanline = 0; // Row
    int16_t cycle = 0; // Column
    
public:
    // Debugging Utilities
    void DrawPixel(int x, int y, SDL_Color c);
    SDL_Texture* GetScreen();
    bool frame_complete = false;
};
