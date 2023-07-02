#pragma once

#include <cstdint>

class Bus;

class olc6502 {
public:
    void ConnectBus(class Bus* n) { bus = n; }
    
private:
    Bus *bus = nullptr;
    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address, bool bReadOnly = false);
};
