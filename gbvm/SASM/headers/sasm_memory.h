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

#include "sasm_assembler.h"
#include "univ_defs.h"
#include "univ_strings.h"

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

typedef enum {
    H0,
    H1,
    I0,
    I1,
    JS,
    KC,
    L0,
    L1,
    L2,
    L3,
    NX,
    OP,
    P0,
    P1,
    P2,
    P3,
    QT,
    RF,
    SP,
    REG_COUNT
} RegID;

typedef union {
    struct
    {
        Register H0;
        Register H1;

        Register I0;
        Register I1;

        Register JS;
        Register KC;

        Register L0;
        Register L1;
        Register L2;
        Register L3;

        Register NX;
        Register OP;

        Register P0;
        Register P1;
        Register P2;
        Register P3;

        Register QT;
        Register RF;
        Register SP;
    };
    Register reg[REG_COUNT];

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