#include "global_definitions.h"
#include "gbvm_errors.h"
#include "gbvm_strings.h"

const char* errorAsCstr(const Error* error)
{
    switch ((*error)) {
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
    case ERR_ILLEGAL_ALU_OPERATION:
        return "ERR_ILLEGAL_ALU_OPERATION";
    default:
        assert(0 && "error_as_cstr : Unreachable");
    }
}

void fileErrorDispWithExit(const char* message, const char* filePath)
{
    fprintf(stderr, "ERROR : %s\n %s : %s\n", message, filePath, strerror(errno));
    exit(1);
}

void executionErrorWithExit(const Error* error)
{
    fprintf(stderr, "Error : %s\n", errorAsCstr(error));
    exit(1);
}

void displayMsgWithExit(const char* message)
{
    fprintf(stderr, "ERROR : %s\n", message);
    // exit(1); done temporarily
}

void displayStringMessageError(const char* msg, String str)
{
    printf("|   |                                                                                                                              |\n");
    fprintf(stderr, "| W | WARNING | '%.*s' |", (int)str.length, str.data);
    fprintf(stderr, " %s", msg);
    for (size_t i = strlen(msg) + (int)str.length; i < 110; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "|\n");

    printf("|   |                                                                                                                              |\n");
}

void debugCommentDisplay(String* s)
{
    String seperator = splitStr(s, ' ');
    printf("| %.*s |", 1, seperator.data);
    if (s->length < 125) {
        printf(" %-*.*s |\n", (int)(124), (int)(s->length), s->data);
    } else {
        printf(" %.*s |\n", (int)(124), s->data);
    }
}