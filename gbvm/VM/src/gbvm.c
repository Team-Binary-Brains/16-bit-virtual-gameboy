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

void dumpFlags(FILE* stream, const Vm* vm)
{
    fprintf(stream, "\n-------------------------------FLAGS------------------------------------------ "
                    "\n  Halt : %d\tOverflow : %d                                                     "
                    "\n  Sign : %d\tCarry    : %d                                                     "
                    "\n  Zero : %d\tParity   : %d                                                     "
                    "\n  Borrow : %d                                                                  "
                    "\n-------------------------------FLAGS------------------------------------------ \n\n",
        getHalt(&(vm->cpu)), getOverflow(&(vm->cpu)),
        getSign(&(vm->cpu)), getCarry(&(vm->cpu)),
        getZero(&(vm->cpu)), getParity(&(vm->cpu)),
        getBorrow(&(vm->cpu)));
}

void loadProgram(Vm* vm, char* inputFile)
{
    vm->prog = disassembleBytecodeIntoProgram(inputFile);
    vm->cpu.registers.IP = 0;
    vm->cpu.registers.SP = 0;
    setHalt(&(vm->cpu), 0);
}

void executeProgram(Vm* vm, int debug, int i)
{
    if (i >= EXECUTION_LIMIT || getHalt(&(vm->cpu))) {
        return;
    }
    String operation = opcodeAsStr(&(vm->prog.instructions[vm->cpu.registers.IP].type));

    switch (debug) {
    case 2:
        scanf("%*c");
        system("clear");
        dumpFlags(stdout, vm);
        dumpStack(stdout, vm);
        printString(operation);
        break;
    case 1:
        scanf("%*c");
        system("clear");
        dumpStack(stdout, vm);
        printString(operation);
        break;
    default:
        break;
    }

    Error error = executeInst(&(vm->prog), &(vm->mem), &(vm->cpu));

    if (error != ERR_OK)
        executionErrorWithExit(&error);

    executeProgram(vm, debug, i + 1);
}