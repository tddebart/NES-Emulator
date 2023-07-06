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
                    uint8_t pixel = ((tile_lsb & 0x01) << 1) | (tile_msb & 0x01);
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
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
            status.vertical_blank = 0;
            address_latch = 0;
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            data = pOAM[oam_addr];
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            // Reads are delayed by one cycle, so output buffer which was written to
            // during the previous read is returned instead
            data = ppu_data_buffer;
            ppu_data_buffer = ppuRead(vram_addr.reg);
            
            // In pallete range, the data is not delayed
            if (vram_addr.reg >= 0x3F00) data = ppu_data_buffer;
            vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
    }

    return data;
}

void PPU::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr)
    {
        case 0x0000: // Control
            control.reg = data;
            tram_addr.nametable_x = control.nametable_x;
            tram_addr.nametable_y = control.nametable_y;
            break;
        case 0x0001: // Mask
            mask.reg = data;
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            oam_addr = data;
            break;
        case 0x0004: // OAM Data
            pOAM[oam_addr] = data;
            break;
        case 0x0005: // Scroll
            if (address_latch == 0) {
                fine_x = data & 0x07;
                tram_addr.coarse_x = data >> 3;
                address_latch = 1;
            }
            else {
                tram_addr.fine_y = data & 0x07;
                tram_addr.coarse_y = data >> 3;
                address_latch = 0;
            }
            break;
        case 0x0006: // PPU Address
            if (address_latch == 0) {
                tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
                address_latch = 1;
            }
            else {
                tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
                vram_addr = tram_addr;
                address_latch = 0;
            }
            break;
        case 0x0007: // PPU Data
            ppuWrite(vram_addr.reg, data);
            vram_addr.reg += (control.increment_mode ? 32 : 1);
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
        if (cartridge->mirror == Cartridge::MIRROR::VERTICAL) {
            // Vertical
            if (addr >= 0x2000 && addr <= 0x23FF) data = tblName[0][addr & 0x03FF];
            if (addr >= 0x2400 && addr <= 0x27FF) data = tblName[1][addr & 0x03FF];
            if (addr >= 0x2800 && addr <= 0x2BFF) data = tblName[0][addr & 0x03FF];
            if (addr >= 0x2C00 && addr <= 0x2FFF) data = tblName[1][addr & 0x03FF];
        }
        else if (cartridge->mirror == Cartridge::MIRROR::HORIZONTAL) {
            // Horizontal
            if (addr >= 0x2000 && addr <= 0x23FF) data = tblName[0][addr & 0x03FF];
            if (addr >= 0x2400 && addr <= 0x27FF) data = tblName[0][addr & 0x03FF];
            if (addr >= 0x2800 && addr <= 0x2BFF) data = tblName[1][addr & 0x03FF];
            if (addr >= 0x2C00 && addr <= 0x2FFF) data = tblName[1][addr & 0x03FF];
        }
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
        if (cartridge->mirror == Cartridge::MIRROR::VERTICAL) {
            // Vertical
            if (addr >= 0x2000 && addr <= 0x23FF) tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x2400 && addr <= 0x27FF) tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x2800 && addr <= 0x2BFF) tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x2C00 && addr <= 0x2FFF) tblName[1][addr & 0x03FF] = data;
        }
        else if (cartridge->mirror == Cartridge::MIRROR::HORIZONTAL) {
            // Horizontal
            if (addr >= 0x2000 && addr <= 0x23FF) tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x2400 && addr <= 0x27FF) tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x2800 && addr <= 0x2BFF) tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x2C00 && addr <= 0x2FFF) tblName[1][addr & 0x03FF] = data;
        }
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

    auto IncrementScrollX = [&]() {
        if (mask.render_background || mask.render_sprites) {
            if (vram_addr.coarse_x == 31) {
                vram_addr.coarse_x = 0;
                vram_addr.nametable_x = ~vram_addr.nametable_x;
            }
            else {
                vram_addr.coarse_x++;
            }
        }
    };
    
    auto IncrementScrollY = [&]() {
        if (mask.render_background || mask.render_sprites) {
            if (vram_addr.fine_y < 7) {
                vram_addr.fine_y++;
            }
            else {
                vram_addr.fine_y = 0;
                
                if (vram_addr.coarse_y == 29) {
                    vram_addr.coarse_y = 0;
                    vram_addr.nametable_y = ~vram_addr.nametable_y;
                }
                else if (vram_addr.coarse_y == 31) {
                    vram_addr.coarse_y = 0;
                }
                else {
                    vram_addr.coarse_y++;
                }
            }
        }
    };
    
    auto ResetAddressX = [&]() {
        if (mask.render_background || mask.render_sprites) {
            vram_addr.nametable_x = tram_addr.nametable_x;
            vram_addr.coarse_x = tram_addr.coarse_x;
        }
    };
    
    auto ResetAddressY = [&]() {
        if (mask.render_background || mask.render_sprites) {
            vram_addr.fine_y = tram_addr.fine_y;
            vram_addr.nametable_y = tram_addr.nametable_y;
            vram_addr.coarse_y = tram_addr.coarse_y;
        }
    };
    
    auto LoadBackgroundShifters = [&]() {
        bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
        bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
        
        bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
        bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
    };
    
    auto UpdateShifters = [&]() {
        if (mask.render_background) {
            bg_shifter_pattern_lo <<= 1;
            bg_shifter_pattern_hi <<= 1;
            
            bg_shifter_attrib_lo <<= 1;
            bg_shifter_attrib_hi <<= 1;
        }
        
        if (mask.render_sprites && cycle >= 1 && cycle < 258) {
            for (int i = 0; i < sprite_count; i++) {
                if (spriteScanline[i].x > 0) {
                    spriteScanline[i].x--;
                }
                else {
                    sprite_shifter_pattern_lo[i] <<= 1;
                    sprite_shifter_pattern_hi[i] <<= 1;
                }
            }
        }
    };
    
    if (scanline >= -1 && scanline < 240) {
        if (scanline == 0 && cycle == 0) {
            // Skip idle cycle
            cycle = 1;
        }
        
        if (scanline == -1 && cycle == 1) {
            status.vertical_blank = 0;
            status.sprite_zero_hit = 0;
            status.sprite_overflow = 0;
            
            for (int i = 0; i < 8; i++) {
                sprite_shifter_pattern_lo[i] = 0;
                sprite_shifter_pattern_hi[i] = 0;
            }
        }

        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {
            UpdateShifters();
            
            switch ((cycle - 1) % 8) {
                case 0:
                    LoadBackgroundShifters();
                    bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
                    break;
                case 2:
                    bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11)
                                                  | (vram_addr.nametable_x << 10)
                                                  | ((vram_addr.coarse_y >> 2) << 3)
                                                  | (vram_addr.coarse_x >> 2));
                    if (vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
                    if (vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
                    bg_next_tile_attrib &= 0x03;
                    break;
                case 4:
                    bg_next_tile_lsb = ppuRead((control.pattern_background << 12)
                                               + ((uint16_t)bg_next_tile_id << 4)
                                               + (vram_addr.fine_y) + 0);
                    break;
                case 6:
                    bg_next_tile_msb = ppuRead((control.pattern_background << 12)
                                               + ((uint16_t)bg_next_tile_id << 4)
                                               + (vram_addr.fine_y) + 8);
                    break;
                case 7:
                    IncrementScrollX();
                    break;
            }
        }
        
        if (cycle == 256) {
            IncrementScrollY();
        }
        
        if (cycle == 257) {
            LoadBackgroundShifters();
            ResetAddressX();
        }
        
        if (cycle == 338 || cycle == 340) {
            bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
        }

        //#region foreground rendering

        if (cycle == 257 && scanline >= 0) {
            std::memset(spriteScanline, 0xFF, 8 * sizeof(sObjectAttributeEntry));
            sprite_count = 0;
            
            uint8_t nOAMEntry = 0;
            bSpriteZeroHitPossible = false;
            while (nOAMEntry < 64 && sprite_count < 9) {
                int16_t diff = ((int16_t)scanline - (int16_t)OAM[nOAMEntry].y);
                if (diff >= 0 && diff < (control.sprite_size ? 16 : 8)) {
                    if (sprite_count < 8) {
                        if (nOAMEntry == 0) {
                            bSpriteZeroHitPossible = true;
                        }
                        
                        memcpy(&spriteScanline[sprite_count], &OAM[nOAMEntry], sizeof(sObjectAttributeEntry));
                    }
                    sprite_count++;
                }
                nOAMEntry++;
            }
            status.sprite_overflow = (sprite_count == 8);
        }

        if (cycle == 340) {
            for (uint8_t i = 0; i < sprite_count; i++) {
                uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
                uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;

                if (!control.sprite_size) {
                    // 8x8 Sprite Mode - The control register determines the pattern table
                    if (!(spriteScanline[i].attribute & 0x80)) {
                        // Sprite is NOT flipped vertically, i.e. normal
                        sprite_pattern_addr_lo = (control.pattern_sprite << 12)
                                                    | (spriteScanline[i].id << 4)
                                                    | (scanline - spriteScanline[i].y);
                    } else {
                        // Sprite is flipped vertically, i.e. upside down
                        sprite_pattern_addr_lo = (control.pattern_sprite << 12)
                                                    | (spriteScanline[i].id << 4)
                                                    | (7 - (scanline - spriteScanline[i].y));
                    }
                } else {
                    // 8x16 Sprite Mode - The sprite attribute determines the pattern table
                    if (!(spriteScanline[i].attribute & 0x80)) {
                        // Sprite is NOT flipped vertically, i.e. normal
                        if (scanline - spriteScanline[i].y < 8) {
                            // Reading Top half Tile
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12)
                                                        | ((spriteScanline[i].id & 0xFE) << 4)
                                                        | ((scanline - spriteScanline[i].y) & 0x07);
                        } else {
                            // Reading Bottom Half Tile
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12)
                                                        | (((spriteScanline[i].id & 0xFE) + 1) << 4)
                                                        | ((scanline - spriteScanline[i].y) & 0x07);
                        }
                    } else {
                        // Sprite is flipped vertically, i.e. upside down
                        if (scanline - spriteScanline[i].y < 8) {
                            // Reading Top half Tile
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12)
                                                        | (((spriteScanline[i].id & 0xFE) + 1) << 4)
                                                        | (7 - (scanline - spriteScanline[i].y) & 0x07);
                        } else {
                            // Reading Bottom Half Tile
                            sprite_pattern_addr_lo = ((spriteScanline[i].id & 0x01) << 12)
                                                        | ((spriteScanline[i].id & 0xFE) << 4)
                                                        | (7 - (scanline - spriteScanline[i].y) & 0x07);
                        }
                    }
                }
                
                sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
                
                sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
                sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);
                
                if (spriteScanline[i].attribute & 0x40) {
                    // Sprite is flipped horizontally
                    auto ReverseBits = [](uint8_t n) {
                        n = (n & 0xF0) >> 4 | (n & 0x0F) << 4;
                        n = (n & 0xCC) >> 2 | (n & 0x33) << 2;
                        n = (n & 0xAA) >> 1 | (n & 0x55) << 1;
                        return n;
                    };
                    
                    sprite_pattern_bits_lo = ReverseBits(sprite_pattern_bits_lo);
                    sprite_pattern_bits_hi = ReverseBits(sprite_pattern_bits_hi);
                }
                
                sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
                sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
            }
        }

        //#endregion
        
        if (scanline == -1 && cycle >= 280 && cycle < 305) {
            ResetAddressY();
        }
    }
    
    if (scanline == 240) {
        // Post render scanline - Do nothing!
    }
    
    if (scanline == 241 && cycle == 1) {
        status.vertical_blank = 1;
        if (control.enable_nmi) {
            nmi = true;
        }
    }
    
    uint8_t bg_pixel = 0x00;
    uint8_t bg_palette = 0x00;

    if (mask.render_background) {
        uint16_t bit_mux = 0x8000 >> fine_x;
        
        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;
        bg_pixel = (p1_pixel << 1) | p0_pixel;
        
        uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
        uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
        bg_palette = (bg_pal1 << 1) | bg_pal0;
    }
    
    uint8_t fg_pixel = 0x00;
    uint8_t fg_palette = 0x00;
    uint8_t fg_priority = 0x00;
    
    if (mask.render_sprites) {
        bSpriteZeroBeingRendered = false;
        
        for (int i = 0; i < sprite_count; i++) {
            if (spriteScanline[i].x == 0) {
                uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
                uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
                fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;
                
                fg_palette = (spriteScanline[i].attribute & 0x03) + 0x04;
                fg_priority = (spriteScanline[i].attribute & 0x20) == 0;
                
                if (fg_pixel != 0) {
                    if (i == 0) {
                        bSpriteZeroBeingRendered = true;
                    }
                    
                    break;
                }
            }
        }
    }
    
    uint8_t pixel = 0x00; // Final Pixel
    uint8_t palette = 0x00; // Final Palette
    
    if (bg_pixel == 0 && fg_pixel == 0) {
        // The background pixel is transparent
        // The foreground pixel is transparent
        // No winner, draw "background" color
        pixel = 0x00;
        palette = 0x00;
    }
    else if (bg_pixel == 0 && fg_pixel > 0) {
        // The background pixel is transparent
        // The foreground pixel is visible
        // Foreground wins!
        pixel = fg_pixel;
        palette = fg_palette;
    }
    else if (bg_pixel > 0 && fg_pixel == 0) {
        // The background pixel is visible
        // The foreground pixel is transparent
        // Background wins!
        pixel = bg_pixel;
        palette = bg_palette;
    }
    else if (bg_pixel > 0 && fg_pixel > 0) {
        // The background pixel is visible
        // The foreground pixel is visible
        // Hmmm...
        if (fg_priority) {
            // Foreground cheats its way to victory!
            pixel = fg_pixel;
            palette = fg_palette;
        }
        else {
            // Background is considered more important!
            pixel = bg_pixel;
            palette = bg_palette;
        }
        
        if (bSpriteZeroHitPossible && bSpriteZeroBeingRendered) {
            if (mask.render_background && mask.render_sprites) {
                if (~(mask.render_background_left | mask.render_sprites_left)) {
                    if (cycle >= 9 && cycle < 258) {
                        status.sprite_zero_hit = 1;
                    }
                }
                else {
                    if (cycle >= 1 && cycle < 258) {
                        status.sprite_zero_hit = 1;
                    }
                }
            }
        }
    }
    
    DrawPixel(screenBuffer, cycle - 1, scanline, GetColorFromPaletteRam(palette, pixel));

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
