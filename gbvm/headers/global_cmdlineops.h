#pragma once
#include "global_definitions.h"

typedef enum {
    INPUT_FILE,
    OUTPUT_FILE,
    ASSEMBLE_MODE,
    DISASSEMBLE_MODE,
} Option;

char* getNextCmdLineArg(int* argc, char*** argv);

Option flagAsOption(char* s);