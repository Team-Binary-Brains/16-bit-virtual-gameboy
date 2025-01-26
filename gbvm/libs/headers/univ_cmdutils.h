/**
 * @file univ_cmdutils.h
 * @brief Contains utility functions for command line argument parsing.
 *
 * This file provides utility functions for parsing command line arguments in a generic manner.
 * It includes functions for retrieving the next command line argument
 * and converting a command line flag to its corresponding option type.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once
#include "univ_defs.h"

/**
 * @brief Enumeration of flag options.
 *
 * This enumeration defines the possible options for command line flags.
 */
typedef enum {
    FILE_INPUT,       /**< Option for file input. */
    FILE_OUTPUT,      /**< Option for file output. */
    MODE_ASSEMBLE,    /**< Option for assembly mode. */
    MODE_DISASSEMBLE, /**< Option for disassembly mode. */
    ASM_LANG,         /**< Option for assembly language. */
} Option;

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

/**
 * @brief Converts a command line flag to its corresponding option type.
 *
 * This function takes a command line flag as input and searches for a matching
 * flag in the OptionStringMap. If a match is found, it returns the corresponding
 * option type. If no match is found, it returns -1.
 *
 * @param s The command line flag to convert.
 * @return The corresponding option type, or -1 if no match is found.
 */
Option flagAsOption(char* s);