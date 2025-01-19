#include "gbvm.h"
#include "gbvm_flags.h"

void dumpStack(FILE* stream, const Vm* vm)
{
    fprintf(stream, "-------------------------------STACK------------------------------------------ \n");
    Word SP = vm->cpu.registers.SP;
    if (SP <= 0) {
        fprintf(stream, " [Empty]\n");
        fprintf(stream, "-------------------------------STACK------------------------------------------ \n\n");
        return;
    }

    Word i = (SP < 5) ? 0 : SP - 5;

    while (i < SP) {
        fprintf(stream, "  %d\n", vm->mem.stack[i]);
        i++;
    }

    fprintf(stream, "-------------------------------STACK------------------------------------------ \n\n");
}
void dumpFlags(FILE* stream, const Vm* vm)
{
    fprintf(stream, "-------------------------------FLAGS------------------------------------------ \n");
    fprintf(stream, "  Halt : %d\t", getHalt(&(vm->cpu)));
    fprintf(stream, "  Sign : %d\n", getSign(&(vm->cpu)));
    fprintf(stream, "  Overflow : %d\t", getOverflow(&(vm->cpu)));
    fprintf(stream, "  Carry : %d\n", getCarry(&(vm->cpu)));
    fprintf(stream, "  Borrow : %d\t", getBorrow(&(vm->cpu)));
    fprintf(stream, "  Parity : %d\n", getParity(&(vm->cpu)));
    fprintf(stream, "  Zero : %d\n", getZero(&(vm->cpu)));
    fprintf(stream, "-------------------------------FLAGS------------------------------------------ \n\n");
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

    if (error != ERR_OK) {
        executionErrorWithExit(&error);
    }
    executeProgram(vm, debug, i + 1);
}