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

/**
 * @brief Structure representing the registers of the CPU.
 */
typedef struct {
    Word AX; /**< Accumulator register */
    Word BX; /**< Base register */
    Word CX; /**< Counter register */
    Word DX; /**< Data register */
    Word SP; /**< Stack pointer register */
    Word IP; /**< Instruction pointer register */
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
