#pragma once

typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_DIV_BY_ZERO,
    ERR_ILLEGAL_INST,
    ERR_ILLEGAL_INST_ACCESS,
    ERR_ILLEGAL_OPERAND,
    ERR_ILLEGAL_ALU_OPERATION,
} Error;

const char* errorAsCstr(const Error*);

void fileErrorDispWithExit(const char*, const char*);

void executionErrorWithExit(const Error*);

void displayMsgWithExit(const char*);