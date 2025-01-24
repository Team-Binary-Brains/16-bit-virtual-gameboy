#include "O_codegenerator.h"
#include "O_lexer.h"
#include "O_parser.h"
#include "univ_cmdutils.h"
#include "univ_defs.h"
#include "univ_errors.h"

char* inputFile = NULL;
char* outputFile = NULL;

void processFlag(char* flag, int* argc, char*** argv);

int main(int argc, char* argv[])
{
    char* progName = getNextCmdLineArg(&argc, &argv);

    while (argc > 0) {
        char* arg = getNextCmdLineArg(&argc, &argv);
        processFlag(arg, &argc, &argv);
    }

    fprintf(stdout, "\nProgram Name : %s", progName);
    fprintf(stdout, "\nInput File : %s", inputFile);
    fprintf(stdout, "\nOutput File : %s\n", outputFile);

    if (!inputFile || !outputFile) {
        displayMsgWithExit("\nMissing Files"
                           "\nUsage : gbasm -i <input file> -o <output file> -l [lang] "
                           "\tSample Command    : ./gbasm -i ./prog/FirstProgram.osh  -o ./prog/example.pblasm\n");
    }

    FILE* file;
    file = fopen(inputFile, "r");

    if (!file) {
        printf("FILE_ERROR: %s file not found\n", inputFile);
        exit(1);
    }

    Token* tokens = lexer(file, inputFile);
    // Node* root = parser(tokens);

    // generateCode(root, outputFile);

    // FILE *assembly_file = fopen("gbvmasm.asm", "r");
    // if(!assembly_file){
    //     printf("ERROR NOT KNOWN: Kya coder banega re tu\n");
    //     exit(1);
    // }

    printf("Compiler Ready!!!\n");

    /*
    TODO: inputting assembly file to assembler
    */
    return 0;
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
    case ASM_LANG:
    case ASSEMBLE_MODE:
    case DISASSEMBLE_MODE:
    default:
        displayMsgWithExit("Unknown option provided.");
    }
}