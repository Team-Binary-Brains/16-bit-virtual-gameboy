#include "gbvm_errors.h"

const char* errorAsCstr(Error error)
{
    switch (error) {
    case ERR_OK:
        return "ERR_OK";
    case ERR_STACK_OVERFLOW:
        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
        return "ERR_STACK_UNDERFLOW";
    case ERR_DIV_BY_ZERO:
        return "ERR_DIV_BY_ZERO";
    case ERR_ILLEGAL_INST:
        return "ERR_ILLEGAL_INST";
    case ERR_ILLEGAL_INST_ACCESS:
        return "ERR_ILLEGAL_INST_ACCESS";
    case ERR_ILLEGAL_OPERAND:
        return "ERR_ILLEGAL_OPERAND";
    default:
        assert(0 && "error_as_cstr : Unreachable");
    }
}

void fileErrorDispWithExit(const char* message, const char* filePath)
{
    fprintf(stderr, "ERROR : %s\n %s : %s\n", message, filePath, strerror(errno));
    exit(1);
}

void executionErrorWithExit(Error error)
{
    fprintf(stderr, "Error : %s\n", errorAsCstr(error));
    exit(1);
}