#include "gbvm.h"
#include "univ_strings.h"
#include "univ_defs.h"

Vm vm = { 0 };

char cmd[256];
char inputFile[100];
char outputFile[100];

void getInput()
{
    printf("Enter input file : ");
    scanf("%99s", inputFile);
}
void getOutput()
{
    printf("Enter output file : ");
    scanf("%99s", outputFile);
}

void getAssemblerCmd()
{
    char mode;
    printf("Select mode : \n");
    printf("a Assembly    \n");
    printf("d Disassembly \n");
    printf("Enter choice : ");

    while (getchar() != '\n')
        ;

    scanf("%c", &mode);

    snprintf(cmd, sizeof(cmd), "./sasm -i %s -o %s -%c", inputFile, outputFile, mode);
}

void getCompilerCmd()
{
    snprintf(cmd, sizeof(cmd), "./occ -i %s -o %s", inputFile, outputFile);
}

void runner(int ch)
{
    getOutput();

    if (ch == 0) {
        getAssemblerCmd();
    } else {
        getCompilerCmd();
    }

    if (system(cmd) != 0) {
        displayMsgWithExit("Task Failed");
    }

    printf("Task Completed\n");
}

void menu(int ch)
{
    if (ch == 0) {
        return;
    }

    getInput();

    if (ch < 3) {
        runner(ch - 1);
        return;
    }

    loadProgram(&vm, inputFile);
    vm.cpu.registers.IP = 0;
    executeProgram(&vm, ch - 3, 0);
    printf("Program completed\n");
}

int main()
{
    int ch = 0;
    do {
        printf("\n0. Exit"
               "\n1. Run Assembler"
               "\n2. Run Compiler"
               "\n3. Run ASSEMBLY Program"
               "\n4. Debug ASSEMBLY Program - with stack"
               "\n5. Debug ASSEMBLY Program - with stack and flags"
               "\nEnter choice : ");
        scanf("%d", &ch);
        menu(ch);

    } while (ch != 0);
}