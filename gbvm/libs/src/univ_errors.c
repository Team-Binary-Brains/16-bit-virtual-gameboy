#include "univ_errors.h"
#include "univ_defs.h"
#include "univ_strings.h"

const char* getNameOfError(const Error* error)
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
    case ERR_NULL_CALL:
        return "ERR_NULL_CALL";
    case ERR_ILLEGAL_MEMORY_ACCESS:
        return "ERR_ILLEGAL_MEMORY_ACCESS";
    case ERR_NAN:
        return "ERR_NAN";
    case ERR_ALREADY_BOUND:
        return "ERR_ALREADY_BOUND";
    default:
        assert(0 && "univ_errors : getNameOfError : Unreachable");
    }
}

void fileErrorDispWithExit(const char* message, const char* filePath)
{
    fprintf(stderr, "ERROR : %s\n %s : %s\n", message, filePath, strerror(errno));
    exit(1);
}

void executionErrorWithExit(const Error* error)
{
    fprintf(stderr, "Error : %s\n", getNameOfError(error));
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
    fprintf(stderr, "\n| W | ERROR | '%.*s' | %s", (int)str.length, str.data, msg);

    for (size_t i = strlen(msg) + str.length; i < 110; i++)
        fprintf(stderr, " ");

    fprintf(stderr, "|"
                    "\n|   |                                                                                                                              |\n");
    exit(1);
}

void debugCommentDisplay(String* s)
{
    String seperator = splitStrByChar(s, ' ');
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

void displayErrorDetailsWithExit(FileLocation location, const char* msg, String reason)
{
    fprintf(stderr, FLFmt ": ERROR: %s `" strFmt "`\n",
        FLArg(location),
        msg,
        strArg(reason));
    exit(1);
}

void displayErrorLocationWithExit(FileLocation location, const char* msg)
{
    fprintf(stderr, "%.*s : %d : ERROR: %s \n",
        (int)location.filePath.length,
        location.filePath.data,
        location.lineNumber,
        msg);
    exit(1);
}

void displayCErrorDetailsWithExit(FileLocation location, const char* msg, const char* reason)
{
    fprintf(stderr, FLFmt ": ERROR: %s `%s`\n",
        FLArg(location),
        msg,
        reason);
    exit(1);
}