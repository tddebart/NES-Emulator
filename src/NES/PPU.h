﻿#pragma once


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
    
private:
    union
    {
        struct
        {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };

        uint8_t reg;
    } status;

    union
    {
        struct
        {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };

        uint8_t reg;
    } mask;

    union PPUCTRL
    {
        struct
        {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1; // unused
            uint8_t enable_nmi : 1;
        };

        uint8_t reg;
    } control;
    
    uint8_t address_latch = 0x00;
    uint8_t ppu_data_buffer = 0x00;
    uint16_t ppu_addr = 0x0000;
};
