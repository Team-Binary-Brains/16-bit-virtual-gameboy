#pragma once

#include "external_includes.h"
#include "gbvm_errors.h"
#include "gbvm_instructions.h"
#include "gbvm_memory.h"

typedef struct {
    Memory mem;
    Program prog;
    CPU cpu;
} Vm;

void dumpStack(FILE*, const Vm*);

Error executeProgram(Vm* vm);