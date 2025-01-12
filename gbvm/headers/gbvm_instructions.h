// #include "gbvm_memory.h"
#include "gbvm_errors.h"

#define PROGRAM_CAPACITY 1024

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
} Opcode;

typedef struct {
    Opcode type;
    const char* name;
} OpcodeString;

static OpcodeString OpcodeStringMap[] = {
    { NOP, "NOP" },
    { PSH, "PSH" },
    { POP, "POP" },
    { DUP, "DUP" },
    { ADD, "ADD" },
    { SUB, "SUB" },
    { MUL, "MUL" },
    { DIV, "DIV" },
    { JMP, "JMP" },
    { JNZ, "JNZ" },
    { EQL, "EQL" },
    { HLT, "HLT" }
};

typedef struct {
    Opcode type;
    Word operand;
} Instruction;

typedef struct {
    Instruction instructions[PROGRAM_CAPACITY];
    Word instruction_count;
    Word instruction_size;
} Program;

Error __psh(Memory*, Word);

Error __add(Memory*);

Error __sub(Memory*);

Error __mul(Memory*);

Error __div(Memory*);

Error __eql(Memory*);

Error __pop(Memory*);

Error __dup(Memory*, Word);

Error executeInst(const Program*, Memory*, CPU*);

const char* opcodeAsCstr(Opcode);

Opcode cstrAsOpcode(const char*);