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
/**
 * @brief Structure representing the registers of the CPU.
 */

typedef QuadWord Register;

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

typedef struct {
    Register AX; /**< Accumulator register */
    Register BX; /**< Base register */
    Register CX; /**< Counter register */
    Register DX; /**< Data register */

    QuadWord CS; /**< Points at current code segment */
    QuadWord IP; /**< Instruction pointer register */

    QuadWord SS; /**< Points at current stack segment */
    QuadWord SP; /**< Stack pointer register */

    QuadWord SI; /**< Source index register */
    QuadWord DI; /**< Destination index register */
    QuadWord DS; /**< Points at current data segment */

    QuadWord ES; /**< Extra segment */
    QuadWord EP; /**< Extra pointer register */
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

void writeToMemory(Memory* memory, MemoryAddr address, DataEntry data);

DataEntry readFromMemory(Memory* memory, MemoryAddr address);

/**
 * @brief Evaluates the value of a register.
 *
 * This function evaluates the value of a register based on the given CPU and temporary string.
 *
 * @param cpu The CPU structure.
 * @param tmp The temporary string.
 * @return The value of the register.
 */
uint64_t evalRegister(CPU* cpu, String tmp);

/**
 * @brief Resolves the address based on the given  string.
 *
 * @param cpu The CPU structure.
 * @param s The string containing the address.
 * @return The resolved address.
 */
MemoryAddr resolveAddress(CPU* cpu, String* s);

MemoryAddr resolveImmediateAddress(Sasm* sasm, String* s);

void setFlag(Flags f, CPU* cpu, bool state);

bool getFlag(Flags f, const CPU* cpu);

void checkAndSetParity(CPU* cpu, uint64_t num);

bool evaluateAddressingMode(Memory* mem, CPU* cpu, AddrMode mode, QuadWord* val, QuadWord* out);
