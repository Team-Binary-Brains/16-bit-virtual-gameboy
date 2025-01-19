#include "gbvm_instructions.h"
#include "gbvm_flags.h"
#include "gbvm_alu.h"

typedef struct {
    Opcode type;
    size_t size;
    char* name;
} OpcodeString;

static OpcodeString OpcodeStringMap[] = {
    { NOP, 3, "NOP" },
    { PSH, 3, "PSH" },
    { POP, 3, "POP" },
    { DUP, 3, "DUP" },
    { ADD, 3, "ADD" },
    { SUB, 3, "SUB" },
    { MUL, 3, "MUL" },
    { DIV, 3, "DIV" },
    { JMP, 3, "JMP" },
    { JNZ, 3, "JNZ" },
    { JIP, 3, "JIP" },
    { EQL, 3, "EQL" },
    { HLT, 3, "HLT" },
};

String opcodeAsStr(const Opcode* type)
{
    size_t len = sizeof(OpcodeStringMap) / sizeof(OpcodeStringMap[0]);

    for (size_t i = 0; i < len; ++i) {
        if ((*type) == OpcodeStringMap[i].type) {
            return (String) { .data = OpcodeStringMap[i].name, .length = OpcodeStringMap[i].size };
        }
    }

    assert(0 && "opcodeAsStr : Unreachable");
}

Opcode strAsOpcode(const String* s)
{
    size_t len = sizeof(OpcodeStringMap) / sizeof(OpcodeStringMap[0]);

    for (size_t i = 0; i < len; ++i) {

        if (s->length != OpcodeStringMap[i].size) {
            continue;
        }
        if (strncmp(s->data, OpcodeStringMap[i].name, s->length) == 0) {
            // printf("Operation : %.*s\n", (int)OpcodeStringMap[i].size, OpcodeStringMap[i].name);
            return OpcodeStringMap[i].type;
        }
    }

    displayStringMessageError("Unknown instruction detected and was ignored", *s);
    return NOP;
}

Error executeInst(const Program* prog, Memory* mem, CPU* cpu)
{
    if (cpu->registers.IP < 0 || cpu->registers.IP >= prog->instruction_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }

    Instruction inst = prog->instructions[cpu->registers.IP];

    Error err = 0;

    switch (inst.type) {

    case NOP:
        break;

    case POP:
        err = __pop(&(cpu->registers), mem);
        break;

    case PSH:
        err = __psh(&(cpu->registers), mem, &(inst.operand));
        break;

    case DUP:
        err = __dup(&(cpu->registers), mem, &(inst.operand));
        break;

    case ADD:
        err = performArithmeticOperation(cpu, mem, ADDITION);
        break;

    case SUB:
        err = performArithmeticOperation(cpu, mem, SUBTRACT);
        break;

    case MUL:
        err = performArithmeticOperation(cpu, mem, MULTIPLY);
        break;

    case DIV:
        err = performArithmeticOperation(cpu, mem, DIVIDE);
        break;

    case EQL:
        err = __eql(&(cpu->registers), mem);
        break;

    case HLT:
        setHalt(cpu, true);
        break;

    case JIP:
        if (cpu->registers.SP < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        if (getSign(cpu)) {
            break;
        }
        cpu->registers.IP = inst.operand;
        return ERR_OK;

    case JNZ:
        if (cpu->registers.SP < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        if (getZero(cpu)) {
            break;
        }
        cpu->registers.IP = inst.operand;
        return ERR_OK;

    case JMP:
        cpu->registers.IP = inst.operand;
        return ERR_OK;

    default:
        return ERR_ILLEGAL_INST;
    }

    if (err) {
        return err;
    }

    cpu->registers.IP += 1;
    return ERR_OK;
}

Error __psh(Registers* r, Memory* mem, const Word* operand)
{
    if (r->SP >= STACK_CAPACITY) {
        return ERR_STACK_OVERFLOW;
    }
    mem->stack[r->SP++] = *operand;
    return 0;
}

Error __eql(Registers* r, Memory* mem)
{
    if (r->SP < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem->stack[r->SP - 2] = mem->stack[r->SP - 1] == mem->stack[r->SP - 2];
    r->SP -= 1;
    return 0;
}

Error __pop(Registers* r, Memory* mem)
{
    if (r->SP < 1) {
        return ERR_STACK_UNDERFLOW;
    }
    printf("%d\n", mem->stack[r->SP - 1]);
    r->SP -= 1;
    return 0;
}

Error __dup(Registers* r, Memory* mem, const Word* operand)
{
    if (r->SP >= STACK_CAPACITY) {
        return ERR_STACK_OVERFLOW;
    }
    if ((r->SP - (*operand)) < 0) {
        return ERR_STACK_UNDERFLOW;
    }
    if ((*operand) < 0) {
        return ERR_ILLEGAL_OPERAND;
    }
    mem->stack[r->SP] = mem->stack[r->SP - (*operand)];
    r->SP += 1;
    return 0;
}