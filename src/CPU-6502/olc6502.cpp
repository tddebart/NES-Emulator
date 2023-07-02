#include "olc6502.h"
#include "Bus.h"

olc6502::olc6502() {
    using c = olc6502;
    lookup =
    {
        {"BRK", &c::BRK, &c::IMM, 7 }, {"ORA", &c::ORA, &c::IZX, 6 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 3 }, {"ORA", &c::ORA, &c::ZP0, 3 }, {"ASL", &c::ASL, &c::ZP0, 5 }, {"???", &c::XXX, &c::IMP, 5 }, {"PHP", &c::PHP, &c::IMP, 3 }, {"ORA", &c::ORA, &c::IMM, 2 }, {"ASL", &c::ASL, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::NOP, &c::IMP, 4 }, {"ORA", &c::ORA, &c::ABS, 4 }, {"ASL", &c::ASL, &c::ABS, 6 }, {"???", &c::XXX, &c::IMP, 6 },
        {"BPL", &c::BPL, &c::REL, 2 }, {"ORA", &c::ORA, &c::IZY, 5 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 4 }, {"ORA", &c::ORA, &c::ZPX, 4 }, {"ASL", &c::ASL, &c::ZPX, 6 }, {"???", &c::XXX, &c::IMP, 6 }, {"CLC", &c::CLC, &c::IMP, 2 }, {"ORA", &c::ORA, &c::ABY, 4 }, {"???", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 7 }, {"???", &c::NOP, &c::IMP, 4 }, {"ORA", &c::ORA, &c::ABX, 4 }, {"ASL", &c::ASL, &c::ABX, 7 }, {"???", &c::XXX, &c::IMP, 7 },
        {"JSR", &c::JSR, &c::ABS, 6 }, {"AND", &c::AND, &c::IZX, 6 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"BIT", &c::BIT, &c::ZP0, 3 }, {"AND", &c::AND, &c::ZP0, 3 }, {"ROL", &c::ROL, &c::ZP0, 5 }, {"???", &c::XXX, &c::IMP, 5 }, {"PLP", &c::PLP, &c::IMP, 4 }, {"AND", &c::AND, &c::IMM, 2 }, {"ROL", &c::ROL, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 2 }, {"BIT", &c::BIT, &c::ABS, 4 }, {"AND", &c::AND, &c::ABS, 4 }, {"ROL", &c::ROL, &c::ABS, 6 }, {"???", &c::XXX, &c::IMP, 6 },
        {"BMI", &c::BMI, &c::REL, 2 }, {"AND", &c::AND, &c::IZY, 5 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 4 }, {"AND", &c::AND, &c::ZPX, 4 }, {"ROL", &c::ROL, &c::ZPX, 6 }, {"???", &c::XXX, &c::IMP, 6 }, {"SEC", &c::SEC, &c::IMP, 2 }, {"AND", &c::AND, &c::ABY, 4 }, {"???", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 7 }, {"???", &c::NOP, &c::IMP, 4 }, {"AND", &c::AND, &c::ABX, 4 }, {"ROL", &c::ROL, &c::ABX, 7 }, {"???", &c::XXX, &c::IMP, 7 },
        {"RTI", &c::RTI, &c::IMP, 6 }, {"EOR", &c::EOR, &c::IZX, 6 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 3 }, {"EOR", &c::EOR, &c::ZP0, 3 }, {"LSR", &c::LSR, &c::ZP0, 5 }, {"???", &c::XXX, &c::IMP, 5 }, {"PHA", &c::PHA, &c::IMP, 3 }, {"EOR", &c::EOR, &c::IMM, 2 }, {"LSR", &c::LSR, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 2 }, {"JMP", &c::JMP, &c::ABS, 3 }, {"EOR", &c::EOR, &c::ABS, 4 }, {"LSR", &c::LSR, &c::ABS, 6 }, {"???", &c::XXX, &c::IMP, 6 },
        {"BVC", &c::BVC, &c::REL, 2 }, {"EOR", &c::EOR, &c::IZY, 5 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 4 }, {"EOR", &c::EOR, &c::ZPX, 4 }, {"LSR", &c::LSR, &c::ZPX, 6 }, {"???", &c::XXX, &c::IMP, 6 }, {"CLI", &c::CLI, &c::IMP, 2 }, {"EOR", &c::EOR, &c::ABY, 4 }, {"???", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 7 }, {"???", &c::NOP, &c::IMP, 4 }, {"EOR", &c::EOR, &c::ABX, 4 }, {"LSR", &c::LSR, &c::ABX, 7 }, {"???", &c::XXX, &c::IMP, 7 },
        {"RTS", &c::RTS, &c::IMP, 6 }, {"ADC", &c::ADC, &c::IZX, 6 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 3 }, {"ADC", &c::ADC, &c::ZP0, 3 }, {"ROR", &c::ROR, &c::ZP0, 5 }, {"???", &c::XXX, &c::IMP, 5 }, {"PLA", &c::PLA, &c::IMP, 4 }, {"ADC", &c::ADC, &c::IMM, 2 }, {"ROR", &c::ROR, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 2 }, {"JMP", &c::JMP, &c::IND, 5 }, {"ADC", &c::ADC, &c::ABS, 4 }, {"ROR", &c::ROR, &c::ABS, 6 }, {"???", &c::XXX, &c::IMP, 6 },
        {"BVS", &c::BVS, &c::REL, 2 }, {"ADC", &c::ADC, &c::IZY, 5 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 4 }, {"ADC", &c::ADC, &c::ZPX, 4 }, {"ROR", &c::ROR, &c::ZPX, 6 }, {"???", &c::XXX, &c::IMP, 6 }, {"SEI", &c::SEI, &c::IMP, 2 }, {"ADC", &c::ADC, &c::ABY, 4 }, {"???", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 7 }, {"???", &c::NOP, &c::IMP, 4 }, {"ADC", &c::ADC, &c::ABX, 4 }, {"ROR", &c::ROR, &c::ABX, 7 }, {"???", &c::XXX, &c::IMP, 7 },
        {"???", &c::NOP, &c::IMP, 2 }, {"STA", &c::STA, &c::IZX, 6 }, {"???", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 6 }, {"STY", &c::STY, &c::ZP0, 3 }, {"STA", &c::STA, &c::ZP0, 3 }, {"STX", &c::STX, &c::ZP0, 3 }, {"???", &c::XXX, &c::IMP, 3 }, {"DEY", &c::DEY, &c::IMP, 2 }, {"???", &c::NOP, &c::IMP, 2 }, {"TXA", &c::TXA, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 2 }, {"STY", &c::STY, &c::ABS, 4 }, {"STA", &c::STA, &c::ABS, 4 }, {"STX", &c::STX, &c::ABS, 4 }, {"???", &c::XXX, &c::IMP, 4 },
        {"BCC", &c::BCC, &c::REL, 2 }, {"STA", &c::STA, &c::IZY, 6 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 6 }, {"STY", &c::STY, &c::ZPX, 4 }, {"STA", &c::STA, &c::ZPX, 4 }, {"STX", &c::STX, &c::ZPY, 4 }, {"???", &c::XXX, &c::IMP, 4 }, {"TYA", &c::TYA, &c::IMP, 2 }, {"STA", &c::STA, &c::ABY, 5 }, {"TXS", &c::TXS, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 5 }, {"???", &c::NOP, &c::IMP, 5 }, {"STA", &c::STA, &c::ABX, 5 }, {"???", &c::XXX, &c::IMP, 5 }, {"???", &c::XXX, &c::IMP, 5 },
        {"LDY", &c::LDY, &c::IMM, 2 }, {"LDA", &c::LDA, &c::IZX, 6 }, {"LDX", &c::LDX, &c::IMM, 2 }, {"???", &c::XXX, &c::IMP, 6 }, {"LDY", &c::LDY, &c::ZP0, 3 }, {"LDA", &c::LDA, &c::ZP0, 3 }, {"LDX", &c::LDX, &c::ZP0, 3 }, {"???", &c::XXX, &c::IMP, 3 }, {"TAY", &c::TAY, &c::IMP, 2 }, {"LDA", &c::LDA, &c::IMM, 2 }, {"TAX", &c::TAX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 2 }, {"LDY", &c::LDY, &c::ABS, 4 }, {"LDA", &c::LDA, &c::ABS, 4 }, {"LDX", &c::LDX, &c::ABS, 4 }, {"???", &c::XXX, &c::IMP, 4 },
        {"BCS", &c::BCS, &c::REL, 2 }, {"LDA", &c::LDA, &c::IZY, 5 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 5 }, {"LDY", &c::LDY, &c::ZPX, 4 }, {"LDA", &c::LDA, &c::ZPX, 4 }, {"LDX", &c::LDX, &c::ZPY, 4 }, {"???", &c::XXX, &c::IMP, 4 }, {"CLV", &c::CLV, &c::IMP, 2 }, {"LDA", &c::LDA, &c::ABY, 4 }, {"TSX", &c::TSX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 4 }, {"LDY", &c::LDY, &c::ABX, 4 }, {"LDA", &c::LDA, &c::ABX, 4 }, {"LDX", &c::LDX, &c::ABY, 4 }, {"???", &c::XXX, &c::IMP, 4 },
        {"CPY", &c::CPY, &c::IMM, 2 }, {"CMP", &c::CMP, &c::IZX, 6 }, {"???", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"CPY", &c::CPY, &c::ZP0, 3 }, {"CMP", &c::CMP, &c::ZP0, 3 }, {"DEC", &c::DEC, &c::ZP0, 5 }, {"???", &c::XXX, &c::IMP, 5 }, {"INY", &c::INY, &c::IMP, 2 }, {"CMP", &c::CMP, &c::IMM, 2 }, {"DEX", &c::DEX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 2 }, {"CPY", &c::CPY, &c::ABS, 4 }, {"CMP", &c::CMP, &c::ABS, 4 }, {"DEC", &c::DEC, &c::ABS, 6 }, {"???", &c::XXX, &c::IMP, 6 },
        {"BNE", &c::BNE, &c::REL, 2 }, {"CMP", &c::CMP, &c::IZY, 5 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 4 }, {"CMP", &c::CMP, &c::ZPX, 4 }, {"DEC", &c::DEC, &c::ZPX, 6 }, {"???", &c::XXX, &c::IMP, 6 }, {"CLD", &c::CLD, &c::IMP, 2 }, {"CMP", &c::CMP, &c::ABY, 4 }, {"NOP", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 7 }, {"???", &c::NOP, &c::IMP, 4 }, {"CMP", &c::CMP, &c::ABX, 4 }, {"DEC", &c::DEC, &c::ABX, 7 }, {"???", &c::XXX, &c::IMP, 7 },
        {"CPX", &c::CPX, &c::IMM, 2 }, {"SBC", &c::SBC, &c::IZX, 6 }, {"???", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"CPX", &c::CPX, &c::ZP0, 3 }, {"SBC", &c::SBC, &c::ZP0, 3 }, {"INC", &c::INC, &c::ZP0, 5 }, {"???", &c::XXX, &c::IMP, 5 }, {"INX", &c::INX, &c::IMP, 2 }, {"SBC", &c::SBC, &c::IMM, 2 }, {"NOP", &c::NOP, &c::IMP, 2 }, {"???", &c::SBC, &c::IMP, 2 }, {"CPX", &c::CPX, &c::ABS, 4 }, {"SBC", &c::SBC, &c::ABS, 4 }, {"INC", &c::INC, &c::ABS, 6 }, {"???", &c::XXX, &c::IMP, 6 },
        {"BEQ", &c::BEQ, &c::REL, 2 }, {"SBC", &c::SBC, &c::IZY, 5 }, {"???", &c::XXX, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 8 }, {"???", &c::NOP, &c::IMP, 4 }, {"SBC", &c::SBC, &c::ZPX, 4 }, {"INC", &c::INC, &c::ZPX, 6 }, {"???", &c::XXX, &c::IMP, 6 }, {"SED", &c::SED, &c::IMP, 2 }, {"SBC", &c::SBC, &c::ABY, 4 }, {"NOP", &c::NOP, &c::IMP, 2 }, {"???", &c::XXX, &c::IMP, 7 }, {"???", &c::NOP, &c::IMP, 4 }, {"SBC", &c::SBC, &c::ABX, 4 }, {"INC", &c::INC, &c::ABX, 7 }, {"???", &c::XXX, &c::IMP, 7 },
    };
}

uint8_t olc6502::read(uint16_t address, bool bReadOnly) {
    return bus->read(address, bReadOnly);
}

void olc6502::write(uint16_t address, uint8_t data) {
    bus->write(address, data);
}

//#region EXTERNAL INPUTS

// Forces the 6502 into a known state. This is hard-wired inside the CPU. The
// registers are set to 0x00, the status register is cleared except for unused
// bit which remains at 1. An absolute address is read from location 0xFFFC
// which contains a second address that the program counter is set to. This 
// allows the programmer to jump to a known and programmable location in the
// memory to start executing from. Typically the programmer would set the value
// at location 0xFFFC at compile time.
void olc6502::reset()
{
    // Get address to set program counter to
    addr_abs = 0xFFFC;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);

    // Set it
    pc = (hi << 8) | lo;

    // Reset internal registers
    a = 0;
    x = 0;
    y = 0;
    stkp = 0xFD;
    status = 0x00 | U;

    // Clear internal helper variables
    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;

    // Reset takes time
    cycles = 8;
}


// Interrupt requests are a complex operation and only happen if the
// "disable interrupt" flag is 0. IRQs can happen at any time, but
// you dont want them to be destructive to the operation of the running 
// program. Therefore the current instruction is allowed to finish
// (which I facilitate by doing the whole thing when cycles == 0) and 
// then the current program counter is stored on the stack. Then the
// current status register is stored on the stack. When the routine
// that services the interrupt has finished, the status register
// and program counter can be restored to how they where before it 
// occurred. This is impemented by the "RTI" instruction. Once the IRQ
// has happened, in a similar way to a reset, a programmable address
// is read form hard coded location 0xFFFE, which is subsequently
// set to the program counter.
void olc6502::irq()
{
    // If interrupts are allowed
    if (GetFlag(I) == 0)
    {
        // Push the program counter to the stack. It's 16-bits dont
        // forget so that takes two pushes
        write(0x0100 + stkp, (pc >> 8) & 0x00FF);
        stkp--;
        write(0x0100 + stkp, pc & 0x00FF);
        stkp--;

        // Then Push the status register to the stack
        SetFlag(B, 0);
        SetFlag(U, 1);
        SetFlag(I, 1);
        write(0x0100 + stkp, status);
        stkp--;

        // Read new program counter location from fixed address
        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi << 8) | lo;

        // IRQs take time
        cycles = 7;
    }
}


// A Non-Maskable Interrupt cannot be ignored. It behaves in exactly the
// same way as a regular IRQ, but reads the new program counter address
// form location 0xFFFA.
void olc6502::nmi()
{
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(0x0100 + stkp, status);
    stkp--;

    addr_abs = 0xFFFA;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi << 8) | lo;

    cycles = 8;
}
//#endregion

void olc6502::clock() {
    if (cycles == 0) {
        // Read next instruction byte. This 8-bit value is used to index
        // the translation table to get the relevant information about
        // how to implement the instruction
        opcode = read(pc);

        // Increment program counter, we read the opcode byte
        pc++;

        cycles = lookup[opcode].cycles;

        // Perform fetch of intermmediate data using the
        // required addressing mode
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
        
        // Perform operation
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

        cycles += (additional_cycle1 & additional_cycle2);
    }

    cycles--;
}

uint8_t olc6502::GetFlag(FLAGS6502 f)
{
    return ((status & f) > 0) ? 1 : 0;
}

// Sets or clears a specific bit of the status register
void olc6502::SetFlag(FLAGS6502 f, bool v)
{
    if (v)
        status |= f;
    else
        status &= ~f;
}


//#region Addressing Modes

// There is no additional data required for this instruction.
uint8_t olc6502::IMP() {
    fetched = a;
    return 0;
}

// The instruction expects the next byte to be used as a value
uint8_t olc6502::IMM() {
    addr_abs = pc++;
    return 0;
}


uint8_t olc6502::ZP0() {
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

// Fundamentally the same as Zero Page addressing, but the contents of the X Register
// is added to the supplied single byte address. This is useful for iterating through
// ranges within the first page.
uint8_t olc6502::ZPX()
{
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}


// Same as above but uses Y Register for offset
uint8_t olc6502::ZPY()
{
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

// A full 16-bit address is loaded and used
uint8_t olc6502::ABS()
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;

    return 0;
}

// Address Mode: Absolute with X Offset
// Fundamentally the same as absolute addressing, but the contents of the X Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t olc6502::ABX()
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += x;

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}


// Address Mode: Absolute with Y Offset
// Fundamentally the same as absolute addressing, but the contents of the Y Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t olc6502::ABY()
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

// Address Mode: Indirect (AKA Pointers)
uint8_t olc6502::IND()
{
    uint16_t ptr_lo = read(pc);
    pc++;
    uint16_t ptr_hi = read(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
    {
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else // Behave normally
    {
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

// Address Mode: Indirect X
// The supplied 8-bit address is offset by X Register to index
// a location in page 0x00. The actual 16-bit address is read 
// from this location
uint8_t olc6502::IZX()
{
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;

    return 0;
}


// Address Mode: Indirect Y
// The supplied 8-bit address indexes a location in page 0x00. From 
// here the actual 16-bit address is read, and the contents of
// Y Register is added to it to offset it. If the offset causes a
// change in page then an additional clock cycle is required.
uint8_t olc6502::IZY()
{
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

// Address Mode: Relative
// This address mode is exclusive to branch instructions. The address
// must reside within -128 to +127 of the branch instruction, i.e.
// you cant directly branch to any address in the addressable range.
uint8_t olc6502::REL()
{
    addr_rel = read(pc);
    pc++;
    if (addr_rel & 0x80)
        addr_rel |= 0xFF00;
    return 0;
}

//#endregion


//#region Instructions

// This function sources the data used by the instruction into 
// a convenient numeric variable. Some instructions dont have to 
// fetch data as the source is implied by the instruction. For example
// "INX" increments the X register. There is no additional data
// required. For all other addressing modes, the data resides at 
// the location held within addr_abs, so it is read from there. 
// Immediate adress mode exploits this slightly, as that has
// set addr_abs = pc + 1, so it fetches the data from the
// next byte for example "LDA $FF" just loads the accumulator with
// 256, i.e. no far reaching memory fetch is required. "fetched"
// is a variable global to the CPU, and is set by calling this 
// function. It also returns it for convenience.
uint8_t olc6502::fetch()
{
    if (!(lookup[opcode].addrmode == &olc6502::IMP))
        fetched = read(addr_abs);
    return fetched;
}

// Instruction: Add with Carry In
// Function:    A = A + M + C
// Flags Out:   C, V, N, Z
uint8_t olc6502::ADC()
{
    // Grab the data that we are adding to the accumulator
    fetch();

    // Add is performed in 16-bit domain for emulation to capture any
    // carry bit, which will exist in bit 8 of the 16-bit word
    temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

    // The carry flag out exists in the high byte bit 0
    SetFlag(C, temp > 255);

    // The Zero flag is set if the result is 0
    SetFlag(Z, (temp & 0x00FF) == 0);

    // The signed Overflow flag is set based on all that up there! :D
    SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

    // The negative flag is set to the most significant bit of the result
    SetFlag(N, temp & 0x80);

    // Load the result into the accumulator (it's 8-bit dont forget!)
    a = temp & 0x00FF;

    // This instruction has the potential to require an additional clock cycle
    return 1;
}

// Instruction: Subtract with Carry In
// Function:    A = A - M - (1 - C)
// Flags Out:   C, V, N, Z
uint8_t olc6502::SBC()
{
    fetch();

    // Operating in 16-bit domain to capture carry out

    // We can invert the bottom 8 bits with bitwise xor
    uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

    // Notice this is exactly the same as addition from here!
    temp = (uint16_t)a + value + (uint16_t)GetFlag(C);
    SetFlag(C, temp & 0xFF00);
    SetFlag(Z, ((temp & 0x00FF) == 0));
    SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
    SetFlag(N, temp & 0x0080);
    a = temp & 0x00FF;
    return 1;
}


uint8_t olc6502::AND() {
    fetch();
    a = a & fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t olc6502::ASL()
{
    fetch();
    temp = (uint16_t)fetched << 1;
    SetFlag(C, (temp & 0xFF00) > 0);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x80);
    if (lookup[opcode].addrmode == &olc6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

//#region branch instructions

// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address 
uint8_t olc6502::BCC()
{
    if (GetFlag(C) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}


// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t olc6502::BCS()
{
    if (GetFlag(C) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}


// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t olc6502::BEQ()
{
    if (GetFlag(Z) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

uint8_t olc6502::BIT()
{
    fetch();
    temp = a & fetched;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, fetched & (1 << 7));
    SetFlag(V, fetched & (1 << 6));
    return 0;
}


// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t olc6502::BMI()
{
    if (GetFlag(N) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}


// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t olc6502::BNE()
{
    if (GetFlag(Z) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}


// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t olc6502::BPL()
{
    if (GetFlag(N) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t olc6502::BVC()
{
    if (GetFlag(V) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}


// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t olc6502::BVS()
{
    if (GetFlag(V) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;

        pc = addr_abs;
    }
    return 0;
}

//#endregion

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t olc6502::BRK()
{
    pc++;

    SetFlag(I, 1);
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    SetFlag(B, 1);
    write(0x0100 + stkp, status);
    stkp--;
    SetFlag(B, 0);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

//#region clear flag instructions

// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t olc6502::CLC()
{
    SetFlag(C, false);
    return 0;
}


// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t olc6502::CLD()
{
    SetFlag(D, false);
    return 0;
}


// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t olc6502::CLI()
{
    SetFlag(I, false);
    return 0;
}


// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t olc6502::CLV()
{
    SetFlag(V, false);
    return 0;
}

//#endregion

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t olc6502::CMP()
{
    fetch();
    temp = (uint16_t)a - (uint16_t)fetched;
    SetFlag(C, a >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 1;
}


// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t olc6502::CPX()
{
    fetch();
    temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, x >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t olc6502::CPY()
{
    fetch();
    temp = (uint16_t)y - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}

// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t olc6502::DEC()
{
    fetch();
    temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}


// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t olc6502::DEX()
{
    x--;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}


// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t olc6502::DEY()
{
    y--;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t olc6502::EOR()
{
    fetch();
    a = a ^ fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t olc6502::INC()
{
    fetch();
    temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t olc6502::INX()
{
    x++;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}


// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t olc6502::INY()
{
    y++;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}

// Instruction: Jump To Location
// Function:    pc = address
uint8_t olc6502::JMP()
{
    pc = addr_abs;
    return 0;
}

// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t olc6502::JSR()
{
    pc--;

    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    pc = addr_abs;
    return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t olc6502::LDA()
{
    fetch();
    a = fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}

// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t olc6502::LDX()
{
    fetch();
    x = fetched;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t olc6502::LDY()
{
    fetch();
    y = fetched;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 1;
}

uint8_t olc6502::LSR()
{
    fetch();
    SetFlag(C, fetched & 0x0001);
    temp = fetched >> 1;
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &olc6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

uint8_t olc6502::NOP()
{
    // Sadly not all NOPs are equal, Ive added a few here
    // based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
    // and will add more based on game compatibility, and ultimately
    // I'd like to cover all illegal opcodes too
    switch (opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
    }
    return 0;
}

// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t olc6502::ORA()
{
    fetch();
    a = a | fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
}


// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t olc6502::PHA()
{
    write(0x0100 + stkp, a);
    stkp--;
    return 0;
}


// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t olc6502::PHP()
{
    write(0x0100 + stkp, status | B | U);
    SetFlag(B, 0);
    SetFlag(U, 0);
    stkp--;
    return 0;
}


// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t olc6502::PLA()
{
    stkp++;
    a = read(0x0100 + stkp);
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t olc6502::PLP()
{
    stkp++;
    status = read(0x0100 + stkp);
    SetFlag(U, 1);
    return 0;
}

// Instruction: Rotate Left
uint8_t olc6502::ROL()
{
    fetch();
    temp = (uint16_t)(fetched << 1) | GetFlag(C);
    SetFlag(C, temp & 0xFF00);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &olc6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

// Instruction: Rotate Right
uint8_t olc6502::ROR()
{
    fetch();
    temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
    SetFlag(C, fetched & 0x01);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &olc6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

// Instruction: Return from Interrupt
uint8_t olc6502::RTI()
{
    stkp++;
    status = read(0x0100 + stkp);
    status &= ~B;
    status &= ~U;

    stkp++;
    pc = (uint16_t)read(0x0100 + stkp);
    stkp++;
    pc |= (uint16_t)read(0x0100 + stkp) << 8;
    return 0;
}

// Instruction: Return from Subroutine
uint8_t olc6502::RTS()
{
    stkp++;
    pc = (uint16_t)read(0x0100 + stkp);
    stkp++;
    pc |= (uint16_t)read(0x0100 + stkp) << 8;

    pc++;
    return 0;
}

// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t olc6502::SEC()
{
    SetFlag(C, true);
    return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t olc6502::SED()
{
    SetFlag(D, true);
    return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t olc6502::SEI()
{
    SetFlag(I, true);
    return 0;
}

// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t olc6502::STA()
{
    write(addr_abs, a);
    return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t olc6502::STX()
{
    write(addr_abs, x);
    return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t olc6502::STY()
{
    write(addr_abs, y);
    return 0;
}


// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t olc6502::TAX()
{
    x = a;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t olc6502::TAY()
{
    y = a;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t olc6502::TSX()
{
    x = stkp;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t olc6502::TXA()
{
    a = x;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}


// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t olc6502::TXS()
{
    stkp = x;
    return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t olc6502::TYA()
{
    a = y;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
}


// This function captures illegal opcodes
uint8_t olc6502::XXX()
{
    return 0;
}

//#endregion

//#region HELPER FUNCTIONS

bool olc6502::complete()
{
	return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> olc6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->read(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup[opcode].addrmode == &olc6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IMM)
		{
			value = bus->read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ZP0)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;												
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ZPX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ZPY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IZX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IZY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ABS)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ABX)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &olc6502::ABY)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &olc6502::IND)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &olc6502::REL)
		{
			value = bus->read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}