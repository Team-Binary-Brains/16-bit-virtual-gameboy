#include "univ_errors.h"
#include "univ_strings.h"
#include "univ_defs.h"

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
        assert(0 && "univ_errors : errorAsCstr : Unreachable");
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
    exit(1);
}

void displayStringMessageError(const char* msg, String str)
{
    fprintf(stderr, "\n|   |                                                                                                                              |");
    fprintf(stderr, "\n| W | WARNING | '%.*s' | %s", (int)str.length, str.data, msg);

    for (size_t i = strlen(msg) + str.length; i < 110; i++)
        fprintf(stderr, " ");

    fprintf(stderr, "|"
                    "\n|   |                                                                                                                              |\n");
}

void debugCommentDisplay(String* s)
{
    String seperator = splitStr(s, ' ');
    fprintf(stdout, "\n| %.*s |", 1, seperator.data);

    if (s->length < 125)
        fprintf(stdout, " %-*.*s |", (int)(124), (int)(s->length), s->data);
    else
        fprintf(stdout, " %.*s |", (int)(124), s->data);
}

void debugMessageDisplay(String* s)
{
    fprintf(stdout, "\n| D |");

    if (s->length < 125)
        fprintf(stdout, " %-*.*s |", (int)(124), (int)(s->length), s->data);
    else
        fprintf(stdout, " %.*s |", (int)(124), s->data);
}