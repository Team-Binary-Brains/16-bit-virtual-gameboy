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
#include "sasm_opcodes.h"

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