#include "gbvm.h"

const char* errorAsCstr(Error error)
{
    switch (error) {
    case ERR_OK:
        return "ERR_OK";
    case ERR_STACK_OVERFLOW:
        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
        return "ERR_STACK_UNDERFLOW";
    case ERR_DIV_BY_ZERO:
        return "ERR_DIV_BY_ZERO";
    case ERR_ILLEGAL_INST:
        return "ERR_ILLEGAL_INST";
    case ERR_ILLEGAL_INST_ACCESS:
        return "ERR_ILLEGAL_INST_ACCESS";
    case ERR_ILLEGAL_OPERAND:
        return "ERR_ILLEGAL_OPERAND";
    default:
        assert(0 && "error_as_cstr : Unreachable");
    }
}

const char* OpcodeAsCstr(Opcode type)
{
    switch (type) {
    case PSH:
        return "PSH";
    case NOP:
        return "NOP";
    case POP:
        return "POP";
    case ADD:
        return "ADD";
    case SUB:
        return "SUB";
    case MUL:
        return "MUL";
    case DIV:
        return "DIV";
    case JMP:
        return "JMP";
    case JNZ:
        return "JNZ";
    case EQL:
        return "EQL";
    case HLT:
        return "HLT";
    case DUP:
        return "DUP";
    default:
        assert(0 && "Opcode_as_cstr : Unreachable");
    }
}

Error executeInst(Vm* vm)
{
    if (vm->cpu.registers.IP < 0 || vm->cpu.registers.IP >= vm->prog.program_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }
    Instruction inst = vm->prog.program[vm->cpu.registers.IP];
    switch (inst.type) {
    case PSH:
        __psh(vm->mem, inst.operand);
        vm->cpu.registers.IP += 1;
        break;

    case ADD:
        __add(vm->mem);
        vm->cpu.registers.IP += 1;
        break;

    case SUB:
        __sub(vm->mem);
        vm->cpu.registers.IP += 1;
        break;

    case MUL:
        __mul(vm->mem);
        vm->cpu.registers.IP += 1;
        break;

    case DIV:
        __div(vm->mem);
        vm->cpu.registers.IP += 1;
        break;
    case JMP:
        vm->cpu.registers.IP = inst.operand;
        break;
    case EQL:
        __eql(vm->mem);
        vm->cpu.registers.IP += 1;
        break;
    case JNZ:
        if (vm->mem.stack_size < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        if (vm->mem.stack[vm->mem.stack_size - 1]) {
            vm->mem.stack_size -= 1;
            vm->cpu.registers.IP = inst.operand;
        } else {
            vm->cpu.registers.IP += 1;
        }
        break;

    case HLT:
        vm->halt = 1;
        break;

    case POP:
        __pop(vm->mem);
        vm->cpu.registers.IP += 1;
        break;
    case DUP:
        if (vm->mem.stack_size >= STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }
        if (vm->mem.stack_size - inst.operand < 0) {
            return ERR_STACK_UNDERFLOW;
        }
        if (inst.operand < 0) {
            return ERR_ILLEGAL_OPERAND;
        }
        vm->mem.stack[vm->mem.stack_size] = vm->mem.stack[vm->mem.stack_size - (inst.operand)];
        vm->mem.stack_size += 1;
        vm->cpu.registers.IP += 1;
        break;
    case NOP:
        vm->cpu.registers.IP += 1;
        break;
    default:
        return ERR_ILLEGAL_INST;
    }
    return ERR_OK;
}

void loadProgram(Vm* vm, Program prog)
{
    assert(prog.program_size < PROGRAM_CAPACITY);
    memcpy(vm->prog.program, prog.program, prog.instruction_count * prog.program_size);
    vm->prog.program_size = prog.program_size;
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
    fwrite(prog.program, prog.program_size, prog.instruction_count, f);
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
    assert(m % sizeof(vm->prog.program[0]) == 0);
    assert((size_t)m <= PROGRAM_CAPACITY * sizeof(vm->prog.program[0]));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR : can't read from file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    vm->prog.program_size = fread(vm->prog.program, sizeof(vm->prog.program[0]), m / sizeof(vm->prog.program[0]), f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR : can't write to file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }

    fclose(f);
}

size_t translate_asm(char* src, size_t src_size, Program prog)
{
    while (src_size > 0) {
        char* end = memchr(src, '\n', src_size);
        size_t n = (end) ? (size_t)(end - src) : src_size;
        printf("%.*s\n", (int)n, src);
        src = end;
        src_size -= n;
        if (src) {
            src += 1;
            src_size -= 1;
        }
    }

    return 0;
}
