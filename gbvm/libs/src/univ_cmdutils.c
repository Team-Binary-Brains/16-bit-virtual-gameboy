#include "univ_cmdutils.h"
#include "univ_defs.h"

typedef struct {
    char* flag;
    Option type;
} OptionFlag;

static OptionFlag OptionStringMap[] = {
    { "-i", INPUT_FILE },
    { "-o", OUTPUT_FILE },
    { "-a", ASSEMBLE_MODE },
    { "-d", DISASSEMBLE_MODE },
    { "-l", ASM_LANG },
};

char* getNextCmdLineArg(int* argc, char*** argv)
{
    assert(*argc > 0);

    char* arg = **argv;

    *argc -= 1;
    *argv += 1;

    return arg;
}

Option flagAsOption(char* s)
{
    size_t len = sizeof(OptionStringMap) / sizeof(OptionStringMap[0]);

    for (size_t i = 0; i < len; ++i) {
        if (strcmp(s, OptionStringMap[i].flag) == 0) {
            return OptionStringMap[i].type;
        }
    }

    return -1;
}