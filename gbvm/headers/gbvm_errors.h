// #include "gbvm_memory.h"
#include "gbvm_strings.h"

typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_DIV_BY_ZERO,
    ERR_ILLEGAL_INST,
    ERR_ILLEGAL_INST_ACCESS,
    ERR_ILLEGAL_OPERAND
} Error;

const char* errorAsCstr(Error);

void fileErrorDispWithExit(const char*, const char*);

void executionErrorWithExit(Error);