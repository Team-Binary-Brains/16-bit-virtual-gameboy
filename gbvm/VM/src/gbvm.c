#include "gbvm.h"
#include "sasm_assembler.h"
#include "sasm_instructions.h"
#include "sasm_flags.h"

void dumpStack(FILE* stream, const Vm* vm)
{
    fprintf(stream, "-------------------------------STACK------------------------------------------");
    Word SP = vm->cpu.registers.SP;

    Word start = (SP < 5) ? 0 : SP - 5;
    Word len = (SP < 5) ? SP : start + 5;

    Word i;
    for (i = start; i < len; i++)
        fprintf(stream, "\n\t%d", vm->mem.stack[i]);

    for (i = len; i < start + 5; i++)
        fprintf(stream, "\n\t[X]  ");

    fprintf(stream, "\n-------------------------------STACK------------------------------------------ \n\n");
}

void dumpFlags(FILE* stream, CPU* cpu)
{
    fprintf(stream, "\n-------------------------------FLAGS------------------------------------------ "
                    "\n  Halt : %d\tOverflow : %d                                                     "
                    "\n  Sign : %d\tCarry    : %d                                                     "
                    "\n  Zero : %d\tParity   : %d                                                     "
                    "\n  Borrow : %d                                                                  "
                    "\n-------------------------------FLAGS------------------------------------------ \n\n",
        getFlag(HALT, cpu), getFlag(OVERFLOW, cpu),
        getFlag(SIGN, cpu), getFlag(CARRY, cpu),
        getFlag(ZERO, cpu), getFlag(PARITY, cpu),
        getFlag(BORROW, cpu));
}

void loadProgram(Vm* vm, char* inputFile)
{
    vm->prog = disassembleBytecodeIntoProgram(inputFile);
    vm->cpu.registers.IP = 0;
    vm->cpu.registers.SP = 0;
    setFlag(HALT, &(vm->cpu), 0);
}

void executeProgram(Vm* vm, int debug, int i)
{
    CPU* cpu = &(vm->cpu);
    Memory* mem = &(vm->mem);
    Program* prog = &(vm->prog);
    Instruction* inst = &(prog->instructions[vm->cpu.registers.IP]);
    if (i >= EXECUTION_LIMIT || getFlag(HALT, cpu)) {
        return;
    }
    String operation = opcodeAsStr(&(inst->type));
    Error error = 0;
    switch (debug) {
    case 2:
        scanf("%*c");
        system("clear");
        error = executeInst(prog, mem, cpu);
        printf("\nInstruction Number :\t%d\nInstruction :\t\t", i);
        printString(operation);
        printf("Operand1 : \t\t%d\nOperand2 : \t\t%d\n", inst->operand, inst->operand2);
        dumpFlags(stdout, cpu);
        dumpStack(stdout, vm);
        break;
    case 1:
        scanf("%*c");
        system("clear");
        error = executeInst(prog, mem, cpu);
        printf("\nInstruction Number :\t%d\nInstruction :\t\t", i);
        printString(operation);
        printf("Operand1 : \t\t%d\nOperand2 : \t\t%d\n", inst->operand, inst->operand2);
        dumpStack(stdout, vm);
        break;
    default:
        error = executeInst(prog, mem, cpu);
        break;
    }

    if (error != ERR_OK)
        executionErrorWithExit(&error);

    executeProgram(vm, debug, i + 1);
}