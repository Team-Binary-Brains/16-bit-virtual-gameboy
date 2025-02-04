/**
 * @file sasm_instructions.h
 * @brief This file contains the declarations of functions and data structures related to the execution of instructions in the SASM assembly language.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "univ_defs.h"
#include "univ_errors.h"
#include "univ_strings.h"
#include "sasm_memory.h"
/**
 * @brief Enumeration of opcodes for SASM instructions.
 */
typedef enum {

    DONOP,
    CLRCF,
    TGLCF,
    SHUTS,

    DECR,
    NEG,
    GOTO,
    JA,
    JNC,
    JC,
    JBE,
    JCXZ,
    JZ,
    JG,
    JGE,
    JL,
    JLE,
    JNZ,
    JNO,
    JNP,
    JNS,
    JO,
    JP,
    JS,

    CPY,
    ADC,
    ADD,
    AND

} Opcode;

/**
 * @brief Structure representing an instruction in the SASM assembly language.
 */
typedef struct {
    Opcode type;  /**< The opcode of the instruction */
    Word operand; /**< The operand of the instruction */
    Word operand2;
    // TODO : DISCARD USE OF THIS FLAG
    Byte registerMode;
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

Error __DONOP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CLRCF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __TGLCF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SHUTS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);

Error __DECR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __NEG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __GOTO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JBE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JCXZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JGE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JLE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);

Error __CPY(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __ADC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __ADD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AND(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);