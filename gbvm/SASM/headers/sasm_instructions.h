/**
 * @file sasm_instructions.h
 * @brief This file contains the declarations of functions and data structures related to the execution of instructions in the SASM assembly language.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include "univ_defs.h"
#include "univ_errors.h"
#include "sasm_memory.h"
#include "univ_strings.h"

/**
 * @brief Enumeration of opcodes for SASM instructions.
 */
typedef enum {
    NOP = 0, /**< No operation */
    HLT,     /**< Halt */
    ADD,     /**< Addition */
    SUB,     /**< Subtraction */
    MUL,     /**< Multiplication */
    DIV,     /**< Division */
    EQL,     /**< Equality */
    POP,     /**< Pop */
    PSH,     /**< Push */
    DUP,     /**< Duplicate */
    JMP,     /**< Jump */
    JNZ,     /**< Jump if not zero */
    JIP,     /**< Jump if positive */
} Opcode;

/**
 * @brief Structure representing an instruction in the SASM assembly language.
 */
typedef struct {
    Opcode type;  /**< The opcode of the instruction */
    Word operand; /**< The operand of the instruction */
} Instruction;

/**
 * @brief Structure representing a program in the SASM assembly language.
 */
typedef struct {
    Instruction instructions[PROGRAM_CAPACITY]; /**< The array of instructions */
    Word instruction_count;                     /**< The number of instructions in the program */
    Word instruction_size;                      /**< The size of each instruction in bytes */
} Program;

/**
 * @brief Pushes a value onto the stack.
 *
 * @param r The CPU registers.
 * @param mem The memory of the virtual machine.
 * @param operand The value to push onto the stack.
 * @return An error code indicating the success or failure of the operation.
 */
Error __psh(Registers* r, Memory* mem, const Word* operand);

/**
 * @brief Performs the equality operation on the top two values of the stack.
 *
 * @param r The CPU registers.
 * @param mem The memory of the virtual machine.
 * @return An error code indicating the success or failure of the operation.
 */
Error __eql(Registers* r, Memory* mem);

/**
 * @brief Pops a value from the stack.
 *
 * @param r The CPU registers.
 * @param mem The memory of the virtual machine.
 * @return An error code indicating the success or failure of the operation.
 */
Error __pop(Registers* r, Memory* mem);

/**
 * @brief Duplicates a value on the stack.
 *
 * @param r The CPU registers.
 * @param mem The memory of the virtual machine.
 * @param operand The number of positions to duplicate.
 * @return An error code indicating the success or failure of the operation.
 */
Error __dup(Registers* r, Memory* mem, const Word* operand);

/**
 * @brief Executes an instruction in the virtual machine.
 *
 * @param prog The program containing the instructions.
 * @param mem The memory of the virtual machine.
 * @param cpu The CPU of the virtual machine.
 * @return An error code indicating the success or failure of the execution.
 */
Error executeInst(const Program* prog, Memory* mem, CPU* cpu);

/**
 * @brief Converts an opcode to its corresponding string representation.
 *
 * @param type The opcode to convert.
 * @return The string representation of the opcode.
 */
String opcodeAsStr(const Opcode* type);

/**
 * @brief Converts a string to its corresponding opcode.
 *
 * @param s The string to convert.
 * @return The opcode corresponding to the string.
 */
Opcode strAsOpcode(const String* s);