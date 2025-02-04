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
} Opcode;

Error (*instructionFuncPtrs[])(CPU* cpu, Memory* mem, Word* operand1, Word* operand2) = {

    __AAA, __AAD, __AAM, __AAS, __CALL, __CBW, __CLD, __CLI, __CMP, __CMPSB,
    __CMPSW, __CWD, __DAA, __DAS, __DIV, __IDIV, __IMUL, __IN, __INC, __INT,
    __INTO, __IRET, __LAHF, __LDS, __LEA, __LES, __LODSB, __LODSW, __LOOP, __LOOPE,
    __LOOPNE, __LOOPNZ, __LOOPZ, __MOVSB, __MOVSW, __MUL, __NOT, __OR, __OUT, __POP,
    __POPA, __POPF, __PUSH, __PUSHA, __PUSHF, __RCL, __RCR, __REP, __REPE, __REPNE,
    __REPNZ, __REPZ, __RET, __RETF, __ROL, __ROR, __SAHF, __SAL, __SAR, __SBB,
    __SCASB, __SCASW, __SHL, __SHR, __STC, __STD, __STI, __STOSB, __STOSW, __SUB,
    __TEST, __XCHG, __XLATB, __XOR
};
typedef struct {
    Opcode type;
    size_t size;
    char* name;
} OpcodeString;
static OpcodeString OpcodeStringMap[] = {

    { POP, 3, "POP    " }, { POPA, 4, "POPA  " }, { POPF, 4, "POPF    " }, { PUSH, 4, "PUSH    " }, { PUSHA, 5, "PUSHA" }, { PUSHF, 5, "PUSHF" },
    { LAHF, 4, "LAHF  " }, { LDS, 3, "LDS    " }, { LEA, 3, "LEA      " }, { LES, 3, "LES      " }, { LODSB, 5, "LODSB" }, { LODSW, 5, "LODSW" },
    { LOOP, 4, "LOOP  " }, { LOOPE, 5, "LOOPE" }, { LOOPNE, 6, "LOOPNE" }, { LOOPNZ, 6, "LOOPNZ" }, { LOOPZ, 5, "LOOPZ" },
    { AAA, 3, "AAA    " }, { AAD, 3, "AAD    " }, { AAM, 3, "AAM      " }, { AAS, 3, "AAS      " }, { DAA, 3, "DAA    " }, { DAS, 3, "DAS    " },
    { CMP, 3, "CMP    " }, { CMPSB, 5, "CMPSB" }, { CMPSW, 5, "CMPSW  " }, { SCASB, 5, "SCASB  " }, { SCASW, 5, "SCASW" },
    { REP, 3, "REP    " }, { REPE, 4, "REPE  " }, { REPNE, 5, "REPNE  " }, { REPNZ, 5, "REPNZ  " }, { REPZ, 4, "REPZ  " },
    { CALL, 4, "CALL  " }, { RET, 3, "RET    " }, { RETF, 4, "RETF    " }, { IRET, 4, "IRET    " },
    { DIV, 3, "DIV    " }, { IDIV, 4, "IDIV  " }, { MUL, 3, "MUL      " }, { IMUL, 4, "IMUL    " },
    { CBW, 3, "CBW    " }, { CWD, 3, "CWD    " }, { CLD, 3, "CLD      " }, { CLI, 3, "CLI      " },
    { IN, 2, "IN      " }, { OUT, 3, "OUT    " }, { INT, 3, "INT      " }, { INTO, 4, "INTO    " },
    { ROL, 3, "ROL    " }, { ROR, 3, "ROR    " }, { RCL, 3, "RCL      " }, { RCR, 3, "RCR      " },
    { SAL, 3, "SAL    " }, { SAR, 3, "SAR    " }, { SHL, 3, "SHL      " }, { SHR, 3, "SHR      " },
    { SAHF, 4, "SAHF  " }, { STOSB, 5, "STOSB" }, { STOSW, 5, "STOSW  " },
    { TEST, 4, "TEST  " }, { XCHG, 4, "XCHG  " }, { XLATB, 5, "XLATB  " },
    { STC, 3, "STC    " }, { STD, 3, "STD    " }, { STI, 3, "STI      " },
    { SUB, 3, "SUB    " }, { SBB, 3, "SBB    " }, { INC, 3, "INC      " },
    { NOT, 3, "NOT    " }, { OR, 2, "OR      " }, { XOR, 3, "XOR      " },
    { MOVSB, 5, "MOVSB" }, { MOVSW, 5, "MOVSW" }
};

Error __AAA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AAD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AAM(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __AAS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CALL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CBW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CLD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CLI(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CMP(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CMPSB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CMPSW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __CWD(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DAA(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DAS(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __DIV(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IDIV(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IMUL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IN(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __INC(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __INT(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __INTO(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __IRET(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
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
Error __MOVSB(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __MOVSW(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __MUL(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
Error __NEG(CPU* cpu, Memory* mem, Word* operand1, Word* operand2);
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
