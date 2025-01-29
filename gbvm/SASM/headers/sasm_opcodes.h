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
#include "sasm_memory.h"

/**
 * @brief Enumeration of opcodes for SASM instructions.
 */
typedef enum {
    AAA,
    AAD,
    AAM,
    AAS,
    ADC,
    ADD,
    AND,
    CALL,
    CBW,
    CLC,
    CLD,
    CLI,
    CMC,
    CMP,
    CMPSB,
    CMPSW,
    CWD,
    DAA,
    DAS,
    DEC,
    DIV,
    HLT,
    IDIV,
    IMUL,
    IN,
    INC,
    INT,
    INTO,
    IRET,
    JA,
    JAE,
    JB,
    JBE,
    JC,
    JCXZ,
    JE,
    JG,
    JGE,
    JL,
    JLE,
    JMP,
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
    NEG,
    NOP,
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

    MOV,
} Opcode;

Error __AAA(Registers* r, Memory* mem, const Word* offset);
Error __AAD(Registers* r, Memory* mem, const Word* offset);
Error __AAM(Registers* r, Memory* mem, const Word* offset);
Error __AAS(Registers* r, Memory* mem, const Word* offset);
Error __ADC(Registers* r, Memory* mem, const Word* offset);
Error __ADD(Registers* r, Memory* mem, const Word* offset);
Error __AND(Registers* r, Memory* mem, const Word* offset);
Error __CALL(Registers* r, Memory* mem, const Word* offset);
Error __CBW(Registers* r, Memory* mem, const Word* offset);
Error __CLC(Registers* r, Memory* mem, const Word* offset);
Error __CLD(Registers* r, Memory* mem, const Word* offset);
Error __CLI(Registers* r, Memory* mem, const Word* offset);
Error __CMC(Registers* r, Memory* mem, const Word* offset);
Error __CMP(Registers* r, Memory* mem, const Word* offset);
Error __CMPSB(Registers* r, Memory* mem, const Word* offset);
Error __CMPSW(Registers* r, Memory* mem, const Word* offset);
Error __CWD(Registers* r, Memory* mem, const Word* offset);
Error __DAA(Registers* r, Memory* mem, const Word* offset);
Error __DAS(Registers* r, Memory* mem, const Word* offset);
Error __DEC(Registers* r, Memory* mem, const Word* offset);
Error __DIV(Registers* r, Memory* mem, const Word* offset);
Error __HLT(Registers* r, Memory* mem, const Word* offset);
Error __IDIV(Registers* r, Memory* mem, const Word* offset);
Error __IMUL(Registers* r, Memory* mem, const Word* offset);
Error __IN(Registers* r, Memory* mem, const Word* offset);
Error __INC(Registers* r, Memory* mem, const Word* offset);
Error __INT(Registers* r, Memory* mem, const Word* offset);
Error __INTO(Registers* r, Memory* mem, const Word* offset);
Error __IRET(Registers* r, Memory* mem, const Word* offset);
Error __JA(Registers* r, Memory* mem, const Word* offset);
Error __JAE(Registers* r, Memory* mem, const Word* offset);
Error __JB(Registers* r, Memory* mem, const Word* offset);
Error __JBE(Registers* r, Memory* mem, const Word* offset);
Error __JC(Registers* r, Memory* mem, const Word* offset);
Error __JCXZ(Registers* r, Memory* mem, const Word* offset);
Error __JE(Registers* r, Memory* mem, const Word* offset);
Error __JG(Registers* r, Memory* mem, const Word* offset);
Error __JGE(Registers* r, Memory* mem, const Word* offset);
Error __JL(Registers* r, Memory* mem, const Word* offset);
Error __JLE(Registers* r, Memory* mem, const Word* offset);
Error __JMP(Registers* r, Memory* mem, const Word* offset);
Error __JNA(Registers* r, Memory* mem, const Word* offset);
Error __JNAE(Registers* r, Memory* mem, const Word* offset);
Error __JNB(Registers* r, Memory* mem, const Word* offset);
Error __JNBE(Registers* r, Memory* mem, const Word* offset);
Error __JNC(Registers* r, Memory* mem, const Word* offset);
Error __JNE(Registers* r, Memory* mem, const Word* offset);
Error __JNG(Registers* r, Memory* mem, const Word* offset);
Error __JNGE(Registers* r, Memory* mem, const Word* offset);
Error __JNL(Registers* r, Memory* mem, const Word* offset);
Error __JNLE(Registers* r, Memory* mem, const Word* offset);
Error __JNO(Registers* r, Memory* mem, const Word* offset);
Error __JNP(Registers* r, Memory* mem, const Word* offset);
Error __JNS(Registers* r, Memory* mem, const Word* offset);
Error __JNZ(Registers* r, Memory* mem, const Word* offset);
Error __JO(Registers* r, Memory* mem, const Word* offset);
Error __JP(Registers* r, Memory* mem, const Word* offset);
Error __JPE(Registers* r, Memory* mem, const Word* offset);
Error __JPO(Registers* r, Memory* mem, const Word* offset);
Error __JS(Registers* r, Memory* mem, const Word* offset);
Error __JZ(Registers* r, Memory* mem, const Word* offset);
Error __LAHF(Registers* r, Memory* mem, const Word* offset);
Error __LDS(Registers* r, Memory* mem, const Word* offset);
Error __LEA(Registers* r, Memory* mem, const Word* offset);
Error __LES(Registers* r, Memory* mem, const Word* offset);
Error __LODSB(Registers* r, Memory* mem, const Word* offset);
Error __LODSW(Registers* r, Memory* mem, const Word* offset);
Error __LOOP(Registers* r, Memory* mem, const Word* offset);
Error __LOOPE(Registers* r, Memory* mem, const Word* offset);
Error __LOOPNE(Registers* r, Memory* mem, const Word* offset);
Error __LOOPNZ(Registers* r, Memory* mem, const Word* offset);
Error __LOOPZ(Registers* r, Memory* mem, const Word* offset);
Error __MOV(Registers* r, Memory* mem, const Word* offset);
Error __MOVSB(Registers* r, Memory* mem, const Word* offset);
Error __MOVSW(Registers* r, Memory* mem, const Word* offset);
Error __MUL(Registers* r, Memory* mem, const Word* offset);
Error __NEG(Registers* r, Memory* mem, const Word* offset);
Error __NOP(Registers* r, Memory* mem, const Word* offset);
Error __NOT(Registers* r, Memory* mem, const Word* offset);
Error __OR(Registers* r, Memory* mem, const Word* offset);
Error __OUT(Registers* r, Memory* mem, const Word* offset);
Error __POP(Registers* r, Memory* mem, const Word* offset);
Error __POPA(Registers* r, Memory* mem, const Word* offset);
Error __POPF(Registers* r, Memory* mem, const Word* offset);
Error __PUSH(Registers* r, Memory* mem, const Word* offset);
Error __PUSHA(Registers* r, Memory* mem, const Word* offset);
Error __PUSHF(Registers* r, Memory* mem, const Word* offset);
Error __RCL(Registers* r, Memory* mem, const Word* offset);
Error __RCR(Registers* r, Memory* mem, const Word* offset);
Error __REP(Registers* r, Memory* mem, const Word* offset);
Error __REPE(Registers* r, Memory* mem, const Word* offset);
Error __REPNE(Registers* r, Memory* mem, const Word* offset);
Error __REPNZ(Registers* r, Memory* mem, const Word* offset);
Error __REPZ(Registers* r, Memory* mem, const Word* offset);
Error __RET(Registers* r, Memory* mem, const Word* offset);
Error __RETF(Registers* r, Memory* mem, const Word* offset);
Error __ROL(Registers* r, Memory* mem, const Word* offset);
Error __ROR(Registers* r, Memory* mem, const Word* offset);
Error __SAHF(Registers* r, Memory* mem, const Word* offset);
Error __SAL(Registers* r, Memory* mem, const Word* offset);
Error __SAR(Registers* r, Memory* mem, const Word* offset);
Error __SBB(Registers* r, Memory* mem, const Word* offset);
Error __SCASB(Registers* r, Memory* mem, const Word* offset);
Error __SCASW(Registers* r, Memory* mem, const Word* offset);
Error __SHL(Registers* r, Memory* mem, const Word* offset);
Error __SHR(Registers* r, Memory* mem, const Word* offset);
Error __STC(Registers* r, Memory* mem, const Word* offset);
Error __STD(Registers* r, Memory* mem, const Word* offset);
Error __STI(Registers* r, Memory* mem, const Word* offset);
Error __STOSB(Registers* r, Memory* mem, const Word* offset);
Error __STOSW(Registers* r, Memory* mem, const Word* offset);
Error __SUB(Registers* r, Memory* mem, const Word* offset);
Error __TEST(Registers* r, Memory* mem, const Word* offset);
Error __XCHG(Registers* r, Memory* mem, const Word* offset);
Error __XLATB(Registers* r, Memory* mem, const Word* offset);
Error __XOR(Registers* r, Memory* mem, const Word* offset);
