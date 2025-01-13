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

Error __psh(Memory*, Word);

Error __add(Memory*);

Error __sub(Memory*);

Error __mul(Memory*);

Error __div(Memory*);

Error __eql(Memory*);

Error __pop(Memory*);

Error __dup(Memory*, Word);

Error executeInst(const Program*, Memory*, CPU*);

String opcodeAsStr(Opcode);

Opcode strAsOpcode(String);