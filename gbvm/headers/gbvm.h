// #include "gbvm_memory.h"
// #include "gbvm_errors.h"
// #include "gbvm_strings.h"
#include "gbvm_instructions.h"

#define EXECUTION_LIMIT 90

typedef struct {
    Memory mem;
    Program prog;
    CPU cpu;
} Vm;

void loadProgram(Vm*, Program);

void dumpStack(FILE*, const Vm*);

FILE* openFileWithCheck(const char*, const char*);

void assembleInstructionsIntoBinary(Program, const char*);

Program loadBytecodeIntoProgram(const char*);

Program translate_asm(String);

String loadFileIntoProgramString(const char*);