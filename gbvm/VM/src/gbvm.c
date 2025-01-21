#include "gbvm.h"
#include "sasm_flags.h"

void dumpStack(FILE* stream, const Vm* vm)
{
    fprintf(stream, "-------------------------------STACK------------------------------------------");
    Word SP = vm->cpu.registers.SP;

    Word i;
    Word len = (SP < 5) ? SP : 5;
    for (i = 0; i < len; i++)
        fprintf(stream, "\n  %d  ", vm->mem.stack[i]);

    for (; i < 5; i++)
        fprintf(stream, "\n  [X]  ");

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

void executeProgram(Vm* vm, int debug, int i)
{
    if (i >= EXECUTION_LIMIT || getHalt(&(vm->cpu))) {
        return;
    }
    String operation = opcodeAsStr(&(vm->prog.instructions[vm->cpu.registers.IP].type));
    Error error = executeInst(&(vm->prog), &(vm->mem), &(vm->cpu));

    switch (debug) {
    case 3:
        dumpFlags(stdout, vm);
    case 2:
        dumpStack(stdout, vm);
    case 1:
        printString(operation);
        scanf("%*c");
        system("clear");
    default:
        break;
    }

    if (error != ERR_OK)
        executionErrorWithExit(&error);

    executeProgram(vm, debug, i + 1);
}