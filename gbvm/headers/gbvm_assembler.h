#include "gbvm_instructions.h"

Program disassembleBytecodeIntoProgram(const char*);

void assembleProgramIntoBytecode(const Program* prog, const char*);

Instruction processLine(String*);

Program parseAsmIntoProgram(String*);

String loadFileIntoString(const char*);