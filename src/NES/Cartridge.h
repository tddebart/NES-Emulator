#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "NES/Mappers/Mapper_000.h"

class Cartridge {
public:
    Cartridge(const std::string& sFileName);
    
    enum MIRROR {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI,
    } mirror = HORIZONTAL;
    
private:
    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
    
    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;
    
    std::shared_ptr<Mapper> pMapper;

public:
    // Communication with Main Bus
    bool cpuRead(uint16_t addr, uint8_t &data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    // Communication with PPU Bus
    bool ppuRead(uint16_t addr, uint8_t &data);
    bool ppuWrite(uint16_t addr, uint8_t data);
};
