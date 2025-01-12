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

void assembleInstructions(Program prog, const char* file_path)
{
    FILE* f = fopen(file_path, "wb");
    if (!f) {
        fprintf(stderr, "ERROR : can't open file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    fwrite(prog.instructions, prog.instruction_size, prog.instruction_count, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR : can't write to file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    fclose(f);
}

void loadBytecode(Vm* vm, const char* file_path)
{
    FILE* f = fopen(file_path, "rb");
    if (!f) {
        fprintf(stderr, "ERROR : can't open file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }

    if (fseek(f, 0, SEEK_END) < 0) {
        fprintf(stderr, "ERROR : can't read from file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    long m = ftell(f);
    if (m < 0) {
        fprintf(stderr, "ERROR : can't read from file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    assert(m % sizeof(vm->prog.instructions[0]) == 0);
    assert((size_t)m <= PROGRAM_CAPACITY * sizeof(vm->prog.instructions[0]));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR : can't read from file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    vm->prog.instruction_size = fread(vm->prog.instructions, sizeof(vm->prog.instructions[0]), m / sizeof(vm->prog.instructions[0]), f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR : can't write to file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }

    fclose(f);
}
/*
Instruction processLine(String line)
{
    String line = ltrim(line);
    Opcode op = cstrAsOpcode(splitStr(line, ' '));
    if (op > PSH) {
        String line = ltrim(line);
        Word operand = cstrToWord(splitStr(line, ' '));
        return MAKE_INST_ARG(op, operand);
    }
    return MAKE_INST(op);
}
Program translate_asm(String src)
{
    Instruction ins[src.parts] = { 0 };
    size_t cnt = src.parts;
    while (cnt > 0) {
        String line = splitStr(&src, '\n');
        processLine(String line);
        Instruction ins[src.parts - cnt] = processLine(line);
    }

    return;
}
*/