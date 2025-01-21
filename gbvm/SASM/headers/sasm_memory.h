#pragma once

#include "univ_defs.h"

typedef struct {
    Word AX;
    Word BX;
    Word CX;
    Word DX;
    Word SP;
    Word IP;
} Registers;

typedef struct {
    Registers registers;
    Word flags;
} CPU;

typedef struct {
    Word stack[STACK_CAPACITY];
} Memory;
