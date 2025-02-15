#include "sasm_instructions.h"
#include "sasm_memory.h"
#include "univ_defs.h"
typedef struct {
    Opcode type;
    size_t size;
    char* name;
} OpcodeString;

/*
TODO CHANGE THIS IMPLEMENTATION FOR NEW INSTRUCTION SET
*/
static OpcodeString OpcodeStringMap[] = {
    { DONOP, 5, "DONOP" }, { CLRCF, 5, "CLRCF" }, { TGLCF, 5, "TGLCF" }, { SHUTS, 5, "SHUTS" },

    { DECR, 4, "DECR" }, { NEG, 3, "NEG" }, { GOTO, 4, "GOTO" }, { JA, 2, "JA    " }, { JA, 4, "JNBE" },
    { JNC, 3, "JAE  " }, { JNC, 3, "JNC" }, { JNC, 3, "JNB  " }, { JC, 2, "JB    " }, { JC, 2, "JC  " },
    { JC, 4, "JNAE  " }, { JBE, 3, "JBE" }, { JBE, 3, "JNA  " }, { JCXZ, 4, "JCXZ" }, { JZ, 2, "JE  " },
    { JZ, 2, "JZ    " }, { JG, 2, "JG  " }, { JG, 4, "JNLE  " }, { JGE, 3, "JGE  " }, { JGE, 3, "JNL" },
    { JL, 2, "JL    " }, { JL, 4, "JNGE" }, { JLE, 3, "JLE  " }, { JLE, 3, "JNG  " }, { JNZ, 3, "JNE" },
    { JNZ, 3, "JNZ  " }, { JNO, 3, "JNO" }, { JNP, 3, "JNP  " }, { JNP, 3, "JPO  " }, { JNS, 3, "JNS" },
    { JO, 2, "JO    " }, { JP, 2, "JP  " }, { JP, 3, "JPE   " }, { JS, 2, "JS    " },

    { CPY, 3, "CPY" }, { ADC, 3, "ADC" }, { ADD, 3, "ADD" }, { AND, 3, "AND" }
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
    return DONOP;
}

/*
TODO CHANGE THIS IMPLEMENTATION FOR NEW INSTRUCTION SET
*/

Error (*instructionFuncPtrs[])(CPU* cpu, Memory* mem, Word* operand1, Word* operand2) = {
    __DONOP, __CLRCF, __TGLCF, __SHUTS,
    __DECR, __NEG, __GOTO, __JA, __JNC, __JC, __JBE, __JCXZ, __JZ, __JG,
    __JGE, __JL, __JLE, __JNZ, __JNO, __JNP, __JNS, __JO, __JP, __JS,
    __CPY, __ADC, __ADD, __AND
};
Error executeInst(const Program* prog, Memory* mem, CPU* cpu)
{
    if (cpu->registers.IP < 0 || cpu->registers.IP >= prog->instruction_size) {
        return ERR_ILLEGAL_INST_ACCESS;
    }
    Instruction inst = prog->instructions[cpu->registers.IP];

    Word *operand1 = NULL, *operand2 = NULL;
    if (inst.registerMode & o2_imm) {

        operand2 = &inst.operand2;
        // printf("\nSRC Data : %d \tMode : Immediate\n", *operand2);

    } else if (inst.registerMode & o2_mem) {

        operand2 = &(mem->stack[inst.operand2]);
        // printf("\nSRC Data : %d \tMode : Memory address [%d]\n", *operand2, inst.operand2);

    } else if (inst.registerMode & o2_reg) {

        switch (inst.operand2) {
        case 1:
            operand2 = &(cpu->registers.AX.full);
            // printf("\nSRC Data : %d \tMode : Register AX\n", *operand2);
            break;
        case 2:
            operand2 = &(cpu->registers.BX.full);
            // printf("\nSRC Data : %d \tMode : Register BX\n", *operand2);
            break;
        case 3:
            operand2 = &(cpu->registers.CX.full);
            // printf("\nSRC Data : %d \tMode : Register CX\n", *operand2);
            break;
        case 4:
            operand2 = &(cpu->registers.DX.full);
            // printf("\nSRC Data : %d \tMode : Register DX\n", *operand2);
            break;
        }
    }

    if (inst.registerMode & o1_mem) {

        operand1 = &(mem->stack[inst.operand]);
        // printf("DST Data : %d \tMode : Memory address [%d]\n", *operand1, inst.operand);

    } else if (inst.registerMode & o1_reg) {

        switch (inst.operand) {

        case 1:
            operand1 = &(cpu->registers.AX.full);
            // printf("DST Data : %d \tMode : Register AX\n", cpu->registers.AX.full);
            break;
        case 2:
            operand1 = &(cpu->registers.BX.full);
            // printf("DST Data : %d \tMode : Register BX\n", cpu->registers.BX.full);
            break;
        case 3:
            operand1 = &(cpu->registers.CX.full);
            // printf("DST Data : %d \tMode : Register CX\n", cpu->registers.CX.full);
            break;
        case 4:
            operand1 = &(cpu->registers.DX.full);
            // printf("DST Data : %d \tMode : Register DX\n", cpu->registers.DX.full);
            break;
        }
    } else {
        operand1 = &inst.operand;
    }

    Error err = instructionFuncPtrs[inst.type](cpu, mem, operand1, operand2);

    if (err) {
        return err;
    }

    cpu->registers.IP += 1;
    return ERR_OK;
}

Error __DONOP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nDO No OPeration\n");
    return ERR_OK;
}

Error __CLRCF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nBefore : %d", getFlag(CARRY, cpu));
    setFlag(CARRY, cpu, false);
    printf("\nAfter  : %d\n", getFlag(CARRY, cpu));
    return ERR_OK;
}

Error __TGLCF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nBefore : %d", getFlag(CARRY, cpu));
    setFlag(CARRY, cpu, !getFlag(CARRY, cpu));
    printf("\nAfter  : %d\n", getFlag(CARRY, cpu));
    return ERR_OK;
}

Error __SHUTS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nSHUTing down System\n");
    setFlag(HALT, cpu, true);
    return ERR_OK;
}

Error __DECR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nBefore : %d", *operand1);

    bool overflow = *operand1 == (-MAX_WORD);
    bool auxiliary = (((*operand1 & 0x0F) + (-1 & 0x0f)) > 0x0f);
    *operand1 = *operand1 + -1;

    setFlag(OVERFLOW, cpu, overflow);
    setFlag(ZERO, cpu, (*operand1 == 0));
    setFlag(SIGN, cpu, (*operand1 < 0));
    setFlag(AUX, cpu, auxiliary);
    checkAndSetParity(cpu, *operand1);

    printf("\nAfter  : %d\n", *operand1);
    return ERR_OK;
}

Error __NEG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("CALLED __NEG\n");
    return ERR_OK;
}

Error __GOTO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nUnconditional Jump to %d\n", *operand1);
    cpu->registers.IP = *operand1 - 1;
    return ERR_OK;
}

Error __JA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (!getFlag(CARRY, cpu) && !getFlag(ZERO, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JA / __JNBE \n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JNC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (!getFlag(CARRY, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JAE / __JNB / __JNC\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(CARRY, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JB /__JNAE / __JC\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JBE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(CARRY, cpu) || getFlag(ZERO, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JBE / __JNA\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JCXZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (cpu->registers.CX.full == 0) {
        printf("\nJUMPING TO %d\nREASON : __JCXZ\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(ZERO, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JE / __JZ\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(SIGN, cpu) == getFlag(OVERFLOW, cpu) && !getFlag(ZERO, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JG / __JNLE\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JGE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(SIGN, cpu) == getFlag(OVERFLOW, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JGE / __JNL\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(SIGN, cpu) != getFlag(OVERFLOW, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JL / __JNGE\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JLE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(SIGN, cpu) != getFlag(OVERFLOW, cpu) || getFlag(ZERO, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JLE / __JNG\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JNZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (!getFlag(ZERO, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JNE / __JNZ\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JNO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (!getFlag(OVERFLOW, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JNO\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JNP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (!getFlag(PARITY, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JNP / __JPO\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JNS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (!getFlag(SIGN, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JNS\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(OVERFLOW, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JO\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(PARITY, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JP / __JPE\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __JS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    if (getFlag(SIGN, cpu)) {
        printf("\nJUMPING TO %d\nREASON : __JS\n", *operand1);
        cpu->registers.IP = *operand1 - 1;
    }
    return ERR_OK;
}

Error __CPY(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nBefore : %d, %d", *operand1, *operand2);
    *operand1 = *operand2;
    printf("\nAfter  : %d\n", *operand1);
    return ERR_OK;
}

Error __ADC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nBefore : %d, %d", *operand1, *operand2);

    bool overflow = (*operand2 > 0 && *operand1 > (MAX_WORD - *operand2));
    bool auxiliary = (((*operand1 & 0x0F) + (*operand2 & 0x0f)) > 0x0f);
    *operand1 = *operand1 + *operand2;
    *operand1 = *operand1 + (getFlag(CARRY, cpu) ? 1 : 0);

    setFlag(CARRY, cpu, overflow);
    setFlag(OVERFLOW, cpu, overflow);
    setFlag(ZERO, cpu, (*operand1 == 0));
    setFlag(SIGN, cpu, (*operand1 < 0));
    setFlag(AUX, cpu, auxiliary);
    checkAndSetParity(cpu, *operand1);

    printf("\nAfter  : %d\n", *operand1);
    return ERR_OK;
}

Error __ADD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nBefore : %d, %d", *operand1, *operand2);

    bool overflow = (*operand2 > 0 && *operand1 > (MAX_WORD - *operand2));
    bool auxiliary = (((*operand1 & 0x0F) + (*operand2 & 0x0f)) > 0x0f);
    *operand1 = *operand1 + *operand2;

    setFlag(CARRY, cpu, overflow);
    setFlag(OVERFLOW, cpu, overflow);
    setFlag(ZERO, cpu, (*operand1 == 0));
    setFlag(SIGN, cpu, (*operand1 < 0));
    setFlag(AUX, cpu, auxiliary);
    checkAndSetParity(cpu, *operand1);

    printf("\nAfter  : %d\n", *operand1);
    return ERR_OK;
}

Error __AND(CPU* cpu, Memory* mem, Word* operand1, Word* operand2)
{
    printf("\nBefore : %d, %d", *operand1, *operand2);

    *operand1 = *operand1 & *operand2;

    setFlag(ZERO, cpu, (*operand1 == 0));
    setFlag(SIGN, cpu, (*operand1 < 0));
    checkAndSetParity(cpu, *operand1);

    printf("\nAfter  : %d\n", *operand1);
    return ERR_OK;
}
