#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK_CAPACITY 1024
#define PROGRAM_CAPACITY 1024
#define EXECUTION_LIMIT 90
#define ARRAY_SIZE(xs) (sizeof(xs) / sizeof((xs)[0]))
#define MAKE_INST_ARG(typ, opr) { .type = (typ), .operand = (opr) }
#define MAKE_INST(typ) { .type = (typ) }

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
    NOP = 0,
    PSH,
    POP,
    DUP,
    ADD,
    SUB,
    MUL,
    DIV,
    JMP,
    JNZ,
    EQL,
    HLT,
} Opcode;

typedef struct {
    Opcode type;
    Word operand;
} Instruction;

typedef struct {
    Word stack[STACK_CAPACITY];
    Word stack_size;
} Memory;

typedef struct {
    Instruction program[PROGRAM_CAPACITY];
    Word instruction_count;
    Word program_size;
} Program;

typedef struct {
    Word AX;
    Word IP;
} Registers;

typedef struct {
    Registers registers;
} CPU;

typedef struct {
    Memory mem;
    Program prog;
    CPU cpu;

    int halt;
} Vm;

const char* errorAsCstr(Error);

const char* opcodeAsCstr(Opcode);

Error executeInst(Vm*);

void loadProgram(Vm*, Program prog);

void dumpStack(FILE*, const Vm*);

void assembleInstructions(Program prog, const char*);

void loadBytecode(Vm*, const char*);

size_t translate_asm(char*, size_t, Program prog);

Error __psh(Memory mem, Word operand)
{
    if (mem.stack_size >= STACK_CAPACITY) {
        return ERR_STACK_OVERFLOW;
    }
    mem.stack[mem.stack_size++] = operand;
    return 0;
}
Error __add(Memory mem)
{
    if (mem.stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem.stack[mem.stack_size - 2] += mem.stack[mem.stack_size - 1];
    mem.stack_size -= 1;
    return 0;
}

Error __sub(Memory mem)
{
    if (mem.stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem.stack[mem.stack_size - 2] -= mem.stack[mem.stack_size - 1];
    mem.stack_size -= 1;
    return 0;
}

Error __mul(Memory mem)
{
    if (mem.stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem.stack[mem.stack_size - 2] *= mem.stack[mem.stack_size - 1];
    mem.stack_size -= 1;
    return 0;
}

Error __div(Memory mem)
{
    if (mem.stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    if (mem.stack[mem.stack_size - 1] == 0) {
        return ERR_DIV_BY_ZERO;
    }
    mem.stack[mem.stack_size - 2] += mem.stack[mem.stack_size - 1];
    mem.stack_size -= 1;
    return 0;
}

Error __eql(Memory mem)
{
    if (mem.stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem.stack[mem.stack_size - 2] = mem.stack[mem.stack_size - 1] == mem.stack[mem.stack_size - 2];
    mem.stack_size -= 1;
    return 0;
}
Error __pop(Memory mem)
{
    if (mem.stack_size < 1) {
        return ERR_STACK_UNDERFLOW;
    }
    printf("%d\n", mem.stack[mem.stack_size - 1]);
    mem.stack_size -= 1;
}