#pragma once
#include "sasm_memory.h"

typedef enum {
    HALT = 1 << 0,
    SIGN = 1 << 1,
    OVERFLOW = 1 << 2,
    CARRY = 1 << 3,
    BORROW = 1 << 4,
    PARITY = 1 << 5,
    ZERO = 1 << 6,
} Flags;

void setHalt(CPU* cpu, bool halt);
bool getHalt(const CPU* cpu);

void setSign(CPU* cpu, bool sign);
bool getSign(const CPU* cpu);

void setOverflow(CPU* cpu, bool overflow);
bool getOverflow(const CPU* cpu);

void setCarry(CPU* cpu, bool carry);
bool getCarry(const CPU* cpu);

void setBorrow(CPU* cpu, bool borrow);
bool getBorrow(const CPU* cpu);

void setParity(CPU* cpu, bool parity);
bool getParity(const CPU* cpu);

void setZero(CPU* cpu, bool zero);
bool getZero(const CPU* cpu);
