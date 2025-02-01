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

Error __AAA(CPU* cpu, Memory* mem, const Instruction* inst);
Error __AAD(CPU* cpu, Memory* mem, const Instruction* inst);
Error __AAM(CPU* cpu, Memory* mem, const Instruction* inst);
Error __AAS(CPU* cpu, Memory* mem, const Instruction* inst);
Error __ADC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __ADD(CPU* cpu, Memory* mem, const Instruction* inst);
Error __AND(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CALL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CBW(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CLC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CLD(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CLI(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CMC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CMP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CMPSB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CMPSW(CPU* cpu, Memory* mem, const Instruction* inst);
Error __CWD(CPU* cpu, Memory* mem, const Instruction* inst);
Error __DAA(CPU* cpu, Memory* mem, const Instruction* inst);
Error __DAS(CPU* cpu, Memory* mem, const Instruction* inst);
Error __DEC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __DIV(CPU* cpu, Memory* mem, const Instruction* inst);
Error __HLT(CPU* cpu, Memory* mem, const Instruction* inst);
Error __IDIV(CPU* cpu, Memory* mem, const Instruction* inst);
Error __IMUL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __IN(CPU* cpu, Memory* mem, const Instruction* inst);
Error __INC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __INT(CPU* cpu, Memory* mem, const Instruction* inst);
Error __INTO(CPU* cpu, Memory* mem, const Instruction* inst);
Error __IRET(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JA(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JAE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JBE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JCXZ(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JG(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JGE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JLE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JMP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNA(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNAE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNBE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNG(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNGE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNLE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNO(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNS(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JNZ(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JO(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JPE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JPO(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JS(CPU* cpu, Memory* mem, const Instruction* inst);
Error __JZ(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LAHF(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LDS(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LEA(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LES(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LODSB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LODSW(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LOOP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LOOPE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LOOPNE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LOOPNZ(CPU* cpu, Memory* mem, const Instruction* inst);
Error __LOOPZ(CPU* cpu, Memory* mem, const Instruction* inst);
Error __MOV(CPU* cpu, Memory* mem, const Instruction* inst);
Error __MOVSB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __MOVSW(CPU* cpu, Memory* mem, const Instruction* inst);
Error __MUL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __NEG(CPU* cpu, Memory* mem, const Instruction* inst);
Error __NOP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __NOT(CPU* cpu, Memory* mem, const Instruction* inst);
Error __OR(CPU* cpu, Memory* mem, const Instruction* inst);
Error __OUT(CPU* cpu, Memory* mem, const Instruction* inst);
Error __POP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __POPA(CPU* cpu, Memory* mem, const Instruction* inst);
Error __POPF(CPU* cpu, Memory* mem, const Instruction* inst);
Error __PUSH(CPU* cpu, Memory* mem, const Instruction* inst);
Error __PUSHA(CPU* cpu, Memory* mem, const Instruction* inst);
Error __PUSHF(CPU* cpu, Memory* mem, const Instruction* inst);
Error __RCL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __RCR(CPU* cpu, Memory* mem, const Instruction* inst);
Error __REP(CPU* cpu, Memory* mem, const Instruction* inst);
Error __REPE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __REPNE(CPU* cpu, Memory* mem, const Instruction* inst);
Error __REPNZ(CPU* cpu, Memory* mem, const Instruction* inst);
Error __REPZ(CPU* cpu, Memory* mem, const Instruction* inst);
Error __RET(CPU* cpu, Memory* mem, const Instruction* inst);
Error __RETF(CPU* cpu, Memory* mem, const Instruction* inst);
Error __ROL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __ROR(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SAHF(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SAL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SAR(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SBB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SCASB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SCASW(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SHL(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SHR(CPU* cpu, Memory* mem, const Instruction* inst);
Error __STC(CPU* cpu, Memory* mem, const Instruction* inst);
Error __STD(CPU* cpu, Memory* mem, const Instruction* inst);
Error __STI(CPU* cpu, Memory* mem, const Instruction* inst);
Error __STOSB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __STOSW(CPU* cpu, Memory* mem, const Instruction* inst);
Error __SUB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __TEST(CPU* cpu, Memory* mem, const Instruction* inst);
Error __XCHG(CPU* cpu, Memory* mem, const Instruction* inst);
Error __XLATB(CPU* cpu, Memory* mem, const Instruction* inst);
Error __XOR(CPU* cpu, Memory* mem, const Instruction* inst);
