#pragma once
#include "univ_defs.h"

typedef enum {
    FILE_INPUT,
    FILE_OUTPUT,
    MODE_ASSEMBLE,
    MODE_DISASSEMBLE,
    ASM_LANG,
} Option;

char* getNextCmdLineArg(int* argc, char*** argv);

Option flagAsOption(char* s);