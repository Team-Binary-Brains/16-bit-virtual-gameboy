/**
 * @file sasm_opcodes.h
 * @brief This file contains the instruction set used in the SASM assembler.
 *
 * @author Soham Metha
 * @date January 2025
 */
#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "univ_errors.h"
#include "univ_defs.h"
#include "sasm_memory.h"

/**
 * @brief Enumeration of opcodes for SASM instructions.
 */
typedef enum {
    AAA,
    AAD,
    AAM,
    AAS,
    CALL,
    CBW,
    CLD,
    CLI,
    CMP,
    CMPSB,
    CMPSW,
    CWD,
    DAA,
    DAS,
    DIV,
    IDIV,
    IMUL,
    IN,
    INC,
    INT,
    INTO,
    IRET,
    JB,
    JBE,
    JC,
    JCXZ,
    JE,
    JG,
    JGE,
    JL,
    JLE,
    JNA,
    JNAE,
    JNB,
    JNBE,
    JNC,
    JNE,
    JNG,
    JNGE,
    JNL,
    JNLE,
    JNO,
    JNP,
    JNS,
    JNZ,
    JO,
    JP,
    JPE,
    JPO,
    JS,
    JZ,
    LAHF,
    LDS,
    LEA,
    LES,
    LODSB,
    LODSW,
    LOOP,
    LOOPE,
    LOOPNE,
    LOOPNZ,
    LOOPZ,
    MOVSB,
    MOVSW,
    MUL,
    NOT,
    OR,
    OUT,
    POP,
    POPA,
    POPF,
    PUSH,
    PUSHA,
    PUSHF,
    RCL,
    RCR,
    REP,
    REPE,
    REPNE,
    REPNZ,
    REPZ,
    RET,
    RETF,
    ROL,
    ROR,
    SAHF,
    SAL,
    SAR,
    SBB,
    SCASB,
    SCASW,
    SHL,
    SHR,
    STC,
    STD,
    STI,
    STOSB,
    STOSW,
    SUB,
    TEST,
    XCHG,
    XLATB,
    XOR,

    DONOP,
    CLRCF,
    TGLCF,
    SHUTS,
    DECR,
    NEG,
    GOTO,
    JA,
    JAE,
    CPY,
    ADC,
    ADD,
    AND,
} Opcode;

/**
 * @brief Structure representing an instruction in the SASM assembly language.
 */
typedef struct {
    Opcode type;  /**< The opcode of the instruction */
    Word operand; /**< The operand of the instruction */
    Word operand2;
    // TODO : DISCARD USE OF THIS FLAG
    Byte registerMode;
} Instruction;

Error __AAA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AAD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AAM(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AAS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __ADC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __ADD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AND(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CALL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CBW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CLRCF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CLD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CLI(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __TGLCF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CMP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CMPSB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CMPSW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CWD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DAA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DAS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DECR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DIV(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SHUTS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IDIV(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IMUL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IN(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __INC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __INT(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __INTO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IRET(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JAE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JBE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JCXZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JGE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JLE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __GOTO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNAE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNBE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNGE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNLE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JNZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JPE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JPO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __JZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LAHF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LDS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LEA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LES(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LODSB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LODSW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LOOP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LOOPE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LOOPNE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LOOPNZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __LOOPZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CPY(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __MOVSB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __MOVSW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __MUL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __NEG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DONOP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __NOT(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __OR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __OUT(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __POP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __POPA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __POPF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __PUSH(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __PUSHA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __PUSHF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __RCL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __RCR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __REP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __REPE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __REPNE(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __REPNZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __REPZ(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __RET(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __RETF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __ROL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __ROR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SAHF(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SAL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SAR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SBB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SCASB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SCASW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SHL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SHR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __STC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __STD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __STI(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __STOSB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __STOSW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __SUB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __TEST(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __XCHG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __XLATB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __XOR(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
