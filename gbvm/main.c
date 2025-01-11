#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VM_STACK_CAPACITY 1024
#define VM_PROGRAM_CAPACITY 1024
#define VM_EXECUTION_LIMIT 102
#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))
#define MAKE_INST_JMP(addr) { .type = INST_JMP, .operand = (addr) }
#define MAKE_INST_JNZ(addr) { .type = INST_JNZ, .operand = (addr) }
#define MAKE_INST_DUP(addr) { .type = INST_DUP, .operand = (addr) }
#define MAKE_INST_PSH(valu) { .type = INST_PSH, .operand = (valu) }
#define MAKE_INST_HLT() { .type = INST_HLT }
#define MAKE_INST_ADD() { .type = INST_ADD }
#define MAKE_INST_SUB() { .type = INST_SUB }
#define MAKE_INST_MUL() { .type = INST_MUL }
#define MAKE_INST_DIV() { .type = INST_DIV }
#define MAKE_INST_POP() { .type = INST_POP }
#define MAKE_INST_EQL() { .type = INST_EQL }

typedef int8_t Byte;
typedef int16_t Word;
typedef int32_t Double_Word;
typedef int64_t Quad_Word;

typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_DIV_BY_ZERO,
    ERR_ILLEGAL_INST,
    ERR_ILLEGAL_INST_ACCESS,
    ERR_ILLEGAL_OPERAND
} Error;

typedef enum {
    INST_NOP = 0,
    INST_PSH,
    INST_POP,
    INST_DUP,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_JMP,
    INST_JNZ,
    INST_EQL,
    INST_HLT,
} Inst_Type;

typedef struct {
    Inst_Type type;
    Quad_Word operand;
} Inst;

typedef struct {
    Quad_Word stack[VM_STACK_CAPACITY];
    Quad_Word stack_size;

    Inst program[VM_PROGRAM_CAPACITY];
    Quad_Word program_size;
    Quad_Word IP;

    int halt;
} Vm;

const char* error_as_cstr(Error error)
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
const char* inst_type_as_cstr(Inst_Type type)
{
    switch (type) {
    case INST_PSH:
        return "INST_PSH";
    case INST_NOP:
        return "INST_NOP";
    case INST_POP:
        return "INST_POP";
    case INST_ADD:
        return "INST_ADD";
    case INST_SUB:
        return "INST_SUB";
    case INST_MUL:
        return "INST_MUL";
    case INST_DIV:
        return "INST_DIV";
    case INST_JMP:
        return "INST_JMP";
    case INST_JNZ:
        return "INST_JNZ";
    case INST_EQL:
        return "INST_EQL";
    case INST_HLT:
        return "INST_HLT";
    case INST_DUP:
        return "INST_DUP";
    default:
        assert(0 && "inst_type_as_cstr : Unreachable");
    }
}
Error vm_execute_inst(Vm* vm)
{
    if (vm->IP < 0 || vm->IP >= vm->program_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }
    Inst inst = vm->program[vm->IP];
    switch (inst.type) {
    case INST_PSH:
        if (vm->stack_size >= VM_STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }
        vm->stack[vm->stack_size++] = inst.operand;
        vm->IP += 1;
        break;

    case INST_ADD:
        if (vm->stack_size < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->stack_size - 2] += vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->IP += 1;
        break;

    case INST_SUB:
        if (vm->stack_size < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->stack_size - 2] -= vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->IP += 1;
        break;

    case INST_MUL:
        if (vm->stack_size < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->stack_size - 2] *= vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->IP += 1;
        break;

    case INST_DIV:
        if (vm->stack_size < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        if (vm->stack[vm->stack_size - 1] == 0) {
            return ERR_DIV_BY_ZERO;
        }
        vm->stack[vm->stack_size - 2] /= vm->stack[vm->stack_size - 1];
        vm->stack_size -= 1;
        vm->IP += 1;
        break;
    case INST_JMP:
        vm->IP = inst.operand;
        break;
    case INST_EQL:
        if (vm->stack_size < 2) {
            return ERR_STACK_UNDERFLOW;
        }
        vm->stack[vm->stack_size - 2] = vm->stack[vm->stack_size - 1] == vm->stack[vm->stack_size - 2];
        vm->stack_size -= 1;
        vm->IP += 1;
        break;
    case INST_JNZ:
        if (vm->stack_size < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        if (vm->stack[vm->stack_size - 1]) {
            vm->stack_size -= 1;
            vm->IP = inst.operand;
        } else {
            vm->IP += 1;
        }
        break;

    case INST_HLT:
        vm->halt = 1;
        break;

    case INST_POP:
        if (vm->stack_size < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        printf("%ld\n", vm->stack[vm->stack_size - 1]);
        vm->stack_size -= 1;
        vm->IP += 1;
        break;
    case INST_DUP:
        if (vm->stack_size >= VM_STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }
        if (vm->stack_size - inst.operand < 0) {
            return ERR_STACK_UNDERFLOW;
        }
        if (inst.operand < 0) {
            return ERR_ILLEGAL_OPERAND;
        }
        vm->stack[vm->stack_size] = vm->stack[vm->stack_size - (inst.operand)];
        vm->stack_size += 1;
        vm->IP += 1;
        break;
    case INST_NOP:
        vm->IP += 1;
        break;
    default:
        return ERR_ILLEGAL_INST;
    }
    return ERR_OK;
}
void vm_load_program_from_memory(Vm* vm, Inst* program, size_t program_size)
{
    assert(program_size < VM_PROGRAM_CAPACITY);
    memcpy(vm->program, program, sizeof(program[0]) * program_size);
    vm->program_size = program_size;
}
void vm_dump_stack(FILE* stream, const Vm* vm)
{
    fprintf(stream, "Stack : \n");
    if (vm->stack_size > 0) {
        for (Quad_Word i = 0; i < vm->stack_size; ++i) {
            fprintf(stream, "  %ld\n", vm->stack[i]);
        }
    } else {
        fprintf(stream, " [Empty]\n");
    }
}
void vm_save_program_to_file(Inst* program, size_t program_size,
    const char* file_path)
{
    FILE* f = fopen(file_path, "wb");
    if (!f) {
        fprintf(stderr, "ERROR : can't open file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    fwrite(program, sizeof(program[0]), program_size, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR : can't write to file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    fclose(f);
}
void vm_load_program_from_file(Vm* vm, const char* file_path)
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
    assert(m % sizeof(vm->program[0]) == 0);
    assert((size_t)m <= VM_PROGRAM_CAPACITY * sizeof(vm->program[0]));

    if (fseek(f, 0, SEEK_SET) < 0) {
        fprintf(stderr, "ERROR : can't read from file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }
    vm->program_size = fread(vm->program, sizeof(vm->program[0]), m / sizeof(vm->program[0]), f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR : can't write to file %s : %s\n", file_path,
            strerror(errno));
        exit(1);
    }

    fclose(f);
}

size_t vm_translate_asm(char* src, size_t src_size, Inst* program,
    size_t program_capacity)
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

Vm vm = { 0 };

char* srcCode = "PSH 0\n"
                "PSH 1\n"
                "DUP 2\n"
                "DUP 2\n"
                "ADD\n"
                "JMP 2\n"
                "HLT\n";

int main()
{
    // vm_save_program_to_file(program, sizeof(program), "./fibonacci.pbl");
    //  vm_load_program_from_memory(&vm, program, ARRAY_SIZE(program));
    vm_load_program_from_file(&vm, "./fibonacci.pbl");

    vm.program_size = vm_translate_asm(srcCode, strlen(srcCode), vm.program,
        VM_PROGRAM_CAPACITY);

    vm_dump_stack(stdout, &vm);
    for (int i = 0; i < VM_EXECUTION_LIMIT && !vm.halt; i++) {
        printf("%s\n", inst_type_as_cstr(vm.program[vm.IP].type));
        Error error = vm_execute_inst(&vm);

        if (error != ERR_OK) {
            fprintf(stderr, "Error : %s\n", error_as_cstr(error));
            exit(1);
        }
    }
    vm_dump_stack(stdout, &vm);
    return 0;
}