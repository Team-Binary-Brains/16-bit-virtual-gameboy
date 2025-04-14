/**
 * @file virex.h
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
#include "univ_malloc.h"
#include "virex_tui.h"
#include "virex_vmcalls.h"

/**
 * @struct Vm
 * Represents the virtual machine instance.
 */

typedef struct {
    Memory mem;   /**< The memory component of the virtual machine. */
    Program prog; /**< The program component of the virtual machine. */
    CPU cpu;      /**< The CPU component of the virtual machine. */
    VmCalls vmCalls;
    display disp;
    Region region;
} Vm;

/**
 * Dumps the contents of the stack to the specified stream.
 *
 * @param stream The stream to dump the stack contents to.
 * @param vm The virtual machine instance.
 */
void dumpStack(WINDOW* win, const Vm* vm);

/**
 * Dumps the flags of the virtual machine to the specified stream.
 *
 * @param stream The stream to dump the flags to.
 * @param cpu The cpu of virtual machine instance.
 */
void dumpFlags(WINDOW* win, CPU* cpu);

void dumpDetails(WINDOW* win, OpcodeDetails* details, Instruction* inst);

/**
 * Executes the program loaded in the virtual machine.
 *
 * @param vm The virtual machine instance.
 * @param debug The debug level (0, 1, or 2).
 * @param i The current execution count.
 */
void executeProgram(Vm* vm, int debug, int i);

/**
 * @brief Executes an instruction in the virtual machine.
 *
 * @param prog The program containing the instructions.
 * @param mem The memory of the virtual machine.
 * @param cpu The CPU of the virtual machine.
 * @return An error code indicating the success or failure of the execution.
 */
Error executeInst(Vm* vm, WINDOW* win);

Register* getReg(RegID id, Vm* vm);

void setReg(RegID id, Vm* vm, DataEntry val);