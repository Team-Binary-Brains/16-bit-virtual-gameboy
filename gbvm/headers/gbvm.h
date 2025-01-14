#include "gbvm_assembler.h"

#define EXECUTION_LIMIT 90

typedef struct {
    Memory mem;
    Program prog;
    CPU cpu;
} Vm;

void dumpStack(FILE*, const Vm*);

Error executeProgram(Vm* vm);