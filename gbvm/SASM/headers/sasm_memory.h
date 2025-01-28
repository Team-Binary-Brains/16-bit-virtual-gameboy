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

typedef struct {
    Register AX; /**< Accumulator register */
    Register BX; /**< Base register */
    Register CX; /**< Counter register */
    Register DX; /**< Data register */

    Word CS; /**< Points at current code segment */
    Word IP; /**< Instruction pointer register */

    Word SS; /**< Points at current stack segment */
    Word SP; /**< Stack pointer register */
    Word BP; /**< Base pointer register */

    Word SI; /**< Source index register */
    Word DI; /**< Destination index register */
    Word DS; /**< Points at current data segment */

    Word ES; /**< Extra segment */
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

void writeToMemory(Memory* memory, Word address, Byte data);

Byte readFromMemory(Memory* memory, Word address);
