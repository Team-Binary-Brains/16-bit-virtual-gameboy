/**
 * @file sasm_alu.h
 * @brief Declaration of operations performed by the ALU.
 *
 * This file contains the declarations of various enums representing different types of operations
 * that can be performed on the CPU stack, such as arithmetic, logical, shift, rotate, and compare.
 * It also includes the declaration of the function performArithmeticOperation, which performs
 * arithmetic operations on the CPU stack and updates the stack and CPU flags accordingly.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once
#include "sasm_memory.h"
#include "univ_errors.h"

typedef enum {
    ARITHMETIC, /**< Represents arithmetic operations */
    LOGICAL,    /**< Represents logical operations */
    SHIFT,      /**< Represents shift operations */
    ROTATE,     /**< Represents rotate operations */
    COMPARE     /**< Represents compare operations */
} OperationType;

typedef enum {
    ADDITION, /**< Represents addition operation */
    SUBTRACT, /**< Represents subtraction operation */
    MULTIPLY, /**< Represents multiplication operation */
    DIVIDE,   /**< Represents division operation */
    MODULO    /**< Represents modulo operation */
} ArithmeticOperations;

typedef enum {
    LEFT_SHIFT, /**< Represents left shift operation */
    RIGHT_SHIFT /**< Represents right shift operation */
} ShiftOperations;

typedef enum {
    LEFT_ROTATE, /**< Represents left rotate operation */
    RIGHT_ROTATE /**< Represents right rotate operation */
} RotateOperations;

typedef enum {
    EQUAL,         /**< Represents equal comparison */
    NOT_EQUAL,     /**< Represents not equal comparison */
    GREATER,       /**< Represents greater than comparison */
    LESS,          /**< Represents less than comparison */
    GREATER_EQUAL, /**< Represents greater than or equal comparison */
    LESS_EQUAL     /**< Represents less than or equal comparison */
} CompareOperations;

/**
 * @brief Performs arithmetic operations on the CPU stack.
 *
 * This function takes in a CPU structure, a Memory structure, and an ArithmeticOperations enum
 * representing the desired arithmetic operation to be performed. It performs the specified
 * arithmetic operation on the top two elements of the CPU stack and updates the stack accordingly.
 * It also updates the CPU flags (carry, overflow, zero, sign, and parity) based on the result
 * of the operation.
 *
 * @param cpu The CPU structure containing the stack and registers.
 * @param mem The Memory structure containing the stack memory.
 * @param operation The arithmetic operation to be performed.
 * @return An Error code indicating the success or failure of the operation.
 *
 */
Error performArithmeticOperation(CPU* cpu, Memory* mem, ArithmeticOperations operation);

/*
TODO: Implement the following cases
case LOGICAL
case SHIFT
case ROTATE
case COMPARE
*/