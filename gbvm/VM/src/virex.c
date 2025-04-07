#include "virex.h"
#include "sasm_memory.h"

inline Register* getReg(RegID id, Vm* vm)
{
    return &vm->cpu.registers.reg[id];
}

inline void setReg(RegID id, Vm* vm, DataEntry val)
{
    Register* tmp = getReg(id, vm);
    tmp->u64 = val;
}

inline uint64_t getInstCnt(Vm* vm)
{
    return vm->prog.instruction_count;
}

inline Instruction* getInstructionAt(Vm* vm, uint64_t index)
{
    return &vm->prog.instructions[index];
}

inline QuadWord* getStack(Vm* vm)
{
    return vm->mem.stack;
}

inline Byte* getMemory(Vm* vm)
{
    return vm->mem.memory;
}

void dumpStack(WINDOW* win, const Vm* vm)
{
    wprintw(win, "\n\n   ");
    for (uint64_t i = 0; i < 256; i++) {
        wprintw(win, "%02X ", vm->mem.memory[i]);
        if (i % 32 == 31) {
            wprintw(win, "\n   ");
        }
    }
}

void dumpFlags(WINDOW* win, CPU* cpu)
{
    wprintw(win, "\n");
    wattron(win, A_REVERSE);
    wprintw(win, "  FLAGS ");
    wattroff(win, A_REVERSE);
    wprintdash(win, 1);
    wprintw(win,
        "  HT : %c F1 : %c\t"
        "  F2 : %c F3 : %c\n"
        "  F4 : %c F5 : %c\t"
        "  F6 : %c F7 : %c\n",
        getFlag(HALT, cpu) ? 'T' : 'F',
        getFlag(F1, cpu) ? 'T' : 'F',
        getFlag(F2, cpu) ? 'T' : 'F',
        getFlag(F3, cpu) ? 'T' : 'F',
        getFlag(F4, cpu) ? 'T' : 'F',
        getFlag(F5, cpu) ? 'T' : 'F',
        getFlag(F6, cpu) ? 'T' : 'F',
        getFlag(F7, cpu) ? 'T' : 'F');
}

void dumpRegs(WINDOW* win, CPU* cpu)
{
    wmove(win, 2, 1);
    wattron(win, A_REVERSE);
    wprintw(win, "  REGISTERS ");
    wattroff(win, A_REVERSE);
    wprintdash(win, 1);
    wprintw(win,
        "  H0 : %ld\t"
        "  H1 : %ld\n"
        "  P0 : %ld\t"
        "  P1 : %ld\n"
        "  P2 : %ld\t"
        "  P3 : %ld\n"
        "  JS : %ld\t"
        "  KC : %ld\n"
        "  NX : %ld\t"
        "  SP : %ld\n"
        "  I0 : %ld\n"
        "  I1 : %ld\n"
        "  L0 : %ld\n"
        "  L1 : %lf\n"
        "  L2 : %ld\n"
        "  L3 : %ld\n"
        "  OP : %ld\n"
        "  QT : %ld\n"
        "  RF : %ld\n",
        cpu->registers.H0.u64,
        cpu->registers.H1.u64,
        cpu->registers.P0.u64,
        cpu->registers.P1.u64,
        cpu->registers.P2.u64,
        cpu->registers.P3.u64,
        cpu->registers.JS.u64,
        cpu->registers.KC.u64,
        cpu->registers.NX.u64,
        cpu->registers.SP.u64,
        cpu->registers.I0.u64,
        cpu->registers.I1.u64,
        cpu->registers.L0.u64,
        cpu->registers.L1.f64,
        cpu->registers.L2.i64,
        cpu->registers.L3.u64,
        cpu->registers.OP.u64,
        cpu->registers.QT.u64,
        cpu->registers.RF.u64);
}

void dumpDetails(WINDOW* win, OpcodeDetails* details, Instruction* inst)
{
    wprintw(win, "\n");
    wattron(win, A_REVERSE);
    wprintw(win, "  INSTRUCTION ");
    wattroff(win, A_REVERSE);
    wprintdash(win, 1);
    wprintw(win, "  %d\t  %s", details->type, details->name);

    if (details->has_operand) {
        wprintw(win,
            "\n────────╮"
            "\n    1.U │ %ld"
            "\n    1.I │ %ld"
            "\n    1.F │ %lf"
            "\n────────╯\n",
            inst->operand.u64,
            inst->operand.i64,
            inst->operand.f64);
    }

    if (details->has_operand2) {
        wprintw(win,
            "────────╮"
            "\n    2.U │ %ld"
            "\n    2.I │ %ld"
            "\n    2.F │ %lf"
            "\n────────╯\n",
            inst->operand2.u64,
            inst->operand2.i64,
            inst->operand2.f64);
    }
}

void executeProgram(Vm* vm, int debug, int lim)
{
    CPU* cpu = &(vm->cpu);
    size_t count = getReg(NX, vm)->u64;
    Instruction* inst = getInstructionAt(vm, getReg(NX, vm)->u64);
    OpcodeDetails details;

    Error error = 0;
    if (debug > 0) {
        WINDOW* prg = vm->disp.windows[PROGRAM];
        size_t i = (count - 2 > 0) ? count - 2 : 0;
        count = (count + getmaxy(prg) > getInstCnt(vm)) ? getInstCnt(vm) : count + getmaxy(prg);
        for (; i < count; i++) {
            details = getOpcodeDetails(getInstructionAt(vm, i)->type);
            if (i == getReg(NX, vm)->u64)
                wattron(prg, A_REVERSE);

            wprintw(prg, "\n   %ld\t│ %s ", i, details.name);
            if (details.has_operand)
                wprintw(prg, "\t %ld", getInstructionAt(vm, i)->operand.u64);
            if (details.has_operand2)
                wprintw(prg, "\t %ld", getInstructionAt(vm, i)->operand2.u64);
            wattroff(prg, A_REVERSE);
        }

        refreshWindow(vm->disp.windows[PROGRAM], getNameForWindow(PROGRAM), 3, 3, 3);
        refreshWindow(vm->disp.windows[OUTPUT], getNameForWindow(OUTPUT), 4, 5, 3);
        refreshWindow(vm->disp.windows[DETAILS], getNameForWindow(DETAILS), 1, 1, 1);
        refreshWindow(vm->disp.windows[MEMORY], getNameForWindow(MEMORY), 2, 2, 3);
        if (debug == 1) {
            wgetch(vm->disp.windows[INPUT]);
        }

        wclear(vm->disp.windows[DETAILS]);
        wclear(prg);
        wclear(vm->disp.windows[MEMORY]);

        details = getOpcodeDetails(inst->type);
        dumpStack(vm->disp.windows[MEMORY], vm);
        dumpRegs(vm->disp.windows[DETAILS], cpu);
        dumpFlags(vm->disp.windows[DETAILS], cpu);
        dumpDetails(vm->disp.windows[DETAILS], &details, inst);
    }

    if (lim == 0 || getFlag(HALT, cpu)) {
        return;
    }

    error = executeInst(vm, vm->disp.windows[OUTPUT]);

    if (error != ERR_OK)
        executionErrorWithExit(&error);

    executeProgram(vm, debug, lim - 1);
}

#define READ_OP(type, out)                                                 \
    {                                                                      \
        if (getReg(SP, vm)->u64 < 1) {                                     \
            return ERR_STACK_UNDERFLOW;                                    \
        }                                                                  \
        const MemoryAddr addr = getStack(vm)[getReg(SP, vm)->u64 - 1].u64; \
        if (addr >= MEMORY_CAPACITY) {                                     \
            return ERR_ILLEGAL_MEMORY_ACCESS;                              \
        }                                                                  \
        type tmp;                                                          \
        memcpy(&tmp, &getMemory(vm)[addr], sizeof(type));                  \
        getStack(vm)[getReg(SP, vm)->u64 - 1].out = tmp;                   \
        getReg(NX, vm)->u64++;                                             \
    }

#define ARITH_OP(reg, in, op)  \
    {                          \
        reg = reg op in;       \
        getReg(NX, vm)->u64++; \
    }

#define BINARY_OP(in, out, op)                                                                                                            \
    {                                                                                                                                     \
        if (getReg(SP, vm)->u64 < 2) {                                                                                                    \
            return ERR_STACK_UNDERFLOW;                                                                                                   \
        }                                                                                                                                 \
        getStack(vm)[getReg(SP, vm)->u64 - 2].out = getStack(vm)[getReg(SP, vm)->u64 - 2].in op getStack(vm)[getReg(SP, vm)->u64 - 1].in; \
        getReg(SP, vm)->u64 -= 1;                                                                                                         \
        getReg(NX, vm)->u64++;                                                                                                            \
    }

#define CAST_OP(r1, r2, src, dst, cast) \
    {                                   \
        r1->dst = cast r2->src;         \
        getReg(NX, vm)->u64++;          \
    }

Error executeInst(Vm* vm, WINDOW* win)
{
    if (getReg(NX, vm)->u64 >= getInstCnt(vm)) {
        printf("error %ld %ld", getReg(NX, vm)->u64, getInstCnt(vm));
        return ERR_ILLEGAL_INST_ACCESS;
    }
    Instruction inst = *getInstructionAt(vm, getReg(NX, vm)->u64);
    if (inst.opr1IsInline) {
        inst.operand.u64 = getReg(inst.operand.u64, vm)->u64;
    }
    if (inst.opr2IsInline) {
        inst.operand2.u64 = getReg(inst.operand2.u64, vm)->u64;
    }
    

    // printf("\nenter : %d %s", inst.type, OpcodeDetailsLUT[inst.type].name);
    switch (inst.type) {

    case INST_DONOP:
        getReg(NX, vm)->u64++;
        break;

    case INST_RETVL:

        break;

    case INST_INVOK:
        if (inst.operand.u64 > vm->vmCalls.internalVmCallsDefined)
            return ERR_ILLEGAL_OPERAND;

        if (!vm->vmCalls.VmCallI[inst.operand.u64])
            return ERR_NULL_CALL;

        const Error err = vm->vmCalls.VmCallI[inst.operand.u64](&vm->cpu, &vm->mem, win, &vm->region);
        if (err != ERR_OK)
            return err;

        getReg(NX, vm)->u64++;
        break;
    case INST_PUSHR:
        if (getReg(SP, vm)->u64 >= STACK_CAPACITY)
            return ERR_STACK_OVERFLOW;

        getStack(vm)[getReg(SP, vm)->u64++] = *getReg(inst.operand.u64, vm);
        getReg(NX, vm)->u64++;
        break;
    case INST_SPOPR:
        if (getReg(SP, vm)->u64 < 1)
            return ERR_STACK_UNDERFLOW;

        *getReg(inst.operand.u64, vm) = getStack(vm)[getReg(SP, vm)->u64 - 1];
        getReg(SP, vm)->u64 -= 1;
        getReg(NX, vm)->u64++;
        break;

    case INST_SHUTS:
        setFlag(HALT, &vm->cpu, 1);
        break;

    case INST_SETR:
        vm->cpu.registers.reg[inst.operand2.u64].u64 = inst.operand.u64;
        getReg(NX, vm)->u64++;
        break;

    case INST_GETR:
        getStack(vm)[getReg(SP, vm)->u64++] = *getReg(inst.operand.u64, vm);
        getReg(NX, vm)->u64++;
        break;

    case INST_CALL:
        if (getReg(SP, vm)->u64 >= STACK_CAPACITY)
            return ERR_STACK_OVERFLOW;

        getStack(vm)[getReg(SP, vm)->u64++].u64 = getReg(NX, vm)->u64 + 1;
        setReg(NX, vm, inst.operand.u64);
        break;

    case INST_LOOP:
        if (getReg(inst.operand2.u64, vm)->u64 > 0) {
            setReg(NX, vm, inst.operand.u64);
        } else {
            getReg(NX, vm)->u64++;
        }
        getReg(inst.operand2.u64, vm)->u64 -= 1;
        break;

    case INST_PUSH:
        if (getReg(SP, vm)->u64 >= STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }
        getStack(vm)[getReg(SP, vm)->u64++] = inst.operand;
        getReg(NX, vm)->u64++;
        break;

    case INST_SPOP:
        if (getReg(SP, vm)->u64 < 1) {
            return ERR_STACK_UNDERFLOW;
        }
        setReg(QT, vm, getStack(vm)[getReg(SP, vm)->u64 - 1].u64);
        getReg(SP, vm)->u64 -= 1;
        getReg(NX, vm)->u64++;
        break;

    case INST_SWAP:
        if (inst.operand.u64 >= getReg(SP, vm)->u64) {
            return ERR_STACK_UNDERFLOW;
        }

        const uint64_t a = getReg(SP, vm)->u64 - 1;
        const uint64_t b = getReg(SP, vm)->u64 - 1 - inst.operand.u64;

        QuadWord t = getStack(vm)[a];
        getStack(vm)[a] = getStack(vm)[b];
        getStack(vm)[b] = t;
        getReg(NX, vm)->u64++;
        break;

    case INST_ADDI:
        ARITH_OP(getReg(L2, vm)->i64, inst.operand.i64, +);
        break;

    case INST_SUBI:
        ARITH_OP(getReg(L2, vm)->i64, inst.operand.i64, -);
        break;

    case INST_MULI:
        ARITH_OP(getReg(L2, vm)->i64, inst.operand.i64, *);
        break;

    case INST_DIVI:
        if (inst.operand.i64 == 0)
            return ERR_DIV_BY_ZERO;
        ARITH_OP(getReg(L2, vm)->i64, inst.operand.i64, /);
        break;

    case INST_MODI:
        if (inst.operand.i64 == 0)
            return ERR_DIV_BY_ZERO;
        ARITH_OP(getReg(L2, vm)->i64, inst.operand.i64, %);
        break;

    case INST_ADDU:
        ARITH_OP(getReg(L3, vm)->u64, inst.operand.u64, +);
        break;

    case INST_SUBU:
        ARITH_OP(getReg(L3, vm)->u64, inst.operand.u64, -);
        break;

    case INST_MULU:
        ARITH_OP(getReg(L3, vm)->u64, inst.operand.u64, +);
        break;

    case INST_DIVU:
        if (inst.operand.u64 == 0)
            return ERR_DIV_BY_ZERO;
        ARITH_OP(getReg(L3, vm)->u64, inst.operand.u64, /);
        break;

    case INST_MODU:
        if (inst.operand.u64 == 0)
            return ERR_DIV_BY_ZERO;
        ARITH_OP(getReg(L3, vm)->u64, inst.operand.u64, %);
        break;

    case INST_ADDF:
        ARITH_OP(getReg(L1, vm)->f64, inst.operand.f64, +);
        break;

    case INST_SUBF:
        ARITH_OP(getReg(L1, vm)->f64, inst.operand.f64, -);
        break;

    case INST_MULF:
        ARITH_OP(getReg(L1, vm)->f64, inst.operand.f64, *);
        break;

    case INST_DIVF:
        if (inst.operand.f64 == 0.0)
            return ERR_DIV_BY_ZERO;
        ARITH_OP(getReg(L1, vm)->f64, inst.operand.f64, /);
        break;

    case INST_JMPU:
        setReg(NX, vm, inst.operand.u64);
        break;

    case INST_JMPC:
        if (getReg(SP, vm)->u64 < 1)
            return ERR_STACK_UNDERFLOW;

        if (getStack(vm)[getReg(SP, vm)->u64 - 1].u64)
            setReg(NX, vm, inst.operand.u64);
        else
            getReg(NX, vm)->u64++;

        getReg(SP, vm)->u64 -= 1;
        break;

    case INST_ANDB:
        BINARY_OP(u64, u64, &);
        break;

    case INST_NOTB:
        if (getReg(SP, vm)->u64 < 1)
            return ERR_STACK_UNDERFLOW;

        getStack(vm)[getReg(SP, vm)->u64 - 1].u64 = ~getStack(vm)[getReg(SP, vm)->u64 - 1].u64;
        getReg(NX, vm)->u64++;
        break;

    case INST_COPY:
        setReg(inst.operand.u64, vm, getReg(inst.operand2.u64, vm)->u64);
        getReg(NX, vm)->u64++;
        break;

    case INST_DUPS:
        if (getReg(SP, vm)->u64 >= STACK_CAPACITY) {
            return ERR_STACK_OVERFLOW;
        }

        if (getReg(SP, vm)->u64 - inst.operand.u64 <= 0) {
            return ERR_STACK_UNDERFLOW;
        }

        getStack(vm)[getReg(SP, vm)->u64] = getStack(vm)[getReg(SP, vm)->u64 - 1 - inst.operand.u64];
        getReg(SP, vm)->u64 += 1;
        getReg(NX, vm)->u64++;
        break;

    case INST_RET:
        if (getReg(SP, vm)->u64 < 1) {
            return ERR_STACK_UNDERFLOW;
        }

        setReg(NX, vm, getStack(vm)[getReg(SP, vm)->u64 - 1].u64);
        getReg(SP, vm)->u64--;
        break;

    case INST_NOT:
        if (getReg(SP, vm)->u64 < 1) {
            return ERR_STACK_UNDERFLOW;
        }

        getStack(vm)[getReg(SP, vm)->u64 - 1].u64 = !getStack(vm)[getReg(SP, vm)->u64 - 1].u64;
        getReg(NX, vm)->u64++;
        break;

    case INST_EQI:
        BINARY_OP(i64, u64, ==);
        break;

    case INST_GEI:
        BINARY_OP(i64, u64, >=);
        break;

    case INST_GTI:
        BINARY_OP(i64, u64, >);
        break;

    case INST_LEI:
        BINARY_OP(i64, u64, <=);
        break;

    case INST_LTI:
        BINARY_OP(i64, u64, <);
        break;

    case INST_NEI:
        BINARY_OP(i64, u64, !=);
        break;

    case INST_EQU:
        BINARY_OP(u64, u64, ==);
        break;

    case INST_GEU:
        BINARY_OP(u64, u64, >=);
        break;

    case INST_GTU:
        BINARY_OP(u64, u64, >);
        break;

    case INST_LEU:
        BINARY_OP(u64, u64, <=);
        break;

    case INST_LTU:
        BINARY_OP(u64, u64, <);
        break;

    case INST_NEU:
        BINARY_OP(u64, u64, !=);
        break;

    case INST_EQF:
        BINARY_OP(f64, u64, ==);
        break;

    case INST_GEF:
        BINARY_OP(f64, u64, >=);
        break;

    case INST_GTF:
        BINARY_OP(f64, u64, >);
        break;

    case INST_LEF:
        BINARY_OP(f64, u64, <=);
        break;

    case INST_LTF:
        BINARY_OP(f64, u64, <);
        break;

    case INST_NEF:
        BINARY_OP(f64, u64, !=);
        break;

    case INST_ORB:
        BINARY_OP(u64, u64, |);
        break;

    case INST_XOR:
        BINARY_OP(u64, u64, ^);
        break;

    case INST_SHR:
        BINARY_OP(u64, u64, >>);
        break;

    case INST_SHL:
        BINARY_OP(u64, u64, <<);
        break;

    case INST_I2F:
        CAST_OP(getReg(L1, vm), getReg(L2, vm), i64, f64, (double));
        break;

    case INST_U2F:
        CAST_OP(getReg(L1, vm), getReg(L3, vm), u64, f64, (double));
        break;

    case INST_F2I:
        CAST_OP(getReg(L2, vm), getReg(L1, vm), f64, i64, (int64_t));
        break;

    case INST_F2U:
        CAST_OP(getReg(L3, vm), getReg(L1, vm), f64, u64, (uint64_t)(int64_t));
        break;

    case INST_READ1U:
        READ_OP(uint8_t, u64);
        break;

    case INST_READ2U:
        READ_OP(uint16_t, u64);
        break;

    case INST_READ4U:
        READ_OP(uint32_t, u64);
        break;

    case INST_READ8U:
        READ_OP(uint64_t, u64);
        break;

    case INST_READ1I:
        READ_OP(int8_t, i64);
        break;

    case INST_READ2I:
        READ_OP(int16_t, i64);
        break;

    case INST_READ4I:
        READ_OP(int32_t, i64);
        break;

    case INST_READ8I:
        READ_OP(int64_t, i64);
        break;

    case INST_WRITE1:
        {
            if (getReg(SP, vm)->u64 < 2) {
                return ERR_STACK_UNDERFLOW;
            }
            const MemoryAddr addr = getStack(vm)[getReg(SP, vm)->u64 - 2].u64;
            if (addr >= MEMORY_CAPACITY) {
                return ERR_ILLEGAL_MEMORY_ACCESS;
            }
            getMemory(vm)[addr] = (MemoryAddr)getStack(vm)[getReg(SP, vm)->u64 - 1].u64;
            getReg(SP, vm)->u64 -= 2;
            getReg(NX, vm)->u64++;
        }
        break;

    case INST_WRITE2:
        {
            if (getReg(SP, vm)->u64 < 2) {
                return ERR_STACK_UNDERFLOW;
            }
            const MemoryAddr addr = getStack(vm)[getReg(SP, vm)->u64 - 2].u64;
            if (addr >= MEMORY_CAPACITY - 1) {
                return ERR_ILLEGAL_MEMORY_ACCESS;
            }
            Word value = (Word)getStack(vm)[getReg(SP, vm)->u64 - 1].u64;
            memcpy(&getMemory(vm)[addr], &value, sizeof(value));
            getReg(SP, vm)->u64 -= 2;
            getReg(NX, vm)->u64++;
        }
        break;

    case INST_WRITE4:
        {
            if (getReg(SP, vm)->u64 < 2) {
                return ERR_STACK_UNDERFLOW;
            }
            const MemoryAddr addr = getStack(vm)[getReg(SP, vm)->u64 - 2].u64;
            if (addr >= MEMORY_CAPACITY - 3) {
                return ERR_ILLEGAL_MEMORY_ACCESS;
            }
            DoubleWord value = (DoubleWord)getStack(vm)[getReg(SP, vm)->u64 - 1].u64;
            memcpy(&getMemory(vm)[addr], &value, sizeof(value));
            getReg(SP, vm)->u64 -= 2;
            getReg(NX, vm)->u64++;
        }
        break;

    case INST_WRITE8:
        {
            if (getReg(SP, vm)->u64 < 2) {
                return ERR_STACK_UNDERFLOW;
            }
            const MemoryAddr addr = getStack(vm)[getReg(SP, vm)->u64 - 2].u64;
            if (addr >= MEMORY_CAPACITY - 7) {
                return ERR_ILLEGAL_MEMORY_ACCESS;
            }
            QuadWord value = getStack(vm)[getReg(SP, vm)->u64 - 1];
            memcpy(&getMemory(vm)[addr], &value, sizeof(value));
            getReg(SP, vm)->u64 -= 2;
            getReg(NX, vm)->u64++;
        }
        break;

    case NUMBER_OF_INSTS:
    default:
        return ERR_ILLEGAL_INST;
    }

    return ERR_OK;
}