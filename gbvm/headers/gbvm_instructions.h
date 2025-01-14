#pragma once

#include "external_includes.h"
#include "gbvm_errors.h"
#include "gbvm_memory.h"
#include "gbvm_strings.h"

#define MAKE_INST_ARG(typ, opr) { .type = (typ), .operand = (opr) }
#define MAKE_INST(typ) { .type = (typ) }

typedef enum {
    NOP = 0,
    HLT,
    ADD,
    SUB,
    MUL,
    DIV,
    EQL,
    POP,
    PSH,
    DUP,
    JMP,
    JNZ,
    JIP,
} Opcode;

typedef struct {
    Opcode type;
    size_t size;
    char* name;
} OpcodeString;

typedef struct {
    Opcode type;
    Word operand;
} Instruction;

typedef struct {
    Instruction instructions[PROGRAM_CAPACITY];
    Word instruction_count;
    Word instruction_size;
} Program;

Error __psh(Registers*, Memory*, const Word*);

Error __add(Registers*, Memory*);

Error __sub(Registers*, Memory*);

Error __mul(Registers*, Memory*);

Error __div(Registers*, Memory*);

Error __eql(Registers*, Memory*);

Error __pop(Registers*, Memory*);

Error __dup(Registers*, Memory*, const Word*);

Error executeInst(const Program*, Memory*, CPU*);

String opcodeAsStr(const Opcode*);

Opcode strAsOpcode(const String*);