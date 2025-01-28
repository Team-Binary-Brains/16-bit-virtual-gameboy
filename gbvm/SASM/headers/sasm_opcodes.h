/**
 * @file sasm_opcodes.h
 * @brief This file contains the instruction set used in the SASM assembler.
 *
 * @author Soham Metha
 * @date January 2025
 */

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
    MOV,
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
} Opcode;

void __AAA();
void __AAD();
void __AAM();
void __AAS();
void __ADC();
void __ADD();
void __AND();
void __CALL();
void __CBW();
void __CLC();
void __CLD();
void __CLI();
void __CMC();
void __CMP();
void __CMPSB();
void __CMPSW();
void __CWD();
void __DAA();
void __DAS();
void __DEC();
void __DIV();
void __HLT();
void __IDIV();
void __IMUL();
void __IN();
void __INC();
void __INT();
void __INTO();
void __IRET();
void __JA();
void __JAE();
void __JB();
void __JBE();
void __JC();
void __JCXZ();
void __JE();
void __JG();
void __JGE();
void __JL();
void __JLE();
void __JMP();
void __JNA();
void __JNAE();
void __JNB();
void __JNBE();
void __JNC();
void __JNE();
void __JNG();
void __JNGE();
void __JNL();
void __JNLE();
void __JNO();
void __JNP();
void __JNS();
void __JNZ();
void __JO();
void __JP();
void __JPE();
void __JPO();
void __JS();
void __JZ();
void __LAHF();
void __LDS();
void __LEA();
void __LES();
void __LODSB();
void __LODSW();
void __LOOP();
void __LOOPE();
void __LOOPNE();
void __LOOPNZ();
void __LOOPZ();
void __MOV();
void __MOVSB();
void __MOVSW();
void __MUL();
void __NEG();
void __NOP();
void __NOT();
void __OR();
void __OUT();
void __POP();
void __POPA();
void __POPF();
void __PUSH();
void __PUSHA();
void __PUSHF();
void __RCL();
void __RCR();
void __REP();
void __REPE();
void __REPNE();
void __REPNZ();
void __REPZ();
void __RET();
void __RETF();
void __ROL();
void __ROR();
void __SAHF();
void __SAL();
void __SAR();
void __SBB();
void __SCASB();
void __SCASW();
void __SHL();
void __SHR();
void __STC();
void __STD();
void __STI();
void __STOSB();
void __STOSW();
void __SUB();
void __TEST();
void __XCHG();
void __XLATB();
void __XOR();