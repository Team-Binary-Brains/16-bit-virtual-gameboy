#pragma once

#include "gbvm_instructions.h"
#include "gbvm_memory.h"
#include "gbvm_strings.h"

Program disassembleBytecodeIntoProgram(const char*);

void assembleProgramIntoBytecode(const Program* prog, const char*);

Instruction processLine(String*);

Program parseAsmIntoProgram(String*);

String loadFileIntoString(const char*);