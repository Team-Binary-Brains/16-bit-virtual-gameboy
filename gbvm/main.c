#include "gbvm.h"

Vm vm = { 0 };

int main()
{

    Program prog = translate_asm(loadFileIntoProgramString("prog/fibonacci.pblasm"));
    // loadProgram(&vm, prog);

    for (int i = 0; i < EXECUTION_LIMIT && !vm.cpu.halt; i++) {
        // printString(opcodeAsStr(prog.instructions[vm.cpu.registers.IP].type));

        Error error = executeInst(&prog, &(vm.mem), &(vm.cpu));

        if (error != ERR_OK) {
            executionErrorWithExit(error);
        }
    }

    dumpStack(stdout, &vm);
    return 0;
}