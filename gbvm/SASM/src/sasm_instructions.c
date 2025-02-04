#include "sasm_instructions.h"
#include "sasm_flags.h"
#include "sasm_alu.h"
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
    { DONOP, 5, "DONOP" },
    { CLRCF, 5, "CLRCF" },
    { TGLCF, 5, "TGLCF" },
    { SHUTS, 5, "SHUTS" },
    { DECR, 4, "DECR" },
    { NEG, 3, "NEG" },
    { GOTO, 4, "GOTO" },
    { JA, 2, "JA" },
    { JAE, 3, "JAE" },
    { CPY, 3, "CPY" },
    { ADC, 3, "ADC" },
    { ADD, 3, "ADD" },
    { AND, 3, "AND" },

    { AAA, 3, "AAA" },
    { AAD, 3, "AAD" },
    { AAM, 3, "AAM" },
    { AAS, 3, "AAS" },
    { CALL, 4, "CALL" },
    { CBW, 3, "CBW" },
    { CLD, 3, "CLD" },
    { CLI, 3, "CLI" },
    { CMP, 3, "CMP" },
    { CMPSB, 5, "CMPSB" },
    { CMPSW, 5, "CMPSW" },
    { CWD, 3, "CWD" },
    { DAA, 3, "DAA" },
    { DAS, 3, "DAS" },
    { DIV, 3, "DIV" },
    { IDIV, 4, "IDIV" },
    { IMUL, 4, "IMUL" },
    { IN, 2, "IN" },
    { INC, 3, "INC" },
    { INT, 3, "INT" },
    { INTO, 4, "INTO" },
    { IRET, 4, "IRET" },
    { JB, 2, "JB" },
    { JBE, 3, "JBE" },
    { JC, 2, "JC" },
    { JCXZ, 4, "JCXZ" },
    { JE, 2, "JE" },
    { JG, 2, "JG" },
    { JGE, 3, "JGE" },
    { JL, 2, "JL" },
    { JLE, 3, "JLE" },
    { JNA, 3, "JNA" },
    { JNAE, 4, "JNAE" },
    { JNB, 3, "JNB" },
    { JNBE, 4, "JNBE" },
    { JNC, 3, "JNC" },
    { JNE, 3, "JNE" },
    { JNG, 3, "JNG" },
    { JNGE, 4, "JNGE" },
    { JNL, 3, "JNL" },
    { JNLE, 4, "JNLE" },
    { JNO, 3, "JNO" },
    { JNP, 3, "JNP" },
    { JNS, 3, "JNS" },
    { JNZ, 3, "JNZ" },
    { JO, 2, "JO" },
    { JP, 2, "JP" },
    { JPE, 3, "JPE" },
    { JPO, 3, "JPO" },
    { JS, 2, "JS" },
    { JZ, 2, "JZ" },
    { LAHF, 4, "LAHF" },
    { LDS, 3, "LDS" },
    { LEA, 3, "LEA" },
    { LES, 3, "LES" },
    { LODSB, 5, "LODSB" },
    { LODSW, 5, "LODSW" },
    { LOOP, 4, "LOOP" },
    { LOOPE, 5, "LOOPE" },
    { LOOPNE, 6, "LOOPNE" },
    { LOOPNZ, 6, "LOOPNZ" },
    { LOOPZ, 5, "LOOPZ" },
    { MOVSB, 5, "MOVSB" },
    { MOVSW, 5, "MOVSW" },
    { MUL, 3, "MUL" },
    { NOT, 3, "NOT" },
    { OR, 2, "OR" },
    { OUT, 3, "OUT" },
    { POP, 3, "POP" },
    { POPA, 4, "POPA" },
    { POPF, 4, "POPF" },
    { PUSH, 4, "PUSH" },
    { PUSHA, 5, "PUSHA" },
    { PUSHF, 5, "PUSHF" },
    { RCL, 3, "RCL" },
    { RCR, 3, "RCR" },
    { REP, 3, "REP" },
    { REPE, 4, "REPE" },
    { REPNE, 5, "REPNE" },
    { REPNZ, 5, "REPNZ" },
    { REPZ, 4, "REPZ" },
    { RET, 3, "RET" },
    { RETF, 4, "RETF" },
    { ROL, 3, "ROL" },
    { ROR, 3, "ROR" },
    { SAHF, 4, "SAHF" },
    { SAL, 3, "SAL" },
    { SAR, 3, "SAR" },
    { SBB, 3, "SBB" },
    { SCASB, 5, "SCASB" },
    { SCASW, 5, "SCASW" },
    { SHL, 3, "SHL" },
    { SHR, 3, "SHR" },
    { STC, 3, "STC" },
    { STD, 3, "STD" },
    { STI, 3, "STI" },
    { STOSB, 5, "STOSB" },
    { STOSW, 5, "STOSW" },
    { SUB, 3, "SUB" },
    { TEST, 4, "TEST" },
    { XCHG, 4, "XCHG" },
    { XLATB, 5, "XLATB" },
    { XOR, 3, "XOR" }
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
    __AAA, __AAD, __AAM, __AAS, __CALL, __CBW,
    __CLD, __CLI, __CMP, __CMPSB, __CMPSW, __CWD, __DAA, __DAS,
    __DIV, __IDIV, __IMUL, __IN, __INC, __INT, __INTO, __IRET,
    __JB, __JBE, __JC, __JCXZ, __JE, __JG, __JGE, __JL, __JLE,
    __JNA, __JNAE, __JNB, __JNBE, __JNC, __JNE, __JNG, __JNGE, __JNL,
    __JNLE, __JNO, __JNP, __JNS, __JNZ, __JO, __JP, __JPE, __JPO, __JS,
    __JZ, __LAHF, __LDS, __LEA, __LES, __LODSB, __LODSW, __LOOP, __LOOPE,
    __LOOPNE, __LOOPNZ, __LOOPZ, __MOVSB, __MOVSW, __MUL, __NOT,
    __OR, __OUT, __POP, __POPA, __POPF, __PUSH, __PUSHA, __PUSHF, __RCL, __RCR,
    __REP, __REPE, __REPNE, __REPNZ, __REPZ, __RET, __RETF, __ROL, __ROR, __SAHF,
    __SAL, __SAR, __SBB, __SCASB, __SCASW, __SHL, __SHR, __STC, __STD, __STI,
    __STOSB, __STOSW, __SUB, __TEST, __XCHG, __XLATB, __XOR,

    __DONOP, __CLRCF, __TGLCF, __SHUTS,
    __DECR, __NEG, __GOTO, __JA, __JAE,
    __CPY, __ADC, __ADD,
    __AND
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
