# GameBoy Virtual Machine (GBVM)
**File:** `gbvm.h`  
**Author:** Soham Metha  
**Date:** January 2025

This file contains the declarations of functions and structures related to the virtual machine. The virtual machine represents the execution environment for the Game Boy program.

## Structures

### `Vm`
Represents the virtual machine instance.

- `Memory mem`: The memory component of the virtual machine.
- `Program prog`: The program component of the virtual machine.
- `CPU cpu`: The CPU component of the virtual machine.

## Functions

### void loadProgram(Vm* vm, char* inputFile)
Loads the program from the specified input file into the virtual machine.

- **Parameters**:
  - `vm`: The virtual machine instance.
  - `inputFile`: The input binary file containing the program bytecode.

### void dumpStack(FILE* stream, const Vm* vm)
Dumps the contents of the stack to the specified stream.

- **Parameters**:
  - `stream`: The stream to dump the stack contents to.
  - `vm`: The virtual machine instance.

### void dumpFlags(FILE* stream, const Vm* vm)
Dumps the flags of the virtual machine to the specified stream.

- **Parameters**:
  - `stream`: The stream to dump the flags to.
  - `vm`: The virtual machine instance.

### void executeProgram(Vm* vm, int debug, int i)
Recursively executes the program loaded in the virtual machine.

- **Parameters**:
  - `vm`: The virtual machine instance.
  - `debug`: The debug level (0, 1, or 2).
  - `i`: The current execution count.