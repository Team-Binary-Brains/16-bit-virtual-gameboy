#include "univ_cmdutils.h"
#include "univ_defs.h"

typedef struct {
    char* flag;
    Option type;
} OptionFlag;

static OptionFlag OptionStringMap[] = {
    { "-i", FILE_INPUT },
    { "-o", FILE_OUTPUT },
    { "-a", MODE_ASSEMBLE },
    { "-d", MODE_DISASSEMBLE },
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