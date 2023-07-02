#pragma once

#include <cstdint>
#include <array>

#include "olc6502.h"

class Bus {
public:
    Bus();

    // Devices on the bus
    olc6502 cpu;
    
    // Fake RAM for now
    std::array<uint8_t, 64*1024> ram;
    
    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address, bool bReadOnly = false);
};
