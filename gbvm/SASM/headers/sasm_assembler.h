/**
 * @file sasm_assembler.h
 * @brief This file contains functions for assembling and disassembling programs in a custom assembly language.
 *
 * The functions in this file are used to manipulate custom assembly code and bytecode.
 */

#pragma once

#include "sasm_instructions.h"
#include "univ_strings.h"

/**
 * @brief Disassembles the bytecode from a binary file into a Program structure.
 *
 * This function takes the path to a binary file containing bytecode and
 * disassembles it into a Program structure.
 * The disassembled Program structure is returned.
 *
 * @param filePath The path to the file containing the bytecode.
 * @return The disassembled Program structure.
 */
Program disassembleBytecodeIntoProgram(const char* filePath);

/**
 * @brief Assembles a Program structure into bytecode and writes it to a binary file.
 *
 * This function takes a Program structure and assembles it into bytecode.
 * The assembled bytecode is then written to a binary file specified by the filePath parameter.
 *
 * @param prog The Program structure to assemble.
 * @param filePath The path to the output file.
 */
void assembleProgramIntoBytecode(const Program* prog, const char* filePath);

/**
 * @brief Processes a line of assembly code and converts it into an Instruction structure.
 *
 * This function is a helper function used in the 'parseAsmIntoProgram' function.
 * It takes a line of assembly code as input and processes it, converting it into
 * an Instruction structure. The processed Instruction structure is returned.
 *
 * @param line The line of assembly code to process.
 * @return The processed Instruction structure.
 */
Instruction processLine(String* line);

/**
 * @brief Parses a string of assembly code into a Program structure.
 *
 * This function takes a string of assembly code as input and parses
 * it into a Program structure.
 * The parsed Program structure is returned.
 *
 * @param src The string of assembly code to parse.
 * @return The parsed Program structure.
 */
Program parseAsmIntoProgram(String* src);

/**
 * @brief Loads the contents of a file into a string.
 *
 * This function takes the path to a file and loads its
 * contents into a string. The loaded string is returned.
 *
 * @param filePath The path to the file to load.
 * @return The loaded string.
 */
String loadFileIntoString(const char* filePath);

/**
 * @brief Writes a Program structure to a non-binary file.
 *
 * This function takes a Program structure and writes it
 * to a non-binary file specified by the filePath parameter.
 *
 * @param prog The Program structure to write.
 * @param filePath The path to the output file.
 */
void writeProgramToFile(const Program* prog, const char* filePath);

/**
 * @brief Performs assembly mode operations.
 *
 * This function is a helper function that will be executed when running
 * in assembly mode. It takes the path to an input file containing assembly
 * code and the path to an output file to write the assembled bytecode.
 * It performs the necessary steps to assemble the code and write the
 * bytecode to the output file.
 * It returns 0 if successful.
 *
 * @param inputFile The path to the input file containing assembly code.
 * @param outputFile The path to the output file to write the assembled bytecode.
 * @return 0 if successful.
 */
int assemblyMode(char* inputFile, char* outputFile);

/**
 * @brief Performs disassembly mode operations.
 *
 * This function is a helper function that will be executed when running
 * in disassembly mode. It takes the path to an input file containing
 * bytecode and the path to an output file to write the disassembled
 * assembly code. It performs the necessary steps to disassemble the
 * bytecode and write the assembly code to the output file.
 * It returns 0 if successful.
 *
 * @param inputFile The path to the input file containing bytecode.
 * @param outputFile The path to the output file to write the disassembled assembly code.
 * @return 0 if successful.
 */
int disassemblyMode(char* inputFile, char* outputFile);