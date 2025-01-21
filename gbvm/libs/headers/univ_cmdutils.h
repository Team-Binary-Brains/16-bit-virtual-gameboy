#pragma once
#include "univ_defs.h"

typedef enum {
    INPUT_FILE,
    OUTPUT_FILE,
    ASSEMBLE_MODE,
    DISASSEMBLE_MODE,
    ASM_LANG,
} Option;

char* getNextCmdLineArg(int* argc, char*** argv);

Option flagAsOption(char* s);