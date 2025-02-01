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

/**
 * @brief Structure representing the registers of the CPU.
 */

typedef union {
    Word full;
    Byte data[2];
} Register;
typedef enum {
    o1_mem = 1 << 0,
    o1_reg = 1 << 1,
    o2_mem = 1 << 2,
    o2_reg = 1 << 3,
    o2_imm = 1 << 4,
} modes;
typedef struct {
    Register AX; /**< Accumulator register */
    Register BX; /**< Base register */
    Register CX; /**< Counter register */
    Register DX; /**< Data register */

    Word CS; /**< Points at current code segment */
    Word IP; /**< Instruction pointer register */

    Word SS; /**< Points at current stack segment */
    Word SP; /**< Stack pointer register */

    Word SI; /**< Source index register */
    Word DI; /**< Destination index register */
    Word DS; /**< Points at current data segment */

    Word ES; /**< Extra segment */
    Word EP; /**< Extra pointer register */
} Registers;

/**
 * @brief Structure representing the CPU.
 */
typedef struct {
    Registers registers; /**< CPU registers */
    Word flags;          /**< CPU flags */
} CPU;

/**
 * @brief Structure representing the memory of the system.
 */
typedef struct {
    Word stack[STACK_CAPACITY]; /**< Stack memory */
} Memory;

void writeToMemory(Memory* memory, Word address, Word data);

Byte readFromMemory(Memory* memory, Word address);
