#include "gbvm.h"
#include "sasm_assembler.h"
#include "sasm_instructions.h"
#include "sasm_flags.h"
#include "univ_strings.h"
#include "univ_defs.h"

Vm vm = { 0 };

void runAssembler()
{
    char mode[10];
    char cmd[256];
    char inputFile[100];
    char outputFile[100];

    printf("Enter input file :  ");
    scanf("%99s", inputFile);

    printf("Enter output file : ");
    scanf("%99s", outputFile);

    printf("Select mode : \n");
    printf("a Assembly    \n");
    printf("d Disassembly \n");
    printf("Enter choice : ");
    scanf("%9s", mode);

    snprintf(cmd, sizeof(cmd), "./gbasm -i %s -o %s -%s", inputFile, outputFile, mode);

    if (system(cmd) != 0) {
        displayMsgWithExit("Assembler failed");
    }

    printf("Task Completed\n");
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
    system("make gbasm;make occ");
    printf("\n\nASSEMBLER AND OCC LOADED\n\n");

    int ch = 0;
    do {

        printf("\n0. Exit"
               "\n1. Run Assembler"
               "\n2. Run ASSEMBLY Program"
               "\n3. Debug ASSEMBLY Program - with stack"
               "\n4. Debug ASSEMBLY Program - with stack and flags"
               "\nEnter choice : ");
        scanf("%d", &ch);
        menu(ch);

    } while (ch != 0);

    return system("make clean;");
}