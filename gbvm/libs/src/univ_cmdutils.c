#include "univ_cmdutils.h"
#include "univ_defs.h"
#include "univ_hashmap.h"

typedef struct {
    char* flag;
    Option type;
} OptionFlag;

static OptionFlag Options[] = {
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

void createOptionStringMap(HashTable *OptionStringMap)
{
    OptionStringMap = createHashTable(1000, stringHashFunc, stringKeyCompare, stringKeyDestroy, intValueDestroy);
    size_t len = sizeof(Options) / sizeof(Options[0]);
    for (size_t i = 0; i < len; ++i) {
        char *key = strdup(Options[i].flag);           // Dynamically allocate memory for the key
        int *value = malloc(sizeof(int));     // Dynamically allocate memory for the value
        *value = Options[i].type;
        insert(OptionStringMap, key, value);
    }
}