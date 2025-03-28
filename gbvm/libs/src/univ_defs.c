#include "univ_defs.h"

char* getNextCmdLineArg(int* argc, char*** argv)
{
    assert(*argc > 0);

    char* arg = **argv;

    *argc -= 1;
    *argv += 1;

    return arg;
}