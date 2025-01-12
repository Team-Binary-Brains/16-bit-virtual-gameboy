// #include "gbvm_memory.h"
// #include "gbvm_errors.h"
#include "gbvm_instructions.h"

#define EXECUTION_LIMIT 90

typedef struct {
    Memory mem;
    Program prog;
    CPU cpu;
} Vm;

void loadProgram(Vm*, Program prog);

void dumpStack(FILE*, const Vm*);

void assembleInstructions(Program prog, const char*);

void loadBytecode(Vm*, const char*);

Program translate_asm(char*, size_t);
