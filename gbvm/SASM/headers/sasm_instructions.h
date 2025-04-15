#include "univ_defs.h"

typedef enum {
    META_HALT = 1 << 0,
    META_F1 = 1 << 1,
    META_F2 = 1 << 2,
    META_F3 = 1 << 3,
    META_F4 = 1 << 4,
    META_F5 = 1 << 5,
    META_F6 = 1 << 6,
    META_F7 = 1 << 7
} Meta;

typedef enum {

    INST_DONOP = 0,
    INST_INVOK,
    INST_RETVL,
    INST_PUSHR,
    INST_SPOPR,
    INST_SHUTS,
    INST_SETR,
    INST_GETR,
    INST_CALL,
    INST_LOOP,
    INST_PUSH,
    INST_SPOP,
    INST_SWAP,
    INST_ADDI,
    INST_SUBI,
    INST_MULI,
    INST_DIVI,
    INST_MODI,
    INST_ADDU,
    INST_SUBU,
    INST_MULU,
    INST_DIVU,
    INST_MODU,
    INST_ADDF,
    INST_SUBF,
    INST_MULF,
    INST_DIVF,
    INST_JMPU,
    INST_JMPC,
    INST_ANDB,
    INST_NOTB,
    INST_COPY,
    INST_DUPS,
    INST_RET,
    INST_NOT,
    INST_EQI,
    INST_GEI,
    INST_GTI,
    INST_LEI,
    INST_LTI,
    INST_NEI,
    INST_EQU,
    INST_GEU,
    INST_GTU,
    INST_LEU,
    INST_LTU,
    INST_NEU,
    INST_EQF,
    INST_GEF,
    INST_GTF,
    INST_LEF,
    INST_LTF,
    INST_NEF,
    INST_ORB,
    INST_XOR,
    INST_SHR,
    INST_SHL,
    INST_I2F,
    INST_U2F,
    INST_F2I,
    INST_F2U,
    INST_READ1U,
    INST_READ2U,
    INST_READ4U,
    INST_READ8U,
    INST_READ1I,
    INST_READ2I,
    INST_READ4I,
    INST_READ8I,
    INST_WRITE1,
    INST_WRITE2,
    INST_WRITE4,
    INST_WRITE8,
    NUMBER_OF_INSTS
} Opcode;

typedef QuadWord Register;
typedef struct Instruction Instruction;
typedef struct OpcodeDetails OpcodeDetails;
typedef struct Program Program;
typedef struct CPU CPU;
typedef struct Memory Memory;
typedef union Registers Registers;

typedef enum {
    REG_H0,
    REG_H1,
    REG_I0,
    REG_I1,
    REG_JS,
    REG_KC,
    REG_L0,
    REG_L1,
    REG_L2,
    REG_L3,
    REG_NX,
    REG_OP,
    REG_P0,
    REG_P1,
    REG_P2,
    REG_P3,
    REG_QT,
    REG_RF,
    REG_SP,
    REG_COUNT,
} RegID;

struct Instruction {
    Opcode type;
    QuadWord operand;
    QuadWord operand2;
    bool opr1IsInline;
    bool opr2IsInline;
};

struct OpcodeDetails {
    Opcode type;
    const char* name;
    bool has_operand;
    bool has_operand2;
};

struct Program {
    Instruction instructions[PROGRAM_CAPACITY]; /**< The array of instructions */
    DataEntry instruction_count;                /**< The number of instructions in the program */
    DataEntry instruction_size;                 /**< The size of each instruction in bytes */
};

union Registers {
    struct
    {
        Register H0;
        Register H1;

        Register I0;
        Register I1;

        Register JS;
        Register KC;

        Register L0;
        Register L1;
        Register L2;
        Register L3;

        Register NX;
        Register OP;

        Register P0;
        Register P1;
        Register P2;
        Register P3;

        Register QT;
        Register RF;
        Register SP;
    };
    Register reg[REG_COUNT];
};

struct CPU {
    Registers registers;
    volatile short flags;
};

struct Memory {
    QuadWord stack[STACK_CAPACITY];
    Byte memory[MEMORY_CAPACITY];
};
bool getOpcodeDetailsFromName(String name, OpcodeDetails* outPtr);

OpcodeDetails getOpcodeDetails(Opcode type);

void setFlag(Meta f, CPU* cpu, bool state);

bool getFlag(Meta f, const CPU* cpu);
