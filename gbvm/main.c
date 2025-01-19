#include "gbvm.h"
#include "gbvm_assembler.h"
#include "gbvm_strings.h"
#include "gbvm_instructions.h"
#include "global_definitions.h"

Vm vm = { 0 };

void runAssembler()
{
    char mode[10];
    char cmd[256];
    char inputFile[100];
    char outputFile[100];

    printf("Enter input file : ");
    scanf("%99s", inputFile);

    printf("Enter output file : ");
    scanf("%99s", outputFile);

    printf("Select mode : \n");
    printf("a Assembly\n");
    printf("d Disassembly\n");
    printf("Enter choice : ");
    scanf("%9s", mode);

    snprintf(cmd, sizeof(cmd), "./gbasm -i %s -o %s -%s", inputFile, outputFile, mode);
    if (system(cmd) != 0) {
        displayMsgWithExit("Assembler failed");
    } else {
        printf("Assembler completed\n");
    }
}

void menu(int ch)
{
    if (ch == 0) {
        return;
    }
    if (ch == 1) {
        runAssembler();
        return;
    }
    char inputFile[100];

    printf("Enter input file : ");
    scanf("%99s", inputFile);

    vm.prog = disassembleBytecodeIntoProgram(inputFile);
    vm.cpu.registers.IP = 0;
    vm.cpu.registers.SP = 0;
    setHalt(&(vm.cpu), 0);

    executeProgram(&vm, ch - 2, 0);
    printf("Program completed\n");
}

int main()
{
    printf("\n\nVM STARTING\n\n");
    system("make gbasm;");
    printf("\n\nASSEMBLER RELOADED\n\n");

    int ch = 0;
    do {
        printf("0. Exit\n");
        printf("1. Run Assembler\n");
        printf("2. Run Program\n");
        printf("3. Debug Program\n");
        printf("Enter choice : ");
        scanf("%d", &ch);
        menu(ch);
    } while (ch != 0);

    system("make clean;");
    return 0;
}