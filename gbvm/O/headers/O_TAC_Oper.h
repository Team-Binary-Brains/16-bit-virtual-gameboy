#pragma once
// Enumeration of TAC operations.
typedef enum {
    TAC_ASSIGN,
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_MOD,
    TAC_EQ,
    TAC_NE,
    TAC_LT,
    TAC_LE,
    TAC_GT,
    TAC_GE,
    TAC_LABEL,
    TAC_GOTO,
    TAC_IF,
    TAC_PRINT,
    TAC_SCAN,
    TAC_EXIT
} TacOp;