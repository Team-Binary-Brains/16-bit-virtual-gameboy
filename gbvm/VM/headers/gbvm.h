#pragma once

#include "univ_defs.h"
#include "univ_errors.h"
#include "sasm_instructions.h"
#include "sasm_memory.h"

typedef struct {
    Memory mem;
    Program prog;
    CPU cpu;
} Vm;

void dumpStack(FILE*, const Vm*);

void dumpFlags(FILE*, const Vm*);

void executeProgram(Vm* vm, int debug, int i);