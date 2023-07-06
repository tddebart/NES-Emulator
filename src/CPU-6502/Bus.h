#pragma once

#include <cstdint>
#include <array>
#include <memory>

#include "olc6502.h"
#include "NES/PPU.h"
#include "NES/Cartridge.h"

class Bus {
public:
    Bus();

    //#region Devices on the bus

    // The 6502 derived processor
    olc6502 cpu;

    // The 2C02 Picture Processing Unit
    PPU ppu;

    // The Cartridge or "GamePak"
    std::shared_ptr<Cartridge> cartridge;
    
    uint8_t controller[2];
    
    std::array<uint8_t, 2048> cpuRam;
    
    //#endregion
    
    void cpuWrite(uint16_t address, uint8_t data);
    uint8_t cpuRead(uint16_t address, bool bReadOnly = false);

    // Connects a cartridge object to the internal buses
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    // Resets the system
    void reset();
    // Clocks the system - a single whole system tick
    void clock();
    
private:
    uint32_t nSystemClockCounter = 0; 
    
    uint8_t controller_state[2];
};
