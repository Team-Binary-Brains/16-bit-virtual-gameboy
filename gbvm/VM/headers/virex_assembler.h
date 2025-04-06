#pragma once
#include "virex.h"

void loadInternalCallIntoVm(Vm* Vm, InternalVmCall call);
void loadStandardCallsIntoVm(Vm* Vm);
/**
 * Loads the program from the specified input file into the virtual machine.
 *
 * @param vm The virtual machine instance.
 * @param inputFile The input binary file containing the program bytecode.
 */
void loadProgramIntoVm(Vm* vm, const char* inputFile);