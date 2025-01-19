#pragma once

#include "global_definitions.h"
#include "gbvm_errors.h"
#include "gbvm_instructions.h"
#include "gbvm_memory.h"

typedef struct {
    Memory mem;
    Program prog;
    CPU cpu;
} Vm;

void dumpStack(FILE*, const Vm*);

void dumpFlags(FILE*, const Vm*);

void executeProgram(Vm* vm, int debug, int i);