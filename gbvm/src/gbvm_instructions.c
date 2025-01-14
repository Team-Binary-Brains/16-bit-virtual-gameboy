#include "gbvm_instructions.h"

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
    { EQL, 3, "EQL" },
    { HLT, 3, "HLT" }
};

String opcodeAsStr(const Opcode* type)
{
    for (size_t i = 0; i < sizeof(OpcodeStringMap) / sizeof(OpcodeStringMap[0]); ++i) {
        if ((*type) == OpcodeStringMap[i].type) {
            return (String) { .data = OpcodeStringMap[i].name, .length = OpcodeStringMap[i].size };
        }
    }
    assert(0 && "opcodeAsStr : Unreachable");
}

Opcode strAsOpcode(const String* s)
{
    for (size_t i = 0; i < sizeof(OpcodeStringMap) / sizeof(OpcodeStringMap[0]); ++i) {

        if (s->length != OpcodeStringMap[i].size || memcmp(s->data, OpcodeStringMap[i].name, s->length)) {
            continue;
        } else {
            // printf("Operation : %.*s\n", (int)OpcodeStringMap[i].size, OpcodeStringMap[i].name);
            return OpcodeStringMap[i].type;
        }
    }
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
        err = __pop(mem);
        break;

    case PSH:
        err = __psh(mem, &(inst.operand));
        break;

    case DUP:
        err = __dup(mem, &(inst.operand));
        break;

    case ADD:
        err = __add(mem);
        break;

    case SUB:
        err = __sub(mem);
        break;

    case MUL:
        err = __mul(mem);
        break;

    case DIV:
        err = __div(mem);
        break;

    case EQL:
        err = __eql(mem);
        break;

    case JMP:
        cpu->registers.IP = inst.operand;
        return ERR_OK;

    case JNZ:
        if (mem->stack_size < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        if (!(mem->stack[mem->stack_size - 1])) {
            break;
        }
        mem->stack_size -= 1;
        cpu->registers.IP = inst.operand;
        return ERR_OK;

    case HLT:
        cpu->halt = 1;
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

Error __psh(Memory* mem, const Word* operand)
{
    if (mem->stack_size >= STACK_CAPACITY) {
        return ERR_STACK_OVERFLOW;
    }
    mem->stack[mem->stack_size++] = *operand;
    return 0;
}

Error __add(Memory* mem)
{
    if (mem->stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem->stack[mem->stack_size - 2] += mem->stack[mem->stack_size - 1];
    mem->stack_size -= 1;
    return 0;
}

Error __sub(Memory* mem)
{
    if (mem->stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem->stack[mem->stack_size - 2] -= mem->stack[mem->stack_size - 1];
    mem->stack_size -= 1;
    return 0;
}

Error __mul(Memory* mem)
{
    if (mem->stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem->stack[mem->stack_size - 2] *= mem->stack[mem->stack_size - 1];
    mem->stack_size -= 1;
    return 0;
}

Error __div(Memory* mem)
{
    if (mem->stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    if (mem->stack[mem->stack_size - 1] == 0) {
        return ERR_DIV_BY_ZERO;
    }
    mem->stack[mem->stack_size - 2] /= mem->stack[mem->stack_size - 1];
    mem->stack_size -= 1;
    return 0;
}

Error __eql(Memory* mem)
{
    if (mem->stack_size < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    mem->stack[mem->stack_size - 2] = mem->stack[mem->stack_size - 1] == mem->stack[mem->stack_size - 2];
    mem->stack_size -= 1;
    return 0;
}

Error __pop(Memory* mem)
{
    if (mem->stack_size < 1) {
        return ERR_STACK_UNDERFLOW;
    }
    printf("%d\n", mem->stack[mem->stack_size - 1]);
    mem->stack_size -= 1;
    return 0;
}

Error __dup(Memory* mem, const Word* operand)
{
    if (mem->stack_size >= STACK_CAPACITY) {
        return ERR_STACK_OVERFLOW;
    }
    if ((mem->stack_size - (*operand)) < 0) {
        return ERR_STACK_UNDERFLOW;
    }
    if ((*operand) < 0) {
        return ERR_ILLEGAL_OPERAND;
    }
    mem->stack[mem->stack_size] = mem->stack[mem->stack_size - (*operand)];
    mem->stack_size += 1;
    return 0;
}