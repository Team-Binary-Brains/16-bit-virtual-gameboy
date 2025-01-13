#include "gbvm.h"

void loadProgram(Vm* vm, Program prog)
{
    assert(prog.instruction_size < PROGRAM_CAPACITY);

    memcpy(vm->prog.instructions, prog.instructions, prog.instruction_count * prog.instruction_size);
    vm->prog.instruction_size = prog.instruction_size;
    vm->prog.instruction_count = prog.instruction_count;
}

void dumpStack(FILE* stream, const Vm* vm)
{
    fprintf(stream, "Stack : \n");
    if (vm->mem.stack_size > 0) {
        for (Word i = 0; i < vm->mem.stack_size; ++i) {
            fprintf(stream, "  %d\n", vm->mem.stack[i]);
        }
    } else {
        fprintf(stream, " [Empty]\n");
    }
}

FILE* openFileWithCheck(const char* filePath, const char* mode)
{
    FILE* f = fopen(filePath, mode);

    if (!f) {
        fileErrorDispWithExit("can't open file", filePath);
    }
    return f;
}

void closeFileWithCheck(const char* filePath, FILE* file)
{
    if (ferror(file)) {
        fileErrorDispWithExit("can't write to file", filePath);
    }

    fclose(file);
}

void assembleInstructionsIntoBinary(Program prog, const char* filePath)
{
    FILE* f = openFileWithCheck(filePath, "wb");

    fwrite(prog.instructions, prog.instruction_size, prog.instruction_count, f);

    closeFileWithCheck(filePath, f);
}

Program loadBytecodeIntoProgram(const char* filePath)
{
    FILE* f = openFileWithCheck(filePath, "rb");
    Program prog;
    if (fseek(f, 0, SEEK_END) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    Word m = ftell(f);

    if (m < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    prog.instruction_size = sizeof(prog.instructions[0]);
    assert(m % prog.instruction_size == 0);
    prog.instruction_count = m / prog.instruction_size;
    assert(prog.instruction_count <= PROGRAM_CAPACITY);

    if (fseek(f, 0, SEEK_SET) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    prog.instruction_size = fread(prog.instructions, prog.instruction_size, prog.instruction_count, f);

    closeFileWithCheck(filePath, f);

    return prog;
}

Instruction processLine(String line)
{
    line = trim(line);
    Opcode op = strAsOpcode(splitStr(&line, ' '));
    if (op < PSH) {
        return (Instruction) { .type = op };
    }
    line = ltrim(line);
    Word operand = strToWord(splitStr(&line, ' '));
    // printf("Operand :%d\n", operand);
    return (Instruction) { .type = op, .operand = operand };
}

Program translate_asm(String src)
{
    Instruction ins[src.parts];
    Program prog;
    prog.instruction_count = 0;
    while (src.parts > 0) {
        String line = splitStr(&src, '\n');
        // printf("Line : %.*s\n", (int)(line.parts), line.data);
        ins[prog.instruction_count++] = processLine(line);
    }

    memcpy(prog.instructions, ins, sizeof(ins));
    prog.instruction_size = sizeof(ins[0]);
    return prog;
}

String loadFileIntoProgramString(const char* filePath)
{
    FILE* f = openFileWithCheck(filePath, "r");
    if (fseek(f, 0, SEEK_END) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    Word m = ftell(f);

    if (m < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    char* buffer = malloc(m);

    if (buffer == NULL) {
        fileErrorDispWithExit("can't allocate memory for file", filePath);
    }

    if (fseek(f, 0, SEEK_SET) < 0) {
        fileErrorDispWithExit("can't read from file", filePath);
    }

    size_t n = fread(buffer, 1, m, f);

    closeFileWithCheck(filePath, f);

    return (String) {
        .parts = n,
        .data = buffer
    };
}