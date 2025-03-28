#include "sasm_instructions.h"
#include "univ_errors.h"

static OpcodeDetails OpcodeDetailsMap[NUMBER_OF_INSTS] = {
    [INST_DONOP] = { .type = INST_DONOP, .name = "DONOP", .has_operand = 0, .has_operand2 = 0 },
    [INST_RETRN] = { .type = INST_RETRN, .name = "RETRN", .has_operand = 0, .has_operand2 = 0 },
    [INST_CALLN] = { .type = INST_CALLN, .name = "CALLN", .has_operand = 1, .has_operand2 = 0 },
    [INST_CALLF] = { .type = INST_CALLF, .name = "CALLF", .has_operand = 1, .has_operand2 = 0 },
    [INST_SHUTS] = { .type = INST_SHUTS, .name = "SHUTS", .has_operand = 0, .has_operand2 = 0 },
    [INST_PUSH] = { .type = INST_PUSH, .name = "PUSH", .has_operand = 1, .has_operand2 = 0 },
    [INST_DROP] = { .type = INST_DROP, .name = "DROP", .has_operand = 0, .has_operand2 = 0 },
    [INST_SWAP] = { .type = INST_SWAP, .name = "SWAP", .has_operand = 1, .has_operand2 = 0 },
    [INST_ADDI] = { .type = INST_ADDI, .name = "ADDI", .has_operand = 0, .has_operand2 = 0 },
    [INST_SUBI] = { .type = INST_SUBI, .name = "SUBI", .has_operand = 0, .has_operand2 = 0 },
    [INST_MULI] = { .type = INST_MULI, .name = "MULI", .has_operand = 0, .has_operand2 = 0 },
    [INST_DIVI] = { .type = INST_DIVI, .name = "DIVI", .has_operand = 0, .has_operand2 = 0 },
    [INST_MODI] = { .type = INST_MODI, .name = "MODI", .has_operand = 0, .has_operand2 = 0 },
    [INST_MULU] = { .type = INST_MULU, .name = "MULU", .has_operand = 0, .has_operand2 = 0 },
    [INST_DIVU] = { .type = INST_DIVU, .name = "DIVU", .has_operand = 0, .has_operand2 = 0 },
    [INST_MODU] = { .type = INST_MODU, .name = "MODU", .has_operand = 0, .has_operand2 = 0 },
    [INST_ADDF] = { .type = INST_ADDF, .name = "ADDF", .has_operand = 0, .has_operand2 = 0 },
    [INST_SUBF] = { .type = INST_SUBF, .name = "SUBF", .has_operand = 0, .has_operand2 = 0 },
    [INST_MULF] = { .type = INST_MULF, .name = "MULF", .has_operand = 0, .has_operand2 = 0 },
    [INST_DIVF] = { .type = INST_DIVF, .name = "DIVF", .has_operand = 0, .has_operand2 = 0 },
    [INST_JMPU] = { .type = INST_JMPU, .name = "JMPU", .has_operand = 1, .has_operand2 = 0 },
    [INST_JMPC] = { .type = INST_JMPC, .name = "JMPC", .has_operand = 1, .has_operand2 = 0 },
    [INST_ANDB] = { .type = INST_ANDB, .name = "ANDB", .has_operand = 0, .has_operand2 = 0 },
    [INST_NOTB] = { .type = INST_NOTB, .name = "NOTB", .has_operand = 0, .has_operand2 = 0 },
    [INST_CPY] = { .type = INST_CPY, .name = "CPY", .has_operand = 1, .has_operand2 = 1 },
    [INST_DUP] = { .type = INST_DUP, .name = "DUP", .has_operand = 1, .has_operand2 = 0 },
    [INST_NOT] = { .type = INST_NOT, .name = "NOT", .has_operand = 0, .has_operand2 = 0 },
    [INST_EQI] = { .type = INST_EQI, .name = "EQI", .has_operand = 0, .has_operand2 = 0 },
    [INST_GEI] = { .type = INST_GEI, .name = "GEI", .has_operand = 0, .has_operand2 = 0 },
    [INST_GTI] = { .type = INST_GTI, .name = "GTI", .has_operand = 0, .has_operand2 = 0 },
    [INST_LEI] = { .type = INST_LEI, .name = "LEI", .has_operand = 0, .has_operand2 = 0 },
    [INST_LTI] = { .type = INST_LTI, .name = "LTI", .has_operand = 0, .has_operand2 = 0 },
    [INST_NEI] = { .type = INST_NEI, .name = "NEI", .has_operand = 0, .has_operand2 = 0 },
    [INST_EQU] = { .type = INST_EQU, .name = "EQU", .has_operand = 0, .has_operand2 = 0 },
    [INST_GEU] = { .type = INST_GEU, .name = "GEU", .has_operand = 0, .has_operand2 = 0 },
    [INST_GTU] = { .type = INST_GTU, .name = "GTU", .has_operand = 0, .has_operand2 = 0 },
    [INST_LEU] = { .type = INST_LEU, .name = "LEU", .has_operand = 0, .has_operand2 = 0 },
    [INST_LTU] = { .type = INST_LTU, .name = "LTU", .has_operand = 0, .has_operand2 = 0 },
    [INST_NEU] = { .type = INST_NEU, .name = "NEU", .has_operand = 0, .has_operand2 = 0 },
    [INST_EQF] = { .type = INST_EQF, .name = "EQF", .has_operand = 0, .has_operand2 = 0 },
    [INST_GEF] = { .type = INST_GEF, .name = "GEF", .has_operand = 0, .has_operand2 = 0 },
    [INST_GTF] = { .type = INST_GTF, .name = "GTF", .has_operand = 0, .has_operand2 = 0 },
    [INST_LEF] = { .type = INST_LEF, .name = "LEF", .has_operand = 0, .has_operand2 = 0 },
    [INST_LTF] = { .type = INST_LTF, .name = "LTF", .has_operand = 0, .has_operand2 = 0 },
    [INST_NEF] = { .type = INST_NEF, .name = "NEF", .has_operand = 0, .has_operand2 = 0 },
    [INST_ORB] = { .type = INST_ORB, .name = "ORB", .has_operand = 0, .has_operand2 = 0 },
    [INST_XOR] = { .type = INST_XOR, .name = "XOR", .has_operand = 0, .has_operand2 = 0 },
    [INST_SHR] = { .type = INST_SHR, .name = "SHR", .has_operand = 0, .has_operand2 = 0 },
    [INST_SHL] = { .type = INST_SHL, .name = "SHL", .has_operand = 0, .has_operand2 = 0 },
    [INST_I2F] = { .type = INST_I2F, .name = "I2F", .has_operand = 0, .has_operand2 = 0 },
    [INST_U2F] = { .type = INST_U2F, .name = "U2F", .has_operand = 0, .has_operand2 = 0 },
    [INST_F2I] = { .type = INST_F2I, .name = "F2I", .has_operand = 0, .has_operand2 = 0 },
    [INST_F2U] = { .type = INST_F2U, .name = "F2U", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ1U] = { .type = INST_READ1U, .name = "READ1U", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ2U] = { .type = INST_READ2U, .name = "READ2U", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ4U] = { .type = INST_READ4U, .name = "READ4U", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ8U] = { .type = INST_READ8U, .name = "READ8U", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ1I] = { .type = INST_READ1I, .name = "READ1I", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ2I] = { .type = INST_READ2I, .name = "READ2I", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ4I] = { .type = INST_READ4I, .name = "READ4I", .has_operand = 0, .has_operand2 = 0 },
    [INST_READ8I] = { .type = INST_READ8I, .name = "READ8I", .has_operand = 0, .has_operand2 = 0 },
    [INST_WRITE1] = { .type = INST_WRITE1, .name = "WRITE1", .has_operand = 0, .has_operand2 = 0 },
    [INST_WRITE2] = { .type = INST_WRITE2, .name = "WRITE2", .has_operand = 0, .has_operand2 = 0 },
    [INST_WRITE4] = { .type = INST_WRITE4, .name = "WRITE4", .has_operand = 0, .has_operand2 = 0 },
    [INST_WRITE8] = { .type = INST_WRITE8, .name = "WRITE8", .has_operand = 0, .has_operand2 = 0 },
};

QuadWord quadword_u64(uint64_t u64)
{
    return (QuadWord) { .as_u64 = u64 };
}

QuadWord quadword_i64(int64_t i64)
{
    return (QuadWord) { .as_i64 = i64 };
}

QuadWord quadword_f64(double f64)
{
    return (QuadWord) { .as_f64 = f64 };
}

QuadWord quadword_ptr(void* ptr)
{
    return (QuadWord) { .as_ptr = ptr };
}

bool getOpcodeDetailsFromName(String name, OpcodeDetails* out_ptr)
{
    for (Opcode type = 0; type < NUMBER_OF_INSTS; type += 1) {
        if (compareStr(convertCstrToStr(OpcodeDetailsMap[type].name), name)) {
            *out_ptr = OpcodeDetailsMap[type];
            return 1;
        }
    }

    displayStringMessageError("Unknown instruction detected and was ignored", name);
    return 0;
}

OpcodeDetails getOpcodeDetails(Opcode type)
{
    assert(type < NUMBER_OF_INSTS);
    return OpcodeDetailsMap[type];
}
