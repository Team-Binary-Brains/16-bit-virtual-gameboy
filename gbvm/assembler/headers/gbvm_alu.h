#pragma once
#include "gbvm_memory.h"
#include "gbvm_errors.h"

typedef enum {
    ARITHMETIC,
    LOGICAL,
    SHIFT,
    ROTATE,
    COMPARE
} OperationType;

typedef enum {
    ADDITION,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO
} ArithmeticOperations;

typedef enum {
    AND,
    OR,
    XOR,
    NOT
} LogicalOperations;

typedef enum {
    LEFT_SHIFT,
    RIGHT_SHIFT
} ShiftOperations;

typedef enum {
    LEFT_ROTATE,
    RIGHT_ROTATE
} RotateOperations;

typedef enum {
    EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL
} CompareOperations;

Error performArithmeticOperation(CPU* cpu, Memory* mem, ArithmeticOperations operation);

/*
TODO: Implement the following cases
case LOGICAL
case SHIFT
case ROTATE
case COMPARE
*/