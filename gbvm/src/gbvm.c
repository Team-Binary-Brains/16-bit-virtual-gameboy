#include "gbvm.h"

void dumpStack(FILE* stream, const Vm* vm)
{
    fprintf(stream, "Stack : \n");
    if (vm->mem.stack_size > 0) {
        for (Word i = 0; i < vm->mem.stack_size; ++i) {
            fprintf(stream, "  %d\n", vm->mem.stack[i]);
        }
    } else {
        fprintf(stream, " [Empty]\n");
    }
}

Error executeProgram(Vm* vm)
{
    for (int i = 0; i < EXECUTION_LIMIT && !vm->cpu.halt; i++) {
        printString(opcodeAsStr(&(vm->prog.instructions[vm->cpu.registers.IP].type)));

        Error error = executeInst(&(vm->prog), &(vm->mem), &(vm->cpu));

        if (error != ERR_OK) {
            executionErrorWithExit(&error);
        }
    }
    return ERR_OK;
}