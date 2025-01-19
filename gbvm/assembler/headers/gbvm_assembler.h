#pragma once

#include "gbvm_instructions.h"
#include "gbvm_memory.h"
#include "gbvm_strings.h"

Program disassembleBytecodeIntoProgram(const char*);

void assembleProgramIntoBytecode(const Program* prog, const char*);

Instruction processLine(String*);

Program parseAsmIntoProgram(String*);

String loadFileIntoString(const char*);

void writeProgramToFile(const Program* prog, const char* filePath);

int assemblyMode(char* inputFile, char* outputFile);

int disassemblyMode(char* inputFile, char* outputFile);