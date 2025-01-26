#include "sasm_assembler.h"
#include "univ_cmdutils.h"

char* inputFile = NULL;
char* outputFile = NULL;
int (*mode)(char*, char*) = &assemblyMode;

void processFlag(char* flag, int* argc, char*** argv);

int main(int argc, char* argv[])
{
    char* progName = getNextCmdLineArg(&argc, &argv);

    
    while (argc > 0) {
        char* arg = getNextCmdLineArg(&argc, &argv);
        processFlag(arg, &argc, &argv);
    }

    fprintf(stdout, "\nProgram Name : %s\n", progName);
    fprintf(stdout, "Input File : %s\n", inputFile);
    fprintf(stdout, "Output File : %s\n", outputFile);
    fprintf(stdout, "Mode : %s\n\n", mode == &assemblyMode ? "Assembly" : "Disassembly");

    if (!inputFile || !outputFile) {
        displayMsgWithExit("Missing Files\n"
                           "Usage : gbasm -i <input file> -o <output file> [-a | -d]\n "
                           "\t -a : Assemble mode\n "
                           "\t -d : Disassemble mode\n "
                           "\tDefault mode is assemble\n\n "
                           "\tAssembly Sample Command    : ./gbasm -i ../prog/fibonacci.pblasm  -o ../prog/fibonacci.pbl\n"
                           "\tDisassembly Sample Command : ./gbasm -o ../prog/fibonacci2.pblasm -i ../prog/fibonacci.pbl -d\n");
    }


    return mode(inputFile, outputFile);
}

void processFlag(char* flag, int* argc, char*** argv)
{
    Option opt = flagAsOption(flag);

    switch (opt) {
    case INPUT_FILE:
        inputFile = getNextCmdLineArg(argc, argv);
        return;
    case OUTPUT_FILE:
        outputFile = getNextCmdLineArg(argc, argv);
        return;
    case ASSEMBLE_MODE:
        mode = &assemblyMode;
        return;
    case DISASSEMBLE_MODE:
        mode = &disassemblyMode;
        return;
    default:
        displayMsgWithExit("Unknown option provided.");
    }
}