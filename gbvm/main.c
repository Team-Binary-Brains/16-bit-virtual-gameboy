#include "gbvm.h"

Vm vm = { 0 };

int main()
{
    String src = loadFileIntoString("prog/fibonacci.pblasm");
    Program prog = parseAsmIntoProgram(&src);
    vm.prog = prog;

    executeProgram(&vm);

    dumpStack(stdout, &vm);
    return 0;
}