#include "gbvm.h"
#include "gbvm_flags.h"

void dumpStack(FILE* stream, const Vm* vm)
{
    fprintf(stream, "-------------------------------STACK------------------------------------------ \n");
    if (vm->cpu.registers.SP > 0) {
        for (Word i = 0; i < vm->cpu.registers.SP; ++i) {
            fprintf(stream, "  %d\n", vm->mem.stack[i]);
        }
    } else {
        fprintf(stream, " [Empty]\n");
    }
    fprintf(stream, "-------------------------------STACK------------------------------------------ \n\n");
}
void dumpFlags(FILE* stream, const Vm* vm)
{
    fprintf(stream, "-------------------------------FLAGS------------------------------------------ \n");
    fprintf(stream, "  Halt : %d\n", getHalt(&(vm->cpu)));
    fprintf(stream, "  Sign : %d\n", getSign(&(vm->cpu)));
    fprintf(stream, "  Overflow : %d\n", getOverflow(&(vm->cpu)));
    fprintf(stream, "  Carry : %d\n", getCarry(&(vm->cpu)));
    fprintf(stream, "  Borrow : %d\n", getBorrow(&(vm->cpu)));
    fprintf(stream, "  Parity : %d\n", getParity(&(vm->cpu)));
    fprintf(stream, "  Zero : %d\n", getZero(&(vm->cpu)));
    fprintf(stream, "-------------------------------FLAGS------------------------------------------ \n\n");
}
Error executeProgram(Vm* vm)
{
    for (int i = 0; i < EXECUTION_LIMIT && !getHalt(&(vm->cpu)); i++) {
        // printString(opcodeAsStr(&(vm->prog.instructions[vm->cpu.registers.IP].type)));

        Error error = executeInst(&(vm->prog), &(vm->mem), &(vm->cpu));

        if (error != ERR_OK) {
            executionErrorWithExit(&error);
        }
    }
    return ERR_OK;
}