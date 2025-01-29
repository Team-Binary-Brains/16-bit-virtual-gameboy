/**
 * @file univ_defs.h
 * @brief Universal definitions for the 16-bit Virtual Gameboy (GBVM) library.
 *
 * This header file contains various universal definitions used in the GBVM library.
 * It includes necessary standard C libraries and defines several constants and types.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXECUTION_LIMIT 1024      /**< The maximum number of instructions that can be executed. */
#define PROGRAM_CAPACITY 1024     /**< The maximum capacity of the program memory. */
#define STACK_CAPACITY 1024       /**< The maximum capacity of the stack memory. */
#define MAX_WORD 32767            /**< The maximum value that can be stored in a Word type. */
#define ADDRESS_RANGE_LOW 0X0000  /**< The lower bound of the memory address range. */
#define ADDRESS_RANGE_HIGH 0XFFFF /**< The upper bound of the memory address range. */

typedef char Byte;               /**< Represents a single byte. */
typedef short int Word;          /**< Represents a 16-bit word. */
typedef int Double_Word;         /**< Represents a 32-bit double word. */
typedef long long int Quad_Word; /**< Represents a 64-bit quad word. */