#include "gbvm.h"
#include "gbvm_assembler.h"
#include "gbvm_strings.h"
#include "gbvm_instructions.h"
#include "external_includes.h"

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