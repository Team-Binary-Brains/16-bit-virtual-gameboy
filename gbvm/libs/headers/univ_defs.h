/**
 * @file univ_defs.h
 * @brief Universal definitions for the 16-bit Virtual Gameboy (GBVM) library.
 *
 * This header file contains various universal definitions used in the GBVM library.
 * It includes necessary standard C libraries and defines several constants and types.
 *

 * @authors Soham Metha, Omkar Jagtap
 * @date January 2025
 */

#pragma once

#define INTERNAL_VMCALLS_CAPACITY 1024
#define EXTERNAL_VMCALLS_CAPACITY 1024
#define LOAD_FACTOR_THRESHOLD 0.75
#define _XOPEN_SOURCE_EXTENDED
#define BINDINGS_CAPACITY 1024
#define PROGRAM_CAPACITY 1024
#define MEMORY_CAPACITY 65536
#define LABELS_CAPACITY 1024
#define MAX_INCLUDE_LEVEL 10
#define STACK_CAPACITY 1024
#define FILE_MAGIC 0x484f53
#define FILE_VERSION 0x4D41
#define COMMENT_SYMBOL ';'
#define PREP_SYMBOL '%'

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>

typedef uint8_t Byte;
typedef uint16_t Word;
typedef uint32_t DoubleWord;

typedef uint64_t DataEntry;
typedef uint64_t InstAddr;
typedef uint64_t MemoryAddr;
typedef uint64_t StackAddr;
typedef uint64_t NativeID;

typedef union {
    uint64_t as_u64;
    int64_t as_i64;
    double as_f64;
    void* as_ptr;
} QuadWord;

/**
 * @brief Retrieves the next command line argument.
 *
 * This function retrieves the next command line argument from the argument list
 * and updates the argument count and argument list pointers to only contain the
 * remaining pointers.
 *
 * @param argc A pointer to the argument count.
 * @param argv A pointer to the argument list.
 * @return The next command line argument.
 */
char* getNextCmdLineArg(int* argc, char*** argv);