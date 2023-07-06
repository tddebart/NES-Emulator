#include "Bus.h"

Bus::Bus() {
    // Clear RAM contents
    for (auto &i : cpuRam) {
        i = 0x00;
    }
    
    // Connect CPU to communication bus
    cpu.ConnectBus(this);
}

void Bus::cpuWrite(uint16_t address, uint8_t data) {
    if (cartridge->cpuWrite(address, data)) {
        
    }
    else if (address >= 0x0000 && address <= 0x1FFF) {
        cpuRam[address & 0x07FF] = data;
    }
    else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu.cpuWrite(address & 0x0007, data);
    }
    // Controller
    else if (address >= 0x4016 && address <= 0x4017) {
        controller_state[address & 0x0001] = controller[address & 0x0001];
    }
}

uint8_t Bus::cpuRead(uint16_t address, bool bReadOnly) {
    uint8_t data = 0x00;

    if (cartridge->cpuRead(address, data)) {

    }
    else if (address >= 0x0000 && address <= 0x1FFF) {
        data = cpuRam[address & 0x07FF];
    }
    else if (address >= 0x2000 && address <= 0x3FFF) {
        data = ppu.cpuRead(address & 0x0007, bReadOnly);
    }
    // Controller
    else if (address >= 0x4016 && address <= 0x4017) {
        data = (controller_state[address & 0x0001] & 0x80) > 0;
        controller_state[address & 0x0001] <<= 1;
    }
    
    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cartridge = cartridge;
    ppu.ConnectCartridge(cartridge);
}

void Bus::reset() {
    cpu.reset();
    nSystemClockCounter = 0;
}

void Bus::clock() {
    ppu.clock();
    
    if (nSystemClockCounter % 3 == 0) {
        cpu.clock();
    }
    
    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nmi();
    }
    
    nSystemClockCounter++;
}
