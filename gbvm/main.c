#include "gbvm.h"
#include "gbvm_assembler.h"
#include "gbvm_strings.h"
#include "gbvm_instructions.h"
#include "external_includes.h"

Vm vm = { 0 };

int main()
{
    String src = loadFileIntoString("prog/fibonacci.pblasm");

    printf("\n\n");
    printf("|--------------------------------------------------Comments detected while parsing-------------------------------------------------|\n");
    printf("|   |                                                                                                                              |\n");
    Program prog = parseAsmIntoProgram(&src);
    printf("|   |                                                                                                                              |\n");
    printf("|-------------------------------------------------------Compilation completed------------------------------------------------------|\n");
    printf("\n\n");

    vm.prog = prog;

    executeProgram(&vm);

    dumpStack(stdout, &vm);
    dumpFlags(stdout, &vm);
    return 0;
}