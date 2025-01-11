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
#define MAKE_INST_NOP() { .type = INST_NOP }

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

const char* error_as_cstr(Error);

const char* inst_type_as_cstr(Inst_Type);

Error vm_execute_inst(Vm*);

void vm_load_program_from_memory(Vm*, Inst*, size_t);

void vm_dump_stack(FILE*, const Vm*);

void vm_save_program_to_file(Inst*, size_t, const char*);

void vm_load_program_from_file(Vm*, const char*);

size_t vm_translate_asm(char*, size_t, Inst*, size_t);
