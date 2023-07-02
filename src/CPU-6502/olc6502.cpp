#include "olc6502.h"
#include "Bus.h"

uint8_t olc6502::read(uint16_t address, bool bReadOnly) {
    return bus->read(address, bReadOnly);
}

void olc6502::write(uint16_t address, uint8_t data) {
    bus->write(address, data);
}


