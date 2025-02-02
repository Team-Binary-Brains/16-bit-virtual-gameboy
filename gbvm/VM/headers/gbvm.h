/**
 * @file gbvm.h
 * @brief This file contains the declarations of functions and structures related to the virtual machine.
 *
 * The virtual machine represents the execution environment for the Game Boy program.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include "univ_defs.h"
#include "univ_errors.h"
#include "sasm_instructions.h"
#include "sasm_memory.h"

/**
 * @struct Vm
 * Represents the virtual machine instance.
 */
typedef struct {
    Memory mem;   /**< The memory component of the virtual machine. */
    Program prog; /**< The program component of the virtual machine. */
    CPU cpu;      /**< The CPU component of the virtual machine. */
} Vm;

/**
 * Loads the program from the specified input file into the virtual machine.
 *
 * @param vm The virtual machine instance.
 * @param inputFile The input binary file containing the program bytecode.
 */
void loadProgram(Vm* vm, char* inputFile);

/**
 * Dumps the contents of the stack to the specified stream.
 *
 * @param stream The stream to dump the stack contents to.
 * @param vm The virtual machine instance.
 */
void dumpStack(FILE* stream, const Vm* vm);

/**
 * Dumps the flags of the virtual machine to the specified stream.
 *
 * @param stream The stream to dump the flags to.
 * @param cpu The cpu of virtual machine instance.
 */
void dumpFlags(FILE* stream, CPU* cpu);

/**
 * Executes the program loaded in the virtual machine.
 *
 * @param vm The virtual machine instance.
 * @param debug The debug level (0, 1, or 2).
 * @param i The current execution count.
 */
void executeProgram(Vm* vm, int debug, int i);