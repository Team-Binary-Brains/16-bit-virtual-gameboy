#include "gbvm.h"

Vm vm = { 0 };

char* srcCode = "PSH 0\n"
                "PSH 1\n"
                "DUP 2\n"
                "DUP 2\n"
                "ADD\n"
                "JMP 2\n"
                "HLT\n";

Instruction ins[] = {
    MAKE_INST_ARG(PSH, 0),
    MAKE_INST_ARG(PSH, 1),
    MAKE_INST_ARG(DUP, 2),
    MAKE_INST_ARG(DUP, 2),
    MAKE_INST(ADD),
    MAKE_INST_ARG(JMP, 2),
    MAKE_INST(HLT),
};
int main()
{
    Program prog;
    memcpy(prog.instructions, ins, sizeof(ins));
    prog.instruction_count = sizeof(ins) / sizeof(ins[0]);
    prog.instruction_size = sizeof(ins[0]);

    assembleInstructions(prog, "./prog/fibonacci.pbl");
    // loadProgram(&vm, prog);
    loadBytecode(&vm, "./prog/fibonacci.pbl");

    // loadProgram(&vm, translate_asm(srcCode, strlen(srcCode)));

    dumpStack(stdout, &vm);
    for (int i = 0; i < EXECUTION_LIMIT && !vm.cpu.halt; i++) {
        printf("%s\n", opcodeAsCstr(prog.instructions[vm.cpu.registers.IP].type));
        Error error = executeInst(&(vm.prog), &(vm.mem), &(vm.cpu));

        if (error != ERR_OK) {
            fprintf(stderr, "Error : %s\n", errorAsCstr(error));
            exit(1);
        }
    }
    dumpStack(stdout, &vm);
    return 0;
}