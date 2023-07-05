#pragma once


#include <cstdint>
#include <memory>
#include <SDL.h>
#include "Cartridge.h"

const int nes_width = 256;
const int nes_height = 240;
const int pattern_width = 128;

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
    
    SDL_Texture* screenTexture;
    uint32_t screenBuffer[nes_height * nes_width];
    SDL_Texture* nameTableTexture[2];
    uint32_t nameTableBuffer[2][256 * 240];
    SDL_Texture* patternTableTexture[2];
    uint32_t patternTableBuffer[2][pattern_width * pattern_width];
private:
    // The Cartridge or "GamePak"
    std::shared_ptr<Cartridge> cartridge;
    
    
    SDL_Color palScreen[0x40];
    
    int16_t scanline = 0; // Row
    int16_t cycle = 0; // Column
    
public:
    // Debugging Utilities
    void DrawPixel(uint32_t buffer[], int x, int y, SDL_Color c, int maxX = nes_width, int maxY = nes_height);
    SDL_Texture* GetScreen() const;
    SDL_Texture* GetNameTable(uint8_t i);
    SDL_Texture* GetPatternTable(uint8_t i, uint8_t palette);
    bool frame_complete = false;

    SDL_Color GetColorFromPaletteRam(uint8_t palette, uint8_t pixel);
};
