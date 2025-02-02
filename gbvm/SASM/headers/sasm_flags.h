/**
 * @file sasm_flags.h
 * @brief Declaration of the CPU flags manipulation functions.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include "sasm_memory.h"

typedef enum {
    HALT = 1 << 0,         // 1
    SIGN = 1 << 1,         // 2
    OVERFLOW = 1 << 2,     // 4
    CARRY = 1 << 3,        // 8
    BORROW = 1 << 4,       // 16
    PARITY = 1 << 5,       // 32
    ZERO = 1 << 6,         // 64
    AUX = 1 << 7           // 128
} Flags;

void setFlag(Flags f, CPU* cpu, bool state);

bool getFlag(Flags f, const CPU* cpu);

void checkAndSetParity(CPU* cpu, Word num);