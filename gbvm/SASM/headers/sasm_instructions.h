/**
 * @file sasm_instructions.h
 * @brief This file contains the declarations of functions and data structures related to the execution of instructions in the SASM assembly language.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include "univ_defs.h"
#include "univ_strings.h"
/**
 * @brief Enumeration of opcodes for SASM instructions.
 */
typedef enum {

    INST_DONOP = 0,
    INST_RETRN,
    INST_CALLN,
    INST_CALLF,
    INST_SHUTS,
    INST_PUSH,
    INST_DROP,
    INST_SWAP,
    INST_ADDI,
    INST_SUBI,
    INST_MULI,
    INST_DIVI,
    INST_MODI,
    INST_MULU,
    INST_DIVU,
    INST_MODU,
    INST_ADDF,
    INST_SUBF,
    INST_MULF,
    INST_DIVF,
    INST_JMPU,
    INST_JMPC,
    INST_ANDB,
    INST_NOTB,
    INST_CPY,
    INST_SET,
    INST_GET,
    INST_DUP,
    INST_NOT,
    INST_EQI,
    INST_GEI,
    INST_GTI,
    INST_LEI,
    INST_LTI,
    INST_NEI,
    INST_EQU,
    INST_GEU,
    INST_GTU,
    INST_LEU,
    INST_LTU,
    INST_NEU,
    INST_EQF,
    INST_GEF,
    INST_GTF,
    INST_LEF,
    INST_LTF,
    INST_NEF,
    INST_ORB,
    INST_XOR,
    INST_SHR,
    INST_SHL,
    INST_I2F,
    INST_U2F,
    INST_F2I,
    INST_F2U,
    INST_READ1U,
    INST_READ2U,
    INST_READ4U,
    INST_READ8U,
    INST_READ1I,
    INST_READ2I,
    INST_READ4I,
    INST_READ8I,
    INST_WRITE1,
    INST_WRITE2,
    INST_WRITE4,
    INST_WRITE8,

    NUMBER_OF_INSTS
} Opcode;

/**
 * @brief Structure representing an instruction in the SASM assembly language.
 */
typedef struct {
    Opcode type;      /**< The opcode of the instruction */
    QuadWord operand; /**< The operand of the instruction */
    QuadWord operand2;
} Instruction;

typedef struct {
    Opcode type;
    const char* name;
    bool has_operand;
    bool has_operand2;
} OpcodeDetails;

/**
 * @brief Structure representing a program in the SASM assembly language.
 */
typedef struct Program {
    Instruction instructions[PROGRAM_CAPACITY]; /**< The array of instructions */
    DataEntry instruction_count;                /**< The number of instructions in the program */
    DataEntry instruction_size;                 /**< The size of each instruction in bytes */
} Program;

// TYPE CASTING
QuadWord quadword_u64(uint64_t u64);
QuadWord quadword_i64(int64_t i64);
QuadWord quadword_f64(double f64);
QuadWord quadword_ptr(void* ptr);

bool getOpcodeDetailsFromName(String name, OpcodeDetails* outPtr);

OpcodeDetails getOpcodeDetails(Opcode type);
