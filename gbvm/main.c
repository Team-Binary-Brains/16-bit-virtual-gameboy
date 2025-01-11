#include "gbvm.h"

Vm vm = { 0 };

char* srcCode = "PSH 0\n"
                "PSH 1\n"
                "DUP 2\n"
                "DUP 2\n"
                "ADD\n"
                "JMP 2\n"
                "HLT\n";

int main()
{
    // vm_save_program_to_file(program, sizeof(program), "./fibonacci.pbl");
    // vm_load_program_from_memory(&vm, program, ARRAY_SIZE(program));
    vm_load_program_from_file(&vm, "./prog/fibonacci.pbl");

    // vm.program_size = vm_translate_asm(srcCode, strlen(srcCode), vm.program, VM_PROGRAM_CAPACITY);

    vm_dump_stack(stdout, &vm);
    for (int i = 0; i < VM_EXECUTION_LIMIT && !vm.halt; i++) {
        // printf("%s\n", inst_type_as_cstr(vm.program[vm.IP].type));
        Error error = vm_execute_inst(&vm);

        if (error != ERR_OK) {
            fprintf(stderr, "Error : %s\n", error_as_cstr(error));
            exit(1);
        }
    }
    vm_dump_stack(stdout, &vm);
    return 0;
}