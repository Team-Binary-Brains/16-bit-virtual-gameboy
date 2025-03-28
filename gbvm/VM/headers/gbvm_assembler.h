#pragma once
#include "gbvm.h"

void pushCall(Vm* Vm, InternalVmCall call);
void loadStandardCalls(Vm* Vm);
/**
 * Loads the program from the specified input file into the virtual machine.
 *
 * @param vm The virtual machine instance.
 * @param inputFile The input binary file containing the program bytecode.
 */
void loadProgram(Vm* vm, const char* inputFile);