#include <iostream>
#include "PPU.h"
#include "Cartridge.h"
#include "../SDL/SetupSDL.h"

PPU::PPU() {
    palScreen[0x00] = SDL_Color(84, 84, 84);
    palScreen[0x01] = SDL_Color(0, 30, 116);
    palScreen[0x02] = SDL_Color(8, 16, 144);
    palScreen[0x03] = SDL_Color(48, 0, 136);
    palScreen[0x04] = SDL_Color(68, 0, 100);
    palScreen[0x05] = SDL_Color(92, 0, 48);
    palScreen[0x06] = SDL_Color(84, 4, 0);
    palScreen[0x07] = SDL_Color(60, 24, 0);
    palScreen[0x08] = SDL_Color(32, 42, 0);
    palScreen[0x09] = SDL_Color(8, 58, 0);
    palScreen[0x0A] = SDL_Color(0, 64, 0);
    palScreen[0x0B] = SDL_Color(0, 60, 0);
    palScreen[0x0C] = SDL_Color(0, 50, 60);
    palScreen[0x0D] = SDL_Color(0, 0, 0);
    palScreen[0x0E] = SDL_Color(0, 0, 0);
    palScreen[0x0F] = SDL_Color(0, 0, 0);

    palScreen[0x10] = SDL_Color(152, 150, 152);
    palScreen[0x11] = SDL_Color(8, 76, 196);
    palScreen[0x12] = SDL_Color(48, 50, 236);
    palScreen[0x13] = SDL_Color(92, 30, 228);
    palScreen[0x14] = SDL_Color(136, 20, 176);
    palScreen[0x15] = SDL_Color(160, 20, 100);
    palScreen[0x16] = SDL_Color(152, 34, 32);
    palScreen[0x17] = SDL_Color(120, 60, 0);
    palScreen[0x18] = SDL_Color(84, 90, 0);
    palScreen[0x19] = SDL_Color(40, 114, 0);
    palScreen[0x1A] = SDL_Color(8, 124, 0);
    palScreen[0x1B] = SDL_Color(0, 118, 40);
    palScreen[0x1C] = SDL_Color(0, 102, 120);
    palScreen[0x1D] = SDL_Color(0, 0, 0);
    palScreen[0x1E] = SDL_Color(0, 0, 0);
    palScreen[0x1F] = SDL_Color(0, 0, 0);

    palScreen[0x20] = SDL_Color(236, 238, 236);
    palScreen[0x21] = SDL_Color(76, 154, 236);
    palScreen[0x22] = SDL_Color(120, 124, 236);
    palScreen[0x23] = SDL_Color(176, 98, 236);
    palScreen[0x24] = SDL_Color(228, 84, 236);
    palScreen[0x25] = SDL_Color(236, 88, 180);
    palScreen[0x26] = SDL_Color(236, 106, 100);
    palScreen[0x27] = SDL_Color(212, 136, 32);
    palScreen[0x28] = SDL_Color(160, 170, 0);
    palScreen[0x29] = SDL_Color(116, 196, 0);
    palScreen[0x2A] = SDL_Color(76, 208, 32);
    palScreen[0x2B] = SDL_Color(56, 204, 108);
    palScreen[0x2C] = SDL_Color(56, 180, 204);
    palScreen[0x2D] = SDL_Color(60, 60, 60);
    palScreen[0x2E] = SDL_Color(0, 0, 0);
    palScreen[0x2F] = SDL_Color(0, 0, 0);

    palScreen[0x30] = SDL_Color(236, 238, 236);
    palScreen[0x31] = SDL_Color(168, 204, 236);
    palScreen[0x32] = SDL_Color(188, 188, 236);
    palScreen[0x33] = SDL_Color(212, 178, 236);
    palScreen[0x34] = SDL_Color(236, 174, 236);
    palScreen[0x35] = SDL_Color(236, 174, 212);
    palScreen[0x36] = SDL_Color(236, 180, 176);
    palScreen[0x37] = SDL_Color(228, 196, 144);
    palScreen[0x38] = SDL_Color(204, 210, 120);
    palScreen[0x39] = SDL_Color(180, 222, 120);
    palScreen[0x3A] = SDL_Color(168, 226, 144);
    palScreen[0x3B] = SDL_Color(152, 226, 180);
    palScreen[0x3C] = SDL_Color(160, 214, 228);
    palScreen[0x3D] = SDL_Color(160, 162, 160);
    palScreen[0x3E] = SDL_Color(0, 0, 0);
    palScreen[0x3F] = SDL_Color(0, 0, 0);
}

SDL_Texture *PPU::GetScreen() const {
    return screenTexture;
}

SDL_Texture *PPU::GetNameTable(uint8_t i) {
    return nameTableTexture[i];
}

SDL_Texture *PPU::GetPatternTable(uint8_t i, uint8_t palette) {
    for (uint16_t nTileY = 0; nTileY < 16; nTileY++) {
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++) {
            uint16_t nOffset = nTileY * 256 + nTileX * 16;

            for (uint16_t row = 0; row < 8; row++) {
                uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000);
                uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008);

                for (uint16_t col = 0; col < 8; col++) {
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
                    tile_lsb >>= 1; tile_msb >>= 1;

                    auto x = nTileX * 8 + (7 - col);
                    auto y = nTileY * 8 + row;
                    DrawPixel(patternTableBuffer[i], x, y, GetColorFromPaletteRam(palette, pixel), pattern_width, pattern_width);
                }
            }
        }
    }
    
    return patternTableTexture[i];
}

SDL_Color PPU::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel) {
    return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

uint8_t PPU::cpuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;

    switch (addr)
    {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }

    return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr)
    {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }
}

uint8_t PPU::ppuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;
    
    if (cartridge->ppuRead(addr, data)) {
        
    }
    // Pattern table
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    // Name table
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        
    }
    // Palette RAM
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        // Mirroring
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        
        data = tblPalette[addr];
    }
    
    return data;
}

void PPU::ppuWrite(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;
    
    if (cartridge->ppuWrite(addr, data)) {
        
    }
    // Pattern table
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // Usually a ROM
        tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    // Name table
    else if (addr >= 0x2000 && addr <= 0x3EFF) {

    }
    // Palette RAM
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        // Mirroring
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;

        tblPalette[addr] = data;
    }
}

void PPU::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cartridge = cartridge;
    
    // Make screenTexture
    if (screenTexture != nullptr) {
        SDL_DestroyTexture(screenTexture);
    }
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, nes_width, nes_height);
    
    // Make nameTableTexture
    if (nameTableTexture[0] != nullptr) {
        SDL_DestroyTexture(nameTableTexture[0]);
    }
    if (nameTableTexture[1] != nullptr) {
        SDL_DestroyTexture(nameTableTexture[1]);
    }
    nameTableTexture[0] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, nes_width, nes_height);
    nameTableTexture[1] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, nes_width, nes_height);
    
    // Make patternTableTexture
    if (patternTableTexture[0] != nullptr) {
        SDL_DestroyTexture(patternTableTexture[0]);
    }
    if (patternTableTexture[1] != nullptr) {
        SDL_DestroyTexture(patternTableTexture[1]);
    }
    patternTableTexture[0] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, pattern_width, pattern_width);
    patternTableTexture[1] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, pattern_width, pattern_width);
}

void PPU::clock() {
    // Fake some noise for now
    DrawPixel(screenBuffer, cycle - 1, scanline,  palScreen[(rand() % 2) ? 0x3F : 0x30]);

    // Advance renderer - it never stops, it's relentless
    cycle++;
    if (cycle >= 341) {
        cycle = 0;
        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frame_complete = true;
        }
    }
}

void PPU::DrawPixel(uint32_t buffer[], int x, int y, SDL_Color c, int maxX, int maxY) {
    if (x >= maxX || y >= maxY || x < 0 || y < 0) {
        return;
    }

    buffer[(y * maxX) + x] = (c.r << 24) | (c.g << 16) | (c.b << 8) | 0xFF;
}
