﻿#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>

// Prevent circular include
class Bus;

class olc6502 {
public:
    olc6502();
    
    // enum for status register
    enum FLAGS6502
    {
        C = (1 << 0),	// Carry Bit
        Z = (1 << 1),	// Zero
        I = (1 << 2),	// Disable Interrupts
        D = (1 << 3),	// Decimal Mode (unused in this implementation)
        B = (1 << 4),	// Break
        U = (1 << 5),	// Unused
        V = (1 << 6),	// Overflow
        N = (1 << 7),	// Negative
    };
    
    uint8_t a = 0x00;		// Accumulator Register
    uint8_t x = 0x00;		// X Register
    uint8_t y = 0x00;		// Y Register
    uint8_t stkp = 0x00;	// Stack Pointer (points to location on bus)
    uint16_t pc = 0x0000;	// Program Counter
    uint8_t status = 0x00;	// Status Register
    
    void ConnectBus(class Bus* n) { bus = n; }

    // External event functions. In hardware these represent pins that are asserted
    // to produce a change in state.
    void reset();	// Reset Interrupt - Forces CPU into known state
    void irq();		// Interrupt Request - Executes an instruction at a specific location
    void nmi();		// Non-Maskable Interrupt Request - As above, but cannot be disabled
    void clock();	// Perform one clock cycle's worth of update
    
    // Indicates the current instruction has completed by returning true. This is
	// a utility function to enable "step-by-step" execution, without manually 
	// clocking every cycle
	bool complete();

    // Produces a map of strings, with keys equivalent to instruction start locations
    // in memory, for the specified address range
    std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);
    
private:
    // Addressing Modes
    uint8_t IMP();	uint8_t IMM();
    uint8_t ZP0();	uint8_t ZPX();
    uint8_t ZPY();	uint8_t REL();
    uint8_t ABS();	uint8_t ABX();
    uint8_t ABY();	uint8_t IND();
    uint8_t IZX();	uint8_t IZY();

    // Opcodes (56)
    // These functions return 0 normally, but some are capable of
    // requiring more clock cycles when executed under certain
    // conditions combined with certain addressing modes. If that
    // happens, they return 1.
    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
    uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
    uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
    uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
    uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
    uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
    uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
    uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
    uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
    uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
    uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
    uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
    uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
    uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

    // Illegal Opcode
    uint8_t XXX();


    // The read location of data can come from two sources, a memory address, or
    // its immediately available as part of the instruction. This function decides
    // depending on address mode of instruction byte
    uint8_t fetch();

    // Assisstive variables to facilitate emulation
    uint8_t  fetched     = 0x00;   // Represents the working input value to the ALU
    uint16_t temp        = 0x0000; // A convenience variable used everywhere
    uint16_t addr_abs    = 0x0000; // All used memory addresses end up in here
    uint16_t addr_rel    = 0x00;   // Represents absolute address following a branch
    uint8_t  opcode      = 0x00;   // Is the instruction byte
    uint8_t  cycles      = 0;	   // Counts how many cycles the instruction has remaining
    uint32_t clock_count = 0;	   // A global accumulation of the number of clocks

    struct INSTRUCTION
    {
        std::string name;
        uint8_t     (olc6502::*operate )(void) = nullptr;
        uint8_t     (olc6502::*addrmode)(void) = nullptr;
        uint8_t     cycles = 0;
    };

    std::vector<INSTRUCTION> lookup;
    
    
    Bus *bus = nullptr;
    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address, bool bReadOnly = false);
    
    uint8_t GetFlag(FLAGS6502 f);
    void    SetFlag(FLAGS6502 f, bool v);
};
