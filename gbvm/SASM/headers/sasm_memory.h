/**
 * @file sasm_memory.h
 * @brief Declaration of the memory structures for the GBVM virtual machine.
 *
 * This file contains the declaration of the memory structures used by the GBVM (GameBoy Virtual Machine).
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include "univ_defs.h"
#include "univ_strings.h"
#include "sasm_assembler.h"

#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

typedef QuadWord Register;

typedef enum {
    HALT = 1 << 0,     // 1
    F1 = 1 << 1,       // 2
    F2 = 1 << 2,       // 4
    F3 = 1 << 3,       // 8
    F4 = 1 << 4,       // 16
    F5 = 1 << 5,       // 32
    F6 = 1 << 6,       // 64
    F7 = 1 << 7        // 128
} Flags;

typedef struct {
    Register AX;
    Register BX;
    Register CX;
    Register DX;

    QuadWord IP;

    QuadWord SP;

} Registers;

/**
 * @brief Structure representing the CPU.
 */
typedef struct {
    Registers registers;  /**< CPU registers */
    volatile short flags; /**< CPU flags */
} CPU;

/**
 * @brief Structure representing the memory of the system.
 */
typedef struct {
    QuadWord stack[STACK_CAPACITY]; /**< Stack memory */
    Byte memory[MEMORY_CAPACITY];
} Memory;

void setFlag(Flags f, CPU* cpu, bool state);

bool getFlag(Flags f, const CPU* cpu);

bool evaluateAddressingMode(Memory* mem, CPU* cpu, AddrMode mode, QuadWord* val, QuadWord* out);
